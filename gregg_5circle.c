/*============================---(source-start)---============================*/
#include "gregg.h"



/*============================--------------------============================*/
/*===----                       identify circles                       ----===*/
/*============================--------------------============================*/
static void      o___CIRCLES_________________o (void) {;}
/*
 * circles can have 2 to 4 internal points depending on the circumstances
 * circles have two sizes      -- large (a) and small (e)
 * circles have two directions -- clockwise and counter-clockwise
 * circles may not close at the beginning and ending of words
 *
 */
int       min1      = 0;               /* circle entrance point               */
int       min2      = 0;               /* circle exit point                   */
int       one       = 0;               /* new circle intersection point       */
int       two       = 0;               /* new circle intersection point       */
char      use[5]    = "";              /* circle letter use                   */


char     /*----: determine if two lines intersect -----------------------*/
CIRCLE__intersect    (int a_p1, int a_p3)
{
   /*---(locals)-------------------------*/
   int       p1  = a_p1;               /* 1st point 1st segment               */
   int       p2  = a_p1 + 1;           /* 2nd point 1st segment               */
   int       p3  = a_p3;               /* 1st point 2nd segment               */
   int       p4  = a_p3 + 1;           /* 2nd point 2nd segment               */
   double    A1, B1, C1;               /* 1st segment calcs                   */
   double    A2, B2, C2;               /* 2nd segment calcs                   */
   double    det;                      /* determinant                         */
   int       x, y;                     /* hypothetical intersection           */
   int       _max    = 0;              /* axis minimum point                  */
   int       _min    = 0;              /* axis maximum point                  */
   /*---(solve segment one)--------------*/
   A1 = o.key[p2].y_raw - o.key[p1].y_raw;
   B1 = o.key[p1].x_raw - o.key[p2].x_raw;
   C1 = (A1 * o.key[p1].x_raw) + (B1 * o.key[p1].y_raw);
   DEBUG_CIRCLE  printf("   1st seg :: %4d,%4d to %4d,%4d :: s=%8.3f, b=%4d :: A=%4.0f, B=%4.0f, C=%4.0f\n", o.key[p1].x_raw, o.key[p1].y_raw, o.key[p2].x_raw, o.key[p2].y_raw, o.key[p2].slope, o.key[p2].icept, A1, B1, C1);
   /*---(solve segment two)--------------*/
   A2 = o.key[p4].y_raw - o.key[p3].y_raw;
   B2 = o.key[p3].x_raw - o.key[p4].x_raw;
   C2 = (A2 * o.key[p3].x_raw) + (B2 * o.key[p3].y_raw);
   DEBUG_CIRCLE  printf("   2nd seg :: %4d,%4d to %4d,%4d :: s=%8.3f, b=%4d :: A=%4.0f, B=%4.0f, C=%4.0f\n", o.key[p3].x_raw, o.key[p3].y_raw, o.key[p4].x_raw, o.key[p4].y_raw, o.key[p4].slope, o.key[p4].icept, A2, B2, C2);
   /*---(hypothetical intersection)------*/
   det = (A1 * B2) - (A2 * B1);
   if (det == 0) {
      DEBUG_CIRCLE  printf("   PARALLEL, failing out\n");
      return -1;
   }
   x  = (int) ((B2 * C1) - (B1 * C2)) / det;
   y  = (int) ((A1 * C2) - (A2 * C1)) / det;
   DEBUG_CIRCLE  printf("   hypothetical intersection at %4d, %4d\n", x, y);
   /*---(check 1st segment)--------------*/
   _min   = (int) fmin(o.key[p1].x_raw, o.key[p2].x_raw);
   _max   = (int) fmax(o.key[p1].x_raw, o.key[p2].x_raw);
   DEBUG_CIRCLE  printf("   test x on 1st :: %4d < %4d < %4d : ", _min, x, _max);
   if (_min > x || _max < x)  {
      DEBUG_CIRCLE  printf("FAILED, failing out\n");
      return -1;
   }
   DEBUG_CIRCLE  printf("accepted\n");
   _min   = (int) fmin(o.key[p1].y_raw, o.key[p2].y_raw);
   _max   = (int) fmax(o.key[p1].y_raw, o.key[p2].y_raw);
   DEBUG_CIRCLE  printf("   test y on 1st :: %4d < %4d < %4d : ", _min, y, _max);
   if (_min > y || _max < y) {
      DEBUG_CIRCLE  printf("FAILED, failing out\n");
      return -1;
   }
   DEBUG_CIRCLE  printf("accepted\n");
   /*---(check 2nd segment)--------------*/
   _min   = (int) fmin(o.key[p3].x_raw, o.key[p4].x_raw);
   _max   = (int) fmax(o.key[p3].x_raw, o.key[p4].x_raw);
   DEBUG_CIRCLE  printf("   test x on 2nd :: %4d < %4d < %4d : ", _min, x, _max);
   if (_min > x || _max < x) {
      DEBUG_CIRCLE  printf("FAILED, failing out\n");
      return -1;
   }
   DEBUG_CIRCLE  printf("accepted\n");
   _min   = (int) fmin(o.key[p3].y_raw, o.key[p4].y_raw);
   _max   = (int) fmax(o.key[p3].y_raw, o.key[p4].y_raw);
   DEBUG_CIRCLE  printf("   test y on 2nd :: %4d < %4d < %4d : ", _min, y, _max);
   if (_min > y || _max < y) {
      DEBUG_CIRCLE  printf("FAILED, failing out\n");
      return -1;
   }
   DEBUG_CIRCLE  printf("accepted\n");
   /*---(complete)-----------------------*/
   DEBUG_CIRCLE  printf("   success\n");
   return 0;
}

int      /*----: determine the circle crossover point)-------------------*/
CIRCLE__minpoint     (int a_p1, int a_p3)
{
   DEBUG_CIRCLE  printf("   looking for minimum connection distance...\n");
   /*---(locals)-------------------------*/
   int       p1  = a_p1;               /* 1st point 1st segment               */
   int       p2  = a_p1 + 1;           /* 2nd point 1st segment               */
   int       p3  = a_p3;               /* 1st point 2nd segment               */
   int       p4  = a_p3 + 1;           /* 2nd point 2nd segment               */
   int i = 0;
   int j = 0;
   int  my_x = 0;
   int  my_y = 0;
   int  ci_x = 0;
   int  ci_y = 0;
   float my_dist = 0.0;
   float my_min  = 1000.0;
   for (i = o.key[p1].p_bas; i <= o.key[p2].p_bas; ++i) {
      if (i == 0)  {
         /*> DEBUG_CIRCLE  printf("   %03d (fake/beg) can not be used in connection distance calcs\n", i);   <*/
         continue;
      }
      /*> DEBUG_CIRCLE  printf("   %03d\n", i);                                     <*/
      my_x = o.bas[i].x_raw;
      my_y = o.bas[i].y_raw;
      for (j = o.key[p3].p_bas; j <= o.key[p4].p_bas; ++j) {
         if (j == o.nbas - 1) {
            /*> DEBUG_CIRCLE  printf("   %03d (fake/end) can not be used in connection distance calcs\n", i);   <*/
            continue;
         }
         ci_x = abs(o.bas[j].x_raw - my_x);
         ci_y = abs(o.bas[j].y_raw - my_y);
         my_dist = sqrt((ci_x * ci_x) + (ci_y * ci_y));
         if (my_dist < my_min) {
            my_min = my_dist;
            min1    = i;
            min2    = j;
         }
         /*> DEBUG_CIRCLE  printf("       vs %03d at %7.2f so min is %7.2f\n", j, my_dist, my_min);   <*/
      }
   }
   DEBUG_CIRCLE  printf("   min found at i=%3d, j=%3d, dist=%7.2f\n", min1, min2, my_min);
   if (my_min > 12.0) {
      DEBUG_CIRCLE  printf("   REJECTED, greater than limit of 12.0\n");
      min1 = -1;
      min2 = -1;
      return -1;
   }
   DEBUG_CIRCLE  printf("   success\n");
   /*---(complete)-----------------------*/
   return 0;
}

int      /*----: size of circle at cross-section ------------------------*/
CIRCLE__size        (int a_one, int a_two)
{
   /*---(locals)-------------------------*/
   int       space     = 0;            /* how far from intersetion to measure */
   int       half      = 0;            /* halfway point                       */
   int       mid1      = 0;            /* 1st midpoint                        */
   int       mid2      = 0;            /* 2nd midpoint                        */
   int       _distx    = 0;            /* x-axis distance between midpoints   */
   int       _disty    = 0;            /* y-axis distance between midpoints   */
   int       _dist     = 0;            /* full distance average               */
   int       _dist1    = 0;            /* full distance between midpoints     */
   int       _dist2    = 0;            /* full distance between half points   */
   DEBUG_CIRCLE  printf("   search for midpoint size...\n");
   /*---(figure points)------------------*/
   half     = a_one + ((a_two - a_one) / 2);
   space    = (a_two - a_one) / 4;
   mid1     = half - space;
   mid2     = half + space;
   DEBUG_CIRCLE  printf("   _min1 =%3d, _min2 =%3d, space=%3d, half =%3d, mid1 =%3d, mid2 =%3d\n", a_one, a_two, space, half, mid1, mid2);
   /*---(calc midsection)----------------*/
   _distx   = abs(o.bas[mid2].x_raw - o.bas[mid1].x_raw);
   _disty   = abs(o.bas[mid2].y_raw - o.bas[mid1].y_raw);
   _dist1   = (int) sqrt((_distx * _distx) + (_disty * _disty));
   DEBUG_CIRCLE  printf("   midpoints  : _distx=%3d, _disty=%3d, _dist1=%3d\n", _distx, _disty, _dist1);
   o.avg[mid1].type = 'm';
   o.avg[mid2].type = 'm';
   /*---(get half mark)------------------*/
   _distx   = abs(o.bas[half].x_raw - o.bas[a_one].x_raw);
   _disty   = abs(o.bas[half].y_raw - o.bas[a_one].y_raw);
   _dist2   = (int) sqrt((_distx * _distx) + (_disty * _disty));
   DEBUG_CIRCLE  printf("   halfpoints : _distx=%3d, _disty=%3d, _dist2=%3d\n", _distx, _disty, _dist2);
   o.avg[half].type = 'm';
   /*---(averagge)-----------------------*/
   _dist = (_dist1 + _dist2) / 2.0;
   DEBUG_CIRCLE  printf("   so, _dist =%3d\n", _dist);
   /*---(complete)-----------------------*/
   return _dist;
}

char     /*----: push intersection backward -----------------------------*/
CIRCLE__backward    (void)
{
   int       i, j;                     /* loop iterators -- basic points      */
   float     xd, yd;
   float     dist;
   float     min       = 1000;
   int       pt        = 0;
   int       new       = 0;
   int       old_nkey  = o.nkey;
   DEBUG_CIRCLE  printf("\n");
   DEBUG_CIRCLE  printf("   extend circle intersection backwards from start...\n");
   DEBUG_CIRCLE  printf("      circle beg is    key = %3d, bas = %3d\n", one, o.key[one].p_bas);
   DEBUG_CIRCLE  printf("      circle end is    key = %3d, bas = %3d\n", two, o.key[two].p_bas);
   DEBUG_CIRCLE  printf("      circle midpoint  key = %3d, bas = %3d\n", one + 2, o.key[one + 2].p_bas);
   DEBUG_CIRCLE  printf("      total circle point are key = %3d, bas = %3d\n", o.nkey, o.nbas);
   DEBUG_CIRCLE  printf("      so, checking forward from bas   0 to %3d...\n", o.key[one].p_bas);
   DEBUG_CIRCLE  printf("      against, forward  from    bas %3d to %3d...\n", o.key[one + 2].p_bas, o.key[two].p_bas);
   if (one == 0) return 0;
   for (i = o.key[one - 1].p_bas; i < o.key[one].p_bas; ++i) {
      for (j = o.key[one + 2].p_bas; j <= o.key[two].p_bas; ++j) {
         xd   = abs(o.bas[j].x_raw - o.bas[i].x_raw);
         yd   = abs(o.bas[j].y_raw - o.bas[i].y_raw);
         dist = (int) sqrt((xd * xd) + (yd * yd));
         /*> DEBUG_CIRCLE  printf("      checking i=%3d, j=%3d, dist=%6.2f\n", i, j, dist);   <*/
         if (dist <= 3.0) {
            min = dist;
            pt  = i;
            break;
         }
      }
      if (pt  != 0) break;
   }
   /*---(mark)---------------------------*/
   if (pt  != 0 && pt  != o.key[one].p_bas) {
      new = KEY_add   (pt , 'y', '-');
      if (old_nkey != o.nkey) {
         DEBUG_CIRCLE  printf("      ADDED KEY\n");
         one = new;
         ++two;
         KEY_label (one + 1, two - one, use);
      } else {
         DEBUG_CIRCLE  printf("      existing key\n");
      }
      DEBUG_CIRCLE  printf("      min circle is bas=%3d, key=%3d (n=%2d), dist=%6.2f, use=%-5.5s\n", pt, new, o.nkey, min, use);
   } else {
      DEBUG_CIRCLE  printf("      min circle already exists\n");
   }
   /*---(complete)-----------------------*/
   return 0;
}

char     /*----: push intersection forward ------------------------------*/
CIRCLE__forward     (void)
{
   int       i, j;                     /* loop iterators -- basic points      */
   float     xd, yd;
   float     dist;
   float     min       = 1000;
   int       pt        = 0;
   int       new       = 0;
   int       old_nkey  = o.nkey;
   DEBUG_CIRCLE  printf("\n");
   DEBUG_CIRCLE  printf("   extend circle intersection forwards from end...\n");
   DEBUG_CIRCLE  printf("      circle beg is    key = %3d, bas = %3d\n", one, o.key[one].p_bas);
   DEBUG_CIRCLE  printf("      circle end is    key = %3d, bas = %3d\n", two, o.key[two].p_bas);
   DEBUG_CIRCLE  printf("      circle midpoint  key = %3d, bas = %3d\n", two - 2, o.key[two - 2].p_bas);
   DEBUG_CIRCLE  printf("      total circle point are key = %3d, bas = %3d\n", o.nkey, o.nbas);
   DEBUG_CIRCLE  printf("      so, checking backward from bas %3d to %3d...\n", o.nkey - 1, o.key[two].p_bas);
   DEBUG_CIRCLE  printf("      against, forward from      bas %3d to %3d...\n", o.key[two - 1].p_bas, o.key[one].p_bas);
   if (two == o.nkey - 1) return 0;
   for (j = o.key[two + 1].p_bas; j > o.key[two].p_bas; --j) {
      for (i = o.key[two - 2].p_bas; i > o.key[one].p_bas; --i) {
         xd   = abs(o.bas[j].x_raw - o.bas[i].x_raw);
         yd   = abs(o.bas[j].y_raw - o.bas[i].y_raw);
         dist = (int) sqrt((xd * xd) + (yd * yd));
         /*> DEBUG_CIRCLE  printf("      checking i=%3d, j=%3d, dist=%6.2f\n", i, j, dist);   <*/
         if (dist <= 3.0) {
            min = dist;
            pt  = j;
            break;
         }
      }
      if (pt  != 0) break;
   }
   /*---(mark)---------------------------*/
   if (pt  != 0 && pt  != o.key[two].p_bas) {
      new = KEY_add   (pt , 'y', '-');
      if (old_nkey != o.nkey) {
         DEBUG_CIRCLE  printf("      ADDED KEY\n");
         KEY_label (new, 1, "+");
         two = new;
      } else {
         DEBUG_CIRCLE  printf("      existing key\n");
      }
      DEBUG_CIRCLE  printf("      min circle is bas=%3d, key=%3d (n=%2d), dist=%6.2f, use=%-5.5s\n", pt, new, o.nkey, min, "+");
   } else {
      DEBUG_CIRCLE  printf("      min circle already exists\n");
   }
   /*---(complete)-----------------------*/
   return 0;
}

char
CIRCLE__update      (int a_one, int a_two, int a_size)
{
   /*---(locals)-------------------------*/
   int       d1, d2, dd;               /* degrees for figuring direction      */
   int       i         = 0;            /* loop iterator -- key points         */
   /*---(determine use)------------------*/
   strncpy(use, "-", 5);
   if (a_size   <=  8     )  {
      DEBUG_CIRCLE  printf("   REJECTED, too small\n");
      return -1;
   }
   if (a_size   >= 20     )  strncpy(use, "a", 5);
   else                      strncpy(use, "e", 5);
   DEBUG_CIRCLE  printf("   calling it an <<%s>>\n", use);
   /*---(create new keys)----------------*/
   one = KEY_add (a_one, 'y', '-');
   two = KEY_add (a_two, 'y', '-');
   DEBUG_CIRCLE  printf("   added one=%4d and two=%4d\n", one, two);
   /*---(get direction)------------------*/
   /*> d1 = o.key[one + 2].degs;                                                         <* 
    *> d2 = o.key[one + 3].degs;                                                         <*/
   d1 = o.key[one + 1].degs;
   d2 = o.key[one + 2].degs;
   dd = d1 - d2;
   DEBUG_CIRCLE  printf("   direction   :: d1=%3d, d2=%3d, dd=%3d", d1, d2, dd);
   if (dd >  180) dd -=  360;
   if (dd < -180) dd +=  360;
   DEBUG_CIRCLE  printf(", rdd=%3d", dd);
   if (dd >= 0) {
      DEBUG_CIRCLE  printf(", so clockwise\n");
   } else {
      DEBUG_CIRCLE  printf(", so counter-clockwise\n");
      use[0] = toupper(use[0]);
   }
   /*---(label)--------------------------*/
   for (i = one + 1; i <= two; ++i) {
      strncpy(o.key[i].use, "+", 5);
   }
   strncpy(o.key[one + 1].use, use, 5);
   /*---(complete)-----------------------*/
   return 0;
}

int
CIRCLE__identify    (int a_p1, int a_p3)
{
   /*---(locals)-------------------------*/
   int       p1        = a_p1;         /* 1st point 1st segment               */
   int       p2        = a_p1 + 1;     /* 2nd point 1st segment               */
   int       p3        = a_p3;         /* 1st point 2nd segment               */
   int       p4        = a_p3 + 1;     /* 2nd point 2nd segment               */
   char      rc        = 0;            /* generic return code                 */
   int       csize     = 0;            /* circle size                         */
   int       i;
   /*---(defense)------------------------*/
   if (p4 >= o.nkey)  {
      DEBUG_CIRCLE  printf("   REJECTED, final point outside valid range\n");
      return -1;
   }
   for (i = p2; i <= p4; ++i) {
      if (o.key[i].type == POINT_START) {
         DEBUG_CIRCLE  printf("   REJECTED, crosses a stroke start at %2d\n", i);
         return -1;
      }
      if (strncmp(o.key[i].use, "-", 5) != 0) {
         DEBUG_CIRCLE  printf("   REJECTED, point %2d taken by a <<%s>>\n", i, o.key[i].use);
         return -1;
      }
   }
   DEBUG_CIRCLE  printf("   all points in range and valid\n");
   /*---(intersection)-------------------*/
   rc       = CIRCLE__intersect (a_p1, a_p3);
   if (rc != 0) return -1;
   /*---(minimum connection)-------------*/
   rc       = CIRCLE__minpoint  (a_p1, a_p3);
   if (rc != 0) return -1;
   /*---(identify)-----------------------*/
   csize  = CIRCLE__size      (min1, min2);
   /*---(mark)---------------------------*/
   rc     = CIRCLE__update               (min1, min2, csize);
   if (rc != 0) return -1;
   CIRCLE__backward             ();
   CIRCLE__forward              ();
   /*---(complete)-----------------------*/
   /*> DEBUG_CIRCLE  POINT_list (stdout, o.key, o.nkey);                            <*/
   DEBUG_CIRCLE  printf("\n\n");
   return 1;
}

char
CIRCLE_driver      (void)
{
   DEBUG_CIRCLE  printf("CIRCLE DETECTION (begin)\n");
   /*---(locals)-------------------------*/
   int       _found  = 1;
   int       i, j;
   int       pt;
   /*---(add end points)-----------------*/
   for (i = 0; i < o.navg; ++i) {
      if (o.avg[i].type == POINT_START ) KEY_add (i, 'y', POINT_START );
      if (o.avg[i].type == POINT_FINISH) KEY_add (i, 'y', POINT_FINISH);
   }
   KEY_calc  ('c');
   /*---(report out)---------------------*/
   /*> DEBUG_CIRCLE  POINT_list (stdout, o.key, o.nkey);                            <*/
   while (_found == 1) {
      _found = 0;
      for (i = 0; i < o.nkey - 3; ++i) {
         if (strcmp(o.key[i + 1].use, "-") != 0) continue;
         for (j = i + 2; j < i + 6; ++j) {
            if (strcmp(o.key[j + 1].use, "-") != 0) break;
            DEBUG_CIRCLE  printf("\n   TEST %d/%d to %d/%d\n", i, i + 1, j, j + 1);
            _found = CIRCLE__identify (i, j);
            if (_found == 1) break;
         }
         if (_found == 1) break;
      }
   }
   /*---(put points back)----------------*/
   for (i = 0; i < o.nkey; ++i) {
      if (o.key[i].type == POINT_START || o.key[i].type == POINT_FINISH) {
         KEY_del (i);
         --i;
      }
   }
   /*---(prepare for matching)-----------*/
   KEY_prep  ();
   KEY_calc  ('n');
   /*---(report out)---------------------*/
   /*> DEBUG_CIRCLE  POINT_list (stdout, o.key, o.nkey);                            <*/
   /*---(complete)-----------------------*/
   DEBUG_CIRCLE  printf("CIRCLE DETECTION (end)\n\n");
   return 0;
}

