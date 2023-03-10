#include "framework.h" // previously aura/user/user.h


namespace colorertake5
{


   TextHRDMapper::TextHRDMapper(::aura::application * papp) :
      object(papp),
      RegionMapperImpl(papp)
   {
   }

   TextHRDMapper::~TextHRDMapper()
   {
   }

   /** Loads region definitions from HRD spfile->
   Multiple files could be loaded.
   */
   void TextHRDMapper::loadRegionMappings(serialize & is)
   {

      string str;
      is >> str;

      sp(::xml::document) hbase = new xml::document(m_papp);
      hbase->load(str);

      if (hbase->get_name() != "hrd")
      {
         _throw(exception(get_app(), string("Error loading HRD file")));
      };

      for(sp(::xml::node)curel = hbase->first_child(); curel; curel = curel->get_next_sibling())
      {
         if (curel->get_type() == xml::node_element && curel->get_name() == "assign")
         {
            string name = (curel)->attr("name");
            if(name.is_empty())
               continue;

            if (regionDefines[name] != NULL)
            {
               const TextRegion *rd = TextRegion::cast(regionDefines[name]);
//               delete rd->stext; delete rd->etext;
//               delete rd->sback; delete rd->eback;
               delete rd;
            }
            string stext;
            string etext;
            string sback;
            string eback;
            string sval;
            sval = (curel)->attr("stext");
            if (sval.has_char()) stext = (sval);
            sval = (curel)->attr("etext");
            if (sval.has_char()) etext = (sval);
            sval = (curel)->attr("sback");
            if (sval.has_char()) sback = (sval);
            sval = (curel)->attr("eback");
            if (sval.has_char()) eback = (sval);

            RegionDefine *rdef = new TextRegion(stext, etext, sback, eback);
            regionDefines.set_at(name, rdef);
         };
      };
   };

   /** Writes all currently loaded region definitions into
   XML spfile-> Note, that this method writes all loaded
   defines from all loaded HRD files.
   */
   void TextHRDMapper::saveRegionMappings(serialize & writer) const
   {
      writer << string("<?xml version=\"1.0\"?>\n\
                       <!DOCTYPE hrd SYSTEM \"../hrd.dtd\">\n\n\
                       <hrd>\n");
      /*  for(string *key = regionDefines.enumerateKey(); key; key=regionDefines.nextkey()){
      const TextRegion *rdef = TextRegion::cast(regionDefines.get(key));
      writer->write(string("  <define name='")+key+"'");
      if (rdef->stext != NULL) writer->write(string(" stext='")+rdef->stext+"'");
      if (rdef->etext != NULL) writer->write(string(" etext='")+rdef->etext+"'");
      if (rdef->sback != NULL) writer->write(string(" sback='")+rdef->sback+"'");
      if (rdef->eback != NULL) writer->write(string(" eback='")+rdef->eback+"'");
      writer->write(string("/>\n"));
      };*/
      //  writer->write(string("\n</hrd>\n"));
   };

   /** Adds or replaces region definition */
   void TextHRDMapper::setRegionDefine(const char * name, RegionDefine *rd)
   {
      const TextRegion *rd_new = TextRegion::cast(rd);
      string stext;
      string etext;
      string sback;
      string eback;
      if (rd_new->stext.has_char()) stext = (rd_new->stext);
      if (rd_new->etext.has_char()) etext = (rd_new->etext);
      if (rd_new->sback.has_char()) sback = (rd_new->sback);
      if (rd_new->eback .has_char()) eback = (rd_new->eback);

      RegionDefine *rd_old = regionDefines[name];
      if (rd_old != NULL)
      {
         const TextRegion *rdef = TextRegion::cast(rd_old);
//         delete rdef->stext; delete rdef->etext;
         //       delete rdef->sback; delete rdef->eback;
         delete rdef;
      };

      RegionDefine *new_region = new TextRegion(stext, etext, sback, eback);
      regionDefines.set_at(name, new_region);

      // Searches and replaces old region references
      for(int32_t idx = 0; idx < regionDefinesVector.get_size(); idx++)
         if (regionDefinesVector.element_at(idx) == rd_old)
         {
            regionDefinesVector.set_at(idx, new_region);
            break;
         };
   };


} // namespace colorertake5


/* ***** BEGIN LICENSE BLOCK *****
* Version: MPL 1.1/GPL 2.0/LGPL 2.1
*
* The contents of this file are subject to the Mozilla Public License Version
* 1.1 (the "License"); you may not use this file except in compliance with
* the License. You may obtain a copy of the License at
* http://www.mozilla.org/MPL/
*
* Software distributed under the License is distributed on an "AS IS" basis,
* WITHOUT WARRANTY OF ANY KIND, either express or implied. See the License
* for the specific language governing rights and limitations under the
* License.
*
* The Original Code is the Colorer Library.
*
* The Initial Developer of the Original Code is
* Cail Lomecb <cail@nm.ru>.
* Portions created by the Initial Developer are Copyright (C) 1999-2005
* the Initial Developer.
*
* Contributor(s):
*
* Alternatively, the contents of this file may be used under the terms of
* either the GNU General Public License Version 2 or later (the "GPL"), or
* the GNU Lesser General Public License Version 2.1 or later (the "LGPL"),
* in which case the provisions of the GPL or the LGPL are applicable instead
* of those above. If you wish to allow use of your version of this file only
* under the terms of either the GPL or the LGPL, and not to allow others to
* use your version of this file under the terms of the MPL, indicate your
* decision by deleting the provisions above and replace them with the notice
* and other provisions required by the GPL or the LGPL. If you do not delete
* the provisions above, a recipient may use your version of this file under
* the terms of any one of the MPL, the GPL or the LGPL.
*
* ***** END LICENSE BLOCK ***** */


