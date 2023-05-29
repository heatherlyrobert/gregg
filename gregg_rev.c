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
#define    SCALING     1.3

char
REV_load__show_tail  (void)
{
   int         x_len       =    0;
   x_len = strlen (g_fake.load);
   DEBUG_OUTP   yLOG_info    ("tail"      , g_fake.load + x_len - 50);
   return 0;
}

char          /*----: list all points of a particular type -------------------*/
REV_load_report    (void)
{
   int         i;
   char        t           [LEN_MASS] = "";
   char       *p           = NULL;
   char       *s           = NULL;
   /*---(header)-------------------------*/
   printf ("load points inventory (%d)\n", g_fake.count);
   strlcpy (t, g_fake.load, LEN_MASS);
   p = strtok_r (t, ";", &s);
   if (p == NULL) {
      printf ("   zero points\n");
   }
   /*---(points)-------------------------*/
   for (i = 0; i < g_fake.count; ++i) {
      if      (p [0] == 'Õ')  printf ("%3d %c %s\n", i, p[0], p + 1);
      else if (p [0] == 'Ô')  printf ("%3d %c %s\n", i, p[0], p + 1);
      else                    printf ("%3d · %s\n", i, p);
      p = strtok_r (NULL, ";", &s);
   }
   /*---(complete)-----------------------*/
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
   g_fake.n      = 0;
   g_fake.type       = '-';
   g_fake.rot        = 0.0;
   g_fake.beg        = 0.0;
   g_fake.arc        = 0.0;
   g_fake.xradius    = 0.0;
   g_fake.yradius    = 0.0;
   /*---(defense)------------------------*/
   --rce;  if (a_ltr == NULL)  return rce;
   /*---(prepare)------------------------*/
   strlcpy (g_fake.label, a_ltr, LEN_TERSE);
   strlcpy (x_name, a_ltr, LEN_TERSE);
   if (x_name [0] == 'e') {
      x_name [0] = 'a';
      x_flip     = 'y';
   }
   /*---(find letter)--------------------*/
   n  = REVERSE_find_letter (x_name, LTRS_ALL);
   --rce;  if (n <= 0)         return rce;
   /*---(save values)--------------------*/
   g_fake.n      = n;
   g_fake.type       = g_loc [n].type;
   g_fake.rot        = g_loc [n].r_ellipse;
   g_fake.beg        = g_loc [n].b_arc;
   g_fake.arc        = g_loc [n].l_arc;
   g_fake.xradius    = g_loc [n].x_ellipse;
   g_fake.yradius    = g_loc [n].y_ellipse;
   /*---(adjust for ae)------------------*/
   if (x_flip == 'y') {
      if      (g_fake.xradius <=  8)  g_fake.xradius = 3;
      else if (g_fake.xradius <= 11)  g_fake.xradius = 4;
      else                       g_fake.xradius = 5;
      if      (g_fake.yradius <=  8)  g_fake.yradius = 3;
      else if (g_fake.yradius <= 11)  g_fake.yradius = 4;
      else                       g_fake.yradius = 5;
   }
   /*---(scale)--------------------------*/
   g_fake.xradius   *= SCALING;
   g_fake.yradius   *= SCALING;
   /*---(complete)-----------------------*/
   return 0;
}

char
REVERSE_begin           (int x, int y)
{
   FAKE_start   ();
   return 0;
}

char
REVERSE_end             (void)
{
   /*> FAKE_lift     (g_fake.sx  , g_fake.sy  );                                                <*/
   /*> FAKE_done    ();                                                           <*/
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
   --rce;  if (g_fake.type != SHAPE_LINE)    return rce;
   /*---(header)-------------------------*/
   DEBUG_OUTP   yLOG_enter   (__FUNCTION__);
   /*---(prepare)------------------------*/
   if (a_type == SHAPE_LOAD)  x_sizer = SCR2RAW;
   d     = g_fake.rot;
   r     = d * DEG2RAD;
   l     = g_fake.arc * x_sizer;
   x_sin = sin (r);
   x_cos = cos (r);
   /*---(rationalize)--------------------*/
   a_skip *= 2;
   if (a_skip <=  0)          a_skip = 1;
   if (a_skip >= 25)          a_skip = 25;
   DEBUG_OUTP   yLOG_complex ("basics"    , "%2d# %-5.5s  %6.1fd, %8.3fr, %4.1fl, %2ds", g_fake.n, g_fake.label, d, r, l, a_skip);
   /*---(create points)------------------*/
   CREATE_head (0, a_type);
   for (i = 0; i <= l; i += a_skip) {
      x = g_fake.sx   + (i * x_cos);
      y = g_fake.sy   + (i * x_sin);
      CREATE_single     (0, a_type, x, y);
   }
   CREATE_tail (0, a_type, x, y);
   /*---(save)---------------------------*/
   g_fake.sx   = x;
   g_fake.sy   = y;
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
   --rce;  if (g_fake.type != SHAPE_CIRCLE) {
      DEBUG_OUTP   yLOG_exitr   (__FUNCTION__, rce);
      return rce;
   }
   /*---(tables)-------------------------*/
   if (a_type == SHAPE_LOAD)  x_sizer = SCR2RAW;
   l        = g_fake.xradius * x_sizer;
   x_rot    = g_fake.rot * DEG2RAD;
   /*---(determine adjustment)-----------*/
   x_adj  = l * cos (x_rot);
   y_adj  = l * sin (x_rot);
   /*---(rationalize)--------------------*/
   if (g_fake.xradius < 6) a_skip *= 2;
   if (a_skip <=  0)  a_skip = 1;
   if (a_skip >= 25)  a_skip = 25;
   DEBUG_OUTP   yLOG_complex ("basics"    , "%2d# %-5.5s  %5.1fl %5.1fr %5.1fxa %5.1fya %2ds", g_fake.n, g_fake.label, l, x_rot, x_adj, y_adj, a_skip);
   /*---(start)--------------------------*/
   CREATE_head (0, a_type);
   /*---(create points)------------------*/
   for (i = 0; i <= 360; i += a_skip) {
      r     = i * DEG2RAD;
      x     = g_fake.sx   + (l * cos (r)) - x_adj;
      y     = g_fake.sy   + (l * sin (r)) - y_adj;
      CREATE_single     (0, a_type, x, y);
   }
   /*---(save)---------------------------*/
   CREATE_tail (0, a_type, x, y);
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
   --rce;  if (g_fake.type != SHAPE_ELLIPSE) return rce;
   /*---(header)-------------------------*/
   DEBUG_OUTP   yLOG_enter   (__FUNCTION__);
   /*---(tables)-------------------------*/
   if (a_type == SHAPE_LOAD)  x_sizer = SCR2RAW;
   x_len  = g_fake.xradius * x_sizer;
   y_len  = g_fake.yradius * x_sizer;
   x_beg  = g_fake.beg;
   x_arc  = g_fake.arc;
   x_dots = 10;
   /*---(rationalize)--------------------*/
   if (g_fake.xradius < 6) a_skip *= 4;
   if (a_skip <=  0)  a_skip = 1;
   if (a_skip >= 25)  a_skip = 25;
   DEBUG_OUTP   yLOG_complex ("basics"    , "%2d# %-5.5s  %5.1fxl %5.1dyl %5.1fb %5.1fa  %2ds", g_fake.n, g_fake.label, x_len, y_len, x_beg, x_arc, a_skip);
   /*---(angles)-------------------------*/
   a      = x_beg * DEG2RAD;
   a_sin  = sin(a);
   a_cos  = cos(a);
   b      = g_fake.rot * DEG2RAD;
   b_sin  = sin (b);
   b_cos  = cos (b);
   /*---(determine adjustment)-----------*/
   x_adj  = (x_len * a_cos * b_cos) - (y_len * a_sin * b_sin);
   y_adj  = (x_len * a_cos * b_sin) + (y_len * a_sin * b_cos);
   /*---(draw dotted outline)------------*/
   DEBUG_OUTP   yLOG_note    ("before dots");
   if (a_type == SHAPE_SAMPLE)   CREATE_draw_reset (0, a_type);
   if (a_type == SHAPE_SAMPLE) {
      CREATE_dots_begin ();
      for (i = 0; i <= 360; i += x_dots) {
         a     = i * DEG2RAD;
         a_sin = sin (a);
         a_cos = cos (a);
         x     = g_fake.sx   + (x_len * a_cos * b_cos) - (y_len * a_sin * b_sin) - x_adj;
         y     = g_fake.sy   + (x_len * a_cos * b_sin) + (y_len * a_sin * b_cos) - y_adj;
         CREATE_dots_point (x, y);
      }
      CREATE_dots_end   ();
   }
   DEBUG_OUTP   yLOG_note    ("after dots");
   /*---(create points)------------------*/
   if (a_type != SHAPE_LOAD)     CREATE_draw_begin ();
   if (x_arc >= 0) {
      DEBUG_OUTP   yLOG_note    ("draw positive");
      for (i = x_beg; i <= x_beg + x_arc; i += a_skip) {
         a     = i * DEG2RAD;
         a_sin = sin (a);
         a_cos = cos (a);
         x     = g_fake.sx   + (x_len * a_cos * b_cos) - (y_len * a_sin * b_sin) - x_adj;
         y     = g_fake.sy   + (x_len * a_cos * b_sin) + (y_len * a_sin * b_cos) - y_adj;
         CREATE_single     (0, a_type, x, y);
         if (i == x_beg) {  x_1st = x; y_1st = y; }
      }
   } else {
      DEBUG_OUTP   yLOG_note    ("draw negative");
      for (i = x_beg; i >= x_beg + x_arc; i -= a_skip) {
         a     = i * DEG2RAD;
         a_sin = sin (a);
         a_cos = cos (a);
         x     = g_fake.sx   + (x_len * a_cos * b_cos) - (y_len * a_sin * b_sin) - x_adj;
         y     = g_fake.sy   + (x_len * a_cos * b_sin) + (y_len * a_sin * b_cos) - y_adj;
         CREATE_single     (0, a_type, x, y);
         if (i == x_beg) {  x_1st = x; y_1st = y; }
      }
   }
   DEBUG_OUTP   yLOG_note    ("after draw");
   /*---(save)---------------------------*/
   CREATE_tail (0, a_type, x, y);
   /*> if (a_type == SHAPE_SAMPLE)   CREATE_draw_wrap  (x, y);                           <*/
   g_fake.sx   = x;
   g_fake.sy   = y;
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
   --rce;  if (g_fake.type != SHAPE_TEARDROP) {
      DEBUG_OUTP   yLOG_exitr   (__FUNCTION__, rce);
      return rce;
   }
   /*---(table values)-------------------*/
   if (a_type == SHAPE_LOAD)  x_sizer = SCR2RAW;
   x_len  = g_fake.xradius * x_sizer * 1.8;
   y_len  = g_fake.yradius * x_sizer * 1.8;
   x_rot  = g_fake.rot;
   x_arc  = g_fake.arc;
   /*---(determine adjustment)-----------*/
   /*> x_adj  = l * cos (x_rot * DEG2RAD);                                            <* 
    *> y_adj  = l * sin (x_rot * DEG2RAD);                                            <*/
   /*---(rationalize)--------------------*/
   if (g_fake.xradius < 6) a_skip *= 2;
   if (a_skip <=  0)  a_skip = 1;
   if (a_skip >= 25)  a_skip = 25;
   DEBUG_OUTP   yLOG_complex ("basics"    , "%2d# %-5.5s  %5.1fxl  %5.1fyl %5.1fr", g_fake.n, g_fake.label, x_len, y_len, x_rot);
   /*---(start drawing)------------------*/
   CREATE_head (0, a_type);
   CREATE_single     (0, a_type, g_fake.sx  , g_fake.sy  );
   /*---(draw points)--------------------*/
   if (x_arc >= 0) {
      for (s = -1.915;  s <= 1.915; s += 0.01) {
         a    = x_len * ((2.0 / cosh (s)) - 0.577);
         b    = y_len * (s - 2.0 * tanh (s));
         x    = g_fake.sx   + a * cos (x_rot * DEG2RAD) + b * sin (x_rot * DEG2RAD);
         y    = g_fake.sy   - a * sin (x_rot * DEG2RAD) + b * cos (x_rot * DEG2RAD);
         CREATE_single     (0, a_type, x, y);
         DEBUG_OUTP_M yLOG_complex ("point"     , "%5.1fs, %5.1fa, %5.1fb, %5.1fx, %5.1fy", s, a, b, x, y);
      }
   } else {
      for (s = 1.915;  s >= -1.915; s -= 0.01) {
         a    = x_len * ((2.0 / cosh (s)) - 0.577);
         b    = y_len * (s - 2.0 * tanh (s));
         x    = g_fake.sx   + a * cos (x_rot * DEG2RAD) + b * sin (x_rot * DEG2RAD);
         y    = g_fake.sy   - a * sin (x_rot * DEG2RAD) + b * cos (x_rot * DEG2RAD);
         CREATE_single     (0, a_type, x, y);
         DEBUG_OUTP_M yLOG_complex ("point"     , "%5.1fs, %5.1fa, %5.1fb, %5.1fx, %5.1fy", s, a, b, x, y);
      }
   }
   /*---(end drawing)--------------------*/
   /*> CREATE_single     (0, a_type, g_fake.sx  , g_fake.sy  );                       <*/
   CREATE_tail (0, a_type, x, y);
   /*> CREATE_draw_end   (g_fake.sx  , g_fake.sy  );                                               <* 
    *> if (a_type == SHAPE_SAMPLE)   CREATE_draw_wrap  (g_fake.sx  , g_fake.sy  );                 <*/
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
   /*---(header)-------------------------*/
   DEBUG_OUTP   yLOG_enter   (__FUNCTION__);
   /*---(defense)------------------------*/
   --rce;  if (g_fake.type != SHAPE_DOT   ) {
      DEBUG_OUTP   yLOG_exitr   (__FUNCTION__, rce);
      return rce;
   }
   /*---(tables)-------------------------*/
   if (a_type == SHAPE_LOAD)  x_sizer = SCR2RAW;
   l  = g_fake.xradius * x_sizer * 0.5;
   /*---(draw)---------------------------*/
   CREATE_head (0, a_type);
   for (i = 0; i <= 360; i += a_skip) {
      r  = i * DEG2RAD;
      x = g_fake.sx   + l * sin (r);
      y = g_fake.sy   + l * cos (r);
      CREATE_single     (0, a_type, x, y);
   }
   CREATE_tail (0, a_type, x, y);
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
   --rce;  if (g_fake.type != SHAPE_SPACE)   return rce;
   /*---(header)-------------------------*/
   DEBUG_OUTP   yLOG_enter   (__FUNCTION__);
   /*---(tables)-------------------------*/
   if (a_type == SHAPE_LOAD)  x_sizer = SCR2RAW;
   x  = g_fake.xradius * x_sizer;
   y  = g_fake.yradius * x_sizer;
   CREATE_head (0, a_type);
   CREATE_tail (0, a_type, x, y);
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
      CREATE_single     (0, a_type, x, y);
   }
   /*---(save)---------------------------*/
   g_fake.sx   = x;
   g_fake.sy   = y;
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
   short       i = 0, j = 0;
   /*---(header)-------------------------*/
   DEBUG_OUTP   yLOG_enter   (__FUNCTION__);
   rc = REVERSE_make_current (a_ltr);
   DEBUG_OUTP   yLOG_value   ("current"   , rc);
   --rce;  if (rc < 0) {
      DEBUG_OUTP   yLOG_exitr   (__FUNCTION__, rce);
      return rce;
   }
   switch (g_fake.type) {
      /*> case SHAPE_LINE     :  REVERSE_line     (a_type, a_skip);  break;              <*/
   case SHAPE_LINE     :
      CREATE_line (0, SHAPE_DRAW, g_fake.arc, g_fake.rot, &(g_fake.sx), &(g_fake.sy));
      break;
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
   g_fake.sx   +=  0.0 * a_sizer * SCALING;
   g_fake.sy   -= 10.0 * a_sizer * SCALING;
   return 0;
}

char
REVERSE_make_next       (void)
{
   /*---(header)-------------------------*/
   DEBUG_OUTP   yLOG_senter  (__FUNCTION__);
   DEBUG_OUTP   yLOG_svalue  ("x", g_fake.sx  );
   DEBUG_OUTP   yLOG_svalue  ("y", g_fake.sy  );
   DEBUG_OUTP   yLOG_svalue  ("p", g_fake.push);
   DEBUG_OUTP   yLOG_svalue  ("s", SCALING);
   g_fake.sx   = g_fake.push + 10.0 * SCALING;
   DEBUG_OUTP   yLOG_svalue  ("x", g_fake.sx  );
   if (g_fake.sx   > 750)  {
      DEBUG_OUTP   yLOG_snote   ("hit right");
      g_fake.sx    = 30.0;
      g_fake.sy   -= 60.0 * SCALING;
      DEBUG_OUTP   yLOG_svalue  ("x", g_fake.sx  );
      DEBUG_OUTP   yLOG_svalue  ("y", g_fake.sy  );
   }
   /*---(complete)-----------------------*/
   DEBUG_OUTP   yLOG_sexit   (__FUNCTION__);
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
   DEBUG_OUTP   yLOG_complex ("big_pos "  , "%5.1fsx, %5.1fsy", g_fake.sx  , g_fake.sy  );
   --rce;  if (a_type == SHAPE_DRAW)     CREATE_draw_init  ();
   else if    (a_type == SHAPE_LOAD)     FAKE_start  ();
   else   {
      DEBUG_OUTP   yLOG_exitr   (__FUNCTION__, rce);
      return rce;
   }
   if (a_type == SHAPE_LOAD)  x_sizer = SCR2RAW;
   /*---(start parsing)------------------*/
   DEBUG_OUTP   yLOG_complex ("big_pos "  , "%5.1fsx, %5.1fsy", g_fake.sx  , g_fake.sy  );
   strlcpy  (t, a_outline, LEN_HUND);
   strltrim (t, ySTR_BOTH, LEN_HUND);
   p = strtok_r (t, q, &r);
   if (p != NULL && p [0] == 'a')  g_fake.sx   += 20;
   if (p != NULL && p [0] == 'e')  g_fake.sx   += 10;
   /*> REVERSE_make_letter  (">", a_type, a_skip);                           <*/
   /*---(walk)---------------------------*/
   while (p != NULL) {
      DEBUG_OUTP   yLOG_info    ("p"         , p);
      x_last = p;
      if (strcmp (">", p) == 0) {
         DEBUG_OUTP   yLOG_note    ("found a multipart indicator");
         if      (a_type == SHAPE_LOAD) {
            FAKE_lift   (g_fake.sx  , g_fake.sy  );
            REVERSE_make_offset (x_sizer);
            FAKE_touch  (g_fake.sx  , g_fake.sy  );
         } else {
            REVERSE_make_offset (x_sizer);
         }
      } else {
         DEBUG_OUTP   yLOG_note    ("normal letter");
         /*> REVERSE_make_letter  ("dot", a_type, a_skip);                            <*/
         REVERSE_make_letter  (p, a_type, a_skip);
         /*> switch (g_fake.type) {                                                        <* 
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
   if (x_last != NULL && x_last [0] == 'a')  g_fake.sx   += 20;
   if (x_last != NULL && x_last [0] == 'e')  g_fake.sx   += 10;
   /*---(wrap-up)------------------------*/
   g_fake.n = 0;
   if      (a_type == SHAPE_DRAW)     CREATE_draw_wrap  (0, a_type, g_fake.sx  , g_fake.sy  );
   else                               FAKE_done  (g_fake.sx  , g_fake.sy  );
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
   if (a_reset == 'y')  CREATE_draw_reset  (0, a_type);
   strlcpy  (t, a_text, LEN_HUGE);
   p = strtok_r (t, q, &r);
   /*---(walk)---------------------------*/
   while (p != NULL) {
      x_len = strlen (p);
      DEBUG_OUTP   yLOG_complex ("parsed"    , "%2d[%s]", x_len, p);
      if (x_len == 1 && p [0] == (uchar) '¦')   g_fake.sx   = 9999;
      else                                      REVERSE_gregg_word (p, a_type, a_skip);
      g_fake.sx   = g_fake.push + 10.0 * SCALING;
      DEBUG_OUTP   yLOG_value   ("g_fake.rig"     , g_fake.rig);
      DEBUG_OUTP   yLOG_value   ("g_fake.push"    , g_fake.push);
      /*---(prepare next)----------------*/
      REVERSE_make_next    ();
      DEBUG_OUTP   yLOG_complex ("big_pos "  , "%5.1fsx, %5.1fsy", g_fake.sx  , g_fake.sy  );
      p = strtok_r (NULL, q, &r);
   }
   /*---(done)---------------------------*/
   REV_load_raw ();
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
   tWORD     *x_word      = NULL;
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
   /*> rc = BTREE_by_english (&x_word, a_word);                                       <*/
   DEBUG_OUTP   yLOG_value   ("btree"     , rc);
   DEBUG_OUTP   yLOG_point   ("x_word"    , x_word);
   --rce;  if (rc < 0 || x_word == NULL) {
      DEBUG_OUTP   yLOG_exitr   (__FUNCTION__, rce);
      return rce;
   }
   strlcpy (my.word , a_word         , LEN_HUND);
   strlcpy (my.gregg, x_word->w_gregg, LEN_HUND);
   /*---(translate)----------------------*/
   rc = WORDS_drawn_show     (x_word->w_drawn, my.shown);
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
   if (a_reset == 'y')  CREATE_draw_reset  (0, a_type);
   strlcpy  (t, a_text, LEN_HUGE);
   p = strtok_r (t, q, &r);
   /*---(walk)---------------------------*/
   while (p != NULL) {
      x_len = strlen (p);
      DEBUG_OUTP   yLOG_complex ("parsed"    , "%2d[%s]", x_len, p);
      if (x_len == 1 && p [0] == (uchar) '¦')   g_fake.sx   = 9999;
      else                                      REVERSE_english_word (p, a_type, a_skip);
      g_fake.sx   = g_fake.push + 10.0 * SCALING;
      DEBUG_OUTP   yLOG_value   ("g_fake.rig"     , g_fake.rig);
      DEBUG_OUTP   yLOG_value   ("g_fake.push"    , g_fake.push);
      /*---(prepare next)----------------*/
      REVERSE_make_next    ();
      DEBUG_OUTP   yLOG_complex ("big_pos "  , "%5.1fsx, %5.1fsy", g_fake.sx  , g_fake.sy  );
      p = strtok_r (NULL, q, &r);
   }
   /*---(done)---------------------------*/
   REV_load_report ();
   REV_load_raw ();
   /*---(complete)-----------------------*/
   DEBUG_OUTP   yLOG_exit    (__FUNCTION__);
   return 0;
}



/*============================--------------------============================*/
/*===----                         page handling                        ----===*/
/*============================--------------------============================*/
static void o___PAGES_____________________o (void) {;}

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
   CREATE_draw_reset  (0, SHAPE_DRAW);
   g_fake.sx   =  30.0;
   g_fake.sy   = -50.0;
   /*---(create texture)-----------------*/
   yCOLOR_opengl_clear (YCOLOR_BAS, YCOLOR_MAX);
   rc = yGLTEX_new (&my.t_tex, &my.t_fbo, &my.t_depth, 850, 1100);
   rc = yGLTEX_draw  (my.t_tex, my.t_fbo, YGLTEX_TOPLEF, 850, 1100, 1.0);
   /*> glColor4f (1.00f, 1.00f, 1.00f, 1.0f);                                         <*/
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
   /*> rc = yGLTEX_tex2png      ("/tmp/gregg.png", 850, 1100);                        <*/
   /*---(close texture)------------------*/
   /*> rc = yGLTEX_done         (my.t_tex);                                           <*/
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
REV__unit        (char *a_question, int a_num)
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
      snprintf (t, 80, "%s", g_fake.load);
      if (strlen (g_fake.load) > 80)  sprintf (s, "å%s>", t);
      else                      sprintf (s, "å%sæ", t);
      snprintf (unit_answer, LEN_FULL, "REVERSE load %3d : %s", g_fake.count, s);
   }
   else if (strncmp (a_question, "box"       , 20)  == 0) {
      snprintf (t, 80, "%s", g_fake.load);
      if (strlen (g_fake.load) > 80)  sprintf (s, "[%s>", t);
      else                      sprintf (s, "[%s]", t);
      snprintf (unit_answer, LEN_FULL, "REVERSE box  %3d : %s", g_fake.count, s);
   }
   /*---(complete)-----------------------*/
   return unit_answer;
}



