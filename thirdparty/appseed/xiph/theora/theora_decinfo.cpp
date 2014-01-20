/********************************************************************
 *                                                                  *
 * THIS FILE IS PART OF THE OggTheora SOFTWARE CODEC SOURCE CODE.   *
 * USE, DISTRIBUTION AND REPRODUCTION OF THIS LIBRARY SOURCE IS     *
 * GOVERNED BY A BSD-STYLE SOURCE LICENSE INCLUDED WITH THIS SOURCE *
 * IN 'COPYING'. PLEASE READ THESE TERMS BEFORE DISTRIBUTING.       *
 *                                                                  *
 * THE Theora SOURCE CODE IS COPYRIGHT (C) 2002-2009                *
 * by the Xiph.Org Foundation and contributors http://www.xiph.org/ *
 *                                                                  *
 ********************************************************************

  function:
    last mod: $Id: decinfo.c 16503 2009-08-22 18:14:02Z giles $

 ********************************************************************/
#include "framework.h"



/*Unpacks a series of octets from a given byte array into the pack buffer.
  No checking is done to ensure the buffer contains enough data.
  _opb: The pack buffer to read the octets from.
  _buf: The byte array to store the unpacked bytes in.
  _len: The number of octets to unpack.*/
static void oc_unpack_octets(oc_pack_buf *_opb,char *_buf,size_t _len){
  while(_len-->0){
    long val;
    val=oc_pack_read(_opb,8);
    *_buf++=(char)val;
  }
}

/*Unpacks a 32-bit integer encoded by octets in little-endian form.*/
static long oc_unpack_length(oc_pack_buf *_opb){
  long ret[4];
  int32_t  i;
  for(i=0;i<4;i++)ret[i]=oc_pack_read(_opb,8);
  return ret[0]|ret[1]<<8|ret[2]<<16|ret[3]<<24;
}

static int32_t oc_info_unpack(oc_pack_buf *_opb,th_info *_info){
  long val;
  /*Check the codec bitstream version.*/
  val=oc_pack_read(_opb,8);
  _info->version_major=(uchar)val;
  val=oc_pack_read(_opb,8);
  _info->version_minor=(uchar)val;
  val=oc_pack_read(_opb,8);
  _info->version_subminor=(uchar)val;
  /*verify we can parse this bitstream version.
     We accept earlier minors and all subminors, by spec*/
  if(_info->version_major>TH_VERSION_MAJOR||
   _info->version_major==TH_VERSION_MAJOR&&
   _info->version_minor>TH_VERSION_MINOR){
    return TH_EVERSION;
  }
  /*Read the encoded frame description.*/
  val=oc_pack_read(_opb,16);
  _info->frame_width=(uint32_t)val<<4;
  val=oc_pack_read(_opb,16);
  _info->frame_height=(uint32_t)val<<4;
  val=oc_pack_read(_opb,24);
  _info->pic_width=(uint32_t)val;
  val=oc_pack_read(_opb,24);
  _info->pic_height=(uint32_t)val;
  val=oc_pack_read(_opb,8);
  _info->pic_x=(uint32_t)val;
  val=oc_pack_read(_opb,8);
  _info->pic_y=(uint32_t)val;
  val=oc_pack_read(_opb,32);
  _info->fps_numerator=(uint32_t)val;
  val=oc_pack_read(_opb,32);
  _info->fps_denominator=(uint32_t)val;
  if(_info->frame_width==0||_info->frame_height==0||
   _info->pic_width+_info->pic_x>_info->frame_width||
   _info->pic_height+_info->pic_y>_info->frame_height||
   _info->fps_numerator==0||_info->fps_denominator==0){
    return TH_EBADHEADER;
  }
  /*Note: The sense of pic_y is inverted in what we pass back to the
     application compared to how it is stored in the bitstream.
    This is because the bitstream uses a right-handed coordinate system, while
     applications expect a left-handed one.*/
  _info->pic_y=_info->frame_height-_info->pic_height-_info->pic_y;
  val=oc_pack_read(_opb,24);
  _info->aspect_numerator=(uint32_t)val;
  val=oc_pack_read(_opb,24);
  _info->aspect_denominator=(uint32_t)val;
  val=oc_pack_read(_opb,8);
  _info->colorspace=(th_colorspace)val;
  val=oc_pack_read(_opb,24);
  _info->target_bitrate=(int32_t)val;
  val=oc_pack_read(_opb,6);
  _info->quality=(int32_t)val;
  val=oc_pack_read(_opb,5);
  _info->keyframe_granule_shift=(int32_t)val;
  val=oc_pack_read(_opb,2);
  _info->pixel_fmt=(th_pixel_fmt)val;
  if(_info->pixel_fmt==TH_PF_RSVD)return TH_EBADHEADER;
  val=oc_pack_read(_opb,3);
  if(val!=0||oc_pack_bytes_left(_opb)<0)return TH_EBADHEADER;
  return 0;
}

static int32_t oc_comment_unpack(oc_pack_buf *_opb,th_comment *_tc){
  long len;
  int32_t  i;
  /*Read the vendor string.*/
  len=oc_unpack_length(_opb);
  if(len<0||len>oc_pack_bytes_left(_opb))return TH_EBADHEADER;
  _tc->vendor=(char *) _ogg_malloc((size_t)len+1);
  if(_tc->vendor==NULL)return TH_EFAULT;
  oc_unpack_octets(_opb,_tc->vendor,len);
  _tc->vendor[len]='\0';
  /*Read the user comments.*/
  _tc->comments=(int32_t)oc_unpack_length(_opb);
  len=_tc->comments;
  if(len<0||len>(LONG_MAX>>2)||len<<2>oc_pack_bytes_left(_opb)){
    _tc->comments=0;
    return TH_EBADHEADER;
  }
  _tc->comment_lengths=(int32_t *)_ogg_malloc(
   _tc->comments*sizeof(_tc->comment_lengths[0]));
  _tc->user_comments=(char **)_ogg_malloc(
   _tc->comments*sizeof(_tc->user_comments[0]));
  for(i=0;i<_tc->comments;i++){
    len=oc_unpack_length(_opb);
    if(len<0||len>oc_pack_bytes_left(_opb)){
      _tc->comments=i;
      return TH_EBADHEADER;
    }
    _tc->comment_lengths[i]=len;
    _tc->user_comments[i]=(char *) _ogg_malloc((size_t)len+1);
    if(_tc->user_comments[i]==NULL){
      _tc->comments=i;
      return TH_EFAULT;
    }
    oc_unpack_octets(_opb,_tc->user_comments[i],len);
    _tc->user_comments[i][len]='\0';
  }
  return oc_pack_bytes_left(_opb)<0?TH_EBADHEADER:0;
}

static int32_t oc_setup_unpack(oc_pack_buf *_opb,th_setup_info *_setup){
  int32_t ret;
  /*Read the quantizer tables.*/
  ret=oc_quant_params_unpack(_opb,&_setup->qinfo);
  if(ret<0)return ret;
  /*Read the Huffman trees.*/
  return oc_huff_trees_unpack(_opb,_setup->huff_tables);
}

static void oc_setup_clear(th_setup_info *_setup){
  oc_quant_params_clear(&_setup->qinfo);
  oc_huff_trees_clear(_setup->huff_tables);
}

static int32_t oc_dec_headerin(oc_pack_buf *_opb,th_info *_info,
 th_comment *_tc,th_setup_info **_setup,ogg_packet *_op){
  char buffer[6];
  long val;
  int32_t  packtype;
  int32_t  ret;
  val=oc_pack_read(_opb,8);
  packtype=(int32_t)val;
  /*If we're at a data packet and we have received all three headers, we're
     done.*/
  if(!(packtype&0x80)&&_info->frame_width>0&&_tc->vendor!=NULL&&*_setup!=NULL){
    return 0;
  }
  /*Check the codec string.*/
  oc_unpack_octets(_opb,buffer,6);
  if(memcmp(buffer,"theora",6)!=0)return TH_ENOTFORMAT;
  switch(packtype){
    /*Codec info header.*/
    case 0x80:{
      /*This should be the first packet, and we should not already be
         initialized.*/
      if(!_op->b_o_s||_info->frame_width>0)return TH_EBADHEADER;
      ret=oc_info_unpack(_opb,_info);
      if(ret<0)th_info_clear(_info);
      else ret=3;
    }break;
    /*Comment header.*/
    case 0x81:{
      if(_tc==NULL)return TH_EFAULT;
      /*We shoud have already decoded the info header, and should not yet have
         decoded the comment header.*/
      if(_info->frame_width==0||_tc->vendor!=NULL)return TH_EBADHEADER;
      ret=oc_comment_unpack(_opb,_tc);
      if(ret<0)th_comment_clear(_tc);
      else ret=2;
    }break;
    /*Codec setup header.*/
    case 0x82:{
      oc_setup_info *setup;
      if(_tc==NULL||_setup==NULL)return TH_EFAULT;
      /*We should have already decoded the info header and the comment header,
         and should not yet have decoded the setup header.*/
      if(_info->frame_width==0||_tc->vendor==NULL||*_setup!=NULL){
        return TH_EBADHEADER;
      }
      setup=(oc_setup_info *)_ogg_calloc(1,sizeof(*setup));
      if(setup==NULL)return TH_EFAULT;
      ret=oc_setup_unpack(_opb,setup);
      if(ret<0){
        oc_setup_clear(setup);
        _ogg_free(setup);
      }
      else{
        *_setup=setup;
        ret=1;
      }
    }break;
    default:{
      /*We don't know what this header is.*/
      return TH_EBADHEADER;
    }break;
  }
  return ret;
}


/*Decodes one header packet.
  This should be called repeatedly with the packets at the beginning of the
   stream until it returns 0.*/
int32_t th_decode_headerin(th_info *_info,th_comment *_tc,
 th_setup_info **_setup,ogg_packet *_op){
  oc_pack_buf opb;
  if(_op==NULL)return TH_EBADHEADER;
  if(_info==NULL)return TH_EFAULT;
  oc_pack_readinit(&opb,_op->packet,_op->bytes);
  return oc_dec_headerin(&opb,_info,_tc,_setup,_op);
}

void th_setup_free(th_setup_info *_setup){
  if(_setup!=NULL){
    oc_setup_clear(_setup);
    _ogg_free(_setup);
  }
}
