#include "framework.h"
#include "ftp_output.h"



namespace ftp
{


   output::output(::aura::application * papp) :
      object(papp)
   {
   }

   output::~output()
   {

   }


   void output::OnSendCommand(const command& command, const stringa& straArguments)
   {

      if (command.AsString().length() == 0)
         return;

      string strImplode = straArguments.implode(" ");

      if (command.AsString().find_ci("PASS") >= 0)
      {
         if (strImplode.has_char())
         {
            WriteLine("< PASS **********", "S");
         }
         else
         {
            WriteLine("< PASS ", "S");
         }
      }
      else
      {

         WriteLine("> " + command.AsString() + " " + strImplode, "S");

      }

   }

   void output::OnResponse(const reply & Reply)
   {
      if (Reply.Value().length() == 0)
         return;

      string strStatus = "R"; // RGB(0, 150, 0);
      if (Reply.Code().IsPermanentNegativeCompletionReply())
         strStatus = "N"; // RGB(255, 0, 0);
      else if (Reply.Code().IsTransientNegativeCompletionReply())
         strStatus = "W"; // = RGB(200, 200, 0);

      WriteLine("< " + Reply.Value(), strStatus);

   }

   void output::OnInternalError(const string& strErrorMsg, const string& strFileName, DWORD dwLineNr)
   {

      string strMessage;

      strMessage.Format(("%s ==> File \"%s\" (%d)"), strErrorMsg.c_str(), strFileName.c_str(), dwLineNr);

      WriteLine(strMessage, "E");

   }

   void output::WriteLine(const string & strLine, const string &  strStatus)
   {

      TRACE("%s: %s", strStatus, strLine);

   }




} // namespace ftp
