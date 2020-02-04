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


/*============================--------------------============================*/
/*===----                        small support                         ----===*/
/*============================--------------------============================*/
static void o___SUPPORT________________o (void) {;}

int           /*----: check distance to potential point ----------------------*/
BASE__distance     (int n, int a_off)
{
   float     xd, yd;
   xd   = fabs (o.raw [n].x_pos - o.bas [o.nbas - a_off].x_pos);
   yd   = fabs (o.raw [n].y_pos - o.bas [o.nbas - a_off].y_pos);
   return sqrt ((xd * xd) + (yd * yd));
}



/*============================--------------------============================*/
/*===----                      adding raw points                       ----===*/
/*============================--------------------============================*/
static void o___NEW____________________o (void) {;}

char          /*----: add an additional basic point from a raw point ---------*/
BASE__point          (int n)
{
   /*---(locals)-----------+-----+-----+-*/
   char        rce         =    0;
   int         x_dup       =    0;
   float       d           =  0.0;
   /*---(header)-------------------------*/
   DEBUG_RAW    yLOG_senter  (__FUNCTION__);
   DEBUG_RAW    yLOG_sint    (n);
   /*---(filter duplicates)--------------*/
   DEBUG_RAW    yLOG_sint    (o.bas [o.nbas - 1].p_raw);
   if (o.nbas > 0 && n == o.bas [o.nbas - 1].p_raw) {
      DEBUG_RAW    yLOG_sexitr  (__FUNCTION__, rce);
      return rce;
   }
   /*---(filter too close)---------------*/
   DEBUG_RAW    yLOG_schar   (o.raw [n].type);
   --rce;  if (strchr ("S><F", o.raw [n].type) == NULL) {
      d = BASE__distance (n, -1);
      DEBUG_RAW    yLOG_sdouble (d);
      if (d <= 3.0) {
         DEBUG_RAW    yLOG_sexitr  (__FUNCTION__, rce);
         return rce;
      }
   }
   /*---(add)----------------------------*/
   DEBUG_RAW    yLOG_sint    (o.nbas);
   DEBUG_RAW    yLOG_sint    (o.raw [n].x_raw);
   DEBUG_RAW    yLOG_sint    (o.raw [n].y_raw);
   /*---(head)----------------------------*/
   o.bas [o.nbas].series  = POINTS_BAS;
   o.avg [o.navg].series  = POINTS_AVG;
   o.bas [o.nbas].seq     = o.avg [o.navg].seq     = o.nbas;
   o.bas [o.nbas].p_raw   = o.avg [o.navg].p_raw   = n;
   o.bas [o.nbas].p_bas   = o.avg [o.navg].p_bas   = o.nbas;
   o.bas [o.nbas].fake    = o.avg [o.navg].fake    = o.raw [n].fake;
   o.bas [o.nbas].type    = o.avg [o.navg].type    = o.raw [n].type;
   /*---(touchpad)------------------------*/
   o.bas [o.nbas].x_touch = o.avg [o.navg].x_touch = o.raw [n].x_touch;
   o.bas [o.nbas].x_raw   = o.avg [o.navg].x_raw   = o.raw [n].x_raw;
   o.bas [o.nbas].y_touch = o.avg [o.navg].y_touch = o.raw [n].y_touch;
   o.bas [o.nbas].y_raw   = o.avg [o.navg].y_raw   = o.raw [n].y_raw;
   /*---(display)-------------------------*/
   o.bas [o.nbas].x_pos   = o.avg [o.navg].x_pos   = o.raw [n].x_pos;
   o.bas [o.nbas].x_rel   = o.avg [o.navg].x_rel   = o.raw [n].x_rel;
   o.bas [o.nbas].y_pos   = o.avg [o.navg].y_pos   = o.raw [n].y_pos;
   o.bas [o.nbas].y_rel   = o.avg [o.navg].y_rel   = o.raw [n].y_rel;
   /*---(counters)-----------------------*/
   ++o.nbas;
   ++o.navg;
   DEBUG_RAW    yLOG_sint    (o.nbas);
   /*---(complete)-----------------------*/
   DEBUG_RAW    yLOG_sexit   (__FUNCTION__);
   return 0;
}

char          /*----: calculate additional information on basic points -------*/
BASE__calc           (void)
{
   /*---(locals)-----------+-----+-----+-*/
   int i;
   /*---(calc real points)---------------*/
   for (i = 0; i < o.nbas; ++i)     POINT_calc (POINTS_BAS, o.bas + i, 'n');
   for (i = 0; i < o.navg; ++i)     POINT_calc (POINTS_AVG, o.avg + i, 'a');
   /*---(fill in bas data)---------------*/
   /*> o.bas [0].slope = o.bas [1].slope = o.bas [2].slope;                           <* 
    *> o.bas [0].icept = o.bas [1].icept = o.bas [2].icept;                           <* 
    *> o.bas [0].rads  = o.bas [1].rads  = o.bas [2].rads;                            <* 
    *> o.bas [0].degs  = o.bas [1].degs  = o.bas [2].degs;                            <* 
    *> o.bas [0].quad = o.bas [1].quad = o.bas [2].quad;                              <* 
    *> o.bas [o.nbas - 1].slope = o.bas [o.navg - 2].slope;                           <* 
    *> o.bas [o.nbas - 1].icept = o.bas [o.navg - 2].icept;                           <* 
    *> o.bas [o.nbas - 1].rads = o.bas [o.navg - 2].rads;                             <* 
    *> o.bas [o.nbas - 1].degs = o.bas [o.navg - 2].degs;                             <* 
    *> o.bas [o.nbas - 1].quad = o.bas [o.navg - 2].quad;                             <*/
   /*---(fill in avg data)---------------*/
   /*> o.avg [0].slope = o.avg [1].slope = o.avg [2].slope;                           <* 
    *> o.avg [0].icept = o.avg [1].icept = o.avg [2].icept;                           <* 
    *> o.avg [0].rads = o.avg [1].rads = o.avg [2].rads;                              <* 
    *> o.avg [0].degs = o.avg [1].degs = o.avg [2].degs;                              <* 
    *> o.avg [0].quad = o.avg [1].quad = o.avg [2].quad;                              <* 
    *> o.avg [o.navg - 1].slope = o.avg [o.navg - 2].slope;                           <* 
    *> o.avg [o.navg - 1].icept = o.avg [o.navg - 2].icept;                           <* 
    *> o.avg [o.navg - 1].rads = o.avg [o.navg - 2].rads;                             <* 
    *> o.avg [o.navg - 1].degs = o.avg [o.navg - 2].degs;                             <* 
    *> o.avg [o.navg - 1].quad = o.avg [o.navg - 2].quad;                             <*/
   /*---(complete)-----------------------*/
   return 0;
}

char          /*----: move the beg/end points out to test circles ------------*/
BASE__extend         (void)
{
   /*---(locals)-------------------------*/
   float     x_rads    = 0.0;          /* radians                             */
   int       i         = 0;            /* loop iterator -- bas points         */
   int       x, y;
   /*---(header)-------------------------*/
   DEBUG_AVG    yLOG_enter   (__FUNCTION__);
   /*---(beginning)----------------------*/
   for (i = 0; i < o.nbas; ++i) {
      if (o.avg [i].type == POINT_START) {
         /*---(rads from normal)--------*/
         x_rads = o.avg [i + 2].rads;
         /*---(save head)---------------*/
         x = o.bas [i + 1].x_raw;
         y = o.bas [i + 1].y_raw;
         /*---(extend start)------------*/
         o.bas [i].x_raw  = o.avg [i].x_raw  = o.bas [i + 1].x_raw - (10 * my.ratio * cos (x_rads)) ;
         o.bas [i].y_raw  = o.avg [i].y_raw  = o.bas [i + 1].y_raw - (10 * my.ratio * sin (x_rads)) ;
         DEBUG_AVG   yLOG_complex ("new beg"   , "%8.3f, from %4dx %4dy, to %4dx %4dy", x_rads, x, y, o.bas [i].x_raw, o.bas [i].y_raw);
         /*---(recalc)------------------*/
         POINT_position (o.bas + i);
         POINT_calc     (POINTS_BAS, o.bas + i, 'n');
         POINT_position (o.avg + i);
         POINT_calc     (POINTS_AVG, o.avg + i, 'a');
         /*---(done)--------------------*/
      }
      /*---(ending)-------------------------*/
      if (o.avg [i].type == POINT_FINISH) {
         /*---(rads from tail)----------*/
         x_rads = o.bas [i - 1].rads;
         /*---(save tail)---------------*/
         x = o.bas [i - 1].x_raw;
         y = o.bas [i - 1].y_raw;
         /*---(extend start)------------*/
         o.bas [i].x_raw  = o.avg [i].x_raw  = o.bas [i - 1].x_raw + (10 * my.ratio * cos (x_rads)) ;
         o.bas [i].y_raw  = o.avg [i].y_raw  = o.bas [i - 1].y_raw + (10 * my.ratio * sin (x_rads)) ;
         DEBUG_AVG   yLOG_complex ("new end"   , "%8.3f, from %4dx %4dy, to %4dx %4dy", x_rads, x, y, o.bas [i].x_raw, o.bas [i].y_raw);
         /*---(recalc)------------------*/
         POINT_position (o.bas + i);
         POINT_calc     (POINTS_BAS, o.bas + i, 'n');
         POINT_position (o.avg + i);
         POINT_calc     (POINTS_AVG, o.avg + i, 'a');
         /*---(done)--------------------*/
      }
   }
   /*---(complete)-----------------------*/
   DEBUG_AVG    yLOG_exit    (__FUNCTION__);
   return 0;
}

char          /*----: push bas points as far as possible into sharp corners --*/
BASE__sharpen        (void)
{
   /*---(locals)-------------------------*/
   int       d1, d2, dd;                    /* calculated angles              */
   int       sharp     = 70;                /* magnitude of sharp break       */
   int       i         = 0;                 /* loop iterator -- basic points  */
   /*---(process)------------------------*/
   DEBUG_AVG   printf("   sharp tolerance set to >= %d\n", sharp);
   for (i = 1; i < o.nbas - 1; ++i) {
      if (o.avg[i].type == POINT_START || o.avg[i - 1].type == POINT_START) continue;
      if (o.avg[i].type == POINT_FINISH || o.avg[i + 1].type == POINT_FINISH) continue;
      if (o.avg[i - 1].quad == 0)                        continue;
      /*---(calculate)-------------------*/
      d1     = o.avg[i - 1].degs;
      d2     = o.avg[i + 1].degs;
      dd     = d1 - d2;
      if (dd >  180) dd -= 360;
      if (dd < -180) dd += 360;
      DEBUG_AVG   printf("   %3d : %5d vs. %5d, so %5d   ", i, d1, d2, abs(dd));
      o.bas[i].cano = o.avg[i].cano = '-';
      /*---(check)-----------------------*/
      if (abs(dd) >= sharp) {
         o.bas[i].cano = o.avg[i].cano = 'x';
         DEBUG_AVG   printf("SHARP\n");
      } else {
         DEBUG_AVG   printf("no\n");
      }
   }
   /*---(complete)-----------------------*/
   return 0;
}

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
         d  = BASE__distance (i, -2);
         DEBUG_AVG    yLOG_double  ("to tail"   , d);
         if (d <= 4.5) {
            DEBUG_AVG    yLOG_note    ("collapsing last normal before tail");
            --o.nbas;
            --o.navg;
         }
      }
      /*---(normal)----------------------*/
      BASE__point  (i);
      /*---(done)------------------------*/
   }
   /*---(end points)---------------------*/
   DEBUG_AVG    yLOG_value   ("o.nbas"    , o.nbas);
   /*---(run calculations)---------------*/
   BASE__calc    ();
   BASE__extend  ();
   BASE__sharpen ();
   BASE__calc    ();
   /*---(display version)----------------*/
   /*> for (i = 0; i < o.navg; ++i) {                                                 <* 
    *>    POINT_display (o.bas + i);                                                  <* 
    *>    POINT_display (o.avg + i);                                                  <* 
    *> }                                                                              <*/
   /*---(complete)-----------------------*/
   DEBUG_AVG    yLOG_exit    (__FUNCTION__);
   return 0;
}




