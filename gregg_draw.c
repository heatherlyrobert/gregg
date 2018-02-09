/*============================---(source-start)---============================*/

#include "gregg.h"

const float  DEG2RAD = 3.1415927 / 180.0;
const float  RAD2DEG = 180.0 / 3.1415927;
int          g_transx = 0;
int          g_transy = 0;


/*---(sample texture)--------------------*/
int       sample_w    =   400;                /* texture width  (samples)       */
int       sample_h    =   400;                /* texture height (samples)       */
uint      sample_tex  =     0;                /* texture for image              */
uint      sample_fbo  =     0;                /* framebuffer                    */
uint      sample_dep  =     0;                /* depth buffer                   */


char      draw_bands           (void);


/*---(opengl objects)--------------------*/
static uint s_tex       =     0;            /* texture for image              */
static uint s_fbo       =     0;            /* framebuffer                    */
static uint s_depth     =     0;            /* depth buffer                   */



char DRAW_cursor        (void);



/*====================------------------------------------====================*/
/*===----                       program level                          ----===*/
/*====================------------------------------------====================*/
static void      o___PROGRAM_________________o (void) {;}

char
DRAW__sizes        (cint a_wide, cint a_tall)
{
   /*---(locals)-----------+-----+-----+-*/
   /*---(header)----------------------*/
   DEBUG_GRAF   yLOG_enter   (__FUNCTION__);
   /*---(main)---------------------------*/
   my.x_scale   = 31500;
   my.y_scale   = 19700;
   /*> my.x_min     =  -125;                                                          <* 
    *> my.y_min     = 125 - 350;                                                      <*/
   /*---(detailed text)------------------*/
   win.d_xoff   =   200;
   win.d_yoff   =   100;
   win.d_zoff   =   100;
   win.d_point  =    10;
   win.d_bar    =    20;
   win.d_ansx   =   200;
   win.d_ansy   =  -160;
   yVIKEYS_view_bounds   (YVIKEYS_MAIN  , &my.x_min, &my.x_max , &my.y_min, &my.y_max );
   yVIKEYS_view_coords   (YVIKEYS_MAIN  , NULL     , &my.x_wide, NULL     , &my.y_tall);
   my.x_full    = my.x_wide + 40;
   my.x_center  = abs (my.x_min) / (float) my.x_full;
   my.y_center  = abs (my.y_min) / (float) a_tall;
   my.ratio     = my.x_scale  / (float) my.x_full;
   /*---(texture)------------------------*/
   win.tex_h    = a_tall * 2.0;
   win.tex_w    = a_wide * 2.0;
   /*---(readout)------------------------*/
   DEBUG_GRAF   yLOG_complex ("data"      , "xoff %3d, yoff %3d, zoff %3d, pnt  %3d", win.d_xoff, win.d_yoff, win.d_zoff, win.d_point);
   DEBUG_GRAF   yLOG_complex ("answer"    , "ansx %3d, ansy %3d, bar  %3d"          , win.d_ansx, win.d_ansy, win.d_bar);
   /*---(complete)-----------------------*/
   DEBUG_GRAF   yLOG_exit    (__FUNCTION__);
   return 0;
}

char         /*-> tbd --------------------------------[ shoot  [gz.742.001.01]*/ /*-[00.0000.112.!]-*/ /*-[--.---.---.--]-*/
DRAW_init            (void)
{
   /*---(locals)-----------+-----+-----+-*/
   char        rc          =    0;
   int         x_wide, x_tall;
   /*---(header)-------------------------*/
   DEBUG_GRAF   yLOG_enter   (__FUNCTION__);
   /*---(stuff)--------------------------*/
   yVIKEYS_view_init     ("gregg shorthand interpreter", VER_NUM, 500, 350, 0);
   yVIKEYS_cmds_direct   (":palette 190 rcomp pale earthy");
   yVIKEYS_view_setup    (YVIKEYS_MAIN     , YVIKEYS_FLAT, YVIKEYS_TOPLEF, -125, 500, 125 - 350, 350, 0, 0, YCOLOR_BAS    , DRAW_primary);
   yVIKEYS_view_colors   (YCOLOR_POS, YCOLOR_BAS, YCOLOR_NEG, YCOLOR_POS);
   yVIKEYS_cmds_direct   (":ribbon show");
   DRAW__sizes           (500, 350);
   yGLTEX_init     ();
   yVIKEYS_view_color_clear (YCOLOR_BAS_MED);
   FONT__load      ();
   dlist_init ();
   yVIKEYS_view_ribbon ("play"    , "dj"          );
   yVIKEYS_view_ribbon ("talk"    , "video_cam"   );
   yVIKEYS_view_ribbon ("align"   , "layers"      );
   yVIKEYS_view_ribbon ("draw"    , "resolution"  );
   yVIKEYS_view_ribbon ("tools"   , "shower"      );
   yGLTEX_new (&s_tex, &s_fbo, &s_depth, win.tex_w, win.tex_h);
   yVIKEYS_view_option (YVIKEYS_OVERLAY, "data"  , OVERLAY_data    , "current point statistics");
   yVIKEYS_view_option (YVIKEYS_OVERLAY, "sample", OVERLAY_samples , "letter samples");
   yVIKEYS_layer_add   ("raw"     , LAYER_raws, "raw data points"     );
   yVIKEYS_layer_add   ("bas"     , LAYER_base, "bas/avg data points" );
   yVIKEYS_layer_add   ("key"     , LAYER_keys, "key/crit data points");
   yVIKEYS_layer_add   ("cur"     , LAYER_curr, "current point"       );
   yVIKEYS_cmds_direct (":gridsize 1");
   yVIKEYS_cmds_direct (":layer bas");
   yVIKEYS_cmds_direct (":layer cur");
   yVIKEYS_cmds_direct (":overlay data");
   DRAW_back ();
   yVIKEYS_map_config  (YVIKEYS_RIGHT, MAP_mapper);
   /*---(complete)-----------------------*/
   DEBUG_GRAF   yLOG_exit    (__FUNCTION__);
   return 0;
}


char
DRAW_wrap            (void)
{
   /*---(locals)-----------+-----+-----+-*/
   char        rc          =    0;
   /*---(header)-------------------------*/
   DEBUG_GRAF   yLOG_enter   (__FUNCTION__);
   /*---(stuff)--------------------------*/
   rc = yGLTEX_free (&s_tex, &s_fbo, &s_depth);
   rc = FONT__free       ();
   rc = yVIKEYS_view_wrap ();
   /*---(complete)-----------------------*/
   DEBUG_GRAF   yLOG_exit    (__FUNCTION__);
   return 0;
}



/*====================------------------------------------====================*/
/*===----                        panel display                         ----===*/
/*====================------------------------------------====================*/
static void      o___PANELS__________________o (void) {;}

char
DRAW_primary         (float a_mag)
{
   char     x_on = 'y';
   int      x_left, x_bott, x_wide, x_tall;
   int      x_xmin, x_xmax;
   int      x_ymin, x_ymax;
   yVIKEYS_view_size     (YVIKEYS_MAIN, &x_left, &x_wide, &x_bott, &x_tall, NULL);
   yVIKEYS_view_bounds   (YVIKEYS_MAIN, &x_xmin, &x_xmax, &x_ymin, &x_ymax);
   /*> printf ("t %c %3d %3d %3d %3d %s\n", x_on, x_left, x_bott, x_wide, x_tall, x_text);   <*/
   /*---(setup view)---------------------*/
   glViewport      (x_left, x_bott, x_wide, x_tall);
   glMatrixMode    (GL_PROJECTION);
   glLoadIdentity  ();
   glOrtho         (x_xmin, x_xmax, x_ymin, x_ymax,  -500.0,   500.0);
   glMatrixMode    (GL_MODELVIEW);
   /*---(background)---------------------*/
   glPushMatrix    (); {
      glBindTexture(GL_TEXTURE_2D, s_tex);
      glBegin         (GL_POLYGON); {
         glTexCoord2d (    0.00 ,      1.00);
         glVertex3f  (x_xmin, x_ymax,  0.0f);
         glTexCoord2d (    1.00 ,      1.00);
         glVertex3f  (x_xmax, x_ymax,  0.0f);
         glTexCoord2d (    1.00 ,      0.00);
         glVertex3f  (x_xmax, x_ymin,  0.0f);
         glTexCoord2d (    0.00 ,      0.00);
         glVertex3f  (x_xmin, x_ymin,  0.0f);
      } glEnd   ();
      glBindTexture(GL_TEXTURE_2D, 0);
   } glPopMatrix   ();
   /*---(parts)--------------------------*/
   DRAW_cursor ();
   if (my.touch != 'y') {
      /*> DRAW_info_counts ();                                                        <*/
      /*> DRAW_info_base   ();                                                        <*/
      /*> DRAW_info_answer ();                                                        <*/
      /*> glPushMatrix    (); {                                                       <* 
       *>    glColor4f    (0.20f, 0.20f, 0.20f, 1.0f);                                <* 
       *>    glTranslatef (x_xmax - 50.0, x_ymin + 75.0,    0.0f);            <* 
       *>    yFONT_print  (win.font_bg,  12, YF_BOTRIG, my.f_full);                   <* 
       *> } glPopMatrix   ();                                                         <*/
      /*> glPushMatrix    (); {                                                       <* 
       *>    glColor4f    (0.00f, 0.00f, 0.00f, 1.0f);                                <* 
       *>    glTranslatef (x_xmin +  3.0, x_ymin + 23.0,    0.0f);            <* 
       *>    yFONT_print  (win.font_bg,  12, YF_BOTLEF, o.actual);                    <* 
       *>    glTranslatef (0.0              , -20.0            ,    0.0f);            <* 
       *>    yFONT_print  (win.font_bg,  12, YF_BOTLEF, o.word);                      <* 
       *> } glPopMatrix   ();                                                         <*/
      /*> glPushMatrix    (); {                                                       <* 
       *>    glColor4f    (1.00f, 1.00f, 1.00f, 1.0f);                                <* 
       *>    glTranslatef ( 25.0,  -25.0, 0.0);                                       <* 
       *>    WORDS_result ();                                                         <* 
       *> } glPopMatrix   ();                                                         <*/
   }
   /*---(complete)-----------------------*/
   return;
}

char
DRAW_back            (void)
{
   /*---(locals)-----------+-----------+-*/
   char        rce         =  -10;
   char        rc          =    0;
   int         i           =    0;
   int         j           =    0;
   int         x_xmin      =    0;
   int         x_xmax      =    0;
   int         x_ymin      =    0;
   int         x_ymax      =    0;
   /*> int         x_side      =    0;                                                <* 
    *> float       x_left      =    0;                                                <*/
   /*---(header)-------------------------*/
   DEBUG_GRAF   yLOG_enter   (__FUNCTION__);
   /*---(setup)--------------------------*/
   yVIKEYS_view_bounds   (YVIKEYS_MAIN, &x_xmin, &x_xmax, &x_ymin, &x_ymax);
   rc = yGLTEX_draw_start   (s_fbo, YGLTEX_GREGG, win.tex_w, win.tex_h, 2.0);
   /*---(draw)---------------------------*/
   glCallList (dl_back);
   /*> draw_horz   ();                                                                <*/
   /*> SHOW_SAMPLE  sample_show ();                                                   <*/
   /*---(ribbon)-------------------------*/
   /*> x_side = win.r_wide - 5;                                                       <* 
    *> x_left = x_side + (win.r_wide - x_side) / 2.0;                                 <*/
   /*> SHOW_PLAYER {                                                                  <* 
    *>    glPushMatrix    (); {                                                       <* 
    *>       glTranslatef  (x_xmin + 5, win.m_ymin + x_side + 40, 10.0);              <* 
    *>       glColor4f    (1.00f, 1.00f, 1.00f, 1.0f);                                <* 
    *>       glTranslatef  (x_side    , 0.0        ,  0.0);                           <* 
    *>       glTranslatef  (x_side    , 0.0        ,  0.0);                           <* 
    *>       glTranslatef  (x_side    , 0.0        ,  0.0);                           <* 
    *>       glTranslatef  (x_side    , 0.0        ,  0.0);                           <* 
    *>       glTranslatef  (x_side    , 0.0        ,  0.0);                           <* 
    *>       yFONT_icon ("play"    , "first"       , x_side);                         <* 
    *>       glTranslatef  (x_side    , 0.0        ,  0.0);                           <* 
    *>       yFONT_icon ("play"    , "prev"        , x_side);                         <* 
    *>       glTranslatef  (x_side    , 0.0        ,  0.0);                           <* 
    *>       yFONT_icon ("play"    , "next"        , x_side);                         <* 
    *>       glTranslatef  (x_side    , 0.0        ,  0.0);                           <* 
    *>       yFONT_icon ("play"    , "last"        , x_side);                         <* 
    *>       glTranslatef  (x_side    , 0.0        ,  0.0);                           <* 
    *>       yFONT_icon ("play"    , "play"        , x_side);                         <* 
    *>       glTranslatef  (x_side    , 0.0        ,  0.0);                           <* 
    *>       yFONT_icon ("play"    , "pause"       , x_side);                         <* 
    *>       glTranslatef  (x_side    , 0.0        ,  0.0);                           <* 
    *>       yFONT_icon ("play"    , "slower"      , x_side);                         <* 
    *>       glTranslatef  (x_side    , 0.0        ,  0.0);                           <* 
    *>       yFONT_icon ("play"    , "normal"      , x_side);                         <* 
    *>       glTranslatef  (x_side    , 0.0        ,  0.0);                           <* 
    *>       yFONT_icon ("play"    , "faster"      , x_side);                         <* 
    *>    } glPopMatrix   ();                                                         <* 
    *> }                                                                              <*/
   /*> glPushMatrix    (); {                                                          <* 
    *>    glColor4f    (1.00f, 1.00f, 1.00f, 1.0f);                                   <* 
    *>    glTranslatef (150.0, -150.0, 0.0);                                          <* 
    *>    WORDS_display (my.words, 'y');                                              <* 
    *> } glPopMatrix   ();                                                            <*/
   /*> glPushMatrix    (); {                                                          <* 
    *>    glColor4f    (1.00f, 1.00f, 1.00f, 1.0f);                                   <* 
    *>    glTranslatef (0.0, 0.0, 0.0);                                               <* 
    *>    WORDS_display (my.guide, '-');                                              <* 
    *> } glPopMatrix   ();                                                            <*/
   /*---(mipmaps)------------------------*/
   rc = yGLTEX_draw_end  (s_tex);
   /*---(complete)-----------------------*/
   DEBUG_GRAF   yLOG_exit    (__FUNCTION__);
   return 0;
}



/*====================------------------------------------====================*/
/*===----                            points                            ----===*/
/*====================------------------------------------====================*/
static void      o___GUIDES__________________o (void) {;}

/*> char          /+----: draw the saved status ----------------------------------+/   <* 
 *> draw_quad1         (char a_quad)                                                   <* 
 *> {                                                                                  <* 
 *>    /+---(locals)-------------------------+/                                        <* 
 *>    int       i         = 0;            /+ loop iterator -- group              +/   <* 
 *>    int       j         = 0;            /+ loop iterator -- location           +/   <* 
 *>    /+---(locals)-------------------------+/                                        <* 
 *>    for (i = 0; i < MAX_GROUPS; ++i) {                                              <* 
 *>       if (strncmp(groups[j].gr, "eof", 5)              == 0) break;                <* 
 *>       if (groups[i].fl[0] != a_quad)  continue;                                    <* 
 *>       for (j = 0; j < MAX_LETTERS; ++j) {                                          <* 
 *>          if (strncmp(g_loc[j].n, "eof", 5)              == 0) break;                 <* 
 *>       }                                                                            <* 
 *>    }                                                                               <* 
 *>    /+---(complete)-----------------------+/                                        <* 
 *>    return 0;                                                                       <* 
 *> }                                                                                  <*/

/*> char          /+----: draw horizontal guides ---------------------------------+/   <* 
 *> draw_horz          (void)                                                          <* 
 *> {                                                                                  <* 
 *>    float z = 6.0;                                                                  <* 
 *>    glLineWidth(1.0);                                                               <* 
 *>    glColor4f(1.0f, 0.0f, 0.0f, 1.0f);                                              <* 
 *>    glBegin(GL_LINES);                                                              <* 
 *>    glVertex3f(   0.0,   3.5, z);                                                   <* 
 *>    glVertex3f( 200.0,   3.5, z);                                                   <* 
 *>    glVertex3f(   0.0,  16.0, z);                                                   <* 
 *>    glVertex3f( 200.0,  16.0, z);                                                   <* 
 *>    glVertex3f(   0.0,  25.0, z);                                                   <* 
 *>    glVertex3f( 200.0,  25.0, z);                                                   <* 
 *>    glVertex3f(   0.0,  -3.5, z);                                                   <* 
 *>    glVertex3f( 200.0,  -3.5, z);                                                   <* 
 *>    glVertex3f(   0.0, -16.0, z);                                                   <* 
 *>    glVertex3f( 200.0, -16.0, z);                                                   <* 
 *>    glVertex3f(   0.0, -25.0, z);                                                   <* 
 *>    glVertex3f( 200.0, -25.0, z);                                                   <* 
 *>    glEnd();                                                                        <* 
 *>    return 0;                                                                       <* 
 *> }                                                                                  <*/

char          /*----: draw the saved status ----------------------------------*/
DRAW_cursor        (void)
{
   /*> if (my.touch == 'y')  return 0;                                                <*/
   glPointSize(5.0);
   glColor4f (0.15f, 0.30f, 0.15f, 1.0f);
   glBegin(GL_POINTS);
   glVertex3f (my.xpos, my.ypos, 20.0);
   glEnd();
   return 0;
}

/*> char          /+----: draw the saved status ----------------------------------+/   <* 
 *> draw_saved         (void)                                                          <* 
 *> {                                                                                  <* 
 *>    /+---(locals)-------------------------+/                                        <* 
 *>    float     d, rad;                   /+ degrees and radians                 +/   <* 
 *>    float     x, y, z;                  /+ cartesian coordinates               +/   <* 
 *>    char      msg[10];                                                              <* 
 *>    z = 6.0;                                                                        <* 
 *>    glLineWidth(3.0);                                                               <* 
 *>    glBegin(GL_LINE_STRIP);                                                         <* 
 *>    if (o.saved == 'y') glColor4f(0.0f, 1.0f, 0.0f, 0.5f);                          <* 
 *>    else                glColor4f(1.0f, 0.0f, 0.0f, 0.5f);                          <* 
 *>    for (d = 0; d <= 360; d +=  1) {                                                <* 
 *>       rad = d * DEG2RAD;                                                           <* 
 *>       x   = SIZE_SML_AVG * cos(rad);                                               <* 
 *>       y   = SIZE_SML_AVG * sin(rad);                                               <* 
 *>       glVertex3f( x, y, z);                                                        <* 
 *>    }                                                                               <* 
 *>    glEnd();                                                                        <* 
 *>    return 0;                                                                       <* 
 *> }                                                                                  <*/

char          /*----: draw modification bands down from circles --------------*/
draw_bands         (void)
{
   /*---(locals)-------------------------*/
   int       i;
   int       j;
   int       out_minx;
   int       out_maxx;
   int       out_miny;
   int       out_maxy;
   int       minx, maxx;
   int       miny, maxy;
   float     z  =   4.00;
   /*---(run the list)-------------------*/
   out_minx = o.key[0].x_pos;
   out_maxx = o.key[0].x_pos;
   out_miny = o.key[0].y_pos;
   out_maxy = o.key[0].y_pos;
   for (i = 0; i < o.nkey; ++i) {
      /*---(update outline bounds)-------*/
      if (o.key[i].x_pos < out_minx)  out_minx = o.key[i].x_pos;
      if (o.key[i].x_pos > out_maxx)  out_maxx = o.key[i].x_pos;
      if (o.key[i].y_pos < out_miny)  out_miny = o.key[i].y_pos;
      if (o.key[i].y_pos > out_maxy)  out_maxy = o.key[i].y_pos;
      if (o.key[i].use[0] == 'a' || o.key[i].use[0] == 'A' ||
            o.key[i].use[0] == 'e' || o.key[i].use[0] == 'E') {
         /*---(update circle bounds)-----*/
         minx = o.key[i].x_pos;
         maxx = o.key[i].x_pos;
         maxy = o.key[i].y_pos;
         miny = o.key[i].y_pos;
         for (j = i + 1; j < o.nkey; ++j) {
            if (o.key[j].use[0] != '+') break;
            if (o.key[j].x_pos < minx)  minx = o.key[j].x_pos;
            if (o.key[j].x_pos > maxx)  maxx = o.key[j].x_pos;
            if (o.key[j].y_pos < miny)  miny = o.key[j].y_pos;
            if (o.key[j].y_pos > maxy)  maxy = o.key[j].y_pos;
         }
         /*---(circle adjustment band)---*/
         glColor4f(0.0f, 0.0f, 1.0f, 0.3f);
         glBegin(GL_POLYGON);
         glVertex3f( minx,   200,  z);
         glVertex3f( maxx,   200,  z);
         glVertex3f( maxx,  miny,  z);
         glVertex3f( minx,  miny,  z);
         glEnd();
      }
   }
   /*---(complete)-----------------------*/
   return 0;
}



/*====================------------------------------------====================*/
/*===----                            points                            ----===*/
/*====================------------------------------------====================*/
static void      o___POINTS__________________o (void) {;}

char          /*----: draw the current point ---------------------------------*/
LAYER_curr          (void)
{
   /*---(locals)-------------------------*/
   float   r1 =   2.0;
   float   d;
   float   rad;
   float   x, y;
   float   z  =  30.00;
   /*---(draw current point)-------------*/
   /*> printf ("showing curr\n");                                                     <*/
   glPushMatrix(); {
      /*> glTranslatef( o.avg[o.cavg].x_pos, o.avg[o.cavg - 1].y_pos,    0.0);               <*/
      glTranslatef( o.avg[o.cavg].x_pos, o.avg[o.cavg].y_pos,    0.0);
      glBegin(GL_POLYGON); {
         glColor4f(0.0f, 0.0f, 0.0f, 1.0f);       /* nice medium grey            */
         for (d = 0; d <= 360; d += 10) {
            rad = d * DEG2RAD;
            x   = r1 * cos(rad);
            y   = r1 * sin(rad);
            glVertex3f( x, y, z);
         }
      } glEnd();
      /*---(draw tangent line)--------------*/
      /*> if (o.cavg > 1 && o.bas [o.cavg].fake != 'y' && o.bas [o.cavg].fake != 'y') {   <*/
      int xa  = (int) (cos (o.avg [o.cavg].rads) * 20);
      int ya  = (int) (sin (o.avg [o.cavg].rads) * 20);
      glColor4f (1.0f, 1.0f, 1.0f, 1.0f);
      glLineWidth (1.0);
      glBegin(GL_LINES); {
         glVertex3f( 0.0, 0.0, z);
         glVertex3f( -xa, -ya, z);
         glVertex3f( 0.0, 0.0, z);
         glVertex3f(  xa,  ya, z);
      } glEnd();
      glPointSize(5.0);
      glColor4f (0.0f, 0.0f, 0.0f, 1.0f);
      glBegin(GL_POINTS); {
         glVertex3f( -xa, -ya, z);
      } glEnd();
      glColor4f(1.0f, 1.0f, 0.0f, 1.0f);
      glBegin(GL_POINTS); {
         glVertex3f(  xa,  ya, z);
      } glEnd();
      glPointSize(0.8);
      /*> }                                                                           <*/
   } glPopMatrix();
   /*---(complete)-----------------------*/
   return 0;
}

char          /*----: draw the raw points ------------------------------------*/
LAYER_raws          (void)
{
   /*---(locals)-------------------------*/
   float   z     =    5.0;
   int i;
   glLineWidth (2.0);
   glPointSize (2.0);
   yVIKEYS_view_color (YCOLOR_POS_DRK, 1.00);
   /*> glColor4f(0.0f, 0.0f, 0.7f, 1.0f);                                             <*/
   /*> glBegin(GL_LINE_STRIP);                                                        <*/
   glBegin(GL_POINTS);
   for (i = 0; i < o.nraw; ++i) {
      if (o.raw[i].x_pos == 999 && o.raw[i].y_pos == 999) {
         glEnd();
         glBegin(GL_POINTS);
         continue;
      }
      glVertex3f( o.raw[i].x_pos, o.raw[i].y_pos, z);
   }
   glEnd();
   glLineWidth(0.8);
   return 0;
}

char          /*----: draw the bas/avg points --------------------------------*/
LAYER_base          (void)
{
   /*---(locals)-------------------------*/
   float   z     =    5.25;
   int     i;
   for (i = 0; i < o.navg; ++i) {
      /*---(draw the point)--------------*/
      if      (o.avg[i].cano  == 'x') glColor4f(1.0f, 0.0f, 0.0f, 1.0f);
      else if (o.avg[i].type  == 'm') glColor4f(1.0f, 0.0f, 1.0f, 1.0f);
      else if (o.bas[i].fake  == 'y') glColor4f(0.0f, 0.0f, 0.0f, 1.0f);
      else                            glColor4f(0.7f, 0.7f, 0.0f, 1.0f);
      if (o.bas[i].fake  == 'y')      glPointSize (4.0);
      else                            glPointSize (2.0);
      glBegin     (GL_POINTS); {
         glVertex3f( o.avg[i].x_pos, o.avg[i].y_pos, z);
      } glEnd();
      /*---(draw the S extension)--------*/
      if (o.avg [i].type == POINT_START) {
         glEnable      (GL_LINE_STIPPLE);
         glLineStipple (1, 0x3333);
         glColor4f     (0.0f, 0.0f, 0.0f, 0.5f);
         glLineWidth   (2.0);
         glBegin (GL_LINES); {
            glVertex3f (o.avg [i    ].x_pos, o.avg [i    ].y_pos, z - 1.50);
            glVertex3f (o.avg [i + 1].x_pos, o.avg [i + 1].y_pos, z - 1.50);
         } glEnd();
         glDisable     (GL_LINE_STIPPLE);
      }
      /*---(draw the F extension)--------*/
      if (o.avg [i].type == POINT_FINISH) {
         glEnable      (GL_LINE_STIPPLE);
         glLineStipple (1, 0x3333);
         glColor4f     (0.0f, 0.0f, 0.0f, 0.5f);
         glLineWidth   (2.0);
         glBegin (GL_LINES); {
            glVertex3f (o.avg [i    ].x_pos, o.avg [i    ].y_pos, z - 1.50);
            glVertex3f (o.avg [i - 1].x_pos, o.avg [i - 1].y_pos, z - 1.50);
         } glEnd();
         glDisable     (GL_LINE_STIPPLE);
      }
   }
   glLineWidth (0.8);
   return 0;
}

char          /*----: draw the key points and lines --------------------------*/
LAYER_keys          (void)
{
   /*---(locals)-------------------------*/
   float   r1 =   2.0;
   float   d;
   float   rad;
   float   x, y;
   float   z  =   5.75;
   int     pt = 0;
   int     i;
   /*---(draw keys)----------------------*/
   for (i = 0; i < o.nkey; ++i) {
      /*---(draw the point)--------------*/
      glPushMatrix(); {
         glTranslatef( o.key[i].x_pos, o.key[i].y_pos,    0.0);
         glColor4f(1.0f, 1.0f, 1.0f, 0.5f);
         if (o.key[i].cano  == 'x') glColor4f (1.0f, 0.0f, 0.0f, 0.5f);
         if (o.key[i].fake  == 'y') glColor4f (0.0f, 1.0f, 1.0f, 0.5f);
         glBegin(GL_POLYGON); {
            for (d = 0; d <= 360; d += 10) {
               rad = d * DEG2RAD;
               x   = r1 * cos(rad);
               y   = r1 * sin(rad);
               glVertex3f( x, y, z);
            }
         } glEnd();
      } glPopMatrix();
      /*---(draw the segment)------------*/
      if (o.key[i].type != '>') {
         glColor4f(1.0f, 1.0f, 1.0f, 1.0f);
         glLineWidth(2.0);
         glBegin(GL_LINES); {
            glVertex3f(o.key[i - 1].x_pos, o.key[i - 1].y_pos, z + 0.25);
            glVertex3f(o.key[i].x_pos,     o.key[i].y_pos,     z + 0.25);
         } glEnd();
      }
      /*---(draw the hidden lines)-------*/
      if (i > 0 && o.key[i].type == '>') {
         glColor4f(0.5f, 0.5f, 0.5f, 1.0f);
         glLineWidth(2.0);
         glBegin(GL_LINES); {
            glVertex3f(o.key[i - 1].x_pos, o.key[i - 1].y_pos, z - 1.00);
            glVertex3f(o.key[i].x_pos,     o.key[i].y_pos,     z - 1.00);
         } glEnd();
      }
      /*---(draw the S extension)--------*/
      if (o.key[i].type == POINT_HEAD) {
         pt = o.key[i].p_bas - 1;
         glEnable      (GL_LINE_STIPPLE);
         glLineStipple (1, 0x3333);
         glColor4f     (0.2f, 0.2f, 0.2f, 1.0f);
         glLineWidth   (2.0);
         glBegin (GL_LINES); {
            glVertex3f(o.bas[pt].x_pos    , o.bas[pt].y_pos  , z - 1.50);
            glVertex3f(o.key[i].x_pos     , o.key[i].y_pos   , z - 1.50);
         } glEnd();
         glDisable     (GL_LINE_STIPPLE);
      }
      /*---(draw the F extension)--------*/
      if (o.key[i + 1].type == '>' || i + 1 == o.nkey) {
         pt = o.key[i].p_bas + 1;
         glEnable      (GL_LINE_STIPPLE);
         glLineStipple (1, 0x3333);
         glColor4f     (0.2f, 0.2f, 0.2f, 1.0f);
         glLineWidth   (2.0);
         glBegin (GL_LINES); {
            glVertex3f(o.bas[pt].x_pos    , o.bas[pt].y_pos  , z - 1.50);
            glVertex3f(o.key[i].x_pos     , o.key[i].y_pos   , z - 1.50);
         } glEnd();
         glDisable     (GL_LINE_STIPPLE);
      }
   }
   /*---(complete)-----------------------*/
   return 0;
}



/*====================------------------------------------====================*/
/*===----                          information                         ----===*/
/*====================------------------------------------====================*/
static void      o___INFO____________________o (void) {;}

/*> char                                                                              <* 
 *> DRAW_info_counts     (void)                                                       <* 
 *> {                                                                                 <* 
 *>    /+---(locals)-----------+-----+-----+-+/                                       <* 
 *>    char        t           [100];                                                 <* 
 *>    /+---(point counts)-------------------+/                                       <* 
 *>    glColor4f     (0.0f, 0.0f, 0.0f, 1.0f);                                        <* 
 *>    glPushMatrix  (); {                                                            <* 
 *>       glTranslatef  (win.m_xmax - 125.0, win.m_ymin + 28.0, 50);                  <* 
 *>       snprintf      (t, 100, "%4d", o.nraw);                                      <* 
 *>       FONT__label   ("raw#", t);                                                  <* 
 *>       snprintf      (t, 100, "%4d", o.navg);                                      <* 
 *>       FONT__label   ("avg#", t);                                                  <* 
 *>       snprintf      (t, 100, "%4d", o.nkey);                                      <* 
 *>       FONT__label   ("key#", t);                                                  <* 
 *>    } glPopMatrix ();                                                              <* 
 *>    glPushMatrix  (); {                                                            <* 
 *>       glTranslatef  (win.m_xmax - 225.0, win.m_ymin + 28.0, 50);                  <* 
 *>       snprintf      (t, 100, "%4d", o.total);                                     <* 
 *>       FONT__label   ("rec#", t);                                                  <* 
 *>       snprintf      (t, 100, "%4d", o.curr);                                      <* 
 *>       FONT__label   ("cur#", t);                                                  <* 
 *>    } glPopMatrix ();                                                              <* 
 *>    /+---(complete)-----------------------+/                                       <* 
 *>    return 0;                                                                      <* 
 *> }                                                                                 <*/

char
OVERLAY_data    (void)
{
   /*---(locals)-----------+-----+-----+-*/
   char        t           [100];
   float       x_slope     =  0.0;
   int         x_icept     =    0;
   int         x_off       =  200;
   int         y_off       =  105;
   /*---(average point)------------------*/
   yVIKEYS_view_color (YCOLOR_BAS_MOR, 0.65);
   glPushMatrix  (); {
      /*---(counts)----------------------*/
      glTranslatef  (x_off, y_off, 30);
      snprintf      (t, 100, "%6d", o.nraw);
      FONT__label   ("raw#", t);
      snprintf      (t, 100, "%6d", o.nbas);
      FONT__label   ("bas#", t);
      snprintf      (t, 100, "%6d", o.navg);
      FONT__label   ("avg#", t);
      snprintf      (t, 100, "%6d", o.nkey);
      FONT__label   ("key#", t);
      /*---(basics)----------------------*/
      /*> glTranslatef  (0.0  , -10.0 , 0.0);                                         <*/
      snprintf      (t, 100, "%6d", o.avg[o.cavg].p_bas);
      FONT__label   ("num"   , t);
      snprintf      (t, 100, "%6d", o.avg[o.cavg].p_raw);
      FONT__label   ("raw"   , t);
      snprintf      (t, 100, "%6d", o.avg[o.cavg].x_full);
      FONT__label   ("x_full", t);
      snprintf      (t, 100, "%6d", o.avg[o.cavg].y_full);
      FONT__label   ("y_full", t);
      snprintf      (t, 100, "%6d", o.avg[o.cavg].x_pos);
      FONT__label   ("x_pos" , t);
      snprintf      (t, 100, "%6d", o.avg[o.cavg].y_pos);
      FONT__label   ("y_pos" , t);
      snprintf      (t, 100, "%6d", o.avg[o.cavg].xd);
      FONT__label   ("x-diff", t);
      snprintf      (t, 100, "%6d", o.avg[o.cavg].yd);
      FONT__label   ("y-diff", t);
      snprintf      (t, 100, "%6d", o.avg[o.cavg].len);
      FONT__label   ("len"   , t);
      /*---(more complex)----------------*/
      x_slope = o.avg[o.cavg].slope;
      x_icept = o.avg[o.cavg].icept;
      /*> if (x_slope >  999) { x_slope = 999.99; b = 999; }                          <* 
       *> if (x_slope < -999) { x_slope = -999.99; b = -999; }                        <*/
      snprintf      (t, 100, "%9.2f", x_slope);
      FONT__label   ("slope" , t);
      snprintf      (t, 100, "%6d",   x_icept);
      FONT__label   ("b-cept", t);
      snprintf      (t, 100, "%9.2f", o.avg[o.cavg].rads);
      FONT__label   ("rads"  , t);
      snprintf      (t, 100, "%6d",   o.avg[o.cavg].degs);
      FONT__label   ("degs"  , t);
      snprintf      (t, 100, "%6d",   o.avg[o.cavg].quad);
      FONT__label   ("quad"  , t);
      snprintf      (t, 100, "     %c", o.avg[o.cavg].type);
      FONT__label   ("type"  , t);
      /*---(conversion)------------------*/
      snprintf      (t, 100, "%9.2f", my.x_center);
      FONT__label   ("x_cen" , t);
      snprintf      (t, 100, "%9.2f", my.y_center);
      FONT__label   ("y_cen" , t);
      snprintf      (t, 100, "%9.2f", my.ratio);
      FONT__label   ("ratio" , t);
      /*---(done)------------------------*/
   } glPopMatrix();
   /*---(complete)-----------------------*/
   return 0;
}

char
DRAW_info_answer     (void)
{
   /*---(locals)-----------+-----+-----+-*/
   char        t           [100];
   /*---(point counts)-------------------*/
   glColor4f     (0.0f, 0.0f, 0.0f, 1.0f);
   glPushMatrix  (); {
      glTranslatef(  win.d_ansx, win.d_ansy, win.d_zoff);
      snprintf      (t, 100, "%s",   o.actual);
      FONT__label   ("actu"  , t);
      snprintf      (t, 100, "%s",   o.word);
      FONT__label   ("word"  , t);
   } glPopMatrix ();
   /*---(complete)-----------------------*/
   return 0;
}

char
DRAW_info (void)
{
   DEBUG_GRAF  printf("   - draw_info()\n");
   char msg[100];
   glColor4f(1.0f, 1.0f, 0.0f, 1.0f);
   if (o.saved == 'y') {
      glPushMatrix();
      glTranslatef( -210.0, -150.0, win.d_zoff);
      snprintf(msg, 100, "%4d", o.curr);
      yFONT_print(win.font_bg,  18, YF_BOTLEF, msg);
      glPopMatrix();
   }
   /*---(point counts)----------------------*/
   glColor4f(0.0f, 0.0f, 0.0f, 1.0f);
   glPushMatrix();
   glTranslatef (win.d_xoff, win.d_yoff, win.d_zoff);
   snprintf(msg, 100, "%4d", o.total);
   FONT__label ("out#", msg);
   snprintf(msg, 100, "%4d", o.curr);
   FONT__label ("outc", msg);
   snprintf(msg, 100, "%4d", o.nraw);
   FONT__label ("raw#", msg);
   snprintf(msg, 100, "%4d", o.navg);
   FONT__label ("avg#", msg);
   snprintf(msg, 100, "%4d", o.cavg);
   FONT__label ("avgc", msg);
   snprintf(msg, 100, "%4d", o.nkey);
   FONT__label ("key#", msg);
   /*---(average point)---------------------*/
   glTranslatef(   0, -win.d_bar * 0.5, 0.0);
   snprintf(msg, 100, "%4d", o.avg[o.cavg].p_bas);
   FONT__label ("num", msg);
   snprintf(msg, 100, "%4d", o.avg[o.cavg].p_raw);
   FONT__label ("raw", msg);
   snprintf(msg, 100, "%4d", o.avg[o.cavg].x_pos);
   FONT__label ("x"  , msg);
   snprintf(msg, 100, "%4d", o.avg[o.cavg].y_pos);
   FONT__label ("y"  , msg);
   snprintf(msg, 100, "%4d", o.avg[o.cavg].xd);
   FONT__label ("xd" , msg);
   snprintf(msg, 100, "%4d", o.avg[o.cavg].yd);
   FONT__label ("yd" , msg);
   snprintf(msg, 100, "%4d", o.avg[o.cavg].len);
   FONT__label ("len" , msg);
   /*---(more complex)----------------------*/
   float x_slope = o.avg[o.cavg].slope;
   int   x_icept = o.avg[o.cavg].icept;
   snprintf(msg, 100, "%7.2f", x_slope);
   FONT__label ("s" , msg);
   snprintf(msg, 100, "%4d",   x_icept);
   FONT__label ("b"   , msg);
   snprintf(msg, 100, "%7.2f", o.avg[o.cavg].rads);
   FONT__label ("r" , msg);
   snprintf(msg, 100, "%4d",   o.avg[o.cavg].degs);
   FONT__label ("d"   , msg);
   snprintf(msg, 100, "%4d",   o.avg[o.cavg].quad);
   FONT__label ("q"   , msg);
   snprintf(msg, 100, "   %c", o.avg[o.cavg].type);
   FONT__label ("t"   , msg);
   /*---(calculated)------------------------*/
   int  xkey = KEY_find(o.cavg);
   if (xkey >= 0) {
      glTranslatef(   0, -win.d_bar * 0.5, 0.0);
      snprintf(msg, 100, "%4d",   xkey);
      FONT__label ("key"  , msg);
      snprintf(msg, 100, "%4d",   o.key[xkey].xd);
      FONT__label ("xd" , msg);
      snprintf(msg, 100, "%4d",   o.key[xkey].yd);
      FONT__label ("yd" , msg);
      snprintf(msg, 100, "%4d",   o.key[xkey].len);
      FONT__label ("len" , msg);
      /*---(more complex)----------------------*/
      x_slope = o.key[xkey].slope;
      x_icept = o.key[xkey].icept;
      snprintf(msg, 100, "%7.2f", x_slope);
      FONT__label ("s" , msg);
      snprintf(msg, 100, "%4d",   x_icept);
      FONT__label ("b"   , msg);
      snprintf(msg, 100, "%7.2f", o.key[xkey].rads);
      FONT__label ("r" , msg);
      snprintf(msg, 100, "%4d",   o.key[xkey].degs);
      FONT__label ("d"   , msg);
      snprintf(msg, 100, "%4d",   o.key[xkey].quad);
      FONT__label ("q"   , msg);
      snprintf(msg, 100, "   %c", o.key[xkey].type);
      FONT__label ("t"   , msg);
      /*---(advanced)--------------------------*/
      snprintf(msg, 100, "%4d"  , o.key[xkey].range);
      FONT__label ("ra"  , msg);
      snprintf(msg, 100, "%7.2f", o.key[xkey].cdepth);
      FONT__label ("c" , msg);
      snprintf(msg, 100, "  %2d", o.key[xkey].ccat);
      FONT__label ("cc"  , msg);
      snprintf(msg, 100, "   %c", o.key[xkey].cano);
      FONT__label ("ca"  , msg);
      snprintf(msg, 100, "   %c", o.key[xkey].fake);
      FONT__label ("a"   , msg);
      snprintf(msg, 100, "%4.4s", o.key[xkey].use);
      FONT__label ("u"   , msg);
   }
   glPopMatrix();
   glPushMatrix();
   glTranslatef(  win.d_ansx, win.d_ansy, win.d_zoff);
   snprintf(msg, 100, "%4d of %4d",   o.curr, o.total);
   FONT__label ("curr"  , msg);
   snprintf(msg, 100, "%s",   o.when);
   FONT__label ("when"  , msg);
   snprintf(msg, 100, "%s",   o.note);
   FONT__label ("note"  , msg);
   snprintf(msg, 100, "%s",   o.expect);
   FONT__label ("expe"  , msg);
   snprintf(msg, 100, "%s",   o.actual);
   FONT__label ("actu"   , msg);
   snprintf(msg, 100, "%s",   o.grade);
   FONT__label ("grad"   , msg);
   snprintf(msg, 100, "%s",   o.word);
   FONT__label ("word"   , msg);
   snprintf(msg, 100, "%s",   o.letters);
   FONT__label ("ltrs"   , msg);
   glPopMatrix();
   /*---(complete)-------------------------*/
   DEBUG_GRAF  printf("   - draw_info()          : complete\n");
   return 0;
}


/*====================------------------------------------====================*/
/*===----                             fonts                            ----===*/
/*====================------------------------------------====================*/
static void      o___FONTS___________________o (void) {;}

char
FONT__load           (void)
{
   win.font_bg = yFONT_load (win.face_bg);
   if (win.font_bg < 0) {
      fprintf(stderr, "Problem loading %s\n", win.face_bg);
      exit(1);
   }
   win.font_sm = yFONT_load (win.face_sm);
   if (win.font_sm < 0) {
      fprintf(stderr, "Problem loading %s\n", win.face_sm);
      exit(1);
   }
   /*> win.icons  = yFONT_symload (ICON_SET, 20, 33, '-');                            <*/
   win.icons  = yFONT_iconload ();
   if (win.icons < 0) {
      fprintf(stderr, "Problem loading %s\n", ICON_SET);
      exit(1);
   }
   return 0;
}

char
FONT__label          (char *a_label, char *a_content)
{
   glPushMatrix(); {
      yFONT_print  (win.font_sm,  win.d_point, YF_BOTLEF, a_label);
      glTranslatef (   45,  0,  0);
      yFONT_print  (win.font_sm,  win.d_point, YF_BOTLEF, ":");
      glTranslatef (   10,  0,  0);
      yFONT_print  (win.font_sm,  win.d_point, YF_BOTLEF, a_content);
   } glPopMatrix ();
   glTranslatef(   0,          - (win.d_bar * 0.8),  0);
   return 0;
}

char
FONT__free           (void)
{
   yFONT_free (win.font_bg);
   yFONT_free (win.font_sm);
   return 0;
}



/*====================------------------------------------====================*/
/*===----                        writing samples                       ----===*/
/*====================------------------------------------====================*/
static void      o___SAMPLES_________________o (void) {;}

/*> char               /+----: create a texture for writing samples --------------+/                                    <* 
 *> sample_init        (void)                                                                                           <* 
 *> {                                                                                                                   <* 
 *>    /+---(generate)-----------------------+/                                                                         <* 
 *>    glGenFramebuffersEXT         (1, &sample_fbo);                                                                   <* 
 *>    glGenTextures                (1, &sample_tex);                                                                   <* 
 *>    glGenRenderbuffersEXT        (1, &sample_dep);                                                                   <* 
 *>    /+---(bind)---------------------------+/                                                                         <* 
 *>    glBindFramebufferEXT         (GL_FRAMEBUFFER_EXT,  sample_fbo);                                                  <* 
 *>    glBindTexture                (GL_TEXTURE_2D,       sample_tex);                                                  <* 
 *>    /+---(settings)-----------------------+/                                                                         <* 
 *>    glTexParameteri              (GL_TEXTURE_2D,  GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);                   <* 
 *>    glTexParameteri              (GL_TEXTURE_2D,  GL_TEXTURE_MAG_FILTER, GL_LINEAR);                                 <* 
 *>    glTexParameteri              (GL_TEXTURE_2D,  GL_TEXTURE_WRAP_S, GL_REPEAT);                                     <* 
 *>    glTexParameteri              (GL_TEXTURE_2D,  GL_TEXTURE_WRAP_T, GL_REPEAT);                                     <* 
 *>    glTexEnvi                    (GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_REPLACE);                                  <* 
 *>    glTexParameteri              (GL_TEXTURE_2D,  GL_GENERATE_MIPMAP, GL_TRUE);                                      <* 
 *>    /+---(copy)---------------------------+/                                                                         <* 
 *>    glTexImage2D                 (GL_TEXTURE_2D, 0, GL_RGBA, sample_w, sample_h, 0, GL_RGBA, GL_UNSIGNED_BYTE, 0);   <* 
 *>    glFramebufferTexture2DEXT    (GL_FRAMEBUFFER_EXT, GL_COLOR_ATTACHMENT0_EXT, GL_TEXTURE_2D, sample_tex, 0);       <* 
 *>    /+---(depth)--------------------------+/                                                                         <* 
 *>    glBindRenderbufferEXT        (GL_RENDERBUFFER_EXT, sample_dep);                                                  <* 
 *>    glRenderbufferStorageEXT     (GL_RENDERBUFFER_EXT, GL_DEPTH_COMPONENT24, sample_w, sample_h);                    <* 
 *>    glFramebufferRenderbufferEXT (GL_FRAMEBUFFER_EXT, GL_DEPTH_ATTACHMENT_EXT, GL_RENDERBUFFER_EXT, sample_dep);     <* 
 *>    /+---(unbind)-------------------------+/                                                                         <* 
 *>    glBindFramebufferEXT         (GL_FRAMEBUFFER_EXT, 0);                                                            <* 
 *>    /+---(complete)-----------------------+/                                                                         <* 
 *>    return 0;                                                                                                        <* 
 *> }                                                                                                                   <*/

char       /*----: draw the writing samples to the texture -------------------*/
OVERLAY_samples        (void)
{
   int i = 0;
   glColor4f             (0.0f, 0.0f, 0.0f, 1.0f);
   /*---(title)--------------------------*/
   /*> glPushMatrix(); {                                                              <* 
    *>    glScalef              (1.0, 1.0, 1.0);                                      <* 
    *>    glTranslatef          (    0.0,   -5.0,    0.0);                            <* 
    *>    yFONT_print           (win.font_sm,  5, YF_TOPLEF, "writing samples");      <* 
    *> } glPopMatrix();                                                               <*/
   /*---(cycle samples)------------------*/
   glPushMatrix(); {
      glTranslatef (win.d_xoff - 50, win.d_yoff + 30, win.d_zoff);
      glScalef              (1.5, 1.5, 1.5);
      for (i = 1; i < MAX_LETTERS && strncmp(g_loc[i].n, "eof", 5) != 0; ++i) {
         glPushMatrix();
         glTranslatef(g_loc[i].tx, g_loc[i].ty,  0.0);
         yFONT_print(win.font_sm,  5, YF_BOTRIG, g_loc[i].n);
         glCallList(dl_dotted + i);
         glPopMatrix();
      }
   } glPopMatrix();
   return 0;
}

/*> char       /+----: draw the writing samples to the texture -------------------+/            <* 
 *> sample_etch        (void)                                                                   <* 
 *> {                                                                                           <* 
 *>    int         i           = 0;                                                             <* 
 *>    /+---(setup)--------------------------+/                                                 <* 
 *>    glViewport            (0.0,  0.0, sample_w, sample_h);                                   <* 
 *>    glMatrixMode          (GL_PROJECTION);                                                   <* 
 *>    glLoadIdentity        ();                                                                <* 
 *>    glOrtho               (0.0,   sample_w, -sample_h,  0.0, -500.0,  500.0);                <* 
 *>    glMatrixMode          (GL_MODELVIEW);                                                    <* 
 *>    glBindTexture         (GL_TEXTURE_2D, 0);                                                <* 
 *>    glBindFramebufferEXT  (GL_FRAMEBUFFER_EXT,  sample_fbo);                                 <* 
 *>    glClearColor          (0.3f, 0.5f, 0.3f, 1.0f);       /+ nice medium green          +/   <* 
 *>    glClear               (GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);                       <* 
 *>    glColor4f             (0.0f, 0.0f, 0.0f, 1.0f);                                          <* 
 *>    /+---(title)--------------------------+/                                                 <* 
 *>    glPushMatrix(); {                                                                        <* 
 *>       glScalef(2.0, 2.0, 2.0);                                                              <* 
 *>       glTranslatef          (    0.0,   -5.0,    0.0);                                      <* 
 *>       yFONT_print           (win.font_sm,  5, YF_TOPLEF, "writing samples");                <* 
 *>    } glPopMatrix();                                                                         <* 
 *>    /+---(cycle samples)------------------+/                                                 <* 
 *>    glPushMatrix(); {                                                                        <* 
 *>       glScalef(2.0, 2.0, 2.0);                                                              <* 
 *>       for (i = 0; i < MAX_LETTERS && strncmp(g_loc[i].n, "eof", 5) != 0; ++i) {               <* 
 *>          glPushMatrix();                                                                    <* 
 *>          glTranslatef(g_loc[i].tx, g_loc[i].ty,  0.0);                                          <* 
 *>          yFONT_print(win.font_sm,  5, YF_BOTRIG, g_loc[i].n);                                 <* 
 *>          glCallList(dl_dotted + i);                                                         <* 
 *>          glPopMatrix();                                                                     <* 
 *>       }                                                                                     <* 
 *>    } glPopMatrix();                                                                         <* 
 *>    /+---(mipmaps)------------------------+/                                                 <* 
 *>    glBindFramebufferEXT  (GL_FRAMEBUFFER_EXT, 0);                                           <* 
 *>    glBindTexture         (GL_TEXTURE_2D, sample_tex);                                       <* 
 *>    glGenerateMipmapEXT   (GL_TEXTURE_2D);                                                   <* 
 *>    glBindTexture         (GL_TEXTURE_2D, 0);                                                <* 
 *>    /+---(complete)-------------------------+/                                               <* 
 *>    return 0;                                                                                <* 
 *> }                                                                                           <*/

/*> char       /+----: display the writing sample to the window ------------------+/   <* 
 *> sample_draw        (void)                                                          <* 
 *> {                                                                                  <* 
 *>    glPushMatrix(); {                                                               <* 
 *>       glTranslatef(160, 115, -15.0);                                               <* 
 *>       glBindTexture   (GL_TEXTURE_2D, sample_tex);                                 <* 
 *>       glBegin(GL_POLYGON); {                                                       <* 
 *>          glTexCoord2f ( 0.0      ,  1.0);                                          <* 
 *>          glVertex3f   ( 0.0      ,  0.0       , 0.0);                              <* 
 *>          glTexCoord2f ( 1.0      ,  1.0);                                          <* 
 *>          glVertex3f   ( sample_w / 2.0 ,  0.0       , 0.0);                        <* 
 *>          glTexCoord2f ( 1.0      ,  0.0);                                          <* 
 *>          glVertex3f   ( sample_w / 2.0 ,  -sample_h / 2.0 , 0.0);                  <* 
 *>          glTexCoord2f ( 0.0      ,  0.0);                                          <* 
 *>          glVertex3f   ( 0.0      ,  -sample_h / 2.0 , 0.0);                        <* 
 *>       } glEnd();                                                                   <* 
 *>       glBindTexture   (GL_TEXTURE_2D, 0);                                          <* 
 *>    } glPopMatrix();                                                                <* 
 *>    return 0;                                                                       <* 
 *> }                                                                                  <*/


/*> char               /+ PURPOSE : delete a framebuffer -------------------------+/   <* 
 *> sample_free        (void)                                                          <* 
 *> {                                                                                  <* 
 *>    /+---(generate)-----------------------+/                                        <* 
 *>    glDeleteTextures                (1, &sample_tex);                               <* 
 *>    glDeleteRenderbuffersEXT        (1, &sample_dep);                               <* 
 *>    glDeleteFramebuffersEXT         (1, &sample_fbo);                               <* 
 *>    /+---(complete)-----------------------+/                                        <* 
 *>    return 0;                                                                       <* 
 *> }                                                                                  <*/



/*============================----(source-end)----============================*/
