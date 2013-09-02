#pragma once


class CLASS_DECL_c element :
   virtual public base
{
public:


   enum flag
   {
      flag_auto_clean = 1 << 0,
      flag_discard_to_factory = 1 << 1,
      flag_ready_for_delete = 1 << 2,
      flag_auto_delete = 1 << 3,
      //flag_heap_alloc = 1 << 4,
      flag_locked = 1 << 5,
   };

   uint32_t                   m_ulFlags;
   sp(::application)      m_pcaapp;
   sp(::application)     m_papp;
   //::ca2::ptra *            m_pptraListener;
   //::ca2::ptra *            m_pptraListened;
   factory_item_base *        m_pfactoryitembase;
   void *                     m_pthis;


   element();
   element(const element & o);
   element(sp(::application) papp);
   virtual ~element();

   virtual void delete_this();


   virtual sp(element) clone();


   static void system(const char * pszProjectName);

   inline allocatorsp allocer();

   virtual sp(::application) get_app() const;
   virtual void set_app(sp(::application) papp);

   inline bool is_set_ca_flag(element::flag eflag)
   {
      return (m_ulFlags & ((uint32_t) eflag)) == (uint32_t) eflag;
   }

   inline void clear_ca_flag(element::flag eflag)
   {
      m_ulFlags &= ~eflag;
   }

   inline void set_ca_flag(element::flag eflag)
   {
      m_ulFlags |= eflag;
   }


   element & operator = (const element & o);


};


class CLASS_DECL_c allocator :
   virtual public element
{
public:


   virtual ~allocator();


};


class CLASS_DECL_c allocatorsp :
   public sp(allocator)
{
public:


   allocatorsp(sp(::application) papp);


};


