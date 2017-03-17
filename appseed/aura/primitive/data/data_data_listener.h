#pragma once


namespace data
{

   class data;


   class CLASS_DECL_AURA listener :
      virtual public ::object
   {
   public:


      ref_array < data > m_dataptra;


      listener();
      virtual ~listener();


      bool listen(data * pdata, bool bListen = true);


      virtual void on_update_data(data * pdata, int32_t iHint);

   };

   template < class VALUE, class ARG_VALUE = const VALUE &, class PAIR = pair < listener *, VALUE > >
   using listener_map = map < listener *, listener *, VALUE, ARG_VALUE, PAIR >;


} // namespace data




