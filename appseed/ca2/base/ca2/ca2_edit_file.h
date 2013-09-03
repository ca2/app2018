#pragma once


namespace ca2
{


   class CLASS_DECL_ca2 edit_file :
      public ::ca2::filesp,
      public ::ca2::tree,
      public ::ca2::tree_data
   {
   public:


      enum EItemType
      {
         ItemTypeUndefined,
         ItemTypeDelete,
         ItemTypeEdit,
         ItemTypeInsert,
         ItemTypeGroup
      };

      class Item : public tree_item_data
      {
      public:

         file_position m_dwPosition;


         Item();

         virtual UINT read_ch(edit_file * pfile);

         virtual file_position get_position(bool bForward);

         virtual BYTE * get_data() ;
         virtual EItemType get_type();
         virtual ::primitive::memory_size get_extent();
         virtual ::primitive::memory_size get_file_extent();
         virtual BYTE * reverse_get_data();
         virtual EItemType reverse_get_type();
         virtual ::primitive::memory_size reverse_get_extent() ;
         virtual ::primitive::memory_size reverse_get_file_extent() ;
         virtual ::primitive::memory_size get_extent(bool bForward);
         virtual EItemType get_type(bool bForward);
         virtual ::primitive::memory_size get_file_extent(bool bForward);
         virtual BYTE * get_data(bool bForward);
         virtual ::primitive::memory_offset get_delta_length() ;

#undef new
         DECLARE_AND_IMPLEMENT_DEFAULT_ALLOCATION
#define new DEBUG_NEW


      };

      class DeleteItem : public Item
      {
      public:


         primitive::memory m_memstorage;


         virtual UINT read_ch(edit_file * pfile);

         virtual EItemType get_type();
         virtual ::primitive::memory_size get_extent() ;
         virtual ::primitive::memory_size get_file_extent();
         virtual BYTE * get_data() ;
         virtual EItemType reverse_get_type() ;
         virtual ::primitive::memory_size reverse_get_extent();
         virtual ::primitive::memory_size reverse_get_file_extent();
         virtual BYTE * reverse_get_data();

         virtual ::primitive::memory_offset get_delta_length() ;
      };

      class InsertItem : public Item
      {
      public:


         primitive::memory m_memstorage;


         virtual UINT read_ch(edit_file * pfile);

         virtual EItemType get_type() ;
         virtual ::primitive::memory_size get_extent() ;
         virtual ::primitive::memory_size get_file_extent() ;
         virtual BYTE * get_data() ;
         virtual EItemType reverse_get_type() ;
         virtual ::primitive::memory_size reverse_get_extent() ;
         virtual ::primitive::memory_size reverse_get_file_extent() ;
         virtual BYTE * reverse_get_data();
         virtual ::primitive::memory_offset get_delta_length();

      };

      class EditItem : public Item
      {
      public:


         primitive::memory m_memstorage;
         primitive::memory m_memstorageReverse;


         virtual EItemType get_type();
         virtual ::primitive::memory_size get_extent() ;
         virtual ::primitive::memory_size get_file_extent();
         virtual BYTE * get_data() ;
         virtual EItemType reverse_get_type();
         virtual ::primitive::memory_size reverse_get_extent() ;
         virtual ::primitive::memory_size reverse_get_file_extent();
         virtual BYTE * reverse_get_data() ;
         virtual ::primitive::memory_offset get_delta_length();

      };

      class ItemPtrArray : public array < sp(Item), sp(Item) >
      {
      public:
      };

      class GroupItem : public Item, public ItemPtrArray
      {
      public:


         GroupItem * m_pgroupitem;


         virtual EItemType get_type();
         virtual ::primitive::memory_size get_extent();
         virtual ::primitive::memory_size get_file_extent();
         virtual BYTE * get_data() ;
         virtual EItemType reverse_get_type();
         virtual ::primitive::memory_size reverse_get_extent() ;
         virtual ::primitive::memory_size reverse_get_file_extent();
         virtual BYTE * reverse_get_data() ;
         virtual ::primitive::memory_offset get_delta_length();

         virtual UINT read_ch(edit_file * pfile);

#undef new
         DECLARE_AND_IMPLEMENT_DEFAULT_ALLOCATION
#define new DEBUG_NEW

      };

      file_position           m_dwPosition;
      file_size               m_dwInternalFilePosition;
      file_size               m_dwFileLength;
      file_position           m_dwReadPosition;
      file_offset             m_iCurItem;


      //ItemPtrArray         m_itemptra;
      //tree_data            m_tree;
      sp(::ca2::tree_item)   m_ptreeitem;
      sp(::ca2::tree_item)   m_ptreeitemFlush;
      ::primitive::memory_offset              m_iBranch;
      sp(::file::file)          m_pfile;
      GroupItem *          m_pgroupitem;
      bool                 m_bRootDirection;

      void SetFile(sp(::file::file) pfile);

      void FillFilePosition(sp(Item) pitem);

      edit_file(sp(base_application) papp);
      virtual ~edit_file();

      virtual bool IsValid() const;
      //void load_string(string & str);
      //void create(::primitive::memory_offset iSize = -1);
      //void seek_to_begin();
      file_position seek(file_offset lOff, ::file::e_seek  nFrom);
      file_position get_position() const;

      //virtual string GetFilePath() const;

      file_size get_length() const;
      //void Truncate(::primitive::memory_offset iPosition);
      //void clear();
      virtual ::primitive::memory_size read(void *lpBuf, ::primitive::memory_size nCount);
      virtual void write(const void * lpBuf, ::primitive::memory_size nCount);
      virtual void Insert(const void * lpBuf, ::primitive::memory_size nCount);
      virtual void Delete(::primitive::memory_size uiCount);



      void flush();
      bool SaveTo(::file::byte_output_stream & file);
      bool Save(::file::file & file);
      bool Save_N_to_CRLF(::file::file & file);


      void Insert(DeleteItem * pitem);
      void Insert(EditItem * pitem);
      void Insert(InsertItem * pitem);
      bool CanUndo();
      bool CanRedo();
      ::count GetRedoBranchCount();
      bool Undo();
      bool Redo();
      void MacroBegin();
      void MacroEnd();


      sp(::ca2::tree_item) get_previous(sp(::ca2::tree_item) pitem);
      sp(::ca2::tree_item) get_next(sp(::ca2::tree_item) pitem, bool bChild = true);

      virtual sp(tree_item_data) on_allocate_item();
      //virtual void on_delete_item(sp(tree_item_data) pitem);


   protected:


      void TreeInsert(sp(Item) pitem);
      bool calc_root_direction();


   private:


      edit_file(const edit_file & ) :
         ::ca2::tree(NULL),
         ::ca2::tree_data(NULL)
      {
      }


   };


} // namespace ca2


