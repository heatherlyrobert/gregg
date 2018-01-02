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


char      draw_bands (void);


/*---(opengl objects)--------------------*/
static uint s_tex       =     0;            /* texture for image              */
static uint s_fbo       =     0;            /* framebuffer                    */
static uint s_depth     =     0;            /* depth buffer                   */



/*====================------------------------------------====================*/
/*===----                       program level                          ----===*/
/*====================------------------------------------====================*/
static void      o___PROGRAM_________________o (void) {;}

char         /*-> tbd --------------------------------[ shoot  [gz.742.001.01]*/ /*-[00.0000.112.!]-*/ /*-[--.---.---.--]-*/
DRAW_init            (void)
{
   /*---(locals)-----------+-----+-----+-*/
   char        rc          =    0;
   /*---(header)-------------------------*/
   DEBUG_GRAF   yLOG_enter   (__FUNCTION__);
   /*---(stuff)--------------------------*/
   rc = DRAW__resize ('i', "gregg_input"    , 520, 370);
   rc = yXINIT_start (win.w_title, win.w_wide, win.w_tall, YX_FOCUSABLE, YX_FIXED, '-');
   rc = yGLTEX_init     ();
   glClearColor    (0.3f, 0.5f, 0.3f, 1.0f);       /* nice medium green          */
   rc = FONT__load      ();
   rc = dlist_init ();
   win.tex_h        =  700;
   win.tex_w        = 1000;
   rc = yGLTEX_new (&s_tex, &s_fbo, &s_depth, win.tex_w, win.tex_h);
   rc = DRAW_back ();
   /*---(complete)-----------------------*/
   DEBUG_GRAF   yLOG_exit    (__FUNCTION__);
   return 0;
}

char
DRAW__resize       (cchar a_format, cchar *a_title, cint a_wide, cint a_tall)
{
   /*---(header)----------------------*/
   DEBUG_GRAF   yLOG_enter   (__FUNCTION__);
   /*---(text fields)--------------------*/
   DEBUG_GRAF   yLOG_snote   ("text");
   win.w_format = a_format;
   strlcpy (win.w_title, a_title, LEN_DESC);
   DEBUG_GRAF   yLOG_info    ("w_title"   , win.w_title);
   strlcpy (win.t_text , "Gregg Shorthand Interpreter", LEN_DESC);
   DEBUG_GRAF   yLOG_info    ("t_text"    , win.t_text);
   strlcpy (win.c_text , ":command line"              , LEN_DESC);
   DEBUG_GRAF   yLOG_info    ("c_text"    , win.c_text);
   /*---(widths)-------------------------*/
   win.w_wide = a_wide;
   win.t_wide =  20;
   win.m_wide = win.c_wide = a_wide - win.t_wide;
   /*---(lefts)--------------------------*/
   win.t_left =   0;
   win.m_left = win.c_left = win.t_wide;
   /*---(talls)--------------------------*/
   win.w_tall = win.t_tall = a_tall;
   win.c_tall =  20;  
   win.m_tall = a_tall - win.c_tall;
   /*---(bottoms)------------------------*/
   win.c_bott = win.t_bott =   0;  
   win.m_bott = win.c_bott + win.c_tall;
   /*---(main)---------------------------*/
   win.m_ymax   =  125;
   win.m_ymin   =  win.m_ymax - win.m_tall;
   win.m_xmin   = -125;
   win.m_xmax   =  win.m_xmin + win.m_wide;
   /*---(detailed text)------------------*/
   win.d_xoff      =   200;
   win.d_yoff      =   100;
   win.d_zoff      =   100;
   win.d_point     =     8;
   win.d_bar       =    20;
   win.d_ansx      =   200;
   win.d_ansy      =  -160;
   /*---(readout)------------------------*/
   DEBUG_GRAF   yLOG_complex ("window"    , "bott %3d, left %3d, wide %3d, tall %3d", 0         , 0         , win.w_wide, win.w_tall);
   DEBUG_GRAF   yLOG_complex ("title"     , "bott %3d, left %3d, wide %3d, tall %3d", win.t_bott, win.t_left, win.t_wide, win.t_tall);
   DEBUG_GRAF   yLOG_complex ("command"   , "bott %3d, left %3d, wide %3d, tall %3d", win.c_bott, win.c_left, win.c_wide, win.c_tall);
   DEBUG_GRAF   yLOG_complex ("primary"   , "bott %3d, left %3d, wide %3d, tall %3d", win.m_bott, win.m_left, win.m_wide, win.m_tall);
   DEBUG_GRAF   yLOG_complex ("data"      , "xoff %3d, yoff %3d, zoff %3d, pnt  %3d", win.d_xoff, win.d_yoff, win.d_zoff, win.d_point);
   DEBUG_GRAF   yLOG_complex ("answer"    , "ansx %3d, ansy %3d, bar  %3d"          , win.d_ansx, win.d_ansy, win.d_bar);
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
   rc = yXINIT_end();
   /*---(complete)-----------------------*/
   DEBUG_GRAF   yLOG_exit    (__FUNCTION__);
   return 0;
}



/*====================------------------------------------====================*/
/*===----                        panel display                         ----===*/
/*====================------------------------------------====================*/
static void      o___PANELS__________________o (void) {;}

char
DRAW_primary         (void)
{
   /*---(setup view)---------------------*/
   glViewport      (win.m_left, win.m_bott, win.m_wide, win.m_tall);
   glMatrixMode    (GL_PROJECTION);
   glLoadIdentity  ();
   glOrtho         (win.m_xmin, win.m_xmax, win.m_ymin, win.m_ymax,  -500.0,   500.0);
   glMatrixMode    (GL_MODELVIEW);
   /*---(background)---------------------*/
   glPushMatrix    (); {
      glBindTexture(GL_TEXTURE_2D, s_tex);
      /*> glColor4f    (0.20f, 0.20f, 0.20f, 1.0f);                                   <*/
      glBegin         (GL_POLYGON); {
         glTexCoord2d (     0.00 ,      1.00);
         glVertex3f  (win.m_xmin + 3, win.m_ymax - 3,  0.0f);
         glTexCoord2d (     1.00 ,      1.00);
         glVertex3f  (win.m_xmax - 3, win.m_ymax - 3,  0.0f);
         glTexCoord2d (     1.00 ,      0.00);
         glVertex3f  (win.m_xmax - 3, win.m_ymin + 3,  0.0f);
         glTexCoord2d (     0.00 ,      0.00);
         glVertex3f  (win.m_xmin + 3, win.m_ymin + 3,  0.0f);
      } glEnd   ();
      glBindTexture(GL_TEXTURE_2D, 0);
   } glPopMatrix   ();
   /*---(background)---------------------*/
   /*> glPushMatrix(); {                                                              <* 
    *>    glBindTexture(GL_TEXTURE_2D, s_tex);                                        <* 
    *>    glBegin(GL_QUAD_STRIP); {                                                   <* 
    *>       glTexCoord2d (     0.00 ,      0.00);                                    <* 
    *>       glVertex3f   (win.m_xmin, win.m_ymin,   0.00);                           <* 
    *>       glTexCoord2d (     0.00 ,      1.00);                                    <* 
    *>       glVertex3f   (win.m_xmin, win.m_ymax,   0.00);                           <* 
    *>       glTexCoord2d (     1.00 ,      0.00);                                    <* 
    *>       glVertex3f   (win.m_xmax, win.m_ymin,   0.00);                           <* 
    *>       glTexCoord2d (     1.00 ,      1.00);                                    <* 
    *>       glVertex3f   (win.m_xmax, win.m_ymax,   0.00);                           <* 
    *>    } glEnd();                                                                  <* 
    *>    glBindTexture(GL_TEXTURE_2D, 0);                                            <* 
    *> } glPopMatrix();                                                               <*/
   /*---(parts)--------------------------*/
   /*> DRAW_slide_avg ();                                                             <*/
   if (my.touch != 'y') {
      DRAW_base        ();
      DRAW_keys        ();
      DRAW_curr        ();
      /*> DRAW_info_counts ();                                                        <* 
       *> DRAW_info_base   ();                                                        <* 
       *> DRAW_info_answer ();                                                        <*/
   }
   /*---(complete)-----------------------*/
   return;
}

char
DRAW_title         (void)
{
   /*---(setup view)---------------------*/
   glViewport      (win.t_left, win.t_bott, win.t_wide, win.t_tall);
   glMatrixMode    (GL_PROJECTION);
   glLoadIdentity  ();
   glOrtho         ( 0.0f, win.t_wide, 0.0f, win.t_tall,  -500.0,   500.0);
   glMatrixMode    (GL_MODELVIEW);
   /*---(background)---------------------*/
   glPushMatrix    (); {
      glColor4f    (0.40f, 0.40f, 0.00f, 1.0f);
      glBegin         (GL_POLYGON); {
         glVertex3f  (0.0f      , win.t_tall,  0.0f);
         glVertex3f  (win.t_wide, win.t_tall,  0.0f);
         glVertex3f  (win.t_wide, 0.0f     ,  0.0f);
         glVertex3f  (0.0f      , 0.0f     ,  0.0f);
      } glEnd   ();
   } glPopMatrix   ();
   /*---(display)------------------------*/
   glPushMatrix    (); {
      glTranslatef (win.t_wide,   5.0f,    0.0f);
      glColor4f    (0.00f, 0.00f, 0.00f, 1.00f);
      glRotatef    ( 90.0, 0.0f, 0.0f, 1.0f);
      yFONT_print  (win.font_bg,  12, YF_BOTLEF, win.t_text);
      glTranslatef (win.t_tall - 10.0,   0.0f,    0.0f);
      yFONT_print  (win.font_bg,  12, YF_BOTRIG, VER_NUM);
   } glPopMatrix   ();
   /*---(complete)-----------------------*/
   return;
}

char
DRAW_command       (void)
{
   /*---(setup view)---------------------*/
   glViewport      (win.c_left, win.c_bott, win.c_wide, win.c_tall);
   glMatrixMode    (GL_PROJECTION);
   glLoadIdentity  ();
   glOrtho         ( 0.0f, win.w_wide, 0.0f, win.c_tall,  -500.0,   500.0);
   glMatrixMode    (GL_MODELVIEW);
   /*---(background)---------------------*/
   glPushMatrix    (); {
      glColor4f    (0.00f, 0.00f, 0.15f, 1.0f);
      glBegin         (GL_POLYGON); {
         glVertex3f  (0.0f      , win.c_tall,  0.0f);
         glVertex3f  (win.w_wide, win.c_tall,  0.0f);
         glVertex3f  (win.w_wide, 0.0f     ,  0.0f);
         glVertex3f  (0.0f      , 0.0f     ,  0.0f);
      } glEnd   ();
   } glPopMatrix   ();
   /*---(display)------------------------*/
   glPushMatrix    (); {
      glColor4f    (1.00f, 1.00f, 1.00f, 1.00f);
      glTranslatef (    5.0f,    0.0f,    0.0f);
      yFONT_print  (win.font_bg,  12, YF_BOTLEF, win.c_text);
      glTranslatef (win.c_wide - 10.0,   0.0f,    0.0f);
      yFONT_print  (win.font_bg,  12, YF_BOTRIG, my.keys);
   } glPopMatrix   ();
   /*---(complete)-----------------------*/
   return;
}

char
DRAW_back            (void)
{
   /*---(locals)-----------+-----------+-*/
   char        rce         =  -10;
   char        rc          =    0;
   /*---(header)-------------------------*/
   DEBUG_GRAF   yLOG_enter   (__FUNCTION__);
   /*---(setup)--------------------------*/
   rc = yGLTEX_draw_start   (s_fbo, YGLTEX_GREGG, win.tex_w, win.tex_h, 2.0);
   /*---(draw)---------------------------*/
   glCallList (dl_back);
   draw_horz   ();
   DRAW_slide_avg ();
   sample_show ();
   /*---(mipmaps)------------------------*/
   rc = yGLTEX_draw_end  (s_tex);
   /*---(complete)-----------------------*/
   DEBUG_GRAF   yLOG_exit    (__FUNCTION__);
   return 0;
}



/*====================------------------------------------====================*/
/*===----                           drivers                            ----===*/
/*====================------------------------------------====================*/
static void      o___DRIVERS_________________o (void) {;}

char
DRAW_main (void)
{
   /*---(locals)-----------+------+------+-*/
   long        x_start;
   long        x_stop;
   /*---(header)-------------------------*/
   DEBUG_WIND   yLOG_enter    (__FUNCTION__);
   /*> x_start = time_stamp();                                                        <*/
   glClear         (GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
   /*> glLoadIdentity();                                                              <*/
   /*---(draw)-----------------------------*/
   /*> DRAW_info();                                                                   <*/
   DEBUG_WIND   yLOG_note     ("post-godview");
   /*> glCallList (dl_back);                                                          <*/
   DEBUG_WIND   yLOG_note     ("post-displist");
   DRAW_title       ();
   DRAW_command     ();
   DRAW_primary     ();
   DRAW_cursor ();
   /*> draw_oslider();                                                                <*/
   /*> draw_kslider();                                                                <*/
   /*> DRAW_raws   ();                                                                <*/
   /*> draw_avgs   ();                                                                <*/
   /*> DRAW_keys   ();                                                                <*/
   /*> DRAW_curr   ();                                                                <*/
   /*> draw_bands  ();                                                                <*/
   /*> draw_saved  ();                                                                <*/
   /*> draw_horz   ();                                                                <*/
   /*> words_result ();                                                               <*/
   /*> sample_draw ();                                                                <*/
   /*---(word)-----------------------------*/
   /*> glPushMatrix();                                                                <* 
    *> glColor4f(1.0f, 1.0f, 0.0f, 1.0f);                                             <* 
    *> glTranslatef(  -40.0,  25.0, 10.0);                                            <* 
    *> yFONT_print(win.font_bg,  18, YF_BOTLEF, o.word);                               <* 
    *> glTranslatef(    0.0, -85.0,  0.0);                                            <* 
    *> yFONT_print(win.font_bg,  18, YF_BOTLEF, o.actual);                             <* 
    *> glPopMatrix();                                                                 <*/
   DEBUG_WIND   yLOG_note     ("post-printing");
   /*> words_display("MA MAD MAM MANK MACH MATN MANT MAF MAP MAR MAK AM");            <*/
   /*> words_display("DA DAD DAM DANK DACH DATN DANT DAF DAP DAR DAK AD");            <*/
   /*> words_display("CHA CHAD CHAM CHANK CHACH CHATN CHANT CHAF CHAP CHAR CHAK ACH");   <*/
   /*> words_display("NGA NGAD NGAM NGANK NGACH NGATN NGANT NGAF NGAP NGAR NGAK ANG");   <*/
   /*> words_display("KA KAD KAM KANK KACH KATN KANT KAF KAP KAR KAK AK");            <*/
   /*> words_display("RA RAD RAM RANK RACH RATN RANT RAF RAP RAR RAK AR");            <*/
   /*> words_display("FA FAD FAM FANK FACH FATN FANT FAF FAP FAR FAK AF");            <*/
   /*> words_display("PA PAD PAM PANK PACH PATN PANT PAF PAP PAR PAK AP");            <*/
   /*> words_display("TNA TNAD TNAM TNANK TNACH TNATN TNANT TNAF TNAP TNAR TNAK ATN");   <*/
   /*> words_display("NTA NTAD NTAM NTANK NTACH NTATN NTANT NTAF NTAP NTAR NTAK ANT");   <*/
   /*> words_display("get letter because matter say task late different draft cover project table discount fact first following had forward house line appreciate about");   <*/
   /*---(force the redraw)-----------------*/
   glXSwapBuffers(DISP, BASE);
   glFlush();
   /*---(complete)-------------------------*/
   /*> x_stop  = time_stamp();                                                        <*/
   /*---(complete)-----------------------*/
   DEBUG_WIND   yLOG_exit     (__FUNCTION__);
   return 0;
}

char
DRAW_work          (void)
{
}

long
time_stamp()                      /* PURPOSE : timestamp in microseconds      */
{
   /* second
    * millisecond  ms  0.001 sec
    * microsecond  us  0.000001 sec
    * nanosecond   ns  0.000000001 sec
    * picosecond   ps  0.000000000001 sec
    */
   long             tint = 0;
   struct timeval   tv;
   gettimeofday(&tv, NULL);
   tint += (int) (tv.tv_sec * 1000);
   tint += (int) tv.tv_usec / 1000;
   return tint;

}



/*====================------------------------------------====================*/
/*===----                            sliders                           ----===*/
/*====================------------------------------------====================*/
static void      o___SLIDERS_________________o (void) {;}

char
DRAW_slide_avg       (void)
{
   float   x_lef = win.m_xmin;
   float   x_rig = win.m_xmax +  2;
   float   x_top = win.m_ymin + 40;
   float   x_bot = win.m_ymin;
   float   x_cnt =  50;
   float   x_inc = (x_rig - x_lef) / x_cnt;
   float   z     =   20.0;
   float   x1, x2;
   int     i = 0;
   /*---(first)----------------------------*/
   /*> glColor4f(0.3f, 0.3f, 0.0f, 1.0f);       /+ nice medium grey            +/     <* 
    *>    glBegin(GL_POLYGON);                                                        <* 
    *>    glVertex3f( x_lef, x_top, z);                                               <* 
    *>    glVertex3f( x_rig, x_top, z);                                               <* 
    *>    glVertex3f( x_rig, x_bot, z);                                               <* 
    *>    glVertex3f( x_lef, x_bot, z);                                               <* 
    *>    glEnd();                                                                    <*/
   glColor4f(0.4f, 0.2f, 0.1f, 0.3f);       /* nice medium grey            */
   for (i = 0; i < x_cnt; i++) {
      x1 = x_lef + (x_inc * i);
      x2 = x1 + (x_inc * 0.70);
      glBegin(GL_POLYGON);
      glVertex3f( x1, x_top, z);
      glVertex3f( x2, x_top, z);
      glVertex3f( x2, x_bot, z);
      glVertex3f( x1, x_bot, z);
      glEnd();
   }
   /*---(second)---------------------------*/
   /*> glLineWidth(1.2);                                                              <* 
    *> glColor4f(0.7f, 0.7f, 0.7f, 1.0f);                                             <* 
    *> glBegin(GL_LINE_STRIP);                                                        <* 
    *> glVertex3f( x_lef, x_top, z + 0.1);                                            <* 
    *> glVertex3f( x_rig, x_top, z + 0.1);                                            <* 
    *> glVertex3f( x_rig, x_bot, z + 0.1);                                            <* 
    *> glVertex3f( x_lef, x_bot, z + 0.1);                                            <* 
    *> glVertex3f( x_lef, x_top, z + 0.1);                                            <* 
    *> glEnd();                                                                       <*/
   /*---(third)----------------------------*/
   /*> glLineWidth(0.8);                                                              <* 
    *> if (o.navg == 0) return 0;                                                     <* 
    *> glColor4f(0.7f, 0.7f, 0.7f, 1.0f);                                             <* 
    *> float x_inc = (x_top - x_bot) / o.navg;                                        <*/
   /*---(current point)-------------------------*/
   /*> glPushMatrix();                                                                        <* 
    *> glTranslatef( x_lef, x_top - (x_inc * o.cavg) + x_inc,    0.0);                        <* 
    *> /+> printf("%d\n", o.cavg);                                                      <+/   <* 
    *> glColor4f(0.0f, 0.0f, 0.0f, 1.0f);                                                     <* 
    *> glBegin(GL_POLYGON);                                                                   <* 
    *> glVertex3f(  0,      0, 20.0);                                                         <* 
    *> glVertex3f( 15,      0, 20.0);                                                         <* 
    *> glVertex3f( 15, -x_inc, 20.0);                                                         <* 
    *> glVertex3f(  0, -x_inc, 20.0);                                                         <* 
    *> glEnd();                                                                               <* 
    *> glPopMatrix();                                                                         <*/
   /*---(complete)------------------------------*/
   return 0;
}

char
draw_kslider()
{
   float       i           = 0.0;
   float   x_lef = -win.m_xmin + (win.d_bar * 3.00);
   float   x_rig =  win.m_xmax - (win.d_bar * 1.20);
   float   x_top =  win.m_ymax - (win.d_bar * 0.85);
   float   x_bot =  win.m_ymax - (win.d_bar * 0.15);
   float   z     =   10.0;
   glColor4f(0.3f, 0.6f, 0.3f, 1.0f);       /* nice medium grey            */
   glBegin(GL_POLYGON);
   glVertex3f( x_lef, x_top, z);
   glVertex3f( x_rig, x_top, z);
   glVertex3f( x_rig, x_bot, z);
   glVertex3f( x_lef, x_bot, z);
   glEnd();
   glLineWidth(1.2);
   glColor4f(0.7f, 0.7f, 0.7f, 1.0f);
   glBegin(GL_LINE_STRIP);
   glVertex3f( x_lef, x_top, z + 0.1);
   glVertex3f( x_rig, x_top, z + 0.1);
   glVertex3f( x_rig, x_bot, z + 0.1);
   glVertex3f( x_lef, x_bot, z + 0.1);
   glVertex3f( x_lef, x_top, z + 0.1);
   glEnd();
   glLineWidth(0.8);
   if (o.nkey == 0) return 0;
   glColor4f(0.7f, 0.7f, 0.7f, 1.0f);
   float x_inc = (x_rig - x_lef - 10) / o.nkey;
   for (i = x_lef +  5; i <= x_rig -  5; i += x_inc) {
      glBegin(GL_LINES);
      glVertex3f( i, x_top, z + 1);
      glVertex3f( i, x_bot, z + 1);
      glEnd();
   }
   glBegin(GL_LINES);
   glVertex3f( x_rig -  5, x_top, z + 1);
   glVertex3f( x_rig -  5, x_bot, z + 1);
   glEnd();
   glColor4f(0.2f, 0.4f, 0.2f, 1.0f);       /* nice medium grey            */
   glBegin(GL_POLYGON);
   glVertex3f( x_rig     , x_top, z);
   glVertex3f( x_rig     , x_bot, z);
   glVertex3f( x_rig -  5, x_bot, z);
   glVertex3f( x_rig -  5, x_top, z);
   glEnd();
   glBegin(GL_POLYGON);
   glVertex3f( x_lef     , x_top, z);
   glVertex3f( x_lef     , x_bot, z);
   glVertex3f( x_lef +  5, x_bot, z);
   glVertex3f( x_lef +  5, x_top, z);
   glEnd();
   /*---(complete)------------------------------*/
   return 0;
}

char
draw_oslider()
{
   float       i           = 0.0;
   float   x_lef =  win.m_xmax - (win.d_bar * 1.0);
   float   x_rig =  win.m_xmax - (win.d_bar * 0.2);
   float   x_top =  win.m_ymax - (win.d_bar * 0.2);
   float   x_bot = -win.m_ymin + (win.d_bar * 0.2);
   float   z     =   10.0;
   /*---(filler)----------------------------*/
   glColor4f(0.3f, 0.6f, 0.3f, 1.0f);       /* nice medium grey            */
   glBegin(GL_POLYGON);
   glVertex3f( x_lef, x_top, z);
   glVertex3f( x_rig, x_top, z);
   glVertex3f( x_rig, x_bot, z);
   glVertex3f( x_lef, x_bot, z);
   glEnd();
   /*---(outer box)-------------------------*/
   /*> glLineWidth(1.0);                                                              <* 
    *> glColor4f(0.7f, 0.7f, 0.7f, 0.5f);                                             <* 
    *> glBegin(GL_LINE_STRIP);                                                        <* 
    *> glVertex3f( x_lef, x_top, z + 0.1);                                            <* 
    *> glVertex3f( x_rig, x_top, z + 0.1);                                            <* 
    *> glVertex3f( x_rig, x_bot, z + 0.1);                                            <* 
    *> glVertex3f( x_lef, x_bot, z + 0.1);                                            <* 
    *> glVertex3f( x_lef, x_top, z + 0.1);                                            <* 
    *> glEnd();                                                                       <*/
   /*---(inner boxes)-----------------------*/
   glLineWidth(0.8);
   if (o.total == 0) return 0;
   glColor4f(0.2f, 0.4f, 0.2f, 1.0f);
   float x_inc = (x_top - x_bot) / o.total;
   for (i = x_bot; i <= x_top; i += x_inc) {
      glBegin(GL_LINES);
      glVertex3f( x_lef, i, z + 0.2);
      glVertex3f( x_rig, i, z + 0.2);
      glEnd();
   }
   /*---(current point)---------------------*/
   /*> float   r1 =   5.0;                                                                 <* 
    *> float   d;                                                                          <* 
    *> float   rad;                                                                        <* 
    *> float   x, y;                                                                       <* 
    *> glPushMatrix();                                                                     <* 
    *> glTranslatef( x_lef + 7, x_top - 5 - (x_inc * o.curr) + (x_inc * 0.5),    0.0);   <* 
    *> glBegin(GL_POLYGON);                                                                <* 
    *> glColor4f(1.0f, 1.0f, 1.0f, 1.0f);                                                  <* 
    *> for (d = 0; d <= 360; d += 10) {                                                    <* 
    *>    rad = d * DEG2RAD;                                                               <* 
    *>    x   = r1 * cos(rad);                                                             <* 
    *>    y   = r1 * sin(rad);                                                             <* 
    *>    glVertex3f( x, y, z);                                                            <* 
    *> }                                                                                   <* 
    *> glEnd();                                                                            <* 
    *> glPopMatrix();                                                                      <*/
   /*---(complete)--------------------------*/
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
 *>          if (strncmp(loc[j].n, "eof", 5)              == 0) break;                 <* 
 *>       }                                                                            <* 
 *>    }                                                                               <* 
 *>    /+---(complete)-----------------------+/                                        <* 
 *>    return 0;                                                                       <* 
 *> }                                                                                  <*/

char          /*----: draw horizontal guides ---------------------------------*/
draw_horz          (void)
{
   float z = 6.0;
   glLineWidth(1.0);
   glColor4f(1.0f, 0.0f, 0.0f, 1.0f);
   glBegin(GL_LINES);
   glVertex3f(   0.0,   3.5, z);
   glVertex3f( 200.0,   3.5, z);
   glVertex3f(   0.0,  16.0, z);
   glVertex3f( 200.0,  16.0, z);
   glVertex3f(   0.0,  25.0, z);
   glVertex3f( 200.0,  25.0, z);
   glVertex3f(   0.0,  -3.5, z);
   glVertex3f( 200.0,  -3.5, z);
   glVertex3f(   0.0, -16.0, z);
   glVertex3f( 200.0, -16.0, z);
   glVertex3f(   0.0, -25.0, z);
   glVertex3f( 200.0, -25.0, z);
   glEnd();
   return 0;
}

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

char          /*----: draw the saved status ----------------------------------*/
draw_saved         (void)
{
   /*---(locals)-------------------------*/
   float     d, rad;                   /* degrees and radians                 */
   float     x, y, z;                  /* cartesian coordinates               */
   char      msg[10];
   z = 6.0;
   glLineWidth(3.0);
   glBegin(GL_LINE_STRIP);
   if (o.saved == 'y') glColor4f(0.0f, 1.0f, 0.0f, 0.5f);
   else                glColor4f(1.0f, 0.0f, 0.0f, 0.5f);
   for (d = 0; d <= 360; d +=  1) {
      rad = d * DEG2RAD;
      x   = SIZE_R1 * cos(rad);
      y   = SIZE_R1 * sin(rad);
      glVertex3f( x, y, z);
   }
   glEnd();
   return 0;
}

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
   out_minx = o.key[0].xpos;
   out_maxx = o.key[0].xpos;
   out_miny = o.key[0].ypos;
   out_maxy = o.key[0].ypos;
   for (i = 0; i < o.nkey; ++i) {
      /*---(update outline bounds)-------*/
      if (o.key[i].xpos < out_minx)  out_minx = o.key[i].xpos;
      if (o.key[i].xpos > out_maxx)  out_maxx = o.key[i].xpos;
      if (o.key[i].ypos < out_miny)  out_miny = o.key[i].ypos;
      if (o.key[i].ypos > out_maxy)  out_maxy = o.key[i].ypos;
      if (o.key[i].use[0] == 'a' || o.key[i].use[0] == 'A' ||
            o.key[i].use[0] == 'e' || o.key[i].use[0] == 'E') {
         /*---(update circle bounds)-----*/
         minx = o.key[i].xpos;
         maxx = o.key[i].xpos;
         maxy = o.key[i].ypos;
         miny = o.key[i].ypos;
         for (j = i + 1; j < o.nkey; ++j) {
            if (o.key[j].use[0] != '+') break;
            if (o.key[j].xpos < minx)  minx = o.key[j].xpos;
            if (o.key[j].xpos > maxx)  maxx = o.key[j].xpos;
            if (o.key[j].ypos < miny)  miny = o.key[j].ypos;
            if (o.key[j].ypos > maxy)  maxy = o.key[j].ypos;
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

char          /*----: draw the current point ---------------------------------*/
DRAW_curr          (void)
{
   /*---(locals)-------------------------*/
   float   r1 =   2.0;
   float   d;
   float   rad;
   float   x, y;
   float   z  =   5.25;
   /*---(draw current point)-------------*/
   glPushMatrix();
   /*> glTranslatef( o.avg[o.cavg - 1].xpos, o.avg[o.cavg - 1].ypos,    0.0);               <*/
   glTranslatef( o.avg[o.cavg - 1].xpos, o.avg[o.cavg - 1].ypos,    0.0);
   glBegin(GL_POLYGON);
   glColor4f(0.0f, 0.0f, 0.0f, 1.0f);       /* nice medium grey            */
   for (d = 0; d <= 360; d += 10) {
      rad = d * DEG2RAD;
      x   = r1 * cos(rad);
      y   = r1 * sin(rad);
      glVertex3f( x, y, z);
   }
   glEnd();
   /*---(draw tangent line)--------------*/
   if (o.cavg > 2 && o.bas[o.cavg - 1].fake != 'y' && o.bas[o.cavg - 2].fake != 'y') {
      int xa  = (int) (cos(o.avg[o.cavg - 1].r) * 20);
      int ya  = (int) (sin(o.avg[o.cavg - 1].r) * 20);
      glBegin(GL_LINES);
      glVertex3f( 0.0, 0.0, z);
      glVertex3f( -xa, -ya, z);
      glVertex3f( 0.0, 0.0, z);
      glVertex3f(  xa,  ya, z);
      glEnd();
      glPointSize(5.0);
      glColor4f(0.0f, 0.0f, 0.0f, 1.0f);
      glBegin(GL_POINTS);
      glVertex3f( -xa, -ya, z);
      glEnd();
      glColor4f(1.0f, 1.0f, 0.0f, 1.0f);
      glBegin(GL_POINTS);
      glVertex3f(  xa,  ya, z);
      glEnd();
      glPointSize(0.8);
   }
   glPopMatrix();
   /*---(complete)-----------------------*/
   return 0;
}



/*====================------------------------------------====================*/
/*===----                            points                            ----===*/
/*====================------------------------------------====================*/
static void      o___POINTS__________________o (void) {;}

char          /*----: draw the raw points ------------------------------------*/
DRAW_raws          (void)
{
   /*---(locals)-------------------------*/
   float   z     =    5.0;
   int i;
   glLineWidth(2.0);
   glPointSize(2.0);
   glColor4f(0.0f, 0.0f, 0.7f, 1.0f);
   /*> glBegin(GL_LINE_STRIP);                                                        <*/
   glBegin(GL_POINTS);
   for (i = 0; i < o.nraw; ++i) {
      if (o.raw[i].xpos == 999 && o.raw[i].ypos == 999) {
         glEnd();
         glBegin(GL_POINTS);
         continue;
      }
      glVertex3f( o.raw[i].xpos, o.raw[i].ypos, z);
   }
   glEnd();
   glLineWidth(0.8);
   return 0;
}

char          /*----: draw the bas/avg points --------------------------------*/
DRAW_base          (void)
{
   /*---(locals)-------------------------*/
   float   z     =    5.25;
   int i;
   glPointSize (2.0);
   glColor4f   (0.7f, 0.7f, 0.0f, 1.0f);
   glBegin     (GL_POINTS);
   for (i = 0; i < o.navg; ++i) {
      if      (o.avg[i].ca    == 'x') glColor4f(1.0f, 0.0f, 0.0f, 1.0f);
      else if (o.avg[i].t     == 'm') glColor4f(1.0f, 0.0f, 1.0f, 1.0f);
      else if (o.bas[i].fake  == 'y') glColor4f(0.0f, 0.0f, 0.0f, 1.0f);
      else                            glColor4f(0.7f, 0.7f, 0.0f, 1.0f);
      glVertex3f( o.avg[i].xpos, o.avg[i].ypos, z);
   }
   glEnd();
   glLineWidth(0.8);
   return 0;
}

char          /*----: draw the key points and lines --------------------------*/
DRAW_keys          (void)
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
      glPushMatrix();
      glTranslatef( o.key[i].xpos, o.key[i].ypos,    0.0);
      glColor4f(1.0f, 1.0f, 1.0f, 0.5f);
      if (o.key[i].ca    == 'x') glColor4f(1.0f, 0.0f, 0.0f, 0.5f);
      if (o.key[i].fake  == 'y') glColor4f(0.0f, 1.0f, 1.0f, 0.5f);
      glBegin(GL_POLYGON);
      for (d = 0; d <= 360; d += 10) {
         rad = d * DEG2RAD;
         x   = r1 * cos(rad);
         y   = r1 * sin(rad);
         glVertex3f( x, y, z);
      }
      glEnd();
      glPopMatrix();
      /*---(draw the segment)------------*/
      if (o.key[i].t != '>') {
         glColor4f(1.0f, 1.0f, 1.0f, 1.0f);
         glLineWidth(2.0);
         glBegin(GL_LINES);
         glVertex3f(o.key[i - 1].xpos, o.key[i - 1].ypos, z + 0.25);
         glVertex3f(o.key[i].xpos,     o.key[i].ypos,     z + 0.25);
         glEnd();
      }
      /*---(draw the hidden lines)-------*/
      if (i > 0 && o.key[i].t == '>') {
         glColor4f(0.5f, 0.5f, 0.5f, 1.0f);
         glLineWidth(2.0);
         glBegin(GL_LINES);
         glVertex3f(o.key[i - 1].xpos, o.key[i - 1].ypos, z - 1.00);
         glVertex3f(o.key[i].xpos,     o.key[i].ypos,     z - 1.00);
         glEnd();
      }
      /*---(draw the S extension)--------*/
      if (o.key[i].t == '>') {
         pt = o.key[i].p_bas - 1;
         glEnable(GL_LINE_STIPPLE);
         glLineStipple(1, 0x3333);
         glColor4f(0.2f, 0.2f, 0.2f, 1.0f);
         glLineWidth(2.0);
         glBegin(GL_LINES);
         glVertex3f(o.bas[pt].xpos    , o.bas[pt].ypos  , z - 1.50);
         glVertex3f(o.key[i].xpos     , o.key[i].ypos   , z - 1.50);
         glEnd();
         glDisable(GL_LINE_STIPPLE);
      }
      /*---(draw the F extension)--------*/
      if (o.key[i + 1].t == '>' || i + 1 == o.nkey) {
         pt = o.key[i].p_bas + 1;
         glEnable(GL_LINE_STIPPLE);
         glLineStipple(1, 0x3333);
         glColor4f(0.2f, 0.2f, 0.2f, 1.0f);
         glLineWidth(2.0);
         glBegin(GL_LINES);
         glVertex3f(o.bas[pt].xpos    , o.bas[pt].ypos  , z - 1.50);
         glVertex3f(o.key[i].xpos     , o.key[i].ypos   , z - 1.50);
         glEnd();
         glDisable(GL_LINE_STIPPLE);
      }
   }
   /*---(complete)-----------------------*/
   return 0;
}



/*====================------------------------------------====================*/
/*===----                          information                         ----===*/
/*====================------------------------------------====================*/
static void      o___INFO____________________o (void) {;}

char
DRAW_info_counts     (void)
{
   /*---(locals)-----------+-----+-----+-*/
   char        t           [100];
   /*---(point counts)-------------------*/
   glColor4f     (0.0f, 0.0f, 0.0f, 1.0f);
   glPushMatrix  (); {
      glTranslatef  (win.d_xoff, win.d_yoff, win.d_zoff);
      snprintf      (t, 100, "%4d", o.nraw);
      FONT__label   ("raw#", t);
      snprintf      (t, 100, "%4d", o.navg);
      FONT__label   ("avg#", t);
      snprintf      (t, 100, "%4d", o.nkey);
      FONT__label   ("key#", t);
   } glPopMatrix ();
   /*---(complete)-----------------------*/
   return 0;
}

char
DRAW_info_base       (void)
{
   /*---(locals)-----------+-----+-----+-*/
   char        t           [100];
   float       s           =  0.0;
   int         b           =    0;
   /*---(average point)------------------*/
   glColor4f     (0.0f, 0.0f, 0.0f, 1.0f);
   glPushMatrix  (); {
      /*---(basics)----------------------*/
      glTranslatef  (win.d_xoff, win.d_yoff - 50, win.d_zoff);
      snprintf      (t, 100, "%4d", o.avg[o.cavg - 1].p_bas);
      FONT__label   ("num", t);
      snprintf      (t, 100, "%4d", o.avg[o.cavg - 1].p_raw);
      FONT__label   ("raw", t);
      snprintf      (t, 100, "%4d", o.avg[o.cavg - 1].xpos);
      FONT__label   ("x"  , t);
      snprintf      (t, 100, "%4d", o.avg[o.cavg - 1].ypos);
      FONT__label   ("y"  , t);
      snprintf      (t, 100, "%4d", o.avg[o.cavg - 1].xd);
      FONT__label   ("xd" , t);
      snprintf      (t, 100, "%4d", o.avg[o.cavg - 1].yd);
      FONT__label   ("yd" , t);
      snprintf      (t, 100, "%4d", o.avg[o.cavg - 1].l);
      FONT__label   ("len" , t);
      /*---(more complex)----------------*/
      s = o.avg[o.cavg - 1].s;
      b = o.avg[o.cavg - 1].b;
      if (s >  999) { s = 999.99; b = 999; }
      if (s < -999) { s = -999.99; b = -999; }
      snprintf      (t, 100, "%7.2f", s);
      FONT__label   ("s" , t);
      snprintf      (t, 100, "%4d",   b);
      FONT__label   ("b"   , t);
      snprintf      (t, 100, "%7.2f", o.avg[o.cavg - 1].r);
      FONT__label   ("r" , t);
      snprintf      (t, 100, "%4d",   o.avg[o.cavg - 1].d);
      FONT__label   ("d"   , t);
      snprintf      (t, 100, "%4d",   o.avg[o.cavg - 1].q);
      FONT__label   ("q"   , t);
      snprintf      (t, 100, "   %c", o.avg[o.cavg - 1].t);
      FONT__label   ("t"   , t);
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
   DEBUG_G  printf("   - draw_info()\n");
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
   snprintf(msg, 100, "%4d", o.avg[o.cavg - 1].p_bas);
   FONT__label ("num", msg);
   snprintf(msg, 100, "%4d", o.avg[o.cavg - 1].p_raw);
   FONT__label ("raw", msg);
   snprintf(msg, 100, "%4d", o.avg[o.cavg - 1].xpos);
   FONT__label ("x"  , msg);
   snprintf(msg, 100, "%4d", o.avg[o.cavg - 1].ypos);
   FONT__label ("y"  , msg);
   snprintf(msg, 100, "%4d", o.avg[o.cavg - 1].xd);
   FONT__label ("xd" , msg);
   snprintf(msg, 100, "%4d", o.avg[o.cavg - 1].yd);
   FONT__label ("yd" , msg);
   snprintf(msg, 100, "%4d", o.avg[o.cavg - 1].l);
   FONT__label ("len" , msg);
   /*---(more complex)----------------------*/
   float s = o.avg[o.cavg - 1].s;
   int   b = o.avg[o.cavg - 1].b;
   if (s > 999) {
      s = 999.99;
      b = 999;
   }
   if (s < -999) {
      s = -999.99;
      b = -999;
   }
   snprintf(msg, 100, "%7.2f", s);
   FONT__label ("s" , msg);
   snprintf(msg, 100, "%4d",   b);
   FONT__label ("b"   , msg);
   snprintf(msg, 100, "%7.2f", o.avg[o.cavg - 1].r);
   FONT__label ("r" , msg);
   snprintf(msg, 100, "%4d",   o.avg[o.cavg - 1].d);
   FONT__label ("d"   , msg);
   snprintf(msg, 100, "%4d",   o.avg[o.cavg - 1].q);
   FONT__label ("q"   , msg);
   snprintf(msg, 100, "   %c", o.avg[o.cavg - 1].t);
   FONT__label ("t"   , msg);
   /*---(calculated)------------------------*/
   int  xkey = KEY_find(o.cavg - 1);
   if (xkey >= 0) {
      glTranslatef(   0, -win.d_bar * 0.5, 0.0);
      snprintf(msg, 100, "%4d",   xkey);
      FONT__label ("key"  , msg);
      snprintf(msg, 100, "%4d",   o.key[xkey].xd);
      FONT__label ("xd" , msg);
      snprintf(msg, 100, "%4d",   o.key[xkey].yd);
      FONT__label ("yd" , msg);
      snprintf(msg, 100, "%4d",   o.key[xkey].l);
      FONT__label ("len" , msg);
      /*---(more complex)----------------------*/
      s = o.key[xkey].s;
      b = o.key[xkey].b;
      if (s > 999) {
         s = 999.99;
         b = 999;
      }
      if (s < -999) {
         s = -999.99;
         b = -999;
      }
      snprintf(msg, 100, "%7.2f", s);
      FONT__label ("s" , msg);
      snprintf(msg, 100, "%4d",   b);
      FONT__label ("b"   , msg);
      snprintf(msg, 100, "%7.2f", o.key[xkey].r);
      FONT__label ("r" , msg);
      snprintf(msg, 100, "%4d",   o.key[xkey].d);
      FONT__label ("d"   , msg);
      snprintf(msg, 100, "%4d",   o.key[xkey].q);
      FONT__label ("q"   , msg);
      snprintf(msg, 100, "   %c", o.key[xkey].t);
      FONT__label ("t"   , msg);
      /*---(advanced)--------------------------*/
      snprintf(msg, 100, "%4d"  , o.key[xkey].ra);
      FONT__label ("ra"  , msg);
      snprintf(msg, 100, "%7.2f", o.key[xkey].c);
      FONT__label ("c" , msg);
      snprintf(msg, 100, "  %2d", o.key[xkey].cc);
      FONT__label ("cc"  , msg);
      snprintf(msg, 100, "   %c", o.key[xkey].ca);
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
   DEBUG_G  printf("   - draw_info()          : complete\n");
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
   return 0;
}

char
FONT__label          (char *a_label, char *a_content)
{
   glPushMatrix(); {
      yFONT_print(win.font_sm,  win.d_point, YF_BOTLEF, a_label);
      glTranslatef(   30,  0,  0);
      yFONT_print(win.font_sm,  win.d_point, YF_BOTLEF, ":");
      glTranslatef(   10,  0,  0);
      yFONT_print(win.font_sm,  win.d_point, YF_BOTLEF, a_content);
   } glPopMatrix();
   glTranslatef(   0,          - (win.d_bar * 0.6),  0);
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
sample_show        (void)
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
      for (i = 1; i < MAX_LETTERS && strncmp(loc[i].n, "eof", 5) != 0; ++i) {
         glPushMatrix();
         glTranslatef(loc[i].tx, loc[i].ty,  0.0);
         yFONT_print(win.font_sm,  5, YF_BOTRIG, loc[i].n);
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
 *>       for (i = 0; i < MAX_LETTERS && strncmp(loc[i].n, "eof", 5) != 0; ++i) {               <* 
 *>          glPushMatrix();                                                                    <* 
 *>          glTranslatef(loc[i].tx, loc[i].ty,  0.0);                                          <* 
 *>          yFONT_print(win.font_sm,  5, YF_BOTRIG, loc[i].n);                                 <* 
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
