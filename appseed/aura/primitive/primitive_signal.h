#pragma once


namespace message
{


   class dispatch;


} // namespace message


class signal;
class request_signal;


class CLASS_DECL_AURA signal_details :
   public object
{
public:


   UINT                       m_uiMessage;
   WPARAM                     m_wparam;
   lparam                     m_lparam;
   index                      m_iIndex;
   property_set *             m_pset;
   index                      m_iParam;
   bool                       m_bRet;
   sp(class signal)           m_psignal;



   signal_details(::aura::application * papp = NULL);
   signal_details(class signal * psignal);
   signal_details(::aura::application * pap, class signal * psignal);
   virtual ~signal_details();


   bool emit(); // returns m_bRet

   bool all_previous(); // returns bRet

   bool previous(); // returns bRet

   property_set & operator()();


};


class CLASS_DECL_AURA signalizable :
   virtual public ::object
{
public:


   ref_array < class signal > m_signalptra;


   signalizable();
   virtual ~signalizable();


   virtual void on_request_signal(request_signal * pobj);

   void register_signal(class signal * psignal);
   void unregister_signal(class signal * psignal);
   void unregister_target(signalizable* psignalizable);
   void filter_target(signalizable* psignalizable);


   virtual void install_message_handling(::message::dispatch * pdispatch);

   virtual void signalizable_disconnect_all();

};

class CLASS_DECL_AURA base_signalizable_array :
   virtual protected spa(signalizable)
{
public:

   inline void add(signalizable * psignalizable)
   {
      install_handlers(psignalizable);
      spa(signalizable)::add(psignalizable);
   }

   inline void remove(signalizable * psignalizable)
   {
      spa(signalizable)::remove(psignalizable);
   }

   inline signalizable * element_at(index i) { return spa(signalizable)::element_at(i); }
   inline ::count get_size() { return spa(signalizable)::get_size(); }
   inline ::count get_count() { return spa(signalizable)::get_count(); }
   inline ::count size() { return spa(signalizable)::size(); }
   inline ::count count() { return spa(signalizable)::count(); }

   virtual void install_handlers(signalizable *) {}

};


template < class SIGNALIZABLE >
class signalizable_array :
   virtual public base_signalizable_array
{
public:

   inline void add(SIGNALIZABLE * psignalizable)
   {
      base_signalizable_array::add(psignalizable);
   }

   inline void remove(SIGNALIZABLE * psignalizable)
   {
      base_signalizable_array::remove(psignalizable);
   }

   SIGNALIZABLE * element_at(index i) { return dynamic_cast < SIGNALIZABLE * > (base_signalizable_array::element_at(i)); }

};

class CLASS_DECL_AURA signal :
   virtual public object
{
public:


   class signal_delegate :
      virtual public object
   {
   public:

      virtual ~signal_delegate() { }
      virtual void emit(signal_details * pobject) = 0;
      virtual signalizable * get_signalizable() { return NULL;}
   };

   template < class T >
   class signal_delegate_instance : public signal_delegate
   {
   public:

      signal_delegate_instance(T * psignalizable)
      {
#if defined(DEBUG) && defined(WINDOWS)
         // The object that is connected to a signal should be signalizable
         // (derived from signalizable);
         signalizable * psignalizableTest = psignalizable;
         DBG_UNREFERENCED_LOCAL_VARIABLE(psignalizableTest);
#endif
         m_psignalizable = psignalizable;
      };
      T * m_psignalizable; // should be derived from
      void (T::*m_pfn)(signal_details *);
      virtual void emit(signal_details * pobject);
      bool operator == (const signal_delegate_instance & deleg) const;
      virtual signalizable * get_signalizable() { return m_psignalizable;}
   };


   spa(signal_delegate) m_delegateptra;


   signal();
   virtual ~signal();

   bool has_handler();
   void emit(signal_details * pobject = NULL);
   void emit_previous(signal_details * pobject);
   void emit_all_previous(signal_details * pobject);



   template < class T >
   void connect(T * psignalizable, void (T::*pfn)(signal_details *))
   {
      signal_delegate_instance < T > * pdelegate = canew(signal_delegate_instance < T >(psignalizable));
      pdelegate->m_pfn = pfn;
      m_delegateptra.add(pdelegate);
      psignalizable->register_signal(this);
   }

   template < class T >
   bool is_connected(T * psignalizable, void (T::*pfn)(signal_details *))
   {
      for(int32_t i = 0; i < m_delegateptra.get_size(); i++)
      {
         signal_delegate_instance < T > * pdelegate = m_delegateptra.typed_ptr_at < signal_delegate_instance < T > > (i);
         if(pdelegate != NULL && pdelegate->m_psignalizable == psignalizable && pdelegate->m_pfn == pfn)
            return true;
      }
      return false;
   }

   template < class T >
   bool disconnect(T * psignalizable,void (T::*pfn)(signal_details *))
   {
      for(int32_t i = 0; i < m_delegateptra.get_size(); i++)
      {
         signal_delegate_instance < T > * pdelegate = m_delegateptra.typed_ptr_at < signal_delegate_instance < T > >(i);
         if(pdelegate != NULL && pdelegate->m_psignalizable == psignalizable && pdelegate->m_pfn == pfn)
         {
            psignalizable->unregister_signal(this);
            m_delegateptra.remove_at(i);
            return true;
         }
      }
      return false;
   }


   void disconnect(signalizable * psignalizable);
   void leave_only(signalizable * psignalizable);

   void disconnect_all();


};


template < class T >
void connect(class signal & sig, T * p, void (T::*pfn)(signal_details *))
{

   sig.connect(p, pfn);

}


template < class T >
void signal::signal_delegate_instance<T>::emit(signal_details * pobject)
{

   (m_psignalizable->*m_pfn)(pobject);

}


template < class T >
bool signal::signal_delegate_instance<T>::operator == (const signal_delegate_instance<T> & deleg) const
{

   return m_psignalizable == deleg.m_psignalizable && m_pfn == deleg.m_pfn;

}


class CLASS_DECL_AURA signalid :
   virtual public object
{
public:


   virtual ~signalid();
   virtual bool is_equal(signalid * pid) = 0;
   virtual bool matches(signalid * pid) = 0;
   virtual signalid * copy() = 0;


};

class CLASS_DECL_AURA signalid_array :
   virtual public spa(signalid)
{
public:


   signalid_array();
   virtual ~signalid_array();


   signalid * get(signalid * pid);


};


class CLASS_DECL_AURA dispatch :
   virtual public ::object
{
public:


   class CLASS_DECL_AURA handler_item_base :
      virtual public object
   {
   public:


      virtual ~handler_item_base();

      virtual signalizable* get_signalizable() = 0;

   };


   template < class T >
   class handler_item :
      public handler_item_base
   {
   public:
      T *                     m_psignalizable;
      // Pointer to signal object directly associate with the
      // prototype.
      // This is a cached value and not the
      // storage holder of the object.
      virtual signalizable* get_signalizable() { return dynamic_cast < signalizable* > (m_psignalizable); }
   };


   class CLASS_DECL_AURA handler_item_array :
      public spa(handler_item_base)
   {
   public:
      virtual ~handler_item_array();
      bool HasSignalizable(signalizable* psignalizable);
   };


   class CLASS_DECL_AURA signal_item :
      virtual public ::object
   {
   public:


      sp(signalid)         m_pid;
      class ::signal *     m_psignal;

      handler_item_array   m_handlera;


      signal_item();
      virtual ~signal_item();


   };


   class CLASS_DECL_AURA signal_item_ptr_array :
      public spa(signal_item)
   {
   public:

      bool emit(signal_details * pobj);

   };


   class CLASS_DECL_AURA signal_item_array :
      public spa(signal_item)
   {
   public:


      void GetSignalsById(signal_item_ptr_array & signalptra, signalid * pid);
      signal_item * GetSignalById(signalid * pid);


   };


   void RemoveMessageHandler(signalizable* psignalizable);
   // Prototype_bool_WPARAM_LPARAM;

   template < class T >
   bool AddMessageHandler(
      signalid * pid,
      T * psignalizable,
      void (T::*pfn)(signal_details *),
      bool bAddUnique = true);



   int32_t                  m_iHandling;
   signal_item_array          m_signala;


   dispatch();
   virtual ~dispatch();


};


inline void dispatch::signal_item_array::GetSignalsById(dispatch::signal_item_ptr_array & signalptra, signalid * pid)
{

   for(int32_t i = 0; i < this->get_size(); i++)
   {

      signal_item & signal = *this->element_at(i);

      if (signal.m_pid.is_null())
      {

         continue;

      }

      if(signal.m_pid->matches(pid))
      {

         signalptra.add(&signal);

      }

   }

}


inline dispatch::signal_item * dispatch::signal_item_array::GetSignalById(signalid * pid)
{
   for(int32_t i = 0; i < this->get_size(); i++)
   {
      signal_item & signal = *this->element_at(i);
      if(signal.m_pid->matches(pid))
      {
         return &signal;
      }
   }
   return NULL;
}


#define DECL_GEN_SIGNAL(function) void function(signal_details * pobj)

//  turned that virtual signals members are disfunctional
// because signals are emitted to all registered handlers with abort semantics and even between different objects
// virtual members and signal dispatching messes this, I don't know exactacly in what way, but in a configusing way
// and even the signal dispatch sometimes seems to dispatch things to wrong functions more easily (bad handler->member matching).
// #define DECL_GEN_V_SIGNAL(function) virtual void function(signal_details * pobj)


#define BEG_GEN_SIGNAL(cl, function, signal_impl_class) void cl::function(signal_details * pobj) \
{ SCAST_PTR(signal_impl_class, pobj, psignal);
#define END_GEN_SIGNAL() }



