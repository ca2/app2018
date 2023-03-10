#pragma once


namespace visual
{


   class CLASS_DECL_AURA icon :
      virtual public ::object
   {
   public:

      void *                        m_picon;
      map < size, size, void * >    m_iconmap;
      bool                          m_bAutoDelete;
      string                        m_strAppTrayIcon;
      size                          m_size;
      ::draw2d::dibmap              m_dibmap;

      icon(::aura::application * papp);
      icon(::aura::application * papp, void * picon, bool bAttach = true);
#ifdef WINDOWS
      icon(::aura::application * papp, HICON hicon, bool bAttach = true);
#endif
      virtual ~icon();


      operator void *();
#ifdef WINDOWS
      operator HICON();
#endif

      bool load_file(string strPath);
      bool load_matter(string strPath);
      bool load_app_tray_icon(string strPath);


      ::size get_size();

      virtual void on_update_icon();

      ::draw2d::dib * get_dib(int cx, int cy);

   };


   class CLASS_DECL_AURA icon_int_map :
      virtual public int_map < icon * >
   {
   public:

   };


} // namespace visual


#ifdef WINDOWSEX

CLASS_DECL_AURA HICON load_icon(::aura::application * papp, stringa & straMatter, string strIcon, int cx, int cy);

#endif

