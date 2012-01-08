﻿#include "StdAfx.h"


namespace webserver
{


   string AlphaSelectPerformerLinkMaker::code_getLink(const int64_array & iaClassId, const char * code)
   {
      UNREFERENCED_PARAMETER(iaClassId);
      UNREFERENCED_PARAMETER(code);
   //   $style = style;
   //   $lang = lang;
   //   $code = urlencode($code);
   //      handslist.php?list=".LISTID_CLASS_PERFORMER."&class=strClassId&lang=$lang&namecode=$code&style=$style
      //return "/r?" + LISTID_CLASS_PERFORMER + ."(as.$code).$lang.$style.".LISTID_PERFORMER_CLASS.".strClassId.";
      return "";

   }

   string AlphaSelectPerformerLinkMaker3::code_getLink(const int64_array & iaClassId, const char * code)
   {
   //   $style = style;
   //   $lang = lang;
   //   $code = urlencode($code);
   //      handslist.php?list=".LISTID_CLASS_PERFORMER."&class=strClassId&lang=$lang&namecode=$code&style=$style
      return ui_url("artists?cid=" + iaClassId.implode(",") + "&acode=" + string(code) + "&target=" + get("target"));
   }

   AlphaSelectPerformer::AlphaSelectPerformer()
   {
   
      m_iTopicType = gprop("g_music_topictype");

      construct();

   }


   AlphaSelectPerformer::AlphaSelectPerformer(int64_t topic)
   {
 
      m_strLangStyle = gprop("param_style");
      string strTopic(gen::str::itoa(topic));

      if(strTopic == "song")
      {
         m_iTopicType = 0;
      }
      else if(strTopic == "chord" || strTopic == "cifras")
      {
         m_iTopicType = 2;
      }
      else if(strTopic == "lyrics" || strTopic == "letras")
      {
         m_iTopicType = 1;
      }

      construct();

   }

   void AlphaSelectPerformer::construct()
   {

      m_kptitlea["a"] =unitext("ア");
      m_kptitlea["i"] = unitext("イ");
      m_kptitlea["u"] =unitext( "ウ");
      m_kptitlea["e"] = unitext("エ");
      m_kptitlea["o"] = unitext("オ");

      m_kptitlea["pa"] = unitext("パ");
      m_kptitlea["pi"] = unitext("ピ");
      m_kptitlea["pu"] = unitext("プ");
      m_kptitlea["pe"] = unitext("ペ");
      m_kptitlea["po"] = unitext("ポ");

      m_kptitlea["ba"] = unitext("バ");
      m_kptitlea["bi"] = unitext("ビ");
      m_kptitlea["bu"] = unitext("ブ");
      m_kptitlea["be"] = unitext("ベ");
      m_kptitlea["bo"] = unitext("ボ");

   }

   void AlphaSelectPerformer::out(int style)
   {
      stringa codea = code_getArray();
      code_printCells(codea, style);
   }

   stringa AlphaSelectPerformer::code_getArray(int style)
   {

      UNREFERENCED_PARAMETER(style);

      m_iaClassId.QuickSort();

      string strSql;

      if(m_iaClassId.has_elements())
      {

         string strClassId;
         strClassId = gen::str::i64toa(m_iaClassId[0]);
         string strParentClassIda = m_iaClassId.surround_and_implode(NULL, "<", ">");

         strSql.Format("SELECT codea FROM tmp_class_2 WHERE classid='%s' AND parentclassida='%s' AND topictype = %d", strClassId, strParentClassIda, m_iTopicType);

      }
      else
      {
         strSql.Format("SELECT DISTINCT codea FROM tmp_class_2 WHERE topictype = %d", m_iTopicType);
      }


      string strCodea = musicdb().query_item(strSql);


      stringa straCode;
      straCode.add_tokens(strCodea, ";", FALSE);
      stringa fullcodea = code_getFullArray(1);

      string strFull;
      stringa retcodea;
      for(int i = 0; i < fullcodea.get_count(); i++) // full base_array item
      {
         strFull = fullcodea[i];
         if(straCode.contains_ci(strFull))
         {
            retcodea.add(strFull);
         }
      }

      return retcodea;


   /*   $codea = base_array();
      $codea[] = "?";
      $codea[] = "0";
      $codea[] = "a";
      $codea[] = "b";
      $codea[] = "c";
      $codea[] = "d";
      $codea[] = "e";
      $codea[] = "f";
      $codea[] = "g";
      $codea[] = "h";
      $codea[] = "i";
      $codea[] = "j";
      $codea[] = "k";
      $codea[] = "l";
      $codea[] = "m";
      $codea[] = "n";
      $codea[] = "o";
      $codea[] = "p";
      $codea[] = "q";
      $codea[] = "r";
      $codea[] = "s";
      $codea[] = "t";
      $codea[] = "u";
      $codea[] = "v";
      $codea[] = "w";
      $codea[] = "x";
      $codea[] = "y";
      $codea[] = "z";
   /*      $codea[] = "kp-&#12490;"; //pi katakana
      $codea[] = "kp-&#12500;"; //pi katakana
      $codea[] = "kp-&#12521;"; //pi kata*/

   /*   $codea[] = "kp-a";
      $codea[] = "kp-i";
      $codea[] = "kp-u";
      $codea[] = "kp-e";
      $codea[] = "kp-o";

      $codea[] = "kp-ba";
      $codea[] = "kp-bi";
      $codea[] = "kp-bu";
      $codea[] = "kp-be";
      $codea[] = "kp-bo";

      $codea[] = "*"; // all

      if($style == 2)
      {
         $newcodea = base_array();
         foreach($codea as $code)
         {
            $count = code_getCount($code);
            if($count > 0)
            {
               $newcodea[] = $code;
            }
         }
         return $newcodea;
      }
      return $codea;*/
   }

   stringa AlphaSelectPerformer::code_getFullArray(int style)
   {
      stringa codea;
      if(m_strLangStyle == "ja" && style == 1)
      {
         codea.add("kp-a");
         codea.add("kp-i");
         codea.add("kp-u");
         codea.add("kp-e");
         codea.add("kp-o");

         codea.add("kp-ba");
         codea.add("kp-bi");
         codea.add("kp-bu");
         codea.add("kp-be");
         codea.add("kp-bo");

         codea.add("?");
         codea.add("0");
         codea.add("a");
         codea.add("b");
         codea.add("c");
         codea.add("d");
         codea.add("e");
         codea.add("f");
         codea.add("g");
         codea.add("h");
         codea.add("i");
         codea.add("j");
         codea.add("k");
         codea.add("l");
         codea.add("m");
         codea.add("n");
         codea.add("o");
         codea.add("p");
         codea.add("q");
         codea.add("r");
         codea.add("s");
         codea.add("t");
         codea.add("u");
         codea.add("v");
         codea.add("w");
         codea.add("x");
         codea.add("y");
         codea.add("z");
   /*      codea.add("kp-&#12490;"); //pi katakana
         codea.add("kp-&#12500;"); //pi katakana
         codea.add("kp-&#12521;"); //pi kata*/
         codea.add("*"); // all
      }
      else
      {
         codea.add("?");
         codea.add("0");
         codea.add("a");
         codea.add("b");
         codea.add("c");
         codea.add("d");
         codea.add("e");
         codea.add("f");
         codea.add("g");
         codea.add("h");
         codea.add("i");
         codea.add("j");
         codea.add("k");
         codea.add("l");
         codea.add("m");
         codea.add("n");
         codea.add("o");
         codea.add("p");
         codea.add("q");
         codea.add("r");
         codea.add("s");
         codea.add("t");
         codea.add("u");
         codea.add("v");
         codea.add("w");
         codea.add("x");
         codea.add("y");
         codea.add("z");
   /*      codea.add("kp-&#12490;"); //pi katakana
         codea.add("kp-&#12500;"); //pi katakana
         codea.add("kp-&#12521;"); //pi kata*/

         codea.add("kp-a");
         codea.add("kp-i");
         codea.add("kp-u");
         codea.add("kp-e");
         codea.add("kp-o");

         codea.add("kp-ba");
         codea.add("kp-bi");
         codea.add("kp-bu");
         codea.add("kp-be");
         codea.add("kp-bo");

         codea.add("*"); // all

      }
      if(style == 2)
      {
         stringa newcodea;
         for(int i = 0; i < codea.get_count(); i++)
         {
            int iCount = code_getCount(codea[i]);
            if(iCount > 0)
            {
               newcodea.add(codea[i]);
            }
         }
         return newcodea;
      }
      return codea;
   }

   void AlphaSelectPerformer::out_xml1()
   {
      code_printXml1Tree(code_getArray(), -1);
   }

   void AlphaSelectPerformer::print_td(const char * prefix, const char * pszTitle)
   {

      string strPrefix(prefix);
      string regexp;

      if(strPrefix == "?")
      {
         regexp = "^[^[:alpha:]]";
      }
      else
      {
         regexp = "^$prefix";
      }

      string strTitle;
      if(pszTitle == NULL)
      {
         strTitle = strPrefix;
      }
      else
      {
         strTitle = pszTitle;
      }

   //   int iTopicType = m_iTopicType;

      string strSql;
      if(m_iaClassId.has_elements())
      {
         strSql.Format("SELECT count(person.id) FROM person, topic_song, topic_class, topic_person WHERE person.id = topic_person.person AND topic_person.topic = topic_class.topic AND topic_person.topictype = topic_class.topictype AND topic_song.song = topic_class.song AND topic_class.class = %s AND person.name REGEXP '%s'",
            m_iaClassId.implode(","), regexp);
      }
      else
      {
         strSql.Format("SELECT count(id) FROM person WHERE name REGEXP '%s'",
            regexp);
      }

      var item = musicdb().query_item(strSql);

      if(item != false)
      {
         print("<td class = \"alphaSelect\">");
         print("<a class=\"alphaSelect\" href=\"javascript:$js('$regexp')\">");
         print(strTitle);
         print("</a>");
         print("</td>\n");
      }

   }

   int AlphaSelectPerformer::code_getCount(const char * pszCode)
   {
   
      string regexp = get_reg_exp(pszCode);

      int iTopicType = m_iTopicType;

      string tt_cond;

      if(iTopicType == 1)
      {
         tt_cond = "(topic_song.topictype = 1 OR topic_song.topictype = 1001)";
      }
      else
      {
         tt_cond = "topic_song.topictype = " + gen::str::itoa(iTopicType);
      }

      string where_add;

      if(regexp.Mid(0, 3) == "kp-")
      {
         where_add = "katakana_prefix REGEXP '" + regexp.Mid(3) + "'";
      }
      else
      {
         where_add = "name REGEXP '" + regexp + "'";
      }

      string classcondition;


      string strSql;
      strSql.Format(
         "SELECT COUNT(DISTINCT person.id) FROM person INNER JOIN topic_person ON topic_person.person = person.id INNER JOIN topic_song ON topic_person.topic = topic_song.topic AND topic_person.topictype = topic_song.topictype INNER JOIN tmp_song ON tmp_song.song = topic_song.song WHERE %s AND %s %s",
         tt_cond, where_add, classcondition);
      dprint(strSql);
      dprint("<br>");
      return musicdb().query_item(strSql);

   }


   void AlphaSelectPerformer::code_printCells(const stringa & codea, int style)
   {
      int iIndex = 0;
      ::count count;
      print("\n");
      if(style == 2)
      {
         if(strlen(get("acode")) > 0)
         {
            code_printCell(get("acode"), style, iIndex);
         }
         count = codea.get_count();
         int divcount = max(1, min(5, count / 4));
         int_array ia;
         int_array iaCount;
         int_array iaSum;
         int sum = 0;
   //         print "count=$count";
   //      int halfdiv = divcount / 2;
         int maxcount = 0;
         int iCount;
         for(int i = -divcount + 2; i < divcount  ; i += 2)
         {
            ia.add(sum);
            iCount = count / divcount + i;
            sum += iCount;
            iaCount.add(iCount);
            iaSum.add(sum);
            if(iCount > maxcount)
            {
               maxcount = iCount;
            }
         }
         ia.add(sum);
         iCount = count - sum;
         if(iCount > maxcount)
         {
            maxcount = iCount;
         }
         iaCount.add(iCount);
         iaSum.add(count);
         int index = 0;
         print("<table cellpadding=\"0\" cellspacing=\"0\">\n");
         print("<tbody>\n");
         int i;
         for(int irow = 0; irow < maxcount; irow++)
         {
            print("<tr>\n");
            for(int idiv = 0; idiv < divcount; idiv++)
            {
               print("<td>\n");
               i = ia[idiv];
               if(i < iaSum[idiv] && get("acode") != codea[i])
               {
                  code_printCell(codea[i], style, index);
               }
               else
               {
                  print("&nbsp;");
               }
                ia[idiv] = i + 1;
               print("</td>\n");
            }
            print("</tr>\n");
         }
         print("</tbody>\n");
         print("</table>\n");
         print("\n");
      }
      else if(style == 3)
      {
         if(strlen(get("acode")) > 0)
         {
            code_printCell(get("acode"), style, iIndex);
         }
         count = codea.get_count();
         int index = 0;
         int i = 0;
         print("<table width=\"100%\" cellpadding=\"0\" cellspacing=\"0\" class=\"as-table\">");
         print("<tbody>");
         print("<tr>");
         print("<td>");
         for(i = 0; i < count; i++)
         {
            if(get("acode") != codea[i])
            {
               code_printCell(codea[i], style, index);
            }
         }
         print("</td>");
         print("</tr>");
         print("</tbody>");
         print("</table>");
         print("\n");
      }
      else
      {
         if(strlen(get("acode")) > 0)
         {
            code_printCell(get("acode"), style, iIndex);
         }
         iIndex = 0;
   //      bool bOpen = false;
         for(int i = 0; i < codea.get_count(); i++)
         {
            string strCode = codea.element_at(i);
            if(get("acode") != strCode)
            {
               code_printCell(strCode, style, iIndex);
            }
         }
         print("\n");
      }

   }

   void AlphaSelectPerformer::code_printCell(const char * pszCode, int style, int & iIndex)
   {

      UNREFERENCED_PARAMETER(style);

      string strCode(pszCode);

      string strTitle = get_title(pszCode);

      //$count = code_getCount($code);
      string suffix3;
      string suffix2;
      string suffix1;
      string prefix3;
      string prefix2;
      string prefix1;
      string strIndex;
      strIndex.Format("%d", iIndex);

      if(strCode == get("acode"))
      {
         suffix3 = " as3-sel";
         suffix2 = " as2-sel";
         suffix1 = " as1-sel";
         print("<script>");
         print("var asid = '" + strIndex + "';");
         print("</script>");

      }

      string id1 = "as1" + strIndex;
      string id2 = "as2" + strIndex;
      string id3 = "as3" + strIndex;
   //      if($count > 0)
      {
         print("<a class=\"as\" href=\"" + m_plinkmaker->code_getLink(m_iaClassId, pszCode) +"\">");
   //         print "<div class="as3" id="$id3" onmouseover="asMOv(\"$id1\",\"$id2\",\"$id3\")" onmouseout="asMOu(\"$id1\",\"$id2\",\"$id3\")" onmousedown="asMDo(\"$id1\",\"$id2\",\"$id3\")" onmouseup="asMUp(\"$id1\",\"$id2\",\"$id3\")">";
   //         print "<div class="as3" id="$id3" onmouseover="asMOv($index)" onmouseout="asMOu($index)" onmousedown="asMDo($index)" onmouseup="asMUp($index)">";
      //   print "<div class="as2" id="$id2">";
         //print "<div class="as1" id="$id1">";

         print("<div class=\"" + prefix3 + "as3" + suffix3 + " id=\"" + id3 + "\" onmouseover=\"asMOv("+strIndex+")\" onmouseout=\"asMOu("+strIndex+")\" onmousedown=\"asMDo("+strIndex+")\" onmouseup=\"asMUp("+strIndex+")\">");
         print("<div class=\"" + prefix2 + "as2" + suffix2 + " id=\"" + id2 + "\">");
         print( "<div class=\"" + prefix1 + "as1" + suffix1 + " id=\"" + id1 + "\">");
         print (strTitle);
         print ("</div>");
         print ("</div>");
         print ("</div>");
         print ("</a>\n");
      }
   /*      else
      {
         print "<div class="as3-d" id="$id3">";
         print "<div class="as2-d" id="$id2">";
         print "<div class="as1-d" id="$id1">";
         print $title;
         print "</div>";
         print "</div>";
         print "</div>";
      }*/
      iIndex++;
   }


   void AlphaSelectPerformer::code_printXml1Tree(const stringa & codea, int style)
   {
      print("\n");
      int iIndex = 0;
      for(int i = 0; i < codea.get_count(); i++)
      {
         code_printXml1Element(codea[i], style, iIndex);
      }
      print("\n");

   }

   void AlphaSelectPerformer::code_printXml1Element(const char * pszCode, int style, int & iIndex)
   {
      UNREFERENCED_PARAMETER(style);
      string strCode(pszCode);

      string strTitle = get_title(pszCode);
   //      print "$title";

   //      $count = code_getCount($code);


   //      if($count > 0)
      {
         print("   <folder>\n");
         print("      <type>normal</type>\n");
         print("      <id>performersbycategory.category(" + m_iaClassId.implode(",") + ").regexp(\"" + strCode + "\")</id>\n");
         print("      <title>" + strTitle + "</title>\n");
         print("   </folder>\n");

   /*         $id1 = "as1$index";
         $id2 = "as2$index";
         $id3 = "as3$index";
         print "<a class="as" href="".linkMaker->code_getLink(strClassId, $code)."">";
   //         print "<div class="as3" id="$id3" onmouseover="asMOv(\"$id1\",\"$id2\",\"$id3\")" onmouseout="asMOu(\"$id1\",\"$id2\",\"$id3\")" onmousedown="asMDo(\"$id1\",\"$id2\",\"$id3\")" onmouseup="asMUp(\"$id1\",\"$id2\",\"$id3\")">";
   //         print "<div class="as3" id="$id3" onmouseover="asMOv($index)" onmouseout="asMOu($index)" onmousedown="asMDo($index)" onmouseup="asMUp($index)">";
      //   print "<div class="as2" id="$id2">";
         //print "<div class="as1" id="$id1">";
         print "<div class="as3" id="$id3" onmouseover="asMOv($index)" onmouseout="asMOu($index)" onmousedown="asMDo($index)" onmouseup="asMUp($index)">";
         print "<div class="as2" id="$id2">";
         print "<div class="as1" id="$id1">";
         print $title;
         print "</div>";
         print "</div>";
         print "</div>";
         print "</a>\n";*/
         iIndex++;
      }
   }

   void AlphaSelectPerformer::code_printSpan(const char * pszCode)
   {
      string strTitle = get_title(pszCode);

      //int count = code_getCount(pszCode);

      //if(count > 0)
      {
         print("<div class = \"alphaSelect\">");
         //print("<a class=\"alphaSelect\" href=\"javascript:".js."("".htmlspecialchars($code)."")\">");
         print(strTitle);
         print("</a>");
         print("</div>\n");
      }
   }



   string AlphaSelectPerformer::get_title(const char * pszCode)
   {
      string strCode(pszCode);
      strCode.make_lower();
      if(strCode.get_length() == 1 && isalpha(strCode[0]))
      {
         return strCode;
      }
      else if(gen::str::begins_eat(strCode, "kp-"))
      {
         return m_kptitlea[strCode];
      }
      else if(strCode == "0")
      {
         return "0-9";
      }
      else
      {
         return strCode;
      }
   }


   string AlphaSelectPerformer::get_reg_exp(const char * pszCode)
   {
      string strCode(pszCode);
      if(strCode == "*")
      {
         return ".*";
      }
      strCode.make_lower();
      if(strCode.get_length() == 1 && isalpha(strCode[0]))
      {
         return "^" + strCode;
      }
      else if(gen::str::begins_eat(strCode, "kp-"))
      {
         return "kp-^" + strCode;
      }
      else if(strCode == "?")
      {
         return "^[^[:alpha:]]";
      }
      else if(strCode == "0")
      {
            return "^[0-9]";
      }
      else
      {
         return "";
      }
   }


} // namespace webserver


