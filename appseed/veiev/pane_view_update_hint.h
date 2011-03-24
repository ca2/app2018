#pragma once


namespace veiev
{

enum EPaneView
{
   PaneViewNone,
   PaneViewContextMenu,
   PaneViewPrimaryCommand,
   PaneViewSecondCommand,
   PaneViewFileManager,
   PaneViewThreeActionLaunch,
   PaneViewConfiguration,
};


class CLASS_DECL_CA2_VEIEV pane_view_update_hint :
   public ::radix::object  
{
public:
   enum e_type
   {
      TypeGetView,
      TypeSetView,
      TypeOnShowKaraoke,
      TypeOnShowView,
   };
   // DECLARE_DYNAMIC(pane_view_update_hint)
public:
	void set_type(e_type e_type);
	bool is_type_of(e_type e_type);

	pane_view_update_hint();
	virtual ~pane_view_update_hint();

   EPaneView m_eview;
protected:
   e_type m_etype;

};

} // namespace veiev
