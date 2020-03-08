/*============================---(source-start)---============================*/
#include    "gregg.h"

/*
 *
 * situation
 *    gregg is engineered to take a data stream of touch points, analyze it,
 *    and detirmine the most likely solution word.
 *
 * challenge
 *    manually creating data to test this processes rigorously is ugly,
 *    error prone, and combursome and therefore neglected.
 *
 * proposal
 *    create a reverse process whereby we cn create a source data stream from
 *    any selected word, both for testing and training.
 *
 * value
 *    the more testing data, the higher quality the solution.  and, the outlines
 *    can provide immediate feedback for user input.
 *
 *
 */

#define    SCR2RAW     10
#define    LEN_OUT     100000
#define    SCALING     1.3

static char   s_load       [LEN_OUT] = "";
static short  s_count      = 0;
static float  s_xpos       = 0;
static float  s_ypos       = 0;

static float  s_lef        = 0;
static float  s_rig        = 0;
static float  s_top        = 0;
static float  s_bot        = 0;

static float  s_push       = 0;



static short  s_index      =    0;
static char   s_label      [LEN_TERSE];
static char   s_type       =  '-';
static float  s_rot        =    0;
static float  s_beg        =    0;
static float  s_arc        =    0;
static float  s_xradius    =    0;
static float  s_yradius    =    0;



/*============================--------------------============================*/
/*===----                        creating output                       ----===*/
/*============================--------------------============================*/
static void o___OUTPUT_________________o (void) {;}

char
REVERSE_out__show_tail  (void)
{
   int         x_len       =    0;
   x_len = strlen (s_load);
   DEBUG_OUTP   yLOG_info    ("tail"      , s_load + x_len - 50);


}

char
REVERSE_out_start       (void)
{
   DEBUG_OUTP   yLOG_enter   (__FUNCTION__);
   strlcpy (s_load, ""         , LEN_OUT);
   s_count = 0;
   s_xpos = 0;
   s_ypos = 0;
   REVERSE_out_touch  (0, 0);
   DEBUG_OUTP   yLOG_exit    (__FUNCTION__);
   return 0;
}

char
REVERSE_out_touch       (int x, int y)
{
   char        t           [LEN_LABEL] = "";
   DEBUG_OUTP   yLOG_enter   (__FUNCTION__);
   sprintf (t, ";TOUCH,%d,%d", x, y);
   strlcat (s_load, t, LEN_OUT);
   s_xpos = x;
   s_ypos = y;
   ++s_count;
   REVERSE_out__show_tail ();
   DEBUG_OUTP   yLOG_exit    (__FUNCTION__);
   return 0;
}

char
REVERSE_out_append      (int x, int y)
{
   char        t           [LEN_LABEL] = "";
   sprintf (t, ";%d,%d", x, y);
   strlcat (s_load, t, LEN_OUT);
   ++s_count;
   REVERSE_out__show_tail ();
   return 0;
}

char
REVERSE_out_lift        (int x, int y)
{
   char        t           [LEN_LABEL] = "";
   DEBUG_OUTP   yLOG_enter   (__FUNCTION__);
   sprintf (t, ";LIFT,%d,%d", x, y);
   strlcat (s_load, t, LEN_OUT);
   ++s_count;
   REVERSE_out__show_tail ();
   DEBUG_OUTP   yLOG_exit    (__FUNCTION__);
   return 0;
}

char
REVERSE_out_done        (void)
{
   DEBUG_OUTP   yLOG_enter   (__FUNCTION__);
   REVERSE_out_lift (s_xpos, s_ypos);
   strlcat (s_load, ";"        , LEN_OUT);
   DEBUG_OUTP   yLOG_exit    (__FUNCTION__);
   return 0;
}

char
REVERSE_out_load        (void)
{
   DEBUG_OUTP   yLOG_enter   (__FUNCTION__);
   RAW_load (s_load);
   DEBUG_OUTP   yLOG_exit    (__FUNCTION__);
   return 0;
}



/*============================--------------------============================*/
/*===----                         helper functions                     ----===*/
/*============================--------------------============================*/
static void o___HELPER_________________o (void) {;}

ushort
REVERSE_find_letter     (char *a_ltr, char a_scope)
{
   /*---(locals)-----------+-----+-----+-*/
   short       i           =    0;
   short       n           =    0;
   /*---(walk letters)-------------------*/
   if (a_ltr == NULL)  return n;
   for (i = 0; i < MAX_LETTERS; ++i) {
      /*---(check end)-------------------*/
      if (a_scope != LTRS_ALL && strcmp (g_loc[i].label, "END") == 0)    break;
      if (strcmp (g_loc[i].label, "EOF") == 0)                           break;
      /*---(check match)-----------------*/
      if (strcmp (a_ltr, g_loc[i].label) != 0)                           continue;
      /*---(found)-----------------------*/
      n = i;
      break;
   }
   /*---(complete)-----------------------*/
   return n;
}

char
REVERSE_make_current    (char *a_ltr)
{
   /*---(locals)-----------+-----+-----+-*/
   char        rce         =  -10;
   short       i           =    0;
   short       n           =    0;
   char        x_name      [LEN_TERSE] = "";
   char        x_flip      =  '-';
   /*---(clear values)-------------------*/
   s_index      = 0;
   s_type       = '-';
   s_rot        = 0.0;
   s_beg        = 0.0;
   s_arc        = 0.0;
   s_xradius    = 0.0;
   s_yradius    = 0.0;
   /*---(defense)------------------------*/
   --rce;  if (a_ltr == NULL)  return rce;
   /*---(prepare)------------------------*/
   strlcpy (s_label, a_ltr, LEN_TERSE);
   strlcpy (x_name, a_ltr, LEN_TERSE);
   if (x_name [0] == 'e') {
      x_name [0] = 'a';
      x_flip     = 'y';
   }
   /*---(find letter)--------------------*/
   n  = REVERSE_find_letter (x_name, LTRS_ALL);
   --rce;  if (n <= 0)         return rce;
   /*---(save values)--------------------*/
   s_index      = n;
   s_type       = g_loc [n].type;
   s_rot        = g_loc [n].r_ellipse;
   s_beg        = g_loc [n].b_arc;
   s_arc        = g_loc [n].l_arc;
   s_xradius    = g_loc [n].x_ellipse;
   s_yradius    = g_loc [n].y_ellipse;
   /*---(adjust for ae)------------------*/
   if (x_flip == 'y') {
      if      (s_xradius <=  8)  s_xradius = 3;
      else if (s_xradius <= 11)  s_xradius = 4;
      else                       s_xradius = 5;
      if      (s_yradius <=  8)  s_yradius = 3;
      else if (s_yradius <= 11)  s_yradius = 4;
      else                       s_yradius = 5;
   }
   /*---(scale)--------------------------*/
   s_xradius   *= SCALING;
   s_yradius   *= SCALING;
   /*---(complete)-----------------------*/
   return 0;
}

char
REVERSE_begin           (int x, int y)
{
   REVERSE_out_start   ();
   /*> REVERSE_out_touch   (x, y);                                                    <*/
   return 0;
}

char
REVERSE_end             (void)
{
   /*> REVERSE_out_lift    (s_xpos, s_ypos);                                                <*/
   REVERSE_out_done    ();
   return 0;
}



/*============================--------------------============================*/
/*===----                         display lists                        ----===*/
/*============================--------------------============================*/
static void o___DRAWING________________o (void) {;}

char
REVERSE_draw_reset  (void)
{
   s_xpos    = 0;
   s_ypos    = 0;
   s_lef     = 0;
   s_rig     = 0;
   s_top     = 0;
   s_bot     = 0;
   return 0;
}

char
REVERSE_draw_init   (void)
{
   /*---(initialize)---------------------*/
   s_lef     = 0;
   s_rig     = 0;
   s_top     = 0;
   s_bot     = 0;
   s_push    = 0;
   /*---(complete)-----------------------*/
   return 0;
}

char
REVERSE_draw_begin  (void)
{
   /*---(start drawing)------------------*/
   glLineWidth (2.0);
   glBegin (GL_LINE_STRIP);
   /*---(complete)-----------------------*/
   return 0;
}

char
REVERSE_draw_point  (float x, float y)
{
   /*---(draw)---------------------------*/
   glVertex3f ( x, y, 5.0);
   /*---(bounds)-------------------------*/
   if (x < s_lef)   s_lef  = x;
   if (x > s_rig)   s_rig  = x;
   if (y < s_bot)   s_bot  = y;
   if (y > s_top)   s_top  = y;
   if (x > s_push)  s_push = x;
   /*---(complete)-----------------------*/
   return 0;
}

char
REVERSE_draw_end    (float x, float y)
{
   /*---(stop drawing)-------------------*/
   glEnd();
   glLineWidth (0.8);
   /*---(complete)-----------------------*/
   return 0;
}

char
REVERSE_draw_wrap   (float x, float y)
{
   /*---(get overall calcs)--------------*/
   o.tmp [0].p_bas  = 0;
   o.tmp [0].x_raw  = 0;
   o.tmp [0].y_raw  = 0;
   o.tmp [1].p_bas  = 1;
   o.tmp [1].x_raw  = x * 100.0;
   o.tmp [1].y_raw  = y * 100.0;
   POINT_calc (POINTS_TMP, o.tmp + 1, 'n');
   /*---(trending)-----------------------*/
   if (s_index > 0)  {
      g_loc [s_index].x_end  = x;
      g_loc [s_index].y_end  = y;
      g_loc [s_index].deg    = o.tmp [1].degs;
      g_loc [s_index].xy_len = o.tmp [1].len / 100.0;
      DEBUG_OUTP   yLOG_complex ("trend"     , "%5.1fd, %5.1fl, %5.1fx, %5.1fy", g_loc [s_index].deg, g_loc [s_index].xy_len, g_loc [s_index].x_end, g_loc [s_index].y_end);
   }
   /*---(bounds)-------------------------*/
   if (s_index > 0)  {
      g_loc [s_index].x_rig  = s_rig;
      g_loc [s_index].x_lef  = s_lef;
      g_loc [s_index].y_top  = s_top;
      g_loc [s_index].y_bot  = s_bot;
      DEBUG_OUTP   yLOG_complex ("bounds"    , "%5.1fl, %5.1fr, %5.1ft, %5.1fb", g_loc [s_index].x_lef, g_loc [s_index].x_rig, g_loc [s_index].y_top, g_loc [s_index].y_bot);
   }
   /*---(complete)-----------------------*/
   return 0;
}

char
REVERSE_dots_begin  (void)
{
   /*---(initialize)---------------------*/
   s_xpos    = 0;
   s_ypos    = 0;
   /*---(start drawing)------------------*/
   glLineWidth (1.0);
   glPointSize (1.0);
   yVIKEYS_view_color (YCOLOR_BAS_MIN, 0.50);
   glBegin(GL_POINTS);
   /*---(complete)-----------------------*/
   return 0;
}

char
REVERSE_dots_point  (float x, float y)
{
   glVertex3f ( x, y, 5.0);
   return 0;
}

char
REVERSE_dots_end    (void)
{
   glEnd();
   glLineWidth (0.8);
   return 0;
}



/*============================--------------------============================*/
/*===----                        creating shapes                       ----===*/
/*============================--------------------============================*/
static void o___SHAPES_________________o (void) {;}

char
REVERSE_line            (char a_type, char a_skip)
{
   /*---(locals)-----------+-----+-----+-*/
   char        rce         =  -10;
   float       d           =    0;
   float       r           =  0.0;
   float       l           =    0;
   short       i           =    0;
   float       x_sin, x_cos;
   float       x, y, px, py;
   int         x_sizer     =    1;
   /*---(defense)------------------------*/
   --rce;  if (s_type != SHAPE_LINE)    return rce;
   /*---(header)-------------------------*/
   DEBUG_OUTP   yLOG_enter   (__FUNCTION__);
   /*---(prepare)------------------------*/
   if (a_type == SHAPE_LOAD)  x_sizer = SCR2RAW;
   d     = s_rot;
   r     = d * DEG2RAD;
   l     = s_arc * x_sizer;
   x_sin = sin (r);
   x_cos = cos (r);
   /*---(rationalize)--------------------*/
   a_skip *= 2;
   if (a_skip <=  0)          a_skip = 1;
   if (a_skip >= 25)          a_skip = 25;
   DEBUG_OUTP   yLOG_complex ("basics"    , "%2d# %-5.5s  %6.1fd, %8.3fr, %4.1fl, %2ds", s_index, s_label, d, r, l, a_skip);
   /*---(create points)------------------*/
   if (a_type == SHAPE_SAMPLE)   REVERSE_draw_reset ();
   if (a_type != SHAPE_LOAD)     REVERSE_draw_begin ();
   for (i = 0; i <= l; i += a_skip) {
      x = s_xpos + (i * x_cos);
      y = s_ypos + (i * x_sin);
      /*> DEBUG_OUTP   yLOG_complex ("point"     , "%3#, %4.1fx, %4.1fy", i, x, y);   <*/
      if (a_type != SHAPE_LOAD)     REVERSE_draw_point (x, y);
      else                          REVERSE_out_append (x, y);
   }
   if (a_type != SHAPE_LOAD)     REVERSE_draw_end   (x, y);
   if (a_type == SHAPE_SAMPLE)   REVERSE_draw_wrap  (x, y);
   /*---(save)---------------------------*/
   s_xpos = x;
   s_ypos = y;
   /*---(complete)-----------------------*/
   DEBUG_OUTP   yLOG_exit    (__FUNCTION__);
   return 0;
}

char
REVERSE_circle          (char a_type, char a_skip)
{
   /*---(locals)-----------+-----+-----+-*/
   char        rce         =  -10;
   int         i           =    0;
   float       l;
   float       x_rot;
   float       r, x, y;
   float       x_adj, y_adj;
   int         x_sizer     =    1;
   /*---(header)-------------------------*/
   DEBUG_OUTP   yLOG_enter   (__FUNCTION__);
   /*---(defense)------------------------*/
   --rce;  if (s_type != SHAPE_CIRCLE) {
      DEBUG_OUTP   yLOG_exitr   (__FUNCTION__, rce);
      return rce;
   }
   /*---(tables)-------------------------*/
   if (a_type == SHAPE_LOAD)  x_sizer = SCR2RAW;
   l        = s_xradius * x_sizer;
   x_rot    = s_rot * DEG2RAD;
   /*---(determine adjustment)-----------*/
   x_adj  = l * cos (x_rot);
   y_adj  = l * sin (x_rot);
   /*---(rationalize)--------------------*/
   if (s_xradius < 6) a_skip *= 2;
   if (a_skip <=  0)  a_skip = 1;
   if (a_skip >= 25)  a_skip = 25;
   DEBUG_OUTP   yLOG_complex ("basics"    , "%2d# %-5.5s  %5.1fl %5.1fr %5.1fxa %5.1fya %2ds", s_index, s_label, l, x_rot, x_adj, y_adj, a_skip);
   /*---(start)--------------------------*/
   if (a_type == SHAPE_SAMPLE)   REVERSE_draw_reset ();
   if (a_type != SHAPE_LOAD)     REVERSE_draw_begin ();
   /*---(create points)------------------*/
   for (i = 0; i <= 360; i += a_skip) {
      r     = i * DEG2RAD;
      x     = s_xpos + (l * cos (r)) - x_adj;
      y     = s_ypos + (l * sin (r)) - y_adj;
      /*> DEBUG_OUTP   yLOG_complex ("point"     , "%3#, %4.1fx, %4.1fy", i, x, y);   <*/
      if (a_type != SHAPE_LOAD)     REVERSE_draw_point (x, y);
      else                          REVERSE_out_append (x, y);
   }
   /*---(save)---------------------------*/
   if (a_type != SHAPE_LOAD)     REVERSE_draw_end   (s_xpos, s_ypos);
   if (a_type == SHAPE_SAMPLE)   REVERSE_draw_wrap  (s_xpos, s_ypos);
   /*---(complete)-----------------------*/
   DEBUG_OUTP   yLOG_exit    (__FUNCTION__);
   return 0;
}

char
REVERSE_ellipse         (char a_type, char a_skip)
{
   /*---(locals)-----------+-----+-----+-*/
   char        rce         =  -10;
   int         i           =    0;
   float       a, a_sin, a_cos;
   float       b, b_sin, b_cos;
   float       x, y;
   int         x_beg, x_arc;
   int         x_len, y_len;
   float       x_adj, y_adj;
   int         x_sizer     =    1;
   int         x_dots      =    5;
   float       x_1st, y_1st;
   /*---(defense)------------------------*/
   --rce;  if (s_type != SHAPE_ELLIPSE) return rce;
   /*---(header)-------------------------*/
   DEBUG_OUTP   yLOG_enter   (__FUNCTION__);
   /*---(tables)-------------------------*/
   if (a_type == SHAPE_LOAD)  x_sizer = SCR2RAW;
   x_len  = s_xradius * x_sizer;
   y_len  = s_yradius * x_sizer;
   x_beg  = s_beg;
   x_arc  = s_arc;
   x_dots = 10;
   /*---(rationalize)--------------------*/
   if (s_xradius < 6) a_skip *= 4;
   if (a_skip <=  0)  a_skip = 1;
   if (a_skip >= 25)  a_skip = 25;
   DEBUG_OUTP   yLOG_complex ("basics"    , "%2d# %-5.5s  %5.1fxl %5.1dfl %5.1fb %5.1fa  %2ds", s_index, s_label, x_len, y_len, x_beg, x_arc, a_skip);
   /*---(angles)-------------------------*/
   a      = x_beg * DEG2RAD;
   a_sin  = sin(a);
   a_cos  = cos(a);
   b      = s_rot * DEG2RAD;
   b_sin  = sin (b);
   b_cos  = cos (b);
   /*---(determine adjustment)-----------*/
   x_adj  = (x_len * a_cos * b_cos) - (y_len * a_sin * b_sin);
   y_adj  = (x_len * a_cos * b_sin) + (y_len * a_sin * b_cos);
   /*---(draw dotted outline)------------*/
   DEBUG_OUTP   yLOG_note    ("before dots");
   if (a_type == SHAPE_SAMPLE)   REVERSE_draw_reset ();
   if (a_type == SHAPE_SAMPLE) {
      REVERSE_dots_begin ();
      for (i = 0; i <= 360; i += x_dots) {
         a     = i * DEG2RAD;
         a_sin = sin (a);
         a_cos = cos (a);
         x     = s_xpos + (x_len * a_cos * b_cos) - (y_len * a_sin * b_sin) - x_adj;
         y     = s_ypos + (x_len * a_cos * b_sin) + (y_len * a_sin * b_cos) - y_adj;
         REVERSE_dots_point (x, y);
      }
      REVERSE_dots_end   ();
   }
   DEBUG_OUTP   yLOG_note    ("after dots");
   /*---(create points)------------------*/
   if (a_type != SHAPE_LOAD)     REVERSE_draw_begin ();
   if (x_arc >= 0) {
      DEBUG_OUTP   yLOG_note    ("draw positive");
      for (i = x_beg; i <= x_beg + x_arc; i += a_skip) {
         a     = i * DEG2RAD;
         a_sin = sin (a);
         a_cos = cos (a);
         x     = s_xpos + (x_len * a_cos * b_cos) - (y_len * a_sin * b_sin) - x_adj;
         y     = s_ypos + (x_len * a_cos * b_sin) + (y_len * a_sin * b_cos) - y_adj;
         /*> DEBUG_OUTP   yLOG_complex ("point"     , "%3#, %4.1fx, %4.1fy", i, x, y);   <*/
         if (a_type != SHAPE_LOAD)     REVERSE_draw_point (x, y);
         else                          REVERSE_out_append (x, y);
         if (i == x_beg) {  x_1st = x; y_1st = y; }
      }
   } else {
      DEBUG_OUTP   yLOG_note    ("draw negative");
      for (i = x_beg; i >= x_beg + x_arc; i -= a_skip) {
         a     = i * DEG2RAD;
         a_sin = sin (a);
         a_cos = cos (a);
         x     = s_xpos + (x_len * a_cos * b_cos) - (y_len * a_sin * b_sin) - x_adj;
         y     = s_ypos + (x_len * a_cos * b_sin) + (y_len * a_sin * b_cos) - y_adj;
         /*> DEBUG_OUTP   yLOG_complex ("point"     , "%3#, %4.1fx, %4.1fy", i, x, y);   <*/
         if (a_type != SHAPE_LOAD)     REVERSE_draw_point (x, y);
         else                          REVERSE_out_append (x, y);
         if (i == x_beg) {  x_1st = x; y_1st = y; }
      }
   }
   DEBUG_OUTP   yLOG_note    ("after draw");
   /*---(save)---------------------------*/
   if (a_type != SHAPE_LOAD)     REVERSE_draw_end   (x, y);
   if (a_type == SHAPE_SAMPLE)   REVERSE_draw_wrap  (x, y);
   s_xpos = x;
   s_ypos = y;
   /*---(complete)-----------------------*/
   DEBUG_OUTP   yLOG_exit    (__FUNCTION__);
   return 0;
}

char
REVERSE_teardrop        (char a_type, char a_skip)
{ /* uses the syntractrix of poleni, also called the convict's curve */
   /*---(locals)-----------+-----+-----+-*/
   char        rce         =  -10;
   float       s           =    0;
   float       x_len       =    0;
   float       y_len       =    0;
   float       a, b;
   float       c;
   float       x, y;
   int         x_sizer     =    1;
   float       x_rot       =  0.0;
   float       x_arc       =  0.0;
   /*---(header)-------------------------*/
   DEBUG_OUTP   yLOG_enter   (__FUNCTION__);
   /*---(defense)------------------------*/
   --rce;  if (s_type != SHAPE_TEARDROP) {
      DEBUG_OUTP   yLOG_exitr   (__FUNCTION__, rce);
      return rce;
   }
   /*---(table values)-------------------*/
   if (a_type == SHAPE_LOAD)  x_sizer = SCR2RAW;
   x_len  = s_xradius * x_sizer * 1.8;
   y_len  = s_yradius * x_sizer * 1.8;
   x_rot  = s_rot;
   x_arc  = s_arc;
   /*---(determine adjustment)-----------*/
   /*> x_adj  = l * cos (x_rot * DEG2RAD);                                            <* 
    *> y_adj  = l * sin (x_rot * DEG2RAD);                                            <*/
   /*---(rationalize)--------------------*/
   if (s_xradius < 6) a_skip *= 2;
   if (a_skip <=  0)  a_skip = 1;
   if (a_skip >= 25)  a_skip = 25;
   DEBUG_OUTP   yLOG_complex ("basics"    , "%2d# %-5.5s  %5.1fxl  %5.1fyl %5.1fr", s_index, s_label, x_len, y_len, x_rot);
   /*---(start drawing)------------------*/
   if (a_type == SHAPE_SAMPLE)   REVERSE_draw_reset ();
   if (a_type != SHAPE_LOAD)     REVERSE_draw_begin ();
   if (a_type != SHAPE_LOAD)     REVERSE_draw_point (s_xpos, s_ypos);
   else                          REVERSE_out_append (s_xpos, s_ypos);
   /*---(draw points)--------------------*/
   if (x_arc >= 0) {
      for (s = -1.915;  s <= 1.915; s += 0.01) {
         a    = x_len * ((2.0 / cosh (s)) - 0.577);
         b    = y_len * (s - 2.0 * tanh (s));
         x    = s_xpos + a * cos (x_rot * DEG2RAD) + b * sin (x_rot * DEG2RAD);
         y    = s_ypos - a * sin (x_rot * DEG2RAD) + b * cos (x_rot * DEG2RAD);
         if (a_type != SHAPE_LOAD)     REVERSE_draw_point (x, y);
         else                          REVERSE_out_append (x, y);
         DEBUG_OUTP_M yLOG_complex ("point"     , "%5.1fs, %5.1fa, %5.1fb, %5.1fx, %5.1fy", s, a, b, x, y);
      }
   } else {
      for (s = 1.915;  s >= -1.915; s -= 0.01) {
         a    = x_len * ((2.0 / cosh (s)) - 0.577);
         b    = y_len * (s - 2.0 * tanh (s));
         x    = s_xpos + a * cos (x_rot * DEG2RAD) + b * sin (x_rot * DEG2RAD);
         y    = s_ypos - a * sin (x_rot * DEG2RAD) + b * cos (x_rot * DEG2RAD);
         if (a_type != SHAPE_LOAD)     REVERSE_draw_point (x, y);
         else                          REVERSE_out_append (x, y);
         DEBUG_OUTP_M yLOG_complex ("point"     , "%5.1fs, %5.1fa, %5.1fb, %5.1fx, %5.1fy", s, a, b, x, y);
      }
   }
   /*---(end drawing)--------------------*/
   if (a_type != SHAPE_LOAD)     REVERSE_draw_point (s_xpos, s_ypos);
   else                          REVERSE_out_append (s_xpos, s_ypos);
   if (a_type != SHAPE_LOAD)     REVERSE_draw_end   (s_xpos, s_ypos);
   if (a_type == SHAPE_SAMPLE)   REVERSE_draw_wrap  (s_xpos, s_ypos);
   /*---(complete)-----------------------*/
   DEBUG_OUTP   yLOG_exit    (__FUNCTION__);
   return 0;
}

char
REVERSE_dot             (char a_type, char a_skip)
{
   /*---(locals)-----------+-----+-----+-*/
   char        rce         =  -10;
   int         i           =    0;
   float       r, l;
   float       x, y;
   int         x_sizer     =    1;
   /*---(defense)------------------------*/
   --rce;  if (s_type != SHAPE_DOT)     return rce;
   /*---(header)-------------------------*/
   DEBUG_OUTP   yLOG_enter   (__FUNCTION__);
   /*---(tables)-------------------------*/
   if (a_type == SHAPE_LOAD)  x_sizer = SCR2RAW;
   l  = s_xradius * x_sizer;
   /*---(draw)---------------------------*/
   if (a_type == SHAPE_SAMPLE)   REVERSE_draw_reset ();
   if (a_type != SHAPE_LOAD)     REVERSE_draw_begin ();
   for (i = 0; i <= 360; i += a_skip) {
      r  = i * DEG2RAD;
      x = s_xpos + l * sin (r);
      y = s_ypos + l * cos (r);
      if (a_type != SHAPE_LOAD)     REVERSE_draw_point (x, y);
      else                          REVERSE_out_append (x, y);
   }
   if (a_type != SHAPE_LOAD)     REVERSE_draw_end   (x, y);
   if (a_type == SHAPE_SAMPLE)   REVERSE_draw_wrap  (x, y);
   /*---(complete)-----------------------*/
   DEBUG_OUTP   yLOG_exit    (__FUNCTION__);
   return 0;
}

char
REVERSE_space           (char a_type, char a_skip)
{
   /*---(locals)-----------+-----+-----+-*/
   char        rce         =  -10;
   float       x, y;
   int         x_sizer     =    1;
   /*---(defense)------------------------*/
   --rce;  if (s_type != SHAPE_SPACE)   return rce;
   /*---(header)-------------------------*/
   DEBUG_OUTP   yLOG_enter   (__FUNCTION__);
   /*---(tables)-------------------------*/
   if (a_type == SHAPE_LOAD)  x_sizer = SCR2RAW;
   x  = s_xradius * x_sizer;
   y  = s_yradius * x_sizer;
   if (a_type == SHAPE_SAMPLE)   REVERSE_draw_reset ();
   if (a_type != SHAPE_LOAD)     REVERSE_draw_begin ();
   if (a_type != SHAPE_LOAD)     REVERSE_draw_end   (x, y);
   if (a_type == SHAPE_SAMPLE)   REVERSE_draw_wrap  (x, y);
   /*---(complete)-----------------------*/
   DEBUG_OUTP   yLOG_exit    (__FUNCTION__);
   return 0;
}

char
REVERSE_draw_exit      (float x_beg, float y_beg, float x_end, float y_end, char a_type, char a_skip)
{
   /*---(locals)-----------+-----+-----+-*/
   char        rce         =  -10;
   int         i           =    0;
   float       xd, yd;
   float       sx, sy, l;
   float       x, y;
   /*---(prepare)------------------------*/
   xd   = x_end - x_beg;
   yd   = y_end - y_beg;
   if (xd == 0)  xd = 0.0000001;
   l    = sqrt ((xd * xd) + (yd * yd));
   sx   = xd / l;
   sy   = yd / l;
   /*---(rationalize)--------------------*/
   if (a_skip <=  0)          a_skip = 1;
   if (a_skip >= 25)          a_skip = 25;
   /*---(create points)------------------*/
   for (i = 0; i <= l; i += a_skip) {
      x = x_beg + (i * sx);
      y = y_beg + (i * sy);
      if (a_type != SHAPE_LOAD)     REVERSE_draw_point (x, y);
      else                          REVERSE_out_append (x, y);
   }
   /*---(save)---------------------------*/
   s_xpos = x;
   s_ypos = y;
   /*---(complete)-----------------------*/
   return 0;
}



/*============================--------------------============================*/
/*===----                          outlines                            ----===*/
/*============================--------------------============================*/
static void o___OUTLINES_______________o (void) {;}

char
REVERSE_make_letter     (uchar *a_ltr, char a_type, char a_skip)
{
   /*---(locals)-----------+-----+-----+-*/
   char        rce         =  -10;
   char        rc          =    0;
   /*---(header)-------------------------*/
   DEBUG_OUTP   yLOG_enter   (__FUNCTION__);
   rc = REVERSE_make_current (a_ltr);
   DEBUG_OUTP   yLOG_value   ("current"   , rc);
   --rce;  if (rc < 0) {
      DEBUG_OUTP   yLOG_exitr   (__FUNCTION__, rce);
      return rce;
   }
   switch (s_type) {
   case SHAPE_LINE     :  REVERSE_line     (a_type, a_skip);  break;
   case SHAPE_ELLIPSE  :  REVERSE_ellipse  (a_type, a_skip);  break;
   case SHAPE_CIRCLE   :  REVERSE_circle   (a_type, a_skip);  break;
   case SHAPE_TEARDROP :  REVERSE_teardrop (a_type, a_skip);  break;
   case SHAPE_DOT      :  REVERSE_dot      (a_type, a_skip);  break;
   case SHAPE_SPACE    :  REVERSE_space    (a_type, a_skip);  break;
   }
   /*---(complete)-----------------------*/
   DEBUG_OUTP   yLOG_exit    (__FUNCTION__);
   return 0;
}

char
REVERSE_make_offset     (int a_sizer)
{
   s_xpos +=  0.0 * a_sizer * SCALING;
   s_ypos -= 10.0 * a_sizer * SCALING;
   return 0;
}

char
REVERSE_make_next       (void)
{
   s_xpos = s_push + 10.0 * SCALING;
   if (s_xpos > 750)  {
      s_xpos  = 30.0;
      s_ypos -= 60.0 * SCALING;
   }
   return 0;
}

char
REVERSE_gregg_word      (uchar *a_outline, char a_type, char a_skip)
{
   /*---(locals)-----------+-----+-----+-*/
   char        rce         =  -10;
   int         i           =    0;
   char        t           [LEN_HUND]  = "";
   char       *p           = NULL;
   uchar      *q           = "·";
   char       *r           = NULL;
   int         n           =    0;
   uchar      *x_last      = NULL;
   int         x_sizer     =    1;
   /*---(header)-------------------------*/
   DEBUG_OUTP   yLOG_enter   (__FUNCTION__);
   /*---(defense)------------------------*/
   DEBUG_OUTP   yLOG_point   ("a_outline" , a_outline);
   --rce;  if (a_outline == NULL) {
      DEBUG_OUTP   yLOG_exitr   (__FUNCTION__, rce);
      return rce;
   }
   DEBUG_OUTP   yLOG_info    ("a_outline" , a_outline);
   /*---(prepare)------------------------*/
   DEBUG_OUTP   yLOG_char    ("a_type"    , a_type);
   DEBUG_OUTP   yLOG_complex ("big_pos "  , "%5.1fsx, %5.1fsy", s_xpos, s_ypos);
   --rce;  if (a_type == SHAPE_DRAW)     REVERSE_draw_init  ();
   else if    (a_type == SHAPE_LOAD)     REVERSE_out_start  ();
   else   {
      DEBUG_OUTP   yLOG_exitr   (__FUNCTION__, rce);
      return rce;
   }
   if (a_type == SHAPE_LOAD)  x_sizer = SCR2RAW;
   /*---(start parsing)------------------*/
   DEBUG_OUTP   yLOG_complex ("big_pos "  , "%5.1fsx, %5.1fsy", s_xpos, s_ypos);
   strlcpy  (t, a_outline, LEN_HUND);
   strltrim (t, ySTR_BOTH, LEN_HUND);
   p = strtok_r (t, q, &r);
   if (p != NULL && p [0] == 'a')  s_xpos += 20;
   if (p != NULL && p [0] == 'e')  s_xpos += 10;
   /*> REVERSE_make_letter  (">", a_type, a_skip);                           <*/
   /*---(walk)---------------------------*/
   while (p != NULL) {
      DEBUG_OUTP   yLOG_info    ("p"         , p);
      x_last = p;
      if (strcmp (">", p) == 0) {
         DEBUG_OUTP   yLOG_note    ("found a multipart indicator");
         if      (a_type == SHAPE_LOAD) {
            REVERSE_out_lift    (s_xpos, s_ypos);
            REVERSE_make_offset (x_sizer);
            REVERSE_out_touch   (s_xpos, s_ypos);
         } else {
            REVERSE_make_offset (x_sizer);
         }
      } else {
         DEBUG_OUTP   yLOG_note    ("normal letter");
         /*> REVERSE_make_letter  ("dot", a_type, a_skip);                            <*/
         REVERSE_make_letter  (p, a_type, a_skip);
         /*> switch (s_type) {                                                        <* 
          *> case SHAPE_LINE     :  REVERSE_line     (a_type, a_skip);  break;        <* 
          *> case SHAPE_ELLIPSE  :  REVERSE_ellipse  (n, a_type, a_skip);  break;     <* 
          *> case SHAPE_CIRCLE   :  REVERSE_circle   (n, a_type, a_skip);  break;     <* 
          *> case SHAPE_TEARDROP :  REVERSE_teardrop (n, a_type, a_skip);  break;     <* 
          *> case SHAPE_DOT      :  REVERSE_dot      (n, a_type, a_skip);  break;     <* 
          *> case SHAPE_SPACE    :  REVERSE_space    (n, a_type, a_skip);  break;     <* 
          *> }                                                                        <*/
      }
      p = strtok_r (NULL, q, &r);
   }
   if (x_last != NULL && x_last [0] == 'a')  s_xpos += 20;
   if (x_last != NULL && x_last [0] == 'e')  s_xpos += 10;
   /*---(wrap-up)------------------------*/
   s_index = 0;
   if      (a_type == SHAPE_DRAW)     REVERSE_draw_wrap  (s_xpos, s_ypos);
   else                               REVERSE_out_done   ();
   /*---(complete)-----------------------*/
   DEBUG_OUTP   yLOG_exit    (__FUNCTION__);
   return 0;
}

char
REVERSE_gregg_text      (uchar *a_text, char a_type, char a_skip, char a_reset)
{
   /*---(locals)-----------+-----+-----+-*/
   char        rce         =  -10;
   char        t           [LEN_HUGE]  = "";
   uchar      *p           = NULL;
   uchar      *q           = " ";
   char       *r           = NULL;
   int         x_len       =    0;
   /*---(header)-------------------------*/
   DEBUG_OUTP   yLOG_enter   (__FUNCTION__);
   /*---(defense)------------------------*/
   DEBUG_OUTP   yLOG_point   ("a_text"    , a_text);
   --rce;  if (a_text == NULL) {
      DEBUG_OUTP   yLOG_exitr   (__FUNCTION__, rce);
      return rce;
   }
   DEBUG_OUTP   yLOG_info    ("a_text"    , a_text);
   /*---(start parsing)------------------*/
   if (a_reset == 'y')  REVERSE_draw_reset  ();
   strlcpy  (t, a_text, LEN_HUGE);
   p = strtok_r (t, q, &r);
   /*---(walk)---------------------------*/
   while (p != NULL) {
      x_len = strlen (p);
      DEBUG_OUTP   yLOG_complex ("parsed"    , "%2d[%s]", x_len, p);
      if (x_len == 1 && p [0] == (uchar) '¦')   s_xpos = 9999;
      else                                      REVERSE_gregg_word (p, a_type, a_skip);
      s_xpos = s_push + 10.0 * SCALING;
      DEBUG_OUTP   yLOG_value   ("s_rig"     , s_rig);
      DEBUG_OUTP   yLOG_value   ("s_push"    , s_push);
      /*---(prepare next)----------------*/
      REVERSE_make_next    ();
      DEBUG_OUTP   yLOG_complex ("big_pos "  , "%5.1fsx, %5.1fsy", s_xpos, s_ypos);
      p = strtok_r (NULL, q, &r);
   }
   /*---(complete)-----------------------*/
   DEBUG_OUTP   yLOG_exit    (__FUNCTION__);
   return 0;
}



/*============================--------------------============================*/
/*===----                       display by english                     ----===*/
/*============================--------------------============================*/
static void o___BY_ENGLISH________________o (void) {;}

char
REVERSE_english_word    (uchar *a_word, char a_type, char a_skip)
{
   /*---(locals)-----------+-----+-----+-*/
   char        rce         =  -10;
   char        rc          =    0;
   tWORDS     *x_word      = NULL;
   /*---(header)-------------------------*/
   DEBUG_OUTP   yLOG_enter   (__FUNCTION__);
   /*---(defense)------------------------*/
   DEBUG_OUTP   yLOG_point   ("a_word"    , a_word);
   --rce;  if (a_word == NULL) {
      DEBUG_OUTP   yLOG_exitr   (__FUNCTION__, rce);
      return rce;
   }
   DEBUG_OUTP   yLOG_info    ("a_word"    , a_word);
   /*---(look it up)---------------------*/
   rc = BTREE_by_english (&x_word, a_word);
   DEBUG_OUTP   yLOG_value   ("btree"     , rc);
   DEBUG_OUTP   yLOG_point   ("x_word"    , x_word);
   --rce;  if (rc < 0 || x_word == NULL) {
      DEBUG_OUTP   yLOG_exitr   (__FUNCTION__, rce);
      return rce;
   }
   strlcpy (my.word , a_word       , LEN_HUND);
   strlcpy (my.gregg, x_word->gregg, LEN_HUND);
   /*---(translate)----------------------*/
   rc = WORDS_drawn_show     (x_word->drawn, my.shown);
   DEBUG_OUTP   yLOG_value   ("tranlate"  , rc);
   --rce;  if (rc < 0) {
      DEBUG_OUTP   yLOG_exitr   (__FUNCTION__, rce);
      return rce;
   }
   DEBUG_OUTP   yLOG_info    ("my.shown"  , my.shown);
   /*---(display)------------------------*/
   rc = REVERSE_gregg_word (my.shown, a_type, a_skip);
   DEBUG_OUTP   yLOG_value   ("outline"   , rc);
   --rce;  if (rc < 0) {
      DEBUG_OUTP   yLOG_exitr   (__FUNCTION__, rce);
      return rce;
   }
   /*---(complete)-----------------------*/
   DEBUG_OUTP   yLOG_exit    (__FUNCTION__);
   return 0;
}

char
REVERSE_english_text    (uchar *a_text, char a_type, char a_skip, char a_reset)
{
   /*---(locals)-----------+-----+-----+-*/
   char        rce         =  -10;
   char        t           [LEN_HUGE]  = "";
   uchar      *p           = NULL;
   uchar      *q           = " ";
   char       *r           = NULL;
   int         n           =    0;
   int         x_len       =    0;
   /*---(header)-------------------------*/
   DEBUG_OUTP   yLOG_enter   (__FUNCTION__);
   /*---(defense)------------------------*/
   DEBUG_OUTP   yLOG_point   ("a_text"    , a_text);
   --rce;  if (a_text == NULL) {
      DEBUG_OUTP   yLOG_exitr   (__FUNCTION__, rce);
      return rce;
   }
   DEBUG_OUTP   yLOG_info    ("a_text"    , a_text);
   /*---(start parsing)------------------*/
   if (a_reset == 'y')  REVERSE_draw_reset  ();
   strlcpy  (t, a_text, LEN_HUGE);
   p = strtok_r (t, q, &r);
   /*---(walk)---------------------------*/
   while (p != NULL) {
      x_len = strlen (p);
      DEBUG_OUTP   yLOG_complex ("parsed"    , "%2d[%s]", x_len, p);
      if (x_len == 1 && p [0] == (uchar) '¦')   s_xpos = 9999;
      else                                      REVERSE_english_word (p, a_type, a_skip);
      s_xpos = s_push + 10.0 * SCALING;
      DEBUG_OUTP   yLOG_value   ("s_rig"     , s_rig);
      DEBUG_OUTP   yLOG_value   ("s_push"    , s_push);
      /*---(prepare next)----------------*/
      REVERSE_make_next    ();
      DEBUG_OUTP   yLOG_complex ("big_pos "  , "%5.1fsx, %5.1fsy", s_xpos, s_ypos);
      p = strtok_r (NULL, q, &r);
   }
   /*---(complete)-----------------------*/
   DEBUG_OUTP   yLOG_exit    (__FUNCTION__);
   return 0;
}



/*============================--------------------============================*/
/*===----                         page handling                        ----===*/
/*============================--------------------============================*/
static void o___PAGES_____________________o (void) {;}

static uint      s_tex     =   0;
static uint      s_fbo     =   0;
static uint      s_depth   =   0;

char
REVERSE_page_beg        (void)
{
   /*---(locals)-----------+-----+-----+-*/
   char        rc          =    0;
   int         n           =    0;
   int         x, y;
   /*---(header)-------------------------*/
   DEBUG_OUTP   yLOG_enter   (__FUNCTION__);
   /*---(prepare)------------------------*/
   REVERSE_draw_reset  ();
   s_xpos =  30.0;
   s_ypos = -50.0;
   /*---(create texture)-----------------*/
   yVIKEYS_view_color_clear (YCOLOR_BAS_MED);
   rc = yGLTEX_new (&s_tex, &s_fbo, &s_depth, 850, 1100);
   rc = yGLTEX_draw_start   (s_fbo, YGLTEX_TOPLEF, 850, 1100, 1.0);
   glColor4f (1.00f, 1.00f, 1.00f, 1.0f);
   /*---(complete)-----------------------*/
   DEBUG_OUTP   yLOG_exit    (__FUNCTION__);
   return 0;
}

char
REVERSE_page_end        (void)
{
   /*---(locals)-----------+-----+-----+-*/
   char        rc          =    0;
   /*---(header)-------------------------*/
   DEBUG_OUTP   yLOG_enter   (__FUNCTION__);
   /*---(write out image)----------------*/
   rc = yGLTEX_tex2png      ("/tmp/gregg.png", 850, 1100);
   /*---(close texture)------------------*/
   rc = yGLTEX_draw_end     (s_tex);
   /*---(complete)-----------------------*/
   DEBUG_OUTP   yLOG_exit    (__FUNCTION__);
   return 0;
}



/*============================--------------------============================*/
/*===----                         data reporting                       ----===*/
/*============================--------------------============================*/
static void o___REPORTING_________________o (void) {;}

char
REVERSE_report          (void)
{
   /*---(locals)-----------+-----+-----+-*/
   short       i           =    0;
   /*---(walk letters)-------------------*/
   for (i = 0; i < MAX_LETTERS; ++i) {
      if (strcmp (g_loc [i].label, "EOF") == 0)   break;
      if (strcmp (g_loc [i].label, "."  ) == 0)   break;
      if (strcmp (g_loc [i].label, "bof") == 0)   continue;
      printf ("%3d  %-5.5s %6.1fd %6.1fl\n", i, g_loc [i].label, g_loc [i].deg, g_loc [i].xy_len);
   }
   /*---(complete)-----------------------*/
}


/*============================--------------------============================*/
/*===----                         unit testing                         ----===*/
/*============================--------------------============================*/
static void o___UNIT_TEST_________________o (void) {;}

char*        /*-> unit test accessor -----------------[ light  [us.D90.241.L0]*/ /*-[03.0000.00#.#]-*/ /*-[--.---.---.--]-*/
REVERSE__unit        (char *a_question, int a_num)
{
   /*---(locals)-----------+-----+-----+-*/
   int         i           = 0;
   char        t           [100] = "";
   char        s           [100] = "";
   int         x_end       = 0;
   int         y_end       = 0;
   /*---(initialize)---------------------*/
   strlcpy (unit_answer, "REVERSE unit     : unknownn request", 100);
   /*---(core data)----------------------*/
   if      (strncmp (a_question, "load"      , 20)  == 0) {
      snprintf (t, 80, "%s", s_load);
      if (strlen (s_load) > 80)  sprintf (s, "[%s>", t);
      else                      sprintf (s, "[%s]", t);
      snprintf (unit_answer, LEN_STR, "REVERSE load %3d : %s", s_count, s);
   }
   else if (strncmp (a_question, "box"       , 20)  == 0) {
      snprintf (t, 80, "%s", s_load);
      if (strlen (s_load) > 80)  sprintf (s, "[%s>", t);
      else                      sprintf (s, "[%s]", t);
      snprintf (unit_answer, LEN_STR, "REVERSE box  %3d : %s", s_count, s);
   }
   /*---(complete)-----------------------*/
   return unit_answer;
}



