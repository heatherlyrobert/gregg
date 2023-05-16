/*============================---(source-start)---============================*/
#include    "gregg.h"



/*
 *
 */





static float     s_xpos     =   30;
static float     s_ybase    =  -50;
static float     s_ypos     =  -50;

static float     s_bx       =  0.0;
static float     s_by       =  0.0;

static float     s_lef      =  0.0;
static float     s_cen      =  0.0;
static float     s_rig      =  0.0;

static float     s_top      =  0.0;
static float     s_mid      =  0.0;
static float     s_bot      =  0.0;

static float     s_ex       =  0.0;
static float     s_ey       =  0.0;

static char      s_ltr      [LEN_SHORT] = "";


/*============================--------------------============================*/
/*===----                       carry over values                      ----===*/
/*============================--------------------============================*/
static void o___SAVING____________________o (void) {;}

char
PAGE_word_begin         (void)
{
   s_bx    = s_by    = 0.0;
   s_lef   = s_cen   = s_rig   = 0.0;
   s_top   = s_mid   = s_bot   = 0.0;
   s_ex    = s_ey    = 0.0;
   return 0;
}

char
PAGE_letter_save        (float bx, float by, float a_lef, float a_rig, float a_top, float a_bot, float ex, float ey, char a_ltr [LEN_SHORT])
{  /* distances are relative to last letter end, current letter start */
   float     dx, dy;
   dx      = ex - bx;
   dy      = ey - by;
   s_bx    = bx;
   s_by    = by;
   s_lef   = a_lef - dx;
   s_rig   = a_rig - dx;
   s_cen   = (s_rig + s_lef) / 2.0;
   s_top   = a_top - dy;
   s_bot   = a_bot - dy;
   s_mid   = (s_top + s_bot) / 2.0;
   s_ex    = ex;
   s_ey    = ey;
   strlcpy (s_ltr, a_ltr, LEN_SHORT);
   return 0;
}

char*
PAGE_saved              (void)
{
   sprintf (g_print, "%6.1fbx  %6.1fby,  %6.1fex  %6.1fey,  %6.1fl  %6.1fr  %6.1fc,  %6.1ft  %6.1fb  %6.1fm", s_bx, s_by, s_ex, s_ey, s_lef, s_rig, s_cen, s_top, s_bot, s_mid);
   return g_print;
}



/*============================--------------------============================*/
/*===----                         page handling                        ----===*/
/*============================--------------------============================*/
static void o___PROGRAM___________________o (void) {;}

char
PAGE_init               (void)
{
   DEBUG_OUTP   yLOG_enter   (__FUNCTION__);
   my.t_tex = my.t_fbo = my.t_depth = -1;
   my.p_sizing = 0.0;  /* must initialize for config to update letters */
   PAGE_config ('/', -1, -1, '/', -1.0,    -1, -1, -1, -1,   -1, '/', -1, -1, -1, '/');
   DEBUG_OUTP   yLOG_exit    (__FUNCTION__);
   return 0;
}


/*============================--------------------============================*/
/*===----                         page handling                        ----===*/
/*============================--------------------============================*/
static void o___MEMORY____________________o (void) {;}

char
PAGE_config             (char a_layout, short a_wide, short a_tall, char a_anchor, float a_scale, short a_left, short a_right, short a_top, short a_bottom, float a_sizing, char a_align, short a_ascent, short a_descent, short a_spacing, char a_gapping)
{
   /*---(locals)-----------+-----+-----+-*/
   char        rce         =  -10;
   char        rc          =    0;
   int         n           =    0;
   float       x_sizing    = my.p_sizing;
   /*---(header)-------------------------*/
   DEBUG_OUTP   yLOG_enter   (__FUNCTION__);
   DEBUG_OUTP   yLOG_complex ("args"      , "%3d/%c   %4dw %4dt %2d %5.2f   ····· ·····   %4dl %4dr   %4dt %4db   %5.2f %2d   %4da %4dd   %4ds %3d/%c", a_layout, chrvisible (a_layout), a_wide, a_tall, a_anchor, a_scale, a_left, a_right, a_top, a_bottom, a_sizing, a_align, a_ascent, a_descent, a_spacing, a_gapping, chrvisible (a_gapping));
   /*---(change configuration -----------*/
   DEBUG_OUTP   yLOG_complex ("before"    , "%3d/%c   %4dw %4dt %2d %5.2f   %4dw %4dt   %4dl %4dr   %4dt %4db   %5.2f %2d   %4da %4dd   %4ds %3d/%c", my.w_layout, chrvisible (my.w_layout), my.w_wide, my.w_tall, my.w_anchor, my.w_scale, my.t_wide, my.t_tall, my.p_left, my.p_right, my.p_top, my.p_bottom, my.p_sizing, my.p_align, my.p_ascent, my.p_descent, my.p_spacing, my.p_gapping, chrvisible (my.p_gapping));
   /*---(overall layout)-----------------*/
   if      (a_layout  == '/')  my.w_layout  = LAYOUT_INTERPRET;
   else if (a_layout  != '-')  my.w_layout  = a_layout;
   switch (my.w_layout) {
   case LAYOUT_INTERPRET  :  strlcpy (my.w_title, "gregg shorthand interpreter"       , LEN_HUND);       break;
   case LAYOUT_PAGEVIEW   :  strlcpy (my.w_title, "gregg shorthand pageview"          , LEN_HUND);       break;
   case LAYOUT_DICTIONARY :  strlcpy (my.w_title, "gregg shorthand dictionary"        , LEN_HUND);       break;
   case LAYOUT_CONNECT    :  strlcpy (my.w_title, "gregg shorthand connection checker", LEN_HUND);       break;
   }
   /*---(accept sizes)-------------------*/
   if      (a_wide     <  0)   my.w_wide    =  500;
   else if (a_wide      > 0)   my.w_wide    = a_wide;
   if      (a_tall     <  0)   my.w_tall    =  350;
   else if (a_tall      > 0)   my.w_tall    = a_tall;
   /*---(window anchoring)---------------*/
   if      (a_anchor  == '/')  my.w_anchor  = YF_GREGG;
   else if (a_anchor  != '-')  my.w_anchor  = a_anchor;
   /*---(process scale)------------------*/
   if      (a_scale    <  0)   my.w_scale   =  2.0;
   else if (a_scale     > 0)   my.w_scale   = a_scale;
   /*---(texture sizing)-----------------*/
   my.t_wide   = my.w_wide * my.w_scale;
   my.t_tall   = my.w_tall * my.w_scale;
   /*---(accept margins)-----------------*/
   if      (a_left     <  0)   my.p_left    =   20         * my.w_scale;
   else if (a_left      > 0)   my.p_left    = a_left       * my.w_scale;
   if      (a_right    <  0)   my.p_right   =   30         * my.w_scale;
   else if (a_right     > 0)   my.p_right   = a_right      * my.w_scale;
   if      (a_top      <  0)   my.p_top     =   20         * my.w_scale;
   else if (a_top       > 0)   my.p_top     = a_top        * my.w_scale;
   if      (a_bottom   <  0)   my.p_bottom  =   30         * my.w_scale;
   else if (a_bottom    > 0)   my.p_bottom  = a_bottom     * my.w_scale;
   /*---(process outline sizing)---------*/
   if      (a_sizing   <  0)   my.p_sizing  =  1.0;
   else if (a_sizing    > 0)   my.p_sizing  = a_sizing;
   if      (a_align   == '/')  my.p_align   = YF_ORIGIN;
   else if (a_align   != '-')  my.p_align   = a_align;
   /*---(accept lines)-------------------*/
   if      (a_ascent   <  0)   my.p_ascent  =   20         * my.w_scale * my.p_sizing;
   else if (a_ascent    > 0)   my.p_ascent  = a_ascent     * my.w_scale * my.p_sizing;
   if      (a_descent  <  0)   my.p_descent =   20         * my.w_scale * my.p_sizing;
   else if (a_descent   > 0)   my.p_descent = a_descent    * my.w_scale * my.p_sizing;
   if      (a_spacing  <  0)   my.p_spacing =   15         * my.w_scale * my.p_sizing;
   else if (a_spacing   > 0)   my.p_spacing = a_spacing    * my.w_scale * my.p_sizing;
   if      (a_gapping == '/')  my.p_gapping = GAP_NORM;
   else if (a_gapping != '-')  my.p_gapping = a_gapping;
   /*---(calculate others)---------------*/
   my.p_linesize    = my.p_ascent + my.p_descent;
   /*---(done configuration)-------------*/
   DEBUG_OUTP   yLOG_complex ("after"     , "%3d/%c   %4dw %4dt %2d %5.2f   %4dw %4dt   %4dl %4dr   %4dt %4db   %5.2f %2d   %4da %4dd   %4ds %3d/%c", my.w_layout, chrvisible (my.w_layout), my.w_wide, my.w_tall, my.w_anchor, my.w_scale, my.t_wide, my.t_tall, my.p_left, my.p_right, my.p_top, my.p_bottom, my.p_sizing, my.p_align, my.p_ascent, my.p_descent, my.p_spacing, my.p_gapping, chrvisible (my.p_gapping));
   /*---(update letters)-----------------*/
   DEBUG_OUTP   yLOG_complex ("sizing"    , "%4.2f p_sizing vs %4.2f x_sizing", my.p_sizing, x_sizing);
   if (my.p_sizing != x_sizing) {
      TABLE_letters_data   (my.w_scale);
      DLIST_letters_make   (my.p_sizing);
   }
   /*---(complete)-----------------------*/
   DEBUG_OUTP   yLOG_exit    (__FUNCTION__);
   return 0;
}

char
PAGE_new                (char a_layout, short a_wide, short a_tall, char a_anchor, float a_scale, short a_left, short a_right, short a_top, short a_bottom, float a_sizing, char a_align, short a_ascent, short a_descent, short a_spacing, char a_gapping)
{
   /*---(locals)-----------+-----+-----+-*/
   char        rce         =  -10;
   char        rc          =    0;
   int         n           =    0;
   int         x, y;
   /*---(header)-------------------------*/
   DEBUG_OUTP   yLOG_enter   (__FUNCTION__);
   DEBUG_OUTP   yLOG_complex ("args"      , "%c   %4dw %4dt %2d %5.2f   %4dl %4dr   %4dt %4db   %5.2f %2d   %4da %4dd   %4ds %c", a_layout, a_wide, a_tall, a_anchor, a_scale, a_left, a_right, a_top, a_bottom, a_sizing, a_align, a_ascent, a_descent, a_spacing, a_gapping);
   /*---(free memory)--------------------*/
   DEBUG_OUTP   yLOG_complex ("pointers"  , "%4d tex, %4d fbo, %4d depth", my.t_tex, my.t_fbo, my.t_depth);
   --rce;  if ((int) my.t_tex >= 0) {
      DEBUG_OUTP   yLOG_note    ("must clear texture first");
      PAGE_free ();
      DEBUG_OUTP   yLOG_value   ("free"      , rc);
      if (rc < 0) {
         DEBUG_OUTP   yLOG_exitr   (__FUNCTION__, rce);
         return rce;
      }
   }
   /*---(change configuration -----------*/
   PAGE_config (a_layout, a_wide, a_tall, a_anchor, a_scale, a_left, a_right, a_top, a_bottom, a_sizing, a_align, a_ascent, a_descent, a_spacing, a_gapping);
   /*---(prepare)------------------------*/
   CREATE_draw_reset  (0, SHAPE_DRAW);
   s_xpos      = my.p_left;
   s_ybase     = - (my.p_top + my.p_ascent);
   s_ypos      = s_ybase;
   DEBUG_OUTP   yLOG_complex ("position"  , "%4.0f xpos, %4.0f ybase, %4.0f ypos", s_xpos, s_ybase, s_ypos);
   /*---(create texture)-----------------*/
   yCOLOR_opengl_clear (YCOLOR_BAS, YCOLOR_MED);
   DEBUG_OUTP   yLOG_complex ("size"      , "%4dw, %4dt", my.t_wide, my.t_tall);
   rc = yGLTEX_new (&(my.t_tex), &(my.t_fbo), &(my.t_depth), my.t_wide, my.t_tall);
   DEBUG_OUTP   yLOG_value   ("new"       , rc);
   DEBUG_OUTP   yLOG_complex ("pointers"  , "%4d tex, %4d fbo, %4d depth", my.t_tex, my.t_fbo, my.t_depth);
   --rce;  if (rc < 0) {
      DEBUG_OUTP   yLOG_exitr   (__FUNCTION__, rce);
      return rce;
   }
   rc = yGLTEX_draw         (my.t_tex, my.t_fbo, my.w_anchor, my.t_wide, my.t_tall, my.w_scale);
   DEBUG_OUTP   yLOG_value   ("draw"      , rc);
   --rce;  if (rc < 0) {
      DEBUG_OUTP   yLOG_exitr   (__FUNCTION__, rce);
      return rce;
   }
   yGLTEX_bounds       (NULL, &(my.t_lef), &(my.t_rig), &(my.t_xlen), &(my.t_bot), &(my.t_top), &(my.t_ylen));
   DEBUG_OUTP   yLOG_complex ("bounds"    , "%4d lef, %4d rig, %4d xlen,  %4d bot, %4d top, %4d ylen", my.t_lef, my.t_rig, my.t_xlen, my.t_bot, my.t_top, my.t_ylen);
   /*---(complete)-----------------------*/
   DEBUG_OUTP   yLOG_exit    (__FUNCTION__);
   return 0;
}

/*                                                 --------page------------------      ----margins-----    ---------outlines----------  */
char PAGE_new_default   (void)  { return PAGE_new ('/', -1     , -1   , '/', -1.0,      -1, -1, -1, -1,    -1.0, '/', -1, -1, -1, '/'); }
char PAGE_new_again     (void)  { return PAGE_new ('-', 0      , 0    , '-',  0.0,       0,  0,  0,  0,     0.0, '-',  0,  0,  0, '-'); }
char PAGE_new_screen    (void)  { return PAGE_new ('/', 1366   , 768  , '/', -1.0,      -1, -1, -1, -1,    -1.0, '/', -1, -1, -1, '/'); }
char PAGE_new_sized     (short a_wide, short a_tall, char a_anchor, float a_scale)  { return PAGE_new ('/', a_wide, a_tall, a_anchor, a_scale,   -1, -1, -1, -1,    -1.0, '/', -1, -1, -1, '/'); }

char*
PAGE_detail             (void)
{
   sprintf (g_print, "%c   %4dw %4dt %2d %5.2f   %4dw %4dt   %4dl %4dr   %4dt %4db   %5.2f %2d   %4da %4dd   %4ds %c   å%sæ", my.w_layout, my.w_wide, my.w_tall, my.w_anchor, my.w_scale, my.t_wide, my.t_tall, my.p_left, my.p_right, my.p_top, my.p_bottom, my.p_sizing, my.p_align, my.p_ascent, my.p_descent, my.p_spacing, my.p_gapping, my.w_title);
   return g_print;
}

char
PAGE_ready              (char a_png [LEN_PATH])
{
   /*---(locals)-----------+-----+-----+-*/
   char        rce         =  -10;
   char        rc          =    0;
   /*---(header)-------------------------*/
   DEBUG_OUTP   yLOG_enter   (__FUNCTION__);
   /*---(write out image)----------------*/
   --rce;  if (a_png != NULL) {
      rc = yGLTEX_tex2png (a_png, my.t_wide, my.t_tall);
      DEBUG_OUTP   yLOG_value   ("tex2png"   , rc);
      if (rc < 0) {
         DEBUG_OUTP   yLOG_exitr   (__FUNCTION__, rce);
         return rce;
      }
   }
   /*---(close texture)------------------*/
   rc = yGLTEX_done    (my.t_tex);
   DEBUG_OUTP   yLOG_value   ("done"      , rc);
   --rce;  if (rc < 0) {
      DEBUG_OUTP   yLOG_exitr   (__FUNCTION__, rce);
      return rce;
   }
   /*---(complete)-----------------------*/
   DEBUG_OUTP   yLOG_exit    (__FUNCTION__);
   return 0;
}

char
PAGE_free               (void)
{
   /*---(locals)-----------+-----+-----+-*/
   char        rce         =  -10;
   char        rc          =    0;
   /*---(header)-------------------------*/
   DEBUG_OUTP   yLOG_enter   (__FUNCTION__);
   /*---(free memory)--------------------*/
   rc = yGLTEX_free    (&(my.t_tex), &(my.t_fbo), &(my.t_depth));
   DEBUG_OUTP   yLOG_value   ("free"      , rc);
   --rce;  if (rc < 0) {
      DEBUG_OUTP   yLOG_exitr   (__FUNCTION__, rce);
      return rce;
   }
   /*---(ground)-------------------------*/
   my.t_tex = my.t_fbo = my.t_depth = -1;
   DEBUG_OUTP   yLOG_complex ("pointers"  , "%4d tex, %4d fbo, %4d depth", my.t_tex, my.t_fbo, my.t_depth);
   /*---(complete)-----------------------*/
   DEBUG_OUTP   yLOG_exit    (__FUNCTION__);
   return 0;
}



/*============================--------------------============================*/
/*===----                         page handling                        ----===*/
/*============================--------------------============================*/
static void o___SHOWN______________________o (void) {;}

char
PAGE_shown_letter       (char a_act, uchar *a_ltr)
{
   /*---(locals)-----------+-----+-----+-*/
   char        rce         =  -10;
   char        rc          =    0;
   short       n           =    0;
   char        x_ltr       [LEN_SHORT] = "";
   char        x_label     [LEN_SHORT] = "";
   float       x_rot       =    0;
   /*---(header)-------------------------*/
   DEBUG_OUTP   yLOG_enter   (__FUNCTION__);
   /*---(check for vowels)---------------*/
   if (strchr ("aAeEiIou", a_ltr [0]) != NULL) {
      sprintf (x_ltr, "%2.2s", a_ltr);
      x_rot = atof (a_ltr + 3); 
   } else {
      strlcpy (x_ltr, a_ltr, LEN_SHORT);
   }
   /*---(find base)----------------------*/
   DEBUG_OUTP   yLOG_info    ("x_ltr"     , x_ltr);
   n = TABLE_letter_by_name (x_ltr, LTRS_ALL, NULL, NULL, x_label, NULL, NULL, NULL, NULL, NULL, NULL);
   DEBUG_OUTP   yLOG_value   ("current"   , n);
   --rce;  if (n < 0) {
      DEBUG_OUTP   yLOG_exitr   (__FUNCTION__, rce);
      return rce;
   }
   DEBUG_OUTP   yLOG_info    ("showing"   , x_label);
   /*---(show letter)--------------------*/
   if (x_rot != 0.0)   glRotatef  ( x_rot, 0.0f, 0.0f, 1.0f);
   if (a_act == SHAPE_SAMPLE)   glCallList   (dl_dotted  + n);
   else                         glCallList   (dl_solid   + n);
   glTranslatef (g_loc [n].x_end, g_loc [n].y_end,  0.0);
   if (x_rot != 0.0)   glRotatef  (-x_rot, 0.0f, 0.0f, 1.0f);
   /*---(complete)-----------------------*/
   DEBUG_OUTP   yLOG_exit    (__FUNCTION__);
   return 0;
}



/*============================--------------------============================*/
/*===----                         page handling                        ----===*/
/*============================--------------------============================*/
static void o___LETTERS____________________o (void) {;}

char
PAGE_gregg_letter       (char a_act, uchar *a_ltr)
{
   /*---(locals)-----------+-----+-----+-*/
   char        rce         =  -10;
   char        rc          =    0;
   short       n           =    0;
   char        x_ltr       [LEN_SHORT] = "";
   char        x_label     [LEN_SHORT] = "";
   int         l           =    0;
   char        x_char      =  '·';
   /*---(header)-------------------------*/
   DEBUG_OUTP   yLOG_enter   (__FUNCTION__);
   /*---(check for ae)-------------------*/
   if (strcmp (a_ltr, " ") == 0)  sprintf (x_ltr, "e%s", s_ltr + 1);
   else                           strlcpy (x_ltr, a_ltr, LEN_SHORT);
   /*---(find)---------------------------*/
   DEBUG_OUTP   yLOG_info    ("x_ltr"     , x_ltr);
   n = TABLE_letter_by_name (x_ltr, LTRS_ALL, NULL, NULL, x_label, NULL, NULL, NULL, NULL, NULL, NULL);
   DEBUG_OUTP   yLOG_value   ("current"   , n);
   --rce;  if (n < 0) {
      DEBUG_OUTP   yLOG_exitr   (__FUNCTION__, rce);
      return rce;
   }
   DEBUG_OUTP   yLOG_info    ("showing"   , x_label);
   /*---(sheck special)------------------*/
   l      = strlen (x_ltr);
   x_char = x_ltr [0];
   if (l == 1 && strchr ("+-á", x_char) != NULL) {
      glColor4f (0.0, 0.0, 0.8, 1.0);
      switch (x_char) {
      case '-' :
         glLineWidth   (3.0);
         glBegin(GL_LINES); {
            glVertex3f (s_cen - 5, s_bot - 4, 250);
            glVertex3f (s_cen + 5, s_bot - 4, 250);
         } glEnd();
         break;
      case '+' :
         glLineWidth   (3.0);
         glBegin(GL_LINES); {
            glVertex3f (s_cen, s_mid, 250);
            glVertex3f (s_ex , s_ey , 250);
         } glEnd();
         break;
      case 'á' :
         glPointSize   (5.0);
         glBegin(GL_POINTS); {
            glVertex3f (s_cen, s_mid, 250);
         } glEnd();
         break;
      }
      glColor4f (0.0, 0.0, 0.0, 1.0);
   }
   /*---(show letter)--------------------*/
   else {
      if (x_ltr [0] == '/')  glColor4f (0.7, 0.0, 0.0, 1.0);
      if (a_ltr [0] == (uchar) ' ')  glColor4f (0.0, 0.0, 0.8, 1.0);
      if (a_act == SHAPE_SAMPLE)   glCallList   (dl_dotted  + n);
      else                         glCallList   (dl_solid   + n);
      if (x_ltr [0] == '>') glTranslatef (s_rig, s_bot,  0.0);
      glTranslatef (g_loc [n].x_end, g_loc [n].y_end,  0.0);
      if (x_ltr [0] == '/')  glColor4f (0.0, 0.0, 0.0, 1.0);
      if (a_ltr [0] == (uchar) ' ')  glColor4f (0.0, 0.0, 0.0, 1.0);
   }
   /*---(save data)----------------------*/
   PAGE_letter_save (0.0, 0.0, g_loc [n].x_lef, g_loc [n].x_rig, g_loc [n].y_top, g_loc [n].y_bot, g_loc [n].x_end, g_loc [n].y_end, x_ltr);
   /*---(complete)-----------------------*/
   DEBUG_OUTP   yLOG_exit    (__FUNCTION__);
   return 0;
}

char
PAGE_gregg_size         (char a_gregg [LEN_TITLE], char *r_count, short *r_points, float *r_xmin, float *r_xmax, float *r_wide, float *r_ymin, float *r_ymax, float *r_tall, short r_list [LEN_LABEL])
{
   /*---(locals)-----------+-----+-----+-*/
   char        rce         =  -10;
   char        rc          =    0;
   uchar       x_gregg     [LEN_TITLE] = "";
   char       *p           = NULL;
   char       *q           =  "·";
   char       *r           = NULL;
   short       n           =    0;
   char        x_label     [LEN_SHORT] = "";
   float       cx          =    0;
   float       cy          =    0;
   float       x_min       =    0;
   float       x_max       =    0;
   float       y_min       =    0;
   float       y_max       =    0;
   short       x_cnt       =    0;
   short       x_pnts      =    0;
   int         i           =    0;
   /*---(header)-------------------------*/
   DEBUG_OUTP   yLOG_enter   (__FUNCTION__);
   /*---(default)------------------------*/
   if (r_xmin   != NULL)  *r_xmin    = 0.0;
   if (r_xmax   != NULL)  *r_xmax    = 0.0;
   if (r_wide   != NULL)  *r_wide    = 0.0;
   if (r_ymin   != NULL)  *r_ymin    = 0.0;
   if (r_ymax   != NULL)  *r_ymax    = 0.0;
   if (r_tall   != NULL)  *r_tall    = 0.0;
   if (r_count  != NULL)  *r_count   = 0;
   if (r_points != NULL)  *r_points  = 0;
   if (r_list   != NULL)  { for (i = 0; i < LEN_LABEL; ++i)  r_list [i] = -1; }
   /*---(defense)------------------------*/
   DEBUG_OUTP   yLOG_point   ("a_gregg"   , a_gregg);
   --rce;  if (a_gregg == NULL) {
      DEBUG_OUTP   yLOG_exitr   (__FUNCTION__, rce);
      return rce;
   }
   /*---(prepare)------------------------*/
   strlcpy (x_gregg, a_gregg, LEN_TITLE);
   /*---(begin parse)--------------------*/
   p = strtok_r (x_gregg, q, &r);
   /*---(walk letters)-------------------*/
   --rce;  while (p != NULL) {
      /*---(find letter)-----------------*/
      DEBUG_OUTP   yLOG_info    ("p"         , p);
      n = TABLE_letter_by_name (p, LTRS_ALL, NULL, NULL, x_label, NULL, NULL, NULL, NULL, NULL, NULL);
      DEBUG_OUTP   yLOG_value   ("current"   , n);
      --rce;  if (n < 0) {
         DEBUG_OUTP   yLOG_exitr   (__FUNCTION__, rce);
         return rce;
      }
      DEBUG_OUTP   yLOG_info    ("showing"   , x_label);
      /*---(width)-----------------------*/
      if (cx + g_loc [n].x_lef < x_min)  x_min = cx + g_loc [n].x_lef;
      if (cx + g_loc [n].x_rig > x_max)  x_max = cx + g_loc [n].x_rig;
      DEBUG_OUTP   yLOG_complex ("x"         , "%6.1f min, %6.1f max", x_min, x_max);
      /*---(heiight)---------------------*/
      if (cy + g_loc [n].y_top > y_max)  y_max = cy + g_loc [n].y_top;
      if (cy + g_loc [n].y_bot < y_min)  y_min = cy + g_loc [n].y_bot;
      DEBUG_OUTP   yLOG_complex ("y"         , "%6.1f min, %6.1f max", y_min, y_max);
      /*---(stats)-----------------------*/
      if (r_list != NULL)  r_list [x_cnt] = n;
      ++x_cnt;
      x_pnts += g_loc [n].count;
      DEBUG_OUTP   yLOG_complex ("stats"     , "%2dc, %4dp, %d", x_cnt, x_pnts, n);
      /*---(save)------------------------*/
      cx += g_loc [n].x_end;
      cy += g_loc [n].y_end;
      /*---(next)------------------------*/
      p = strtok_r (NULL   , q, &r);
      DEBUG_OUTP   yLOG_point   ("p"         , p);
      /*---(done)------------------------*/
   }
   /*---(save-back)----------------------*/
   if (r_xmin   != NULL)  *r_xmin    = x_min;
   if (r_xmax   != NULL)  *r_xmax    = x_max;
   if (r_wide   != NULL)  *r_wide    = -x_min + x_max;
   if (r_ymin   != NULL)  *r_ymin    = y_min;
   if (r_ymax   != NULL)  *r_ymax    = y_max;
   if (r_tall   != NULL)  *r_tall    = -y_min + y_max;
   if (r_count  != NULL)  *r_count   = x_cnt;
   if (r_points != NULL)  *r_points  = x_pnts;
   /*---(complete)-----------------------*/
   DEBUG_OUTP   yLOG_exit    (__FUNCTION__);
   return 0;
}

char
PAGE_gregg_word         (char a_act, char a_gregg [LEN_TITLE], float *b_xpos, float *b_ypos)
{
   /*---(locals)-----------+-----+-----+-*/
   char        rce         =  -10;
   char        rc          =    0;
   char        x_count     =    0;
   short       x_points    =    0;
   float       x_min, x_max, x_wide;
   float       y_min, y_max, y_tall;
   short       x_list      [LEN_LABEL];
   int         i           =    0;
   char        x_label     [LEN_TERSE] = "";
   float       x_move      =    0;
   float       x_fore      =    0;
   /*---(header)-------------------------*/
   DEBUG_OUTP   yLOG_enter   (__FUNCTION__);
   /*---(defense)------------------------*/
   DEBUG_OUTP   yLOG_point   ("a_gregg"   , a_gregg);
   --rce;  if (a_gregg == NULL) {
      DEBUG_OUTP   yLOG_exitr   (__FUNCTION__, rce);
      return rce;
   }
   /*---(get data)-----------------------*/
   rc = PAGE_gregg_size (a_gregg, &x_count, &x_points, &x_min, &x_max, &x_wide, &y_min, &y_max, &y_tall, x_list);
   DEBUG_OUTP   yLOG_value   ("sizing"    , rc);
   --rce;  if (rc < 0) {
      DEBUG_OUTP   yLOG_exitr   (__FUNCTION__, rce);
      return rce;
   }
   /*---(draw outline)-------------------*/
   glPushMatrix(); {
      /*---(get to start)----------------*/
      DEBUG_OUTP   yLOG_complex ("start"     , "%6.1fx, %6.1fy", *b_xpos, *b_ypos);
      glTranslatef (*b_xpos, *b_ypos,  0.0);
      /*---(mark default start)----------*/
      if (my.p_hinting == STYLE_HINTS) {
         glColor4f     (0.0, 1.0, 0.0, 1.0);
         glLineWidth   (0.3);
         glBegin       (GL_LINES); {
            glVertex3f (  0,    5,  50);
            glVertex3f (  0,   -5,  50);
            glVertex3f ( -5,    0,  50);
            glVertex3f (  5,    0,  50);
         } glEnd       ();
      }
      /*---(adjust for start)------------*/
      if (my.p_align != YF_ORIGIN) {
         DEBUG_OUTP   yLOG_complex ("not origin", "%6.1fx, %6.1fy", -x_min, -y_max);
         glTranslatef (-x_min, -y_max,  0.0);
      }
      switch (my.p_align) {
      case YF_ORIGIN :
         x_fore = x_max + my.p_spacing;
         DEBUG_OUTP   yLOG_complex ("x-align"   , "origin  %6.1ff, %6.1ff, %3ds", x_move, x_fore, my.p_spacing);
         break;
      case YF_TOPLEF : case YF_MIDLEF : case YF_BASLEF : case YF_BOTLEF : case YF_ORILEF :
         x_fore = x_wide * 1.0 + my.p_spacing;
         DEBUG_OUTP   yLOG_complex ("x-align"   , "left    %6.1ff, %6.1ff, %3ds", x_move, x_fore, my.p_spacing);
         break;
      case YF_TOPCEN : case YF_MIDCEN : case YF_BASCEN : case YF_BOTCEN : case YF_ORICEN :
         x_fore = x_wide * 0.5 + my.p_spacing;
         x_move = -(x_wide * 0.5);
         DEBUG_OUTP   yLOG_complex ("x-align"   , "center  %6.1ff, %6.1ff, %3ds", x_move, x_fore, my.p_spacing);
         glTranslatef (x_move, 0.0,  0.0);
         break;
      case YF_TOPRIG : case YF_MIDRIG : case YF_BASRIG : case YF_BOTRIG : case YF_ORIRIG :
         x_fore = my.p_spacing;
         x_move = -(x_wide * 1.0);
         DEBUG_OUTP   yLOG_complex ("x-align"   , "right   %6.1ff, %6.1ff, %3ds", x_move, x_fore, my.p_spacing);
         glTranslatef (-(x_wide * 1.0), 0.0,  0.0);
         break;
      }
      switch (my.p_align) {
      case YF_ORILEF : case YF_ORICEN : case YF_ORIRIG :
         glTranslatef (0.0   ,  y_max,  0.0);
         break;
      case YF_TOPLEF : case YF_TOPCEN : case YF_TOPRIG :
         break;
      case YF_MIDLEF : case YF_MIDCEN : case YF_MIDRIG :
         glTranslatef (0.0, (y_tall * 0.5),  0.0);
         break;
      case YF_BASLEF : case YF_BASCEN : case YF_BASRIG :
      case YF_BOTLEF : case YF_BOTCEN : case YF_BOTRIG :
         glTranslatef (0.0, (y_tall * 1.0),  0.0);
         break;
      }
      /*---(box outline)-----------------*/
      if (my.p_hinting == STYLE_HINTS) {
         glColor4f     (1.0, 1.0, 0.0, 0.5);
         glLineWidth   (0.3);
         glBegin       (GL_LINE_STRIP); {
            glVertex3f (x_min, y_max, -10);
            glVertex3f (x_max, y_max, -10);
            glVertex3f (x_max, y_min, -10);
            glVertex3f (x_min, y_min, -10);
            glVertex3f (x_min, y_max, -10);
         } glEnd       ();
      }
      /*---(back to defaults)------------*/
      if      (strldcnt (a_gregg, '<', LEN_TITLE) > 0)  glColor4f (0.7, 0.0, 0.0, 1.0);
      else if (strldcnt (a_gregg, '¢', LEN_TITLE) > 0)  glColor4f (0.7, 0.0, 0.0, 1.0);
      else                                              glColor4f (0.0, 0.0, 0.0, 1.0);
      /*---(walk letters)----------------*/
      /*> glScalef              (my.p_sizing, my.p_sizing, my.p_sizing);                       <*/
      for (i = 0; i < x_count; ++i)  {
         strlcpy (x_label, g_loc [x_list [i]].label, LEN_TERSE);
         DEBUG_OUTP   yLOG_info    ("x_label"   , x_label);
         if (strcmp (x_label, "<") == 0)   glColor4f (0.0, 0.0, 0.0, 1.0);
         if (strcmp (x_label, "¢") == 0)   glColor4f (0.0, 0.0, 0.0, 1.0);
         if (strcmp (x_label, ">") == 0)   glColor4f (0.7, 0.0, 0.0, 1.0);
         rc =  PAGE_gregg_letter (a_act, x_label);
      }
      /*> glScalef              (1.0, 1.0, 1.0);                                      <*/
   } glPopMatrix();
   /*---(back to defaults)---------------*/
   glColor4f (0.0, 0.0, 0.0, 1.0);
   /*---(save-back)----------------------*/
   switch (my.p_gapping) {
   case GAP_GRID :
      *b_xpos += my.p_spacing;
      break;
   default  :
      *b_xpos += x_fore;
      break;
   }
   /*---(complete)-----------------------*/
   DEBUG_OUTP   yLOG_exit    (__FUNCTION__);
   return 0;
}

char
PAGE_next_line          (float *b_xpos, float *b_ypos)
{
   if (b_xpos != NULL) {
      if (my.p_gapping == GAP_GRID)  *b_xpos = my.p_left + (my.p_spacing * 0.5);
      else                           *b_xpos = my.p_left;
   }
   if (b_ypos != NULL)  *b_ypos -= my.p_linesize;
   return 0;
}

char
PAGE_next_grid          (float *b_xpos, float *b_ypos)
{
   if (b_xpos != NULL)  *b_xpos += my.p_spacing;
   return 0;
}

char
PAGE_gregg              (char a_act, char a_gregg [LEN_RECD], float *b_xpos, float *b_ypos)
{
   /*---(locals)-----------+-----+-----+-*/
   char        rce         =  -10;
   char        rc          =    0;
   uchar       x_gregg     [LEN_RECD] = "";
   char       *p           = NULL;
   char       *q           =  " ";
   char       *r           = NULL;
   int         l           =    0;
   short       n           =    0;
   tWORD      *x_word      = NULL;
   uchar       x_shown     [LEN_HUND] = "";
   short       x_drawn     [LEN_LABEL];
   /*---(header)-------------------------*/
   DEBUG_OUTP   yLOG_enter   (__FUNCTION__);
   /*---(defense)------------------------*/
   DEBUG_OUTP   yLOG_point   ("a_gregg"   , a_gregg);
   --rce;  if (a_gregg == NULL) {
      DEBUG_OUTP   yLOG_exitr   (__FUNCTION__, rce);
      return rce;
   }
   /*---(prepare)------------------------*/
   strlcpy (x_gregg, a_gregg, LEN_RECD);
   /*---(begin parse)--------------------*/
   p = strtok_r (x_gregg, q, &r);
   DEBUG_OUTP   yLOG_point   ("p"         , p);
   if (p == NULL) {
      DEBUG_OUTP   yLOG_exitr   (__FUNCTION__, rce);
      return rce;
   }
   /*---(walk letters)-------------------*/
   --rce;  while (p != NULL) {
      l = strlen (p);
      DEBUG_OUTP   yLOG_complex ("parsed"    , "%2d[%s]", l, p);
      if (strcmp (p, "¦") == 0) {
         DEBUG_OUTP   yLOG_note    ("found new line");
         PAGE_next_line (b_xpos, b_ypos);
      } else if (strcmp (p, "£") == 0) {
         DEBUG_OUTP   yLOG_note    ("found empty grid");
         PAGE_next_grid (b_xpos, b_ypos);
      } else {
         n = WORDS_by_gregg (p, &x_word);
         if (n >= 0)  strlcpy (x_shown, x_word->w_shown, LEN_HUND);
         else         FIX_gregg  (p, x_shown, x_drawn, NULL);
         rc =  PAGE_gregg_word (a_act, x_shown, b_xpos, b_ypos);
         DEBUG_OUTP   yLOG_value   ("word"      , rc);
      }
      DEBUG_OUTP   yLOG_complex ("new pos"   , "%6.1fx, %6.1fy", *b_xpos, *b_ypos);
      p = strtok_r (NULL   , q, &r);
      DEBUG_OUTP   yLOG_point   ("p"         , p);
   }
   /*---(complete)-----------------------*/
   DEBUG_OUTP   yLOG_exit    (__FUNCTION__);
   return 0;
}



/*============================--------------------============================*/
/*===----                         page handling                        ----===*/
/*============================--------------------============================*/
static void o___DRIVER_____________________o (void) {;}

char
PAGE_demo_letters       (void)
{
   /*---(locals)-----------+-----+-----+-*/
   char        rce         =  -10;
   char        rc          =    0;
   /*---(header)-------------------------*/
   DEBUG_OUTP   yLOG_enter   (__FUNCTION__);
   /*---(page)---------------------------*/
   rc = PAGE_new ('-'        , 1366   , 768  , '/', 1.0,   -1, -1, -1, -1,   1.0, YF_MIDCEN, 20, 20, 50, GAP_GRID);
   DEBUG_OUTP   yLOG_value   ("default"   , rc);
   --rce;  if (rc < 0) {
      DEBUG_OUTP   yLOG_exitr   (__FUNCTION__, rce);
      return rce;
   }
   glPushMatrix    (); {
      s_xpos = my.p_left + (my.p_spacing * 0.5);
      s_ypos = s_ybase;
      PAGE_gregg (SHAPE_DRAW, "t      d      dd      n      m      mm      sh     ch     j      ng       ngk        ¦" , &s_xpos, &s_ypos);
      PAGE_gregg (SHAPE_DRAW, "f      v      p       b      k      g       r      l      rd     ld       s       z  ¦"       , &s_xpos, &s_ypos);
      PAGE_gregg (SHAPE_DRAW, "th     tn     tm      ht     nt     nd      ts     df     pt     bd       xs      xz ¦"       , &s_xpos, &s_ypos);
   } glPopMatrix   ();
   /*---(page)---------------------------*/
   rc = PAGE_ready   ("/tmp/gregg_page.png");
   rc = PAGE_free    ();
   DEBUG_OUTP   yLOG_value   ("default"   , rc);
   --rce;  if (rc < 0) {
      DEBUG_OUTP   yLOG_exitr   (__FUNCTION__, rce);
      return rce;
   }
   /*---(complete)-----------------------*/
   DEBUG_OUTP   yLOG_exit    (__FUNCTION__);
   return 0;
}

char
PAGE_demo_cat_none      (void)
{
   /*---(locals)-----------+-----+-----+-*/
   char        rce         =  -10;
   char        rc          =    0;
   /*---(header)-------------------------*/
   DEBUG_OUTP   yLOG_enter   (__FUNCTION__);
   /*---(page)---------------------------*/
   rc = PAGE_new ('-'        , 1366   , 768  , '/', 2.0,   -1, -1, -1, -1,   3.0, YF_MIDCEN, 10, 10, 50, GAP_GRID);
   DEBUG_OUTP   yLOG_value   ("default"   , rc);
   --rce;  if (rc < 0) {
      DEBUG_OUTP   yLOG_exitr   (__FUNCTION__, rce);
      return rce;
   }
   /*---(page)---------------------------*/
   glPushMatrix    (); {
      s_xpos = my.p_left + (my.p_spacing * 0.5);
      s_ypos = s_ybase;
      PAGE_gregg (SHAPE_DRAW, "axd·t   axd·d   axd·dd   axm·n   axm·m   axm·mm   axj·sh  axj·ch  axj·j   axN·ng  axN·ngk          ¦" , &s_xpos, &s_ypos);
      PAGE_gregg (SHAPE_DRAW, "axf·f   axf·v   axp·p    axp·b   axk·k   axk·g    axr·r   axr·l   axR·rd  axR·ld  axs·s     axz·z  ¦"       , &s_xpos, &s_ypos);
      PAGE_gregg (SHAPE_DRAW, "exd·t   exd·d   exd·dd   exm·n   exm·m   exm·mm   exj·sh  exj·ch  exj·j   exN·ng  exN·ngk          ¦" , &s_xpos, &s_ypos);
      PAGE_gregg (SHAPE_DRAW, "exf·f   exf·v   exp·p    exp·b   exk·k   exk·g    exr·r   exr·l   exR·rd  exR·ld  exs·s     exz·z  ¦"       , &s_xpos, &s_ypos);
   } glPopMatrix   ();
   /*---(page)---------------------------*/
   rc = PAGE_ready   ("/tmp/gregg_page.png");
   rc = PAGE_free    ();
   DEBUG_OUTP   yLOG_value   ("default"   , rc);
   --rce;  if (rc < 0) {
      DEBUG_OUTP   yLOG_exitr   (__FUNCTION__, rce);
      return rce;
   }
   /*---(complete)-----------------------*/
   DEBUG_OUTP   yLOG_exit    (__FUNCTION__);
   return 0;
}

char
PAGE_demo_cat_d         (void)
{
   /*---(locals)-----------+-----+-----+-*/
   char        rce         =  -10;
   char        rc          =    0;
   /*---(header)-------------------------*/
   DEBUG_OUTP   yLOG_enter   (__FUNCTION__);
   /*---(page)---------------------------*/
   rc = PAGE_new (STYLE_HINTS, 1366   , 768  , '/', 2.0,   -1, -1, -1, -1,   2.0, YF_MIDCEN, 10, 10, 50, GAP_GRID);
   DEBUG_OUTP   yLOG_value   ("default"   , rc);
   --rce;  if (rc < 0) {
      DEBUG_OUTP   yLOG_exitr   (__FUNCTION__, rce);
      return rce;
   }
   /*---(page)---------------------------*/
   glPushMatrix    (); {
      s_xpos = my.p_left + (my.p_spacing * 0.5);
      s_ypos = s_ybase;
      PAGE_gregg (SHAPE_DRAW, "a·d  t·a·t   t·a·d   t·a·dd   t·a·n   t·a·m   t·a·mm   t·a·sh  t·a·ch  t·a·j   t·a·ng  t·a·ngk          ¦" , &s_xpos, &s_ypos);
      PAGE_gregg (SHAPE_DRAW, "t·a·f   t·a·v   t·a·p    t·a·b   t·a·k   t·a·g    t·a·r   t·a·l   t·a·rd  t·a·ld  t·a·s     t·a·z  ¦"       , &s_xpos, &s_ypos);
      PAGE_gregg (SHAPE_DRAW, "t·a·ht  t·a·nt  t·a·nd  t·a·th  t·a·tn  t·a·tm    t·a·df  t·a·bd   ¦"       , &s_xpos, &s_ypos);
      PAGE_gregg (SHAPE_DRAW, "e·d  t·e·t   t·e·d   t·e·dd   t·e·n   t·e·m   t·e·mm   t·e·sh  t·e·ch  t·e·j   t·e·ng  t·e·ngk          ¦" , &s_xpos, &s_ypos);
      PAGE_gregg (SHAPE_DRAW, "t·e·f   t·e·v   t·e·p    t·e·b   t·e·k   t·e·g    t·e·r   t·e·l   t·e·rd  t·e·ld  t·e·s·t   t·e·z·t  ¦"       , &s_xpos, &s_ypos);
      PAGE_gregg (SHAPE_DRAW, "t·e·ht  t·e·nt  t·e·nd  t·e·th  t·e·tn  t·e·tm    t·e·df  t·e·bd   ¦"       , &s_xpos, &s_ypos);
      PAGE_gregg (SHAPE_DRAW, "t·e·z·t  t·e·z·t·>·´  t·e·z·t·>·r  t·e·z·t·>·b    /·<·e·d   /·<·a·d ¦"       , &s_xpos, &s_ypos);
   } glPopMatrix   ();
   /*---(page)---------------------------*/
   rc = PAGE_ready   ("/tmp/gregg_page.png");
   rc = PAGE_free    ();
   DEBUG_OUTP   yLOG_value   ("default"   , rc);
   --rce;  if (rc < 0) {
      DEBUG_OUTP   yLOG_exitr   (__FUNCTION__, rce);
      return rce;
   }
   /*---(complete)-----------------------*/
   DEBUG_OUTP   yLOG_exit    (__FUNCTION__);
   return 0;
}

char
PAGE_demo_cat_m         (void)
{
   /*---(locals)-----------+-----+-----+-*/
   char        rce         =  -10;
   char        rc          =    0;
   /*---(header)-------------------------*/
   DEBUG_OUTP   yLOG_enter   (__FUNCTION__);
   /*---(page)---------------------------*/
   rc = PAGE_new ('-'        , 1366   , 768  , '/', 2.0,   -1, -1, -1, -1,   3.0, YF_MIDCEN, 10, 10, 50, GAP_GRID);
   DEBUG_OUTP   yLOG_value   ("default"   , rc);
   --rce;  if (rc < 0) {
      DEBUG_OUTP   yLOG_exitr   (__FUNCTION__, rce);
      return rce;
   }
   /*---(page)---------------------------*/
   glPushMatrix    (); {
      s_xpos = my.p_left + (my.p_spacing * 0.5);
      s_ypos = s_ybase;
      PAGE_gregg (SHAPE_DRAW, "axm·m  n·amd·t   n·amd·d   n·amd·dd   n·amm·n   n·amm·m   n·amm·mm   n·amj·sh  n·amj·ch  n·amj·j   n·amN·ng  n·amN·ngk          ¦" , &s_xpos, &s_ypos);
      PAGE_gregg (SHAPE_DRAW, "n·amf·f   n·amf·v   n·amp·p    n·amp·b   n·amk·k   n·amk·g    n·amr·r   n·amr·l   n·amR·rd  n·amR·ld  n·ams·s     n·amz·z  ¦"       , &s_xpos, &s_ypos);
      PAGE_gregg (SHAPE_DRAW, "n·amH·ht  n·amH·nt  n·amH·nd  n·amT·th  n·amT·tn  n·amT·tm    n·amD·df  n·amP·bd   ¦"       , &s_xpos, &s_ypos);
   } glPopMatrix   ();
   /*---(page)---------------------------*/
   rc = PAGE_ready   ("/tmp/gregg_page.png");
   rc = PAGE_free    ();
   DEBUG_OUTP   yLOG_value   ("default"   , rc);
   --rce;  if (rc < 0) {
      DEBUG_OUTP   yLOG_exitr   (__FUNCTION__, rce);
      return rce;
   }
   /*---(complete)-----------------------*/
   DEBUG_OUTP   yLOG_exit    (__FUNCTION__);
   return 0;
}

char
PAGE_demo_cat_k         (void)
{
   /*---(locals)-----------+-----+-----+-*/
   char        rce         =  -10;
   char        rc          =    0;
   /*---(header)-------------------------*/
   DEBUG_OUTP   yLOG_enter   (__FUNCTION__);
   /*---(page)---------------------------*/
   rc = PAGE_new ('-'        , 1366   , 768  , '/', 2.0,   -1, -1, -1, -1,   3.0, YF_MIDCEN, 10, 10, 50, GAP_GRID);
   DEBUG_OUTP   yLOG_value   ("default"   , rc);
   --rce;  if (rc < 0) {
      DEBUG_OUTP   yLOG_exitr   (__FUNCTION__, rce);
      return rce;
   }
   /*---(page)---------------------------*/
   glPushMatrix    (); {
      s_xpos = my.p_left + (my.p_spacing * 0.5);
      s_ypos = s_ybase;
      PAGE_gregg (SHAPE_DRAW, "axk·k  k·akd·t   k·akd·d   k·akd·dd   k·akm·n   k·akm·m   k·akm·mm   k·akj·sh  k·akj·ch  k·akj·j   k·akN·ng  k·akN·ngk          ¦" , &s_xpos, &s_ypos);
      PAGE_gregg (SHAPE_DRAW, "k·akf·f   k·akf·v   k·akp·p    k·akp·b   k·akk·k   k·akk·g    k·akr·r   k·akr·l   k·akR·rd  k·akR·ld  k·aks·s     k·akz·z  ¦"       , &s_xpos, &s_ypos);
      PAGE_gregg (SHAPE_DRAW, "k·akH·ht  k·akH·nt  k·akH·nd  k·akT·th  k·akT·tn  k·akT·tm    k·akD·df  k·akP·bd   ¦"       , &s_xpos, &s_ypos);
   } glPopMatrix   ();
   /*---(page)---------------------------*/
   rc = PAGE_ready   ("/tmp/gregg_page.png");
   rc = PAGE_free    ();
   DEBUG_OUTP   yLOG_value   ("default"   , rc);
   --rce;  if (rc < 0) {
      DEBUG_OUTP   yLOG_exitr   (__FUNCTION__, rce);
      return rce;
   }
   /*---(complete)-----------------------*/
   DEBUG_OUTP   yLOG_exit    (__FUNCTION__);
   return 0;
}

char
PAGE_demo_cat_r         (void)
{
   /*---(locals)-----------+-----+-----+-*/
   char        rce         =  -10;
   char        rc          =    0;
   /*---(header)-------------------------*/
   DEBUG_OUTP   yLOG_enter   (__FUNCTION__);
   /*---(page)---------------------------*/
   rc = PAGE_new ('-'        , 1366   , 768  , '/', 2.0,   -1, -1, -1, -1,   3.0, YF_MIDCEN, 10, 10, 50, GAP_GRID);
   DEBUG_OUTP   yLOG_value   ("default"   , rc);
   --rce;  if (rc < 0) {
      DEBUG_OUTP   yLOG_exitr   (__FUNCTION__, rce);
      return rce;
   }
   /*---(page)---------------------------*/
   glPushMatrix    (); {
      s_xpos = my.p_left + (my.p_spacing * 0.5);
      s_ypos = s_ybase;
      PAGE_gregg (SHAPE_DRAW, "axr·r  r·ard·t   r·ard·d   r·ard·dd   r·arm·n   r·arm·m   r·arm·mm   r·arj·sh  r·arj·ch  r·arj·j   r·arN·ng  r·arN·ngk          ¦" , &s_xpos, &s_ypos);
      PAGE_gregg (SHAPE_DRAW, "r·arf·f   r·arf·v   r·arp·p    r·arp·b   r·ark·k   r·ark·g    r·arr·r   r·arr·l   r·arR·rd  r·arR·ld  r·ars·s     r·arz·z  ¦"       , &s_xpos, &s_ypos);
      PAGE_gregg (SHAPE_DRAW, "r·arH·ht  r·arH·nt  r·arH·nd  r·arT·th  r·arT·tn  r·arT·tm    r·arD·df  r·arP·bd   ¦"       , &s_xpos, &s_ypos);
   } glPopMatrix   ();
   /*---(page)---------------------------*/
   rc = PAGE_ready   ("/tmp/gregg_page.png");
   rc = PAGE_free    ();
   DEBUG_OUTP   yLOG_value   ("default"   , rc);
   --rce;  if (rc < 0) {
      DEBUG_OUTP   yLOG_exitr   (__FUNCTION__, rce);
      return rce;
   }
   /*---(complete)-----------------------*/
   DEBUG_OUTP   yLOG_exit    (__FUNCTION__);
   return 0;
}

char
PAGE_demo_cat_j         (void)
{
   /*---(locals)-----------+-----+-----+-*/
   char        rce         =  -10;
   char        rc          =    0;
   /*---(header)-------------------------*/
   DEBUG_OUTP   yLOG_enter   (__FUNCTION__);
   /*---(page)---------------------------*/
   rc = PAGE_new ('-'        , 1366   , 768  , '/', 2.0,   -1, -1, -1, -1,   3.0, YF_MIDCEN, 10, 10, 50, GAP_GRID);
   DEBUG_OUTP   yLOG_value   ("default"   , rc);
   --rce;  if (rc < 0) {
      DEBUG_OUTP   yLOG_exitr   (__FUNCTION__, rce);
      return rce;
   }
   /*---(page)---------------------------*/
   glPushMatrix    (); {
      s_xpos = my.p_left + (my.p_spacing * 0.5);
      s_ypos = s_ybase;
      PAGE_gregg (SHAPE_DRAW, "axj·ch  ch·ajd·t   ch·ajd·d   ch·ajd·dd   ch·ajm·n   ch·ajm·m   ch·ajm·mm   ch·ajj·sh  ch·ajj·ch  ch·ajj·j   ch·ajN·ng  ch·ajN·ngk          ¦" , &s_xpos, &s_ypos);
      PAGE_gregg (SHAPE_DRAW, "ch·ajf·f   ch·ajf·v   ch·ajp·p    ch·ajp·b   ch·ajk·k   ch·ajk·g    ch·ajr·r   ch·ajr·l   ch·ajR·rd  ch·ajR·ld  ch·ajs·s     ch·ajz·z  ¦"       , &s_xpos, &s_ypos);
      PAGE_gregg (SHAPE_DRAW, "ch·ajH·ht  ch·ajH·nt  ch·ajH·nd  ch·ajT·th  ch·ajT·tn  ch·ajT·tm    ch·ajD·df  ch·ajP·bd   ¦"       , &s_xpos, &s_ypos);
   } glPopMatrix   ();
   /*---(page)---------------------------*/
   rc = PAGE_ready   ("/tmp/gregg_page.png");
   rc = PAGE_free    ();
   DEBUG_OUTP   yLOG_value   ("default"   , rc);
   --rce;  if (rc < 0) {
      DEBUG_OUTP   yLOG_exitr   (__FUNCTION__, rce);
      return rce;
   }
   /*---(complete)-----------------------*/
   DEBUG_OUTP   yLOG_exit    (__FUNCTION__);
   return 0;
}

char
PAGE_demo_cat_f         (void)
{
   /*---(locals)-----------+-----+-----+-*/
   char        rce         =  -10;
   char        rc          =    0;
   /*---(header)-------------------------*/
   DEBUG_OUTP   yLOG_enter   (__FUNCTION__);
   /*---(page)---------------------------*/
   rc = PAGE_new ('-'        , 1366   , 768  , '/', 2.0,   -1, -1, -1, -1,   3.0, YF_MIDCEN, 10, 10, 50, GAP_GRID);
   DEBUG_OUTP   yLOG_value   ("default"   , rc);
   --rce;  if (rc < 0) {
      DEBUG_OUTP   yLOG_exitr   (__FUNCTION__, rce);
      return rce;
   }
   /*---(page)---------------------------*/
   glPushMatrix    (); {
      s_xpos = my.p_left + (my.p_spacing * 0.5);
      s_ypos = s_ybase;
      PAGE_gregg (SHAPE_DRAW, "axf·f  f·afd·t   f·afd·d   f·afd·dd   f·afm·n   f·afm·m   f·afm·mm   f·afj·sh  f·afj·ch  f·afj·j   f·afN·ng  f·afN·ngk          ¦" , &s_xpos, &s_ypos);
      PAGE_gregg (SHAPE_DRAW, "f·aff·f   f·aff·v   f·afp·p    f·afp·b   f·afk·k   f·afk·g    f·afr·r   f·afr·l   f·afR·rd  f·afR·ld  f·afs·s     f·afz·z  ¦"       , &s_xpos, &s_ypos);
      PAGE_gregg (SHAPE_DRAW, "f·afH·ht  f·afH·nt  f·afH·nd  f·afT·th  f·afT·tn  f·afT·tm    f·afD·df  f·afP·bd   ¦"       , &s_xpos, &s_ypos);
   } glPopMatrix   ();
   /*---(page)---------------------------*/
   rc = PAGE_ready   ("/tmp/gregg_page.png");
   rc = PAGE_free    ();
   DEBUG_OUTP   yLOG_value   ("default"   , rc);
   --rce;  if (rc < 0) {
      DEBUG_OUTP   yLOG_exitr   (__FUNCTION__, rce);
      return rce;
   }
   /*---(complete)-----------------------*/
   DEBUG_OUTP   yLOG_exit    (__FUNCTION__);
   return 0;
}

char
PAGE_demo_cat_p         (void)
{
   /*---(locals)-----------+-----+-----+-*/
   char        rce         =  -10;
   char        rc          =    0;
   /*---(header)-------------------------*/
   DEBUG_OUTP   yLOG_enter   (__FUNCTION__);
   /*---(page)---------------------------*/
   rc = PAGE_new ('-'        , 1366   , 768  , '/', 2.0,   -1, -1, -1, -1,   3.0, YF_MIDCEN, 10, 10, 50, GAP_GRID);
   DEBUG_OUTP   yLOG_value   ("default"   , rc);
   --rce;  if (rc < 0) {
      DEBUG_OUTP   yLOG_exitr   (__FUNCTION__, rce);
      return rce;
   }
   /*---(page)---------------------------*/
   glPushMatrix    (); {
      s_xpos = my.p_left + (my.p_spacing * 0.5);
      s_ypos = s_ybase;
      PAGE_gregg (SHAPE_DRAW, "axp·p  p·apd·t   p·apd·d   p·apd·dd   p·apm·n   p·apm·m   p·apm·mm   p·apj·sh  p·apj·ch  p·apj·j   p·apN·ng  p·apN·ngk          ¦" , &s_xpos, &s_ypos);
      PAGE_gregg (SHAPE_DRAW, "p·apf·f   p·apf·v   p·app·p    p·app·b   p·apk·k   p·apk·g    p·apr·r   p·apr·l   p·apR·rd  p·apR·ld  p·aps·s     p·apz·z  ¦"       , &s_xpos, &s_ypos);
      PAGE_gregg (SHAPE_DRAW, "p·apH·ht  p·apH·nt  p·apH·nd  p·apT·th  p·apT·tn  p·apT·tm    p·apD·df  p·apP·bd   ¦"       , &s_xpos, &s_ypos);
   } glPopMatrix   ();
   /*---(page)---------------------------*/
   rc = PAGE_ready   ("/tmp/gregg_page.png");
   rc = PAGE_free    ();
   DEBUG_OUTP   yLOG_value   ("default"   , rc);
   --rce;  if (rc < 0) {
      DEBUG_OUTP   yLOG_exitr   (__FUNCTION__, rce);
      return rce;
   }
   /*---(complete)-----------------------*/
   DEBUG_OUTP   yLOG_exit    (__FUNCTION__);
   return 0;
}

char
PAGE_demo_cat_u         (void)
{
   /*---(locals)-----------+-----+-----+-*/
   char        rce         =  -10;
   char        rc          =    0;
   /*---(header)-------------------------*/
   DEBUG_OUTP   yLOG_enter   (__FUNCTION__);
   /*---(page)---------------------------*/
   rc = PAGE_new ('-'        , 1366   , 768  , '/', 1.0,   -1, -1, -1, -1,   1.0, YF_MIDCEN, 20, 20, 50, GAP_GRID);
   DEBUG_OUTP   yLOG_value   ("default"   , rc);
   --rce;  if (rc < 0) {
      DEBUG_OUTP   yLOG_exitr   (__FUNCTION__, rce);
      return rce;
   }
   /*---(page)---------------------------*/
   glPushMatrix    (); {
      s_xpos = my.p_left + (my.p_spacing * 0.5);
      s_ypos = s_ybase;
      PAGE_gregg (SHAPE_DRAW, "o1 o2 £ o3 o4 o5 £ o7 o8 o9  ¦" , &s_xpos, &s_ypos);
      PAGE_gregg (SHAPE_DRAW, "r·o·b  sh·t·/·nd  m·o·r  /·e·d  /·a·t  /·e·j     ¦" , &s_xpos, &s_ypos);
   } glPopMatrix   ();
   /*---(page)---------------------------*/
   rc = PAGE_ready   ("/tmp/gregg_page.png");
   rc = PAGE_free    ();
   DEBUG_OUTP   yLOG_value   ("default"   , rc);
   --rce;  if (rc < 0) {
      DEBUG_OUTP   yLOG_exitr   (__FUNCTION__, rce);
      return rce;
   }
   /*---(complete)-----------------------*/
   DEBUG_OUTP   yLOG_exit    (__FUNCTION__);
   return 0;
}

char
PAGE_demo_dict          (void)
{
   /*---(locals)-----------+-----+-----+-*/
   char        rce         =  -10;
   char        rc          =    0;
   tWORD      *x_word      = NULL;
   int         i           =    0;
   /*---(header)-------------------------*/
   DEBUG_OUTP   yLOG_enter   (__FUNCTION__);
   /*---(page)---------------------------*/
   rc = PAGE_new (STYLE_HINTS, 1366   , 768  , '/', 2.0,   -1, -1, -1, -1,   1.0, YF_MIDCEN, 20, 20, 50, GAP_GRID);
   DEBUG_OUTP   yLOG_value   ("default"   , rc);
   --rce;  if (rc < 0) {
      DEBUG_OUTP   yLOG_exitr   (__FUNCTION__, rce);
      return rce;
   }
   /*---(page)---------------------------*/
   glPushMatrix    (); {
      s_xpos = my.p_left + (my.p_spacing * 0.5);
      s_ypos = s_ybase;
      WORDS_eng_by_index (i, &x_word);
      while (x_word != NULL) {
         printf ("%3d, %s, %s\n", i, x_word->w_english, x_word->w_gregg);
         PAGE_gregg (SHAPE_DRAW, x_word->w_gregg, &s_xpos, &s_ypos);
         WORDS_eng_by_index (++i, &x_word);
         if (i % 10 == 0)  PAGE_gregg (SHAPE_DRAW, "¦", &s_xpos, &s_ypos);
         if (i > 50)  break;
      }
   } glPopMatrix   ();
   /*---(page)---------------------------*/
   rc = PAGE_ready   ("/tmp/gregg_page.png");
   rc = PAGE_free    ();
   DEBUG_OUTP   yLOG_value   ("default"   , rc);
   --rce;  if (rc < 0) {
      DEBUG_OUTP   yLOG_exitr   (__FUNCTION__, rce);
      return rce;
   }
   /*---(complete)-----------------------*/
   DEBUG_OUTP   yLOG_exit    (__FUNCTION__);
   return 0;
}

char
PAGE_demo               (void)
{
   /*---(locals)-----------+-----+-----+-*/
   char        rce         =  -10;
   char        rc          =    0;
   float       x           =   20;
   float       i           =   45;
   char       *p           = NULL;
   char       *q           =  "·";
   char       *r           = NULL;
   char        x_cons      [LEN_DESC] = "t·d·dd·th·tn·tm·ht·nt·nd·ds·df·n·m·mm·k·g·r·l·rd·ld·ng·ngk·sh·ch·j·s·f·v·z·p·b·pt·bd";
   /*---(header)-------------------------*/
   DEBUG_OUTP   yLOG_enter   (__FUNCTION__);
   /*---(page)---------------------------*/
   rc = PAGE_new ('-'        , 1366   , 768  , '/', 2.0,   -1, -1, -1, -1,   3.0, YF_MIDCEN, 10, 10, 50, GAP_GRID);
   DEBUG_OUTP   yLOG_value   ("default"   , rc);
   --rce;  if (rc < 0) {
      DEBUG_OUTP   yLOG_exitr   (__FUNCTION__, rce);
      return rce;
   }
   /*---(page)---------------------------*/
   glPushMatrix    (); {
      s_xpos = my.p_left + (my.p_spacing * 0.5);
      s_ypos = s_ybase;
      PAGE_gregg (SHAPE_DRAW, "t      d      dd      n      m      mm      sh     ch     j      ng       ngk        ¦" , &s_xpos, &s_ypos);
      PAGE_gregg (SHAPE_DRAW, "f      v      p       b      k      g       r      l      rd     ld       s       z  ¦"       , &s_xpos, &s_ypos);
      PAGE_gregg (SHAPE_DRAW, "th     tn     tm      ht     nt     nd      ts     df     pt     bd       xs      xz ¦"       , &s_xpos, &s_ypos);
      /*> PAGE_gregg (SHAPE_DRAW, "ad·t   ad·d   ad·dd   am·n   am·m   am·mm   aj·sh  aj·ch  aj·j   angk·ng  angk·ngk   ¦" , &s_xpos, &s_ypos);   <*/
      /*> PAGE_gregg (SHAPE_DRAW, "av·f   av·v   ab·p    ab·b   ag·k   ag·g    al·r   al·l   al·rd  al·ld    as·s       ¦"       , &s_xpos, &s_ypos);   <*/
      /*> PAGE_gregg (SHAPE_DRAW, "k·r    £      g·l     £      r·k    £       l·g    ¦"       , &s_xpos, &s_ypos);   <*/
      /*> PAGE_gregg (SHAPE_DRAW, "p·r    £      p·l     £      b·r    £       b·l    £      s·z    £        z·s        ¦"       , &s_xpos, &s_ypos);   <*/
      /*> PAGE_gregg (SHAPE_DRAW, "k·p    £      g·p     £      f·r    £       f·l    ¦"       , &s_xpos, &s_ypos);   <*/
      /*> PAGE_gregg (SHAPE_DRAW, "l·z    £      z·l     £      z·r    ¦"       , &s_xpos, &s_ypos);   <*/
      /*> PAGE_gregg (SHAPE_DRAW, "th·s          ¦"       , &s_xpos, &s_ypos);        <*/
      /*> PAGE_gregg (SHAPE_DRAW, "ath·th  ath·tn  ath·tm      aht·ht  aht·nt  aht·nd     ¦"       , &s_xpos, &s_ypos);   <*/
      PAGE_gregg (SHAPE_DRAW, "t·adx  d·adx  dd·adx  n·amx  m·amx  mm·amx  sh·ajx  ch·ajx  j·ajx   ng·aNx  ngk·aNx   ¦" , &s_xpos, &s_ypos);
      PAGE_gregg (SHAPE_DRAW, "f·afx  v·afx   p·apx  b·apx  k·akx  g·akx   r·arx  l·arx  rd·aRx  ld·aRx              ¦"       , &s_xpos, &s_ypos);
      PAGE_gregg (SHAPE_DRAW, "th·aTx tn·aTx tm·aTx  ht·aHx nt·aHx nd·aHx  df·aDx  bd·aPx    s·asx   z·azx            ¦"       , &s_xpos, &s_ypos);
   } glPopMatrix   ();
   s_ybase -= 200;

   /*---(page)---------------------------*/
   /*> glPushMatrix    (); {                                                                    <* 
    *>    /+> yCOLOR_opengl (YCOLOR_BAS, YCOLOR_MOR, 1.00);                               <+/   <* 
    *>    /+---(d)---------------------------+/                                                 <* 
    *>    /+> my.p_align = YF_MIDCEN;                                                        <*    <* 
    *>     *> glTranslatef ( my.p_left + 25, s_ybase,  0.0);                                 <*    <* 
    *>     *> s_ypos  = s_ybase;                                                          <+/   <* 
    *>    PAGE_gregg_word (SHAPE_DRAW, "ad·t", &s_xpos, &s_ypos);                               <* 
    *>    s_xpos  = (x += i);                                                                   <* 
    *>    s_ypos  = s_ybase;                                                                    <* 
    *>    CREATE_letter (SHAPE_DRAW  , "ad"   , 1.0, &s_xpos, &s_ypos);                         <* 
    *>    CREATE_letter (SHAPE_DRAW  , "d"    , 1.0, &s_xpos, &s_ypos);                         <* 
    *>    /+> s_xpos += my.p_spacing;                                                        <+/   <* 
    *>    s_xpos  = (x += i);                                                                   <* 
    *>    s_ypos  = s_ybase;                                                                    <* 
    *>    CREATE_letter (SHAPE_DRAW  , "ad"   , 1.0, &s_xpos, &s_ypos);                         <* 
    *>    CREATE_letter (SHAPE_DRAW  , "dd"   , 1.0, &s_xpos, &s_ypos);                         <* 
    *>    /+> s_xpos += my.p_spacing;                                                        <+/   <* 
    *>    s_xpos  = (x += i);                                                                   <* 
    *>    s_ypos  = s_ybase;                                                                    <* 
    *>    /+---(m)---------------------------+/                                                 <* 
    *>    CREATE_letter (SHAPE_DRAW  , "am"   , 1.0, &s_xpos, &s_ypos);                         <* 
    *>    CREATE_letter (SHAPE_DRAW  , "n"    , 1.0, &s_xpos, &s_ypos);                         <* 
    *>    /+> s_xpos += my.p_spacing;                                                        <+/   <* 
    *>    s_xpos  = (x += i);                                                                   <* 
    *>    s_ypos  = s_ybase;                                                                    <* 
    *>    CREATE_letter (SHAPE_DRAW  , "am"   , 1.0, &s_xpos, &s_ypos);                         <* 
    *>    CREATE_letter (SHAPE_DRAW  , "m"    , 1.0, &s_xpos, &s_ypos);                         <* 
    *>    /+> s_xpos += my.p_spacing;                                                        <+/   <* 
    *>    s_xpos  = (x += i);                                                                   <* 
    *>    s_ypos  = s_ybase;                                                                    <* 
    *>    CREATE_letter (SHAPE_DRAW  , "am"   , 1.0, &s_xpos, &s_ypos);                         <* 
    *>    CREATE_letter (SHAPE_DRAW  , "mm"   , 1.0, &s_xpos, &s_ypos);                         <* 
    *>    /+> s_xpos += my.p_spacing;                                                        <+/   <* 
    *>    s_xpos  = (x += i);                                                                   <* 
    *>    s_ypos  = s_ybase;                                                                    <* 
    *>    /+---(g)---------------------------+/                                                 <* 
    *>    CREATE_letter (SHAPE_DRAW  , "ag"   , 1.0, &s_xpos, &s_ypos);                         <* 
    *>    CREATE_letter (SHAPE_DRAW  , "k"    , 1.0, &s_xpos, &s_ypos);                         <* 
    *>    /+> s_xpos += my.p_spacing;                                                        <+/   <* 
    *>    s_xpos  = (x += i);                                                                   <* 
    *>    s_ypos  = s_ybase;                                                                    <* 
    *>    CREATE_letter (SHAPE_DRAW  , "ag"   , 1.0, &s_xpos, &s_ypos);                         <* 
    *>    CREATE_letter (SHAPE_DRAW  , "g"    , 1.0, &s_xpos, &s_ypos);                         <* 
    *>    /+> s_xpos += my.p_spacing;                                                        <+/   <* 
    *>    s_xpos  = (x += i);                                                                   <* 
    *>    s_ypos  = s_ybase;                                                                    <* 
    *>    /+---(l)---------------------------+/                                                 <* 
    *>    CREATE_letter (SHAPE_DRAW  , "al"   , 1.0, &s_xpos, &s_ypos);                         <* 
    *>    CREATE_letter (SHAPE_DRAW  , "r"    , 1.0, &s_xpos, &s_ypos);                         <* 
    *>    /+> s_xpos += my.p_spacing;                                                        <+/   <* 
    *>    s_xpos  = (x += i);                                                                   <* 
    *>    s_ypos  = s_ybase;                                                                    <* 
    *>    CREATE_letter (SHAPE_DRAW  , "al"   , 1.0, &s_xpos, &s_ypos);                         <* 
    *>    CREATE_letter (SHAPE_DRAW  , "l"    , 1.0, &s_xpos, &s_ypos);                         <* 
    *>    /+> s_xpos += my.p_spacing + 25;                                                   <+/   <* 
    *>    s_xpos  = (x += i);                                                                   <* 
    *>    s_ypos  = s_ybase;                                                                    <* 
    *>    /+---(j)---------------------------+/                                                 <* 
    *>    CREATE_letter (SHAPE_DRAW  , "aj"   , 1.0, &s_xpos, &s_ypos);                         <* 
    *>    CREATE_letter (SHAPE_DRAW  , "j"    , 1.0, &s_xpos, &s_ypos);                         <* 
    *>    /+> s_xpos += my.p_spacing + 25;                                                   <+/   <* 
    *>    s_xpos  = (x += i);                                                                   <* 
    *>    s_ypos  = s_ybase;                                                                    <* 
    *>    CREATE_letter (SHAPE_DRAW  , "aj"   , 1.0, &s_xpos, &s_ypos);                         <* 
    *>    CREATE_letter (SHAPE_DRAW  , "ch"   , 1.0, &s_xpos, &s_ypos);                         <* 
    *>    /+> s_xpos += my.p_spacing + 25;                                                   <+/   <* 
    *>    s_xpos  = (x += i);                                                                   <* 
    *>    s_ypos  = s_ybase;                                                                    <* 
    *>    CREATE_letter (SHAPE_DRAW  , "aj"   , 1.0, &s_xpos, &s_ypos);                         <* 
    *>    CREATE_letter (SHAPE_DRAW  , "sh"   , 1.0, &s_xpos, &s_ypos);                         <* 
    *>    /+> s_xpos += my.p_spacing + 25;                                                   <+/   <* 
   *>    s_xpos  = (x += i);                                                                   <* 
      *>    s_ypos  = s_ybase;                                                                    <* 
      *>    /+---(v)---------------------------+/                                                 <* 
      *>    CREATE_letter (SHAPE_DRAW  , "av"   , 1.0, &s_xpos, &s_ypos);                         <* 
      *>    CREATE_letter (SHAPE_DRAW  , "f"    , 1.0, &s_xpos, &s_ypos);                         <* 
      *>    /+> s_xpos += my.p_spacing + 25;                                                   <+/   <* 
      *>    s_xpos  = (x += i);                                                                   <* 
      *>    s_ypos  = s_ybase;                                                                    <* 
      *>    CREATE_letter (SHAPE_DRAW  , "av"   , 1.0, &s_xpos, &s_ypos);                         <* 
      *>    CREATE_letter (SHAPE_DRAW  , "v"    , 1.0, &s_xpos, &s_ypos);                         <* 
      *>    /+> s_xpos += my.p_spacing + 25;                                                   <+/   <* 
      *>    s_xpos  = (x += i);                                                                   <* 
      *>    s_ypos  = s_ybase;                                                                    <* 
      *>    /+---(b)---------------------------+/                                                 <* 
      *>    CREATE_letter (SHAPE_DRAW  , "ab"   , 1.0, &s_xpos, &s_ypos);                         <* 
      *>    CREATE_letter (SHAPE_DRAW  , "p"    , 1.0, &s_xpos, &s_ypos);                         <* 
      *>    /+> s_xpos += my.p_spacing + 25;                                                   <+/   <* 
      *>    s_xpos  = (x += i);                                                                   <* 
      *>    s_ypos  = s_ybase;                                                                    <* 
      *>    CREATE_letter (SHAPE_DRAW  , "ab"   , 1.0, &s_xpos, &s_ypos);                         <* 
      *>    CREATE_letter (SHAPE_DRAW  , "b"    , 1.0, &s_xpos, &s_ypos);                         <* 
      *>    /+> s_xpos += my.p_spacing + 25;                                                   <+/   <* 
      *>    s_xpos  = (x += i);                                                                   <* 
      *>    s_ypos  = s_ybase;                                                                    <* 
      *> } glPopMatrix   ();                                                                      <*/
      /*---(page)---------------------------*/
      rc = PAGE_ready   ("/tmp/gregg_page.png");
   rc = PAGE_free    ();
   DEBUG_OUTP   yLOG_value   ("default"   , rc);
   --rce;  if (rc < 0) {
      DEBUG_OUTP   yLOG_exitr   (__FUNCTION__, rce);
      return rce;
   }
   /*---(complete)-----------------------*/
   DEBUG_OUTP   yLOG_exit    (__FUNCTION__);
   return 0;
}



