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
char DRAW_xaxis           (void);
char DRAW_yaxis           (void);



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
   win.d_point  =     8;
   win.d_bar    =    20;
   win.d_ansx   =   200;
   win.d_ansy   =  -160;
   win.d_reqx   =  -100;
   win.d_reqy   =  -180;
   yVIEW_bounds  (YVIEW_MAIN  , NULL, NULL, &(my.x_min), &(my.x_max ), &(my.x_wide), &(my.y_min), &(my.y_max ), &(my.y_tall));
   my.x_wide    = my.x_wide + 40;
   my.x_center  = abs (my.x_min) / (float) my.x_wide;
   my.y_center  = abs (my.y_min) / (float) a_tall;
   /*> my.ratio     = my.x_scale  / (float) my.x_wide;                                <*/
   my.ratio     = 10;
   my.x_scale   = my.x_wide * my.ratio;
   my.y_scale   = my.y_tall * my.ratio;
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
   /*---(window)-------------------------*/
   /*> yVIKEYS_view_config   ("gregg shorthand interpreter", P_VERNUM, YVIKEYS_OPENGL, 500, 350, 0);   <*/
   /*> yVIKEYS_view_setup    (YVIEW_MAIN     , YVIKEYS_FLAT, YVIKEYS_TOPLEF, -125, 500, 125 - 350, 350, 0, 0, YCOLOR_BAS, YOLOR_MED, DRAW_primary);   <*/
   yVIEW_full    (YVIEW_MAIN , YVIEW_FLAT, YVIEW_BOTLEF, YCOLOR_BAS, YCOLOR_MED, DRAW_primary);
   /*> yVIEW_simple  (YVIEW_XAXIS, YCOLOR_BAS, YCOLOR_DRK, DRAW_xaxis  );             <*/
   /*> yVIEW_simple  (YVIEW_YAXIS, YCOLOR_BAS, YCOLOR_DRK, DRAW_yaxis  );             <*/
   /*> yVIEW_defsize (YVIEW_YAXIS, 15, 0);                                            <*/
   /*> yVIKEYS_cmds_direct (":xaxis show");                                           <*/
   /*> yVIKEYS_cmds_direct (":yaxis show");                                           <*/
   DRAW__sizes   (500, 350);
   /*---(colors)-------------------------*/
   /*> yCOLOR_palette (190, "rcomp", "pale", "earthy");                               <*/
   yCOLOR_palette (190, "rcomp", "pale", "warm");
   /*> yVIKEYS_view_colors   (YCOLOR_POS, YCOLOR_BAS, YCOLOR_NEG, YCOLOR_POS);        <*/
   /*---(custom drawing)-----------------*/
   yGLTEX_config   ();
   FONT__load      ();
   dlist_init      ();
   yGLTEX_new (&s_tex, &s_fbo, &s_depth, win.tex_w, win.tex_h);
   DRAW_back       ();
   /*---(ribbon)-------------------------*/
   /*> yVIKEYS_view_ribbon ("play"    , "dj"          );                              <* 
    *> yVIKEYS_view_ribbon ("talk"    , "video_cam"   );                              <* 
    *> yVIKEYS_view_ribbon ("align"   , "layers"      );                              <* 
    *> yVIKEYS_view_ribbon ("draw"    , "resolution"  );                              <* 
    *> yVIKEYS_view_ribbon ("tools"   , "shower"      );                              <* 
    *> yVIKEYS_cmds_direct (":ribbon show");                                          <*/
   /*---(overlay/layers)-----------------*/
   /*> yVIKEYS_view_option (YVIKEYS_OVERLAY, "point" , OVERLAY_data    , "current point statistics");   <*/
   /*> yVIKEYS_view_option (YVIKEYS_OVERLAY, "sample", OVERLAY_samples , "letter samples");   <*/
   /*> yVIKEYS_dataset_add   ("raw"     , LAYER_raws, "raw data points"     );        <*/
   /*> yVIKEYS_dataset_add   ("bas"     , LAYER_base, "bas/avg data points" );        <*/
   /*> yVIKEYS_dataset_add   ("key"     , LAYER_keys, "key/crit data points");        <*/
   /*> yVIKEYS_dataset_add   ("cur"     , LAYER_curr, "current point"       );        <*/
   /*> yVIKEYS_cmds_direct (":gridsize 1");                                           <*/
   /*> yVIKEYS_cmds_direct (":layer bas");                                            <*/
   /*> yVIKEYS_cmds_direct (":layer cur");                                            <*/
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
   short    x_xmin, x_xmax, x_wide;
   short    x_ymin, x_ymax, x_tall;
   int      x = 0, y = 0;
   float    i = 0, j = 0;
   char     t      [LEN_LABEL] = "";
   /*---(header)-------------------------*/
   DEBUG_GRAF   yLOG_enter   (__FUNCTION__);
   yVIEW_bounds  (YVIEW_MAIN, NULL, NULL, &x_xmin, &x_xmax, &x_wide, &x_ymin, &x_ymax, &x_tall);
   /*> printf ("t %c %3d %3d %3d %3d %s\n", x_on, x_left, x_bott, x_wide, x_tall, x_text);   <*/
   /*---(setup view)---------------------*/
   /*> glViewport      (x_left, x_bott, x_wide, x_tall);                              <* 
    *> glMatrixMode    (GL_PROJECTION);                                               <* 
    *> glLoadIdentity  ();                                                            <* 
    *> glOrtho         (x_xmin, x_xmax, x_ymin, x_ymax,  -500.0,   500.0);            <* 
    *> glMatrixMode    (GL_MODELVIEW);                                                <*/
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
   /*> DRAW_cursor ();                                                                <*/
   OVERLAY_samples ();
   glEnable      (GL_LINE_STIPPLE);
   glLineStipple (1, 0x3333);
   glLineWidth   (0.8);
   yCOLOR_opengl (YCOLOR_BAS, YCOLOR_MOR, 0.40);
   for  (x = -500; x <= 500; x += 100) {
      glPushMatrix    (); {
         glBegin(GL_LINES); {
            glVertex3f( x,  500, 250);
            glVertex3f( x, -500, 250);
         } glEnd();
         glTranslatef ( x,    5, 250);
         sprintf (t, "%4dx", x);
         yFONT_print(win.font_pretty,   8, YF_BOTLEF, t);
      } glPopMatrix   ();
   }
   for  (y = -500; y <= 500; y += 100) {
      glPushMatrix    (); {
         glBegin(GL_LINES); {
            glVertex3f(  0, y, 250);
            glVertex3f(500, y, 250);
         } glEnd();
         glTranslatef (490, y + 5, 250);
         sprintf (t, "%4dy", y);
         yFONT_print(win.font_pretty,   8, YF_BOTRIG, t);
      } glPopMatrix   ();
   }
   glDisable(GL_LINE_STIPPLE);
   /*> for  (x = -500; x <= 500; x += 100) {                                          <* 
    *>    for  (y = -500; y <= 500; y += 100) {                                       <* 
    *>       i = j = 0;                                                               <* 
    *>       glPushMatrix    (); {                                                    <* 
    *>          glTranslatef (x, y, 250);                                             <* 
    *>          sprintf (t, "%4dx, %4dy", x, y);                                      <* 
    *>          yFONT_print(win.font_pretty,   8, YF_BOTLEF, t);                      <* 
    *>          CREATE_draw_init  ();                                                 <* 
    *>          yCOLOR_opengl (YCOLOR_BAS, YCOLOR_MOR, 0.65);                         <* 
    *>          CREATE_line (SHAPE_DRAW, 0, 20,  1, &i, &j);                          <* 
    *>          CREATE_draw_wrap  (i, j);                                                <* 
    *>       } glPopMatrix   ();                                                      <* 
    *>    }                                                                           <* 
    *> }                                                                              <*/
   glPushMatrix    (); {
      yCOLOR_opengl (YCOLOR_BAS, YCOLOR_MOR, 0.65);
      glTranslatef (350, 50 , 250);
      i = j = 0;
      CREATE_line (0, SHAPE_DRAW,  0, 20, &i, &j);
      glTranslatef (  0, 15 ,   0);
      i = j = 0;
      CREATE_line (0, SHAPE_DRAW, 30, 20, &i, &j);
      glTranslatef (  0, 15 ,   0);
      i = j = 0;
      CREATE_line (0, SHAPE_DRAW, 60, 20, &i, &j);
      glTranslatef (  0, 15 ,   0);
      i = j = 0;
      CREATE_line (0, SHAPE_DRAW, 90, 20, &i, &j);
   } glPopMatrix   ();
   glPushMatrix    (); {
      yCOLOR_opengl (YCOLOR_BAS, YCOLOR_MOR, 0.65);
      glTranslatef (325, 50 , 250);
      i = j = 0;
      CREATE_circle (0, SHAPE_DRAW,  5,  0, &i, &j);
      glTranslatef (  5, 30 ,   0);
      i = j = 0;
      CREATE_circle (0, SHAPE_DRAW, 10,  0, &i, &j);
   } glPopMatrix   ();
   glPushMatrix    (); {
      yCOLOR_opengl (YCOLOR_BAS, YCOLOR_MOR, 0.65);
      glTranslatef (225, 25, 250);
      i = j = 0;
      CREATE_letter (SHAPE_DRAW  , "l", &i, &j);
      glTranslatef (  7,  4,   0);
      i = j = 0;
      CREATE_letter (SHAPE_DRAW  , "r", &i, &j);
      glTranslatef (  6,  3,   0);
      i = j = 0;
      CREATE_letter (SHAPE_DRAW  , "o", &i, &j);
      glTranslatef ( 20, 20,   0);
      i = j = 0;
      CREATE_letter (SHAPE_DRAW  , "g", &i, &j);
      glTranslatef ( 13, -3,   0);
      i = j = 0;
      CREATE_letter (SHAPE_DRAW  , "k", &i, &j);
      glTranslatef ( 10, -3,   0);
      i = j = 0;
      CREATE_letter (SHAPE_DRAW  , "u", &i, &j);
      /*> i = j = 0;                                                                  <* 
       *> CREATE_ellipse (SHAPE_CHECK,   0, 20,  10,  0, 180, 1, &i, &j);             <* 
       *> glTranslatef (  0, 30 ,   0);                                               <* 
       *> i = j = 0;                                                                  <* 
       *> CREATE_ellipse (SHAPE_CHECK,  30, 20,  10,  0, 360, 1, &i, &j);             <*/
   } glPopMatrix   ();
   glPushMatrix    (); {
      yCOLOR_opengl (YCOLOR_BAS, YCOLOR_MOR, 0.65);
      glTranslatef (150,  25, 250);
      g_fake.c = i = j = 0;
      CREATE_letter (SHAPE_DRAW  , "ag"   , &i, &j);
      CREATE_letter (SHAPE_DRAW  , "g"    , &i, &j);
      CREATE_letter (SHAPE_DRAW  , "n"    , &i, &j);
      glTranslatef (-50,  0,   0);
      g_fake.c = i = j = 0;
      CREATE_letter (SHAPE_DRAW  , "ag"   , &i, &j);
      CREATE_letter (SHAPE_DRAW  , "k"    , &i, &j);
      CREATE_letter (SHAPE_DRAW  , "t"    , &i, &j);
   } glPopMatrix   ();
   glPushMatrix    (); {
      yCOLOR_opengl (YCOLOR_BAS, YCOLOR_MOR, 0.65);
      glTranslatef (150,  50, 250);
      g_fake.c = i = j = 0;
      CREATE_letter (SHAPE_DRAW  , "al"   , &i, &j);
      CREATE_letter (SHAPE_DRAW  , "l"    , &i, &j);
      glTranslatef (-50,  0,   0);
      g_fake.c = i = j = 0;
      CREATE_letter (SHAPE_DRAW  , "al"   , &i, &j);
      CREATE_letter (SHAPE_DRAW  , "r"    , &i, &j);
   } glPopMatrix   ();
   glPushMatrix    (); {
      yCOLOR_opengl (YCOLOR_BAS, YCOLOR_MOR, 0.65);
      glTranslatef (175, 100, 250);
      g_fake.c = i = j = 0;
      CREATE_letter (SHAPE_DRAW  , "av"   , &i, &j);
      CREATE_letter (SHAPE_DRAW  , "v"    , &i, &j);
      glTranslatef (-50,  0,   0);
      g_fake.c = i = j = 0;
      CREATE_letter (SHAPE_DRAW  , "av"   , &i, &j);
      CREATE_letter (SHAPE_DRAW  , "f"    , &i, &j);
      glTranslatef (-25,  0,   0);
      g_fake.c = i = j = 0;
      CREATE_letter (SHAPE_DRAW  , "as"   , &i, &j);
      CREATE_letter (SHAPE_DRAW  , "s"    , &i, &j);
   } glPopMatrix   ();
   glPushMatrix    (); {
      yCOLOR_opengl (YCOLOR_BAS, YCOLOR_MOR, 0.65);
      glTranslatef (175, 150, 250);
      g_fake.c = i = j = 0;
      CREATE_letter (SHAPE_DRAW  , "ab"   , &i, &j);
      CREATE_letter (SHAPE_DRAW  , "b"    , &i, &j);
      glTranslatef (-50,  0,   0);
      g_fake.c = i = j = 0;
      CREATE_letter (SHAPE_DRAW  , "ab"   , &i, &j);
      CREATE_letter (SHAPE_DRAW  , "p"    , &i, &j);
      glTranslatef (-25,  0,   0);
      g_fake.c = i = j = 0;
      CREATE_letter (SHAPE_DRAW  , "az"   , &i, &j);
      CREATE_letter (SHAPE_DRAW  , "z"    , &i, &j);
   } glPopMatrix   ();
   glPushMatrix    (); {
      yCOLOR_opengl (YCOLOR_BAS, YCOLOR_MOR, 0.65);
      glTranslatef (175, 175, 250);
      g_fake.c = i = j = 0;
      CREATE_letter (SHAPE_DRAW  , "axm"  , &i, &j);
      CREATE_letter (SHAPE_DRAW  , "mm"   , &i, &j);
      glTranslatef (-50,  0,   0);
      g_fake.c = i = j = 0;
      CREATE_letter (SHAPE_DRAW  , "axm"  , &i, &j);
      CREATE_letter (SHAPE_DRAW  , "m"    , &i, &j);
      glTranslatef (-25,  0,   0);
      g_fake.c = i = j = 0;
      CREATE_letter (SHAPE_DRAW  , "axm"  , &i, &j);
      CREATE_letter (SHAPE_DRAW  , "n"    , &i, &j);
   } glPopMatrix   ();
   glPushMatrix    (); {
      yCOLOR_opengl (YCOLOR_BAS, YCOLOR_MOR, 0.65);
      glTranslatef (175, 200, 250);
      g_fake.c = i = j = 0;
      CREATE_letter (SHAPE_DRAW  , "ad"   , &i, &j);
      CREATE_letter (SHAPE_DRAW  , "dd"   , &i, &j);
      glTranslatef (-25,  0,   0);
      g_fake.c = i = j = 0;
      CREATE_letter (SHAPE_DRAW  , "ad"   , &i, &j);
      CREATE_letter (SHAPE_DRAW  , "d"    , &i, &j);
      glTranslatef (-25,  0,   0);
      g_fake.c = i = j = 0;
      CREATE_letter (SHAPE_DRAW  , "ad"   , &i, &j);
      CREATE_letter (SHAPE_DRAW  , "t"    , &i, &j);
   } glPopMatrix   ();
   glPushMatrix    (); {
      yCOLOR_opengl (YCOLOR_BAS, YCOLOR_MOR, 0.65);
      glTranslatef (175, 250, 250);
      g_fake.c = i = j = 0;
      CREATE_letter (SHAPE_DRAW  , "aj"   , &i, &j);
      CREATE_letter (SHAPE_DRAW  , "j"    , &i, &j);
      glTranslatef (-25,  0,   0);
      g_fake.c = i = j = 0;
      CREATE_letter (SHAPE_DRAW  , "aj"   , &i, &j);
      CREATE_letter (SHAPE_DRAW  , "ch"   , &i, &j);
      glTranslatef (-25,  0,   0);
      g_fake.c = i = j = 0;
      CREATE_letter (SHAPE_DRAW  , "aj"   , &i, &j);
      CREATE_letter (SHAPE_DRAW  , "sh"   , &i, &j);
   } glPopMatrix   ();
   glPushMatrix    (); {
      yCOLOR_opengl (YCOLOR_BAS, YCOLOR_MOR, 0.65);
      glTranslatef (175, 300, 250);
      g_fake.c = i = j = 0;
      CREATE_letter (SHAPE_DRAW  , "angk" , &i, &j);
      CREATE_letter (SHAPE_DRAW  , "ngk"  , &i, &j);
      glTranslatef (-50,  0,   0);
      g_fake.c = i = j = 0;
      CREATE_letter (SHAPE_DRAW  , "angk" , &i, &j);
      CREATE_letter (SHAPE_DRAW  , "ng"   , &i, &j);
   } glPopMatrix   ();
   /*> REVERSE_outline ("s·l·o2·b", SHAPE_DRAW, 1);                                   <*/
   /*> REVERSE_outline ("r·o2·b", SHAPE_DRAW, 1);                                     <*/
   /*> REVERSE_outline ("k·a·r"   , SHAPE_DRAW, 1);                                   <*/
   /*> REVERSE_outline ("r·ag·k·t·sh" , SHAPE_DRAW, 1);                               <*/
   /*> REVERSE_outline ("k·al·r" , SHAPE_DRAW, 1);                                    <*/
   /*> REVERSE_outline ("f·b·r·k·l·n" , SHAPE_DRAW, 1);                               <*/
   /*> REVERSE_outline ("f·b·r·k·v·l" , SHAPE_DRAW, 1);                               <*/
   /*> REVERSE_outline ("k·r·e·a·t" , SHAPE_DRAW, 1);                                 <*/
   /*> REVERSE_outline ("m·amj·sh·ejm·n"   , SHAPE_DRAW, 1);                          <*/
   /*> REVERSE_outline ("m·emd·d·edm·m"    , SHAPE_DRAW, 1);                          <*/
   /*> REVERSE_outline ("aT·tm"                  , SHAPE_DRAW, 1);                    <*/
   /*> REVERSE_outline ("aH·nd"                  , SHAPE_DRAW, 1);                    <*/
   /*> REVERSE_outline ("ap·b·r"                 , SHAPE_DRAW, 1);                    <*/
   /*> REVERSE_outline ("ar·l·g·lg"                , SHAPE_DRAW, 1);                 <*/
   /*> REVERSE_text    ("m·emd·d·edm·m m·amj·sh·ajm·n"    , SHAPE_DRAW, 1, 'y');      <*/
   /*> REVERSE_text    ("r·o2·b sh·t·nd"                        , SHAPE_DRAW, 1, 'y');   <*/
   /*---(cycle samples)------------------*/
   glPushMatrix(); {
      /*> glTranslatef (win.d_xoff - 50, win.d_yoff + 30, win.d_zoff);                <*/
      glTranslatef (win.d_xoff -  75, win.d_yoff + 125, win.d_zoff);
      /*> REVERSE_english_text ("rob shorthand"     , SHAPE_DRAW, 1, 'y');            <*/
      /*> REVERSE_english_text ("rob"               , SHAPE_LOAD, 1, 'y');            <*/
   } glPopMatrix();
   /*> REVERSE_english_text (my.words, SHAPE_DRAW, 1, 'y');                           <*/
   DRAW_info_request ();
   if (my.touch != 'y') {
      /*> DRAW_info_counts ();                                                        <*/
      /*> DRAW_info_base   ();                                                        <*/
      /*> DRAW_info_answer ();                                                        <*/
      /*> glPushMatrix    (); {                                                       <* 
       *>    glColor4f    (0.20f, 0.20f, 0.20f, 1.0f);                                <* 
       *>    glTranslatef (x_xmax - 50.0, x_ymin + 75.0,    0.0f);            <* 
       *>    yFONT_print  (win.font_pretty,  12, YF_BOTRIG, my.f_full);                   <* 
       *> } glPopMatrix   ();                                                         <*/
      /*> glPushMatrix    (); {                                                       <* 
       *>    glColor4f    (0.00f, 0.00f, 0.00f, 1.0f);                                <* 
       *>    glTranslatef (x_xmin +  3.0, x_ymin + 23.0,    0.0f);            <* 
       *>    yFONT_print  (win.font_pretty,  12, YF_BOTLEF, o.actual);                    <* 
       *>    glTranslatef (0.0              , -20.0            ,    0.0f);            <* 
       *>    yFONT_print  (win.font_pretty,  12, YF_BOTLEF, o.word);                      <* 
       *> } glPopMatrix   ();                                                         <*/
      /*> glPushMatrix    (); {                                                       <* 
       *>    glColor4f    (1.00f, 1.00f, 1.00f, 1.0f);                                <* 
       *>    glTranslatef ( 25.0,  -25.0, 0.0);                                       <* 
       *>    WORDS_result ();                                                         <* 
       *> } glPopMatrix   ();                                                         <*/
   }
   if (o.nraw > 0)  LAYER_raws ();
   /*> if (o.navg > 0 && my.time_point >= o.nraw) {                                   <*/
   if (o.navg > 0) {
      LAYER_base ();
      /*> LAYER_curr ();                                                              <*/
      LAYER_keys ();
   }
   /*---(complete)-----------------------*/
   DEBUG_GRAF   yLOG_exit    (__FUNCTION__);
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
   yVIEW_color_clear (YVIEW_MAIN);
   yVIEW_bounds    (YVIEW_MAIN, NULL, NULL, &x_xmin, &x_xmax, NULL, &x_ymin, &x_ymax, NULL);
   rc = yGLTEX_draw         (s_fbo, YGLTEX_GREGG, win.tex_w, win.tex_h, 2.0);
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
   rc = yGLTEX_done      (s_tex);
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
 *>       if (strncmp(g_groups [j].gr, "EOF", 5)              == 0) break;                <* 
 *>       if (g_groups [i].fl[0] != a_quad)  continue;                                    <* 
 *>       for (j = 0; j < MAX_LETTERS; ++j) {                                          <* 
 *>          if (strncmp(g_loc[j].label, "EOF", 5)              == 0) break;                 <* 
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
         glBegin(GL_POLYGON); {
            glVertex3f( minx,   200,  z);
            glVertex3f( maxx,   200,  z);
            glVertex3f( maxx,  miny,  z);
            glVertex3f( minx,  miny,  z);
         } glEnd();
      }
   }
   /*---(complete)-----------------------*/
   return 0;
}

char
DRAW_xaxis           (void)
{
   int         x_wide      =    0;
   int         x_pnt       =    0;
   int         x_deg       =    0;
   float       x_radius    =  3.0;
   float       x_spacing   = 14.0;
   float rad;
   float z     =  5.0;
   float x, y;
   int   right = 0;
   int   left  = 0;
   /*> DEBUG__SHAPES  if (g_loc[a_who].x_rig == 0) printf("   %-5s", g_loc[a_who].label);    <*/
   yVIEW_size  (YVIEW_XAXIS, NULL, NULL, &x_wide, NULL, NULL);
   x_spacing = (x_wide - 20.0) / (o.navg - 1);
   glPushMatrix(); {
      glTranslatef ( 10.0, 7.0,    0.0);
      for (x_pnt = 0; x_pnt < o.navg; ++x_pnt) {
         glLineWidth  (2.0);
         switch (o.avg [x_pnt].type) {
         case 'S' : yCOLOR_opengl (YCOLOR_BAS, YCOLOR_MIN, 0.4);  break;
         case '>' : yCOLOR_opengl (YCOLOR_BAS, YCOLOR_MOR, 0.4);  break;
         case '-' : yCOLOR_opengl (YCOLOR_BAS, YCOLOR_MOR, 0.4);  break;
         case 'F' : yCOLOR_opengl (YCOLOR_BAS, YCOLOR_MIN, 0.4);  break;
         }
         if (x_pnt == o.cavg)   yCOLOR_opengl (YCOLOR_BAS, YCOLOR_MAX, 0.3);
         glBegin(GL_LINE_STRIP);
         for (x_deg = 0; x_deg <= 360; x_deg += 5) {
            rad  = x_deg * DEG2RAD;
            x = x_radius * sin(rad);
            y = x_radius * cos(rad);
            glVertex3f( x, y, z);
         }
         glEnd();
         glTranslatef ( x_spacing, 0.0,    0.0);
      }
   } glPopMatrix();
   return 0;
}

char
DRAW_yaxis           (void)
{
   int         x_out       =    0;
   int         x_tall      =    0;
   float       x_radius    =    0;
   yVIEW_size  (YVIEW_YAXIS, NULL, NULL, NULL, NULL, &x_tall);
   x_radius  = (x_tall / (float) (o.total)) / 2.0;
   glPushMatrix(); {
      glTranslatef (  0.0, -x_radius,    0.0);
      for (x_out = o.total - 1; x_out >= 0; --x_out) {
         if (x_out == o.curr)   glColor4f    ( 1.0, 1.0, 1.0, 0.3); 
         else                   glColor4f    ( 0.5, 0.5, 0.5, 0.3); 
         glBegin(GL_POLYGON); {
            glVertex3f ( 2.0,  x_radius - 2.0,  20);
            glVertex3f (13.0,  x_radius - 2.0,  20);
            glVertex3f (13.0, -x_radius + 2.0,  20);
            glVertex3f ( 2.0, -x_radius + 2.0,  20);
         } glEnd();
         glTranslatef (  0.0, -x_radius * 2,    0.0);
      }
   } glPopMatrix();
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
      yCOLOR_opengl (YCOLOR_SPE, YCOLOR_WRN, 1.00);
      /*> glTranslatef( o.avg[o.cavg].x_pos, o.avg[o.cavg - 1].y_pos,    0.0);               <*/
      glTranslatef (win.d_xoff -  75, win.d_yoff + 125, win.d_zoff);
      glTranslatef( o.avg[o.cavg].x_pos * my.zoom, o.avg[o.cavg].y_pos * my.zoom,    0.0);
      glBegin(GL_POLYGON); {
         /*> glColor4f(0.0f, 0.0f, 0.0f, 1.0f);       /+ nice medium grey            +/   <*/
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
      /*> glColor4f (1.0f, 1.0f, 1.0f, 1.0f);                                         <*/
      glLineWidth (1.0);
      glBegin(GL_LINES); {
         glVertex3f( 0.0, 0.0, z);
         glVertex3f( -xa, -ya, z);
         glVertex3f( 0.0, 0.0, z);
         glVertex3f(  xa,  ya, z);
      } glEnd();
      glPointSize(5.0);
      /*> glColor4f (0.0f, 0.0f, 0.0f, 1.0f);                                         <*/
      glBegin(GL_POINTS); {
         glVertex3f( -xa, -ya, z);
      } glEnd();
      /*> glColor4f(1.0f, 1.0f, 0.0f, 1.0f);                                          <+/   <*/
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
   int     i;
   short       x_end       =     0;
   /*---(header)-------------------------*/
   DEBUG_GRAF   yLOG_enter   (__FUNCTION__);
   /*---(setup)--------------------------*/
   glPushMatrix (); {
      glTranslatef (win.d_xoff -  75, win.d_yoff + 125, win.d_zoff);
      glLineWidth (2.0);
      glPointSize (1.5);
      glColor4f  (0.0, 0.0, 0.0, 1.0);
      yCOLOR_opengl (YCOLOR_BAS, YCOLOR_MIN, 1.00);
      /*> glColor4f(0.0f, 0.0f, 0.7f, 1.0f);                                             <*/
      /*> glBegin(GL_LINE_STRIP);                                                        <*/
      DEBUG_GRAF   yLOG_value   ("nraw"      , o.nraw);
      DEBUG_GRAF   yLOG_value   ("time_point", my.time_point);
      if (my.time_point > o.nraw)  x_end = o.nraw;
      else                         x_end = my.time_point;
      DEBUG_GRAF   yLOG_value   ("x_end"     , x_end);
      glBegin(GL_POINTS);
      for (i = 0; i < x_end; ++i) {
         if (o.raw[i].x_pos == 999 && o.raw[i].y_pos == 999) {
            glEnd();
            glBegin(GL_POINTS);
            continue;
         }
         glVertex3f( o.raw[i].x_pos * my.zoom, o.raw[i].y_pos * my.zoom, z);
         DEBUG_GRAF   yLOG_complex ("point"     , "%4dx, %4dy, %4dz", o.raw[i].x_pos * my.zoom, o.raw[i].y_pos * my.zoom, z);
      }
      glEnd();
      if (my.time_point < o.nraw) {
         glPointSize (3.0);
         glColor4f   (1.0f, 0.0f, 0.0f, 1.0f);
         glBegin     (GL_POINTS);
         glVertex3f  (o.raw [my.time_point].x_pos * my.zoom, o.raw [my.time_point].y_pos * my.zoom, z);
         glEnd();
      }
      glLineWidth(0.8);
      if (my.time_point >= o.nraw) {
         my.time_lapse = '-';
         my.time_point = 9999;
      } else {
         my.time_point += 2;
      }
   } glPopMatrix ();
   /*---(complete)-----------------------*/
   DEBUG_GRAF   yLOG_exit    (__FUNCTION__);
   return 0;
}

char          /*----: draw the bas/avg points --------------------------------*/
LAYER_base          (void)
{
   /*---(locals)-------------------------*/
   float   z     =    5.25;
   int     i;
   glPushMatrix (); {
      glTranslatef (win.d_xoff -  75, win.d_yoff + 125, win.d_zoff);
      yCOLOR_opengl (YCOLOR_SPE, YCOLOR_INP, 1.00);
      for (i = 0; i < o.navg; ++i) {
         /*---(draw the point)--------------*/
         /*> if      (o.avg[i].sharp == 'x') glColor4f(1.0f, 0.0f, 0.0f, 1.0f);       <* 
          *> else if (o.avg[i].type  == 'm') glColor4f(1.0f, 0.0f, 1.0f, 1.0f);       <* 
          *> else if (o.bas[i].fake  == 'y') glColor4f(0.0f, 0.0f, 0.0f, 1.0f);       <* 
          *> else                            glColor4f(0.7f, 0.7f, 0.0f, 1.0f);       <*/
         if (o.bas[i].fake  == 'y')      glPointSize (4.0);
         else if (my.zoom <= 2)          glPointSize (1.0);
         else                            glPointSize (2.0);
         glBegin     (GL_POINTS); {
            glVertex3f( o.avg[i].x_pos * my.zoom, o.avg[i].y_pos * my.zoom, z);
            /*> glVertex3f( o.avg [i].x_pos + 125, o.avg [i].y_pos + 225, z);            <*/
         } glEnd();
         /*---(draw the S extension)--------*/
         if (o.avg [i].type == POINT_START) {
            glEnable      (GL_LINE_STIPPLE);
            glLineStipple (1, 0x3333);
            glLineWidth   (2.0);
            glBegin (GL_LINES); {
               glVertex3f (o.avg [i    ].x_pos * my.zoom, o.avg [i    ].y_pos * my.zoom, z - 1.50);
               glVertex3f (o.avg [i + 1].x_pos * my.zoom, o.avg [i + 1].y_pos * my.zoom, z - 1.50);
            } glEnd();
            glDisable     (GL_LINE_STIPPLE);
         }
         /*---(draw the F extension)--------*/
         if (o.avg [i].type == POINT_FINISH) {
            glEnable      (GL_LINE_STIPPLE);
            glLineStipple (1, 0x3333);
            glLineWidth   (2.0);
            glBegin (GL_LINES); {
               glVertex3f (o.avg [i    ].x_pos * my.zoom, o.avg [i    ].y_pos * my.zoom, z - 1.50);
               glVertex3f (o.avg [i - 1].x_pos * my.zoom, o.avg [i - 1].y_pos * my.zoom, z - 1.50);
            } glEnd();
            glDisable     (GL_LINE_STIPPLE);
         }
      }
      glLineWidth (0.8);
   } glPopMatrix ();
   return 0;
}

char          /*----: draw the key points and lines --------------------------*/
LAYER_keys          (void)
{
   /*---(locals)-------------------------*/
   float   r1 =   4.0;
   float   d;
   float   rad;
   float   x, y;
   float   z  =   5.75;
   int     pt = 0;
   int     i;
   /*---(draw keys)----------------------*/
   if      (my.zoom < 2)   r1 =  2.0;
   else if (my.zoom < 4)   r1 =  3.0;
   else                    r1 =  4.0;
   glPushMatrix (); {
      yCOLOR_opengl (YCOLOR_SPE, YCOLOR_ERR, 1.00);
      glTranslatef (win.d_xoff -  75, win.d_yoff + 125, win.d_zoff);
      for (i = 0; i < o.nkey; ++i) {
         /*---(draw the point)--------------*/
         glPushMatrix (); {
            glTranslatef  (o.key[i].x_pos * my.zoom, o.key[i].y_pos * my.zoom,    0.0);
            if (o.key[i].fake  == 'y') glColor4f (0.0f, 1.0f, 1.0f, 0.5f);
            glBegin (GL_POLYGON); {
               for (d = 0; d <= 360; d += 10) {
                  rad = d * DEG2RAD;
                  x   = r1 * cos (rad);
                  y   = r1 * sin (rad);
                  glVertex3f (x, y, z);
               }
            } glEnd ();
         } glPopMatrix ();
         /*---(draw the segment)------------*/
         if (o.key[i].type != '>') {
            glEnable      (GL_LINE_STIPPLE);
            glLineStipple (1, 0x3333);
            glLineWidth   (1.0);
            glBegin       (GL_LINES); {
               glVertex3f(o.key[i - 1].x_pos * my.zoom, o.key[i - 1].y_pos * my.zoom, z + 0.25);
               glVertex3f(o.key[i].x_pos * my.zoom,     o.key[i].y_pos * my.zoom,     z + 0.25);
            } glEnd();
            glDisable     (GL_LINE_STIPPLE);
         }
         /*---(draw the hidden lines)-------*/
         /*> if (i > 0 && o.key[i].type == '>') {                                                      <* 
          *>    glColor4f(0.5f, 0.5f, 0.5f, 1.0f);                                                     <* 
          *>    glLineWidth(2.0);                                                                      <* 
          *>    glBegin(GL_LINES); {                                                                   <* 
          *>       glVertex3f(o.key[i - 1].x_pos * my.zoom, o.key[i - 1].y_pos * my.zoom, z - 1.00);   <* 
          *>       glVertex3f(o.key[i].x_pos * my.zoom,     o.key[i].y_pos * my.zoom,     z - 1.00);   <* 
          *>    } glEnd();                                                                             <* 
          *> }                                                                                         <*/
         /*---(draw the S extension)--------*/
         /*> if (o.key[i].type == POINT_HEAD) {                                                        <* 
          *>    pt = o.key[i].p_bas - 1;                                                               <* 
          *>    glEnable      (GL_LINE_STIPPLE);                                                       <* 
          *>    glLineStipple (1, 0x3333);                                                             <* 
          *>    glColor4f     (0.2f, 0.2f, 0.2f, 1.0f);                                                <* 
          *>    glLineWidth   (2.0);                                                                   <* 
          *>    glBegin (GL_LINES); {                                                                  <* 
          *>       glVertex3f(o.bas[pt].x_pos * my.zoom    , o.bas[pt].y_pos * my.zoom  , z - 1.50);   <* 
          *>       glVertex3f(o.key[i].x_pos * my.zoom     , o.key[i].y_pos * my.zoom   , z - 1.50);   <* 
          *>    } glEnd();                                                                             <* 
          *>    glDisable     (GL_LINE_STIPPLE);                                                       <* 
          *> }                                                                                         <*/
         /*---(draw the F extension)--------*/
         /*> if (o.key[i + 1].type == '>' || i + 1 == o.nkey) {                                        <* 
          *>    pt = o.key[i].p_bas + 1;                                                               <* 
          *>    glEnable      (GL_LINE_STIPPLE);                                                       <* 
          *>    glLineStipple (1, 0x3333);                                                             <* 
          *>    glColor4f     (0.2f, 0.2f, 0.2f, 1.0f);                                                <* 
          *>    glLineWidth   (2.0);                                                                   <* 
          *>    glBegin (GL_LINES); {                                                                  <* 
          *>       glVertex3f(o.bas[pt].x_pos * my.zoom    , o.bas[pt].y_pos * my.zoom  , z - 1.50);   <* 
          *>       glVertex3f(o.key[i].x_pos * my.zoom     , o.key[i].y_pos * my.zoom   , z - 1.50);   <* 
          *>    } glEnd();                                                                             <* 
          *>    glDisable     (GL_LINE_STIPPLE);                                                       <* 
          *> }                                                                                         <*/
      }
   } glPopMatrix ();
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
   yCOLOR_opengl (YCOLOR_BAS, YCOLOR_MOR, 0.65);
   glPushMatrix  (); {
      /*---(counts)----------------------*/
      glTranslatef  (x_off, y_off, 30);
      snprintf      (t, 100, "%6d", o.nraw);
      FONT__label   ("raw#", t);
      snprintf      (t, 100, "%6d", o.navg);
      FONT__label   ("avg#", t);
      snprintf      (t, 100, "%6d", o.nkey);
      FONT__label   ("key#", t);
      /*---(basics)----------------------*/
      /*> glTranslatef  (0.0  , -10.0 , 0.0);                                         <*/
      snprintf      (t, 100, "%6d"  , o.avg[o.cavg].p_bas);
      FONT__label   ("num"   , t);
      snprintf      (t, 100, "%6d"  , o.avg[o.cavg].p_raw);
      FONT__label   ("raw"   , t);
      snprintf      (t, 100, "%6d"  , o.avg[o.cavg].x_raw);
      FONT__label   ("x_raw", t);
      snprintf      (t, 100, "%6d"  , o.avg[o.cavg].y_raw);
      FONT__label   ("y_raw", t);
      snprintf      (t, 100, "%6.1f", o.avg[o.cavg].x_pos);
      FONT__label   ("x_pos" , t);
      snprintf      (t, 100, "%6.1f", o.avg[o.cavg].y_pos);
      FONT__label   ("y_pos" , t);
      snprintf      (t, 100, "%6.1f", o.avg[o.cavg].xd);
      FONT__label   ("x-diff", t);
      snprintf      (t, 100, "%6.1f", o.avg[o.cavg].yd);
      FONT__label   ("y-diff", t);
      snprintf      (t, 100, "%6.1f", o.avg[o.cavg].len);
      FONT__label   ("len"   , t);
      /*---(more complex)----------------*/
      x_slope = o.avg[o.cavg].slope;
      x_icept = o.avg[o.cavg].icept;
      /*> if (x_slope >  999) { x_slope = 999.99; b = 999; }                          <* 
       *> if (x_slope < -999) { x_slope = -999.99; b = -999; }                        <*/
      snprintf      (t, 100, "%9.2f", x_slope);
      FONT__label   ("slope" , t);
      snprintf      (t, 100, "%6.1f",   x_icept);
      FONT__label   ("b-cept", t);
      snprintf      (t, 100, "%9.2f", o.avg[o.cavg].rads);
      FONT__label   ("rads"  , t);
      snprintf      (t, 100, "%6.1f",   o.avg[o.cavg].degs);
      FONT__label   ("degs"  , t);
      snprintf      (t, 100, "     %1d",   o.avg[o.cavg].quad);
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
DRAW_info_request    (void)
{
   /*---(locals)-----------+-----+-----+-*/
   char        t           [100];
   /*---(point counts)-------------------*/
   glColor4f     (0.0f, 0.0f, 0.0f, 1.0f);
   glPushMatrix  (); {
      glTranslatef(  win.d_reqx, win.d_reqy + 10.0, win.d_zoff);
      snprintf      (t, 100, "%s",   my.word);
      FONT__label   ("word"  , t);
      snprintf      (t, 100, "%s",   my.gregg);
      FONT__label   ("gregg" , t);
      snprintf      (t, 100, "%s",   my.shown);
      FONT__label   ("shown" , t);
      snprintf      (t, 100, "%s",   o.gregg);
      FONT__label   ("result", t);
   } glPopMatrix ();
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
   /*> DEBUG_GRAF  printf("   - draw_info()\n");                                      <*/
   char msg[100];
   glColor4f(1.0f, 1.0f, 0.0f, 1.0f);
   if (o.saved == 'y') {
      glPushMatrix();
      glTranslatef( -210.0, -150.0, win.d_zoff);
      snprintf(msg, 100, "%4d", o.curr);
      yFONT_print(win.font_pretty,  18, YF_BOTLEF, msg);
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
      snprintf(msg, 100, "%7.2f", o.key[xkey].depth);
      FONT__label ("de", msg);
      snprintf(msg, 100, "   %c", o.key[xkey].ratio);
      FONT__label ("rt"  , msg);
      snprintf(msg, 100, "  %2d", o.key[xkey].ccat);
      FONT__label ("cc"  , msg);
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
   /*> DEBUG_GRAF  printf("   - draw_info()          : complete\n");                  <*/
   return 0;
}


/*====================------------------------------------====================*/
/*===----                             fonts                            ----===*/
/*====================------------------------------------====================*/
static void      o___FONTS___________________o (void) {;}

char
FONT__load           (void)
{
   win.font_fixed = yFONT_load (win.face_fixed);
   if (win.font_fixed < 0) {
      fprintf(stderr, "Problem loading %s\n", win.face_fixed);
      exit(1);
   }
   win.font_pretty = yFONT_load (win.face_pretty);
   if (win.font_pretty < 0) {
      fprintf(stderr, "Problem loading %s\n", win.face_pretty);
      exit(1);
   }
   win.icons  = yFONT_iconload ();
   if (win.icons < 0) {
      fprintf(stderr, "Problem loading %s\n", ICON_SET);
      exit(1);
   }
   /*> yVIKEYS_view_font (win.font_fixed);                                            <*/
   return 0;
}

char
FONT__label          (char *a_label, char *a_content)
{
   glPushMatrix(); {
      yFONT_print  (win.font_fixed,  win.d_point, YF_BOTLEF, a_label);
      glTranslatef (   45,  0,  0);
      yFONT_print  (win.font_fixed,  win.d_point, YF_BOTLEF, ":");
      glTranslatef (   10,  0,  0);
      yFONT_print  (win.font_fixed,  win.d_point, YF_BOTLEF, a_content);
   } glPopMatrix ();
   glTranslatef(   0,          - (win.d_bar * 0.8),  0);
   return 0;
}

char
FONT__free           (void)
{
   yFONT_free (win.font_pretty);
   yFONT_free (win.font_fixed);
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
   /*---(title)--------------------------*/
   /*> glPushMatrix(); {                                                              <* 
    *>    glScalef              (1.0, 1.0, 1.0);                                      <* 
    *>    glTranslatef          (    0.0,   -5.0,    0.0);                            <* 
    *>    yFONT_print           (win.font_fixed,  5, YF_TOPLEF, "writing samples");      <* 
    *> } glPopMatrix();                                                               <*/
   /*---(cycle samples)------------------*/
   glPushMatrix(); {
      /*> glTranslatef (win.d_xoff - 50, win.d_yoff + 30, win.d_zoff);                <*/
      glTranslatef (win.d_xoff + 100, win.d_yoff + 250, win.d_zoff);
      glScalef              (1.5, 1.5, 1.5);
      for (i = 1; i < MAX_LETTERS && strncmp(g_loc[i].label, "EOF", 5) != 0; ++i) {
         if (g_loc [i].x_show == 0 && g_loc [i].y_show == 0)  continue;
         glPushMatrix();
         yCOLOR_opengl (YCOLOR_BAS, YCOLOR_MIN, 1.0);
         glTranslatef(g_loc [i].x_show, g_loc [i].y_show,  0.0);
         yFONT_print (win.font_fixed,  5, g_loc [i].align, g_loc [i].label);
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
 *>       yFONT_print           (win.font_fixed,  5, YF_TOPLEF, "writing samples");                <* 
 *>    } glPopMatrix();                                                                         <* 
 *>    /+---(cycle samples)------------------+/                                                 <* 
 *>    glPushMatrix(); {                                                                        <* 
 *>       glScalef(2.0, 2.0, 2.0);                                                              <* 
 *>       for (i = 0; i < MAX_LETTERS && strncmp(g_loc[i].label, "EOF", 5) != 0; ++i) {               <* 
 *>          glPushMatrix();                                                                    <* 
 *>          glTranslatef(g_loc[i].x_show, g_loc[i].y_show,  0.0);                                          <* 
 *>          yFONT_print(win.font_fixed,  5, YF_BOTRIG, g_loc[i].label);                                 <* 
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
