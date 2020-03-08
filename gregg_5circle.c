/*============================---(source-start)---============================*/
#include "gregg.h"




static int         s_min       =    7;
static int         s_max       =   20;



/*============================--------------------============================*/
/*===----                      program level                           ----===*/
/*============================--------------------============================*/
static void o___PROGRAM________________o (void) {;}

char
CIRCLE_config           (int a_min, int a_max)
{
   s_min    = a_min;
   s_max    = a_max;
   return 0;
}


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
int       s_mina      = 0;               /* circle entrance point               */
int       s_minb      = 0;               /* circle exit point                   */
int       one       = 0;               /* new circle intersection point       */
int       two       = 0;               /* new circle intersection point       */
char      use[5]    = "";              /* circle letter use                   */


char     /*----: determine if two lines intersect -----------------------*/
CIRCLE__intersect        (int a_p1, int a_p3)
{
   /*---(locals)-----------+-----+-----+-*/
   char        rce         =  -10;
   int       p1  = a_p1;               /* 1st point 1st segment               */
   int       p2  = a_p1 + 1;           /* 2nd point 1st segment               */
   int       p3  = a_p3;               /* 1st point 2nd segment               */
   int       p4  = a_p3 + 1;           /* 2nd point 2nd segment               */
   double    A1, B1, C1;               /* 1st segment calcs                   */
   double    A2, B2, C2;               /* 2nd segment calcs                   */
   double    x_det;                    /* determinant                         */
   int       x, y;                     /* hypothetical intersection           */
   int       _max    = 0;              /* axis minimum point                  */
   int       _min    = 0;              /* axis maximum point                  */
   /*---(header)-------------------------*/
   DEBUG_CIRC   yLOG_enter   (__FUNCTION__);
   DEBUG_CIRC   yLOG_complex ("checking"  , "bas/avg points %4d:%4d to %4d:%4d", p1, p2, p3, p4);
   /*---(solve segment one)--------------*/
   A1 = o.bas [p2].y_raw - o.bas [p1].y_raw;
   B1 = o.bas [p1].x_raw - o.bas [p2].x_raw;
   C1 = (A1 * o.bas [p1].x_raw) + (B1 * o.bas [p1].y_raw);
   DEBUG_CIRC   yLOG_complex ("1st seg"   , "%4d,%4d to %4d,%4d :: s=%8.3f, b=%4d :: A=%4.0f, B=%4.0f, C=%4.0f", o.bas [p1].x_raw, o.bas [p1].y_raw, o.bas [p2].x_raw, o.bas [p2].y_raw, o.bas [p2].slope, o.bas [p2].icept, A1, B1, C1);
   /*---(solve segment two)--------------*/
   A2 = o.bas [p4].y_raw - o.bas [p3].y_raw;
   B2 = o.bas [p3].x_raw - o.bas [p4].x_raw;
   C2 = (A2 * o.bas [p3].x_raw) + (B2 * o.bas [p3].y_raw);
   DEBUG_CIRC   yLOG_complex ("2nd seg"   , "%4d,%4d to %4d,%4d :: s=%8.3f, b=%4d :: A=%4.0f, B=%4.0f, C=%4.0f", o.bas [p3].x_raw, o.bas [p3].y_raw, o.bas [p4].x_raw, o.bas [p4].y_raw, o.bas [p4].slope, o.bas [p4].icept, A2, B2, C2);
   /*---(hypothetical intersection)------*/
   x_det = (A1 * B2) - (A2 * B1);
   DEBUG_CIRC   yLOG_double  ("x_det"     , x_det);
   --rce;  if (x_det == 0) {
      DEBUG_CIRC   yLOG_exitr   (__FUNCTION__, rce);
      return rce;
   }
   x  = (int) ((B2 * C1) - (B1 * C2)) / x_det;
   y  = (int) ((A1 * C2) - (A2 * C1)) / x_det;
   DEBUG_CIRC   yLOG_complex ("possible"  , "%4d, %4d", x, y);
   /*---(check 1st segment)--------------*/
   _min   = (int) fmin(o.bas [p1].x_raw, o.bas [p2].x_raw);
   _max   = (int) fmax(o.bas [p1].x_raw, o.bas [p2].x_raw);
   DEBUG_CIRC   yLOG_complex ("x on 1st"  , "%4d < %4d < %4d", _min, x, _max);
   --rce;  if (_min > x || _max < x)  {
      DEBUG_CIRC   yLOG_exitr   (__FUNCTION__, rce);
      return rce;
   }
   _min   = (int) fmin(o.bas [p1].y_raw, o.bas [p2].y_raw);
   _max   = (int) fmax(o.bas [p1].y_raw, o.bas [p2].y_raw);
   DEBUG_CIRC   yLOG_complex ("y on 1st"  , "%4d < %4d < %4d", _min, y, _max);
   --rce;  if (_min > y || _max < y) {
      DEBUG_CIRC   yLOG_exitr   (__FUNCTION__, rce);
      return rce;
   }
   /*---(check 2nd segment)--------------*/
   _min   = (int) fmin(o.bas [p3].x_raw, o.bas [p4].x_raw);
   _max   = (int) fmax(o.bas [p3].x_raw, o.bas [p4].x_raw);
   DEBUG_CIRC   yLOG_complex ("x on 2nd"  , "%4d < %4d < %4d", _min, x, _max);
   --rce;  if (_min > x || _max < x) {
      DEBUG_CIRC   yLOG_exitr   (__FUNCTION__, rce);
      return rce;
   }
   _min   = (int) fmin(o.bas [p3].y_raw, o.bas [p4].y_raw);
   _max   = (int) fmax(o.bas [p3].y_raw, o.bas [p4].y_raw);
   DEBUG_CIRC   yLOG_complex ("y on 2nd"  , "%4d < %4d < %4d", _min, y, _max);
   --rce;  if (_min > y || _max < y) {
      DEBUG_CIRC   yLOG_exitr   (__FUNCTION__, rce);
      return rce;
   }
   /*---(complete)-----------------------*/
   DEBUG_CIRC   yLOG_note    ("confirmed circle");
   DEBUG_CIRC   yLOG_exit    (__FUNCTION__);
   return 0;
}

int      /*----: determine the circle crossover point)-------------------*/
CIRCLE__fix_ends     (int a_p1, int a_p3)
{
   /*---(locals)-----------+-----+-----+-*/
   char        rce         =  -10;
   char        rc          =    0;
   int         p1          = a_p1;          /* 1st point 1st segment         */
   int         p2          = a_p1 + 1;      /* 2nd point 1st segment         */
   int         p3          = a_p3;          /* 1st point 2nd segment         */
   int         p4          = a_p3 + 1;      /* 2nd point 2nd segment         */
   int         a , ab, ae;
   int         b , bb, be;
   float       xa, ya, xb, yb;
   float       xd, yd;
   float       d;
   float       x_min       = 9999.9;
   /*---(header)-------------------------*/
   DEBUG_CIRC   yLOG_enter   (__FUNCTION__);
   DEBUG_CIRC   yLOG_complex ("checking"  , "bas/avg points %4d:%4d to %4d:%4d", p1, p2, p3, p4);
   /*---(cycle first points)-------------*/
   ab  = o.avg [p1].p_raw;
   ae  = o.avg [p2].p_raw;
   for (a = ab; a <= ae; ++a) {
      /*---(skip fake first)-------------*/
      if (a == 0)  continue;
      /*---(get position)----------------*/
      xa  = o.raw [a].x_raw;
      ya  = o.raw [a].y_raw;
      /*---(cycle second point)----------*/
      bb  = o.avg [p3].p_raw;
      be  = o.avg [p4].p_raw;
      for (b = bb; b <= be; ++b) {
         /*---(skip fake last)-----------*/
         if (b == o.nraw - 1)   continue;
         /*---(get position)----------------*/
         xb  = o.raw [b].x_raw;
         yb  = o.raw [b].y_raw;
         /*---(get diffs)----------------*/
         xd   = abs (xb - xa);
         yd   = abs (yb - ya);
         DEBUG_CIRC_M yLOG_complex ("compare"   , "%4d at %4.0f,%4.0f and %4d at %4.0f,%4.0f, diff %4.0f,%4.0f", a, xa, ya, b, xb, yb, xd, yd);
         if (xd + yd >= 10)  continue;
         /*---(calc distance)------------*/
         d    = sqrt ((xd * xd) + (yd * yd));
         /*---(check min)----------------*/
         if (d < x_min) {
            x_min     = d;
            s_mina    = a;
            s_minb    = b;
            DEBUG_CIRC_M yLOG_complex ("new min"   , "%4d and %4d at %8.3f", s_mina, s_minb, x_min);
         }
         /*---(done)---------------------*/
      }
   }
   /*---(test results)-------------------*/
   DEBUG_CIRC   yLOG_complex ("minimum"   , "%4d:%4d (%4d) %4d:%4d and %4d:%4d (%4d) %4d:%4d at %8.2f", p1, ab, s_mina, p2, ae, p3, bb, s_minb, p4, be, x_min);
   --rce;  if (x_min > 12.0) {
      DEBUG_CIRC   yLOG_note    ("minimum rejected as too large");
      s_mina = -1;
      s_minb = -1;
      DEBUG_CIRC   yLOG_exitr   (__FUNCTION__, rce);
      return rce;
   }
   /*---(add new base points)------------*/
   BASE_handler (ab, s_mina, ae);
   BASE_handler (bb, s_minb, be);
   /*> if (s_mina != ab && s_mina != ae)  BASE_adjust (p2, s_mina);                         <*/
   /*> if (s_minb != bb && s_minb != be)  BASE_adjust (p3, s_minb);                         <*/
   /*> BASE_calc_all ();                                                              <*/
   /*---(complete)-----------------------*/
   DEBUG_CIRC   yLOG_exit    (__FUNCTION__);
   return 0;
}

char
CIRCLE_idenfication     (void)
{
   short       x_min       = 9999;
   short       x_max       =-9999;
   short       y_min       = 9999;
   short       y_max       =-9999;
   int         i           =    0;
   short       x_beg       =    0;
   short       x_end       =    0;
   short       xd, yd;
   char        x_type      =  '-';
   short       x_avg       =    0;
   float       x_qtr       =  0.0;
   /*---(header)-------------------------*/
   DEBUG_CIRC   yLOG_enter   (__FUNCTION__);
   /*---(get extremes)-------------------*/
   for (i = s_mina; i <= s_minb; ++i) {
      /*---(min)---------------*/
      if (o.raw [i].x_raw < x_min)   x_min = o.raw [i].x_raw;
      if (o.raw [i].y_raw < y_min)   y_min = o.raw [i].y_raw;
      /*---(max)---------------*/
      if (o.raw [i].x_raw > x_max)   x_max = o.raw [i].x_raw;
      if (o.raw [i].y_raw > y_max)   y_max = o.raw [i].y_raw;
      /*---(done)--------------*/
   }
   xd = x_max - x_min;
   yd = y_max - y_min;
   x_avg = (xd + yd) / 2.0;
   DEBUG_CIRC   yLOG_complex ("x-extreme" , "min %4d, max %4d, diff %4d", x_min, x_max, xd);
   DEBUG_CIRC   yLOG_complex ("y-extreme" , "min %4d, max %4d, diff %4d", y_min, y_max, yd);
   DEBUG_CIRC   yLOG_value   ("area"      , xd * yd);
   DEBUG_CIRC   yLOG_value   ("average"   , x_avg);
   /*---(type)---------------------------*/
   if (x_avg <= 150)  x_type = 'e';
   else               x_type = 'a';
   /*---(mark raw points)----------------*/
   for (i = s_mina; i <= s_minb; ++i) {
      o.raw [i].marked = x_type;
   }
   /*---(mark bas/avg points)------------*/
   x_beg = o.raw [s_mina].p_bas;
   x_end = o.raw [s_minb].p_bas;
   for (i = x_beg + 1; i <= x_end; ++i) {
      o.bas [i].prekey = '/';
      o.avg [i].prekey = '/';
      o.bas [i].marked = x_type;
      o.avg [i].marked = x_type;
   }
   /*---(prekey points)------------------*/
   o.bas [x_end].prekey = o.avg [x_end].prekey = 'k';
   o.bas [x_beg].prekey = o.avg [x_beg].prekey = 'k';
   /*---(rough intermediate keys)--------*/
   x_qtr = (x_end - x_beg + 1) / 4.0;
   i = x_beg + x_qtr;
   o.bas [i    ].prekey = o.avg [i    ].prekey = 'k';
   i = x_beg + 2.0 * x_qtr;
   o.bas [i    ].prekey = o.avg [i    ].prekey = 'k';
   i = x_beg + 3.0 * x_qtr;
   o.bas [i    ].prekey = o.avg [i    ].prekey = 'k';
   /*---(complete)-----------------------*/
   DEBUG_CIRC   yLOG_exit    (__FUNCTION__);
   return 0;
}



int      /*----: size of circle at cross-section ------------------------*/
CIRCLE__size_OLD    (int a_one, int a_two)
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
   DEBUG_CIRC   printf("   search for midpoint size...\n");
   /*---(figure points)------------------*/
   half     = a_one + ((a_two - a_one) / 2);
   space    = (a_two - a_one) / 4;
   mid1     = half - space;
   mid2     = half + space;
   DEBUG_CIRC   printf("   _min1 =%3d, _min2 =%3d, space=%3d, half =%3d, mid1 =%3d, mid2 =%3d\n", a_one, a_two, space, half, mid1, mid2);
   /*---(calc midsection)----------------*/
   _distx   = abs(o.bas[mid2].x_raw - o.bas[mid1].x_raw);
   _disty   = abs(o.bas[mid2].y_raw - o.bas[mid1].y_raw);
   _dist1   = (int) sqrt((_distx * _distx) + (_disty * _disty));
   DEBUG_CIRC   printf("   midpoints  : _distx=%3d, _disty=%3d, _dist1=%3d\n", _distx, _disty, _dist1);
   o.avg[mid1].type = 'm';
   o.avg[mid2].type = 'm';
   /*---(get half mark)------------------*/
   _distx   = abs(o.bas[half].x_raw - o.bas[a_one].x_raw);
   _disty   = abs(o.bas[half].y_raw - o.bas[a_one].y_raw);
   _dist2   = (int) sqrt((_distx * _distx) + (_disty * _disty));
   DEBUG_CIRC   printf("   halfpoints : _distx=%3d, _disty=%3d, _dist2=%3d\n", _distx, _disty, _dist2);
   o.avg[half].type = 'm';
   /*---(averagge)-----------------------*/
   _dist = (_dist1 + _dist2) / 2.0;
   DEBUG_CIRC   printf("   so, _dist =%3d\n", _dist);
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
   DEBUG_CIRC   printf("\n");
   DEBUG_CIRC   printf("   extend circle intersection backwards from start...\n");
   DEBUG_CIRC   printf("      circle beg is    key = %3d, bas = %3d\n", one, o.key[one].p_bas);
   DEBUG_CIRC   printf("      circle end is    key = %3d, bas = %3d\n", two, o.key[two].p_bas);
   DEBUG_CIRC   printf("      circle midpoint  key = %3d, bas = %3d\n", one + 2, o.key[one + 2].p_bas);
   DEBUG_CIRC   printf("      total circle point are key = %3d, bas = %3d\n", o.nkey, o.nbas);
   DEBUG_CIRC   printf("      so, checking forward from bas   0 to %3d...\n", o.key[one].p_bas);
   DEBUG_CIRC   printf("      against, forward  from    bas %3d to %3d...\n", o.key[one + 2].p_bas, o.key[two].p_bas);
   if (one == 0) return 0;
   for (i = o.key[one - 1].p_bas; i < o.key[one].p_bas; ++i) {
      for (j = o.key[one + 2].p_bas; j <= o.key[two].p_bas; ++j) {
         xd   = abs(o.bas[j].x_raw - o.bas[i].x_raw);
         yd   = abs(o.bas[j].y_raw - o.bas[i].y_raw);
         dist = (int) sqrt((xd * xd) + (yd * yd));
         /*> DEBUG_CIRC   printf("      checking i=%3d, j=%3d, dist=%6.2f\n", i, j, dist);   <*/
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
      new = KEY_add (pt, 'y', POINT_NORMAL, NULL);
      if (old_nkey != o.nkey) {
         DEBUG_CIRC   printf("      ADDED KEY\n");
         one = new;
         ++two;
         KEY_label (one + 1, two - one, use);
      } else {
         DEBUG_CIRC   printf("      existing key\n");
      }
      DEBUG_CIRC   printf("      min circle is bas=%3d, key=%3d (n=%2d), dist=%6.2f, use=%-5.5s\n", pt, new, o.nkey, min, use);
   } else {
      DEBUG_CIRC   printf("      min circle already exists\n");
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
   DEBUG_CIRC   printf("\n");
   DEBUG_CIRC   printf("   extend circle intersection forwards from end...\n");
   DEBUG_CIRC   printf("      circle beg is    key = %3d, bas = %3d\n", one, o.key[one].p_bas);
   DEBUG_CIRC   printf("      circle end is    key = %3d, bas = %3d\n", two, o.key[two].p_bas);
   DEBUG_CIRC   printf("      circle midpoint  key = %3d, bas = %3d\n", two - 2, o.key[two - 2].p_bas);
   DEBUG_CIRC   printf("      total circle point are key = %3d, bas = %3d\n", o.nkey, o.nbas);
   DEBUG_CIRC   printf("      so, checking backward from bas %3d to %3d...\n", o.nkey - 1, o.key[two].p_bas);
   DEBUG_CIRC   printf("      against, forward from      bas %3d to %3d...\n", o.key[two - 1].p_bas, o.key[one].p_bas);
   if (two == o.nkey - 1) return 0;
   for (j = o.key[two + 1].p_bas; j > o.key[two].p_bas; --j) {
      for (i = o.key[two - 2].p_bas; i > o.key[one].p_bas; --i) {
         xd   = abs(o.bas[j].x_raw - o.bas[i].x_raw);
         yd   = abs(o.bas[j].y_raw - o.bas[i].y_raw);
         dist = (int) sqrt((xd * xd) + (yd * yd));
         /*> DEBUG_CIRC   printf("      checking i=%3d, j=%3d, dist=%6.2f\n", i, j, dist);   <*/
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
      new = KEY_add (pt, 'y', POINT_NORMAL, NULL);
      if (old_nkey != o.nkey) {
         DEBUG_CIRC   printf("      ADDED KEY\n");
         KEY_label (new, 1, "+");
         two = new;
      } else {
         DEBUG_CIRC   printf("      existing key\n");
      }
      DEBUG_CIRC   printf("      min circle is bas=%3d, key=%3d (n=%2d), dist=%6.2f, use=%-5.5s\n", pt, new, o.nkey, min, "+");
   } else {
      DEBUG_CIRC   printf("      min circle already exists\n");
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
      DEBUG_CIRC   printf("   REJECTED, too small\n");
      return -1;
   }
   if (a_size   >= 20     )  strncpy(use, "a", 5);
   else                      strncpy(use, "e", 5);
   DEBUG_CIRC   printf("   calling it an <<%s>>\n", use);
   /*---(create new keys)----------------*/
   one = KEY_add (a_one, 'y', POINT_NORMAL, NULL);
   two = KEY_add (a_two, 'y', POINT_NORMAL, NULL);
   DEBUG_CIRC   printf("   added one=%4d and two=%4d\n", one, two);
   /*---(get direction)------------------*/
   /*> d1 = o.key[one + 2].degs;                                                         <* 
    *> d2 = o.key[one + 3].degs;                                                         <*/
   d1 = o.key[one + 1].degs;
   d2 = o.key[one + 2].degs;
   dd = d1 - d2;
   DEBUG_CIRC   printf("   direction   :: d1=%3d, d2=%3d, dd=%3d", d1, d2, dd);
   if (dd >  180) dd -=  360;
   if (dd < -180) dd +=  360;
   DEBUG_CIRC   printf(", rdd=%3d", dd);
   if (dd >= 0) {
      DEBUG_CIRC   printf(", so clockwise\n");
   } else {
      DEBUG_CIRC   printf(", so counter-clockwise\n");
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

/*> int                                                                                            <* 
 *> CIRCLE__identify    (int a_p1, int a_p3)                                                       <* 
 *> {                                                                                              <* 
 *>    /+---(locals)-------------------------+/                                                    <* 
 *>    int       p1        = a_p1;         /+ 1st point 1st segment               +/               <* 
 *>    int       p2        = a_p1 + 1;     /+ 2nd point 1st segment               +/               <* 
 *>    int       p3        = a_p3;         /+ 1st point 2nd segment               +/               <* 
 *>    int       p4        = a_p3 + 1;     /+ 2nd point 2nd segment               +/               <* 
 *>    char      rc        = 0;            /+ generic return code                 +/               <* 
 *>    int       csize     = 0;            /+ circle size                         +/               <* 
 *>    int       i;                                                                                <* 
 *>    /+---(defense)------------------------+/                                                    <* 
 *>    if (p4 >= o.nkey)  {                                                                        <* 
 *>       DEBUG_CIRC   printf("   REJECTED, final point outside valid range\n");                   <* 
 *>       return -1;                                                                               <* 
 *>    }                                                                                           <* 
 *>    for (i = p2; i <= p4; ++i) {                                                                <* 
 *>       if (o.key[i].type == POINT_START) {                                                      <* 
 *>          DEBUG_CIRC   printf("   REJECTED, crosses a stroke start at %2d\n", i);               <* 
 *>          return -1;                                                                            <* 
 *>       }                                                                                        <* 
 *>       if (strncmp(o.key[i].use, "-", 5) != 0) {                                                <* 
 *>          DEBUG_CIRC   printf("   REJECTED, point %2d taken by a <<%s>>\n", i, o.key[i].use);   <* 
 *>          return -1;                                                                            <* 
 *>       }                                                                                        <* 
 *>    }                                                                                           <* 
 *>    DEBUG_CIRC   printf("   all points in range and valid\n");                                  <* 
 *>    /+---(intersection)-------------------+/                                                    <* 
 *>    rc       = CIRCLE__intersect (a_p1, a_p3);                                                  <* 
 *>    if (rc != 0) return -1;                                                                     <* 
 *>    /+---(minimum connection)-------------+/                                                    <* 
 *>    rc       = CIRCLE__fix_ends  (a_p1, a_p3);                                                  <* 
 *>    if (rc != 0) return -1;                                                                     <* 
 *>    /+---(identify)-----------------------+/                                                    <* 
 *>    csize  = CIRCLE__size_OLD  (s_mina, s_minb);                                                    <* 
 *>    /+---(mark)---------------------------+/                                                    <* 
 *>    rc     = CIRCLE__update               (s_mina, s_minb, csize);                                  <* 
 *>    if (rc != 0) return -1;                                                                     <* 
 *>    CIRCLE__backward             ();                                                            <* 
 *>    CIRCLE__forward              ();                                                            <* 
 *>    /+---(complete)-----------------------+/                                                    <* 
 *>    /+> DEBUG_CIRC   POINT_list (stdout, o.key, o.nkey);                            <+/         <* 
 *>    DEBUG_CIRC   printf("\n\n");                                                                <* 
 *>    return 1;                                                                                   <* 
 *> }                                                                                              <*/

/*> char                                                                                     <* 
 *> CIRCLE_driver_OLD      (void)                                                            <* 
 *> {                                                                                        <* 
 *>    DEBUG_CIRC   printf("CIRCLE DETECTION (begin)\n");                                    <* 
 *>    /+---(locals)-------------------------+/                                              <* 
 *>    int       _found  = 1;                                                                <* 
 *>    int       i, j;                                                                       <* 
 *>    int       pt;                                                                         <* 
 *>    /+---(add end points)-----------------+/                                              <* 
 *>    for (i = 0; i < o.navg; ++i) {                                                        <* 
 *>       if (o.avg[i].type == POINT_START ) KEY_add (i, 'y', POINT_START , NULL);           <* 
 *>       if (o.avg[i].type == POINT_FINISH) KEY_add (i, 'y', POINT_FINISH, NULL);           <* 
 *>    }                                                                                     <* 
 *>    KEY_calc  ('c');                                                                      <* 
 *>    /+---(report out)---------------------+/                                              <* 
 *>    /+> DEBUG_CIRC   POINT_list (stdout, o.key, o.nkey);                            <+/   <* 
 *>    while (_found == 1) {                                                                 <* 
 *>       _found = 0;                                                                        <* 
 *>       for (i = 0; i < o.nkey - 3; ++i) {                                                 <* 
 *>          if (strcmp(o.key[i + 1].use, "-") != 0) continue;                               <* 
 *>          for (j = i + 2; j < i + 6; ++j) {                                               <* 
 *>             if (strcmp(o.key[j + 1].use, "-") != 0) break;                               <* 
 *>             DEBUG_CIRC   printf("\n   TEST %d/%d to %d/%d\n", i, i + 1, j, j + 1);       <* 
 *>             _found = CIRCLE__identify (i, j);                                            <* 
 *>             if (_found == 1) break;                                                      <* 
 *>          }                                                                               <* 
 *>          if (_found == 1) break;                                                         <* 
 *>       }                                                                                  <* 
 *>    }                                                                                     <* 
 *>    /+---(put points back)----------------+/                                              <* 
 *>    for (i = 0; i < o.nkey; ++i) {                                                        <* 
 *>       if (o.key[i].type == POINT_START || o.key[i].type == POINT_FINISH) {               <* 
 *>          KEY_del (i);                                                                    <* 
 *>          --i;                                                                            <* 
 *>       }                                                                                  <* 
 *>    }                                                                                     <* 
 *>    /+---(prepare for matching)-----------+/                                              <* 
 *>    KEY_prep  ();                                                                         <* 
 *>    KEY_calc  ('n');                                                                      <* 
 *>    /+---(report out)---------------------+/                                              <* 
 *>    /+> DEBUG_CIRC   POINT_list (stdout, o.key, o.nkey);                            <+/   <* 
 *>    /+---(complete)-----------------------+/                                              <* 
 *>    DEBUG_CIRC   printf("CIRCLE DETECTION (end)\n\n");                                    <* 
 *>    return 0;                                                                             <* 
 *> }                                                                                        <*/

char
CIRCLE_driver           (void)
{
   char        rc          =    0;
   int         a, b;
   int         x_beg       =    0;
   int         x_end       =    0;
   int         c           =    0;
   /*---(header)-------------------------*/
   DEBUG_CIRC   yLOG_enter   (__FUNCTION__);
   DEBUG_CIRC   yLOG_value   ("s_min"     , s_min);
   DEBUG_CIRC   yLOG_value   ("s_max"     , s_max);
   for (a = 0; a < o.navg - s_min; ++a) {
      if (o.bas [a].type == POINT_TAIL)    continue;
      if (o.bas [a].type == POINT_FINISH)  continue;
      x_beg = a + s_min;
      x_end = a + s_max;
      if (x_end >= o.navg)  x_end = o.navg - 1;
      for (b = x_beg; b < x_end; ++b) {
         if (o.bas [b].type == POINT_TAIL)    break;
         if (o.bas [b].type == POINT_FINISH)  break;
         rc = CIRCLE__intersect (a, b);
         if (rc < 0)  continue;
         rc = CIRCLE__fix_ends  (a, b);
         CIRCLE_idenfication    ();
         a = b;
         ++c;
         break;
      }
   }
   DEBUG_CIRC   yLOG_exit    (__FUNCTION__);
   return 0;
}

