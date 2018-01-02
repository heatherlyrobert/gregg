/*============================---(source-start)---============================*/
#include "gregg.h"




/*> char fname[] = "/home/dotsuu/p_gvskav/gregg.gregg_shorthand_system/strokes.dat";   <*/
char fname[] = "/home/dotsuu/p_gvskav/gregg.gregg_shorthand_system/strokes_new.dat";
char oname[] = "/home/dotsuu/p_gvskav/gregg.gregg_shorthand_system/strokes_new.dat";




/*============================--------------------============================*/
/*===----                            key points                        ----===*/
/*============================--------------------============================*/
void o___KEY_POINTS_____________o (void) {;}

PRIV char     /*----: swap two points during the sort ------------------------*/
KEY__swap          (int a_i, int a_j)
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
KEY__sort          (void)
{
   int i;
   int j;
   for (i = 0; i < o.nkey - 1; ++i) {
      for (j = i + 1; j < o.nkey; ++j) {
         if (o.key[i].p_bas > o.key[j].p_bas) KEY__swap (i, j);
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
KEY_calc           (char a_mode)
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
KEY_find           (int a_pt)
{
   int i;
   for (i = 0; i < o.nkey; ++i) {
      if (o.key[i].p_bas == a_pt) return i;
   }
   return -1;
}

char          /*----: add a key point from a avg point -----------------------*/
KEY_add            (int a_pt, char a_fake, char a_type)
{
   DEBUG__KEY  printf("   key request for %4d as a=%c, t=%c : ", a_pt, a_fake, a_type);
   /*---(locals)-------------------------*/
   int pt  = -1;
   /*---(check for existing)-------------*/
   pt  = KEY_find (a_pt);
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
   KEY__sort ();
   KEY_calc  ('n');
   /*---(locate after sort)--------------*/
   pt = KEY_find (a_pt);
   DEBUG__KEY  printf("added as %d\n", pt);
   /*---(complete)-----------------------*/
   return pt;
}

char          /*----: remove a key point -------------------------------------*/
KEY_del            (int a_pt)
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
   KEY_calc ('n');
   return 0;
}

char          /*---: prepare key points for matching -------------------------*/
KEY_prep           (void)
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
KEY__clean           (void)
{
   int i;
   int     nclean  = 0;
   DEBUG__KEY  printf("   cleaning for similar points\n");
   for (i = 1 ; i < o.nkey; ++i) {
      if (o.key[i].t == '>')            continue;
      if (o.key[i].q == o.key[i - 1].q) {
         DEBUG__KEY  printf("   cleaned out %d\n", i);
         KEY_del(i - 1);
         --i;
         ++nclean;
      }
   }
   if (nclean == 0) DEBUG__KEY  printf("   none cleaned\n");
   return 0;
}

PRIV char     /*----: move the beg/end points out to test circles ------------*/
KEY__extend        (void)
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
KEY__sharpen       (void)
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
            KEY_del (i);
            KEY_add (new, '-', '-');
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
            KEY_del (i);
            KEY_add (new, '-', '-');
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
            KEY_del (i);
            KEY_add (new, '-', '-');
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
            KEY_del (i);
            KEY_add (new, '-', '-');
         } else {
            DEBUG__KEY  printf("         max remains\n");
         }
      }
      /*---(quad 1 to 2)-----------------*/
      else {
         DEBUG__KEY  printf("      #?? (%3d) %3d is %1d and %1d\n", i, o.key[i].p_bas, q1, q2);
      }
   }
   KEY_calc  ('n');
   /*---(complete)-----------------------*/
   return 0;
}

char          /*----: identify key points from bas/avg ones ------------------*/
KEY_filter         (void)
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
         KEY_add (i, '-', '-');
      } else if (o.avg[i - 1].t == 'S') {
         KEY_add (i, '-', '>');
      } else if (o.avg[i - 1].q == 0) {
         continue;
      } else if (o.avg[i].q != o.avg[i - 1].q) {
         KEY_add (i, '-', '-');
      }
   }
   KEY__clean   ();
   KEY__extend  ();
   KEY__sharpen ();
   /*---(completion)---------------------*/
   DEBUG__KEY  printf("KEY POINTS (end)\n\n");
   return 0;
}

char          /*---: label a range of key points with a use ------------------*/
KEY_label          (int a_pt, int a_count, char *a_use)
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
