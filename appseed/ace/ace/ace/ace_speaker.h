#pragma once


namespace tts
{


   class CLASS_DECL_ACE speaker:
      virtual public object
   {
   public:

      
      speaker(::ace::application * papp);

      virtual ~speaker();


      virtual bool speak(const string & strText);

      virtual bool speak(const string & strLang, const string & strText, bool bSynch = false);

      virtual bool stop(string strLang);

      virtual bool is_speaking(string strLang);


   };


} // namespace tts






