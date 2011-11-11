/* (c)Copyright 1996-2000 NTT Cyber Space Laboratories */
/*                Modified on 2000.09.06 by N. Iwakami */

// HeaderManager.cpp: CHeaderManager �N���X�̃C���v�������e�[�V����
//
//////////////////////////////////////////////////////////////////////

#include "StdAfx.h"
#include <iostream>
#include "HeaderManager.h"

//////////////////////////////////////////////////////////////////////
// �\�z/����
//////////////////////////////////////////////////////////////////////

CHeaderManager::~CHeaderManager()
{

}

/*==============================================================================*/
/* Name:        CHeaderManager::create()                                        */
/* Description: �w�b�_�}�l�[�W���𐶐�����B                                    */
/* Return:      ���������w�b�_�}�l�[�W���ւ̃|�C���^�A�����Ɏ��s�����ꍇ�� NULL */
/* Access:      public (static)                                                 */
/*==============================================================================*/
CHeaderManager* CHeaderManager::Create(CChunkChunk &twinChunk)
{
	try {
		// �`�����N�}�l�[�W���𐶐�����B
		CHeaderManager* theManager = NULL;
		theManager = new CHeaderManager;
		theManager->Init( twinChunk );

		// TWIN�`�����N�̃w�b�_���擾����
		theManager->m_chunkID = twinChunk.GetID();
		if ( theManager->m_chunkID == "" ) {
			return NULL;
		}
		
		return theManager;
	}
	catch (err_WrongChunkFormat) {
		return NULL;
	}
}


/*============================================================================*/
/* Name:        CHeaderManager::Init()                                        */
/* Description: �w�b�_�}�l�[�W���̏�����������BCreate() ����̂݌Ă΂��B   */
/* Return:      �Ȃ�                                                          */
/* Access:      external                                                      */
/*============================================================================*/
void CHeaderManager::Init(CChunkChunk &twinChunk)
{
	try {
		// ��{�`�����N����{�`�����N�o���N�Ɏ��߂�B
		PickUpSubChunks( &m_primaryChunkBank, &twinChunk );

		// �⏕�`�����N����������⏕�`�����N�o���N�Ɏ��߂�B
		CChunkChunk scndChunk =  GetPrimaryChunk ( "SCND" );
		PickUpSubChunks( &m_secondaryChunkBank, &scndChunk );
	}

	catch (CChunkChunk::err_FailGetChunk){
		//std::cout << "Fail!!" << std::endl;
	}
	catch (err_FailGetChunk) {
		//std::cout << "Fail getting SCND chnunk" << std::endl;
	}

}


/*-------------------------------------------------------------------------------*/
/* Name:        CHeaderManager::PickUpSubChunks()                                */
/* Description: �`�����N����͂��āA�T�u�`�����N���E���o���`�����N�o���N�ɗa���� */
/* Return:      �Ȃ�                                                             */
/* Access:      static                                                           */
/*-------------------------------------------------------------------------------*/
void CHeaderManager::PickUpSubChunks(CChunkBank  *theChunkBank, // In/Out: �`�����N�o���N
									 CChunkChunk *inputChunk)   // Input:  ���̓`�����N
{
	// ����
	const int idSize = 4; // �`�����N ID �̃T�C�Y�i�S�����j

	inputChunk->rewind(); // �`�����N����͂���O�ɂ܂��߂����s��

	std::string id;
	CChunk *subChunk;
	try {
		while ( subChunk = inputChunk->GetNextChunk() ) { // �`�����N����T�u�`�����N�����o��
			id = subChunk->GetID();
			theChunkBank->insert( CChunkBank::value_type( id, *subChunk ) ); // ���o�����T�u�`�����N���`�����N�o���N�ɓo�^
			delete subChunk;
		}
	}

	catch (CChunkChunk::err_FailGetChunk) {
		throw err_WrongChunkFormat();
	}

}


CChunk CHeaderManager::GetChunk(CChunkBank *theChunkBank, string id)
{
	CChunkBank::iterator answer;

	answer = theChunkBank->find( id );		// �`�����N�̂���Ȃ���₢���킹��B
	if ( answer != theChunkBank->end() ){	// �����
		return answer->second;				// ���̃`�����N��߂��B
	}

	throw err_FailGetChunk(); // �`�����N���Ȃ���Ώ������������B

}

//////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////
// CUniStringInfo �N���X�̎���
//////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////

//////////////////////////////////////////////////////////////////////
// �\�z/����
//////////////////////////////////////////////////////////////////////

CUniStringInfo::CUniStringInfo( string id, string primary, string secondary, int primCode, int scndCode )
{
	m_id = id;
	m_primary = primary;
	m_secondary = secondary;
	m_primaryCharCode   = primCode;
	m_secondaryCharCode = scndCode;
}

CUniStringInfo::CUniStringInfo( string id, CHeaderManager& theManager )
{
	// ID ��ݒ肷��
	m_id = id;
	m_primaryCharCode = unknown_code;
	m_secondaryCharCode = unknown_code;

	int flag = 0;
	// ��{�`�����N�����R�s�[����
	try {
		CStringChunk primChunk( theManager.GetPrimaryChunk( m_id ) );
		PutPrimaryInfo( primChunk );
	}
	catch (CHeaderManager::err_FailGetChunk) {
		flag = 1;
	//	throw err_FailConstruction();
	}
	if (flag) throw err_FailConstruction();

	// �⏕�`�����N�����R�s�[����
	try {
		CStringChunk scndChunk( theManager.GetSecondaryChunk( m_id ) );
		PutSecondaryInfo( scndChunk );
	}
	catch (CHeaderManager::err_FailGetChunk) {
	}
	catch (err_NoCharCode) {
		throw err_FailConstruction();
	}
}

CUniStringInfo::~CUniStringInfo()
{

}


void CUniStringInfo::PutPrimaryInfo(CStringChunk& theChunk)
{
	// ID ���`�F�b�N
	if ( m_id == "" ) {
		m_id = theChunk.GetID();
	}
	else if ( m_id != theChunk.GetID() ) {
		throw err_ID();
	}

	// �f�[�^����������
	m_primary = theChunk.GetString();
}

void CUniStringInfo::PutSecondaryInfo(CStringChunk& theChunk)
{
	// ID ���`�F�b�N
	if ( m_id == "" ) {
		m_id = theChunk.GetID();
	}
	else if ( m_id != theChunk.GetID() ) {
		throw err_ID();
	}

	// �f�[�^����������
	string secondary = theChunk.GetString();
	if ( secondary.size() < 2 ) { // �����R�[�h��񂪂��邩�ǂ����`�F�b�N
		throw err_NoCharCode();
	}

	m_primaryCharCode = secondary[0] - '0';		// �����R�[�h�f�[�^
	m_secondaryCharCode = secondary[1] - '0';

	m_secondary = secondary.erase(0, 2);

}

