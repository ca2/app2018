#pragma once


using base_string_to_string = string_map < string,const string & > ;


using string_to_string  =  base_string_to_string;

using string_table  =  string_map < string_to_string *,string_to_string * >;

