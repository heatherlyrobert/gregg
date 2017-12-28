/*============================---(source-start)---============================*/
#include "gregg.h"



const float FULL_CIRCLE  = 2 * 3.14159;   // circle in radians

/*> char fname[] = "/home/dotsuu/p_gvskav/gregg.gregg_shorthand_system/strokes.dat";   <*/
char fname[] = "/home/dotsuu/p_gvskav/gregg.gregg_shorthand_system/strokes_new.dat";
char oname[] = "/home/dotsuu/p_gvskav/gregg.gregg_shorthand_system/strokes_new.dat";



/*============================--------------------============================*/
/*===----                          raw points                          ----===*/
/*============================--------------------============================*/
void o___RAW_POINTS_____________o (void) {;}
PRIV int     ntries    = 0;            /* number of raw points attempted      */
PRIV char    stype     = '-';          /* p=prefix, o=outline, c=continue     */
PRIV int     xadj      = 0;
PRIV int     yadj      = 0;

PRIV char     /*----: add a new raw point to outline -------------------------*/
raw_point          (int a_x, int a_y, char a_type)
{
   /*---(begin)--------------------------*/
   ++ntries;
   DEBUG_I     printf("   %4d : %4dx, %4dy,    %c\n", o.nraw, a_x, a_y, a_type);
   DEBUG__RAW  printf("   %4d : %4d : %4dx, %4dy : ", ntries, o.nraw, a_x, a_y);
   /*---(defenses)-----------------------*/
   if (o.nraw >= MAX_POINTS) {
      DEBUG__RAW  printf("REJECTED, too many points (%d >= %d\n", o.nraw, MAX_POINTS);
      return -2;
   }
   if (o.nraw > 0 && a_type == o.raw[o.nraw - 1].t && a_x == o.raw[o.nraw - 1].x && a_y == o.raw[o.nraw - 1].y) {
      DEBUG__RAW  printf("REJECTED, same as last point\n");
      return -3;
   }
   /*---(save point)---------------------*/
   o.raw[o.nraw].o   = o.nraw;
   o.raw[o.nraw].x   = a_x;
   o.raw[o.nraw].y   = a_y;
   o.raw[o.nraw].t   = a_type;
   if (o.raw[o.nraw].t != 'S') gen_calc(o.raw + o.nraw, 'n');
   /*---(garbage catching)---------------*/
   if (o.nraw > 0 && o.raw[o.nraw].t != 'F' && o.raw[o.nraw].l > 5) {
      DEBUG__RAW  printf("NEW STROKE, point too far away (%d > 1)\n", o.raw[o.nraw].l);
      raw_point (o.raw[o.nraw - 1].x, o.raw[o.nraw - 1].y, 'F');
      o.raw[o.nraw - 1].a = 'y';
      raw_point (a_x, a_y, 'S');
      o.raw[o.nraw - 1].a = 'y';
      raw_point (a_x, a_y, '-');
      o.raw[o.nraw - 1].t = '>';
   } else {
      DEBUG__RAW {
         if      (a_type == 'S') printf("beg (fake) point, adding\n");
         else if (a_type == 'F') printf("end (fake) point, adding\n");
         else                    printf("unique, adding\n");
      }
      ++o.nraw;
   }
   /*---(complete)-----------------------*/
   return 0;
}

char          /*----: add a new raw point to outline -------------------------*/
raw_add            (int a_x, int a_y)
{
   raw_point (a_x, a_y, '-');
   return 0;
}

char          /*----: start a new outline with a prefix ----------------------*/
raw_pre            (int a_x, int a_y)
{
   /*---(initialize)---------------------*/
   DEBUG__RAW  printf("RAW POINTS (prefix)\n");
   out_clear ();
   stype   = 'p';
   ntries  =  0;
   /*---(save points)--------------------*/
   raw_point (a_x, a_y, 'S');
   o.raw[o.nraw - 1].a = 'y';
   raw_point (a_x, a_y, '-');
   o.raw[o.nraw - 1].t = '>';
   /*---(save adjustment)----------------*/
   xadj = 0;
   yadj = 0;
   /*---(complete)-----------------------*/
   return 0;
}

char          /*----: start a new outline ------------------------------------*/
raw_beg            (int a_x, int a_y)
{
   /*---(check for prefix)---------------*/
   DEBUG__RAW  printf("RAW POINTS (begin)\n");
   if (stype != 'p') {
      out_clear ();
      ntries  =  0;
   }
   /*---(save points)--------------------*/
   stype   = 'o';
   raw_point (a_x, a_y, 'S');
   o.raw[o.nraw - 1].a = 'y';
   raw_point (a_x, a_y, '-');
   o.raw[o.nraw - 1].t = '>';
   /*---(save adjustment)----------------*/
   xadj = a_x;
   yadj = a_y;
   /*---(complete)-----------------------*/
   return 0;
}

char          /*----: start a new outline ------------------------------------*/
raw_read           (int a_x, int a_y)
{
   /*---(check for prefix)---------------*/
   DEBUG__RAW  printf("RAW POINTS (begin)\n");
   /*---(save points)--------------------*/
   out_clear ();
   ntries  =  0;
   raw_point (a_x, a_y, 'S');
   o.raw[o.nraw - 1].a = 'y';
   raw_point (a_x, a_y, '-');
   o.raw[o.nraw - 1].t = '>';
   /*---(save adjustment)----------------*/
   xadj = 0;
   yadj = 0;
   /*---(complete)-----------------------*/
   return 0;
}

char          /*----: continue an outline with another stroke ----------------*/
raw_cont           (int a_x, int a_y)
{
   /*---(check for prefix)---------------*/
   DEBUG__RAW  printf("RAW POINTS (continue)\n");
   /*---(save points)--------------------*/
   stype   = 'c';
   raw_point (a_x, a_y, 'S');
   o.raw[o.nraw - 1].a = 'y';
   raw_point (a_x, a_y, '-');
   o.raw[o.nraw - 1].t = '>';
   /*---(save adjustment)----------------*/
   xadj = 0;
   yadj = 0;
   /*---(complete)-----------------------*/
   return 0;
}

char          /*----: finish off a set of outline raw points -----------------*/
raw_end            (int a_x, int a_y)
{
   raw_point (a_x, a_y, '-');
   raw_point (a_x, a_y, 'F');
   o.raw[o.nraw - 1].a = 'y';
   raw_equalize();
   DEBUG__RAW       gen_list(o.raw, o.nraw);
   DEBUG__RAW  printf("RAW POINTS (end)\n\n");
   return 0;
}

char          /*----: make raw xy relative to neutral origin -----------------*/
raw_equalize       (void)
{
   DEBUG__RAW  printf("   adjust all by %dx, %dy\n", xadj, yadj);
   for (int i = 0; i < o.nraw; ++i) {
      o.raw[i].x   -= xadj;
      o.raw[i].y   -= yadj;
   }
   return 0;
}



/*============================--------------------============================*/
/*===----                          basic points                        ----===*/
/*============================--------------------============================*/
void o___BAS_POINTS_____________o (void) {;}

PRIV int      /*----: locate a basic point based on its raw point ------------*/
bas_find           (int a_raw)
{
   for (int i = 0; i < o.nbas; ++i) {
      if (o.bas[i].p == a_raw) return i;
   }
   return -1;
}

PRIV char     /*----: add an additional basic point from a raw point ---------*/
bas_add            (int a_raw)
{
   int       rc        = 0;
   DEBUG__BAS  printf("   %3d : ", a_raw);
   if ((rc = bas_find(a_raw)) >= 0) {
      DEBUG__BAS  printf("REJECTED, already exists as %4d\n", rc);
      return -1;
   }
   DEBUG__BAS  printf("added as %4d at %4dx, %4dy\n", o.nbas, o.raw[a_raw].x, o.raw[a_raw].y);
   o.bas[o.nbas].o    = o.avg[o.navg].o   = a_raw;
   o.bas[o.nbas].p    = o.avg[o.navg].p   = o.nbas;
   o.bas[o.nbas].x    = o.avg[o.navg].x   = o.raw[a_raw].x;
   o.bas[o.nbas].y    = o.avg[o.navg].y   = o.raw[a_raw].y;
   o.bas[o.nbas].t    = o.avg[o.navg].t   = o.raw[a_raw].t;
   strncpy(o.bas[o.nbas].u, "-", 5);
   strncpy(o.avg[o.nbas].u, "-", 5);
   ++o.nbas;
   ++o.navg;
   return 0;
}

PRIV char     /*----: calculate additional information on basic points -------*/
bas_calc           (void)
{
   int i;
   for (i = 0; i < o.nbas; ++i) {
      if (o.bas[i].t     == 'S') continue;
      if (o.bas[i - 1].t == 'S') continue;
      if (o.bas[i].t     == 'F') continue;
      gen_calc(o.bas + i, 'n');
      gen_calc(o.avg + i, 'a');
   }
   return 0;
}

PRIV char     /*----: move the beg/end points out to test circles ------------*/
bas_extend         (void)
{
   /*---(locals)-------------------------*/
   float     rad       = 0.0;          /* radians                             */
   int       i         = 0;            /* loop iterator -- bas points         */
   /*---(beginning)----------------------*/
   for (i = 0; i < o.nbas; ++i) {
      if (o.avg[i].t == 'S') {
         rad = o.avg[i + 2].r;
         o.bas[i].x  = o.avg[i].x  = o.bas[i + 1].x - (20 * cos(rad)) ;
         o.bas[i].y  = o.avg[i].y  = o.bas[i + 1].x - (20 * sin(rad)) ;
         DEBUG__BAS  printf("   extended beg %4d to %4dx, %4dy for circle detection\n", 0, o.bas[i].x, o.bas[i].y);
      }
      /*---(ending)-------------------------*/
      if (o.avg[i].t == 'F') {
         rad = o.bas[i - 1].r;
         o.bas[i].x  = o.avg[i].x  = o.bas[i - 1].x + (20 * cos(rad)) ;
         o.bas[i].y  = o.avg[i].y  = o.bas[i - 1].y + (20 * sin(rad)) ;
         DEBUG__BAS  printf("   extended end %4d to %4dx, %4dy for circle detection\n", o.nbas - 1, o.bas[i].x, o.bas[i].y);
      }
   }
   /*---(complete)-----------------------*/
   return 0;
}

PRIV char     /*----: push bas points as far as possible into sharp corners --*/
bas_sharpen        (void)
{
   /*---(locals)-------------------------*/
   int       d1, d2, dd;                    /* calculated angles              */
   int       sharp     = 70;                /* magnitude of sharp break       */
   int       i         = 0;                 /* loop iterator -- basic points  */
   /*---(process)------------------------*/
   DEBUG__BAS  printf("   sharp tolerance set to >= %d\n", sharp);
   for (i = 1; i < o.nbas - 1; ++i) {
      if (o.avg[i].t == 'S' || o.avg[i - 1].t == 'S') continue;
      if (o.avg[i].t == 'F' || o.avg[i + 1].t == 'F') continue;
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
bas_filter         (void)
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
      DEBUG__BAS  printf("%3d [%c] ", i, o.raw[i].t);
      /*---(handle starts)---------------*/
      if (o.raw[i].t == 'S') {
         DEBUG__BAS  printf("S/>                    ACCEPT\n");
         bas_add (i);
         o.bas[o.nbas - 1].a = 'y';
         bas_add (i + 1);
         o.bas[o.nbas - 1].t = '>';
         o.avg[o.nbas - 1].t = '>';
         /*> strcpy(o.bas[o.nbas - 1].t, ">");                                        <* 
          *> strcpy(o.avg[o.nbas - 1].t, ">");                                        <*/
         ++i;
         continue;
      }
      /*---(handle finsishes)------------*/
      if (o.raw[i].t == 'F') {
         DEBUG__BAS  printf("F                      ACCEPT\n");
         xd   = fabs(o.raw[i].x - o.bas[o.nbas - 1].x);
         yd   = fabs(o.raw[i].y - o.bas[o.nbas - 1].y);
         dist = sqrt((xd * xd) + (yd * yd));
         if (dist <=  3.0 && o.bas[o.nbas - 1].t!= '>') {
            DEBUG__BAS  printf("too close to F         reject, i=%c, i-1=%c\n", o.raw[i].t, o.raw[i - 1].t);
            --o.nbas;
            --o.navg;
         }
         bas_add (i - 1);
         bas_add (i);
         o.bas[o.nbas - 1].a = 'y';
         continue;
      }
      /*---(handle normal)---------------*/
      xd   = abs(o.raw[i].x - o.bas[o.nbas - 1].x);
      yd   = abs(o.raw[i].y - o.bas[o.nbas - 1].y);
      dist = sqrt((xd * xd) + (yd * yd));
      if (dist >  3.0) {
         DEBUG__BAS  printf("good distance  %6.2f  ACCEPT\n", dist);
         bas_add (i);
      } else {
         DEBUG__BAS  printf("too close with %6.2f\n", dist);
      }
   }
   /*---(end points)---------------------*/
   DEBUG__BAS  printf("   added %4d total bas points\n\n", o.nbas);
   /*---(run calculations)---------------*/
   bas_calc    ();
   /*> bas_extend  ();                                                                <*/
   bas_sharpen ();
   bas_calc    ();
   /*---(output)-------------------------*/
   DEBUG__BAS       gen_list(o.bas, o.nbas);
   DEBUG__AVERAGES  gen_list(o.avg, o.navg);
   /*---(complete)-----------------------*/
   DEBUG__BAS  printf("BAS POINTS (end)\n\n");
   return 0;
}



/*============================--------------------============================*/
/*===----                         average points                       ----===*/
/*============================--------------------============================*/
void o___AVG_POINTS_____________o (void) {;}

char
avg_pick           (int a_y)
{
   /*---(extremes)-------------------------*/
   float   x_min  =  win.top;
   float   x_max  =  win.bottom;
   float   x_ran  =  x_min + x_max;
   float   x_inc  =  x_ran / o.navg;
   /*---(location)-------------------------*/
   float   x_pos  =  x_ran -  (a_y + win.bottom);
   if (x_pos <  0    ) return 0;
   if (x_pos >= x_ran) return 0;
   int     x_num  =  (x_pos / x_inc);
   /*---(make change)----------------------*/
   if (x_num + 1 != o.cavg) {
      o.cavg = x_num + 1;
      draw_main();
   }
   /*---(complete)-------------------------*/
   return 0;
}

int           /*----: locate a avg point based on raw id ---------------------*/
avg_find           (int a_pt)
{
   for (int i = 0; i < o.navg; ++i) {
      if (o.avg[i].o == a_pt - 1) return i;
   }
   return -1;
}



/*============================--------------------============================*/
/*===----                            key points                        ----===*/
/*============================--------------------============================*/
void o___KEY_POINTS_____________o (void) {;}

PRIV char     /*----: swap two points during the sort ------------------------*/
key_swap           (int a_i, int a_j)
{
   int or, p, x, y, a, t;
   char  u[5];
   or = o.key[a_i].o;
   p  = o.key[a_i].p;
   x  = o.key[a_i].x;
   y  = o.key[a_i].y;
   a  = o.key[a_i].a;
   t  = o.key[a_i].t;
   strncpy(u, o.key[a_i].u, 5);
   o.key[a_i].o = o.key[a_j].o;
   o.key[a_i].p = o.key[a_j].p;
   o.key[a_i].x = o.key[a_j].x;
   o.key[a_i].y = o.key[a_j].y;
   o.key[a_i].a = o.key[a_j].a;
   o.key[a_i].t = o.key[a_j].t;
   strncpy(o.key[a_i].u, o.key[a_j].u, 5);
   o.key[a_j].o = or;
   o.key[a_j].p = p;
   o.key[a_j].x = x;
   o.key[a_j].y = y;
   o.key[a_j].a = a;
   o.key[a_j].t = t;
   strncpy(o.key[a_j].u, u, 5);
   return 0;
}

PRIV char     /*----: reorder key points by bas id ---------------------------*/
key_sort           (void)
{
   for (int i = 0; i < o.nkey - 1; ++i) {
      for (int j = i + 1; j < o.nkey; ++j) {
         if (o.key[i].p > o.key[j].p) key_swap(i, j);
      }
   }
   return 0;
}

char          /*----: calculate curvature between two consecutive key points -*/
key_curve          (int a_pt)
{
   int deg   = o.key[a_pt].d;
   int range = -1;
   range = match_range(deg);
   o.key[a_pt].ra = range;
   /*---(calc the arch)-------------------------*/
   int   p1       = o.key[a_pt - 1].p;
   int   p4       = o.key[a_pt].p;
   float slope    = o.key[a_pt].s;
   int   b        = o.key[a_pt].b;
   float curve    = 0.0;
   float maxcurve = 0.0;
   float lcurve   = 0.0;
   float rcurve   = 0.0;
   float theta    = 0.0;
   int   thetad   = 0;
   float linex    = 0.0;
   float liney    = 0.0;
   float diffx    = 0.0;
   float diffy    = 0.0;
   /*> DEBUG__CURVES {                                                                          <* 
    *>    printf("RUNNING CURVE CALCS\n");                                                      <* 
    *>    printf("key 1 = %2d at bas = %2d x=%4d, y=%4d\n", a_start, p1, xx1, yy1);             <* 
    *>    printf("key 2 = %2d at bas = %2d x=%4d, y=%4d\n", a_start + a_count, p4, xx4, yy4);   <* 
    *>    printf("so  slope = %8.3f and intercept = %4d\n", slope, b);                          <* 
    *> }                                                                                        <*/
   for (int i = p1 + 1; i < p4; ++i) {
      liney   = (slope * o.bas[i].x) + b;
      linex   = (o.bas[i].y - b) / slope;
      diffx   = (o.bas[i].x    - linex);
      diffy   = (o.bas[i].y    - liney);
      theta   = atan(diffy/diffx);
      thetad  = theta * RAD2DEG;
      curve   = fabs(diffx * sin(theta));
      switch (range) {
      case 1:  if (diffx > 0.0) curve *= -1;  break;
      case 2:  if (diffy < 0.0) curve *= -1;  break;
      case 3:  if (diffx < 0.0) curve *= -1;  break;
      case 4:  if (diffx < 0.0) curve *= -1;  break;
      case 5:  if (diffy < 0.0) curve *= -1;  break;
      case 6:  if (diffx < 0.0) curve *= -1;  break;
      case 7:  if (diffx < 0.0) curve *= -1;  break;
      }
      if (curve > lcurve) lcurve = curve;
      if (curve < rcurve) rcurve = curve;
      if (fabs(curve) > fabs(maxcurve)) maxcurve = curve;
   }
   DEBUG__CURVES printf("key_curve for key points %d/%d (bas %02d/%02d) maxcurve=%5.1f\n", a_pt, a_pt + 1, p1, p4, maxcurve);
   o.key[a_pt].c  = maxcurve;
   o.key[a_pt].ca = '-';
   /*> if (lcurve != 0 && rcurve != 0) o.key[a_pt].ca = 'x';                          <*/
   o.key[a_pt].cc = -9;
   if      (maxcurve >=  (o.key[a_pt].l * 0.15) || maxcurve >=  2.0)   o.key[a_pt].cc = +1;
   else if (maxcurve <= -(o.key[a_pt].l * 0.15) || maxcurve <= -2.0)   o.key[a_pt].cc = -1;
   else                                                                o.key[a_pt].cc =  0;
   return 0;
}

char          /*----: calculate key point information ------------------------*/
key_calc           (char a_mode)
{
   /*---(usage notes)--------------------*/
   /*
    *  a_mode    n = normal operation (no S/F points)
    *            c = circle operation (includes S/F)
    */
   /*---(locals)-------------------------*/
   int       i         = 0;            /* loop iterator -- key points         */
   /*---(process)------------------------*/
   for (i = 0; i < o.nkey; ++i) {
      /*---(initialize)------------------*/
      o.key[i].xd = o.key[i].yd = o.key[i].l                = 0;
      o.key[i].s  = o.key[i].b  = o.key[i].r  = o.key[i].d  = 0;
      o.key[i].q  = o.key[i].ra = o.key[i].c  = o.key[i].cc = 0;
      o.key[i].ca = '-';
      /*---(filter)----------------------*/
      if (a_mode == 'n' && o.key[i].t == '>') continue;
      if (a_mode == 'c' && o.key[i].t == 'S') continue;
      /*---(calculate)-------------------*/
      gen_calc  (o.key + i, 'n');
      key_curve (i);
   }
   /*---(complete)-----------------------*/
   return 0;
}

int           /*----: locate a key point based on bas id ---------------------*/
key_find           (int a_pt)
{
   for (int i = 0; i < o.nkey; ++i) {
      if (o.key[i].p == a_pt) return i;
   }
   return -1;
}

char          /*----: add a key point from a avg point -----------------------*/
key_add            (int a_pt, char a_fake, char a_type)
{
   DEBUG__KEY  printf("   key request for %4d as a=%c, t=%c : ", a_pt, a_fake, a_type);
   /*---(locals)-------------------------*/
   int pt  = -1;
   /*---(check for existing)-------------*/
   pt  = key_find (a_pt);
   if (pt >= 0) {
      DEBUG__KEY  printf("already exists as %d\n", pt);
      return pt;
   }
   /*---(add new key)--------------------*/
   o.key[o.nkey].o = o.avg[a_pt].o;
   o.key[o.nkey].p = o.avg[a_pt].p;
   o.key[o.nkey].x = o.avg[a_pt].x;
   o.key[o.nkey].y = o.avg[a_pt].y;
   strncpy(o.key[o.nkey].u, "-", 5);
   /*---(add params)---------------------*/
   o.key[o.nkey].a = a_fake;
   o.key[o.nkey].t = a_type;
   /*---(update the keys)----------------*/
   ++o.nkey;
   key_sort();
   key_calc('n');
   /*---(locate after sort)--------------*/
   pt = key_find (a_pt);
   DEBUG__KEY  printf("added as %d\n", pt);
   /*---(complete)-----------------------*/
   return pt;
}

char          /*----: remove a key point -------------------------------------*/
key_del            (int a_pt)
{
   DEBUG__KEY  printf("   key delete  for %4d\n", a_pt);
   for (int i = a_pt + 1 ; i < o.nkey; ++i) {
      o.key[i - 1].o = o.key[i].o;
      o.key[i - 1].p = o.key[i].p;
      o.key[i - 1].x = o.key[i].x;
      o.key[i - 1].y = o.key[i].y;
      o.key[i - 1].a = o.key[i].a;
      o.key[i - 1].t = o.key[i].t;
      strncpy(o.key[i - 1].u, o.key[i].u, 5);
   }
   --o.nkey;
   key_calc('n');
   return 0;
}

char          /*---: prepare key points for matching -------------------------*/
key_prep           (void)
{
   int     nclean  = 0;
   DEBUG__KEY  printf("   preparing for matching\n");
   for (int i = 0 ; i < o.nkey; ++i) {
      if (o.key[i].t == '>') {
         strncpy(o.key[i].u, ">", 5);
      }
   }
   return 0;
}

PRIV char     /*---: remove similar key points -------------------------------*/
key_clean          (void)
{
   int     nclean  = 0;
   DEBUG__KEY  printf("   cleaning for similar points\n");
   for (int i = 1 ; i < o.nkey; ++i) {
      if (o.key[i].t == '>')            continue;
      if (o.key[i].q == o.key[i - 1].q) {
         DEBUG__KEY  printf("   cleaned out %d\n", i);
         key_del(i - 1);
         --i;
         ++nclean;
      }
   }
   if (nclean == 0) DEBUG__KEY  printf("   none cleaned\n");
   return 0;
}

PRIV char     /*----: move the beg/end points out to test circles ------------*/
key_extend         (void)
{

   /*---(locals)-------------------------*/
   float     rad       = 0.0;          /* radians                             */
   int       i         = 0;            /* loop iterator -- bas points         */
   int       pt        = 0;
   /*---(beginning)----------------------*/
   for (i = 0; i < o.nkey; ++i) {
      if (o.key[i    ].t == '>') {
         pt  = o.key[i    ].p - 1;
         rad = o.key[i + 1].r;
         o.bas[pt].x  = o.avg[pt].x  = o.key[i].x - (20 * cos(rad)) ;
         o.bas[pt].y  = o.avg[pt].y  = o.key[i].y - (20 * sin(rad)) ;
         DEBUG__KEY  printf("   extended beg %4d to %4dx, %4dy for circle detection\n", pt, o.bas[pt].x, o.bas[pt].y);
      }
      /*---(ending)-------------------------*/
      if (o.key[i + 1].t == '>' || i + 1 == o.nkey) {
         pt  = o.key[i    ].p + 1;
         rad = o.key[i    ].r;
         o.bas[pt].x  = o.avg[pt].x  = o.key[i].x + (20 * cos(rad)) ;
         o.bas[pt].y  = o.avg[pt].y  = o.key[i].y + (20 * sin(rad)) ;
         DEBUG__BAS  printf("   extended end %4d to %4dx, %4dy for circle detection\n", pt, o.bas[pt].x, o.bas[pt].y);
      }
   }
   /*---(complete)-----------------------*/
   return 0;
}

PRIV char     /*---: get key points into the corners/extremes ----------------*/
key_sharpen        (void)
{
   /*---(locals)-------------------------*/
   int       q1        = 0;
   int       q2        = 0;
   int       i         = 0;
   int       j         = 0;
   int       max       = 0;
   int       new       = 0;
   int       base      = 0;
   /*---(beginning)----------------------*/
   DEBUG__KEY  printf("   pushing key points into corners/extremes\n");
   for (i = 0 ; i < o.nkey; ++i) {
      if (o.key[i    ].t == '>') {
         DEBUG__KEY  printf("      #00 [SKIPPING] (%3d) stroke beginning ------\n", i);
         continue;
      }
      if (o.key[i    ].t == '>' || i+ 1 == o.nkey) {
         DEBUG__KEY  printf("      #00 [SKIPPING] (%3d) stroke ending ---------\n", i);
         continue;
      }
      q1   = o.key[i    ].q;
      q2   = o.key[i + 1].q;
      new  = o.key[i].p;
      /*---(bigger y)--------------------*/
      if ((q1 == 1 && q2 == 4) || (q1 == 2 && q2 == 3)) {
         base = max  = o.key[i].y;
         DEBUG__KEY  printf("      #01 [BIGGER Y] (%3d) %3d is %1d and %1d : base = %3d --------------------\n", i, o.key[i].p, q1, q2, base);
         for (j = o.key[i - 1].p; j < o.key[i + 1].p; ++j) {
            DEBUG__KEY  printf("         %3d is %3d : ", j, o.avg[j].y);
            if        (o.avg[j].p == o.key[i].p) {
               DEBUG__KEY  printf("existing\n");
            } else if (o.avg[j].y >  base) {
               DEBUG__KEY  printf("NEW MAX\n");
               max = o.avg[j].y;
               new = o.avg[j].p;
            } else {
               DEBUG__KEY  printf("-\n");
            }
         }
         if (new != o.key[i].p) {
            DEBUG__KEY  printf("         new max, %3d moves from %3d to %3d\n", i, o.key[i].p, new);
            key_del (i);
            key_add (new, '-', '-');
         } else {
            DEBUG__KEY  printf("         max remains\n");
         }
      }
      /*---(lesser y)--------------------*/
      else if ((q1 == 4 && q2 == 1) || (q1 == 3 && q2 == 2)) {
         base = max  = o.key[i].y;
         DEBUG__KEY  printf("      #02 [LESSER Y] (%3d) %3d is %1d and %1d : base = %3d --------------------\n", i, o.key[i].p, q1, q2, base);
         for (j = o.key[i - 1].p; j < o.key[i + 1].p; ++j) {
            DEBUG__KEY  printf("         %3d is %3d : ", j, o.avg[j].y);
            if        (o.avg[j].p == o.key[i].p) {
               DEBUG__KEY  printf("existing\n");
            } else if (o.avg[j].y <  base) {
               DEBUG__KEY  printf("NEW MAX\n");
               max = o.avg[j].y;
               new = o.avg[j].p;
            } else {
               DEBUG__KEY  printf("-\n");
            }
         }
         if (new != o.key[i].p) {
            DEBUG__KEY  printf("         new max, %3d moves from %3d to %3d\n", i, o.key[i].p, new);
            key_del (i);
            key_add (new, '-', '-');
         } else {
            DEBUG__KEY  printf("         max remains\n");
         }
      }
      /*---(bigger x)--------------------*/
      else if ((q1 == 1 && q2 == 2) || (q1 == 4 && q2 == 3)) {
         base = max  = o.key[i].x;
         DEBUG__KEY  printf("      #03 [BIGGER X] (%3d) %3d is %1d and %1d : base = %3d --------------------\n", i, o.key[i].p, q1, q2, base);
         for (j = o.key[i - 1].p; j < o.key[i + 1].p; ++j) {
            DEBUG__KEY  printf("         %3d is %3d : ", j, o.avg[j].x);
            if        (o.avg[j].p == o.key[i].p) {
               DEBUG__KEY  printf("existing\n");
            } else if (o.avg[j].x >  base) {
               DEBUG__KEY  printf("NEW MAX\n");
               max = o.avg[j].x;
               new = o.avg[j].p;
            } else {
               DEBUG__KEY  printf("-\n");
            }
         }
         if (new != o.key[i].p) {
            DEBUG__KEY  printf("         new max, %3d moves from %3d to %3d\n", i, o.key[i].p, new);
            key_del (i);
            key_add (new, '-', '-');
         } else {
            DEBUG__KEY  printf("         max remains\n");
         }
      }
      /*---(lesser x)--------------------*/
      else if ((q1 == 2 && q2 == 1) || (q1 == 3 && q2 == 4)) {
         base = max  = o.key[i].x;
         DEBUG__KEY  printf("      #04 [LESSER X] (%3d) %3d is %1d and %1d : base = %3d --------------------\n", i, o.key[i].p, q1, q2, base);
         for (j = o.key[i - 1].p; j < o.key[i + 1].p; ++j) {
            DEBUG__KEY  printf("         %3d is %3d : ", j, o.avg[j].x);
            if        (o.avg[j].p == o.key[i].p) {
               DEBUG__KEY  printf("existing\n");
            } else if (o.avg[j].x <  base) {
               DEBUG__KEY  printf("NEW MAX\n");
               max = o.avg[j].x;
               new = o.avg[j].p;
            } else {
               DEBUG__KEY  printf("-\n");
            }
         }
         if (new != o.key[i].p) {
            DEBUG__KEY  printf("         new max, %3d moves from %3d to %3d\n", i, o.key[i].p, new);
            key_del (i);
            key_add (new, '-', '-');
         } else {
            DEBUG__KEY  printf("         max remains\n");
         }
      }
      /*---(quad 1 to 2)-----------------*/
      else {
         DEBUG__KEY  printf("      #?? (%3d) %3d is %1d and %1d\n", i, o.key[i].p, q1, q2);
      }
   }
   key_calc ('n');
   /*---(complete)-----------------------*/
   return 0;
}

char          /*----: identify key points from bas/avg ones ------------------*/
key_filter         (void)
{
   DEBUG__KEY  printf("KEY POINTS (begin)\n");
   /*---(identify criticals)-------------*/
   o.nkey = 0;
   /*> key_beg ();                                                                    <*/
   /*> key_end ();                                                                    <*/
   /*---(process)------------------------*/
   for (int i = 1; i < o.navg - 1; ++i) {
      /*---(filter)----------------------*/
      if      (o.avg[i].t == 'S')  continue;
      if      (o.avg[i].t == 'F')  continue;
      /*---(mark)------------------------*/
      if        (o.avg[i + 1].t == 'F') {
         key_add (i, '-', '-');
      } else if (o.avg[i - 1].t == 'S') {
         key_add (i, '-', '>');
      } else if (o.avg[i - 1].q == 0) {
         continue;
      } else if (o.avg[i].q != o.avg[i - 1].q) {
         key_add (i, '-', '-');
      }
   }
   key_clean   ();
   key_extend  ();
   key_sharpen ();
   DEBUG__KEY  printf("   done with %d points\n", o.nkey);
   DEBUG__KEY  gen_list(o.key, o.nkey);
   /*---(completion)---------------------*/
   DEBUG__KEY  printf("KEY POINTS (end)\n\n");
   return 0;
}

char          /*---: label a range of key points with a use ------------------*/
key_label          (int a_pt, int a_count, char *a_use)
{
   for (int i = a_pt; i < a_pt + a_count; ++i) {
      /*> printf("labeling %d with %c\n", i, a_letter);                               <*/
      if      (i == a_pt)                strncpy(o.key[i].u, a_use, 5);
      else if (strcmp(a_use, "-") == 0)  strncpy(o.key[i].u, a_use, 5);
      else                               strncpy(o.key[i].u, "+", 5);
   }
   return 0;
}



/*============================--------------------============================*/
/*===----                           outlines                           ----===*/
/*============================--------------------============================*/
void o___OUTLINES_______________o (void) {;}

char          /*----: clear all point data -----------------------------------*/
out_clear          (void)
{
   /*---(clear header)-------------------*/
   o.when   [0]  = '\0';
   o.expect [0]  = '\0';
   o.note   [0]  = '\0';
   o.saved       = 'n';
   /*---(clear results)------------------*/
   o.letters[0]  = '\0';
   o.actual [0]  = '\0';
   o.grade  [0]  = '\0';
   o.word   [0]  = '\0';
   /*---(clear counters)-----------------*/
   o.craw = o.cavg = o.ckey = 0;
   o.nraw = o.nbas = o.navg = o.nkey  = 0;
   /*---(clear structures)---------------*/
   tPOINT  *p = NULL;
   DEBUG_I printf("   clearing ");
   for (int i = 0; i < 4; ++i) {
      switch (i) {
      case 0: p = o.raw; DEBUG_I printf("raw, "); break;
      case 1: p = o.bas; DEBUG_I printf("bas, "); break;
      case 2: p = o.avg; DEBUG_I printf("avg, "); break;
      case 3: p = o.key; DEBUG_I printf("key\n"); break;
      }
      for (int j = 0; j < MAX_POINTS; ++j) {
         p[j].o  = p[j].p  = 0;
         p[j].x  = p[j].y  = p[j].xd = p[j].yd = 0;
         p[j].l  = p[j].b  = p[j].d  = p[j].q  = p[j].cc = 0;
         p[j].s  = p[j].r  = p[j].ra = p[j].c  = 0.0;
         p[j].ca = p[j].t  = p[j].a  = '-';
         strncpy(p[j].u, "-", 5);
      }
   }
   ntries  = 0;
   //---(complete)-------------------------------#
   return 0;
}

char
out_init (void)
{
   /*---(locals)---------------------------*/
   FILE     *f;
   char      s[MAX_LINE]   = "";
   int       i             = 0;
   int       len           = 0;
   /*---(open file)------------------------*/
   f = fopen(fname, "r");
   if (f == NULL) return -1;
   /*---(count records)--------------------*/
   for (i = 0; fgets(s, MAX_LINE, f) ; ++i) {
      len = strlen(s);
      if (len > 1 && s[0] != '#' && s[0] != ' ') ++o.total;
   }
   /*> printf("out_init() - found %d saved outlines\n", o.total);                     <*/
   o.craw     = 1;
   o.cavg     = 1;
   o.ckey     = 1;
   o.curr     = 1;
   return 0;
}

char
out_pick (int a_y)
{
   if (o.total == 0) return 0;
   float   x_top =  win.top    - (win.bar * 1.0) -  5;
   float   x_bot = -win.bottom + (win.bar * 1.0) +  5;
   float   x_ran = x_top - x_bot;
   float   x_inc = (x_top - x_bot) / o.total;
   float   x_pos = x_ran - (a_y - x_bot);
   int     x_num = (x_pos / x_inc) + 1;
   if (a_y >= x_top) {
      /*> printf("hit home/top side\n");                                              <*/
      x_num = 1;
   }
   if (a_y <= x_bot) {
      /*> printf("hit end/bottom side\n");                                            <*/
      x_num = o.total;
   }
   /*> printf("got %4dy with %4.0ft, %4.0fb, %4.0fr, %do, %4.0fi, %4.0fp, so go to %d\n",   <* 
    *>       a_y, x_top, x_bot, x_ran, o.total, x_inc,                                      <* 
    *>       x_pos, x_num);                                                                 <*/
   if (x_num != o.curr) {
      out_read(x_num);
      /*> gen_list(key, o.nkey);                                                     <*/
   }
   return 0;
}

char
out_read (int a_num)
{
   DEBUG_I  printf("OUTLINE READ (begin)\n");
   DEBUG__RAW  printf("RAW POINTS (begin)\n");
   out_clear();
   /*---(locals)---------------------------*/
   FILE     *f;
   char      s[MAX_LINE] = "";         // current record
   int       n = 0;                    // current record number in file
   int       len       = 0;
   /*---(open file)------------------------*/
   f = fopen(fname, "r");
   if (f == NULL) return -1;
   /*---(process)--------------------------*/
   while (fgets(s, MAX_LINE, f)) {
      ++n;
      if (a_num != n)                            continue;
      len = strlen(s);
      /*> if (len > 1 && s[0] != '#' && s[0] != ' ') continue;                        <*/
      /*> if (s[0] == '#' || s[0] == '\n')           return 0;                        <*/
      char   *p = NULL;             // pointer to substring
      char   *q1 = "|";             // delimiters
      char   *q2 = "(),";           // delimiters
      int     x_count = 0;          // count of raw points
      int     x = 0;                // raw x-coord
      int     y = 0;                // raw y-coord
      char    x_info[100] = "";
      o.curr = a_num;
      DEBUG_I  printf("   index    = %d\n", o.curr);
      /*---(date)-------------------------*/
      p = strtok(s, q1);
      if (p == NULL)     return -1;
      /*> printf("date = <<%s>>\n", p);                                            <*/
      strncpy(o.when,   str_trim(p), 50);
      DEBUG_I  printf("   date     = %s\n", o.when);
      /*---(expected result)--------------*/
      p = strtok(NULL, q1);
      if (p == NULL)     return -1;
      /*> printf("name = <<%s>>\n", p);                                            <*/
      strncpy(o.expect, str_trim(p), 50);
      DEBUG_I  printf("   expected = %s\n", o.expect);
      /*---(note)-------------------------*/
      p = strtok(NULL, q1);
      if (p == NULL)     return -1;
      /*> printf("name = <<%s>>\n", p);                                            <*/
      strncpy(o.note,   str_trim(p), 50);
      DEBUG_I  printf("   note     = %s\n", o.note);
      /*---(count)------------------------*/
      p = strtok(NULL, q1);
      if (p == NULL)     return -1;
      x_count = atoi(p);
      DEBUG_I  printf("   expecting %4d points\n", x_count);
      /*> printf("out_read(%d) has %d points\n", a_num, x_count);                  <*/
      p = strtok(NULL, q2);
      for (int i = 0; i < x_count; ++i) {
         p = strtok(NULL, q2);
         if (p == NULL)  {
            DEBUG_I  printf("(%04d) x=null\n", i);
            break;
         }
         x = atoi(p);
         p = strtok(NULL, q2);
         if (p == NULL) {
            DEBUG_I  printf("(%04d) x=%03d, y=null\n", i, x);
            break;
         }
         y = atoi(p);
         /*> printf("out_read %4dx, %4dy\n", x, y);                                <*/
         if      (i == 0)               raw_read  (x, y);
         else if (i == x_count - 1)     raw_end   (x, y);
         else                           raw_add   (x, y);
         sprintf(x_info, "(%04d) x=%03d, y=%03d", i, x, y);
      }
   }
   DEBUG_I  printf("   took in %4d points\n", o.nraw);
   DEBUG_I  printf("OUTLINE READ (end)\n\n");
   DEBUG__RAW  gen_list(o.raw, o.nraw);
   DEBUG__RAW  printf("RAW POINTS (end)\n\n");
   o.craw     = 1;
   o.cavg     = 1;
   o.ckey     = 1;
   o.saved    = 'y';
   fclose(f);
   bas_filter    ();
   key_filter    ();
   match_flatten ();
   match_squeeze ();
   circle_driver ();
   match_sharps  ();
   match_driver  ();
   /*> DEBUG__KEY  gen_list(o.key, o.nkey);                                           <*/
   return 0;
}

char
out_append         (void)
{
   DEBUG_O  printf("OUT_APPEND (begin)\n");
   /*---(locals)---------------------------*/
   FILE     *f;
   char      d[500]     = "";
   /*---(open file)------------------------*/
   f = fopen(oname, "a");
   if (f == NULL) {
      DEBUG_O  printf("   could not open output file\n");
      DEBUG_O  printf("OUT_APPEND (end)\n\n");
      return -1;
   }
   DEBUG_O  printf("   file open for append\n");
   /*---(write date)-----------------------*/
   time_t      time_date = time(NULL);
   struct tm*  curr_time = localtime(&time_date);
   char        asc_time[20];
   strftime(asc_time, 20, "%y.%m.%d.%H.%M.%S", curr_time);
   strncpy(d, asc_time, 100);
   char  *n = strchr(d, '\n');
   if (n != NULL)  *n = '\0';
   fprintf(f, "%-20.20s | ", d);
   /*---(leave space for answer)-----------*/
   fprintf(f, "%-20.20s | ", " ");
   /*---(leave space for notes)------------*/
   fprintf(f, "%-30.30s | ", " ");
   /*---(write count)----------------------*/
   fprintf(f, "%03d | ", o.nraw);
   /*---(process)--------------------------*/
   for (int i = 0; i < o.nraw; ++i) {
      fprintf(f, "(%+04d,%+04d)", o.raw[i].x, o.raw[i].y);
   }
   /*---(finish off record)----------------*/
   fprintf(f, " |\n");
   fflush(f);
   fclose(f);
   /*---(touch up some vars)---------------*/
   o.saved    = 'y';
   ++o.total;
   o.curr = o.total;
   /*---(complete)-------------------------*/
   DEBUG_O  printf("OUT_APPEND (end)\n\n");
   return 0;
}



/*============================--------------------============================*/
/*===----                            overall                           ----===*/
/*============================--------------------============================*/
void o___GENERIC________________o (void) {;}

char          /*----: perform standard interpoint calculations ---------------*/
gen_calc           (tPOINT *a_curr,  char a_span)
{
   /*---(set the ends)---------------------*/
   tPOINT  *one  = a_curr - 1;
   tPOINT  *two  = a_curr;
   if (a_span == 'a')  two = a_curr + 1;
   /*---(x and y diff)---------------------*/
   a_curr->xd  = two->x - one->x;
   a_curr->yd  = two->y - one->y;
   /*---(length)---------------------------*/
   a_curr->l   = (int) sqrt((a_curr->xd * a_curr->xd) + (a_curr->yd * a_curr->yd));
   /*---(slope and intercept)--------------*/
   if      (a_curr->xd != 0) a_curr->s = (float) a_curr->yd / a_curr->xd;
   else if (a_curr->yd >  0) a_curr->s = +100000;
   else                      a_curr->s = -100000;
   a_curr->b   = (int) (a_curr->y - (a_curr->s * a_curr->x));
   /*---(radians/degrees)------------------*/
   a_curr->r   = atan2(a_curr->yd, a_curr->xd);
   if (a_curr->r > FULL_CIRCLE) a_curr->r -= FULL_CIRCLE;
   if (a_curr->r < 0)           a_curr->r += FULL_CIRCLE;
   a_curr->d   = (int) (a_curr->r * 57.29577951308);
   /*---(quadrant)-------------------------*/
   if (a_curr->xd >= 0) {
      if (a_curr->yd >= 0) a_curr->q = 1;
      else                 a_curr->q = 4;
   } else {
      if (a_curr->yd >= 0) a_curr->q = 2;
      else                 a_curr->q = 3;
   }
   /*---(correct the deg as required)------*/
   /*> if (a_curr->q == 2) a_curr->d -= 180;                                          <* 
    *> if (a_curr->q == 3) a_curr->d += 180;                                          <*/
   /*---(complete)-------------------------*/
   return 0;
}

char          /*----: list all points of a particular type -------------------*/
gen_list           (tPOINT *a_curr, int a_count)
{
   //---(display list)---------------------------#
   printf("\n");
   printf("point inventory----------------------------------------------------------------------\n");
   printf("### bas raw a | -xx- -yy- xd- yd- | len -slope-- icept -rad- deg | q r curve cc c t u\n");
   for (int i = 0; i < a_count; ++i) {
      gen_show(a_curr + i, i);
   }
   printf("### bas raw a | -xx- -yy- xd- yd- | len -slope-- icept -rad- deg | q r curve cc c t u\n");
   printf("-------------------------------------------------------------------------------------\n");
   printf("\n");
   //---(complete)-------------------------------#
   return 0;
}

char          /*----: display all information on an individual point ---------*/
gen_show           (tPOINT *a_curr, int a_num)
{
   float s  = a_curr->s;
   int   b  = a_curr->b;
   if (s > 999) {
      s = 999.99;
      b = 999;
   }
   if (s < -999) {
      s = -999.99;
      b = -999;
   }
   printf("%3d %3d %3d %c", a_num, a_curr->p, a_curr->o, a_curr->a);
   printf(" | %4d %4d %3d %3d", a_curr->x, a_curr->y, a_curr->xd, a_curr->yd);
   printf(" | %3d %8.2f %5d %5.2f %3d", a_curr->l, s, b, a_curr->r, a_curr->d);
   printf(" | %1d %1d %5.1f %2d %c %c %s\n", a_curr->q, a_curr->ra, a_curr->c, a_curr->cc, a_curr->ca, a_curr->t, a_curr->u);
   return 0;
}



/*============================----(source-end)----============================*/
