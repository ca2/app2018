#include "framework.h" // from "aura/net/net_sockets.h"
#include "aura/net/net_sockets.h"
#ifdef _MSC_VER
#pragma warning(disable:4786)
#endif


namespace http
{


#define TMPSIZE 32000


   form::form(::aura::application * papp) :
      ::object(papp),
      m_setGet(papp),
      m_setPost(papp)
   {

      m_bRaw = false;

   }


   void form::parse_body(file::file *infil, const char * pszContentType, size_t content_length)
   {

      if (::comparison::gt(content_length, Application.http().m_setHttp["max_http_post"].i64()))
      {

         return;

      }

      ::file::plain_text_stream is(infil);

      UNREFERENCED_PARAMETER(content_length);

      size_t extra = 2;

      string content_type(pszContentType);

      infil->seek_to_begin();

      if (content_type.get_length() >= 19 && content_type.Mid(0, 19) == "multipart/form-data")
      {
         ::str::parse pa(content_type,";=");
         char *tempcmp = NULL;
         size_t tc = 0;
         strsize iBoundaryLength = 0;
         string str = pa.getword();
         m_strBoundary = "";
         while (!str.is_empty())
         {
            if (!strcmp(str,"boundary"))
            {
               m_strBoundary = pa.getword();
               iBoundaryLength = m_strBoundary.get_length();
               tempcmp = new char[iBoundaryLength + extra];
            }
            //
            str = pa.getword();
         }
         if (!m_strBoundary.is_empty())
         {
            string content_type;
            string current_name;
            string current_filename;
            string slask;
            while((slask.has_char() && !strncmp(slask,m_strBoundary,m_strBoundary.get_length())) || infil ->read_string(slask))
            {
               content_type = "";
               current_name = "";
               current_filename = "";
               if ((strstr(slask,m_strBoundary) || strstr(m_strBoundary,slask)) && strcmp(slask, m_strBoundary))
               {
                  m_strBoundary = slask;
               }
               if (!strcmp(slask, m_strBoundary))
               {
                  bool bRead;
                  // get headers until is_empty line
                  while((bRead = infil->read_string(slask)))
                  {
                     slask.trim();
                     if(slask.is_empty())
                        break;
                     ::str::parse pa(slask,";");
                     string h = pa.getword();
                     if(!stricmp_dup(h,"Content-type:"))
                     {
                        content_type = pa.getword();
                     }
                     else if(!stricmp_dup(h,"Content-Disposition:"))
                     {
                        h = pa.getword();
                        if (!strcmp(h,"form-data"))
                        {
                           pa.EnableQuote(true);
                           h = pa.getword();
                           while(h.has_char())
                           {
                              ::str::parse pa2(h,"=");
                              string name = pa2.getword();
                              h = pa2.getrest();
                              if (!strcmp(name,"name"))
                              {
                                 if (!h.is_empty() && h[0] == '"')
                                 {
                                    current_name = h.Mid(1, h.get_length() - 2);
                                 }
                                 else
                                 {
                                    current_name = h;
                                 }
                              }
                              else if (!strcmp(name,"filename"))
                              {
                                 if (!h.is_empty() && h[0] == '"')
                                 {
                                    current_filename = h.Mid(1, h.get_length() - 2);
                                 }
                                 else
                                 {
                                    current_filename = h;
                                 }
                                 size_t x = 0;
                                 for (int32_t i = 0; i < current_filename.get_length(); i++)
                                 {
                                    if (current_filename[i] == '/' || current_filename[i] == '\\')
                                       x = i + 1;
                                 }
                                 if (x)
                                 {
                                    current_filename = current_filename.Mid(x);
                                 }
                              }
                              h = pa.getword();
                           }
                        }
                     }
                  }
                  if (!bRead)
                  {

                     break;

                  }
                  // read content, save...?
                  if (current_filename.is_empty()) // not a file
                  {
                     string val;
                     while (infil -> read_string(slask) && strncmp(slask,m_strBoundary,m_strBoundary.get_length() ))
                     {
                        val += slask;
                     }
                     // remove trailing cr/linefeed
                     while (!val.is_empty() && (val[val.get_length() - 1] == 13 || val[val.get_length() - 1] == 10))
                     {
                        val = val.Mid(0,val.get_length() - 1);
                     }
                     //                  cgi = new CGI(current_name, val);
                     //                  m_cgi.push_back(cgi);
                     m_setPost[current_name] = val;
                  }
                  else // current_filename.get_length() > 0
                  {
                     // read until m_strBoundary...
//                     FILE *fil;
                     int32_t out = 0;
                     char ca;
                     string strFormat;
                     ::datetime::time t = ::datetime::time::get_current_time();

                     string   strTime;
                     string   strIndex;
                     string strTempFile;
                     int i = 0;

                     //static mutex s_mutex(NULL);

                     //single_lock sl(&s_mutex, true);

                     while (true)
                     {
                        strTime = t.FormatGmt("%Y\\%m\\%d\\%H\\%M\\%S\\");
                        strIndex.Format("%08x\\", i);
                        strTempFile = "C:\\upload\\" + strTime + strIndex + current_filename;
                        if (!Application.file().exists(strTempFile))
                           break;
                        string strMessage;
                        strMessage = System.datetime().international().get_gmt_date_time() + " " + strTempFile;
                        file_append_wait_dup("C:\\ca2\\toomuchuploads.txt", strMessage);
                        i++;
                     }

                     ::file::file_sp spfile(Application.file().get_file(strTempFile, ::file::defer_create_directory | ::file::type_binary | ::file::mode_create | ::file::mode_write));
                     //sl.unlock();
                     if(spfile.is_set())
                     {
                        while (infil -> read(&ca,1))
                        {
                           if (out)
                           {
                              spfile->write(&tempcmp[tc], 1); // %! ??? should we write value of 'ca' here?
                           }
                           tempcmp[tc] = ca;
                           tc++;
                           if (tc >= iBoundaryLength + extra)
                           {
                              tc = 0;
                              out = 1;
                           }
                           if (tc)
                           {
                              if (!strncmp(tempcmp + tc + extra, m_strBoundary, iBoundaryLength - tc) &&
                                    !strncmp(tempcmp, m_strBoundary.Mid(iBoundaryLength - tc), tc))
                              {
                                 break;
                              }
                           }
                           else
                           {
                              if (!strncmp(tempcmp + extra, m_strBoundary, iBoundaryLength))
                              {
                                 break;
                              }
                           }

                        }

                        spfile->close();

                        //cgi = new CGI(current_name,fn,fn);
                        //m_cgi.push_back(cgi);
                        m_setPost[current_name]["name"]           = current_filename;
                        m_setPost[current_name]["tmp_name"]       = strTempFile;
                        m_setPost[current_name]["content_type"]   = content_type;

                        slask = m_strBoundary;

                     }
                     else
                     {
                        // couldn't open file
                        break;
                     }
                  }
               }
               else
               {
                  // Probably '<m_strBoundary>--'
                  break;
               }
            }
         }
         if(tempcmp != NULL)
         {
            delete[] tempcmp;
         }
      }
      else
         // "x-www-form-urlencoded" is the default
         //if (strstr(content_type, "x-www-form-urlencoded"))
      {
         string str;
         infil->full_read_string(str);
         //strsize len = str.get_length();
//         strsize clen = content_length;
         //TRACE("x-www-form-urlencoded POST is %d bytes length and reported content-length header is %d.", len);
         m_setPost.parse_url_query(str);
      }
   }


   form::~form()
   {
   }


   void form::EnableRaw(bool bRaw)
   {
      m_bRaw = bRaw;
   }


   void form::strcpyval(string & v,const char *value) const
   {
      v = "";
      for (size_t i = 0; i < strlen(value); i++)
      {
         if (value[i] == '<')
         {
            v += "&lt;";
         }
         else if (value[i] == '>')
         {
            v += "&gt;";
         }
         else if (value[i] == '&')
         {
            v += "&amp;";
         }
         else
         {
            v += value[i];
         }
      }
   }





   string & form::boundary()
   {
      return m_strBoundary;
   }

   void form::clear()
   {
      m_bRaw = false;
      m_strBoundary.Empty();
      m_setGet.clear();
      m_setPost.clear();
      m_setRequest.clear();
   }

} //  namespace http


