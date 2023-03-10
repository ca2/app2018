#include "framework.h"


void prop_id_debug(::aura::application * papp);


property::property()
{


}


void property::get_value(var & value)
{
   value = get_value();
}

void property::get(var & value)
{
   value = get_value();
}

var & property::get()
{
   return get_value();
}

void property::set(const var & var)
{
   set_value(var);
}

void property::unset()
{
   get_value().unset();
}

string property::operator + (const char * psz) const
{
   return get_value() + psz;
}

string property::operator + (const string & str) const
{

   return get_value() + str;

}



property & property::operator = (const property & prop)
{

   if(&prop != this)
   {

//      m_iIndex = prop.m_iIndex;

      if(m_element1.is_empty())
      {

         output_debug_string("\nHI!! Shouldn't you use a reference find this by use reference and set a breakpoint here!!!\n");
         m_element1      = prop.m_element1;

      }

      m_element2 = prop.m_element2;

   }

   return *this;

}


//property::property(::index iIndex)
//{
//
//   m_iIndex = iIndex;
//
//}
//

property::property(const property & prop)
{

   m_element1 = prop.m_element1;
   m_element2 = prop.m_element2;

}


property::property(id idName)
{

   m_element1 = idName;

}


property::property(id idName, var var)
{

   m_element1 = idName;

   set_value(var);

}

bool property::is_set() const
{
   return get_value().is_set();
}

bool property::is_new() const
{
   return get_value().is_new();
}

bool property::is_null() const
{
   return get_value().is_null();
}

bool property::is_new_or_null() const
{
   return get_value().is_new_or_null();
}

bool property::is_empty() const
{
   return get_value().is_empty();
}

bool property::is_true() const
{
   return get_value().is_true();
}

string property::implode(const char * pszGlue) const
{
   return get_value().implode(pszGlue);
}

void property::parse_json(const string & str)
{
   const char * pszJson = str;
   parse_json(pszJson, str.get_length());
}

void property::parse_json(const char * & pszJson, strsize length)
{
   parse_json(pszJson, pszJson + length - 1);
}

void property::parse_json(const char * & pszJson,const char * pszEnd)
{
   parse_json_id(m_element1, pszJson, pszEnd);
   parse_json_value(m_element2,pszJson,pszEnd);
}

void property::parse_json_id(id & id, const char * & pszJson, const char * pszEnd)
{

   ::str::consume_spaces(pszJson, 0, pszEnd);

   char sz[1024];

   char * psz = sz;

   strsize iBuffer = sizeof(sz);

   ::str::consume_quoted_value_ex2(pszJson, pszEnd, &psz, iBuffer);

   id = psz;

   if (iBuffer > sizeof(sz))
   {

      ::memory_free(psz);

   }

}

void property::parse_json_value(var & var, const char * & pszJson,const char * pszEnd)
{
   ::str::consume_spaces(pszJson,0,pszEnd);
   ::str::consume(pszJson,":",1,pszEnd);
   var.parse_json(pszJson,pszEnd);
}


void property::skip_json_id(const char * & pszJson, const char * pszEnd)
{

   ::str::consume_spaces(pszJson, 0, pszEnd);

   ::str::skip_quoted_value_ex2(pszJson, pszEnd);

}


void property::skip_json_value( const char * & pszJson, const char * pszEnd)
{
   
   ::str::consume_spaces(pszJson, 0, pszEnd);
   
   ::str::consume(pszJson, ":", 1, pszEnd);
   
   ::var::skip_json(pszJson, pszEnd);
   
}



string & property::get_http_post(string & str) const
{

   str += m_element1.str();

   str += "=";

   str += url_encode_dup(m_element2.get_string());

   return str;

}


stringa & property::stra()
{
   return get_value().stra();
}

int_array & property::inta()
{
   return get_value().inta();
}

var_array & property::vara()
{
   return get_value().vara();
}

property_set & property::propset()
{
   return get_value().propset();
}

const stringa & property::stra() const
{
   return get_value().stra();
}

const int_array & property::inta() const
{
   return get_value().inta();
}

const var_array & property::vara() const
{
   return get_value().vara();
}

const property_set & property::propset() const
{
   return get_value().propset();
}


strsize property::get_length() const
{
   return get_value().get_length();
}

void property::get_string(char * psz) const
{
   get_value().get_string(psz);
}

property & property::operator[](id idName)
{
   return propset().operator [](idName);
}

var property::element_at(index iIndex) const
{
   switch(get_value().m_etype)
   {
   case var::type_stra:
      return const_cast<property *>(this)->stra().operator [](iIndex);
   case var::type_inta:
      return const_cast<property *>(this)->inta().operator [](iIndex);
   case var::type_vara:
      return const_cast<property *>(this)->vara().operator [](iIndex);
   case var::type_propset:
      return const_cast<property *>(this)->propset().at(iIndex);
   default:
      if(const_cast<property *>(this)->get_count() == 1)
         return const_cast<property *>(this)->get_value();
      _throw(simple_exception(get_app(), "unsuported!!"));
   }
}

var property::at(index iIndex) const
{
   return this->element_at(iIndex);
}


void property::io(stream & serialize)
{

   serialize(m_element1);
   serialize(m_element2);

}


::count property::get_count() const
{
   return get_value().get_count();
}

::count property::array_get_count() const
{
   return get_value().array_get_count();
}

bool property::array_contains(const char * psz, index first, index last) const
{
   return get_value().array_contains(psz, first, last);
}

bool property::array_contains_ci(const char * psz, index first, index last) const
{
   return get_value().array_contains_ci(psz, first, last);
}


var property::equals_ci_get(const char * pszCompare, var varOnEqual, var varOnDifferent) const
{
   if(compare_value_ci(pszCompare) == 0)
   {
      return varOnEqual;
   }
   else
   {
      return varOnDifferent;
   }
}

var property::equals_ci_get(const char * pszCompare, var varOnEqual) const
{
   if(compare_value_ci(pszCompare) == 0)
   {
      return varOnEqual;
   }
   else
   {
      return var();
   }
}


int32_t property::compare_value_ci(const char * psz) const
{
   return get_value().compare_ci(psz);
}

::comparison::var_strict property::strict_compare() const
{
   return ((const var &)((property *) (this))->get_value());
}

int32_t property::str_compare(const property & prop) const
{
   return get_string().compare(prop.get_string());
}

bool property::strict_equal(const property & prop) const
{
   return get_value().strict_equal(prop.get_value());
}

bool property::strict_equal(const var & var) const
{
   return get_value().strict_equal(var);
}

bool property::strict_equal(const char * psz) const
{
   return get_value().strict_equal(psz);
}

bool property::strict_equal(const string & str) const
{
   return get_value().strict_equal(str);
}

bool property::strict_equal(double d) const
{
   return get_value().strict_equal(d);
}

bool property::strict_equal(int32_t i) const
{
   return get_value().strict_equal(i);
}

bool property::strict_equal(bool b) const
{
   return get_value().strict_equal(b);
}


bool property::strict_different(const property & prop) const
{
   return get_value().strict_different(const_cast < property & > (prop).get_value());
}

bool property::strict_different(const var & var) const
{
   return get_value().strict_different(var);
}

bool property::strict_different(const char * psz) const
{
   return get_value().strict_different(psz);
}

bool property::strict_different(const string & str) const
{
   return get_value().strict_different(str);
}

bool property::strict_different(double d) const
{
   return get_value().strict_different(d);
}

bool property::strict_different(int32_t i) const
{
   return get_value().strict_different(i);
}

bool property::strict_different(bool b) const
{
   return get_value().strict_different(b);
}


bool property::operator == (const property & prop) const
{
   return get_value() == prop.get_value();
}

bool property::operator == (const var & var) const
{
   return get_value() == var;
}

bool property::operator == (const char * psz) const
{
   return get_value() == psz;
}

bool property::operator == (const string & str) const
{
   return get_value() == str;
}

bool property::operator == (int32_t i) const
{
   return get_value() == i;
}

bool property::operator == (bool b) const
{
   return get_value() == b;
}


bool property::operator != (const property & prop) const
{
   return get_value() != prop.get_value();
}

bool property::operator != (const var & var) const
{
   return get_value() != var;
}

bool property::operator != (const char * psz) const
{
   return get_value() != psz;
}

bool property::operator != (const string & str) const
{
   return get_value() != str;
}

bool property::operator != (int32_t i) const
{
   return get_value() != i;
}

bool property::operator != (bool b) const
{
   return get_value() != b;
}

var property::explode(const char * pszSeparator, bool bAddEmpty) const
{
   return get_value().explode(pszSeparator, bAddEmpty);
}



















bool property::operator < (const property & prop) const
{
   return get_value() < prop.get_value();
}

bool property::operator < (const var & var) const
{
   return get_value() < var;
}

bool property::operator < (const char * psz) const
{
   return get_value() < psz;
}

bool property::operator < (const string & str) const
{
   return get_value() < str;
}

bool property::operator < (int32_t i) const
{
   return get_value() < i;
}

bool property::operator < (bool b) const
{
   return get_value() < b;
}

























bool property::operator <= (const property & prop) const
{
   return get_value() <= prop.get_value();
}

bool property::operator <= (const var & var) const
{
   return get_value() <= var;
}

bool property::operator <= (const char * psz) const
{
   return get_value() <= psz;
}

bool property::operator <= (const string & str) const
{
   return get_value() <= str;
}

bool property::operator <= (int32_t i) const
{
   return get_value() <= i;
}

bool property::operator <= (bool b) const
{
   return get_value() <= b;
}
























bool property::operator >= (const property & prop) const
{
   return get_value() >= prop.get_value();
}

bool property::operator >= (const var & var) const
{
   return get_value() >= var;
}

bool property::operator >= (const char * psz) const
{
   return get_value() >= psz;
}

bool property::operator >= (const string & str) const
{
   return get_value() >= str;
}

bool property::operator >= (int32_t i) const
{
   return get_value() >= i;
}

bool property::operator >= (bool b) const
{
   return get_value() >= b;
}
























bool property::operator > (const property & prop) const
{
   return get_value() > prop.get_value();
}

bool property::operator > (const var & var) const
{
   return get_value() > var;
}

bool property::operator > (const char * psz) const
{
   return get_value() > psz;
}

bool property::operator > (const string & str) const
{
   return get_value() > str;
}

bool property::operator > (int32_t i) const
{
   return get_value() > i;
}

bool property::operator > (bool b) const
{
   return get_value() > b;
}




var property::operator - (int32_t i) const
{
   return get_value() - i;
}

var property::operator - (uint32_t user) const
{
   return get_value() - user;
}

var property::operator - (int64_t i) const
{
   return get_value() - i;
}

var property::operator - (uint64_t user) const
{
   return get_value() - user;
}

var property::operator - (double d) const
{
   return get_value() - d;
}

var property::operator - (const var & var) const
{
   return get_value() - var;
}




var property::operator + (int32_t i) const
{
   return get_value() + i;
}

var property::operator + (uint32_t user) const
{
   return get_value() + user;
}

var property::operator + (int64_t i) const
{
   return get_value() + i;
}

var property::operator + (uint64_t user) const
{
   return get_value() + user;
}

var property::operator + (double d) const
{
   return get_value() + d;
}

var property::operator + (const var & var) const
{
   return get_value() + var;
}















































var property::operator / (int32_t i) const
{
   return get_value() / i;
}

var property::operator / (uint32_t user) const
{
   return get_value() / user;
}

var property::operator / (int64_t i) const
{
   return get_value() / i;
}

var property::operator / (uint64_t user) const
{
   return get_value() / user;
}

var property::operator / (double d) const
{
   return get_value() / d;
}

var property::operator / (const var & var) const
{
   return get_value() / var;
}




var property::operator * (int32_t i) const
{
   return get_value() * i;
}

var property::operator * (uint32_t user) const
{
   return get_value() * user;
}

var property::operator * (int64_t i) const
{
   return get_value() * i;
}

var property::operator * (uint64_t user) const
{
   return get_value() * user;
}

var property::operator * (double d) const
{
   return get_value() * d;
}

var property::operator * (const var & var) const
{
   return get_value() * var;
}

















































property & property::operator -= (int32_t i)
{
   operator=(*this - i);
   return *this;
}

property & property::operator -= (uint32_t user)
{
   operator=(*this - user);
   return *this;
}

property & property::operator -= (int64_t i)
{
   operator=(*this - i);
   return *this;
}

property & property::operator -= (uint64_t user)
{
   operator=(*this - user);
   return *this;
}

property & property::operator -= (double d)
{
   operator=(*this - d);
   return *this;
}

property & property::operator -= (const var & var)
{
   operator=(*this - var);
   return *this;
}

property & property::operator -= (const property & prop)
{
   operator=(get_value() - prop.get_value());
   return *this;
}























































property & property::operator += (int32_t i)
{
   operator=(*this + i);
   return *this;
}

property & property::operator += (uint32_t user)
{
   operator=(*this + user);
   return *this;
}

property & property::operator += (int64_t i)
{
   operator=(*this + i);
   return *this;
}

property & property::operator += (uint64_t user)
{
   operator=(*this + user);
   return *this;
}

property & property::operator += (double d)
{
   operator=(*this + d);
   return *this;
}

property & property::operator += (const var & var)
{
   operator=(*this + var);
   return *this;
}

property & property::operator += (const property & prop)
{
   operator=(get_value() + prop.get_value());
   return *this;
}

property & property::operator += (const char * psz)
{
   operator=(*this + psz);
   return *this;
}

property & property::operator += (const string & str)
{
   operator=(*this + str);
   return *this;
}

























property & property::operator /= (int32_t i)
{
   operator=(*this / i);
   return *this;
}

property & property::operator /= (uint32_t user)
{
   operator=(*this / user);
   return *this;
}

property & property::operator /= (int64_t i)
{
   operator=(*this / i);
   return *this;
}

property & property::operator /= (uint64_t user)
{
   operator=(*this / user);
   return *this;
}

property & property::operator /= (double d)
{
   operator=(*this / d);
   return *this;
}

property & property::operator /= (const var & var)
{
   operator=(*this / var);
   return *this;
}

property & property::operator /= (const property & prop)
{
   operator=(get_value() / prop.get_value());
   return *this;
}

























property & property::operator *= (int32_t i)
{
   operator=(*this * i);
   return *this;
}

property & property::operator *= (uint32_t user)
{
   operator=(*this * user);
   return *this;
}

property & property::operator *= (int64_t i)
{
   operator=(*this * i);
   return *this;
}

property & property::operator *= (uint64_t user)
{
   operator=(*this * user);
   return *this;
}

property & property::operator *= (double d)
{
   operator=(*this * d);
   return *this;
}

property & property::operator *= (const var & var)
{
   operator=(*this * var);
   return *this;
}

property & property::operator *= (const property & prop)
{
   operator=(get_value() * prop.get_value());
   return *this;
}











































property_ptra::property_ptra(::aura::application * papp):
   object(papp)
{
   set_size(0, 64);
}




PropertySignalObject::PropertySignalObject()
{
}




string property::get_xml(::xml::disp_option * opt /*= &optDefault*/ )
{
   //   ::file::plain_text_stream ostring;
   //   //ostring << (const char *)m_strName << "='" << (const char *)m_strValue << "' ";

   //   ostring << (const char *)m_strName << L"=" << (CHAR)opt->value_quotation_mark
   //      << (const char *)(opt->reference_value&&opt->m_pentities?opt->m_pentities->entity_to_ref(m_strValue):m_strValue)
   //      << (CHAR)opt->value_quotation_mark << L" ";
   //   return ostring.str();

   if(opt == ((::xml::disp_option *) 1))
   {

      opt = System.xml().m_poptionDefault;

   }

   string str;

   str = name();
   str += "=";
   str += opt->value_quotation_mark;
   string strValue;
   if(opt->reference_value && opt->m_pentities)
      strValue = opt->m_pentities->entity_to_ref(get_string());
   else
      strValue = get_string();

   strValue.replace("\\", "\\\\"); // should be first
   strValue.replace("\n", "\\n");
   strValue.replace("\t", "\\t");
   strValue.replace("\r", "\\r");
   strValue.replace("'", "\\'");
   strValue.replace("\"", "\\\"");

   str += strValue;
   str += opt->value_quotation_mark;
   str += " ";

   return str;
}



void prop_id_debug(::aura::application * papp)
{

   comparable_array < ::id > idaSchema;

   property_set set(papp);

   idaSchema.add(set["prop1"]);


}
















string  operator + (const char * psz, const property & prop)
{
   return psz + prop.get_value();
}


string  operator + (const string & str, const property & prop)
{
   return str + prop.get_value();
}
















var  operator - (int32_t i, const property & prop)
{
   return i - prop.get_value();
}


var  operator - (uint32_t user, const property & prop)
{
   return user - prop.get_value();
}


var  operator - (int64_t l, const property & prop)
{
   return l - prop.get_value();
}


var  operator - (uint64_t ul, const property & prop)
{
   return ul - prop.get_value();
}


var  operator - (double d, const property & prop)
{
   return d - prop.get_value();
}


var  operator - (const var & var, const property & prop)
{
   return var - prop.get_value();
}


var  operator - (const property & prop1, const property & prop2)
{
   return prop1.get_value() - prop2.get_value();
}
















var  operator + (int32_t i, const property & prop)
{
   return i + prop.get_value();
}


var  operator + (uint32_t user, const property & prop)
{
   return user + prop.get_value();
}


var  operator + (int64_t l, const property & prop)
{
   return l + prop.get_value();
}


var  operator + (uint64_t ul, const property & prop)
{
   return ul + prop.get_value();
}


var  operator + (double d, const property & prop)
{
   return d + prop.get_value();
}


var  operator + (const var & var, const property & prop)
{
   return var + prop.get_value();
}


var  operator + (const property & prop1, const property & prop2)
{
   return prop1.get_value() + prop2.get_value();
}
















var  operator / (int32_t i, const property & prop)
{
   return i / prop.get_value();
}


var  operator / (uint32_t user, const property & prop)
{
   return user / prop.get_value();
}


var  operator / (int64_t l, const property & prop)
{
   return l / prop.get_value();
}


var  operator / (uint64_t ul, const property & prop)
{
   return ul / prop.get_value();
}


var  operator / (double d, const property & prop)
{
   return d / prop.get_value();
}


var  operator / (const var & var, const property & prop)
{
   return var / prop.get_value();
}


var  operator / (const property & prop1, const property & prop2)
{
   return prop1.get_value() / prop2.get_value();
}
















var  operator * (int32_t i, const property & prop)
{
   return i * prop.get_value();
}


var  operator * (uint32_t user, const property & prop)
{
   return user * prop.get_value();
}


var  operator * (int64_t l, const property & prop)
{
   return l * prop.get_value();
}


var  operator * (uint64_t ul, const property & prop)
{
   return ul * prop.get_value();
}


var  operator * (double d, const property & prop)
{
   return d * prop.get_value();
}


var  operator * (const var & var, const property & prop)
{
   return var * prop.get_value();
}


var  operator * (const property & prop1, const property & prop2)
{
   return prop1.get_value() * prop2.get_value();
}










































#define ROUND(x,y) (((x)+(y-1))&~(y-1))
#define ROUND4(x) ROUND(x, 4)
#undef new











bool property::operator == (int64_t i) const
{
   return get_value() == i;
}
bool property::operator != (int64_t i) const
{
   return get_value() == i;
}
bool property::operator < (int64_t i) const
{
   return get_value() == i;
}
bool property::operator <= (int64_t i) const
{
   return get_value() == i;
}
bool property::operator >= (int64_t i) const
{
   return get_value() >= i;
}
bool property::operator > (int64_t i) const
{
   return get_value() > i;
}








