﻿#ifndef CA2_APP_BASE_DATABASE_DEFINITION_H
#define CA2_APP_BASE_DATABASE_DEFINITION_H


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


   class CLASS_DECL_AXIS DbErrors :
      virtual public ::exception::exception
   {

   public:

      DbErrors(::aura::application * papp);
      DbErrors(::aura::application * papp, const char *msg, ...);

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




   class CLASS_DECL_AXIS field_properties :
      virtual public ::object
   {
   public:


      string            name;
      string            display_name;
      EDataType         type;
      string            field_table; //?
      bool              read_only;
      uint32_t      field_len;
      uint32_t      field_flags;
      int32_t               idx;


      field_properties();
      field_properties(const field_properties & field_properties);

      field_properties & operator = (const field_properties & field_properties);


      virtual void io(stream & stream);

   };


   class CLASS_DECL_AXIS field
   {
   public:


      field_properties  m_properties;
      var               m_value;



      field();
      field(const field & field);

      field & operator = (const field & field);


   };

   class CLASS_DECL_AXIS record :
      virtual public var_array
   {
   public:

      record();
      record(const record & record);
      virtual ~record();


      record & operator = (const record & record);

   };

   class CLASS_DECL_AXIS query_data :
      virtual public array < record, record & >,
      virtual public object
   {

      virtual void io(stream & stream) override;

   };

   typedef array <field, field &> CFields;
   typedef array < field_properties, field_properties & > record_properties;

   class CLASS_DECL_AXIS result_set :
      virtual public ::object
   {
   public:


      record_properties       m_record_header;
      query_data              m_records;


      result_set();
      result_set(::aura::application * papp);
      result_set(const result_set & set);


      virtual void io(stream & stream);


      result_set & operator = (const result_set & set);


   };

   class CLASS_DECL_AXIS parameter_list
   {
   public:


      stringa        m_stra;
      var_array      m_fieldvaluea;


      var & GetFieldValue(index i);
      const char * GetKey(index i);
      ::count get_size();
      bool is_empty();
      parameter_list & operator = (const parameter_list & paramlist);


   };


} // namespace database



#endif // CA2_APP_BASE_DATABASE_DEFINITION_H




