/*============================---(source-start)---============================*/
#include    "gregg.h"


static char      s_style    =  '-';
static short     s_wide     =  850;
static short     s_tall     = 1100;
static short     s_left     =   30;
static short     s_right    =   30;
static short     s_top      =   20;
static short     s_bottom   =   30;
static short     s_ascent   =   30;
static short     s_descent  =   30;
static short     s_spacing  =   10;
static short     s_linesize =   50;


static uint      s_tex      =  -1;
static uint      s_fbo      =  -1;
static uint      s_depth    =  -1;


static float     s_xpos     =  30;
static float     s_ybase    = -50;
static float     s_ypos     = -50;



/*============================--------------------============================*/
/*===----                         page handling                        ----===*/
/*============================--------------------============================*/
static void o___MEMORY____________________o (void) {;}

char
PAGE_new                (char a_style, short a_wide, short a_tall, short a_left, short a_right, short a_top, short a_bottom, short a_ascent, short a_descent, short a_spacing)
{
   /*---(locals)-----------+-----+-----+-*/
   char        rce         =  -10;
   char        rc          =    0;
   int         n           =    0;
   int         x, y;
   /*---(header)-------------------------*/
   DEBUG_OUTP   yLOG_enter   (__FUNCTION__);
   DEBUG_OUTP   yLOG_complex ("args"      , "%c   %4dw  %4dt   %4dl %4dr   %4dt %4db   %4da %4dd   %4dm", a_style, a_wide, a_tall, a_left, a_right, a_top, a_bottom, a_ascent, a_descent, a_spacing);
   /*---(free memory)--------------------*/
   DEBUG_OUTP   yLOG_complex ("pointers"  , "%4d tex, %4d fbo, %4d depth", s_tex, s_fbo, s_depth);
   --rce;  if (s_tex >= 0) {
      DEBUG_OUTP   yLOG_note    ("must clear texture first");
      rc = yGLTEX_free    (&s_tex, &s_fbo, &s_depth);
      DEBUG_OUTP   yLOG_value   ("free"      , rc);
      if (rc < 0) {
         DEBUG_OUTP   yLOG_exitr   (__FUNCTION__, rce);
         return rce;
      }
   }
   /*---(change configuration -----------*/
   DEBUG_OUTP   yLOG_complex ("before"    , "%c   %4dw  %4dt   %4dl %4dr   %4dt %4db   %4da %4dd   %4dm", s_style, s_wide, s_tall, s_left, s_right, s_top, s_bottom, s_ascent, s_descent, s_spacing);
   /*---(accept style)-------------------*/
   if      (a_style != '-')  s_style   = a_style;
   /*---(accept sizes)-------------------*/
   if      (a_wide    <  0)  s_wide    =  850;
   else if (a_wide     > 0)  s_wide    = a_wide;
   if      (a_tall    <  0)  s_tall    = 1100;
   else if (a_tall     > 0)  s_tall    = a_tall;
   /*---(accept margins)-----------------*/
   if      (a_left    <  0)  s_left    =   20;
   else if (a_left     > 0)  s_left    = a_left;
   if      (a_right   <  0)  s_right   =   30;
   else if (a_right    > 0)  s_right   = a_right;
   if      (a_top     <  0)  s_top     =   20;
   else if (a_top      > 0)  s_top     = a_top;
   if      (a_bottom  <  0)  s_bottom  =   30;
   else if (a_bottom   > 0)  s_bottom  = a_bottom;
   /*---(accept lines)-------------------*/
   if      (a_ascent  <  0)  s_ascent  =   30;
   else if (a_ascent   > 0)  s_ascent  = a_ascent;
   if      (a_descent <  0)  s_descent =   30;
   else if (a_descent  > 0)  s_descent = a_descent;
   if      (a_spacing <  0)  s_spacing =   10;
   else if (a_spacing  > 0)  s_spacing = a_spacing;
   s_linesize    = s_ascent + s_descent;
   /*---(done configuration)-------------*/
   DEBUG_OUTP   yLOG_complex ("after"     , "%c   %4dw  %4dt   %4dl %4dr   %4dt %4db   %4da %4dd   %4dm", s_style, s_wide, s_tall, s_left, s_right, s_top, s_bottom, s_ascent, s_descent, s_spacing);
   /*---(prepare)------------------------*/
   CREATE_draw_reset  (0, SHAPE_DRAW);
   s_xpos      = s_left;
   s_ybase     = - (s_top + s_ascent);
   s_ypos      = s_ybase;
   DEBUG_OUTP   yLOG_complex ("position"  , "%4.0f xpos, %4.0f ybase, %4.0f ypos", s_xpos, s_ybase, s_ypos);
   /*---(create texture)-----------------*/
   yCOLOR_opengl_clear (YCOLOR_BAS, YCOLOR_MAX);
   rc = yGLTEX_new (&s_tex, &s_fbo, &s_depth, s_wide, s_tall);
   DEBUG_OUTP   yLOG_value   ("new"       , rc);
   DEBUG_OUTP   yLOG_complex ("pointers"  , "%4d tex, %4d fbo, %4d depth", s_tex, s_fbo, s_depth);
   --rce;  if (rc < 0) {
      DEBUG_OUTP   yLOG_exitr   (__FUNCTION__, rce);
      return rce;
   }
   rc = yGLTEX_draw         (s_fbo, YGLTEX_TOPLEF, s_wide, s_tall, 1.0);
   DEBUG_OUTP   yLOG_value   ("draw"      , rc);
   --rce;  if (rc < 0) {
      DEBUG_OUTP   yLOG_exitr   (__FUNCTION__, rce);
      return rce;
   }
   /*---(complete)-----------------------*/
   DEBUG_OUTP   yLOG_exit    (__FUNCTION__);
   return 0;
}

char PAGE_default       (void)                        { return PAGE_new ('-', -1, -1, -1, -1, -1, -1, -1, -1, -1); }
char PAGE_sized         (short a_wide, short a_tall)  { return PAGE_new ('-', a_wide, a_tall, -1, -1, -1, -1, -1, -1, -1); }
char PAGE_screen        (void)                        { return PAGE_new ('-', 1366, 768, -1, -1, -1, -1, -1, -1, -1); }

char
PAGE_end                (void)
{
   /*---(locals)-----------+-----+-----+-*/
   char        rce         =  -10;
   char        rc          =    0;
   /*---(header)-------------------------*/
   DEBUG_OUTP   yLOG_enter   (__FUNCTION__);
   /*---(write out image)----------------*/
   --rce;  rc = yGLTEX_tex2png ("/tmp/gregg.png", s_wide, s_tall);
   DEBUG_OUTP   yLOG_value   ("tex2png"   , rc);
   if (rc < 0) {
      DEBUG_OUTP   yLOG_exitr   (__FUNCTION__, rce);
      return rce;
   }
   /*---(close texture)------------------*/
   rc = yGLTEX_done    (s_tex);
   DEBUG_OUTP   yLOG_value   ("done"      , rc);
   --rce;  if (rc < 0) {
      DEBUG_OUTP   yLOG_exitr   (__FUNCTION__, rce);
      return rce;
   }
   /*---(free memory)--------------------*/
   rc = yGLTEX_free    (&s_tex, &s_fbo, &s_depth);
   DEBUG_OUTP   yLOG_value   ("free"      , rc);
   --rce;  if (rc < 0) {
      DEBUG_OUTP   yLOG_exitr   (__FUNCTION__, rce);
      return rce;
   }
   /*---(complete)-----------------------*/
   DEBUG_OUTP   yLOG_exit    (__FUNCTION__);
   return 0;
}



/*============================--------------------============================*/
/*===----                         page handling                        ----===*/
/*============================--------------------============================*/
static void o___LETTERS____________________o (void) {;}

char
PAGE_letter             (char a_act, uchar *a_ltr)
{
   /*---(locals)-----------+-----+-----+-*/
   char        rce         =  -10;
   char        rc          =    0;
   short       n           =    0;
   char        x_label     [LEN_SHORT] = "";
   /*---(header)-------------------------*/
   DEBUG_OUTP   yLOG_enter   (__FUNCTION__);
   n = CREATE_find_by_name (a_ltr, LTRS_ALL, NULL, NULL, x_label, NULL, NULL, NULL, NULL, NULL, NULL);
   DEBUG_OUTP   yLOG_value   ("current"   , n);
   --rce;  if (n < 0) {
      DEBUG_OUTP   yLOG_exitr   (__FUNCTION__, rce);
      return rce;
   }
   DEBUG_OUTP   yLOG_info    ("showing"   , x_label);
   /*---(show letter)--------------------*/
   if (a_act == SHAPE_SAMPLE)   glCallList   (dl_dotted  + n);
   else                         glCallList   (dl_solid   + n);
   glTranslatef (g_loc [n].x_end, g_loc [n].y_end,  0.0);
   /*---(complete)-----------------------*/
   DEBUG_OUTP   yLOG_exit    (__FUNCTION__);
   return 0;
}

char
PAGE_gregg              (char a_act, char a_gregg [LEN_TITLE])
{
   /*---(locals)-----------+-----+-----+-*/
   char        rce         =  -10;
   char        rc          =    0;
   uchar       x_gregg     [LEN_TITLE] = "";
   char       *p           = NULL;
   char       *q           =  "·";
   char       *r           = NULL;
   short       n           =    0;
   /*---(header)-------------------------*/
   DEBUG_OUTP   yLOG_enter   (__FUNCTION__);
   /*---(defense)------------------------*/
   DEBUG_CONF   yLOG_point   ("a_gregg"   , a_gregg);
   --rce;  if (a_gregg == NULL) {
      DEBUG_CONF   yLOG_exitr   (__FUNCTION__, rce);
      return rce;
   }
   /*---(prepare)------------------------*/
   strlcpy (x_gregg, a_gregg, LEN_TITLE);
   /*---(begin parse)--------------------*/
   p = strtok_r (x_gregg, q, &r);
   DEBUG_CONF   yLOG_point   ("p"         , p);
   if (p == NULL) {
      DEBUG_CONF   yLOG_exitr   (__FUNCTION__, rce);
      return rce;
   }
   /*---(back to defaults)---------------*/
   glColor4f (0.0, 0.0, 0.0, 1.0);
   /*---(walk letters)-------------------*/
   glPushMatrix(); {
      --rce;  while (p != NULL) {
         DEBUG_CONF   yLOG_info    ("p"         , p);
         if (strcmp (p, ">") == 0)   glColor4f (0.7, 0.0, 0.0, 1.0);
         rc =  PAGE_letter (a_act, p);
         p = strtok_r (NULL   , q, &r);
         DEBUG_CONF   yLOG_point   ("p"         , p);
      }
   } glPopMatrix();
   /*---(back to defaults)---------------*/
   glColor4f (0.0, 0.0, 0.0, 1.0);
   /*---(complete)-----------------------*/
   DEBUG_OUTP   yLOG_exit    (__FUNCTION__);
   return 0;
}



/*============================--------------------============================*/
/*===----                         page handling                        ----===*/
/*============================--------------------============================*/
static void o___DRIVER_____________________o (void) {;}

char
PAGE_demo               (void)
{
   /*---(locals)-----------+-----+-----+-*/
   char        rce         =  -10;
   char        rc          =    0;
   float       x           =   20;
   float       i           =   45;
   /*---(header)-------------------------*/
   DEBUG_OUTP   yLOG_enter   (__FUNCTION__);
   /*---(page)---------------------------*/
   rc = PAGE_screen ();
   DEBUG_OUTP   yLOG_value   ("default"   , rc);
   --rce;  if (rc < 0) {
      DEBUG_OUTP   yLOG_exitr   (__FUNCTION__, rce);
      return rce;
   }
   /*---(page)---------------------------*/
   /*> glPushMatrix    (); {                                                                   <* 
    *>    /+> glLineWidth  (1.5);                                                        <+/   <* 
    *>    /+> glPointSize  (2.0);                                                        <+/   <* 
    *>    glColor4f    (0.0f, 0.0f, 0.0f, 0.7f);                                               <* 
    *>    glTranslatef (200, s_ybase,  0.0);                                                   <* 
    *>    /+> glCallList   (dl_solid  + 2);                                              <+/   <* 
    *>    PAGE_letter (SHAPE_TRAIN, "l");                                                      <* 
    *>    PAGE_letter (SHAPE_TRAIN, "j");                                                      <* 
    *> } glPopMatrix   ();                                                                     <*/
   glTranslatef (200, s_ybase,  0.0);
   PAGE_gregg (SHAPE_DRAW, "l·j");
   glTranslatef (200, 0.0    ,  0.0);
   PAGE_gregg (SHAPE_DRAW, "ad·d·sh");
   glTranslatef (200, 0.0    ,  0.0);
   PAGE_gregg (SHAPE_DRAW, "ad·d·sh·>·u");
   s_ybase -= 100;
   /*---(page)---------------------------*/
   glPushMatrix    (); {
      /*> yCOLOR_opengl (YCOLOR_BAS, YCOLOR_MOR, 1.00);                               <*/
      /*---(d)---------------------------*/
      s_xpos  = (x += i);
      s_ypos  = s_ybase;
      CREATE_letter (SHAPE_DRAW  , "ad"   , &s_xpos, &s_ypos);
      CREATE_letter (SHAPE_DRAW  , "t"    , &s_xpos, &s_ypos);
      /*> s_xpos += s_spacing;                                                        <*/
      s_xpos  = (x += i);
      s_ypos  = s_ybase;
      CREATE_letter (SHAPE_DRAW  , "ad"   , &s_xpos, &s_ypos);
      CREATE_letter (SHAPE_DRAW  , "d"    , &s_xpos, &s_ypos);
      /*> s_xpos += s_spacing;                                                        <*/
      s_xpos  = (x += i);
      s_ypos  = s_ybase;
      CREATE_letter (SHAPE_DRAW  , "ad"   , &s_xpos, &s_ypos);
      CREATE_letter (SHAPE_DRAW  , "dd"   , &s_xpos, &s_ypos);
      /*> s_xpos += s_spacing;                                                        <*/
      s_xpos  = (x += i);
      s_ypos  = s_ybase;
      /*---(m)---------------------------*/
      CREATE_letter (SHAPE_DRAW  , "am"   , &s_xpos, &s_ypos);
      CREATE_letter (SHAPE_DRAW  , "n"    , &s_xpos, &s_ypos);
      /*> s_xpos += s_spacing;                                                        <*/
      s_xpos  = (x += i);
      s_ypos  = s_ybase;
      CREATE_letter (SHAPE_DRAW  , "am"   , &s_xpos, &s_ypos);
      CREATE_letter (SHAPE_DRAW  , "m"    , &s_xpos, &s_ypos);
      /*> s_xpos += s_spacing;                                                        <*/
      s_xpos  = (x += i);
      s_ypos  = s_ybase;
      CREATE_letter (SHAPE_DRAW  , "am"   , &s_xpos, &s_ypos);
      CREATE_letter (SHAPE_DRAW  , "mm"   , &s_xpos, &s_ypos);
      /*> s_xpos += s_spacing;                                                        <*/
      s_xpos  = (x += i);
      s_ypos  = s_ybase;
      /*---(g)---------------------------*/
      CREATE_letter (SHAPE_DRAW  , "ag"   , &s_xpos, &s_ypos);
      CREATE_letter (SHAPE_DRAW  , "k"    , &s_xpos, &s_ypos);
      /*> s_xpos += s_spacing;                                                        <*/
      s_xpos  = (x += i);
      s_ypos  = s_ybase;
      CREATE_letter (SHAPE_DRAW  , "ag"   , &s_xpos, &s_ypos);
      CREATE_letter (SHAPE_DRAW  , "g"    , &s_xpos, &s_ypos);
      /*> s_xpos += s_spacing;                                                        <*/
      s_xpos  = (x += i);
      s_ypos  = s_ybase;
      /*---(l)---------------------------*/
      CREATE_letter (SHAPE_DRAW  , "al"   , &s_xpos, &s_ypos);
      CREATE_letter (SHAPE_DRAW  , "r"    , &s_xpos, &s_ypos);
      /*> s_xpos += s_spacing;                                                        <*/
      s_xpos  = (x += i);
      s_ypos  = s_ybase;
      CREATE_letter (SHAPE_DRAW  , "al"   , &s_xpos, &s_ypos);
      CREATE_letter (SHAPE_DRAW  , "l"    , &s_xpos, &s_ypos);
      /*> s_xpos += s_spacing + 25;                                                   <*/
      s_xpos  = (x += i);
      s_ypos  = s_ybase;
      /*---(j)---------------------------*/
      CREATE_letter (SHAPE_DRAW  , "aj"   , &s_xpos, &s_ypos);
      CREATE_letter (SHAPE_DRAW  , "j"    , &s_xpos, &s_ypos);
      /*> s_xpos += s_spacing + 25;                                                   <*/
      s_xpos  = (x += i);
      s_ypos  = s_ybase;
      CREATE_letter (SHAPE_DRAW  , "aj"   , &s_xpos, &s_ypos);
      CREATE_letter (SHAPE_DRAW  , "ch"   , &s_xpos, &s_ypos);
      /*> s_xpos += s_spacing + 25;                                                   <*/
      s_xpos  = (x += i);
      s_ypos  = s_ybase;
      CREATE_letter (SHAPE_DRAW  , "aj"   , &s_xpos, &s_ypos);
      CREATE_letter (SHAPE_DRAW  , "sh"   , &s_xpos, &s_ypos);
      /*> s_xpos += s_spacing + 25;                                                   <*/
      s_xpos  = (x += i);
      s_ypos  = s_ybase;
      /*---(v)---------------------------*/
      CREATE_letter (SHAPE_DRAW  , "av"   , &s_xpos, &s_ypos);
      CREATE_letter (SHAPE_DRAW  , "f"    , &s_xpos, &s_ypos);
      /*> s_xpos += s_spacing + 25;                                                   <*/
      s_xpos  = (x += i);
      s_ypos  = s_ybase;
      CREATE_letter (SHAPE_DRAW  , "av"   , &s_xpos, &s_ypos);
      CREATE_letter (SHAPE_DRAW  , "v"    , &s_xpos, &s_ypos);
      /*> s_xpos += s_spacing + 25;                                                   <*/
      s_xpos  = (x += i);
      s_ypos  = s_ybase;
      /*---(b)---------------------------*/
      CREATE_letter (SHAPE_DRAW  , "ab"   , &s_xpos, &s_ypos);
      CREATE_letter (SHAPE_DRAW  , "p"    , &s_xpos, &s_ypos);
      /*> s_xpos += s_spacing + 25;                                                   <*/
      s_xpos  = (x += i);
      s_ypos  = s_ybase;
      CREATE_letter (SHAPE_DRAW  , "ab"   , &s_xpos, &s_ypos);
      CREATE_letter (SHAPE_DRAW  , "b"    , &s_xpos, &s_ypos);
      /*> s_xpos += s_spacing + 25;                                                   <*/
      s_xpos  = (x += i);
      s_ypos  = s_ybase;
   } glPopMatrix   ();
   /*---(page)---------------------------*/
   rc = PAGE_end     ();
   DEBUG_OUTP   yLOG_value   ("default"   , rc);
   --rce;  if (rc < 0) {
      DEBUG_OUTP   yLOG_exitr   (__FUNCTION__, rce);
      return rce;
   }
   /*---(complete)-----------------------*/
   DEBUG_OUTP   yLOG_exit    (__FUNCTION__);
   return 0;
}



