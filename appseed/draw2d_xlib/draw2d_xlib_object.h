#pragma once


namespace draw2d_xlib
{


   class CLASS_DECL_DRAW2D_XLIB object :
      virtual public ::draw2d::object
   {
   public:


      object();
      virtual ~object();


      virtual void * get_os_data() const;


      bool delete_object();


      int32_t get_object(int32_t nCount, LPVOID lpObject) const;
      UINT GetObjectType() const;
      bool CreateStockObject(int32_t nIndex);
      bool UnrealizeObject();
      //bool operator==(const object& obj) const;
      //bool operator!=(const object& obj) const;

      virtual void dump(dump_context & dumpcontext) const;
      virtual void assert_valid() const;

   };


} // namespace draw2d_xlib
