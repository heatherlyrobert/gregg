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
SUFFIX_purge            (void)
{
   /*---(locals)-----------+-----------+-*/
   int         i           =    0;
   int         j           =    0;
   for (i = 0; i < MAX_SUFFIX; ++i) {
      /*---(main)-----------------*/
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
   return SUFFIX_purge ();
}

char SUFFIX_wrap     (void)  { return SUFFIX_purge (); }



/*============================--------------------============================*/
/*===----                          data load                           ----===*/
/*============================--------------------============================*/
static void o___LOAD______________________o (void) {;}

char
SUFFIX_handler          (int n, char a_verb [LEN_LABEL], char a_exist, void *a_handler)
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
   rc = yPARSE_autoread (a_fname, NULL, SUFFIX_handler);
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

short    SUFFIX_count   (void)   { return s_nsuffix; }

char*
SUFFIX_detail           (uchar n)
{
   char        r           [LEN_LABEL] = "·åæ";
   char        s           [LEN_LABEL] = "·åæ";
   char        t           [LEN_LABEL] = "·åæ";
   char        u           [LEN_LABEL] = "·  ";
   char        v           [LEN_LABEL] = "·";
   char        w           [LEN_LABEL] = "·";
   strcpy  (g_print, "n/a");
   if (n < s_nsuffix) {
      strldisp (s_suffix [n].s_name  ,  9, r, 12);
      strldisp (s_suffix [n].s_gregg ,  9, t, 12);
      if (s_suffix [n].s_page > 0)  sprintf  (u, "%-3d", s_suffix [n].s_page);
      if (s_suffix [n].s_used > 0)  sprintf  (v, "%d", s_suffix [n].s_used);
      strldisp (s_suffix [n].s_change,  9, s, 12);
      strldisp (s_suffix [n].s_base  ,  9, w, 12);
      sprintf (g_print, "%-3d  %-3d  %-12.12s  %c  %c %c  %c %c %-3.3s  %-12.12s  %-12.12s  %-12.12s  %s",
            n, s_suffix [n].s_line, r, s_suffix [n].s_type,
            s_suffix [n].s_part, s_suffix [n].s_sub,  s_suffix [n].s_src, s_suffix [n].s_cat, u,
            t, w, s, v);
   }
   return g_print;
}



/*============================--------------------============================*/
/*===----                        data finding                          ----===*/
/*============================--------------------============================*/
static void o___FIND______________________o (void) {;}

short
SUFFIX_by_name          (char a_suffix [LEN_LABEL], char *r_type, char r_gregg [LEN_LABEL], char r_base [LEN_TITLE], char r_change [LEN_LABEL])
{
   /*---(locals)-----------+-----+-----+-*/
   char        rce         =  -10;
   int         i           =    0;
   /*---(default)------------------------*/
   if (r_type    != NULL)  *r_type  = '·';
   if (r_gregg   != NULL)  strcpy (r_gregg  , "");
   if (r_base    != NULL)  strcpy (r_base   , "");
   if (r_change  != NULL)  strcpy (r_change , "");
   /*---(defense)------------------------*/
   --rce;  if (a_suffix == NULL)  return rce;
   /*---(search)-------------------------*/
   for (i = 0; i < s_nsuffix; ++i) {
      /*---(filter)----------------------*/
      if (s_suffix [i].s_name [0] != a_suffix [0])       continue;  /* quick check first           */
      if (strcmp (s_suffix [i].s_name, a_suffix) != 0)   continue;  /*    then longer if necessary */
      /*---(suffix)----------------------*/
      if (r_gregg != NULL)  strlcpy (r_gregg, s_suffix [i].s_gregg  , LEN_LABEL);
      if (r_type  != NULL)  *r_type = s_suffix [i].s_type;
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
SUFFIX__field           (cchar a_eprefix [LEN_TERSE], cchar a_field [LEN_TITLE], char r_vary [LEN_TERSE], char r_english [LEN_TITLE], char *r_type, char r_gsuffix [LEN_LABEL], char r_base [LEN_TITLE], char r_change [LEN_LABEL])
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
   n = SUFFIX_by_name (x_request, &x_type, x_gregg, x_base, x_change);
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
SUFFIX__simple          (tWORD *a_base, tWORD *a_last, cchar a_vary [LEN_TERSE], cchar a_english [LEN_TITLE], cchar a_gprefix [LEN_LABEL], cchar a_gsuffix [LEN_LABEL], tWORD **r_new)
{
   /*---(locals)-----------+-----+-----+-*/
   char        rc          =    0;
   /*---(header)-------------------------*/
   DEBUG_CONF   yLOG_enter   (__FUNCTION__);
   /*---(make variation)--------------*/
   rc = DICT__variation_quick (a_base, a_last, a_english, a_vary, a_gprefix, a_gsuffix, r_new);
   DEBUG_CONF   yLOG_value   ("variation" , rc);
   /*---(complete)-----------------------*/
   DEBUG_CONF   yLOG_exit    (__FUNCTION__);
   return rc;
}

/*> char                                                                                                                                                                                                                                           <* 
 *> SUFFIX__addon           (tWORD *a_base, tWORD *a_last, cchar a_vary [LEN_TERSE], cchar a_english [LEN_TITLE], cchar a_base [LEN_TITLE], cchar a_change [LEN_LABEL], cchar a_gprefix [LEN_LABEL], cchar a_gsuffix [LEN_LABEL], tWORD **r_new)   <* 
 *> {                                                                                                                                                                                                                                              <* 
 *>    /+> /+---(locals)-----------+-----+-----+-+/                                                      <*                                                                                                                                        <* 
 *>     *> char        rc          =    0;                                                               <*                                                                                                                                        <* 
 *>     *> char        x_english   [LEN_TITLE]  = "";                                                    <*                                                                                                                                        <* 
 *>     *> char        x_base      [LEN_TITLE] = "";                                                     <*                                                                                                                                        <* 
 *>     *> char        x_change    [LEN_LABEL] = "";                                                     <*                                                                                                                                        <* 
 *>     *> /+---(header)-------------------------+/                                                      <*                                                                                                                                        <* 
 *>     *> DEBUG_CONF   yLOG_enter   (__FUNCTION__);                                                     <*                                                                                                                                        <* 
 *>     *> /+---(update english)--------------+/                                                         <*                                                                                                                                        <* 
 *>     *> SUFFIX_english_change (a_english, a_change, x_english);                                       <*                                                                                                                                        <* 
 *>     *> DEBUG_CONF   yLOG_complex ("english"   , "%s, %s, %s", a_english, a_change, x_english);       <*                                                                                                                                        <* 
 *>     *> /+---(make variation)--------------+/                                                         <*                                                                                                                                        <* 
 *>     *> rc = DICT__variation_quick (a_base, a_last, a_english, a_vary, a_gprefix, a_suffix, r_new);   <*                                                                                                                                        <* 
 *>     *> /+---(make base)-------------------+/                                                         <*                                                                                                                                        <* 
 *>     *> rc = DICT__find_variation  (x_base, x_suffix, NULL, NULL, NULL);                              <*                                                                                                                                        <* 
 *>     *> rc = DICT__variation_quick (a_base, x_save, x_english, x_base, a_gregg, x_suffix, &x_vary);   <*                                                                                                                                        <* 
 *>     *> if (rc >= 0)  x_save = x_vary;                                                                <+/                                                                                                                                       <* 
 *> }                                                                                                                                                                                                                                              <*/

/*> /+---(also version)-------------------+/                                                         <* 
 *> else if (x_action == 'a') {                                                                      <* 
 *>    /+---(make variation)--------------+/                                                         <* 
 *>    SUFFIX_english_change (x_english, x_change, t);                                               <* 
 *>    DEBUG_CONF   yLOG_complex ("english"   , "%s, %s, %s", x_english, x_change, t);               <* 
 *>    rc = DICT__variation_quick (a_base, a_last, t, x_type, a_gregg, x_suffix, &x_vary);           <* 
 *>    if (rc >= 0)  x_save = x_vary;                                                                <* 
 *>    /+---(make base)-------------------+/                                                         <* 
 *>    DICT__find_variation (x_base, x_suffix, NULL, NULL, NULL);                                    <* 
 *>    rc = DICT__variation_quick (a_base, x_save, x_english, x_base, a_gregg, x_suffix, &x_vary);   <* 
 *>    if (rc >= 0)  x_save = x_vary;                                                                <* 
 *> }                                                                                                <*/
/*---(group version)------------------*/
/*> else if (x_action == '+') {                                                                               <* 
 *>    /+---(make first/base)-------------+/                                                                  <* 
 *>    p = strtok_r (x_base, q, &r);                                                                          <* 
 *>    /+> DICT__find_variation (p, x_suffix, NULL, NULL, NULL);                       <+/                    <* 
 *>    rc = SUFFIX_by_name (p, NULL, x_suffix, NULL, NULL);                                                   <* 
 *>    if (rc >= 0)  rc = DICT__variation_quick (a_base, a_last, x_english, p, a_gregg, x_suffix, &x_vary);   <* 
 *>    if (rc >= 0)  x_save = x_vary;                                                                         <* 
 *>    /+---(cycle others)----------------+/                                                                  <* 
 *>    p = strtok_r (NULL, q, &r);                                                                            <* 
 *>    while (p != NULL) {                                                                                    <* 
 *>       /+---(find)---------------------+/                                                                  <* 
 *>       /+> DICT__find_variation (p, x_suffix, NULL, NULL, x_change);                <+/                    <* 
 *>       rc = SUFFIX_by_name (p, NULL, x_suffix, NULL, NULL);                                                <* 
 *>       DEBUG_CONF   yLOG_complex ("variation" , "%s, %s", p, x_suffix);                                    <* 
 *>       /+---(update)-------------------+/                                                                  <* 
 *>       SUFFIX_english_change (x_english, x_change, t);                                                     <* 
 *>       DEBUG_CONF   yLOG_complex ("english"   , "%s, %s, %s", x_english, x_change, t);                     <* 
 *>       /+---(add)----------------------+/                                                                  <* 
 *>       rc = DICT__variation_quick (a_base, x_save, t, p, a_gregg, x_suffix, &x_vary);                      <* 
 *>       if (rc >= 0)  x_save = x_vary;                                                                      <* 
 *>       /+---(next)---------------------+/                                                                  <* 
 *>       p = strtok_r (NULL, q, &r);                                                                         <* 
 *>    }                                                                                                      <* 
 *>    /+---(done)------------------------+/                                                                  <*/
/*> }                                                                                 <* 
 *> /+---(save-back)----------------------+/                                          <* 
 *> if (r_new  != NULL)  *r_new = x_vary;                                             <* 
 *> /+---(complete)-----------------------+/                                          <* 
 *> DEBUG_CONF   yLOG_exit    (__FUNCTION__);                                         <* 
 *> return 0;                                                                         <*/


char
SUFFIX_driver           (tWORD *a_base, tWORD *a_last, cchar a_eprefix [LEN_TERSE], cchar a_gprefix [LEN_TERSE], cchar *a_vary, tWORD **r_new)
{
   /*---(locals)-----------+-----+-----+-*/
   char        rce         =  -10;
   char        rc          =    0;
   char        x_action    =  '-';
   char       *p           = NULL;
   char        l           =    0;
   char        x_left      =    0;
   char        x_type      [LEN_TERSE] = "";
   char        x_english   [LEN_TITLE] = "";
   char        x_suffix    [LEN_LABEL] = "";
   char        x_base      [LEN_LABEL] = "";
   char        x_change    [LEN_LABEL] = "";
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
   sprintf (x_english, "%s%s"   , a_eprefix, p + 2);
   /*---(find variation)-----------------*/
   rc = SUFFIX_by_name (x_type, &x_action, x_suffix, x_base, x_change);
   DEBUG_CONF   yLOG_value   ("suffix"    , rc);
   --rce;  if (rc < 0) {
      DEBUG_CONF   yLOG_exitr   (__FUNCTION__, rce);
      return rce;
   }
   /*---(complete)-----------------------*/
   DEBUG_CONF   yLOG_exit    (__FUNCTION__);
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
      fprintf (f, "%s\n", SUFFIX_detail (i));
      x_save = s_suffix [i].s_gregg [2];
   }
   /*---(done)------------------------*/
   return 0;
}


