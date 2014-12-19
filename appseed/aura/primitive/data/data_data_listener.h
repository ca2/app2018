#pragma once


namespace data
{

   class data;


   class CLASS_DECL_AURA listener :
      virtual public ::object
   {
   public:


      ptr_array < data > m_dataptra;


      listener(::aura::application * papp);
      virtual ~listener();


      bool listen(data * pdata, bool bListen = true);


      virtual void on_update_data(data * pdata, int32_t iHint);

   };




} // namespace data




