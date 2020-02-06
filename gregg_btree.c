/*============================---(source-start)---============================*/
#include "gregg.h"



static tWORDS  *s_eroot  = NULL;
static tWORDS  *s_groot  = NULL;
static char     s_levels =    0;


static  int     s_sorts      = 0;
static  int     s_comps      = 0;
static  int     s_swaps      = 0;
static  int     s_teles      = 0;



static char     s_result =    0;
static char     s_path   [LEN_DESC]  = "-";
static char     s_depth  =    0;
static char     s_last   [LEN_LABEL] = "";
static tWORDS  *s_save   = NULL;




/*====================------------------------------------====================*/
/*===----                       program level                          ----===*/
/*====================------------------------------------====================*/
static void  o___PROGRAM_________o () { return; }

char
BTREE_init        (void)
{
   s_eroot  = NULL;
   s_groot  = NULL;
   s_levels = 0;
   s_sorts  = 0;
   s_comps  = 0;
   s_swaps  = 0;
   s_teles  = 0;
   return 0;
}



/*====================------------------------------------====================*/
/*===----                        shared helpers                        ----===*/
/*====================------------------------------------====================*/
static void  o___HELPERS_________o () { return; }



/*> 2,626,262,626                                                                     <* 
 *>             1                                                                     <* 
 *>           100                                                                     <* 
 *>        10,000                                                                     <* 
 *>     1,000,000                                                                     <* 
 *>   100,000,000                                                                     <*/

llong
BTREE_english2key       (uchar *a_word)
{
   /*---(locals)-----------+-----------+-*/
   char        rce         =  -10;
   char        i           =    0;
   int         x_len       =    0;
   llong       x_mult      =    1;
   llong       x_key       =    0;
   uchar       x_ch        =  '-';
   char        x_num       =    0;
   uchar      *x_valid     = "abcdefghijklmnopqrstuvwxyz";
   DEBUG_SORT   yLOG_enter   (__FUNCTION__);
   DEBUG_SORT   yLOG_point   ("a_word"    , a_word);
   --rce;  if (a_word == NULL) {
      DEBUG_SORT   yLOG_exitr   (__FUNCTION__, rce);
      return rce;
   }
   DEBUG_SORT   yLOG_info    ("a_word"    , a_word);
   x_len = strlen (a_word);
   DEBUG_SORT   yLOG_value   ("x_len"     , x_len);
   for (i = 0; i < 8; ++i) {
      x_mult *= 100;
      DEBUG_SORT   yLOG_llong   ("x_mult"    , x_mult);
   }
   if (x_len > 9)  x_len = 9;
   for (i = 0; i < x_len; ++i) {
      x_ch    = a_word [i];
      if (strchr (x_valid, x_ch) == NULL)  x_num = 0;
      else                                 x_num = x_ch - 'a' + 1;
      x_key  += x_num * x_mult;
      DEBUG_SORT   yLOG_char    ("x_ch"      , x_ch);
      DEBUG_SORT   yLOG_value   ("x_num"     , x_num);
      DEBUG_SORT   yLOG_llong   ("x_mult"    , x_mult);
      DEBUG_SORT   yLOG_llong   ("x_key"     , x_key);
      x_mult /= 100;
   }
   DEBUG_SORT   yLOG_exit    (__FUNCTION__);
   return x_key;
}

llong
BTREE_gregg2key         (uchar *a_word, uchar *a_index)
{
   /*---(locals)-----------+-----+-----+-*/
   char        rce         =  -10;
   uchar       t           [LEN_HUND]  = "";
   uchar      *p           = NULL;
   uchar      *q           = "·";
   char       *r           = NULL;
   int         x_len       =    0;
   uchar      *x_valid     = "abcdefghijklmnopqrstuvwxyz>´";
   int         i           =    0;
   int         n           =   -1;
   char        c           =    0;
   llong       x_mult      =    1;
   llong       x_key       =    0;
   uchar       x_index    [LEN_LABEL]  = "";
   /*---(header)-------------------------*/
   DEBUG_SORT   yLOG_enter   (__FUNCTION__);
   /*---(defense)------------------------*/
   DEBUG_SORT   yLOG_point   ("a_word"    , a_word);
   --rce;  if (a_word == NULL) {
      DEBUG_SORT   yLOG_exitr   (__FUNCTION__, rce);
      return rce;
   }
   DEBUG_SORT   yLOG_info    ("a_word"    , a_word);
   /*---(prepare)------------------------*/
   strlcpy  (t, a_word   , LEN_HUND);
   strltrim (t, ySTR_BOTH, LEN_HUND);
   for (i = 0; i < 8; ++i)   x_mult *= 100;
   for (i = 0; i < LEN_LABEL; ++i)   x_index [i] = 0;
   /*---(parse)--------------------------*/
   p = strtok_r (t, q, &r);
   if (p != NULL && strcmp (p, ">") == 0)   p = strtok_r (NULL, q, &r);
   --rce;  while (p != NULL) {
      DEBUG_SORT   yLOG_info    ("p"         , p);
      /*---(check quality)---------------*/
      x_len = strlen (p);
      for (i = 0; i < x_len; ++i) {
         if (strchr (x_valid, p [i]) == NULL) {
            DEBUG_SORT   yLOG_value   ("weird char", p [i]);
            DEBUG_SORT   yLOG_exitr   (__FUNCTION__, rce);
            return rce;
         }
      }
      /*---(find letter)-----------------*/
      n = REVERSE_find_letter (p, LTRS_NORM);
      /*> for (i = 0; i < MAX_LETTERS; ++i) {                                         <* 
       *>    if (strcmp (g_loc [i].label, "eof") == 0)  break;                        <* 
       *>    if (strcmp (g_loc [i].label, p) != 0)      continue;                     <* 
       *>    n = i;                                                                   <* 
       *> }                                                                           <*/
      DEBUG_SORT   yLOG_value   ("n"         , n);
      if (n < 0) {
         DEBUG_SORT   yLOG_exitr   (__FUNCTION__, rce);
         return rce;
      }
      /*---(add to key)------------------*/
      if (c <= 8) {
         if (n <  100)  x_key  += n * x_mult;
         DEBUG_SORT   yLOG_complex ("x_key"     , "%2d %-20.20lld %-20.20lld", c, x_mult, x_key);
         x_mult /= 100;
      }
      /*---(add to index)----------------*/
      x_index [c] = n;
      /*---(next)------------------------*/
      ++c;
      p = strtok_r (NULL, q, &r);
      /*---(done)------------------------*/
   }
   /*---(save)---------------------------*/
   if (a_index != NULL)  strlcpy (a_index, x_index, LEN_LABEL);
   /*---(complete)-----------------------*/
   DEBUG_SORT   yLOG_exit    (__FUNCTION__);
   return x_key;
}



/*====================------------------------------------====================*/
/*===----                         sort the cells                       ----===*/
/*====================------------------------------------====================*/
static void  o___SORT____________o () { return; }


char
BTREE__swap_english     (tWORDS *a_one, tWORDS *a_two)
{
   /*---(locals)-----------+-----------+-*/
   char        rce         =  -10;
   /*---(beginning)----------------------*/
   DEBUG_SORT   yLOG_senter  (__FUNCTION__);
   DEBUG_SORT   yLOG_spoint  (a_one);
   --rce;  if (a_one == NULL) {
      DEBUG_SORT   yLOG_sexitr  (__FUNCTION__, rce);
      return rce;
   }
   DEBUG_SORT   yLOG_spoint  (a_two);
   --rce;  if (a_two == NULL) {
      DEBUG_SORT   yLOG_sexitr  (__FUNCTION__, rce);
      return rce;
   }
   --rce;  if (a_one == a_two) {
      DEBUG_SORT   yLOG_snote   ("same, no action");
      DEBUG_SORT   yLOG_sexitr  (__FUNCTION__, rce);
      return rce;
   }
   /*---(cut two from list)--------------*/
   DEBUG_SORT   yLOG_snote   ("unlink");
   DEBUG_SORT   yLOG_snote   (a_two->english);
   if (a_two->e_next != NULL)   a_two->e_next->e_prev = a_two->e_prev;
   else                         e_tword               = a_two->e_prev;
   if (a_two->e_prev != NULL)   a_two->e_prev->e_next = a_two->e_next;
   else                         e_hword               = a_two->e_next;
   /*---(insert before one)--------------*/
   DEBUG_SORT   yLOG_snote   ("insert before");
   DEBUG_SORT   yLOG_snote   (a_one->english);
   if (a_one->e_prev != NULL)   a_one->e_prev->e_next = a_two;
   else                         e_hword               = a_two;
   a_two->e_prev = a_one->e_prev;
   a_two->e_next = a_one;
   a_one->e_prev = a_two;
   /*---(complete)-----------------------*/
   DEBUG_SORT   yLOG_sexit   (__FUNCTION__);
   return 0;
}

char
BTREE__swap_gregg       (tWORDS *a_one, tWORDS *a_two)
{
   /*---(locals)-----------+-----------+-*/
   char        rce         =  -10;
   /*---(beginning)----------------------*/
   DEBUG_SORT   yLOG_senter  (__FUNCTION__);
   DEBUG_SORT   yLOG_spoint  (a_one);
   --rce;  if (a_one == NULL) {
      DEBUG_SORT   yLOG_sexitr  (__FUNCTION__, rce);
      return rce;
   }
   DEBUG_SORT   yLOG_spoint  (a_two);
   --rce;  if (a_two == NULL) {
      DEBUG_SORT   yLOG_sexitr  (__FUNCTION__, rce);
      return rce;
   }
   --rce;  if (a_one == a_two) {
      DEBUG_SORT   yLOG_snote   ("same, no action");
      DEBUG_SORT   yLOG_sexitr  (__FUNCTION__, rce);
      return rce;
   }
   /*---(cut two from list)--------------*/
   DEBUG_SORT   yLOG_snote   ("unlink");
   DEBUG_SORT   yLOG_snote   (a_two->gregg);
   if (a_two->g_next != NULL)   a_two->g_next->g_prev = a_two->g_prev;
   else                         g_tword               = a_two->g_prev;
   if (a_two->g_prev != NULL)   a_two->g_prev->g_next = a_two->g_next;
   else                         g_hword               = a_two->g_next;
   /*---(insert before one)--------------*/
   DEBUG_SORT   yLOG_snote   ("insert before");
   DEBUG_SORT   yLOG_snote   (a_one->gregg);
   if (a_one->g_prev != NULL)   a_one->g_prev->g_next = a_two;
   else                         g_hword               = a_two;
   a_two->g_prev = a_one->g_prev;
   a_two->g_next = a_one;
   a_one->g_prev = a_two;
   /*---(complete)-----------------------*/
   DEBUG_SORT   yLOG_sexit   (__FUNCTION__);
   return 0;
}

#define   IF_ENG    if (a_type == 'e') 
#define   ELSE_GRG  else

char
BTREE_dgnome            (char a_type)
{
   /*---(locals)-----------+-----+-----+-*/
   char        rce         =  -10;
   char        rc          =    0;
   int         x_match     =    0;
   char        x_flag      =  '-';
   tWORDS     *x_head      = NULL;          /* current position               */
   tWORDS     *x_base      = NULL;          /* current position               */
   tWORDS     *x_tele      = NULL;          /* teleport point                 */
   tWORDS     *x_comp      = NULL;          /* comparison item                */
   char        x_swap      =  '-';
   /*---(header)-------------------------*/
   DEBUG_SORT   yLOG_enter   (__FUNCTION__);
   /*---(check list)---------------------*/
   IF_ENG    x_head = e_hword;
   ELSE_GRG  x_head = g_hword;
   DEBUG_SORT   yLOG_point   ("x_head"      , x_head);
   --rce;  if (x_head == NULL) {
      DEBUG_SORT   yLOG_exitr   (__FUNCTION__, rce);
      return 0;
   }
   ++s_sorts;
   /*---(prepare)------------------------*/
   s_swaps = 0;
   s_comps = 0;
   s_teles = 0;
   IF_ENG    x_base  = e_hword->e_next;
   ELSE_GRG  x_base  = g_hword->g_next;
   if (x_base != NULL) {
      IF_ENG    { x_comp = x_base->e_prev;  x_tele = x_base->e_next; }
      ELSE_GRG  { x_comp = x_base->g_prev;  x_tele = x_base->g_next; }
   }
   /*---(do the gnome walk)--------------*/
   while (x_base != NULL) {
      DEBUG_SORT   yLOG_complex ("pointers"  , "base %-10.10p, comp %-10.10p, tele %-10.10p", x_base, x_comp, x_tele);
      /*---(beginning)-------------------*/
      if (x_comp == NULL) {
         DEBUG_SORT   yLOG_note    ("bounce off beginning");
         if (x_swap == 'y') {
            ++s_swaps;
            IF_ENG    BTREE__swap_english (e_hword, x_base);
            ELSE_GRG  BTREE__swap_gregg   (g_hword, x_base);
            x_swap = '-';
         }
         ++s_teles;
         x_base = x_tele;
         if (x_base != NULL) {
            IF_ENG    { x_comp = x_base->e_prev;  x_tele = x_base->e_next; }
            ELSE_GRG  { x_comp = x_base->g_prev;  x_tele = x_base->g_next; }
         }
         continue;
      }
      /*---(compare)---------------------*/
      IF_ENG    { DEBUG_SORT   yLOG_complex ("compare"   , "base key %18lld (%-10.10s), comp key %18lld (%-10.10s)   %4d %c %4d   %4d %4d %4d", x_base->e_key, x_base->english, x_comp->e_key, x_comp->english, rc, x_flag, x_match, s_comps, s_teles, s_swaps); }
      ELSE_GRG  { DEBUG_SORT   yLOG_complex ("compare"   , "base key %18lld (%-10.10s), comp key %18lld (%-10.10s)   %4d %c %4d   %4d %4d %4d", x_base->g_key, x_base->gregg  , x_comp->g_key, x_comp->gregg  , rc, x_flag, x_match, s_comps, s_teles, s_swaps); }
      ++s_comps;
      IF_ENG    rc = x_comp->e_key < x_base->e_key;
      ELSE_GRG  rc = x_comp->g_key < x_base->g_key;
      if (rc == 1) {
         x_match = -1;
         x_flag  = 'y';
      } else {
         x_match =  1;
         x_flag  = '#';
      }
      if (x_match <= 0) {
         if (x_swap == 'y') {
            ++s_swaps;
            IF_ENG    BTREE__swap_english (x_comp->e_next, x_base);
            ELSE_GRG  BTREE__swap_gregg   (x_comp->g_next, x_base);
            x_swap = '-';
         }
         ++s_teles;
         x_base = x_tele;
         if (x_base != NULL) {
            IF_ENG    { x_comp = x_base->e_prev;  x_tele = x_base->e_next; }
            ELSE_GRG  { x_comp = x_base->g_prev;  x_tele = x_base->g_next; }
         }
         continue;
      }
      /*---(swap)------------------------*/
      x_swap = 'y';
      IF_ENG    x_comp  = x_comp->e_prev;
      ELSE_GRG  x_comp  = x_comp->g_prev;
      /*---(next)------------------------*/
   }
   DEBUG_SORT   yLOG_value   ("size"       , e_nword);
   DEBUG_SORT   yLOG_value   ("compares"   , s_comps);
   DEBUG_SORT   yLOG_value   ("teleports"  , s_teles);
   DEBUG_SORT   yLOG_value   ("swaps"      , s_swaps);
   /*---(complete)-----------------------*/
   DEBUG_SORT   yLOG_exit    (__FUNCTION__);
   return 0;
}



/*====================------------------------------------====================*/
/*===----                        build the btree                       ----===*/
/*====================------------------------------------====================*/
static void  o___BUILD___________o () { return; }

int 
BTREE__depth       (int a_size)
{
   int         c           =    0;
   if (a_size <= 0)  return 0;
   while (a_size > 1) {
      a_size /= 2;
      ++c;
   }
   ++c;
   return c;
}

int 
BTREE__span        (int a_levels)
{
   if (a_levels <= 0)  return 0;
   return pow (2, a_levels) - 1;
}

tWORDS*
BTREE__nextlevel   (char a_type, int a_lvl, int a_pos, int a_dist, char a_dir, tWORDS *a_word)
{
   /*---(locals)-----------+-----+-----+-*/
   tWORDS     *x_word      = NULL;
   int         x_pos       =    0;
   int         c           =    0;
   llong       x_key       =    0;
   char       *x_name      = NULL;
   /*---(pre-defense)--------------------*/
   if (a_dist == 0)   return NULL;
   /*---(header)-------------------------*/
   DEBUG_SORT   yLOG_enter   (__FUNCTION__);
   /*---(depth)--------------------------*/
   if (a_lvl + 1 > s_levels)  s_levels = a_lvl + 1;
   /*---(check new position)-------------*/
   x_word = a_word;
   switch (a_dir) {
   case 'L' :
      x_pos = a_pos - a_dist;
      if (x_pos <= 0) {
         DEBUG_SORT   yLOG_note    ("too far left, skip level");
         x_word = BTREE__nextlevel (a_type, a_lvl, a_pos, a_dist / 2, 'L', x_word);
         DEBUG_SORT   yLOG_exit    (__FUNCTION__);
         return x_word;
      }
      break;
   case 'R' :
      x_pos = a_pos + a_dist;
      if (x_pos > e_nword) {
         DEBUG_SORT   yLOG_note    ("too far right, skip level");
         x_word = BTREE__nextlevel (a_type, a_lvl, a_pos, a_dist / 2, 'R', x_word);
         DEBUG_SORT   yLOG_exit    (__FUNCTION__);
         return x_word;
      }
      break;
   default  :
      DEBUG_SORT   yLOG_note    ("a_dir not LR, skipping");
      DEBUG_SORT   yLOG_exit    (__FUNCTION__);
      return NULL;
      break;
   }
   /*---(get to new node)----------------*/
   while (c < a_dist) {
      if (a_dir == 'L') {
         IF_ENG    x_word = x_word->e_prev;
         ELSE_GRG  x_word = x_word->g_prev;
      }
      if (a_dir == 'R') {
         IF_ENG    x_word = x_word->e_next;
         ELSE_GRG  x_word = x_word->g_next;
      }
      ++c;
   }
   /*---(recurse)------------------------*/
   IF_ENG    x_key  = x_word->e_key;
   ELSE_GRG  x_key  = x_word->g_key;
   IF_ENG    x_name = x_word->english;
   ELSE_GRG  x_name = x_word->gregg;
   DEBUG_SORT   yLOG_complex ("a_btree"   , "%2d %c %4d %4d %-20.20lld %s", a_lvl, a_dir, a_dist, x_pos, x_key, x_name);
   /*---(left side)----------------------*/
   IF_ENG    x_word->e_left  = BTREE__nextlevel (a_type, a_lvl + 1, x_pos, a_dist / 2, 'L', x_word);
   ELSE_GRG  x_word->g_left  = BTREE__nextlevel (a_type, a_lvl + 1, x_pos, a_dist / 2, 'L', x_word);
   /*---(right side)---------------------*/
   IF_ENG    x_word->e_right = BTREE__nextlevel (a_type, a_lvl + 1, x_pos, a_dist / 2, 'R', x_word);
   ELSE_GRG  x_word->g_right = BTREE__nextlevel (a_type, a_lvl + 1, x_pos, a_dist / 2, 'R', x_word);
   /*---(complete)-----------------------*/
   DEBUG_SORT   yLOG_exit    (__FUNCTION__);
   return x_word;
}

char
BTREE_build             (char a_type)
{
   /*---(locals)-----------+-----+-----+-*/
   char        rce         =  -10;
   int         x_lvl       =    0;
   int         x_span      =    0;
   int         x_ctr       =    0;
   int         c           =    0;
   tWORDS     *x_root      = NULL;
   llong       x_key       =    0;
   char       *x_name      = NULL;
   /*---(header)-------------------------*/
   DEBUG_SORT   yLOG_enter   (__FUNCTION__);
   /*---(defense)------------------------*/
   IF_ENG    x_root = e_hword;
   ELSE_GRG  x_root = g_hword;
   --rce;  if (x_root == NULL) {
      DEBUG_SORT   yLOG_exitr   (__FUNCTION__, rce);
      return rce;
   }
   /*---(statistics)---------------------*/
   x_lvl  = BTREE__depth (e_nword);
   DEBUG_SORT   yLOG_value   ("est depth" , x_lvl);
   x_span = BTREE__span (x_lvl);
   DEBUG_SORT   yLOG_value   ("max span"  , x_span);
   DEBUG_SORT   yLOG_double  ("est usage" , e_nword / 2.0 + 1);
   x_ctr  = (e_nword / 2) + 1;
   /*---(find root)----------------------*/
   DEBUG_SORT   yLOG_value   ("center"    , x_ctr);
   c = 1;
   while (c < x_ctr) {
      IF_ENG    x_root = x_root->e_next;
      ELSE_GRG  x_root = x_root->g_next;
      ++c;
   }
   DEBUG_SORT   yLOG_point   ("ROOT"      , x_root);
   /*---(save root)----------------------*/
   IF_ENG    s_eroot = x_root;
   ELSE_GRG  s_groot = x_root;
   /*---(left side)----------------------*/
   IF_ENG    x_root->e_left  = BTREE__nextlevel (a_type, 1, x_ctr, x_span / 4 + 1, 'L', x_root);
   ELSE_GRG  x_root->g_left  = BTREE__nextlevel (a_type, 1, x_ctr, x_span / 4 + 1, 'L', x_root);
   /*---(root)---------------------------*/
   IF_ENG    x_key  = x_root->e_key;
   ELSE_GRG  x_key  = x_root->g_key;
   IF_ENG    x_name = x_root->english;
   ELSE_GRG  x_name = x_root->gregg;
   DEBUG_SORT   yLOG_complex ("a_btree"   , "%2d %c %4d %4d %-20.20lld %s", 0, '@', x_ctr, x_span / 4, x_key, x_name);
   /*---(right side)---------------------*/
   IF_ENG    x_root->e_right = BTREE__nextlevel (a_type, 1, x_ctr, x_span / 4 + 1, 'R', x_root);
   ELSE_GRG  x_root->g_right = BTREE__nextlevel (a_type, 1, x_ctr, x_span / 4 + 1, 'R', x_root);
   /*---(statistics)---------------------*/
   DEBUG_SORT   yLOG_value   ("real depth", s_levels);
   /*---(complete)-----------------------*/
   DEBUG_SORT   yLOG_exit    (__FUNCTION__);
   return 0;
}



/*====================------------------------------------====================*/
/*===----                        reporting out                         ----===*/
/*====================------------------------------------====================*/
static void  o___REPORT__________o () { return; }

char
BTREE__display        (char a_type, int a_lvl, tWORDS *a_word)
{
   char        x_pre       [LEN_RECD] = "";
   int         i           =    0;
   if (a_word == NULL)  return 0;
   if (a_lvl > 20)      return 0;
   for (i = 0; i < a_lvl; ++i)  strcat (x_pre, "  ");
   IF_ENG    BTREE__display      (a_type, a_lvl + 1, a_word->e_left);
   ELSE_GRG  BTREE__display      (a_type, a_lvl + 1, a_word->g_left);
   /*> IF_ENG    printf ("%s%-20.20s %lld\n", x_pre, a_word->english, a_word->e_key);   <*/
   /*> ELSE_GRG  printf ("%s%-20.20s %lld\n", x_pre, a_word->gregg  , a_word->g_key);   <*/
   IF_ENG    BTREE__display      (a_type, a_lvl + 1, a_word->e_right);
   ELSE_GRG  BTREE__display      (a_type, a_lvl + 1, a_word->g_right);
   return 0;
}

char
BTREE_list              (char a_type)
{
   /*---(locals)-----------+-----+-----+-*/
   char        rce         =    0;
   tWORDS     *x_root      = NULL;
   tWORDS     *x_curr      = NULL;
   /*---(header)-------------------------*/
   DEBUG_DATA   yLOG_enter   (__FUNCTION__);
   /*---(defense)------------------------*/
   IF_ENG    x_root = s_eroot;
   ELSE_GRG  x_root = s_groot;
   --rce;  if (x_root == NULL) {
      DEBUG_SORT   yLOG_exitr   (__FUNCTION__, rce);
      return rce;
   }
   BTREE__display      (a_type, 0, x_root);
   x_curr = x_root;
   while  (x_curr != NULL) {
      DEBUG_SORT   yLOG_complex ("a_btree"   , "%-14.14p   e=%-20.20s ek=%20.20lld l%-14.14p r%-14.14p   g=%-20.20s gk=%20.20lld l%-14.14p r%-14.14p", x_curr, x_curr->english, x_curr->e_key, x_curr->e_left, x_curr->e_right, x_curr->gregg, x_curr->g_key, x_curr->g_left, x_curr->g_right);
      x_curr = x_curr->e_next;
   }
   DEBUG_DATA   yLOG_exit    (__FUNCTION__);
   return 0;
}



/*====================------------------------------------====================*/
/*===----                        searching the btree                   ----===*/
/*====================------------------------------------====================*/
static void  o___SEARCH__________o () { return; }

char
BTREE__searchdown       (char a_type, tWORDS **a_found, tWORDS *a_word, char *a_dir, llong a_key)
{
   /*---(locals)-----------+-----+-----+-*/
   int         rc          =    0;
   llong       x_key       =    0;
   DEBUG_SORT   yLOG_complex ("args"      , "%c %-10.10p %-10.10p  %s  %-20lld", a_type, a_found, a_word, a_dir, a_key);
   /*---(defense)------------------------*/
   if (a_word == NULL)  return -1;
   /*---(prepare)------------------------*/
   ++s_depth;
   strlcat (s_path, a_dir, LEN_DESC);
   /*---(compare)------------------------*/
   IF_ENG    x_key = a_word->e_key;
   ELSE_GRG  x_key = a_word->g_key;
   if      (x_key == a_key)  rc =  0;
   else if (x_key >  a_key)  rc =  1;
   else                      rc = -1;
   DEBUG_SORT   yLOG_complex ("compare"   , "%c %s  a%-20lld  x%-20lld  %4d", a_type, a_dir, a_key, x_key, rc);
   /*---(go left)------------------------*/
   if (rc >  0) {
      IF_ENG    return BTREE__searchdown (a_type, a_found, a_word->e_left , "L", a_key);
      ELSE_GRG  return BTREE__searchdown (a_type, a_found, a_word->g_left , "L", a_key);
   }
   /*---(go right)-----------------------*/
   if (rc <  0) {
      IF_ENG    return BTREE__searchdown (a_type, a_found, a_word->e_right, "R", a_key);
      ELSE_GRG  return BTREE__searchdown (a_type, a_found, a_word->g_right, "R", a_key);
   }
   /*---(match)--------------------------*/
   *a_found = a_word;
   return 0;
}

char
BTREE__start            (char a_type, tWORDS **a_found, char *a_word)
{
   /*---(locals)-----------+-----+-----+-*/
   char        rce         =  -10;
   char        rc          =    0;
   tWORDS     *x_word      = NULL;
   llong       x_key       =   -1;
   tWORDS     *x_root      = NULL;
   /*---(header)-------------------------*/
   DEBUG_DATA   yLOG_enter   (__FUNCTION__);
   /*---(defense)------------------------*/
   IF_ENG    x_root = s_eroot;
   ELSE_GRG  x_root = s_groot;
   --rce;  if (x_root == NULL) {
      DEBUG_SORT   yLOG_exitr   (__FUNCTION__, rce);
      return rce;
   }
   /*---(prepare)------------------------*/
   s_depth = 0;
   strlcpy (s_path, "", LEN_DESC);
   if (a_found != NULL)  *a_found = NULL;
   /*---(prepare)------------------------*/
   IF_ENG    x_key   = BTREE_english2key (a_word);
   ELSE_GRG  x_key   = BTREE_gregg2key   (a_word, NULL);
   DEBUG_DATA   yLOG_llong   ("x_key"     , x_key);
   --rce;  if (x_key < 0) {
      DEBUG_DATA   yLOG_note    ("bad label");
      s_depth = 0;
      strlcpy (s_path, "-", LEN_DESC);
      s_result = -1;
      DEBUG_DATA   yLOG_exitr   (__FUNCTION__, rce);
      return rce;
   }
   /*---(search)-------------------------*/
   DEBUG_DATA   yLOG_note    ("dive into btree");
   rc = BTREE__searchdown (a_type, &x_word, x_root, "@", x_key);
   DEBUG_DATA   yLOG_value   ("max depth" , s_levels);
   DEBUG_DATA   yLOG_value   ("s_depth"   , s_depth);
   DEBUG_DATA   yLOG_info    ("s_path"    , s_path);
   /*---(check)--------------------------*/
   --rce;  if (x_word == NULL) {
      DEBUG_DATA   yLOG_note    ("not found");
      s_result = -1;
      DEBUG_DATA   yLOG_exitr   (__FUNCTION__, rce);
      return rce;
   }
   /*---(return)-------------------------*/
   DEBUG_DATA   yLOG_note    ("found");
   if (a_found != NULL)  *a_found = x_word;
   strlcpy (s_last, a_word, LEN_LABEL);
   s_save   = x_word;
   s_result = 0;
   /*---(complete)-----------------------*/
   DEBUG_DATA   yLOG_exit    (__FUNCTION__);
   return 0;
}

char BTREE_by_english  (tWORDS **a_found, char *a_word) { return BTREE__start ('e', a_found, a_word); }
char BTREE_by_gregg    (tWORDS **a_found, char *a_word) { return BTREE__start ('g', a_found, a_word); }



/*====================------------------------------------====================*/
/*===----                      unit testing                            ----===*/
/*====================------------------------------------====================*/
static void  o___UNITTEST________o () { return; }

char*        /*-> unit test accessor -----------------[ light  [us.B60.2A3.F2]*/ /*-[01.0000.00#.#]-*/ /*-[--.---.---.--]-*/
BTREE__unit        (char *a_question, int n)
{
   /*---(locals)-------------------------*/
   tWORDS     *x_curr      = NULL;
   int         c           =    0;
   char        t           [LEN_LABEL] = "";
   /*---(parse location)-----------------*/
   strcpy  (unit_answer, "BTREE            : label could not be parsed");
   /*---(overall)------------------------*/
   if      (strcmp (a_question, "sorts"         ) == 0) {
      snprintf (unit_answer, LEN_FULL, "BTREE sorts      : %4d#, %4dl, %4dc, %4ds, %4dt", s_sorts, s_levels, s_comps, s_swaps, s_teles);
   }
   else if (strcmp (a_question, "result"        ) == 0) {
      snprintf (unit_answer, LEN_FULL, "BTREE result     : %2d %-8.8s  %2d %s", s_result, s_last, s_depth, s_path);
   }
   /*---(complete)-----------------------*/
   return unit_answer;
}


