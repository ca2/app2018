#pragma once


class string_lang_map
{
public:

   int      m_iPo;
   string   m_strLanguageCode;

};



class CLASS_DECL_AURA builtin_strlangmap :
   virtual public ::object
{
public:


   STRLANGMAP *                     m_pmap;
   string_map < string_lang_map >   m_pomap;
   stringa                          m_straLang;
   string                           m_strLang;
   string_map < string_to_string >  m_text;
   string_map < string_to_string >  m_header;
   string_to_string                 m_po;


   builtin_strlangmap(::aura::application * papp, STRLANGMAP * pmap);
   virtual ~builtin_strlangmap();


   virtual bool process_init();

   virtual string get_current_language();

   virtual bool set_current_language(string strLang);
   virtual bool set_current_language(index iSel);


   virtual index get_language_list(stringa & stra);


   virtual bool _load_text(string strLang);
   virtual string _get_text(string strLang, string str);

   virtual string __get_text(string str);


};


