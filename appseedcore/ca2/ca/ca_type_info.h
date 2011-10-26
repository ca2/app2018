#pragma once


class factory_item_base;


namespace ca
{

   class CLASS_DECL_ca type_info :
      virtual public ca
   {
   public:

      ::ca::smart_pointer < mutex >       m_spmutex;
      id                                  m_id;
      ph(factory_item_base)               m_pfactoryitem; 

      type_info();
      type_info(const type_info * pinfo);
      type_info(const type_info & info);
      type_info(const std_type_info * pinfo);
      type_info(const std_type_info & info);
      type_info(const char * pszRawName);
      type_info(id id);
      virtual ~type_info();

      type_info & operator = (const ::ca::type_info * pinfo);
      type_info & operator = (::ca::type_info info);
      type_info & operator = (const char * psz);
      type_info & operator = (const std_type_info & info);
      type_info & operator = (id id);



      bool operator == (const std_type_info & info) const;
      bool operator == (::ca::type_info info) const;
      bool operator == (const char * pszRawName) const;

      bool operator != (const std_type_info & info) const;
      bool operator != (::ca::type_info info) const;
      bool operator != (const char * pszRawName) const;

      virtual void raw_name(const char * pszRawName);
      virtual const char * raw_name() const;

      virtual ptra * new_ptra();

      operator bool() const
      {
         return m_id.m_psz != NULL;
      }
   };


   template < typename T >
   inline ::ca::type_info & get_type_info()
   {
      static ::ca::type_info s_typeinfo = typeid(T);
      return s_typeinfo;
   }

} // namespace ca


CLASS_DECL_ca bool operator == (const std_type_info & info1, ::ca::type_info info2);
CLASS_DECL_ca bool operator != (const std_type_info & info1, ::ca::type_info info2);


