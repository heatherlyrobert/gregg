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
static float   s_append    =  4.0;
static float   s_adjust    =  0.5;


/*============================--------------------============================*/
/*===----                        small support                         ----===*/
/*============================--------------------============================*/
static void o___SUPPORT________________o (void) {;}

float         /*----: check distance to potential point ----------------------*/
BASE__dist              (short a, short b)
{
   float       xd, yd, d;
   xd   = fabs (o.raw [a].x_pos - o.raw [b].x_pos);
   yd   = fabs (o.raw [a].y_pos - o.raw [b].y_pos);
   d    = sqrt ((xd * xd) + (yd * yd));
   DEBUG_AVG_M  yLOG_complex ("dist"      , "%4d (%4d,%4d) to %4d (%4d,%4d) = %4.1fx, %4.1fy, %4.1fd", a, o.raw [a].x_raw, o.raw [a].y_raw, b, o.raw [b].x_raw, o.raw [b].y_raw, xd, yd, d);
   return d;
}



/*============================--------------------============================*/
/*===----                      adding raw points                       ----===*/
/*============================--------------------============================*/
static void o___NEW____________________o (void) {;}

char
BASE__raw2bas       (short a_raw, short a_bas)
{
   /*---(tie raw and bas)-----------------*/
   o.raw [a_raw].p_bas    = a_bas;
   o.bas [a_bas].p_raw   = o.avg [a_bas].p_raw   = a_raw;
   o.bas [a_bas].p_bas   = o.avg [a_bas].p_bas   = a_bas;
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

char          /*----: add an additional basic point from a raw point ---------*/
BASE_append             (short a_raw)
{
   /*---(locals)-----------+-----+-----+-*/
   char        rce         =    0;
   int         x_dup       =    0;
   float       d           =  0.0;
   float       xd, yd;
   /*---(header)-------------------------*/
   DEBUG_RAW    yLOG_senter  (__FUNCTION__);
   DEBUG_RAW    yLOG_sint    (a_raw);
   /*---(filter duplicates)--------------*/
   DEBUG_RAW    yLOG_sint    (o.bas [o.nbas - 1].p_raw);
   if (o.nbas > 0 && a_raw == o.bas [o.nbas - 1].p_raw) {
      DEBUG_RAW    yLOG_sexitr  (__FUNCTION__, rce);
      return rce;
   }
   /*---(if taken already)---------------*/
   DEBUG_RAW    yLOG_sint    (o.raw [a_raw].p_bas);
   --rce;  if (o.raw [a_raw].p_bas >= 0) {
      DEBUG_RAW    yLOG_snote   ("already a base point");
      DEBUG_RAW    yLOG_sexit   (__FUNCTION__);
      return 0;
   }
   /*---(filter too close)---------------*/
   DEBUG_RAW    yLOG_schar   (o.raw [a_raw].type);
   --rce;  if (strchr ("S><F", o.raw [a_raw].type) == NULL) {
      d = BASE__dist (a_raw, o.bas [o.nbas - 1].p_raw);
      DEBUG_RAW    yLOG_sdouble (d);
      if (d <= s_append) {
         DEBUG_RAW    yLOG_snote   ("too close to last point");
         DEBUG_RAW    yLOG_sexitr  (__FUNCTION__, rce);
         return rce;
      }
   }
   /*---(add)----------------------------*/
   DEBUG_RAW    yLOG_sint    (o.nbas);
   DEBUG_RAW    yLOG_sint    (o.raw [a_raw].x_raw);
   DEBUG_RAW    yLOG_sint    (o.raw [a_raw].y_raw);
   DEBUG_RAW    yLOG_sint    (o.raw [a_raw].x_pos);
   DEBUG_RAW    yLOG_sint    (o.raw [a_raw].y_pos);
   /*---(typing)--------------------------*/
   o.bas [o.nbas].series  = POINTS_BAS;
   o.avg [o.navg].series  = POINTS_AVG;
   BASE__raw2bas  (a_raw, o.nbas);
   /*---(counters)-----------------------*/
   ++o.nbas;
   ++o.navg;
   DEBUG_RAW    yLOG_sint    (o.nbas);
   /*---(complete)-----------------------*/
   DEBUG_RAW    yLOG_sexit   (__FUNCTION__);
   return 0;
}

char
BASE_insert             (short a_raw)
{
   /*---(locals)-----------+-----+-----+-*/
   char        rce         =  -10;
   int         i           =    0;
   short       n           =   -1;
   /*---(header)-------------------------*/
   DEBUG_RAW    yLOG_enter   (__FUNCTION__);
   DEBUG_RAW    yLOG_value   ("a_raw"     , a_raw);
   /*---(find point)---------------------*/
   for (i = 0; i < o.nbas; ++i) {
      DEBUG_RAW    yLOG_complex ("compare"   , "%2d, %4d to %4d", i, a_raw, o.bas [i].p_raw);
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
      BASE_push_up (i);
   }
   /*---(insert new point)---------------*/
   DEBUG_RAW    yLOG_note    ("copy data");
   BASE__raw2bas  (a_raw, n);
   ++o.nbas;
   ++o.navg;
   /*---(update values)------------------*/
   DEBUG_RAW    yLOG_note    ("recalculate");
   BASE_calc_all ();
   /*---(complete)-----------------------*/
   DEBUG_RAW    yLOG_exit    (__FUNCTION__);
   return 0;
}

char
BASE_adjust             (short a_bas, short a_raw)
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
   BASE__raw2bas  (a_raw, a_bas);
   BASE_calc_all ();
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
   /*---(get distances)------------------*/
   da   = BASE__dist (a_bef, a_new);
   db   = BASE__dist (a_new, a_aft);
   df   = BASE__dist (a_bef, a_aft);
   DEBUG_AVG    yLOG_complex (" dist"     , "%4.1fa, %4.1fb, %4.1ff", da, db, df);
   /*---(update)-------------------------*/
   if      (da <= s_adjust) {
      x_bas  = o.raw [a_bef].p_bas;
      x_type = o.bas [x_bas].type;
      DEBUG_AVG    yLOG_complex (" bef small", "%4d, %4d, %c", a_bef, x_bas, x_type);
      if (strchr ("S><F", x_type) != NULL)  BASE_insert (a_new);
      else                                  BASE_adjust (x_bas, a_new);
      rc = 0;
   } else if (db <= s_adjust) {
      x_bas  = o.raw [a_aft].p_bas;
      x_type = o.bas [x_bas].type;
      DEBUG_AVG    yLOG_complex (" aft small", "%4d, %4d, %c", a_bef, x_bas, x_type);
      if (strchr ("S><F", x_type) != NULL)  BASE_insert (a_new);
      else                                  BASE_adjust (x_bas, a_new);
      rc = 0;
   } else  {
      DEBUG_AVG    yLOG_note    (" middle addition");
      BASE_insert (a_new);
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

char          /*----: calculate additional information on basic points -------*/
BASE_calc_all        (void)
{
   /*---(locals)-----------+-----+-----+-*/
   int i;
   /*---(calc real points)---------------*/
   for (i = 0; i < o.nbas; ++i) POINT_calc (POINTS_BAS, o.bas + i, 'n');
   for (i = 0; i < o.navg; ++i) POINT_calc (POINTS_AVG, o.avg + i, 'a');
   /*---(fill in bas data)---------------*/
   o.bas [0].slope = o.bas [1].slope = o.bas [2].slope;
   o.bas [0].icept = o.bas [1].icept = o.bas [2].icept;
   o.bas [0].rads  = o.bas [1].rads  = o.bas [2].rads;
   o.bas [0].degs  = o.bas [1].degs  = o.bas [2].degs;
   o.bas [0].quad = o.bas [1].quad = o.bas [2].quad;
   o.bas [o.nbas - 1].slope = o.bas [o.navg - 2].slope;
   o.bas [o.nbas - 1].icept = o.bas [o.navg - 2].icept;
   o.bas [o.nbas - 1].rads = o.bas [o.navg - 2].rads;
   o.bas [o.nbas - 1].degs = o.bas [o.navg - 2].degs;
   o.bas [o.nbas - 1].quad = o.bas [o.navg - 2].quad;
   /*---(fill in avg data)---------------*/
   o.avg [0].slope = o.avg [1].slope = o.avg [2].slope;
   o.avg [0].icept = o.avg [1].icept = o.avg [2].icept;
   o.avg [0].rads = o.avg [1].rads = o.avg [2].rads;
   o.avg [0].degs = o.avg [1].degs = o.avg [2].degs;
   o.avg [0].quad = o.avg [1].quad = o.avg [2].quad;
   o.avg [o.navg - 1].slope = o.avg [o.navg - 2].slope;
   o.avg [o.navg - 1].icept = o.avg [o.navg - 2].icept;
   o.avg [o.navg - 1].rads = o.avg [o.navg - 2].rads;
   o.avg [o.navg - 1].degs = o.avg [o.navg - 2].degs;
   o.avg [o.navg - 1].quad = o.avg [o.navg - 2].quad;
   /*---(complete)-----------------------*/
   return 0;
}



/*============================--------------------============================*/
/*===----                     adjusting endpoints                      ----===*/
/*============================--------------------============================*/
static void o___ENDS___________________o (void) {;}

char          /*----: move the beg/end points out to test circles ------------*/
BASE__push_out       (short a_bas, char a_dir)
{
   int         x_off       =    1;
   int         x_radoff    =    2;
   float       x_rads      =  0.0;
   float       x_dist      =  0.0;
   float       x, y;
   switch (a_dir) {
   case '-' :  x_radoff =  2;  x_off =  1;  x_dist = -15.0;  break;
   case '+' :  x_radoff = -1;  x_off = -1;  x_dist =  15.0;  break;
   default  :  return -1;                                    break;
   }
   x_rads = o.avg [a_bas + x_radoff].rads;
   x = o.bas [a_bas + x_off].x_raw;
   y = o.bas [a_bas + x_off].y_raw;
   o.bas [a_bas].x_raw  = o.avg [a_bas].x_raw  = o.bas [a_bas + x_off].x_raw + (x_dist * my.ratio * cos (x_rads));
   o.bas [a_bas].y_raw  = o.avg [a_bas].y_raw  = o.bas [a_bas + x_off].y_raw + (x_dist * my.ratio * sin (x_rads));
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
BASE__pull_in        (short a_bas)
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
BASE_extend_ends     (void)
{
   /*---(locals)-------------------------*/
   int       i         = 0;            /* loop iterator -- bas points         */
   /*---(header)-------------------------*/
   DEBUG_AVG    yLOG_enter   (__FUNCTION__);
   /*---(beginning)----------------------*/
   for (i = 0; i < o.nbas; ++i) {
      if (o.avg [i].type == POINT_START)   BASE__push_out (i, '-');
      if (o.avg [i].type == POINT_FINISH)  BASE__push_out (i, '+');
   }
   /*---(complete)-----------------------*/
   DEBUG_AVG    yLOG_exit    (__FUNCTION__);
   return 0;
}

char
BASE_retract_ends    (void)
{
   /*---(locals)-------------------------*/
   int       i         = 0;            /* loop iterator -- bas points         */
   /*---(header)-------------------------*/
   DEBUG_AVG    yLOG_enter   (__FUNCTION__);
   /*---(beginning)----------------------*/
   for (i = 0; i < o.nbas; ++i) {
      if (o.avg [i].type == POINT_START)   BASE__pull_in  (i);
      if (o.avg [i].type == POINT_FINISH)  BASE__pull_in  (i);
   }
   /*---(complete)-----------------------*/
   DEBUG_AVG    yLOG_exit    (__FUNCTION__);
   return 0;
}



/*============================--------------------============================*/
/*===----                      speciality routines                     ----===*/
/*============================--------------------============================*/
static void o___SPECIALTY______________o (void) {;}

/*> char          /+----: push bas points as far as possible into sharp corners --+/          <* 
 *> BASE__sharpen        (void)                                                               <* 
 *> {                                                                                         <* 
 *>    /+---(locals)-------------------------+/                                               <* 
 *>    int       d1, d2, dd;                    /+ calculated angles              +/          <* 
 *>    int       sharp     = 70;                /+ magnitude of sharp break       +/          <* 
 *>    int       i         = 0;                 /+ loop iterator -- basic points  +/          <* 
 *>    /+---(process)------------------------+/                                               <* 
 *>    DEBUG_AVG   printf("   sharp tolerance set to >= %d\n", sharp);                        <* 
 *>    for (i = 1; i < o.nbas - 1; ++i) {                                                     <* 
 *>       if (o.avg[i].type == POINT_START || o.avg[i - 1].type == POINT_START) continue;     <* 
 *>       if (o.avg[i].type == POINT_FINISH || o.avg[i + 1].type == POINT_FINISH) continue;   <* 
 *>       if (o.avg[i - 1].quad == 0)                        continue;                        <* 
 *>       /+---(calculate)-------------------+/                                               <* 
 *>       d1     = o.avg[i - 1].degs;                                                         <* 
 *>       d2     = o.avg[i + 1].degs;                                                         <* 
 *>       dd     = d1 - d2;                                                                   <* 
 *>       if (dd >  180) dd -= 360;                                                           <* 
 *>       if (dd < -180) dd += 360;                                                           <* 
 *>       DEBUG_AVG   printf("   %3d : %5d vs. %5d, so %5d   ", i, d1, d2, abs(dd));          <* 
 *>       o.bas[i].cano = o.avg[i].cano = '-';                                                <* 
 *>       /+---(check)-----------------------+/                                               <* 
 *>       if (abs(dd) >= sharp) {                                                             <* 
 *>          o.bas[i].cano = o.avg[i].cano = 'x';                                             <* 
 *>          DEBUG_AVG   printf("SHARP\n");                                                   <* 
 *>       } else {                                                                            <* 
 *>          DEBUG_AVG   printf("no\n");                                                      <* 
 *>       }                                                                                   <* 
 *>    }                                                                                      <* 
 *>    /+---(complete)-----------------------+/                                               <* 
 *>    return 0;                                                                              <* 
 *> }                                                                                         <*/

char
BASE_add_corners        (void)
{
   /*---(locals)-----------+-----+-----+-*/
   char        rc          =    0;
   short       b           =    0;
   short       r           =    0;
   char        q1, q2;
   short       x_beg, x_end;
   short       x_max, x_min;
   short       y_max, y_min;
   char        qq          =     0;
   short       x_new       =     0;
   float       xd, yd, da, db, df;
   /*---(header)-------------------------*/
   DEBUG_AVG    yLOG_enter   (__FUNCTION__);
   /*---(find inflections)---------------*/
   for (b = 0; b < o.nbas - 1; ++b) {
      /*---(prepare)-----------*/
      q1 = o.bas [b - 1].quad;
      q2 = o.bas [b    ].quad;
      /*---(filter)------------*/
      if (strchr ("S<F", o.bas [b].type) != NULL)  continue;
      if (q1 == q2)                                continue;
      /*---(find sharpest)-----*/
      x_beg = o.bas [b - 1].p_raw;
      x_end = o.bas [b    ].p_raw;
      DEBUG_AVG    yLOG_complex ("BEG_END"   , "%3d:%3d to %3d:%3d", b - 1, x_beg, b, x_end);
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
      if (x_new == x_beg || x_new == x_end)  continue;
      /*---(see distances)-----*/
      b += BASE_handler (x_beg, x_new, x_end);
      /*> da   = BASE__dist (x_new, x_beg);                                           <*/
      /*> xd   = fabs (o.raw [x_new].x_pos - o.raw [x_beg].x_pos);                    <* 
       *> yd   = fabs (o.raw [x_new].y_pos - o.raw [x_beg].y_pos);                    <* 
       *> da   = sqrt ((xd * xd) + (yd * yd));                                        <*/
      /*> db   = BASE__dist (x_end, x_new);                                           <*/
      /*> xd   = fabs (o.raw [x_end].x_pos - o.raw [x_new].x_pos);                    <* 
       *> yd   = fabs (o.raw [x_end].y_pos - o.raw [x_new].y_pos);                    <* 
       *> db   = sqrt ((xd * xd) + (yd * yd));                                        <*/
      /*> df   = BASE__dist (x_end, x_beg);                                           <*/
      /*> xd   = fabs (o.raw [x_end].x_pos - o.raw [x_beg].x_pos);                    <* 
       *> yd   = fabs (o.raw [x_end].y_pos - o.raw [x_beg].y_pos);                    <* 
       *> df   = sqrt ((xd * xd) + (yd * yd));                                        <*/
      /*> DEBUG_AVG    yLOG_complex (" dist"     , "%4.1fa, %4.1fb, %4.1ff", da, db, df);   <*/
      /*---(update)------------*/
      /*> if      (da <= s_adjust)  BASE_adjust (b - 1, x_new);                       <* 
       *> else if (db <= s_adjust)  BASE_adjust (b    , x_new);                       <* 
       *> else {                                                                      <* 
       *>    BASE_insert (x_new);                                                     <* 
       *>    ++b;                                                                     <* 
       *> }                                                                           <*/
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
BASE_filter        (void)
{
   /*---(locals)-----------+-----+-----+-*/
   int         i           =    0;
   char        x_prev      =  '-';
   float       d           =  0.0;
   /*---(header)-------------------------*/
   DEBUG_AVG    yLOG_enter   (__FUNCTION__);
   /*---(beg points)---------------------*/
   o.nbas = o.navg = 0;
   /*---(find additional points)---------*/
   for (i = 0; i < o.nraw; ++i) {
      /*---(prepare)------------------------*/
      if (o.nbas > 0)  x_prev  = o.bas [o.nbas - 1].type;
      /*---(check collapse)--------------*/
      if (o.raw [i].type == POINT_TAIL && x_prev == POINT_NORMAL) {
         d  = BASE__dist  (i, i - 2);
         DEBUG_AVG    yLOG_double  ("to tail"   , d);
         if (d <= 4.5) {
            DEBUG_AVG    yLOG_note    ("collapsing last normal before tail");
            --o.nbas;
            --o.navg;
         }
      }
      /*---(normal)----------------------*/
      BASE_append (i);
      /*---(done)------------------------*/
   }
   /*---(end points)---------------------*/
   DEBUG_AVG    yLOG_value   ("o.nbas"    , o.nbas);
   /*---(run calculations)---------------*/
   BASE_calc_all      ();
   BASE_extend_ends  ();
   /*> BASE__sharpen ();                                                              <*/
   BASE_calc_all ();
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

char
BASE_map_update          (tMAPPED *a_map, int a_ycur, int a_ynew)
{
   /*---(locals)-----------+-----------+-*/
   int         i           =    0;
   /*---(header)-------------------------*/
   DEBUG_MAP    yLOG_enter   (__FUNCTION__);
   /*---(update map)---------------------*/
   for (i = 0; i < o.navg; ++i) {
      a_map->map [i] = i;
   }
   /*---(globals)------------------------*/
   a_map->gmin   = a_map->gamin  = 0;
   a_map->gmax   = a_map->gamax  = o.navg - 1;
   a_map->uavail = o.navg;
   /*---(screen)-------------------------*/
   a_map->umin   = a_map->ubeg   = 0;
   a_map->umax   = a_map->uend   = a_map->ulen   = a_map->utend  = o.navg - 1;
   /*---(units)--------------------------*/
   a_map->gbeg   = 0;
   a_map->gend   = o.navg - 1;
   /*---(update current)-----------------*/
   if (a_ycur == a_ynew) {
      o.cavg       = a_map->gcur;
   } else {
      a_map->ucur  = 0;
      a_map->gcur  = 0;
   }
   /*---(complete)-----------------------*/
   DEBUG_MAP    yLOG_exit    (__FUNCTION__);
   return 0;
}



