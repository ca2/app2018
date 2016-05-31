#pragma once


namespace ace
{


   class CLASS_DECL_AURA live_signal :
      virtual public object
   {
   public:

      object_spa       m_ptraLive;

      live_signal();
      live_signal(const live_signal & signal);

      void keep(object * pliveobject);
      void unkeep(object * pliveobject);

      virtual void on_keep_alive();
      virtual bool is_alive();

      live_signal & operator = (const live_signal & signal);

   };


} // namespace ace










