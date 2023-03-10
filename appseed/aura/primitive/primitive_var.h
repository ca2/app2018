#pragma once



enum para_return
{
   e_fail = 0x80000000,
   s_success = 0
};

bool is_return_ok(para_return eret);

template < typename ENUM >
ENUM enum_default()
{
   return (ENUM)0;
}


class CLASS_DECL_AURA var
{
public:

   enum e_type
   {
      type_parareturn,
      type_new,
      type_null,
      type_key_exists,
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
      type_float,
      type_double,
      type_pvar,
      type_element,
      type_bool,
      type_pbool,
      type_stra,
      type_inta,
      type_vara,
      type_propset,
      type_prop,
      type_memory,
      type_time,
      type_filetime,
      type_id,
      type_pid,
      type_int64a,
      type_char,
      type_byte,
      type_duration,
      type_pduration
   };

   e_type             m_etype;
   union
   {
      void               * m_p;
      para_return          m_parareturn;
      bool                 m_b;
      bool *               m_pb;
      int32_t              m_i32;
      uint32_t             m_ui32;
      int64_t              m_i64;
      uint64_t             m_ui64;
      int32_t *            m_pi32;
      uint32_t *           m_pui32;
      int64_t *            m_pi64;
      uint64_t *           m_pui64;
      string *             m_pstr;
      float                m_f;
      double               m_d;
      var *                m_pvar;
      __time64_t           m_time;
      FILETIME             m_filetime;
      id *                 m_pid;
      int_array *          m_pia;
      stringa *            m_pstra;
      int64_array *        m_pia64;
      var_array *          m_pvara;
      property_set *       m_pset;
      property *           m_pprop;
      uchar                m_uch;
      char                 m_ch;
      class duration       m_duration;
      class duration *     m_pduration;
   };


   sp(object)                      m_sp;
   string                           m_str;
   id                               m_id;


   inline var();
   inline var(e_type etype);
   var(bool b);
   var(int32_t i);
   var(uint32_t ui);
   var(int64_t i);
   var(uint64_t ui);
#ifdef APPLEOS
#ifdef OS64BIT
   var(long l);
#endif
#endif
   var(int32_t * pi);
   var(uint32_t * pi);
   var(int64_t * pi);
   var(uint64_t * pui);
   //   var(int64_t l);
   //   var(uint64_t ul);
   //   var(int64_t int64_t ll);
   //   var(uint64_t int64_t ull);
   var(float f);
   var(double d);
   var(const char * psz);
   var(string str);
   var(const id & id);
   var(bool * pb);
   var(const ::datetime::time & time);
   var(const FILETIME & time);
   var(const SYSTEMTIME & time);
   var(string * pstr);
   var(var * pvar);
   var(const var * pvar);
   var(object * pobject);
   var(const stringa & var);
   var(const int_array & var);
   var(const var_array & var);
   var(const property_set & set);
   var(const var & var);
   var(const property & prop);
   //var(const pair_set_interface & set);
   //var(const str_str_interface & set);
//   var(const string_composite & composite);
   var(const class duration & duration);
   var(class duration * pduration);
   //#if defined(MOVE_SEMANTICS)
   // inline var(var && v);
   //#endif
   template < class T >
   var(const sp(T) & sp)
   {
      m_etype = type_new;
      operator = (sp.m_p);
   }

   ~var();

   void set_type(e_type e_type,bool bConvert = true);
   e_type get_type() const;

   bool is_numeric() const;

   inline bool is_number() const
   {

      return is_numeric();

   }




   bool                             get_bool(bool bDefault = false)     const;
   int32_t                          i32(int32_t iDefault = 0)  const;
   template < typename ENUM >
   ENUM                             e(ENUM edefault = enum_default < ENUM >())  const { return (ENUM)i64(); }
   uint32_t                         u32(uint32_t uiDefault = 0)  const;
   int64_t                          i64(int64_t iDefault = 0)  const;
   uint64_t                         uint64(uint64_t uiDefault = 0)  const;
   inline int_ptr                   intptr(int_ptr iDefault = 0)  const;
   inline uint_ptr                  uintptr(uint_ptr uiDefault = 0)  const;
   float                            get_float(float fDefault = 0.f)   const;
   double                           get_double(double dDefault = 0.0)   const;
   string                           to_r_string() const;
   string                           get_string(const char * pszOnNull = NULL) const;
   string &                         get_ref_string(const char * pszOnNull = NULL);
   const string &                   get_ref_string(const char * pszOnNull = NULL) const;
   string &                         to_string(string & str) const;
   string                           to_string() const;
   id                               get_id(const char * pszOnNull = NULL)   const;
   id &                             get_ref_id(const char * pszOnNull = NULL);
   class memory &        memory();
   stringa &                        stra();
   int_array &                      inta();
   int64_array &                    int64a();
   var_array &                      vara();
   class duration &                 duration();
   property_set &                   propset(::aura::application * papp = NULL);
   property &                       prop();
   const class memory &  memory() const;
   const stringa &                  stra() const;
   const int_array &                inta() const;
   const int64_array &              int64a() const;
   const var_array &                vara()  const;
   const property_set &             propset() const;
   const property &                 prop() const;
   const class duration &           duration() const;

   bool is_scalar() const;
   inline bool is_array() const;
   bool is_real() const;
   bool is_integer() const;
   bool is_natural() const;

   bool is_double() const;
   bool is_duration() const;

   bool ok() const;
   bool failed() const;


   var dereference();


   ::comparison::var_strict strict_compare() const;

   void           set_string(const char * psz);
   void           set_string(const string & str);
   void           set_string(string && str);
   void           set_id(const id & id);
   void unset();
   void unset(const string & strPropertySetKey);

   bool is_set() const;
   bool is_new() const;
   bool is_null() const;
   bool is_new_or_null() const;
   bool is_empty() const;
   bool is_false() const;
   bool is_true(bool bDefault = false) const;
   bool has_char() const;

   bool has_property(const char * pszName) const;
   bool is_property_true(const char * pszName) const;
   bool is_property_false(const char * pszName) const;

   var & operator ++(int32_t);

   operator bool() const;

   operator int32_t() const;
#ifdef WINDOWS
   operator LONG() const;
#endif
   operator uint32_t() const;
   operator int64_t() const;
   operator uint64_t() const;
   operator double() const;
   operator class duration() const;

   //operator string & ();
   //operator id &();
   operator const char * () const;
   //operator ::file_time & ();

   operator string & ()
   {

      if(m_etype == var::type_pstring)
      {

         return *m_pstr;

      }
      else if(m_etype != var::type_string)
      {

         m_str = get_string();

      }

      return m_str;

   }

//   operator ::file::path() const { return get_file_path(); }

   operator ::datetime::file_time() const;

   operator ::datetime::time() const;

   void get_string(char * psz) const;

   strsize get_length() const;

   /*   template < class T >
   var & operator = (sp(T) p)
   {
   set_type(type_element, false);
   m_sp = p;
   return *this;
   }
   */
   inline var & operator = (object * p)
   {
      set_type(type_element,false);
      m_sp = p;
      return *this;
   }

   var & operator = (para_return & eret);
   var & operator = (bool b);
   var & operator = (bool * pb);
   var & operator = (int32_t i);
   var & operator = (int32_t * pi);
   var & operator = (uint32_t ui);
   var & operator = (uint32_t * pui);
#ifdef WINDOWS
   var & operator = (LPDWORD pui);
   var & operator = (LONG pui);
   var & operator = (DWORD dw);
#endif
   var & operator = (int64_t i);
   var & operator = (int64_t * pi);
   var & operator = (uint64_t i);
   var & operator = (uint64_t * pi);
   var & operator = (float f);
   var & operator = (double d);
   var & operator = (const ::datetime::time & time);
   var & operator = (const FILETIME & time);
   var & operator = (const SYSTEMTIME & time);
   inline var & operator = (const char * psz);
   inline var & operator = (const string & str);
   inline var & operator = (string && str);
   var & operator = (string * pstr);
   var & operator = (var * pvar);
   var & operator = (const var * pvar);
   var & operator = (const unichar * lpcsz);
   var & operator = (const property & prop);
   var & operator = (const var & var);
   var & operator = (const int_array & ia);
   var & operator = (const stringa & stra);
   var & operator = (const class memory & memory);
   var & operator = (const var_array & vara);
   var & operator = (const property_set & propset);
   //var & operator = (const pair_set_interface & propset);
   //var & operator = (const str_str_interface & propset);
//   var & operator = (const string_composite & composite);
   var & operator = (const id & id);
   var & operator = (id * pid);
   var & operator = (const class duration & pid);
   var & operator = (class duration * pduration);
   //#ifdef MOVE_SEMANTICS
   //inline var & operator = (var && v);
   //#endif


   template < class T >
   void get_array(T & dsta) const
   {

      for(int i = 0; i < array_get_count(); i++)
      {

         dsta.add((typename T::BASE_TYPE) at(i));

      }

   }

   template < class T >
   var & operator = (const sp(T) & sp)
   {

      return this->operator = (sp.m_p);

   }

   template < class T >
   sp(T) cast(T * pDefault)
   {

      if(m_etype == type_pvar && m_pvar != NULL)
         return m_pvar->cast < T >(pDefault);

      if(m_etype != type_element)
         return NULL;

      sp(T) p = m_sp;

      if(p.is_null())
         return pDefault;

      return p;

   }


   template < class T >
   sp(T) cast(T * pDefault) const
   {
      return ((var *) this)->cast < T >(pDefault);
   }


   template < class T >
   T * cast()
   {

      if(m_etype == type_pvar && m_pvar != NULL)
         return m_pvar->cast < T >();

      if(m_etype != type_element)
         return NULL;

      return m_sp.cast < T >();


   }


   template < class T >
   T * cast() const
   {
      return ((var *) this)->cast < T >();
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

   friend bool CLASS_DECL_AURA strict_equal(const char * psz,const var & var);
   friend bool CLASS_DECL_AURA strict_equal(const string & str,const var & var);
   friend bool CLASS_DECL_AURA strict_equal(double d,const var & var);
   friend bool CLASS_DECL_AURA strict_equal(int32_t i,const var & var);
   friend bool CLASS_DECL_AURA strict_equal(bool b,const var & var);

   friend bool CLASS_DECL_AURA strict_different(const char * psz,const var & var);
   friend bool CLASS_DECL_AURA strict_different(const string & str,const var & var);
   friend bool CLASS_DECL_AURA strict_different(double d,const var & var);
   friend bool CLASS_DECL_AURA strict_different(int32_t i,const var & var);
   friend bool CLASS_DECL_AURA strict_different(bool b,const var & var);

   int32_t compare(const var & var) const;
   int32_t compare(const char * psz) const;

   int32_t compare_ci(const var & var) const;
   int32_t compare_ci(const char * psz) const;

   bool operator == (const var & var) const;
   bool operator == (const char * psz) const;
   bool operator == (const string & str) const;
   bool operator == (int64_t i) const;
   bool operator == (int32_t i) const;
   bool operator == (bool b) const;

   bool operator != (const var & var) const;
   bool operator != (const char * psz) const;
   bool operator != (const string & str) const;
   bool operator != (int64_t i) const;
   bool operator != (int32_t i) const;
   bool operator != (bool b) const;

   bool operator < (const var & var) const;
   bool operator < (const char * psz) const;
   bool operator < (const string & str) const;
   bool operator < (int64_t i) const;
   bool operator < (int32_t i) const;
   bool operator < (bool b) const;

   bool operator <= (const var & var) const;
   bool operator <= (const char * psz) const;
   bool operator <= (const string & str) const;
   bool operator <= (int64_t i) const;
   bool operator <= (int32_t i) const;
   bool operator <= (bool b) const;

   bool operator >= (const var & var) const;
   bool operator >= (const char * psz) const;
   bool operator >= (const string & str) const;
   bool operator >= (int64_t i) const;
   bool operator >= (int32_t i) const;
   bool operator >= (bool b) const;

   bool operator > (const var & var) const;
   bool operator > (const char * psz) const;
   bool operator > (const string & str) const;
   bool operator > (int64_t i) const;
   bool operator > (int32_t i) const;
   bool operator > (bool b) const;

   void io(stream & serialize);

   string implode(const char * pszGlue) const;
   var explode(const char * pszGlue,bool bAddEmpty = true) const;

   var first() const;
   var last() const;
   var first();
   var last();
   inline ::count get_count() const;
   var operator[] (const var & varKey) const;
   const var & operator[] (const string & strKey) const;
   const var & operator[] (const char * pszKey) const;
   var operator[] (const id & idKey) const;
   var operator[] (index iKey) const;
   var operator[] (const var & varKey);
   var & operator[] (const string & strKey);
   var & operator[] (const char * pszKey);
   var operator[] (const id & idKey);
   var operator[] (index iKey);
   var at(index i) const;
   var at(index i);
   var key(index i) const;
#if OSBIT != 32
   inline var operator[] (int32_t iKey) const { return operator[]((index)iKey); }
   inline var operator[] (int32_t iKey) { return operator[]((index)iKey); }
   inline var at(int32_t i) const { return at((index)i); }
   inline var at(int32_t i) { return at((index)i); }
   inline var key(int32_t i) const { return key((index)i); }
#endif
   inline ::count array_get_count() const;
   inline index array_get_upper_bound() const;
   bool array_contains(const char * psz,index find = 0,::count count = -1) const;
   bool array_contains_ci(const char * psz,index find = 0,::count count = -1) const;

   var equals_ci_get(const char * pszCompare,var varOnEqual,var varOnDifferent) const;
   var equals_ci_get(const char * pszCompare,var varOnEqual) const;


   var operator - (int32_t i) const;
   var operator - (uint32_t ui) const;
   var operator - (int64_t i) const;
   var operator - (uint64_t ui) const;
   var operator - (double d) const;

   friend var CLASS_DECL_AURA operator - (int32_t i,const var & var);
   friend var CLASS_DECL_AURA operator - (uint32_t ui,const var & var);
   friend var CLASS_DECL_AURA operator - (int64_t l,const var & var);
   friend var CLASS_DECL_AURA operator - (uint64_t ul,const var & var);
   friend var CLASS_DECL_AURA operator - (double d,const var & var);
   friend var CLASS_DECL_AURA operator - (const var & var1,const var & var2);

   var operator + (int32_t i) const;
   var operator + (uint32_t ui) const;
   var operator + (int64_t i) const;
   var operator + (uint64_t ui) const;
   var operator + (double d) const;

   friend var CLASS_DECL_AURA operator + (int32_t i,const var & var);
   friend var CLASS_DECL_AURA operator + (uint32_t ui,const var & var);
   friend var CLASS_DECL_AURA operator + (int64_t l,const var & var);
   friend var CLASS_DECL_AURA operator + (uint64_t ul,const var & var);
   friend var CLASS_DECL_AURA operator + (double d,const var & var);
   friend var CLASS_DECL_AURA operator + (const var & var1,const var & var2);

   var operator / (int32_t i) const;
   var operator / (uint32_t ui) const;
   var operator / (int64_t i) const;
   var operator / (uint64_t ui) const;
   var operator / (double d) const;

   friend var CLASS_DECL_AURA operator / (int32_t i,const var & var);
   friend var CLASS_DECL_AURA operator / (uint32_t ui,const var & var);
   friend var CLASS_DECL_AURA operator / (int64_t l,const var & var);
   friend var CLASS_DECL_AURA operator / (uint64_t ul,const var & var);
   friend var CLASS_DECL_AURA operator / (double d,const var & var);
   friend var CLASS_DECL_AURA operator / (const var & var1,const var & var2);

   var operator * (int32_t i) const;
   var operator * (uint32_t ui) const;
   var operator * (int64_t i) const;
   var operator * (uint64_t ui) const;
   var operator * (double d) const;

   friend var CLASS_DECL_AURA operator * (int32_t i,const var & var);
   friend var CLASS_DECL_AURA operator * (uint32_t ui,const var & var);
   friend var CLASS_DECL_AURA operator * (int64_t l,const var & var);
   friend var CLASS_DECL_AURA operator * (uint64_t ul,const var & var);
   friend var CLASS_DECL_AURA operator * (double d,const var & var);
   friend var CLASS_DECL_AURA operator * (const var & var1,const var & var2);

   var & operator -= (int32_t i);
   var & operator -= (uint32_t ui);
   var & operator -= (int64_t i);
   var & operator -= (uint64_t ui);
   var & operator -= (double d);
   var & operator -= (const var & var);

   var & operator += (int32_t i);
   var & operator += (uint32_t ui);
   var & operator += (int64_t i);
   var & operator += (uint64_t ui);
   var & operator += (double d);
   var & operator += (const var & var);

   var & operator /= (int32_t i);
   var & operator /= (uint32_t ui);
   var & operator /= (int64_t i);
   var & operator /= (uint64_t ui);
   var & operator /= (double d);
   var & operator /= (const var & var);

   var & operator *= (int32_t i);
   var & operator *= (uint32_t ui);
   var & operator *= (int64_t i);
   var & operator *= (uint64_t ui);
   var & operator *= (double d);
   var & operator *= (const var & var);

   void consume_number(const char * & psz);
   void consume_number(const char * & psz,const char * pszEnd);
   void consume_identifier(const char * & psz);
   void consume_identifier(const char * & psz,const char * pszEnd);
   void parse_json(const char * & pszJson);
   void parse_json(const char * & pszJson, const char * pszEnd);
   const char * parse_json(const string & strJson);
   ::var::e_type find_json_child(const char * & pszJson, const var & var);
   ::var::e_type find_json_child(const char * & pszJson, const char * pszEnd, const var & var);
   ::var::e_type find_json_id(const char * & pszJson, const char * pszEnd, const var & var);
   bool parse_json_step(const char * & pszJson);
   bool parse_json_step(const char * & pszJson, const char * pszEnd);

   string & get_json(string & str, bool bNewLine = true) const;
   string get_json(bool bNewLine = true) const;

   static void skip_number(const char * & psz);
   static void skip_number(const char * & psz, const char * pszEnd);
   static void skip_identifier(const char * & psz);
   static void skip_identifier(const char * & psz, const char * pszEnd);
   static void skip_json(const char * & pszJson);
   static void skip_json(const char * & pszJson, const char * pszEnd);

   //#undef new
   // DECLARE_AXIS_FIXED_ALLOC(var)

   void null();

   ::file::path get_file_path() const;

   void _001Add(const stringa & stra);

};


namespace str
{


   inline CLASS_DECL_AURA string from(const var & var)
   {

      return var.get_string();

   }


} // namespace str


inline string CLASS_DECL_AURA operator+ (const char * psz,const var & var)
{

   string strResult(psz);

   strResult += var.get_string();

   return strResult;
}


inline string CLASS_DECL_AURA operator+ (const var & var,const char * psz)
{

   string strResult(var.get_string());

   strResult += psz;

   return strResult;
}


inline string CLASS_DECL_AURA operator+ (const string & str,const var & var)
{

   string strResult(str);

   strResult += var.get_string();

   return strResult;
}


inline string CLASS_DECL_AURA operator+ (const var & var,const string & str)
{

   string strResult(var.get_string());

   strResult += str;

   return strResult;
}


inline bool var::get_bool(bool bDefault) const
{
   return is_true(bDefault);
}

// returns 0 for unknown conversions
inline var::operator int32_t() const
{
   return i32();
}

// returns 0 for unknown conversions
inline var::operator uint32_t() const
{
   return u32();
}

// returns 0 for unknown conversions
inline var::operator int64_t() const
{
   return i64();
}

// returns 0 for unknown conversions
inline var::operator uint64_t() const
{
   return uint64();
}


// returns 0.0 for unknown conversions?
inline var::operator double() const
{
   return get_double();
}


inline var::operator class duration() const
{
   return duration();
}

inline int_ptr var::intptr(int_ptr iDefault) const
{

#if defined(OS64BIT)

   return i64(iDefault);

#else

   return i32(iDefault);

#endif

}

inline uint_ptr var::uintptr(uint_ptr uiDefault) const
{

#ifdef OS64BIT

   return uint64(uiDefault);

#else

   return u32(uiDefault);

#endif

}


#ifdef WINDOWS

inline var::operator LONG () const
{

   return operator int32_t();

}

inline var & var::operator = (LPDWORD pui)
{

   return operator = ((uint32_t *)pui);

}

#endif


inline string & var::to_string(string & str) const
{

   return str = get_string();

}

inline string var::to_string() const
{

   return get_string();

}

//inline string::string(const var & var) :
//stdstring<simple_string>(string_trait::GetDefaultManager())
//{
//
//   operator = (var.to_string());
//
//}
//
//
//inline string & string::operator = (const var & var)
//{
//
//   return operator = (var.to_string());
//
//}


inline string  & operator += (string & str,const var & var)
{

   str += var.get_string();

   return str;

}


/*#ifdef MOVE_SEMANTICS


inline var::var(var && v)
{

memcpy(this, &v, sizeof(var));

v.m_sp.m_p           = NULL;
v.m_str.m_pszData    = NULL;
v.m_id.m_pstr        = NULL;

}


#endif

#ifdef MOVE_SEMANTICS
inline var & var::operator = (var && v)
{

if(this != &v)
{

m_sp.release();
m_str.~string();
m_id.~id();

memcpy(this, &v, sizeof(var));

v.m_sp.m_p           = NULL;
v.m_str.m_pszData    = NULL;
v.m_id.m_pstr        = NULL;

}

return *this;

}

#endif
*/


inline class var & var::operator = (const char * psz)
{

   set_string(psz);

   return *this;

}


inline class var & var::operator = (const string & str)
{

   set_string(str);

   return *this;

}


inline class var & var::operator = (string && str)
{

   set_string(::move(str));

   return *this;

}


inline void var::set_string(const char * psz)
{
   if(get_type() == type_pstring)
   {
      *m_pstr = psz;
   }
   else if(get_type() == type_pvar)
   {
      *m_pvar = psz;
   }
   else
   {
      set_type(type_string,false);
      m_str = psz;
   }
}


inline void var::set_string(const string & str)
{
   if(get_type() == type_pstring)
   {
      *m_pstr = str;
   }
   else if(get_type() == type_pvar)
   {
      *m_pvar = str;
   }
   else
   {
      set_type(type_string,false);
      m_str = str;
   }
}

inline void var::set_string(string && str)
{
   if(get_type() == type_pstring)
   {
      *m_pstr = ::move(str);
   }
   else if(get_type() == type_pvar)
   {
      *m_pvar = ::move(str);
   }
   else
   {
      set_type(type_string,false);
      m_str = ::move(str);
   }
}



class CLASS_DECL_AURA ovar :
   virtual public object
{
public:

   var m_var;

};
