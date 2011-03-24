#include "StdAfx.h"

namespace mail
{

   db_pop3::db_pop3(::ca::application * papp) :
      ca(papp),
      pop3(papp)
   {

      m_pdatabase = new ::sqlite::base(papp);

      CSingleLock slDataset(&m_csDataset, TRUE);
      m_pdataset = (::sqlite::set *) m_pdatabase->CreateDataset();
      m_bRun = false;

   }

   string db_pop3::get_user()
   {
      return m_paccount->m_strLogin;
   }

   string db_pop3::get_pass()
   {
      string strPass;
      System.crypt().file_get(Application.dir().default_userappdata(Application.dir().default_os_user_path_prefix(), m_paccount->m_strEmail, "license_auth/00003"),
         strPass, "user_vault");
      return strPass;
   }

   string db_pop3::get_host()
   {
      return m_paccount->m_strServer;
   }

   string db_pop3::get_transaction()
   {
      return m_strTransaction;
   }

   void db_pop3::start()
   {
      m_strTransaction = "UIDL";
      pop3::start();
   }

   UINT db_pop3::run()
   {
      System.dir().mk(System.dir().name(m_paccount->m_strMailDb));
      m_pdatabase->setDatabase(m_paccount->m_strMailDb);
      m_pdatabase->connect();
      //create string Table if necessary
      try
      {
         m_pdatabase->start_transaction();
         m_pdataset->query("select * from sqlite_master where type like 'table' and name like 'inbox'");
         if(m_pdataset->num_rows()==0)
         {
            m_pdataset->exec("create table inbox (id text primary key, headers text, body text, sentdatetime text, subject text, sender text)");
         }
         m_pdatabase->commit_transaction();
      }
      catch (...)
      {
         m_pdatabase->rollback_transaction();
         return (UINT) -2;
      }

      return pop3::run();
   }

   void db_pop3::filter_id()
   {
      CSingleLock slDataset(&m_csDataset, TRUE);
      for(int i = 0; i < m_straId.get_size();)
      {
         m_pdataset->query("select count(*) from inbox where id = '" + m_straId[i] + "'");
         if(m_pdataset->num_rows() == 0)
         {
            i++;
         }
         else
         {
            m_pdataset->first();
            if(m_pdataset->FieldValueAt(0).get_integer() < 1)
            {
               i++;
            }
            else
            {
               m_straId.remove_at(i);
            }
         }
      }
   }

   string normalize_datetime(const char * psz)
   {
      string strYear;
      string strMonth;
      string strDay;
      string strHour;
      string strMinute;
      string strSecond;
      string strNumber;
      string strWeekday;
      string strAlpha;

      base_array < bool, bool > baAlpha;
      stringa straDate;
      stringa straDateSeparator;

      bool bNumber = false;
      bool bAlpha = false;
      string strLastSeparator;

      int iLen = int(strlen(psz));
      int i = 0;

      while(i <= iLen)
      {
         if(isdigit(psz[i]))
         {
            if(bAlpha)
            {
               // invalid
               return "invalid";
            }
            else
            {
               bNumber = true;
               strNumber += psz[i];
            }
         }
         else if(isalpha(psz[i]))
         {
            if(bNumber)
            {
               // invalid
               return "invalid";
            }
            else
            {
               bAlpha = true;
               strAlpha += psz[i];
            }
         }
         else if(psz[i] == ':')
         {
            bNumber = false;
            bAlpha = false;
            if(strMinute.get_length() > 0 && strLastSeparator == ':')
            {
               return "invalid";
            }
            else if(strHour.get_length() > 0 && strLastSeparator == ':')
            {
               strMinute = strNumber;
            }
            else if(strHour.get_length() == 0 && strLastSeparator != ':')
            {
               strHour = strNumber;
            }
            strNumber.Empty();
            strLastSeparator = ':';
         }
         else
         {
            if(strMinute.get_length() > 0 && strLastSeparator == ':')
            {
               strSecond = strNumber;
            }
            else if(strHour.get_length() > 0 && strLastSeparator == ':')
            {
               strMinute = strNumber;
            }
            else
            {
               if(strNumber.get_length() > 0)
               {
                  bNumber = false;
                  bAlpha = false;
                  straDateSeparator.add(psz[i]);
                  straDate.add(strNumber);
                  baAlpha.add(false);
                  strNumber.Empty();
               }
               else if(strAlpha.get_length() > 0)
               {
                  bNumber = false;
                  bAlpha = false;
                  straDateSeparator.add(psz[i]);
                  straDate.add(strAlpha);
                  baAlpha.add(true);
                  strAlpha.Empty();
               }
            }
            strLastSeparator = psz[i];
         }
         i++;
      }

      for(int i = 0; i < straDate.get_size();)
      {
         if(baAlpha[i])
         {
            if(straDate[i] == "Jan" 
               || straDate[i] == "January" )
            {
               strMonth = "01";
            }
            else if(straDate[i] == "Feb" 
               || straDate[i] == "February" )
            {
               strMonth = "02";
            }
            else if(straDate[i] == "Mar" 
               || straDate[i] == "March" )
            {
               strMonth = "03";
            }
            else if(straDate[i] == "Apr" 
               || straDate[i] == "April" )
            {
               strMonth = "04";
            }
            else if(straDate[i] == "May" )
            {
               strMonth = "05";
            }
            else if(straDate[i] == "Jun" 
               || straDate[i] == "June" )
            {
               strMonth = "06";
            }
            else if(straDate[i] == "Jul" 
               || straDate[i] == "July" )
            {
               strMonth = "07";
            }
            else if(straDate[i] == "Aug" 
               || straDate[i] == "August" )
            {
               strMonth = "08";
            }
            else if(straDate[i] == "Sep" 
               || straDate[i] == "September" )
            {
               strMonth = "09";
            }
            else if(straDate[i] == "Oct" 
               || straDate[i] == "October" )
            {
               strMonth = "10";
            }
            else if(straDate[i] == "Nov" 
               || straDate[i] == "November" )
            {
               strMonth = "11";
            }
            else if(straDate[i] == "Dec" 
               || straDate[i] == "December" )
            {
               strMonth = "12";
            }
            else if(straDate[i] == "Sun" 
               || straDate[i] == "Sunday" )
            {
               strWeekday = "01";
            }
            else if(straDate[i] == "Mon" 
               || straDate[i] == "Monday" )
            {
               strWeekday = "02";
            }
            else if(straDate[i] == "Tue" 
               || straDate[i] == "Tuesday" )
            {
               strWeekday = "03";
            }
            else if(straDate[i] == "Wed" 
               || straDate[i] == "Wednesday" )
            {
               strWeekday = "04";
            }
            else if(straDate[i] == "Thu" 
               || straDate[i] == "Thursday" )
            {
               strWeekday = "05";
            }
            else if(straDate[i] == "Fri" 
               || straDate[i] == "Friday" )
            {
               strWeekday = "06";
            }
            else if(straDate[i] == "Sat" 
               || straDate[i] == "Saturday" )
            {
               strWeekday = "07";
            }
            straDate.remove_at(i);
            baAlpha.remove_at(i);
         }
         else
         {
            i++;
         }
      }

      if(strMonth.get_length() > 0)
      {
         if(straDate.get_size() >= 1)
         {
            if(atoi(straDate[0]) >= 32)
            {
               strYear = straDate[0];
            }
            else if(straDate.get_size() == 1)
            {
               return "ambiguous";
            }
            if(straDate.get_size() >= 2)
            {
               if(atoi(straDate[1]) >= 32)
               {
                  strYear = straDate[1];
                  strDay = straDate[0];
               }
               else 
               {
                  strDay = straDate[1];
               }
            }
         }
         goto ret;
      }



      index iFind = straDateSeparator.find_first("-");
      if(iFind >= 0)
      {
         iFind = straDateSeparator.find_first("-", iFind + 1);
         if(iFind > 0)
         {
            strYear = straDate[iFind - 1];
            strMonth = straDate[iFind];
            strDay = straDate[iFind + 1];
         }
      }
      else
      {
         iFind = straDateSeparator.find_first("/");
         if(iFind >= 0)
         {
            return "ambiguous";
         }
         iFind = straDateSeparator.find_first(".");
         if(iFind >= 0)
         {
            iFind = straDateSeparator.find_first("-", iFind + 1);
            if(iFind > 0)
            {
               strYear = straDate[iFind - 1];
               strDay = straDate[iFind];
               strMonth = straDate[iFind + 1];
            }
         }
      }
ret:
      string strRet;
      strRet.Format("%04d-%02d-%02d %02d:%02d:%02d", 
         atoi(strYear),
         atoi(strMonth),
         atoi(strDay),
         atoi(strHour),
         atoi(strMinute),
         atoi(strSecond));
      return strRet;
   }

   void db_pop3::store()
   {
      CSingleLock slDataset(&m_csDataset, TRUE);
      stringa straHeaders;
      straHeaders.add_tokens(m_strHeaders, "\r\n", TRUE);
      string strSentDateTime;
      string strSubject;
      string strKey;
      string strValue;
      string strLine;
      string strSender;
      int iPos;
      for(int i = 0; i < straHeaders.get_size(); i++)
      {
         strLine = straHeaders[i];
         iPos = strLine.find(":");
         if(iPos > 0)
         {
            strKey = strLine.Mid(0, iPos);
            strValue = strLine.Mid(iPos + 1);
            if(strKey.CompareNoCase("Date") == 0)
            {
               strSentDateTime = normalize_datetime(strValue);
            }
            else if(strKey.CompareNoCase("Subject") == 0)
            {
               strSubject = strValue;
            }
            else if(strKey.CompareNoCase("From") == 0)
            {
               strSender = strValue;
            }
         }
      }
      string strSql;
      
      strSql.Format("insert into inbox (id, headers, body, sentdatetime, subject, sender) values ('%s', '%s', '%s', '%s', '%s', '%s')",
         m_pdatabase->escape(m_id),
         m_pdatabase->escape(m_strHeaders),
         m_pdatabase->escape(m_strBody),
         m_pdatabase->escape(strSentDateTime),
         m_pdatabase->escape(strSubject),
         m_pdatabase->escape(strSender));
      m_pdataset->exec(strSql);
      slDataset.Unlock();
      update_lists();
   }


   void db_pop3::update_lists()
   {
      if(get_mail_tree() != NULL)
      {
         get_mail_tree()->update_list();
      }
      if(get_mail_list() != NULL)
      {
         get_mail_list()->_001OnUpdateItemCount();
      }
   }

   tree *           db_pop3::get_mail_tree()
   {
      return App(m_papp).m_pmailtree;
   }

   list *             db_pop3::get_mail_list()
   {
      return App(m_papp).m_pmaillist;
   }

} // namespace mail
