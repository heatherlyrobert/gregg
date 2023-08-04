/*=================================(beg-code)=================================*/
#include "gregg.h"



/*============================--------------------============================*/
/*===----                       parts-of-speech                        ----===*/
/*============================--------------------============================*/
static void o___SPEECH_________________o (void) {;}

typedef struct cPARTS  tPARTS;
struct cPARTS {
   char        abbr;
   char        sub;  
   char        name        [LEN_LABEL];
   int         count;
   int         scount;
};
static tPARTS  s_parts  [LEN_HUND] = {
   /*---(unassigned)------------------*/
   { '¢', '¢', "bad primary"          , 0 , 0},
   { '·', '·', "tbd"                  , 0 , 0},
   { 'R', '·', "root-word"            , 0 , 0},
   { 'n', '·', "noun"                 , 0 , 0},
   { 'p', '·', "pronoun"              , 0 , 0},
   /*---(verbs)-----------------------*/
   { 'v', '·', "verb"                 , 0 , 0},
   { 'v', 'r',   "regular (base)"     , 0 , 0},
   { 'v', '/',   "irregular (base)"   , 0 , 0},
   { 'v', ' ',   "personally added"   , 0 , 0},
   /*---(adjectives)------------------*/
   { 'j', '·', "adjective"            , 0 , 0},
   { 'd', '·', "adverb"               , 0 , 0},
   { 'e', '·', "preposition"          , 0 , 0},
   { 'c', '·', "conjuction"           , 0 , 0},
   { 'c', 'o',    "coordinating"      , 0 , 0},
   { 'c', 'r',    "coorelating"       , 0 , 0},
   { 'c', 's',    "subordinating"     , 0 , 0},
   { 'i', '·', "interjection"         , 0 , 0},
   /*---(detiremener)-----------------*/
   { 't', '·', "detiremener"          , 0 , 0},
   { 't', 'a',   "articles"           , 0 , 0},
   { 't', 'q',   "quantifiers"        , 0 , 0},
   { 't', 's',   "possessives"        , 0 , 0},
   { 't', 'm',   "demonstratives"     , 0 , 0},
   /*---(end)-------------------------*/
   {  0 ,  0 , "---"                  , 0 , 0},
   /*---(done)------------------------*/
};

char
CATS__find_speech       (char a_abbr)
{
   /*---(locals)-------------------------*/
   char        rce         =  -10;
   char        i           =    0;
   for (i = 0; i < LEN_HUND; ++i) {
      if (s_parts [i].abbr == 0)       break;
      if (s_parts [i].sub  != '·')     continue;
      if (s_parts [i].abbr != a_abbr)  continue;
      ++(s_parts [i].count);
      return a_abbr;
   }
   ++(s_parts [0].count);
   return '¢';
}

char
CATS__find_sub          (char a_abbr, char a_sub)
{
   /*---(locals)-------------------------*/
   char        rce         =  -10;
   char        i           =    0;
   for (i = 0; i < LEN_HUND; ++i) {
      if (s_parts [i].abbr == 0)       break;
      if (s_parts [i].abbr != a_abbr)  continue;
      if (s_parts [i].sub  != a_sub)   continue;
      ++(s_parts [i].scount);
      return a_sub;
   }
   ++(s_parts [0].scount);
   return '¢';
}



/*============================--------------------============================*/
/*===----                         info sources                         ----===*/
/*============================--------------------============================*/
static void o___SOURCES________________o (void) {;}

typedef struct cSOURCE  tSOURCE;
struct cSOURCE {
   char        abbr;
   char        source      [LEN_TITLE];
   int         count;
};
static tSOURCE  s_source   [LEN_LABEL] = {
   /*---(unassigned)------------------*/
   { '¢', "bad source"               , 0 },
   { '·', "tbd"                      , 0 },
   /*---(assiged)---------------------*/
   { '1', "pamplet         1st 1888" , 0 },
   { '2', "revised         2nd 1893" , 0 },
   { '3', "revised         3rd 1898" , 0 },
   { '4', "revised         4th 1908" , 0 },
   { '5', "pre-anniversary 5th 1916" , 0 },
   { '6', "anniversary     6th 1929" , 0 },
   { '7', "simplified      7th 1949" , 0 },
   { '8', "diamond         8th 1963" , 0 },
   { '9', "series 90       9th 1978" , 0 },
   { '´', "centennial     10th 1988" , 0 },
   /*---(end)-------------------------*/
   {  0 , "---"                      , 0 },
   /*---(done)------------------------*/
};

char
CATS__find_source       (char a_source)
{
   /*---(locals)-------------------------*/
   char        i           =    0;
   /*---(search)-------------------------*/
   for (i = 0; i < LEN_LABEL; ++i) {
      if (s_source [i].abbr == 0)         break;
      if (s_source [i].abbr != a_source)  continue;
      ++(s_source [i].count);
      return a_source;
   }
   ++(s_source [0].count);
   return '¢';
}

typedef struct cTYPES    tTYPES;
struct cTYPES {
   char        abbr;
   char        source      [LEN_TITLE];
   int         count;
};
static tTYPES  s_types    [LEN_TERSE] = {
   /*---(unassigned)------------------*/
   { '·', "tbd"                  , 0 },
   /*---(unassigned)------------------*/
   { '°', "word-sign  (manual)"  , 0 },
   { 'Ï', "word-short (manual)"  , 0 },
   { '¯', "phrases    (manual)"  , 0 },
   { 'm', "manual"               , 0 },
   { 'p', "phrase book"          , 0 },
   { 'u', "most used words"      , 0 },
   { 'd', "dictionary"           , 0 },
   { '!', "custom"               , 0 },
   /*---(end)-------------------------*/
   {  0 , "---"                  , 0 },
   /*---(done)------------------------*/
};

char
CATS__find_type         (char a_type)
{
   /*---(locals)-----------+-----+-----+-*/
   char        i           =    0;
   /*---(search)-------------------------*/
   for (i = 0; i < LEN_TERSE; ++i) {
      if (s_types [i].abbr == 0)         break;
      if (s_types [i].abbr != a_type)    continue;
      ++(s_types [i].count);
      return a_type;
   }
   ++(s_types [0].count);
   return '¢';
}



/*============================--------------------============================*/
/*===----                         frequency                            ----===*/
/*============================--------------------============================*/
static void o___FREQ___________________o (void) {;}

typedef struct cGRPS  tGRPS;
struct cGRPS {
   char        grp;  
   char        name        [LEN_TITLE];
   int         count;
};
static tGRPS   g_grps [LEN_LABEL] = {
   /*---(unassigned)------------------*/
   { '·', "tbd"                      , 0 },
   { 'a', "top 50"                   , 0 },
   { 'b', "top 100"                  , 0 },
   { 'c', "top 200"                  , 0 },
   { 'd', "top 500"                  , 0 },
   { 'e', "top 1000"                 , 0 },
   { 'f', "top 2000"                 , 0 },
   { 'g', "top 5000"                 , 0 },
   { 'h', "top 1000"                 , 0 },
   { 'z', "> 10000"                  , 0 },
   /*---(end)-------------------------*/
   {  0 , "---"                      , 0 },
   /*---(done)------------------------*/
};

char
CATS__find_grp          (char a_grp)
{
   /*---(locals)-------------------------*/
   char        rce         =  -10;
   char        i           =    0;
   for (i = 0; i < LEN_LABEL; ++i) {
      if (g_grps  [i].grp  == 0)       break;
      if (g_grps  [i].grp  != a_grp)   continue;
      ++(g_grps  [i].count);
      return a_grp;
   }
   ++(g_grps  [0].count);
   return '¢';
}



/*============================--------------------============================*/
/*===----                        program level                         ----===*/
/*============================--------------------============================*/
static void o___PROGRAM________________o (void) {;}

char
CATS_init          (void)
{
   /*---(locals)-------------------------*/
   short       i           =    0;
   char        t           [LEN_SHORT] = "";
   /*---(header)-------------------------*/
   DEBUG_CONF   yLOG_enter   (__FUNCTION__);
   /*---(prepare)------------------------*/
   /*---(complete)-----------------------*/
   DEBUG_CONF   yLOG_exit    (__FUNCTION__);
   return 0;
}



/*============================--------------------============================*/
/*===----                        main drivers                          ----===*/
/*============================--------------------============================*/
static void o___DRIVER_________________o (void) {;}

char
CATS__prepare           (cchar a_cats [LEN_LABEL], char *r_len, char *r_part, char *r_sub, char *r_src, char *r_cat, short *r_page, char *r_grp, short *r_freq)
{
   /*---(locals)-----------+-----------+-*/
   char        rce         =  -10;
   char        rc          =    0;
   char        l           =    0;
   /*---(header)-------------------------*/
   DEBUG_CONF   yLOG_enter   (__FUNCTION__);
   /*---(default)-----------------------*/
   if (r_len   != NULL)  *r_len  = 0;
   if (r_part  != NULL)  *r_part = '·';
   if (r_sub   != NULL)  *r_sub  = '·';
   if (r_src   != NULL)  *r_src  = '·';
   if (r_cat   != NULL)  *r_cat  = '·';
   if (r_page  != NULL)  *r_page = 0;
   if (r_grp   != NULL)  *r_grp  = '·';
   if (r_freq  != NULL)  *r_freq = 0;
   /*---(defense)------------------------*/
   DEBUG_CONF   yLOG_point   ("a_cats"    , a_cats);
   --rce;  if (a_cats == NULL) {
      DEBUG_CONF   yLOG_exitr   (__FUNCTION__, rce);
      return rce;
   }
   DEBUG_CONF   yLOG_info    ("a_cats"    , a_cats);
   /*---(length)-------------------------*/
   l = strlen (a_cats);
   DEBUG_CONF   yLOG_value   ("l"         , l);
   if (r_len   != NULL)  *r_len = l;
   /*---(complete)-----------------------*/
   DEBUG_CONF   yLOG_exit    (__FUNCTION__);
   return 0;
}

char
CATS__driver            (cchar *a_func, cchar a_type, cchar a_cats [LEN_LABEL], char *r_part, char *r_sub, char *r_src, char *r_cat, short *r_page, char *r_grp, short *r_freq)
{
   /*---(locals)-----------+-----------+-*/
   char        rce         =  -10;
   char        rc          =    0;
   char        l           =    0;
   char        t           [LEN_TERSE] = "";
   /*---(header)-------------------------*/
   DEBUG_CONF   yLOG_enter   (a_func);
   /*---(prepare)------------------------*/
   rc = CATS__prepare  (a_cats, &l, r_part, r_sub, r_src, r_cat, r_page, r_grp, r_freq);
   DEBUG_CONF   yLOG_value   ("prepare"   , rc);
   --rce;  if (rc < 0) {
      DEBUG_CONF   yLOG_exitr   (a_func, rce);
      return rce;
   }
   DEBUG_CONF   yLOG_complex ("a_cats"    , "%2då%-18.18sæ", l, a_cats);
   /*---(check type)---------------------*/
   DEBUG_CONF   yLOG_char    ("a_type"    , a_type);
   --rce;  if (a_type == 0 || strchr ("bsp", a_type) == NULL) {
      DEBUG_CONF   yLOG_exitr   (a_func, rce);
      return rce;
   }
   /*---(part of speech)-----------------*/
   switch (a_type) {
   case 'b'  : case 's'  :
      if (l <  1)  break;
      if (r_part != NULL)  *r_part = CATS__find_speech (a_cats [0]);
      if (l <  3)  break;
      if (r_sub  != NULL)  *r_sub  = CATS__find_speech (a_cats [2]);
      break;
   }
   DEBUG_CONF   yLOG_complex ("part"      , "%c part, %c sub", *r_part, *r_sub);
   /*---(source)-------------------------*/
   switch (a_type) {
   case 'b'  : case 's'  :
      if (l <  6) break;
      if (r_src  != NULL)  *r_src  = CATS__find_source (a_cats [5]);
      if (l <  8) break;
      if (r_cat  != NULL)  *r_cat  = CATS__find_type   (a_cats [7]);
      if (l < 10) break;
      sprintf (t, "%-3.3s", a_cats + 9);
      if (r_page != NULL)  *r_page = atoi (t);
      break;
   case 'p'  :
      if (l <  1) break;
      if (r_src  != NULL)  *r_src  = CATS__find_source (a_cats [0]);
      if (l <  3) break;
      if (r_cat  != NULL)  *r_cat  = CATS__find_type   (a_cats [2]);
      if (l <  5) break;
      sprintf (t, "%-3.3s", a_cats + 4);
      if (r_page != NULL)  *r_page = atoi (t);
      break;
   }
   DEBUG_CONF   yLOG_complex ("source"    , "%c src, %c cat, %d page", *r_src, *r_cat, *r_page);
   /*---(source)-------------------------*/
   switch (a_type) {
   case 'b'  :
      if (l < 14) break;
      if (r_grp  != NULL)  *r_grp  = CATS__find_grp    (a_cats [13]);
      if (l < 16) break;
      if (r_freq != NULL)  *r_freq = atoi (a_cats + 15);
      break;
   }
   DEBUG_CONF   yLOG_complex ("group"     , "%c grp, %d freq", *r_grp, *r_freq);
   /*---(complete)-----------------------*/
   DEBUG_CONF   yLOG_exit    (a_func);
   return 0;
}

char
CATS_prefix             (cchar a_cats [LEN_LABEL], char *r_src, char *r_cat, short *r_page)
{
   return CATS__driver (__FUNCTION__, 'p', a_cats, NULL  , NULL , r_src, r_cat, r_page, NULL, NULL);
}

char
CATS_suffix             (cchar a_cats [LEN_LABEL], char *r_part, char *r_sub, char *r_src, char *r_cat, short *r_page)
{
   return CATS__driver (__FUNCTION__, 's', a_cats, r_part, r_sub, r_src, r_cat, r_page, NULL, NULL);
}

char
CATS_base               (cchar a_cats [LEN_LABEL], char *r_part, char *r_sub, char *r_src, char *r_cat, short *r_page, char *r_grp, short *r_freq)
{
   return CATS__driver (__FUNCTION__, 'b', a_cats, r_part, r_sub, r_src, r_cat, r_page, r_grp, r_freq);
}

