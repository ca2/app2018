#include "framework.h"
#include <openssl/ssl.h>

#if defined(LINUX)
#include <ctype.h>
#endif


namespace mail
{


   pop3_socket::pop3_socket(::sockets::socket_handler_base& h) :
      ::ca::ca(h.get_app()),
      socket(h),
      stream_socket(h),
      tcp_socket(h),
      m_estate(state_disconnected)
   {
      m_bSsl = false;
      m_bTls = false;
      SetLineProtocol();
   }


   pop3_socket::~pop3_socket()
   {
   }




   void pop3_socket::OnLine(const string & line)
   {
      string strLine(line);
      stringa stra;
      switch (m_estate)
      {
      case state_disconnected:
         stra.add_tokens(strLine, " ", TRUE);
         if(stra.get_size() > 0 && stra[0] == "+OK")
         {
            if(m_bTls)
            {
               m_estate = state_auth_1;
               Send("STLS\r\n");
            }
            else
            {
               m_estate = state_auth_2;
               string str = "USER " + m_ppop3->get_user() + "\r\n";
               Send((const char *) str);
            }
         }
         break;
      case state_auth_1:
         stra.add_tokens(strLine, " ", TRUE);
         if(stra.get_size() > 0 && stra[0] == "+OK")
         {
            m_bTls = true;
            m_estate = state_auth_2;
            EnableSSL();
            OnSSLConnect();
            //Handler().Select(1, 0);
   /*         if(GetOutputLength())
               OnWrite();
            if(IsReconnect())
               OnReconnect();
            else
               OnConnect();
            Handler().AddList(GetSocket(), LIST_CALLONCONNECT, false);*/
            string str = "USER " + m_ppop3->get_user() + "\r\n";
            Send((const char *) str);
         }
         break;
      case state_auth_2:
         stra.add_tokens(strLine, " ", TRUE);
         if(stra.get_size() > 0 && stra[0] == "+OK")
         {
            m_estate = state_transaction;
            Send("PASS " + m_ppop3->get_pass() + "\r\n");
         }
         break;
      case state_transaction:
         stra.add_tokens(strLine, " ", TRUE);
         if(stra.get_size() > 0 && stra[0] == "+OK")
         {
            if(gen::str::begins_ci(m_ppop3->get_transaction(), "UIDL"))
            {
               m_estate = state_listmessages_start;
            }
            else if(gen::str::begins_ci(m_ppop3->get_transaction(), "STAT"))
            {
               m_estate = state_stat_start;
            }
            else if(gen::str::begins_ci(m_ppop3->get_transaction(), "TOP"))
            {
               m_estate = state_top_start;
            }
            else if(gen::str::begins_ci(m_ppop3->get_transaction(), "RETR"))
            {
               m_estate = state_retr_start;
            }
            else if(gen::str::begins_ci(m_ppop3->get_transaction(), "LIST") && m_ppop3->get_transaction().get_length() >= 6)
            {
               m_estate = state_receive_list;
            }
            else if(gen::str::begins_ci(m_ppop3->get_transaction(), "RETRHD"))
            {
               m_estate = state_retr_only_headers_start;
               Send("RETR\r\n");
               break;
            }
            Send(m_ppop3->get_transaction() + "\r\n");
         }
         break;
      case state_quit:
         {
            m_estate = state_finished;
         }
         break;
      case state_rset:
         {
            m_ppop3->on_finished_transaction();
            m_estate = state_transaction;
         }
         break;
      case state_stat_start:
         {
            stra.add_tokens(strLine, " ", TRUE);
            if(stra.get_size() > 0 && stra[0] == "+OK")
            {
               m_ppop3->set_stat_count(atoi(stra[1]));
               m_ppop3->set_stat_size(atoi(stra[2]));
               if(m_ppop3->m_bSynch)
               {
                  Send("QUIT\r\n");
                  m_estate = state_quit;
               }
               else
               {
                  m_ppop3->on_finished_transaction();
                  m_estate = state_transaction;
               }
            }
            break;
         }
      case state_receive_list:
         {
            stra.add_tokens(strLine, " ", TRUE);
            if(stra.get_size() > 0 && stra[0] == "+OK")
            {
               m_ppop3->set_list_size(atoi(stra[2]));
               if(m_ppop3->m_bSynch)
               {
                  Send("QUIT\r\n");
                  m_estate = state_quit;
               }
               else
               {
                  m_ppop3->on_finished_transaction();
                  m_estate = state_transaction;
               }
            }
            break;
         }
      case state_top_start:
         stra.add_tokens(strLine, " ", TRUE);
         if(stra.get_size() > 0 && stra[0] == "+OK")
         {
               m_estate = state_receive_top_headers;
               m_ppop3->m_iHeaderLine = 0;
               m_strKey = "";
               m_ppop3->m_setHeaders.m_propertya.remove_all();
         }
         break;
      case state_retr_start:
         stra.add_tokens(strLine, " ", TRUE);
         if(stra.get_size() > 0 && stra[0] == "+OK")
         {
               m_estate = state_retr_headers;
               m_ppop3->m_iHeaderLine = 0;
               m_strKey = "";
               m_ppop3->m_setHeaders.m_propertya.remove_all();
         }
         break;
      case state_retr_only_headers_start:
         stra.add_tokens(strLine, " ", TRUE);
         if(stra.get_size() > 0 && stra[0] == "+OK")
         {
               m_estate = state_retr_only_headers;
               m_ppop3->m_iHeaderLine = 0;
               m_strKey = "";
               m_ppop3->m_setHeaders.m_propertya.remove_all();
         }
         break;
      case state_listmessages_start:
         stra.add_tokens(strLine, " ", TRUE);
         if(stra.get_size() > 0 && stra[0] == "+OK")
         {
            m_estate = state_listmessages;
         }
         break;
      case state_listmessages:
         if(strLine == ".")
         {

            m_ppop3->filter_id();
            if(retrieve())
               m_estate = state_receive_message;
            else
               m_estate = state_transaction;
         }
         else
         {
            stra.add_tokens(strLine, " ", TRUE);
            m_ppop3->m_straIndex.add(stra[0]);
            m_ppop3->m_straId.add(stra[1]);
         }
         break;
      case state_receive_message:
         stra.add_tokens(strLine, " ", TRUE);
         if(stra.get_size() > 0 && stra[0] == "+OK")
         {
               m_estate = state_receive_headers;
               m_ppop3->m_iHeaderLine = 0;
         }
         else
         {
            if(retrieve())
               m_estate = state_receive_message;
            else
               m_estate = state_finished;
         }
         break;
      case state_receive_headers:
         if(strLine == ".")
         {
            m_ppop3->store();
            if(retrieve())
               m_estate = state_receive_message;
            else
               m_estate = state_finished;
         }
         else if(strLine == "")
         {
            m_estate = state_receive_body;
         }
         else
         {
            if(m_ppop3->m_iHeaderLine > 0
               && (strLine[0] == '\t' || strLine[0] == ' '))
            {
               m_ppop3->m_strHeaders += strLine;
            }
            else
            {
               if(m_ppop3->m_iHeaderLine > 0)
               {
                  m_ppop3->m_strHeaders += "\r\n";
               }
               m_ppop3->m_strHeaders += strLine;
               m_ppop3->m_iHeaderLine++;
            }
         }
         break;
      case state_retr_only_headers:
      case state_retr_headers:
      case state_receive_top_headers:
         if(strLine == ".")
         {
            m_ppop3->store();
            if(m_ppop3->m_bSynch)
            {
               Send("QUIT\r\n");
               m_estate = state_quit;
            }
            else
            {
               m_ppop3->on_finished_transaction();
               m_estate = state_transaction;
            }
         }
         else if(strLine == "")
         {
            if(m_strKey.has_char())
            {
               m_ppop3->m_setHeaders[m_strKey] = m_strValue;
            }
            if(m_estate == state_retr_only_headers)
            {
               if(m_ppop3->m_bSynch)
               {
                  Send("QUIT\r\n");
                  m_estate = state_quit;
               }
               else
               {
                  Send("RSET\r\n");
                  m_estate = state_rset;
               }
            }
            else if(m_estate == state_receive_top_headers)
            {
               m_estate = state_receive_top_body;
            }
            else
            {
               m_estate = state_retr_body;
            }
         }
         else
         {
            if(isspace(strLine[0]))
            {
               m_strValue += strLine;
            }
            else
            {
               if(m_strKey.has_char())
               {
                 m_ppop3->m_setHeaders[m_strKey] = m_strValue;
               }
               ::gen::parse pa(strLine,":");
               m_strKey = pa.getword();
               m_strValue = pa.getrest();
            }

            if(m_ppop3->m_iHeaderLine > 0
               && (strLine[0] == '\t' || strLine[0] == ' '))
            {
               m_ppop3->m_strHeaders += strLine;
            }
            else
            {
               if(m_ppop3->m_iHeaderLine > 0)
               {
                  m_ppop3->m_strHeaders += "\r\n";
               }
               m_ppop3->m_strHeaders += strLine;
               m_ppop3->m_iHeaderLine++;
            }
         }
         break;
      case state_receive_body:
         if(strLine == ".")
         {
            m_ppop3->store();
            if(retrieve())
               m_estate = state_receive_message;
            else
            {
               if(m_ppop3->m_bSynch)
               {
                  Send("QUIT\r\n");
                  m_estate = state_quit;
               }
               else
               {
                  m_ppop3->on_finished_transaction();
                  m_estate = state_transaction;
               }
            }
         }
         else
         {
            m_ppop3->m_strBody += strLine + "\r\n";
         }
         break;
      case state_retr_body:
      case state_receive_top_body:
         if(strLine == ".")
         {
            m_ppop3->store();
            if(m_ppop3->m_bSynch)
            {
               Send("QUIT\r\n");
               m_estate = state_quit;
            }
            else
            {
               m_ppop3->on_finished_transaction();
               m_estate = state_transaction;
            }
         }
         else
         {
            m_ppop3->m_strBody += strLine + "\r\n";
         }
         break;
         default:
            break;
            
      }
   }

   bool pop3_socket::retrieve()
   {
      if(m_ppop3->m_straId.get_size() == 0)
         return false;
      m_ppop3->m_id = m_ppop3->m_straId[0];
      string strSend;
      strSend.Format("RETR %s\r\n", m_ppop3->m_straIndex[0]);
      Send((const char *) strSend);
      m_ppop3->m_strHeaders.Empty();
      m_ppop3->m_strBody.Empty();
      m_ppop3->m_straId.remove_at(0);
      m_ppop3->m_straIndex.remove_at(0);
      return true;
   }


   void pop3_socket::SendPrompt()
   {
   }




   void pop3_socket::InitSSLClient()
   {
#if defined(HAVE_OPENSSL)
      if(m_bTls)
      {
         InitializeContext("", TLSv1_method());
      }
      else
      {
         InitializeContext("", SSLv3_method());
      }
#endif
   }


} // namespace mail


