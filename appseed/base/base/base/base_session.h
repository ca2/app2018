#pragma once




class CLASS_DECL_BASE base_session :
   virtual public base_application
{
public:


   ::plane::session *                                       m_pplanesession;
   string_map < sp(base_application) >                      m_mapApplication;
   sp(::userpresence::userpresence)                         m_puserpresence;
   sp(::ifs)                                                m_pifs;
   sp(::fs::remote_native)                                  m_prfs;
   bool                                                     m_bDrawCursor;


   ::visual::e_cursor                                       m_ecursor;
   ::visual::e_cursor                                       m_ecursorDefault;
   ::core::copydesk_sp                                      m_spcopydesk;





   inline ::userpresence::userpresence & userpresence() { return *m_puserpresence; }

   virtual bool is_session();


   
   ::core::copydesk & copydesk();

   virtual sp(::base_application) start_application(const char * pszType, const char * pszAppId, sp(::create_context) pcreatecontext);

   virtual ::visual::cursor * get_cursor();
   virtual void set_cursor(::visual::e_cursor ecursor);
   virtual void set_default_cursor(::visual::e_cursor ecursor);
   virtual ::visual::cursor * get_default_cursor();

   virtual COLORREF get_default_color(uint64_t ui);



};




