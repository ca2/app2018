#pragma once


class stringa;

namespace primitive
{

   class memory;

} // namespace primitive

namespace gen
{
   enum para_return
   {
      e_fail = 0x80000000,
      s_success = 0,
   };

   bool ok(para_return eret);
}

class var_array;

namespace gen
{
   class property;
   class property;
   class property_set;
   class pair_set_interface;
   class str_str_interface;
}


class CLASS_DECL_ca var
{
public:

   enum e_type
   {
      type_parareturn,
      type_new,
      type_null,
      type_empty,
      type_empty_argument,
      type_string,
      type_pstring,
      type_int32,
      type_pint32,
      type_uint32,
      type_puint32,
      type_int64,
      type_pint64,
      type_uint64,
      type_puint64,
      type_pvar,
      type_ca2,
      type_bool,
      type_pbool,
      type_double,
      type_stra,
      type_inta,
      type_vara,
      type_propset,
      type_prop,
      type_memory,
      type_time,
      type_filetime,
      type_uint,
      type_id,
      type_pid,
      type_int64a,
      type_char,
      type_byte,
   };

   void set_type(e_type e_type, bool bConvert = true);
   e_type get_type() const;

   bool is_numeric() const;

   inline bool is_number() const
   {

      return is_numeric();

   }



   inline var();
   inline var(e_type etype);
   var(bool b);
   var(int32_t i);
   var(uint32_t ui);
   var(int64_t i);
   var(uint64_t ui);
   var(int32_t * pi);
   var(uint32_t * pi);
   var(int64_t * pi);
   var(uint64_t * pui);
//   var(long l);
//   var(unsigned long ul);
//   var(long long ll);
//   var(unsigned long long ull);
   var(double d);
   var(const char * psz);
   var(string str);
   var(bool * pb);
   var(const ::datetime::time & time);
   var(const FILETIME & time);
   var(const SYSTEMTIME & time);
   var(string * pstr);
   var(var * pvar);
   var(::ca::ca * pca2);
   var(const stringa & var);
   var(const int_array & var);
   var(const var_array & var);
   var(const gen::property_set & set);
   var(const var & var);
   var(const gen::property & prop);
   var(const gen::pair_set_interface & set);
   var(const gen::str_str_interface & set);
   var(const string_composite & composite);
   var(const ::ca::null & null);   var(const id & id);
   ~var();

   e_type             m_etype;
   union
   {
      gen::para_return     m_parareturn;
      int32_t              m_i32;
      uint32_t             m_ui32;
      int64_t              m_i64;
      uint64_t             m_ui64;
      int32_t *            m_pi32;
      uint32_t *           m_pui32;
      int64_t *            m_pi64;
      uint64_t *           m_pui64;
      bool *               m_pb;
      string *             m_pstr;
      bool                 m_b;
      double               m_d;
      var *                m_pvar;
      __time64_t           m_time;
      FILETIME             m_filetime;
      id *                 m_pid;
      int_array *          m_pia;
      stringa *            m_pstra;
      int64_array *        m_pia64;
      var_array *          m_pvara;
      gen::property_set *  m_pset;
      gen::property *      m_pprop;
      unsigned char        m_uch;
      char                 m_ch;
   };
   ::ca::ca *                       m_pca2;
   string                           m_str;
   id                               m_id;

   string                           to_r_string() const;
   string                           get_string(const char * pszOnNull = NULL) const;
   string &                         get_ref_string(const char * pszOnNull = NULL);
   id                               get_id(const char * pszOnNull = NULL)   const;
   id &                             get_ref_id(const char * pszOnNull = NULL);
   int32_t                          int32(int32_t iDefault = 0)  const;
   unsigned long                    get_ulong()    const;
   double                           get_double()   const;
   bool                             get_bool()     const;
   class primitive::memory &        memory();
   stringa &                        stra();
   int_array &                      inta();
   int64_array &                    int64a();
   var_array &                      vara();
   gen::property_set &              propset(::ca::application * papp = NULL);
   gen::property &                  prop();
   const class primitive::memory &  memory() const;
   stringa                          stra() const;
   int_array                        inta() const;
   int64_array                      int64a() const;
   var_array                        vara()  const;
   gen::property_set                propset() const;
   gen::property                prop() const;

   bool is_scalar() const;
   inline bool is_array() const;
   bool is_double() const;
   bool is_integer() const;
   bool is_ulong() const;

   bool ok() const;
   bool failed() const;


   var dereference();

   void on_delete(::ca::ca * poc);


   variable_strict_compare strict_compare() const;


   void           set_string(const char * psz);
   void           set_id(const id & id);
   void unset();

   bool is_true() const;


   bool is_set() const;
   bool is_new() const;
   bool is_null() const;
   bool is_new_or_null() const;
   bool is_empty() const;
   bool has_char() const;

   bool has_property(const char * pszName) const;

   var & operator ++(int32_t);

   operator bool() const
   {
      return m_etype == type_bool && m_b;
   }

   operator int32_t() const;
#ifdef WINDOWS
   operator LONG() const;
#endif
   operator uint32_t() const;
#ifdef WINDOWS
   operator DWORD() const;
#endif
   operator int64_t() const;
   operator uint64_t() const;

   //operator string & ();
   //operator id &();
   operator const char * () const;
   //operator ::file_time & ();

   operator ::datetime::file_time () const;

   operator ::datetime::time() const;

   void get_string(char * psz) const;

   strsize get_length() const;

   var & operator = (::ca::ca * pca2);
   var & operator = (gen::para_return & eret);
   var & operator = (bool b);
   var & operator = (bool * pb);
   var & operator = (int32_t i);
   var & operator = (int32_t * pi);
   var & operator = (uint32_t ui);
   var & operator = (uint32_t * pui);
#ifdef WINDOWS
   var & operator = (DWORD ui);
   var & operator = (LPDWORD pui);
#endif
   var & operator = (int64_t i);
   var & operator = (int64_t * pi);
   var & operator = (uint64_t i);
   var & operator = (uint64_t * pi);
   var & operator = (const ::datetime::time & time);
   var & operator = (const FILETIME & time);
   var & operator = (const SYSTEMTIME & time);
   var & operator = (double d);
   var & operator = (string str);
   var & operator = (string * pstr);
   var & operator = (var * pvar);
   var & operator = (const char * psz);
   var & operator = (const wchar_t * lpcsz);
   var & operator = (const gen::property & prop);
   var & operator = (const var & var);
   var & operator = (const int_array & ia);
   var & operator = (const stringa & stra);
   var & operator = (const class primitive::memory & memory);
   var & operator = (const var_array & vara);
   var & operator = (const gen::property_set & propset);
   var & operator = (const gen::pair_set_interface & propset);
   var & operator = (const gen::str_str_interface & propset);
   var & operator = (const string_composite & composite);
   var & operator = (const id & id);
   var & operator = (id * pid);

   template < class T >
   T * ca2()
   {
      if(m_etype == type_pvar && m_pvar != NULL)
         return m_pvar->ca2 < T > ();
      if(m_etype != type_ca2 || m_pca2 == NULL)
         return NULL;
      return dynamic_cast < T * > (m_pca2);
   }

   template < class T >
   const T * ca2() const
   {
      if(m_etype != type_ca2 || m_pca2 == NULL)
         return NULL;
      return dynamic_cast < T * > (m_pca2);
   }

   bool strict_equal(const var & var) const;
   bool strict_equal(const char * psz) const;
   bool strict_equal(const string & str) const;
   bool strict_equal(double d) const;
   bool strict_equal(int32_t i) const;
   bool strict_equal(bool b) const;

   bool strict_different(const var & var) const;
   bool strict_different(const char * psz) const;
   bool strict_different(const string & str) const;
   bool strict_different(double d) const;
   bool strict_different(int32_t i) const;
   bool strict_different(bool b) const;

   friend bool CLASS_DECL_ca strict_equal(const char * psz, const var & var);
   friend bool CLASS_DECL_ca strict_equal(const string & str, const var & var);
   friend bool CLASS_DECL_ca strict_equal(double d, const var & var);
   friend bool CLASS_DECL_ca strict_equal(int32_t i, const var & var);
   friend bool CLASS_DECL_ca strict_equal(bool b, const var & var);

   friend bool CLASS_DECL_ca strict_different(const char * psz, const var & var);
   friend bool CLASS_DECL_ca strict_different(const string & str, const var & var);
   friend bool CLASS_DECL_ca strict_different(double d, const var & var);
   friend bool CLASS_DECL_ca strict_different(int32_t i, const var & var);
   friend bool CLASS_DECL_ca strict_different(bool b, const var & var);

   int32_t compare(const var & var) const;
   int32_t compare(const char * psz) const;

   int32_t compare_ci(const var & var) const;
   int32_t compare_ci(const char * psz) const;

   bool operator == (const var & var) const;
   bool operator == (const char * psz) const;
   bool operator == (const string & str) const;
   bool operator == (int32_t i) const;
   bool operator == (bool b) const;

   bool operator != (const var & var) const;
   bool operator != (const char * psz) const;
   bool operator != (const string & str) const;
   bool operator != (int32_t i) const;
   bool operator != (bool b) const;

   bool operator < (const var & var) const;
   bool operator < (const char * psz) const;
   bool operator < (const string & str) const;
   bool operator < (int32_t i) const;
   bool operator < (bool b) const;

   bool operator <= (const var & var) const;
   bool operator <= (const char * psz) const;
   bool operator <= (const string & str) const;
   bool operator <= (int32_t i) const;
   bool operator <= (bool b) const;

   bool operator >= (const var & var) const;
   bool operator >= (const char * psz) const;
   bool operator >= (const string & str) const;
   bool operator >= (int32_t i) const;
   bool operator >= (bool b) const;

   bool operator > (const var & var) const;
   bool operator > (const char * psz) const;
   bool operator > (const string & str) const;
   bool operator > (int32_t i) const;
   bool operator > (bool b) const;

   void write(ex1::byte_output_stream & ostream);
   void read(ex1::byte_input_stream & ostream);

   string implode(const char * pszGlue) const;
   var explode(const char * pszGlue, bool bAddEmpty = true) const;

   var first() const;
   var last() const;
   inline count get_count() const;
   const var & operator[] (var varKey) const;
   const var & operator[] (const char * pszKey) const;
   const var & operator[] (index iKey) const;
#if defined(AMD64) && defined(WINDOWS)
   inline const var & operator[] (int32_t iKey) const { return operator[]((index) iKey); }
#endif
   var & operator[] (var varKey);
   var & operator[] (const char * pszKey);
   var & operator[] (index iKey);
#if defined(AMD64) && defined(WINDOWS)
   inline var & operator[] (int32_t iKey) { return operator[]((index) iKey); }
#endif
   var at(index i) const;
#if defined(AMD64) && defined(WINDOWS)
   inline var at(int32_t i) const { return at((index) i); }
#endif
   var at(index i);
#if defined(AMD64) && defined(WINDOWS)
   inline var at(int32_t i) { return at((index) i); }
#endif
   var key(index i) const;
#if defined(AMD64) && defined(WINDOWS)
   inline var key(int32_t i) const { return key((index) i); }
#endif
   inline count array_get_count() const;
   inline index array_get_upper_bound() const;
   bool array_contains(const char * psz, index find = 0, count count = -1) const;
   bool array_contains_ci(const char * psz, index find = 0, count count = -1) const;

   var equals_ci_get(const char * pszCompare, var varOnEqual, var varOnDifferent) const;
   var equals_ci_get(const char * pszCompare, var varOnEqual) const;



   var operator - (int32_t i) const;
   var operator - (uint32_t user) const;
   var operator - (long i) const;
   var operator - (unsigned long user) const;
   var operator - (double d) const;

   friend var CLASS_DECL_ca operator - (int32_t i, const var & var);
   friend var CLASS_DECL_ca operator - (uint32_t user, const var & var);
   friend var CLASS_DECL_ca operator - (long l, const var & var);
   friend var CLASS_DECL_ca operator - (unsigned long ul, const var & var);
   friend var CLASS_DECL_ca operator - (double d, const var & var);
   friend var CLASS_DECL_ca operator - (const var & var1, const var & var2);

   var operator + (int32_t i) const;
   var operator + (uint32_t user) const;
   var operator + (long i) const;
   var operator + (unsigned long user) const;
   var operator + (double d) const;

   friend var CLASS_DECL_ca operator + (int32_t i, const var & var);
   friend var CLASS_DECL_ca operator + (uint32_t user, const var & var);
   friend var CLASS_DECL_ca operator + (long l, const var & var);
   friend var CLASS_DECL_ca operator + (unsigned long ul, const var & var);
   friend var CLASS_DECL_ca operator + (double d, const var & var);
   friend var CLASS_DECL_ca operator + (const var & var1, const var & var2);

   var operator / (int32_t i) const;
   var operator / (uint32_t user) const;
   var operator / (long i) const;
   var operator / (unsigned long user) const;
   var operator / (double d) const;

   friend var CLASS_DECL_ca operator / (int32_t i, const var & var);
   friend var CLASS_DECL_ca operator / (uint32_t user, const var & var);
   friend var CLASS_DECL_ca operator / (long l, const var & var);
   friend var CLASS_DECL_ca operator / (unsigned long ul, const var & var);
   friend var CLASS_DECL_ca operator / (double d, const var & var);
   friend var CLASS_DECL_ca operator / (const var & var1, const var & var2);

   var operator * (int32_t i) const;
   var operator * (uint32_t user) const;
   var operator * (long i) const;
   var operator * (unsigned long user) const;
   var operator * (double d) const;

   friend var CLASS_DECL_ca operator * (int32_t i, const var & var);
   friend var CLASS_DECL_ca operator * (uint32_t user, const var & var);
   friend var CLASS_DECL_ca operator * (long l, const var & var);
   friend var CLASS_DECL_ca operator * (unsigned long ul, const var & var);
   friend var CLASS_DECL_ca operator * (double d, const var & var);
   friend var CLASS_DECL_ca operator * (const var & var1, const var & var2);

   var & operator -= (int32_t i);
   var & operator -= (uint32_t user);
   var & operator -= (long i);
   var & operator -= (unsigned long user);
   var & operator -= (double d);
   var & operator -= (const var & var);

   var & operator += (int32_t i);
   var & operator += (uint32_t user);
   var & operator += (long i);
   var & operator += (unsigned long user);
   var & operator += (double d);
   var & operator += (const var & var);

   var & operator /= (int32_t i);
   var & operator /= (uint32_t user);
   var & operator /= (long i);
   var & operator /= (unsigned long user);
   var & operator /= (double d);
   var & operator /= (const var & var);

   var & operator *= (int32_t i);
   var & operator *= (uint32_t user);
   var & operator *= (long i);
   var & operator *= (unsigned long user);
   var & operator *= (double d);
   var & operator *= (const var & var);

   void consume_number(const char * & psz);
   void consume_number(const char * & psz, const char * pszEnd);
   void parse_json(const char * & pszJson);
   void parse_json(const char * & pszJson, const char * pszEnd);




#undef new
#undef delete

   void * PASCAL operator new(size_t nSize);
   void * PASCAL operator new(size_t, void * p);
   void PASCAL operator delete(void * p);
   void PASCAL operator delete(void * p, void * pPlace);

   // for file name/line number tracking using DEBUG_NEW
   void * PASCAL operator new(size_t nSize, const char * lpszFileName, int32_t nLine);
   void PASCAL operator delete(void *p, const char * lpszFileName, int32_t nLine);

#define new DEBUG_NEW


};

namespace gen
{


   namespace str
   {


      inline CLASS_DECL_ca string from(const var & var)
      {

         return var.get_string();

      }


   } // namespace str


} // namespace gen


inline string CLASS_DECL_ca operator+ (const char * psz, const var & var)
{

   string strResult(psz);

   strResult += var.get_string();

   return strResult;
}


inline string CLASS_DECL_ca operator+ (const var & var, const char * psz)
{

   string strResult(var.get_string());

   strResult += psz;

   return strResult;
}


inline string CLASS_DECL_ca operator+ (const string & str, const var & var)
{

   string strResult(str);

   strResult += var.get_string();

   return strResult;
}


inline string CLASS_DECL_ca operator+ (const var & var, const string & str)
{

   string strResult(var.get_string());

   strResult += str;

   return strResult;
}

#ifdef WINDOWS

inline var::operator LONG () const
{

	return operator int32_t();

}

inline var::operator DWORD () const
{

	return operator uint32_t();

}

inline var & var::operator = (DWORD ui)
{

	return operator = ((uint32_t) ui);

}


inline var & var::operator = (LPDWORD pui)
{

	return operator = ((uint32_t *) pui);

}

#endif


