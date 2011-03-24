#include "StdAfx.h"


#undef new


bit_array::bit_array()
{
   m_pdata        = NULL;
   m_iDataCount   = 0;
   m_iBitCount    = 0;
}

bit_array::~bit_array()
{
   if(m_pdata != NULL && INT_PTR(m_pdata) != 1) 
      delete[] m_pdata;
}

void bit_array::ensure_size(int iSize, bool bSet)
{
   if(m_pdata == NULL
   || INT_PTR(m_pdata) == 1
   || iSize > m_iBitCount)
   {
      set_size(iSize, bSet);
   }
}

void bit_array::set_size(int iBitCount, bool bSet)
{
   int * pdataOld       = m_pdata;
//   int iOldDataCount    = m_iDataCount;
   int iOldBitCount     = m_iBitCount;
   m_iBitCount          = iBitCount;
   m_iDataCount         = gen::math::ceil_div(m_iBitCount, (int) 8 * sizeof(int));
   if(m_iDataCount > 0)
   {
      m_pdata              = new int[m_iDataCount];
      if(m_pdata != NULL)
      {
         if(INT_PTR(pdataOld) == 1)
         {
            gen::bit::set(m_pdata, true, 0, m_iDataCount * 8 * sizeof(int) - 1);
         }
         else if(pdataOld != NULL)
         {
            gen::bit::int_aligned_copy(m_pdata, 0, pdataOld, 0, iOldBitCount - 1);
            delete [] pdataOld;
         }
         if(iOldBitCount < m_iBitCount)
         {
            gen::bit::set(m_pdata, bSet, iOldBitCount, m_iBitCount - 1);
         }
      }
   }
   else
   {
      if(bSet)
      {
         m_pdata = (int *) 1;
      }
      else
      {
         m_pdata = 0;
      }
   }
}

int bit_array::get_size()
{
   return m_iBitCount;
}

void bit_array::set_bit(int pos)
{
   if(INT_PTR(m_pdata)== 1)
      return;
   ensure_size(pos + 1);
   m_pdata[pos>>5] |= 1 << (pos&0x1f);
}

void bit_array::clear_bit(int pos)
{
   if(m_pdata == NULL)
      return;
   ensure_size(pos + 1, true);
   m_pdata[pos>>5] &= ~(1 << (pos&0x1f));
}

void bit_array::add_range(int s, int e)
{
   if(INT_PTR(m_pdata) == 1)
      return;
   ensure_size(e + 1);
   int cs = s>>5;
   if (s&0x1f)
   {
      int fillbytes = 0xFFFFFFFF << (s&0x1f);
      if((e>>5) == (s>>5))
         fillbytes &= 0xFFFFFFFF >> (0x1F - e&0x1F);
      m_pdata[cs] |= fillbytes;
      cs++;
   }
   int ce = e>>5;
   if (s>>5 != ce && (e&0x1f) != 0x1f)
   {
      m_pdata[ce] |= 0xFFFFFFFF >> (0x1F - e&0x1F);
      ce--;
   }
   for(int idx = cs; idx <= ce; idx++)
      m_pdata[idx] = 0xFFFFFFFF;
   if (cs == 0 && ce == m_iDataCount-1)
   {
      delete[] m_pdata;
      m_pdata = (int*)1;
   }
}

void bit_array::clear_range(int s, int e)
{
   if(m_pdata == NULL)
      return;
   ensure_size(e + 1, true);
   int cs = s>>5;
   if (s&0x1f)
   {
      int fillbytes = 0xFFFFFFFF << (s&0x1f);
      if((e&0x1F) == (s&0x1F))
         fillbytes &= 0xFFFFFFFF >> (0x1F - e&0x1F);
      m_pdata[cs] &= ~fillbytes;
      cs++;
   }
   int ce = e>>5;
   if (s>>5 != ce && (e&0x1f) != 0x1f)
   {
      m_pdata[ce] &= ~(0xFFFFFFFF >> (0x1F-(e&0x1F)));
      ce--;
   }
   for(int idx = cs; idx <= ce; idx++)
      m_pdata[idx] = 0x0;
   if (cs == 0 && ce == m_iDataCount-1)
   {
      delete[] m_pdata;
      m_pdata = (int*)0;
   }
}

void bit_array::add_bit_array(bit_array & ba)
{
   if(INT_PTR(m_pdata) == 1) 
      return;
   if(!&ba || !ba.m_pdata)
      return;
   if(INT_PTR(ba.m_pdata) == 1)
   {
      m_pdata = (int*)1;
      return;
   }
   ensure_size(ba.get_size());
   for(int i = 0; i < m_iDataCount; i++)
      m_pdata[i] |= ba.m_pdata[i];
}

void bit_array::clear_bit_array(bit_array & ba)
{
  if(m_pdata == NULL) 
     return;
  if(&ba == NULL || ba.m_pdata == NULL)
     return;
  if(INT_PTR(ba.m_pdata) == 1)
  {
    if(m_pdata != NULL)
       delete[] m_pdata;
    m_pdata = NULL;
    return;
  }
  ensure_size(ba.get_size(), true);
  for(int i = 0; i < ba.m_iDataCount && i < m_iDataCount; i++)
   m_pdata[i] &= ~ba.m_pdata[i];
}

void bit_array::intersect_bit_array(bit_array & ba)
{
   if(m_pdata == NULL) 
      return;
   if(ba.m_pdata == NULL)
   {
      delete[] m_pdata;
      m_pdata = 0;
      return;
   }
   if(INT_PTR(ba.m_pdata) == 1)
      return;
   ensure_size(ba.get_size(), true);
   for(int i = 0; i < ba.m_iDataCount && i < m_iDataCount; i++)
      m_pdata[i] &= ba.m_pdata[i];
}

void bit_array::add_bit_array(char *bits, int count)
{
   if (INT_PTR(m_pdata) == 1) return;
   ensure_size(count * 8);
   for(int i = 0; i < count && i < m_iDataCount*4;i++)
      ((char*)m_pdata)[i] |= bits[i];
}

void bit_array::clear_bit_array(char *bits, int count)
{
   if(m_pdata == NULL) 
      return;
   ensure_size(count * 8, true);
   for(int i = 0; i < count && i < m_iDataCount*4;i++)
      ((char*)m_pdata)[i] &= ~bits[i];
}

bool bit_array::get_bit(int pos) const
{
   if(m_pdata == NULL) 
      return false;
   if(INT_PTR(m_pdata) == 1) 
      return true;
   if(pos >= m_iBitCount)
      return false;
   return (m_pdata[pos>>5] & (1<<(pos&0x1f))) != 0;
}


/* ***** BEGIN LICENSE BLOCK *****
 * Version: MPL 1.1/GPL 2.0/LGPL 2.1
 *
 * The contents of this file are subject to the Mozilla Public License Version
 * 1.1 (the "License"); you may not use this file except in compliance with
 * the License. You may obtain a copy of the License at
 * http://www.mozilla.org/MPL/
 *
 * Software distributed under the License is distributed on an "AS IS" basis,
 * WITHOUT WARRANTY OF ANY KIND, either express or implied. See the License
 * for the specific language governing rights and limitations under the
 * License.
 *
 * The Original Code is the Colorer Library.
 *
 * The Initial Developer of the Original Code is
 * Cail Lomecb <cail@nm.ru>.
 * Portions created by the Initial Developer are Copyright (C) 1999-2005
 * the Initial Developer. 
 *
 * Contributor(s):
 *
 * Alternatively, the contents of this file may be used under the terms of
 * either the GNU General Public License Version 2 or later (the "GPL"), or
 * the GNU Lesser General Public License Version 2.1 or later (the "LGPL"),
 * in which case the provisions of the GPL or the LGPL are applicable instead
 * of those above. If you wish to allow use of your version of this file only
 * under the terms of either the GPL or the LGPL, and not to allow others to
 * use your version of this file under the terms of the MPL, indicate your
 * decision by deleting the provisions above and replace them with the notice
 * and other provisions required by the GPL or the LGPL. If you do not delete
 * the provisions above, a recipient may use your version of this file under
 * the terms of any one of the MPL, the GPL or the LGPL.
 *
 * ***** END LICENSE BLOCK ***** */
