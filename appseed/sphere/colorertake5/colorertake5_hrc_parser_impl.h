﻿#pragma once

namespace colorertake5
{

   class file_type_impl;

   /** Implementation of HRCParser.
       Reads and mantains HRC database of syntax rules,
       used by text_parser implementations to make
       realtime text syntax parsing.
       @ingroup colorer_parsers
   */
   class CLASS_DECL_SPHERE HRCParserImpl : public HRCParser
   {
   public:
      HRCParserImpl(::aura::application * papp);
      virtual ~HRCParserImpl();

      void setErrorHandler(ErrorHandler *eh);

      void loadSource(const char * pszSourceLocation, const char * pszSource);
      file_type *getFileType(const char *name);
      file_type *enumerateFileTypes(int32_t index);
      file_type *chooseFileType(const char *fileName, const char *firstLine, int32_t typeNo = 0);

      ::count getRegionCount();
      class region *getRegion(int32_t id);
      class region* getRegion(const char *name);

      string getVersion();

   protected:
      friend class file_type_impl;

      enum QualifyNameType { QNT_DEFINE, QNT_SCHEME, QNT_ENTITY };

      // types and packages
      string_map<file_type_impl *>       fileTypeHash;
      // types, not packages
      pointer_array < file_type_impl * >       fileTypeVector;

      string_map<scheme_impl *>          schemeHash;
      string_map<int32_t>                   disabledSchemes;

      pointer_array < region * >   regionNamesVector;
      string_map<region *>   regionNamesHash;
      string_to_string              schemeEntitiesHash;

      string versionName;

      file_type_impl *parseType;
      bool structureChanged;
      bool updateStarted;

      ::file::path m_strCurrentSourceLocation;
      string m_strCurrentSource;

      //DocumentBuilder docbuilder;
      ErrorHandler *errorHandler;

      void loadFileType(file_type *filetype);

      void parseHRC(const char * psz);
      void addPrototype(sp(::xml::node)elem);
      void addType(sp(::xml::node)elem);

      void addScheme(sp(::xml::node)elem);
      void addSchemeNodes(scheme_impl *scheme, sp(::xml::node)elem);

      void loadBlockRegions(SchemeNode *node, sp(::xml::node)el);
      void loadRegions(SchemeNode *node, sp(::xml::node)el, bool st);

      string qualifyOwnName(const char *name);
      bool checkNameExist(const char *name, file_type_impl *parseType, QualifyNameType qntype, bool logErrors);
      string qualifyForeignName(const char *name, QualifyNameType qntype, bool logErrors);

      void updateLinks();
      string useEntities(const char * name);
      class region *getNCRegion(sp(::xml::node)el, const char * tag);
      class region *getNCRegion(const char *name, bool logErrors);


   };


} // namespace colorertake5namespace colorertake5

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
 *  Eugene Efremov <4mirror@mail.ru>
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
