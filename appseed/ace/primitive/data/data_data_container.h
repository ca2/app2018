#pragma once


namespace data
{


   class CLASS_DECL_AURA data_container :
      virtual public ::object
   {
   public:


      sp(data)       m_spdata;


      data_container();
      virtual ~data_container();


      virtual bool set_data(::data::data * pdata);
      data * get_data();

      virtual bool is_data_locked() const;

      virtual void on_update_data(int32_t iHint);

   };

   class CLASS_DECL_AURA data_container_base :
      virtual public ::object
   {
   public:


      spa(data)       m_spadata;


      data_container_base(::ace::application * papp);
      virtual ~data_container_base();


      bool add_data(::data::data * pdata);
      bool remove_data(::data::data * pdata);
      data * get_data(index i);
      ::count get_count() const;

      bool is_empty() const;

      template < class TYPE >
      TYPE * get_typed_data(index i = 0)
      {
         
         return m_spadata.typed_ptr_at < TYPE >(i);

      }


      template < class TYPE >
      bool replace_data(TYPE * pdata)
      {

         TYPE * pderived = NULL;

         for(index i = 0; i < m_spadata.get_count(); i++)
         {

            try
            {

               pderived = m_spadata.typed_ptr_at < TYPE > (i);

            }
            catch(...)
            {

            }

            if(pderived != NULL)
            {

               retry_single_lock rsl1(pderived->m_pmutex, millis(1), millis(1));

               retry_single_lock rsl2(pdata->m_pmutex, millis(1), millis(1));

               m_spadata[i] = pdata;

               return true;

            }

         }

         m_spadata.add(pdata);

         return true;

      }


      template < class TYPE >
      bool replace_data(smart_pointer < TYPE > & data)
      {

         return replace_data(data.m_p);

      }

      object_spa get_sync();

   };



} // namespace data



