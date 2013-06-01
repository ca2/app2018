#include "framework.h"
#include "base/x/x_charcategory2.h"
#include "base/x/x_charcategory_names.h"


int32_t gen_ch_class_reference_tables();


inline bool is_legal_uni_index(int64_t c)
{
   return c >= ((uint64_t) 0xffff) ? false : true;
}

int32_t gen_ch_class_reference_tables()
{
   return sizeof(char_bidi_names);
}


namespace ca
{

   ch_class::ch_class()
   {
      infoIndex = new bit_array *[256];
      memset(infoIndex, 0, 256 * sizeof(bit_array *));
   }

   ch_class::~ch_class()
   {
     clear();
     delete [] infoIndex;
   }

   /**
     Creates ch_class object from regexp character class syntax.
     Extensions (comparing to Perl):
     inner class substraction [{L}-[{Lu}]], addition [{L}[1234]], intersection [{L}&[{Lu}]]
   */
   ch_class *ch_class::createCharClass(const char * ccs, strsize pos, strsize *retPos)
   {
      string str = ccs;
      if(str == "(%back;?#1[\\.\\:]|\\^)\\M[^%nname;]+")
      {
         //__debug_break();
      }
      ch_class *cc = new ch_class();
      ch_class cc_temp;
      bool inverse = false;
      string prev_char;

      if(ccs[pos] != '[')
         return NULL;
      pos++;
      if(ccs[pos] == '^')
      {
         inverse = true;
         pos++;
      }
      for(; natural(pos) < strlen(ccs); pos++)
      {
         if(ccs[pos] == ']')
         {
            if(retPos != NULL)
               *retPos = pos;
            if(inverse)
            {
               ch_class * newcc = new ch_class();
               newcc->fill();
               newcc->clear_class(*cc);
               delete cc;
               cc = newcc;
            }
            return cc;
         }
         if(ccs[pos] == '{')
         {
            string categ;
            if(!::ca::str::get_curly_content(&ccs[pos], categ))
               return 0;
            if(categ == "ALL")
               cc->fill();
            else if (categ == "ASSIGNED")
               cc->add_category("");
            else if (categ == "UNASSIGNED")
            {
               cc_temp.clear();
               cc_temp.add_category("");
               cc->fill();
               cc->clear_class(cc_temp);
            }
            else if(categ.get_length())
               cc->add_category(*categ);
            pos += categ.get_length() + 1;
            prev_char = BAD_WCHAR;
            continue;
         }
         if(ccs[pos] == '\\' && natural(pos+1) < strlen(ccs))
         {
//            int32_t retEnd;
            prev_char = BAD_WCHAR;
            switch(ccs[pos+1])
            {
            case 'd':
               cc->add_category(CHAR_CATEGORY_Nd);
               break;
            case 'D':
               cc_temp.fill();
               cc_temp.clear_category(CHAR_CATEGORY_Nd);
               cc->add_class(cc_temp);
               break;
            case 'w':
               cc->add_category("L");
               cc->add_category(CHAR_CATEGORY_Nd);
               cc->add_char("_");
               break;
            case 'W':
               cc_temp.fill();
               cc_temp.clear_category("L");
               cc_temp.clear_category(CHAR_CATEGORY_Nd);
               cc_temp.clear_char("_");
               cc->add_class(cc_temp);
               break;
            case 's':
               cc->add_category("Z");
               cc->add_char(string(wchar_t(0x09)));
               cc->add_char(string(wchar_t(0x0A)));
               cc->add_char(string(wchar_t(0x0C)));
               cc->add_char(string(wchar_t(0x0D)));
               break;
            case 'S':
               cc_temp.fill();
               cc_temp.clear_category("Z");
               cc_temp.clear_char(string(wchar_t(0x09)));
               cc_temp.clear_char(string(wchar_t(0x0A)));
               cc_temp.clear_char(string(wchar_t(0x0C)));
               cc_temp.clear_char(string(wchar_t(0x0D)));
               cc->add_class(cc_temp);
               break;
            default:
               index retEnd;
               prev_char = ::ca::str::uni_to_utf8(
                  ::ca::str::get_escaped_char(ccs, pos, retEnd));
               if(prev_char.is_empty())
                  break;
               cc->add_char(prev_char);
               pos = retEnd-1;
               break;
            }
            pos++;
            continue;
         };
         // substract -[class]
         if (natural(pos+1) < strlen(ccs) && ccs[pos] == '-' && ccs[pos+1] == '[')
         {
            strsize retEnd;
            ch_class * scc = createCharClass(ccs, pos+1, &retEnd);
            if(natural(retEnd) == strlen(ccs))
               return NULL;
            if(scc == NULL)
            {
               delete cc;
               return NULL;
            }
            cc->clear_class(*scc);
            delete scc;
            pos = retEnd;
            prev_char = BAD_WCHAR;
            continue;
         }
         // intersect &&[class]
         if (natural(pos+2) < strlen(ccs) && ccs[pos] == '&' && ccs[pos+1] == '&' && ccs[pos+2] == '[')
         {
            strsize retEnd;
            ch_class *scc = createCharClass(ccs, pos+2, &retEnd);
            if(natural(retEnd) == strlen(ccs))
               return NULL;
            if(scc == NULL)
            {
               delete cc;
               return NULL;
            }
            cc->intersect_class(*scc);
            delete scc;
            pos = retEnd;
            prev_char = BAD_WCHAR;
            continue;
         }
         // add [class]
         if (ccs[pos] == '[')
         {
            strsize retEnd;
            ch_class *scc = createCharClass(ccs, pos, &retEnd);
            if(scc == NULL)
            {
               delete cc;
               return NULL;
            }
            cc->add_class(*scc);
            delete scc;
            pos = retEnd;
            prev_char = BAD_WCHAR;
            continue;
         }

         if(ccs[pos] == '-' && prev_char.has_char() && natural(pos+1) < strlen(ccs) && ccs[pos+1] != ']')
         {

            index retEnd;

            string nextc = ::ca::str::uni_to_utf8(::ca::str::get_escaped_char(ccs, pos+1, retEnd));

            if(nextc.is_empty())
               break;

            cc->add_range(prev_char, nextc);

            pos = retEnd;

            continue;

         }

         cc->add_char(::ca::str::get_utf8_char(&ccs[pos]));

         prev_char = ccs[pos];

      }
      delete cc;
      return NULL;
   }

   void ch_class::add_char(const char * pszUtf8Char)
   {
      int64_t iChar = ::ca::ch::uni_index(pszUtf8Char);
      bit_array * tablePos = infoIndex[iChar >> 8];
      if (!tablePos)
      {
         tablePos = new bit_array();
         infoIndex[iChar >> 8] = tablePos;
      }
      tablePos->set_bit(iChar & 0xFF);
   }

   void ch_class::clear_char(const char * pszUtf8Char)
   {
      int64_t iChar = ::ca::ch::uni_index(pszUtf8Char);
      bit_array *tablePos = infoIndex[iChar >> 8];
      if(!tablePos)
         return;
      tablePos->clear_bit(iChar & 0xFF);
   }

   void ch_class::add_range(const char * s, const char * e)
   {
      int64_t iCharStart = ::ca::ch::uni_index(s);
      int64_t iCharEnd = ::ca::ch::uni_index(e);
      for(int64_t ti = iCharStart >> 8; ti <= iCharEnd >> 8; ti++)
      {
         if (!infoIndex[ti])
            infoIndex[ti] = new bit_array();
         infoIndex[ti]->add_range(
            (ti == iCharStart >> 8) ? iCharStart & 0xFF : 0,
            (ti == iCharEnd >> 8)? iCharEnd & 0xFF : 0xFF);
      }
   }

   void ch_class::clear_range(const char * s, const char * e)
   {
      int64_t iCharStart = ::ca::ch::uni_index(s);
      int64_t iCharEnd = ::ca::ch::uni_index(e);
      for(int64_t ti = iCharStart >> 8; ti <= iCharEnd >> 8; ti++)
      {
         if (!infoIndex[ti])
            infoIndex[ti] = new bit_array();
         infoIndex[ti]->clear_range(
            ti == iCharStart >> 8 ? iCharStart & 0xFF : 0,
            ti == iCharEnd >> 8 ? iCharEnd & 0xFF:0xFF);
      }
   }

   void ch_class::add_category(ECharCategory cat)
   {
      if(!cat || cat >= CHAR_CATEGORY_LAST)
         return;
      for (int32_t i = 0; i < 256; i++)
      {
         uint16_t pos = arr_idxCharCategoryIdx[(int32_t(cat)-1) * 256 + i];
         if(!pos)
            continue;
         bit_array *tablePos = infoIndex[i];
         if(tablePos == NULL)
         {
            tablePos = new bit_array();
            infoIndex[i] = tablePos;
         }
         tablePos->add_bit_array((char*)(arr_idxCharCategory+pos), 8*4);
      }
   }

   void ch_class::add_category(string cat)
   {
      for(int32_t pos = 0; pos < ARRAY_SIZE(char_category_names); pos++)
      {
         if(::ca::str::begins(cat, char_category_names[pos]))
         {
            add_category(ECharCategory(pos));
         }
      }
   }

   void ch_class::clear_category(ECharCategory cat)
   {
      if(!cat || cat >= CHAR_CATEGORY_LAST)
         return;
      for(int32_t i = 0; i < 256; i++)
      {
         uint16_t pos = arr_idxCharCategoryIdx[(int32_t(cat)-1) * 256 + i];
         if(!pos)
            continue;
         bit_array *tablePos = infoIndex[i];
         if(!tablePos)
         {
            tablePos = new bit_array();
            infoIndex[i] = tablePos;
         }
         tablePos->clear_bit_array((char*)(arr_idxCharCategory+pos), 8*4);
      }
   }

   void ch_class::clear_category(string cat)
   {
      for(int32_t pos = 0; pos < ARRAY_SIZE(char_category_names); pos++)
      {
         int32_t ci;
         for(ci = 0; ci < cat.get_length() && cat[ci] == char_category_names[pos][ci]; ci++);
         if(ci == cat.get_length()) clear_category(ECharCategory(pos));
      }
   }

   void ch_class::add_class(const ch_class &cclass)
   {
      for(int32_t p = 0; p < 256; p++)
      {
         if (infoIndex[p] == NULL)
            infoIndex[p] = new bit_array();
         infoIndex[p]->add_bit_array(*cclass.infoIndex[p]);
      }
   }

   void ch_class::intersect_class(const ch_class &cclass)
   {
      for(int32_t p = 0; p < 256; p++)
      {
         if (infoIndex[p] != NULL)
         {
            infoIndex[p]->intersect_bit_array(*cclass.infoIndex[p]);
         }
      }
   }

   void ch_class::clear_class(const ch_class &cclass)
   {
      for(int32_t p = 0; p < 256; p++)
      {
         if(infoIndex[p] != NULL)
         {
            infoIndex[p]->clear_bit_array(*cclass.infoIndex[p]);
         }
      }
   }

   void ch_class::clear()
   {
      for(int32_t i = 0; i < 256; i++)
      {
         if(infoIndex[i] != NULL)
         {
            delete infoIndex[i];
            infoIndex[i] = NULL;
         }
      }
   }

   void ch_class::fill()
   {
      for(int32_t i = 0; i < 256; i++)
      {
         if(infoIndex[i] == NULL)
            infoIndex[i] = new bit_array();
         infoIndex[i]->add_range(0, 0xFF);
      }
   }

   bool ch_class::in_class(const char * pszUtf8Char) const
   {
      int64_t c = ::ca::ch::uni_index(pszUtf8Char);
      if(!is_legal_uni_index(c))
         return false;
      bit_array * tablePos = infoIndex[c>>8];
      if(tablePos == NULL)
         return false;
      return tablePos->get_bit(c & 0xFF);
   }

} // namespace ca


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
