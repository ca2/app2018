#pragma once


class CLASS_DECL_ca2 bergedge_interface :
   virtual public plane::application,
   virtual public ::ca::session
{
public:


   virtual bool open_by_file_extension(sp(::ca::create_context) pcreatecontext);
   //virtual sp(::uinteraction::frame::frame) get_frame_schema(const char * pszLibrary, const char * pszFrameSchemaName);


   virtual void on_app_request_bergedge_callback(sp(::ca::application) papp);


//   virtual ::fontopus::user * login(::ca::property_set & set) = 0;

};




