#pragma once



class CLASS_DECL_BASE ptra :
   virtual public spa(element)
{
public:


};


typedef ::map < sp(element), sp(element), sp(element), sp(element) > element_map;
typedef ::map < sp(element), sp(element), ptra, ptra > map_many;



#undef __id
#define __id(x) (m_pbaseapp->m_pbasesystem->m_cidpool.id_##x)



class CLASS_DECL_BASE id_pool
{
public:

   id id_eu;
   id id_uk;
   id id_fi;
   id id_gr;
   id id_nl;
   id id_be;
   id id_it;
   id id_cz;


   id id_dk;
   id id_en_uk;
   id id_lu;
   id id_ie;
   id id_no;
   id id_cy;
   id id_su;
   id id_lv;
   id id_li;
   id id_hu;
   id id_sk;
   id id_si;
   id id_ro;
   id id_kz;
   id id_ru;
   id id_pl;
   id id_tr;
   id id_ee;
   id id_asia;
   id id_vn;
   id id_in;
   id id_kg;
   id id_kr;
   id id_my;
   id id_ph;
   id id_sg;


   id id_zh;
   id id_sy;
   id id_ps;
   id id_ar;
   id id_bo;
   id id_cl;
   id id_co;
   id id_gs;
   id id_py;
   id id_uy;
   id id_ve;
   id id_amdesur;
   id id_au;
   id id_oceania;
   id id_nz;
   id id_tl;
   id id_ug;
   id id_sc;
   id id_cm;
   id id_za;
   id id_africa;
   id id_us;
   id id_caribe;
   id id_ht;
   id id_cu;
   id id_tc;
   id id_centralam;
   id id_bz;
   id id_america;

   id id_cc;
   id id_se;
   id id_sv_se;
   id id_sv;


   id id_at;
   id id_de_at;
   id id_de;
   id id_de_de;

   id id_br;
   id id_pt_br;
   id id_pt;

   id id_es;

   id id_cn;
   id id_zh_cn;
   id id_zh_tw;
   id id_hk;

   id id_ja;
   id id_jp;
   id id_ja_jp;

   id id_fr;
   id id_fr_ca;
   id id_ca;

   id id_en;
   id id_en_us;
   id id_en_ca;
   id id_std;

   id id_pe;
   id id_mx;
   id id_ec;

   id id_tw;

   id id_earth;
   id id_vg;
   id id_ae;
   id id_il;


   id id_http_host;
   id id_http_version;
   id id_http_status_code;
   id id_http_status;
   id id_http_method;
   id id_http_protocol;
   id id_http_referer;
   id id_https;

   id id_query_string;

   id id_content_length;
   id id_content_type;
   id id_connection;
   id id_location;
   id id_set_cookie;
   id id_cookie;
   id id_cache_control;
   id id_pragma;
   id id_expires;
   id id_host;
   id id_referer;

   id id_close;

   id id_put;

   id id_remote_addr;
   id id_request_uri;

   id id_accept_encoding;
   id id_content_encoding;

   id id_keep_alive;

   id id_user_agent;
   id id_from;
   id id_accept_language;


   id id_accept;


   id_pool()
   {

      reset();

   }


   void reset() const
   {
      (const_cast < id_pool * > (this))->_reset();
   }


protected:

   void _reset();

};




class CLASS_DECL_BASE base_system :
   virtual public base_application
{
public:


   stridsp(type)                                m_typemap;
   spa(service_base)                            m_serviceptra;
   sp(base_factory)                             m_pfactory;
   ::url::departament                           m_urldepartament;
   sp(class ::xml::departament)                 m_pxml;
   const id_pool                                m_cidpool;
   sp(class ::core::log)                        m_plog;


   static id_space *                            s_pidspace;
   static class id                              idEmpty;
   class ::str::base64                          m_base64;

   ::file::system_sp                            m_spfile;
   ::file::dir::system_sp                       m_spdir;
   class ::sockets::net                         m_net;
   ::http::system                               m_httpsystem;
   ::string_to_string                           m_mapAppLibrary;
   class machine_event_central *                m_pmachineeventcentral;
   class ::fontopus::user_set                   m_userset;
   ::crypto::crypto_sp                          m_spcrypto;
   sp(class ::datetime::departament)            m_pdatetime;
   string_map < int_to_string >                 m_mapEnumToName;
   string_map < string_to_int >                 m_mapNameToEnum;
   sp(::user::window_draw)                      m_ptwf;






   base_system(sp(base_application) papp);


   virtual bool initialize_instance();


   class base_factory                           & factory();
   inline ::url::departament                    & url()     { return m_urldepartament; }
   ::xml::departament                           & xml();
   class ::str::base64                          & base64();

   class ::core::log                            & log();

   inline class ::http::system                  & http()    { return m_httpsystem; }
   inline ::file::system                        & file()    { return *m_spfile; }
   inline ::file::dir::system                   & dir()     { return *m_spdir; }
   class ::sockets::net                         & net();
   class ::core::compress                       & compress();
   class ::machine_event_central                & machine_event_central();
   ::fontopus::user_set                         & userset();

   class ::crypto::crypto                       & crypto();
   ::datetime::departament                      & datetime();
   ::user::str                                  & str();





   using base_application::alloc;
   virtual sp(element) alloc(sp(base_application) papp, sp(type) info);
   virtual sp(element) alloc(sp(base_application) papp, const class id & idType);

   virtual sp(element) on_alloc(sp(base_application) papp, sp(type) info);
   virtual sp(element) clone();
   virtual sp(element) clone(sp(element) pobj);
   template < typename T >
   inline T * cast_clone(T * pt)
   {
      return dynamic_cast < T * > (clone(dynamic_cast < sp(element) > (pt)));
   }

   template < class T >
   sp(type) type_info()
   {
      return get_type_info(typeid(T));
   }

   virtual sp(::type) get_type_info(const ::std_type_info & info);
   sp(::type) get_type_info(const class id & idType)
   {
      return m_typemap[idType];
   }


   virtual void discard_to_factory(sp(element) pca);


   static ::exception::engine & eengine();


   virtual UINT os_post_to_all_threads(UINT uiMessage, WPARAM wparam, lparam lparam);

   virtual bool process_initialize();


   virtual void wait_twf();


   virtual bool is_system();

   sp(::user::window_draw)               get_twf();


   virtual void on_allocation_error(sp(base_application) papp, sp(type) info);
   //   sp(element) alloc(sp(base_application) papp, sp(type) info);
   sp(element) alloc(sp(base_application) papp, const std_type_info & info);
   //   virtual sp(element) on_alloc(sp(base_application) papp, sp(type) info);


   static inline class id id(const ::std_type_info & info);
   static inline class id id(const char * psz);
   static inline class id id(const string & str);
   static inline class id id(int64_t i);
   static inline class id_space & id();
   inline class id id(const var & var);
   inline class id id(const property & prop);


   virtual bool assert_failed_line(const char * lpszFileName, int32_t iLine);
   virtual bool on_assert_failed_line(const char * pszFileName, int32_t iLine);


   void set_enum_name(sp(type) etype, int32_t i, const char * psz)
   {
      m_mapEnumToName[etype->name()][i] = psz;
      m_mapNameToEnum[etype->name()][psz] = i;
   }
   string get_enum_name(sp(type) info, int32_t i)
   {
      return m_mapEnumToName[info->name()].get(i, "");
   }

   template < class E, E edefault>
   void from_name(base_enum < E, edefault > & b, const char * psz, E iDefault = edefault)
   {
      b = enum_from_name(System.type_info < E >(), psz, iDefault);
   }

   template < class E, E edefault>
   string get_name(const base_enum < E, edefault > & b)
   {
      return get_enum_name(System.type_info < E >(), (int32_t)(E)b);
   }

   template < class ENUM >
   ENUM enum_from_name(sp(type) info, const char * psz, int32_t iDefault = 0)
   {
      return (ENUM)m_mapNameToEnum[info->name()].get(psz, iDefault);
   }
   int32_t enum_from_name(sp(type) info, const char * psz, int32_t iDefault = 0)
   {
      return m_mapNameToEnum[info->name()].get(psz, iDefault);
   }
   int32_t enum_from_name(const std_type_info & info, const char * psz, int32_t iDefault = 0)
   {
#ifdef WINDOWS
      return m_mapNameToEnum[info.name()].get(psz, iDefault);
#else
      return m_mapNameToEnum[info.name()].get(psz, iDefault);
#endif
   }

   template < class TYPE >
   void set_enum_name(sp(type) etype, TYPE e, const char * psz)
   {
      set_enum_name(etype, (int32_t)e, psz);
   }
   template < class TYPE >
   string get_enum_name(sp(type) etype, TYPE e)
   {
      return get_enum_name(etype, (int32_t)e);
   }

   template < class TYPE >
   void set_enum_name(const std_type_info & info, TYPE e, const char * psz)
   {
      set_enum_name(get_type_info(info), (int32_t)e, psz);
   }
   template < class TYPE >
   string get_enum_name(const std_type_info & info, TYPE e)
   {
      return get_enum_name(get_type_info(info), (int32_t)e);
   }

   template < class TYPE >
   void set_enum_name(TYPE e, const char * psz)
   {
      set_enum_name(System.type_info < TYPE >(), (int32_t)e, psz);
   }
   template < class TYPE >
   string get_enum_name(TYPE e)
   {
      return get_enum_name(System.type_info < TYPE >(), (int32_t)e);
   }




};





template < size_t _Bits >
inline ::file::output_stream & operator << (::file::output_stream & _Ostr, const bitset<_Bits>& _Right)
{
   // insert bitset as a string
   return (_Ostr << _Right.template to_string());
}

// TEMPLATE operator>>
template < size_t _Bits >
inline ::file::input_stream & operator >>( ::file::input_stream &  _Istr, bitset<_Bits>& _Right)
{
   // extract bitset as a string
   string _Str;

   _Istr >> _Str;

   _Right = bitset<_Bits>(_Str);	// convert string and store

   return (_Istr);

}



