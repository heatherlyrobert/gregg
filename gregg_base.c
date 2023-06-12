/*=================================(beg-code)=================================*/
#include "gregg.h"
#include "gregg_priv.h"

/*
 * PURPOSE     manage word bases (no prefix or suffix), i.e., one per line in the .dict files.
 *
 */



/*====================------------------------------------====================*/
/*===----                        program level                         ----===*/
/*====================------------------------------------====================*/
static void  o___PROGRAM_________o () { return; }

char
BASE_init               (void)
{
   /*---(locals)-----------+-----+-----+-*/
   char        rce         =  -10;
   char        rc          =    0;
   /*---(header)-------------------------*/
   DEBUG_CONF   yLOG_enter   (__FUNCTION__);
   /*---(initialize)---------------------*/
   rc = ySORT_btree (B_BASE, "base");
   DEBUG_CONF   yLOG_value   ("base"   , rc);
   --rce;  if (rc < 0) {
      DEBUG_CONF   yLOG_exitr   (__FUNCTION__, rce);
      return rce;
   }
   /*---(complete)-----------------------*/
   DEBUG_CONF   yLOG_exit    (__FUNCTION__);
   return 0;
}

char
BASE__purge             (void)
{
   /*---(locals)-----------+-----+-----+-*/
   char        rce         =  -10;
   char        rc          =    0;
   tBASE      *x_curr      =    0;
   /*---(header)-------------------------*/
   DEBUG_CONF   yLOG_enter   (__FUNCTION__);
   /*---(purging)------------------------*/
   BASE__by_cursor (YDLST_TAIL, &x_curr);
   while (x_curr != NULL) {
      DEBUG_CONF   yLOG_info    ("english"   , x_curr->b_english);
      BASE__unhook (x_curr);
      BASE__free   (&x_curr);
      BASE__by_cursor (YDLST_TAIL, &x_curr);
   }
   /*---(complete)-----------------------*/
   DEBUG_CONF   yLOG_exit    (__FUNCTION__);
   return 0;
}

char
BASE_wrap               (void)
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
BASE__wipe              (void *a_base)
{
   /*---(locals)-----------+-----+-----+-*/
   int         i           =    0;
   tBASE      *x_base      = NULL;
   /*---(cast)---------------------------*/
   x_base = (tBASE *) a_base;
   /*---(header)-------------------------*/
   x_base->b_english [0] = '\0';
   x_base->b_gregg   [0] = '\0';
   /*---(updates)--------------*/
   for (i = 0; i < LEN_HUND;  ++i)  x_base->b_shown  [i] = '\0';
   for (i = 0; i < LEN_LABEL; ++i)  x_base->b_drawn  [i] =  0;
   for (i = 0; i < LEN_TERSE; ++i)  x_base->b_tree   [i] =  0;
   x_base->b_sort    [0] = '\0';
   /*---(parsing)--------------*/
   x_base->b_file    = -1;
   x_base->b_line    = -1; 
   /*---(variations)-----------*/
   x_base->b_head    = NULL;
   x_base->b_tail    = NULL;
   x_base->b_ndict   = 0;
   x_base->b_ref     = 0;
   /*---(part-of-speech)-------*/
   x_base->b_part    = '·';
   x_base->b_sub     = '·';
   /*---(source)---------------*/
   x_base->b_src     = '·';
   x_base->b_cat     = '·';
   x_base->b_page    = 0;
   /*---(frequency)------------*/
   x_base->b_grp     = '·';
   x_base->b_freq    = 0;
   /*---(ysort)----------------*/
   x_base->b_ysort   = NULL;
   /*---(complete)-------------*/
   return 1;
}

char  BASE__new            (void **r_new)  { return SHARED_new  (B_BASE, '-', sizeof (tBASE), BASE__wipe, r_new); }
char  BASE__force          (void **r_new)  { return SHARED_new  (B_BASE, 'y', sizeof (tBASE), BASE__wipe, r_new); }
char  BASE__free           (void **r_old)  { return SHARED_free (B_BASE, r_old); }



/*====================------------------------------------====================*/
/*===----                    hooking and unhooking                     ----===*/
/*====================------------------------------------====================*/
static void  o___HOOKING_________o () { return; }

char
BASE__hook              (void *a_new)
{
   /*---(locals)-----------+-----+-----+-*/
   char        rce         =  -10;
   char        rc          =    0;
   tBASE      *x_new       = NULL;
   /*---(header)-------------------------*/
   DEBUG_CONF   yLOG_enter   (__FUNCTION__);
   /*---(cast)---------------------------*/
   x_new  = (tBASE *) a_new;
   /*---(base by english)----------------*/
   rc = ySORT_hook (B_BASE, x_new, x_new->b_sort, &(x_new->b_ysort));
   DEBUG_CONF   yLOG_value   ("hook"      , rc);
   --rce;  if (rc < 0) {
      DEBUG_CONF   yLOG_exitr   (__FUNCTION__, rce);
      return rce;
   }
   rc = ySORT_prepare (B_BASE);
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
BASE__unhook            (void *a_old)
{
   /*---(locals)-----------+-----+-----+-*/
   char        rce         =  -10;
   char        rc          =    0;
   tBASE      *x_old       = NULL;
   /*---(header)-------------------------*/
   DEBUG_CONF   yLOG_enter   (__FUNCTION__);
   /*---(cast)---------------------------*/
   x_old  = (tBASE *) a_old;
   /*---(english)------------------------*/
   rc = ySORT_unhook (&(x_old->b_ysort));
   DEBUG_CONF   yLOG_value   ("un-base"   , rc);
   --rce;  if (rc < 0) {
      DEBUG_CONF   yLOG_exitr   (__FUNCTION__, rce);
      return rce;
   }
   rc = ySORT_prepare (B_BASE);
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

short  BASE__count         (void)                            { return ySORT_count     (B_BASE); }
char   BASE__by_index      (int n, void **r_base)            { return ySORT_by_index  (B_BASE, n, r_base); }
char   BASE__by_cursor     (char a_dir, void **r_base)       { return ySORT_by_cursor (B_BASE, a_dir, r_base); }



/*====================------------------------------------====================*/
/*===----                      driver functions                        ----===*/
/*====================------------------------------------====================*/
static void  o___DRIVER__________o () { return; }

char
BASE__prepare           (short a_line, cchar a_english [LEN_TITLE], cchar a_gregg [LEN_TITLE])
{
   /*---(locals)-----------+-----------+-*/
   char        rce         =  -10;
   char        rc          =  -10;
   /*---(begin)--------------------------*/
   DEBUG_CONF   yLOG_enter   (__FUNCTION__);
   /*---(defense)------------------------*/
   DEBUG_CONF   yLOG_point   ("a_english" , a_english);
   --rce;  if (a_english == NULL) {
      yURG_err ('w', "%d base english word is null", a_line);
      DEBUG_CONF   yLOG_exitr   (__FUNCTION__, rce);
      return rce;
   }
   DEBUG_CONF   yLOG_info    ("a_english" , a_english);
   --rce;  if (a_english [0] == '\0') {
      yURG_err ('w', "%d base english word is empty", a_line);
      DEBUG_CONF   yLOG_exitr   (__FUNCTION__, rce);
      return rce;
   }
   DEBUG_CONF   yLOG_point   ("a_gregg"   , a_gregg);
   --rce;  if (a_gregg   == NULL) {
      yURG_err ('w', "%d base gregg outline is null", a_line);
      DEBUG_CONF   yLOG_exitr   (__FUNCTION__, rce);
      return rce;
   }
   DEBUG_CONF   yLOG_info    ("a_gregg"   , a_gregg);
   --rce;  if (a_gregg   [0] == '\0') {
      yURG_err ('w', "%d base gregg outline is empty", a_line);
      DEBUG_CONF   yLOG_exitr   (__FUNCTION__, rce);
      return rce;
   }
   /*---(complete)-----------------------*/
   DEBUG_CONF   yLOG_exit    (__FUNCTION__);
   return 0;
}

char
BASE__populate          (void *a_new, char a_file, short a_line, char a_english [LEN_TITLE], char a_gregg [LEN_TITLE])
{
   /*---(locals)-----------+-----------+-*/
   char        rce         =  -10;
   char        rc          =    0;
   tBASE      *x_new       = NULL;
   /*---(header)-------------------------*/
   DEBUG_CONF   yLOG_enter   (__FUNCTION__);
   /*---(cast)---------------------------*/
   x_new  = (tBASE *) a_new;
   /*---(header)-------------------------*/
   DEBUG_CONF   yLOG_note    ("add all data");
   x_new->b_file = a_file;
   x_new->b_line = a_line;
   strlcpy (x_new->b_english, a_english, LEN_TITLE);
   strlcpy (x_new->b_gregg  , a_gregg  , LEN_TITLE);
   /*---(critical gregg)-----------------*/
   rc = FIX_gregg      (x_new->b_gregg, x_new->b_shown, x_new->b_drawn, x_new->b_tree, x_new->b_sort);
   DEBUG_CONF   yLOG_value   ("fixing"    , rc);
   --rce;  if (rc < 0) {
      DEBUG_CONF   yLOG_exitr   (__FUNCTION__, rce);
      return rce;
   }
   /*---(complete)-----------------------*/
   DEBUG_CONF   yLOG_exit    (__FUNCTION__);
   return 0;
}

char
BASE_create             (char a_file, short a_line, cchar a_english [LEN_TITLE], cchar a_gregg [LEN_TITLE], cchar a_cats [LEN_TITLE], void **r_base)
{
   /*---(locals)-----------+-----------+-*/
   char        rce         =  -10;
   char        rc          =    0;
   tBASE      *x_new       = NULL;
   /*---(begin)--------------------------*/
   DEBUG_CONF   yLOG_enter   (__FUNCTION__);
   /*---(pre-check)----------------------*/
   rc = BASE__prepare  (a_line, a_english, a_gregg);
   DEBUG_CONF   yLOG_value   ("prepare"   , rc);
   --rce;  if (rc < 0) {
      DEBUG_CONF   yLOG_exitr   (__FUNCTION__, rce);
      return rce;
   }
   /*---(new base)-----------------------*/
   rc = BASE__new      (&x_new);
   DEBUG_CONF   yLOG_value   ("new"       , rc);
   --rce;  if (rc < 0) {
      DEBUG_CONF   yLOG_exitr   (__FUNCTION__, rce);
      return rce;
   }
   /*---(key data)-----------------------*/
   rc = BASE__populate (x_new, a_file, a_line, a_english, a_gregg);
   DEBUG_CONF   yLOG_value   ("populate"  , rc);
   --rce;  if (rc < 0) {
      BASE__free     (&x_new);
      DEBUG_CONF   yLOG_exitr   (__FUNCTION__, rce);
      return rce;
   }
   /*---(sort)---------------------------*/
   rc = BASE__hook     (x_new);
   DEBUG_CONF   yLOG_value   ("hook"      , rc);
   --rce;  if (rc < 0) {
      BASE__free     (&x_new);
      DEBUG_CONF   yLOG_exitr   (__FUNCTION__, rce);
      return rce;
   }
   /*---(categorization)-----------------*/
   rc = CATS_base      (a_cats, &(x_new->b_part), &(x_new->b_sub), &(x_new->b_src), &(x_new->b_cat), &(x_new->b_page), &(x_new->b_grp), &(x_new->b_freq));
   DEBUG_CONF   yLOG_value   ("cats"      , rc);
   /*---(save-back)----------------------*/
   if (r_base != NULL)  *r_base = x_new;
   /*---(complete)-----------------------*/
   DEBUG_CONF   yLOG_exit    (__FUNCTION__);
   return 0;
}

char
BASE_add_dict           (void *a_base, void *a_dict)
{
   /*---(locals)-----------+-----------+-*/
   char        rce         =  -10;
   char        rc          =  -10;
   tBASE      *x_base      = NULL;
   tDICT      *x_dict      = NULL;
   /*---(begin)--------------------------*/
   DEBUG_CONF   yLOG_enter   (__FUNCTION__);
   /*---(defense)------------------------*/
   DEBUG_CONF   yLOG_point   ("a_base"    , a_base);
   --rce;  if (a_base    == NULL) {
      DEBUG_CONF   yLOG_exitr   (__FUNCTION__, rce);
      return rce;
   }
   DEBUG_CONF   yLOG_point   ("a_dict"    , a_dict);
   --rce;  if (a_dict    == NULL) {
      DEBUG_CONF   yLOG_exitr   (__FUNCTION__, rce);
      return rce;
   }
   /*---(prepare)------------------------*/
   x_base = (tBASE *) a_base;
   x_dict = (tDICT *) a_dict;
   /*---(set/ground)---------------------*/
   x_dict->d_base = x_base;
   x_dict->d_prev = NULL;
   x_dict->d_next = NULL;
   /*---(check for first)----------------*/
   if (x_base->b_head == NULL) {
      x_base->b_head  = x_dict;
      x_base->b_tail  = x_dict;
      x_base->b_ndict = 1;
   }
   /*---(handle append)------------------*/
   else {
      x_dict->d_prev         = x_base->b_tail;
      x_base->b_tail->d_next = x_dict;
      x_base->b_tail         = x_dict;
      ++(x_base->b_ndict);
   }
   /*---(complete)-----------------------*/
   DEBUG_CONF   yLOG_exit    (__FUNCTION__);
   return 0;
}



/*====================------------------------------------====================*/
/*===----                      debugging support                       ----===*/
/*====================------------------------------------====================*/
static void  o___DEBUG___________o () { return; }

char
BASE__detail            (void *a_base, char a_out [LEN_FULL])
{
   /*---(locals)-----------+-----+-----+-*/
   char        rce         =  -10;
   tBASE      *x_base      = NULL;
   char        s           [LEN_TITLE] = " ·åæ";
   char        t           [LEN_TITLE] = " ·åæ";
   char        u           [LEN_DESC]  = " ·åæ";
   char        v           [LEN_TERSE] = "   ·";
   /*---(defense)------------------------*/
   --rce;  if (a_out  == NULL)  return rce;
   strcpy (a_out, "");
   if (a_base == NULL)          return 0;
   /*---(cast)---------------------------*/
   x_base  = (tBASE *) a_base;
   /*---(prepare)------------------------*/
   sprintf  (s, "%2då%.20sæ", strlen (x_base->b_english), x_base->b_english);
   sprintf  (t, "%2då%.20sæ", strlen (x_base->b_gregg)  , x_base->b_gregg);
   if (x_base->b_line >= 0)  sprintf (v, "%2d/%3d", x_base->b_file, x_base->b_line);
   sprintf  (u, "%2då%.30sæ", strlen (x_base->b_shown), x_base->b_shown);
   /*---(consolidate)--------------------*/
   sprintf (a_out, "%-24.24s  %s  %-3d  %c %c  %c %c %-3d  %c %-4d  %-24.24s  %-34.34s  %s", s, v, x_base->b_ndict, x_base->b_part, x_base->b_sub, x_base->b_src, x_base->b_cat, x_base->b_page, x_base->b_grp, x_base->b_freq, t, u, x_base->b_sort);
   /*---(complete)-----------------------*/
   return 0;
}

char*
BASE__pointer           (short n, void *a_base)
{
   char        t           [LEN_FULL]  = "";
   BASE__detail (a_base, t);
   if (strcmp (g_print, "") == 0)  strcpy  (g_print, "n/a");
   else                            sprintf (g_print, "%-5d %s", n, t);
   return g_print;
}

char*
BASE__entry             (int n)
{
   tBASE      *x_base      = NULL;
   char        t           [LEN_FULL]  = "";
   static int  x_prev      =    0;
   if (n == YDLST_INC)  n = x_prev + 1;
   BASE__by_index (n, &x_base);
   BASE__detail (x_base, t);
   if (strcmp (t, "") == 0)  strcpy  (g_print, "n/a");
   else                      sprintf (g_print, "%-5d %s", n, t);
   x_prev = n;
   return g_print;
}



/*====================------------------------------------====================*/
/*===----                       reporting                              ----===*/
/*====================------------------------------------====================*/
static void  o___REPORTING_______o () { return; }

char
BASE_dump_tree          (FILE *f)
{
   /*> /+---(locals)-----------+-----+-----+-+/                                                                <* 
    *> int         x_pos       =    0;                                                                         <* 
    *> int         x_gregg     =    0;                                                                         <* 
    *> int         i           =    0;                                                                         <* 
    *> int         a           =    0;                                                                         <* 
    *> int         c           =    0;                                                                         <* 
    *> char        r           [LEN_SHORT] = "";                                                               <* 
    *> char        s           [LEN_SHORT] = "";                                                               <* 
    *> char        t           [LEN_SHORT] = "";                                                               <* 
    *> char        u           [LEN_SHORT] = "";                                                               <* 
    *> char        v           [LEN_SHORT] = "";                                                               <* 
    *> char        x_col       =    0;                                                                         <* 
    *> tWORD      *x_curr      = NULL;                                                                         <* 
    *> /+---(header)-------------------------+/                                                                <* 
    *> DEBUG_CONF   yLOG_enter   (__FUNCTION__);                                                               <* 
    *> WORDS_eng_by_index  (a, &x_curr);                                                                       <* 
    *> while (x_curr != NULL) {                                                                                <* 
    *>    if (strcmp (x_curr->w_vary, "<") != 0) {                                                             <* 
    *>       WORDS_eng_by_index (++a, &x_curr);                                                                <* 
    *>       continue;                                                                                         <* 
    *>    }                                                                                                    <* 
    *>    if (x_curr->w_tree [0] > 0)  sprintf (r, "%02d" , x_curr->w_tree [0]);                               <* 
    *>    else                         strcpy  (r, "  ");                                                      <* 
    *>    if (x_curr->w_tree [1] > 0)  sprintf (s, "·%02d", x_curr->w_tree [1]);                               <* 
    *>    else                         strcpy  (s, "   ");                                                     <* 
    *>    if (x_curr->w_tree [2] > 0)  sprintf (t, "·%02d", x_curr->w_tree [2]);                               <* 
    *>    else                         strcpy  (t, "   ");                                                     <* 
    *>    if (x_curr->w_tree [3] > 0)  sprintf (u, "·%02d", x_curr->w_tree [3]);                               <* 
    *>    else                         strcpy  (u, "   ");                                                     <* 
    *>    if (x_curr->w_tree [4] > 0)  sprintf (v, "·%02d", x_curr->w_tree [4]);                               <* 
    *>    else                         strcpy  (v, "   ");                                                     <* 
    *>    fprintf (f, "%2s%2s%2s%2s%2s  %-20.20s  %s\n", r, s, t, u, v, x_curr->w_gregg, x_curr->w_english);   <* 
    *>    WORDS_eng_by_index (++a, &x_curr);                                                                   <* 
    *> }                                                                                                       <* 
    *> /+---(complete)-----------------------+/                                                                <* 
    *> DEBUG_CONF   yLOG_exit    (__FUNCTION__);                                                               <* 
    *> return 0;                                                                                               <*/
}



/*====================-----------------==-----------------====================*/
/*===----                      database handling                       ----===*/
/*====================-----------------==-----------------====================*/
static void o___DATABASE__________________o (void) {;}

static  short    s_count = 0;
static  tBASE  **s_index = NULL;

char
BASE_index_new          (short a_count)
{
   /*---(locals)-----------+-----+-----+-*/
   char        rce         =  -10;
   char        x_tries     =    0;
   /*---(header)-------------------------*/
   DEBUG_FILE   yLOG_enter   (__FUNCTION__);
   /*---(defense)------------------------*/
   DEBUG_FILE  yLOG_value   ("a_count"   , a_count);
   --rce;  if (a_count <= 0) {
      DEBUG_FILE  yLOG_exitr   (__FUNCTION__, rce);
      return rce;
   }
   DEBUG_FILE  yLOG_point   ("s_index"   , s_index);
   --rce;  if (s_index != NULL) {
      DEBUG_FILE  yLOG_exitr   (__FUNCTION__, rce);
      return rce;
   }
   /*---(create index)-------------------*/
   while (s_index == NULL) {
      ++x_tries;
      s_index = (tBASE *) malloc (sizeof (*s_index) * a_count);
      if (x_tries > 3)   break;
   }
   DEBUG_FILE   yLOG_sint    (x_tries);
   DEBUG_FILE   yLOG_spoint  (s_index);
   --rce;  if (s_index == NULL) {
      DEBUG_FILE   yLOG_sexitr  (__FUNCTION__, rce);
      return rce;
   }
   /*---(save)---------------------------*/
   s_count = a_count;
   /*---(complete)-----------------------*/
   DEBUG_FILE   yLOG_exit    (__FUNCTION__);
   return 0;
}

char
BASE_index_add          (short n, void *a_base)
{
   /*---(locals)-----------+-----+-----+-*/
   char        rce         =  -10;
   tBASE      *x_base      = NULL;
   /*---(header)-------------------------*/
   DEBUG_FILE   yLOG_enter   (__FUNCTION__);
   /*---(defense)------------------------*/
   DEBUG_FILE  yLOG_value   ("n"         , n);
   --rce;  if (n < 0 || n >= s_count) {
      DEBUG_FILE  yLOG_exitr   (__FUNCTION__, rce);
      return rce;
   }
   DEBUG_FILE  yLOG_point   ("s_index"   , s_index);
   --rce;  if (s_index == NULL) {
      DEBUG_FILE  yLOG_exitr   (__FUNCTION__, rce);
      return rce;
   }
   /*---(save)---------------------------*/
   x_base = (tBASE *) a_base;
   s_index [n] = x_base;
   x_base->b_ref = n;
   /*---(complete)-----------------------*/
   DEBUG_FILE   yLOG_exit    (__FUNCTION__);
   return 0;
}

void*
BASE_index_get          (short n)
{
   /*---(locals)-----------+-----+-----+-*/
   char        rce         =  -10;
   /*---(header)-------------------------*/
   DEBUG_FILE   yLOG_enter   (__FUNCTION__);
   /*---(defense)------------------------*/
   DEBUG_FILE  yLOG_value   ("n"         , n);
   --rce;  if (n < 0 || n >= s_count) {
      DEBUG_FILE  yLOG_exitr   (__FUNCTION__, rce);
      return NULL;
   }
   DEBUG_FILE  yLOG_point   ("s_index"   , s_index);
   --rce;  if (s_index == NULL) {
      DEBUG_FILE  yLOG_exitr   (__FUNCTION__, rce);
      return NULL;
   }
   /*---(complete)-----------------------*/
   DEBUG_FILE   yLOG_exit    (__FUNCTION__);
   return s_index [n];
}

char
BASE_index_free         (void)
{
   /*---(locals)-----------+-----+-----+-*/
   char        rce         =  -10;
   /*---(header)-------------------------*/
   DEBUG_FILE   yLOG_enter   (__FUNCTION__);
   /*---(defense)------------------------*/
   DEBUG_FILE  yLOG_point   ("s_index"   , s_index);
   --rce;  if (s_index == NULL) {
      DEBUG_FILE  yLOG_exitr   (__FUNCTION__, rce);
      return rce;
   }
   /*---(free and ground)----------------*/
   free (s_index);
   s_index = NULL;
   s_count = 0;
   /*---(complete)-----------------------*/
   DEBUG_FILE   yLOG_exit    (__FUNCTION__);
   return 0;
}

char
BASE_write              (FILE *a_file)
{
   /*---(locals)-----------+-----+-----+-*/
   char        rce         =  -10;
   char        rc          =    0;
   char        x_count     =    0;
   tBASE      *x_base      = NULL;
   short       c           =    0;
   /*---(header)-------------------------*/
   DEBUG_OUTP   yLOG_enter   (__FUNCTION__);
   /*---(defense)------------------------*/
   DEBUG_OUTP  yLOG_point   ("a_file"    , a_file);
   --rce;  if (a_file == NULL) {
      DEBUG_OUTP  yLOG_exitr   (__FUNCTION__, rce);
      return rce;
   }
   x_count = BASE__count ();
   DEBUG_OUTP  yLOG_value   ("x_count"   , x_count);
   --rce;  if (x_count <= 0) {
      DEBUG_OUTP  yLOG_exitr   (__FUNCTION__, rce);
      return rce;
   }
   /*---(write all)----------------------*/
   BASE__by_cursor (YDLST_HEAD, &x_base);
   while (x_base != NULL) {
      DEBUG_OUTP  yLOG_info    ("english"   , x_base->b_english);
      rc = fwrite (x_base, sizeof (tBASE), 1, a_file);
      if (rc != 1)   break;
      BASE_index_add  (c, x_base);
      BASE__by_cursor (YDLST_NEXT, &x_base);
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
BASE_read               (FILE *a_file, short a_count)
{
   /*---(locals)-----------+-----+-----+-*/
   char        rce         =  -10;
   char        rc          =    0;
   char        x_count     =    0;
   short       i           =    0;
   tBASE      *x_base      = NULL;
   /*---(header)-------------------------*/
   DEBUG_OUTP   yLOG_enter   (__FUNCTION__);
   /*---(defense)------------------------*/
   DEBUG_OUTP  yLOG_point   ("a_file"    , a_file);
   --rce;  if (a_file == NULL) {
      DEBUG_OUTP  yLOG_exitr   (__FUNCTION__, rce);
      return rce;
   }
   x_count = BASE__count ();
   DEBUG_OUTP  yLOG_value   ("x_count"   , x_count);
   --rce;  if (x_count >  0) {
      DEBUG_OUTP  yLOG_exitr   (__FUNCTION__, rce);
      return rce;
   }
   /*---(write all)----------------------*/
   for (i = 0; i < a_count; ++i) {
      BASE__force (&x_base);
      rc = fread  (x_base, sizeof (tBASE), 1, a_file);
      DEBUG_OUTP  yLOG_info    ("english"   , x_base->b_english);
      if (rc != 1)   break;
      BASE_index_add (i, x_base);
      BASE__hook     (x_base);
      x_base->b_head  = 0;
      x_base->b_tail  = 0;
      x_base->b_ndict = 0;
   }
   /*---(check)--------------------------*/
   x_count = BASE__count ();
   DEBUG_OUTP  yLOG_value   ("x_count"   , x_count);
   --rce;  if (i != x_count) {
      DEBUG_OUTP  yLOG_exitr   (__FUNCTION__, rce);
      return rce;
   }
   /*---(complete)-----------------------*/
   DEBUG_OUTP   yLOG_exit    (__FUNCTION__);
   return 0;
}





