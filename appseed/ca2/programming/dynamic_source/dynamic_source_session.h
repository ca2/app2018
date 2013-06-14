#pragma once


namespace dynamic_source
{


   class CLASS_DECL_ca2 session :
      virtual public ::ca2::object
   {
   public:


      string                              m_strId;
      mutex                               m_mutex;
      ::ca2::property_set                  m_set;
      ::datetime::time                    m_timeExpiry;
      ::dynamic_source::script_manager *  m_pmanager;


      session(const string & strId, ::dynamic_source::script_manager * pmanager);
      virtual ~session();

      virtual int64_t add_ref();
      virtual int64_t release();


   };


   class CLASS_DECL_ca2 session_map :
      virtual public strsp(session)
   {
   public:


   };


} // namespace dynamic_source




