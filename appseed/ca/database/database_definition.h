#pragma once


#include "database_stringhelper.h"


#define S_NO_CONNECTION "No active connection";

#define DB_BUFF_MAX           8*1024    // Maximum buffer's capacity

#define DB_CONNECTION_NONE   0
#define DB_CONNECTION_OK   1
#define DB_CONNECTION_BAD   2

#define DB_COMMAND_OK      0   // OK - command executed
#define DB_EMPTY_QUERY      1   // Query didn't return tuples
#define DB_TUPLES_OK      2   // Query returned tuples
#define DB_ERROR      5
#define DB_BAD_RESPONSE      6
#define DB_UNEXPECTED      7   // This shouldn't ever happen
#define DB_UNEXPECTED_RESULT   -1       //For integer functions


namespace database
{


   /******************** Class DbErrors definition *********************

               error handling

   ******************************************************************/
   class CLASS_DECL_ca DbErrors
   {

   public:

   /* constructor */
     DbErrors();
     DbErrors(const char *msg, ...);

   };

   enum dsStates { dsSelect, dsInsert, dsEdit, dsUpdate, dsDelete, dsInactive };
   enum sqlType {sqlSelect,sqlUpdate,sqlInsert,sqlDelete,sqlExec};

   enum EDataType
   {
      DataTypeString,
      DataTypeBoolean,
      DataTypeChar,
      DataTypeWChar,
      DataTypeWideString,
      DataTypeShort,
      DataTypeUShort,
      DataTypeLong,
      DataTypeI64,
      DataTypeULong,
      DataTypeFloat,
      DataTypeDouble,
      DataTypeLongDouble,
      DataTypeObject
   };




   class CLASS_DECL_ca field_properties :
      virtual public ::ex1::byte_serializable
   {
   public:


      string            name;
      string            display_name;
      EDataType         type;
      string            field_table; //?
      bool              read_only;
      unsigned int      field_len;
      unsigned int      field_flags;
      int               idx;


      field_properties();
      field_properties(const field_properties & field_properties);

      field_properties & operator = (const field_properties & field_properties);


      virtual void write(::ex1::byte_output_stream & ostream);
      virtual void read(::ex1::byte_input_stream & istream);

   };

   class CLASS_DECL_ca field
   {
   public:


      field_properties  m_properties;
      var               m_value;



      field();
      field(const field & field);

      field & operator = (const field & field);


   };

   class CLASS_DECL_ca record :
      virtual public var_array
   {
   public:

      record();
      record(const record & record);
      virtual ~record();


      record & operator = (const record & record);

   };

   class CLASS_DECL_ca query_data :
      public ::ex1::byte_serializable_array < base_array < record, record & > >
   {
   };

   typedef base_array <field, field &> CFields;
   typedef ::ex1::byte_serializable_array < base_array < field_properties, field_properties & > > record_properties;

   class CLASS_DECL_ca result_set :
      virtual public ::ex1::byte_serializable
   {
   public:


      record_properties       record_header;
      query_data              records;


      result_set();
      result_set(::ca::application * papp);
      result_set(const result_set & set);


      virtual void write(::ex1::byte_output_stream & ostream);
      virtual void read(::ex1::byte_input_stream & istream);


      result_set & operator = (const result_set & set);


   };

   class CLASS_DECL_ca parameter_list
   {
   public:


      stringa        m_stra;
      var_array      m_fieldvaluea;


      var & GetFieldValue(index i);
      const char * GetKey(index i);
      count get_size();
      bool is_empty();
      parameter_list & operator = (const parameter_list & paramlist);


   };


} // namespace database


