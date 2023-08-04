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

char         /*-> clear a single suffix entry -----------------[ leaf······ ]-*/ /*-åfcg´·1á···1á···á·´6·7´2··´·1·1·á·æ¬å1á1···´···á·······´···´····æ¬å·····´1···1··´····´····æ-*/
SUFFIX__wipe            (void *c_curr)
{
   /*---(locals)-----------+-----------+-*/
   tSUFFIX    *x_curr      = NULL;
   int         i           =    0;
   /*---(prepare)--------------*/
   x_curr = (tSUFFIX *) c_curr;
   /*---(main)-----------------*/
   x_curr->s_ref        = -1;
   x_curr->s_name   [0] = '\0';
   x_curr->s_gregg  [0] = '\0';
   for (i = 0; i < LEN_LABEL; ++i)   x_curr->s_drawn [i] = 0;
   /*---(update)---------------*/
   x_curr->s_type       = '·';
   x_curr->s_base   [0] = '\0';
   x_curr->s_change [0] = '\0';
   /*---(cats)-----------------*/
   x_curr->s_part       = '·';
   x_curr->s_sub        = '·';
   /*---(source)---------------*/
   x_curr->s_line       = 0;
   x_curr->s_src        = '·';
   x_curr->s_cat        = '·';
   x_curr->s_page       = 0;
   /*---(count)----------------*/
   x_curr->s_used       = 0;
   /*---(done------------------*/
   return 0;
}

char         /*-> wipe all suffix data ------------------------[ node+····· ]-*/ /*-åfcg´·vá····á···á·´1·A´12·´·1·1·á·æ¬å2á2···´1·1á1······´···´····æ¬å·····´11·····´····´234·æ-*/
SUFFIX__purge           (void)
{
   int         i           =    0;
   for (i = 0; i < MAX_SUFFIX; ++i)  SUFFIX__wipe (&s_suffix [i]);
   s_nsuffix = 0;
   return 0;
}

char         /*-> suffix initialization -----------------------[ ´········· ]-*/ /*-åûcg´·vá····á···á·´1·A´··1´···1·á·æ¬å1á·1··´2·2á1··1···´···´····æ¬å·····´1···1··´····´····æ-*/
SUFFIX_init             (void)
{
   strlcpy (my.n_suffix, "/var/lib/gregg/suffix.txt", LEN_HUND);
   return SUFFIX__purge ();
}

char         /*-> suffix wrap-up ------------------------------[ node······ ]-*/ /*-åfcg´·vá····á···á·´1·A´···´···1·á·æ¬å·á····´1·1á1······´···´····æ¬å·····´·······´····´····æ-*/
SUFFIX_wrap             (void)
{
   return SUFFIX__purge ();
}



/*============================--------------------============================*/
/*===----                          data load                           ----===*/
/*============================--------------------============================*/
static void o___LOAD______________________o (void) {;}

char         /*-> parse and create suffix ---------------------[ leafy····· ]-*/ /*-åfcg´·4á4··á···´E26´83·´1·H13á·æ¬å1á1···´VGFá··C3···´···´····æ¬å>linr´3···3··´····´113·æ-*/
SUFFIX__handler         (int a_line, char a_verb [LEN_LABEL], char a_exist, void *a_handler)
{
   /*---(locals)-----------+-----------+-*/
   char        rce         =  -10;
   int         rc          =    0;
   int         c           =    0;
   int         n           =    0;
   tSUFFIX    *u           = NULL;
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
   /*---(prepare)------------------------*/
   n = s_nsuffix;
   u = &(s_suffix [n]);
   /*---(pull all)-----------------------*/
   rc = yPARSE_scanf (a_verb, "LLL3L", u->s_gregg, x_cats, x_nada, u->s_base, u->s_change);
   DEBUG_INPT  yLOG_value   ("handler"   , rc);
   --rce;  if (rc < 0) {
      DEBUG_INPT  yLOG_exit    (__FUNCTION__);
      yURG_err ('f', "yparse could not process record");
      return rce;
   }
   DEBUG_INPT  yLOG_complex ("inputs"    , "%-20.20s, %-20.20s, %-20.20s, %-20.20s, %s", u->s_gregg, x_cats, x_nada, u->s_base, u->s_change);
   /*---(fluff and filler)---------------*/
   if (u->s_gregg  [0]           == '¬')   strcpy (u->s_gregg  , "");
   if (strcmp (u->s_base  , "·") == 0)     strcpy (u->s_base   , "");
   if (u->s_base   [0]           == '¬')   strcpy (u->s_base   , "");
   if (x_cats [0]                == '¬')   strcpy (x_cats      , "");
   if (u->s_change [0]           == '¬')   strcpy (u->s_change , "");
   if (strcmp (u->s_change, "·") == 0)     strcpy (u->s_change , "");
   /*---(add non-scanf)------------------*/
   u->s_ref  = n;
   u->s_line = a_line;
   strlcpy (u->s_name, a_verb, LEN_LABEL);
   DEBUG_INPT  yLOG_complex ("final"     , "%3d#, %-10.10s, %s", n, u->s_name, u->s_gregg);
   /*---(categories)---------------------*/
   l = strlen (x_cats);
   if (l >=  1)   u->s_part = x_cats [0];
   if (l >=  3)   u->s_sub  = x_cats [2];
   if (l >=  6)   u->s_src  = x_cats [5];
   if (l >=  8)   u->s_cat  = x_cats [7];
   if (l >= 10)   u->s_page = atoi (x_cats + 9);
   DEBUG_INPT  yLOG_complex ("cats"      , "%2d len, %3d %3d  %3d %3d %3d", l, u->s_part, u->s_sub, u->s_src, u->s_cat, u->s_page);
   /*---(type)---------------------------*/
   if      (strchr ("*+-", u->s_name [0]) != NULL)   u->s_type = '+';
   else if (u->s_base [0] != '\0')                   u->s_type = 'a';
   else                                              u->s_type = '·';
   /*---(increment)----------------------*/
   n = ++s_nsuffix;
   DEBUG_INPT  yLOG_value   ("s_nsuffix" , n);
   /*---(complete)-----------------------*/
   DEBUG_INPT  yLOG_exit    (__FUNCTION__);
   return rc;
}

char         /*->  --------------------------------------------[ node······ ]-*/ /*-ågcg´·1á1··á···´753´21·´1·212á·æ¬å·á····´JG3á1·11···´···´····æ¬å>li··´·······´····´222·æ-*/
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

char         /*-> return english from structure ---------------[ leaf······ ]-*/ /*-ågcg´·2á11·á···´3·6´2··´··211á·æ¬å1á·1··´2·2á··11···´···´····æ¬å·····´1···1··´····´····æ-*/
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

char
SUFFIX_shown            (void *a_suffix, char r_shown [LEN_LABEL])
{
   /*---(locals)-------------------------*/
   char        rce         =  -10;
   tSUFFIX    *x_suffix    = NULL;
   /*---(default)------------------------*/
   if (r_shown != NULL)  strcpy (r_shown, "");
   /*---(defense)------------------------*/
   --rce;  if (a_suffix  == NULL)  return rce;
   /*---(save-back)----------------------*/
   x_suffix = (tSUFFIX *) a_suffix;
   strlcpy (r_shown, x_suffix->s_gregg, LEN_LABEL);
   /*---(complete)-----------------------*/
   return 0;
}



/*============================--------------------============================*/
/*===----                           reporting                          ----===*/
/*============================--------------------============================*/
static void o___REPORT____________________o (void) {;}

char         /*-> dump all suffix data to file ----------------[ ´········· ]-*/ /*-ågcg´·1á····á···á1´3·7´23·´1111·á·æ¬å1á·1··´3·3á1·2····´·w·´····æ¬å·····´·······´····´····æ-*/
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

char         /*-> write all suffixes to database --------------[ ´········· ]-*/ /*-ågcg´·1á1···á···á1´735´35·´11413á·æ¬å1á·1··´981á··1····´·W·´····æ¬å>lo··´·······´····´222·æ-*/
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

char         /*-> read all suffixes from database -------------[ ´········· ]-*/ /*-ågcg´·2á2···á···á1´736´35·´11413á·æ¬å1á·1··´981á··1····´R··´····æ¬å>lo··´·······´····´222·æ-*/
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



