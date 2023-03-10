#pragma once


namespace exception
{


   class CLASS_DECL_AURA exception :
      virtual public ::object
   {
   public:


      bool           m_bHandled;
      bool           m_bContinue;
      const char *   m_pszMessage;
      const char *   m_pszException;

      // A exception class is meant to be a small utility/tool class.
      // m_bLog -> too much managing (micro-managing and also big-managing)
      // from utility/tool small class of the Logging System.
      // General-ever Log can be done by final handlers at main Loop and crash handlers
      // Log can be supressed or translated at optional middle-stack handlers.
      // bool        m_bLog;

      const char *   m_pszFile;
      int            m_iLine;


      exception();
      exception(e_context_switcher_failed efail);
      virtual ~exception();
      virtual void Delete();


      const char * cat_message(const char * pszMessage);
      const char * cat_exception(const char * pszException);
      const char * set_file(const char * pszFile);

      virtual const char * what () const NOTHROW;

   };


   class CLASS_DECL_AURA result:
      virtual public exception,
      virtual public spa(exception)
   {
   public:


      result() {}
      result(e_context_switcher_no_exception) {}
      result(e_context_switcher_failed e) :
         exception(e)
      {
         add(canew(exception(e)));
      }
      result(std::initializer_list < exception * > list);
      ~result()
      {
         release();
      }


      string get_all_messages();

      operator bool()
      {
         return get_count() <= 0;
      }

   };



   class CLASS_DECL_AURA result_sp:
      virtual public sp(result)
   {
   public:


      result_sp() {}
      result_sp(e_context_switcher_no_exception) {}
      result_sp(e_context_switcher_failed efailed)
      {
         m_p = canew(result(efailed));
         if(m_p != NULL)
         {
            m_p->add_ref();
         }
      }
      result_sp(exception * pexception)
      {
         if(pexception == NULL)
            return;
         m_p = canew(result({pexception}));
         if(m_p != NULL)
         {
            m_p->add_ref();
         }
      }
//#if defined(__GNUC__) // weak
//      template < typename T >
//      result_sp(sp(T) t)
//      {
//         exception * pexception = t.template cast < exception >();
//         if(pexception == NULL)
//         {
//
//            _throw(simple_exception(get_app(), "smart pointer is not exception"));
//
//         }
//         m_p = canew(result({pexception}));
//         if(m_p != NULL)
//         {
//            m_p->add_ref();
//         }
//      }
//#endif
      ~result_sp()
      {
         release();
      }


      bool succeeded() const
      {
         return is_null();
      }

      bool failed() const
      {
         return !is_null();
      }

      void add(exception * pexception)
      {
         if(pexception == NULL)
            return;
         if(m_p == NULL)
         {
            m_p = canew(result());
            if(m_p != NULL)
            {
               m_p->add_ref();
            }
         }
         if(m_p != NULL)
         {
            m_p->add(pexception);
         }
      }

   };


   class CLASS_DECL_AURA exception_sp :
      public sp(exception)
   {
   public:


      exception_sp() { }
      exception_sp(exception * pexception) : sp(exception)(pexception) { }
      exception_sp(const sp(exception)  spexception) : sp(exception)(spexception) { }
      ~exception_sp() { }


      inline bool is_exit();
      void rethrow_exit();


   };


} // namespace exception


#if defined(APPLEOS)
class ns_exception :
   virtual public ::exception::exception
{
public:

   int m_iCode;

   ns_exception();
   ~ns_exception();

};

#endif


typedef ::exception::result_sp cres;

typedef ::exception::exception_sp esp671;

#define _throw(EXCEPTION_WITH_OPTIONAL_CONSTRUCTION) throw new EXCEPTION_WITH_OPTIONAL_CONSTRUCTION

template < typename EXCEPTION >
void _rethrow(EXCEPTION * pexception)
{

      throw pexception;

}

#define _throw_exit(e) _throw(exit_exception(get_app(), e))

typedef ::exception::exception excpt;


