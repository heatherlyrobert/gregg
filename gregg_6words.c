/*============================---(source-start)---============================*/
#include "gregg.h"


#define     OUT_XGAP    10


/*
 *
 *    saying - a oft repeated set of words
 *    saying - the act of talking
 *
 *    liver  - someone who lives
 *    liver  - organ
 *
 *
 *
 */


/*
 *  parts
 *     v  verb
 *
 *  subs
 *     v r  top 600 regular verbs
 *     v /  top 100 irregular verbs
 *     v �  additions
 *
 *  grp
 *     �  word-sign (fast)  keep all pre-anniversary to get max speed
 *     �  phrase (fast)
 *     �  all others
 *
 *
 *
 *
 *
 */






static uchar    s_ver   = '�';
static uchar    s_book  = '�';
static int     s_page  =   0;
static uchar    s_type  = '�';

static uchar    s_diff  = '�';
static uchar    s_simp  = '�';
static uchar    s_3rd   = '�';
static uchar    s_4th   = '�';
static uchar    s_top   = '�';  /*  A = 100, b =500, c = 2000, - = others  */


/*> char       *english;                     /+ english word                   +/     <*/
/*> char        e_len;                                                                <* 
 *> uchar      *gregg;                       /+ gregg translation              +/     <* 
 *> char        g_len;                                                                <* 
 *> short       drawn       [LEN_LABEL];     /+ gregg as drawn                 +/     <*/
/*---(info)-----------------*/
/*> char        part;                        /+ primary part of speech         +/     <*/


char
WORDS_init              (void)
{
   /*---(locals)-----------+-----+-----+-*/
   char        rce         =  -10;
   char        rc          =    0;
   /*---(header)-------------------------*/
   DEBUG_CONF   yLOG_enter   (__FUNCTION__);
   /*---(initialize)---------------------*/
   rc = ySORT_btree (B_ENGLISH, "english");
   DEBUG_CONF   yLOG_value   ("english"   , rc);
   --rce;  if (rc < 0) {
      DEBUG_CONF   yLOG_exitr   (__FUNCTION__, rce);
      return rce;
   }
   rc = ySORT_btree (B_GREGG  , "gregg");
   DEBUG_CONF   yLOG_value   ("gregg"     , rc);
   --rce;  if (rc < 0) {
      DEBUG_CONF   yLOG_exitr   (__FUNCTION__, rce);
      return rce;
   }
   rc = ySORT_btree (B_UNIQUE , "unique");
   DEBUG_CONF   yLOG_value   ("unique"    , rc);
   --rce;  if (rc < 0) {
      DEBUG_CONF   yLOG_exitr   (__FUNCTION__, rce);
      return rce;
   }
   /*---(initialize)---------------------*/
   rc = ySORT_btree (B_TREE   , "tree");
   DEBUG_CONF   yLOG_value   ("tree"      , rc);
   --rce;  if (rc < 0) {
      DEBUG_CONF   yLOG_exitr   (__FUNCTION__, rce);
      return rce;
   }
   /*---(defaults)-----------------------*/
   my.r_nbase = 0;
   my.r_nword = 0;
   /*---(complete)-----------------------*/
   DEBUG_CONF   yLOG_exit    (__FUNCTION__);
   return 0;
}

char
WORDS__wipe             (tWORD *a_word)
{
   /*---(locals)-----------+-----+-----+-*/
   int         i           =    0;
   /*---(header)-------------------------*/
   a_word->w_english [0] = '\0';
   a_word->w_gregg   [0] = '\0';
   a_word->w_unique  [0] = '\0';
   /*---(updates)--------------*/
   for (i = 0; i < LEN_HUND;  ++i)  a_word->w_shown  [i] = '\0';
   for (i = 0; i < LEN_LABEL; ++i)  a_word->w_drawn  [i] = -1;
   for (i = 0; i < LEN_TERSE; ++i)  a_word->w_tree   [i] = -1;
   /*---(audit)----------------*/
   /*> a_word->trouble   = '-';                                                       <* 
    *> a_word->fancy     = NULL;                                                      <* 
    *> a_word->g_audit   = '-';                                                       <* 
    *> a_word->f_audit   = '-';                                                       <* 
    *> a_word->a_audit   = '-';                                                       <*/
   /*---(parsing)--------------*/
   a_word->w_file    = -1;
   a_word->w_line    = -1; 
   a_word->w_bref    = -1; 
   /*---(variations)-----------*/
   strcpy (a_word->w_vary, "<");
   a_word->w_base    = NULL;
   a_word->w_next    = NULL;
   a_word->w_nvary   = 0;
   /*---(part-of-speech)-------*/
   a_word->w_part    = '�';
   a_word->w_sub     = '�';
   /*---(source)---------------*/
   a_word->w_src     = '�';
   a_word->w_cat     = '�';
   a_word->w_page    = 0;
   /*---(frequency)------------*/
   a_word->w_grp     = '�';
   a_word->w_freq    = 0;
   /*---(ysort)----------------*/
   a_word->ysort_e   = NULL;
   a_word->ysort_g   = NULL;
   a_word->ysort_u   = NULL;
   a_word->ysort_t   = NULL;
   /*---(complete)-------------*/
   return 1;
}


/*====================------------------------------------====================*/
/*===----                      memory allocation                       ----===*/
/*====================------------------------------------====================*/
static void  o___MEMORY__________o () { return; }

char
WORDS__check            (char a_english [LEN_TITLE], char a_gregg [LEN_TITLE], char a_part, char r_full [LEN_DESC])
{
   /*---(locals)-----------+-----------+-*/
   char        rce         =  -10;
   char        rc          =  -10;
   tWORD     *x_dup       = NULL;
   /*---(begin)--------------------------*/
   DEBUG_CONF   yLOG_enter   (__FUNCTION__);
   /*---(defense)------------------------*/
   DEBUG_CONF   yLOG_point   ("a_english" , a_english);
   --rce;  if (a_english == NULL) {
      DEBUG_CONF   yLOG_exitr   (__FUNCTION__, rce);
      return rce;
   }
   DEBUG_CONF   yLOG_info    ("a_english" , a_english);
   --rce;  if (a_english [0] == '\0') {
      DEBUG_CONF   yLOG_exitr   (__FUNCTION__, rce);
      return rce;
   }
   DEBUG_CONF   yLOG_point   ("a_gregg"   , a_gregg);
   --rce;  if (a_gregg   == NULL) {
      DEBUG_CONF   yLOG_exitr   (__FUNCTION__, rce);
      return rce;
   }
   DEBUG_CONF   yLOG_info    ("a_gregg"   , a_gregg);
   --rce;  if (a_gregg   [0] == '\0') {
      DEBUG_CONF   yLOG_exitr   (__FUNCTION__, rce);
      return rce;
   }
   /*---(check for duplicate)------------*/
   sprintf (r_full, "%-20.20s  %c  %s", a_english, a_part, a_gregg);
   rc = ySORT_by_name (B_UNIQUE , r_full, &x_dup);
   DEBUG_CONF   yLOG_point   ("x_exist"   , x_dup);
   --rce;  while (x_dup != NULL) {
      DEBUG_CONF   yLOG_note    ("english/gregg combination already exists");
      DEBUG_CONF   yLOG_point   ("->name"    , x_dup->w_english);
      DEBUG_CONF   yLOG_exit    (__FUNCTION__);
      return 1;
   }
   /*---(complete)-----------------------*/
   DEBUG_CONF   yLOG_exit    (__FUNCTION__);
   return 0;
}

char
WORDS_new               (char a_force, tWORD **r_new)
{
   /*---(locals)-----------+-----------+-*/
   char        rce         =  -10;
   char        rc          =    0;
   int         x_tries     =    0;
   tWORD     *x_new       = NULL;
   /*---(begin)--------------------------*/
   DEBUG_CONF   yLOG_enter   (__FUNCTION__);
   /*---(defense)------------------------*/
   DEBUG_CONF   yLOG_point   ("r_new"     , r_new);
   --rce;  if (r_new  == NULL) {
      DEBUG_CONF   yLOG_exitr   (__FUNCTION__, rce);
      return rce;
   }
   DEBUG_CONF   yLOG_point   ("*r_new"    , *r_new);
   DEBUG_CONF   yLOG_char    ("a_force"   , a_force);
   --rce;  if (a_force != 'y' && *r_new  != NULL) {
      DEBUG_CONF   yLOG_exitr   (__FUNCTION__, rce);
      return rce;
   }
   /*---(default)------------------------*/
   *r_new == NULL;
   /*---(create cell)--------------------*/
   while (x_new == NULL) {
      ++x_tries;
      x_new = (tWORD *) malloc (sizeof (tWORD));
      if (x_tries > 10)   break;
   }
   DEBUG_CONF   yLOG_value   ("x_tries"   , x_tries);
   DEBUG_CONF   yLOG_point   ("x_new"     , x_new);
   --rce;  if (x_new == NULL) {
      DEBUG_CONF   yLOG_note    ("cell could not be created");
      DEBUG_CONF   yLOG_exitr   (__FUNCTION__, rce);
      return rce;
   }
   /*---(header)-------------------------*/
   DEBUG_CONF   yLOG_note    ("add all data");
   WORDS__wipe (x_new);
   /*---(save-back)----------------------*/
   *r_new = x_new;
   /*---(complete)-----------------------*/
   DEBUG_CONF   yLOG_exit    (__FUNCTION__);
   return 0;
}

char
WORDS__populate         (tWORD *a_new, char a_english [LEN_TITLE], char a_gregg [LEN_TITLE], char a_part, char a_full [LEN_DESC])
{
   /*---(locals)-----------+-----------+-*/
   char        rce         =  -10;
   char        rc          =    0;
   /*---(begin)--------------------------*/
   DEBUG_CONF   yLOG_enter   (__FUNCTION__);
   /*---(header)-------------------------*/
   DEBUG_CONF   yLOG_note    ("add all data");
   strlcpy (a_new->w_english, a_english, LEN_TITLE);
   strlcpy (a_new->w_gregg  , a_gregg  , LEN_TITLE);
   strlcpy (a_new->w_unique , a_full   , LEN_DESC);
   a_new->w_part = a_part;
   /*---(drawn)-------------------------*/
   /*> rc = AUDIT_gregg_outline (a_new->w_gregg, x_fancy);                            <* 
    *> printf ("%-15.15s  %s\n", a_new->w_english, x_fancy);                          <*/
   /*---(drawn)-------------------------*/
   rc = FIX_gregg (a_new->w_gregg, a_new->w_shown, a_new->w_drawn, a_new->w_tree);
   /*---(hook to tree)------------------*/
   rc = FIX_hook   (a_new);
   DEBUG_CONF   yLOG_value   ("fix"       , rc);
   --rce;  if (rc < 0) {
      DEBUG_CONF   yLOG_exitr   (__FUNCTION__, rce);
      return rce;
   }
   /*---(complete)-----------------------*/
   DEBUG_CONF   yLOG_exit    (__FUNCTION__);
   return 0;
}

char
WORDS_new_full          (char a_english [LEN_TITLE], char a_gregg [LEN_TITLE], char a_part, char a_base, tWORD **r_new)
{
   /*---(locals)-----------+-----------+-*/
   char        rce         =  -10;
   char        rc          =    0;
   char        x_full      [LEN_DESC]  = "";
   /*---(begin)--------------------------*/
   DEBUG_CONF   yLOG_enter   (__FUNCTION__);
   /*---(verify)-------------------------*/
   rc = WORDS__check  (a_english, a_gregg, a_part, x_full);
   DEBUG_CONF   yLOG_value   ("check"     , rc);
   --rce; if (rc < 0) {
      DEBUG_CONF   yLOG_exitr   (__FUNCTION__, rce);
      return rce;
   }
   if (rc == 1) {
      DEBUG_CONF   yLOG_exit    (__FUNCTION__);
      return 1;
   }
   /*---(malloc)-------------------------*/
   rc = WORDS_new     ('-', r_new);
   DEBUG_CONF   yLOG_value   ("malloc"    , rc);
   --rce; if (rc < 0) {
      DEBUG_CONF   yLOG_exitr   (__FUNCTION__, rce);
      return rce;
   }
   /*---(populate)-----------------------*/
   rc = WORDS__populate  (*r_new, a_english, a_gregg, a_part, x_full);
   DEBUG_CONF   yLOG_value   ("populate"  , rc);
   --rce; if (rc < 0) {
      DEBUG_CONF   yLOG_exitr   (__FUNCTION__, rce);
      return rce;
   }
   /*---(stats)--------------------------*/
   if (a_base == 'B' && strchr (a_gregg, '<') == NULL) {
      (*r_new)->w_bref = my.r_nbase;
      ++(my.r_nbase);
   }
   ++(my.r_nword);
   /*---(complete)-----------------------*/
   DEBUG_CONF   yLOG_exit    (__FUNCTION__);
   return 0;
}

char WORDS_new_base          (char a_english [LEN_TITLE], char a_gregg [LEN_TITLE], char a_part, tWORD **r_new)    { return WORDS_new_full (a_english, a_gregg, a_part, 'B', r_new); }
char WORDS_new_vary          (char a_english [LEN_TITLE], char a_gregg [LEN_TITLE], char a_part, tWORD **r_new)    { return WORDS_new_full (a_english, a_gregg, a_part, '-', r_new); }


char         /*-> remove a cell completely -----------[ ------ [fe.943.224.81]*/ /*-[11.0000.133.7]-*/ /*-[--.---.---.--]-*/
WORDS__free             (tWORD **b_word)
{
   /*---(locals)-----------+-----------+-*/
   char        rce         =  -10;
   char        rc          =    0;
   tWORD     *x_old       = NULL;
   /*---(beginning)----------------------*/
   DEBUG_CONF   yLOG_enter   (__FUNCTION__);
   /*---(defense: valid cell)------------*/
   DEBUG_CONF   yLOG_point   ("b_word"    , b_word);
   --rce;  if (b_word == NULL) {
      DEBUG_CONF   yLOG_exitr   (__FUNCTION__, rce);
      return rce;
   }
   DEBUG_CONF   yLOG_point   ("*b_word"   , *b_word);
   --rce;  if (*b_word == NULL) {
      DEBUG_CONF   yLOG_exitr   (__FUNCTION__, rce);
      return rce;
   }
   x_old = *b_word;
   /*---(unhook from btree)--------------*/
   rc = WORDS_unhook (x_old);
   DEBUG_CONF   yLOG_value   ("unhook"    , rc);
   --rce;  if (rc < 0) {
      DEBUG_CONF   yLOG_exitr   (__FUNCTION__, rce);
      return rce;
   }
   /*---(wipe data)----------------------*/
   free (x_old);
   /*---(save-back)----------------------*/
   *b_word = NULL;
   /*---(complete)-----------------------*/
   DEBUG_CONF   yLOG_exit    (__FUNCTION__);
   return 0;
}

char
WORDS_purge             (void)
{
   /*---(locals)-----------+-----+-----+-*/
   char        rce         =  -10;
   char        rc          =    0;
   tWORD      *x_word      = NULL;
   /*---(header)-------------------------*/
   DEBUG_CONF   yLOG_enter   (__FUNCTION__);
   /*---(walk)---------------------------*/
   rc = WORDS_eng_by_index  (0, &x_word);
   while (x_word != NULL) {
      rc = WORDS__free   (&x_word);
      rc = WORDS_eng_by_index  (0, &x_word);
   }
   /*---(defaults)-----------------------*/
   my.r_nbase = 0;
   my.r_nword = 0;
   /*---(complete)-----------------------*/
   DEBUG_CONF   yLOG_exit    (__FUNCTION__);
   return 0;
}



/*====================------------------------------------====================*/
/*===----                        searching                             ----===*/
/*====================------------------------------------====================*/
static void  o___HOOKING_________o () { return; }

char
WORDS_hook              (tWORD *a_new)
{
   /*---(locals)-----------+-----+-----+-*/
   char        rce         =  -10;
   char        rc          =    0;
   /*---(header)-------------------------*/
   DEBUG_CONF   yLOG_enter   (__FUNCTION__);
   /*---(english)------------------------*/
   rc = ySORT_hook (B_ENGLISH, a_new, a_new->w_english, &(a_new->ysort_e));
   DEBUG_CONF   yLOG_value   ("hook eng"  , rc);
   --rce;  if (rc < 0) {
      DEBUG_CONF   yLOG_exitr   (__FUNCTION__, rce);
      return rce;
   }
   rc = ySORT_prepare (B_ENGLISH);
   DEBUG_CONF   yLOG_value   ("prep eng"  , rc);
   --rce;  if (rc < 0) {
      DEBUG_CONF   yLOG_exitr   (__FUNCTION__, rce);
      return rce;
   }
   /*---(gregg)--------------------------*/
   rc = ySORT_hook (B_GREGG  , a_new, a_new->w_gregg  , &(a_new->ysort_g));
   DEBUG_CONF   yLOG_value   ("hook gregg", rc);
   --rce;  if (rc < 0) {
      DEBUG_CONF   yLOG_exitr   (__FUNCTION__, rce);
      return rce;
   }
   rc = ySORT_prepare (B_GREGG);
   DEBUG_CONF   yLOG_value   ("prep gregg", rc);
   --rce;  if (rc < 0) {
      DEBUG_CONF   yLOG_exitr   (__FUNCTION__, rce);
      return rce;
   }
   /*---(unique)-------------------------*/
   rc = ySORT_hook (B_UNIQUE , a_new, a_new->w_unique , &(a_new->ysort_u));
   DEBUG_CONF   yLOG_value   ("hook uniq" , rc);
   --rce;  if (rc < 0) {
      DEBUG_CONF   yLOG_exitr   (__FUNCTION__, rce);
      return rce;
   }
   rc = ySORT_prepare (B_UNIQUE);
   DEBUG_CONF   yLOG_value   ("prep uniq" , rc);
   --rce;  if (rc < 0) {
      DEBUG_CONF   yLOG_exitr   (__FUNCTION__, rce);
      return rce;
   }
   /*---(complete)-----------------------*/
   DEBUG_CONF   yLOG_exit    (__FUNCTION__);
   return 0;
}

char
WORDS_unhook            (tWORD *a_old)
{
   /*---(locals)-----------+-----+-----+-*/
   char        rce         =  -10;
   char        rc          =    0;
   /*---(header)-------------------------*/
   DEBUG_CONF   yLOG_enter   (__FUNCTION__);
   /*---(english)------------------------*/
   rc = ySORT_unhook (&(a_old->ysort_e));
   DEBUG_CONF   yLOG_value   ("un-eng"    , rc);
   --rce;  if (rc < 0) {
      DEBUG_CONF   yLOG_exitr   (__FUNCTION__, rce);
      return rce;
   }
   rc = ySORT_prepare (B_ENGLISH);
   DEBUG_CONF   yLOG_value   ("prep eng"  , rc);
   --rce;  if (rc < 0) {
      DEBUG_CONF   yLOG_exitr   (__FUNCTION__, rce);
      return rce;
   }
   /*---(gregg)--------------------------*/
   rc = ySORT_unhook (&(a_old->ysort_g));
   DEBUG_CONF   yLOG_value   ("un-gregg"  , rc);
   --rce;  if (rc < 0) {
      DEBUG_CONF   yLOG_exitr   (__FUNCTION__, rce);
      return rce;
   }
   rc = ySORT_prepare (B_GREGG);
   DEBUG_CONF   yLOG_value   ("prep gregg", rc);
   --rce;  if (rc < 0) {
      DEBUG_CONF   yLOG_exitr   (__FUNCTION__, rce);
      return rce;
   }
   /*---(unique)-------------------------*/
   rc = ySORT_unhook (&(a_old->ysort_u));
   DEBUG_CONF   yLOG_value   ("un-uniq"   , rc);
   --rce;  if (rc < 0) {
      DEBUG_CONF   yLOG_exitr   (__FUNCTION__, rce);
      return rce;
   }
   rc = ySORT_prepare (B_UNIQUE);
   DEBUG_CONF   yLOG_value   ("prep uniq" , rc);
   --rce;  if (rc < 0) {
      DEBUG_CONF   yLOG_exitr   (__FUNCTION__, rce);
      return rce;
   }
   /*---(tree)---------------------------*/
   rc = ySORT_unhook (&(a_old->ysort_t));
   DEBUG_CONF   yLOG_value   ("un-tree"   , rc);
   --rce;  if (rc < 0) {
      DEBUG_CONF   yLOG_exitr   (__FUNCTION__, rce);
      return rce;
   }
   rc = ySORT_prepare (B_TREE);
   DEBUG_CONF   yLOG_value   ("prep tree" , rc);
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

int  WORDS_eng_count         (void)                            { return ySORT_count     (B_ENGLISH); }
char WORDS_eng_by_name       (uchar *a_text, tWORD **r_word)   { return ySORT_by_name   (B_ENGLISH, a_text, r_word); }
char WORDS_eng_by_index      (int n, tWORD **r_word)           { return ySORT_by_index  (B_ENGLISH, n, r_word); }
char WORDS_eng_by_cursor     (char a_dir, tWORD **r_word)      { return ySORT_by_cursor (B_ENGLISH, a_dir, r_word); }

char
WORDS_eng_all           (FILE *a_file, char *f_callback (FILE *a_file, tWORD *a_curr))
{
   /*---(locals)-----------+-----+-----+-*/
   int         a           =    0;
   tWORD      *x_curr      = NULL;
   tWORD      *x_sub       = NULL;
   /*---(header)---------------------------*/
   DEBUG_CONF   yLOG_enter   (__FUNCTION__);
   WORDS_eng_by_index  (a, &x_curr);
   while (x_curr != NULL) {
      if (strcmp (x_curr->w_vary, "<") == 0) {
         DEBUG_CONF   yLOG_info    ("base"      , x_curr->w_english);
         f_callback (a_file, x_curr);
         x_sub = x_curr->w_next;
         while (x_sub != NULL) {
            DEBUG_CONF   yLOG_info    ("vary"      , x_sub->w_english);
            f_callback (a_file, x_sub);
            x_sub  = x_sub->w_next;
         }
      }
      ++a;
      WORDS_eng_by_index (a, &x_curr);
   }
   /*---(complete)-------------------------*/
   DEBUG_CONF   yLOG_exit    (__FUNCTION__);
   return 0;
}

char WORDS_by_gregg          (uchar *a_text, tWORD **r_word)   { return ySORT_by_name   (B_GREGG  , a_text, r_word); }

char
WORDS_detail            (tWORD *a_word, char a_out [LEN_FULL])
{
   char        rce         =  -10;
   char        s           [LEN_TITLE] = " ���";
   char        t           [LEN_TITLE] = " ���";
   char        u           [LEN_DESC]  = " ���";
   char        v           [LEN_TERSE] = "   �";
   int         x_ref       =    0;
   --rce;  if (a_out  == NULL)  return rce;
   strcpy (a_out, "");
   if (a_word == NULL) return 0;
   sprintf  (s, "%2d�%.20s�", strlen (a_word->w_english), a_word->w_english);
   sprintf  (t, "%2d�%.20s�", strlen (a_word->w_gregg)  , a_word->w_gregg);
   if (a_word->w_line >= 0)  sprintf (v, "%2d/%4d", a_word->w_file, a_word->w_line);
   sprintf  (u, "%2d�%.30s�", strlen (a_word->w_shown), a_word->w_shown);
   if      (a_word->w_bref >= 0)      x_ref = a_word->w_bref;
   else if (a_word->w_base != NULL)   x_ref = a_word->w_base->w_bref;
   else                               x_ref = -66;
   sprintf (a_out, "%-24.24s  %s  %3d  %-4.4s  %c %c  %c %c %-3d  %c %-4d  %-24.24s  %-34.34s  %s", s, v, x_ref, a_word->w_vary, a_word->w_part, a_word->w_sub, a_word->w_src, a_word->w_cat, a_word->w_page, a_word->w_grp, a_word->w_freq, t, u, FIX_tree_showable (a_word->w_tree, 5));
   return 0;
}

char*
WORDS_entry             (int n)
{
   tWORD      *x_word      = NULL;
   char        t           [LEN_FULL]  = "";
   WORDS_eng_by_index (n, &x_word);
   WORDS_detail (x_word, t);
   if (strcmp (t, "") == 0)  strcpy  (g_print, "n/a");
   else                      sprintf (g_print, "%-5d %s", n, t);
   return g_print;
}


/*============================--------------------============================*/
/*===----                   private data structures                    ----===*/
/*============================--------------------============================*/
static tWORD s_words [MAX_WORDS];
static int    s_nword;

/*---(gregg index)------------------------------*/
/*> tWORD *out_index [MAX_LETTERS][MAX_LETTERS][MAX_LETTERS];                         <*/

/*---(english index)----------------------------*/
tWORD *eng_index [27][27][27];

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

/*> char                                                                                              <* 
 *> WORDS_dict_source       (tWORD *a_new, uchar *a_source)                                          <* 
 *> {                                                                                                 <* 
 *>    /+---(locals)-----------+-----+-----+-+/                                                       <* 
 *>    char        rce         =  -10;                                                                <* 
 *>    uchar       t           [LEN_LABEL] = "";                                                      <* 
 *>    uchar       s           [LEN_LABEL] = "";                                                      <* 
 *>    int         x_len       =    0;                                                                <* 
 *>    uchar      *x_valid     = "abcdefghijklmnopqrstuvwxyzABCDEFGHIJKLMNOPQRSTUVWXYZ0123456789�";   <* 
 *>    /+---(header)-------------------------+/                                                       <* 
 *>    DEBUG_CONF   yLOG_enter   (__FUNCTION__);                                                      <* 
 *>    /+---(prepare)------------------------+/                                                       <* 
 *>    s_ver  = s_book = s_type = '�';                                                                <* 
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
 *>    --rce;  if (s_ver  == 0 || strchr ("ospad�", s_ver ) == NULL) {                                <* 
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
 *>    --rce;  if (s_type == 0 || strchr ("bwp�", s_type) == NULL) {                                  <* 
 *>       DEBUG_CONF   yLOG_exitr   (__FUNCTION__, rce);                                              <* 
 *>       return rce;                                                                                 <* 
 *>    }                                                                                              <* 
 *>    if (a_new != NULL)  a_new->type = s_type;                                                      <* 
 *>    /+---(complete)-----------------------+/                                                       <* 
 *>    DEBUG_CONF   yLOG_exit    (__FUNCTION__);                                                      <* 
 *>    return 0;                                                                                      <* 
 *> }                                                                                                 <*/

/*> char                                                                              <* 
 *> WORDS_dict_notes   (tWORD *a_new, uchar *a_notes)                                <* 
 *> {                                                                                 <* 
 *>    /+---(locals)-----------+-----+-----+-+/                                       <* 
 *>    char        rce         =  -10;                                                <* 
 *>    uchar       t           [LEN_LABEL] = "";                                      <* 
 *>    uchar       s           [LEN_LABEL] = "";                                      <* 
 *>    int         x_len       =    0;                                                <* 
 *>    /+---(header)-------------------------+/                                       <* 
 *>    DEBUG_CONF   yLOG_enter   (__FUNCTION__);                                      <* 
 *>    /+---(prepare)------------------------+/                                       <* 
 *>    s_diff = s_simp = s_3rd  = s_4th  = s_top  = '�';                              <* 
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
 *>    --rce;  if (s_diff  == 0 || strchr ("smhv�", s_diff ) == NULL) {               <* 
 *>       DEBUG_CONF   yLOG_exitr   (__FUNCTION__, rce);                              <* 
 *>       return rce;                                                                 <* 
 *>    }                                                                              <* 
 *>    if (a_new != NULL)  a_new->diff = s_diff;                                      <* 
 *>    /+---(simplified)---------------------+/                                       <* 
 *>    s_simp = t [2];                                                                <* 
 *>    DEBUG_CONF   yLOG_char    ("s_simp"    , s_simp);                              <* 
 *>    --rce;  if (s_simp  == 0 || strchr ("*+�-/" , s_simp ) == NULL) {              <* 
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
WORDS_drawn_show        (short a_drawn [], uchar *a_out)
{
   int         i           =    0;
   short       n           =    0;
   char        t           [LEN_TERSE] = "";
   strlcpy (a_out, "" , LEN_HUND);
   for (i = 0; i < LEN_LABEL; ++i) {
      n = a_drawn [i];
      if (n == 0)  break;
      if (i > 0)  strlcat (a_out, "�", LEN_HUND);
      if (n > 0)  strlcpy (t, g_loc [n].label, LEN_HUND);
      else {
         strlcpy (t, g_loc [-n].label, LEN_HUND);
         t [0] = 'e';
      }
      strlcat (a_out, t, LEN_HUND);
   }
   return 0;
}

/*> static struct {                                                                   <* 
 *>    char        prev        [LEN_SHORT];                                           <* 
 *>    char        next        [LEN_SHORT];                                           <* 
 *>    char        shape;                                                             <* 
 *>    float       rot;                                                               <* 
 *> } static s_loops [LEN_HUND] {                                                     <* 
 *>    {  CAT_NONE, CAT_D   , '1',  100  },                                           <* 
 *>    {  CAT_NONE, CAT_M   , '1',  100  },                                           <* 
 *>    {  CAT_NONE, CAT_K   , '1',  100  },                                           <* 
 *>    {  CAT_NONE, CAT_R   , '1',  100  },                                           <* 
 *> }                                                                                 <*/

char
WORDS_table_ae          (void)
{
   /*> /+---(locals)-----------+-----+-----+-+/                                                 <* 
    *> uchar       x_prev      =    0;                                                          <* 
    *> uchar       x_next      =    0;                                                          <* 
    *> int         x_len       =    0;                                                          <* 
    *> uchar       x_ltr       [LEN_TERSE] = "";                                                <* 
    *> ushort      n           =    0;                                                          <* 
    *> /+---(header)-------------------------+/                                                 <* 
    *> DEBUG_CONF   yLOG_enter   (__FUNCTION__);                                                <* 
    *> x_len = strlen (CAT_ALL);                                                                <* 
    *> for (x_prev = 0; x_prev < x_len; ++x_prev) {                                             <* 
    *>    for (x_next = 0; x_next < x_len; ++x_next) {                                          <* 
    *>       if (CAT_ALL [x_next] == '�')    continue;                                          <* 
    *>       sprintf (x_ltr, "a%c%c", CAT_ALL [x_prev], CAT_ALL [x_next]);                      <* 
    *>       /+> printf ("%-3.3s  " , x_ltr);                                             <+/   <* 
    *>       n = REVERSE_find_letter (x_ltr, LTRS_ALL);                                         <* 
    *>       if (n > 0) {                                                                       <* 
    *>          /+> printf ("%-3.3s  " , x_ltr);                                          <+/   <* 
    *>          strlcpy (s_ae [x_prev][x_next], x_ltr, LEN_TERSE);                              <* 
    *>       } else {                                                                           <* 
    *>          strlcpy (s_ae [x_prev][x_next], "a", LEN_TERSE);                                <* 
    *>          /+> printf ("X%-2.2s  ", x_ltr + 1);                                      <+/   <* 
    *>       }                                                                                  <* 
    *>       DEBUG_CONF   yLOG_complex ("table"     , "%2d %2d %s", x_prev, x_next, x_ltr);     <* 
    *>    }                                                                                     <* 
    *>    /+> printf ("\n");                                                              <+/   <* 
    *> }                                                                                        <*/
   /*---(complete)-----------------------*/
   DEBUG_CONF   yLOG_exit    (__FUNCTION__);
   return 0;
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

/*> char                                                                                                                      <* 
 *> WORDS_drawn_fix_OLD        (uchar *a_index, short a_drawn [])                                                             <* 
 *> {                                                                                                                         <* 
 *>    /+---(locals)-----------+-----+-----+-+/                                                                               <* 
 *>    char        rce         =  -10;                                                                                        <* 
 *>    int         i           =    0;                                                                                        <* 
 *>    uchar       x_prev      =    0;                                                                                        <* 
 *>    uchar       x_curr      =    0;                                                                                        <* 
 *>    uchar       x_next      =    0;                                                                                        <* 
 *>    uchar       x_pcat      =    0;                                                                                        <* 
 *>    uchar       x_ncat      =    0;                                                                                        <* 
 *>    uchar       x_answer    =  '-';                                                                                        <* 
 *>    uchar       x_name      [LEN_TERSE] = "";                                                                              <* 
 *>    ushort      n           =    0;                                                                                        <* 
 *>    /+---(header)-------------------------+/                                                                               <* 
 *>    DEBUG_CONF   yLOG_enter   (__FUNCTION__);                                                                              <* 
 *>    /+---(defense)------------------------+/                                                                               <* 
 *>    DEBUG_CONF   yLOG_point   ("a_index"   , a_index);                                                                     <* 
 *>    --rce;  if (a_index == NULL) {                                                                                         <* 
 *>       DEBUG_CONF   yLOG_exitr   (__FUNCTION__, rce);                                                                      <* 
 *>       return rce;                                                                                                         <* 
 *>    }                                                                                                                      <* 
 *>    DEBUG_CONF   yLOG_point   ("a_drawn"   , a_drawn);                                                                     <* 
 *>    --rce;  if (a_drawn == NULL) {                                                                                         <* 
 *>       DEBUG_CONF   yLOG_exitr   (__FUNCTION__, rce);                                                                      <* 
 *>       return rce;                                                                                                         <* 
 *>    }                                                                                                                      <* 
 *>    /+---(purge output)-------------------+/                                                                               <* 
 *>    for (i = 0; i < LEN_LABEL; ++i)  a_drawn [i] = 0;                                                                      <* 
 *>    /+---(walk index)---------------------+/                                                                               <* 
 *>    for (i = 0; i < LEN_LABEL; ++i) {                                                                                      <* 
 *>       /+---(get positions)---------------+/                                                                               <* 
 *>       if (i > 0)  x_prev = a_index [i - 1];                                                                               <* 
 *>       x_curr = a_index [i];                                                                                               <* 
 *>       x_next = a_index [i + 1];                                                                                           <* 
 *>       DEBUG_CONF   yLOG_complex ("LOOP"      , "#%2d, prev %3d, curr %3d, next %3d", i, x_prev, x_curr, x_next);          <* 
 *>       if (x_curr == 0)   break;                                                                                           <* 
 *>       /+---(adjust)----------------------+/                                                                               <* 
 *>       x_pcat   = g_loc [x_prev].lcat;                                                                                     <* 
 *>       x_ncat   = g_loc [x_next].lcat;                                                                                     <* 
 *>       if (g_loc [x_curr].lcat == CAT_A || g_loc [x_curr].lcat == CAT_E) {                                                 <* 
 *>          DEBUG_CONF   yLOG_note    ("adjusting a/e vowels");                                                              <* 
 *>          strlcpy (x_name, s_ae [x_pcat][x_ncat], LEN_TERSE);                                                              <* 
 *>          DEBUG_CONF   yLOG_complex ("table"     , "prev %2d, next %2d, %s", x_pcat, x_ncat, x_name);                      <* 
 *>          DEBUG_CONF   yLOG_complex ("p-less"    , "prev %2d, next %2d, %s", x_pcat-1, x_ncat, s_ae [x_pcat-1][x_ncat]);   <* 
 *>          DEBUG_CONF   yLOG_complex ("p-more"    , "prev %2d, next %2d, %s", x_pcat+1, x_ncat, s_ae [x_pcat+1][x_ncat]);   <* 
 *>          DEBUG_CONF   yLOG_complex ("n-less"    , "prev %2d, next %2d, %s", x_pcat, x_ncat-1, s_ae [x_pcat][x_ncat-1]);   <* 
 *>          DEBUG_CONF   yLOG_complex ("n-more"    , "prev %2d, next %2d, %s", x_pcat, x_ncat+1, s_ae [x_pcat][x_ncat+1]);   <* 
 *>          n = REVERSE_find_letter (x_name, LTRS_ALL);                                                                      <* 
 *>          a_drawn [i] = n;                                                                                                 <* 
 *>          if (g_loc [x_curr].lcat == CAT_E)   a_drawn [i] = -n;                                                            <* 
 *>          DEBUG_CONF   yLOG_complex ("update"    , "%-5.5s %4d %4d", x_name, n, a_drawn [i]);                              <* 
 *>       } else if (g_loc [x_curr].lcat == CAT_O || g_loc [x_curr].lcat == CAT_U) {                                          <* 
 *>          sprintf (x_name, "o%c", s_ou [x_pcat][x_ncat]);                                                                  <* 
 *>          if (g_loc [x_curr].lcat == CAT_U)  x_name [0] = 'u';                                                             <* 
 *>          n = REVERSE_find_letter (x_name, LTRS_ALL);                                                                      <* 
 *>          if (n <= 0) {                                                                                                    <* 
 *>             x_name [1] = '\0';                                                                                            <* 
 *>             n = REVERSE_find_letter (x_name, LTRS_ALL);                                                                   <* 
 *>          }                                                                                                                <* 
 *>          a_drawn [i] = n;                                                                                                 <* 
 *>       } else if (strlen (g_loc [x_curr].label) == 1 && strchr ("sz", g_loc [x_curr].label [0]) != NULL) {                 <* 
 *>          sprintf (x_name, "s%c", s_sz [x_pcat][x_ncat]);                                                                  <* 
 *>          n = REVERSE_find_letter (x_name, LTRS_ALL);                                                                      <* 
 *>          if (n <= 0) {                                                                                                    <* 
 *>             x_name [1] = '\0';                                                                                            <* 
 *>             n = REVERSE_find_letter (x_name, LTRS_ALL);                                                                   <* 
 *>          }                                                                                                                <* 
 *>          a_drawn [i] = n;                                                                                                 <* 
 *>       } else {                                                                                                            <* 
 *>          DEBUG_CONF   yLOG_value   ("copy"      , a_index [i]);                                                           <* 
 *>          a_drawn [i] = a_index [i];                                                                                       <* 
 *>       }                                                                                                                   <* 
 *>       /+---(done)------------------------+/                                                                               <* 
 *>    }                                                                                                                      <* 
 *>    /+---(report out)---------------------+/                                                                               <* 
 *>    for (i = 0; i < LEN_LABEL; ++i) {                                                                                      <* 
 *>       DEBUG_CONF   yLOG_complex ("a_drawn"   , "%2d %3d", i, a_drawn [i]);                                                <* 
 *>    }                                                                                                                      <* 
 *>    /+---(complete)-----------------------+/                                                                               <* 
 *>    DEBUG_CONF   yLOG_exit    (__FUNCTION__);                                                                              <* 
 *>    return 0;                                                                                                              <* 
 *> }                                                                                                                         <*/


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
      if (strncmp(s_words[i].w_english, "EOF",  MAX_LEN) == 0)  return -1;
      /*> DEBUG_CONFH   printf("look <<%s>>\n", s_words[i].gregg);                    <*/
      if (strncmp(s_words[i].w_gregg, a_outstring, MAX_LEN) != 0)  continue;
      /*> DEBUG_CONFTING  printf("%3d) <<%s>>\n", i, s_words[i].gregg);             <* 
       *> DEBUG_CONFH   printf("%3d) <<%s>>\n", i, s_words[i].gregg);             <*/
      ++count;
      if (count == 1) strlcpy (o.word, "", MAX_LEN);
      if (count >  1) strncat(o.word, ",", MAX_LEN);
      strncat(o.word, s_words[i].w_english, MAX_LEN);
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
   strlcpy (a_outstring, s_words[a_word].w_gregg, MAX_LEN);
   /*---(clear letters)------------------*/
   for (i = 0; i < 30; ++i) {
      letters[i] = 0;
   }
   /*---(get the first letter, always ">")------*/
   /*> DEBUG_CONFTING  printf("OUTSTRING = %s\n", a_outstring);                       <*/
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
      /*> DEBUG_CONFTING  printf("   letter = <<%s>>\n", x_ltr);                      <*/
      for (i = 0; i < MAX_LETTERS; ++i) {
         if (strncmp(x_ltr, g_loc[i].label, 5) != 0) continue;
         letters[count]  = i;
         break;
      }
      if (letters[count] == 0) continue;
      /*> DEBUG_CONFTING  printf("      ind = <<%d>>\n", letters[count]);             <*/
      ++count;
   }
   for (i = 0; i < count; ++i) {
      offset = 0;
      if (g_loc[letters[i]].type == 'c') { /*> offset = words_vowel(i, count) - 1;                                      <*/
         /*> offset = words_vowel(i, count);                                          <*/
         letters[i] += offset;
      }
      /*> DEBUG_CONFTING  printf("%d, ", letters[i]);                                 <*/
   }
   /*> DEBUG_CONFTING  printf("\n");                                                  <*/
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
 *>       if (i > 0 && strncmp(g_groups [i].cat, g_groups [i - 1].cat, 5) == 0) continue;             <* 
 *>       /+> printf("%s ", g_groups [i].cat);                                             <+/     <* 
 *>       if (strncmp(g_groups [i].cat, "EOF",  5) == 0)                    break;                 <* 
 *>       if (strncmp(g_loc[letter].cat, g_groups [i].cat, 5) != 0)            continue;           <* 
 *>       one = g_groups [i].gr_num;                                                               <* 
 *>       break;                                                                                <* 
 *>    }                                                                                        <* 
 *>    /+---(current letter)-----------------+/                                                 <* 
 *>    letter = letters[a_index];                                                               <* 
 *>    two    = 0;                                                                              <* 
 *>    for (i = 0; i < MAX_GROUPS; ++i) {                                                       <* 
 *>       if (i > 0 && strncmp(g_groups [i].cat, g_groups [i - 1].cat, 5) == 0) continue;             <* 
 *>       /+> printf("%s ", g_groups [i].cat);                                             <+/     <* 
 *>       if (strncmp(g_groups [i].cat, "EOF",  5) == 0)                    break;                 <* 
 *>       if (strncmp(g_loc[letter].cat, g_groups [i].cat, 5) != 0)            continue;           <* 
 *>       two = g_groups [i].gr_num;                                                               <* 
 *>       break;                                                                                <* 
 *>    }                                                                                        <* 
 *>    /+---(complete)-----------------------+/                                                 <* 
 *>    if (one ==  5 && two == 13) {                                                            <* 
 *>       /+> DEBUG_CONFTING  printf("found a C-V\n");                                    <+/   <* 
 *>       if (strncmp(g_loc[letters[a_index - 1]].label, "u", 5) != 0) {                        <* 
 *>          adjx = -4;                                                                         <* 
 *>          adjy =  4;                                                                         <* 
 *>       }                                                                                     <* 
 *>    } else if (one == 14 && two ==  6) {                                                     <* 
 *>       /+> DEBUG_CONFTING  printf("found a P-R\n");                                    <+/   <* 
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
 *>          if (i > 0 && strncmp(g_groups [i].cat, g_groups [i - 1].cat, 5) == 0) continue;                            <* 
 *>          /+> printf("%s ", g_groups [i].cat);                                             <+/                    <* 
 *>          if (strncmp(g_groups [i].cat, "EOF",  5) == 0)                    break;                                <* 
 *>          if (strncmp(g_loc[letter].cat, g_groups [i].cat, 5) != 0)            continue;                          <* 
 *>          one = g_groups [i].gr_num;                                                                              <* 
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
 *>          if (i > 0 && strncmp(g_groups [i].cat, g_groups [i - 1].cat, 5) == 0) continue;                            <* 
 *>          /+> printf("%s ", g_groups [i].cat);                                             <+/                    <* 
 *>          if (strncmp(g_groups [i].cat, "EOF",  5) == 0)                    break;                                <* 
 *>          if (strncmp(g_loc[letter].cat, g_groups [i].cat, 5) != 0)            continue;                          <* 
 *>          two = g_groups [i].gr_num;                                                                              <* 
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
   tWORD     *x_curr      = NULL;
   int         x_fore      = 0;
   int         x_back      = 0;
   /*---(initialize)---------------------*/
   strlcpy (unit_answer, "WORDS unit       : unknownn request", 100);
   /*---(core data)----------------------*/
   if        (strncmp (a_question, "parsing"   , 20)  == 0) {
      /*> sprintf  (s, "%2d[%.20s]", strlen (s_english), s_english);                  <*/
      /*> sprintf  (t, "%2d[%.20s]", strlen (s_gregg)  , s_gregg);                    <*/
      /*> snprintf (unit_answer, LEN_STR, "WORDS parsing    : %-24.24s  %-24.24s  %c  %c  %3d  %c    %c  %c  %c  %c  %c", s, t, s_ver, s_book, s_page, s_type, s_diff, s_simp, s_3rd, s_4th, s_top);   <*/
   }
   else if (strcmp(a_question, "e_count"       ) == 0) {
      /*> x_curr = e_hword; while (x_curr != NULL) { ++x_fore; x_curr = x_curr->e_next; }   <* 
       *> x_curr = e_tword; while (x_curr != NULL) { ++x_back; x_curr = x_curr->e_prev; }   <*/
      /*> snprintf (unit_answer, LEN_FULL, "WORDS e_count    : %4dn, %4df, %4db", e_nword, x_fore, x_back);   <*/
   }
   else if (strcmp(a_question, "g_count"       ) == 0) {
      /*> x_curr = g_hword; while (x_curr != NULL) { ++x_fore; x_curr = x_curr->g_next; }   <* 
       *> x_curr = g_tword; while (x_curr != NULL) { ++x_back; x_curr = x_curr->g_prev; }   <*/
      /*> snprintf (unit_answer, LEN_FULL, "WORDS g_count    : %4dn, %4df, %4db", g_nword, x_fore, x_back);   <*/
   }
   else if   (strncmp (a_question, "e_entry"   , 20)  == 0) {
      /*> x_curr = e_hword;                                                                                                                             <* 
       *> while (x_curr != NULL) {                                                                                                                      <* 
       *>    if (c == a_num)   break;                                                                                                                   <* 
       *>    x_curr = x_curr->e_next;                                                                                                                   <* 
       *>    ++c;                                                                                                                                       <* 
       *> }                                                                                                                                             <* 
       *> if (x_curr == NULL)  snprintf (unit_answer, LEN_FULL, "WORDS eng   (%2d) :  -[]                       -[]                      []", a_num);   <* 
       *> else {                                                                                                                                        <* 
       *>    sprintf  (s, "%2d[%.20s]", x_curr->e_len, x_curr->w_english);                                                                                <* 
       *>    sprintf  (t, "%2d[%.20s]", x_curr->g_len, x_curr->w_gregg);                                                                                  <* 
       *>    WORDS_drawn_show (x_curr->w_drawn, x_show);                                                                                                  <* 
       *>    sprintf  (u, "[%.20s]"   , x_show);                                                                                                        <* 
       *>    snprintf (unit_answer, LEN_STR, "WORDS eng   (%2d) : %-24.24s  %-24.24s  %s", a_num, s, t, u);                                             <* 
       *> }                                                                                                                                             <*/
   }
   else if   (strncmp (a_question, "g_entry"   , 20)  == 0) {
      /*> x_curr = g_hword;                                                                                                                             <* 
       *> while (x_curr != NULL) {                                                                                                                      <* 
       *>    if (c == a_num)   break;                                                                                                                   <* 
       *>    x_curr = x_curr->g_next;                                                                                                                   <* 
       *>    ++c;                                                                                                                                       <* 
       *> }                                                                                                                                             <* 
       *> if (x_curr == NULL)  snprintf (unit_answer, LEN_FULL, "WORDS gregg (%2d) :  -[]                       -[]                      []", a_num);   <* 
       *> else {                                                                                                                                        <* 
       *>    sprintf  (s, "%2d[%.20s]", x_curr->e_len, x_curr->w_english);                                                                                <* 
       *>    sprintf  (t, "%2d[%.20s]", x_curr->g_len, x_curr->w_gregg);                                                                                  <* 
       *>    WORDS_drawn_show (x_curr->w_drawn, x_show);                                                                                                  <* 
       *>    sprintf  (u, "[%.20s]"   , x_show);                                                                                                        <* 
       *>    snprintf (unit_answer, LEN_STR, "WORDS gregg (%2d) : %-24.24s  %-24.24s  %s", a_num, s, t, u);                                             <* 
       *> }                                                                                                                                             <*/
   }
   /*---(complete)-----------------------*/
   return unit_answer;
}




/*============================----(source-end)----============================*/
