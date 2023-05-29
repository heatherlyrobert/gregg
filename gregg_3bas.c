/*=================================(beg-code)=================================*/
#include "gregg.h"


/*
 *  bas/avg points are the same, but calculations are different.  they are
 *  used to smooth the naturally jagged contures out of the source drawing.
 *
 *  start, head, tail, and finish should all be the same, then the points
 *  inbetween are sampled.
 *
 *
 */

/*---(configuration)------------------*/
static float   s_append    = 10.0;
static float   s_adjust    =  2.0;
static short   s_progress  =    0;
static float   s_sharp     = 70.0;
static short   s_beg       =    0;



/*============================--------------------============================*/
/*===----                      program level                           ----===*/
/*============================--------------------============================*/
static void o___PROGRAM________________o (void) {;}

char
BAS_config             (float a_append, float a_adjust, float a_sharp)
{
   s_append = a_append;
   s_adjust = a_adjust;
   s_sharp  = a_sharp;
   CIRCLE_config ((5 * 40) / s_append, (20 * 40) / s_append);
   return 0;
}

char
BAS_init             (void)
{
   o.nbas     = 0;
   o.navg     = 0;
   POINT_clear_series (POINTS_BAS);
   POINT_clear_series (POINTS_AVG);
   return 0;
}



/*============================--------------------============================*/
/*===----                        small support                         ----===*/
/*============================--------------------============================*/
static void o___SUPPORT________________o (void) {;}

float         /*----: check distance to potential point ----------------------*/
BAS__dist               (short a, short b)
{
   float       xd, yd, d;
   if (a < 0 || a >= o.nraw)    return -1;
   if (b < 0 || b >= o.nraw)    return -1;
   DEBUG_RAW    yLOG_sint    (a);
   DEBUG_RAW    yLOG_sint    (b);
   xd   = fabs (o.raw [a].x_raw - o.raw [b].x_raw);
   DEBUG_RAW    yLOG_sdouble (xd);
   yd   = fabs (o.raw [a].y_raw - o.raw [b].y_raw);
   DEBUG_RAW    yLOG_sdouble (yd);
   d    = sqrt ((xd * xd) + (yd * yd));
   /*> DEBUG_AVG_M  yLOG_complex ("dist"      , "%4d (%4d,%4d) to %4d (%4d,%4d) = %4.1fx, %4.1fy, %4.1fd", a, o.raw [a].x_raw, o.raw [a].y_raw, b, o.raw [b].x_raw, o.raw [b].y_raw, xd, yd, d);   <*/
   return d;
}



/*============================--------------------============================*/
/*===----                      copying and moving                      ----===*/
/*============================--------------------============================*/
void o___MOVEMENT_______________o (void) {;}

char
BAS__push_up            (short a_old)
{
   /*---(locals)-----------+-----+-----+-*/
   short       x_new       = a_old + 1;
   short       x_raw       =    0;
   /*---(header)-------------------------*/
   DEBUG_RAW    yLOG_complex ("copy"      , "%3d into %3d", a_old, x_new);
   /*---(raw points)---------------------*/
   x_raw  = o.bas [a_old].p_raw;
   /*---(fix raw ties)-------------------*/
   o.raw [x_raw].p_bas   = x_new;
   /*---(series/seq)---------------------*/
   o.bas [x_new].series  = o.avg [x_new].series      = o.bas [a_old].series;
   o.bas [x_new].seq     = o.avg [x_new].seq         = x_new;
   o.bas [x_new].type    = o.avg [x_new].type        = o.bas [a_old].type;
   /*---(tie raw and bas)----------------*/
   o.bas [x_new].p_raw   = o.avg [x_new].p_raw       = x_raw;
   o.bas [x_new].p_bas   = o.avg [x_new].p_bas       = x_new;
   o.bas [x_new].p_key   = o.avg [x_new].p_key       = o.bas [a_old].p_key;
   /*---(characteristics)----------------*/
   o.bas [x_new].fake    = o.avg [x_new].fake        = o.bas [a_old].fake;
   o.bas [x_new].prekey  = o.avg [x_new].prekey      = o.bas [a_old].prekey;
   o.bas [x_new].marked  = o.avg [x_new].marked      = o.bas [a_old].marked;
   /*---(original)-----------------------*/
   o.bas [x_new].x_touch = o.avg [x_new].x_touch     = o.bas [a_old].x_touch;
   o.bas [x_new].y_touch = o.avg [x_new].y_touch     = o.bas [a_old].y_touch;
   o.bas [x_new].x_raw   = o.avg [x_new].x_raw       = o.bas [a_old].x_raw;
   o.bas [x_new].y_raw   = o.avg [x_new].y_raw       = o.bas [a_old].y_raw;
   /*---(screen)-------------------------*/
   o.bas [x_new].x_rel   = o.avg [x_new].x_rel       = o.bas [a_old].x_rel;
   o.bas [x_new].y_rel   = o.avg [x_new].y_rel       = o.bas [a_old].y_rel;
   o.bas [x_new].x_pos   = o.avg [x_new].x_pos       = o.bas [a_old].x_pos;
   o.bas [x_new].y_pos   = o.avg [x_new].y_pos       = o.bas [a_old].y_pos;
   /*---(other)--------------------------*/
   o.bas [x_new].sharp   = o.avg [x_new].sharp       = o.bas [a_old].sharp;
   /*---(clear old)----------------------*/
   POINT_clear (&(o.bas [a_old]), POINT_NORMAL);
   POINT_clear (&(o.avg [a_old]), POINT_NORMAL);
   /*---(done)---------------------------*/
   return 0;
}

char
BAS_bas2avg             (short a_bas)
{
   /*---(leave blank)--------------------*/
   o.avg [a_bas].xd     = o.avg [a_bas].yd     = o.avg [a_bas].len     = 0;
   /*---(update)-------------------------*/
   o.avg [a_bas].icept  = o.bas [a_bas].icept;
   o.avg [a_bas].slope  = o.bas [a_bas].slope;
   o.avg [a_bas].degs   = o.bas [a_bas].degs;
   o.avg [a_bas].quad   = o.bas [a_bas].quad;
   /*---(done)---------------------------*/
   return 0;
}



/*============================--------------------============================*/
/*===----                      adding raw points                       ----===*/
/*============================--------------------============================*/
static void o___NEW____________________o (void) {;}

char
BAS__force_point        (uchar a_type, short x, short y)
{
   /*---(tie raw and bas)-----------------*/
   o.bas [o.nbas].p_raw   = o.avg [o.nbas].p_raw   = -1;
   o.bas [o.nbas].p_bas   = o.avg [o.nbas].p_bas   = o.nbas;
   /*---(head)----------------------------*/
   o.bas [o.nbas].seq     = o.avg [o.nbas].seq     = o.nbas;
   o.bas [o.nbas].fake    = o.avg [o.nbas].fake    = '-';
   o.bas [o.nbas].type    = o.avg [o.nbas].type    = a_type;
   /*---(touchpad)------------------------*/
   o.bas [o.nbas].x_touch = o.avg [o.nbas].x_touch = x;
   o.bas [o.nbas].x_raw   = o.avg [o.nbas].x_raw   = x;
   o.bas [o.nbas].y_touch = o.avg [o.nbas].y_touch = y;
   o.bas [o.nbas].y_raw   = o.avg [o.nbas].y_raw   = y;
   /*---(series)--------------------------*/
   o.bas [o.nbas].series  = POINTS_BAS;
   o.avg [o.nbas].series  = POINTS_AVG;
   /*---(position)-----------------------*/
   POINT_pos (o.bas + o.nbas);
   POINT_pos (o.avg + o.navg);
   /*---(counters)-----------------------*/
   ++o.nbas;
   ++o.navg;
   /*---(complete)-----------------------*/
   return 0;
}

/*> char                                                                              <* 
 *> BASE__raw2bas       (short a_raw, short a_bas)                                    <* 
 *> {                                                                                 <* 
 *>    /+---(tie raw and bas)-----------------+/                                      <* 
 *>    o.raw [a_raw].p_bas   = a_bas;                                                 <* 
 *>    o.bas [a_bas].p_raw   = o.avg [a_bas].p_raw   = a_raw;                         <* 
 *>    o.bas [a_bas].p_bas   = o.avg [a_bas].p_bas   = a_bas;                         <* 
 *>    /+---(head)----------------------------+/                                      <* 
 *>    o.bas [a_bas].seq     = o.avg [a_bas].seq     = a_bas;                         <* 
 *>    o.bas [a_bas].fake    = o.avg [a_bas].fake    = o.raw [a_raw].fake;            <* 
 *>    o.bas [a_bas].type    = o.avg [a_bas].type    = o.raw [a_raw].type;            <* 
 *>    /+---(touchpad)------------------------+/                                      <* 
 *>    o.bas [a_bas].x_touch = o.avg [a_bas].x_touch = o.raw [a_raw].x_touch;         <* 
 *>    o.bas [a_bas].x_raw   = o.avg [a_bas].x_raw   = o.raw [a_raw].x_raw;           <* 
 *>    o.bas [a_bas].y_touch = o.avg [a_bas].y_touch = o.raw [a_raw].y_touch;         <* 
 *>    o.bas [a_bas].y_raw   = o.avg [a_bas].y_raw   = o.raw [a_raw].y_raw;           <* 
 *>    /+---(display)-------------------------+/                                      <* 
 *>    o.bas [a_bas].x_pos   = o.avg [a_bas].x_pos   = o.raw [a_raw].x_pos;           <* 
 *>    o.bas [a_bas].x_rel   = o.avg [a_bas].x_rel   = o.raw [a_raw].x_rel;           <* 
 *>    o.bas [a_bas].y_pos   = o.avg [a_bas].y_pos   = o.raw [a_raw].y_pos;           <* 
 *>    o.bas [a_bas].y_rel   = o.avg [a_bas].y_rel   = o.raw [a_raw].y_rel;           <* 
 *>    /+---(complete)-----------------------+/                                       <* 
 *>    return 0;                                                                      <* 
 *> }                                                                                 <*/

short         /*----: add an additional basic point from a raw point ---------*/
BAS_append              (short a_raw, uchar a_force)
{
   /*---(locals)-----------+-----+-----+-*/
   char        rce         =  -10;
   float       d           =  0.0;
   float       xd, yd;
   uchar       x_type      =    0;
   short       x_bas       =    0;
   short       x_curr      =    0;
   short       x_prev      =   -1;
   short       x_pprev     =   -1;
   short       x_tail      =   -1;
   float       dt, pt      =  0.0;
   short       x_new       =   -1;
   /*---(if tail)------------------------*/
   if (o.raw [a_raw].type == POINT_TAIL)  BAS_append (a_raw - 5, POINT_SUFFIX);
   /*---(header)-------------------------*/
   DEBUG_RAW    yLOG_senter  (__FUNCTION__);
   DEBUG_RAW    yLOG_sint    (a_raw);
   x_curr = a_raw;
   /*---(defense)------------------------*/
   --rce;  if (x_curr < 0 || x_curr >= o.nraw) {
      DEBUG_RAW    yLOG_snote   ("raw index out of available range");
      DEBUG_RAW    yLOG_sexitr  (__FUNCTION__, rce);
      return rce;
   }
   /*---(if taken already)---------------*/
   --rce;  if (o.raw [x_curr].p_bas >= 0) {
      DEBUG_RAW    yLOG_snote   ("already assigned a base point");
      if (a_force == POINT_NORMAL) {
         DEBUG_RAW    yLOG_sexitr  (__FUNCTION__, rce);
         return rce;
      }
      DEBUG_RAW    yLOG_snote   ("reassign");
      o.bas [o.nbas - 1].type = a_force;
      DEBUG_RAW    yLOG_sexit   (__FUNCTION__);
      return x_curr;
   }
   /*---(prepare)------------------------*/
   x_type = o.raw [x_curr].type;
   DEBUG_RAW    yLOG_schar   (x_type);
   x_bas  = o.nbas - 1;
   DEBUG_RAW    yLOG_sint    (x_bas);
   if (o.nbas >= 1)  x_prev  = o.bas [x_bas].p_raw;
   DEBUG_RAW    yLOG_sint    (x_prev);
   if (o.nbas >= 2)  x_pprev = o.bas [x_bas - 1].p_raw;
   DEBUG_RAW    yLOG_sint    (x_pprev);
   /*---(filter duplicates)--------------*/
   if (x_prev >= 0 && x_curr == x_prev) {
      DEBUG_RAW    yLOG_snote   ("same raw as last base point");
      DEBUG_RAW    yLOG_sexitr  (__FUNCTION__, rce);
      return rce;
   }
   /*---(if tiny loop)-------------------*/
   /*> if (x_type == POINT_NORMAL) {                                                  <* 
    *>    if (x_prev  >= 0) {                                                         <* 
    *>       xd = fabs (o.raw [x_curr].x_raw - o.raw [x_prev].x_raw);                 <* 
    *>       yd = fabs (o.raw [x_curr].y_raw - o.raw [x_prev].y_raw);                 <* 
    *>       if (xd <  1.5 && yd < 1.5)  x_curr += 3;                                 <* 
    *>    }                                                                           <* 
    *>    if (x_pprev >= 0) {                                                         <* 
    *>       xd = fabs (o.raw [x_curr].x_raw - o.raw [x_pprev].x_raw);                <* 
    *>       yd = fabs (o.raw [x_curr].y_raw - o.raw [x_pprev].y_raw);                <* 
    *>       if (xd <  1.5 && yd < 1.5)  x_curr += 3;                                 <* 
    *>    }                                                                           <* 
    *>    if (x_curr != a_raw) {                                                      <* 
    *>       DEBUG_RAW    yLOG_snote   ("updated current");                           <* 
    *>       DEBUG_RAW    yLOG_sint    (x_curr);                                      <* 
    *>    }                                                                           <* 
    *> }                                                                              <*/
   /*---(filter too close)---------------*/
   DEBUG_RAW    yLOG_schar   (a_force);
   --rce;  if (x_type == POINT_NORMAL && a_force == POINT_NORMAL) {
      d = BAS__dist (x_curr, x_prev);
      DEBUG_RAW    yLOG_sdouble (d);
      if (d >= 0 && d <= s_append) {
         DEBUG_RAW    yLOG_snote   ("too close to prev");
         DEBUG_RAW    yLOG_sexitr  (__FUNCTION__, rce);
         return rce;
      }
      /*> d = BAS__dist (x_curr, x_pprev);                                           <* 
       *> DEBUG_RAW    yLOG_sdouble (d);                                              <* 
       *> if (d >= 0 && d <= s_append) {                                              <* 
       *>    DEBUG_RAW    yLOG_snote   ("too close to prev-prev");                    <* 
       *>    DEBUG_RAW    yLOG_sexitr  (__FUNCTION__, rce);                           <* 
       *>    return rce;                                                              <* 
       *> }                                                                           <*/
   }
   /*---(collapse last point)------------*/
   --rce;  if (a_force == POINT_SUFFIX) {
      if (x_prev >= a_raw) {
         DEBUG_RAW    yLOG_snote   ("last point collapsed");
         --o.nbas;
         --o.navg;
      } else {
         x_tail = a_raw + 5;
         d  = BAS__dist (x_curr, x_pprev);
         DEBUG_RAW    yLOG_sdouble (d);
         dt = BAS__dist (x_curr, x_tail);
         DEBUG_RAW    yLOG_sdouble (dt);
         pt = BAS__dist (x_prev, x_tail);
         DEBUG_RAW    yLOG_sdouble (pt);
         /*> if (pt <= dt) {                                                             <* 
          *>    DEBUG_AVG    yLOG_snote   ("prev after suffix");                         <* 
          *>    --o.nbas;                                                                <* 
          *>    --o.navg;                                                                <* 
          *> }                                                                           <* 
          *> else if (d <= s_append)  {                                                  <* 
          *>    DEBUG_AVG    yLOG_snote   ("collapse last");                             <* 
          *>    --o.nbas;                                                                <* 
          *>    --o.navg;                                                                <* 
          *> }                                                                           <*/
      }
   }
   /*---(add)----------------------------*/
   DEBUG_RAW    yLOG_sint    (o.nbas);
   DEBUG_RAW    yLOG_sint    (o.raw [x_curr].x_raw);
   DEBUG_RAW    yLOG_sint    (o.raw [x_curr].y_raw);
   DEBUG_RAW    yLOG_sint    (o.raw [x_curr].x_pos);
   DEBUG_RAW    yLOG_sint    (o.raw [x_curr].y_pos);
   /*---(typing)--------------------------*/
   o.bas [o.nbas].series  = POINTS_BAS;
   o.avg [o.navg].series  = POINTS_AVG;
   POINT_raw2bas  (x_curr, o.nbas);
   if (strchr (POINT_ALIGNER, a_force) != NULL) {
      o.bas [o.nbas].type = a_force;
      o.avg [o.navg].type = a_force;
   }
   /*---(counters)-----------------------*/
   ++o.nbas;
   ++o.navg;
   DEBUG_RAW    yLOG_sint    (o.nbas);
   /*---(report complete)----------------*/
   DEBUG_RAW    yLOG_sexit   (__FUNCTION__);
   /*---(check head)---------------------*/
   x_new = x_curr;
   if (o.raw [x_curr].type == POINT_HEAD)  x_new = BAS_append (x_curr + 5, POINT_PREFIX);
   /*---(complete)-----------------------*/
   return x_new;
}

char
BAS_insert              (short a_raw)
{
   /*---(locals)-----------+-----+-----+-*/
   char        rce         =  -10;
   int         i           =    0;
   short       n           =   -1;
   /*---(header)-------------------------*/
   DEBUG_RAW    yLOG_enter   (__FUNCTION__);
   DEBUG_RAW    yLOG_value   ("a_raw"     , a_raw);
   /*---(find point)---------------------*/
   --rce;  for (i = 0; i < o.nbas; ++i) {
      DEBUG_RAW    yLOG_complex ("compare"   , "%2d, %4d to %4d", i, a_raw, o.bas [i].p_raw);
      if (a_raw == o.bas [i].p_raw) {
         DEBUG_RAW    yLOG_exitr   (__FUNCTION__, rce);
         return rce;
      }
      if (a_raw >  o.bas [i].p_raw) {  DEBUG_RAW    yLOG_complex ("result"    , "%4d >  %4d", a_raw, o.bas [i].p_raw); }
      if (a_raw <  o.bas [i].p_raw) {  n = i; break;  }
   }
   /*---(check misses/dups)--------------*/
   DEBUG_RAW    yLOG_value   ("n"         , n);
   --rce;  if (n < 0) {
      DEBUG_RAW    yLOG_exitr   (__FUNCTION__, rce);
      return rce;
   }
   /*---(open slot)----------------------*/
   DEBUG_RAW    yLOG_note    ("open up slot");
   for (i = o.navg - 1; i >= n; --i) {
      BAS__push_up (i);
   }
   /*---(insert new point)---------------*/
   DEBUG_RAW    yLOG_note    ("copy data");
   POINT_raw2bas  (a_raw, n);
   ++o.nbas;
   ++o.navg;
   /*---(update values)------------------*/
   DEBUG_RAW    yLOG_note    ("recalculate");
   BAS_calc_all ();
   /*> POINT_list (stdout, 'd', o.bas, o.nbas);                                       <*/
   /*> POINT_list (stdout, 'd', o.avg, o.navg);                                       <*/
   /*---(complete)-----------------------*/
   DEBUG_RAW    yLOG_exit    (__FUNCTION__);
   return 0;
}

char
BAS_adjust              (short a_bas, short a_raw)
{
   /*---(locals)-----------+-----+-----+-*/
   short       x_old       =    0;
   short       x_key       =    0;
   /*---(header)-------------------------*/
   DEBUG_AVG    yLOG_senter  (__FUNCTION__);
   /*---(save values)--------------------*/
   x_old  = o.bas [a_bas].p_raw;
   DEBUG_AVG    yLOG_sint    (x_old);
   DEBUG_AVG    yLOG_sint    (a_raw);
   x_key  = o.bas [a_bas].p_key;
   /*---(turn off old)-------------------*/
   o.raw [x_old].p_bas  = -1;
   o.raw [x_old].p_key  = -1;
   /*---(update to new)------------------*/
   o.raw [a_raw].p_bas  = a_bas;
   o.raw [a_raw].p_key  = x_key;
   /*---(repoint bas)--------------------*/
   POINT_raw2bas  (a_raw, a_bas);
   BAS_calc_all ();
   /*---(complete)-----------------------*/
   DEBUG_AVG    yLOG_sexit   (__FUNCTION__);
   return 0;
}

char
BASE_handler            (short a_bef, short a_new, short a_aft)
{
   /*---(locals)-----------+-----+-----+-*/
   float       da, db, df;
   char        rc;
   short       x_bas;
   char        x_type;
   /*---(header)-------------------------*/
   DEBUG_AVG    yLOG_enter   (__FUNCTION__);
   /*---(filter)-------------------------*/
   if (a_bef == a_new || a_aft == a_new) {
      DEBUG_RAW    yLOG_exit    (__FUNCTION__);
      return 0;
   }
   /*---(get distances)------------------*/
   da   = BAS__dist (a_bef, a_new);
   db   = BAS__dist (a_new, a_aft);
   df   = BAS__dist (a_bef, a_aft);
   DEBUG_AVG    yLOG_complex (" dist"     , "%4.1fa, %4.1fb, %4.1ff", da, db, df);
   /*---(update)-------------------------*/
   if      (da <= s_adjust) {
      x_bas  = o.raw [a_bef].p_bas;
      x_type = o.bas [x_bas].type;
      DEBUG_AVG    yLOG_complex (" bef small", "%4d, %4d, %c", a_bef, x_bas, x_type);
      if (strchr ("S><F", x_type) != NULL)  BAS_insert (a_new);
      else                                  BAS_adjust (x_bas, a_new);
      rc = 0;
   } else if (db <= s_adjust) {
      x_bas  = o.raw [a_aft].p_bas;
      x_type = o.bas [x_bas].type;
      DEBUG_AVG    yLOG_complex (" aft small", "%4d, %4d, %c", a_bef, x_bas, x_type);
      if (strchr ("S><F", x_type) != NULL)  BAS_insert (a_new);
      else                                  BAS_adjust (x_bas, a_new);
      rc = 0;
   } else  {
      DEBUG_AVG    yLOG_note    (" middle addition");
      BAS_insert (a_new);
      rc = 1;
   }
   /*---(complete)-----------------------*/
   DEBUG_AVG    yLOG_exit    (__FUNCTION__);
   return rc;
}



/*============================--------------------============================*/
/*===----                    calculate statistics                      ----===*/
/*============================--------------------============================*/
static void o___STATISTICS_____________o (void) {;}

char
BASE__calc_copy      (short a_dst, short a_src)
{
   /*---(base data)----------------------*/
   o.bas [a_dst].slope = o.bas [a_src].slope;
   o.bas [a_dst].icept = o.bas [a_src].icept;
   o.bas [a_dst].rads  = o.bas [a_src].rads;
   o.bas [a_dst].degs  = o.bas [a_src].degs;
   o.bas [a_dst].quad  = o.bas [a_src].quad;
   /*---(average data)-------------------*/
   o.avg [a_dst].slope = o.avg [a_src].slope;
   o.avg [a_dst].icept = o.avg [a_src].icept;
   o.avg [a_dst].rads  = o.avg [a_src].rads;
   o.avg [a_dst].degs  = o.avg [a_src].degs;
   o.avg [a_dst].quad  = o.avg [a_src].quad;
   return 0;
}

char          /*----: calculate additional information on basic points -------*/
BAS_calc_all         (void)
{
   /*---(locals)-----------+-----+-----+-*/
   int         i;
   char        x_type      =  '-';
   /*---(calc real points)---------------*/
   for (i = 0; i < o.nbas; ++i) POINT_calc (POINTS_BAS, o.bas + i, 'n');
   for (i = 0; i < o.navg; ++i) POINT_calc (POINTS_AVG, o.avg + i, 'a');
   /*---(fill in bas data)---------------*/
   for (i = 0; i < o.navg; ++i) {
      x_type = o.bas [i].type;
      if (x_type == POINT_START)  BASE__calc_copy (i, i + 2);
      if (x_type == POINT_HEAD)   BASE__calc_copy (i, i + 1);
      if (x_type == POINT_TAIL)   BASE__calc_copy (i, i - 1);
      if (x_type == POINT_FINISH) BASE__calc_copy (i, i - 2);
   }
   /*---(complete)-----------------------*/
   return 0;
}



/*============================--------------------============================*/
/*===----                     adjusting endpoints                      ----===*/
/*============================--------------------============================*/
static void o___ENDS___________________o (void) {;}

char          /*----: move the beg/end points out to test circles ------------*/
BAS__push_out        (short a_bas, char a_dir)
{
   short       x_raw;
   short       x_off       =    1;
   short       x_radoff    =    2;
   float       x_rads      =  0.0;
   float       x_dist      =  0.0;
   float       x, y;
   switch (a_dir) {
   case '-' :  x_radoff =  1;  x_off =  1;  x_dist = -15.0;  break;
   case '+' :  x_radoff = -1;  x_off = -1;  x_dist =  15.0;  break;
   default  :  return -1;                                    break;
   }
   /*---(get proper rads)----------------*/
   x_rads = o.avg [a_bas + x_off].rads;
   /*---(get raw point xy)---------------*/
   x_raw  = o.bas [a_bas + x_off].p_raw;
   x = o.raw [x_raw].x_raw;
   y = o.raw [x_raw].y_raw;
   /*---(calculate)----------------------*/
   o.bas [a_bas].x_raw  = o.avg [a_bas].x_raw  = x + (x_dist * my.ratio * cos (x_rads));
   o.bas [a_bas].y_raw  = o.avg [a_bas].y_raw  = y + (x_dist * my.ratio * sin (x_rads));
   DEBUG_AVG   yLOG_complex ("new ends"  , "%3d %c, %8.3f, from %4dx %4dy, to %4dx %4dy", a_bas, a_dir, x_rads, x, y, o.bas [a_bas].x_raw, o.bas [a_bas].y_raw);
   /*---(recalc)------------------*/
   POINT_pos   (o.bas + a_bas);
   POINT_calc  (POINTS_BAS, o.bas + a_bas, 'n');
   POINT_pos   (o.avg + a_bas);
   POINT_calc  (POINTS_AVG, o.avg + a_bas, 'a');
   /*---(complete)-----------------------*/
   return 0;
}

char          /*----: move the beg/end points out to test circles ------------*/
BAS__pull_in         (short a_bas)
{
   short       x_raw;
   float       x, y;
   x_raw = o.bas [a_bas].p_raw;
   x = o.raw [x_raw].x_raw;
   y = o.raw [x_raw].y_raw;
   o.bas [a_bas].x_raw  = o.avg [a_bas].x_raw  = x;
   o.bas [a_bas].y_raw  = o.avg [a_bas].y_raw  = y;
   /*---(recalc)------------------*/
   POINT_pos   (o.bas + a_bas);
   POINT_calc  (POINTS_BAS, o.bas + a_bas, 'n');
   POINT_pos   (o.avg + a_bas);
   POINT_calc  (POINTS_AVG, o.avg + a_bas, 'a');
   /*---(complete)-----------------------*/
   return 0;
}

char          /*----: move the beg/end points out to test circles ------------*/
BAS_extend_ends      (void)
{
   /*---(locals)-------------------------*/
   int       i         = 0;            /* loop iterator -- bas points         */
   /*---(header)-------------------------*/
   DEBUG_AVG    yLOG_enter   (__FUNCTION__);
   /*---(beginning)----------------------*/
   for (i = 0; i < o.nbas; ++i) {
      if (o.avg [i].type == POINT_START)   BAS__push_out (i, '-');
      if (o.avg [i].type == POINT_FINISH)  BAS__push_out (i, '+');
   }
   /*---(complete)-----------------------*/
   DEBUG_AVG    yLOG_exit    (__FUNCTION__);
   return 0;
}

char
BAS_retract_ends     (void)
{
   /*---(locals)-------------------------*/
   int       i         = 0;            /* loop iterator -- bas points         */
   /*---(header)-------------------------*/
   DEBUG_AVG    yLOG_enter   (__FUNCTION__);
   /*---(beginning)----------------------*/
   for (i = 0; i < o.nbas; ++i) {
      if (o.avg [i].type == POINT_START)   BAS__pull_in  (i);
      if (o.avg [i].type == POINT_FINISH)  BAS__pull_in  (i);
   }
   /*---(complete)-----------------------*/
   DEBUG_AVG    yLOG_exit    (__FUNCTION__);
   return 0;
}



/*============================--------------------============================*/
/*===----                      speciality routines                     ----===*/
/*============================--------------------============================*/
static void o___SPECIALTY______________o (void) {;}

char
BAS_add_extremes        (void)
{
   /*---(locals)-----------+-----+-----+-*/
   char        rc          =    0;
   short       b           =    0;
   short       r           =    0;
   char        x_type      =  '-';
   char        q1, q2;
   short       x_beg, x_end;
   short       x_max, x_min;
   short       y_max, y_min;
   char        qq          =     0;
   short       x_new       =     0;
   float       xd, yd, da, db, df;
   short       x_bas       =     0;
   /*---(header)-------------------------*/
   DEBUG_AVG    yLOG_enter   (__FUNCTION__);
   /*---(find inflections)---------------*/
   for (b = 0; b < o.nbas - 1; ++b) {
      /*---(prepare)-----------*/
      x_type = o.avg [b].type;
      q1     = o.avg [b - 1].quad;
      q2     = o.avg [b    ].quad;
      x_beg  = o.avg [b - 1].p_raw;
      x_end  = o.avg [b    ].p_raw;
      DEBUG_AVG    yLOG_complex ("BEG_END"   , "%3d:%3d %d, %3d:%3d %d, %c", b - 1, x_beg, q1, b, x_end, q2, x_type);
      /*---(filter)------------*/
      if (strchr ("SF", o.avg [b].type) != NULL)  continue;
      if (q1 == q2)                               continue;
      /*---(find min/max)------*/
      x_max = x_min = y_max = y_min = x_beg;
      for (r = x_beg; r <= x_end; ++r) {
         if (o.raw [r].x_raw < o.raw [x_min].x_raw)   x_min = r;
         if (o.raw [r].x_raw > o.raw [x_max].x_raw)   x_max = r;
         if (o.raw [r].y_raw < o.raw [y_min].y_raw)   y_min = r;
         if (o.raw [r].y_raw > o.raw [y_max].y_raw)   y_max = r;
      }
      DEBUG_AVG    yLOG_complex (" min/max"  , "%3dxn, %3dxx, %3dyn, %3dyx", x_min, x_max, y_min, y_max);
      /*---(pick best)---------*/
      qq = q1 * 10 + q2;
      switch (qq) {
      case 12 :  case 13 :  x_new = x_max;  break;
      case 14 :             x_new = y_max;  break;
      case 21 :             x_new = x_min;  break;
      case 23 :  case 24 :  x_new = y_max;  break;
      case 31 :  case 32 :  x_new = y_min;  break;
      case 34 :             x_new = x_min;  break;
      case 41 :  case 42 :  x_new = y_min;  break;
      case 43 :             x_new = x_max;  break;
      default :             x_new = -1;     break;
      }
      DEBUG_AVG    yLOG_complex (" best"     , "%2dqq at %3d at %4dx, %4dy", qq, x_new, o.raw [x_new].x_raw, o.raw [x_new].y_raw);
      /*---(add)---------------*/
      b += BASE_handler (x_beg, x_new, x_end);
      /*---(mark key)----------*/
      x_bas = o.raw [x_new].p_bas;
      o.bas [x_bas].prekey = 'k';
      o.avg [x_bas].prekey = 'k';
      /*---(done)--------------*/
   }
   /*---(complete)-----------------------*/
   DEBUG_AVG    yLOG_exit    (__FUNCTION__);
   return 0;
}

char
BAS_mark_sharps         (void)
{
   /*---(locals)-----------+-----+-----+-*/
   char        rc          =    0;
   short       b           =    0;
   short       r           =    0;
   char        x_type      =  '-';
   float       d1, d2, dd;
   /*---(header)-------------------------*/
   DEBUG_AVG    yLOG_enter   (__FUNCTION__);
   /*---(find inflections)---------------*/
   for (b = 1; b < o.nbas - 1; ++b) {
      /*---(prepare)-----------*/
      x_type = o.bas [b].type;
      d1     = o.bas [b    ].degs;
      d2     = o.bas [b + 1].degs;
      /*---(get difference)----*/
      dd     = fabs (d1 - d2);
      if (dd > 180) dd = 360 - dd;
      /*> DEBUG_AVG    yLOG_complex ("BEF_AFT"   , "#%3d %4.0fd, %3d %4.0fd, %3d %4.0fd, %c, %4.0f", b - 1, d1, b, o.avg [b].degs, b + 1, d2, x_type, dd);   <*/
      DEBUG_AVG    yLOG_complex ("BEF_AFT"   , "%3d %4.0fd, %3d %4.0fd, %c, %4.0f", b, d1, b + 1, d2, x_type, dd);
      /*---(mark key)----------*/
      if (dd >= s_sharp) {
         DEBUG_AVG    yLOG_note    ("marking point");
         o.bas [b].sharp  = 'x';
         o.avg [b].sharp  = 'x';
      }
      /*---(done)--------------*/
   }
   /*---(complete)-----------------------*/
   DEBUG_AVG    yLOG_exit    (__FUNCTION__);
   return 0;
}



/*============================--------------------============================*/
/*===----                         main driver                          ----===*/
/*============================--------------------============================*/
static void o___DRIVER_________________o (void) {;}

char          /*----: filter raw points into basic points --------------------*/
BAS_filter         (void)
{
   /*---(locals)-----------+-----+-----+-*/
   short       x_new       =   -1;
   int         i           =    0;
   char        x_type      =  '-';
   char        x_prev      =  '-';
   float       d           =  0.0;
   /*---(header)-------------------------*/
   DEBUG_AVG    yLOG_enter   (__FUNCTION__);
   /*---(find additional points)---------*/
   for (i = 0; i < o.nraw; ++i) {
      /*---(prepare)------------------------*/
      if (o.nbas > 0)  x_prev  = o.bas [o.nbas - 1].type;
      x_type = o.raw [i].type;
      /*---(normal)----------------------*/
      x_new = BAS_append (i, POINT_NORMAL);
      if (x_new >= 0)  i = x_new;
      /*---(done)------------------------*/
   }
   /*---(end points)---------------------*/
   DEBUG_AVG    yLOG_value   ("o.nbas"    , o.nbas);
   /*---(run calculations)---------------*/
   /*> BAS_calc_all      ();                                                         <*/
   /*> BASE__sharpen ();                                                              <*/
   BAS_calc_all ();
   /*---(display version)----------------*/
   /*> for (i = 0; i < o.navg; ++i) {                                                 <* 
    *>    POINT_display (o.bas + i);                                                  <* 
    *>    POINT_display (o.avg + i);                                                  <* 
    *> }                                                                              <*/
   /*---(complete)-----------------------*/
   DEBUG_AVG    yLOG_exit    (__FUNCTION__);
   return 0;
}



/*============================--------------------============================*/
/*===----                        yVIKEYS mapping                       ----===*/
/*============================--------------------============================*/
static void o___MAPPING________________o (void) {;}

/*> char                                                                              <* 
 *> BASE_map_update          (tMAPPED *a_map, int a_ycur, int a_ynew)                 <* 
 *> {                                                                                 <* 
 *>    /+---(locals)-----------+-----------+-+/                                       <* 
 *>    int         i           =    0;                                                <* 
 *>    /+---(header)-------------------------+/                                       <* 
 *>    DEBUG_MAP    yLOG_enter   (__FUNCTION__);                                      <* 
 *>    /+---(update map)---------------------+/                                       <* 
 *>    for (i = 0; i < o.navg; ++i) {                                                 <* 
 *>       a_map->map [i] = i;                                                         <* 
 *>    }                                                                              <* 
 *>    /+---(globals)------------------------+/                                       <* 
 *>    a_map->gmin   = a_map->gamin  = 0;                                             <* 
 *>    a_map->gmax   = a_map->gamax  = o.navg - 1;                                    <* 
 *>    a_map->uavail = o.navg;                                                        <* 
 *>    /+---(screen)-------------------------+/                                       <* 
 *>    a_map->umin   = a_map->ubeg   = 0;                                             <* 
 *>    a_map->umax   = a_map->uend   = a_map->ulen   = a_map->utend  = o.navg - 1;    <* 
 *>    /+---(units)--------------------------+/                                       <* 
 *>    a_map->gbeg   = 0;                                                             <* 
 *>    a_map->gend   = o.navg - 1;                                                    <* 
 *>    /+---(update current)-----------------+/                                       <* 
 *>    if (a_ycur == a_ynew) {                                                        <* 
 *>       o.cavg       = a_map->gcur;                                                 <* 
 *>    } else {                                                                       <* 
 *>       a_map->ucur  = 0;                                                           <* 
 *>       a_map->gcur  = 0;                                                           <* 
 *>    }                                                                              <* 
 *>    /+---(complete)-----------------------+/                                       <* 
 *>    DEBUG_MAP    yLOG_exit    (__FUNCTION__);                                      <* 
 *>    return 0;                                                                      <* 
 *> }                                                                                 <*/



