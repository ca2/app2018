#pragma once




template < typename PRED >
class forking_thread:
   virtual public thread
{
public:

   PRED m_pred;

   sp(object) m_pholdref;

   forking_thread(::aura::application * papp, sp(object) pholdref, PRED pred) :
      object(papp),
      thread(papp),
      m_pholdref(pholdref),
      m_pred(pred)
   {

      begin();

   }

   forking_thread(::aura::application * papp,PRED pred) :
      object(papp),
      thread(papp),
      m_pred(pred)
   {

      begin();

   }


   virtual ~forking_thread()
   {
   
   }

   int32_t run()
   {

      m_pred();

      return 0;

   }

};


template < typename PRED >
::thread * fork(::aura::application * papp, sp(object) pholdref, PRED pred)
{

   return new forking_thread < PRED >(papp, pholdref, pred);

}

template < typename PRED >
::thread * fork(::aura::application * papp,PRED pred)
{

   return new forking_thread < PRED >(papp,pred);

}

template < typename PRED >
inline void object::fork(PRED pred)
{

   ::fork(get_app(),pred);

}
