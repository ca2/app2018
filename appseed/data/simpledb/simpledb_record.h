#pragma once


namespace simpledb
{

   class CLASS_DECL_CORE record_row : 
      virtual public ::object
   {
   public:

      table *     m_ptable;
      stringa     m_straFields;
      var         m_var;

      virtual void write(::file::output_stream & ostream) const;
      virtual void read(::file::input_stream & ostream);

   };


} // namespace simpledb


