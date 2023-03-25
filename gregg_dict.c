/*============================---(source-start)---============================*/
#include "gregg.h"

/*
 *
 * metis § dc2·· § allow for blank/empty varitions in source files                        § N2O468 §  · §
 * metis § sn··· § handle newer categories field format                                   § N2O46u §  · §
 *
 *
 */


static char    s_name   [LEN_PATH]  = "/var/lib/gregg/gregg.dict";

char   s_fields  [MAX_FIELD][LEN_TITLE];
char   s_nfield  = 0;


tPARTS  g_parts  [LEN_HUND] = {
   /*---(unassigned)------------------*/
   { '¢', '¢', "bad primary"          , 0 , 0},
   { '·', '·', "tbd"                  , 0 , 0},
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
char    PARTS_OF_SPEECH [LEN_LABEL] = "";

tSOURCE  g_source   [LEN_LABEL] = {
   /*---(unassigned)------------------*/
   { '¢', "bad source"               , 0 },
   { '·', "tbd"                      , 0 },
   /*---(assiged)---------------------*/
   { '1', "pamplet         1st 1888" , 0 },
   { '2', "pamplet         2nd 1888" , 0 },
   { '3', "booklet         3rd 1888" , 0 },
   { '4', "booklet         4th 1888" , 0 },
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

tGRPS   g_grps [LEN_HUND] = {
   /*---(unassigned)------------------*/
   { '¢', '·', "bad primary"          , 0 },
   { '·', '·', "tbd"                  , 0 },
   { 'n', '·', "noun"                 , 0 },
   { 'p', '·', "pronoun"              , 0 },
   /*---(verbs)-----------------------*/
   { 'v', '·', "verb"                 , 0 },
   { 'v', 'r',   "regular (600 base)"     , 0 },
   { 'v', '/',   "irregular (100 base)"   , 0 },
   { 'v', ' ',   "personally added"   , 0 },
   /*---(adjectives)------------------*/
   { 'j', '·', "adjective"            , 0 },
   { 'd', '·', "adverb"               , 0 },
   { 'e', '·', "preposition"          , 0 },
   { 'c', '·', "conjuction"           , 0 },
   { 'i', '·', "interjection"         , 0 },
   /*---(detiremener)-----------------*/
   { 't', '·', "detiremener"          , 0 },
   { 't', 'a',   "articles"           , 0 },
   { 't', 'q',   "quantifiers"        , 0 },
   { 't', 's',   "possessives"        , 0 },
   { 't', 'm',   "demonstratives"     , 0 },
   /*---(end)-------------------------*/
   {  0 ,  0 , "---"                  , 0 },
   /*---(done)------------------------*/
};

tTYPES  g_types    [LEN_TERSE] = {
   /*---(unassigned)------------------*/
   { '·', "tbd"                  , 0 },
   /*---(unassigned)------------------*/
   { '°', "word-sign"            , 0 },
   { 'Ï', "word-shortening"      , 0 },
   { '¯', "phrases (shortened)"  , 0 },
   { 'm', "manual"               , 0 },
   { 'd', "dictionary"           , 0 },
   { 'o', "other"                , 0 },
   /*---(end)-------------------------*/
   {  0 , "---"                  , 0 },
   /*---(done)------------------------*/
};


tVARY  g_varies [LEN_DESC] = {
   /*-----   123456789-123456789 , 123456789-123456789-123456789-123456789-123456789    123456789-123456789-123456789-123456789-123456789-123456789-123456789- */
   { "·"  , ""        , "base"               , ""                                                , "observe, care, drink, know, like, use, forget"                          },
   /*---(tenses)------------------------*/
   { "´"  , ">·´"     , "present continuous" , "-ing"                                            , "observing, caring, drinking, knowing, liking, using, forgetting"        },
   { "s"  , ">·s"     , "present simple"     , "-s, -es, -ies"                                   , "observes, cares, drinks, knows, likes, uses, forgets"                   },
   { "d"  , ">·d"     , "past simple"        , "-d, -ed, -ied"                                   , "observed, cared, drank, knew, liked, used, forgot"                      },
   { "u"  , ">·u"     , "future simple"      , "will >"                                          , "will observe"                                                           },
   /*---(other uses)--------------------*/
   { "r"  , ">·r"     , "actor/doer"         , "-r, -ar, -er, -eer, -or, -ant, -ent"             , "observer, carer, drinker, knower, user"                                 },
   { "rs" , ">·r·z"   , "actor/doer (plural)", "-rs, -ers, -ors, -ants, -ents"                   , "observers, carers, drinkers, knowers, users"                            },
   { "b"  , ">·b"     , "capable of"         , "-ble, -able, -ible"                              , "observeable, drinkable, knowable, likeable, useable, forgetable"        },
   { "v"  , ">·v"     , "adjective"          , "-ive, -ative, -itive, -ant, -ent"                , "observant"                                                              },
   { "sh" , ">·sh"    , "act/process"        , "-tion, -sion"                                    , "observation"                                                            },
   { "e"  , ">·e"     , "place to do"        , "-ry, -ory, -ery, -ary, -ium"                     , "observatory"                                                            },
   { "n"  , ">·n"     , "state or quality"   , "-ence, -ance, -ancy, -ency, -cy, -acy"           , "observance"                                                             },
   { "l"  , ">·l"     , "without"            , "-less"                                           , "careless, ageless, useless"                                             },
   { "--" , ""        , "property of"        , "-al, -il, -ile, -ial"                            , "observational"                                                          },
   { "t"  , ""        , "office/function"    , "-ate, -ite, -ship"                               , "certificate, demonstrate"                                               },
   { "f"  , ""        , "having quality"     , "-ful, -fil"                                      , "careful, beautiful, useful"                                             },
   /*---(end)---------------------------*/
   { ""   , ""        , "---"                , "---"                                             , "---"                                                                    },
   /*---(done)--------------------------*/
};

/*> uchar      *x_valids    = " ´ e a t d dd th tn tm ts df n m mm u k g o r l nk ng sh ch j z p b s f v pt bd ";   <*/

char
DICT__find_speech       (char a_abbr)
{
   /*---(locals)-------------------------*/
   char        rce         =  -10;
   char        i           =    0;
   for (i = 0; i < LEN_HUND; ++i) {
      if (g_parts [i].abbr == 0)       break;
      if (g_parts [i].sub  != '·')     continue;
      if (g_parts [i].abbr != a_abbr)  continue;
      ++(g_parts [i].count);
      return a_abbr;
   }
   ++(g_parts [0].count);
   return '¢';
}

char
DICT__find_sub          (char a_abbr, char a_sub)
{
   /*---(locals)-------------------------*/
   char        rce         =  -10;
   char        i           =    0;
   for (i = 0; i < LEN_HUND; ++i) {
      if (g_parts [i].abbr == 0)       break;
      if (g_parts [i].abbr != a_abbr)  continue;
      if (g_parts [i].sub  != a_sub)   continue;
      ++(g_parts [i].scount);
      return a_sub;
   }
   ++(g_parts [0].scount);
   return '¢';
}

char
DICT__find_grp          (char a_abbr, char a_grp)
{
   /*---(locals)-------------------------*/
   char        rce         =  -10;
   char        i           =    0;
   for (i = 0; i < LEN_HUND; ++i) {
      if (g_grps  [i].abbr == 0)       break;
      if (g_grps  [i].abbr != a_abbr)  continue;
      if (g_grps  [i].grp  != a_grp)   continue;
      ++(g_grps  [i].count);
      return a_grp;
   }
   ++(g_grps  [0].count);
   return '¢';
}

char
DICT__find_source       (char a_source)
{
   /*---(locals)-------------------------*/
   char        i           =    0;
   /*---(search)-------------------------*/
   for (i = 0; i < LEN_LABEL; ++i) {
      if (g_source [i].abbr == 0)         break;
      if (g_source [i].abbr != a_source)  continue;
      ++(g_source [i].count);
      return a_source;
   }
   ++(g_source [0].count);
   return '¢';
}

char
DICT__find_type         (char a_type)
{
   /*---(locals)-------------------------*/
   char        i           =    0;
   /*---(search)-------------------------*/
   for (i = 0; i < LEN_TERSE; ++i) {
      if (g_types [i].abbr == 0)         break;
      if (g_types [i].abbr != a_type)    continue;
      ++(g_types [i].count);
      return a_type;
   }
   ++(g_types [0].count);
   return '¢';
}

/*============================--------------------============================*/
/*===----                          dictionary                          ----===*/
/*============================--------------------============================*/
static void      o___DICTIONARY______________o (void) {;}

char
DICT_init          (void)
{
   /*---(locals)-------------------------*/
   char        i           =    0;
   char        t           [LEN_SHORT] = "";
   /*---(header)-------------------------*/
   DEBUG_CONF   yLOG_enter   (__FUNCTION__);
   /*---(prepare)------------------------*/
   strcpy (PARTS_OF_SPEECH, "");
   for (i = 0; i < LEN_HUND; ++i) {
      if (g_parts [i].abbr == 0)   break;
      if (g_parts [i].sub  != '·') continue;
      sprintf (t, "%c", g_parts [i].abbr);
      strlcat (PARTS_OF_SPEECH, t, LEN_LABEL);
   }
   DEBUG_CONF   yLOG_info    ("PARTS_OF..", PARTS_OF_SPEECH);
   /*---(complete)-----------------------*/
   DEBUG_CONF   yLOG_exit    (__FUNCTION__);
   return 0;
}

char
DICT__open              (char a_name [LEN_PATH], FILE **r_file)
{
   /*---(locals)-----------+-----+-----+-*/
   char        rce         =  -10;
   FILE       *f           = NULL;
   /*---(header)-------------------------*/
   DEBUG_CONF   yLOG_enter   (__FUNCTION__);
   /*---(defense)------------------------*/
   DEBUG_CONF   yLOG_point   ("a_name"    , a_name);
   --rce;  if (a_name == NULL) {
      DEBUG_CONF   yLOG_exitr   (__FUNCTION__, rce);
      return rce;
   }
   DEBUG_CONF   yLOG_info    ("a_name"    , a_name);
   --rce;  if (a_name [0] == '\0') {
      DEBUG_CONF   yLOG_exitr   (__FUNCTION__, rce);
      return rce;
   }
   DEBUG_CONF   yLOG_point   ("r_file"    , r_file);
   --rce;  if (r_file == NULL) {
      DEBUG_CONF   yLOG_exitr   (__FUNCTION__, rce);
      return rce;
   }
   DEBUG_CONF   yLOG_point   ("*r_file"   , *r_file);
   --rce;  if (*r_file != NULL) {
      DEBUG_CONF   yLOG_exitr   (__FUNCTION__, rce);
      return rce;
   }
   /*---(open)---------------------------*/
   f = fopen (a_name, "rt");
   DEBUG_CONF   yLOG_point   ("f"         , f);
   --rce;  if (f == NULL) {
      DEBUG_CONF   yLOG_exitr   (__FUNCTION__, rce);
      return rce;
   }
   /*---(save-back)----------------------*/
   if (r_file != NULL)  *r_file = f;
   strlcpy (s_name, a_name, LEN_PATH);
   /*---(complete)-----------------------*/
   DEBUG_CONF   yLOG_exit    (__FUNCTION__);
   return 0;
}

char
DICT__close             (FILE **b_file)
{
   /*---(locals)-----------+-----+-----+-*/
   char        rce         =  -10;
   char        rc          =    0;
   FILE       *f           = NULL;
   /*---(header)-------------------------*/
   DEBUG_CONF   yLOG_enter   (__FUNCTION__);
   /*---(defense)------------------------*/
   DEBUG_CONF   yLOG_point   ("b_file"    , b_file);
   --rce;  if (b_file == NULL) {
      DEBUG_CONF   yLOG_exitr   (__FUNCTION__, rce);
      return rce;
   }
   DEBUG_CONF   yLOG_point   ("*b_file"   , *b_file);
   --rce;  if (*b_file == NULL) {
      DEBUG_CONF   yLOG_exitr   (__FUNCTION__, rce);
      return rce;
   }
   f = *b_file;
   /*---(open)---------------------------*/
   rc = fclose (f);
   DEBUG_CONF   yLOG_value   ("close"     , rc);
   --rce;  if (rc < 0) {
      DEBUG_CONF   yLOG_exitr   (__FUNCTION__, rce);
      return rce;
   }
   /*---(save-back)----------------------*/
   if (b_file != NULL)  *b_file = NULL;
   strlcpy (s_name, "", LEN_PATH);
   /*---(complete)-----------------------*/
   DEBUG_CONF   yLOG_exit    (__FUNCTION__);
   return 0;
}

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
      strltrim (s_fields [s_nfield], ySTR_BOTH, LEN_TITLE);
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
DICT__primary           (short a_line, cchar a_english [LEN_TITLE], cchar a_gregg [LEN_TITLE], tWORD **r_word)
{
   /*---(locals)-----------+-----+-----+-*/
   char        rce         =  -10;
   char        rc          =    0;
   char        x_gregg     [LEN_TITLE] = "";
   tWORD      *x_new       = NULL;
   /*---(header)-------------------------*/
   DEBUG_CONF   yLOG_enter   (__FUNCTION__);
   /*---(defense)------------------------*/
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
   /*---(check gregg)--------------------*/
   if (a_gregg [0] == '>')   strlcpy (x_gregg, a_gregg + 2, LEN_TITLE);
   else                      strlcpy (x_gregg, a_gregg    , LEN_TITLE);
   strldchg (x_gregg, '.', '·' , LEN_TITLE);
   DEBUG_CONF   yLOG_info    ("x_gregg"   , x_gregg);
   /*---(add word)-----------------------*/
   rc = WORDS__new (a_english, x_gregg, &x_new);
   DEBUG_CONF   yLOG_value   ("new"       , rc);
   --rce;  if (rc < 0) {
      DEBUG_CONF   yLOG_exitr   (__FUNCTION__, rce);
      return rce;
   }
   /*---(save-back)----------------------*/
   x_new->line = a_line;
   if (r_word != NULL)  *r_word = x_new;
   /*---(complete)-----------------------*/
   DEBUG_CONF   yLOG_exit    (__FUNCTION__);
   return 0;
}

char
DICT__category_five     (tWORD *a_new, char l, cchar *a_cats)
{
   /*---(header)-------------------------*/
   DEBUG_CONF   yLOG_enter   (__FUNCTION__);
   /*---(part-of-speech)-----------------*/
   if (l >= 1)  a_new->part = DICT__find_speech (a_cats [0]);
   else         a_new->part = DICT__find_speech ('¢');
   /*---(sub part-of-speech)-------------*/
   if (l >= 3)  a_new->sub  = DICT__find_sub    (a_new->part, a_cats [2]);
   else         a_new->sub  = DICT__find_sub    (a_new->part, '¢');
   /*---(source group)-------------------*/
   if (l >= 5)  a_new->grp  = DICT__find_grp    (a_new->part, a_cats [4]);
   else         a_new->grp  = DICT__find_grp    (a_new->part, '¢');
   /*---(source)-------------------------*/
   if (l >= 7)  a_new->src  = DICT__find_source (a_cats [6]);
   else         a_new->src  = DICT__find_source ('¢');
   /*---(category)-----------------------*/
   if (l >= 9)  a_new->cat  = DICT__find_type   (a_cats [8]);
   else         a_new->cat  = DICT__find_type   ('¢');
   /*---(complete)-----------------------*/
   DEBUG_CONF   yLOG_exit    (__FUNCTION__);
   return 0;
}

char
DICT__category_six      (tWORD *a_new, char l, cchar *a_cats)
{
   /*---(locals)-----------+-----+-----+-*/
   short       x_page      =    0;
   /*---(header)-------------------------*/
   DEBUG_CONF   yLOG_enter   (__FUNCTION__);
   /*---(part-of-speech)-----------------*/
   if (l >= 1)  a_new->part = DICT__find_speech (a_cats [0]);
   else         a_new->part = DICT__find_speech ('¢');
   /*---(sub part-of-speech)-------------*/
   if (l >= 3)  a_new->sub  = DICT__find_sub    (a_new->part, a_cats [2]);
   else         a_new->sub  = DICT__find_sub    (a_new->part, '¢');
   /*---(source group)-------------------*/
   if (l >= 5)  a_new->grp  = DICT__find_grp    (a_new->part, a_cats [4]);
   else         a_new->grp  = DICT__find_grp    (a_new->part, '¢');
   /*---(source)-------------------------*/
   if (l >= 8)  a_new->src  = DICT__find_source (a_cats [7]);
   else         a_new->src  = DICT__find_source ('¢');
   /*---(category)-----------------------*/
   if (l >= 10) a_new->cat  = DICT__find_type   (a_cats [9]);
   else         a_new->cat  = DICT__find_type   ('¢');
   /*---(page)---------------------------*/
   if (l >= 12) a_new->page = atoi (a_cats + 11);
   /*---(complete)-----------------------*/
   DEBUG_CONF   yLOG_exit    (__FUNCTION__);
   return 0;
}

char
DICT__category          (tWORD *a_new, cchar *a_cats)
{
   /*---(locals)-----------+-----+-----+-*/
   char        rce         =  -10;
   char        rc          =    0;
   char        l           =    0;
   /*---(defenses)-----------------------*/
   DEBUG_CONF   yLOG_point   ("a_new"     , a_new);
   --rce;  if (a_new == NULL) {
      DEBUG_CONF   yLOG_exitr   (__FUNCTION__, rce);
      return rce;
   }
   DEBUG_CONF   yLOG_point   ("a_cats"    , a_cats);
   --rce;  if (a_cats == NULL) {
      DEBUG_CONF   yLOG_exitr   (__FUNCTION__, rce);
      return rce;
   }
   l = strlen (a_cats);
   /*---(part-of-speech)-----------------*/
   if (l >= 1)  a_new->part = DICT__find_speech (a_cats [0]);
   else         a_new->part = DICT__find_speech ('¢');
   /*---(sub part-of-speech)-------------*/
   if (l >= 3)  a_new->sub  = DICT__find_sub    (a_new->part, a_cats [2]);
   else         a_new->sub  = DICT__find_sub    (a_new->part, '¢');
   /*---(source group)-------------------*/
   if (l >= 5)  a_new->grp  = DICT__find_grp    (a_new->part, a_cats [4]);
   else         a_new->grp  = DICT__find_grp    (a_new->part, '¢');
   /*---(source)-------------------------*/
   if (l >= 7)  a_new->src  = DICT__find_source (a_cats [6]);
   else         a_new->src  = DICT__find_source ('¢');
   /*---(category)-----------------------*/
   if (l >= 9)  a_new->cat  = DICT__find_type   (a_cats [8]);
   else         a_new->cat  = DICT__find_type   ('¢');
   /*---(complete)-----------------------*/
   DEBUG_CONF   yLOG_exit    (__FUNCTION__);
   return 0;
}

char
DICT__variation_quick   (tWORD *a_base, tWORD *a_last, char *a_english, char *a_vary, tWORD **r_new)
{
   /*---(locals)-----------+-----+-----+-*/
   char        rce         =  -10;
   char        rc          =    0;
   char        x_gregg     [LEN_TITLE] = "";
   char        x_suffix    [LEN_TERSE] = "";
   tWORD      *x_new       = NULL;
   /*---(build suffix)-------------------*/
   if (strcmp (a_vary, "rs") == 0)  strcpy (x_suffix, "r·z");
   else                             strcpy (x_suffix, a_vary);
   /*---(build gregg)--------------------*/
   sprintf (x_gregg, "%s·>·%s", a_base->gregg, x_suffix);
   DEBUG_CONF   yLOG_info    ("x_gregg"   , x_gregg);
   /*---(add word)-----------------------*/
   rc = WORDS__new (a_english, x_gregg, &x_new);
   DEBUG_CONF   yLOG_value   ("new"       , rc);
   --rce;  if (rc < 0) {
      DEBUG_CONF   yLOG_exitr   (__FUNCTION__, rce);
      return rce;
   }
   /*---(carry-over)---------------------*/
   x_new->line = a_base->line;
   strlcpy (x_new->vary, a_vary, LEN_SHORT);
   x_new->part = a_base->part;
   x_new->sub  = a_base->sub;
   x_new->grp  = a_base->grp;
   x_new->src  = a_base->src;
   x_new->cat  = a_base->cat;
   /*---(connect to chain)---------------*/
   x_new->base = a_base;
   if (a_last != NULL)  a_last->next = x_new;
   /*---(save-back)----------------------*/
   if (r_new  != NULL)  *r_new = x_new;
   /*---(complete)-----------------------*/
   DEBUG_CONF   yLOG_exit    (__FUNCTION__);
   return 0;
}

char
DICT__variation         (tWORD *a_base, tWORD *a_last, cchar *a_vary, tWORD **r_new)
{
   /*---(locals)-----------+-----+-----+-*/
   char        rce         =  -10;
   char        rc          =    0;
   char       *p           = NULL;
   char        l           =    0;
   char        x_left      =    0;
   char        x_type      [LEN_TITLE]  = "";
   char        x_english   [LEN_TITLE]  = "";
   char        x_gregg     [LEN_TITLE] = "";
   tWORD     *x_vary      = NULL;
   tWORD     *x_save      = NULL;
   uchar       x_check     [LEN_LABEL] = "";
   uchar      *x_valids    = " ´ s e a t d dd th tn tm ts df n m mm u k g o r l nk ng sh ch j z p b s f v pt bd ";
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
   --rce;  if (l < 1 || l > 2) {
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
   sprintf (x_english, "%s"   , p + 2);
   /*---(get type)-----------------------*/
   sprintf (x_check, " %s ", x_type);
   --rce;  if (strstr (x_valids, x_check) == NULL) {
      DEBUG_CONF   yLOG_exitr   (__FUNCTION__, rce);
      return rce;
   }
   /*---(make variation)-----------------*/
   rc = DICT__variation_quick (a_base, a_last, x_english, x_type, &x_vary);
   x_save = x_vary;
   /*---(build second)-------------------*/
   if (strcmp (x_type, "r") == 0) {
      strlcat (x_english, "s", LEN_TITLE);
      rc = DICT__variation_quick (a_base, x_save, x_english, "rs", &x_vary);
   }
   /*---(save-back)----------------------*/
   if (r_new  != NULL)  *r_new = x_vary;
   /*---(complete)-----------------------*/
   DEBUG_CONF   yLOG_exit    (__FUNCTION__);
   return 0;
}

char
DICT__parse             (short a_line, uchar *a_recd)
{
   /*---(locals)-----------+-----+-----+-*/
   char        rce         =  -10;
   char        rc          =    0;
   int         i           =    0;
   tWORD      *x_new       = NULL;
   tWORD      *x_last      = NULL;
   tWORD      *x_vary      = NULL;
   /*---(header)-------------------------*/
   DEBUG_CONF   yLOG_enter   (__FUNCTION__);
   /*---(split)--------------------------*/
   rc = DICT__split (a_recd);
   DEBUG_CONF   yLOG_value   ("split"     , rc);
   --rce;  if (rc < 0) {
      DEBUG_CONF   yLOG_exitr   (__FUNCTION__, rce);
      return rce;
   }
   /*---(primary)------------------------*/
   rc = DICT__primary (a_line, s_fields [0], s_fields [1], &x_new);
   DEBUG_CONF   yLOG_value   ("primary"   , rc);
   --rce;  if (rc < 0) {
      DEBUG_CONF   yLOG_exitr   (__FUNCTION__, rce);
      return rce;
   }
   /*---(categories)---------------------*/
   if (s_nfield > 2) {
      rc = DICT__category (x_new, s_fields [2]);
      DEBUG_CONF   yLOG_value   ("primary"   , rc);
      --rce;  if (rc < 0) {
         DEBUG_CONF   yLOG_exitr   (__FUNCTION__, rce);
         return rce;
      }
   }
   /*---(variations)---------------------*/
   x_last = x_new;
   for (i = 3; i < s_nfield; ++i) {
      rc = DICT__variation (x_new, x_last, s_fields [i], &x_vary);
      DEBUG_CONF   yLOG_value   ("primary"   , rc);
      --rce;  if (rc < 0) {
         DEBUG_CONF   yLOG_exitr   (__FUNCTION__, rce);
         return rce;
      }
      x_last = x_vary;
   }
   /*---(complete)-----------------------*/
   DEBUG_CONF   yLOG_exit    (__FUNCTION__);
   return 0;
}

char
DICT__read              (FILE *a_file, short *r_line, char r_recd [LEN_RECD])
{
   /*---(locals)-----------+-----+-----+-*/
   char        rce         =  -10;
   char        rc          =    0;
   int         x_len       =    0;
   /*---(begin)----------------------------*/
   DEBUG_CONF   yLOG_enter   (__FUNCTION__);
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
   strcpy (r_recd, "(eof)");
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
      /*---(clean-up)--------------------*/
      x_len = strlen (r_recd);
      if (x_len < 20)          continue;
      if (r_recd [x_len - 1] == '\n')  r_recd [--x_len] = '\0';
      DEBUG_CONF   yLOG_info    ("r_recd"    , r_recd);
      break;
   }
   /*---(complete)-----------------------*/
   DEBUG_CONF   yLOG_exit    (__FUNCTION__);
   return 0;
}

char
WORDS_import            (char a_name [LEN_PATH])
{
   /*---(locals)-----------+-----+-----+-*/
   char        rce         =  -10;
   char        rc          =    0;
   FILE       *f           = NULL;
   char        x_recd      [LEN_RECD]  = "";
   short       x_line      =    0;
   /*---(begin)----------s-----------------*/
   DEBUG_CONF   yLOG_enter   (__FUNCTION__);
   /*---(open dictionary)------------------*/
   rc = DICT__open (a_name, &f);
   --rce;  if (rc < 0) {
      DEBUG_CONF   yLOG_exitr   (__FUNCTION__, rce);
      return rce;
   }
   /*---(process)--------------------------*/
   while (1) {
      /*---(next)----------------*/
      rc = DICT__read  (f, &x_line, x_recd);
      if (rc < 0)  break;
      /*---(parse)---------------*/
      rc = DICT__parse (x_line, x_recd);
      /*---(done)----------------*/
   }
   /*---(close dictionary)-----------------*/
   rc = DICT__close (&f);
   --rce;  if (rc < 0) {
      DEBUG_CONF   yLOG_exitr   (__FUNCTION__, rce);
      return rce;
   }
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
   char        t           [LEN_HUND]  = "";
   char        n           [LEN_SHORT] = "";
   char        e           [LEN_TITLE] = "";
   char        g           [LEN_TITLE] = "";
   tWORD       *x_curr      = NULL;
   /*---(search)-------------------------*/
   WORDS_eng_by_cursor (YDLST_HEAD, &x_curr);
   while (x_curr != NULL) {
      if (strcmp (x_curr->vary, "<") == 0) {
         WORDS_detail (x_curr, t);
         strlpadn (++c, n, '.', '<', 5);
         printf ("%5.5s··%s\n", n, t);
         /*> sprintf (e, "%2då%sæ", x_curr->e_len, x_curr->english);                  <* 
          *> sprintf (g, "%2då%sæ", x_curr->g_len, x_curr->gregg);                    <* 
          *> printf ("%-4d %-4d %-24.24s %s\n", ++c, x_curr->line, e, g);             <*/
         while (x_curr->next != NULL) {
            x_curr = x_curr->next;
            WORDS_detail (x_curr, t);
            printf ("       %s\n", t);
            /*> sprintf (e, "%2då%sæ", x_curr->e_len, x_curr->english);               <* 
             *> sprintf (g, "%2då%sæ", x_curr->g_len, x_curr->gregg);                 <* 
             *> printf ("          %-24.24s %s\n", e, g);                             <*/
         }
      }
      ++a;
      WORDS_eng_by_cursor (YDLST_NEXT, &x_curr);
   }
   /*---(complete)-----------------------*/
   return 0;
}
