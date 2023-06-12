/*============================---(source-start)---============================*/
#include    "gregg.h"

/*
 *  PURPOSE     store and apply word prefixes
 *
 */

typedef  struct  cPREFIX   tPREFIX;
struct cPREFIX {
   /*---(main)-----------------*/
   short       p_ref;                       /* ref for db write/read          */
   char        p_name      [LEN_LABEL];     /* name used in .dict files       */
   char        p_english   [LEN_LABEL];     /* text prefixed to english       */
   char        p_gregg     [LEN_LABEL];     /* gregg prefix                   */
   short       p_drawn     [LEN_LABEL];     /* fast gregg prefix (indexed)    */
   /*---(source)---------------*/
   short       p_line;                      /* line in prefix.txt             */
   char        p_src;                       /* source version of gregg        */
   char        p_cat;                       /* word-sign, normal, custom, ... */
   short       p_page;                      /* location within source         */
   /*---(count)----------------*/
   short       p_used;                      /* how many time in dictionary    */
   /*---(done)-----------------*/
};
static tPREFIX  s_prefix [LEN_FULL];
static short    s_nprefix  = 0;



/*============================--------------------============================*/
/*===----                        program level                         ----===*/
/*============================--------------------============================*/
static void o___PROGRAM___________________o (void) {;}

char
PREFIX__purge           (void)
{
   /*---(locals)-----------+-----------+-*/
   int         i           =    0;
   int         j           =    0;
   for (i = 0; i < LEN_FULL; ++i) {
      /*---(main)-----------------*/
      s_prefix [i].p_ref  = -1;
      strlcpy (s_prefix [i].p_name   , "", LEN_LABEL);
      strlcpy (s_prefix [i].p_english, "", LEN_LABEL);
      strlcpy (s_prefix [i].p_gregg  , "", LEN_LABEL);
      for (j = 0; j < LEN_LABEL; ++j)   s_prefix [i].p_drawn [j] = 0;
      /*---(source)---------------*/
      s_prefix [i].p_line = 0;
      s_prefix [i].p_src  = '·';
      s_prefix [i].p_cat  = '·';
      s_prefix [i].p_page = 0;
      /*---(count)----------------*/
      s_prefix [i].p_used = 0;
      /*---(done------------------*/
   }
   s_nprefix = 0;
   /*---(complete)-----------------------*/
   return 0;
}

char
PREFIX_init             (void)
{
   strlcpy (my.n_prefix, "/var/lib/gregg/prefix.txt", LEN_HUND);
   return PREFIX__purge ();
}

char PREFIX_wrap     (void)  { return PREFIX__purge (); }



/*============================--------------------============================*/
/*===----                          data load                           ----===*/
/*============================--------------------============================*/
static void o___LOAD______________________o (void) {;}

char
PREFIX__handler         (int n, char a_verb [LEN_LABEL], char a_exist, void *a_handler)
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
      yURG_err ('f', "prefix handler called with null/empty verb");
      return rce;
   }
   DEBUG_INPT  yLOG_info    ("a_verb"    , a_verb);
   /*---(check fields)-------------------*/
   rc = yPARSE_ready (&c);
   DEBUG_INPT  yLOG_value   ("fields"    , c);
   --rce;  if (c < 4) {
      DEBUG_INPT  yLOG_exit    (__FUNCTION__);
      yURG_err ('f', "not enough fields for prefix");
      return rce;
   }
   /*---(pull all)-----------------------*/
   rc = yPARSE_scanf (a_verb, "TTTT", x_cats, s_prefix [s_nprefix].p_english, x_nada, s_prefix [s_nprefix].p_gregg);
   DEBUG_INPT  yLOG_value   ("handler"   , rc);
   --rce;  if (rc < 0) {
      DEBUG_INPT  yLOG_exit    (__FUNCTION__);
      yURG_err ('f', "yparse could not process record");
      return rce;
   }
   /*---(finish)-------------------------*/
   s_prefix [s_nprefix].p_ref  = s_nprefix;
   s_prefix [s_nprefix].p_line = n;
   strlcpy (s_prefix [s_nprefix].p_name, a_verb, LEN_LABEL);
   l = strlen (x_cats);
   if (l >= 1)   s_prefix [s_nprefix].p_src  = x_cats [0];
   if (l >= 3)   s_prefix [s_nprefix].p_cat  = x_cats [2];
   if (l >= 5)   s_prefix [s_nprefix].p_page = atoi (x_cats + 4);
   DEBUG_INPT  yLOG_complex ("final"     , "%3d#, %-10.10s, %-10.10s, %s", s_nprefix, s_prefix [s_nprefix].p_name, s_prefix [s_nprefix].p_english, s_prefix [s_nprefix].p_gregg);
   /*---(update drawn)-------------------*/
   if (strcmp (s_prefix [s_nprefix].p_gregg, "") != 0 && strcmp (s_prefix [s_nprefix].p_gregg, "·") != 0) {
      rc = FIX_gregg  (s_prefix [s_nprefix].p_gregg, NULL, s_prefix [s_nprefix].p_drawn, NULL, NULL);
      DEBUG_INPT  yLOG_value   ("fix"       , rc);
      --rce;  if (rc < 0) {
         DEBUG_INPT  yLOG_exit    (__FUNCTION__);
         return rce;
      }
   }
   /*---(increment)----------------------*/
   ++s_nprefix;
   DEBUG_INPT  yLOG_value   ("s_nprefix" , s_nprefix);
   /*---(complete)-----------------------*/
   DEBUG_INPT  yLOG_exit    (__FUNCTION__);
   return rc;
}

char
PREFIX_load             (char a_fname [LEN_HUND])
{
   /*---(locals)-----------+-----+-----+-*/
   char        rce         =  -10;
   char        rc          =    0;
   /*---(header)-------------------------*/
   DEBUG_INPT  yLOG_enter   (__FUNCTION__);
   /*---(begin feedback)-----------------*/
   yURG_msg ('>', "load the latest prefix file...");
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
   rc = yPARSE_autoread (a_fname, NULL, PREFIX__handler);
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
   yURG_msg ('>', "all read correctly, SUCCESS, accepted %d", s_nprefix);
   yURG_msg (' ', "");
   /*---(complete)-----------------------*/
   DEBUG_INPT  yLOG_exit    (__FUNCTION__);
   return 0;
}



/*============================--------------------============================*/
/*===----                        debug support                         ----===*/
/*============================--------------------============================*/
static void o___DEBUG_____________________o (void) {;}

short    PREFIX__count  (void)   { return s_nprefix; }

char*
PREFIX__detail_by_ptr   (short n, void *a_prefix)
{
   tPREFIX    *x_prefix    = NULL;
   char        r           [LEN_LABEL] = "·åæ";
   char        s           [LEN_LABEL] = "·åæ";
   char        t           [LEN_LABEL] = "·åæ";
   char        u           [LEN_LABEL] = "·  ";
   char        v           [LEN_LABEL] = "·";
   if (a_prefix == NULL) {
      strcpy  (g_print, "n/a");
      return g_print;
   }
   x_prefix = (tPREFIX *) a_prefix;
   sprintf  (r, "%1då%.9sæ", strlen (x_prefix->p_name   ), x_prefix->p_name   );
   sprintf  (s, "%1då%.9sæ", strlen (x_prefix->p_english), x_prefix->p_english);
   sprintf  (t, "%1då%.9sæ", strlen (x_prefix->p_gregg  ), x_prefix->p_gregg  );
   if (x_prefix->p_page > 0)  sprintf  (u, "%-3d", x_prefix->p_page);
   if (x_prefix->p_used > 0)  sprintf  (v, "%d", x_prefix->p_used);
   sprintf (g_print, "%-3d  %-3d  %-12.12s  %c %c %-3.3s  %-12.12s  %-12.12s  %s", n, x_prefix->p_line, r, x_prefix->p_src, x_prefix->p_cat, u, s, t, v);
   return g_print;
}

char*
PREFIX__detail          (uchar n)
{
   strcpy  (g_print, "n/a");
   if (n < s_nprefix)  PREFIX__detail_by_ptr (n, &(s_prefix [n]));
   return g_print;
}



/*============================--------------------============================*/
/*===----                           driver                             ----===*/
/*============================--------------------============================*/
static void o___DRIVER____________________o (void) {;}

char
PREFIX_english_change   (cchar a_base [LEN_TITLE], cchar a_change [LEN_LABEL], char r_update [LEN_TITLE], char r_prefix [LEN_LABEL], char *r_trunc)
{  /*---(notes)--------------------------*/
   /*
    *  all arrows must be at end of prefix
    *     first the actual prefix
    *     then (if needed) ×
    *     last (if needed) Ö or ÖÖ
    *
    */
   /*---(locals)-----------+-----+-----+-*/
   char        rce         =  -10;
   char        l           =    0;
   char       *p           = NULL;
   char        x_update    [LEN_TITLE] = "";
   static char x_original  [LEN_LABEL] = "";
   static char x_change    [LEN_LABEL] = "";
   char        x_prefix    [LEN_LABEL] = "";
   char        x_pretty    [LEN_LABEL] = "";
   char        x_trunc     =    0;
   char        c           =  ' ';
   /*---(header)-------------------------*/
   DEBUG_INPT  yLOG_enter   (__FUNCTION__);
   /*---(default)------------------------*/
   if (r_update != NULL)  strcpy (r_update, "");
   if (r_prefix != NULL)  strcpy (r_prefix, "");
   if (r_trunc  != NULL)  *r_trunc = 0;
   /*---(defenses)-----------------------*/
   DEBUG_INPT  yLOG_point   ("a_base"    , a_base);
   --rce;  if (a_base    == NULL) {
      DEBUG_INPT  yLOG_exitr   (__FUNCTION__, rce);
      return rce;
   }
   DEBUG_INPT  yLOG_point   ("a_change"  , a_change);
   --rce;  if (a_change  == NULL) {
      DEBUG_INPT  yLOG_exitr   (__FUNCTION__, rce);
      return rce;
   }
   /*---(prepare)------------------------*/
   DEBUG_INPT  yLOG_info    ("x_original", x_original);
   DEBUG_INPT  yLOG_info    ("a_change"  , a_change);
   if (strcmp (a_change, "") != 0) {
      strlcpy (x_change  , a_change  , LEN_LABEL);
      strlcpy (x_original, a_change  , LEN_LABEL);
   } else {
      strlcpy (x_change, x_original, LEN_LABEL);
   }
   l = strlen (x_change);
   DEBUG_INPT  yLOG_complex ("x_change"  , "%2då%sæ", l, x_change);
   /*---(check for root adds)------------*/
   c = x_change [l - 1];
   DEBUG_INPT  yLOG_char    ("c"         , c);
   if (strchr (YSTR_UPPER, c) != NULL) {
      c = tolower (c);
      x_change [--l] = '\0';
   } else {
      c = '·';
   }
   DEBUG_INPT  yLOG_complex ("x_change"  , "%2då%sæ", l, x_change);
   /*---(check for root truncs)----------*/
   p = strchr (x_change, 'Ö');
   if (p != NULL) {
      if (strncmp (p, "ÖÖ", 2) == 0) {
         strlcpy (x_update, a_base + 2, LEN_TITLE);
         p [ 0] = p [ 1] = '\0';
      } else {
         strlcpy (x_update, a_base + 1, LEN_TITLE);
         p [ 0] = '\0';
      }
   } else {
      strlcpy (x_update, a_base + 0, LEN_TITLE);
   }
   /*---(check for prefix change)--------*/
   p = strchr (x_change, '×');
   if (p != NULL) {
      p [ 0] = '\0';
      strlcpy (x_prefix, x_change, LEN_LABEL);
      x_trunc = 1;
   } else {
      strlcpy (x_prefix, x_change, LEN_LABEL);
   }
   /*---(save-back)----------------------*/
   if (r_update != NULL) {
      if (c != '·')  sprintf (r_update, "%c%s", c, x_update);
      else           strlcpy (r_update, x_update, LEN_TITLE);
   }
   if (r_prefix != NULL)  strlcpy (r_prefix, x_prefix, LEN_LABEL);
   if (r_trunc  != NULL)  *r_trunc = x_trunc;
   /*---(complete)-----------------------*/
   DEBUG_INPT  yLOG_exit    (__FUNCTION__);
   return 0;
}

char
PREFIX_english_final    (void *a_prefix, cchar a_base [LEN_TITLE], char r_update [LEN_TITLE])
{
   char        rce         =  -10;
   char        rc          =    0;
   char        x_english   [LEN_TITLE];
   char        x_pretty    [LEN_LABEL];
   char        x_trunc     =    0;
   int         l           =    0;
   /*---(header)-------------------------*/
   DEBUG_INPT  yLOG_enter   (__FUNCTION__);
   /*---(defense)------------------------*/
   DEBUG_INPT  yLOG_point   ("a_prefix"  , a_prefix);
   --rce;  if (a_prefix  == NULL) {
      DEBUG_INPT  yLOG_exit    (__FUNCTION__);
      return 0;
   }
   rc = PREFIX_english (a_prefix, x_pretty);
   DEBUG_INPT  yLOG_value   ("pretty"    , rc);
   --rce;  if (rc < 0) {
      DEBUG_INPT  yLOG_exitr   (__FUNCTION__, rce);
      return rce;
   }
   DEBUG_INPT  yLOG_info    ("x_pretty"  , x_pretty);
   rc = PREFIX_english_change (a_base, "", x_english, NULL, &x_trunc);
   DEBUG_INPT  yLOG_value   ("change"    , rc);
   --rce;  if (rc < 0) {
      DEBUG_INPT  yLOG_exitr   (__FUNCTION__, rce);
      return rce;
   }
   DEBUG_INPT  yLOG_info    ("x_english" , x_english);
   DEBUG_INPT  yLOG_value   ("x_trunc"   , x_trunc);
   if (x_trunc == 1) {
      l = strlen (x_pretty);
      if (l > 0)  x_pretty [l - 1] = '\0';
   }
   /*---(save-back)----------------------*/
   if (r_update != NULL)  sprintf (r_update, "%s%s", x_pretty, x_english);
   /*---(complete)-----------------------*/
   DEBUG_INPT  yLOG_exit    (__FUNCTION__);
   return 0;
}

short
PREFIX__by_name         (char a_prefix [LEN_LABEL], char r_english [LEN_LABEL], char r_gregg [LEN_LABEL], void **r_point)
{
   /*---(locals)-------------------------*/
   char        rce         =  -10;
   char        i           =    0;
   /*---(default)------------------------*/
   if (r_english != NULL)  strcpy (r_english, "");
   if (r_gregg   != NULL)  strcpy (r_gregg  , "");
   if (r_point   != NULL)  *r_point = NULL;
   /*---(defense)------------------------*/
   --rce;  if (a_prefix  == NULL)  return rce;
   /*---(search)-------------------------*/
   for (i = 0; i < s_nprefix; ++i) {
      /*---(filter)----------------------*/
      if (s_prefix [i].p_name [0] != a_prefix [0])           continue;  /* quick check first           */
      if (strcmp (s_prefix [i].p_name, a_prefix) != 0)       continue;  /*    then longer if necessary */
      /*---(save-back)-------------------*/
      if (r_english != NULL)  strlcpy (r_english, s_prefix [i].p_english, LEN_LABEL);
      if (r_gregg   != NULL)  strlcpy (r_gregg  , s_prefix [i].p_gregg  , LEN_LABEL);
      if (r_point   != NULL)  *r_point = &(s_prefix [i]);
      return i;
      /*---(done)------------------------*/
   }
   /*---(not-found)----------------------*/
   return --rce;
}

char
PREFIX_driver           (cchar a_prefix [LEN_LABEL], char b_english [LEN_TITLE], char b_gregg [LEN_TITLE], void **r_point)
{
   /*---(locals)-------------------------*/
   char        rce         =  -10;
   char        rc          =    0;
   short       n           =    0;
   char        x_english   [LEN_TITLE] = "";
   char        x_prefix    [LEN_LABEL] = "";
   char        x_pretty    [LEN_LABEL] = "";
   char        x_gbase     [LEN_TITLE] = "";
   char        x_gregg     [LEN_LABEL] = "";
   char        x_trunc     =    0;
   int         l           =    0;
   /*---(header)-------------------------*/
   DEBUG_INPT  yLOG_enter   (__FUNCTION__);
   /*---(default)------------------------*/
   if (r_point != NULL)  *r_point = NULL;
   /*---(defense)------------------------*/
   DEBUG_INPT  yLOG_point   ("a_prefix"  , a_prefix);
   --rce;  if (a_prefix  == NULL) {
      DEBUG_INPT  yLOG_exitr   (__FUNCTION__, rce);
      return rce;
   }
   DEBUG_INPT  yLOG_info    ("a_prefix"  , a_prefix);
   DEBUG_INPT  yLOG_point   ("b_english" , b_english);
   --rce;  if (b_english == NULL || b_english [0] == '\0') {
      DEBUG_INPT  yLOG_exitr   (__FUNCTION__, rce);
      return rce;
   }
   DEBUG_INPT  yLOG_info    ("b_english" , b_english);
   DEBUG_INPT  yLOG_point   ("b_gregg"   , b_gregg);
   --rce;  if (b_gregg   == NULL || b_gregg   [0] == '\0') {
      DEBUG_INPT  yLOG_exitr   (__FUNCTION__, rce);
      return rce;
   }
   DEBUG_INPT  yLOG_info    ("b_gregg"   , b_gregg);
   DEBUG_INPT  yLOG_point   ("r_point"   , r_point);
   --rce;  if (r_point   == NULL) {
      DEBUG_INPT  yLOG_exitr   (__FUNCTION__, rce);
      return rce;
   }
   /*---(short-cut)----------------------*/
   if (strlen (a_prefix)  == 0 || strcmp (a_prefix, "·") == 0 || strcmp (a_prefix, "´") == 0) {
      DEBUG_INPT  yLOG_note    ("nothing to do");
      DEBUG_INPT  yLOG_exit    (__FUNCTION__);
      return 0;
   }
   /*---(prepare)------------------------*/
   strlcpy (x_english, b_english, LEN_TITLE);
   strlcpy (x_gbase  , b_gregg  , LEN_TITLE);
   /*---(update)-------------------------*/
   rc = PREFIX_english_change  (b_english, a_prefix, x_english, x_prefix, &x_trunc);
   DEBUG_INPT  yLOG_value   ("english"   , rc);
   --rce;  if (rc < 0) {
      DEBUG_INPT  yLOG_exitr   (__FUNCTION__, rce);
      return rce;
   }
   /*---(real prefix)--------------------*/
   n  = PREFIX__by_name         (x_prefix, x_pretty, x_gregg, r_point);
   DEBUG_INPT  yLOG_value   ("prefix"    , n);
   --rce;  if (n < 0) {
      DEBUG_INPT  yLOG_exitr   (__FUNCTION__, rce);
      return rce;
   }
   if (x_trunc == 1) {
      l = strlen (x_pretty);
      x_pretty [l - 1] = '\0';
   }
   /*---(save-back)----------------------*/
   sprintf (b_english, "%s%s", x_pretty, x_english);
   sprintf (b_gregg  , "%s%s", x_gregg , x_gbase);
   /*---(complete)-----------------------*/
   DEBUG_INPT  yLOG_exit    (__FUNCTION__);
   return 0;
}

char
PREFIX_english          (void *a_prefix, char r_english [LEN_LABEL])
{
   /*---(locals)-------------------------*/
   char        rce         =  -10;
   tPREFIX    *x_prefix    = NULL;
   /*---(default)------------------------*/
   if (r_english != NULL)  strcpy (r_english, "");
   /*---(defense)------------------------*/
   --rce;  if (a_prefix  == NULL)  return rce;
   /*---(save-back)----------------------*/
   x_prefix = (tPREFIX *) a_prefix;
   strlcpy (r_english, x_prefix->p_english, LEN_LABEL);
   /*---(complete)-----------------------*/
   return 0;
}



/*====================-----------------==-----------------====================*/
/*===----                      database handling                       ----===*/
/*====================-----------------==-----------------====================*/
static void o___DATABASE__________________o (void) {;}

short
PREFIX_encode           (void *a_prefix)
{
   tPREFIX    *x_prefix    = NULL;
   if (a_prefix == NULL)  return 0;
   x_prefix = (tPREFIX *) a_prefix;
   return  x_prefix->p_ref;
}

void*
PREFIX_decode           (short a_prefix)
{
   if (a_prefix == 0)     return NULL;
   return  &(s_prefix [a_prefix]);
}

char
PREFIX_write            (FILE *a_file)
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
   DEBUG_OUTP  yLOG_value   ("s_nprefix" , s_nprefix);
   --rce;  if (s_nprefix <= 0) {
      DEBUG_OUTP  yLOG_exitr   (__FUNCTION__, rce);
      return rce;
   }
   /*---(write all)----------------------*/
   for (i = 0; i < s_nprefix; ++i) {
      rc = fwrite (&(s_prefix [i]), sizeof (tPREFIX), 1, a_file);
      if (rc != 1)   break;
   }
   /*---(check)--------------------------*/
   DEBUG_OUTP  yLOG_value   ("i"         , i);
   --rce;  if (s_nprefix != i) {
      DEBUG_OUTP  yLOG_exitr   (__FUNCTION__, rce);
      return rce;
   }
   /*---(complete)-----------------------*/
   DEBUG_OUTP   yLOG_exit    (__FUNCTION__);
   return 0;
}

char
PREFIX_read             (FILE *a_file, short a_count)
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
   DEBUG_OUTP  yLOG_value   ("s_nprefix" , s_nprefix);
   --rce;  if (s_nprefix > 0) {
      DEBUG_OUTP  yLOG_exitr   (__FUNCTION__, rce);
      return rce;
   }
   /*---(write all)----------------------*/
   for (i = 0; i < a_count; ++i) {
      rc = fread  (&(s_prefix [i]), sizeof (tPREFIX), 1, a_file);
      if (rc != 1)   break;
      ++s_nprefix;
   }
   /*---(check)--------------------------*/
   DEBUG_OUTP  yLOG_value   ("a_count"   , a_count);
   --rce;  if (s_nprefix != a_count) {
      DEBUG_OUTP  yLOG_exitr   (__FUNCTION__, rce);
      return rce;
   }
   /*---(complete)-----------------------*/
   DEBUG_OUTP   yLOG_exit    (__FUNCTION__);
   return 0;
}







