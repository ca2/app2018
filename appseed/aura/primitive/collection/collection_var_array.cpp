#include "framework.h"


var_array::var_array(::aura::application * papp) :
   object(papp)
{
}

var_array::var_array(const stringa & stra)
{
   operator = (stra);
}

var_array::var_array(const int_array & ia)
{
   operator = (ia);
}

var_array::var_array(const property_set & propset)
{
   operator = (propset);
}

var_array::var_array(const var_array & vara)
{
   operator = (vara);
}

var_array::~var_array()
{

}

index var_array::add(var var)
{
   if(var.get_type() != var::type_empty_argument)
   {
      return ::array < class var >::add(var);
   }
   return -1;
}

::count var_array::add(const var_array & vara)
{
   for(int32_t i = 0; i < vara.get_size(); i++)
   {
      add(vara[i]);
   }
   return this->get_count();
}


::count var_array::add_unique(const var_array & vara)
{

   ::count c = 0;

   for(int32_t i = 0; i < vara.get_size(); i++)
   {

      if(!contains(vara[i]))
      {

         add(vara[i]);

         c++;

      }

   }

   return c;

}


string var_array::implode(const char * pszGlue) const
{
   string str;
   for(int32_t i = 0; i < this->get_count(); i++)
   {
      if(i > 0)
         str += pszGlue;
      str += this->element_at(i).get_string();
   }
   return str;
}

index var_array::find_first(const char * psz, index find, index last) const
{
   if(find < 0)
      find += this->get_count();
   if(last < 0)
      last += this->get_count();
   for(; find < this->get_count(); find++)
   {
      if(this->element_at(find).get_string() == psz)
         return find;
   }
   return -1;
}

index var_array::find_first_ci(const char * psz, index find, index last) const
{
   if(find < 0)
      find += this->get_count();
   if(last < 0)
      last += this->get_count();
   for(; find < this->get_count(); find++)
   {
      if(this->element_at(find).get_string().compare_ci(psz) == 0)
         return find;
   }
   return -1;
}


index var_array::find_first(const var & var, index find, index last) const
{
   if(find < 0)
      find += this->get_count();
   if(last < 0)
      last += this->get_count();
   for(; find < this->get_count(); find++)
   {
      if(this->element_at(find) == var)
         return find;
   }
   return -1;
}

bool var_array::contains_ci(const char * lpcsz, index find, index last, ::count countMin, ::count countMax) const
{
   ::count count = 0;
   while((count < countMin || (countMax >= 0 && count <= countMax))
         && (find = find_first_ci(lpcsz, find, last)) >= 0)
      count++;
   return count >= countMin && conditional(countMax >= 0, count <= countMax);
}

bool var_array::contains(const char * lpcsz, index find, index last, ::count countMin, ::count countMax) const
{
   ::count count = 0;
   while((count < countMin || (countMax >= 0 && count <= countMax))
         && (find = find_first(lpcsz, find, last)) >= 0)
      count++;
   return count >= countMin && conditional(countMax >= 0, count <= countMax);
}

bool var_array::contains(const var & var, index find, index last, ::count countMin, ::count countMax) const
{
   ::count count = 0;
   while((count < countMin || (countMax >= 0 && count <= countMax))
         && (find = find_first(var, find, last)) >= 0)
      count++;
   return count >= countMin && conditional(countMax >= 0, count <= countMax);
}

::count var_array::remove_first_ci(const char * lpcsz, index find, index last)
{
   if((find = find_first_ci(lpcsz, find, last)) >= 0)
      remove_at(find);
   return find;
}

::count var_array::remove_first(const char * lpcsz, index find, index last)
{
   if((find = find_first(lpcsz, find, last)) >= 0)
      remove_at(find);
   return find;
}

::count var_array::remove_first(const var & var, index find, index last)
{
   if((find = find_first(var, find, last)) >= 0)
      remove_at(find);
   return find;
}


::count var_array::remove_ci(const char * lpcsz, index find, index last, ::count countMin, ::count countMax)
{
   ::count count = 0;
   if(contains_ci(lpcsz, find, last, countMin, countMax))
      while(conditional(countMax >= 0, count < countMax)
            && (find = remove_first_ci(lpcsz, find, last)) >= 0)
         count++;
   return count;
}


::count var_array::remove(const char * lpcsz, index find, index last, ::count countMin, ::count countMax)
{
   ::count count = 0;
   if(contains(lpcsz, find, last, countMin, countMax))
      while(conditional(countMax >= 0, count < countMax)
            && (find = remove_first(lpcsz, find, last)) >= 0)
         count++;
   return count;
}


::count var_array::remove(const var & var, index find, index last, ::count countMin, ::count countMax)
{
   ::count count = 0;
   if(contains(var, find, last, countMin, countMax))
      while(conditional(countMax >= 0, count < countMax)
            && (find = remove_first(var, find, last)) >= 0)
         count++;
   return count;
}

::count var_array::remove(const var_array & vara)
{
   if(&vara == this)
   {
      return remove_all();
   }
   else
   {
      ::count count = 0;
      for(index i = 0; i < vara.get_count(); i++)
      {
         count += remove(vara[i]);
      }
      return count;
   }
}

var_array & var_array::operator -=(var var)
{
   remove(var);
   return *this;
}

var_array & var_array::operator -=(var_array vara)
{
   remove(vara);
   return *this;
}

var_array var_array::operator -(var var) const
{
   var_array vara(*this);
   vara.remove(var);
   return vara;
}

var_array var_array::operator -(var_array varaParam) const
{
   var_array vara(*this);
   vara.remove(varaParam);
   return vara;
}

var_array & var_array::operator +=(var var)
{
   add(var);
   return *this;
}

var_array & var_array::operator +=(var_array vara)
{
   add(vara);
   return *this;
}

var_array var_array::operator +(var var) const
{
   var_array vara(*this);
   vara.add(var);
   return vara;
}

var_array var_array::operator +(var_array varaParam) const
{
   var_array vara(*this);
   vara.add(varaParam);
   return vara;
}

var_array & var_array::operator = (const stringa & stra)
{
   remove_all();
   for(int32_t i = 0; i < stra.get_count(); i++)
   {
      add(stra[i]);
   }
   return *this;
}

var_array & var_array::operator = (const int_array & inta)
{
   remove_all();
   for(int32_t i = 0; i < inta.get_count(); i++)
   {
      add(inta[i]);
   }
   return *this;
}


var_array & var_array::operator = (const property_set & propset)
{

   remove_all();

   index i = 0;

   for(auto assoc : propset)
   {

      set_at(i++, assoc.m_element2);

   }

   return *this;

}


var_array & var_array::operator = (const var_array & vara)
{
   if(this != &vara)
   {
      remove_all();
      for(int32_t i = 0; i < vara.get_count(); i++)
      {
         add(vara[i]);
      }
   }
   return *this;
}

void var_array::parse_json(const char * & pszJson)
{
   parse_json(pszJson, pszJson + strlen(pszJson) - 1);
}

int g_iRandomNumberGenerator = 0;

void var_array::parse_json(const char * & pszJson, const char * pszEnd)
{
   ::str::consume_spaces(pszJson, 0, pszEnd);
   ::str::consume(pszJson, "[", 1, pszEnd);
   ::str::consume_spaces(pszJson, 0, pszEnd);
   if (*pszJson == ']')
   {
      pszJson++;
      return;
   }
   
   
   ::thread * pthread = NULL;
   while(true)
   {
      g_iRandomNumberGenerator++;
      if(pthread == NULL)
      {
         if((g_iRandomNumberGenerator % 25) == 0)
         {
            pthread = ::get_thread();
         }
      }
      else if(!pthread->thread_get_run())
      {

         _throw(::exit_exception(::get_app(), exit_thread));

      }
      ::var & var = add_new();
      var.parse_json(pszJson, pszEnd);
      ::str::consume_spaces(pszJson, 0, pszEnd);
      if(*pszJson == ',')
      {
         pszJson++;
         continue;
      }
      else if(*pszJson == ']')
      {
         pszJson++;
         return;
      }
      else
      {
         string str = "not expected character : ";
         str += pszJson;
         _throw(simple_exception(get_app(), str));
      }
   }
}


void var_array::skip_json(const char * & pszJson)
{
   skip_json(pszJson, pszJson + strlen(pszJson) - 1);
}

void var_array::skip_json(const char * & pszJson, const char * pszEnd)
{
   ::str::consume_spaces(pszJson, 0, pszEnd);
   ::str::consume(pszJson, "[", 1, pszEnd);
   while (true)
   {
      var::skip_json(pszJson, pszEnd);
      ::str::consume_spaces(pszJson, 0, pszEnd);
      if (*pszJson == ',')
      {
         pszJson++;
         continue;
      }
      else if (*pszJson == ']')
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


void var_array::io(stream & serialize)
{

   serialize.stream_array(*this);

}



string & var_array::get_json(string & str, bool bNewLine) const
{

   str +="[";

   if(get_count() > 0)
   {

      element_at(0).get_json(str, bNewLine);

   }

   for (index i = 1; i < get_count(); i++)
   {

      if (bNewLine)
      {

         str += ", \r\n";

      }
      else
      {

         str += ", ";

      }

      element_at(i).get_json(str, bNewLine);

   }

   if (bNewLine)
   {

      str += "\r\n";

   }

   str += "]";

   return str;

}





