#pragma once


class CLASS_DECL_SPHERE pcre_context :
   virtual public object
{
   public:


      void *      m_pthis;


      virtual ~pcre_context();


      static pcre_context * create_context(::aura::application * papp, int iSizeData);


   protected:

      pcre_context(::aura::application * papp);

};


class CLASS_DECL_SPHERE pcre_util :
   virtual public object
{
   public:


      virtual ~pcre_util();

      static pcre_util * compile(::aura::application * papp, const string & str);

      virtual int matches(const string & str, pcre_context * pcreContext = NULL) = 0;

      virtual int matches(const char * psz, strsize len, pcre_context * pcreContext = NULL) = 0;

      virtual void matches(strsize_array & ia, const string & str, pcre_context * pcreContext = NULL) = 0;

      virtual void matches(strsize_array & ia, const char * psz, strsize len, pcre_context * pcreContext = NULL) = 0;

      virtual bool replace(string & str, const string & strPrefix, string & strRet, pcre_context * pcreContext = NULL) = 0;

   protected:

      pcre_util(::aura::application * papp);


};


class CLASS_DECL_SPHERE pcre_code :
   virtual public pcre_util
{
   public:


      virtual ~pcre_code();

      static pcre_code * compile(pcre_context * pcreContext, const string & str);

   protected:

      pcre_code(::aura::application * papp);


};




