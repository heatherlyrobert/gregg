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
   DEBUG__BAS  printf("   %3d : ", a_raw);
   if ((rc = BASE__find (a_raw)) >= 0) {
      DEBUG__BAS  printf("REJECTED, already exists as %4d\n", rc);
      return -1;
   }
   DEBUG__BAS  printf("added as %4d at %4dx, %4dy\n", o.nbas, o.raw[a_raw].xpos, o.raw[a_raw].ypos);
   o.bas[o.nbas].p_raw = o.avg[o.navg].p_raw = a_raw;
   o.bas[o.nbas].p_bas = o.avg[o.navg].p_bas = o.nbas;
   o.bas[o.nbas].xpos  = o.avg[o.navg].xpos  = o.raw[a_raw].xpos;
   o.bas[o.nbas].ypos  = o.avg[o.navg].ypos  = o.raw[a_raw].ypos;
   o.bas[o.nbas].type  = o.avg[o.navg].type  = o.raw[a_raw].type;
   strncpy(o.bas[o.nbas].use, "-", 5);
   strncpy(o.avg[o.nbas].use, "-", 5);
   ++o.nbas;
   ++o.navg;
   return 0;
}

PRIV char     /*----: calculate additional information on basic points -------*/
BASE__calc           (void)
{
   int i;
   for (i = 0; i < o.nbas; ++i) {
      if (o.bas[i].type     == 'S') continue;
      if (o.bas[i - 1].type == 'S') continue;
      if (o.bas[i].type     == 'F') continue;
      POINT_calc (o.bas + i, 'n');
      POINT_calc (o.avg + i, 'a');
   }
   return 0;
}

PRIV char     /*----: move the beg/end points out to test circles ------------*/
BASE__extend         (void)
{
   /*---(locals)-------------------------*/
   float     rad       = 0.0;          /* radians                             */
   int       i         = 0;            /* loop iterator -- bas points         */
   /*---(beginning)----------------------*/
   for (i = 0; i < o.nbas; ++i) {
      if (o.avg[i].type == 'S') {
         rad = o.avg[i + 2].r;
         o.bas[i].xpos  = o.avg[i].xpos  = o.bas[i + 1].xpos - (20 * cos(rad)) ;
         o.bas[i].ypos  = o.avg[i].ypos  = o.bas[i + 1].xpos - (20 * sin(rad)) ;
         DEBUG__BAS  printf("   extended beg %4d to %4dx, %4dy for circle detection\n", 0, o.bas[i].xpos, o.bas[i].ypos);
      }
      /*---(ending)-------------------------*/
      if (o.avg[i].type == 'F') {
         rad = o.bas[i - 1].r;
         o.bas[i].xpos  = o.avg[i].xpos  = o.bas[i - 1].xpos + (20 * cos(rad)) ;
         o.bas[i].ypos  = o.avg[i].ypos  = o.bas[i - 1].ypos + (20 * sin(rad)) ;
         DEBUG__BAS  printf("   extended end %4d to %4dx, %4dy for circle detection\n", o.nbas - 1, o.bas[i].xpos, o.bas[i].ypos);
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
   DEBUG__BAS  printf("   sharp tolerance set to >= %d\n", sharp);
   for (i = 1; i < o.nbas - 1; ++i) {
      if (o.avg[i].type == 'S' || o.avg[i - 1].type == 'S') continue;
      if (o.avg[i].type == 'F' || o.avg[i + 1].type == 'F') continue;
      if (o.avg[i - 1].q == 0)                        continue;
      /*---(calculate)-------------------*/
      d1     = o.avg[i - 1].d;
      d2     = o.avg[i + 1].d;
      dd     = d1 - d2;
      if (dd >  180) dd -= 360;
      if (dd < -180) dd += 360;
      DEBUG__BAS  printf("   %3d : %5d vs. %5d, so %5d   ", i, d1, d2, abs(dd));
      o.bas[i].ca = o.avg[i].ca = '-';
      /*---(check)-----------------------*/
      if (abs(dd) >= sharp) {
         o.bas[i].ca = o.avg[i].ca = 'x';
         DEBUG__BAS  printf("SHARP\n");
      } else {
         DEBUG__BAS  printf("no\n");
      }
   }
   /*---(complete)-----------------------*/
   return 0;
}

char          /*----: filter raw points into basic points --------------------*/
BASE_filter        (void)
{
   DEBUG__BAS  printf("BAS POINTS (begin)\n");
   /*---(locals)-------------------------*/
   float     xd, yd;
   float     dist      = 0.0;
   int       i;
   /*---(beg points)---------------------*/
   o.nbas = o.navg = 0;
   /*---(find additional points)---------*/
   for (i = 0; i < o.nraw; ++i) {
      DEBUG__BAS  printf("%3d [%c] ", i, o.raw[i].type);
      /*---(handle starts)---------------*/
      if (o.raw[i].type == 'S') {
         DEBUG__BAS  printf("S/>                    ACCEPT\n");
         BASE__add (i);
         o.bas[o.nbas - 1].fake = 'y';
         BASE__add (i + 1);
         o.bas[o.nbas - 1].type = '>';
         o.avg[o.nbas - 1].type = '>';
         ++i;
         continue;
      }
      /*---(handle finsishes)------------*/
      if (o.raw[i].type == 'F') {
         DEBUG__BAS  printf("F                      ACCEPT\n");
         xd   = fabs(o.raw[i].xpos - o.bas[o.nbas - 1].xpos);
         yd   = fabs(o.raw[i].ypos - o.bas[o.nbas - 1].ypos);
         dist = sqrt((xd * xd) + (yd * yd));
         if (dist <=  3.0 && o.bas[o.nbas - 1].type!= '>') {
            DEBUG__BAS  printf("too close to F         reject, i=%c, i-1=%c\n", o.raw[i].type, o.raw[i - 1].type);
            --o.nbas;
            --o.navg;
         }
         BASE__add (i - 1);
         BASE__add (i);
         o.bas[o.nbas - 1].fake = 'y';
         continue;
      }
      /*---(handle normal)---------------*/
      xd   = abs(o.raw[i].xpos - o.bas[o.nbas - 1].xpos);
      yd   = abs(o.raw[i].ypos - o.bas[o.nbas - 1].ypos);
      dist = sqrt((xd * xd) + (yd * yd));
      if (dist >  3.0) {
         DEBUG__BAS  printf("good distance  %6.2f  ACCEPT\n", dist);
         BASE__add (i);
      } else {
         DEBUG__BAS  printf("too close with %6.2f\n", dist);
      }
   }
   /*---(end points)---------------------*/
   DEBUG__BAS  printf("   added %4d total bas points\n\n", o.nbas);
   /*---(run calculations)---------------*/
   BASE__calc    ();
   BASE__extend  ();
   BASE__sharpen ();
   BASE__calc    ();
   /*---(complete)-----------------------*/
   DEBUG__BAS  printf("BAS POINTS (end)\n\n");
   return 0;
}



