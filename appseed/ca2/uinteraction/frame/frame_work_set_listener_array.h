#pragma once


namespace uinteraction
{


namespace frame
{

   class WorkSetListener;
   class WorkSet;

   class CLASS_DECL_ca2 WorkSetListenerArray :
      public spa(WorkSetListener)
   {
      friend class WorkSet;
      friend class WorkSetListener;
   public:
      virtual void on_delete(sp(::ca::ca) poc);
      void attach(WorkSetListener *plistener);
      //void add(WorkSetListener * plistener);
      WorkSetListenerArray();
      virtual ~WorkSetListenerArray();

      virtual void WFLOnSizingGripMove(
         WorkSet * pwf,
         sp(::user::interaction) pwnd,
         EGrip egrip);

      virtual void WFLOnDock(
         WorkSet * pwf,
         sp(::user::interaction) pwnd);

      virtual void WFLOnClose(
         WorkSet * pwf,
         sp(::user::interaction) pwnd);

      virtual void WFLOnNcLButtonDown(
         WorkSet * pworkset,
         sp(::user::interaction) pwnd,
         UINT nHitTest,
         point point);
   };

} //   namespace frame






} // namespace uinteraction






