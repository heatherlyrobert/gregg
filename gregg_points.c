/*============================---(source-start)---============================*/
#include "gregg.h"




/*> char fname[] = "/home/dotsuu/p_gvskav/gregg.gregg_shorthand_system/strokes.dat";   <*/
char fname[] = "/home/dotsuu/p_gvskav/gregg.gregg_shorthand_system/strokes_new.dat";
char oname[] = "/home/dotsuu/p_gvskav/gregg.gregg_shorthand_system/strokes_new.dat";


/*============================--------------------============================*/
/*===----                          basic points                        ----===*/
/*============================--------------------============================*/
void o___BAS_POINTS_____________o (void) {;}

PRIV int      /*----: locate a basic point based on its raw point ------------*/
bas_find           (int a_raw)
{
   int i = 0;
   for (i = 0; i < o.nbas; ++i) {
      if (o.bas[i].p_bas == a_raw) return i;
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
   DEBUG__BAS  printf("added as %4d at %4dx, %4dy\n", o.nbas, o.raw[a_raw].xpos, o.raw[a_raw].ypos);
   o.bas[o.nbas].p_raw = o.avg[o.navg].p_raw = a_raw;
   o.bas[o.nbas].p_bas    = o.avg[o.navg].p_bas    = o.nbas;
   o.bas[o.nbas].xpos    = o.avg[o.navg].xpos    = o.raw[a_raw].xpos;
   o.bas[o.nbas].ypos    = o.avg[o.navg].ypos    = o.raw[a_raw].ypos;
   o.bas[o.nbas].t    = o.avg[o.navg].t    = o.raw[a_raw].t;
   strncpy(o.bas[o.nbas].use, "-", 5);
   strncpy(o.avg[o.nbas].use, "-", 5);
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
      POINT_calc (o.bas + i, 'n');
      POINT_calc (o.avg + i, 'a');
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
         o.bas[i].xpos  = o.avg[i].xpos  = o.bas[i + 1].xpos - (20 * cos(rad)) ;
         o.bas[i].ypos  = o.avg[i].ypos  = o.bas[i + 1].xpos - (20 * sin(rad)) ;
         DEBUG__BAS  printf("   extended beg %4d to %4dx, %4dy for circle detection\n", 0, o.bas[i].xpos, o.bas[i].ypos);
      }
      /*---(ending)-------------------------*/
      if (o.avg[i].t == 'F') {
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
         o.bas[o.nbas - 1].fake = 'y';
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
         xd   = fabs(o.raw[i].xpos - o.bas[o.nbas - 1].xpos);
         yd   = fabs(o.raw[i].ypos - o.bas[o.nbas - 1].ypos);
         dist = sqrt((xd * xd) + (yd * yd));
         if (dist <=  3.0 && o.bas[o.nbas - 1].t!= '>') {
            DEBUG__BAS  printf("too close to F         reject, i=%c, i-1=%c\n", o.raw[i].t, o.raw[i - 1].t);
            --o.nbas;
            --o.navg;
         }
         bas_add (i - 1);
         bas_add (i);
         o.bas[o.nbas - 1].fake = 'y';
         continue;
      }
      /*---(handle normal)---------------*/
      xd   = abs(o.raw[i].xpos - o.bas[o.nbas - 1].xpos);
      yd   = abs(o.raw[i].ypos - o.bas[o.nbas - 1].ypos);
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
   DEBUG__BAS       POINT_list (o.bas, o.nbas);
   DEBUG__AVERAGES  POINT_list (o.avg, o.navg);
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
   float   x_min  =  win.m_ymax;
   float   x_max  =  win.m_ymin;
   float   x_ran  =  x_min + x_max;
   float   x_inc  =  x_ran / o.navg;
   /*---(location)-------------------------*/
   float   x_pos  =  x_ran -  (a_y + win.m_ymin);
   if (x_pos <  0    ) return 0;
   if (x_pos >= x_ran) return 0;
   int     x_num  =  (x_pos / x_inc);
   /*---(make change)----------------------*/
   if (x_num + 1 != o.cavg) {
      o.cavg = x_num + 1;
      /*> DRAW_main();                                                                <*/
   }
   /*---(complete)-------------------------*/
   return 0;
}

int           /*----: locate a avg point based on raw id ---------------------*/
avg_find           (int a_pt)
{
   int i;
   for (i = 0; i < o.navg; ++i) {
      if (o.avg[i].p_raw == a_pt - 1) return i;
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
   or = o.key[a_i].p_raw;
   p  = o.key[a_i].p_bas;
   x  = o.key[a_i].xpos;
   y  = o.key[a_i].ypos;
   a  = o.key[a_i].fake;
   t  = o.key[a_i].t;
   strncpy(u, o.key[a_i].use, 5);
   o.key[a_i].p_raw = o.key[a_j].p_raw;
   o.key[a_i].p_bas = o.key[a_j].p_bas;
   o.key[a_i].xpos = o.key[a_j].xpos;
   o.key[a_i].ypos = o.key[a_j].ypos;
   o.key[a_i].fake = o.key[a_j].fake;
   o.key[a_i].t = o.key[a_j].t;
   strncpy(o.key[a_i].use, o.key[a_j].use, 5);
   o.key[a_j].p_raw = or;
   o.key[a_j].p_bas = p;
   o.key[a_j].xpos = x;
   o.key[a_j].ypos = y;
   o.key[a_j].fake = a;
   o.key[a_j].t = t;
   strncpy(o.key[a_j].use, u, 5);
   return 0;
}

PRIV char     /*----: reorder key points by bas id ---------------------------*/
key_sort           (void)
{
   int i;
   int j;
   for (i = 0; i < o.nkey - 1; ++i) {
      for (j = i + 1; j < o.nkey; ++j) {
         if (o.key[i].p_bas > o.key[j].p_bas) key_swap(i, j);
      }
   }
   return 0;
}

char          /*----: calculate curvature between two consecutive key points -*/
key_curve          (int a_pt)
{
   int i;
   int deg   = o.key[a_pt].d;
   int range = -1;
   range = match_range(deg);
   o.key[a_pt].ra = range;
   /*---(calc the arch)-------------------------*/
   int   p1       = o.key[a_pt - 1].p_bas;
   int   p4       = o.key[a_pt].p_bas;
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
      /*> printf("RUNNING CURVE CALCS\n");                                                      <* 
       *> printf("key 1 = %2d at bas = %2d x=%4d, y=%4d\n", a_start, p1, xx1, yy1);             <* 
       *> printf("key 2 = %2d at bas = %2d x=%4d, y=%4d\n", a_start + a_count, p4, xx4, yy4);   <* 
       *> printf("so  slope = %8.3f and intercept = %4d\n", slope, b);                          <*/
   for (i = p1 + 1; i < p4; ++i) {
      liney   = (slope * o.bas[i].xpos) + b;
      linex   = (o.bas[i].ypos - b) / slope;
      diffx   = (o.bas[i].xpos    - linex);
      diffy   = (o.bas[i].ypos    - liney);
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
      POINT_calc (o.key + i, 'n');
      key_curve (i);
   }
   /*---(complete)-----------------------*/
   return 0;
}

int           /*----: locate a key point based on bas id ---------------------*/
key_find           (int a_pt)
{
   int i;
   for (i = 0; i < o.nkey; ++i) {
      if (o.key[i].p_bas == a_pt) return i;
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
   o.key[o.nkey].p_raw = o.avg[a_pt].p_raw;
   o.key[o.nkey].p_bas = o.avg[a_pt].p_bas;
   o.key[o.nkey].xpos = o.avg[a_pt].xpos;
   o.key[o.nkey].ypos = o.avg[a_pt].ypos;
   strncpy(o.key[o.nkey].use, "-", 5);
   /*---(add params)---------------------*/
   o.key[o.nkey].fake = a_fake;
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
   int i;
   for (i = a_pt + 1 ; i < o.nkey; ++i) {
      o.key[i - 1].p_raw = o.key[i].p_raw;
      o.key[i - 1].p_bas = o.key[i].p_bas;
      o.key[i - 1].xpos = o.key[i].xpos;
      o.key[i - 1].ypos = o.key[i].ypos;
      o.key[i - 1].fake = o.key[i].fake;
      o.key[i - 1].t = o.key[i].t;
      strncpy(o.key[i - 1].use, o.key[i].use, 5);
   }
   --o.nkey;
   key_calc('n');
   return 0;
}

char          /*---: prepare key points for matching -------------------------*/
key_prep           (void)
{
   int i;
   int     nclean  = 0;
   DEBUG__KEY  printf("   preparing for matching\n");
   for (i = 0 ; i < o.nkey; ++i) {
      if (o.key[i].t == '>') {
         strncpy(o.key[i].use, ">", 5);
      }
   }
   return 0;
}

PRIV char     /*---: remove similar key points -------------------------------*/
key_clean          (void)
{
   int i;
   int     nclean  = 0;
   DEBUG__KEY  printf("   cleaning for similar points\n");
   for (i = 1 ; i < o.nkey; ++i) {
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
         pt  = o.key[i    ].p_bas - 1;
         rad = o.key[i + 1].r;
         o.bas[pt].xpos  = o.avg[pt].xpos  = o.key[i].xpos - (20 * cos(rad)) ;
         o.bas[pt].ypos  = o.avg[pt].ypos  = o.key[i].ypos - (20 * sin(rad)) ;
         DEBUG__KEY  printf("   extended beg %4d to %4dx, %4dy for circle detection\n", pt, o.bas[pt].xpos, o.bas[pt].ypos);
      }
      /*---(ending)-------------------------*/
      if (o.key[i + 1].t == '>' || i + 1 == o.nkey) {
         pt  = o.key[i    ].p_bas + 1;
         rad = o.key[i    ].r;
         o.bas[pt].xpos  = o.avg[pt].xpos  = o.key[i].xpos + (20 * cos(rad)) ;
         o.bas[pt].ypos  = o.avg[pt].ypos  = o.key[i].ypos + (20 * sin(rad)) ;
         DEBUG__BAS  printf("   extended end %4d to %4dx, %4dy for circle detection\n", pt, o.bas[pt].xpos, o.bas[pt].ypos);
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
      new  = o.key[i].p_bas;
      /*---(bigger y)--------------------*/
      if ((q1 == 1 && q2 == 4) || (q1 == 2 && q2 == 3)) {
         base = max  = o.key[i].ypos;
         DEBUG__KEY  printf("      #01 [BIGGER Y] (%3d) %3d is %1d and %1d : base = %3d --------------------\n", i, o.key[i].p_bas, q1, q2, base);
         for (j = o.key[i - 1].p_bas; j < o.key[i + 1].p_bas; ++j) {
            DEBUG__KEY  printf("         %3d is %3d : ", j, o.avg[j].ypos);
            if        (o.avg[j].p_bas == o.key[i].p_bas) {
               DEBUG__KEY  printf("existing\n");
            } else if (o.avg[j].ypos >  base) {
               DEBUG__KEY  printf("NEW MAX\n");
               max = o.avg[j].ypos;
               new = o.avg[j].p_bas;
            } else {
               DEBUG__KEY  printf("-\n");
            }
         }
         if (new != o.key[i].p_bas) {
            DEBUG__KEY  printf("         new max, %3d moves from %3d to %3d\n", i, o.key[i].p_bas, new);
            key_del (i);
            key_add (new, '-', '-');
         } else {
            DEBUG__KEY  printf("         max remains\n");
         }
      }
      /*---(lesser y)--------------------*/
      else if ((q1 == 4 && q2 == 1) || (q1 == 3 && q2 == 2)) {
         base = max  = o.key[i].ypos;
         DEBUG__KEY  printf("      #02 [LESSER Y] (%3d) %3d is %1d and %1d : base = %3d --------------------\n", i, o.key[i].p_bas, q1, q2, base);
         for (j = o.key[i - 1].p_bas; j < o.key[i + 1].p_bas; ++j) {
            DEBUG__KEY  printf("         %3d is %3d : ", j, o.avg[j].ypos);
            if        (o.avg[j].p_bas == o.key[i].p_bas) {
               DEBUG__KEY  printf("existing\n");
            } else if (o.avg[j].ypos <  base) {
               DEBUG__KEY  printf("NEW MAX\n");
               max = o.avg[j].ypos;
               new = o.avg[j].p_bas;
            } else {
               DEBUG__KEY  printf("-\n");
            }
         }
         if (new != o.key[i].p_bas) {
            DEBUG__KEY  printf("         new max, %3d moves from %3d to %3d\n", i, o.key[i].p_bas, new);
            key_del (i);
            key_add (new, '-', '-');
         } else {
            DEBUG__KEY  printf("         max remains\n");
         }
      }
      /*---(bigger x)--------------------*/
      else if ((q1 == 1 && q2 == 2) || (q1 == 4 && q2 == 3)) {
         base = max  = o.key[i].xpos;
         DEBUG__KEY  printf("      #03 [BIGGER X] (%3d) %3d is %1d and %1d : base = %3d --------------------\n", i, o.key[i].p_bas, q1, q2, base);
         for (j = o.key[i - 1].p_bas; j < o.key[i + 1].p_bas; ++j) {
            DEBUG__KEY  printf("         %3d is %3d : ", j, o.avg[j].xpos);
            if        (o.avg[j].p_bas == o.key[i].p_bas) {
               DEBUG__KEY  printf("existing\n");
            } else if (o.avg[j].xpos >  base) {
               DEBUG__KEY  printf("NEW MAX\n");
               max = o.avg[j].xpos;
               new = o.avg[j].p_bas;
            } else {
               DEBUG__KEY  printf("-\n");
            }
         }
         if (new != o.key[i].p_bas) {
            DEBUG__KEY  printf("         new max, %3d moves from %3d to %3d\n", i, o.key[i].p_bas, new);
            key_del (i);
            key_add (new, '-', '-');
         } else {
            DEBUG__KEY  printf("         max remains\n");
         }
      }
      /*---(lesser x)--------------------*/
      else if ((q1 == 2 && q2 == 1) || (q1 == 3 && q2 == 4)) {
         base = max  = o.key[i].xpos;
         DEBUG__KEY  printf("      #04 [LESSER X] (%3d) %3d is %1d and %1d : base = %3d --------------------\n", i, o.key[i].p_bas, q1, q2, base);
         for (j = o.key[i - 1].p_bas; j < o.key[i + 1].p_bas; ++j) {
            DEBUG__KEY  printf("         %3d is %3d : ", j, o.avg[j].xpos);
            if        (o.avg[j].p_bas == o.key[i].p_bas) {
               DEBUG__KEY  printf("existing\n");
            } else if (o.avg[j].xpos <  base) {
               DEBUG__KEY  printf("NEW MAX\n");
               max = o.avg[j].xpos;
               new = o.avg[j].p_bas;
            } else {
               DEBUG__KEY  printf("-\n");
            }
         }
         if (new != o.key[i].p_bas) {
            DEBUG__KEY  printf("         new max, %3d moves from %3d to %3d\n", i, o.key[i].p_bas, new);
            key_del (i);
            key_add (new, '-', '-');
         } else {
            DEBUG__KEY  printf("         max remains\n");
         }
      }
      /*---(quad 1 to 2)-----------------*/
      else {
         DEBUG__KEY  printf("      #?? (%3d) %3d is %1d and %1d\n", i, o.key[i].p_bas, q1, q2);
      }
   }
   key_calc ('n');
   /*---(complete)-----------------------*/
   return 0;
}

char          /*----: identify key points from bas/avg ones ------------------*/
key_filter         (void)
{
   int i;
   DEBUG__KEY  printf("KEY POINTS (begin)\n");
   /*---(identify criticals)-------------*/
   o.nkey = 0;
   /*> key_beg ();                                                                    <*/
   /*> key_end ();                                                                    <*/
   /*---(process)------------------------*/
   for (i = 1; i < o.navg - 1; ++i) {
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
   DEBUG__KEY  POINT_list (o.key, o.nkey);
   /*---(completion)---------------------*/
   DEBUG__KEY  printf("KEY POINTS (end)\n\n");
   return 0;
}

char          /*---: label a range of key points with a use ------------------*/
key_label          (int a_pt, int a_count, char *a_use)
{
   int i;
   for (i = a_pt; i < a_pt + a_count; ++i) {
      /*> printf("labeling %d with %c\n", i, a_letter);                               <*/
      if      (i == a_pt)                strncpy(o.key[i].use, a_use, 5);
      else if (strcmp(a_use, "-") == 0)  strncpy(o.key[i].use, a_use, 5);
      else                               strncpy(o.key[i].use, "+", 5);
   }
   return 0;
}



/*============================----(source-end)----============================*/
