// _vmstwinvqdecTwinVQ.cpp: implementation of the CTwinVQ class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "_vmstwinvqdecTwinVQ.h"
#include "tvqdec.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

namespace _vmstwinvqdec
{

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CTwinVQ::CTwinVQ()
{

}

CTwinVQ::~CTwinVQ()
{

}


/*----------------------------------------------------------------------------*/
/* Name:        get_string()                                                  */
/* Description: get string from the bitstream file                            */
/* Return:      none                                                          */
/* Access:      static                                                        */
/*----------------------------------------------------------------------------*/
static
int get_string (char *buf, int nbytes, BFILE *bfp)
{
	int c, ichar, ibit;
	
	for ( ichar=0; ichar<nbytes; ichar++ ) {
		ibit = get_bstm( &c, CHAR_BITS, bfp );
		if ( ibit < CHAR_BITS) {
			break;
		}
		buf[ichar] = c;
	}

	buf[ichar] = '\0';
	return ichar;
}


/*============================================================================*/
/* Name:        TvqGetHeaderInfo()                                            */
/* Description: read bitstream header and create the header chunk data        */
/* Return:      returns 1 when error or 0                                     */
/* Access:      external                                                      */
/*============================================================================*/
CChunkChunk* CTwinVQ::TvqGetBsHeaderInfo(BFILE *bfp)             // Input:  bitstream file pointer
{

	// TwinVQ �w�b�_���t�@�C������ǂݍ��݁A�w�b�_�f�[�^�\���̂𓾂�
	CChunkChunk* twinChunk = LoadTwinChunk( bfp );
	if ( twinChunk == NULL ) {
		fprintf( stderr, "Failed to read header. Check the bitstream file.\n" );
		return NULL;
	}

	// "DATA" �`�����N�w�b�_����ǂ�
	char lbuf[BUFSIZ];
	get_string(lbuf, KEYWORD_BYTES, bfp);
	if ( strcmp ( lbuf, "DATA" ) ) {
		fprintf ( stderr, "TwinVQ format error. No \"DATA\" chunk was found.\n" );
		return NULL;
	}

	return twinChunk;
}

/*============================================================================*/
/* Name:        TvqInitBsReader()                                             */
/* Description: initialize the bitstream reader                               */
/* Return:      Returns 1 when error or returns 0                             */
/* Access:      external                                                      */
/*============================================================================*/
int CTwinVQ::TvqInitBsReader( headerInfo *setupInfo )
{

	TVQ_VERSION = TvqCheckVersion( setupInfo->ID );
	if ( TVQ_VERSION == TVQ_UNKNOWN_VERSION ) {
		return 1;
	}
	
	TvqGetConfInfo(&cf);

	TvqGetVectorInfo( bits_0, bits_1 );
	
	iframe = 0;

	return 0;
}

/*----------------------------------------------------------------------------*/
/* Name:        GetVqInfo()                                                   */
/* Description: get VQ information                                            */
/* Return:      (int) number of bits read                                     */
/* Access:      static                                                        */
/*----------------------------------------------------------------------------*/

int CTwinVQ::GetVqInfo( tvqConfInfoSubBlock *cfg,
			  int bits0[],
			  int bits1[],
			  int variableBits,
			  INDEX *index,
			  BFILE *bfp)
{
	int idiv;
	int bitcount = 0;

	if ( index->btype == BLK_LONG ){
		TvqUpdateVectorInfo( variableBits, &cfg->ndiv, bits0, bits1 ); // re-calculate VQ bits
	}
	for ( idiv=0; idiv<cfg->ndiv; idiv++ ){
		bitcount += get_bstm( &index->wvq[idiv], bits0[idiv], bfp );       /* CB 0 */
		bitcount += get_bstm( &index->wvq[idiv + cfg->ndiv], bits1[idiv], bfp ); /* CB 1 */
	}

	return bitcount;
}

/*----------------------------------------------------------------------------*/
/* Name:        GetBseInfo()                                                  */
/* Description: get BSE information                                           */
/* Return:      (int) number of bits read                                     */
/* Access:      static                                                        */
/*----------------------------------------------------------------------------*/
int CTwinVQ::GetBseInfo( tvqConfInfo *cf, tvqConfInfoSubBlock *cfg, INDEX *index, BFILE *bfp )
{
	int i_sup, isf, itmp, idiv;
	int bitcount = 0;

	for ( i_sup=0; i_sup<cf->N_CH; i_sup++ ){
		for ( isf=0; isf<cfg->nsf; isf++ ){
			for ( idiv=0; idiv<cfg->fw_ndiv; idiv++ ){
				itmp = idiv + ( isf + i_sup * cfg->nsf ) * cfg->fw_ndiv;
				bitcount += get_bstm( &index->fw[itmp], cfg->fw_nbit, bfp );
			}
		}
	}
	for ( i_sup=0; i_sup<cf->N_CH; i_sup++ ){
		for ( isf=0; isf<cfg->nsf; isf++ ){
			bitcount += get_bstm( &index->fw_alf[i_sup * cfg->nsf + isf], cf->FW_ARSW_BITS, bfp);
		}
	}

	return bitcount;
}

/*----------------------------------------------------------------------------*/
/* Name:        GetGainInfo()                                                 */
/* Description: get gain information                                          */
/* Return:      (int) number of bits read                                     */
/* Access:      static                                                        */
/*----------------------------------------------------------------------------*/
int CTwinVQ::GetGainInfo( tvqConfInfo *cf, tvqConfInfoSubBlock *cfg, INDEX *index, BFILE *bfp )
{
	int i_sup, iptop, isf;
	int bitcount = 0;

	for ( i_sup=0; i_sup<cf->N_CH; i_sup++ ){
		iptop = ( cfg->nsubg + 1 ) * i_sup;
		bitcount += get_bstm( &index->pow[iptop], cf->GAIN_BITS, bfp );
		for ( isf=0; isf<cfg->nsubg; isf++ ){
			bitcount += get_bstm( &index->pow[iptop+isf+1], cf->SUB_GAIN_BITS, bfp );
		}
	}

	return bitcount;
}

/*----------------------------------------------------------------------------*/
/* Name:        GetLspInfo()                                                  */
/* Description: get LSP information                                           */
/* Return:      (int) number of bits read                                     */
/* Access:      static                                                        */
/*----------------------------------------------------------------------------*/
int CTwinVQ::GetLspInfo( tvqConfInfo *cf, INDEX *index, BFILE *bfp )
{
	int i_sup, itmp;
	int bitcount = 0;

	for ( i_sup=0; i_sup<cf->N_CH; i_sup++ ){
		bitcount += get_bstm( &index->lsp[i_sup][0], cf->LSP_BIT0, bfp ); /* pred. switch */
		bitcount += get_bstm( &index->lsp[i_sup][1], cf->LSP_BIT1, bfp ); /* first stage */
		for ( itmp=0; itmp<cf->LSP_SPLIT; itmp++ ){         /* second stage */
			bitcount += get_bstm( &index->lsp[i_sup][itmp+2], cf->LSP_BIT2, bfp );
		}
	}

	return bitcount;
}

/*----------------------------------------------------------------------------*/
/* Name:        GetPpcInfo()                                                  */
/* Description: get PPC information                                           */
/* Return:      (int) number of bits read                                     */
/* Access:      static                                                        */
/*----------------------------------------------------------------------------*/
int CTwinVQ::GetPpcInfo( tvqConfInfo *cf, INDEX *index, BFILE *bfp )
{
	int idiv, i_sup;
	int bitcount = 0;
	
	for ( idiv=0; idiv<cf->N_DIV_P; idiv++ ){
		bitcount += get_bstm( &(index->pls[idiv]), bits_0[BLK_PPC][idiv], bfp );       /*CB0*/
		bitcount += get_bstm( &(index->pls[idiv+cf->N_DIV_P]), bits_1[BLK_PPC][idiv], bfp);/*CB1*/
	}
	for (i_sup=0; i_sup<cf->N_CH; i_sup++){
		bitcount += get_bstm(&(index->pit[i_sup]), cf->BASF_BIT, bfp);
		bitcount += get_bstm(&(index->pgain[i_sup]), cf->PGAIN_BIT, bfp);
	}
	
	return bitcount;
}

/*----------------------------------------------------------------------------*/
/* Name:        GetEbcInfo()                                                  */
/* Description: get EBC information                                           */
/* Return:      (int) number of bits read                                     */
/* Access:      static                                                        */
/*----------------------------------------------------------------------------*/
int CTwinVQ::GetEbcInfo( tvqConfInfo *cf, tvqConfInfoSubBlock *cfg, INDEX *index, BFILE *bfp )
{
	int i_sup, isf, itmp;
	int bitcount = 0;

	for ( i_sup=0; i_sup<cf->N_CH; i_sup++ ){
		for ( isf=0; isf<cfg->nsf; isf++){
			int indexSfOffset = isf * ( cfg->ncrb - cfg->ebc_crb_base ) - cfg->ebc_crb_base;
			for ( itmp=cfg->ebc_crb_base; itmp<cfg->ncrb; itmp++ ){
				bitcount += get_bstm( &index->bc[i_sup][itmp+indexSfOffset], cfg->ebc_bits, bfp);
			}
		}
	}
	
	return bitcount;
}

/*============================================================================*/
/* Name:        TvqReadBsFrame()                                              */
/* Description: read bitstream frame                                          */
/* Return:      (int) 1: successful reading, 0: imcompleted reading           */
/* Access:      external                                                      */
/*============================================================================*/
int CTwinVQ::TvqReadBsFrame(INDEX *index, /* Output: quantization indexes */
				   BFILE *bfp)	 /* Input:  bitstream file pointer */
{
	/*--- Variables ---*/
	tvqConfInfoSubBlock *cfg;
	int variableBits;
	int bitcount;
	int numFixedBitsPerFrame = TvqGetNumFixedBitsPerFrame();
	
	/*--- Initialization ---*/
	variableBits = 0;
	bitcount = 0;

	/*--- read block independent factors ---*/
	/* Window type */
	bitcount += get_bstm( &index->w_type, cf.BITS_WTYPE, bfp );
	if ( TvqWtypeToBtype( index->w_type, &index->btype ) ) {
		fprintf( stderr, "Error: unknown window type: %d\n", index->w_type );
		return 0;
	}
	int btype = index->btype;

	/*--- read block dependent factors ---*/
	cfg = &cf.cfg[btype]; // set the block dependent paremeters table

	bitcount += variableBits;
	
	/* Interleaved vector quantization */
	bitcount += GetVqInfo( cfg, bits_0[btype], bits_1[btype], variableBits, index, bfp );
	
	/* Bark-scale envelope */
	bitcount += GetBseInfo( &cf, cfg, index, bfp );
	/* Gain */
	bitcount += GetGainInfo( &cf, cfg, index, bfp );
	/* LSP */
	bitcount += GetLspInfo( &cf, index, bfp );
	/* PPC */
	if ( cfg->ppc_enable ){
		bitcount += GetPpcInfo( &cf, index, bfp );
	}
	/* Energy Balance Calibration */
	if ( cfg->ebc_enable ){
		bitcount += GetEbcInfo( &cf, cfg, index, bfp );
	}
	
	iframe += 1;

	return bitcount == numFixedBitsPerFrame ? 1 : 0;
	
}


/*============================================================================*/
/* Name:        TvqSkipFrame()                                                */
/* Description: skip frame                                                    */
/* Return:      (int)                                                         */
/* Access:      external                                                      */
/*============================================================================*/
int CTwinVQ::TvqSkipFrame(BFILE *bfp, long step)
{
	
	int numBits = TvqGetNumFixedBitsPerFrame();
	iframe += step;
	return bseek(bfp, numBits*step, BSEEK_CUR);

}

/*============================================================================*/
/* Name:        TvqGetFramePoint()                                            */
/* Description: returns current frame point                                   */
/* Return:      (int) current frame point                                     */
/* Access:      external                                                      */
/*============================================================================*/
int CTwinVQ::TvqGetBsFramePoint()
{
	return iframe;
}


/*----------------------------------------------------------------------------*/
/* Name:        LoadTwinChunk()                                               */
/* Description: load the TWIN chunk from a bitstream file                     */
/* Return:      (CChunkChunk) the TWIN chunk                                  */
/* Access:      static                                                        */
/*----------------------------------------------------------------------------*/
CChunkChunk* CTwinVQ::LoadTwinChunk( BFILE *bfp )
{
	int chunkSize;

	// �`�����N ID ��ǂ�ŁA�o�[�W�������`�F�b�N����
	char chunkID[KEYWORD_BYTES+VERSION_BYTES+1];
	get_string ( chunkID, KEYWORD_BYTES+VERSION_BYTES, bfp );
	TVQ_VERSION = TvqCheckVersion( chunkID );
	if ( TVQ_VERSION == TVQ_UNKNOWN_VERSION ) {
#if DEBUG_PRINT > 0 
		fprintf( stderr, "Header reading error: Unknown version (%s).\n", chunkID );
#endif
		return NULL;
	}

	// �`�����N�T�C�Y��ǂށB
	if ( get_bstm( &chunkSize, ELEM_BYTES * CHAR_BITS, bfp ) <= 0 ){
#if DEBUG_PRINT > 0 
		fprintf( stderr, "Header reading error: Failed to get header size.\n" );
#endif
		return NULL;
	};

	// �`�����N�T�C�Y�������f�[�^��ǂ�
	char *chunkData = new char[chunkSize+1];
	if ( get_string ( chunkData, chunkSize, bfp ) < chunkSize ) {
#if DEBUG_PRINT > 0 
		fprintf( stderr, "Header reading error: Failed to read header data.\n" );
#endif
		return NULL;
	}

	// TWIN �`�����N�𐶐����A�f�[�^����������
	CChunkChunk* TwinChunk = new CChunkChunk( chunkID );
	TwinChunk->PutData( chunkSize, chunkData );

	delete[] chunkData;

	return TwinChunk;
}


} // namespace _vmstwinvqdec
