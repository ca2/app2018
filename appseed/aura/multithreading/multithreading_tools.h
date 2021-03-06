#pragma once


class thread_toolset;


class CLASS_DECL_AURA tool_thread :
   virtual public ::thread
{
public:


   index                   m_iThread;
   thread_tools *          m_ptools;
   manual_reset_event      m_evStart;
   manual_reset_event      m_evReady;
   void *                  m_pdata;
   index                   m_cCount;
   index                   m_iIndex;
   ::count                 m_iScan;
   ::count                 m_iCount;
   sp(::pred_holder_base)  m_ppred;
   sp(object)              m_pholdref;


   tool_thread(::thread_tools * ptools);


   virtual void run() override;


   bool set_pred(::pred_holder_base * ppred);

   void reset();
   void start() override;

};


class CLASS_DECL_AURA thread_tool :
   virtual public ::object
{
public:


   thread_toolset *     m_ptoolset;
   tool_thread *        m_pthread;


   thread_tool();
   virtual ~thread_tool();

   virtual void run();

};


class CLASS_DECL_AURA thread_toolset :
   virtual public spa(thread_tool)
{
public:


   thread_tools *    m_pthreadtools;


   thread_toolset(::thread_tools * ptools);
   virtual ~thread_toolset();


   bool add_tool(::thread_tool * ptool);

   inline ::count get_span() const;
   bool nok() const { return ::comparison::le(((int_ptr) this), sizeof(::thread_toolset)) || is_empty(); };

   bool prepare(::count cIteration);
   inline bool operator()();

};


class CLASS_DECL_AURA thread_tools :
   virtual public ::object
{
public:


   spa(tool_thread)              m_threada;
   sync_object_ptra              m_synca;
   ::count                       m_cCount;
   ::count                       m_cIteration;
   ::count                       m_cSpan;
   ::thread::e_op                m_eop;
   sp(::thread_toolset)          m_ptoolset;
   ::multithreading::e_priority  m_epriority;



   thread_tools(::aura::application * papp, ::multithreading::e_priority epriority);
   virtual ~thread_tools();


   ::count get_count() const { return m_threada.get_count(); }
   bool is_empty() const { return get_count() <= 0; }
   bool is_full() const { return m_cCount >= m_threada.get_count(); }
   bool fillable() const { return !is_full(); }
   ::count get_span() const { return m_cSpan; }
   bool nok() const { return ::comparison::le(((int_ptr) this), sizeof(::thread_tools)) || is_empty(); };

   bool prepare(::thread::e_op eop, ::count cIteration = 0);
   bool start();
   bool wait();
   bool operator()();

   bool add_pred(::pred_holder_base * ppred);

   template < typename T >
   ::thread_toolset * create_thread_toolset()
   {

      thread_toolset * pset = canew(thread_toolset(this));

      for(auto & pthread : m_threada)
      {

         T * ptool = canew(T);

         ptool->m_pthread     = pthread;

         ptool->m_ptoolset    = pset;

         pset->add(ptool);

      }

      return pset;

   }

   bool select_toolset(thread_toolset * pset);


};


CLASS_DECL_AURA ::thread_tools * get_thread_tools(::multithreading::e_priority epriority = ::multithreading::priority_none);
CLASS_DECL_AURA ::thread_toolset * get_thread_toolset(::thread::e_tool etool);



template < typename PRED >
::count fork_count_end(::aura::application * papp, ::count iCount, PRED pred, index iStart = 0, ::multithreading::e_priority epriority = ::multithreading::priority_none)
{

   if (iCount <= 0)
   {

      return -1;

   }

   auto ptools = ::get_thread_tools(epriority);

   if (ptools == NULL || ptools->get_count() <= 1)
   {

      for (index i = iStart; i < iCount; i++)
      {

         pred(i);

      }

      return 1;

   }

   if (!ptools->prepare(::thread::op_fork_count, iCount - iStart))
   {

      return -1;

   }

   sync_object_ptra ptra;

   ::count iScan = MAX(1, MIN(iCount - iStart, ptools->get_count()));

   for (index iOrder = 0; iOrder < iScan; iOrder++)
   {

      sp(pred_holder_base) pbase = canew(forking_count_pred < PRED >(papp, iOrder, iOrder + iStart, iScan, iCount, pred));

      ptools->add_pred(pbase);

   }

   if (!(*ptools)())
   {

      return -1;

   }

   return iScan;

}






::count thread_toolset::get_span() const
{
   return m_pthreadtools->get_span();
}

bool thread_toolset::operator()()
{
   return m_pthreadtools->operator()();

}
