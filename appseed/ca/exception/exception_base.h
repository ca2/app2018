#pragma once


#undef delete


class string;


class CLASS_DECL_ca base_exception :
   virtual public ::ca::exception,
   virtual public call_stack
{
public:


   base_exception(::ca::application * papp, unsigned int uiSkip = 4 /* -1 for disable call stack*/);   // sets m_bAutoDelete = TRUE
   explicit base_exception(::ca::application * papp, bool bAutoDelete, unsigned int uiSkip = 4 /* -1 for disable call stack*/);   // sets m_bAutoDelete = bAutoDelete
   // Implementation (setting m_bAutoDelete to FALSE is advanced)
   virtual ~base_exception();


   void Delete();  // use to delete exception in 'catch' block


   virtual bool get_error_message(string & str, PUINT pnHelpContext = NULL);
   virtual string get_message(PUINT pnHelpContext = NULL);


   virtual int ReportError(UINT nType = MB_OK, const char * pszMessageID = NULL);


   void PASCAL operator delete(void * pbData);
   void PASCAL operator delete(void * pbData, const char * lpszFileName, int nLine);


};





