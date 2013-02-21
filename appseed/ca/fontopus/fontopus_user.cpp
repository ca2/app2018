#include "framework.h"


namespace fontopus
{


   user::user(::ca::application * papp) :
      ca(papp)
   {

      m_ppresence       = NULL;
      m_phttpcookies    = new ::http::cookies();
      m_pifs            = NULL;

   }

   user::~user()
   {
      if(m_phttpcookies != NULL)
         delete m_phttpcookies;
   }

   void user::start_veiev_synch()
   {
      gen::signal_object * psignal = new gen::signal_object();
      gen::emit(get_app(), this, &user::on_end_veiev_synch, this, &user::veiev_synch, psignal);
   }


   bool user::initialize()
   {

      if(&Session == NULL)
         return true;

      Session.userpresence().defer_initialize_user_presence();

      return true;

   }

   bool user::create_ifs()
   {

      if(get_app()->is_system()
      || get_app()->is_cube())
         return false;


      if(m_strLogin == carlosgustavocecynlundgren
      || m_strLogin == camilosasuketsumanuma)
         return false;

      
      if(m_pifs != NULL)
      {
         return true;
      }

      m_pifs = Session.m_pifs;

      return true;

   }

   void user::veiev_synch(gen::signal_object * pobj)
   {
      UNREFERENCED_PARAMETER(pobj);
      int32_t iRestart = 0;
restart:
      if(iRestart >= 10)
         return;
      string strPath;
      var rec = Application.m_simpledb.db().veiev_post().last();
      rec.propset().set_app(get_app());
      rec.propset().remove_by_name("message");
      strPath = "http://europe001.veiev.api.server.ca2.cc/get?" + rec.propset().get_http_post();
      gen::property_set post(get_app());
      gen::property_set headers(get_app());
      gen::property_set set(get_app());
      set["interactive_user"] = true;
      string str;
      Application.http().get(strPath, str, post, headers, set);
      str.trim();
      if(str.has_char())
      {
         xml::document doc(get_app());
         doc.load(str);
         xml::node & node = *doc.get_root();
         string strThread;
         if(node.get_name() == "messages" && node.get_children_count() > 0)
         {
            for(int32_t i = 0; i < node.get_children_count(); i++)
            {
               xml::node * pnodeMessage = node.child_at(i);
               string strSender;
               string strBody;
               xml::node * pnodeSender = pnodeMessage->get_child("sender");
               if(pnodeSender != NULL)
                  strSender = pnodeSender->get_value();
//               int32_t iLength = strSender.get_length();
               xml::node * pnodeBody = pnodeMessage->get_child("body");
               if(pnodeBody != NULL)
               {
                  strBody = pnodeBody->get_value();
                  if(pnodeMessage->get_name() == "message")
                  {
                     strBody = pnodeBody->get_value();
                     pnodeBody->attrs()["message"] = strBody;
                     Application.m_simpledb.db().veiev_post().write(pnodeBody->attrs());
                  }
               }
            }
            iRestart++;
            goto restart;
         }
      }


   }

   void user::on_end_veiev_synch(gen::signal_object * pobj)
   {
      UNREFERENCED_PARAMETER(pobj);
   }

   string user::get_sessid(const char * pszText, bool bInteractive)
   {
      if(pszText == NULL)
      {
         pszText = "https://account.ca2.cc/";
      }
      string strText(pszText);
      if(strText.is_empty())
      {
         strText = "https://account.ca2.cc/";
      }
      strText = System.url().get_server(strText);
      if(strText.is_empty())
         strText = pszText;
      string strSessId = m_sessionidmap[strText];
      if(strSessId.has_char())
         return strSessId;
      class validate authuser(get_app(), "system\\user\\authenticate.xhtml", true, bInteractive);
      user * puser = authuser.get_user(pszText);
      if(puser == NULL)
         strSessId = "not_auth";
      else
      {
         strSessId = puser->m_strFontopusServerSessId;
         if(puser->m_strRequestingServer != strText)
            strSessId = "not_auth";
         else if(strSessId.is_empty())
            strSessId = "not_auth";
         delete puser;
      }
      m_sessionidmap[strText] = strSessId;
      return strSessId;
    }

   void user::set_sessid(const char * pszSessid, const char * pszText)
   {
      if(pszText == NULL)
      {
         pszText = "https://account.ca2.cc/";
      }
      string strText(pszText);
      if(strText.is_empty())
      {
         strText = "https://account.ca2.cc/";
      }
      strText = System.url().get_server(strText);
      m_sessionidmap[strText] = pszSessid;
   }


   string user::get_ca2_server(const char * pszPrefix)
   {

      string strPrefix(pszPrefix);
      string strDomain(".ca2.cc");

      stringa straServer;

      straServer.add(strPrefix + strDomain);
      straServer.add("eu-" + strPrefix + strDomain);
      straServer.add("asia-" + strPrefix + strDomain);

      for(int32_t i = 0; i < 3; i++)
      {

         for(int32_t j = 0; j < straServer.get_size(); j++)
         {
                     
            string strSessId = ApplicationUser.get_sessid(straServer[j]); 

            if(strSessId != "not_auth")
            {

               return straServer[j];

            }

         }

      }

      return straServer[0];

   }


} // namespace fontopus


