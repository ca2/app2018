/**************************************************************************************************
	$Id: bits.h,v 1.5 2005/04/20 16:43:22 bboy Exp $

	Copyright (C) 2002-2005  Don Moore <bboy@bboy.net>

	This program is free software; you can redistribute it and/or modify
	it under the terms of the GNU General Public License as published by
	the Free Software Foundation; either version 2 of the License, or
	(at Your option) any later version.

	This program is distributed in the hope that it will be useful,
	but WITHOUT ANY WARRANTY; without even the implied warranty of
	MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
	GNU General Public License for more details.

	You should have received a copy of the GNU General Public License
	along with this program; if not, write to the Free Software
	Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA  02111-1307  USA
**************************************************************************************************/

#ifndef _MYDNS_BITS_H
#define _MYDNS_BITS_H


#define SIZE16		sizeof(uint16_t)
#define SIZE32		sizeof(uint32_t)


/* Copy value of uint16_t `n' into `src'; Move `src' forward to next offset */
#define DNS_GET16(t, n, src) { \
	register uchar *t_src = (uchar *)(src); \
	(n) = (t) (((uint16_t)t_src[0] << 8) \
	    | ((uint16_t)t_src[1])) \
	    ; \
	((uchar *&)src) += SIZE16; \
}

/* Copy value of uint32_t `n' into `dest'; Move `dest' forward to next offset */
#define DNS_GET32(n, src) { \
	register uchar *t_src = (uchar *)(src); \
	(n) = ((uint32_t)t_src[0] << 24) \
	    | ((uint32_t)t_src[1] << 16) \
	    | ((uint32_t)t_src[2] << 8) \
	    | ((uint32_t)t_src[3]) \
	    ; \
	((uchar * &)src) += SIZE32; \
}

/* Copy value of uint16_t `n' into `dest'; Move `dest' forward to next offset */
#define DNS_PUT16(dest, n) { \
	register uint16_t t_n = (uint16_t)(n); \
	register uchar *t_dest = (uchar *)(dest); \
	*t_dest++ = (uchar) (t_n >> 8); \
	*t_dest   = (uchar) t_n; \
	((uchar * &)dest) += SIZE16; \
}

/* Copy value of uint32_t `n' into `dest'; Move `dest' forward to next offset */
#define DNS_PUT32(dest, n) { \
	register uint32_t t_n = (uint32_t)(n); \
	register uchar *t_dest = (uchar *)(dest); \
	*t_dest++ = (uchar) (t_n >> 24); \
	*t_dest++ = (uchar) (t_n >> 16); \
	*t_dest++ = (uchar) (t_n >> 8); \
	*t_dest   = (uchar) t_n; \
	((uchar * &)dest) += SIZE32; \
}

/* Copy `len' bytes of data from `src' to `dest'; Move `dest' forward `len' bytes */
#define DNS_PUT(dest, src, len) { \
	memcpy((dest), (src), (len)); \
	((uchar * &)dest) += (len); \
}

/* Copy `len' bytes of data from `src' to `dest'; Move `dest' forward `len' bytes */
#define DNS_GET(dest, src, len) { \
	memcpy((dest), (src), (len)); \
	((uchar * &)src) += (len); \
}


#endif /* _MYDNS_BITS_H */

/* vi:set ts=3: */
