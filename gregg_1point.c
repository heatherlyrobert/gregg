/*=================================(beg-code)=================================*/
#include "gregg.h"



const float FULL_CIRCLE  = 2 * 3.14159;   // circle in radians






/*============================--------------------============================*/
/*===----                       little helpers                         ----===*/
/*============================--------------------============================*/
void o___HELPER_________________o (void) {;}

tPOINT*
POINT__series           (char a_type, char *a_name, int *a_count, int *a_max)
{
   /*---(locals)-----------+-----+-----+-*/
   tPOINT     *p           = NULL;
   int         c           =    0;
   char        x_name      [LEN_TERSE] = "";
   int         m           =    0;
   /*---(point type)---------------------*/
   switch (a_type) {
   case POINTS_RAW : p = o.raw;  c = o.nraw;  m = LEN_HUGE;    strlcpy (x_name, "RAW", LEN_TERSE);   break;
   case POINTS_BAS : p = o.bas;  c = o.nbas;  m = MAX_POINTS;  strlcpy (x_name, "BAS", LEN_TERSE);   break;
   case POINTS_AVG : p = o.avg;  c = o.navg;  m = MAX_POINTS;  strlcpy (x_name, "AVG", LEN_TERSE);   break;
   case POINTS_TMP : p = o.tmp;  c = 5;       m = 5;           strlcpy (x_name, "TMP", LEN_TERSE);   break;
   case POINTS_KEY : p = o.key;  c = o.nkey;  m = MAX_POINTS;  strlcpy (x_name, "KEY", LEN_TERSE);   break;
   default         : p = NULL;   c = 0;       m = 0;           strlcpy (x_name, ""   , LEN_TERSE);   break;
   }
   /*---(return)-------------------*/
   if (a_count != NULL)  *a_count = c;
   if (a_name  != NULL)  strlcpy (a_name, x_name, LEN_TERSE);
   if (a_max   != NULL)  *a_max   = m;
   /*---(complete)-----------------*/
   return p;
}



/*============================--------------------============================*/
/*===----                        data wiping                           ----===*/
/*============================--------------------============================*/
void o___CLEARING_______________o (void) {;}

char
POINT_clear             (tPOINT *a_point, char a_type)
{
   /*---(base)---------------------------*/
   a_point->series  = a_type;
   a_point->seq     = -1;
   a_point->type    = POINT_NONE;
   /*---(links)--------------------------*/
   a_point->p_raw   = -1;
   a_point->p_bas   = -1;
   a_point->p_key   = -1;
   /*---(markings)-----------------------*/
   a_point->fake    = '-';
   a_point->prekey  = '-';
   a_point->marked  = '-';
   /*---(touchpad)-----------------------*/
   a_point->x_touch = a_point->x_raw  = 0;
   a_point->y_touch = a_point->y_raw  = 0;
   /*---(statistics)---------------------*/
   a_point->xd      = a_point->yd     = 0;
   a_point->len     = a_point->icept  = a_point->degs   = a_point->quad   = a_point->ccat   = 0;
   a_point->slope   = a_point->rads   = a_point->range  = a_point->depth  = a_point->ratio  = 0.0;
   a_point->sharp   = '-';
   strlcpy (a_point->use, "-", LEN_TERSE);
   /*---(display)---------------*/
   a_point->x_rel   = a_point->y_rel  = 0.0;
   a_point->x_pos   = a_point->y_pos  = 0;
   /*---(complete)--------------*/
   return 0;
}

char
POINT_clear_series      (char a_type)
{
   /*---(locals)-----------+-----+-----+-*/
   char        rce         =  -10;
   int         i           =    0;
   tPOINT     *p           = NULL;
   int         x_max       =    0;
   /*---(header)-------------------------*/
   DEBUG_CALC   yLOG_senter  (__FUNCTION__);
   DEBUG_DATA   yLOG_schar    (a_type);
   /*---(point type)------------------*/
   p = POINT__series (a_type, NULL, NULL, &x_max);
   DEBUG_DATA   yLOG_spoint   (p);
   --rce;  if (p == NULL) {
      DEBUG_CALC   yLOG_sexitr  (__FUNCTION__, rce);
      return rce;
   }
   /*---(clear all)-------------------*/
   DEBUG_DATA   yLOG_snote    ("clear all data");
   for (i = 0; i < x_max; ++i) {
      POINT_clear (p + i, a_type);
   }
   /*---(complete)--------------*/
   DEBUG_CALC   yLOG_sexit   (__FUNCTION__);
   return 0;
}

char
POINT_clear_all         (void)
{
   /*---(locals)-----------+-----+-----+-*/
   int         i           =    0;
   /*---(header)-------------------------*/
   DEBUG_CALC   yLOG_enter   (__FUNCTION__);
   /*---(clear types)--------------------*/
   for (i = 0; i < strlen (POINTS_ALL); ++i) {
      POINT_clear_series (POINTS_ALL [i]);
   }
   /*---(complete)--------------*/
   DEBUG_CALC   yLOG_exit    (__FUNCTION__);
   return 0;
}

char
POINT_clear_for_more    (void)
{
   /*---(locals)-----------+-----+-----+-*/
   int         i           =    0;
   /*---(header)-------------------------*/
   DEBUG_CALC   yLOG_enter   (__FUNCTION__);
   /*---(clear raw links)----------------*/
   for (i = 0; i < LEN_HUGE; ++i) {
      /*---(links)-----------------------*/
      o.raw [i].p_bas   = -1;
      o.raw [i].p_key   = -1;
      /*---(markings)--------------------*/
      o.raw [i].prekey  = '-';
      o.raw [i].marked  = '-';
   }
   /*---(clear all others)---------------*/
   for (i = 0; i < strlen (POINTS_NOT_RAW); ++i) {
      POINT_clear_series (POINTS_NOT_RAW [i]);
   }
   /*---(complete)--------------*/
   DEBUG_CALC   yLOG_exit    (__FUNCTION__);
   return 0;
}



/*============================--------------------============================*/
/*===----                     creation/destruction                     ----===*/
/*============================--------------------============================*/
void o___CREATION_______________o (void) {;}

char
POINT__copy             (char a_type, short a_old, short a_new, char a_clear)
{
   /*---(locals)-----------+-----+-----+-*/
   short       x_raw       =    0;
   short       x_bas       =    0;
   tPOINT     *p           = NULL;
   char        x_name      [LEN_TERSE] = "";
   /*---(header)-------------------------*/
   p = POINT__series (a_type, x_name, NULL, NULL);
   /*---(header)-------------------------*/
   DEBUG_DATA    yLOG_complex ("copy"      , "%s, %3d into %3d", x_name, a_old, a_new);
   /*---(fix raw ties)-------------------*/
   switch (a_type) {
   case POINTS_BAS :
   case POINTS_AVG :
      x_raw  = o.bas [a_old].p_raw;
      o.raw [x_raw].p_bas   = a_new;
      break;
   case POINTS_KEY :
      x_bas  = o.key [a_old].p_bas;
      o.bas [x_bas].p_key   = a_new;
      o.avg [x_bas].p_key   = a_new;
      break;
   }
   /*---(series/seq)---------------------*/
   p [a_new].seq     = a_new;
   p [a_new].series  = p [a_old].series;
   p [a_new].type    = p [a_old].type;
   /*---(tie raw and bas)----------------*/
   /*> if (strchr ("R" , x_name [0]) == NULL)  p [a_new].p_raw   = p [a_old].p_raw;   <* 
    *> if (strchr ("BA", x_name [0]) == NULL)  p [a_new].p_bas   = p [a_old].p_bas;   <* 
    *> if (strchr ("K" , x_name [0]) == NULL)  p [a_new].p_key   = p [a_old].p_key;   <*/
   p [a_new].p_raw   = p [a_old].p_raw;
   p [a_new].p_bas   = p [a_old].p_bas;
   p [a_new].p_key   = p [a_old].p_key;
   /*---(characteristics)----------------*/
   p [a_new].fake    = p [a_old].fake;
   p [a_new].prekey  = p [a_old].prekey;
   p [a_new].marked  = p [a_old].marked;
   /*---(original)-----------------------*/
   p [a_new].x_touch = p [a_old].x_touch;
   p [a_new].y_touch = p [a_old].y_touch;
   p [a_new].x_raw   = p [a_old].x_raw;
   p [a_new].y_raw   = p [a_old].y_raw;
   /*---(screen)-------------------------*/
   p [a_new].x_rel   = p [a_old].x_rel;
   p [a_new].y_rel   = p [a_old].y_rel;
   p [a_new].x_pos   = p [a_old].x_pos;
   p [a_new].y_pos   = p [a_old].y_pos;
   /*---(clear old)----------------------*/
   if (a_clear == 'y')  POINT_clear (&(p [a_old]), POINT_NORMAL);
   /*---(done)---------------------------*/
   return 0;
}

char
POINT_copy              (char a_type, short a_old, short a_new)
{
   POINT__copy (a_type, a_old, a_new, '-');
   return 0;
}

char
POINT_move              (char a_type, short a_old, short a_new)
{
   POINT__copy (a_type, a_old, a_new, 'y');
   return 0;
}

char
POINT_push_up           (char a_type, short a_old)
{
   POINT__copy (a_type, a_old, a_old + 1, 'y');
   /*> ++(o.bas [a_old + 1].seq);                                                     <* 
    *> ++(o.avg [a_old + 1].seq);                                                     <*/
   return 0;
}

char
POINT_pull_down         (char a_type, short a_old)
{
   POINT__copy (a_type, a_old + 1, a_old, 'y');
   return 0;
}

char
POINT_wipe              (char a_type, short a_old)
{
   /*---(locals)-----------+-----+-----+-*/
   tPOINT     *p           = NULL;
   short       x_raw       =    0;
   short       x_bas       =    0;
   /*---(header)-------------------------*/
   p = POINT__series (a_type, NULL, NULL, NULL);
   switch (a_type) {
   case POINTS_BAS :
   case POINTS_AVG :
      x_raw  = o.bas [a_old].p_raw;
      o.raw [x_raw].p_bas   = -1;
      break;
   case POINTS_KEY :
      x_bas  = o.key [a_old].p_bas;
      o.bas [x_bas].p_key   = -1;
      o.avg [x_bas].p_key   = -1;
      break;
   }
   return POINT_clear (p + a_old, a_type);
}

char
POINT_reseq             (char a_type)
{
   /*---(locals)-----------+-----+-----+-*/
   tPOINT     *p           = NULL;
   short       i           =    0;
   char        x_name      [LEN_TERSE];
   int         c           =    0;
   int         x_max       =    0;
   int         x_seq       =    0;
   /*---(header)-------------------------*/
   DEBUG_DATA    yLOG_senter  (__FUNCTION__);
   p = POINT__series (a_type, x_name, &c, &x_max);
   DEBUG_DATA    yLOG_schar   (a_type);
   DEBUG_DATA    yLOG_snote   (x_name);
   DEBUG_DATA    yLOG_sint    (c);
   DEBUG_DATA    yLOG_sint    (x_max);
   for (i = 0; i < x_max; ++i) {
      x_seq = i;
      if (i >= c)  x_seq = -1;
      p [i].seq   = x_seq;
      switch (a_type) {
      case POINTS_RAW  :
         p [i].p_raw = x_seq;
         break;
      case POINTS_BAS  :
      case POINTS_AVG  :
         p [i].p_bas = x_seq;
         break;
      case POINTS_KEY  :
         p [i].p_key = x_seq;
         break;
      }
   }
   /*---(complete)-----------------------*/
   DEBUG_DATA    yLOG_sexit   (__FUNCTION__);
   return 0;
}

char
POINT_rekey             (void)
{
   /*---(locals)-----------+-----+-----+-*/
   short       i           =    0;
   int         c           =    0;
   short       x_raw       =    0;
   short       x_bas       =    0;
   /*---(header)-------------------------*/
   DEBUG_DATA    yLOG_senter  (__FUNCTION__);
   /*---(clear raw key links)------------*/
   for (i = 0; i < o.nraw; ++i) {
      o.raw [i].p_key = -1;
   }
   /*---(clear bas/avg key links)--------*/
   for (i = 0; i < o.nbas; ++i) {
      o.bas [i].p_key = -1;
      o.avg [i].p_key = -1;
   }
   /*---(update)-------------------------*/
   for (i = 0; i < o.nkey; ++i) {
      /*---(get pointers)------*/
      x_raw = o.key [i].p_raw;
      x_bas = o.key [i].p_bas;
      /*---(update raw)--------*/
      o.raw [x_raw].p_key = i;
      /*---(update bas/avg)----*/
      o.bas [x_bas].p_key = i;
      o.avg [x_bas].p_key = i;
      /*---(done)--------------*/
   }
   /*---(complete)-----------------------*/
   DEBUG_DATA    yLOG_sexit   (__FUNCTION__);
   return 0;
}

char
POINT_swap              (char a_type, short a_old, short a_new)
{
   short       x_tmp       =    0;
   POINT__series (a_type, NULL, NULL, &x_tmp);
   --x_tmp;
   POINT__copy (a_type, a_old, x_tmp, 'y');
   POINT__copy (a_type, a_new, a_old, 'y');
   POINT__copy (a_type, x_tmp, a_new, 'y');
   return 0;
}



/*============================--------------------============================*/
/*===----                       raw point support                      ----===*/
/*============================--------------------============================*/
void o___RAWS___________________o (void) {;}

char
POINT_raw_add           (char a_type, int x, int y)
{
   /*---(locals)-----------+-----+-----+-*/
   char        rce         =  -10;
   /*---(header)-------------------------*/
   DEBUG_DATA    yLOG_enter   (__FUNCTION__);
   /*---(defense)------------------------*/
   DEBUG_DATA    yLOG_char    ("a_type"    , a_type);
   --rce;  if (a_type == 0 || strchr (POINT_TYPES, a_type) == NULL) {
      DEBUG_DATA    yLOG_exitr   (__FUNCTION__, rce);
      return rce;
   }
   DEBUG_DATA    yLOG_value   ("x"         , x);
   DEBUG_DATA    yLOG_value   ("y"         , y);
   /*> --rce;  if (x < 0 || y < 0) {                                                  <* 
    *>    DEBUG_DATA    yLOG_exitr   (__FUNCTION__, rce);                             <* 
    *>    return rce;                                                                 <* 
    *> }                                                                              <*/
   /*---(base values)--------------------*/
   DEBUG_DATA    yLOG_complex ("args"      , "%c, %4dx, %4dy", a_type, x, y);
   o.raw [o.nraw].series  = POINTS_RAW;
   o.raw [o.nraw].seq     = o.nraw;
   o.raw [o.nraw].p_raw   = o.nraw;
   o.raw [o.nraw].x_touch = o.raw [o.nraw].x_raw = x;
   o.raw [o.nraw].y_touch = o.raw [o.nraw].y_raw = y;
   o.raw [o.nraw].type    = a_type;
   /*---(label fakes)--------------------*/
   switch (a_type) {
   case POINT_START   :
      DEBUG_DATA    yLOG_note    ("add start");
      o.raw [o.nraw].fake = POINT_FAKE;
      break;
   case POINT_HEAD    :
      DEBUG_DATA    yLOG_note    ("add head");
      o.raw [o.nraw].fake = POINT_NORMAL;
      break;
   case POINT_NORMAL  :
      DEBUG_DATA    yLOG_note    ("add normal");
      o.raw [o.nraw].fake = POINT_NORMAL;
      break;
   case POINT_TAIL    :
      DEBUG_DATA    yLOG_note    ("add tail");
      o.raw [o.nraw].fake = POINT_NORMAL;
      break;
   case POINT_FINISH  :
      DEBUG_DATA    yLOG_note    ("add finish");
      o.raw [o.nraw].fake = POINT_FAKE;
      break;
   }
   /*---(update counters)----------------*/
   ++o.nraw;
   POINT_reseq (POINTS_RAW);
   /*---(complete)-----------------------*/
   DEBUG_DATA    yLOG_exit    (__FUNCTION__);
   return 0;
}



/*============================--------------------============================*/
/*===----                      base point support                      ----===*/
/*============================--------------------============================*/
void o___BASE___________________o (void) {;}

char
POINT_raw2bas       (short a_raw, short a_bas)
{
   /*---(tie raw and bas)-----------------*/
   o.raw [a_raw].p_bas   = a_bas;
   o.bas [a_bas].p_raw   = o.avg [a_bas].p_raw   = a_raw;
   o.bas [a_bas].p_bas   = o.avg [a_bas].p_bas   = a_bas;
   o.bas [a_bas].p_key   = o.avg [a_bas].p_key   = -1;
   /*---(head)----------------------------*/
   o.bas [a_bas].seq     = o.avg [a_bas].seq     = a_bas;
   o.bas [a_bas].fake    = o.avg [a_bas].fake    = o.raw [a_raw].fake;
   o.bas [a_bas].type    = o.avg [a_bas].type    = o.raw [a_raw].type;
   /*---(touchpad)------------------------*/
   o.bas [a_bas].x_touch = o.avg [a_bas].x_touch = o.raw [a_raw].x_touch;
   o.bas [a_bas].x_raw   = o.avg [a_bas].x_raw   = o.raw [a_raw].x_raw;
   o.bas [a_bas].y_touch = o.avg [a_bas].y_touch = o.raw [a_raw].y_touch;
   o.bas [a_bas].y_raw   = o.avg [a_bas].y_raw   = o.raw [a_raw].y_raw;
   /*---(display)-------------------------*/
   o.bas [a_bas].x_pos   = o.avg [a_bas].x_pos   = o.raw [a_raw].x_pos;
   o.bas [a_bas].x_rel   = o.avg [a_bas].x_rel   = o.raw [a_raw].x_rel;
   o.bas [a_bas].y_pos   = o.avg [a_bas].y_pos   = o.raw [a_raw].y_pos;
   o.bas [a_bas].y_rel   = o.avg [a_bas].y_rel   = o.raw [a_raw].y_rel;
   /*---(complete)-----------------------*/
   return 0;
}

#define     FIND_EXACT      '='
#define     FIND_INSERT     '<'

short
POINT_bas_find          (char a_style, short a_raw)
{
   /*---(locals)-----------+-----+-----+-*/
   char        rce         =  -10;
   short       i           =    0;
   short       n           =   -1;
   /*---(header)-------------------------*/
   DEBUG_DATA    yLOG_enter   (__FUNCTION__);
   /*---(defense)------------------------*/
   DEBUG_DATA    yLOG_value   ("a_raw"     , a_raw);
   --rce;  if (a_raw < 0 || a_raw >= o.nraw) {
      DEBUG_DATA    yLOG_note    ("out of range");
      DEBUG_DATA    yLOG_exitr   (__FUNCTION__, rce);
      return rce;
   }
   /*---(find point)---------------------*/
   --rce;  for (i = 0; i < o.nbas; ++i) {
      DEBUG_DATA_M  yLOG_complex ("compare"   , "%2d, %4d to %4d", i, a_raw, o.bas [i].p_raw);
      /*---(found)-------------*/
      if (a_style == FIND_EXACT  && a_raw == o.bas [i].p_raw) {
         DEBUG_DATA    yLOG_value   ("found"     , i);
         n = i;
         break;
      }
      /*---(duplicate)---------*/
      if (a_style == FIND_INSERT && a_raw == o.bas [i].p_raw) {
         DEBUG_DATA    yLOG_note    ("duplicate, already exists");
         DEBUG_DATA    yLOG_exitr   (__FUNCTION__, rce);
         return rce;
      }
      /*---(put before)--------*/
      if (a_style == FIND_INSERT && a_raw <  o.bas [i].p_raw) {
         DEBUG_DATA    yLOG_value   ("found"     , i);
         n = i;
         break;
      }
      /*---(done)--------------*/
   }
   /*---(failed)-------------------------*/
   DEBUG_DATA    yLOG_value   ("n"         , n);
   if (n < 0) {
      /*---(not found)---------*/
      if (a_style == FIND_EXACT) {
         DEBUG_DATA    yLOG_exitr   (__FUNCTION__, rce);
         return rce;
      }
      /*---(append)------------*/
      n = MAX_POINTS + 1;
   }
   /*---(failed)-------------------------*/
   DEBUG_DATA    yLOG_exit    (__FUNCTION__);
   return n;
}

char
POINT_bas_add           (short a_raw)
{
   /*---(locals)-----------+-----+-----+-*/
   char        rce         =  -10;
   short       i           =    0;
   short       n           =   -1;
   /*---(header)-------------------------*/
   DEBUG_DATA    yLOG_enter   (__FUNCTION__);
   /*---(find)---------------------------*/
   n = POINT_bas_find (FIND_INSERT, a_raw);
   DEBUG_DATA    yLOG_value   ("n"         , n);
   /*---(duplicate)----------------------*/
   if (n < 0) {
      DEBUG_DATA    yLOG_exitr   (__FUNCTION__, rce);
      return rce;
   }
   /*---(append)-------------------------*/
   if (n >= MAX_POINTS) {
      DEBUG_DATA    yLOG_note    ("append mode");
      POINT_raw2bas (a_raw, o.nbas);
   }
   /*---(insert)-------------------------*/
   else {
      DEBUG_DATA    yLOG_note    ("insert mode");
      for (i = o.nbas - 1; i >= n; --i)   POINT_push_up (POINTS_BAS, i);
      POINT_raw2bas (a_raw, n);
   }
   /*---(increment counters)-------------*/
   ++o.nbas;
   ++o.navg;
   POINT_reseq (POINTS_BAS);
   POINT_reseq (POINTS_AVG);
   /*---(complete)-----------------------*/
   DEBUG_DATA    yLOG_exit    (__FUNCTION__);
   return 0;
}

char
POINT_bas_adjust        (short a_old, short a_new)
{
   /*---(locals)-----------+-----+-----+-*/
   char        rce         =  -10;
   short       n           =   -1;
   short       x_key       =    0;
   /*---(header)-------------------------*/
   DEBUG_DATA    yLOG_enter   (__FUNCTION__);
   /*---(defense)------------------------*/
   DEBUG_DATA    yLOG_value   ("a_new"     , a_new);
   --rce;  if (a_new < 0 || a_new >= o.nraw) {
      DEBUG_DATA    yLOG_note    ("out of range");
      DEBUG_DATA    yLOG_exitr   (__FUNCTION__, rce);
      return rce;
   }
   /*---(find)---------------------------*/
   n = POINT_bas_find (FIND_EXACT, a_old);
   /*---(not found)----------------------*/
   --rce;  if (n < 0) {
      DEBUG_DATA    yLOG_exitr   (__FUNCTION__, rce);
      return rce;
   }
   /*---(save)---------------------------*/
   x_key  = o.raw [a_old].p_key;
   /*---(clear old raw)------------------*/
   o.raw [a_old].p_bas  = -1;
   o.raw [a_old].p_key  = -1;
   /*---(update new raw)-----------------*/
   o.raw [a_new].p_bas  = n;
   o.raw [a_new].p_key  = x_key;
   /*---(update bas/avg)-----------------*/
   o.bas [n].p_raw  = o.avg [n].p_raw  = a_new;
   /*---(update key)---------------------*/
   if (x_key >= 0)   o.key [x_key].p_raw  = a_new;
   /*---(complete)-----------------------*/
   DEBUG_DATA    yLOG_exit    (__FUNCTION__);
   return 0;
}

char
POINT_bas_del           (short a_raw)
{
   /*---(locals)-----------+-----+-----+-*/
   char        rce         =  -10;
   short       i           =    0;
   short       n           =   -1;
   short       x_raw       =   -1;
   /*---(header)-------------------------*/
   DEBUG_DATA    yLOG_enter   (__FUNCTION__);
   /*---(find)---------------------------*/
   n = POINT_bas_find (FIND_EXACT, a_raw);
   /*---(not found)----------------------*/
   --rce;  if (n < 0) {
      DEBUG_DATA    yLOG_exitr   (__FUNCTION__, rce);
      return rce;
   }
   /*---(check key)----------------------*/
   --rce;  if (o.bas [n].p_key >= 0) {
      DEBUG_DATA    yLOG_exitr   (__FUNCTION__, rce);
      return rce;
   }
   /*---(remove)-------------------------*/
   x_raw  = o.bas [n].p_raw;
   o.raw [x_raw].p_bas   = -1;
   for (i = n; i < o.nbas; ++i)  {
      POINT_pull_down (POINTS_BAS, i);
      POINT_pull_down (POINTS_AVG, i);
   }
   POINT_wipe (POINTS_BAS, o.nbas - 1);
   POINT_wipe (POINTS_AVG, o.nbas - 1);
   /*---(decrement counters)-------------*/
   --o.nbas;
   --o.navg;
   POINT_reseq (POINTS_BAS);
   POINT_reseq (POINTS_AVG);
   /*---(complete)-----------------------*/
   DEBUG_DATA    yLOG_exit    (__FUNCTION__);
   return 0;
}



/*============================--------------------============================*/
/*===----                       key point support                      ----===*/
/*============================--------------------============================*/
void o___KEYS___________________o (void) {;}

char
POINT_bas2key       (short a_bas, short a_key)
{
   /*---(locals)-----------+-----+-----+-*/
   short       x_raw       =    0;
   /*---(fix raw ties)-------------------*/
   x_raw  = o.bas [a_bas].p_raw;
   /*---(tie raw and bas)-----------------*/
   o.raw [x_raw].p_key   = a_key;
   o.bas [a_bas].p_key   = o.avg [a_bas].p_key   = a_key;
   /*---(tie key)-------------------------*/
   o.key [a_key].p_raw   = x_raw;
   o.key [a_key].p_bas   = a_bas;
   o.key [a_key].p_key   = a_key;
   /*---(head)----------------------------*/
   o.key [a_key].seq     = a_key;
   o.key [a_key].fake    = o.bas [a_bas].fake;
   o.key [a_key].type    = o.bas [a_bas].type;
   /*---(touchpad)------------------------*/
   o.key [a_key].x_touch = o.bas [a_bas].x_touch;
   o.key [a_key].x_raw   = o.bas [a_bas].x_raw;
   o.key [a_key].y_touch = o.bas [a_bas].y_touch;
   o.key [a_key].y_raw   = o.bas [a_bas].y_raw;
   /*---(display)-------------------------*/
   o.key [a_key].x_pos   = o.bas [a_bas].x_pos;
   o.key [a_key].x_rel   = o.bas [a_bas].x_rel;
   o.key [a_key].y_pos   = o.bas [a_bas].y_pos;
   o.key [a_key].y_rel   = o.bas [a_bas].y_rel;
   /*---(complete)-----------------------*/
   return 0;
}

short
POINT_key_find          (char a_style, short a_bas)
{
   /*---(locals)-----------+-----+-----+-*/
   char        rce         =  -10;
   short       i           =    0;
   short       n           =   -1;
   /*---(header)-------------------------*/
   DEBUG_DATA    yLOG_enter   (__FUNCTION__);
   /*---(defense)------------------------*/
   DEBUG_DATA    yLOG_value   ("a_bas"     , a_bas);
   --rce;  if (a_bas < 0 || a_bas >= o.nbas) {
      DEBUG_DATA    yLOG_note    ("out of range");
      DEBUG_DATA    yLOG_exitr   (__FUNCTION__, rce);
      return rce;
   }
   /*---(find point)---------------------*/
   --rce;  for (i = 0; i < o.nkey; ++i) {
      DEBUG_DATA_M  yLOG_complex ("compare"   , "%2d, %4d to %4d", i, a_bas, o.key [i].p_bas);
      /*---(found)-------------*/
      if (a_style == FIND_EXACT  && a_bas == o.key [i].p_bas) {
         DEBUG_DATA    yLOG_value   ("found"     , i);
         n = i;
         break;
      }
      /*---(duplicate)---------*/
      if (a_style == FIND_INSERT && a_bas == o.key [i].p_bas) {
         DEBUG_DATA    yLOG_note    ("duplicate, already exists");
         DEBUG_DATA    yLOG_exitr   (__FUNCTION__, rce);
         return rce;
      }
      /*---(put before)--------*/
      if (a_style == FIND_INSERT && a_bas <  o.key [i].p_bas) {
         DEBUG_DATA    yLOG_value   ("found"     , i);
         n = i;
         break;
      }
      /*---(done)--------------*/
   }
   /*---(failed)-------------------------*/
   DEBUG_DATA    yLOG_value   ("n"         , n);
   if (n < 0) {
      /*---(not found)---------*/
      if (a_style == FIND_EXACT) {
         DEBUG_DATA    yLOG_exitr   (__FUNCTION__, rce);
         return rce;
      }
      /*---(append)------------*/
      n = MAX_POINTS + 1;
   }
   /*---(failed)-------------------------*/
   DEBUG_DATA    yLOG_exit    (__FUNCTION__);
   return n;
}

char
POINT_key_add           (short a_bas)
{
   /*---(locals)-----------+-----+-----+-*/
   char        rce         =  -10;
   short       i           =    0;
   short       n           =   -1;
   /*---(header)-------------------------*/
   DEBUG_DATA    yLOG_enter   (__FUNCTION__);
   /*---(find)---------------------------*/
   n = POINT_key_find (FIND_INSERT, a_bas);
   DEBUG_DATA    yLOG_value   ("n"         , n);
   /*---(duplicate)----------------------*/
   if (n < 0) {
      DEBUG_DATA    yLOG_exitr   (__FUNCTION__, rce);
      return rce;
   }
   /*---(append)-------------------------*/
   if (n >= MAX_POINTS) {
      DEBUG_DATA    yLOG_note    ("append mode");
      POINT_bas2key (a_bas, o.nkey);
   }
   /*---(insert)-------------------------*/
   else {
      DEBUG_DATA    yLOG_note    ("insert mode");
      for (i = o.nkey - 1; i >= n; --i)   POINT_push_up (POINTS_KEY, i);
      POINT_bas2key (a_bas, n);
   }
   /*---(increment counters)-------------*/
   ++o.nkey;
   POINT_reseq (POINTS_KEY);
   POINT_rekey ();
   /*---(complete)-----------------------*/
   DEBUG_DATA    yLOG_exit    (__FUNCTION__);
   return 0;
}

char
POINT_key_del           (short a_bas)
{
   /*---(locals)-----------+-----+-----+-*/
   char        rce         =  -10;
   short       i           =    0;
   short       n           =   -1;
   short       x_bas       =   -1;
   /*---(header)-------------------------*/
   DEBUG_DATA    yLOG_enter   (__FUNCTION__);
   /*---(find)---------------------------*/
   n = POINT_key_find (FIND_EXACT, a_bas);
   /*---(not found)----------------------*/
   --rce;  if (n < 0) {
      DEBUG_DATA    yLOG_exitr   (__FUNCTION__, rce);
      return rce;
   }
   /*---(remove)-------------------------*/
   x_bas  = o.key [n].p_bas;
   o.bas [x_bas].p_key   = -1;
   o.avg [x_bas].p_key   = -1;
   for (i = n; i < o.nkey; ++i)  {
      POINT_pull_down (POINTS_KEY, i);
   }
   POINT_wipe (POINTS_KEY, o.nkey - 1);
   /*---(decrement counters)-------------*/
   --o.nkey;
   POINT_reseq (POINTS_KEY);
   POINT_rekey ();
   /*---(complete)-----------------------*/
   DEBUG_DATA    yLOG_exit    (__FUNCTION__);
   return 0;
}



/*============================--------------------============================*/
/*===----                      sequence checking                       ----===*/
/*============================--------------------============================*/
void o___SEQUENCING_____________o (void) {;}

char
POINT_seq_start    (char a_type, tPOINT *p, int c)
{
   /*---(locals)-----------+-----+-----+-*/
   char        rce         =  -10;
   char        x_prev      =  '-';
   /*---(check initial start)------------*/
   --rce;  if (c == 0 && a_type != POINT_START) {
      DEBUG_RAW    yLOG_snote   ("zero must be S");
      return rce;
   }
   /*---(quick defense)------------------*/
   if (c == 0)   return 0;
   /*---(prepare)------------------------*/
   x_prev = p [c - 1].type;
   /*---(check later starts)-------------*/
   --rce;  if (a_type == POINT_START && x_prev != POINT_FINISH) {
      DEBUG_RAW    yLOG_snote   ("S must follow F");
      return rce;
   }
   --rce;  if (x_prev == POINT_FINISH && a_type != POINT_START) {
      DEBUG_RAW    yLOG_snote   ("F must preceed S");
      return rce;
   }
   /*---(complete)-----------------------*/
   return 0;
}

char
POINT_seq_head     (char a_type, tPOINT *p, int c, int a_xpad, int a_ypad)
{
   /*---(locals)-----------+-----+-----+-*/
   char        rce         =  -10;
   char        x_prev      =  '-';
   short       x_xpad      =    0;
   short       x_ypad      =    0;
   /*---(quick defense)------------------*/
   if (c == 0)   return 0;
   /*---(prepare)------------------------*/
   x_prev = p [c - 1].type;
   x_xpad = p [c - 1].x_touch;
   x_ypad = p [c - 1].y_touch;
   /*---(check sequencing)---------------*/
   --rce;  if (x_prev == POINT_START && a_type != POINT_HEAD) {
      DEBUG_RAW    yLOG_snote   ("> must follow S");
      return rce;
   }
   --rce;  if (a_type == POINT_HEAD && x_prev != POINT_START) {
      DEBUG_RAW    yLOG_snote   ("S must preceed >");
      return rce;
   }
   --rce;  if (a_type == POINT_HEAD && (a_xpad != x_xpad || a_ypad != x_ypad)) {
      DEBUG_RAW    yLOG_snote   ("S and > must match");
      return rce;
   }
   /*---(complete)-----------------------*/
   return 0;
}

char
POINT_seq_normal   (char a_type, tPOINT *p, int c, int a_xpad, int a_ypad)
{
   /*---(locals)-----------+-----+-----+-*/
   char        rce         =  -10;
   char        x_prev      =  '-';
   short       x_xpad      =    0;
   short       x_ypad      =    0;
   char        x_dup       =    0;
   /*---(quick defense)------------------*/
   if (c == 0)   return 0;
   /*---(prepare)------------------------*/
   x_prev = p [c - 1].type;
   x_xpad = p [c - 1].x_touch;
   x_ypad = p [c - 1].y_touch;
   /*---(check sequencing)---------------*/
   --rce;  if (a_type == POINT_NORMAL && strchr (">-", x_prev) == NULL) {
      DEBUG_RAW    yLOG_snote   ("- must follow > or -");
      return rce;
   }
   /*---(check duplication)--------------*/
   --rce;  if (a_type == POINT_NORMAL) {
      x_dup = 0;
      if (a_xpad == x_xpad) ++x_dup;
      if (a_ypad == x_ypad) ++x_dup;
      if (x_dup >= 2) {
         DEBUG_RAW    yLOG_snote   ("duplicate xy pos -");
         return rce;
      }
   }
   /*---(complete)-----------------------*/
   return 0;
}

char
POINT_seq_tail     (char a_type, tPOINT *p, int c, int a_xpad, int a_ypad)
{
   /*---(locals)-----------+-----+-----+-*/
   char        rce         =  -10;
   char        x_prev      =  '-';
   short       x_xpad      =    0;
   short       x_ypad      =    0;
   /*---(quick defense)------------------*/
   if (c == 0)   return 0;
   /*---(prepare)------------------------*/
   x_prev = p [c - 1].type;
   x_xpad = p [c - 1].x_touch;
   x_ypad = p [c - 1].y_touch;
   /*---(check sequencing)---------------*/
   --rce;  if (a_type == POINT_TAIL && strchr (">-", x_prev) == NULL) {
      DEBUG_RAW    yLOG_snote   ("< must follow > or -");
      return rce;
   }
   --rce;  if (a_type == POINT_TAIL && x_prev == POINT_NORMAL && a_xpad == x_xpad && a_ypad == x_ypad) {
      DEBUG_RAW    yLOG_snote   ("last - and < match, compress");
      p [c - 1].type = POINT_TAIL;
      return rce;
   }
   /*---(complete)-----------------------*/
   return 0;
}

char
POINT_seq_finish   (char a_type, tPOINT *p, int c, int a_xpad, int a_ypad)
{
   /*---(locals)-----------+-----+-----+-*/
   char        rce         =  -10;
   char        x_prev      =  '-';
   short       x_xpad      =    0;
   short       x_ypad      =    0;
   /*---(quick defense)------------------*/
   if (c == 0)   return 0;
   /*---(prepare)------------------------*/
   x_prev = p [c - 1].type;
   x_xpad = p [c - 1].x_touch;
   x_ypad = p [c - 1].y_touch;
   /*---(check sequencing)---------------*/
   --rce;  if (a_type == POINT_FINISH && x_prev != POINT_TAIL) {
      DEBUG_RAW    yLOG_snote   ("F must folllow <");
      return rce;
   }
   --rce;  if (x_prev == POINT_TAIL && a_type != POINT_FINISH) {
      DEBUG_RAW    yLOG_snote   ("< must preceed F");
      return rce;
   }
   --rce;  if (a_type == POINT_FINISH && (a_xpad != x_xpad || a_ypad != x_ypad)) {
      DEBUG_RAW    yLOG_snote   ("F and < must match");
      return rce;
   }
   /*---(complete)-----------------------*/
   return 0;
}



/*============================--------------------============================*/
/*===----                     generate statistics                      ----===*/
/*============================--------------------============================*/
void o___STATISTICS_____________o (void) {;}

char
POINT_pos          (tPOINT *a_curr)
{
   /*---(locals)-----------+-----+-----+-*/
   char        rce         =  -10;
   /*---(header)-------------------------*/
   DEBUG_CALC   yLOG_senter  (__FUNCTION__);
   /*---(defense)------------------------*/
   DEBUG_CALC   yLOG_spoint  (a_curr);
   --rce;  if (a_curr == NULL) {
      DEBUG_CALC   yLOG_sexitr  (__FUNCTION__, rce);
      return rce;
   }
   --rce;  if (my.ratio == 0 || my.x_scale == 0 ||my.y_scale == 0) {
      DEBUG_CALC   yLOG_snote   ("scaling not set");
      DEBUG_CALC   yLOG_sexitr  (__FUNCTION__, rce);
      return rce;
   }
   /*---(xpos)-----------------------------*/
   a_curr->x_pos = a_curr->x_raw / my.ratio;
   DEBUG_CALC   yLOG_sint    (a_curr->x_pos);
   /*---(ypos)-----------------------------*/
   a_curr->y_pos = a_curr->y_raw / my.ratio;
   DEBUG_CALC   yLOG_sint    (a_curr->y_pos);
   /*---(complete)-------------------------*/
   DEBUG_CALC   yLOG_sexit   (__FUNCTION__);
   return 0;
}

char
POINT_calc         (char a_type, tPOINT *a_curr, char a_span)
{
   /*---(locals)-----------+-----+-----+-*/
   char        rce         =  -10;
   tPOINT     *x_1st       = NULL;
   tPOINT     *x_2nd       = NULL;
   /*---(header)-------------------------*/
   DEBUG_CALC   yLOG_senter  (__FUNCTION__);
   /*---(defense)------------------------*/
   DEBUG_CALC   yLOG_spoint  (a_curr);
   --rce;  if (a_curr == NULL || a_curr < o.raw || a_curr >= o.key + MAX_POINTS) {
      DEBUG_CALC   yLOG_sexitr  (__FUNCTION__, rce);
      return rce;
   }
   DEBUG_CALC   yLOG_schar   (a_type);
   --rce;  if (a_type == 0 || strchr (POINTS_ALL, a_type) == NULL) {
      DEBUG_CALC   yLOG_sexitr  (__FUNCTION__, rce);
      return rce;
   }
   DEBUG_CALC   yLOG_schar   (a_span);
   --rce;  if (a_span == 0  || strchr (SPAN_ALL  , a_span) == NULL) {
      DEBUG_CALC   yLOG_sexitr  (__FUNCTION__, rce);
      return rce;
   }
   --rce;  if (my.ratio   == 0 || my.x_scale == 0 ||my.y_scale == 0) {
      DEBUG_CALC   yLOG_snote   ("scaling not set");
      DEBUG_CALC   yLOG_sexitr  (__FUNCTION__, rce);
      return rce;
   }
   /*---(position only)------------------*/
   if (a_curr->type == POINT_START) {
      DEBUG_CALC   yLOG_snote   ("no calcs for start");
      DEBUG_CALC   yLOG_sexit   (__FUNCTION__);
      return 0;
   }
   if (a_span == SPAN_NORM && a_curr->type == POINT_HEAD) {
      DEBUG_CALC   yLOG_snote   ("can not bas head");
      DEBUG_CALC   yLOG_sexit   (__FUNCTION__);
      return 0;
   }
   if (a_curr->type == POINT_FINISH) {
      DEBUG_CALC   yLOG_snote   ("no calcs for finish");
      DEBUG_CALC   yLOG_sexit   (__FUNCTION__);
      return 0;
   }
   /*---(set the ends)-------------------*/
   x_1st  = a_curr - 1;
   x_2nd  = a_curr;
   if (a_span == SPAN_AVG )  x_2nd = a_curr + 1;
   DEBUG_CALC   yLOG_sint (x_1st->seq);
   DEBUG_CALC   yLOG_sint (x_2nd->seq);
   /*---(x and y change)-----------------*/
   DEBUG_CALC   yLOG_sint (x_1st->x_raw);
   DEBUG_CALC   yLOG_sint (x_2nd->x_raw);
   a_curr->xd     = x_2nd->x_raw - x_1st->x_raw;
   DEBUG_CALC   yLOG_sint (a_curr->xd);
   DEBUG_CALC   yLOG_sint (x_1st->y_raw);
   DEBUG_CALC   yLOG_sint (x_2nd->y_raw);
   a_curr->yd     = x_2nd->y_raw - x_1st->y_raw;
   DEBUG_CALC   yLOG_sint (a_curr->yd);
   if (a_curr->xd == 0 && a_curr->yd == 0) {
      if (a_span == SPAN_NORM) {
         DEBUG_CALC   yLOG_snote   ("no change, no calcs");
         DEBUG_CALC   yLOG_sexit   (__FUNCTION__);
         return 0;
      } else {
         DEBUG_CALC   yLOG_snote   ("reversal, copy bas");
         BASE_bas2avg (a_curr->seq);
         DEBUG_CALC   yLOG_sexit   (__FUNCTION__);
         return 0;
      }
   }
   /*---(length)-------------------------*/
   a_curr->len    = sqrt((a_curr->xd * a_curr->xd) + (a_curr->yd * a_curr->yd));
   DEBUG_CALC   yLOG_sdouble (a_curr->len);
   /*---(slope)--------------------------*/
   if      (a_curr->xd != 0)  a_curr->slope = (float) a_curr->yd / a_curr->xd;
   else if (a_curr->yd >  0)  a_curr->slope = +999.99;
   else                       a_curr->slope = -999.99;
   if      (a_curr->slope > +999.99) a_curr->slope = +999.99;
   else if (a_curr->slope < -999.99) a_curr->slope = -999.99;
   DEBUG_CALC   yLOG_sdouble (a_curr->slope);
   /*---(intercept)----------------------*/
   a_curr->icept   = a_curr->y_raw - (a_curr->slope * a_curr->x_raw);
   if      (a_curr->icept >  99999)  a_curr->icept =  99999;
   else if (a_curr->icept < -99999)  a_curr->icept = -99999;
   DEBUG_CALC   yLOG_sdouble (a_curr->icept);
   /*---(radians)------------------------*/
   a_curr->rads  = atan2f (a_curr->yd, a_curr->xd);
   if (a_curr->rads > FULL_CIRCLE) a_curr->rads -= FULL_CIRCLE;
   if (a_curr->rads < 0)           a_curr->rads += FULL_CIRCLE;
   DEBUG_CALC   yLOG_sdouble (a_curr->rads);
   /*---(degrees)------------------------*/
   a_curr->degs  = a_curr->rads * 57.29577951308;
   DEBUG_CALC   yLOG_sdouble (a_curr->degs);
   /*---(quadrant)-----------------------*/
   if (a_curr->xd >= 0) {
      if (a_curr->yd >= 0) a_curr->quad = 1;
      else                 a_curr->quad = 4;
   } else {
      if (a_curr->yd >= 0) a_curr->quad = 2;
      else                 a_curr->quad = 3;
   }
   DEBUG_CALC   yLOG_sint    (a_curr->quad);
   /*---(correct the deg as required)------*/
   /*> if (a_curr->quad == 2) a_curr->degs -= 180;                                          <* 
    *> if (a_curr->quad == 3) a_curr->degs += 180;                                          <*/
   /*> /+---(fill-in beginning)--------------+/                                       <* 
    *> if (x_1st->type == POINT_HEAD) {                                               <* 
    *>    /+---(head first)------------------+/                                       <* 
    *>    x_1st->slope = a_curr->slope;                                               <* 
    *>    x_1st->icept = a_curr->icept;                                               <* 
    *>    x_1st->rads  = a_curr->rads;                                                <* 
    *>    x_1st->degs  = a_curr->degs;                                                <* 
    *>    x_1st->quad  = a_curr->quad;                                                <* 
    *>    /+---(start too)-------------------+/                                       <* 
    *>    if (a_type != POINTS_KEY) {                                                 <* 
    *>       x_1st     = x_1st - 1;                                                   <* 
    *>       x_1st->slope = a_curr->slope;                                            <* 
    *>       x_1st->icept = a_curr->icept;                                            <* 
    *>       x_1st->rads  = a_curr->rads;                                             <* 
    *>       x_1st->degs  = a_curr->degs;                                             <* 
    *>       x_1st->quad  = a_curr->quad;                                             <* 
    *>    }                                                                           <* 
    *>    /+---(done)------------------------+/                                       <* 
    *> }                                                                              <*/
   /*---(complete)-------------------------*/
   DEBUG_CALC   yLOG_sexit   (__FUNCTION__);
   return 0;
}

char
POINT_curve             (short a_start, short a_finish, float a_len, float a_slope, float a_icept, float *a_depth, float *a_ratio, char *a_cat)
{
   /*   0 = line       0.0   to   3.0
    *   1 = bendy      3.0+  to   7.0
    *   2 = curvy      7.0+  to  25.0
    *   3 = loopy     25.0+  to  40.0
    *   4 = balloon   40.0+  to  --.-
    */
   /*---(locals)-----------+-----+-----+-*/
   char        rce         =  -10;
   short       x_beg       =    0;
   short       x_end       =    0;
   int         i           =    0;
   float       xr, yr;
   float       xp, yp;
   float       xd, yd, fd;
   float       s1, s2;
   float       b1, b2;
   char        x_label      [LEN_LABEL];
   char        x_cat       =  '0';
   char        x_horz      =  '?';
   char        x_vert      =  '?';
   float       f_max       =  0.0;
   float       x_min       =    0;
   float       x_max       =    0;
   float       y_min       =    0;
   float       y_max       =    0;
   /*---(header)-------------------------*/
   DEBUG_MATCH   yLOG_enter   (__FUNCTION__);
   DEBUG_MATCH   yLOG_complex ("args"      , "%3ds, %3df, %6.0fs, %6.0fi, %-10.10pd, %-10.10pc",  a_start, a_finish, a_slope, a_icept, a_depth, a_cat);
   /*---(defense)-------------------------------*/
   --rce;  if (a_start < 0 || a_start >= o.nkey) {
      DEBUG_MATCH   yLOG_exitr   (__FUNCTION__, rce);
      return rce;
   }
   --rce;  if (a_finish < 0 || a_finish >= o.nkey) {
      DEBUG_MATCH   yLOG_exitr   (__FUNCTION__, rce);
      return rce;
   }
   /*---(assign base points)--------------------*/
   x_beg   = o.key [a_start ].p_bas;
   x_end   = o.key [a_finish].p_bas;
   DEBUG_MATCH   yLOG_complex ("bases"     , "%3ds, %3df", x_beg, x_end);
   for (i = x_beg; i <= x_end; ++i) {
      /*---(get raw)---------------------*/
      xr   = o.bas [i].x_raw;
      yr   = o.bas [i].y_raw;
      /*---(reset base values)-----------*/
      s1   = a_slope;
      b1   = a_icept;
      /*---(if horizontal)---------------*/
      if (s1 > -0.01 && s1 < 0.01) {
         strlcpy (x_label, "horz", LEN_LABEL);
         yp   = b1;
         xp   = xr;
         s2 = b2 = 0.0;
      }
      /*---(if vertical)-----------------*/
      else if (s1 < -999.0 || s1 > 999.0) {
         strlcpy (x_label, "vert", LEN_LABEL);
         xp   = o.bas [x_beg].x_raw;
         yp   = yr;
         s2 = b2 = 0.0;
      }
      /*---(normal)----------------------*/
      else {
         strlcpy (x_label, "norm", LEN_LABEL);
         /*---(get perpendicular values)-*/
         s2   = -1.0 / s1;
         if (s2 ==   0.0)    s2 =  0.001;
         b2   = yr - (s2 * xr);
         /*---(calc intersection)--------*/
         xp   = (b2 - b1) / (s1 - s2);
         yp   = (s1 * xp) + b1;
         DEBUG_MATCH   yLOG_complex ("details"   , "%6.0fb1, %6.0fb2, %6.0fbd, %6.1fs1, %6.1fs2, %6.1fsd, %6.1fcx, %6.1fxp, %6.1fcy, %6.1fyp", b1, b2, b2 - b1, s1, s2, s1 - s2, (b2 - b1) / (s1 - s2), xp, (s1 * xp) + b1, yp);
      }
      /*---(calc differences)------------*/
      xd   = xr - xp;
      yd   = yr - yp;
      fd   = sqrt ((xd * xd) + (yd * yd));
      /*---(directions)------------------*/
      if (xd < x_min)  x_min = xd;
      if (xd > x_max)  x_max = xd;
      if (yd < y_min)  y_min = yd;
      if (yd > y_max)  y_max = yd;
      if (xd >=  3.0 && x_horz != '+') {
         if (x_horz == '?')   x_horz = '+';
         else                 x_horz = '*';
      }
      if (xd <= -3.0 && x_horz != '-') {
         if (x_horz == '?')   x_horz = '-';
         else                 x_horz = '*';
      }
      if (yd >=  3.0 && x_vert != '+') {
         if (x_vert == '?')   x_vert = '+';
         else                 x_vert = '*';
      }
      if (yd <= -3.0 && x_vert != '-') {
         if (x_vert == '?')   x_vert = '-';
         else                 x_vert = '*';
      }
      /*---(report out)------------------*/
      DEBUG_MATCH   yLOG_complex ("main line" , "%3db, %4.0fxr %4.0fyr, %6.1fs1 %6.0fb1", i, xr, yr, s1, b1);
      DEBUG_MATCH   yLOG_complex ("intersect" , "%4.4s, %4.0fxp %4.0fyp, %6.1fs2 %6.0fb2", x_label, xp, yp, s2, b2);
      DEBUG_MATCH   yLOG_complex ("dist"      , "%4.0fxd %4.0fyd %4.0ffd", xd, yd, fd);
      /*---(set the curve)---------------*/
      if (fd > f_max)  f_max = fd;
   }
   DEBUG_MATCH   yLOG_complex ("maxes"     , "%4.0ff, %4.0f to %4.0fx %c, %4.0f to %4.0fy %c", f_max, x_min, x_max, x_horz, y_min, y_max, x_vert);
   /*---(categorize)---------------------*/
   if      (f_max >= 120.0)      x_cat = +8;
   else if (f_max >= 100.0)      x_cat = +7;
   else if (f_max >=  80.0)      x_cat = +6;
   else if (f_max >=  60.0)      x_cat = +5;
   else if (f_max >=  40.0)      x_cat = +4;
   else if (f_max >=  25.0)      x_cat = +3;
   else if (f_max >=   7.0)      x_cat = +2;
   else if (f_max >=   3.0)      x_cat = +1;
   else                          x_cat =  0;
   /*---(return)-------------------------*/
   if (a_depth != NULL)  *a_depth = f_max;
   if (a_ratio != NULL)  *a_ratio = f_max / a_len;
   if (a_cat   != NULL)  *a_cat   = x_cat;
   /*---(complete)-----------------------*/
   DEBUG_MATCH   yLOG_exit    (__FUNCTION__);
   return 0;
}



/*============================--------------------============================*/
/*===----                        data reporting                        ----===*/
/*============================--------------------============================*/
void o___REPORTING______________o (void) {;}

char          /*----: display all information on an individual point ---------*/
POINT_show         (FILE *a_file, char a_style, tPOINT *a_curr, int a_num)
{
   /*> float x_slope  = a_curr->slope;                                                <* 
    *> int   x_icept  = a_curr->icept;                                                <*/
   char        q1          [LEN_LABEL] = "";
   char        q2          [LEN_LABEL] = " |";
   if (a_style == 'g') {
      strlcpy (q1, " ", LEN_LABEL);
      strlcpy (q2, " ", LEN_LABEL);
   }
   fprintf (a_file, "%4d%s %3d%s %3d%s %4d%s %c%s "      , a_num         , q1, a_curr->p_key , q1, a_curr->p_bas , q1, a_curr->p_raw , q1, a_curr->fake, q2);
   fprintf (a_file, "%6d%s %6d%s %4.0f%s %4.0f%s "       , a_curr->x_raw, q1, a_curr->y_raw, q1, a_curr->xd    , q1, a_curr->yd  , q2);
   fprintf (a_file, "%4.0f%s %8.2f%s %6.0f%s %5.2f%s %3.0f%s ", a_curr->len   , q1, a_curr->slope , q1, a_curr->icept , q1, a_curr->rads, q1, a_curr->degs , q2);
   fprintf (a_file, "%c%s %c%s %c%s %c%s "                    , a_curr->type, q1, a_curr->prekey, q1, a_curr->marked, q1, a_curr->sharp , q1);
   fprintf (a_file, "%1d%s %1d%s %5.1f%s %5.3f%s %1d%s " , a_curr->quad  , q1, a_curr->range , q1, a_curr->depth , q1, a_curr->ratio , q1, a_curr->ccat, q1);
   fprintf (a_file, "%-3.3s%s "                         , a_curr->use   , q2);
   fprintf (a_file, "%6.3f%s %6.3f%s "                   , a_curr->x_rel , q1, a_curr->y_rel , q1);
   fprintf (a_file, "%4.0f%s %4.0f%s\n"                      , a_curr->x_pos , q1, a_curr->y_pos , q1);
   return 0;
}

char          /*----: list all points of a particular type -------------------*/
POINT_list         (FILE *a_file, char a_style, tPOINT *a_series, int a_count)
{
   int         i;
   char        x_label     [LEN_TERSE] = "";
   /*---(header)-------------------------*/
   DEBUG_DATA   yLOG_senter  (__FUNCTION__);
   DEBUG_DATA   yLOG_schar   (a_style);
   if (a_style == 'd') {
      if      (a_series == &(o.raw))   strlcpy (x_label, "RAW", LEN_TERSE);
      else if (a_series == &(o.bas))   strlcpy (x_label, "BAS", LEN_TERSE);
      else if (a_series == &(o.avg))   strlcpy (x_label, "AVG", LEN_TERSE);
      else if (a_series == &(o.key))   strlcpy (x_label, "KEY", LEN_TERSE);
      fprintf (a_file, "\n");
      fprintf (a_file, "%-3.3s point inventory (%3d) --------------------------------------------------------------------------------------------------------------\n", x_label, a_count);
      fprintf (a_file, "seq# key bas raw- a | --xx-- --yy-- -xd- -yd- | -len -slope-- b-cept -rad- deg | t k m s q r depth ratio c use | -xrel- -yrel- xpos ypos\n");
   } else if (a_style == 'g') {
      fprintf (a_file, "###  key  bas  raw  a  --xx--  --yy--  -xd-  -yd-  -len  -slope--  b-cept  -rad-  deg  q  r  curve-  cc  c  t  u  --xrel  --yrel  xpos  ypos \n");
   }
   DEBUG_DATA   yLOG_snote   (x_label);
   /*---(points)-------------------------*/
   DEBUG_DATA   yLOG_sint    (a_count);
   for (i = 0; i < a_count; ++i) {
      POINT_show (a_file, a_style, a_series + i, i);
   }
   /*---(footer)-------------------------*/
   if (a_style == 'd') {
      fprintf (a_file, "seq# key bas raw- a | --xx-- --yy-- -xd- -yd- | -len -slope-- b-cept -rad- deg | t k m s q r depth ratio c use | -xrel- -yrel- xpos ypos\n");
      fprintf (a_file, "%-3.3s point inventory---------------------------------------------------------------------------------------------------------------------\n", x_label);
      fprintf (a_file, "\n");
   }
   /*---(complete)-----------------------*/
   DEBUG_DATA   yLOG_sexit   (__FUNCTION__);
   return 0;
}

char
POINT_clipboard    (char *a_cmd, char *a_opt)
{
   /*---(locals)-----------+-----+-----+-*/
   char        rce         =  -10;
   FILE       *f           = NULL;
   char        x_style     =  '-';
   /*---(defense)------------------------*/
   --rce;  if (a_cmd == NULL)  return rce;
   --rce;  if (a_opt == NULL)  return rce;
   /*---(open file)----------------------*/
   --rce;  if (strcmp (a_cmd, "clip" ) == 0) {
      f = fopen ("/root/z_gehye/vi_clip.txt", "w");
      if (f == NULL)  return rce;
      x_style = 'd';
   } else if (strcmp (a_cmd, "gyges") == 0) {
      f = fopen ("/root/z_gehye/vi_clip.txt", "w");
      if (f == NULL)  return rce;
      x_style = 'g';
   } else {
      f = stdout;
      x_style = 'd';
   }
   /*---(dump points)--------------------*/
   --rce;  if (strcmp (a_opt, "key") == 0)   POINT_list (f, x_style, o.key, o.nkey);
   else if    (strcmp (a_opt, "avg") == 0)   POINT_list (f, x_style, o.avg, o.navg);
   else if    (strcmp (a_opt, "bas") == 0)   POINT_list (f, x_style, o.bas, o.nbas);
   else if    (strcmp (a_opt, "raw") == 0)   POINT_list (f, x_style, o.raw, o.nraw);
   else       return rce;
   /*---(close file)---------------------*/
   if      (strcmp (a_cmd, "clip" ) == 0)  fclose (f);
   else if (strcmp (a_cmd, "gyges") == 0)  fclose (f);
   /*---(complete)-----------------------*/
   return 0;
}



/*============================--------------------============================*/
/*===----                         unit testing                         ----===*/
/*============================--------------------============================*/
static void o___UNIT_TEST_________________o (void) {;}

char*        /*-> unit test accessor -----------------[ light  [us.D90.241.L0]*/ /*-[03.0000.00#.#]-*/ /*-[--.---.---.--]-*/
POINT__unit          (char *a_question, char a_type, int a_num)
{
   /*---(locals)-----------+-----+-----+-*/
   int         i           = 0;
   char        t           [100] = "";
   char        s           [100] = "";
   int         x_end       = 0;
   int         y_end       = 0;
   tPOINT     *p           = NULL;
   int         c           =    0;
   char        x_pre       [LEN_TERSE] = "";
   /*---(initialize)---------------------*/
   strlcpy (unit_answer, "POINT unit       : unknownn request", 100);
   p = POINT__series (a_type, x_pre, &c, NULL);
   if (p == NULL)  return unit_answer;
   /*---(core data)----------------------*/
   if        (strncmp (a_question, "types"     , 20)  == 0) {
      for (i = 0; i < 45; ++i) {
         if (i >= c)  break;
         t [i]     = p [i].type;
         t [i + 1] = 0;
      }
      if (c > 45)  sprintf (s, "[%s>", t);
      else         sprintf (s, "[%s]", t);
      snprintf (unit_answer, LEN_STR, "%-3.3s by type      : %2d %s", x_pre, c, s);
   }
   else if   (strncmp (a_question, "links"     , 20)  == 0) {
      snprintf (unit_answer, LEN_STR, "%-3.3s links   (%2d) : %c ", x_pre, a_num, p [a_num].type);
      if (p [a_num].seq   >= 0)  sprintf  (t, " %4d%c" , p [a_num].seq  , '#');
      else                       sprintf  (t, "    -%c",                  '#');
      strlcat (unit_answer, t, LEN_STR);
      if (p [a_num].p_raw >= 0)  sprintf  (t, " %4d%c" , p [a_num].p_raw, 'r');
      else                       sprintf  (t, "    -%c",                  'r');
      strlcat (unit_answer, t, LEN_STR);
      if (p [a_num].p_bas >= 0)  sprintf  (t, " %4d%c" , p [a_num].p_bas, 'b');
      else                       sprintf  (t, "    -%c",                  'b');
      strlcat (unit_answer, t, LEN_STR);
      if (p [a_num].p_key >= 0)  sprintf  (t, " %4d%c" , p [a_num].p_key, 'k');
      else                       sprintf  (t, "    -%c",                  'k');
      strlcat (unit_answer, t, LEN_STR);
   }
   else if   (strncmp (a_question, "touch"     , 20)  == 0) {
      snprintf (unit_answer, LEN_STR, "%-3.3s touch   (%2d) : %c  %4dx %4dy  %4dx %4dy",
            x_pre, a_num, p [a_num].type,
            p [a_num].x_touch, p [a_num].y_touch,
            p [a_num].x_raw  , p [a_num].y_raw);
   }
   else if   (strncmp (a_question, "pos"       , 20)  == 0) {
      snprintf (unit_answer, LEN_STR, "%-3.3s pos     (%2d) : %c  %4dx %4dy  %4.0fx %4.0fy %4.0fxd %4.0fyd %3.0fl %8.3fs %6.0fi %6.3fr %4.0fd %dq",
            x_pre, a_num, p [a_num].type,
            p [a_num].x_raw, p [a_num].y_raw,
            p [a_num].x_pos, p [a_num].y_pos,
            p [a_num].xd   , p [a_num].yd   , p [a_num].len,
            p [a_num].slope, p [a_num].icept,
            p [a_num].rads , p [a_num].degs , p [a_num].quad);
   }
   else if   (strncmp (a_question, "curve"     , 20)  == 0) {
      snprintf (unit_answer, LEN_STR, "%-3.3s curve   (%2d) : %6.1fl %6.1fd %6.3fr   %1dc",
            x_pre, a_num, p [a_num].len, p [a_num].depth, p [a_num].ratio, p [a_num].ccat);
   }
   /*---(complete)-----------------------*/
   return unit_answer;
}

