#pragma once


namespace user
{


   class list;


   class CLASS_DECL_CORE mesh_cache_interface :
      virtual public ::object
   {
   public:


      mesh_cache_interface(::aura::application * papp);
      virtual ~mesh_cache_interface();


      virtual void _001CacheHint(mesh * plist, index iItemStart, ::count nItemCount) = 0;
      virtual void _001GetItemText(::user::mesh_item * pitem) = 0;
      virtual void _001GetGroupText(::user::mesh_item * pitem);


   };


} // namespace user

