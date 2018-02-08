/*=================================(beg-code)=================================*/
#include "gregg.h"



PRIV int      /*----: locate a basic point based on its raw point ------------*/
BASE__find            (int a_raw)
{
   int i = 0;
   for (i = 0; i < o.nbas; ++i) {
      if (o.bas[i].p_bas == a_raw) return i;
   }
   return -1;
}

PRIV char     /*----: add an additional basic point from a raw point ---------*/
BASE__add            (int a_raw)
{
   int       rc        = 0;
   DEBUG_AVG   printf("   %3d : ", a_raw);
   if ((rc = BASE__find (a_raw)) >= 0) {
      DEBUG_AVG   printf("REJECTED, already exists as %4d\n", rc);
      return -1;
   }
   DEBUG_AVG   printf("added as %4d at %4dx, %4dy\n", o.nbas, o.raw[a_raw].x_full, o.raw[a_raw].y_full);
   o.bas [o.nbas].p_raw  = o.avg [o.navg].p_raw  = a_raw;
   o.bas [o.nbas].p_bas  = o.avg [o.navg].p_bas  = o.nbas;
   o.bas [o.nbas].x_full = o.avg [o.navg].x_full = o.raw [a_raw].x_full;
   o.bas [o.nbas].x_pos  = o.avg [o.navg].x_pos  = o.raw [a_raw].x_pos;
   o.bas [o.nbas].y_full = o.avg [o.navg].y_full = o.raw [a_raw].y_full;
   o.bas [o.nbas].y_pos  = o.avg [o.navg].y_pos  = o.raw [a_raw].y_pos;
   o.bas [o.nbas].type   = o.avg [o.navg].type   = o.raw [a_raw].type;
   strncpy (o.bas [o.nbas].use, "-", 5);
   strncpy (o.avg [o.nbas].use, "-", 5);
   ++o.nbas;
   ++o.navg;
   return 0;
}

PRIV char     /*----: calculate additional information on basic points -------*/
BASE__calc           (void)
{
   int i;
   /*---(calc real points)---------------*/
   for (i = 0; i < o.nbas; ++i) {
      /*> POINT_display (o.bas + i);                                                  <*/
      /*> POINT_display (o.avg + i);                                                  <*/
      if (o.bas[i].type     == POINT_START ) continue;
      if (o.bas[i - 1].type == POINT_START ) continue;
      if (o.bas[i].type     == POINT_FINISH) continue;
      POINT_calc (POINTS_BAS, o.bas + i, 'n');
      POINT_calc (POINTS_AVG, o.avg + i, 'a');
   }
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

PRIV char     /*----: move the beg/end points out to test circles ------------*/
BASE__extend         (void)
{
   /*---(locals)-------------------------*/
   float     x_rads    = 0.0;          /* radians                             */
   int       i         = 0;            /* loop iterator -- bas points         */
   /*---(beginning)----------------------*/
   for (i = 0; i < o.nbas; ++i) {
      if (o.avg [i].type == POINT_START) {
         x_rads = o.avg [i + 2].rads;
         o.bas [i].x_full  = o.avg [i].x_full  = o.bas [i + 1].x_full - (20 * o.ratio * cos (x_rads)) ;
         o.bas [i].y_full  = o.avg [i].y_full  = o.bas [i + 1].y_full - (20 * o.ratio * sin (x_rads)) ;
         DEBUG_AVG   printf("   extended beg %4d to %4dx, %4dy for circle detection\n", 0, o.bas[i].x_full, o.bas[i].y_full);
         POINT_calc (POINTS_BAS, o.bas + i, 'n');
         POINT_calc (POINTS_AVG, o.avg + i, 'a');
      }
      /*---(ending)-------------------------*/
      if (o.avg [i].type == POINT_FINISH) {
         x_rads = o.bas [i - 1].rads;
         o.bas [i].x_full  = o.avg [i].x_full  = o.bas [i - 1].x_full + (20 * o.ratio * cos (x_rads)) ;
         o.bas [i].y_full  = o.avg [i].y_full  = o.bas [i - 1].y_full + (20 * o.ratio * sin (x_rads)) ;
         DEBUG_AVG   printf("   extended end %4d to %4dx, %4dy for circle detection\n", o.nbas - 1, o.bas[i].x_full, o.bas[i].y_full);
         POINT_calc (POINTS_BAS, o.bas + i, 'n');
         POINT_calc (POINTS_AVG, o.avg + i, 'a');
      }
   }
   /*---(complete)-----------------------*/
   return 0;
}

PRIV char     /*----: push bas points as far as possible into sharp corners --*/
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

int           /*----: check distance to potential point ----------------------*/
BASE__distance     (int a)
{
   float     xd, yd;
   xd   = fabs (o.raw[a].x_pos - o.bas[o.nbas - 1].x_pos);
   yd   = fabs (o.raw[a].y_pos - o.bas[o.nbas - 1].y_pos);
   return sqrt ((xd * xd) + (yd * yd));
}

char          /*----: filter raw points into basic points --------------------*/
BASE_filter        (void)
{
   DEBUG_AVG   printf("BAS POINTS (begin)\n");
   /*---(locals)-------------------------*/
   /*> float     xd, yd;                                                              <*/
   float     dist      = 0.0;
   int       i;
   /*---(beg points)---------------------*/
   o.nbas = o.navg = 0;
   /*---(find additional points)---------*/
   for (i = 0; i < o.nraw; ++i) {
      DEBUG_AVG   printf("%3d [%c] ", i, o.raw[i].type);
      /*---(handle starts)---------------*/
      if (o.raw[i].type == POINT_START) {
         DEBUG_AVG   printf("S/>                    ACCEPT\n");
         BASE__add (i);
         o.bas[o.nbas - 1].fake = 'y';
         BASE__add (i + 1);
         o.bas[o.nbas - 1].type = POINT_HEAD;
         o.avg[o.nbas - 1].type = POINT_HEAD;
         ++i;
         continue;
      }
      /*---(handle finsishes)------------*/
      if (o.raw[i].type == POINT_FINISH) {
         DEBUG_AVG   printf("F                      ACCEPT\n");
         /*> xd   = fabs(o.raw[i].x_full - o.bas[o.nbas - 1].x_full);                 <* 
          *> yd   = fabs(o.raw[i].y_full - o.bas[o.nbas - 1].y_full);                 <* 
          *> dist = sqrt ((xd * xd) + (yd * yd));                                     <*/
         dist = BASE__distance (i);
         if (dist <=  3.0 && o.bas[o.nbas - 1].type!= POINT_HEAD) {
            DEBUG_AVG   printf("too close to F         reject, i=%c, i-1=%c\n", o.raw[i].type, o.raw[i - 1].type);
            --o.nbas;
            --o.navg;
         }
         BASE__add (i - 1);
         BASE__add (i);
         o.bas[o.nbas - 1].fake = 'y';
         continue;
      }
      /*---(handle normal)---------------*/
      /*> xd   = abs (o.raw[i].x_full - o.bas[o.nbas - 1].x_full);                    <* 
       *> yd   = abs (o.raw[i].y_full - o.bas[o.nbas - 1].y_full);                    <* 
       *> dist = sqrt ((xd * xd) + (yd * yd));                                        <*/
      dist = BASE__distance (i);
      if (dist >  3.0) {
         DEBUG_AVG   printf("good distance  %6.2f  ACCEPT\n", dist);
         BASE__add (i);
      } else {
         DEBUG_AVG   printf("too close with %6.2f\n", dist);
      }
   }
   /*---(end points)---------------------*/
   DEBUG_AVG   printf("   added %4d total bas points\n\n", o.nbas);
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
   DEBUG_AVG   printf("BAS POINTS (end)\n\n");
   return 0;
}




