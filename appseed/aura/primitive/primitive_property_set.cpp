#include "framework.h"


property_set::property_set(::aura::application * papp, bool bAutoAdd, bool bMultiValue) :
   object(papp)
{

//   m_iIndex = 0;

}


property_set::~property_set()
{

}


//id property_set::get_new_id()
//{
//
//   index iMax = -1;
//
//   for(iterator it = begin(); it != end(); it++)
//   {
//
//      if(it->m_element1.m_etype == id::type_integer)
//      {
//
//         if(it->m_element1.m_i > iMax)
//         {
//
//            iMax = (index)it->m_element1.m_i;
//
//         }
//
//      }
//
//   }
//
//   return iMax + 1;
//
//}
//




//
//property & property_set::defer_auto_add(id idName)
//{
//
//   return *add(idName);
//
//}


bool property_set::is_set_empty(::count countMinimum) const
{
   return get_count() < countMinimum;
}

bool property_set::has_properties(::count countMinimum) const
{
   return get_count() >= countMinimum;
}


property * property_set::find_var_ci(const var & var) const
{
   for(const_iterator it = begin(); it != end(); it++)
   {
      if(it->get_value().compare_ci(var) == 0)
         return it.m_ppair;
   }
   return NULL;
}

property * property_set::find_value_ci(var var) const
{
   return find_var_ci(var);
}


property * property_set::find_var(const var & var) const
{
   for(const_iterator it = begin(); it != end(); it++)
   {
      if(it->get_value() == var)
         return it.m_ppair;
   }
   return NULL;
}

property * property_set::find_value(var var) const
{
   return find_var(var);
}

bool property_set::contains_var_ci(const var & var, ::count countMin, ::count countMax) const
{
   ::count count = 0;
   while((count < countMin || (countMax >= 0 && count <= countMax)) && find_var_ci(var) != NULL)
      count++;
   return count >= countMin && conditional(countMax >= 0, count <= countMax);
}

bool property_set::contains_value_ci(var var, ::count countMin, ::count countMax) const
{
   return contains_var_ci(var, countMin, countMax);
}

bool property_set::contains_value_ci(const char * psz, ::count countMin, ::count countMax) const
{
   ::count count = 0;
   while((count < countMin || (countMax >= 0 && count <= countMax)) && find_value_ci(psz) != NULL)
      count++;
   return count >= countMin && conditional(countMax >= 0, count <= countMax);
}


bool property_set::contains_var(const var & var, ::count countMin, ::count countMax) const
{
   ::count count = 0;
   while((count < countMin || (countMax >= 0 && count <= countMax)) && (find_var(var)) != NULL)
      count++;
   return count >= countMin && conditional(countMax >= 0, count <= countMax);
}

bool property_set::contains_value(var var, ::count countMin, ::count countMax) const
{
   return contains_var(var, countMin, countMax);
}

bool property_set::contains_value(const char * psz, ::count countMin, ::count countMax) const
{
   ::count count = 0;
   while((count < countMin || (countMax >= 0 && count <= countMax)) && find_value(psz) != NULL)
      count++;
   return count >= countMin && conditional(countMax >= 0, count <= countMax);
}


bool property_set::remove_first_var_ci(const var & var)
{

   property * pproperty = find_var_ci(var);

   if(pproperty != NULL)
   {

      return remove_key(pproperty->m_element1);

   }

   return false;

}


bool property_set::remove_first_value_ci(var var)
{

   return remove_first_var_ci(var);

}

bool property_set::remove_first_value_ci(const char * lpcsz)
{

   property * pproperty = find_value_ci(lpcsz);

   if(pproperty != NULL)
   {

      return remove_key(pproperty->m_element1);

   }

   return false;

}

bool property_set::remove_first_var(const var & var)
{

   property * pproperty = find_var(var);

   if(pproperty != NULL)
   {

      return remove_key(pproperty->m_element1);

   }

   return false;

}


bool property_set::remove_first_value(var var)
{

   return remove_first_var(var);

}


bool property_set::remove_first_value(const char * lpcsz)
{

   property * pproperty = find_value(lpcsz);

   if(pproperty != NULL)
   {

      return remove_key(pproperty->m_element1);

   }

   return false;

}


::count property_set::remove_var_ci(const var & var, ::count countMin, ::count countMax)
{
   ::count count = 0;
   if(contains_var_ci(var, countMin, countMax))
      while(conditional(countMax >= 0, count < countMax)
            && (remove_first_var_ci(var)))
         count++;
   return count;
}


::count property_set::remove_value_ci(var var, ::count countMin, ::count countMax)
{

   return remove_var_ci(var, countMin, countMax);

}


::count property_set::remove_value_ci(const char * psz, ::count countMin, ::count countMax)
{

   ::count count = 0;

   if(contains_value_ci(psz,countMin,countMax))
   {

      while(conditional(countMax >= 0,count < countMax) && remove_first_value_ci(psz))
      {

         count++;

      }

   }

   return count;

}


::count property_set::remove_var(const var & var, ::count countMin, ::count countMax)
{

   ::count count = 0;

   if(contains_var(var,countMin,countMax))
   {

      while(conditional(countMax >= 0,count < countMax && remove_first_var(var)))
      {

         count++;

      }

   }

   return count;

}


::count property_set::remove_value(var var, ::count countMin, ::count countMax)
{

   return remove_var(var, countMin, countMax);

}


::count property_set::remove_value(const char * psz, ::count countMin, ::count countMax)
{

   ::count count = 0;

   if(contains_value(psz,countMin,countMax))
   {

      while(conditional(countMax >= 0,count < countMax) && remove_first_value(psz))
      {

         count++;

      }

   }

   return count;

}


::count property_set::unset(id idName)
{
   property_map::pair * ppair = PLookup(idName);
   if(ppair == NULL)
      return 0;
   remove_key(ppair->m_element1);
   return 1;
}



bool property_set::is_new(id idName) const
{
   const property * pproperty = find(idName);
   if(pproperty == NULL)
      return true;
   return pproperty->is_new();
}

//bool property_set::is_new(string_interface & str) const
//{
//   return is_new((const char *) string(str));
//}

bool property_set::is_null(id idName) const
{
   const property * pproperty = find(idName);
   if(pproperty == NULL)
      return true;
   return pproperty->is_null();
}

//bool property_set::is_null(string_interface & str) const
//{
//   return is_null((const char *) string(str));
//}

bool property_set::is_new_or_null(id idName) const
{
   const property * pproperty = find(idName);
   if(pproperty == NULL)
      return true;
   return pproperty->is_new_or_null();
}

//bool property_set::is_new_or_null(string_interface & str) const
//{
//   return is_new_or_null((const char *) string(str));
//}


bool property_set::is_empty(id idName) const
{
   const property * pproperty = find(idName);
   if(pproperty == NULL)
      return true;
   return pproperty->is_empty();
}

//bool property_set::is_empty(string_interface & str) const
//{
//   const property * pproperty = find((const char *) string(str));
//   if(pproperty == NULL)
//      return true;
//   return pproperty->is_empty();
//}

//void property_set::OnBeforePropertyChange(property * pproperty)
//{
//   PropertySignalObject ::message::message(&m_signal);
//   ::message::message.m_etype = PropertySignalObject::TypeBeforeChange;
//   ::message::message.m_pproperty = pproperty;
//   ::message::message.m_pset = this;
//   m_signal.emit(&::message::message);
//}
//
//
//void property_set::OnAfterPropertyChange(const var & variableOld, property * pproperty)
//{
//   UNREFERENCED_PARAMETER(variableOld);
//   PropertySignalObject ::message::message(&m_signal);
//   ::message::message.m_etype = PropertySignalObject::TypeAfterChange;
//   ::message::message.m_pproperty = pproperty;
//   ::message::message.m_pset = this;
//   m_signal.emit(&::message::message);
//}
//

void property_set::_008ParseCommandLine(const char * pszCmdLineParam, var & varFile)
{
   string strApp;
   _008Parse(false, pszCmdLineParam, varFile, strApp);
}

void property_set::_008ParseCommandFork(const char * pszCmdLineParam, var & varFile, string & strApp)
{
   _008Parse(true, pszCmdLineParam, varFile, strApp);
}


void property_set::_008Add(const char * pszKey, const char * pszValue)
{

   stringa straKey;

   straKey.explode(".", pszKey);

   if (straKey.get_count() <= 0)
   {

      return;

   }

   property_set * pset = this;

   int32_t i = 0;

   while(i  < straKey.get_upper_bound())
   {

      pset = &(*pset)[straKey[i]].propset();

      i++;

   }

   if(pszValue != NULL && pset->has_property(straKey[i]) && pset->operator[](straKey[i]) != pszValue)
   {

      pset->operator[](straKey[i]).stra().add(pszValue);

   }
   else if (pszValue == NULL)
   {

      pset->operator[](straKey[i]) = var::type_key_exists;

   }
   else
   {

      pset->operator[](straKey[i])= var(pszValue);

   }

}


void property_set::_008Parse(bool bApp, const char * pszCmdLine, var & varFile, string & strApp)
{

   if(pszCmdLine == NULL)
      return;

   stringa stra = get_c_args(pszCmdLine);
   
   int i = 0;
   
   if(bApp && stra.get_count() > 0)
   {
    
      strApp = stra[0];
      
      i++;
      
   }
   
   index iFindColon = stra.find_first(":");
   
   if(iFindColon > i)
   {
    
      if(iFindColon - i > 1)
      {
         
         for(;i < iFindColon; i++)
         {
          
            varFile.stra().add(stra[i]);
            
         }
         
      }
      else
      {
         
         varFile = stra[i];
         
         i++;
         
      }
      
      i++;

   }
   
   for(; i < stra.get_size(); i++)
   {
    
      string str = stra[i];
      
      index iFindEqual = str.find('=');
      
      index iFindQuote = str.find('\"');
      
      if(iFindEqual >= 0)
      {
         
         string strValue;
         
         strValue = str.Mid(iFindEqual + 1);
         
         if(iFindEqual + 1 == iFindQuote)
         {
            
            const char * pszValue = strValue;
         
            strValue = ::str::consume_quoted_value(pszValue);
            
         }
       
         _008Add(str.Left(iFindEqual), strValue);
         
      }
      else
      {
         
         _008Add(str, NULL);
         
      }
      
   }

}


void property_set::skip_json(const char * & pszJson)
{

   skip_json(pszJson, pszJson + strlen(pszJson) - 1);

}


void property_set::skip_json(const char * & pszJson, const char * pszEnd)
{
   ::str::consume_spaces(pszJson, 0, pszEnd);
   if (*pszJson == '\0')
   {
      return;
   }
   ::str::consume(pszJson, "{", 1, pszEnd);
   ::str::consume_spaces(pszJson, 0, pszEnd);
   if (*pszJson == '}')
   {
      pszJson++;
      return;
   }
   while (true)
   {

      ::id id;

      property::skip_json_id(pszJson, pszEnd);

      property::skip_json_value(pszJson, pszEnd);

      ::str::consume_spaces(pszJson, 0, pszEnd);
      if (*pszJson == ',')
      {
         pszJson++;
         continue;
      }
      else if (*pszJson == '}')
      {
         pszJson++;
         return;
      }
      else
      {
         string str = "not expected character : ";
         str += pszJson;
         _throw(simple_exception(::get_app(), str));
      }
   }
}


void property_set::parse_json(const string & strJson)
{

   const char * pszJson = strJson;

   parse_json(pszJson, pszJson + strJson.get_length() - 1);

}


void property_set::parse_json(const char * & pszJson)
{

   parse_json(pszJson, pszJson + strlen(pszJson) - 1);

}


void property_set::parse_json(const char * & pszJson, const char * pszEnd)
{
   ::str::consume_spaces(pszJson, 0, pszEnd);
   if (*pszJson == '\0')
   {
      return;
   }
   ::str::consume(pszJson, "{", 1, pszEnd);
   ::str::consume_spaces(pszJson, 0, pszEnd);
   if (*pszJson == '}')
   {
      pszJson++;
      return;
   }
   while(true)
   {

      ::id id;

      property::parse_json_id(id, pszJson,pszEnd);

      property * pproperty = get_assoc(id);

      property::parse_json_value(pproperty->m_element2,pszJson,pszEnd);

      ::str::consume_spaces(pszJson, 0, pszEnd);
      if(*pszJson == ',')
      {
         pszJson++;
         continue;
      }
      else if(*pszJson == '}')
      {
         pszJson++;
         return;
      }
      else
      {
         string str = "not expected character : ";
         str += pszJson;
         _throw(simple_exception(::get_app(), str));
      }
   }
}

string & property_set::get_json(string & str, bool bNewLine) const
{

   str += "{";

   const_iterator it = begin();

   if(it != end())
   {

      it->get_json(str, bNewLine);

      it++;

   }

   for(; it != end(); it++)
   {

      if (bNewLine)
      {

         str += ", \r\n";

      }
      else
      {

         str += ", ";

      }

      it->get_json(str, bNewLine);

   }

   if (bNewLine)
   {

      str += "\r\n";

   }

   str += "}";


   return str;

}


void property_set::parse_url_query(const char * pszUrl)
{
   if(pszUrl == NULL)
      return;
   const char * pszUrlQuery = strchr(pszUrl, '?');
   if(pszUrlQuery == NULL)
      return _parse_url_query(pszUrl);
   else
      return _parse_url_query(pszUrlQuery + 1);
}

void property_set::_parse_url_query(const char * pszUrlQuery)
{
   if(pszUrlQuery == NULL)
      return;
   const char * pszParam = pszUrlQuery;
   const char * pszParamEnd;
   const char * pszKeyEnd;
   string strKey;
   while(true)
   {
      pszParamEnd = strchr(pszParam, '&');
      pszKeyEnd   = strchr(pszParam, '=');
      if(pszParamEnd == NULL)
      {
         if(pszKeyEnd == NULL)
         {
            strKey = url_decode_dup(pszParam, strlen(pszUrlQuery) - (pszParam - pszUrlQuery));
            _008Add(strKey, "");
         }
         else
         {
            string strKey = url_decode_dup(pszParam, pszKeyEnd - pszParam);
            string strValue = url_decode_dup(pszKeyEnd + 1, strlen(pszUrlQuery) - (pszKeyEnd + 1 - pszUrlQuery));
            _008Add(strKey, strValue);
         }
         return;
      }
      else
      {
         if(pszKeyEnd == NULL || pszKeyEnd > pszParamEnd)
         {
            strKey = url_decode_dup(pszParam, pszParamEnd - pszParam);
            _008Add(strKey, "");
         }
         else
         {
            string strKey = url_decode_dup(pszParam, pszKeyEnd - pszParam);
            string strValue = url_decode_dup(pszKeyEnd + 1, pszParamEnd - (pszKeyEnd + 1));
            _008Add(strKey, strValue);
         }
      }
      pszParam = pszParamEnd + 1;
   }

}

void property_set::parse_http_headers(const char * pszHeaders)
{
   stringa stra;
   stra.add_tokens(pszHeaders, "\r\n", TRUE);
   string strName;
   for(int32_t i = 0; i < stra.get_size(); i++)
   {
      strsize iPos = stra[i].find(":");
      if(iPos < 0)
      {
         strName = stra[i].make_lower();
         set_at(strName, var::type_empty);
      }
      else
      {
         strName = stra[i].Left(iPos).make_lower();
         set_at(strName, stra[i].Mid(iPos + 2));

      }

   }

}


::count property_set::remove_by_name(id idName)
{
   return unset(idName);
}

::count property_set::remove_by_name(stringa & stra)
{
   ::count count = 0;
   for(int32_t i = 0; i < stra.get_count(); i++)
   {
      count += remove_key(stra[i]);
   }
   return count;
}




void property_set::replace_gen(string & str)
{

   strsize iPos;

   char ch;

   char chNext;

   for (iPos = 0; iPos < str.get_length(); iPos++)
   {
      ch = str[iPos];
      if (iPos + 1 < str.get_length())
         chNext = str[iPos + 1];
      else
         chNext = '\0';
      if (ch == '\\')
      {
         iPos++;
         continue;
      }
      else if (ch == '{' && chNext == '$')
      {
         strsize iEnd = str.find('}', iPos + 1);
         if (iEnd < 0)
         {
            //error
            break;
         }
         string strEval = operator[](str.Mid(iPos + 2, iEnd - iPos - 2)).m_element2.get_string();
         str = str.Left(iPos) + strEval + str.Mid(iEnd + 1);
         iPos += strEval.get_length() - 1;
      }
      else if (ch == '$')
      {
         if (!(isalpha_dup(chNext) || chNext == '_'))
         {
            // error
            break;
         }
         strsize iStart = iPos;
         strsize iEnd = iStart + 2;
         for (; iEnd < str.get_length(); iEnd++)
         {
            ch = str[iEnd];
            if (!(isalpha_dup(ch) || ch == '_' || isdigit_dup(ch)))
            {
               break;
            }
         }
         string strEval = gen_eval(str.Mid(iStart, iEnd - iStart));
         str = str.Left(iPos) + strEval + str.Mid(iEnd);
         iPos += strEval.get_length() - 1;
      }
   }
}

string property_set::eval(const char * psz)
{
   string str = psz;
   replace_gen(str);
   return str;
}


string property_set::gen_eval(const char * psz)
{

   string str;

   ASSERT(psz != NULL);

   ASSERT(psz[0] == '$'); // until now accepts only one var

   ASSERT(strlen(psz) >= 2);

   str = &psz[1];

   return operator[](str);

}


void property_set::clear()
{
   remove_all();
}

void property_set::write(::file::ostream & ostream) const
{

   ostream << (const ::property_map &) *this;

}

void property_set::read(::file::istream & istream)
{

   istream >> (::property_map &) *this;

}


string property_set::implode(const char * pszGlue) const
{

   string str;

   const_iterator it = begin();

   if(it != end())
   {

      str = it->get_value().get_string();

      it++;

   }

   for(; it != end(); it++)
   {

      str += pszGlue;

      str += it->get_value().get_string();

   }

   return str;

}


property * property_set::find_value(const char * psz) const
{

   for(const_iterator it = begin(); it != end(); it++)
   {

      if(it->get_string() == psz)
      {

         return (property *) &it->m_element2;

      }

   }

   return NULL;

}


property * property_set::find_value_ci(const char * psz) const
{

   for(const_iterator it = begin(); it != end(); it++)
   {

      if(it->get_string().compare_ci(psz) == 0)
      {

         return (property *) &it->m_element2;

      }
   }

   return NULL;

}


property_set::property_set(const property_set & set)
{

//   m_iIndex = 0;

   operator = (set);

}


//property_set::property_set(const pair_set_interface & set)
//{
//
//   m_iIndex = 0;
//
//   operator = (set);
//
//}
//
//
//property_set::property_set(const str_str_interface & set)
//{
//
//   m_iIndex = 0;
//
//   operator = (set);
//
//}


property & property_set::at(index iIndex)
{

   return operator[](iIndex);

}

property property_set::at(index iIndex) const
{

   return operator[](iIndex);

}


property_set & property_set::operator = (const property_set & set)
{

   if (&set != this)
   {

//      m_iIndex = set.m_iIndex;

      ::lemon::map::copy((property_map & )*this, (const property_map & ) set);

   }

   return *this;

}


property_set & property_set::add(const property_set & set)
{

   if (&set != this)
   {

      for(const_iterator it = set.begin(); it != set.end(); it++)
      {

         operator[](it->name()).m_element2 = it->get_value();

      }

   }

   return *this;

}


property_set & property_set::merge(const property_set & set)
{

   if (&set != this)
   {

      for(const_iterator it = set.begin(); it != set.end(); it++)
      {

         const property * pproperty = it.m_ppair;

         id idName = pproperty->name();

         property * ppropertyThis = find(idName);

         if(!pproperty->is_new())
         {

            if(ppropertyThis != NULL)
            {

               if(ppropertyThis->get_value().get_type() == ::var::type_element || pproperty->get_value().get_type() == ::var::type_element)
               {

                  operator[](pproperty->name()).m_element2 = pproperty->m_element2;

               }
               else if(ppropertyThis->get_value().get_type() == ::var::type_propset)
               {

                  if(pproperty->get_value().get_type() == ::var::type_propset)
                  {

                     ppropertyThis->propset().merge(pproperty->propset());

                  }
                  else
                  {

                     index i = 0;

                     while(true)
                     {

                        if(!has_property(str::from(i)))
                        {

                           operator[](str::from(i)).m_element2 = pproperty->m_element2;

                           break;

                        }

                        i++;

                     }

                  }

               }
               else if(operator[](pproperty->name()).is_empty())
               {

                  operator[](pproperty->name()) = pproperty->m_element2;

               }
               else
               {

                  try
                  {

                     if(operator[](pproperty->name()) == pproperty->m_element2)
                     {

                        continue;

                     }

                  }
                  catch(...)
                  {

                  }

                  operator[](pproperty->name()).vara().add_unique(operator[](pproperty->name()).vara());

               }

            }
            else
            {

               operator[](pproperty->name()).m_element2 = pproperty->m_element2;

            }

         }

      }

   }

   return *this;

}


property_set & property_set::operator += (const property_set & set)
{

   return add(set);

}


property_set & property_set::operator |= (const property_set & set)
{

   return merge(set);

}


//property_set & property_set::operator = (const pair_set_interface & set)
//{
//
//   remove_all();
//
//   int32_t iCount = set.pair_set_interface_get_count();
//
//   for (int32_t i = 0; i < iCount; i++)
//   {
//
//      string strKey = set.pair_set_interface_get_key(i);
//
//      class var var = set.pair_set_interface_get_value(i);
//
//      set_at(strKey, var);
//   }
//
//   return *this;
//
//}
//
//
//
//
//
//
//
//
//property_set & property_set::operator = (const str_str_interface & set)
//{
//
//   remove_all();
//
//   int32_t iCount = set.str_str_interface_get_count();
//
//   for (int32_t i = 0; i < iCount; i++)
//   {
//
//      string strKey = set.str_str_interface_get_key(i);
//
//      class var var = set.str_str_interface_get_value(i);
//
//      set_at(strKey, var);
//   }
//
//   return *this;
//
//}


property * property_set::str_find(const property & property) const
{

   for(const_iterator it = begin(); it != end(); it++)
   {

      if(it->str_compare(property) == 0)
      {

         return (::property *) &it->m_element2;

      }

   }

   return NULL;

}


bool property_set::str_contains(const property_set & set) const
{

   for(const_iterator it = begin(); it != end(); it++)
   {

      if(str_find(*it) == NULL)
      {

         return false;

      }

   }

   return true;

}





string & property_set::get_http_post(string & strPost) const
{

   const_iterator it = begin();

   if(it != end())
   {

      it->get_http_post(strPost);

      it++;

   }

   for(; it != end(); it++)
   {

      strPost += "&";

      it->get_http_post(strPost);

   }

   return strPost;

}














CLASS_DECL_AURA::file::ostream & operator << (::file::ostream & ostream, const property_set & set)
{

   ostream << (const ::property_map &) set;

   return ostream;

}


CLASS_DECL_AURA::file::istream & operator >> (::file::istream & istream, property_set & set)
{

   istream >> (::property_map &) set;

   return istream;


}



stable_property_set::stable_property_set()
{

}


stable_property_set::~stable_property_set()
{


}


bool stable_property_set::is_new_or_null(id id)
{

   index iFind = find(id);

   if (iFind < 0)
   {

      return true;

   }

   if (m_propertya[iFind].m_element2.is_new())
   {

      return true;

   }

   return false;

}


bool stable_property_set::has_property(id id)
{

   index iFind = find(id);

   if (iFind < 0)
   {

      return false;

   }

   return true;

}


index stable_property_set::find(id id)
{

   for (index i = 0; i < m_propertya.get_size(); i++)
   {

      if (m_propertya[i].m_element1 == id)
      {

         return i;

      }

   }

   return -1;

}

property & stable_property_set::set_at(id id, var varValue)
{

   index iFind = find(id);

   if (iFind < 0)
   {

      property & p = m_propertya.add_new();

      p.m_element1 = id;

      return p;

   }
   else
   {

      m_propertya[iFind].m_element2 = varValue;

      return m_propertya[iFind];

   }

}


property & stable_property_set::operator [](id id)
{

   index iFind;

   if (id.is_integer())
   {

      iFind = (index) (id.int64());

      if (iFind < 0 || iFind >= m_propertya.get_count())
      {

         _throw(index_out_of_bounds(get_app()));

      }

   }
   else
   {

      iFind = find(id);

      if (iFind < 0)
      {

         return set_at(id, var::type_new);

      }

   }

   return m_propertya[iFind];

}


