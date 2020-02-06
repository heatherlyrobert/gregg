/*============================---(source-start)---============================*/

#include "gregg.h"


#define     OUT_XGAP    10



tWORDS *e_hword  = NULL;
tWORDS *e_tword  = NULL;
tWORDS *e_cword  = NULL;
int     e_nword  = 0;
int     e_iword  = -1;

tWORDS *g_hword  = NULL;
tWORDS *g_tword  = NULL;
tWORDS *g_cword  = NULL;
int     g_nword  = 0;
int     g_iword  = -1;



static char   *s_name = "/var/lib/gregg/gregg.dict";
static FILE   *s_dict = NULL;
static char    s_recd [LEN_RECD] = "";
static int     s_line = 0;

static uchar    s_english  [LEN_HUND] = "";
static uchar    s_gregg    [LEN_HUND] = "";

static uchar    s_ver   = '·';
static uchar    s_book  = '·';
static int     s_page  =   0;
static uchar    s_type  = '·';

static uchar    s_diff  = '·';
static uchar    s_simp  = '·';
static uchar    s_3rd   = '·';
static uchar    s_4th   = '·';
static uchar    s_top   = '·';  /*  A = 100, b =500, c = 2000, - = others  */



/*====================------------------------------------====================*/
/*===----                      memory allocation                       ----===*/
/*====================------------------------------------====================*/
static void  o___MEMORY__________o () { return; }

char         /*-> create a single new empty cell -----[ leaf   [fe.KB4.224.80]*/ /*-[12.0000.123.A]-*/ /*-[--.---.---.--]-*/
WORDS__new         (tWORDS **a_word, char *a_english, char *a_gregg)
{
   /*---(locals)-----------+-----------+-*/
   char        rce         =  -10;
   int         x_tries     =    0;
   tWORDS     *x_new       = NULL;
   llong       x_ekey      =   -1;
   llong       x_gkey      =   -1;
   uchar       x_index     [LEN_LABEL] = "";
   /*---(begin)--------------------------*/
   DEBUG_CONF   yLOG_enter   (__FUNCTION__);
   /*---(default)------------------------*/
   if (a_word != NULL)  *a_word == NULL;
   /*---(defense)------------------------*/
   DEBUG_CONF   yLOG_point   ("a_english" , a_english);
   --rce;  if (a_english == NULL) {
      DEBUG_CONF   yLOG_exitr   (__FUNCTION__, rce);
      return rce;
   }
   x_ekey     = BTREE_english2key (a_english);
   DEBUG_CONF   yLOG_llong   ("x_ekey"    , x_ekey);
   --rce;  if (x_ekey <= 0) {
      DEBUG_CONF   yLOG_exitr   (__FUNCTION__, rce);
      return rce;
   }
   DEBUG_CONF   yLOG_point   ("a_gregg"   , a_gregg);
   --rce;  if (a_gregg == NULL) {
      DEBUG_CONF   yLOG_exitr   (__FUNCTION__, rce);
      return rce;
   }
   x_gkey     = BTREE_gregg2key   (a_gregg, x_index);
   DEBUG_CONF   yLOG_llong   ("x_gkey"    , x_gkey);
   --rce;  if (x_gkey <= 0) {
      DEBUG_CONF   yLOG_exitr   (__FUNCTION__, rce);
      return rce;
   }
   /*---(create cell)--------------------*/
   while (x_new == NULL) {
      ++x_tries;
      x_new = (tWORDS *) malloc (sizeof (tWORDS));
      if (x_tries > 10)   break;
   }
   DEBUG_CONF   yLOG_value   ("x_tries"   , x_tries);
   DEBUG_CONF   yLOG_point   ("x_new"     , x_new);
   --rce;  if (x_new == NULL) {
      DEBUG_CONF   yLOG_note    ("cell could not be created");
      DEBUG_CONF   yLOG_exitr   (__FUNCTION__, rce);
      return rce;
   }
   ++e_nword;
   ++g_nword;
   DEBUG_CONF   yLOG_value   ("e_nword"   , e_nword);
   /*---(null links)---------------------*/
   x_new->e_next    = NULL;
   x_new->e_prev    = NULL;
   x_new->e_left    = NULL;
   x_new->e_right   = NULL;
   x_new->g_next    = NULL;
   x_new->g_prev    = NULL;
   x_new->g_left    = NULL;
   x_new->g_right   = NULL;
   /*---(link to masters)----------------*/
   DEBUG_CONF   yLOG_note    ("link to english/gregg lists");
   if (e_tword == NULL) {
      /*---(english)-----------*/
      e_hword         = x_new;
      e_tword         = x_new;
      /*---(gregg)-------------*/
      g_hword         = x_new;
      g_tword         = x_new;
      /*---(done)--------------*/
   } else {
      /*---(english)-----------*/
      x_new->e_prev   = e_tword;
      x_new->e_next   = NULL;
      e_tword->e_next = x_new;
      e_tword         = x_new;
      /*---(gregg)-------------*/
      x_new->g_prev   = g_tword;
      x_new->g_next   = NULL;
      g_tword->g_next = x_new;
      g_tword         = x_new;
      /*---(done)--------------*/
   }
   /*---(header)-------------------------*/
   DEBUG_CONF   yLOG_note    ("add all data");
   x_new->english   = strdup (a_english);
   x_new->e_len     = strlen (x_new->english);
   x_new->e_key     = x_ekey;
   x_new->gregg     = strdup (a_gregg);
   x_new->g_len     = strlen (x_new->gregg);
   x_new->g_key     = x_gkey;
   /*---(drawn)-------------------------*/
   WORDS_drawn_fix (x_index, x_new->drawn);
   /*---(source)------------------------*/
   /*> x_new->ver       =  '·';                                                       <* 
    *> x_new->book      =  '·';                                                       <* 
    *> x_new->page      =  '0';                                                       <* 
    *> x_new->type      =  '·';                                                       <*/
   /*---(complexity)--------------------*/
   /*> x_new->diff      =  '·';                                                       <* 
    *> x_new->simp      =  '·';                                                       <* 
    *> x_new->x3rd      =  '·';                                                       <* 
    *> x_new->x4th      =  '·';                                                       <* 
    *> x_new->top       =  '·';                                                       <*/
   /*---(save word)----------------------*/
   if (a_word != NULL)  *a_word = x_new;
   /*---(complete)-----------------------*/
   DEBUG_CONF   yLOG_exit    (__FUNCTION__);
   return 0;
}

char         /*-> remove a cell completely -----------[ ------ [fe.943.224.81]*/ /*-[11.0000.133.7]-*/ /*-[--.---.---.--]-*/
WORDS__free        (tWORDS *a_word)
{
   /*---(locals)-----------+-----------+-*/
   char        rce         =  -10;
   char        rc          =    0;
   /*---(beginning)----------------------*/
   DEBUG_CONF   yLOG_enter   (__FUNCTION__);
   /*---(defense: valid cell)------------*/
   DEBUG_CONF   yLOG_point   ("a_word"    , a_word);
   --rce;  if (a_word == NULL) {
      DEBUG_CONF   yLOG_exitr   (__FUNCTION__, rce);
      return rce;
   }
   /*---(wipe data)----------------------*/
   free (a_word->english);
   a_word->english = NULL;
   free (a_word->gregg);
   a_word->gregg   = NULL;
   /*---(out of english list)------------*/
   if (a_word->e_next != NULL)   a_word->e_next->e_prev = a_word->e_prev;
   else                          e_tword                = a_word->e_prev;
   if (a_word->e_prev != NULL)   a_word->e_prev->e_next = a_word->e_next;
   else                          e_hword                = a_word->e_next;
   /*---(out of gregg list)--------------*/
   if (a_word->g_next != NULL)   a_word->g_next->g_prev = a_word->g_prev;
   else                          g_tword                = a_word->g_prev;
   if (a_word->g_prev != NULL)   a_word->g_prev->g_next = a_word->g_next;
   else                          g_hword                = a_word->g_next;
   /*---(free main)----------------------*/
   DEBUG_CONF   yLOG_note    ("freeing and nulling");
   --e_nword;
   --g_nword;
   DEBUG_CONF   yLOG_value   ("e_nword"   , e_nword);
   free (a_word);
   /*---(complete)-----------------------*/
   DEBUG_CONF   yLOG_exit    (__FUNCTION__);
   return 0;
}



/*============================--------------------============================*/
/*===----                   private data structures                    ----===*/
/*============================--------------------============================*/
static tWORDS s_words [MAX_WORDS];
static int    s_nword;

/*---(gregg index)------------------------------*/
tWORDS *out_index [MAX_LETTERS][MAX_LETTERS][MAX_LETTERS];

/*---(english index)----------------------------*/
tWORDS *eng_index [27][27][27];

PRIV int       a         = 0;           /* 1st letter index                    */
PRIV int       b         = 0;           /* 2nd letter index                    */
PRIV int       c         = 0;           /* 3rd letter index                    */


int    letters[30];




#define    BASX      -90
#define    BASY       90
#define    LINE      -60

int    outx  =     0;   /* outline starting point                  */
int    outy  =     0;
int    posx  =     0;   /* current letter point                    */
int    posy  =     0;


PRIV  char  words_heads (void);
PRIV  char  words_listg  (tWORDS *);
PRIV  char  words_liste  (tWORDS *);



/*============================--------------------============================*/
/*===----                          dictionary                          ----===*/
/*============================--------------------============================*/
static void      o___DICTIONARY______________o (void) {;}

static char     /*----: initialize the dictionary ------------------------------*/
DICT__init         (void)
{
   /*---(locals)-------------------------*/
   /*---(complete)-----------------------*/
   return 0;
}

char
WORDS_dict_open    (void)
{
   /*---(locals)-----------+-----+-----+-*/
   char        rce         =  -10;
   /*---(header)-------------------------*/
   DEBUG_CONF   yLOG_enter   (__FUNCTION__);
   /*---(defense)------------------------*/
   DEBUG_CONF   yLOG_point   ("s_dict"    , s_dict);
   --rce;  if (s_dict != NULL) {
      DEBUG_CONF   yLOG_exitr   (__FUNCTION__, rce);
      return rce;
   }
   /*---(open)---------------------------*/
   s_dict = fopen (s_name, "r");
   DEBUG_CONF   yLOG_point   ("s_dict"    , s_dict);
   --rce;  if (s_dict == NULL) {
      DEBUG_CONF   yLOG_exitr   (__FUNCTION__, rce);
      return rce;
   }
   /*---(initialize)---------------------*/
   s_line   =  0;
   /*---(complete)-----------------------*/
   DEBUG_CONF   yLOG_exit    (__FUNCTION__);
   return 0;
}

char
WORDS_dict_close   (void)
{
   /*---(locals)-----------+-----+-----+-*/
   char        rce         =  -10;
   char        rc          =    0;
   /*---(header)-------------------------*/
   DEBUG_CONF   yLOG_enter   (__FUNCTION__);
   /*---(defense)------------------------*/
   DEBUG_CONF   yLOG_point   ("s_dict"    , s_dict);
   --rce;  if (s_dict == NULL) {
      DEBUG_CONF   yLOG_exitr   (__FUNCTION__, rce);
      return rce;
   }
   /*---(open)---------------------------*/
   rc = fclose (s_dict);
   DEBUG_CONF   yLOG_value   ("close"     , rc);
   --rce;  if (rc < 0) {
      DEBUG_CONF   yLOG_exitr   (__FUNCTION__, rce);
      return rce;
   }
   /*---(ground)-------------------------*/
   s_dict = NULL;
   DEBUG_CONF   yLOG_point   ("s_dict"    , s_dict);
   /*---(complete)-----------------------*/
   DEBUG_CONF   yLOG_exit    (__FUNCTION__);
   return 0;
}

/*> char                                                                                              <* 
 *> WORDS_dict_source       (tWORDS *a_new, uchar *a_source)                                          <* 
 *> {                                                                                                 <* 
 *>    /+---(locals)-----------+-----+-----+-+/                                                       <* 
 *>    char        rce         =  -10;                                                                <* 
 *>    uchar       t           [LEN_LABEL] = "";                                                      <* 
 *>    uchar       s           [LEN_LABEL] = "";                                                      <* 
 *>    int         x_len       =    0;                                                                <* 
 *>    uchar      *x_valid     = "abcdefghijklmnopqrstuvwxyzABCDEFGHIJKLMNOPQRSTUVWXYZ0123456789·";   <* 
 *>    /+---(header)-------------------------+/                                                       <* 
 *>    DEBUG_CONF   yLOG_enter   (__FUNCTION__);                                                      <* 
 *>    /+---(prepare)------------------------+/                                                       <* 
 *>    s_ver  = s_book = s_type = '·';                                                                <* 
 *>    s_page = 0;                                                                                    <* 
 *>    /+---(defense)------------------------+/                                                       <* 
 *>    DEBUG_CONF   yLOG_point   ("a_source"  , a_source);                                            <* 
 *>    --rce;  if (a_source == NULL) {                                                                <* 
 *>       DEBUG_CONF   yLOG_exitr   (__FUNCTION__, rce);                                              <* 
 *>       return rce;                                                                                 <* 
 *>    }                                                                                              <* 
 *>    /+---(working copy)-------------------+/                                                       <* 
 *>    DEBUG_CONF   yLOG_info    ("a_source"  , a_source);                                            <* 
 *>    strlcpy  (t, a_source , LEN_LABEL);                                                            <* 
 *>    strltrim (t, ySTR_BOTH, LEN_LABEL);                                                            <* 
 *>    x_len = strlen (t);                                                                            <* 
 *>    DEBUG_CONF   yLOG_point   ("x_len"     , x_len);                                               <* 
 *>    --rce;  if (x_len != 9) {                                                                      <* 
 *>       DEBUG_CONF   yLOG_exitr   (__FUNCTION__, rce);                                              <* 
 *>       return rce;                                                                                 <* 
 *>    }                                                                                              <* 
 *>    /+---(version)------------------------+/                                                       <* 
 *>    s_ver  = t [0];                                                                                <* 
 *>    DEBUG_CONF   yLOG_char    ("s_ver"     , s_ver);                                               <* 
 *>    --rce;  if (s_ver  == 0 || strchr ("ospad·", s_ver ) == NULL) {                                <* 
 *>       DEBUG_CONF   yLOG_exitr   (__FUNCTION__, rce);                                              <* 
 *>       return rce;                                                                                 <* 
 *>    }                                                                                              <* 
 *>    if (a_new != NULL)  a_new->ver = s_ver;                                                        <* 
 *>    /+---(book)---------------------------+/                                                       <* 
 *>    s_book = t [2];                                                                                <* 
 *>    DEBUG_CONF   yLOG_char    ("s_book"    , s_book);                                              <* 
 *>    --rce;  if (s_book == 0 || strchr (x_valid, s_book) == NULL) {                                 <* 
 *>       DEBUG_CONF   yLOG_exitr   (__FUNCTION__, rce);                                              <* 
 *>       return rce;                                                                                 <* 
 *>    }                                                                                              <* 
 *>    if (a_new != NULL)  a_new->book = s_book;                                                      <* 
 *>    /+---(page)---------------------------+/                                                       <* 
 *>    strlcpy (s, t + 4, 4);                                                                         <* 
 *>    s_page = atoi (s);                                                                             <* 
 *>    DEBUG_CONF   yLOG_value   ("s_page"    , s_page);                                              <* 
 *>    --rce;  if (s_page < 0) {                                                                      <* 
 *>       DEBUG_CONF   yLOG_exitr   (__FUNCTION__, rce);                                              <* 
 *>       return rce;                                                                                 <* 
 *>    }                                                                                              <* 
 *>    if (a_new != NULL)  a_new->page = s_page;                                                      <* 
 *>    /+---(type)---------------------------+/                                                       <* 
 *>    s_type = t [8];                                                                                <* 
 *>    DEBUG_CONF   yLOG_char    ("s_type"    , s_type);                                              <* 
 *>    --rce;  if (s_type == 0 || strchr ("bwp·", s_type) == NULL) {                                  <* 
 *>       DEBUG_CONF   yLOG_exitr   (__FUNCTION__, rce);                                              <* 
 *>       return rce;                                                                                 <* 
 *>    }                                                                                              <* 
 *>    if (a_new != NULL)  a_new->type = s_type;                                                      <* 
 *>    /+---(complete)-----------------------+/                                                       <* 
 *>    DEBUG_CONF   yLOG_exit    (__FUNCTION__);                                                      <* 
 *>    return 0;                                                                                      <* 
 *> }                                                                                                 <*/

/*> char                                                                              <* 
 *> WORDS_dict_notes   (tWORDS *a_new, uchar *a_notes)                                <* 
 *> {                                                                                 <* 
 *>    /+---(locals)-----------+-----+-----+-+/                                       <* 
 *>    char        rce         =  -10;                                                <* 
 *>    uchar       t           [LEN_LABEL] = "";                                      <* 
 *>    uchar       s           [LEN_LABEL] = "";                                      <* 
 *>    int         x_len       =    0;                                                <* 
 *>    /+---(header)-------------------------+/                                       <* 
 *>    DEBUG_CONF   yLOG_enter   (__FUNCTION__);                                      <* 
 *>    /+---(prepare)------------------------+/                                       <* 
 *>    s_diff = s_simp = s_3rd  = s_4th  = s_top  = '·';                              <* 
 *>    /+---(defense)------------------------+/                                       <* 
 *>    DEBUG_CONF   yLOG_point   ("a_notes"   , a_notes);                             <* 
 *>    --rce;  if (a_notes == NULL) {                                                 <* 
 *>       DEBUG_CONF   yLOG_exitr   (__FUNCTION__, rce);                              <* 
 *>       return rce;                                                                 <* 
 *>    }                                                                              <* 
 *>    /+---(working copy)-------------------+/                                       <* 
 *>    DEBUG_CONF   yLOG_info    ("a_notes"   , a_notes);                             <* 
 *>    strlcpy  (t, a_notes , LEN_LABEL);                                             <* 
 *>    strltrim (t, ySTR_BOTH, LEN_LABEL);                                            <* 
 *>    x_len = strlen (t);                                                            <* 
 *>    DEBUG_CONF   yLOG_point   ("x_len"     , x_len);                               <* 
 *>    --rce;  if (x_len != 9) {                                                      <* 
 *>       DEBUG_CONF   yLOG_exitr   (__FUNCTION__, rce);                              <* 
 *>       return rce;                                                                 <* 
 *>    }                                                                              <* 
 *>    /+---(difficulty)---------------------+/                                       <* 
 *>    s_diff = t [0];                                                                <* 
 *>    DEBUG_CONF   yLOG_char    ("s_diff"    , s_diff);                              <* 
 *>    --rce;  if (s_diff  == 0 || strchr ("smhv·", s_diff ) == NULL) {               <* 
 *>       DEBUG_CONF   yLOG_exitr   (__FUNCTION__, rce);                              <* 
 *>       return rce;                                                                 <* 
 *>    }                                                                              <* 
 *>    if (a_new != NULL)  a_new->diff = s_diff;                                      <* 
 *>    /+---(simplified)---------------------+/                                       <* 
 *>    s_simp = t [2];                                                                <* 
 *>    DEBUG_CONF   yLOG_char    ("s_simp"    , s_simp);                              <* 
 *>    --rce;  if (s_simp  == 0 || strchr ("*+·-/" , s_simp ) == NULL) {              <* 
 *>       DEBUG_CONF   yLOG_exitr   (__FUNCTION__, rce);                              <* 
 *>       return rce;                                                                 <* 
 *>    }                                                                              <* 
 *>    if (a_new != NULL)  a_new->simp = s_simp;                                      <* 
 *>    /+---(simplified)---------------------+/                                       <* 
 *>    s_3rd  = t [4];                                                                <* 
 *>    if (a_new != NULL)  a_new->x3rd  = s_3rd;                                      <* 
 *>    DEBUG_CONF   yLOG_char    ("s_3rd"     , s_3rd);                               <* 
 *>    s_4th  = t [6];                                                                <* 
 *>    if (a_new != NULL)  a_new->x4th  = s_4th;                                      <* 
 *>    DEBUG_CONF   yLOG_char    ("s_4th"     , s_4th);                               <* 
 *>    s_top  = t [8];                                                                <* 
 *>    if (a_new != NULL)  a_new->top  = s_top;                                       <* 
 *>    DEBUG_CONF   yLOG_char    ("s_top"     , s_top);                               <* 
 *>    /+---(complete)-----------------------+/                                       <* 
 *>    DEBUG_CONF   yLOG_exit    (__FUNCTION__);                                      <* 
 *>    return 0;                                                                      <* 
 *> }                                                                                 <*/

char
WORDS_dict_vary   (tWORDS *a_new, uchar *a_type, uchar *a_vary)
{
   /*---(locals)-----------+-----+-----+-*/
   char        rce         =  -10;
   char        rc          =    0;
   uchar       t           [LEN_HUND]  = "";
   uchar       x_gregg     [LEN_HUND]  = "";
   tWORDS     *x_other     = NULL;
   uchar       x_check     [LEN_LABEL] = "";
   uchar      *x_valids    = " ´ e a t d dd th tn tm ts df n m mm u k g o r l nk ng sh ch j z p b s f v pt bd ";
   /*---(defenses)-----------------------*/
   DEBUG_CONF   yLOG_point   ("a_new"     , a_new);
   --rce;  if (a_new == NULL) {
      DEBUG_CONF   yLOG_exitr   (__FUNCTION__, rce);
      return rce;
   }
   DEBUG_CONF   yLOG_point   ("a_type"    , a_type);
   --rce;  if (a_type == NULL) {
      DEBUG_CONF   yLOG_exitr   (__FUNCTION__, rce);
      return rce;
   }
   DEBUG_CONF   yLOG_info    ("a_type"    , a_type);
   strlcpy  (t, a_type, LEN_LABEL);
   strltrim (t, ySTR_BOTH, LEN_LABEL);
   sprintf (x_check, " %s ", t);
   --rce;  if (strstr (x_valids, x_check) == NULL) {
      DEBUG_CONF   yLOG_exitr   (__FUNCTION__, rce);
      return rce;
   }
   DEBUG_CONF   yLOG_point   ("a_vary"    , a_vary);
   --rce;  if (a_vary == NULL) {
      DEBUG_CONF   yLOG_exitr   (__FUNCTION__, rce);
      return rce;
   }
   DEBUG_CONF   yLOG_info    ("a_vary"    , a_vary);
   /*---(build gregg)--------------------*/
   sprintf (x_gregg, "%s·>·%s", a_new->gregg, t);
   /*---(add word)-----------------------*/
   strlcpy  (t, a_vary, LEN_LABEL);
   strltrim (t, ySTR_BOTH, LEN_LABEL);
   rc = WORDS__new (&x_other, t, x_gregg);
   DEBUG_CONF   yLOG_value   ("new"       , rc);
   --rce;  if (rc < 0) {
      DEBUG_CONF   yLOG_exitr   (__FUNCTION__, rce);
      return rce;
   }
   /*---(complete)-----------------------*/
   DEBUG_CONF   yLOG_exit    (__FUNCTION__);
   return 0;
}

char
WORDS_dict_parse   (uchar *a_recd)
{
   /*---(locals)-----------+-----+-----+-*/
   char        rce         =  -10;
   char        rc          =    0;
   uchar       x_recd      [LEN_RECD]  = "";
   uchar      *x_english   = NULL;
   uchar      *x_gregg     = NULL;
   /*> uchar      *x_source    = NULL;                                                <*/
   /*> uchar      *x_notes     = NULL;                                                <*/
   uchar      *x_type      = NULL;
   uchar      *x_vary      = NULL;
   char       *q           = ")";
   char       *r           = NULL;
   tWORDS     *x_new       = NULL;
   /*---(header)-------------------------*/
   DEBUG_CONF   yLOG_enter   (__FUNCTION__);
   /*---(prepare)------------------------*/
   strlcpy  (s_english, "", LEN_HUND);
   strlcpy  (s_gregg  , "", LEN_HUND);
   /*> WORDS_dict_source  (NULL, NULL);                                               <*/
   /*> WORDS_dict_notes   (NULL, NULL);                                               <*/
   /*---(defense)------------------------*/
   DEBUG_CONF   yLOG_point   ("a_recd"    , a_recd);
   --rce;  if (a_recd == NULL) {
      DEBUG_CONF   yLOG_exitr   (__FUNCTION__, rce);
      return rce;
   }
   DEBUG_CONF   yLOG_info    ("a_recd"    , a_recd);
   strlcpy (x_recd, a_recd, LEN_RECD);
   /*---(english)------------------------*/
   x_english = strtok_r (x_recd, q, &r);
   DEBUG_CONF   yLOG_point   ("x_english" , x_english);
   --rce;  if (x_english == NULL) {
      DEBUG_CONF   yLOG_exitr   (__FUNCTION__, rce);
      return rce;
   }
   strlcpy (s_english, x_english, LEN_HUND);
   strltrim (s_english, ySTR_BOTH, LEN_HUND);
   DEBUG_CONF   yLOG_info    ("s_english" , s_english);
   /*---(english)------------------------*/
   x_gregg   = strtok_r (NULL  , q, &r);
   DEBUG_CONF   yLOG_point   ("x_gregg"   , x_gregg);
   --rce;  if (x_gregg   == NULL) {
      DEBUG_CONF   yLOG_exitr   (__FUNCTION__, rce);
      return rce;
   }
   strlcpy  (s_gregg  , x_gregg  , LEN_HUND);
   strltrim (s_gregg  , ySTR_BOTH, LEN_HUND);
   strldchg (s_gregg  , '.', '·' , LEN_HUND);
   if (s_gregg [0] == '>') {
      strlcpy (x_gregg, s_gregg    , LEN_HUND);
      strlcpy (s_gregg, x_gregg + 2, LEN_HUND);
   }
   DEBUG_CONF   yLOG_info    ("s_gregg"   , s_gregg);
   /*---(add word)-----------------------*/
   rc = WORDS__new (&x_new, s_english, s_gregg);
   DEBUG_CONF   yLOG_value   ("new"       , rc);
   --rce;  if (rc < 0) {
      DEBUG_CONF   yLOG_exitr   (__FUNCTION__, rce);
      return rce;
   }
   /*---(source)-------------------------*/
   /*> x_source  = strtok_r (NULL  , q, &r);                                          <* 
    *> --rce;  if (x_source  == NULL) {                                               <* 
    *>    DEBUG_CONF   yLOG_exit    (__FUNCTION__);                                   <* 
    *>    return 0;                                                                   <* 
    *> }                                                                              <* 
    *> WORDS_dict_source (x_new, x_source);                                           <*/
   /*---(notes)--------------------------*/
   /*> x_notes   = strtok_r (NULL  , q, &r);                                          <* 
    *> --rce;  if (x_notes   == NULL) {                                               <* 
    *>    DEBUG_CONF   yLOG_exit    (__FUNCTION__);                                   <* 
    *>    return 0;                                                                   <* 
    *> }                                                                              <* 
    *> WORDS_dict_notes  (x_new, x_notes);                                            <*/
   /*---(variations)---------------------*/
   x_type  = strtok_r (NULL  , q, &r);
   DEBUG_CONF   yLOG_point   ("x_type"    , x_type);
   while (x_type != NULL) {
      x_vary  = strtok_r (NULL  , q, &r);
      DEBUG_CONF   yLOG_point   ("x_vary"    , x_vary);
      if (x_vary == NULL) {
         DEBUG_CONF   yLOG_exitr   (__FUNCTION__, rce);
         return rce;
      }
      WORDS_dict_vary    (x_new, x_type, x_vary);
      x_type  = strtok_r (NULL  , q, &r);
      DEBUG_CONF   yLOG_point   ("x_type"    , x_type);
   }
   /*---(complete)-----------------------*/
   DEBUG_CONF   yLOG_exit    (__FUNCTION__);
   return 0;
}

char
WORDS_dict_read    (void)
{
   /*---(locals)-----------+-----+-----+-*/
   char        rce         =  -10;
   char        rc          =    0;
   int         x_len       =    0;
   /*---(begin)----------------------------*/
   DEBUG_CONF   yLOG_enter   (__FUNCTION__);
   /*---(defense)------------------------*/
   DEBUG_CONF   yLOG_point   ("s_dict"    , s_dict);
   --rce;  if (s_dict == NULL) {
      DEBUG_CONF   yLOG_exitr   (__FUNCTION__, rce);
      return rce;
   }
   /*---(find a record)------------------*/
   while (1) {
      /*---(read)------------------------*/
      if (feof (s_dict)) {
         DEBUG_CONF   yLOG_exitr   (__FUNCTION__, rce);
         return rce;
      }
      fgets (s_recd, LEN_RECD, s_dict);
      ++s_line;
      /*---(filter)----------------------*/
      if (s_recd [0] == '\0')  continue;
      if (s_recd [0] == '#')   continue;
      if (s_recd [0] == ' ')   continue;
      /*---(clean-up)--------------------*/
      x_len = strlen (s_recd);
      if (x_len < 20)          continue;
      if (s_recd [x_len - 1] == '\n')  s_recd [--x_len] = '\0';
      DEBUG_CONF   yLOG_info    ("s_recd"    , s_recd);
      break;
   }
   /*---(complete)-----------------------*/
   DEBUG_CONF   yLOG_exit    (__FUNCTION__);
   return 0;
}

char
WORDS_import       (void)            /* read the translation dictionary               */
{
   /*---(locals)-----------+-----+-----+-*/
   char        rce         =  -10;
   char        rc          =    0;
   char      x_recd [MAX_LINE] = "";         // current record
   int       n = 0;                    // current record number in file
   char      x_eng   [MAX_LEN];        /* english word                        */
   char      x_gregg [MAX_LEN];        /* gregg translation                   */
   char      x_ver;                    /* source of gregg translation         */
   char      x_src;                    /* source of gregg translation         */
   int       x_page;                   /* page in source of translation       */
   char      x_type;                   /* type, -=normal, b=brief, p=phrase   */
   int       prev;
   int       curr;
   int       dups      = 0;
   int i;
   char   *p = NULL;             // pointer to substring
   char   *q = "";              // delimiters
   char   *r = NULL;             // pointer to substring
   /*---(begin)----------s-----------------*/
   DEBUG_CONF   yLOG_enter   (__FUNCTION__);
   DICT__init   ();
   /*---(open dictionary)------------------*/
   rc = WORDS_dict_open ();
   --rce;  if (rc < 0) {
      DEBUG_CONF   yLOG_exitr   (__FUNCTION__, rce);
      return rce;
   }
   /*---(process)--------------------------*/
   while (1) {
      /*---(next)----------------*/
      rc = WORDS_dict_read ();
      if (rc < 0)  break;
      /*---(parse)---------------*/
      rc = WORDS_dict_parse (s_recd);
      /*---(done)----------------*/
   }
   /*---(close dictionary)-----------------*/
   rc = WORDS_dict_close ();
   --rce;  if (rc < 0) {
      DEBUG_CONF   yLOG_exitr   (__FUNCTION__, rce);
      return rce;
   }
   /*---(build btree)----------------------*/
   BTREE_dgnome ('e');
   BTREE_build  ('e');
   BTREE_dgnome ('g');
   BTREE_build  ('g');
   /*---(complete)-------------------------*/
   DEBUG_CONF   yLOG_exit    (__FUNCTION__);
   return 0;
}

int
WORDS_find           (char *a_word)
{
   /*---(locals)-----------+-----+-----+-*/
   char        rce         =  -10;
   int         i           =    0;
   /*---(defense)------------------------*/
   --rce;  if (a_word == NULL)  return rce;
   /*---(search)-------------------------*/
   for (i = 0; i < MAX_WORDS; ++i) {
      printf ("reviewing %d, %s, %s\n", i, s_words [i].english, s_words [i].gregg);
      if (strncmp (s_words[i].english, "eof",  MAX_LEN) == 0)  return -1;
      if (strncmp (s_words[i].english, a_word, MAX_LEN) != 0)  continue;
      printf ("FOUND     %d, %s, %s\n", i, s_words [i].english, s_words [i].gregg);
      return  i;
   }
   /*---(complete)-----------------------*/
   return 0;
}

char
WORDS_dict_list    (void)
{
   /*---(locals)-----------+-----+-----+-*/
   int         i           =    0;
   int         c           =    0;
   tWORDS     *x_curr      = NULL;
   uchar       x_show      [LEN_HUND];
   /*---(search)-------------------------*/
   x_curr = e_hword;
   while (x_curr != NULL) {
      WORDS_drawn_show (x_curr->drawn, x_show);
      printf ("%-4d %2d:%-25.25s %2d:%-25.25s %s\n", c, x_curr->e_len, x_curr->english, x_curr->g_len, x_curr->gregg, x_show);
      x_curr = x_curr->e_next;
      ++c;
   }
   /*---(complete)-----------------------*/
   return 0;
}

char
WORDS_drawn_show        (short a_drawn [], uchar *a_out)
{
   int         i           =    0;
   short       n           =    0;
   char        t           [LEN_TERSE] = "";
   strlcpy (a_out, "" , LEN_HUND);
   for (i = 0; i < LEN_LABEL; ++i) {
      n = a_drawn [i];
      if (n == 0)  break;
      if (i > 0)  strlcat (a_out, "·", LEN_HUND);
      if (n > 0)  strlcpy (t, g_loc [n].label, LEN_HUND);
      else {
         strlcpy (t, g_loc [-n].label, LEN_HUND);
         t [0] = 'e';
      }
      strlcat (a_out, t, LEN_HUND);
   }
   return 0;
}

uchar  s_ae  [16] [16] [LEN_TERSE] = {
   /* prev/next ----, d---, oth-, uth-, -df-, -m--, -k--, -r--, -rd-, -or-, -ng-, -ch-, -f--, -p--, -pt- */
   /*   -  */ {"a"  ,"ad" ,"aT" ,"am" ,"aT" ,"axm","ak" ,"ar" ,"ar" ,"-"  ,"a"  ,"a"  ,"af" ,"ap" ,"-"   },
   /*           ----, d---, uth-, oth-, -df-, -m--, -k--, -r--, -rd-, -or-, -ng-, -ch-, -f--, -p--, -pt- */
   /*   d  */ {"a"  ,"add","add","adH","add","amd","a"  ,"a"  ,"a"  ,"-"  ,"a"  ,"a"  ,"a"  ,"a"  ,"-"   },
   /* oth  */ {"a"  ,"a"  ,"a"  ,"a"  ,"a"  ,"am" ,"a"  ,"a"  ,"a"  ,"-"  ,"a"  ,"a"  ,"a"  ,"a"  ,"-"   },
   /* uth  */ {"a"  ,"a"  ,"a"  ,"a"  ,"a"  ,"aHm","a"  ,"a"  ,"a"  ,"-"  ,"a"  ,"a"  ,"a"  ,"a"  ,"-"   },
   /*  df  */ {"a"  ,"a"  ,"a"  ,"a"  ,"a"  ,"aDm","a"  ,"a"  ,"a"  ,"-"  ,"a"  ,"a"  ,"a"  ,"a"  ,"-"   },
   /*           ----, d---, uth-, oth-, -df-, -m--, -k--, -r--, -rd-, -or-, -ng-, -ch-, -f--, -p--, -pt- */
   /*   m  */ {"amx","adm","amT","amH","afm","amm","amk","amr","amr","-"  ,"amN","amj","amf","amp","-"   },
   /*   k  */ {"a"  ,"a"  ,"a"  ,"a"  ,"a"  ,"akm","a"  ,"ar" ,"a"  ,"-"  ,"a"  ,"a"  ,"a"  ,"a"  ,"-"   },
   /*   r  */ {"a"  ,"a"  ,"a"  ,"a"  ,"a"  ,"arm","a"  ,"a"  ,"a"  ,"-"  ,"a"  ,"a"  ,"a"  ,"a"  ,"-"   },
   /*  rd  */ {"a"  ,"a"  ,"a"  ,"a"  ,"a"  ,"aRm","a"  ,"a"  ,"a"  ,"-"  ,"a"  ,"a"  ,"a"  ,"a"  ,"-"   },
   /*> /+  or  +/ {"a"  ,"a"  ,"a"  ,"a"  ,"a"  ,"arm","a"  ,"a"  ,"a"  ,"-"  ,"a"  ,"a"  ,"a"  ,"a"  ,"-"   },   <*/
   /*           ----, d---, uth-, oth-, -df-, -m--, -k--, -r--, -rd-, -or-, -ng-, -ch-, -f--, -p--, -pt- */
   /*  ng  */ {"a"  ,"a"  ,"a"  ,"a"  ,"a"  ,"aNm","a"  ,"a"  ,"a"  ,"-"  ,"a"  ,"a"  ,"a"  ,"a"  ,"-"   },
   /*           ----, d---, uth-, oth-, -df-, -m--, -k--, -r--, -rd-, -or-, -ng-, -ch-, -f--, -p--, -pt- */
   /*  ch  */ {"a"  ,"a"  ,"a"  ,"a"  ,"a"  ,"ajm","a"  ,"a"  ,"a"  ,"-"  ,"a"  ,"a"  ,"a"  ,"a"  ,"-"   },
   /*   f  */ {"a"  ,"a"  ,"a"  ,"a"  ,"a"  ,"afm","a"  ,"a"  ,"a"  ,"-"  ,"a"  ,"a"  ,"a"  ,"a"  ,"-"   },
   /*   p  */ {"a"  ,"a"  ,"a"  ,"a"  ,"a"  ,"apm","a"  ,"a"  ,"a"  ,"-"  ,"a"  ,"a"  ,"a"  ,"a"  ,"-"   },
   /*  pt  */ {"-"  ,"-"  ,"-"  ,"-"  ,"-"  ,"-"  ,"-"  ,"-"  ,"-"  ,"-"  ,"-"  ,"-"  ,"-"  ,"-"  ,"-"   },
   /*           ----, d---, uth-, oth-, -df-, -m--, -k--, -r--, -rd-, -or-, -ng-, -ch-, -f--, -p--, -pt- */
};

uchar  s_ou   [16] [LEN_LABEL] = {
   /*         xdTHDmkrRNjfpP"   */
   /* -   */ "·798·983····8·",
   /* d   */ "······8·······",
   /* oth */ "··············",
   /* uth */ "··············",
   /* df  */ "··············",
   /* m   */ "·····8········",
   /* k   */ "··············",
   /* r   */ "············8·",
   /* rd  */ "··············",
   /* ng  */ "··············",
   /* ch  */ "··············",
   /* f   */ "··············",
   /* p   */ "·4·····4······",
   /* pt  */ "··············",
};

char
WORDS_table_ae          (void)
{
   /*---(locals)-----------+-----+-----+-*/
   uchar       x_prev      =    0;
   uchar       x_next      =    0;
   int         x_len       =    0;
   uchar       x_ltr       [LEN_TERSE] = "";
   ushort      n           =    0;
   /*---(header)-------------------------*/
   DEBUG_CONF   yLOG_enter   (__FUNCTION__);
   x_len = strlen (CAT_ALL);
   for (x_prev = 0; x_prev < x_len; ++x_prev) {
      for (x_next = 0; x_next < x_len; ++x_next) {
         if (CAT_ALL [x_next] == '·')    continue;
         sprintf (x_ltr, "a%c%c", CAT_ALL [x_prev], CAT_ALL [x_next]);
         /*> printf ("%-3.3s  " , x_ltr);                                             <*/
         n = REVERSE_find_letter (x_ltr, LTRS_ALL);
         if (n > 0) {
            /*> printf ("%-3.3s  " , x_ltr);                                          <*/
            strlcpy (s_ae [x_prev][x_next], x_ltr, LEN_TERSE);
         } else {
            strlcpy (s_ae [x_prev][x_next], "a", LEN_TERSE);
            /*> printf ("X%-2.2s  ", x_ltr + 1);                                      <*/
         }
         DEBUG_CONF   yLOG_complex ("table"     , "%2d %2d %s", x_prev, x_next, x_ltr);
      }
      /*> printf ("\n");                                                              <*/
   }
   /*---(complete)-----------------------*/
   DEBUG_CONF   yLOG_exit    (__FUNCTION__);
}


/*> #define    CAT_D       1                                                          <* 
 *> #define    CAT_UTH     2                                                          <* 
 *> #define    CAT_OTH     3                                                          <* 
 *> #define    CAT_DF      4                                                          <* 
 *>                                                                                   <* 
 *> #define    CAT_M       5                                                          <* 
 *> #define    CAT_K       6                                                          <* 
 *> #define    CAT_R       7                                                          <* 
 *> #define    CAT_RD      8                                                          <* 
 *> #define    CAT_OR      9                                                          <* 
 *>                                                                                   <* 
 *> #define    CAT_NG     10                                                          <* 
 *>                                                                                   <* 
 *> #define    CAT_CH     11                                                          <* 
 *> #define    CAT_F      12                                                          <* 
 *> #define    CAT_P      13                                                          <* 
 *> #define    CAT_PT     14                                                          <* 
 *>                                                                                   <* 
 *> #define    CAT_A      20                                                          <* 
 *> #define    CAT_E      21                                                          <* 
 *> #define    CAT_O      22                                                          <* 
 *> #define    CAT_U      23                                                          <*/

char
WORDS_drawn_fix         (uchar *a_index, short a_drawn [])
{
   /*---(locals)-----------+-----+-----+-*/
   char        rce         =  -10;
   int         i           =    0;
   uchar       x_prev      =    0;
   uchar       x_curr      =    0;
   uchar       x_next      =    0;
   uchar       x_pcat      =    0;
   uchar       x_ncat      =    0;
   uchar       x_answer    =  '-';
   uchar       x_name      [LEN_TERSE] = "";
   ushort      n           =    0;
   /*---(header)-------------------------*/
   DEBUG_CONF   yLOG_enter   (__FUNCTION__);
   /*---(defense)------------------------*/
   DEBUG_CONF   yLOG_point   ("a_index"   , a_index);
   --rce;  if (a_index == NULL) {
      DEBUG_CONF   yLOG_exitr   (__FUNCTION__, rce);
      return rce;
   }
   DEBUG_CONF   yLOG_point   ("a_drawn"   , a_drawn);
   --rce;  if (a_drawn == NULL) {
      DEBUG_CONF   yLOG_exitr   (__FUNCTION__, rce);
      return rce;
   }
   /*---(purge output)-------------------*/
   for (i = 0; i < LEN_LABEL; ++i)  a_drawn [i] = 0;
   /*---(walk index)---------------------*/
   for (i = 0; i < LEN_LABEL; ++i) {
      /*---(get positions)---------------*/
      if (i > 0)  x_prev = a_index [i - 1];
      x_curr = a_index [i];
      x_next = a_index [i + 1];
      DEBUG_CONF   yLOG_complex ("LOOP"      , "#%2d, prev %3d, curr %3d, next %3d", i, x_prev, x_curr, x_next);
      if (x_curr == 0)   break;
      /*---(adjust)----------------------*/
      x_pcat   = g_loc [x_prev].lcat;
      x_ncat   = g_loc [x_next].lcat;
      if (g_loc [x_curr].lcat == CAT_A || g_loc [x_curr].lcat == CAT_E) {
         DEBUG_CONF   yLOG_note    ("adjusting a/e vowels");
         strlcpy (x_name, s_ae [x_pcat][x_ncat], LEN_TERSE);
         DEBUG_CONF   yLOG_complex ("table"     , "prev %2d, next %2d, %s", x_pcat, x_ncat, x_name);
         DEBUG_CONF   yLOG_complex ("p-less"    , "prev %2d, next %2d, %s", x_pcat-1, x_ncat, s_ae [x_pcat-1][x_ncat]);
         DEBUG_CONF   yLOG_complex ("p-more"    , "prev %2d, next %2d, %s", x_pcat+1, x_ncat, s_ae [x_pcat+1][x_ncat]);
         DEBUG_CONF   yLOG_complex ("n-less"    , "prev %2d, next %2d, %s", x_pcat, x_ncat-1, s_ae [x_pcat][x_ncat-1]);
         DEBUG_CONF   yLOG_complex ("n-more"    , "prev %2d, next %2d, %s", x_pcat, x_ncat+1, s_ae [x_pcat][x_ncat+1]);
         n = REVERSE_find_letter (x_name, LTRS_ALL);
         a_drawn [i] = n;
         if (g_loc [x_curr].lcat == CAT_E)   a_drawn [i] = -n;
         DEBUG_CONF   yLOG_complex ("update"    , "%-5.5s %4d %4d", x_name, n, a_drawn [i]);
      } else if (g_loc [x_curr].lcat == CAT_O || g_loc [x_curr].lcat == CAT_U) {
         sprintf (x_name, "o%c", s_ou [x_pcat][x_ncat]);
         if (g_loc [x_curr].lcat == CAT_U)  x_name [0] = 'u';
         n = REVERSE_find_letter (x_name, LTRS_ALL);
         if (n <= 0) {
            x_name [1] = '\0';
            n = REVERSE_find_letter (x_name, LTRS_ALL);
         }
         a_drawn [i] = n;
      } else {
         DEBUG_CONF   yLOG_value   ("copy"      , a_index [i]);
         a_drawn [i] = a_index [i];
      }
      /*---(done)------------------------*/
   }
   /*---(report out)---------------------*/
   for (i = 0; i < LEN_LABEL; ++i) {
      DEBUG_CONF   yLOG_complex ("a_drawn"   , "%2d %3d", i, a_drawn [i]);
   }
   /*---(complete)-----------------------*/
   DEBUG_CONF   yLOG_exit    (__FUNCTION__);
   return 0;
}

int
words_outstring (            /* locate outline in the translation dictionary  */
      char     *a_outstring)           /* outline letters                     */
{
   int i;
   if (strcmp(o.grade, "BAD") == 0) {
      strlcpy (o.word, "(malformed)", MAX_LEN);
      return 0;
   }
   strlcpy (o.word, "(unknown)", MAX_LEN);
   int count = 0;
   for (i = 0; i < MAX_WORDS; ++i) {
      if (strncmp(s_words[i].english, "eof",  MAX_LEN) == 0)  return -1;
      /*> DEBUG__MATCHES  printf("look <<%s>>\n", s_words[i].gregg);                    <*/
      if (strncmp(s_words[i].gregg, a_outstring, MAX_LEN) != 0)  continue;
      /*> DEBUG__WRITING  printf("%3d) <<%s>>\n", i, s_words[i].gregg);             <* 
       *> DEBUG__MATCHES  printf("%3d) <<%s>>\n", i, s_words[i].gregg);             <*/
      ++count;
      if (count == 1) strlcpy (o.word, "", MAX_LEN);
      if (count >  1) strncat(o.word, ",", MAX_LEN);
      strncat(o.word, s_words[i].english, MAX_LEN);
      /*> return  i;                                                                  <*/
   }
   return  0;
}



/*============================--------------------============================*/
/*===----                            outlines                          ----===*/
/*============================--------------------============================*/

/*> char                                                                                        <* 
 *> WORDS_display        (char *a_words, char a_base)                                           <* 
 *> {                                                                                           <* 
 *>    char   *q  = " ";                                                                        <* 
 *>    char   *p0 = NULL;                                                                       <* 
 *>    char   *p  = NULL;                                                                       <* 
 *>    char   *pn = NULL;                                                                       <* 
 *>    char   *s  = NULL;                                                                       <* 
 *>    int     i  = 0;                                                                          <* 
 *>    char    x_words[500];                                                                    <* 
 *>    static  x_called =  0;                                                                   <* 
 *>    strlcpy (x_words, a_words, 500);                                                         <* 
 *>    /+> printf("%4d  ALL  : %s\n", x_called++, x_words);                               <+/   <* 
 *>    p0 = p = strtok_r(x_words, q, &s);                                                       <* 
 *>    /+> printf("ok so far\n");                                                         <+/   <* 
 *>    if (p == NULL) return -1;                                                                <* 
 *>    WORDS_start();                                                                           <* 
 *>    /+> printf("ok so again\n");                                                       <+/   <* 
 *>    while (1) {                                                                              <* 
 *>       pn = x_words + (p - p0);                                                              <* 
 *>       /+> i = WORDS_find (pn);                                                        <+/   <* 
 *>       i = english_find (pn);                                                                <* 
 *>       /+> printf("\nsource = %p, word = %s, index = %d\n", x_words, pn, i);           <+/   <* 
 *>       WORDS_outline (i, a_base);                                                            <* 
 *>       p = strtok_r(NULL, q, &s);                                                            <* 
 *>       if (p == NULL) break;                                                                 <* 
 *>    }                                                                                        <* 
 *>    return 0;                                                                                <* 
 *> }                                                                                           <*/

/*> char                                                                                        <* 
 *> WORDS_start (void)           /+ reset outline display to beginning of page    +/            <* 
 *> {  /+-T=initializer-------S=procedure-------I=file------------*=unknown-------+/            <* 
 *>    /+---(initialize)-----------------------+/                                               <* 
 *>    /+> outx  = BASX;                                                                  <+/   <* 
 *>    /+> outy  = BASY;                                                                  <+/   <* 
 *>    outx  = 0.0;                                                                             <* 
 *>    outy  = 0.0;                                                                             <* 
 *>    /+---(complete)-------------------------+/                                               <* 
 *>    return 0;                                                                                <* 
 *> }                                                                                           <*/

/*> char          /+----: display the stylized outline of the current word -------+/            <* 
 *> WORDS_result       (void)                                                                   <* 
 *> {                                                                                           <* 
 *>    /+---(locals)-------------------------+/                                                 <* 
 *>    int       i;                                                                             <* 
 *>    char     *p         = NULL;                                                              <* 
 *>    char     *q         = ",";                                                               <* 
 *>    char      xword[100];                                                                    <* 
 *>    /+---(initialize)---------------------+/                                                 <* 
 *>    strlcpy (xword, o.word, 100);                                                            <* 
 *>    if (xword[0] == '(') return -1;                                                          <* 
 *>    WORDS_start();                                                                           <* 
 *>    /+---(take first word)----------------+/                                                 <* 
 *>    p = strtok(xword, q);                                                                    <* 
 *>    if (p != NULL) {                                                                         <* 
 *>       /+> i = WORDS_find(p);                                                          <+/   <* 
 *>       i = english_find (p);                                                                 <* 
 *>       WORDS_outline (i, '-');                                                               <* 
 *>    }                                                                                        <* 
 *>    /+---(complete)-----------------------+/                                                 <* 
 *>    return 0;                                                                                <* 
 *> }                                                                                           <*/

/*> char                                                                              <* 
 *> WORDS__width         (int a_index)                                                <* 
 *> {                                                                                 <* 
 *> }                                                                                 <*/

char
WORDS_outline        (int a_index, char a_base)
{
   /*---(locals)--------------------------------*/
   int    letter = 0;        /* current letter                                */
   int    offset = 0;        /* pointes to correct version of vowel           */
   int    left   = 0;        /* leftmost position                             */
   int    right  = 0;        /* rightmost position                            */
   int    temp   = 0;        /* calc variable                                 */
   int    ltr    = 0;        /* current letter index                          */
   int i;
   /*---(set up letters)------------------------*/
   int    count  = words_translate(a_index);
   printf("CREATING OUTLINE...\n");
   /*---(test for width too large)--------------*/
   printf("   test width...\n");
   posx = posy = 0;
   for (i = 0; i < count; ++i) {
      ltr   = letters[i];
      posy += g_loc[ltr].y_end;
      if (posy < 50 && posy > -50) {
         temp = posx + g_loc[ltr].x_lef;
         /*> if (temp < left ) left  = temp;                                          <*/
         temp = posx + g_loc[ltr].x_rig;
         if (temp > right) right = temp;
      }
      posx += g_loc[ltr].x_end;
      printf("      %02d (%03d) %-5.5s :: ", i, ltr, g_loc[ltr].label);
      printf("tx = %4.0f, px  = %4d, py  = %4d, le  = %4d, ri = %4d\n",
            g_loc[ltr].x_end, posx, posy, left, right);
   }
   if (outx - left + right > 280) { /* 260 small, 360 pad */
      outx  = BASX;     /* reset to left margin      */
      outy += LINE;     /* go to nextg line           */
   }
   /*---(continue the base line)----------------*/
   printf("   drawing...\n");
   printf("      starting at %4dx, %4dy\n", outx - left, outy);
   if (a_base == 'y') {
      glPushMatrix(); {
         glTranslatef (outx - left,  outy,  0.0);
         glColor4f    (0.0f, 0.0f, 0.0f, 0.7f);
         glLineWidth  ( 3.0);
         glBegin (GL_LINES); {
            glVertex3f   (left            , 0, 50);
            glVertex3f   (right + OUT_XGAP, 0, 50);
         } glEnd ();
         glPointSize  (10.0);
         glBegin (GL_POINTS); {
            glVertex3f   (left            , 0, 50);
            glVertex3f   (right + OUT_XGAP, 0, 50);
         } glEnd ();
      } glPopMatrix();
   }
   /*---(LOOP)----------------------------------*/
   posx = posy = 0;
   /*---(set outline starting point)------------*/
   glPushMatrix();
   glTranslatef(outx - left,  outy,  0.0);
   glLineWidth(1.5);
   for (i = 0; i < count; ++i) {
      /*---(establish values)-------------------*/
      letter = letters[i];
      offset = 0;
      /*---(watch initial left)-----------------*/
      /*> if (i == 0 && g_loc[letter].x_lef < 0) {                                       <* 
       *>    glTranslatef(-g_loc[letter].x_lef,  0,  0.0);                               <* 
       *>    posx  =  -g_loc[letter].x_lef;                                              <* 
       *> }                                                                           <*/
      /*---(draw letter)------------------------*/
      /*> glColor4f(0.0f, 0.0f, 0.5f, 0.7f);                                          <*/
      glColor4f (0.0f, 0.0f, 0.0f, 0.7f);
      if (g_loc[letter].type == 'c') {
         /*> offset = words_vowel(i, count) - 1;                                      <*/
         offset = 0;
         glCallList (dl_solid + letter + offset);
      } else {
         /*> words_consonant(i, count);                                               <*/
         glCallList (dl_solid + letter);
      }
      /*---(update g_loc)-------------------------*/
      printf("      letter = %3d, offset = %2d\n", letter, offset);
      glTranslatef(g_loc[letter].x_end,  g_loc[letter].y_end,  0.0);
      posx += g_loc[letter].x_end;
      posy += g_loc[letter].y_end;
   }
   printf("      ending   at %4dx, %4dy, %4dox\n", outx + posx, outy + posy, outx);
   outx += -left + right + OUT_XGAP;
   glPopMatrix();
   printf("   DONE\n");
   /*---(complete)------------------------------*/
   return 0;
}

int           /*----: translate english word into gregg letters --------------*/
words_translate    (int a_word)
{
   /*---(locals)-------------------------*/
   char   *p = NULL;             // pointer to substring
   char   *q = ".";              // delimiters
   char   x_ltr[5] = "";             // letter
   int    count =    0;          // letter count
   int    offset =   0;
   char   a_outstring[MAX_LEN];
   int       i         = 0;            /* loop iterator : general             */
   /*---(init)---------------------------*/
   strlcpy (a_outstring, s_words[a_word].gregg, MAX_LEN);
   /*---(clear letters)------------------*/
   for (i = 0; i < 30; ++i) {
      letters[i] = 0;
   }
   /*---(get the first letter, always ">")------*/
   DEBUG__WRITING  printf("OUTSTRING = %s\n", a_outstring);
   p = strtok(a_outstring, q);
   if (p == NULL || strncmp(p, "", MAX_LEN) == 0) {
      return -1;
   }
   /*---(run through the letters)---------------*/
   while (1) {
      p = strtok(NULL, q);
      if (p == NULL) {
         if (letters[count - 1] == 1) {
            letters[count] = 2;
            ++count;
         }
         break;
      }
      /*---(new stroke)------------------*/
      if (strncmp(p, ">", 5) == 0) {
         if (count == 0 || letters[count - 1] == 1) {
            letters[count] = 2;
            ++count;
         }
      }
      /*---(handle letter)---------------*/
      strlcpy (x_ltr, p, 5);
      for (i = 0; i < 5; ++i)  x_ltr[i] = tolower(x_ltr[i]);
      DEBUG__WRITING  printf("   letter = <<%s>>\n", x_ltr);
      for (i = 0; i < MAX_LETTERS; ++i) {
         if (strncmp(x_ltr, g_loc[i].label, 5) != 0) continue;
         letters[count]  = i;
         break;
      }
      if (letters[count] == 0) continue;
      DEBUG__WRITING  printf("      ind = <<%d>>\n", letters[count]);
      ++count;
   }
   for (i = 0; i < count; ++i) {
      offset = 0;
      if (g_loc[letters[i]].type == 'c') { /*> offset = words_vowel(i, count) - 1;                                      <*/
         /*> offset = words_vowel(i, count);                                          <*/
         letters[i] += offset;
      }
      DEBUG__WRITING  printf("%d, ", letters[i]);
   }
   DEBUG__WRITING  printf("\n");
   /*---(complete)-----------------------*/
   return count;
}

/*> int                                                                                         <* 
 *> words_consonant (            /+ adjust for combination consontants            +/            <* 
 *>       int       a_index,               /+ -- index to word in dictionary      +/            <* 
 *>       int       a_count)               /+ -- position of current letter       +/            <* 
 *> {                                                                                           <* 
 *>    /+---(locals)-------------------------+/                                                 <* 
 *>    int letter;                                                                              <* 
 *>    int one;                                                                                 <* 
 *>    int two;                                                                                 <* 
 *>    int adjx = 0;                                                                            <* 
 *>    int adjy = 0;                                                                            <* 
 *>    int i;                                                                                   <* 
 *>    if (a_index == 0)            return -1;                                                  <* 
 *>    /+---(previous letter)----------------+/                                                 <* 
 *>    letter = letters[a_index - 1];                                                           <* 
 *>    one    = 0;                                                                              <* 
 *>    for (i = 0; i < MAX_GROUPS; ++i) {                                                       <* 
 *>       if (i > 0 && strncmp(groups[i].cat, groups[i - 1].cat, 5) == 0) continue;             <* 
 *>       /+> printf("%s ", groups[i].cat);                                             <+/     <* 
 *>       if (strncmp(groups[i].cat, "eof",  5) == 0)                    break;                 <* 
 *>       if (strncmp(g_loc[letter].cat, groups[i].cat, 5) != 0)            continue;           <* 
 *>       one = groups[i].gr_num;                                                               <* 
 *>       break;                                                                                <* 
 *>    }                                                                                        <* 
 *>    /+---(current letter)-----------------+/                                                 <* 
 *>    letter = letters[a_index];                                                               <* 
 *>    two    = 0;                                                                              <* 
 *>    for (i = 0; i < MAX_GROUPS; ++i) {                                                       <* 
 *>       if (i > 0 && strncmp(groups[i].cat, groups[i - 1].cat, 5) == 0) continue;             <* 
 *>       /+> printf("%s ", groups[i].cat);                                             <+/     <* 
 *>       if (strncmp(groups[i].cat, "eof",  5) == 0)                    break;                 <* 
 *>       if (strncmp(g_loc[letter].cat, groups[i].cat, 5) != 0)            continue;           <* 
 *>       two = groups[i].gr_num;                                                               <* 
 *>       break;                                                                                <* 
 *>    }                                                                                        <* 
 *>    /+---(complete)-----------------------+/                                                 <* 
 *>    if (one ==  5 && two == 13) {                                                            <* 
 *>       /+> DEBUG__WRITING  printf("found a C-V\n");                                    <+/   <* 
 *>       if (strncmp(g_loc[letters[a_index - 1]].label, "u", 5) != 0) {                        <* 
 *>          adjx = -4;                                                                         <* 
 *>          adjy =  4;                                                                         <* 
 *>       }                                                                                     <* 
 *>    } else if (one == 14 && two ==  6) {                                                     <* 
 *>       /+> DEBUG__WRITING  printf("found a P-R\n");                                    <+/   <* 
 *>       if (strncmp(g_loc[letters[a_index - 1]].label, "o", 5) != 0) {                        <* 
 *>          adjx = -4;                                                                         <* 
 *>          adjy =  4;                                                                         <* 
 *>       }                                                                                     <* 
 *>    }                                                                                        <* 
 *>    glTranslatef(adjx, adjy,  0.0);                                                          <* 
 *>    posx += adjx;                                                                            <* 
 *>    posy += adjy;                                                                            <* 
 *>    /+---(complete)-----------------------+/                                                 <* 
 *>    return 0;                                                                                <* 
 *> }                                                                                           <*/

/*> int                                                                                                           <* 
 *> words_vowel (                /+ adjust vowel shape for layout                 +/                              <* 
 *>       int       a_index,               /+ -- index to word in dictionary      +/                              <* 
 *>       int       a_count)               /+ -- position of current letter       +/                              <* 
 *> {                                                                                                             <* 
 *>    /+---(locals)--------------------------------+/                                                            <* 
 *>    int letter;                                                                                                <* 
 *>    int one;                                                                                                   <* 
 *>    int two;                                                                                                   <* 
 *>    int offset;                                                                                                <* 
 *>    int i;                                                                                                     <* 
 *>    /+---(prev letter)---------------------------+/                                                            <* 
 *>    if (a_index == 0) {                                                                                        <* 
 *>       one = 0;                                                                                                <* 
 *>       /+> printf("   letter one = %2d, %2s ( ", 0, "-");                              <+/                     <* 
 *>    } else {                                                                                                   <* 
 *>       letter = letters[a_index - 1];                                                                          <* 
 *>       /+> printf("   letter one = %2d, %2s, %2s ( ", letter, g_loc[letter].label, g_loc[letter].cat);   <+/   <* 
 *>       one    = 0;                                                                                             <* 
 *>       for (i = 0; i < MAX_GROUPS; ++i) {                                                                      <* 
 *>          if (i > 0 && strncmp(groups[i].cat, groups[i - 1].cat, 5) == 0) continue;                            <* 
 *>          /+> printf("%s ", groups[i].cat);                                             <+/                    <* 
 *>          if (strncmp(groups[i].cat, "eof",  5) == 0)                    break;                                <* 
 *>          if (strncmp(g_loc[letter].cat, groups[i].cat, 5) != 0)            continue;                          <* 
 *>          one = groups[i].gr_num;                                                                              <* 
 *>          break;                                                                                               <* 
 *>       }                                                                                                       <* 
 *>    }                                                                                                          <* 
 *>    /+> printf(") so %2d\n", one);                                                     <+/                     <* 
 *>    /+---(nextg letter)---------------------------+/                                                           <* 
 *>    if (a_index == a_count - 1) {                                                                              <* 
 *>       two = 0;                                                                                                <* 
 *>       /+> printf("   letter one = %2d, %2s ( ", 0, "-");                              <+/                     <* 
 *>    } else {                                                                                                   <* 
 *>       letter = letters[a_index + 1];                                                                          <* 
 *>       /+> printf("   letter two = %2d, %2s, %2s ( ", letter, g_loc[letter].label, g_loc[letter].cat);   <+/   <* 
 *>       two    = 0;                                                                                             <* 
 *>       for (i = 0; i < MAX_GROUPS; ++i) {                                                                      <* 
 *>          if (i > 0 && strncmp(groups[i].cat, groups[i - 1].cat, 5) == 0) continue;                            <* 
 *>          /+> printf("%s ", groups[i].cat);                                             <+/                    <* 
 *>          if (strncmp(groups[i].cat, "eof",  5) == 0)                    break;                                <* 
 *>          if (strncmp(g_loc[letter].cat, groups[i].cat, 5) != 0)            continue;                          <* 
 *>          two = groups[i].gr_num;                                                                              <* 
 *>          break;                                                                                               <* 
 *>       }                                                                                                       <* 
 *>    }                                                                                                          <* 
 *>    /+> printf(") so %2d\n", two);                                                     <+/                     <* 
 *>    /+---(get offset)----------------------------+/                                                            <* 
 *>    offset  = vowels[one][two];                                                                                <* 
 *>    if (offset < 0) offset = 0;                                                                                <* 
 *>    /+> printf("    prv = %2d, nxt = %2d, adj = %2d\n", one, two, offset);             <+/                     <* 
 *>    /+---(complete)------------------------------+/                                                            <* 
 *>    return offset;                                                                                             <* 
 *> }                                                                                                             <*/



/*============================--------------------============================*/
/*===----                         unit testing                         ----===*/
/*============================--------------------============================*/
static void o___UNIT_TEST_________________o (void) {;}

char        unit_answer  [LEN_STR];

char*        /*-> unit test accessor -----------------[ light  [us.D90.241.L0]*/ /*-[03.0000.00#.#]-*/ /*-[--.---.---.--]-*/
WORDS__unit          (char *a_question, int a_num)
{
   /*---(locals)-----------+-----+-----+-*/
   int         i           = 0;
   char        t           [100] = "";
   char        s           [100] = "";
   char        u           [100] = "";
   char        x_show      [100] = "";
   int         c           = 0;
   tWORDS     *x_curr      = NULL;
   int         x_fore      = 0;
   int         x_back      = 0;
   /*---(initialize)---------------------*/
   strlcpy (unit_answer, "WORDS unit       : unknownn request", 100);
   /*---(core data)----------------------*/
   if        (strncmp (a_question, "parsing"   , 20)  == 0) {
      sprintf  (s, "%2d[%.20s]", strlen (s_english), s_english);
      sprintf  (t, "%2d[%.20s]", strlen (s_gregg)  , s_gregg);
      snprintf (unit_answer, LEN_STR, "WORDS parsing    : %-24.24s  %-24.24s  %c  %c  %3d  %c    %c  %c  %c  %c  %c", s, t, s_ver, s_book, s_page, s_type, s_diff, s_simp, s_3rd, s_4th, s_top);
   }
   else if (strcmp(a_question, "e_count"       ) == 0) {
      x_curr = e_hword; while (x_curr != NULL) { ++x_fore; x_curr = x_curr->e_next; }
      x_curr = e_tword; while (x_curr != NULL) { ++x_back; x_curr = x_curr->e_prev; }
      snprintf (unit_answer, LEN_FULL, "WORDS e_count    : %4dn, %4df, %4db", e_nword, x_fore, x_back);
   }
   else if (strcmp(a_question, "g_count"       ) == 0) {
      x_curr = g_hword; while (x_curr != NULL) { ++x_fore; x_curr = x_curr->g_next; }
      x_curr = g_tword; while (x_curr != NULL) { ++x_back; x_curr = x_curr->g_prev; }
      snprintf (unit_answer, LEN_FULL, "WORDS g_count    : %4dn, %4df, %4db", g_nword, x_fore, x_back);
   }
   else if   (strncmp (a_question, "e_entry"   , 20)  == 0) {
      x_curr = e_hword;
      while (x_curr != NULL) {
         if (c == a_num)   break;
         x_curr = x_curr->e_next;
         ++c;
      }
      if (x_curr == NULL)  snprintf (unit_answer, LEN_FULL, "WORDS eng   (%2d) :  -[]                       -[]                      []", a_num);
      else {
         sprintf  (s, "%2d[%.20s]", x_curr->e_len, x_curr->english);
         sprintf  (t, "%2d[%.20s]", x_curr->g_len, x_curr->gregg);
         WORDS_drawn_show (x_curr->drawn, x_show);
         sprintf  (u, "[%.20s]"   , x_show);
         snprintf (unit_answer, LEN_STR, "WORDS eng   (%2d) : %-24.24s  %-24.24s  %s", a_num, s, t, u);
      }
   }
   else if   (strncmp (a_question, "g_entry"   , 20)  == 0) {
      x_curr = g_hword;
      while (x_curr != NULL) {
         if (c == a_num)   break;
         x_curr = x_curr->g_next;
         ++c;
      }
      if (x_curr == NULL)  snprintf (unit_answer, LEN_FULL, "WORDS gregg (%2d) :  -[]                       -[]                      []", a_num);
      else {
         sprintf  (s, "%2d[%.20s]", x_curr->e_len, x_curr->english);
         sprintf  (t, "%2d[%.20s]", x_curr->g_len, x_curr->gregg);
         WORDS_drawn_show (x_curr->drawn, x_show);
         sprintf  (u, "[%.20s]"   , x_show);
         snprintf (unit_answer, LEN_STR, "WORDS gregg (%2d) : %-24.24s  %-24.24s  %s", a_num, s, t, u);
      }
   }
   /*---(complete)-----------------------*/
   return unit_answer;
}




/*============================----(source-end)----============================*/
