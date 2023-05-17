/*============================---(source-start)---============================*/
#include    "gregg.h"

/*
 *  PURPOSE     store and apply word prefixes
 *
 */

/*> g_prefix [LEN_FULL] = {                                                           <* 
 *>    /+ 123456789   123456789- +/                                                   <* 
 *>    /+---t-------------------------------------+/                                  <* 
 *>    { "trans"    , "trans"    , "t·<·"         , "t·<·"         },                 <* 
 *>    /+---d-------------------------------------+/                                  <* 
 *>    { "de"       , "de"       , "d·<·"         , "d·<·"         },                 <* 
 *>    { "dis"      , "dis"      , "d·<·"         , "d·<·"         },                 <* 
 *>    { "dys"      , "dys"      , "d·<·"         , "d·<·"         },                 <* 
 *>    /+---n-------------------------------------+/                                  <* 
 *>    { "in"       , "in"       , "n·<·"         , "n·<·"         },                 <* 
 *>    { "un"       , "un"       , "n·<·"         , "n·<·"         },                 <* 
 *>    /+---m-------------------------------------+/                                  <* 
 *>    { "em"       , "em"       , "m·<·"         , "m·<·"         },                 <* 
 *>    { "im"       , "im"       , "m·<·"         , "m·<·"         },                 <* 
 *>    /+---r-------------------------------------+/                                  <* 
 *>    { "re"       , "re"       , "r·<·"         , "r·<·"         },                 <* 
 *>    /+---l-------------------------------------+/                                  <* 
 *>    /+---k-------------------------------------+/                                  <* 
 *>    { "co"       , "co"       , "k·<·"         , "k·<·"         },                 <* 
 *>    { "con"      , "con"      , "k·<·"         , "k·<·"         },                 <* 
 *>    { "com"      , "com"      , "k·<·"         , "k·<·"         },                 <* 
 *>    { "counter"  , "counter"  , "k·<·"         , "k·<·"         },                 <* 
 *>    /+---g-------------------------------------+/                                  <* 
 *>    /+---f-------------------------------------+/                                  <* 
 *>    { "for"      , "for"      , "f·<·"         , "f·<·"         },                 <* 
 *>    { "fore"     , "fore"     , "f·<·"         , "f·<·"         },                 <* 
 *>    { "ful"      , "ful"      , "f·<·"         , "f·<·"         },                 <* 
 *>    /+---v-------------------------------------+/                                  <* 
 *>    /+---p-------------------------------------+/                                  <* 
 *>    { "par"      , "par"      , "p·a·r·<·"     , "p·<·"         },                 <* 
 *>    { "para"     , "para"     , "p·a·r·a·<·"   , "p·<·"         },                 <* 
 *>    { "per"      , "per"      , "p·r·<·"       , "p·<·"         },                 <* 
 *>    { "peri"     , "peri"     , "p·e·r·e·<·"   , "p·<·"         },                 <* 
 *>    { "pre"      , "pre"      , "p·r·e·<·"     , "p·<·"         },                 <* 
 *>    { "pro"      , "pro"      , "p·r·o·<·"     , "p·<·"         },                 <* 
 *>    { "pur"      , "pur"      , "p·r·<·"       , "p·<·"         },                 <* 
 *>    /+---b-------------------------------------+/                                  <* 
 *>    /+---s-------------------------------------+/                                  <* 
 *>    { "sub"      , "sub"      , "s·<·"         , "s·<·"         },                 <* 
 *>    { "circum"   , "circum"   , "z·<·"         , "z·<·"         },                 <* 
 *>    /+---a-------------------------------------+/                                  <* 
 *>    { "a"        , "a"        , "a·<·"         , "a·<·"         },                 <* 
 *>    { "an"       , "an"       , "a·<·"         , "a·<·"         },                 <* 
 *>    { "anti"     , "anti"     , "a·<·"         , "a·<·"         },                 <* 
 *>    /+---e-------------------------------------+/                                  <* 
 *>    { "e"        , "e"        , "e·<·"         , "e·<·"         },                 <* 
 *>    { "ex"       , "ex"       , "e·xz·<·"      , "e·<·"         },                 <* 
 *>    /+---o-------------------------------------+/                                  <* 
 *>    { "ob"       , "ob"       , "o·<·"         , "o·<·"         },                 <* 
 *>    /+---u-------------------------------------+/                                  <* 
 *>    /+---mutli---------------------------------+/                                  <* 
 *>    { "in-de"    , "inde"     , "nd·<·"        , "nd·<·"        },                 <* 
 *>    { "re-de"    , "rede"     , "rd·<·"        , "rd·<·"        },                 <* 
 *>    { "un-de"    , "unde"     , "nd·<·"        , "nd·<·"        },                 <* 
 *>    { "---"      , "---"      , ""             , ""             },                 <* 
 *> };                                                                                <*/

tPREFIX  g_prefix [LEN_FULL];
short    g_nprefix  = 0;



/*============================--------------------============================*/
/*===----                        program level                         ----===*/
/*============================--------------------============================*/
static void o___PROGRAM___________________o (void) {;}

char
PREFIX_purge            (void)
{
   /*---(locals)-----------+-----------+-*/
   int         i           =    0;
   int         j           =    0;
   for (i = 0; i < LEN_FULL; ++i) {
      g_prefix [i].p_line = 0;
      strlcpy (g_prefix [i].p_english, "", LEN_LABEL);
      strlcpy (g_prefix [i].p_english, "", LEN_LABEL);
      strlcpy (g_prefix [i].p_gregg  , "", LEN_LABEL);
      for (j = 0; j < LEN_LABEL; ++j)   g_prefix [i].p_drawn [j] = 0;
      g_prefix [i].p_used = 0;
   }
   g_nprefix = 0;
   /*---(complete)-----------------------*/
   return 0;
}

char
PREFIX_init             (void)
{
   strlcpy (my.n_prefix, "/var/lib/gregg/prefix.txt", LEN_HUND);
   return PREFIX_purge ();
}

char PREFIX_wrap     (void)  { return PREFIX_purge (); }



/*============================--------------------============================*/
/*===----                          data load                           ----===*/
/*============================--------------------============================*/
static void o___LOAD______________________o (void) {;}

char
PREFIX_handler          (int n, char a_verb [LEN_LABEL], char a_exist, void *a_handler)
{
   /*---(locals)-----------+-----------+-*/
   char        rce         =  -10;
   int         rc          =    0;
   int         c           =    0;
   char        t           [LEN_LABEL] = "";
   /*---(header)-------------------------*/
   DEBUG_INPT  yLOG_enter   (__FUNCTION__);
   /*---(defense)------------------------*/
   DEBUG_INPT  yLOG_point   ("a_verb"    , a_verb);
   --rce;  if (a_verb == NULL || strlen (a_verb) <= 0) {
      DEBUG_INPT  yLOG_exit    (__FUNCTION__);
      yURG_err ('f', "group_handler called with null/empty verb");
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
   rc = yPARSE_scanf (a_verb, "TTT", g_prefix [g_nprefix].p_english, t, g_prefix [g_nprefix].p_gregg);
   DEBUG_INPT  yLOG_value   ("handler"   , rc);
   --rce;  if (rc < 0) {
      DEBUG_INPT  yLOG_exit    (__FUNCTION__);
      yURG_err ('f', "yparse could not process record");
      return rce;
   }
   /*---(finish)-------------------------*/
   g_prefix [g_nprefix].p_line = n;
   strlcpy (g_prefix [g_nprefix].p_name, a_verb, LEN_LABEL);
   DEBUG_INPT  yLOG_complex ("final"     , "%3d#, %-10.10s, %-10.10s, %s", g_nprefix, g_prefix [g_nprefix].p_name, g_prefix [g_nprefix].p_english, g_prefix [g_nprefix].p_gregg);
   ++g_nprefix;
   DEBUG_INPT  yLOG_value   ("g_nprefix" , g_nprefix);
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
   rc = yPARSE_autoread (a_fname, NULL, PREFIX_handler);
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
   yURG_msg ('>', "all read correctly, SUCCESS, accepted %d", g_nprefix);
   yURG_msg (' ', "");
   /*---(complete)-----------------------*/
   DEBUG_INPT  yLOG_exit    (__FUNCTION__);
   return 0;
}



/*============================--------------------============================*/
/*===----                        debug support                         ----===*/
/*============================--------------------============================*/
static void o___DEBUG_____________________o (void) {;}

char*
PREFIX_detail           (uchar n)
{
   char        r           [LEN_LABEL] = "·åæ";
   char        s           [LEN_LABEL] = "·åæ";
   char        t           [LEN_LABEL] = "·åæ";
   strcpy  (g_print, "n/a");
   if (n < g_nprefix) {
      sprintf  (r, "%1då%.9sæ", strlen (g_prefix [n].p_name   ), g_prefix [n].p_name   );
      sprintf  (s, "%1då%.9sæ", strlen (g_prefix [n].p_english), g_prefix [n].p_english);
      sprintf  (t, "%1då%.9sæ", strlen (g_prefix [n].p_gregg  ), g_prefix [n].p_gregg  );
      sprintf (g_print, "%-2d  %-2d  %-12.12s  %-12.12s  %s", n, g_prefix [n].p_line, r, s, t);
   }
   return g_print;
}

/*============================--------------------============================*/
/*===----                        data finding                          ----===*/
/*============================--------------------============================*/
static void o___FIND______________________o (void) {;}

char
PREFIX_by_name          (char a_prefix [LEN_LABEL], char r_english [LEN_LABEL], char r_gregg [LEN_LABEL])
{
   /*---(locals)-------------------------*/
   char        rce         =  -10;
   char        i           =    0;
   /*---(default)------------------------*/
   if (r_gregg   != NULL)  strcpy (r_gregg  , "");
   if (r_english != NULL)  strcpy (r_english, "");
   /*---(defense)------------------------*/
   --rce;  if (a_prefix  == NULL)  return rce;
   --rce;  if (r_english == NULL)  return rce;
   --rce;  if (r_gregg   == NULL)  return rce;
   /*---(search)-------------------------*/
   for (i = 0; i < LEN_HUND; ++i) {
      if (strcmp (g_prefix [i].p_name, "---")    == 0)       break;
      if (strcmp (g_prefix [i].p_name, a_prefix) != 0)       continue;
      strlcpy (r_english, g_prefix [i].p_english, LEN_LABEL);
      strlcpy (r_gregg  , g_prefix [i].p_gregg  , LEN_LABEL);
      return 0;
   }
   return --rce;
}

