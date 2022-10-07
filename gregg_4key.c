/*============================---(source-start)---============================*/
#include "gregg.h"




/*> char fname[] = "/home/dotsuu/p_gvskav/gregg.gregg_shorthand_system/strokes.dat";   <*/
char fname[] = "/home/dotsuu/p_gvskav/gregg.gregg_shorthand_system/strokes_new.dat";
char oname[] = "/home/dotsuu/p_gvskav/gregg.gregg_shorthand_system/strokes_new.dat";



char
KEY_init             (void)
{
   o.nkey     = 0;
   POINT_clear_series (POINTS_KEY);
   return 0;
}


/*============================--------------------============================*/
/*===----                            key points                        ----===*/
/*============================--------------------============================*/
void o___KEY_POINTS_____________o (void) {;}

PRIV char     /*----: swap two points during the sort ------------------------*/
KEY__swap          (int a_i, int a_j)
{
   /*---(locals)-------------------------*/
   short       x_tmp       = MAX_POINTS - 1;
   short       x_raw, x_bas;
   short       
   int or, p, xf, yf, x, y, a, t;
   char  u[5];
   /*---(save 1st)-----------------------*/
   /*---(indexes)------*//*--- seq and key do not move ----------------*/
   or = o.key [a_i].p_raw;
   p  = o.key [a_i].p_bas;
   /*---(raw values)---*/
   xf = o.key [a_i].x_raw;
   yf = o.key [a_i].y_raw;
   x  = o.key [a_i].x_pos;
   y  = o.key [a_i].y_pos;
   a  = o.key [a_i].fake;
   t  = o.key [a_i].type;
   strncpy (u, o.key [a_i].use, 5);
   /*---(copy 2nd to 1st)----------------*/
   o.key [a_i].p_raw  = o.key [a_j].p_raw;
   o.key [a_i].p_bas  = o.key [a_j].p_bas;
   o.key [a_i].x_raw  = o.key [a_j].x_raw;
   o.key [a_i].y_raw  = o.key [a_j].y_raw;
   o.key [a_i].x_pos  = o.key [a_j].x_pos;
   o.key [a_i].y_pos  = o.key [a_j].y_pos;
   o.key [a_i].fake   = o.key [a_j].fake;
   o.key [a_i].type   = o.key [a_j].type;
   strncpy (o.key [a_i].use, o.key [a_j].use, 5);
   /*---(put saved into 2nd)-------------*/
   o.key [a_j].p_raw  = or;
   o.key [a_j].p_bas  = p;
   o.key [a_j].x_raw  = xf;
   o.key [a_j].y_raw  = yf;
   o.key [a_j].x_pos  = x;
   o.key [a_j].y_pos  = y;
   o.key [a_j].fake   = a;
   o.key [a_j].type   = t;
   strncpy (o.key [a_j].use, u, 5);
   /*---(complete)-----------------------*/
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
KEY_curve          (int a_pt)
{
   int i;
   int deg   = o.key[a_pt].degs;
   int range = -1;
   range = MATCH_range(deg);
   o.key[a_pt].range = range;
   /*---(calc the arch)-------------------------*/
   int   p1       = o.key[a_pt - 1].p_bas;
   int   p4       = o.key[a_pt].p_bas;
   float x_slope  = o.key[a_pt].slope;
   int   x_icept  = o.key[a_pt].icept;
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
    *> printf("so  slope = %8.3f and intercept = %4d\n", x_slope, x_icept);                          <*/
   for (i = p1 + 1; i < p4; ++i) {
      liney   = (x_slope * o.bas[i].x_raw) + x_icept;
      linex   = (o.bas[i].y_raw - x_icept) / x_slope;
      diffx   = (o.bas[i].x_raw - linex);
      diffy   = (o.bas[i].y_raw - liney);
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
   /*> DEBUG_CURV  printf("key_curve for key points %d/%d (bas %02d/%02d) maxcurve=%5.1f\n", a_pt, a_pt + 1, p1, p4, maxcurve);   <*/
   o.key[a_pt].depth = maxcurve / my.ratio;
   o.key[a_pt].ccat = -9;
   if      (maxcurve >=  (o.key[a_pt].len * 0.15) || maxcurve >=  2.0)   o.key[a_pt].ccat = +1;
   else if (maxcurve <= -(o.key[a_pt].len * 0.15) || maxcurve <= -2.0)   o.key[a_pt].ccat = -1;
   else                                                                  o.key[a_pt].ccat =  0;
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
      o.key [i].xd    = o.key [i].yd    = o.key [i].len   = 0;
      o.key [i].slope = o.key [i].icept = o.key [i].rads  = o.key [i].degs  = 0;
      o.key [i].quad  = o.key [i].range = o.key [i].depth = o.key [i].ratio = o.key [i].ccat = 0;
      /*---(display)---------------------*/
      /*> POINT_display (o.key + i);                                                  <*/
      /*---(filter)----------------------*/
      if (a_mode == 'n' && o.key[i].type == POINT_HEAD) continue;
      if (a_mode == 'c' && o.key[i].type == POINT_START) continue;
      /*---(calculate)-------------------*/
      POINT_calc  (POINTS_KEY, o.key + i, 'n');
      POINT_curve (i - 1, i, o.key [i].len, o.key [i].slope, o.key [i].icept, &(o.key [i].depth), &(o.key [i].ratio), &(o.key [i].ccat));
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



/*============================--------------------============================*/
/*===----                      adding raw points                       ----===*/
/*============================--------------------============================*/
static void o___NEW____________________o (void) {;}

char
KEY__force_point    (uchar a_type, short x, short y, uchar a_mark, uchar a_sharp)
{
   /*---(tie raw and bas)-----------------*/
   o.key [o.nkey].p_raw   = -1;
   o.key [o.nkey].p_bas   = -1;
   /*---(head)----------------------------*/
   o.key [o.nkey].seq     = o.nkey;
   o.key [o.nkey].fake    = '-';
   o.key [o.nkey].type    = a_type;
   o.key [o.nkey].marked  = a_mark;
   /*---(touchpad)------------------------*/
   o.key [o.nkey].x_touch = x;
   o.key [o.nkey].x_raw   = x;
   o.key [o.nkey].y_touch = y;
   o.key [o.nkey].y_raw   = y;
   /*---(series)--------------------------*/
   o.key [o.nkey].series  = POINTS_KEY;
   /*---(position)-----------------------*/
   POINT_pos (o.key + o.nkey);
   o.key [o.nkey].sharp   = a_sharp;
   /*---(counters)-----------------------*/
   ++o.nkey;
   /*---(complete)-----------------------*/
   return 0;
}

char          /*----: add a key point from a avg point -----------------------*/
KEY_add            (short a_bas, uchar a_fake, uchar a_type, uchar *a_use)
{
   /*> DEBUG_CRIT  printf("   key request for %4d as a=%c, t=%c : ", a_bas, a_fake, a_type);   <*/
   /*---(locals)-------------------------*/
   int pt  = -1;
   short       x_raw       =    0;
   /*---(check for existing)-------------*/
   pt  = KEY_find (a_bas);
   if (pt >= 0) {
      /*> DEBUG_CRIT  printf("already exists as %d\n", pt);                           <*/
      return pt;
   }
   /*---(link raw/bas)-------------------*/
   x_raw = o.bas [a_bas].p_raw;
   o.raw [x_raw].p_key   = o.nkey;
   o.bas [a_bas].p_key   = o.nkey;
   o.avg [a_bas].p_key   = o.nkey;
   /*---(link key)-----------------------*/
   o.key [o.nkey].p_raw  = o.avg [a_bas].p_raw;
   o.key [o.nkey].p_bas  = o.avg [a_bas].p_bas;
   o.key [o.nkey].p_key  = o.nkey;
   /*---(add new key)--------------------*/
   o.key [o.nkey].x_raw  = o.avg [a_bas].x_raw;
   o.key [o.nkey].y_raw  = o.avg [a_bas].y_raw;
   o.key [o.nkey].x_pos  = o.avg [a_bas].x_pos;
   o.key [o.nkey].y_pos  = o.avg [a_bas].y_pos;
   if (a_use == NULL)   strncpy (o.key [o.nkey].use, "-"  , LEN_TERSE);
   else                 strncpy (o.key [o.nkey].use, a_use, LEN_TERSE);
   /*---(add params)---------------------*/
   o.key [o.nkey].fake   = a_fake;
   o.key [o.nkey].type   = a_type;
   /*---(update the keys)----------------*/
   ++o.nkey;
   /*> KEY__sort ();                                                                  <*/
   KEY_calc  ('n');
   /*---(locate after sort)--------------*/
   /*> pt = KEY_find (a_bas);                                                         <*/
   /*> DEBUG_CRIT  printf("added as %d\n", pt);                                       <*/
   /*---(complete)-----------------------*/
   return pt;
}

char          /*----: remove a key point -------------------------------------*/
KEY_del            (int a_pt)
{
   /*> DEBUG_CRIT  printf("   key delete  for %4d\n", a_pt);                          <*/
   int i;
   for (i = a_pt + 1 ; i < o.nkey; ++i) {
      o.key[i - 1].p_raw  = o.key[i].p_raw;
      o.key[i - 1].p_bas  = o.key[i].p_bas;
      o.key[i - 1].x_raw = o.key[i].x_raw;
      o.key[i - 1].y_raw = o.key[i].y_raw;
      o.key[i - 1].x_pos  = o.key[i].x_pos;
      o.key[i - 1].y_pos  = o.key[i].y_pos;
      o.key[i - 1].fake   = o.key[i].fake;
      o.key[i - 1].type   = o.key[i].type;
      strncpy (o.key [i - 1].use, o.key [i].use, 5);
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
   /*> DEBUG_CRIT  printf("   preparing for matching\n");                             <*/
   for (i = 0 ; i < o.nkey; ++i) {
      if (o.key[i].type == POINT_HEAD) {
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
   /*> DEBUG_CRIT  printf("   cleaning for similar points\n");                        <*/
   for (i = 1 ; i < o.nkey; ++i) {
      if (o.key[i].type == POINT_HEAD)            continue;
      if (o.key[i].quad == o.key[i - 1].quad) {
         /*> DEBUG_CRIT  printf("   cleaned out %d\n", i);                            <*/
         KEY_del(i - 1);
         --i;
         ++nclean;
      }
   }
   /*> if (nclean == 0) DEBUG_CRIT  printf("   none cleaned\n");                      <*/
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
      if (o.key[i    ].type == POINT_HEAD) {
         pt  = o.key[i    ].p_bas - 1;
         rad = o.key[i + 1].rads;
         o.bas[pt].x_raw  = o.avg[pt].x_raw  = o.key[i].x_raw - (15 * my.ratio * cos (rad)) ;
         o.bas[pt].y_raw  = o.avg[pt].y_raw  = o.key[i].y_raw - (15 * my.ratio * sin (rad)) ;
         /*> DEBUG_CRIT  printf("   extended beg %4d to %4dx, %4dy for circle detection\n", pt, o.bas[pt].x_raw, o.bas[pt].y_raw);   <*/
      }
      /*---(ending)-------------------------*/
      if (o.key[i + 1].type == POINT_HEAD || i + 1 == o.nkey) {
         pt  = o.key[i    ].p_bas + 1;
         rad = o.key[i    ].rads;
         o.bas[pt].x_raw  = o.avg[pt].x_raw  = o.key[i].x_raw + (15 * my.ratio * cos (rad)) ;
         o.bas[pt].y_raw  = o.avg[pt].y_raw  = o.key[i].y_raw + (15 * my.ratio * sin (rad)) ;
         /*> DEBUG_RAW   printf("   extended end %4d to %4dx, %4dy for circle detection\n", pt, o.bas[pt].x_raw, o.bas[pt].y_raw);   <*/
      }
   }
   /*---(complete)-----------------------*/
   return 0;
}

/*> char          /+----: identify key points from bas/avg ones ------------------+/             <* 
 *> KEY_filter         (void)                                                                    <* 
 *> {                                                                                            <* 
 *>    /+---(locals)-----------+-----+-----+-+/                                                  <* 
 *>    int         i           =    0;                                                           <* 
 *>    char        x_type      =  '-';                                                           <* 
 *>    char        q1          =  '-';                                                           <* 
 *>    char        q2          =  '-';                                                           <* 
 *>    /+---(header)-------------------------+/                                                  <* 
 *>    DEBUG_AVG    yLOG_enter   (__FUNCTION__);                                                 <* 
 *>    /+---(identify criticals)-------------+/                                                  <* 
 *>    o.nkey = 0;                                                                               <* 
 *>    /+> key_beg ();                                                                    <+/    <* 
 *>    /+> key_end ();                                                                    <+/    <* 
 *>    /+---(process)------------------------+/                                                  <* 
 *>    for (i = 0; i < o.navg; ++i) {                                                            <* 
 *>       /+---(filter)----------------------+/                                                  <* 
 *>       if (o.avg [i].type == POINT_START )  continue;                                         <* 
 *>       if (o.avg [i].type == POINT_FINISH)  continue;                                         <* 
 *>       /+---(prepare)---------------------+/                                                  <* 
 *>       x_type  = o.avg [i    ].type;                                                          <* 
 *>       q1      = o.avg [i    ].quad;                                                          <* 
 *>       q2      = o.avg [i + 1].quad;                                                          <* 
 *>       /+---(always key)------------------+/                                                  <* 
 *>       if (x_type == POINT_HEAD) {                                                            <* 
 *>          KEY_add (i, '-', POINT_HEAD, NULL);                                                 <* 
 *>          continue;                                                                           <* 
 *>       }                                                                                      <* 
 *>       if (x_type == POINT_TAIL) {                                                            <* 
 *>          KEY_add (i, '-', POINT_TAIL, NULL);                                                 <* 
 *>          continue;                                                                           <* 
 *>       }                                                                                      <* 
 *>       /+---(premarked)-------------------+/                                                  <* 
 *>       if (o.avg [i].prekey == 'k') {                                                         <* 
 *>          KEY_add (i, '-', POINT_NORMAL, NULL);                                               <* 
 *>          continue;                                                                           <* 
 *>       }                                                                                      <* 
 *>       if (o.avg [i].prekey != '-') {                                                         <* 
 *>          continue;                                                                           <* 
 *>       }                                                                                      <* 
 *>       /+---(mark)------------------------+/                                                  <* 
 *>       if (o.avg [i].quad != o.avg [i + 1].quad) {                                            <* 
 *>          KEY_add (i, '-', POINT_NORMAL, NULL);                                               <* 
 *>       }                                                                                      <* 
 *>    }                                                                                         <* 
 *>    /+> KEY__clean   ();                                                               <+/    <* 
 *>    /+> KEY__extend  ();                                                               <+/    <* 
 *>    /+> CLEAN_sharpen ();                                                               <+/   <* 
 *>    /+---(display version)----------------+/                                                  <* 
 *>    /+> for (i = 0; i < o.nkey; ++i) {                                                 <*     <* 
 *>     *>    POINT_display (o.key + i);                                                  <*     <* 
 *>     *> }                                                                              <+/    <* 
 *>    /+---(complete)-----------------------+/                                                  <* 
 *>    DEBUG_AVG    yLOG_exit    (__FUNCTION__);                                                 <* 
 *>    return 0;                                                                                 <* 
 *> }                                                                                            <*/

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

char
KEY_flatten          (void)
{
   /*---(locals)-------------------------*/
   int       i         = 0;
   int       dd        = 0;
   /*---(report out)---------------------*/
   /*> DEBUG_LINE  printf("STRAIGHTENING LINES (begin)\n");                           <*/
   /*> DEBUG_LINE  POINT_list (stdout, o.key, o.nkey);                              <*/
   /*---(process)------------------------*/
   /*> DEBUG_LINE  printf("   start...\n");                                           <*/
   while (i < o.nkey - 1) {
      /*> DEBUG_LINE  printf("   %3d/%3d) q=%d and q=%d ", i, i + 1, o.key[i].quad, o.key[i + 1].quad);   <*/
      /*---(skip start and finishes)-----*/
      if (o.key[i    ].type == POINT_HEAD || o.key[i + 1].type == POINT_HEAD) {
         /*> DEBUG_LINE  printf(": touches a line start, skipping\n");                <*/
         ++i;
         continue;
      }
      /*---(initial calcs)---------------*/
      dd = abs(o.key[i].degs - o.key[i + 1].degs);
      if (dd > 180) dd = 360 - dd;
      /*> DEBUG_LINE  printf("dd=%4d : ", dd);                                        <*/
      /*---(1 to 4)----------------------*/
      if (o.key[i].quad == 1 && o.key[i + 1].quad == 4) {
         /*> DEBUG_LINE  printf("#1 d1=%4d vs. d2=%4d : ", o.key[i].degs, o.key[i + 1].degs);   <*/
         if (dd <= 25 && o.key[i].degs <= 20 && o.key[i + 1].degs >= 340) {
            /*> DEBUG_LINE  printf("can be removed");                                 <*/
            KEY_del (i);
            i = 0;
         } else {
            /*> DEBUG_LINE  printf("no match");                                       <*/
         }
      }
      /*---(4 to 1)----------------------*/
      else if (o.key[i].quad == 4 && o.key[i + 1].quad == 1) {
         /*> DEBUG_LINE  printf("#2 d1=%4d vs. d2=%4d : ", o.key[i].degs, o.key[i + 1].degs);   <*/
         if (dd <= 25 && o.key[i].degs >= 340 && o.key[i + 1].degs <= 20) {
            /*> DEBUG_LINE  printf("can be removed");                                 <*/
            KEY_del (i);
            i = 0;
         } else {
            /*> DEBUG_LINE  printf("no match");                                       <*/
         }
      }
      /*---(same quad)-------------------*/
      else if (o.key[i].quad == o.key[i + 1].quad) {
         /*> DEBUG_LINE  printf("#3 same quadrant       : can be removed");           <*/
         KEY_del (i);
         i = 0;
      }
      /*---(otherwise)-------------------*/
      else {
         /*> DEBUG_LINE  printf("no rule");                                           <*/
      }
      /*---(wrap)------------------------*/
      /*> DEBUG_LINE  printf("\n");                                                   <*/
      /*> if (i == 0) DEBUG_LINE  printf("   restart tests after delete ---------\n");   <*/
      ++i;
   }
   /*> DEBUG_LINE  printf("   done\n\n");                                             <*/
   /*---(report out)---------------------*/
   /*> DEBUG_LINE  POINT_list (stdout, o.key, o.nkey);                              <*/
   /*---(complete)-------------------------------*/
   /*> DEBUG_LINE  printf("STRAIGHTENING LINES (end)\n\n");                           <*/
   return 0;
}

char     /*---: get key points into the corners/extremes ----------------*/
KEY_stretch        (void)
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
   /*> DEBUG_CRIT  printf("   pushing key points into corners/extremes\n");           <*/
   for (i = 0 ; i < o.nkey; ++i) {
      if (o.key[i    ].type == POINT_HEAD) {
         /*> DEBUG_CRIT  printf("      #00 [SKIPPING] (%3d) stroke beginning ------\n", i);   <*/
         continue;
      }
      if (o.key[i    ].type == POINT_HEAD || i+ 1 == o.nkey) {
         /*> DEBUG_CRIT  printf("      #00 [SKIPPING] (%3d) stroke ending ---------\n", i);   <*/
         continue;
      }
      q1   = o.key[i    ].quad;
      q2   = o.key[i + 1].quad;
      new  = o.key[i].p_bas;
      /*---(bigger y)--------------------*/
      if ((q1 == 1 && q2 == 4) || (q1 == 2 && q2 == 3)) {
         base = max  = o.key[i].y_raw;
         /*> DEBUG_CRIT  printf("      #01 [BIGGER Y] (%3d) %3d is %1d and %1d : base = %3d --------------------\n", i, o.key[i].p_bas, q1, q2, base);   <*/
         for (j = o.key[i - 1].p_bas; j < o.key[i + 1].p_bas; ++j) {
            /*> DEBUG_CRIT  printf("         %3d is %3d : ", j, o.avg[j].y_raw);      <*/
            if        (o.avg[j].p_bas == o.key[i].p_bas) {
               /*> DEBUG_CRIT  printf("existing\n");                                  <*/
            } else if (o.avg[j].y_raw >  base) {
               /*> DEBUG_CRIT  printf("NEW MAX\n");                                   <*/
               max = o.avg[j].y_raw;
               new = o.avg[j].p_bas;
            } else {
               /*> DEBUG_CRIT  printf("-\n");                                         <*/
            }
         }
         if (new != o.key[i].p_bas) {
            /*> DEBUG_CRIT  printf("         new max, %3d moves from %3d to %3d\n", i, o.key[i].p_bas, new);   <*/
            KEY_del (i);
            KEY_add (new, '-', POINT_NORMAL, NULL);
         } else {
            /*> DEBUG_CRIT  printf("         max remains\n");                         <*/
         }
      }
      /*---(lesser y)--------------------*/
      else if ((q1 == 4 && q2 == 1) || (q1 == 3 && q2 == 2)) {
         base = max  = o.key[i].y_raw;
         /*> DEBUG_CRIT  printf("      #02 [LESSER Y] (%3d) %3d is %1d and %1d : base = %3d --------------------\n", i, o.key[i].p_bas, q1, q2, base);   <*/
         for (j = o.key[i - 1].p_bas; j < o.key[i + 1].p_bas; ++j) {
            /*> DEBUG_CRIT  printf("         %3d is %3d : ", j, o.avg[j].y_raw);      <*/
            if        (o.avg[j].p_bas == o.key[i].p_bas) {
               /*> DEBUG_CRIT  printf("existing\n");                                  <*/
            } else if (o.avg[j].y_raw <  base) {
               /*> DEBUG_CRIT  printf("NEW MAX\n");                                   <*/
               max = o.avg[j].y_raw;
               new = o.avg[j].p_bas;
            } else {
               /*> DEBUG_CRIT  printf("-\n");                                         <*/
            }
         }
         if (new != o.key[i].p_bas) {
            /*> DEBUG_CRIT  printf("         new max, %3d moves from %3d to %3d\n", i, o.key[i].p_bas, new);   <*/
            KEY_del (i);
            KEY_add (new, '-', POINT_NORMAL, NULL);
         } else {
            /*> DEBUG_CRIT  printf("         max remains\n");                         <*/
         }
      }
      /*---(bigger x)--------------------*/
      else if ((q1 == 1 && q2 == 2) || (q1 == 4 && q2 == 3)) {
         base = max  = o.key[i].x_raw;
         /*> DEBUG_CRIT  printf("      #03 [BIGGER X] (%3d) %3d is %1d and %1d : base = %3d --------------------\n", i, o.key[i].p_bas, q1, q2, base);   <*/
         for (j = o.key[i - 1].p_bas; j < o.key[i + 1].p_bas; ++j) {
            /*> DEBUG_CRIT  printf("         %3d is %3d : ", j, o.avg[j].x_raw);      <*/
            if        (o.avg[j].p_bas == o.key[i].p_bas) {
               /*> DEBUG_CRIT  printf("existing\n");                                  <*/
            } else if (o.avg[j].x_raw >  base) {
               /*> DEBUG_CRIT  printf("NEW MAX\n");                                   <*/
               max = o.avg[j].x_raw;
               new = o.avg[j].p_bas;
            } else {
               /*> DEBUG_CRIT  printf("-\n");                                         <*/
            }
         }
         if (new != o.key[i].p_bas) {
            /*> DEBUG_CRIT  printf("         new max, %3d moves from %3d to %3d\n", i, o.key[i].p_bas, new);   <*/
            KEY_del (i);
            KEY_add (new, '-', POINT_NORMAL, NULL);
         } else {
            /*> DEBUG_CRIT  printf("         max remains\n");                         <*/
         }
      }
      /*---(lesser x)--------------------*/
      else if ((q1 == 2 && q2 == 1) || (q1 == 3 && q2 == 4)) {
         base = max  = o.key[i].x_raw;
         /*> DEBUG_CRIT  printf("      #04 [LESSER X] (%3d) %3d is %1d and %1d : base = %3d --------------------\n", i, o.key[i].p_bas, q1, q2, base);   <*/
         for (j = o.key[i - 1].p_bas; j < o.key[i + 1].p_bas; ++j) {
            /*> DEBUG_CRIT  printf("         %3d is %3d : ", j, o.avg[j].x_raw);      <*/
            if        (o.avg[j].p_bas == o.key[i].p_bas) {
               /*> DEBUG_CRIT  printf("existing\n");                                  <*/
            } else if (o.avg[j].x_raw <  base) {
               /*> DEBUG_CRIT  printf("NEW MAX\n");                                   <*/
               max = o.avg[j].x_raw;
               new = o.avg[j].p_bas;
            } else {
               /*> DEBUG_CRIT  printf("-\n");                                         <*/
            }
         }
         if (new != o.key[i].p_bas) {
            /*> DEBUG_CRIT  printf("         new max, %3d moves from %3d to %3d\n", i, o.key[i].p_bas, new);   <*/
            KEY_del (i);
            KEY_add (new, '-', POINT_NORMAL, NULL);
         } else {
            /*> DEBUG_CRIT  printf("         max remains\n");                         <*/
         }
      }
      /*---(quad 1 to 2)-----------------*/
      else {
         /*> DEBUG_CRIT  printf("      #?? (%3d) %3d is %1d and %1d\n", i, o.key[i].p_bas, q1, q2);   <*/
      }
   }
   KEY_calc  ('n');
   /*---(complete)-----------------------*/
   return 0;
}

char
KEY_squeeze      (void)
{
   /*---(locals)-------------------------*/
   int       i         = 0;
   int       dd        = 0;
   /*---(report out)---------------------*/
   /*> DEBUG_LINE  printf("SQUEEZING LINES (begin)\n");                               <*/
   /*> DEBUG_LINE  POINT_list (stdout, o.key, o.nkey);                              <*/
   /*---(process)------------------------*/
   /*> DEBUG_LINE  printf("   start...\n");                                           <*/
   while (i < o.nkey - 1) {
      /*> DEBUG_LINE  printf("   %3d/%3d) q=%d and %3d q=%d ", i, i + 1, o.key[i].quad, i + 1, o.key[i + 1].quad);   <*/
      /*---(skip start and finishes)-----*/
      if (o.key[i    ].type == POINT_HEAD || o.key[i + 1].type == POINT_HEAD) {
         /*> DEBUG_LINE  printf(": touches a line start, skipping\n");                <*/
         ++i;
         continue;
      }
      /*---(initial calcs)---------------*/
      dd = abs(o.key[i].degs - o.key[i + 1].degs);
      /*> DEBUG_LINE  printf("dd=%3d : ", dd);                                        <*/
      if (o.key[i].quad == 4 && o.key[i + 1].quad == 3) {
         /*> DEBUG_LINE  printf("rule #1 d1=%4d vs. d2=%4d : ", o.key[i].degs, o.key[i + 1].degs);   <*/
         if (dd <= 20 && o.key[i].degs <= 280 && o.key[i + 1].degs >= 260) {
            /*> DEBUG_LINE  printf("can be removed");                                 <*/
            KEY_del (i);
            i = 0;
         } else {
            /*> DEBUG_LINE  printf("no match");                                       <*/
         }
      } else if (o.key[i].quad == 3 && o.key[i + 1].quad == 4) {
         /*> DEBUG_LINE  printf("rule #2 d1=%4d vs. d2=%4d : ", o.key[i].degs, o.key[i + 1].degs);   <*/
         if (dd <= 20 && o.key[i].degs >= 260 && o.key[i + 1].degs <= 280) {
            /*> DEBUG_LINE  printf("can be removed");                                 <*/
            KEY_del (i);
            i = 0;
         } else {
            /*> DEBUG_LINE  printf("no match");                                       <*/
         }
      } else if (o.key[i].quad == o.key[i + 1].quad) {
         /*> DEBUG_LINE  printf("rule #3 same quadrant       : can be removed");      <*/
         KEY_del (i);
         i = 0;
      } else {
         /*> DEBUG_LINE  printf("no rule");                                           <*/
      }
      /*> DEBUG_LINE  printf("\n");                                                   <*/
      /*> if (i == 0) DEBUG_LINE  printf("   restart tests after delete ---------\n");   <*/
      ++i;
   }
   /*> DEBUG_LINE  printf("   done\n\n");                                             <*/
   /*---(report out)---------------------*/
   /*> DEBUG_LINE  POINT_list (stdout, o.key, o.nkey);                              <*/
   /*---(complete)-------------------------------*/
   /*> DEBUG_LINE  printf("SQUEEZING LINES (end)\n\n");                               <*/
   return 0;
}

/*> char                                                                                                                                 <* 
 *> KEY_sharps           (void)                                                                                                          <* 
 *> {                                                                                                                                    <* 
 *>    int i = 0;                                                                                                                        <* 
 *>    int  avg_pt;                                                                                                                      <* 
 *>    int  d1, d2, dd;                                                                                                                  <* 
 *>    int  sharp = 70;                                                                                                                  <* 
 *>    /+> int  sharp = 60;                                                               <+/                                            <* 
 *>    /+> o.key[o.nkey - 1].sharp = 'x';                                                    <+/                                         <* 
 *>    /+---(header)-------------------------+/                                                                                          <* 
 *>    DEBUG_CRIT   yLOG_enter   (__FUNCTION__);                                                                                         <* 
 *>    for (i = 1; i < o.nkey - 1; ++i) {                                                                                                <* 
 *>       /+> DEBUG__SHARPS  printf("  %02d) ", i);                                       <+/                                            <* 
 *>       if (o.key[i    ].sharp != '-' || o.key[i + 1].sharp != '-') {                                                                  <* 
 *>          /+> DEBUG__SHARPS  printf("skipping as its marked (%c) or the next is (%c)\n", o.key[i].sharp, o.key[i + 1].sharp);   <+/   <* 
 *>          continue;                                                                                                                   <* 
 *>       }                                                                                                                              <* 
 *>       avg_pt = o.key[i].p_bas;                                                                                                       <* 
 *>       d1     = o.avg[avg_pt - 1].degs;                                                                                               <* 
 *>       d2     = o.avg[avg_pt + 1].degs;                                                                                               <* 
 *>       dd     = d1 - d2;                                                                                                              <* 
 *>       if (dd >  180) dd -= 360;                                                                                                      <* 
 *>       if (dd < -180) dd += 360;                                                                                                      <* 
 *>       /+> DEBUG__SHARPS  printf(" is %5d vs. %5d, so %5d   ", d1, d2, abs(dd));       <+/                                            <* 
 *>       o.key[i].sharp = '-';                                                                                                          <* 
 *>       if (abs(dd) >= sharp) {                                                                                                        <* 
 *>          o.key[i].sharp = 'x';                                                                                                       <* 
 *>          /+> DEBUG__SHARPS  printf("SHARP\n");                                        <+/                                            <* 
 *>       }                                                                                                                              <* 
 *>    }                                                                                                                                 <* 
 *>    /+---(complete)-----------------------+/                                                                                          <* 
 *>    DEBUG_CRIT   yLOG_exit    (__FUNCTION__);                                                                                         <* 
 *>    return 0;                                                                                                                         <* 
 *> }                                                                                                                                    <*/

char
KEY_driver              (void)
{
   /*---(locals)-----------+-----+-----+-*/
   int         i           =    0;
   char        x_type      =  '-';
   char        q1          =  '-';
   char        q2          =  '-';
   char        x_prev      =  '-';
   /*---(header)-------------------------*/
   DEBUG_CRIT   yLOG_enter   (__FUNCTION__);
   /*---(process)------------------------*/
   for (i = 0; i < o.navg; ++i) {
      /*---(prepare)---------------------*/
      x_type  = o.avg [i    ].type;
      q1      = o.avg [i    ].quad;
      q2      = o.avg [i + 1].quad;
      DEBUG_CRIT   yLOG_complex ("review"    , "%2d, %c, %4.0fx %4.0fy, %c %c, %d %d", i, x_type, o.avg [i].x_pos, o.avg [i].y_pos, o.avg [i].prekey, o.avg [i].marked, q1, q2);
      /*---(always filter)---------------*/
      if (x_type == POINT_START )  continue;
      if (x_type == POINT_FINISH)  continue;
      /*---(always key)------------------*/
      if      (x_type == POINT_HEAD)    KEY_add (i, '-', POINT_HEAD  , NULL);
      else if (x_type == POINT_TAIL)    KEY_add (i, '-', POINT_TAIL  , NULL);
      else if (o.avg [i].prekey == 'k') KEY_add (i, '-', POINT_NORMAL, NULL);
      else                              continue;
      /*---(premarked)-------------------*/
      o.key [o.nkey - 1].prekey = o.avg [i].prekey;
      o.key [o.nkey - 1].marked = o.avg [i].marked;
      o.key [o.nkey - 1].sharp  = o.avg [i].sharp;
      DEBUG_CRIT   yLOG_complex ("marking"   , "%2d %c %c %c, %2d %c %c %c", o.nkey - 1, o.key [o.nkey - 1].prekey, o.key [o.nkey - 1].marked, o.key [o.nkey - 1].sharp, i, o.avg [i].prekey, o.avg [i].marked, o.avg [i].sharp);
      /*---(premark)---------------------*/
      if (o.key [o.nkey - 1].prekey == 'k') {
         x_prev = o.key [o.nkey - 2].marked;
         if (x_prev != o.key [o.nkey - 1].marked)   sprintf (o.key [o.nkey - 1].use, "%c", o.key [o.nkey - 1].marked);
         else                                       strlcpy (o.key [o.nkey - 1].use, "+", LEN_TERSE);
      }
      /*---(done)------------------------*/
   }
   /*> KEY__clean   ();                                                               <*/
   /*> KEY__extend  ();                                                               <*/
   /*> CLEAN_sharpen ();                                                               <*/
   /*---(display version)----------------*/
   /*> for (i = 0; i < o.nkey; ++i) {                                                 <* 
    *>    POINT_display (o.key + i);                                                  <* 
    *> }                                                                              <*/
   /*---(complete)-----------------------*/
   DEBUG_CRIT   yLOG_exit    (__FUNCTION__);
   return 0;
}




/*============================----(source-end)----============================*/
