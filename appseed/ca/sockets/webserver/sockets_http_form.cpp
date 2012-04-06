#ifdef _MSC_VER
#pragma warning(disable:4786)
#endif
#include "StdAfx.h"



namespace http
{

#define TMPSIZE 32000

   form::form(::ca::application * papp) :
      ca(papp),
      m_setGet(papp),
      m_setPost(papp)
   {
      m_bRaw = false;
   }

   void form::parse_body(http::memory_file *infil, const char * pszContentType, size_t content_length)
   {
      UNREFERENCED_PARAMETER(content_length);

      size_t extra = 2;

      string content_type(pszContentType);

      infil->seek_to_begin();

      if (content_type.get_length() >= 19 && content_type.Mid(0, 19) == "multipart/form-data")
      {
         ::gen::parse pa(content_type,";=");
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
                  // get headers until is_empty line
                  while(infil->read_string(slask))
                  {
                     slask.trim();
                     if(slask.is_empty())
                        break;
                     ::gen::parse pa(slask,";");
                     string h = pa.getword();
                     if(!strcasecmp(h,"Content-type:"))
                     {
                        content_type = pa.getword();
                     }
                     else
                     if(!strcasecmp(h,"Content-Disposition:"))
                     {
                        h = pa.getword();
                        if (!strcmp(h,"form-data"))
                        {
                           pa.EnableQuote(true);
                           h = pa.getword();
                           while(h.has_char())
                           {
                              ::gen::parse pa2(h,"=");
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
                              else
                              if (!strcmp(name,"filename"))
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
                                 for (int i = 0; i < current_filename.get_length(); i++)
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
                     int out = 0;
                     char c;
                     string strTempFile = System.file().time_square(get_app());
                     ex1::filesp spfile(Application.get_file(strTempFile, ::ex1::file::type_binary | ::ex1::file::mode_create | ::ex1::file::mode_write));
                     if(spfile.is_set())
                     {
                        while (infil -> read(&c,1))
                        {
                           if (out)
                           {
                              spfile->write(&tempcmp[tc], 1); // %! ??? should we write value of 'c' here?
                           }
                           tempcmp[tc] = c;
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
      if (strstr(content_type, "x-www-form-urlencoded"))
      {
         string str;
         infil->read_string(str);
         m_setPost.parse_url_query(str);
   /*      bool got_name = false; // tnx to FatherNitwit
         int cl = (int)content_length;
         char c,chigh,clow;
         string slask;
   //      m_current = m_cgi.end();
         string name;

         cl--;
         while (cl >= 0 && infil -> read(&c, 1))
         {
            switch (c)
            {
               case '=': /* end of name */
   /*               name = slask;
                  slask.Empty();
                  got_name = true;
                  break;
               case '&': /* end of value */
   /*               if (got_name)
                  {
   //                  cgi = new CGI(name,slask);
                     if(gen::str::ends_eat(name, "[]"))
                     {
                        m_setPost[name].vara().add(slask);
                     }
                     else
                     {
                        m_setPost[name] = slask;
                     }
                     got_name = false;
                  }
                  else
                  {
   //                  cgi = new CGI(slask, "");
                     m_setPost[slask] = "";
                  }
                  slask.Empty();
                  //m_cgi.push_back(cgi);
                  break;
               case '+': /* space */
   /*               slask += " ";
                  break;
               case '%': /* hex value */
   /*               infil -> read(&chigh,1);
                  cl--;
                  chigh -= 48 + (chigh > '9' ? 7 : 0) + (chigh >= 'a' ? 32 : 0);
                  infil -> read(&clow,1);
                  cl--;
                  clow -= 48 + (clow > '9' ? 7 : 0) + (clow >= 'a' ? 32 : 0);
                  slask += (char)(chigh * 16 + clow);
                  break;
               default: /* just another char */
   /*               slask += c;
                  break;
            }
            //
            cl--;
         }
         if (got_name)
         {
   //                  cgi = new CGI(name,slask);
            if(gen::str::ends_eat(name, "[]"))
            {
               m_setPost[name].vara().add(slask);
            }
            else
            {
               m_setPost[name] = slask;
            }
            got_name = false;
         }
         else
         {
   //                  cgi = new CGI(slask, "");
            m_setPost[slask] = "";
         }*/
      }
   }


   // form(buffer,l) -- request_method GET

   void form::parse_query_string(const string & buffer,size_t l)
   {
      m_setGet.parse_url_query(buffer);
      return;
      string slask;
      string name;
      char c,chigh,clow;
      size_t ptr = 0;
      bool got_name = false;


      ptr = 0;
      while (ptr < l)
      {
         c = buffer.operator[](ptr++);
         switch (c)
         {
            case '=': /* end of name */
               name = slask;
               slask.Empty();
               got_name = true;
               break;
            case '&': /* end of value */
                  if (got_name)
                  {
   //                  cgi = new CGI(name,slask);
                     m_setGet[name] = slask;
                     got_name = false;
                  }
                  else
                  {
   //                  cgi = new CGI(slask, "");
                     m_setGet[slask] = "";
                  }
               slask.Empty();
   //            m_cgi.push_back(cgi);
               break;
            case '+': /* space */
               slask += " ";
               break;
            case '%': /* hex value */
               chigh = buffer.operator[](ptr++);
               chigh -= 48 + (chigh > '9' ? 7 : 0) + (chigh >= 'a' ? 32 : 0);
               clow = buffer.operator[](ptr++);
               clow -= 48 + (clow > '9' ? 7 : 0) + (clow >= 'a' ? 32 : 0);
               slask += (char)(chigh * 16 + clow);
               break;
            default: /* just another char */
               slask += c;
               break;
         }
      }
                  if (got_name)
                  {
   //                  cgi = new CGI(name,slask);
                     m_setGet[name] = slask;
                     got_name = false;
                  }
                  else
                  {
   //                  cgi = new CGI(slask, "");
                     m_setGet[slask] = "";
                  }
   //   m_cgi.push_back(cgi);
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
         else
         if (value[i] == '>')
         {
            v += "&gt;";
         }
         else
         if (value[i] == '&')
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


