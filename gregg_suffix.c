/*============================---(source-start)---============================*/
#include "gregg.h"

/*
 *  PURPOSE   manage application of standard english/gregg suffixes to reduce base words
 *
 *
 */


#define     MAX_SUFFIX     300
typedef  struct  cSUFFIX   tSUFFIX;
struct cSUFFIX {
   /*---(main)-----------------*/
   short       s_ref;                       /* ref for db write/read          */
   char        s_name      [LEN_LABEL];     /* name used in .dict files       */
   char        s_gregg     [LEN_LABEL];     /* gregg suffix                   */
   short       s_drawn     [LEN_LABEL];     /* fast gregg suffix (indexed)    */
   /*---(update)---------------*/
   char        s_type;                      /* normal, add-on, or group       */
   char        s_base      [LEN_TITLE];     /* first add this base            */
   char        s_change    [LEN_LABEL];     /* modifying this way             */
   /*---(cats)-----------------*/
   char        s_part;                      /* primary part of speech         */
   char        s_sub;                       /* sub-part                       */
   /*---(source)---------------*/
   short       s_line;                      /* line in suffix.txt             */
   char        s_src;                       /* source version of gregg        */
   char        s_cat;                       /* word-sign, normal, custom, ... */
   short       s_page;                      /* location within source         */
   /*---(count)----------------*/
   short       s_used;                      /* how many time in dictionary    */
   /*---(done)-----------------*/
};
static tSUFFIX  s_suffix [MAX_SUFFIX];
static short    s_nsuffix = 0;



/*============================--------------------============================*/
/*===----                        program level                         ----===*/
/*============================--------------------============================*/
static void o___PROGRAM___________________o (void) {;}

char
SUFFIX__purge           (void)
{
   /*---(locals)-----------+-----------+-*/
   int         i           =    0;
   int         j           =    0;
   for (i = 0; i < MAX_SUFFIX; ++i) {
      /*---(main)-----------------*/
      s_suffix [i].s_ref  = -1;
      strlcpy (s_suffix [i].s_name   , "", LEN_LABEL);
      strlcpy (s_suffix [i].s_gregg  , "", LEN_LABEL);
      for (j = 0; j < LEN_LABEL; ++j)   s_suffix [i].s_drawn [j] = 0;
      /*---(update)---------------*/
      s_suffix [i].s_type = '·';
      strlcpy (s_suffix [i].s_base   , "", LEN_TITLE);
      strlcpy (s_suffix [i].s_change , "", LEN_LABEL);
      /*---(cats)-----------------*/
      s_suffix [i].s_part = '·';
      s_suffix [i].s_sub  = '·';
      /*---(source)---------------*/
      s_suffix [i].s_line = 0;
      s_suffix [i].s_src  = '·';
      s_suffix [i].s_cat  = '·';
      s_suffix [i].s_page = 0;
      /*---(count)----------------*/
      s_suffix [i].s_used = 0;
      /*---(done------------------*/
   }
   s_nsuffix = 0;
   /*---(complete)-----------------------*/
   return 0;
}

char
SUFFIX_init             (void)
{
   strlcpy (my.n_suffix, "/var/lib/gregg/suffix.txt", LEN_HUND);
   return SUFFIX__purge ();
}

char SUFFIX_wrap     (void)  { return SUFFIX__purge (); }



/*============================--------------------============================*/
/*===----                          data load                           ----===*/
/*============================--------------------============================*/
static void o___LOAD______________________o (void) {;}

char
SUFFIX__handler         (int n, char a_verb [LEN_LABEL], char a_exist, void *a_handler)
{
   /*---(locals)-----------+-----------+-*/
   char        rce         =  -10;
   int         rc          =    0;
   int         c           =    0;
   char        x_nada      [LEN_LABEL] = "";
   char        x_cats      [LEN_LABEL] = "";
   int         l           =    0;
   /*---(header)-------------------------*/
   DEBUG_INPT  yLOG_enter   (__FUNCTION__);
   /*---(defense)------------------------*/
   DEBUG_INPT  yLOG_point   ("a_verb"    , a_verb);
   --rce;  if (a_verb == NULL || strlen (a_verb) <= 0) {
      DEBUG_INPT  yLOG_exit    (__FUNCTION__);
      yURG_err ('f', "suffix handler called with null/empty verb");
      return rce;
   }
   DEBUG_INPT  yLOG_info    ("a_verb"    , a_verb);
   /*---(check fields)-------------------*/
   rc = yPARSE_ready (&c);
   DEBUG_INPT  yLOG_value   ("fields"    , c);
   --rce;  if (c < 7) {
      DEBUG_INPT  yLOG_exit    (__FUNCTION__);
      yURG_err ('f', "not enough fields for suffix");
      return rce;
   }
   /*---(pull all)-----------------------*/
   rc = yPARSE_scanf (a_verb, "LLL3L", s_suffix [s_nsuffix].s_gregg, x_cats, x_nada, s_suffix [s_nsuffix].s_base, s_suffix [s_nsuffix].s_change);
   DEBUG_INPT  yLOG_value   ("handler"   , rc);
   --rce;  if (rc < 0) {
      DEBUG_INPT  yLOG_exit    (__FUNCTION__);
      yURG_err ('f', "yparse could not process record");
      return rce;
   }
   DEBUG_INPT  yLOG_complex ("inputs"    , "%-20.20s, %-20.20s, %-20.20s, %-20.20s, %s", s_suffix [s_nsuffix].s_gregg, x_cats, x_nada, s_suffix [s_nsuffix].s_base, s_suffix [s_nsuffix].s_change);
   /*---(fluff and filler)---------------*/
   if (s_suffix [s_nsuffix].s_gregg  [0]           == '¬')   strcpy (s_suffix [s_nsuffix].s_gregg , "");
   if (strcmp (s_suffix [s_nsuffix].s_base  , "·") == 0)     strcpy (s_suffix [s_nsuffix].s_base  , "");
   if (s_suffix [s_nsuffix].s_base   [0]           == '¬')   strcpy (s_suffix [s_nsuffix].s_base  , "");
   if (x_cats [0]                                  == '¬')   strcpy (x_cats                       , "");
   if (s_suffix [s_nsuffix].s_change [0]           == '¬')   strcpy (s_suffix [s_nsuffix].s_change, "");
   if (strcmp (s_suffix [s_nsuffix].s_change, "·") == 0)     strcpy (s_suffix [s_nsuffix].s_change, "");
   /*---(add non-scanf)------------------*/
   s_suffix [s_nsuffix].s_ref  = s_nsuffix;
   s_suffix [s_nsuffix].s_line = n;
   strlcpy (s_suffix [s_nsuffix].s_name, a_verb, LEN_LABEL);
   DEBUG_INPT  yLOG_complex ("final"     , "%3d#, %-10.10s, %s", s_nsuffix, s_suffix [s_nsuffix].s_name, s_suffix [s_nsuffix].s_gregg);
   /*---(categories)---------------------*/
   l = strlen (x_cats);
   if (l >=  1)   s_suffix [s_nsuffix].s_part = x_cats [0];
   if (l >=  3)   s_suffix [s_nsuffix].s_sub  = x_cats [2];
   if (l >=  6)   s_suffix [s_nsuffix].s_src  = x_cats [5];
   if (l >=  8)   s_suffix [s_nsuffix].s_cat  = x_cats [7];
   if (l >= 10)   s_suffix [s_nsuffix].s_page = atoi (x_cats + 9);
   DEBUG_INPT  yLOG_complex ("cats"      , "%2d len, %3d %3d  %3d %3d %3d", l, s_suffix [s_nsuffix].s_part, s_suffix [s_nsuffix].s_sub, s_suffix [s_nsuffix].s_src, s_suffix [s_nsuffix].s_cat, s_suffix [s_nsuffix].s_page);
   /*---(type)---------------------------*/
   if      (strchr ("*+-", s_suffix [s_nsuffix].s_name [0]) != NULL)   s_suffix [s_nsuffix].s_type = '+';
   else if (s_suffix [s_nsuffix].s_base [0] != '\0')                   s_suffix [s_nsuffix].s_type = 'a';
   else                                                                s_suffix [s_nsuffix].s_type = '·';
   /*---(increment)----------------------*/
   ++s_nsuffix;
   DEBUG_INPT  yLOG_value   ("s_nsuffix" , s_nsuffix);
   /*---(complete)-----------------------*/
   DEBUG_INPT  yLOG_exit    (__FUNCTION__);
   return rc;
}

char
SUFFIX_load             (char a_fname [LEN_HUND])
{
   /*---(locals)-----------+-----+-----+-*/
   char        rce         =  -10;
   char        rc          =    0;
   /*---(header)-------------------------*/
   DEBUG_INPT  yLOG_enter   (__FUNCTION__);
   /*---(begin feedback)-----------------*/
   yURG_msg ('>', "load the latest suffix file...");
   DEBUG_INPT   yLOG_point   ("a_fname"   , a_fname);
   --rce;  if (a_fname == NULL) {
      yURG_err ('f', "file name not provided to function");
      DEBUG_INPT  yLOG_exitr   (__FUNCTION__, rce);
      return rce;
   }
   yURG_msg ('-', "requested file %då%sæ", strlen (a_fname), a_fname);
   DEBUG_INPT   yLOG_info    ("a_fname"   , a_fname);
   /*---(read all lines)-----------------*/
   yURG_msg ('-', "calling auto-reader in yPARSE");
   rc = yPARSE_autoread (a_fname, NULL, SUFFIX__handler);
   DEBUG_PROG  yLOG_value   ("read"      , rc);
   /*---(trouble)------------------------*/
   --rce;  if (rc <  0) {
      DEBUG_PROG  yLOG_note    ("no file found");
      yURG_err ('f', "file not found/accessible");
      yURG_msg (' ', "");
      DEBUG_PROG  yLOG_exitr   (__FUNCTION__, rce);
      return rce;
   }
   /*---(show success)-------------------*/
   yURG_msg ('>', "all read correctly, SUCCESS, accepted %d", s_nsuffix);
   yURG_msg (' ', "");
   /*---(complete)-----------------------*/
   DEBUG_INPT  yLOG_exit    (__FUNCTION__);
   return 0;
}



/*============================--------------------============================*/
/*===----                        debug support                         ----===*/
/*============================--------------------============================*/
static void o___DEBUG_____________________o (void) {;}

short    SUFFIX__count   (void)   { return s_nsuffix; }

char*
SUFFIX__detail_by_ptr   (short n, void *a_suffix)
{
   tSUFFIX    *x_suffix    = NULL;
   char        r           [LEN_LABEL] = "·åæ";
   char        s           [LEN_LABEL] = "·åæ";
   char        t           [LEN_LABEL] = "·åæ";
   char        u           [LEN_LABEL] = "·  ";
   char        v           [LEN_LABEL] = "·";
   char        w           [LEN_LABEL] = "·";
   if (a_suffix == NULL) {
      strcpy  (g_print, "n/a");
      return g_print;
   }
   x_suffix = (tSUFFIX *) a_suffix;
   strldisp (x_suffix->s_name  ,  9, r, 12);
   strldisp (x_suffix->s_gregg ,  9, t, 12);
   if (x_suffix->s_page > 0)  sprintf  (u, "%-3d", x_suffix->s_page);
   if (x_suffix->s_used > 0)  sprintf  (v, "%d", x_suffix->s_used);
   strldisp (x_suffix->s_change,  9, s, 12);
   strldisp (x_suffix->s_base  ,  9, w, 12);
   sprintf (g_print, "%-3d  %-3d  %-12.12s  %c  %c %c  %c %c %-3.3s  %-12.12s  %-12.12s  %-12.12s  %s",
         n, x_suffix->s_line, r, x_suffix->s_type,
         x_suffix->s_part, x_suffix->s_sub,  x_suffix->s_src, x_suffix->s_cat, u,
         t, w, s, v);
   return g_print;
}

char*
SUFFIX__detail          (short n)
{
   strcpy  (g_print, "n/a");
   if (n >= 0 && n < s_nsuffix)  SUFFIX__detail_by_ptr (n, &(s_suffix [n]));
   return g_print;
}



/*============================--------------------============================*/
/*===----                        data finding                          ----===*/
/*============================--------------------============================*/
static void o___FIND______________________o (void) {;}

short
SUFFIX_by_name          (char a_request [LEN_LABEL], char *r_type, char r_gregg [LEN_LABEL], char r_base [LEN_TITLE], char r_change [LEN_LABEL], void **r_point)
{
   /*---(locals)-----------+-----+-----+-*/
   char        rce         =  -10;
   int         i           =    0;
   /*---(default)------------------------*/
   if (r_type    != NULL)  *r_type  = '·';
   if (r_gregg   != NULL)  strcpy (r_gregg  , "");
   if (r_base    != NULL)  strcpy (r_base   , "");
   if (r_change  != NULL)  strcpy (r_change , "");
   if (r_point   != NULL)  *r_point  = NULL;
   /*---(defense)------------------------*/
   --rce;  if (a_request == NULL)  return rce;
   /*---(search)-------------------------*/
   for (i = 0; i < s_nsuffix; ++i) {
      /*---(filter)----------------------*/
      if (s_suffix [i].s_name [0] != a_request [0])       continue;  /* quick check first           */
      if (strcmp (s_suffix [i].s_name, a_request) != 0)   continue;  /*    then longer if necessary */
      /*---(suffix)----------------------*/
      if (r_gregg != NULL)  strlcpy (r_gregg, s_suffix [i].s_gregg  , LEN_LABEL);
      if (r_type  != NULL)  *r_type  = s_suffix [i].s_type;
      if (r_point != NULL)  *r_point = &(s_suffix [i]);
      ++(s_suffix [i].s_used);
      /*---(group)-----------------------*/
      if      (s_suffix [i].s_type == '+') {
         if (r_base    != NULL)  strlcpy (r_base  , s_suffix [i].s_base  , LEN_TITLE);
      }
      /*---(single)----------------------*/
      else if (s_suffix [i].s_type == 'a') {
         if (r_base    != NULL)  strlcpy (r_base  , s_suffix [i].s_base  , LEN_TITLE);
         if (r_change  != NULL)  strlcpy (r_change, s_suffix [i].s_change, LEN_LABEL);
      }
      /*---(done)------------------------*/
      return i;
   }
   /*---(default)------------------------*/
   --rce;  return rce;
}



/*============================--------------------============================*/
/*===----                        word creation                         ----===*/
/*============================--------------------============================*/
static void o___DRIVER____________________o (void) {;}

char
SUFFIX_english_change   (cchar a_english [LEN_TITLE], cchar a_change [LEN_LABEL], char r_update [LEN_TITLE])
{
   /*---(locals)-----------+-----+-----+-*/
   char        rce         =  -10;
   char        l           =    0;
   /*---(default)------------------------*/
   if (r_update != NULL)  strcpy (r_update, "");
   /*---(defenses)-----------------------*/
   --rce;  if (a_english == NULL)   return rce;
   --rce;  if (a_change  == NULL)   return rce;
   --rce;  if (r_update  == NULL)   return rce;
   /*---(prepare)------------------------*/
   strlcpy (r_update, a_english, LEN_TITLE);
   l = strlen (r_update);
   /*---(make updates)-------------------*/
   if (strncmp (a_change, "×××", 3) == 0) {
      r_update [l - 3] = r_update [l - 2] = r_update [l - 1] = '\0';
      strlcat (r_update, a_change + 3, LEN_TITLE);
   } else if (strncmp (a_change, "××", 2) == 0) {
      r_update [l - 2] = r_update [l - 1] = '\0';
      strlcat (r_update, a_change + 2, LEN_TITLE);
   } else if (a_change [0] == '×') {
      r_update [l - 1] = '\0';
      strlcat (r_update, a_change + 1, LEN_TITLE);
   } else {
      strlcat (r_update, a_change    , LEN_TITLE);
   }
   /*---(complete)-----------------------*/
   return 0;
}

short
SUFFIX__field_OLD       (cchar a_eprefix [LEN_TERSE], cchar a_field [LEN_TITLE], char r_vary [LEN_TERSE], char r_english [LEN_TITLE], char *r_type, char r_gsuffix [LEN_LABEL], char r_base [LEN_TITLE], char r_change [LEN_LABEL])
{
   /*---(locals)-----------+-----+-----+-*/
   char        rce         =  -10;
   char        rc          =    0;
   char       *p           = NULL;
   char        l           =    0;
   char        x_left      =    0;
   char        x_type      =  '·';
   char        x_request   [LEN_TERSE]  = "";
   char        x_gregg     [LEN_LABEL]  = "";
   char        x_base      [LEN_TITLE]  = "";
   char        x_change    [LEN_LABEL]  = "";
   char        x_english   [LEN_TITLE]  = "";
   short       n           =    0;
   /*---(header)-------------------------*/
   DEBUG_CONF   yLOG_enter   (__FUNCTION__);
   /*---(defenses)-----------------------*/
   DEBUG_CONF   yLOG_point   ("a_field"   , a_field);
   --rce;  if (a_field == NULL) {
      DEBUG_CONF   yLOG_exitr   (__FUNCTION__, rce);
      return rce;
   }
   DEBUG_CONF   yLOG_info    ("a_field"   , a_field);
   /*---(get request)--------------------*/
   p = strchr (a_field, ')');
   DEBUG_CONF   yLOG_point   ("paren"     , p);
   --rce;  if (p == NULL) {
      DEBUG_CONF   yLOG_exitr   (__FUNCTION__, rce);
      return rce;
   }
   l = p - a_field;
   DEBUG_CONF   yLOG_value   ("l"         , l);
   --rce;  if (l < 1 || l > 4) {
      DEBUG_CONF   yLOG_exitr   (__FUNCTION__, rce);
      return rce;
   }
   sprintf  (x_request, "%*.*s", l, l, a_field);
   strltrim (x_request, ySTR_BOTH, LEN_LABEL);
   /*---(get english)--------------------*/
   l = strlen (a_field);
   x_left = l - (p - a_field) - 2;
   DEBUG_CONF   yLOG_value   ("l"         , l);
   DEBUG_CONF   yLOG_value   ("x_left"    , x_left);
   --rce;  if (x_left < 2) {
      DEBUG_CONF   yLOG_exitr   (__FUNCTION__, rce);
      return rce;
   }
   if (a_eprefix == NULL)   sprintf (x_english, "%s"   , p + 2);
   else                     sprintf (x_english, "%s%s"   , a_eprefix, p + 2);
   /*---(find variation)-----------------*/
   n = SUFFIX_by_name (x_request, &x_type, x_gregg, x_base, x_change, NULL);
   DEBUG_CONF   yLOG_value   ("suffix"    , n);
   --rce;  if (n < 0) {
      DEBUG_CONF   yLOG_exitr   (__FUNCTION__, rce);
      return rce;
   }
   /*---(save-back)----------------------*/
   if (r_vary    != NULL)  strlcpy (r_vary   , x_request, LEN_TERSE);
   if (r_english != NULL)  strlcpy (r_english, x_english, LEN_TITLE);
   if (r_type    != NULL)  *r_type  = x_type;
   if (r_gsuffix != NULL)  strlcpy (r_gsuffix, x_gregg  , LEN_LABEL);
   if (r_base    != NULL)  strlcpy (r_base   , x_base   , LEN_TITLE);
   if (r_change  != NULL)  strlcpy (r_change , x_change , LEN_LABEL);
   /*---(complete)-----------------------*/
   DEBUG_CONF   yLOG_exit    (__FUNCTION__);
   return n;
}

char
SUFFIX__field           (cchar a_field [LEN_TITLE], char r_request [LEN_TERSE], char r_english [LEN_TITLE])
{
   /*---(locals)-----------+-----+-----+-*/
   char        rce         =  -10;
   char        rc          =    0;
   char       *p           = NULL;
   char        l           =    0;
   char        x_left      =    0;
   char        x_request   [LEN_TERSE]  = "";
   char        x_english   [LEN_TITLE]  = "";
   /*---(header)-------------------------*/
   DEBUG_CONF   yLOG_enter   (__FUNCTION__);
   /*---(default)------------------------*/
   if (r_request != NULL)  strcpy (r_request, "");
   if (r_english != NULL)  strcpy (r_english, "");
   /*---(defenses)-----------------------*/
   DEBUG_CONF   yLOG_point   ("a_field"   , a_field);
   --rce;  if (a_field == NULL || a_field [0] == '\0') {
      DEBUG_CONF   yLOG_exitr   (__FUNCTION__, rce);
      return rce;
   }
   DEBUG_CONF   yLOG_info    ("a_field"   , a_field);
   l = strlen (a_field);
   DEBUG_CONF   yLOG_point   ("l"         , l);
   --rce;  if (l < 4) {
      DEBUG_CONF   yLOG_exitr   (__FUNCTION__, rce);
      return rce;
   }
   /*---(get request)--------------------*/
   p = strchr (a_field, ')');
   DEBUG_CONF   yLOG_point   ("paren"     , p);
   --rce;  if (p == NULL) {
      DEBUG_CONF   yLOG_exitr   (__FUNCTION__, rce);
      return rce;
   }
   l = p - a_field;
   DEBUG_CONF   yLOG_value   ("l"         , l);
   --rce;  if (l < 1 || l > 4) {
      DEBUG_CONF   yLOG_exitr   (__FUNCTION__, rce);
      return rce;
   }
   sprintf  (x_request, "%*.*s", l, l, a_field);
   strltrim (x_request, ySTR_BOTH, LEN_LABEL);
   DEBUG_CONF   yLOG_complex ("x_request" , "%2då%sæ", l, x_request);
   /*---(get english)--------------------*/
   l = strlen (a_field);
   DEBUG_CONF   yLOG_value   ("l"         , l);
   sprintf (x_english, "%s"   , p + 2);
   strltrim (x_english, ySTR_BOTH, LEN_TITLE);
   l = strlen (x_english);
   DEBUG_CONF   yLOG_complex ("x_english" , "%2då%sæ", l, x_english);
   /*---(save-back)----------------------*/
   if (r_request != NULL)  strlcpy (r_request, x_request, LEN_TERSE);
   if (r_english != NULL)  strlcpy (r_english, x_english, LEN_TITLE);
   /*---(complete)-----------------------*/
   DEBUG_CONF   yLOG_exit    (__FUNCTION__);
   return 0;
}

char
SUFFIX__single          (void *a_base, void *a_prefix, void *a_suffix, cchar a_english [LEN_TITLE], cchar a_gregg [LEN_TITLE])
{
   /*---(locals)-----------+-----+-----+-*/
   char        rce         =  -10;
   char        rc          =    0;
   tSUFFIX    *x_suffix    = NULL;
   char        x_updated   [LEN_TITLE] = "";
   char        x_gregg     [LEN_TITLE] = "";
   /*---(header)-------------------------*/
   DEBUG_CONF   yLOG_enter   (__FUNCTION__);
   /*---(defenses)-----------------------*/
   DEBUG_CONF   yLOG_point   ("a_base"    , a_base);
   --rce;  if (a_base    == NULL) {
      DEBUG_CONF   yLOG_exitr   (__FUNCTION__, rce);
      return rce;
   }
   DEBUG_CONF   yLOG_point   ("a_suffix"  , a_suffix);
   --rce;  if (a_suffix  == NULL) {
      DEBUG_CONF   yLOG_exitr   (__FUNCTION__, rce);
      return rce;
   }
   /*---(make variation)--------------*/
   x_suffix = (tSUFFIX *) a_suffix;
   rc = SUFFIX_english_change (a_english, x_suffix->s_change, x_updated);
   DEBUG_CONF   yLOG_value   ("english"   , rc);
   --rce;  if (rc < 0) {
      DEBUG_CONF   yLOG_exitr   (__FUNCTION__, rce);
      return rce;
   }
   DEBUG_CONF   yLOG_info    ("x_updated" , x_updated);
   /*---(update gregg)----------------*/
   sprintf (x_gregg, "%s%s", a_gregg, x_suffix->s_gregg);
   DEBUG_CONF   yLOG_info    ("x_gregg"   , x_gregg);
   /*---(create dictionary entry)-----*/
   rc = DICT_create (x_updated, x_gregg, a_prefix, a_base, x_suffix, NULL);
   DEBUG_CONF   yLOG_value   ("creator"   , rc);
   --rce;  if (rc < 0) {
      DEBUG_CONF   yLOG_exitr   (__FUNCTION__, rce);
      return rce;
   }
   /*---(complete)-----------------------*/
   DEBUG_CONF   yLOG_exit    (__FUNCTION__);
   return rc;
}

char
SUFFIX_driver           (void *a_base, void *a_prefix, cchar a_field [LEN_TITLE], cchar a_gregg [LEN_TITLE])
{
   /*---(locals)-----------+-----+-----+-*/
   char        rce         =  -10;
   char        rc          =    0;
   char        x_request   [LEN_TERSE] = "";
   char        t           [LEN_TITLE] = "";
   char        s           [LEN_TITLE] = "";
   char        x_english   [LEN_TITLE] = "";
   char        x_prefix    [LEN_LABEL] = "";
   char        x_action    =  '-';
   tSUFFIX    *x_suffix    = NULL;
   tSUFFIX    *x_other     = NULL;
   char        x_base      [LEN_LABEL] = "";
   short       n           =    0;
   char       *p           = NULL;
   char       *q           =  ",";
   char       *r           = NULL;
   /*---(header)-------------------------*/
   DEBUG_CONF   yLOG_enter   (__FUNCTION__);
   /*---(defenses)-----------------------*/
   DEBUG_CONF   yLOG_point   ("a_base"    , a_base);
   --rce;  if (a_base    == NULL) {
      DEBUG_CONF   yLOG_exitr   (__FUNCTION__, rce);
      return rce;
   }
   DEBUG_CONF   yLOG_point   ("a_field"   , a_field);
   --rce;  if (a_field   == NULL) {
      DEBUG_CONF   yLOG_exitr   (__FUNCTION__, rce);
      return rce;
   }
   DEBUG_CONF   yLOG_info    ("a_field"   , a_field);
   /*---(parse)--------------------------*/
   rc = SUFFIX__field  (a_field, x_request, t);
   DEBUG_CONF   yLOG_value   ("parse"     , rc);
   --rce;  if (rc < 0) {
      DEBUG_CONF   yLOG_exitr   (__FUNCTION__, rce);
      return rce;
   }
   /*---(handle prefix)------------------*/
   if (a_prefix == NULL)  strlcpy (x_english, t, LEN_TITLE);
   else                   PREFIX_english_final (a_prefix, t, x_english);
   /*---(find variation)-----------------*/
   n = SUFFIX_by_name (x_request, &x_action, NULL, x_base, NULL, &x_suffix);
   DEBUG_CONF   yLOG_value   ("suffix"    , n);
   --rce;  if (n < 0) {
      DEBUG_CONF   yLOG_exitr   (__FUNCTION__, rce);
      return rce;
   }
   DEBUG_CONF   yLOG_char    ("x_action"  , x_action);
   /*---(handle simple)------------------*/
   --rce;  if (x_action == '·') {
      rc = SUFFIX__single (a_base, a_prefix, x_suffix, x_english, a_gregg);
      DEBUG_CONF   yLOG_value   ("simple"    , rc);
   }
   /*---(handle add-on)------------------*/
   else if (x_action == 'a') {
      /*---(handle base)-----------------*/
      n = SUFFIX_by_name (x_base, NULL, NULL, NULL, NULL, &x_other);
      DEBUG_CONF   yLOG_value   ("suffix"    , n);
      rc = SUFFIX__single (a_base, a_prefix, x_other , x_english, a_gregg);
      DEBUG_CONF   yLOG_value   ("base one"  , rc);
      /*---(then request)----------------*/
      rc = SUFFIX__single (a_base, a_prefix, x_suffix, x_english, a_gregg);
      DEBUG_CONF   yLOG_value   ("add-on"    , rc);
      /*---(done)------------------------*/
   }
   /*---(handle groups)------------------*/
   else if (x_action == '+') {
      /*---(handle base)-----------------*/
      p = strtok_r (x_base, q, &r);
      n = SUFFIX_by_name (p, NULL, NULL, NULL, NULL, &x_other);
      DEBUG_CONF   yLOG_value   ("suffix"    , n);
      rc = SUFFIX__single (a_base, a_prefix, x_other, x_english, a_gregg);
      DEBUG_CONF   yLOG_value   ("base one"  , rc);
      /*---(work through others)---------*/
      p = strtok_r (NULL, q, &r);
      DEBUG_CONF   yLOG_point   ("next"      , p);
      while (p != NULL) {
         n = SUFFIX_by_name (p, NULL, NULL, NULL, NULL, &x_suffix);
         DEBUG_CONF   yLOG_value   ("suffix"    , n);
         rc = SUFFIX__single (a_base, a_prefix, x_suffix, x_english, a_gregg);
         DEBUG_CONF   yLOG_value   ("add-on"    , rc);
         p = strtok_r (NULL, q, &r);
         DEBUG_CONF   yLOG_point   ("next"      , p);
      }
      /*---(done)------------------------*/
   }
   /*---(handle errors)------------------*/
   else {
      DEBUG_CONF   yLOG_note    ("unknown suffix action");
      DEBUG_CONF   yLOG_exitr   (__FUNCTION__, rce);
      return rce;
   }
   /*---(complete)-----------------------*/
   DEBUG_CONF   yLOG_exit    (__FUNCTION__);
   return 0;
}

char
SUFFIX_english          (void *a_suffix, char r_english [LEN_LABEL])
{
   /*---(locals)-------------------------*/
   char        rce         =  -10;
   tSUFFIX    *x_suffix    = NULL;
   /*---(default)------------------------*/
   if (r_english != NULL)  strcpy (r_english, "");
   /*---(defense)------------------------*/
   --rce;  if (a_suffix  == NULL)  return rce;
   /*---(save-back)----------------------*/
   x_suffix = (tSUFFIX *) a_suffix;
   strlcpy (r_english, x_suffix->s_name, LEN_LABEL);
   /*---(complete)-----------------------*/
   return 0;
}



/*============================--------------------============================*/
/*===----                           reporting                          ----===*/
/*============================--------------------============================*/
static void o___REPORT____________________o (void) {;}

char
SUFFIX_dump             (FILE *f)
{
   /*---(locals)-----------+-----+-----+-*/
   int         i           =    0;
   char        x_save      =    0;
   /*---(search)-------------------------*/
   for (i = 0; i < s_nsuffix; ++i) {
      if (s_suffix [i].s_gregg [2] != x_save)  fprintf (f, "\n");
      fprintf (f, "%s\n", SUFFIX__detail (i));
      x_save = s_suffix [i].s_gregg [2];
   }
   /*---(done)------------------------*/
   return 0;
}



/*====================-----------------==-----------------====================*/
/*===----                      database handling                       ----===*/
/*====================-----------------==-----------------====================*/
static void o___DATABASE__________________o (void) {;}

short
SUFFIX_encode           (void *a_suffix)
{
   tSUFFIX    *x_suffix    = NULL;
   if (a_suffix == NULL)  return 0;
   x_suffix = (tSUFFIX *) a_suffix;
   return  x_suffix->s_ref;
}

void*
SUFFIX_decode           (short a_suffix)
{
   if (a_suffix == 0)     return NULL;
   return  &(s_suffix [a_suffix]);
}

char
SUFFIX_write            (FILE *a_file)
{
   /*---(locals)-----------+-----+-----+-*/
   char        rce         =  -10;
   char        rc          =    0;
   short       i           =    0;
   /*---(header)-------------------------*/
   DEBUG_OUTP   yLOG_enter   (__FUNCTION__);
   /*---(defense)------------------------*/
   DEBUG_OUTP  yLOG_point   ("a_file"    , a_file);
   --rce;  if (a_file == NULL) {
      DEBUG_OUTP  yLOG_exitr   (__FUNCTION__, rce);
      return rce;
   }
   DEBUG_OUTP  yLOG_value   ("s_nsuffix" , s_nsuffix);
   --rce;  if (s_nsuffix <= 0) {
      DEBUG_OUTP  yLOG_exitr   (__FUNCTION__, rce);
      return rce;
   }
   /*---(write all)----------------------*/
   for (i = 0; i < s_nsuffix; ++i) {
      rc = fwrite (&(s_suffix [i]), sizeof (tSUFFIX), 1, a_file);
      if (rc != 1)   break;
   }
   /*---(check)--------------------------*/
   DEBUG_OUTP  yLOG_value   ("i"         , i);
   --rce;  if (s_nsuffix != i) {
      DEBUG_OUTP  yLOG_exitr   (__FUNCTION__, rce);
      return rce;
   }
   /*---(complete)-----------------------*/
   DEBUG_OUTP   yLOG_exit    (__FUNCTION__);
   return 0;
}

char
SUFFIX_read             (FILE *a_file, short a_count)
{
   /*---(locals)-----------+-----+-----+-*/
   char        rce         =  -10;
   char        rc          =    0;
   short       i           =    0;
   /*---(header)-------------------------*/
   DEBUG_OUTP   yLOG_enter   (__FUNCTION__);
   /*---(defense)------------------------*/
   DEBUG_OUTP  yLOG_point   ("a_file"    , a_file);
   --rce;  if (a_file == NULL) {
      DEBUG_OUTP  yLOG_exitr   (__FUNCTION__, rce);
      return rce;
   }
   DEBUG_OUTP  yLOG_value   ("s_nsuffix" , s_nsuffix);
   --rce;  if (s_nsuffix > 0) {
      DEBUG_OUTP  yLOG_exitr   (__FUNCTION__, rce);
      return rce;
   }
   /*---(write all)----------------------*/
   for (i = 0; i < a_count; ++i) {
      rc = fread  (&(s_suffix [i]), sizeof (tSUFFIX), 1, a_file);
      if (rc != 1)   break;
      ++s_nsuffix;
   }
   /*---(check)--------------------------*/
   DEBUG_OUTP  yLOG_value   ("a_count"   , a_count);
   --rce;  if (s_nsuffix != a_count) {
      DEBUG_OUTP  yLOG_exitr   (__FUNCTION__, rce);
      return rce;
   }
   /*---(complete)-----------------------*/
   DEBUG_OUTP   yLOG_exit    (__FUNCTION__);
   return 0;
}



