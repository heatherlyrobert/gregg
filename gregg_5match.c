/*============================---(source-start)---============================*/

#include "gregg.h"





/*============================--------------------============================*/
/*===----                           matching                           ----===*/
/*============================--------------------============================*/

static struct {
   /*---(overall)--------------*/
   short       beg;
   short       opens;
   short       count;
   /*---(linear)---------------*/
   float       xd;                       /* x-axis difference                   */
   float       yd;                       /* y-axis difference                   */
   float       len;                      /* length                              */
   char        range;
   char        range2;
   char        size;                     /* length category                     */
   float       degs;                     /* degree                              */
   float       slope;                    /* slope                               */
   float       icept;
   char        quad;
   /*---(curvature)------------*/
   char        flow       [LEN_LABEL];     /* outline/actual quadrant flow   */
   float       depth;
   float       ratio;
   char        ccat;
   /*---(deprecated)-----------*/
   int         norm;                     /* number of smooth open points        */
   int         sharps;                   /* number of sharp open points at end  */
   char        oshape[10];               /* outline/actual sharp/smooth shape   */
   char        gshape[10];               /* group sharp/smooth shape for test   */
   char        qs;
} opens;


static char combo_driver       (int a_beg, int a_count);



/*============================--------------------============================*/
/*===----                             support                          ----===*/
/*============================--------------------============================*/
static void      o___SUPPORT_________________o (void) {;}

char
MATCH__calc_call        (short a_start, short a_finish)
{
   /*---(locals)-----------+-----+-----+-*/
   short       x_beg       =    0;
   short       x_end       =    0;
   /*---(header)-------------------------*/
   DEBUG_MATCH   yLOG_enter   (__FUNCTION__);
   /*---(assign base points)--------------------*/
   x_beg   = o.key [a_start ].p_bas;
   x_end   = o.key [a_finish].p_bas;
   /*---(get overall calcs)---------------------*/
   o.tmp [0].p_bas   = a_start;
   o.tmp [0].x_raw   = o.avg [x_beg].x_raw;
   o.tmp [0].y_raw   = o.avg [x_beg].y_raw;
   o.tmp [1].p_bas   = a_finish;
   o.tmp [1].x_raw   = o.avg [x_end].x_raw;
   o.tmp [1].y_raw   = o.avg [x_end].y_raw;
   /*---(report out)----------------------------*/
   POINT_calc (POINTS_TMP, o.tmp + 1, 'n');
   DEBUG_MATCH   yLOG_complex ("start"     , "%2dk, %3db %4.0fx %4.0fy", a_start , x_beg, o.tmp [0].x_raw, o.tmp [0].y_raw);
   DEBUG_MATCH   yLOG_complex ("finish"    , "%2dk, %3db %4.0fx %4.0fy", a_finish, x_end, o.tmp [1].x_raw, o.tmp [1].y_raw);
   /*---(save to structure)---------------------*/
   opens.xd     = o.tmp [1].xd;
   opens.yd     = o.tmp [1].yd;
   opens.len    = o.tmp [1].len;
   opens.slope  = o.tmp [1].slope;
   opens.icept  = o.tmp [1].icept;
   opens.degs   = o.tmp [1].degs;
   opens.quad   = o.tmp [1].quad;
   opens.qs     = o.key [a_start + 1].quad;
   DEBUG_MATCH   yLOG_complex ("calcs"     , "%4.0fxd %4.0fyd %4.0fl %8.2fs %8.2fi %4.0fd %1d", opens.xd, opens.yd, opens.len, opens.slope, opens.icept, opens.degs, opens.quad);
   /*---(fill in rest)--------------------------*/
   opens.size   = MATCH_size  (opens.len);
   opens.range  = MATCH_range (opens.degs);
   opens.depth  = 0.0;
   opens.ratio  = 0.0;
   opens.ccat   = 0;
   /*---(complete)-----------------------*/
   DEBUG_MATCH   yLOG_exit    (__FUNCTION__);
   return 0;
}

char
MATCH__curvature        (short a_start, short a_finish)
{
   /*   0 = line
    *   1 = bendy line
    *   2 = normal curve
    *   3 = big curve, arc
    *   4 = balloon
    */
   /*---(locals)-----------+-----+-----+-*/
   short       x_beg       =    0;
   short       x_end       =    0;
   int         i           =    0;
   float       xr, yr;
   float       xp, yp;
   float       xd, yd, fd;
   float       s1, s2;
   float       b1, b2;
   char        x_label      [LEN_LABEL];
   float lcurve   = 0.0;
   float rcurve   = 0.0;
   float theta    = 0.0;
   int   thetad   = 0;
   /*---(header)-------------------------*/
   DEBUG_MATCH   yLOG_enter   (__FUNCTION__);
   DEBUG_MATCH   yLOG_complex ("args"      , "%3ds, %3df", a_start, a_finish);
   /*---(assign base points)--------------------*/
   x_beg   = o.key [a_start ].p_bas;
   x_end   = o.key [a_finish].p_bas;
   DEBUG_MATCH   yLOG_complex ("bases"     , "%3ds, %3df", x_beg, x_end);
   /*> DEBUG_CURV   printf("RUNNING CURVE CALCS\n");                                  <*/
   /*> DEBUG_CURV   printf("so  slope = %8.3f and intercept = %4d\n", opens.slope, opens.icept);   <*/
   /*> DEBUG_CURV   printf("   pt# -xx- -yy- --lx-- --ly-- --dx-- --dy-- theta degs curve\n");   <*/
   for (i = x_beg; i <= x_end; ++i) {
      /*---(get raw)----------------------------*/
      xr   = o.bas [i].x_raw;
      yr   = o.bas [i].y_raw;
      /*---(save line one values)---------------*/
      s1   = opens.slope;
      b1   = opens.icept;
      /*---(if horizontal)----------------------*/
      if (s1 > -0.01 && s1 < 0.01) {
         strlcpy (x_label, "horz", LEN_LABEL);
         yp   = b1;
         xp   = xr;
         s2 = b2 = 0.0;
      }
      /*---(if vertical)------------------------*/
      else if (s1 < -999.0 || s1 > 999.0) {
         strlcpy (x_label, "vert", LEN_LABEL);
         xp   = o.bas [x_beg].x_raw;
         yp   = yr;
         s2 = b2 = 0.0;
      }
      /*---(normal)-----------------------------*/
      else {
         strlcpy (x_label, "norm", LEN_LABEL);
         /*---(get perpendicular values)-----------*/
         s2   = -1.0 / s1;
         if (s2 ==   0.0)    s2 =  0.001;
         if (s2 >  999.0)    s1 =  999.0;
         if (s2 < -999.0)    s2 = -999.0;
         b2   = yr - (s2 * xr);
         if (b2 >  999.0)    b2 =  999.0;
         if (b2 < -999.0)    b2 = -999.0;
         /*---(calc intersection)------------------*/
         xp   = (b2 - b1) / (s1 - s2);
         if (xp >  999.0)    xp =  999.0;
         if (xp < -999.0)    xp = -999.0;
         yp   = (s1 * xp) + b1;
         if (yp >  999.0)    yp =  999.0;
         if (yp < -999.0)    yp = -999.0;
         DEBUG_MATCH   yLOG_complex ("details"   , "%6.0fb2, %6.0fb1, %6.0fbd, %6.1fs1, %6.1fs2, %6.1fsd, %6.1fcx, %6.1fxp, %6.1fcy, %6.1fyp", b1, b2, b2 - b1, s1, s2, s1 - s2, (b2 - b1) / (s1 - s2), xp, (s1 * xp) + b1, yp);
      }
      /*---(calc differences)-------------------*/
      xd   = xr - xp;
      if (xd >  999) xd =  999;
      if (xd < -999) xd = -999;
      yd   = yr - yp;
      if (yd >  999) yd =  999;
      if (yd < -999) yd = -999;
      fd   = sqrt ((xd * xd) + (yd * yd));
      /*---(get the angles)---------------------*/
      /*> if (xd == 0.0)  opens.depth = 0.0;                                         <* 
       *> else {                                                                      <* 
       *>    theta   = atan (yd / xd);                                                <* 
       *>    thetad  = theta * RAD2DEG;                                               <* 
       *>    opens.depth = fabs (xd * sin (theta));                                  <* 
       *> }                                                                           <*/
      DEBUG_MATCH   yLOG_complex ("main line" , "%3db, %4.0fxr %4.0fyr, %6.1fs1 %6.0fb1", i, xr, yr, s1, b1);
      DEBUG_MATCH   yLOG_complex ("intersect" , "%4.4s, %4.0fxp %4.0fyp, %6.1fs2 %6.0fb2", x_label, xp, yp, s2, b2);
      DEBUG_MATCH   yLOG_complex ("dist"      , "%4.0fxd %4.0fyd %4.0ffd %4.0fcd", xd, yd, fd, opens.depth);
      /*---(set the curve)----------------------*/
      /*> if (opens.depth > lcurve) lcurve = opens.depth;                           <* 
       *> if (opens.depth < rcurve) rcurve = opens.depth;                           <* 
       *> if (fabs(opens.depth) > fabs(opens.cm)) opens.cm = opens.depth;           <*/
      if (fd > opens.depth)  opens.depth = fd;
   }
   /*---(categorize)---------------------*/
   if      (opens.depth >=  25.0)                                   opens.ccat = +4;
   else if (opens.depth >=  16.0)                                   opens.ccat = +3;
   else if (opens.depth >=   7.0)                                   opens.ccat = +2;
   else if (opens.depth >=  (opens.len * 0.15) || opens.depth >=  4.0) opens.ccat = +1;
   o.tmp[1].depth  = opens.depth;
   o.tmp[1].ccat    = opens.ccat;
   /*---(complete)-----------------------*/
   DEBUG_MATCH   yLOG_exit    (__FUNCTION__);
   return 0;
}

char
MATCH_calc              (short a_start, char a_count)
{
   /*---(locals)-----------+-----+-----+-*/
   short       x_finish    =    0;
   short       x_beg       =    0;
   short       x_end       =    0;
   int         i           =    0;
   /*---(header)-------------------------*/
   DEBUG_MATCH   yLOG_enter   (__FUNCTION__);
   /*---(prepare)-------------------------------*/
   x_finish  = a_start + a_count;
   MATCH__calc_call (a_start, x_finish);
   /*> POINT_curve (a_start, x_finish, o.tmp [1].len, o.tmp [1].slope, o.tmp [1].icept, &(opens.depth), &(opens.ratio), &(opens.ccat));   <*/
   POINT_curve (a_start, x_finish, o.tmp [1].len, o.tmp [1].slope, o.tmp [1].icept, &(o.tmp [1].depth), &(o.tmp [1].ratio), &(o.tmp [1].ccat));
   opens.depth = o.tmp [1].depth;
   opens.ratio = o.tmp [1].ratio;
   opens.ccat  = o.tmp [1].ccat;
   /*---(put back)------------------------------*/
   if (a_count == 1) {
      o.key [x_finish].depth  = opens.depth;
      o.key [x_finish].ccat   = opens.ccat;
   }
   /*---(complete)-----------------------*/
   DEBUG_MATCH   yLOG_exit    (__FUNCTION__);
   return 0;
}

char
MATCH_calc_OLD          (short a_start, char a_count)
{
   /*---(locals)-----------+-----+-----+-*/
   short       x_finish    =    0;
   short       x_beg       =    0;
   short       x_end       =    0;
   int         i           =    0;
   /*---(header)-------------------------*/
   DEBUG_MATCH   yLOG_enter   (__FUNCTION__);
   /*---(prepare)-------------------------------*/
   x_finish  = a_start + a_count;
   MATCH__calc_call (a_start, x_finish);
   /*---(get points)----------------------------*/
   /*> int         i           = 0;                                                   <* 
    *> int         x_beg         = o.key [a_start              ].p_bas;               <* 
    *> int x_end         = o.key [a_start + a_count    ].p_bas;                       <*/
   /*---(get coordinates)-----------------------*/
   /*> int yy1        = o.avg [x_beg].y_raw;                                          <* 
    *> int yy4        = o.avg [x_end].y_raw;                                          <* 
    *> int xx1        = o.avg [x_beg].x_raw;                                          <* 
    *> int xx4        = o.avg [x_end].x_raw;                                          <*/
   /*---(get overall calcs)---------------------*/
   /*> o.tmp [0].p_bas   = a_start;                                                   <* 
    *> o.tmp [0].x_raw   = o.avg [x_beg].x_raw;                                       <* 
    *> o.tmp [0].y_raw   = o.avg [x_beg].y_raw;                                       <* 
    *> o.tmp [1].p_bas   = a_start + a_count;                                         <* 
    *> o.tmp [1].x_raw   = xx4;                                                       <* 
    *> o.tmp [1].y_raw   = yy4;                                                       <* 
    *> POINT_calc (POINTS_TMP, o.tmp + 1, 'n');                                       <*/
   /*---(more variables)------------------------*/
   /*> opens.xd   = o.tmp[1].xd;                                                      <* 
    *> opens.yd   = o.tmp[1].yd;                                                      <* 
    *> opens.len  = o.tmp[1].len;                                                     <* 
    *> opens.slope    = o.tmp[1].slope;                                               <* 
    *> opens.icept    = o.tmp[1].icept;                                               <* 
    *> opens.degs    = o.tmp[1].degs;                                                 <* 
    *> opens.quad    = o.tmp[1].quad;                                                 <* 
    *> opens.qs   = o.key[a_start + 1].quad;                                          <* 
    *> opens.size = MATCH_size (opens.len);                                           <* 
    *> opens.range = MATCH_range (opens.degs);                                        <* 
    *> opens.depth    = 0;                                                           <* 
    *> opens.ccat = 0;                                                                <* 
    *> opens.cm   = 0;                                                                <*/
   /*---(arch)----------------------------------*/
   float lcurve   = 0.0;
   float rcurve   = 0.0;
   float theta    = 0.0;
   int   thetad   = 0;
   float       xr, yr;
   float       xp, yp;
   float       xd, yd, fd;
   /*---(header)-------------------------*/
   DEBUG_MATCH   yLOG_enter   (__FUNCTION__);
   /*---(assign base points)--------------------*/
   x_beg   = o.key [a_start ].p_bas;
   x_end   = o.key [x_finish].p_bas;
   /*> DEBUG_CURV   printf("RUNNING CURVE CALCS\n");                                  <*/
   /*> DEBUG_CURV   printf("so  slope = %8.3f and intercept = %4d\n", opens.slope, opens.icept);   <*/
   /*> DEBUG_CURV   printf("   pt# -xx- -yy- --lx-- --ly-- --dx-- --dy-- theta degs curve\n");   <*/
   for (i = x_beg + 1; i < x_end; ++i) {
      /*---(get raw)----------------------------*/
      xr   = o.bas [i].x_raw;
      yr   = o.bas [i].y_raw;
      /*---(prepare)----------------------------*/
      if (opens.slope == 0.0) opens.slope = 0.001;
      /*---(calc predicted)---------------------*/
      xp  = (o.bas[i].y_raw - opens.icept) / opens.slope;
      if (xp >  999) xp =  999;
      if (xp < -999) xp = -999;
      yp  = (opens.slope * o.bas[i].x_raw) + opens.icept;
      if (yp >  999) yp =  999;
      if (yp < -999) yp = -999;
      /*---(calc differences)-------------------*/
      xd   = xr - xp;
      if (xd >  999) xd =  999;
      if (xd < -999) xd = -999;
      yd   = (o.bas[i].y_raw    - yp);
      if (yd >  999) yd =  999;
      if (yd < -999) yd = -999;
      fd   = sqrt ((xd * xd) + (yd * yd));
      /*---(get the angles)---------------------*/
      theta   = atan (yd / xd);
      thetad  = theta * RAD2DEG;
      opens.depth = fabs (xd * sin (theta));
      DEBUG_MATCH   yLOG_complex ("point"     , "%3db, %4.0fxr %4.0fyr, %4.0fxp %4.0fyp, %4.0fxd %4.0fyd %4.0ffd %4.0fcd", i, xr, yr, xp, yp, xd, yd, fd, opens.depth);
      /*---(set the curve)----------------------*/
      switch (opens.range) {
      case 1:  if (xd > 0.0) opens.depth *= -1;  break;
      case 2:  if (yd < 0.0) opens.depth *= -1;  break;
      case 3:  if (xd < 0.0) opens.depth *= -1;  break;
      case 4:  if (xd < 0.0) opens.depth *= -1;  break;
      case 5:  if (xd < 0.0) opens.depth *= -1;  break;
      case 6:  if (yd < 0.0) opens.depth *= -1;  break;
      case 7:  if (xd < 0.0) opens.depth *= -1;  break;
      case 8:  if (xd < 0.0) opens.depth *= -1;  break;
      }
      /*> printf("   %3d %4d %4d %6.1f %6.1f %6.1f %6.1f %5.2f %4d %5.1f\n",                      <* 
       *>       i, o.bas[i].x_raw, o.bas[i].y_raw, xp, yp, xd, yd, theta, thetad, opens.depth);   <*/
      if (opens.depth > lcurve) lcurve = opens.depth;
      if (opens.depth < rcurve) rcurve = opens.depth;
      /*> if (fabs(opens.depth) > fabs(opens.cm)) opens.cm = opens.depth;             <*/
   }
   /*> DEBUG_CURV   printf("   pt# -xx- -yy- --lx-- --ly-- --dx-- --dy-- theta degs curve\n");   <*/
   /*> opens.depth  = opens.cm;                                                       <*/
   opens.ccat = -9;
   /*---(positive curves)----------------*/
   /*   O = line
    *   1 = bendy line
    *   2 = curve
    *   3 = arc
    *   4 = balloon
    */
   if      (opens.depth >=  25.0)                                   opens.ccat = +4;
   else if (opens.depth >=  16.0)                                   opens.ccat = +3;
   else if (opens.depth >=   7.0)                                   opens.ccat = +2;
   else if (opens.depth >=  (opens.len * 0.15) || opens.depth >=  4.0)    opens.ccat = +1;
   /*---(negative curves)----------------*/
   else if (opens.depth <= -25.0)                                   opens.ccat = -4;
   else if (opens.depth <= -16.0)                                   opens.ccat = -3;
   else if (opens.depth <=  -7.0)                                   opens.ccat = -2;
   else if (opens.depth <= -(opens.len * 0.15) || opens.depth <= -4.0)    opens.ccat = -1;
   else                                                         opens.ccat =  0;
   o.tmp[1].depth   = opens.depth;
   o.tmp[1].ccat  = opens.ccat;
   /*> printf("between key points %d/%d (bas %02d/%02d) lcurve=%5.1f, rcurve=%5.1f, maxcurve=%5.1f, curve=%5.1f, curvec=%2d\n",   <* 
    *>       a_start, a_start + a_count, p1, p4, lcurve, rcurve, opens.cm, opens.depth, opens.ccat);                                    <*/
   /*---(other)---------------------------------*/
   o.tmp[1].range  = opens.range;
   o.tmp[0].type   = o.tmp[1].type   = '-';
   strncpy(o.tmp[0].use, "-", 3);
   strncpy(o.tmp[1].use, "-", 3);
   /*---(complete)------------------------------*/
   /*> printf("deg = %4d, range = %2d; and len = %4d, size = %d; and curve = %6.2f, curvea = %2d\n",   <* 
    *>       opens.d, opens.range, opens.len, size, opens.depth, opens.ccat);                                                 <*/
   /*---(complete)-----------------------*/
   DEBUG_MATCH   yLOG_exit    (__FUNCTION__);
   return 0;
}

char
MATCH_size              (short a_len)
{
   /*---(size)----------------------------------*/
   /* smaller looks like 6/20/35
    * larger          is 6/24/40
    */
   int    x_size  = -1;
   if      (a_len <  SIZE_SML_MIN * 10.0)    x_size = 0;
   else if (a_len <= SIZE_SML_MAX * 10.0)    x_size = 1;
   else if (a_len <= SIZE_MED_MAX * 10.0)    x_size = 2;
   else if (a_len <= SIZE_LRG_MAX * 10.0)    x_size = 3;
   else                                      x_size = 4;
   return x_size;
}

char
MATCH_range             (int a_deg)
{
   /*---(locals)-----------+-----+-----+-*/
   int         i           =    0;
   int         x_range     =   -1;
   /*---(header)-------------------------*/
   DEBUG_MATCH   yLOG_senter  (__FUNCTION__);
   /*---(range)--------------------------*/
   DEBUG_MATCH   yLOG_sint    (a_deg);
   while (a_deg >  180) a_deg -= 360;
   while (a_deg < -180) a_deg += 360;
   DEBUG_MATCH   yLOG_sint    (a_deg);
   opens.range2  =  0;
   for (i = 0; i < MAX_RANGES; ++i) {
      /*---(catch end)-------------------*/
      if (strncmp (g_ranges [i].nam, "EOF", 5) == 0) break;
      /*---(filter)----------------------*/
      if (a_deg <= g_ranges [i].min) continue;
      if (a_deg >  g_ranges [i].max) continue;
      /*---(save)------------------------*/
      if (x_range < 0) x_range       = g_ranges [i].num;
      else             opens.range2  = g_ranges [i].num;
      /*---(done)------------------------*/
   }
   DEBUG_MATCH   yLOG_sint    (x_range);
   /*---(header)-------------------------*/
   DEBUG_MATCH   yLOG_sexit   (__FUNCTION__);
   return x_range;
}

char
MATCH_flow              (short a_beg, short a_len)
{
   /*---(locals)-----------+-----+-----+-*/
   char        rce         =  -10;
   short       i           =    0;
   char        x_flow      [LEN_LABEL] = "-------";
   /*---(header)-------------------------*/
   DEBUG_MATCH   yLOG_senter  (__FUNCTION__);
   DEBUG_MATCH   yLOG_sint    (a_beg);
   DEBUG_MATCH   yLOG_sint    (a_len);
   /*---(defense)------------------------*/
   --rce;  if (a_len > 7) {
      DEBUG_MATCH   yLOG_sexitr  (__FUNCTION__, rce);
      return rce;
   }
   /*---(populate)-----------------------*/
   for (i = 0; i < a_len; ++i) {
      x_flow [i] = o.key [a_beg + i + 1].quad + '0';
   }
   DEBUG_MATCH   yLOG_snote   (x_flow);
   /*---(return)-------------------------*/
   strlcpy (opens.flow, x_flow, LEN_LABEL);
   /*---(header)-------------------------*/
   DEBUG_MATCH   yLOG_sexit   (__FUNCTION__);
   return 0;
}





/*============================--------------------============================*/
/*===----                            matching                          ----===*/
/*============================--------------------============================*/
static void      o___MATCHING________________o (void) {;}

static char     /*----: compare the open shape to the rule ---------------------*/
match_flow         (int a_rule, int a_beg)
{
   /*---(locals)-------------------------*/
   int       i         = 0;
   int       quad      = 0;                 /* current quadrant quad          */
   int       curr      = 0;                 /* current quadrant quad          */
   char      match     = 1;
   int       len       = 0;                 /* length of flow                 */
   /*---(prepare)------------------------*/
   len   = strlen(g_groups [a_rule].flow );
   /*---(process)------------------------*/
   for (i = 0; i < 4; ++i) {
      /*---(defense)---------------------*/
      if (i >= len || strchr("0- _", g_groups [a_rule].flow [i]) != 0) {
         DEBUG_MATCH   printf("... ");
         continue;
      }
      /*---(gather)----------------------*/
      quad  = g_groups [a_rule].flow [i] - '0';
      curr  = o.key[a_beg + i + 1].quad;
      /*---(grade)-----------------------*/
      if (quad != curr) {
         DEBUG_MATCH   printf("%1dx%1d ", curr, quad);
         match = 0;
      } else {
         DEBUG_MATCH   printf("%1d=%1d ", curr, quad);
      }
   }
   /*---(summary)------------------------*/
   if (match == 1) {
      DEBUG_MATCH   printf(": ");
   } else {
      DEBUG_MATCH   printf(": ---- ---- ---- [ bad flow  ]\n");
   }
   /*---(complete)-----------------------*/
   return match;
}

/*> static char     /+----: compare the open shape to the rule ---------------------+/          <* 
 *> match_shape        (int a_rule, int a_beg)                                                  <* 
 *> {                                                                                           <* 
 *>    /+---(locals)-------------------------+/                                                 <* 
 *>    int       i         = 0;                                                                 <* 
 *>    char      match     = 1;                                                                 <* 
 *>    int       len       = 0;                 /+ length of shape                +/            <* 
 *>    char      curr      = ' ';               /+ current quadrant curve         +/            <* 
 *>    /+---(init)---------------------------+/                                                 <* 
 *>    strncpy (opens.gshape, g_groups [a_rule].sh, 10);                                        <* 
 *>    len   = strlen(opens.gshape);                                                            <* 
 *>    /+---(compare)------------------------+/                                                 <* 
 *>    for (i = 0; i < 3; ++i) {                                                                <* 
 *>       /+> curr  = o.key[a_beg + i + 1].cano;                                          <+/   <* 
 *>       if        (i >= len) {                                                                <* 
 *>          DEBUG_MATCH   printf("... ");                                                      <* 
 *>          continue;                                                                          <* 
 *>       } else if (opens.gshape[i]  == ' ') {                                                 <* 
 *>          DEBUG_MATCH   printf("... ");                                                      <* 
 *>          continue;                                                                          <* 
 *>       } else if (opens.gshape[i]  == 'x') {                                                 <* 
 *>          DEBUG_MATCH   printf("%c<%c ", curr, opens.gshape[i]);                             <* 
 *>          continue;                                                                          <* 
 *>       } else if (curr             == '-') {                                                 <* 
 *>          DEBUG_MATCH   printf("%c>%c ", curr, opens.gshape[i]);                             <* 
 *>          continue;                                                                          <* 
 *>       } else {                                                                              <* 
 *>          DEBUG_MATCH   printf("%c!%c ", curr, opens.gshape[i]);                             <* 
 *>          match = 0;                                                                         <* 
 *>          continue;                                                                          <* 
 *>       }                                                                                     <* 
 *>    }                                                                                        <* 
 *>    /+---(summary)------------------------+/                                                 <* 
 *>    if (match == 1) {                                                                        <* 
 *>       DEBUG_MATCH   printf(": ");                                                           <* 
 *>    } else {                                                                                 <* 
 *>       DEBUG_MATCH   printf(": --- --- --- --- : ---- ---- ---- [ bad shape ]\n");           <* 
 *>    }                                                                                        <* 
 *>    /+---(complete)-----------------------+/                                                 <* 
 *>    return match;                                                                            <* 
 *> }                                                                                           <*/

static char     /*----: get number of open points from current -----------------*/
MATCH__available   (int a_beg)
{
   /*---(locals)-------------------------*/
   int   end       = 0;      /* last open point (before end or sharp)         */
   char  cutoff    = 0;
   /*---(cycle)--------------------------*/
   opens.norm   = 0;
   opens.sharps   = 0;
   strlcpy (opens.oshape, "     ", LEN_TERSE);
   strlcpy (opens.flow  , "     ", LEN_TERSE);
   for (end = a_beg + 1; end < o.nkey; ++end) {
      if (strncmp(o.key[end].use, "-", 5) != 0)  break;
      /*---(count)-----------------------*/
      ++opens.sharps;
      if (cutoff == 0) ++opens.norm;
      /*---(flow)------------------------*/
      if (opens.sharps <= 5) {
         /*> opens.oshape[opens.sharps - 1] = o.key[end].cano;                        <*/
         opens.flow [opens.sharps - 1] = o.key[end].quad + '0';
      }
      /*---(check)-----------------------*/
      /*> if (o.key[end].cano != '-') cutoff = 1;                                     <*/
   }
   --end;
   /*---(report out)---------------------*/
   /*> if (opens.sharps > 0) {                                                                       <* 
    *>    printf("\n   OPEN : beg=%2d, end=%2d, opens=%2d, nines=%2d extra   [%-5.5s] [%-5.5s]\n",   <* 
    *>          a_beg, end, opens.norm, opens.sharps - opens.norm, opens.oshape, opens.flow);       <* 
    *> }                                                                                             <*/
   /*---(complete)-----------------------*/
   return 0;
}

static char     /*----: clean out jogs, jitters, and tails ---------------------*/
MATCH__prep        (void)
{
   /*---(locals)-------------------------*/
   int       i         = 0;                 /* loop iterator -- key points    */
   int       nclean    = 0;                 /* number of cleaned segments     */
   /*---(clean out little leftovers)-----*/
   for (i = 1; i < o.nkey; ++i) {
      if (strncmp(o.key[i].use, "-", 5) != 0) continue;
      if (o.key[i].len <= 5) {
         KEY_label(i, 1, "*");
         ++nclean;
      }
   }
   /*---(report out)---------------------*/
   /*> if (nclean > 0) printf("   cleansed jogs, jitters, and tails\n");           <* 
    *> else            printf("   no prep up needed on little segments\n");        <*/
   /*---(complete)-----------------------*/
   return 0;
}

static char     /*----: clean out small, unused segments -----------------------*/
match_cleanup      (void)
{
   /*---(locals)-------------------------*/
   int       i         = 0;                 /* loop iterator -- key points    */
   int       nclean    = 0;                 /* number of cleaned segments     */
   int       missed    = 0;                 /* number of missed segments      */
   /*---(clean out little leftovers)-----*/
   for (i = 1; i < o.nkey; ++i) {
      if (strncmp(o.key[i].use, "-", 5) != 0) continue;
      if (o.key[i].len < SIZE_SML_MIN) {
         KEY_label(i, 1, "*");
         ++nclean;
      } else {
         ++missed;
      }
   }
   /*---(report out)---------------------*/
   /*> if (nclean > 0) printf("   cleaned up little leftover segment(s)\n");            <* 
    *> else            printf("   no clean up needed on little leftover segments\n");   <* 
    *> if (missed > 0) printf("   NOT DONE (missing some segments)\n");                 <* 
    *> else            printf("   all segments accounted for\n");                       <*/
   /*---(complete)-----------------------*/
   return 0;
}

static char     /*----: report out result of matching --------------------------*/
match_result       (void)
{
   /*---(locals)-------------------------*/
   int       i         = 0;                 /* loop iterator -- key points    */
   char      bad       = 0;
   /*---(actual)-------------------------*/
   DEBUG_MATCH   printf("   actual = \">");
   strncpy(o.actual,  ">", 50);
   strncpy(o.letters, ">", 50);
   for (i = 1; i < o.nkey; ++i) {
      strncat(o.letters, "."     , 50);
      strncat(o.letters, o.key[i].use, 50);
      if (  strncmp(o.key[i].use, "*", 5) != 0 &&
            strncmp(o.key[i].use, "+", 5) != 0) {
         DEBUG_MATCH   printf(".%s", o.key[i].use);
         strncat(o.actual, "."       , 50);
         strncat(o.actual, o.key[i].use, 50);
      }
      if (strncmp(o.key[i].use, "-", 5) == 0) bad = 1;
   }
   DEBUG_MATCH   printf("\"\n");
   /*---(expect)-------------------------*/
   DEBUG_MATCH   printf("   expect = \"%s\"\n", o.expect);
   /*---(grade)--------------------------*/
   if        (bad == 1) {
      strncpy(o.grade, "BAD", 50);
   } else if (strncmp(o.expect, "",       50) == 0) {
      strncpy(o.grade, "null", 50);
   } else if (strncmp(o.actual, o.expect, 50) == 0) {
      strncpy(o.grade, "pass", 50);
   } else {
      strncpy(o.grade, "FAIL", 50);
   }
   DEBUG_MATCH   printf("   graded as a %s", o.grade);
   /*---(complete)-----------------------*/
   return 0;
}

static char     /*----: apply locations in turn --------------------------------*/
match_location     (int a_rule, int a_beg, int a_opens, int a_size)
{
   /*---(locals)-------------------------*/
   int       j         = 0;                 /* loop iterator -- letters       */
   int       k         = 0;                 /* loop iterator -- segments      */
   int       trys_ltr  = 0;                 /* number possible letter rules   */
   char      flow      = 0;                 /* current quadrant flow          */
   char      rc        = 0;                 /* simple return code             */
   char      match[5]  = "";                /* letter that matched            */
   /*---(try letters)---------------------*/
   trys_ltr = 0;
   for (j = 0; j < MAX_LETTERS; ++j) {
      /*---(primary filtering)---------------*/
      if (strncmp (g_loc[j].label, "EOF", 5)              == 0) break;
      /*> if (strncmp (g_groups [a_rule].lcat, g_loc[j].lcat, 5) != 0) continue;           <*/
      /*---(screen letters)------------------*/
      ++trys_ltr;
      if (trys_ltr <= 3) {
         DEBUG_MATCH   printf("%1d:%-2.2s ", g_loc[j].size, g_loc[j].label);
      }
      if (g_loc[j].size != a_size                          ) continue;
      /*---(give results)-----------------*/
      rc = 1;
      strncpy (match, g_loc[j].label, 5);
   }
   for (j = trys_ltr; j < 3; ++j) {
      DEBUG_MATCH   printf(".:.. ");
   }
   if (rc == 1) {
      DEBUG_MATCH   printf("[ MATCH %-4.4s]", match);
      KEY_label(a_beg + 1, a_opens, match);
   } else {
      DEBUG_MATCH   printf("[ no fits   ]");
   }
   /*---(finish locations)----------------*/
   /*> if (trys_ltr == 0) printf("no LOCATION entry\n");                           <* 
    *> else               printf("\n");                                            <*/
   /*> printf("\n");                                                                  <*/
   /*---(complete)-----------------------*/
   return rc;
}

/*> static char     /+----: apply matching rules in turn ---------------------------+/                                               <* 
 *> match_group        (int a_beg, int a_opens, char *a_group)                                                                       <* 
 *> {                                                                                                                                <* 
 *>    /+---(locals)-------------------------+/                                                                                      <* 
 *>    int       i         = 0;                 /+ loop iterator -- key points    +/                                                 <* 
 *>    int       j         = 0;                 /+ curve index                    +/                                                 <* 
 *>    int       trys_grp  = 0;                 /+ number possible group rules    +/                                                 <* 
 *>    int       trys_ltr  = 0;                 /+ number possible letter rules   +/                                                 <* 
 *>    char      rc        = 0;                 /+ simple return code             +/                                                 <* 
 *>    char      num       = 0;                 /+ current quadrant flow          +/                                                 <* 
 *>    char      gname[5]  = "";                                                                                                     <* 
 *>    /+---(run rules)--------------------------+/                                                                                  <* 
 *>    for (i = 0; g_groups [i].range < 900; ++i) {                                                                                  <* 
 *>       /+---(only review active rules)--------+/                                                                                  <* 
 *>       if (g_groups [i].ac != 1       )   continue;    /+ active 1 = yes        +/                                                <* 
 *>       /+---(primary filtering)---------------+/                                                                                  <* 
 *>       if (g_groups [i].opens != a_opens )   continue;    /+ number of opens       +/                                             <* 
 *>       if (g_groups [i].range != opens.range &&               /+ wrong primary range   +/                                         <* 
 *>             g_groups [i].range != opens.range2) continue;    /+    and not secondary  +/                                         <* 
 *>       /+> if (g_groups [i].cu != opens.ccat)   continue;    /+ wrong curviness       +/    <+/                                   <* 
 *>       j = opens.ccat + CURVE_OFFSET;                                                                                             <* 
 *>       if (strchr("- _", g_groups [i].ca [j]) != 0)   continue;    /+ wrong curviness       +/                                    <* 
 *>       /+---(test specific group)-------------+/                                                                                  <* 
 *>       if (strncmp(a_group, "", 5) == 0) {                                                                                        <* 
 *>          strncpy(gname, "n/a", 5);                                                                                               <* 
 *>       } else {                                                                                                                   <* 
 *>          strncpy(gname, a_group, 5);                                                                                             <* 
 *>          if (strncmp(g_groups [i].lcat, a_group, 5) != 0) continue;                                                               <* 
 *>       }                                                                                                                          <* 
 *>       /+---(start report out)----------------+/                                                                                  <* 
 *>       /+> printf("      #%02d [%-3.3s] f=%2d o=%1d, r=%1d, c=%2d : ",                 <*                                         <* 
 *>        *>       i, g_groups [i].gr, a_beg, g_groups [i].opens, g_groups [i].range, opens.ccat);        <+/                       <* 
 *>       ++trys_grp;                                                                                                                <* 
 *>       /+---(secondary filtering)-------------+/                                                                                  <* 
 *>       /+> if (match_shape (i, a_beg) != 1)   continue;                                <+/                                        <* 
 *>       if (match_flow  (i, a_beg) != 1)   continue;                                                                               <* 
 *>       /+---(try letters)---------------------+/                                                                                  <* 
 *>       rc = match_location (i, a_beg, a_opens, opens.lcat);                                                                       <* 
 *>       if (rc != 0) break;                                                                                                        <* 
 *>    }                                                                                                                             <* 
 *>    /+---(finish rules)-----------------------+/                                                                                  <* 
 *>    if (strncmp(a_group, "", 5) == 0) {                                                                                           <* 
 *>       /+> if (trys_grp == 0) printf("      no group rules found (in GROUP table)\n");     <*                                     <* 
 *>        *> else               printf("      end of group rules (%d tries)\n", trys_grp);   <+/                                    <* 
 *>    } else {                                                                                                                      <* 
 *>       /+> if (trys_grp == 0) {                                                                                       <*          <* 
 *>        *>    printf("      #-- [%-3.3s] f=%2d o=%1d, r=%1d, c=%2d :", a_group, a_beg, a_opens, opens.range, opens.ccat);   <*    <* 
 *>        *>    printf(" --- --- --- : --- --- --- --- : ---- ---- ---- [ no joy    ]\n");                              <*          <* 
 *>        *> }                                                                                                          <+/         <* 
 *>    }                                                                                                                             <* 
 *>    /+---(complete)-----------------------+/                                                                                      <* 
 *>    return rc;                                                                                                                    <* 
 *> }                                                                                                                                <*/

char
MATCH_mark            (short a_match)
{
   short       i         =    0;
   char        x_cont    =  '-';
   DEBUG_MATCH   yLOG_value   ("a_match"   , a_match);
   for (i = opens.beg + 1; i <= opens.beg + opens.count; ++i) {
      DEBUG_MATCH   yLOG_value   ("i"         , i);
      o.key [i].marked = '!';
      if      (a_match    >=  0) {
         if (x_cont == '-')  strlcpy (o.key [i].use, g_loc [a_match].label, LEN_TERSE);
         else                strlcpy (o.key [i].use, "+", LEN_TERSE);
         x_cont = 'y';
      }
      else if (opens.size ==  0)  strlcpy (o.key [i].use, "·", LEN_TERSE);
      else if (a_match    == -1)  strlcpy (o.key [i].use, "¡", LEN_TERSE);
      else if (a_match    == -2)  strlcpy (o.key [i].use, "¢", LEN_TERSE);
      DEBUG_MATCH   yLOG_info    ("use"       , o.key [i].use);
      /*> DEBUG_MATCH   yLOG_complex ("mark"      "%2d, %2d, %s", i, a_match, o.key [i].use);   <*/
   }
   return 0;
}

char
MATCH_group        (void)
{
   /*---(locals)-------------------------*/
   char        rce       =  -10;
   short       i         =    0;
   char        rc        =    0;
   short       x_match   =   -1;
   char        x_cat     =    0;
   /*---(header)-------------------------*/
   DEBUG_MATCH   yLOG_enter   (__FUNCTION__);
   DEBUG_MATCH   yLOG_complex ("target"    , "%1dc, %1d/%1dr, %-7.7s, %1dc", opens.count, opens.range, opens.range2, opens.flow, opens.ccat);
   /*---(run rules)--------------------------*/
   for (i = 0; g_groups [i].range < 10; ++i) {
      DEBUG_MATCH   yLOG_complex ("group"     , "%3d#, %1do, %1d/-r, %-7.7s, %-8.8s", i, g_groups [i].opens, g_groups [i].range, g_groups [i].flow, g_groups [i].curve);
      /*---(only review active rules)--------*/
      if (g_groups [i].ac    != 1)                                continue;
      /*---(quick filtering)-----------------*/
      if (g_groups [i].opens != opens.count)                      continue;
      if (g_groups [i].range == opens.range)   ++rc;
      if (g_groups [i].range == opens.range2)  ++rc;
      if (rc < 1)                                                 continue;
      /*---(complex filtering)---------------*/
      if (strcmp (g_groups [i].flow , opens.flow) != 0)           continue;
      if (strchr (g_groups [i].curve, opens.ccat + '0') == NULL)  continue;
      DEBUG_MATCH   yLOG_note    ("FOUND");
      x_match = i;
      break;
   }
   DEBUG_MATCH   yLOG_value   ("x_match"   , x_match);
   --rce;  if (x_match < 0) {
      /*> MATCH_mark (-1);                                                            <*/
      DEBUG_MATCH   yLOG_exitr   (__FUNCTION__, rce);
      return -1;
   }
   /*---(get letter)-------------------------*/
   x_cat   = g_groups [x_match].lcat;
   DEBUG_MATCH   yLOG_complex ("sizing"    , "%2dc, %4.0fl, %1ds", x_cat, opens.len, opens.size);
   x_match = -1;
   for (i = 0; i < MAX_LETTERS; ++i) {
      if (strcmp (g_loc [i].label, "END") == 0)     break;
      if (strcmp (g_loc [i].label, "EOF") == 0)     break;
      DEBUG_MATCH   yLOG_complex ("letter"    , "%3d#, %1dc, %-5.5s, %1ds", i, g_loc [i].lcat, g_loc [i].label, g_loc [i].size);
      if (g_loc [i].lcat != x_cat)                  continue;
      if (g_loc [i].size != opens.size)             continue;
      DEBUG_MATCH   yLOG_note    ("FOUND");
      x_match = i;
      break;
   }
   --rce;  if (x_match < 0) {
      /*> MATCH_mark (-2);                                                            <*/
      DEBUG_MATCH   yLOG_exitr   (__FUNCTION__, rce);
      return -2;
   }
   /*---(mark letter)------------------------*/
   /*> MATCH_mark (x_match);                                                          <*/
   /*---(complete)-----------------------*/
   DEBUG_MATCH   yLOG_exit    (__FUNCTION__);
   return x_match;
}

char          /*----: walk through rules for all normal letters --------------*/
MATCH_driver_OLD   (void)
{
   /*---(locals)-------------------------*/
   int   beg       = 0;      /* current open point                            */
   int   count     = 0;      /* number of opens to test                       */
   char  rc        = 0;      /* simple return code                            */
   int   trys_grp  = 0;      /* number of possible group rules                */
   int   trys_ltr  = 0;      /* number of possible letter rules               */
   /*---(process)------------------------*/
   DEBUG_MATCH   printf("\nMATCHING LETTERS (beg)--------------------\n");
   MATCH__prep     ();
   /*> DEBUG_MATCH   POINT_list (stdout, o.key, o.nkey);                            <*/
   for (beg = 0; beg < o.nkey - 1; ++beg) {
      /*---(get open point count)------------------*/
      MATCH__available (beg);
      if (opens.sharps < 1)  continue;
      /*---(work from longest to shortest)---------*/
      for (count = 7; count > 0; --count) {
         rc = 0;
         if (count > opens.sharps)               continue;
         MATCH_calc (beg, count);
         /*> printf("\n   %02d-%02d%c =========  o=%1d, r=%1d, c=%2d : ",                           <* 
          *>       beg, beg + count, count > (opens.norm) ? '*' : ' ', count, opens.range, opens.ccat);   <*/
         /*> printf("le=%4d, sz=%1d, de=%4d, mc=%5.1f  ==========================\n",   <* 
          *>       opens.len, opens.size, opens.d, opens.cm);                               <*/
         if (count >= 3 && opens.sharps >= 3)  rc = combo_driver (beg, count);
         if (rc == 1) break;
         trys_grp = 0;
         /*---(run rules)--------------------------*/
         /*> rc = match_group (beg, count, "");                                       <*/
         if (rc != 0) break;
      }
   }
   DEBUG_MATCH   printf("\n   done with letter recognition\n");
   /*---(process results)----------------*/
   match_cleanup   ();
   match_result    ();
   words_outstring (o.actual);
   /*---(complete)-----------------------*/
   DEBUG_MATCH   printf("\n");
   /*> DEBUG_MATCH   POINT_list (stdout, o.key, o.nkey);                            <*/
   DEBUG_MATCH   printf("\nMATCHING LETTERS (end)--------------------\n");
   /*> printf("r=%3d, a=%3d, k=%3d, ltr=%s, act=%s, wrd=%s\n",                     <* 
    *>       o.nraw, o.navg, o.nkey, o.actual, o.letters, o.word);                 <*/
   return 0;
}

short 
MATCH_count_open        (short a_beg)
{
   /*---(locals)-----------+-----+-----+-*/
   char        rce         =  -10;
   short       i           =    0;
   short       c           =    0;
   /*---(header)-------------------------*/
   DEBUG_MATCH   yLOG_enter   (__FUNCTION__);
   /*---(defense)------------------------*/
   DEBUG_MATCH   yLOG_value   ("a_beg"     , a_beg);
   --rce;  if (a_beg < 0) {
      DEBUG_MATCH   yLOG_exitr   (__FUNCTION__, rce);
      return rce;
   }
   DEBUG_MATCH   yLOG_value   ("o.nkey"    , o.nkey);
   --rce;  if (a_beg >= o.nkey) {
      DEBUG_MATCH   yLOG_exitr   (__FUNCTION__, rce);
      return rce;
   }
   /*---(wave off)-----------------------*/
   if (o.key [a_beg].type   == POINT_TAIL) {
      DEBUG_MATCH   yLOG_exit    (__FUNCTION__);
      return 0;
   }
   /*---(count opens)--------------------*/
   DEBUG_MATCH   yLOG_value   ("o.nkey"    , o.nkey);
   for (i = a_beg + 1; i < o.nkey; ++i) {
      DEBUG_MATCH   yLOG_complex ("review"    , "%2dk %3db %4dr, %c %c %c,", i, o.key [i].p_bas, o.key [i].p_raw, o.key [i].type, o.key [i].marked, o.key [i].sharp);
      if (o.key [i].marked != '-')          break;
      ++c;
      if (o.key [i].sharp  != '-')          break;
      if (o.key [i].type   == POINT_TAIL)   break;
   }
   /*---(complete)-----------------------*/
   DEBUG_MATCH   yLOG_exit    (__FUNCTION__);
   return c;
}

char
MATCH_result            (char *a_result)
{
   /*---(locals)-----------+-----+-----+-*/
   char        rce         =  -10;
   short       c           =    0;
   short       i           =    0;
   char        t           [LEN_LABEL];
   char        s           [LEN_TERSE];
   /*---(header)-------------------------*/
   DEBUG_MATCH   yLOG_enter   (__FUNCTION__);
   /*---(defense)------------------------*/
   DEBUG_MATCH   yLOG_point   ("a_result"  , a_result);
   --rce;  if (a_result == NULL) {
      DEBUG_MATCH   yLOG_exitr   (__FUNCTION__, rce);
      return rce;
   }
   strlcpy (a_result, "", LEN_HUND);
   /*---(walk)---------------------------*/
   for (i = 1; i < o.nkey; ++i) {
      if        (o.key [i].type == '>') {
         sprintf (t, "%s", "·>");
      } else if (o.key [i].use [0] == '+') {
         strlcpy (t, "", LEN_TERSE);
      } else if (o.key [i].use [0] == '·') {
         strlcpy (t, "", LEN_TERSE);
      } else  {
         strlcpy (s, o.key [i].use, LEN_TERSE);
         if (strcmp (s, "z") == 0)  strlcpy (s, "s", LEN_TERSE);
         if (c > 0)  sprintf (t, "·%s", s);
         else        sprintf (t, "%s" , s);
         ++c;
      }
      strlcat (a_result, t, LEN_HUND);
   }
   DEBUG_MATCH   yLOG_info    ("a_result"  , a_result);
   /*---(complete)-----------------------*/
   DEBUG_MATCH   yLOG_exit    (__FUNCTION__);
   return 0;
}

char
MATCH_driver            (void)
{
   /*---(locals)-----------+-----+-----+-*/
   char        rce         =  -10;
   char        rc          =    0;
   short       x_max       =    7;
   /*---(header)-------------------------*/
   DEBUG_MATCH   yLOG_enter   (__FUNCTION__);
   DEBUG_MATCH   yLOG_value   ("o.nkey"    , o.nkey);
   for (opens.beg = 0; opens.beg < o.nkey - 1; ++opens.beg) {
      /*---(report)----------------------*/
      DEBUG_MATCH   yLOG_complex ("review"    , "%2dk %3db %4dr, %c %c,", opens.beg, o.key [opens.beg].p_bas, o.key [opens.beg].p_raw, o.key [opens.beg].marked, o.key [opens.beg].sharp);
      /*---(opens)-----------------------*/
      opens.opens = MATCH_count_open (opens.beg);
      DEBUG_MATCH   yLOG_value   ("opens"     , opens.opens);
      if (opens.opens <= 0)  continue;
      /*---(cycle)-----------------------*/
      if (opens.opens > x_max)  opens.opens = x_max;
      for (opens.count = opens.opens; opens.count >= 1; --opens.count) {
         /*---(prepare)--------*/
         MATCH_calc  (opens.beg, opens.count);
         MATCH_flow  (opens.beg, opens.count);
         /*---(test match)-----*/
         rc = MATCH_group ();
         MATCH_mark (rc);
         if (rc >= 0)  break;
         /*---(done)-----------*/
      }
      /*---(done)------------------------*/
   }
   MATCH_result (o.gregg);
   /*---(complete)-----------------------*/
   DEBUG_MATCH   yLOG_exit    (__FUNCTION__);
   return 0;
}



/*============================--------------------============================*/
/*===----                         combinations                         ----===*/
/*============================--------------------============================*/
static void      o___COMBOS__________________o (void) {;}

static int      /*----: add a mid point ----------------------------------------*/
combo_check        (int a_beg, int a_count, char a_method, int a_offset, int a_new, char *a_1st, char *a_2nd)
{
   /*---(locals)-------------------------*/
   char      rc1       = 0;            /* return from letter one              */
   char      rc2       = 0;            /* return from letter two              */
   int       new       = 0;            /* artificial key transition           */
   int       xopens    = 0;
   /*---(match 1st segment)--------------*/
   if (a_method == '-')  xopens = a_offset;
   else                  xopens = 2;
   MATCH_calc (a_beg, xopens);
   /*> rc1 = match_group (a_beg, xopens, a_1st);                                      <*/
   /*---(match 2nd segment)--------------*/
   MATCH_calc (a_beg + a_offset, 2);
   /*> printf("\ngshape = <<%s>>, oshape = <<%s>>\n", opens.gshape, opens.oshape);    <*/
   /*> rc2 = match_group (a_beg + a_offset, 2, a_2nd);                                <*/
   /*---(check results)------------------*/
   if (rc1 == 0 || rc2 == 0) {
      if (a_new > 0) KEY_del   (a_new);
      KEY_label (a_beg + 1, a_count, "-");
      return -1;
   }
   /*---(complete)-----------------------*/
   return 1;
}

static int      /*----: add a mid point ----------------------------------------*/
combo_flaty        (int a_beg, int a_count, int a_offset)
{
   /*---(locals)-------------------------*/
   int       p1, p2, p3, p4;
   int       y1, y2;
   float     yt;
   int       i;
   float     dist;
   float     min    = 1000;
   int       mid;
   int       new;
   /*---(initialize points)--------------*/
   p1        = o.key[a_beg    ].p_bas;
   p2        = o.key[a_beg + 1].p_bas;
   p3        = o.key[a_beg + 2].p_bas;
   p4        = o.key[a_beg + 3].p_bas;
   /*---(get target y)-------------------*/
   if (a_offset == 0) {
      y1        = o.avg[p1].y_raw;
      y2        = o.avg[p4].y_raw;
   } else {
      y1        = o.avg[p2].y_raw;
      y2        = o.avg[p3].y_raw;
   }
   yt        = y1;
   DEBUG_MATCH   printf("y1=%4d, y2=%4d, yt=%5.1f, ", y1, y2, yt);
   /*---(find midpoint)------------------*/
   for (i = p2 + 1; i < p3; ++i) {
      dist = fabs(o.avg[i].y_raw - yt);
      /*> DEBUG_MATCH   printf("%2d at %3d is %6.2f\n", i, o.avg[i].y_raw, dist);       <*/
      if (dist <= min) {
         min = dist;
         mid = i;
      }
   }
   if (min > 2.0) {
      DEBUG_MATCH   printf("d=%5.1f, reject\n", min);
      return -1;
   }
   /*---(add midpoint)-------------------*/
   new = KEY_add (mid, 'y', POINT_NORMAL, NULL);
   DEBUG_MATCH   printf("m=%3d, d=%5.1f, k=%2d\n", mid, min, new);
   /*---(complete)-----------------------*/
   return new;
}

static int      /*----: add a mid point ----------------------------------------*/
combo_midy         (int a_beg, int a_count, int a_offset)
{
   /*---(locals)-------------------------*/
   int       p1, p2, p3, p4;
   int       y1, y2;
   float     yt;
   int       i;
   float     dist;
   float     min    = 1000;
   int       mid;
   int       new;
   /*---(initialize points)--------------*/
   p1        = o.key[a_beg    ].p_bas;
   p2        = o.key[a_beg + 1].p_bas;
   p3        = o.key[a_beg + 2].p_bas;
   p4        = o.key[a_beg + 3].p_bas;
   /*---(get target y)-------------------*/
   if (a_offset == 0) {
      y1        = o.avg[p1].y_raw;
      y2        = o.avg[p4].y_raw;
   } else {
      y1        = o.avg[p2].y_raw;
      y2        = o.avg[p3].y_raw;
   }
   yt        = y1 + ((y2 - y1) / 2.0);
   DEBUG_MATCH   printf("y1=%4d, y2=%4d, yt=%5.1f, ", y1, y2, yt);
   /*---(find midpoint)------------------*/
   for (i = p2 + 1; i < p3; ++i) {
      dist = fabs(o.avg[i].y_raw - yt);
      /*> DEBUG_MATCH   printf("%2d at %3d is %6.2f\n", i, o.avg[i].y_raw, dist);       <*/
      if (dist <= min) {
         min = dist;
         mid = i;
      }
   }
   /*---(add midpoint)-------------------*/
   new = KEY_add (mid, 'y', POINT_NORMAL, NULL);
   DEBUG_MATCH   printf("m=%3d, d=%5.1f, k=%2d\n", mid, min, new);
   /*---(complete)-----------------------*/
   return new;
}

static int      /*----: add a mid point ----------------------------------------*/
combo_flatx        (int a_beg, int a_count, int a_offset)
{
   /*---(locals)-------------------------*/
   int       p1, p2, p3, p4;
   int       x1, x2;
   float     xt;
   int       i;
   float     dist;
   float     min    = 1000;
   int       mid;
   int       new;
   /*---(initialize points)--------------*/
   p1        = o.key[a_beg    ].p_bas;
   p2        = o.key[a_beg + 1].p_bas;
   p3        = o.key[a_beg + 2].p_bas;
   p4        = o.key[a_beg + 3].p_bas;
   /*---(get target y)-------------------*/
   if (a_offset == 0) {
      x1        = o.avg[p1].x_raw;
      x2        = o.avg[p4].x_raw;
   } else {
      x1        = o.avg[p2].x_raw;
      x2        = o.avg[p3].x_raw;
   }
   xt        = x1;
   DEBUG_MATCH   printf("x1=%4d, x2=%4d, xt=%5.1f, ", x1, x2, xt);
   /*---(find midpoint)------------------*/
   for (i = p2 + 1; i < p3; ++i) {
      dist = fabs(o.avg[i].x_raw - xt);
      /*> DEBUG_MATCH   printf("%2d at %3d is %6.2f\n", i, o.avg[i].x_raw, dist);       <*/
      if (dist <= min) {
         min = dist;
         mid = i;
      }
   }
   /*---(add midpoint)-------------------*/
   new = KEY_add (mid, 'y', POINT_NORMAL, NULL);
   DEBUG_MATCH   printf("m=%3d, d=%5.1f, k=%2d\n", mid, min, new);
   /*---(complete)-----------------------*/
   return new;
}

static int      /*----: add a mid point ----------------------------------------*/
combo_midx         (int a_beg, int a_count, int a_offset)
{
   /*---(locals)-------------------------*/
   int       p1, p2, p3, p4;
   int       x1, x2;
   float     xt;
   int       i;
   float     dist;
   float     min    = 1000;
   int       mid;
   int       new;
   /*---(initialize points)--------------*/
   p1        = o.key[a_beg    ].p_bas;
   p2        = o.key[a_beg + 1].p_bas;
   p3        = o.key[a_beg + 2].p_bas;
   p4        = o.key[a_beg + 3].p_bas;
   /*---(get target y)-------------------*/
   if (a_offset == 0) {
      x1        = o.avg[p1].x_raw;
      x2        = o.avg[p4].x_raw;
   } else {
      x1        = o.avg[p2].x_raw;
      x2        = o.avg[p3].x_raw;
   }
   xt        = x1 + ((x2 - x1) / 2.0);
   DEBUG_MATCH   printf("x1=%4d, x2=%4d, xt=%5.1f, ", x1, x2, xt);
   /*---(find midpoint)------------------*/
   for (i = p2 + 1; i < p3; ++i) {
      dist = fabs(o.avg[i].x_raw - xt);
      /*> DEBUG_MATCH   printf("%2d at %3d is %6.2f\n", i, o.avg[i].x_raw, dist);       <*/
      if (dist <= min) {
         min = dist;
         mid = i;
      }
   }
   /*---(add midpoint)-------------------*/
   new = KEY_add (mid, 'y', POINT_NORMAL, NULL);
   DEBUG_MATCH   printf("m=%3d, d=%5.1f, k=%2d\n", mid, min, new);
   /*---(complete)-----------------------*/
   return new;
}

static char     /*----: attempt to match a GL,GR,GO,CL,CR,CO,UL,UR,UO ----------*/
combo_flow         (int a_beg, int a_count, char *a_flow)
{
   /*---(locals)-------------------------*/
   int       len       = 0;            /* length of flow string               */
   int       i         = 0;            /* loop iterator -- key point          */
   int       rc        = 0;
   int       quad      = 0;
   /*---(check count)--------------------*/
   len = strlen (a_flow);
   if        (len < a_count) {
      DEBUG_MATCH   printf("flow too short (%d != %d)\n", a_count, len);
      return -1;
   } else {
      DEBUG_MATCH   printf("n=%d, ", a_count);
   }
   /*---(check flow)---------------------*/
   for (i = 1; i <= a_count; ++i) {
      quad = a_flow[i - 1] - '0';
      if (o.key[a_beg + i].quad == quad) {
         DEBUG_MATCH   printf("q%1d:%1d=%1d ", i, o.key[a_beg + i].quad, quad);
      } else {
         DEBUG_MATCH   printf("q%1d:%1dx%1d ", i, o.key[a_beg + i].quad, quad);
         rc = -2;
      }
   }
   if (rc == -2) {
      DEBUG_MATCH   printf(", flow is wrong\n");
   }
   /*---(complete)-----------------------*/
   return rc;
}

static char     /*----: attempt to parse points and match combinations ---------*/
combo_driver       (int a_beg, int a_count)
{
   /*---(locals)-------------------------*/
   char      rc        = 0;            /* return code                         */
   char      flows[10] = "";           /* combination flow                    */
   char      quad      = 0;            /* first quad                          */
   int       i         = 0;
   int       new       = 0;            /* artificial key transition           */
   /*---(begin)--------------------------*/
   if (a_count < 3) {
      DEBUG_MATCH   printf("      no combos, too few points availible (%d < 3)\n", opens.norm);
      return -1;
   }
   /*---(GL)-----------------------------*/
   quad = o.key[a_beg + 1].quad + '0';
   /*> DEBUG_MATCH   printf("      start combos with %2d and q=%d...\n", opens.norm, quad);   <*/
   for (i = 0; i < MAX_COMBOS; ++i) {
      /*> DEBUG_MATCH   printf("      looking at %s, ac=%1d, op=%1d, q1=%c\n", combos[i].name, combos[i].ac, combos[i].opens, combos[i].flow [0]);   <*/
      /*---(screen)----------------------*/
      if (strcmp(g_combos [i].name, "EOF") == 0)     break;
      if (g_combos [i].ac    == 0)                 continue;
      if (g_combos [i].opens    != a_count)           continue;
      if (g_combos [i].flow [0] != quad)              continue;
      /*---(check flow)------------------*/
      DEBUG_MATCH   printf("      combo %3s) ", g_combos [i].name);
      rc = combo_flow (a_beg, a_count, g_combos [i].flow );
      if (rc != 0) continue;
      /*---(assign midpoint)-------------*/
      switch (g_combos [i].mm[0]) {
      case '-' :      /* "--"  */
         new  = 0;
         DEBUG_MATCH   printf(", midpoint exists\n");
         break;
      case 'o' :      /* "oo"  */
         new  = 0;
         DEBUG_MATCH   printf(", letters overlap, midpoint unneccessary\n");
         break;
      case 'f' :
         switch (g_combos [i].mm[1]) {
         case 'y' :   /* "fy"  */
            new  = combo_flaty (a_beg, a_count, g_combos [i].st);
            break;
         case 'x' :   /* "fx"  */
            new  = combo_flatx (a_beg, a_count, g_combos [i].st);
            break;
         default  :
            continue;
            break;
         }
         break;
      case 'a' :
         switch (g_combos [i].mm[1]) {
         case 'y' :   /* "ax"  */
            new  = combo_midy (a_beg, a_count, g_combos [i].st);
            break;
         case 'x' :   /* "ax"  */
            new  = combo_midx (a_beg, a_count, g_combos [i].st);
            break;
         default  :
            continue;
            break;
         }
         break;
      default  :
         continue;
         break;
      }
      /*> printf ("k=%d\n", new);                                                     <*/
      if (new < 0) continue;
      /*---(evaluate)--------------------*/
      rc   = combo_check (a_beg, a_count, g_combos [i].mm[0], g_combos [i].of, new, g_combos [i].on, g_combos [i].tw);
      /*---(complete)-----------------------*/
      if (rc == 1) return rc;
      MATCH_calc (a_beg, a_count);
   }
   /*---(complete)-----------------------*/
   return 0;
}

char
COMBO_driver               (short a_beg, short a_count)
{
}



/*============================--------------------============================*/
/*===----                         unit testing                         ----===*/
/*============================--------------------============================*/
static void o___UNIT_TEST_________________o (void) {;}

char*        /*-> unit test accessor -----------------[ light  [us.D90.241.L0]*/ /*-[03.0000.00#.#]-*/ /*-[--.---.---.--]-*/
MATCH__unit          (char *a_question, int a_num)
{
   /*---(locals)-----------+-----+-----+-*/
   int         i           = 0;
   char        t           [100] = "";
   char        s           [100] = "";
   int         x_end       = 0;
   int         y_end       = 0;
   tPOINT     *p           = NULL;
   int         c           =    0;
   char        x_pre       [LEN_TERSE] = "";
   /*---(initialize)---------------------*/
   strlcpy (unit_answer, "POINT unit       : unknownn request", 100);
   /*---(core data)----------------------*/
   if        (strncmp (a_question, "pos"       , 20)  == 0) {
      snprintf (unit_answer, LEN_STR, "OPENS pos        : %4.0fxd %4.0fyd %3.0fl %8.3fs %6.0fi %4.0fd %dq  --  %d len   %d range (%d)",
            opens.xd   , opens.yd   , opens.len,
            opens.slope, opens.icept,
            opens.degs , opens.quad,
            opens.size , opens.range, opens.range2);
   }
   else if   (strncmp (a_question, "curve"     , 20)  == 0) {
      snprintf (unit_answer, LEN_STR, "OPENS curve      : %6.1fl %6.1fd %6.1fr, %1dc",
            opens.len, opens.depth , opens.ratio, opens.ccat);
   }
   /*---(complete)-----------------------*/
   return unit_answer;
}



/*============================----(source-end)----============================*/
