/**
 * @file thlang.cxx
 */
  
/* Copyright (C) 2000 Stacho Mudrak
 * 
 * $Date: $
 * $RCSfile: $
 * $Revision: $
 *
 * -------------------------------------------------------------------- 
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 2 of the License, or
 * any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 * 
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software
 * Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA  02111-1307  USA
 * --------------------------------------------------------------------
 */
 
#include "thlang.h"
#include "thparse.h"
#include "thlangdata.cxx"
#include "thinit.h"
#include "thdatabase.h"
#include "thexception.h"

int thlang_parse(char * str) {
  return thmatch_token(str, thtt_lang);
}

char * thlang_getid(int id) {
  return thmatch_string(id, thtt_lang);
}

char * thlang_getcxxid(int id) {
  if (id < 0)
    return "THLANG_UNKNOWN";
  else
    return thlang__cxxids[id];
}

const thstok * thlang_get_text_table()
{
  return &(thtt__texts[0]);
}

void thlang_set_translation(char * lang, char * text, char * translation) {
  
  int lang_id;
  lang_id = thlang_parse(lang);
  if (lang_id == THLANG_UNKNOWN)
    ththrow(("unknown language -- %s", lang));
  int text_id;
  text_id = thmatch_token(text, thtt__texts);
  if (text_id == -1)
    ththrow(("unknown text -- %s", text))
  thlang__translations[text_id][lang_id] = thdb.strstore(translation);

}

char * thT(char * txt, int lng) {
  if (lng == THLANG_SYSTEM)
    return txt;
  int sv = thmatch_token(txt,thtt__texts);
  char * trans;
  if (sv == -1)
    return txt;
  lng = thlang_getlang(lng);
  // najde ci ho mame v danom jazyku
  trans = thlang__translations[sv][lng];
  if ((trans == NULL) && (thlang__alternatives[lng] != THLANG_UNKNOWN)) {
    trans = thlang__translations[sv][thlang__alternatives[lng]];
  }
  if (trans != NULL)
    return trans;
  else
    return txt;
}

int thlang_getlang(int lng) {
  if (lng == THLANG_UNKNOWN)
    lng = thini.lang;
  if (lng == THLANG_UNKNOWN)
    lng = THLANG_EN;
  return lng;
}

