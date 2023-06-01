/*============================---(source-start)---============================*/
#include "gregg.h"
#include "gregg_priv.h"

/*
 *
 * metis § tn4·· § verbose dictionary parsing mode to find mistakes                       § N2O4rp §  · §
 * metis § dc4·· § add verification of gregg letters entry                                § N2U1OS §  · §
 */

/*
 *  PREFIXES
 *     il      illogical, illegible
 *     in      inanimate, incalculable, inadequate, inaccessable
 *     un   
 *     re      restructure
 *     co      cohabitate
 *     im      immaturity, impartial, immobilize
 *     mis     misread, misbehave, misdirect, misquote
 *     on      onset 
 *
 *     under
 *     over
 *
 *     macro
 *     micro
 *
 *     extra
 *     mega, tera, kilo, centa, ...
 *
 *     mono
 *     uni
 *     multi
 *     omni
 *
 *     non
 *     out
 *     pan   panhellenic, pandemic
 *     para  parable, parade, parapet, paranoid, parboil, parameter
 *
 *     per
 *     pre
 *     pro
 *
 *     vade         e vs. in vs. per   evade, invade, pervade         (latin for "to go")
 *     scribe       describe, subscribe, prescribe, ascribe, transcribe, inscribe, circumscribe proscribe
 *     suade        persuade, dissuade
 *     tain         maintain, obtain, contain, retain, sustain, attain, entertain, pertain, abstain, distain
 *     verse        reverse, adverse, diverse, converse, traverse, perverse, obverse
 *     version      conversion, diversion, inversion, aversion, subversion, reversion, perversion, introversion, extroversion
 *     vert         convert, obert, covert, divert, revert, avert, subvert, invert, pervert, advert, introvert, extrovert
 *     turb         disturb, preturb
 *     use          refuse, excuse, reuse, misuse, diffuse, confuse, fuse, accuse, overuse, underuse, infuse, disuse, defuse, peruse
 *     spect        respect, aspect, suspect, prospect, inspect, retrospect, circumspect, introspect
 *     sist         resist, assist, consist, insist, persist, desist, subsist, synthesist
 *     mute         commute, transmute, permute, telecommute
 *     jury         injury, perjury
 *     haps         perhaps, mishaps
 *     fect         infect, perfect, affect, effect, defect, refect, confect  (latin/english "make/do")
 *     fort         comfort, effort                                           (latin/english "strong")
 *     sect         insect, disect, intersect                                 (latin/english "cut)
 *     cult         agricult, horticulte, cultivate                           (latin/english "grow")
 *     dur          endure, obdure,                                           (latin/english "harden")
 *     rupt         disrupt, ruture, interrupt, bankrupt                      (latin/english "burst")
 *     ject         object, reject, project, inject, conjecture, deject, subject, interject, trajectory   (latin/english "throw")
 *
 */

static char    s_name   [LEN_PATH]  = "/var/lib/gregg/gregg_manual.dict";

char   s_fields  [MAX_FIELD][LEN_TITLE];
char   s_nfield  = 0;






/*
 * PURPOSE     manage the full dictionary of english words
 *
 */






/*====================------------------------------------====================*/
/*===----                        program level                         ----===*/
/*====================------------------------------------====================*/
static void  o___PROGRAM_________o () { return; }

char
DICT_init               (void)
{
   /*---(locals)-----------+-----+-----+-*/
   char        rce         =  -10;
   char        rc          =    0;
   /*---(header)-------------------------*/
   DEBUG_CONF   yLOG_enter   (__FUNCTION__);
   /*---(initialize)---------------------*/
   rc = ySORT_btree (B_DICT, "dictionary");
   DEBUG_CONF   yLOG_value   ("dict"   , rc);
   --rce;  if (rc < 0) {
      DEBUG_CONF   yLOG_exitr   (__FUNCTION__, rce);
      return rce;
   }
   /*---(complete)-----------------------*/
   DEBUG_CONF   yLOG_exit    (__FUNCTION__);
   return 0;
}

char
DICT__purge             (void)
{
   /*---(locals)-----------+-----+-----+-*/
   char        rce         =  -10;
   char        rc          =    0;
   /*---(header)-------------------------*/
   DEBUG_CONF   yLOG_enter   (__FUNCTION__);
   /*---(complete)-----------------------*/
   DEBUG_CONF   yLOG_exit    (__FUNCTION__);
   return 0;
}

char
DICT_wrap               (void)
{
   /*---(locals)-----------+-----+-----+-*/
   char        rce         =  -10;
   char        rc          =    0;
   /*---(header)-------------------------*/
   DEBUG_CONF   yLOG_enter   (__FUNCTION__);
   /*---(complete)-----------------------*/
   DEBUG_CONF   yLOG_exit    (__FUNCTION__);
   return 0;
}



/*====================------------------------------------====================*/
/*===----                      memory allocation                       ----===*/
/*====================------------------------------------====================*/
static void  o___MEMORY__________o () { return; }

char
DICT__wipe              (void *a_dict)
{
   /*---(locals)-----------+-----+-----+-*/
   int         i           =    0;
   tDICT      *x_dict      = NULL;
   /*---(cast)---------------------------*/
   x_dict = (tDICT *) a_dict;
   /*---(header)-------------------------*/
   x_dict->d_english [0] = '\0';
   x_dict->d_gregg   [0] = '\0';
   /*---(pointers)-----------------------*/
   x_dict->d_prefix  = NULL;
   x_dict->d_base    = NULL;
   x_dict->d_suffix  = NULL;
   /*---(within base)----------*/
   x_dict->d_prev    = NULL;
   x_dict->d_next    = NULL;
   /*---(ysort)----------------*/
   x_dict->d_ysort   = NULL;
   /*---(complete)-------------*/
   return 1;
}

char  DICT__new            (void **r_new)  { return SHARED_new  (B_DICT, '-', sizeof (tDICT), DICT__wipe, r_new); }
char  DICT__force          (void **r_new)  { return SHARED_new  (B_DICT, 'y', sizeof (tDICT), DICT__wipe, r_new); }
char  DICT__free           (void **r_old)  { return SHARED_free (B_DICT, r_old); }



/*====================------------------------------------====================*/
/*===----                    hooking and unhooking                     ----===*/
/*====================------------------------------------====================*/
static void  o___HOOKING_________o () { return; }

char
DICT__hook              (void *a_new)
{
   /*---(locals)-----------+-----+-----+-*/
   char        rce         =  -10;
   char        rc          =    0;
   tDICT      *x_new       = NULL;
   /*---(header)-------------------------*/
   DEBUG_CONF   yLOG_enter   (__FUNCTION__);
   /*---(cast)---------------------------*/
   x_new  = (tDICT *) a_new;
   /*---(base by english)----------------*/
   rc = ySORT_hook (B_DICT, x_new, x_new->d_english, &(x_new->d_ysort));
   DEBUG_CONF   yLOG_value   ("hook"      , rc);
   --rce;  if (rc < 0) {
      DEBUG_CONF   yLOG_exitr   (__FUNCTION__, rce);
      return rce;
   }
   rc = ySORT_prepare (B_DICT);
   DEBUG_CONF   yLOG_value   ("resort"    , rc);
   --rce;  if (rc < 0) {
      DEBUG_CONF   yLOG_exitr   (__FUNCTION__, rce);
      return rce;
   }
   /*---(complete)-----------------------*/
   DEBUG_CONF   yLOG_exit    (__FUNCTION__);
   return 0;
}

char
DICT__unhook            (void *a_old)
{
   /*---(locals)-----------+-----+-----+-*/
   char        rce         =  -10;
   char        rc          =    0;
   tDICT      *x_old       = NULL;
   /*---(header)-------------------------*/
   DEBUG_CONF   yLOG_enter   (__FUNCTION__);
   /*---(cast)---------------------------*/
   x_old  = (tDICT *) a_old;
   /*---(english)------------------------*/
   rc = ySORT_unhook (&(x_old->d_ysort));
   DEBUG_CONF   yLOG_value   ("un-base"   , rc);
   --rce;  if (rc < 0) {
      DEBUG_CONF   yLOG_exitr   (__FUNCTION__, rce);
      return rce;
   }
   rc = ySORT_prepare (B_DICT);
   DEBUG_CONF   yLOG_value   ("resort"    , rc);
   --rce;  if (rc < 0) {
      DEBUG_CONF   yLOG_exitr   (__FUNCTION__, rce);
      return rce;
   }
   /*---(complete)-----------------------*/
   DEBUG_CONF   yLOG_exit    (__FUNCTION__);
   return 0;
}



/*====================------------------------------------====================*/
/*===----                        searching                             ----===*/
/*====================------------------------------------====================*/
static void  o___SEARCH__________o () { return; }

short  DICT__count         (void)                            { return ySORT_count     (B_DICT); }
char   DICT__by_english    (char *a_text, void **r_dict)     { return ySORT_by_name   (B_DICT, a_text, r_dict); }
char   DICT__by_index      (int n, void **r_dict)            { return ySORT_by_index  (B_DICT, n, r_dict); }
char   DICT__by_cursor     (char a_dir, void **r_dict)       { return ySORT_by_cursor (B_DICT, a_dir, r_dict); }



/*====================------------------------------------====================*/
/*===----                         debugging                            ----===*/
/*====================------------------------------------====================*/
static void  o___DEBUGGING_______o () { return; }

char
DICT__detail            (void *a_dict, char a_out [LEN_FULL])
{
   /*---(locals)-----------+-----+-----+-*/
   char        rce         =  -10;
   tDICT      *x_dict      = NULL;
   char        s           [LEN_TITLE] = " ·åæ";
   char        t           [LEN_TITLE] = " ·åæ";
   char        u           [LEN_DESC]  = " ·åæ";
   char        v           [LEN_TITLE] = "·";
   char        w           [LEN_TITLE] = "·";
   /*---(defense)------------------------*/
   --rce;  if (a_out  == NULL)  return rce;
   strcpy (a_out, "");
   if (a_dict == NULL)          return 0;
   /*---(cast)---------------------------*/
   x_dict  = (tDICT *) a_dict;
   /*---(prepare)------------------------*/
   sprintf  (s, "%2då%.20sæ", strlen (x_dict->d_english), x_dict->d_english);
   sprintf  (t, "%2då%.20sæ", strlen (x_dict->d_gregg)  , x_dict->d_gregg);
   sprintf  (u, "%-3.3sp %-3.3sb %-3.3ss", (x_dict->d_prefix == NULL) ? "···" : "SET", (x_dict->d_base == NULL) ? "···" : "SET", (x_dict->d_suffix == NULL) ? "···" : "SET");
   if (x_dict->d_prev != NULL)  strlcpy (v, x_dict->d_prev->d_english, LEN_TITLE);
   if (x_dict->d_next != NULL)  strlcpy (w, x_dict->d_next->d_english, LEN_TITLE);
   /*---(consolidate)--------------------*/
   sprintf (a_out, "%-24.24s  %-24.24s ´ %-14.14s ´ %-20.20s ´ %-20.20s ´", s, t, u, v, w);
   /*---(complete)-----------------------*/
   return 0;
}

char*
DICT__pointer           (short n, void *a_dict)
{
   char        t           [LEN_FULL]  = "";
   DICT__detail (a_dict, t);
   if (strcmp (g_print, "") == 0)  strcpy  (g_print, "n/a");
   else                            sprintf (g_print, "%-5d %s", n, t);
   return g_print;
}

char*
DICT__entry             (int n)
{
   tDICT      *x_dict      = NULL;
   char        t           [LEN_FULL]  = "";
   DICT__by_index (n, &x_dict);
   DICT__detail (x_dict, t);
   if (strcmp (t, "") == 0)  strcpy  (g_print, "n/a");
   else                      sprintf (g_print, "%-5d %s", n, t);
   return g_print;
}



/*====================------------------------------------====================*/
/*===----                        main driver                           ----===*/
/*====================------------------------------------====================*/
static void  o___DRIVER__________o () { return; }

char
DICT_create             (cchar a_english [LEN_TITLE], cchar a_gregg [LEN_TITLE], void *a_prefix, void *a_base, void *a_suffix, void **r_dict)
{
   /*---(locals)-----------+-----------+-*/
   char        rce         =  -10;
   char        rc          =    0;
   tDICT      *x_new       = NULL;
   /*---(begin)--------------------------*/
   DEBUG_CONF   yLOG_enter   (__FUNCTION__);
   /*---(defense)------------------------*/
   DEBUG_CONF   yLOG_point   ("a_english" , a_english);
   --rce;  if (a_english == NULL || a_english [0] == '\0') {
      DEBUG_CONF   yLOG_exitr   (__FUNCTION__, rce);
      return rce;
   }
   DEBUG_CONF   yLOG_info    ("a_english" , a_english);
   DEBUG_CONF   yLOG_point   ("a_gregg"   , a_gregg);
   --rce;  if (a_gregg   == NULL || a_gregg   [0] == '\0') {
      DEBUG_CONF   yLOG_exitr   (__FUNCTION__, rce);
      return rce;
   }
   DEBUG_CONF   yLOG_info    ("a_english" , a_english);
   DEBUG_CONF   yLOG_point   ("a_base"    , a_base);
   --rce;  if (a_base    == NULL) {
      DEBUG_CONF   yLOG_exitr   (__FUNCTION__, rce);
      return rce;
   }
   /*---(new base)-----------------------*/
   rc = DICT__new      (&x_new);
   DEBUG_CONF   yLOG_value   ("new"       , rc);
   --rce;  if (rc < 0) {
      DEBUG_CONF   yLOG_exitr   (__FUNCTION__, rce);
      return rce;
   }
   /*---(key data)-----------------------*/
   strlcpy (x_new->d_english, a_english, LEN_TITLE);
   strlcpy (x_new->d_gregg  , a_gregg  , LEN_TITLE);
   x_new->d_prefix  = a_prefix;
   x_new->d_base    = a_base;
   x_new->d_suffix  = a_suffix;
   /*---(sort)---------------------------*/
   rc = BASE_add_dict  (a_base, x_new);
   DEBUG_CONF   yLOG_value   ("add dict"  , rc);
   --rce;  if (rc < 0) {
      DICT__free     (&x_new);
      DEBUG_CONF   yLOG_exitr   (__FUNCTION__, rce);
      return rce;
   }
   /*---(sort)---------------------------*/
   rc = DICT__hook     (x_new);
   DEBUG_CONF   yLOG_value   ("hook"      , rc);
   --rce;  if (rc < 0) {
      DICT__free     (&x_new);
      DEBUG_CONF   yLOG_exitr   (__FUNCTION__, rce);
      return rce;
   }
   /*---(save-back)----------------------*/
   if (r_dict != NULL)  *r_dict = x_new;
   /*---(complete)-----------------------*/
   DEBUG_CONF   yLOG_exit    (__FUNCTION__);
   return 0;
}



/*============================--------------------============================*/
/*===----                          dictionary                          ----===*/
/*============================--------------------============================*/
static void      o___DICTIONARY______________o (void) {;}

char
DICT__split             (uchar *a_recd)
{
   /*---(locals)-----------+-----+-----+-*/
   char        rce         =  -10;
   char        rc          =    0;
   int         i           =    0;
   char       *p           = NULL;
   char       *q           = "§";
   char       *r           = NULL;
   uchar       x_recd      [LEN_RECD]  = "";
   char       *m           = NULL;
   /*---(header)-------------------------*/
   DEBUG_CONF   yLOG_enter   (__FUNCTION__);
   /*---(prepare)------------------------*/
   for (i = 0; i < MAX_FIELD; ++i)   strcpy (s_fields [i], "");
   s_nfield = 0;
   /*---(defense)------------------------*/
   DEBUG_CONF   yLOG_point   ("a_recd"    , a_recd);
   --rce;  if (a_recd == NULL) {
      DEBUG_CONF   yLOG_exitr   (__FUNCTION__, rce);
      return rce;
   }
   /*---(parse)--------------------------*/
   strlcpy (x_recd, a_recd, LEN_RECD);
   p = strtok_r (x_recd, q, &r);
   while (p != NULL) {
      strlcpy  (s_fields [s_nfield], p, LEN_TITLE);
      m = strchr (s_fields [s_nfield], '¬');
      if (m != NULL)  m [0] = '\0';
      strltrim (s_fields [s_nfield], ySTR_BOTH, LEN_TITLE);
      if (s_nfield == 0)  strllower (s_fields [s_nfield], LEN_TITLE);
      ++s_nfield;
      p = strtok_r (NULL  , q, &r);
   }
   /*---(check count)--------------------*/
   --rce;  if (s_nfield < 2) {
      DEBUG_CONF   yLOG_exitr   (__FUNCTION__, rce);
      return rce;
   }
   /*---(complete)-----------------------*/
   DEBUG_CONF   yLOG_exit    (__FUNCTION__);
   return 0;
}

char
DICT__base              (short a_line, cchar a_recd [LEN_RECD], char r_english [LEN_TITLE], char r_gregg [LEN_TITLE], void **r_base)
{
   /*---(locals)-----------+-----+-----+-*/
   char        rce         =  -10;
   char        rc          =    0;
   char        x_revised   =  '-';
   /*---(header)-------------------------*/
   DEBUG_CONF   yLOG_enter   (__FUNCTION__);
   /*---(default)------------------------*/
   if (r_english != NULL)  strcpy (r_english, "");
   if (r_gregg   != NULL)  strcpy (r_gregg  , "");
   if (r_base    != NULL)  *r_base = NULL;
   /*---(defense)------------------------*/
   DEBUG_CONF   yLOG_point   ("a_recd"    , a_recd);
   --rce;  if (a_recd    == NULL) {
      DEBUG_CONF   yLOG_exitr   (__FUNCTION__, rce);
      return rce;
   }
   DEBUG_CONF   yLOG_info    ("a_recd"    , a_recd);
   DEBUG_CONF   yLOG_point   ("r_english" , r_english);
   --rce;  if (r_english == NULL) {
      DEBUG_CONF   yLOG_exitr   (__FUNCTION__, rce);
      return rce;
   }
   DEBUG_CONF   yLOG_point   ("r_gregg"   , r_gregg);
   --rce;  if (r_gregg   == NULL) {
      DEBUG_CONF   yLOG_exitr   (__FUNCTION__, rce);
      return rce;
   }
   /*---(split)--------------------------*/
   rc = DICT__split (a_recd);
   DEBUG_CONF   yLOG_value   ("split"     , rc);
   --rce;  if (rc < 0) {
      DEBUG_CONF   yLOG_exitr   (__FUNCTION__, rce);
      return rce;
   }
   /*---(prepare base words)-------------*/
   strlcpy (r_english, s_fields [0], LEN_TITLE);
   if (s_nfield >= 5 && (strcmp (s_fields [4], "·") != 0 && strcmp (s_fields [4], "") != 0))   x_revised = 'y';
   if (x_revised != 'y')  strlcpy (r_gregg, s_fields [2], LEN_TITLE);
   else                   strlcpy (r_gregg, s_fields [4], LEN_TITLE);
   DEBUG_CONF   yLOG_complex ("gregg base", "%c, %-20.20s, %s", x_revised, r_english, r_gregg);
   /*---(base, sans-prefix)--------------*/
   rc = BASE_create (my.r_nfile, a_line, r_english, r_gregg, s_fields [3], r_base);
   DEBUG_CONF   yLOG_value   ("base"      , rc);
   --rce;  if (rc < 0) {
      DEBUG_CONF   yLOG_exitr   (__FUNCTION__, rce);
      return rce;
   }
   /*---(base dictionary entry)----------*/
   rc = DICT_create (r_english, r_gregg, NULL, *r_base, NULL, NULL);
   DEBUG_CONF   yLOG_value   ("root"      , rc);
   --rce;  if (rc < 0) {
      DEBUG_CONF   yLOG_exitr   (__FUNCTION__, rce);
      return rce;
   }
   /*---(complete)-----------------------*/
   DEBUG_CONF   yLOG_exit    (__FUNCTION__);
   return 0;
}

char
DICT__primary           (short a_line, cchar a_english [LEN_TITLE], cchar a_gregg [LEN_TITLE], cchar a_cats [LEN_TITLE], tWORD **r_word)
{
   /*---(locals)-----------+-----+-----+-*/
   char        rce         =  -10;
   char        rc          =    0;
   char        x_gregg     [LEN_TITLE] = "";
   tWORD      *x_new       = NULL;
   char        x_part      =  '¢';
   /*---(header)-------------------------*/
   DEBUG_CONF   yLOG_enter   (__FUNCTION__);
   /*---(defense)------------------------*/
   DEBUG_CONF   yLOG_point   ("a_english" , a_english);
   --rce;  if (a_english == NULL) {
      yURG_err ('w', "%d primary english word is null/empty", a_line);
      DEBUG_CONF   yLOG_exitr   (__FUNCTION__, rce);
      return rce;
   }
   DEBUG_CONF   yLOG_info    ("a_english" , a_english);
   DEBUG_CONF   yLOG_point   ("a_gregg"   , a_gregg);
   --rce;  if (a_gregg   == NULL) {
      yURG_err ('w', "%d primary gregg word is null/empty", a_line);
      DEBUG_CONF   yLOG_exitr   (__FUNCTION__, rce);
      return rce;
   }
   /*---(check gregg)--------------------*/
   if (a_gregg [0] == '>')   strlcpy (x_gregg, a_gregg + 2, LEN_TITLE);
   else                      strlcpy (x_gregg, a_gregg    , LEN_TITLE);
   strldchg (x_gregg, '.', '·' , LEN_TITLE);
   DEBUG_CONF   yLOG_info    ("x_gregg"   , x_gregg);
   /*---(preview part of speech)---------*/
   /*> x_part = DICT__category_preview (a_cats);                                      <*/
   /*---(add word)-----------------------*/
   rc = WORDS_new_base (a_english, x_gregg, x_part, &x_new);
   DEBUG_CONF   yLOG_value   ("new"       , rc);
   --rce;  if (rc < 0) {
      yURG_err ('w', "%d primary word å%sæ could not be created", a_line, a_english);
      DEBUG_CONF   yLOG_exitr   (__FUNCTION__, rce);
      return rce;
   }
   /*---(hook to lists)------------------*/
   rc = WORDS_hook (x_new);
   DEBUG_CONF   yLOG_value   ("hook"      , rc);
   --rce;  if (rc < 0) {
      DEBUG_CONF   yLOG_exitr   (__FUNCTION__, rce);
      return rce;
   }
   /*---(save-back)----------------------*/
   x_new->w_file = my.r_nfile;
   x_new->w_line = a_line;
   if (r_word != NULL)  *r_word = x_new;
   /*---(status)-------------------------*/
   yURG_msg ('-', "%d primary word å%sæ å%sæ successful", a_line, a_english, x_gregg);
   /*---(complete)-----------------------*/
   DEBUG_CONF   yLOG_exit    (__FUNCTION__);
   return 0;
}

/*> char                                                                              <* 
 *> DICT__category_five     (tWORD *a_new, char l, cchar *a_cats)                     <* 
 *> {                                                                                 <* 
 *>    /+---(header)-------------------------+/                                       <* 
 *>    DEBUG_CONF   yLOG_enter   (__FUNCTION__);                                      <* 
 *>    /+---(part-of-speech)-----------------+/                                       <* 
 *>    if (l >= 1)  a_new->w_part = CATS__find_speech (a_cats [0]);                   <* 
 *>    else         a_new->w_part = CATS__find_speech ('¢');                          <* 
 *>    /+---(sub part-of-speech)-------------+/                                       <* 
 *>    if (l >= 3)  a_new->w_sub  = CATS__find_sub    (a_new->w_part, a_cats [2]);    <* 
 *>    else         a_new->w_sub  = CATS__find_sub    (a_new->w_part, '¢');           <* 
 *>    /+---(source group)-------------------+/                                       <* 
 *>    if (l >= 5)  a_new->w_grp  = CATS__find_grp    (a_cats [4]);                   <* 
 *>    else         a_new->w_grp  = CATS__find_grp    ('¢');                          <* 
 *>    /+---(source)-------------------------+/                                       <* 
 *>    if (l >= 7)  a_new->w_src  = CATS__find_source (a_cats [6]);                   <* 
 *>    else         a_new->w_src  = CATS__find_source ('¢');                          <* 
 *>    /+---(category)-----------------------+/                                       <* 
 *>    if (l >= 9)  a_new->w_cat  = CATS__find_type   (a_cats [8]);                   <* 
 *>    else         a_new->w_cat  = CATS__find_type   ('¢');                          <* 
 *>    /+---(complete)-----------------------+/                                       <* 
 *>    DEBUG_CONF   yLOG_exit    (__FUNCTION__);                                      <* 
 *>    return 0;                                                                      <* 
 *> }                                                                                 <*/

/*> char                                                                              <* 
 *> DICT__category_six      (tWORD *a_new, char l, cchar *a_cats)                     <* 
 *> {                                                                                 <* 
 *>    /+---(locals)-----------+-----+-----+-+/                                       <* 
 *>    short       x_page      =    0;                                                <* 
 *>    /+---(header)-------------------------+/                                       <* 
 *>    DEBUG_CONF   yLOG_enter   (__FUNCTION__);                                      <* 
 *>    /+---(part-of-speech)-----------------+/                                       <* 
 *>    if (l >= 1)  a_new->w_part = CATS__find_speech (a_cats [0]);                   <* 
 *>    else         a_new->w_part = CATS__find_speech ('¢');                          <* 
 *>    /+---(sub part-of-speech)-------------+/                                       <* 
 *>    if (l >= 3)  a_new->w_sub  = CATS__find_sub    (a_new->w_part, a_cats [2]);    <* 
 *>    else         a_new->w_sub  = CATS__find_sub    (a_new->w_part, '¢');           <* 
 *>    /+---(source group)-------------------+/                                       <* 
 *>    if (l >= 5)  a_new->w_grp  = CATS__find_grp    (a_cats [4]);                   <* 
 *>    else         a_new->w_grp  = CATS__find_grp    ('¢');                          <* 
 *>    /+---(source)-------------------------+/                                       <* 
 *>    if (l >= 8)  a_new->w_src  = CATS__find_source (a_cats [7]);                   <* 
 *>    else         a_new->w_src  = CATS__find_source ('¢');                          <* 
 *>    /+---(category)-----------------------+/                                       <* 
 *>    if (l >= 10) a_new->w_cat  = CATS__find_type   (a_cats [9]);                   <* 
 *>    else         a_new->w_cat  = CATS__find_type   ('¢');                          <* 
 *>    /+---(page)---------------------------+/                                       <* 
 *>    if (l >= 12) a_new->w_page = atoi (a_cats + 11);                               <* 
 *>    /+---(complete)-----------------------+/                                       <* 
 *>    DEBUG_CONF   yLOG_exit    (__FUNCTION__);                                      <* 
 *>    return 0;                                                                      <* 
 *> }                                                                                 <*/

/*> char                                                                              <* 
 *> DICT__category_seven    (tWORD *a_new, char l, cchar *a_cats)                     <* 
 *> {                                                                                 <* 
 *>    /+---(locals)-----------+-----+-----+-+/                                       <* 
 *>    char        t           [LEN_TERSE] = "";                                      <* 
 *>    /+---(header)-------------------------+/                                       <* 
 *>    DEBUG_CONF   yLOG_enter   (__FUNCTION__);                                      <* 
 *>    /+---(part-of-speech)-----------------+/                                       <* 
 *>    if (l >= 1)  a_new->w_part = CATS__find_speech (a_cats [0]);                   <* 
 *>    else         a_new->w_part = CATS__find_speech ('¢');                          <* 
 *>    if (l >= 3)  a_new->w_sub  = CATS__find_sub    (a_new->w_part, a_cats [2]);    <* 
 *>    else         a_new->w_sub  = CATS__find_sub    (a_new->w_part, '¢');           <* 
 *>    /+---(source)-------------------------+/                                       <* 
 *>    if (l >= 6)  a_new->w_src  = CATS__find_source (a_cats [5]);                   <* 
 *>    else         a_new->w_src  = CATS__find_source ('¢');                          <* 
 *>    if (l >= 8)  a_new->w_cat  = CATS__find_type   (a_cats [7]);                   <* 
 *>    else         a_new->w_cat  = CATS__find_type   ('¢');                          <* 
 *>    if (l >= 10) {                                                                 <* 
 *>       sprintf (t, "%-3.3s", a_cats + 9);                                          <* 
 *>       a_new->w_page = atoi (t);                                                   <* 
 *>    }                                                                              <* 
 *>    /+---(frequency)----------------------+/                                       <* 
 *>    if (l >= 14) a_new->w_grp  = CATS__find_grp    (a_cats [13]);                  <* 
 *>    else         a_new->w_grp  = CATS__find_grp    ('¢');                          <* 
 *>    if (l >= 16) a_new->w_freq = atoi (a_cats + 15);                               <* 
 *>    /+---(complete)-----------------------+/                                       <* 
 *>    DEBUG_CONF   yLOG_exit    (__FUNCTION__);                                      <* 
 *>    return 0;                                                                      <* 
 *> }                                                                                 <*/

/*> char                                                                              <* 
 *> DICT__category_preview  (cchar *a_cats)                                           <* 
 *> {                                                                                 <* 
 *>    /+---(locals)-----------+-----+-----+-+/                                       <* 
 *>    char        rce         =  -10;                                                <* 
 *>    char        l           =    0;                                                <* 
 *>    /+---(defenses)-----------------------+/                                       <* 
 *>    if (a_cats == NULL)  return '¢';                                               <* 
 *>    /+---(defenses)-----------------------+/                                       <* 
 *>    l = strlen (a_cats);                                                           <* 
 *>    if (l >= 1)  return CATS__find_speech (a_cats [0]);                            <* 
 *>    /+---(complete)-----------------------+/                                       <* 
 *>    return '¢';                                                                    <* 
 *> }                                                                                 <*/

/*> char                                                                              <* 
 *> DICT__category          (tWORD *a_new, cchar *a_cats)                             <* 
 *> {                                                                                 <* 
 *>    /+---(locals)-----------+-----+-----+-+/                                       <* 
 *>    char        rce         =  -10;                                                <* 
 *>    char        rc          =    0;                                                <* 
 *>    char        l           =    0;                                                <* 
 *>    /+---(header)-------------------------+/                                       <* 
 *>    DEBUG_CONF   yLOG_enter   (__FUNCTION__);                                      <* 
 *>    /+---(defenses)-----------------------+/                                       <* 
 *>    DEBUG_CONF   yLOG_point   ("a_new"     , a_new);                               <* 
 *>    --rce;  if (a_new == NULL) {                                                   <* 
 *>       DEBUG_CONF   yLOG_exitr   (__FUNCTION__, rce);                              <* 
 *>       return rce;                                                                 <* 
 *>    }                                                                              <* 
 *>    DEBUG_CONF   yLOG_point   ("a_cats"    , a_cats);                              <* 
 *>    --rce;  if (a_cats == NULL) {                                                  <* 
 *>       DEBUG_CONF   yLOG_exitr   (__FUNCTION__, rce);                              <* 
 *>       return rce;                                                                 <* 
 *>    }                                                                              <* 
 *>    l = strlen (a_cats);                                                           <* 
 *>    /+---(part-of-speech)-----------------+/                                       <* 
 *>    if (l <= 9) {                                                                  <* 
 *>       DICT__category_five  (a_new, l, a_cats);                                    <* 
 *>    } else if (l <= 13) {                                                          <* 
 *>       DICT__category_six   (a_new, l, a_cats);                                    <* 
 *>    } else {                                                                       <* 
 *>       DICT__category_seven (a_new, l, a_cats);                                    <* 
 *>    }                                                                              <* 
 *>    /+---(complete)-----------------------+/                                       <* 
 *>    DEBUG_CONF   yLOG_exit    (__FUNCTION__);                                      <* 
 *>    return 0;                                                                      <* 
 *> }                                                                                 <*/

char
DICT__variation_quick   (tWORD *a_base, tWORD *a_last, char a_english [LEN_TITLE], char a_vary [LEN_TERSE], char a_prefix [LEN_TERSE], char a_suffix [LEN_TERSE], tWORD **r_new)
{
   /*---(locals)-----------+-----+-----+-*/
   char        rce         =  -10;
   char        rc          =    0;
   char        x_gregg     [LEN_TITLE] = "";
   tWORD      *x_new       = NULL;
   /*---(header)-------------------------*/
   DEBUG_CONF   yLOG_enter   (__FUNCTION__);
   /*---(build gregg)--------------------*/
   sprintf (x_gregg, "%s·%s", a_base->w_gregg, a_suffix);
   DEBUG_CONF   yLOG_info    ("x_gregg"   , x_gregg);
   /*---(add word)-----------------------*/
   rc = WORDS_new_vary (a_english, x_gregg, a_base->w_part, &x_new);
   DEBUG_CONF   yLOG_value   ("new"       , rc);
   --rce;  if (rc < 0) {
      DEBUG_CONF   yLOG_exitr   (__FUNCTION__, rce);
      return rce;
   }
   /*---(hook to lists)------------------*/
   rc = WORDS_hook (x_new);
   DEBUG_CONF   yLOG_value   ("hook"      , rc);
   --rce;  if (rc < 0) {
      DEBUG_CONF   yLOG_exitr   (__FUNCTION__, rce);
      return rce;
   }
   /*---(carry-over)---------------------*/
   x_new->w_file = a_base->w_file;
   x_new->w_line = a_base->w_line;
   strlcpy (x_new->w_vary, a_vary, LEN_TERSE);
   /*---(past-of-speech)-----------------*/
   x_new->w_part = a_base->w_part;
   x_new->w_sub  = a_base->w_sub;
   /*---(source)-------------------------*/
   x_new->w_src  = a_base->w_src;
   x_new->w_cat  = a_base->w_cat;
   x_new->w_page = a_base->w_page;
   /*---(frequency)----------------------*/
   x_new->w_grp  = a_base->w_grp;
   x_new->w_freq = a_base->w_freq;
   /*---(connect to chain)---------------*/
   x_new->w_base = a_base;
   ++(x_new->w_base->w_nvary);
   if (a_last != NULL)  a_last->w_next = x_new;
   /*---(save-back)----------------------*/
   if (r_new  != NULL)  *r_new = x_new;
   /*---(complete)-----------------------*/
   DEBUG_CONF   yLOG_exit    (__FUNCTION__);
   return 0;
}


char
DICT__variation         (tWORD *a_base, tWORD *a_last, cchar a_english [LEN_TERSE], cchar a_gregg [LEN_TERSE], cchar *a_vary, tWORD **r_new)
{
   /*---(locals)-----------+-----+-----+-*/
   char        rce         =  -10;
   char        rc          =    0;
   char        x_action    =  '-';
   char       *p           = NULL;
   char       *q           =  ",";
   char       *r           = NULL;
   char        l           =    0;
   char        x_left      =    0;
   char        x_type      [LEN_TERSE]  = "";
   char        x_english   [LEN_TITLE]  = "";
   char        t           [LEN_TITLE]  = "";
   char        x_suffix    [LEN_LABEL] = "";
   char        x_base      [LEN_LABEL] = "";
   char        x_change    [LEN_LABEL] = "";
   tWORD     *x_vary      = NULL;
   tWORD     *x_save      = NULL;
   uchar       x_check     [LEN_LABEL] = "";
   /*---(header)-------------------------*/
   DEBUG_CONF   yLOG_enter   (__FUNCTION__);
   /*---(defenses)-----------------------*/
   DEBUG_CONF   yLOG_point   ("a_base"    , a_base);
   --rce;  if (a_base == NULL) {
      DEBUG_CONF   yLOG_exitr   (__FUNCTION__, rce);
      return rce;
   }
   DEBUG_CONF   yLOG_point   ("a_vary"    , a_vary);
   --rce;  if (a_vary == NULL) {
      DEBUG_CONF   yLOG_exitr   (__FUNCTION__, rce);
      return rce;
   }
   DEBUG_CONF   yLOG_info    ("a_vary"    , a_vary);
   /*---(parse)--------------------------*/
   p = strchr (a_vary, ')');
   DEBUG_CONF   yLOG_point   ("paren"     , p);
   --rce;  if (p == NULL) {
      DEBUG_CONF   yLOG_exitr   (__FUNCTION__, rce);
      return rce;
   }
   l = p - a_vary;
   DEBUG_CONF   yLOG_value   ("l"         , l);
   --rce;  if (l < 1 || l > 4) {
      DEBUG_CONF   yLOG_exitr   (__FUNCTION__, rce);
      return rce;
   }
   sprintf (x_type   , "%*.*s", l, l, a_vary);
   l = strlen (a_vary);
   x_left = l - (p - a_vary) - 2;
   DEBUG_CONF   yLOG_value   ("l"         , l);
   DEBUG_CONF   yLOG_value   ("x_left"    , x_left);
   --rce;  if (x_left < 2) {
      DEBUG_CONF   yLOG_exitr   (__FUNCTION__, rce);
      return rce;
   }
   sprintf (x_english, "%s%s"   , a_english, p + 2);
   /*---(find variation)-----------------*/
   /*> x_action = DICT__find_variation (x_type, x_suffix, x_endings, x_base, x_change);   <*/
   rc = SUFFIX_by_name (x_type, &x_action, x_suffix, x_base, x_change, NULL);
   DEBUG_CONF   yLOG_value   ("suffix"    , rc);
   --rce;  if (rc < 0) {
      DEBUG_CONF   yLOG_exitr   (__FUNCTION__, rce);
      return rce;
   }
   /*---(simple)-------------------------*/
   if (x_action == '·') {
      rc = DICT__variation_quick (a_base, a_last, x_english, x_type, a_gregg, x_suffix, &x_vary);
      if (rc >= 0)  x_save = x_vary;
   }
   /*---(also version)-------------------*/
   else if (x_action == 'a') {
      /*---(make variation)--------------*/
      SUFFIX_english_change (x_english, x_change, t);
      DEBUG_CONF   yLOG_complex ("english"   , "%s, %s, %s", x_english, x_change, t);
      rc = DICT__variation_quick (a_base, a_last, t, x_type, a_gregg, x_suffix, &x_vary);
      if (rc >= 0)  x_save = x_vary;
      /*---(make base)-------------------*/
      /*> DICT__find_variation (x_base, x_suffix, NULL, NULL, NULL);                  <*/
      SUFFIX_by_name (x_base, NULL, x_suffix, NULL, NULL, NULL);
      rc = DICT__variation_quick (a_base, x_save, x_english, x_base, a_gregg, x_suffix, &x_vary);
      if (rc >= 0)  x_save = x_vary;
   }
   /*---(group version)------------------*/
   else if (x_action == '+') {
      /*---(make first/base)-------------*/
      p = strtok_r (x_base, q, &r);
      /*> DICT__find_variation (p, x_suffix, NULL, NULL, NULL);                       <*/
      rc = SUFFIX_by_name (p, NULL, x_suffix, NULL, NULL, NULL);
      if (rc >= 0)  rc = DICT__variation_quick (a_base, a_last, x_english, p, a_gregg, x_suffix, &x_vary);
      if (rc >= 0)  x_save = x_vary;
      /*---(cycle others)----------------*/
      p = strtok_r (NULL, q, &r);
      while (p != NULL) {
         /*---(find)---------------------*/
         /*> DICT__find_variation (p, x_suffix, NULL, NULL, x_change);                <*/
         rc = SUFFIX_by_name (p, NULL, x_suffix, NULL, NULL, NULL);
         DEBUG_CONF   yLOG_complex ("variation" , "%s, %s", p, x_suffix);
         /*---(update)-------------------*/
         SUFFIX_english_change (x_english, x_change, t);
         DEBUG_CONF   yLOG_complex ("english"   , "%s, %s, %s", x_english, x_change, t);
         /*---(add)----------------------*/
         rc = DICT__variation_quick (a_base, x_save, t, p, a_gregg, x_suffix, &x_vary);
         if (rc >= 0)  x_save = x_vary;
         /*---(next)---------------------*/
         p = strtok_r (NULL, q, &r);
      }
      /*---(done)------------------------*/
   }
   /*---(save-back)----------------------*/
   if (r_new  != NULL)  *r_new = x_vary;
   /*---(complete)-----------------------*/
   DEBUG_CONF   yLOG_exit    (__FUNCTION__);
   return 0;
}

char
DICT__parse_OLD         (short a_line, cchar a_eprefix [LEN_LABEL], cchar a_recd [LEN_RECD])
{
   /*---(locals)-----------+-----+-----+-*/
   char        rce         =  -10;
   char        rc          =    0;
   int         i           =    0;
   char        x_eprefix   [LEN_LABEL] = "";
   char        x_base      [LEN_TITLE] = "";
   char        x_revised   =  '-';
   char        x_english   [LEN_TITLE] = "";
   char        x_gregg     [LEN_TITLE] = "";
   tWORD      *x_new       = NULL;
   tWORD      *x_last      = NULL;
   tWORD      *x_vary      = NULL;
   short       n           =    0;
   /*---(header)-------------------------*/
   DEBUG_CONF   yLOG_enter   (__FUNCTION__);
   /*---(split)--------------------------*/
   rc = DICT__split (a_recd);
   DEBUG_CONF   yLOG_value   ("split"     , rc);
   --rce;  if (rc < 0) {
      DEBUG_CONF   yLOG_exitr   (__FUNCTION__, rce);
      return rce;
   }
   /*---(preprare english)---------------*/
   strlcpy (x_eprefix, a_eprefix   , LEN_LABEL);
   strlcpy (x_base  , s_fields [0], LEN_TITLE);
   if (strcmp (x_eprefix, "") != NULL) {
      /*> rc = DICT__pre_update  (x_eprefix, x_base);                                 <*/
      n  = PREFIX__by_name (x_eprefix, x_english, x_gregg, NULL);
      DEBUG_CONF   yLOG_complex ("find"      , "%4d, %s, %s", n, x_english, x_gregg);
      --rce;  if (n < 0) {
         DEBUG_CONF   yLOG_exitr   (__FUNCTION__, rce);
         return rce;
      }
   }
   sprintf (x_english, "%s%s", x_eprefix, x_base);
   /*---(update with prefix)-------------*/
   if (s_nfield >= 5 && (strcmp (s_fields [4], "·") != 0 && strcmp (s_fields [4], "") != 0))   x_revised = 'y';
   DEBUG_CONF   yLOG_value   ("x_revised" , x_revised);
   /*---(preprare gregg)-----------------*/
   if (x_revised != 'y')  sprintf (x_gregg  , "%s%s", x_gregg, s_fields [2]);
   else                   sprintf (x_gregg  , "%s%s", x_gregg, s_fields [4]);
   DEBUG_CONF   yLOG_complex ("words"     , "%-20.20s, %s", x_english, x_gregg);
   /*---(primary)------------------------*/
   /*> rc = DICT__primary (a_line, x_english, x_gregg, s_fields [3], &x_new);         <*/
   rc = BASE_create (my.r_nfile, a_line, x_english, x_gregg, s_fields [3], &x_new);
   DEBUG_CONF   yLOG_value   ("base"      , rc);
   --rce;  if (rc < 0) {
      DEBUG_CONF   yLOG_exitr   (__FUNCTION__, rce);
      return rce;
   }
   /*> if (x_revised != 'y')  x_new->official = x_new->w_gregg;                       <* 
    *> else                   x_new->official  = strdup (s_fields [2]);               <*/
   x_new->w_nvary = 1;
   /*---(categories)---------------------*/
   /*> if (s_nfield > 3) {                                                            <* 
    *>    rc = DICT__category (x_new, s_fields [3]);                                  <* 
    *>    DEBUG_CONF   yLOG_value   ("primary"   , rc);                               <* 
    *>    --rce;  if (rc < 0) {                                                       <* 
    *>       DEBUG_CONF   yLOG_exitr   (__FUNCTION__, rce);                           <* 
    *>       return rce;                                                              <* 
    *>    }                                                                           <* 
    *> }                                                                              <*/
   /*---(variations)---------------------*/
   x_last = x_new;
   for (i = 5; i <= MAX_FIELD; ++i) {
      if (strlen (s_fields [i]) == 0)  continue;
      rc = DICT__variation (x_new, x_last, x_eprefix, x_gregg, s_fields [i], &x_vary);
      DEBUG_CONF   yLOG_value   ("variation" , rc);
      --rce;  if (rc < 0) {
         DEBUG_CONF   yLOG_note    ("variation failed, continuing");
      }
      x_last = x_vary;
   }
   /*---(complete)-----------------------*/
   DEBUG_CONF   yLOG_exit    (__FUNCTION__);
   return 0;
}

char
DICT__parse             (short a_line, cchar a_prefix [LEN_LABEL], cchar a_recd [LEN_RECD])
{
   /*---(locals)-----------+-----+-----+-*/
   char        rce         =  -10;
   char        rc          =    0;
   char        x_english   [LEN_TITLE] = "";
   char        x_gregg     [LEN_TITLE] = "";


   int         i           =    0;
   char        x_eprefix   [LEN_LABEL] = "";
   char        x_gprefix   [LEN_LABEL] = "";

   char        x_base      [LEN_TITLE] = "";
   char        x_revised   =  '-';
   tWORD      *x_new       = NULL;
   tWORD      *x_last      = NULL;
   tWORD      *x_vary      = NULL;
   short       n           =    0;
   /*---(header)-------------------------*/
   DEBUG_CONF   yLOG_enter   (__FUNCTION__);
   /*---(split)--------------------------*/
   rc = DICT__split (a_recd);
   DEBUG_CONF   yLOG_value   ("split"     , rc);
   --rce;  if (rc < 0) {
      DEBUG_CONF   yLOG_exitr   (__FUNCTION__, rce);
      return rce;
   }
   /*---(prepare base words)-------------*/
   strlcpy (x_english, s_fields [0], LEN_TITLE);
   if (s_nfield >= 5 && (strcmp (s_fields [4], "·") != 0 && strcmp (s_fields [4], "") != 0))   x_revised = 'y';
   DEBUG_CONF   yLOG_value   ("x_revised" , x_revised);
   if (x_revised != 'y')  strlcpy (x_gregg, s_fields [2], LEN_TITLE);
   else                   strlcpy (x_gregg, s_fields [4], LEN_TITLE);
   DEBUG_CONF   yLOG_complex ("gregg base", "%c, %-20.20s, %s", x_revised, x_english, x_gregg);
   /*---(base, sans-prefix)--------------*/
   rc = BASE_create (my.r_nfile, a_line, x_english, x_gregg, s_fields [3], &x_new);
   DEBUG_CONF   yLOG_value   ("base"      , rc);
   --rce;  if (rc < 0) {
      DEBUG_CONF   yLOG_exitr   (__FUNCTION__, rce);
      return rce;
   }
   /*---(prefix update)------------------*/
   rc = PREFIX_driver (a_prefix, x_english, x_gregg, NULL);
   DEBUG_CONF   yLOG_complex ("prefix"    , "%4d, %-20.20s, %s", rc, x_english, x_gregg);
   --rce;  if (rc < 0) {
      DEBUG_CONF   yLOG_exitr   (__FUNCTION__, rce);
      return rce;
   }
   /*---(primary)------------------------*/
   rc = BASE_create (my.r_nfile, a_line, x_english, x_gregg, s_fields [3], &x_new);
   DEBUG_CONF   yLOG_value   ("base"      , rc);
   --rce;  if (rc < 0) {
      DEBUG_CONF   yLOG_exitr   (__FUNCTION__, rce);
      return rce;
   }
   /*> if (x_revised != 'y')  x_new->official = x_new->w_gregg;                       <* 
    *> else                   x_new->official  = strdup (s_fields [2]);               <*/
   x_new->w_nvary = 1;
   /*---(categories)---------------------*/
   /*> if (s_nfield > 3) {                                                            <* 
    *>    rc = DICT__category (x_new, s_fields [3]);                                  <* 
    *>    DEBUG_CONF   yLOG_value   ("primary"   , rc);                               <* 
    *>    --rce;  if (rc < 0) {                                                       <* 
    *>       DEBUG_CONF   yLOG_exitr   (__FUNCTION__, rce);                           <* 
    *>       return rce;                                                              <* 
    *>    }                                                                           <* 
    *> }                                                                              <*/
   /*---(variations)---------------------*/
   x_last = x_new;
   for (i = 5; i <= MAX_FIELD; ++i) {
      if (strlen (s_fields [i]) == 0)  continue;
      rc = DICT__variation (x_new, x_last, x_eprefix, x_gregg, s_fields [i], &x_vary);
      DEBUG_CONF   yLOG_value   ("variation" , rc);
      --rce;  if (rc < 0) {
         DEBUG_CONF   yLOG_note    ("variation failed, continuing");
      }
      x_last = x_vary;
   }
   /*---(complete)-----------------------*/
   DEBUG_CONF   yLOG_exit    (__FUNCTION__);
   return 0;
}

char
DICT__parse_easy        (short a_line, cchar a_recd [LEN_RECD])
{
   return DICT__parse (a_line, "", a_recd);
}

char         /*-> create a new dictionary entry ------------------------------*/
DICT__single            (void *a_base, cchar a_english [LEN_TITLE], cchar a_gregg [LEN_TITLE])
{
}

char
DICT__suffixes          (void *a_base, void *a_prefix, cchar a_english [LEN_TITLE], cchar a_gregg [LEN_TITLE])
{
   /*---(locals)-----------+-----+-----+-*/
   char        rce         =  -10;
   char        rc          =    0;
   int         i           =    0;
   /*---(begin)----------------------------*/
   DEBUG_CONF   yLOG_enter   (__FUNCTION__);
   /*---(defense)------------------------*/
   DEBUG_CONF   yLOG_point   ("a_base"    , a_base);
   --rce;  if (a_base     == NULL) {
      DEBUG_CONF   yLOG_exitr   (__FUNCTION__, rce);
      return rce;
   }
   DEBUG_CONF   yLOG_point   ("a_prefix"  , a_prefix);
   DEBUG_CONF   yLOG_point   ("a_english" , a_english);
   --rce;  if (a_english == NULL) {
      DEBUG_CONF   yLOG_exitr   (__FUNCTION__, rce);
      return rce;
   }
   DEBUG_CONF   yLOG_point   ("a_gregg"   , a_gregg);
   --rce;  if (a_gregg   == NULL) {
      DEBUG_CONF   yLOG_exitr   (__FUNCTION__, rce);
      return rce;
   }
   /*---(cycle fields)-------------------*/
   for (i = 5; i <= MAX_FIELD; ++i) {
      if (strlen (s_fields [i]) == 0)  continue;
      /*> rc = DICT__variation (x_new, x_last, x_eprefix, x_gregg, s_fields [i], &x_vary);   <*/
      DEBUG_CONF   yLOG_value   ("variation" , rc);
      --rce;  if (rc < 0) {
         DEBUG_CONF   yLOG_note    ("variation failed, continuing");
      }
   }
   /*---(complete)-----------------------*/
   DEBUG_CONF   yLOG_exit    (__FUNCTION__);
   return 0;
}

char
DICT__prefixes          (void *a_base, cchar a_english [LEN_TITLE], cchar a_gregg [LEN_TITLE], cchar a_prefixes [LEN_HUND])
{
   /*---(locals)-----------+-----+-----+-*/
   char        rce         =  -10;
   char        rc          =    0;
   char        x_prefixes  [LEN_HUND]  = "";
   char        x_prefix    [LEN_TERSE] = "";
   char        x_english   [LEN_TITLE] = "";
   char        x_gregg     [LEN_TITLE] = "";
   char        t           [LEN_TITLE] = "";
   char       *p           = NULL;
   char       *q           =  ",";
   char       *r           = NULL;
   void       *x_point     = NULL;
   /*---(begin)----------------------------*/
   DEBUG_CONF   yLOG_enter   (__FUNCTION__);
   /*---(defense)------------------------*/
   DEBUG_CONF   yLOG_point   ("a_base"    , a_base);
   --rce;  if (a_base     == NULL) {
      DEBUG_CONF   yLOG_exitr   (__FUNCTION__, rce);
      return rce;
   }
   DEBUG_CONF   yLOG_point   ("a_english" , a_english);
   --rce;  if (a_english == NULL) {
      DEBUG_CONF   yLOG_exitr   (__FUNCTION__, rce);
      return rce;
   }
   DEBUG_CONF   yLOG_point   ("a_gregg"   , a_gregg);
   --rce;  if (a_gregg   == NULL) {
      DEBUG_CONF   yLOG_exitr   (__FUNCTION__, rce);
      return rce;
   }
   DEBUG_CONF   yLOG_point   ("a_prefixes", a_prefixes);
   --rce;  if (a_prefixes == NULL) {
      DEBUG_CONF   yLOG_exitr   (__FUNCTION__, rce);
      return rce;
   }
   DEBUG_CONF   yLOG_info    ("a_prefixes", a_prefixes);
   strlcpy (x_prefixes, a_prefixes, LEN_HUND);
   /*---(short-cut)----------------------*/
   if (strcmp (x_prefixes, "") == 0  || strcmp (x_prefixes, "·") == 0) {
      DEBUG_CONF   yLOG_note    ("no prefixes passed, simple");
      /*> rc = DICT__suffixes (a_base, a_english, a_gregg);                           <*/
      DEBUG_CONF   yLOG_exit    (__FUNCTION__);
      return rc;
   }
   /*---(prefixes)-----------------------*/
   DEBUG_CONF   yLOG_note    ("parsing prefixes");
   p = strtok_r (x_prefixes, q, &r);
   while (p != NULL) {
      strlcpy  (x_prefix, p, LEN_TERSE);
      strltrim (x_prefix, ySTR_BOTH, LEN_TERSE);
      DEBUG_CONF   yLOG_info    ("x_prefix"  , x_prefix);
      if (strcmp (x_prefix, "´") == 0) {
         DEBUG_CONF   yLOG_note    ("requested base prefix also");
         /*> rc = DICT__suffixes (a_base, a_english, a_gregg);                        <*/
      } else {
         DEBUG_CONF   yLOG_note    ("handling prefix");
         strlcpy (x_english, a_english, LEN_TITLE);
         strlcpy (x_gregg  , a_gregg  , LEN_TITLE);
         rc = PREFIX_driver (p, x_english, x_gregg, x_point);
         if (rc >= 0) {
            DEBUG_CONF   yLOG_complex ("prefix"    , "%4d, %-20.20s, %-20.20s, %p", rc, x_english, x_gregg, x_point);
            /*> rc = DICT__suffixes (a_base, x_point, x_english, x_gregg);            <*/
            DEBUG_CONF   yLOG_value   ("suffixes"  , rc);
         } else {
            DEBUG_CONF   yLOG_complex ("FAILED"    , "%d, to find prefix", rc);
         }
      }
      p = strtok_r (NULL  , q, &r);
   }
   /*---(complete)-----------------------*/
   DEBUG_CONF   yLOG_exit    (__FUNCTION__);
   return 0;
}

char
DICT__read              (FILE *a_file, short *r_line, char r_prefixes [LEN_HUND], char r_recd [LEN_RECD])
{
   /*---(locals)-----------+-----+-----+-*/
   char        rce         =  -10;
   char        rc          =    0;
   int         x_len       =    0;
   char       *p           = NULL;
   char        x_prefixes  [LEN_HUND]  = "";
   /*---(begin)----------------------------*/
   DEBUG_CONF   yLOG_enter   (__FUNCTION__);
   /*---(default)------------------------*/
   if (r_recd     != NULL)  strcpy (r_recd    , "(eof)");
   if (r_prefixes != NULL)  strcpy (r_prefixes, "");
   /*---(defense)------------------------*/
   DEBUG_CONF   yLOG_point   ("a_file"    , a_file);
   --rce;  if (a_file == NULL) {
      DEBUG_CONF   yLOG_exitr   (__FUNCTION__, rce);
      return rce;
   }
   DEBUG_CONF   yLOG_point   ("r_recd"    , r_recd);
   --rce;  if (r_recd == NULL) {
      DEBUG_CONF   yLOG_exitr   (__FUNCTION__, rce);
      return rce;
   }
   DEBUG_CONF   yLOG_point   ("r_prefixes", r_prefixes);
   --rce;  if (r_prefixes == NULL) {
      DEBUG_CONF   yLOG_exitr   (__FUNCTION__, rce);
      return rce;
   }
   /*---(check record)-------------------*/
   --rce;  if (feof (a_file)) {
      DEBUG_CONF   yLOG_exitr   (__FUNCTION__, rce);
      return rce;
   }
   /*---(find a record)------------------*/
   --rce;  while (1) {
      /*---(read)------------------------*/
      fgets (r_recd, LEN_RECD, a_file);
      if (feof (a_file)) {
         DEBUG_CONF   yLOG_exitr   (__FUNCTION__, rce);
         strcpy (r_recd, "(eof)");
         return rce;
      }
      if (r_line != NULL)  ++(*r_line);
      /*---(filter)----------------------*/
      if (r_recd [0] == '\0')  continue;
      if (r_recd [0] == '#')   continue;
      if (r_recd [0] == ' ')   continue;
      /*---(check for description)-------*/
      p = strchr (r_recd, '¨');
      if (p != NULL)  p [0] = '\0';
      /*---(check for prefixes)----------*/
      p = strchr (r_recd, '¼');
      if (p != NULL) {
         strlcpy (x_prefixes, p + 1, LEN_HUND);
         strltrim (x_prefixes, ySTR_BOTH, LEN_HUND);
         if (r_prefixes != NULL)   strlcpy (r_prefixes, x_prefixes, LEN_HUND);
         p [0] = '\0';
      }
      /*---(tear off tbd extras)---------*/
      p = strchr (r_recd, '#');
      if (p != NULL)  p [0] = '\0';
      p = strchr (r_recd, '=');
      if (p != NULL)  p [0] = '\0';
      /*---(clean-up)--------------------*/
      x_len = strlen (r_recd);
      if (x_len < 20)          continue;
      if (r_recd [x_len - 1] == '\n')  r_recd [--x_len] = '\0';
      DEBUG_CONF   yLOG_info    ("r_recd"    , r_recd);
      break;
      /*---(done)------------------------*/
   }
   /*---(complete)-----------------------*/
   DEBUG_CONF   yLOG_exit    (__FUNCTION__);
   return 0;
}

char
DICT_import             (cchar a_name [LEN_PATH])
{
   /*---(locals)-----------+-----+-----+-*/
   char        rce         =  -10;
   char        rc          =    0;
   FILE       *f           = NULL;
   char        x_recd      [LEN_RECD]  = "";
   short       x_line      =    0;
   char        x_prefixes  [LEN_HUND]  = "";
   void       *x_base      = NULL;
   char        x_english   [LEN_TITLE] = "";
   char        x_gregg     [LEN_TITLE] = "";
   /*---(begin)----------s-----------------*/
   DEBUG_CONF   yLOG_enter   (__FUNCTION__);
   yURG_msg ('>', "source å%sæ", a_name);
   /*---(open dictionary)------------------*/
   rc = SHARED_open (a_name, 'r', &f);
   --rce;  if (rc < 0) {
      yURG_err ('F', "data source could not be openned");
      DEBUG_CONF   yLOG_exitr   (__FUNCTION__, rce);
      return rce;
   }
   yURG_msg ('-', "data source openned successfully");
   /*---(process)--------------------------*/
   while (1) {
      /*---(next)----------------*/
      rc = DICT__read     (f, &x_line, x_prefixes, x_recd);
      if (rc < 0)  break;
      /*---(create base)---------*/
      rc = DICT__base     (x_line, x_recd, x_english, x_gregg, &x_base);
      /*---(create base)---------*/
      rc = DICT__prefixes (x_base, x_english, x_gregg, x_prefixes);
      /*---(done)----------------*/
   }
   /*---(close dictionary)-----------------*/
   rc = SHARED_close (&f);
   --rce;  if (rc < 0) {
      yURG_err ('F', "data source could not be closed");
      DEBUG_CONF   yLOG_exitr   (__FUNCTION__, rce);
      return rce;
   }
   yURG_msg ('-', "data source closed successfully");
   /*---(paginate)-------------------------*/
   DLIST_paginate ();
   yURG_msg ('-', "data paginated");
   /*---(done)-----------------------------*/
   yURG_msg ('·', "");
   /*---(complete)-------------------------*/
   DEBUG_CONF   yLOG_exit    (__FUNCTION__);
   return 0;
}

int
WORDS_find           (char *a_word)
{
   /*> /+---(locals)-----------+-----+-----+-+/                                           <* 
    *> char        rce         =  -10;                                                    <* 
    *> int         i           =    0;                                                    <* 
    *> /+---(defense)------------------------+/                                           <* 
    *> --rce;  if (a_word == NULL)  return rce;                                           <* 
    *> /+---(search)-------------------------+/                                           <* 
    *> for (i = 0; i < MAX_WORDS; ++i) {                                                  <* 
    *>    printf ("reviewing %d, %s, %s\n", i, s_words [i].english, s_words [i].gregg);   <* 
    *>    if (strncmp (s_words[i].english, "EOF",  MAX_LEN) == 0)  return -1;             <* 
    *>    if (strncmp (s_words[i].english, a_word, MAX_LEN) != 0)  continue;              <* 
    *>    printf ("FOUND     %d, %s, %s\n", i, s_words [i].english, s_words [i].gregg);   <* 
    *>    return  i;                                                                      <* 
    *> }                                                                                  <* 
    *> /+---(complete)-----------------------+/                                           <* 
    *> return 0;                                                                          <*/
}

char
DICT_list          (void)
{
   /*---(locals)-----------+-----+-----+-*/
   int         i           =    0;
   int         a           =    0;
   int         c           =    0;
   char        t           [LEN_FULL]  = "";
   char        n           [LEN_SHORT] = "";
   char        e           [LEN_TITLE] = "";
   char        g           [LEN_TITLE] = "";
   char        x_col       =    0;
   tWORD      *x_curr      = NULL;
   tWORD      *x_sub       = NULL;
   /*---(search)-------------------------*/
   WORDS_eng_by_index  (a, &x_curr);
   while (x_curr != NULL) {
      if (strcmp (x_curr->w_vary, "<") == 0) {
         WORDS_detail (x_curr, t);
         strlpadn (++c, n, '.', '<', 5);
         printf ("%5.5s··%s\n", n, t);
         x_sub = x_curr->w_next;
         x_col = 0;
         while (x_sub != NULL) {
            WORDS_detail (x_sub, t);
            printf ("    %c  %s\n", ++x_col + 'a', t);
            x_sub  = x_sub->w_next;
         }
      }
      ++a;
      WORDS_eng_by_index (a, &x_curr);
   }
   printf ("## found %d primaries and %d total words (%4.2fx)\n", c, a, (float) a / (float) c);
   /*---(complete)-----------------------*/
   return 0;
}

char
DICT_list_all      (void)
{
   /*---(locals)-----------+-----+-----+-*/
   int         i           =    0;
   int         a           =    0;
   int         c           =    0;
   char        t           [LEN_FULL]  = "";
   char        n           [LEN_SHORT] = "";
   char        e           [LEN_TITLE] = "";
   char        g           [LEN_TITLE] = "";
   tWORD       *x_curr      = NULL;
   /*---(search)-------------------------*/
   WORDS_eng_by_index  (a, &x_curr);
   while (x_curr != NULL) {
      WORDS_detail (x_curr, t);
      strlpadn (++a, n, '.', '<', 5);
      printf ("%5.5s··%s\n", n, t);
      WORDS_eng_by_index (a, &x_curr);
   }
   /*---(complete)-----------------------*/
   return 0;
}

/*>                                                                                                                                                                                           <* 
 *>    /+--name-- --suffix---- --also-- ---source-- ---true-endings----------------------------------------- ---examples------------------------------------------------------------ cnt +/   <* 
 *>    { "´"     , ">·´"      , ""     , '5',  29  , "-ing, -thing"                                         , "observing, caring, drinking, knowing, liking, using, forgetting"     , 0 },    <* 
 *>                                                                                                                                                                                           <*/


char
DICT_dump_words         (FILE *f)
{
   /*---(locals)-----------+-----+-----+-*/
   int         i           =    0;
   int         a           =    0;
   int         c           =    0;
   char        t           [LEN_FULL]  = "";
   char        n           [LEN_SHORT] = "";
   char        e           [LEN_TITLE] = "";
   char        g           [LEN_TITLE] = "";
   char        x_col       =    0;
   tWORD      *x_curr      = NULL;
   tWORD      *x_sub       = NULL;
   /*---(header)---------------------------*/
   DEBUG_CONF   yLOG_enter   (__FUNCTION__);
   fprintf (f, "##   %s %s\n", P_NAMESAKE, P_HERITAGE);
   fprintf (f, "##   version %s, %s\n", P_VERNUM, P_VERTXT);
   fprintf (f, "##   inventory of dictionary words\n");
   fprintf (f, "\n");
   fprintf (f, "#@ style     V = printable columnar values\n");
   fprintf (f, "#@ x-parse  14åÏ---···Ï-----------------------··Ï---·Ï---·Ï·Ï··Ï·Ï·Ï--··Ï·Ï---··Ï-----------------------··Ï----------------------------------·æ\n");
   fprintf (f, "#@ titles     åref····english···················line·var··p·s··s·c·pg···g·freq··gregg·····················shown·······························æ\n");
   fprintf (f, "\n");
   WORDS_eng_by_index  (a, &x_curr);
   while (x_curr != NULL) {
      if (strcmp (x_curr->w_vary, "<") == 0) {
         DEBUG_CONF   yLOG_info    ("base"      , x_curr->w_english);
         WORDS_detail (x_curr, t);
         DEBUG_CONF   yLOG_info    ("t"         , t);
         strlpadn (++c, n, '.', '<', 5);
         if (i %  5 == 0)  fprintf (f, "\n");
         if (i % 25 == 0)  fprintf (f, "##-····---english--------------··line·var··p·s··s·c·pg···g·freq··---gregg----------------··---shown---------------------------\n\n");
         i++;
         fprintf (f, "%5.5s··%s\n", n, t);
         x_sub = x_curr->w_next;
         x_col = 0;
         while (x_sub != NULL) {
            DEBUG_CONF   yLOG_info    ("vary"      , x_sub->w_english);
            WORDS_detail (x_sub, t);
            DEBUG_CONF   yLOG_info    ("t"         , t);
            if (i %  5 == 0)  fprintf (f, "\n");
            if (i % 25 == 0)  fprintf (f, "##-····---english--------------··line·var··p·s··s·c·pg···g·freq··---gregg----------------··---shown---------------------------\n\n");
            i++;
            fprintf (f, "    %c  %s\n", ++x_col + 'a', t);
            x_sub  = x_sub->w_next;
         }
      }
      ++a;
      WORDS_eng_by_index (a, &x_curr);
   }
   fprintf (f, "\n");
   fprintf (f, "##-····---english--------------··line·var··p·s··s·c·pg···g·freq··---gregg----------------··---shown---------------------------\n\n");
   fprintf (f, "## found %d primaries and %d total words (%4.2fx)\n", c, a, (float) a / (float) c);
   /*---(complete)-------------------------*/
   DEBUG_CONF   yLOG_exit    (__FUNCTION__);
   return 0;
}

char
DICT_dump_gregg         (FILE *f)
{
   /*---(locals)-----------+-----+-----+-*/
   int         x_pos       =    0;
   int         x_gregg     =    0;
   int         i           =    0;
   int         a           =    0;
   int         c           =    0;
   char        r           [LEN_SHORT] = "";
   char        s           [LEN_SHORT] = "";
   char        t           [LEN_SHORT] = "";
   char        u           [LEN_SHORT] = "";
   char        v           [LEN_SHORT] = "";
   char        x_col       =    0;
   tWORD      *x_curr      = NULL;
   /*---(header)-------------------------*/
   DEBUG_CONF   yLOG_enter   (__FUNCTION__);
   WORDS_eng_by_index  (a, &x_curr);
   while (x_curr != NULL) {
      if (strcmp (x_curr->w_vary, "<") != 0) {
         WORDS_eng_by_index (++a, &x_curr);
         continue;
      }
      if (x_curr->w_tree [0] > 0)  sprintf (r, "%02d" , x_curr->w_tree [0]);
      else                         strcpy  (r, "  ");
      if (x_curr->w_tree [1] > 0)  sprintf (s, "·%02d", x_curr->w_tree [1]);
      else                         strcpy  (s, "   ");
      if (x_curr->w_tree [2] > 0)  sprintf (t, "·%02d", x_curr->w_tree [2]);
      else                         strcpy  (t, "   ");
      if (x_curr->w_tree [3] > 0)  sprintf (u, "·%02d", x_curr->w_tree [3]);
      else                         strcpy  (u, "   ");
      if (x_curr->w_tree [4] > 0)  sprintf (v, "·%02d", x_curr->w_tree [4]);
      else                         strcpy  (v, "   ");
      fprintf (f, "%2s%2s%2s%2s%2s  %-20.20s  %s\n", r, s, t, u, v, x_curr->w_gregg, x_curr->w_english);
      WORDS_eng_by_index (++a, &x_curr);
   }
   /*---(complete)-----------------------*/
   DEBUG_CONF   yLOG_exit    (__FUNCTION__);
   return 0;
}


