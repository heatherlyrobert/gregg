/*=================================(beg-code)=================================*/
#include "gregg.h"



static     short s_count     = 0;



/*============================--------------------============================*/
/*===----                         sample dots                          ----===*/
/*============================--------------------============================*/
static void o___SAMPLE_DOTS____________o (void) {;}

char
CREATE_dots_begin       (void)
{
   glLineWidth (1.0);
   glPointSize (1.0);
   glBegin (GL_POINTS);
   return 0;
}

char
CREATE_dots_point       (float x, float y)
{
   glVertex3f ( x, y, 5.0);
   return 0;
}

char
CREATE_dots_end         (void)
{
   glEnd ();
   glLineWidth (0.8);
   return 0;
}



/*============================--------------------============================*/
/*===----                    loading for raw points                    ----===*/
/*============================--------------------============================*/
static void o___DRAW___________________o (void) {;}

char
CREATE_draw_init        (void)
{
   DEBUG_OUTP   yLOG_senter  (__FUNCTION__);
   s_count        = 0;
   /*---(DEPRECATED)---------------------*/
   g_fake.c       = 0;
   g_fake.lef     = 0;
   g_fake.rig     = 0;
   g_fake.top     = 0;
   g_fake.bot     = 0;
   g_fake.push    = 0;
   /*---(complete)-----------------------*/
   DEBUG_OUTP   yLOG_sexit   (__FUNCTION__);
   return 0;
}

char
CREATE_draw_reset       (short n, char a_act)
{
   DEBUG_OUTP   yLOG_senter  (__FUNCTION__);
   DEBUG_OUTP   yLOG_sint    (n);
   /*---(data load)----------------------*/
   if (a_act == SHAPE_DATA) {
      /*---(trend)-----------------------*/
      g_loc [n].x_rig  = 0.0;
      g_loc [n].x_lef  = 0.0;
      g_loc [n].y_top  = 0.0;
      g_loc [n].y_bot  = 0.0;
      /*---(bonnds)----------------------*/
      g_loc [n].x_end  = 0.0;
      g_loc [n].y_end  = 0.0;
      g_loc [n].deg    = 0.0;
      g_loc [n].xy_len = 0.0;
      /*---(count)-----------------------*/
      g_loc [n].count  = 0;
      DEBUG_OUTP   yLOG_sint    (g_loc [n].count);
   }
   s_count        = 0;
   /*---(DEPRECATED)---------------------*/
   g_fake.c       = 0;
   g_fake.sx      = 0;
   g_fake.sy      = 0;
   g_fake.lef     = 0;
   g_fake.rig     = 0;
   g_fake.top     = 0;
   g_fake.bot     = 0;
   /*---(complete)-----------------------*/
   DEBUG_OUTP   yLOG_sexit   (__FUNCTION__);
   return 0;
}

char
CREATE_draw_begin       (void)
{
   DEBUG_OUTP   yLOG_senter  (__FUNCTION__);
   /*---(start drawing)------------------*/
   glLineWidth (POINT_MED);
   glBegin (GL_LINE_STRIP);
   /*---(complete)-----------------------*/
   DEBUG_OUTP   yLOG_sexit   (__FUNCTION__);
   return 0;
}

char
CREATE_draw_point       (short n, char a_act, float x, float y)
{
   DEBUG_OUTP   yLOG_senter  (__FUNCTION__);
   /*---(draw)---------------------------*/
   if (a_act == SHAPE_CHECK) {
      glColor4f ((g_loc [n].count - s_count) / g_loc [n].count, 0.0, 0.0, 1.0);
   }
   if (a_act != SHAPE_DATA) {
      glVertex3f ( x, y, 5.0);
   }
   /*---(bounds)-------------------------*/
   if (a_act == SHAPE_DATA) {
      if (x < g_loc [n].x_lef)   g_loc [n].x_lef  = x;
      if (x > g_loc [n].x_rig)   g_loc [n].x_rig  = x;
      if (y < g_loc [n].y_bot)   g_loc [n].y_bot  = y;
      if (y > g_loc [n].y_top)   g_loc [n].y_top  = y;
      if (x > g_fake.push)       g_fake.push      = x;
      DEBUG_OUTP   yLOG_complex ("bounds"    , "%-3d, %5.1fl, %5.1fr, %5.1ft, %5.1fb", s_count, g_loc [n].x_lef, g_loc [n].x_rig, g_loc [n].y_top, g_loc [n].y_bot);
      g_loc [n].count = s_count + 1;
   }
   ++s_count;
   /*---(complete)-----------------------*/
   DEBUG_OUTP   yLOG_sexit   (__FUNCTION__);
   return 0;
}

char
CREATE_draw_end         (void)
{
   DEBUG_OUTP   yLOG_senter  (__FUNCTION__);
   /*---(stop drawing)-------------------*/
   glEnd();
   glLineWidth (POINT_SML);
   /*---(complete)-----------------------*/
   DEBUG_OUTP   yLOG_sexit   (__FUNCTION__);
   return 0;
}

char
CREATE_draw_wrap        (short n, char a_act, float x, float y)
{
   float       xd, yd, r;
   DEBUG_OUTP   yLOG_senter  (__FUNCTION__);
   /*---(get overall calcs)--------------*/
   o.tmp [0].p_bas  = 0;
   o.tmp [0].x_raw  = 0;
   o.tmp [0].y_raw  = 0;
   o.tmp [1].p_bas  = 1;
   o.tmp [1].x_raw  = x * 100.0;
   o.tmp [1].y_raw  = y * 100.0;
   POINT_calc (POINTS_TMP, o.tmp + 1, 'n');
   /*---(trending)-----------------------*/
   if (a_act == SHAPE_DATA) {
      g_loc [n].x_end  = x;
      g_loc [n].y_end  = y;
      r   = atan2f (y, x);
      g_loc [n].deg    = r * RAD2DEG;
      g_loc [n].xy_len = sqrt((x * x) + (y * y));
      DEBUG_OUTP   yLOG_complex ("trend"     , "%5.1fd, %5.1fl, %5.1fx, %5.1fy", g_loc [n].deg, g_loc [n].xy_len, g_loc [n].x_end, g_loc [n].y_end);
   }
   /*---(complete)-----------------------*/
   DEBUG_OUTP   yLOG_sexit   (__FUNCTION__);
   return 0;
}



/*============================--------------------============================*/
/*===----                         helper functions                     ----===*/
/*============================--------------------============================*/
static void o___COMBO__________________o (void) {;}

char
CREATE_head             (short n, char a_act)
{
   DEBUG_OUTP   yLOG_enter   (__FUNCTION__);
   DEBUG_OUTP   yLOG_complex ("args"      , "%d, %c", n, a_act);
   switch (a_act) {
   case SHAPE_SAMPLE : case SHAPE_CHECK  : case SHAPE_TRAIN  :
      DEBUG_OUTP   yLOG_note    ("SAMPLE/CHECK/TRAIL, reset and begin");
      CREATE_draw_reset  (n, a_act);
      CREATE_draw_begin  ();
      break;
   case SHAPE_DATA   :
      DEBUG_OUTP   yLOG_note    ("DATA, just reset");
      CREATE_draw_reset  (n, a_act);
      break;
   case SHAPE_LOAD   :
      DEBUG_OUTP   yLOG_note    ("LOAD, nothing to do");
      break;
   case SHAPE_DRAW   :
      DEBUG_OUTP   yLOG_note    ("DRAW, begin");
      CREATE_draw_begin  ();
      break;
   }
   DEBUG_OUTP   yLOG_exit    (__FUNCTION__);
   return 0;
}

char
CREATE_single           (short n, char a_act, float x, float y)
{
   DEBUG_OUTP   yLOG_enter   (__FUNCTION__);
   DEBUG_OUTP   yLOG_complex ("args"      , "%d, %c, %6.1f, %6.1f", n, a_act, x, y);
   switch (a_act) {
   case SHAPE_SAMPLE : case SHAPE_CHECK  : case SHAPE_DRAW   : case SHAPE_TRAIN  : case SHAPE_DATA   :
      DEBUG_OUTP   yLOG_note    ("SAMPLE/CHECK/DRAW/TRAIN/DATA, draw");
      CREATE_draw_point  (n, a_act, x, y);
      break;
   case SHAPE_LOAD   :
      DEBUG_OUTP   yLOG_note    ("LOAD, load");
      FAKE_point  (x, y);
      break;
   }
   DEBUG_OUTP   yLOG_exit    (__FUNCTION__);
   return 0;
}

char
CREATE_tail             (short n, char a_act, float x, float y)
{
   DEBUG_OUTP   yLOG_enter   (__FUNCTION__);
   DEBUG_OUTP   yLOG_complex ("args"      , "%d, %c, %6.1f, %6.1f", n, a_act, x, y);
   switch (a_act) {
   case SHAPE_SAMPLE : case SHAPE_CHECK  : case SHAPE_TRAIN  :
      DEBUG_OUTP   yLOG_note    ("SAMPLE/CHECK/TRAIL, draw and wrap");
      CREATE_draw_point  (n, a_act, x, y);
      CREATE_draw_end    ();
      CREATE_draw_wrap   (n, a_act, x, y);
      break;
   case SHAPE_DATA   :
      DEBUG_OUTP   yLOG_note    ("DATA, just reset");
      CREATE_draw_point  (n, a_act, x, y);
      CREATE_draw_wrap   (n, a_act, x, y);
      break;
   case SHAPE_LOAD   :
      DEBUG_OUTP   yLOG_note    ("LOAD, nothing to do");
      break;
   case SHAPE_DRAW   :
      DEBUG_OUTP   yLOG_note    ("DRAW, draw");
      CREATE_draw_point  (n, a_act, x, y);
      CREATE_draw_end    ();
      break;
   }
   DEBUG_OUTP   yLOG_exit    (__FUNCTION__);
   return 0;
}



/*============================--------------------============================*/
/*===----                        creating shapes                       ----===*/
/*============================--------------------============================*/
static void o___SHAPES_________________o (void) {;}

char
CREATE_line             (short n, char a_act, short a_rot, short a_len, float *b_xpos, float *b_ypos)
{
   /*---(locals)-----------+-----+-----+-*/
   char        rce         =  -10;
   float       r           =  0.0;
   short       i           =    0;
   float       sx, sy;
   float       x_sin, x_cos;
   float       cx, cy;
   /*---(header)-------------------------*/
   DEBUG_OUTP   yLOG_enter   (__FUNCTION__);
   /*---(defense)------------------------*/
   DEBUG_OUTP   yLOG_point   ("b_xpos"    , b_xpos);
   --rce;  if (b_xpos == NULL) {
      DEBUG_OUTP   yLOG_exitr   (__FUNCTION__, rce);
      return rce;
   }
   sx = *b_xpos;
   DEBUG_OUTP   yLOG_value   ("sx"        , sx);
   DEBUG_OUTP   yLOG_point   ("b_ypos"    , b_ypos);
   --rce;  if (b_ypos == NULL) {
      DEBUG_OUTP   yLOG_exitr   (__FUNCTION__, rce);
      return rce;
   }
   sy = *b_ypos;
   DEBUG_OUTP   yLOG_value   ("sy"        , sy);
   /*---(prepare)------------------------*/
   r     = a_rot * DEG2RAD;
   x_sin = sin (r);
   x_cos = cos (r);
   /*---(rationalize)--------------------*/
   DEBUG_OUTP   yLOG_complex ("line"      , "%6.1fd, %8.3fr, %4.1fl", a_rot, r, a_len);
   DEBUG_OUTP   yLOG_complex ("trig"      , "%8.3fsin, %8.3fcos", x_sin, x_cos);
   /*---(create points)------------------*/
   CREATE_head (n, a_act);
   for (i = 0; i <= a_len; ++i) {
      cx = sx + (i * x_cos);
      cy = sy + (i * x_sin);
      DEBUG_OUTP   yLOG_complex ("curr"      , "%8.3fx, %8.3fy", cx, cy);
      CREATE_single (n, a_act, cx, cy);
   }
   CREATE_tail (n, a_act, cx, cy);
   /*---(save)---------------------------*/
   *b_xpos = cx;
   *b_ypos = cy;
   /*---(complete)-----------------------*/
   DEBUG_OUTP   yLOG_exit    (__FUNCTION__);
   return 0;
}

char
CREATE_circle           (short n, char a_act, short a_radius, short a_rot, float *b_xpos, float *b_ypos)
{
   /*---(locals)-----------+-----+-----+-*/
   char        rce         =  -10;
   int         i           =    0;
   float       sx, sy;
   float       r;
   float       x_adj, y_adj;
   float       cx, cy;
   short       x_sizer     =    1;
   /*---(header)-------------------------*/
   DEBUG_OUTP   yLOG_enter   (__FUNCTION__);
   /*---(defense)------------------------*/
   DEBUG_OUTP   yLOG_point   ("b_xpos"    , b_xpos);
   --rce;  if (b_xpos == NULL) {
      DEBUG_OUTP   yLOG_exitr   (__FUNCTION__, rce);
      return rce;
   }
   sx = *b_xpos;
   DEBUG_OUTP   yLOG_value   ("sx"        , sx);
   DEBUG_OUTP   yLOG_point   ("b_ypos"    , b_ypos);
   --rce;  if (b_ypos == NULL) {
      DEBUG_OUTP   yLOG_exitr   (__FUNCTION__, rce);
      return rce;
   }
   sy = *b_ypos;
   DEBUG_OUTP   yLOG_value   ("sy"        , sy);
   /*---(tables)-------------------------*/
   r        = a_rot * DEG2RAD;
   /*---(determine adjustment)-----------*/
   x_adj  = a_radius * cos (r);
   y_adj  = a_radius * sin (r);
   /*---(rationalize)--------------------*/
   DEBUG_OUTP   yLOG_complex ("basics"    , "%5.1fl %5.1fr %5.1fxa %5.1fya", a_radius, r, x_adj, y_adj);
   /*---(start)--------------------------*/
   CREATE_head (n, a_act);
   /*---(create points)------------------*/
   for (i = 0; i <= 360; ++i) {
      r     = i * DEG2RAD;
      cx     = sx   + (a_radius * cos (r)) - x_adj;
      cy     = sy   + (a_radius * sin (r)) - y_adj;
      DEBUG_OUTP   yLOG_complex ("curr"      , "%8.3fx, %8.3fy", cx, cy);
      CREATE_single     (n, a_act, cx, cy);
   }
   /*---(save)---------------------------*/
   CREATE_tail (n, a_act, cx, cy);
   /*---(complete)-----------------------*/
   DEBUG_OUTP   yLOG_exit    (__FUNCTION__);
   return 0;
}

char
CREATE_ellipse_point    (char a_act, short i, short sx, short sy, float a_xlen, float a_ylen, float a_bsin, float a_bcos, float a_xadj, float a_yadj, float *r_xpos, float *r_ypos)
{
   float       a, x_asin, x_acos;
   float       cx, cy;
   a     = i * DEG2RAD;
   x_asin = sin (a);
   x_acos = cos (a);
   *r_xpos  = sx + (a_xlen * x_acos * a_bcos) - (a_ylen * x_asin * a_bsin) - a_xadj;
   *r_ypos  = sy + (a_xlen * x_acos * a_bsin) + (a_ylen * x_asin * a_bcos) - a_yadj;
   return 0;
}

char
CREATE_ellipse          (short n, char a_act, short a_xradius, short a_yradius, short a_rot, short a_beg, short a_arc, char a_dots, float *b_xpos, float *b_ypos)
{
   /*---(locals)-----------+-----+-----+-*/
   char        rce         =  -10;
   int         i           =    0;
   float       a, a_sin, a_cos;
   float       b, b_sin, b_cos;
   float       sx, sy;
   float       cx, cy;
   float       x_len, y_len;
   float       x_adj, y_adj;
   int         x_sizer     =    1;
   /*---(header)-------------------------*/
   DEBUG_OUTP   yLOG_enter   (__FUNCTION__);
   DEBUG_OUTP   yLOG_complex ("ellipse"   , "%3dxr, %3dyx, %3dr, %3db, %3da", a_xradius, a_yradius, a_rot, a_beg, a_arc);
   /*---(defense)------------------------*/
   DEBUG_OUTP   yLOG_point   ("b_xpos"    , b_xpos);
   --rce;  if (b_xpos == NULL) {
      DEBUG_OUTP   yLOG_exitr   (__FUNCTION__, rce);
      return rce;
   }
   sx = *b_xpos;
   DEBUG_OUTP   yLOG_value   ("sx"        , sx);
   DEBUG_OUTP   yLOG_point   ("b_ypos"    , b_ypos);
   --rce;  if (b_ypos == NULL) {
      DEBUG_OUTP   yLOG_exitr   (__FUNCTION__, rce);
      return rce;
   }
   sy = *b_ypos;
   DEBUG_OUTP   yLOG_value   ("sy"        , sy);
   /*---(tables)-------------------------*/
   if (a_dots <= 0)  a_dots = 1;
   x_len  = a_xradius * x_sizer;
   y_len  = a_yradius * x_sizer;
   /*---(rationalize)--------------------*/
   DEBUG_OUTP   yLOG_complex ("basics"    , "%5.1fxl %5.1fyl %4db %4da  %2dd", x_len, y_len, a_beg, a_arc, a_dots);
   /*---(angles)-------------------------*/
   a      = a_beg * DEG2RAD;
   a_sin  = sin(a);
   a_cos  = cos(a);
   b      = a_rot * DEG2RAD;
   /*> b      = (a_rot + 180) * DEG2RAD;                                              <*/
   b_sin  = sin (b);
   b_cos  = cos (b);
   /*---(determine adjustment)-----------*/
   x_adj  = (x_len * a_cos * b_cos) - (y_len * a_sin * b_sin);
   y_adj  = (x_len * a_cos * b_sin) + (y_len * a_sin * b_cos);
   /*---(draw dotted outline)------------*/
   DEBUG_OUTP   yLOG_note    ("before dots");
   if (a_act == SHAPE_SAMPLE) {
      CREATE_draw_reset (0, a_act);
      CREATE_dots_begin ();
      for (i = 0; i <= 360; i += a_dots) {
         CREATE_ellipse_point (a_act, i, sx, sy, x_len, y_len, b_sin, b_cos, x_adj, y_adj, &cx, &cy);
         CREATE_dots_point (cx, cy);
      }
      CREATE_dots_end   ();
   }
   DEBUG_OUTP   yLOG_note    ("after dots");
   /*---(create points)------------------*/
   CREATE_head (n, a_act);
   if (a_arc >= 0) {
      DEBUG_OUTP   yLOG_note    ("draw positive");
      for (i = a_beg; i <= a_beg + a_arc; ++i) {
         CREATE_ellipse_point (a_act, i, sx, sy, x_len, y_len, b_sin, b_cos, x_adj, y_adj, &cx, &cy);
         CREATE_single     (n, a_act, cx, cy);
      }
   } else {
      DEBUG_OUTP   yLOG_note    ("draw negative");
      for (i = a_beg; i >= a_beg + a_arc; --i) {
         CREATE_ellipse_point (a_act, i, sx, sy, x_len, y_len, b_sin, b_cos, x_adj, y_adj, &cx, &cy);
         CREATE_single     (n, a_act, cx, cy);
      }
   }
   DEBUG_OUTP   yLOG_note    ("after draw");
   /*---(save)---------------------------*/
   CREATE_tail (n, a_act, cx, cy);
   *b_xpos     = cx;
   *b_ypos     = cy;
   /*---(complete)-----------------------*/
   DEBUG_OUTP   yLOG_exit    (__FUNCTION__);
   return 0;
}

char
CREATE_teardrop_point   (char a_act, float s, float sx, float sy, short a_rot, short a_xlen, short a_ylen, float *r_xpos, float *r_ypos)
{
   float       a, b;
   a   = a_xlen * ((2.0 / cosh (s)) - 0.577);
   b   = a_ylen * (s - 2.0 * tanh (s));
   *r_xpos  = sx   + a * cos (a_rot * DEG2RAD) + b * sin (a_rot * DEG2RAD);
   *r_ypos  = sy   - a * sin (a_rot * DEG2RAD) + b * cos (a_rot * DEG2RAD);
   return 0;
}


char
CREATE_teardrop         (short n, char a_act, short a_xradius, short a_yradius, short a_rot, short a_beg, short a_arc, float *b_xpos, float *b_ypos)
{ /* uses the syntractrix of poleni, also called the convict's curve */
   /*---(locals)-----------+-----+-----+-*/
   char        rce         =  -10;
   float       s           =    0;
   float       x_len       =    0;
   float       y_len       =    0;
   float       a, b;
   float       sx, sy;
   float       cx, cy;
   /*---(header)-------------------------*/
   DEBUG_OUTP   yLOG_enter   (__FUNCTION__);
   /*---(defense)------------------------*/
   DEBUG_OUTP   yLOG_point   ("b_xpos"    , b_xpos);
   --rce;  if (b_xpos == NULL) {
      DEBUG_OUTP   yLOG_exitr   (__FUNCTION__, rce);
      return rce;
   }
   sx = *b_xpos;
   DEBUG_OUTP   yLOG_value   ("sx"        , sx);
   DEBUG_OUTP   yLOG_point   ("b_ypos"    , b_ypos);
   --rce;  if (b_ypos == NULL) {
      DEBUG_OUTP   yLOG_exitr   (__FUNCTION__, rce);
      return rce;
   }
   sy = *b_ypos;
   DEBUG_OUTP   yLOG_value   ("sy"        , sy);
   /*---(table values)-------------------*/
   x_len  = a_xradius * 1.8;
   y_len  = a_yradius * 1.8;
   /*---(rationalize)--------------------*/
   DEBUG_OUTP   yLOG_complex ("basics"    , "%5.1fxl  %5.1fyl %5.1fr", x_len, y_len, a_rot);
   /*---(start drawing)------------------*/
   CREATE_head (n, a_act);
   CREATE_single     (n, a_act, sx  , sy  );
   /*---(draw points)--------------------*/
   if (a_arc >= 0) {
      for (s = -1.915;  s <= 1.915; s += 0.01) {
         a    = x_len * ((2.0 / cosh (s)) - 0.577);
         b    = y_len * (s - 2.0 * tanh (s));
         cx    = sx   + a * cos (a_rot * DEG2RAD) + b * sin (a_rot * DEG2RAD);
         cy    = sy   - a * sin (a_rot * DEG2RAD) + b * cos (a_rot * DEG2RAD);
         CREATE_single     (n, a_act, cx, cy);
         DEBUG_OUTP_M yLOG_complex ("point"     , "%5.1fs, %5.1fa, %5.1fb, %5.1fx, %5.1fy", s, a, b, cx, cy);
      }
   } else {
      for (s = 1.915;  s >= -1.915; s -= 0.01) {
         a    = x_len * ((2.0 / cosh (s)) - 0.577);
         b    = y_len * (s - 2.0 * tanh (s));
         cx    = sx   + a * cos (a_rot * DEG2RAD) + b * sin (a_rot * DEG2RAD);
         cy    = sy   - a * sin (a_rot * DEG2RAD) + b * cos (a_rot * DEG2RAD);
         CREATE_single     (n, a_act, cx, cy);
         DEBUG_OUTP_M yLOG_complex ("point"     , "%5.1fs, %5.1fa, %5.1fb, %5.1fx, %5.1fy", s, a, b, cx, cy);
      }
   }
   /*---(save)---------------------------*/
   /*> CREATE_single     (n, a_act, sx, sy);                                          <*/
   CREATE_tail  (n, a_act, cx, cy);
   *b_xpos     = cx;
   *b_ypos     = cy;
   /*---(complete)-----------------------*/
   DEBUG_OUTP   yLOG_exit    (__FUNCTION__);
   return 0;
}

char
CREATE_dot              (short n, char a_act, short a_xradius, float *b_xpos, float *b_ypos)
{
   /*---(locals)-----------+-----+-----+-*/
   char        rce         =  -10;
   int         i           =    0;
   float       r, l;
   float       sx, sy;
   float       cx, cy;
   /*---(header)-------------------------*/
   DEBUG_OUTP   yLOG_enter   (__FUNCTION__);
   /*---(defense)------------------------*/
   DEBUG_OUTP   yLOG_point   ("b_xpos"    , b_xpos);
   --rce;  if (b_xpos == NULL) {
      DEBUG_OUTP   yLOG_exitr   (__FUNCTION__, rce);
      return rce;
   }
   sx = *b_xpos;
   DEBUG_OUTP   yLOG_value   ("sx"        , sx);
   DEBUG_OUTP   yLOG_point   ("b_ypos"    , b_ypos);
   --rce;  if (b_ypos == NULL) {
      DEBUG_OUTP   yLOG_exitr   (__FUNCTION__, rce);
      return rce;
   }
   sy = *b_ypos;
   DEBUG_OUTP   yLOG_value   ("sy"        , sy);
   /*---(tables)-------------------------*/
   /*> l  = a_xradius * 0.5;                                                          <*/
   l = a_xradius;
   /*---(draw)---------------------------*/
   CREATE_head (n, a_act);
   for (i = 0; i <= 360; ++i) {
      r  = i * DEG2RAD;
      cx = sx  + l * sin (r);
      cy = sy  + l * cos (r);
      CREATE_single     (n, a_act, cx, cy);
   }
   /*---(save)---------------------------*/
   CREATE_tail (n, a_act, cx, cy);
   /*---(complete)-----------------------*/
   DEBUG_OUTP   yLOG_exit    (__FUNCTION__);
   return 0;
}

char
CREATE_space            (short n, char a_act, short a_xradius, short a_yradius, float *b_xpos, float *b_ypos)
{
   /*---(locals)-----------+-----+-----+-*/
   char        rce         =  -10;
   float       sx, sy;
   float       cx, cy;
   /*---(header)-------------------------*/
   DEBUG_OUTP   yLOG_enter   (__FUNCTION__);
   /*---(defense)------------------------*/
   DEBUG_OUTP   yLOG_point   ("b_xpos"    , b_xpos);
   --rce;  if (b_xpos == NULL) {
      DEBUG_OUTP   yLOG_exitr   (__FUNCTION__, rce);
      return rce;
   }
   sx = *b_xpos;
   DEBUG_OUTP   yLOG_value   ("sx"        , sx);
   DEBUG_OUTP   yLOG_point   ("b_ypos"    , b_ypos);
   --rce;  if (b_ypos == NULL) {
      DEBUG_OUTP   yLOG_exitr   (__FUNCTION__, rce);
      return rce;
   }
   sy = *b_ypos;
   DEBUG_OUTP   yLOG_value   ("sy"        , sy);
   /*---(tables)-------------------------*/
   cx  = sx + a_xradius;
   cy  = sy + a_yradius;
   CREATE_head (n, a_act);
   /*---(save)---------------------------*/
   CREATE_tail (n, a_act, cx, cy);
   *b_xpos     = cx;
   *b_ypos     = cy;
   /*---(complete)-----------------------*/
   DEBUG_OUTP   yLOG_exit    (__FUNCTION__);
   return 0;
}



/*============================--------------------============================*/
/*===----                       specific letters                       ----===*/
/*============================--------------------============================*/
static void o___LETTERS________________o (void) {;}

short 
CREATE_find_by_name     (char *a_ltr, char a_scope, char *r_type, char *r_lcat, char r_label [LEN_TERSE], short *r_xradius, short *r_yradius, short *r_rot, short *r_beg, short *r_arc, char *r_dots)
{
   /*---(locals)-----------+-----+-----+-*/
   char        rce         =  -10;
   short       i           =    0;
   /*---(default)------------------------*/
   if (r_type    != NULL)  *r_type    = '-';
   if (r_lcat    != NULL)  *r_lcat    = 0;
   if (r_label   != NULL)  strcpy (r_label, "");
   if (r_xradius != NULL)  *r_xradius = 0;
   if (r_yradius != NULL)  *r_yradius = 0;
   if (r_rot     != NULL)  *r_rot     = 0;
   if (r_beg     != NULL)  *r_beg     = 0;
   if (r_arc     != NULL)  *r_arc     = 0;
   if (r_dots    != NULL)  *r_dots    = 0;
   /*---(defense)------------------------*/
   --rce;  if (a_ltr == NULL)  return rce;
   /*---(walk letters)-------------------*/
   for (i = 0; i < MAX_LETTERS; ++i) {
      /*---(check end)-------------------*/
      if (a_scope != LTRS_ALL && strcmp (g_loc[i].label, "END") == 0)    break;
      if (strcmp (g_loc[i].label, "EOF") == 0)                           break;
      /*---(check match)-----------------*/
      if (strcmp (a_ltr, g_loc[i].label) != 0)                           continue;
      /*---(found)-----------------------*/
      if (r_type    != NULL)  *r_type    = g_loc [i].type;
      if (r_lcat    != NULL)  *r_lcat    = g_loc [i].lcat;
      if (r_label   != NULL)  strlcpy (r_label, g_loc [i].label, LEN_TERSE);
      if (r_xradius != NULL)  *r_xradius = g_loc [i].x_ellipse;;
      if (r_yradius != NULL)  *r_yradius = g_loc [i].y_ellipse;;
      if (r_rot     != NULL)  *r_rot     = g_loc [i].r_ellipse;
      if (r_beg     != NULL)  *r_beg     = g_loc [i].b_arc;
      if (r_arc     != NULL)  *r_arc     = g_loc [i].l_arc;
      if (r_dots    != NULL)  *r_dots    = g_loc [i].dots;
      return i;
   }
   /*---(complete)-----------------------*/
   return --rce;
}

char
CREATE_letter           (char a_act, uchar *a_ltr, float *b_xpos, float *b_ypos)
{
   /*---(locals)-----------+-----+-----+-*/
   char        rce         =  -10;
   char        rc          =    0;
   short       n           =    0;
   char        x_type, x_dots;
   char        x_label     [LEN_SHORT] = "";
   short       x_rot, x_beg, x_arc, x_xradius, x_yradius;
   /*---(header)-------------------------*/
   DEBUG_OUTP   yLOG_enter   (__FUNCTION__);
   n = CREATE_find_by_name (a_ltr, LTRS_ALL, &x_type, NULL, x_label, &x_xradius, &x_yradius, &x_rot, &x_beg, &x_arc, &x_dots);
   DEBUG_OUTP   yLOG_value   ("current"   , n);
   --rce;  if (n < 0) {
      DEBUG_OUTP   yLOG_exitr   (__FUNCTION__, rce);
      return rce;
   }
   DEBUG_OUTP   yLOG_complex (x_label     , "%c, %3dxr, %3dyx, %3dr, %3db, %3da, %d", x_type, x_xradius, x_yradius, x_rot, x_beg, x_arc, x_dots);
   /*---(training dot)-------------------*/
   /*> if (a_act == SHAPE_TRAIN) {                                                    <* 
    *>    glColor4f (1.0, 0.0, 0.0, 1.0);                                             <* 
    *>    CREATE_dot (n, a_act, 1, b_xpos, b_ypos);                                   <* 
    *>    yCOLOR_opengl (YCOLOR_BAS, YCOLOR_MOR, 0.65);                               <* 
    *> }                                                                              <*/
   /*---(first point)--------------------*/
   switch (x_type) {
   case SHAPE_LINE     :
      CREATE_line     (n, a_act,  x_rot, x_arc, b_xpos, b_ypos);
      break;
   case SHAPE_CIRCLE   :
      CREATE_circle   (n, a_act, x_xradius, x_rot, b_xpos, b_ypos);
      break;
   case SHAPE_ELLIPSE  :
      CREATE_ellipse  (n, a_act, x_xradius, x_yradius, x_rot, x_beg, x_arc, x_dots, b_xpos, b_ypos);
      break;
   case SHAPE_TEARDROP :
      CREATE_teardrop (n, a_act, x_xradius, x_yradius, x_rot, x_beg, x_arc, b_xpos, b_ypos);
      break;
   case SHAPE_DOT      :
      CREATE_dot      (n, a_act, x_xradius, b_xpos, b_ypos);
      break;
   case SHAPE_SPACE    :
      CREATE_space    (n, a_act, x_xradius, x_yradius, b_xpos, b_ypos);
      break;
   }
   /*---(complete)-----------------------*/
   DEBUG_OUTP   yLOG_exit    (__FUNCTION__);
   return 0;
}

char
CREATE_letter_easy      (char a_act, uchar *a_ltr)
{
   float       x = 0, y = 0;
   return CREATE_letter (a_act, a_ltr, &x, &y);
}

char
CREATE_letter_data      (uchar *a_ltr)
{
   float       x = 0, y = 0;
   return CREATE_letter (SHAPE_DATA, a_ltr, &x, &y);
}



/*============================--------------------============================*/
/*===----                        helpful audits                        ----===*/
/*============================--------------------============================*/
static void o___AUDITING_______________o (void) {;}

char
CREATE_detail           (short n, char a_out [LEN_FULL])
{
   char        rce         =   -10;
   --rce;  if (a_out  == NULL)  return rce;
   --rce;  if (n < 0)           return rce;
   sprintf (a_out, "%-3d %-5.5s %c  %4d %4d %4d %4d %4d  %2d %1d %1d  %4d %4d %2d %2d  %6.1f %6.1f %6.1f %6.1f  %6.1f %6.1f %6.1f %6.1f  %3d",
         n, g_loc [n].label, g_loc [n].type,
         g_loc [n].x_ellipse, g_loc [n].y_ellipse, g_loc [n].r_ellipse, g_loc [n].b_arc, g_loc [n].l_arc,
         g_loc [n].lcat, g_loc [n].range, g_loc [n].size,
         g_loc [n].x_show, g_loc [n].y_show, g_loc [n].align, g_loc [n].dots,
         g_loc [n].x_end, g_loc [n].y_end, g_loc [n].deg, g_loc [n].xy_len,
         g_loc [n].x_lef, g_loc [n].x_rig, g_loc [n].y_top, g_loc [n].y_bot,
         g_loc [n].count);
   return 0;
}

char
CREATE_dump             (void)
{
   /*---(locals)-----------+-----+-----+-*/
   char        rce         =  -10;
   short       i           =    0;
   char        t           [LEN_FULL]  = "";
   /*---(walk letters)-------------------*/
   for (i = 0; i < MAX_LETTERS; ++i) {
      if (strcmp (g_loc[i].label, "EOF") == 0)                           break;
      CREATE_detail (i, t);
      printf ("%s\n", t);
   }
   /*---(complete)-----------------------*/
   return 0;
}


