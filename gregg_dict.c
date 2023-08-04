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



#define   MAX_PAGES      100
static void     *s_pages [MAX_PAGES];
static void     *s_lasts [MAX_PAGES];




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
   int         i           =    0;
   /*---(header)-------------------------*/
   DEBUG_CONF   yLOG_enter   (__FUNCTION__);
   /*---(initialize)---------------------*/
   rc = ySORT_btree (B_DICT, "dictionary");
   DEBUG_CONF   yLOG_value   ("dict"   , rc);
   --rce;  if (rc < 0) {
      DEBUG_CONF   yLOG_exitr   (__FUNCTION__, rce);
      return rce;
   }
   /*---(files)--------------------------*/
   SOURCE__purge ();
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
   tDICT      *x_curr      =    0;
   /*---(header)-------------------------*/
   DEBUG_CONF   yLOG_enter   (__FUNCTION__);
   /*---(purging)------------------------*/
   DICT__by_cursor (YDLST_TAIL, &x_curr);
   while (x_curr != NULL) {
      DICT__unhook (x_curr);
      DICT__free   (&x_curr);
      DICT__by_cursor (YDLST_TAIL, &x_curr);
   }
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

/*> char                                                                                     <* 
 *> DICT__walk_by_bases     (FILE *a_file, char *f_callback (FILE *a_file, void *a_curr))    <* 
 *> {                                                                                        <* 
 *>    /+---(locals)-----------+-----+-----+-+/                                              <* 
 *>    int         a           =    0;                                                       <* 
 *>    tDICT      *x_curr      = NULL;                                                       <* 
 *>    tDICT      *x_sub       = NULL;                                                       <* 
 *>    /+---(header)---------------------------+/                                            <* 
 *>    DEBUG_CONF   yLOG_enter   (__FUNCTION__);                                             <* 
 *>    WORDS_eng_by_index  (a, &x_curr);                                                     <* 
 *>    while (x_curr != NULL) {                                                              <* 
 *>       if (strcmp (x_curr->w_vary, "<") == 0) {                                           <* 
 *>          DEBUG_CONF   yLOG_info    ("base"      , x_curr->w_english);                    <* 
 *>          f_callback (a_file, x_curr);                                                    <* 
 *>          x_sub = x_curr->w_next;                                                         <* 
 *>          while (x_sub != NULL) {                                                         <* 
 *>             DEBUG_CONF   yLOG_info    ("vary"      , x_sub->w_english);                  <* 
 *>             f_callback (a_file, x_sub);                                                  <* 
 *>             x_sub  = x_sub->w_next;                                                      <* 
 *>          }                                                                               <* 
 *>       }                                                                                  <* 
 *>       ++a;                                                                               <* 
 *>       WORDS_eng_by_index (a, &x_curr);                                                   <* 
 *>    }                                                                                     <* 
 *>    /+---(complete)-------------------------+/                                            <* 
 *>    DEBUG_CONF   yLOG_exit    (__FUNCTION__);                                             <* 
 *>    return 0;                                                                             <* 
 *> }                                                                                        <*/


/*====================------------------------------------====================*/
/*===----                         debugging                            ----===*/
/*====================------------------------------------====================*/
static void  o___DEBUGGING_______o () { return; }

char
DICT__detail_full       (void *a_dict, char a_style, char a_out [LEN_FULL])
{
   /*---(locals)-----------+-----+-----+-*/
   char        rce         =  -10;
   tDICT      *x_dict      = NULL;
   tBASE      *x_base      = NULL;
   char        s           [LEN_TITLE] = " ·åæ";
   char        t           [LEN_TITLE] = " ·åæ";
   char        u           [LEN_DESC]  = " ·åæ";
   char        v           [LEN_TITLE] = "·";
   char        w           [LEN_TITLE] = "·";
   char        x           [LEN_TITLE] = "·";
   char        y           [LEN_TITLE] = "·";
   /*---(defense)------------------------*/
   --rce;  if (a_out  == NULL)  return rce;
   strcpy (a_out, "");
   if (a_dict == NULL)          return 0;
   /*---(cast)---------------------------*/
   x_dict  = (tDICT *) a_dict;
   x_base  = (tBASE *) (x_dict->d_base);
   /*---(prepare)------------------------*/
   switch (a_style) {
   case  't' :
      sprintf  (s, "%2då%.20sæ", strlen (x_dict->d_english), x_dict->d_english);
      sprintf  (t, "%2då%.30sæ", strlen (x_dict->d_gregg)  , x_dict->d_gregg);
      break;
   case  '-' :
      sprintf  (s, "%.20s", x_dict->d_english);
      sprintf  (t, "%.30s", x_dict->d_gregg);
      break;
   }
   /*> sprintf  (u, "%-3.3sp %-3.3sb %-3.3ss", (x_dict->d_prefix == NULL) ? "···" : "SET", (x_dict->d_base == NULL) ? "···" : "SET", (x_dict->d_suffix == NULL) ? "···" : "SET");   <*/
   if (x_dict->d_prev   != NULL)  strlcpy (v, x_dict->d_prev->d_english, LEN_TITLE);
   if (x_dict->d_next   != NULL)  strlcpy (w, x_dict->d_next->d_english, LEN_TITLE);
   if (x_base           != NULL)  strlcpy (x, x_base->b_english        , LEN_TITLE);
   PREFIX_english (x_dict->d_prefix, u);
   if (strcmp (u, "") == 0)  strcpy (u, "·");
   SUFFIX_english (x_dict->d_suffix, y);
   if (strcmp (y, "") == 0)  strcpy (y, "·");
   /*---(consolidate)--------------------*/
   switch (a_style) {
   case  't' :
      sprintf (a_out, "%-24.24s  %-34.34s ´ %-20.20s ´ %-8.8s %-5.5s ´ %-20.20s ´ %-20.20s ´", s, t, x, u, y, v, w);
      break;
   case  '-' :
      sprintf (a_out, "%-20.20s  %-30.30s ´ %-20.20s ´ %-8.8s %-5.5s ´ %-20.20s ´ %-20.20s ´", s, t, x, u, y, v, w);
      break;
   }
   /*---(complete)-----------------------*/
   return 0;
}

char DICT__detail       (void *a_dict, char a_out [LEN_FULL])   { return DICT__detail_full (a_dict, 't', a_out); }
char DICT__dumper       (void *a_dict, char a_out [LEN_FULL])   { return DICT__detail_full (a_dict, '-', a_out); }

char*
DICT__pointer           (int n, void *a_dict)
{
   char        t           [LEN_FULL]  = "";
   DICT__detail_full (a_dict, 't', t);
   if (strcmp (g_print, "") == 0)  strcpy  (g_print, "n/a");
   else                            sprintf (g_print, "%-5d %s", n, t);
   return g_print;
}

char*
DICT__entry             (int n)
{
   tDICT      *x_dict      = NULL;
   char        t           [LEN_FULL]  = "";
   static int  x_prev      =    0;
   if (n == YDLST_INC)  n = x_prev + 1;
   DICT__by_index (n, &x_dict);
   DICT__detail_full (x_dict, 't', t);
   if (strcmp (t, "") == 0)  strcpy  (g_print, "n/a");
   else                      sprintf (g_print, "%-5d %s", n, t);
   x_prev = n;
   return g_print;
}



/*====================------------------------------------====================*/
/*===----                        main driver                           ----===*/
/*====================------------------------------------====================*/
static void  o___EXISTANCE_______o () { return; }

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
   /*---(add to source)------------------*/
   SOURCE_inc ();
   /*---(save-back)----------------------*/
   if (r_dict != NULL)  *r_dict = x_new;
   /*---(complete)-----------------------*/
   DEBUG_CONF   yLOG_exit    (__FUNCTION__);
   return 0;
}



/*====================-----------------··-----------------====================*/
/*===----                        driver support                        ----===*/
/*====================-----------------··-----------------====================*/
static void  o___DRIVER__________o () { return; }

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
DICT__base              (char a_file, short a_line, cchar a_recd [LEN_RECD], char r_english [LEN_TITLE], char r_gregg [LEN_TITLE], void **r_base)
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
   rc = BASE_create (a_file, a_line, r_english, r_gregg, s_fields [3], r_base);
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
DICT__suffixes          (void *a_base, void *a_prefix, cchar a_gregg [LEN_TITLE])
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
   DEBUG_CONF   yLOG_point   ("a_gregg"   , a_gregg);
   --rce;  if (a_gregg   == NULL) {
      DEBUG_CONF   yLOG_exitr   (__FUNCTION__, rce);
      return rce;
   }
   /*---(cycle fields)-------------------*/
   for (i = 5; i <= MAX_FIELD; ++i) {
      if (strlen (s_fields [i]) == 0)  continue;
      rc = SUFFIX_driver (a_base, a_prefix, s_fields [i], a_gregg);
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
      rc = DICT__suffixes (a_base, NULL, a_gregg);
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
         rc = DICT__suffixes (a_base, NULL, a_gregg);
      } else {
         DEBUG_CONF   yLOG_note    ("handling prefix");
         strlcpy (x_english, a_english, LEN_TITLE);
         strlcpy (x_gregg  , a_gregg  , LEN_TITLE);
         rc = PREFIX_driver (x_prefix, x_english, x_gregg, &x_point);
         DEBUG_CONF   yLOG_complex ("prefix"    , "%4d, %-20.20s, %-20.20s, %p", rc, x_english, x_gregg, x_point);
         if (rc >= 0) {
            rc = DICT_create   (x_english, x_gregg, x_point, a_base, NULL, NULL);
            DEBUG_CONF   yLOG_value   ("create"    , rc);
            if (rc >= 0) {
               rc = DICT__suffixes (a_base, x_point, x_gregg);
               DEBUG_CONF   yLOG_value   ("suffixes"  , rc);
            } else {
               DEBUG_CONF   yLOG_complex ("FAILED"    , "%d, to create prefixed base word", rc);
            }
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
      /*---(clean-up)--------------------*/
      x_len = strlen (r_recd);
      if (x_len < 20)          continue;
      if (r_recd [x_len - 1] == '\n')  r_recd [--x_len] = '\0';
      DEBUG_CONF   yLOG_info    ("r_recd"    , r_recd);
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
         DEBUG_CONF   yLOG_info    ("x_prefixes", x_prefixes);
         if (r_prefixes != NULL)   strlcpy (r_prefixes, x_prefixes, LEN_HUND);
         p [0] = '\0';
      }
      /*---(tear off tbd extras)---------*/
      p = strchr (r_recd, '#');
      if (p != NULL)  p [0] = '\0';
      p = strchr (r_recd, '=');
      if (p != NULL)  p [0] = '\0';
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
   char        x_file      =   0;
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
   /*---(add file)-------------------------*/
   x_file = SOURCE__count ();
   SOURCE_add (a_name);
   /*---(process)--------------------------*/
   while (1) {
      /*---(next)----------------*/
      rc = DICT__read     (f, &x_line, x_prefixes, x_recd);
      if (rc < 0)  break;
      /*---(create base)---------*/
      rc = DICT__base     (x_file, x_line, x_recd, x_english, x_gregg, &x_base);
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
   DICT_paginate ();
   yURG_msg ('-', "data paginated");
   /*---(done)-----------------------------*/
   yURG_msg ('·', "");
   /*---(complete)-------------------------*/
   DEBUG_CONF   yLOG_exit    (__FUNCTION__);
   return 0;
}



/*====================------------------------------------====================*/
/*===----                       reporting                              ----===*/
/*====================------------------------------------====================*/
static void  o___REPORTING_______o () { return; }

char
DICT_dump_words         (FILE *f)
{
   /*---(locals)-----------+-----+-----+-*/
   int         a           =    0;
   int         c           =    0;
   char        t           [LEN_FULL]  = "";
   char        n           [LEN_TERSE] = "";
   tDICT      *x_curr      = NULL;
   char       *x_break     = "##---··---english----------··---gregg----------------------···---base-------------···---pointers---···---prev-------------···---next-------------·´\n\n";
   /*---(header)---------------------------*/
   DEBUG_CONF   yLOG_enter   (__FUNCTION__);
   fprintf (f, "##   %s %s\n", P_NAMESAKE, P_HERITAGE);
   fprintf (f, "##   version %s, %s\n", P_VERNUM, P_VERTXT);
   fprintf (f, "##   inventory of dictionary words\n");
   fprintf (f, "\n");
   fprintf (f, "#@ style     V = printable columnar values\n");
   fprintf (f, "#@ x-parse  14åÏ----··Ï-------------------··Ï-----------------------------···Ï-------------------···Ï-------------···Ï-------------------···Ï-------------------·æ\n");
   fprintf (f, "#@ titles     åref····english···············gregg····························base···················pointers·········prev···················next·················æ\n");
   fprintf (f, "\n");
   DICT__by_cursor  (YDLST_HEAD, &x_curr);
   while (x_curr != NULL) {
      DEBUG_CONF   yLOG_info    ("base"      , x_curr->d_english);
      DICT__dumper (x_curr, t);
      DEBUG_CONF   yLOG_info    ("t"         , t);
      strlpadn (a, n, '.', '<', 5);
      if (a %  5 == 0)  fprintf (f, "\n");
      if (a % 25 == 0)  fprintf (f, x_break);
      fprintf (f, "%-5d  %s\n", a, t);
      ++a;
      DICT__by_cursor (YDLST_NEXT, &x_curr);
   }
   fprintf (f, "\n");
   fprintf (f, x_break);
   fprintf (f, "## found %d total words\n", a);
   /*---(complete)-------------------------*/
   DEBUG_CONF   yLOG_exit    (__FUNCTION__);
   return 0;
}

char
DICT_paginate           (void)
{
   tDICT      *x_word      = NULL;
   tDICT      *x_last      = NULL;
   short       i           =    0;
   short       c           =    0;
   short       p           =    0;
   /*---(header)-------------------------*/
   DEBUG_GRAF   yLOG_enter   (__FUNCTION__);
   /*---(basic inputs)-------------------*/
   DEBUG_GRAF   yLOG_value   ("ppage"     , my.w_ppage);
   /*---(clear paging)-------------------*/
   for (i = 0; i < MAX_PAGES; ++i)  s_pages [i] = s_lasts [i] = NULL;
   my.w_npage;
   /*---(paginate)-----------------------*/
   DICT__by_cursor (YDLST_HEAD, &x_word);
   i = 0;
   while (x_word != NULL) {
      DEBUG_GRAF   yLOG_complex ("x_word"    , "%4d %4d %4d %-15.15s %-15.15s", p, i++, c, x_word->d_english, x_word->d_gregg);
      /*---(prefix filtering)------------*/
      /*> if (my.nopre    == 'y' || x_word->d_prefix != NULL)  {                      <* 
       *>    DEBUG_GRAF   yLOG_note    ("SKIP, prefixed word");                       <* 
       *>    DICT__by_cursor (YDLST_NEXT, &x_word);                                   <* 
       *>    continue;                                                                <* 
       *> }                                                                           <*/
      /*---(suffix filtering)------------*/
      /*> if (my.baseonly == 'y' || x_word->d_suffix != NULL) {                       <* 
       *>    DEBUG_GRAF   yLOG_note    ("SKIP, not a base/root");                     <* 
       *>    DICT__by_cursor (YDLST_NEXT, &x_word);                                   <* 
       *>    continue;                                                                <* 
       *> }                                                                           <*/
      /*---(check for new page)----------*/
      if (c % my.w_ppage == 0) {
         DEBUG_GRAF   yLOG_complex ("NEW PAGE"  , "%4d, %s", p, x_word->d_english);
         s_pages [p] = x_word;
         /*---(end of prev page)---------*/
         if (p > 0) {
            DEBUG_GRAF   yLOG_complex ("add last"  , "%4d, %s", p - 1, x_last->d_english);
            s_lasts [p - 1] = x_last;
         }
         ++p;
      }
      /*---(next word)-------------------*/
      x_last = x_word;
      ++c;
      DICT__by_cursor (YDLST_NEXT, &x_word);
      /*---(done)------------------------*/
   }
   /*---(finish last page)---------------*/
   if (p > 0) {
      s_lasts [p - 1] = x_last;
      DEBUG_GRAF   yLOG_complex ("add last"  , "%4d, %s", p - 1, x_last->d_english);
   }
   /*---(update globals)-----------------*/
   my.w_npage   = p;
   my.w_entries = c;
   DEBUG_GRAF   yLOG_complex ("dict page" , "%3dp, %3dn, %3dc, %3de", my.w_ppage, my.w_npage, my.w_cpage, my.w_entries);
   /*---(complete)-----------------------*/
   DEBUG_GRAF   yLOG_exit    (__FUNCTION__);
   return 0;
}

char
DICT_page_ends          (int a_page, char r_beg [LEN_TITLE], char r_end [LEN_TITLE])
{
   char        rce         =  -10;
   tDICT      *x_end       = NULL;
   if (r_beg != NULL)  strcpy (r_beg, "");
   if (r_end != NULL)  strcpy (r_end, "");
   --rce;  if (a_page < 0)              return rce;
   --rce;  if (a_page >= my.w_npage)    return rce;
   x_end = (tDICT *) s_pages [a_page];
   if (r_beg != NULL)  strlcpy (r_beg, x_end->d_english, LEN_TITLE);
   x_end = (tDICT *) s_lasts [a_page];
   if (r_end != NULL)  strlcpy (r_end, x_end->d_english, LEN_TITLE);
   return my.w_ppage;
}



/*====================-----------------==-----------------====================*/
/*===----                      database handling                       ----===*/
/*====================-----------------==-----------------====================*/
static void o___DATABASE__________________o (void) {;}

char
DICT__encode            (void *a_dict)
{
   /*---(locals)-----------+-----+-----+-*/
   char        rce         =  -10;
   char        rc          =    0;
   tDICT      *x_dict      = NULL;
   tBASE      *x_base      = NULL;
   /*---(header)-------------------------*/
   DEBUG_OUTP   yLOG_senter  (__FUNCTION__);
   /*---(defense)------------------------*/
   DEBUG_OUTP  yLOG_spoint  (a_dict);
   --rce;  if (a_dict == NULL) {
      DEBUG_OUTP  yLOG_sexitr  (__FUNCTION__, rce);
      return rce;
   }
   /*---(encode)-------------------------*/
   x_dict = (tDICT *) a_dict;
   x_base = (tBASE *) x_dict->d_base;
   x_dict->d_prefix = PREFIX_encode  (x_dict->d_prefix);
   x_dict->d_suffix = SUFFIX_encode  (x_dict->d_suffix);
   x_dict->d_base   = x_base->b_ref;
   /*---(complete)-----------------------*/
   DEBUG_OUTP   yLOG_exit    (__FUNCTION__);
   return 0;
}

char
DICT__decode            (void *a_dict)
{
   /*---(locals)-----------+-----+-----+-*/
   char        rce         =  -10;
   char        rc          =    0;
   tDICT      *x_dict      = NULL;
   /*---(header)-------------------------*/
   DEBUG_OUTP   yLOG_senter  (__FUNCTION__);
   /*---(defense)------------------------*/
   DEBUG_OUTP  yLOG_spoint  (a_dict);
   --rce;  if (a_dict == NULL) {
      DEBUG_OUTP  yLOG_sexitr  (__FUNCTION__, rce);
      return rce;
   }
   /*---(encode)-------------------------*/
   x_dict = (tDICT *) a_dict;
   x_dict->d_prefix = PREFIX_decode  (x_dict->d_prefix);
   x_dict->d_suffix = SUFFIX_decode  (x_dict->d_suffix);
   x_dict->d_base   = BASE_index_get (x_dict->d_base);
   /*---(complete)-----------------------*/
   DEBUG_OUTP   yLOG_exit    (__FUNCTION__);
   return 0;
}

char
DICT_write              (FILE *a_file)
{
   /*---(locals)-----------+-----+-----+-*/
   char        rce         =  -10;
   char        rc          =    0;
   char        x_count     =    0;
   tDICT      *x_dict      = NULL;
   short       c           =    0;
   /*---(header)-------------------------*/
   DEBUG_OUTP   yLOG_enter   (__FUNCTION__);
   /*---(defense)------------------------*/
   DEBUG_OUTP  yLOG_point   ("a_file"    , a_file);
   --rce;  if (a_file == NULL) {
      DEBUG_OUTP  yLOG_exitr   (__FUNCTION__, rce);
      return rce;
   }
   x_count = DICT__count ();
   DEBUG_OUTP  yLOG_value   ("x_count"   , x_count);
   --rce;  if (x_count <= 0) {
      DEBUG_OUTP  yLOG_exitr   (__FUNCTION__, rce);
      return rce;
   }
   /*---(write all)----------------------*/
   DICT__by_cursor (YDLST_HEAD, &x_dict);
   while (x_dict != NULL) {
      DEBUG_OUTP  yLOG_info    ("english"   , x_dict->d_english);
      DICT__encode (x_dict);
      rc = fwrite (x_dict, sizeof (tDICT), 1, a_file);
      if (rc != 1)   break;
      DICT__decode (x_dict);
      DICT__by_cursor (YDLST_NEXT, &x_dict);
      ++c;
   }
   /*---(check)--------------------------*/
   DEBUG_OUTP  yLOG_value   ("c"         , c);
   --rce;  if (x_count != c) {
      DEBUG_OUTP  yLOG_exitr   (__FUNCTION__, rce);
      return rce;
   }
   /*---(complete)-----------------------*/
   DEBUG_OUTP   yLOG_exit    (__FUNCTION__);
   return 0;
}

char
DICT_read               (FILE *a_file, short a_count)
{
   /*---(locals)-----------+-----+-----+-*/
   char        rce         =  -10;
   char        rc          =    0;
   char        x_count     =    0;
   short       i           =    0;
   tDICT      *x_dict      = NULL;
   /*---(header)-------------------------*/
   DEBUG_OUTP   yLOG_enter   (__FUNCTION__);
   /*---(defense)------------------------*/
   DEBUG_OUTP  yLOG_point   ("a_file"    , a_file);
   --rce;  if (a_file == NULL) {
      DEBUG_OUTP  yLOG_exitr   (__FUNCTION__, rce);
      return rce;
   }
   x_count = DICT__count ();
   DEBUG_OUTP  yLOG_value   ("x_count"   , x_count);
   --rce;  if (x_count >  0) {
      DEBUG_OUTP  yLOG_exitr   (__FUNCTION__, rce);
      return rce;
   }
   /*---(write all)----------------------*/
   for (i = 0; i < a_count; ++i) {
      DICT__force (&x_dict);
      rc = fread  (x_dict, sizeof (tDICT), 1, a_file);
      DEBUG_OUTP  yLOG_info    ("english"   , x_dict->d_english);
      if (rc != 1)   break;
      DICT__decode (x_dict);
      DICT__hook   (x_dict);
      BASE_add_dict (x_dict->d_base, x_dict);
   }
   /*---(check)--------------------------*/
   x_count = DICT__count ();
   DEBUG_OUTP  yLOG_value   ("x_count"   , x_count);
   --rce;  if (i != x_count) {
      DEBUG_OUTP  yLOG_exitr   (__FUNCTION__, rce);
      return rce;
   }
   /*---(complete)-----------------------*/
   DEBUG_OUTP   yLOG_exit    (__FUNCTION__);
   return 0;
}







