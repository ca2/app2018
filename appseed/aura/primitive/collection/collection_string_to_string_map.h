#pragma once


typedef CLASS_DECL_AURA string_map < string, const string & > base_string_to_string;


class CLASS_DECL_AURA string_to_string :
   virtual public attrib_map < base_string_to_string >
{
public:


   string_to_string(sp(::aura::application) papp = NULL, ::count nBlockSize = 10);
   string_to_string(const string_to_string & map);
	~string_to_string();


   string_to_string & operator = (const string_to_string & map);

};


class CLASS_DECL_AURA string_table :
   virtual public string_map < string_to_string *, string_to_string * >
{
public:


};





