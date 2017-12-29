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


/*====================------------------------------------====================*/
/*===----                           drivers                            ----===*/
/*====================------------------------------------====================*/
static void      o___DRIVERS_________________o (void) {;}

char
draw_main (void)
{
   DEBUG_G  printf("draw_main()\n");
   /*---(start)----------------------------*/
   long  x_start, x_stop;
   x_start = time_stamp();
   glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
   glLoadIdentity();
   /*---(draw)-----------------------------*/
   /*> yGOD_orient_xzy(  50.0, -100.0,  -33.0);                                       <* 
    *> yGOD_locate_xzy( -50.0, -100.0,  -33.0);                                       <*/
   draw_info();
   yGOD_view();
   glCallList(dl_back);
   /*> draw_oslider();                                                                <*/
   /*> draw_aslider();                                                                <*/
   /*> draw_kslider();                                                                <*/
   draw_raws   ();
   draw_avgs   ();
   draw_keys   ();
   draw_curr   ();
   /*> draw_bands  ();                                                                <*/
   draw_saved  ();
   /*> draw_horz   ();                                                                <*/
   words_result ();
   sample_draw ();
   /*---(word)-----------------------------*/
   glPushMatrix();
   glColor4f(1.0f, 1.0f, 0.0f, 1.0f);
   glTranslatef(  -40.0,  25.0, 10.0);
   yFONT_print(win.txf_bg,  18, YF_BOTLEF, o.word);
   glTranslatef(    0.0, -85.0,  0.0);
   yFONT_print(win.txf_bg,  18, YF_BOTLEF, o.actual);
   glPopMatrix();
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
   x_stop  = time_stamp();
   DEBUG_G  printf("   - done, drawing took %ld ms\n\n", x_stop - x_start);
   return 0;
}

char
draw_init(void)
{
   DEBUG_X  printf("draw_init()\n\n");
   /*---(clearing)-----------------------*/
   glClearColor  (0.3f, 0.5f, 0.3f, 1.0f);       /* nice medium green          */
   glClearDepth  (1.0f);
   /*---(basics)-------------------------*/
   glShadeModel  (GL_SMOOTH);
   glEnable      (GL_TEXTURE_2D);    /* NEW */
   glEnable      (GL_DEPTH_TEST);
   glDepthFunc   (GL_LEQUAL);
   /*---(blending)-----------------------*/
   glEnable      (GL_ALPHA_TEST);
   glAlphaFunc   (GL_GEQUAL, 0.0125);
   glEnable      (GL_BLEND);
   glBlendFunc   (GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
   glHint        (GL_PERSPECTIVE_CORRECTION_HINT, GL_NICEST);
   /*---(polygon antialiasing)-----------*/
   glEnable      (GL_POLYGON_SMOOTH);
   glPolygonMode (GL_FRONT_AND_BACK, GL_FILL);
   glHint        (GL_POLYGON_SMOOTH_HINT, GL_NICEST);
   /*---(wrap)---------------------------*/
   glFlush       ();
   sample_init   ();
   sample_etch   ();
   draw_resize   (win.width, win.height);
   /*---(complete)-----------------------*/
   return 0;
}

char
draw_resize(uint a_w, uint a_h)
{
   DEBUG_G  printf("draw_resize(%dw, %dh)\n", a_w, a_h);
   if (a_h == 0) a_h = 1;
   WIDTH   = a_w;
   HEIGHT  = a_h;
   glViewport(0,  0, WIDTH, HEIGHT);
   glMatrixMode(GL_PROJECTION);
   glLoadIdentity();
   gluPerspective(45.0f, (GLfloat) WIDTH / (GLfloat) HEIGHT, 0.01f, 1000.0f);
   /*> glOrtho( -win.left, win.right, -win.bottom, win.top, -150.0,  150.0);      <*/
   glMatrixMode(GL_MODELVIEW);
   return 0;
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
draw_aslider()
{
   float   x_lef = -win.left   + 5;
   float   x_rig = -win.left   + win.bar;
   float   x_top =  win.top    - 5;
   float   x_bot = -win.bottom + 5;
   float   z     =   10.0;
   /*---(first)----------------------------*/
   /*> glColor4f(0.3f, 0.6f, 0.3f, 1.0f);       /+ nice medium grey            +/     <* 
    *> glBegin(GL_POLYGON);                                                           <* 
    *> glVertex3f( x_lef, x_top, z);                                                  <* 
    *> glVertex3f( x_rig, x_top, z);                                                  <* 
    *> glVertex3f( x_rig, x_bot, z);                                                  <* 
    *> glVertex3f( x_lef, x_bot, z);                                                  <* 
    *> glEnd();                                                                       <*/
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
   glLineWidth(0.8);
   if (o.navg == 0) return 0;
   glColor4f(0.7f, 0.7f, 0.7f, 1.0f);
   float x_inc = (x_top - x_bot) / o.navg;
   /*---(current point)-------------------------*/
   glPushMatrix();
   glTranslatef( x_lef, x_top - (x_inc * o.cavg) + x_inc,    0.0);
   /*> printf("%d\n", o.cavg);                                                      <*/
   glColor4f(0.0f, 0.0f, 0.0f, 1.0f);
   glBegin(GL_POLYGON);
   glVertex3f(  0,      0, 20.0);
   glVertex3f( 15,      0, 20.0);
   glVertex3f( 15, -x_inc, 20.0);
   glVertex3f(  0, -x_inc, 20.0);
   glEnd();
   glPopMatrix();
   /*---(complete)------------------------------*/
   return 0;
}

char
draw_kslider()
{
   float       i           = 0.0;
   float   x_lef =  -win.left  + (win.bar * 3.00);
   float   x_rig =  win.right  - (win.bar * 1.20);
   float   x_top =  win.top    - (win.bar * 0.85);
   float   x_bot =  win.top    - (win.bar * 0.15);
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
   float   x_lef =  win.right  - (win.bar * 1.0);
   float   x_rig =  win.right  - (win.bar * 0.2);
   float   x_top =  win.top    - (win.bar * 0.2);
   float   x_bot = -win.bottom + (win.bar * 0.2);
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
   out_minx = o.key[0].x;
   out_maxx = o.key[0].x;
   out_miny = o.key[0].y;
   out_maxy = o.key[0].y;
   for (i = 0; i < o.nkey; ++i) {
      /*---(update outline bounds)-------*/
      if (o.key[i].x < out_minx)  out_minx = o.key[i].x;
      if (o.key[i].x > out_maxx)  out_maxx = o.key[i].x;
      if (o.key[i].y < out_miny)  out_miny = o.key[i].y;
      if (o.key[i].y > out_maxy)  out_maxy = o.key[i].y;
      if (o.key[i].u[0] == 'a' || o.key[i].u[0] == 'A' ||
            o.key[i].u[0] == 'e' || o.key[i].u[0] == 'E') {
         /*---(update circle bounds)-----*/
         minx = o.key[i].x;
         maxx = o.key[i].x;
         maxy = o.key[i].y;
         miny = o.key[i].y;
         for (j = i + 1; j < o.nkey; ++j) {
            if (o.key[j].u[0] != '+') break;
            if (o.key[j].x < minx)  minx = o.key[j].x;
            if (o.key[j].x > maxx)  maxx = o.key[j].x;
            if (o.key[j].y < miny)  miny = o.key[j].y;
            if (o.key[j].y > maxy)  maxy = o.key[j].y;
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
draw_curr          (void)
{
   /*---(locals)-------------------------*/
   float   r1 =   2.0;
   float   d;
   float   rad;
   float   x, y;
   float   z  =   5.25;
   /*---(draw current point)-------------*/
   glPushMatrix();
   /*> glTranslatef( o.avg[o.cavg - 1].x, o.avg[o.cavg - 1].y,    0.0);               <*/
   glTranslatef( o.avg[o.cavg - 1].x, o.avg[o.cavg - 1].y,    0.0);
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
   if (o.cavg > 2 && o.bas[o.cavg - 1].a != 'y' && o.bas[o.cavg - 2].a != 'y') {
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
draw_raws          (void)
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
      if (o.raw[i].x == 999 && o.raw[i].y == 999) {
         glEnd();
         glBegin(GL_POINTS);
         continue;
      }
      glVertex3f( o.raw[i].x, o.raw[i].y, z);
   }
   glEnd();
   glLineWidth(0.8);
   return 0;
}

char          /*----: draw the bas/avg points --------------------------------*/
draw_avgs          (void)
{
   /*---(locals)-------------------------*/
   float   z     =    5.25;
   int i;
   glPointSize(5.0);
   glColor4f(0.7f, 0.7f, 0.0f, 1.0f);
   glBegin(GL_POINTS);
   for (i = 0; i < o.navg; ++i) {
      if      (o.avg[i].ca == 'x') glColor4f(1.0f, 0.0f, 0.0f, 1.0f);
      else if (o.avg[i].t  == 'm') glColor4f(1.0f, 0.0f, 1.0f, 1.0f);
      else if (o.bas[i].a  == 'y') glColor4f(0.0f, 0.0f, 0.0f, 1.0f);
      else                         glColor4f(0.7f, 0.7f, 0.0f, 1.0f);
      glVertex3f( o.avg[i].x, o.avg[i].y, z);
   }
   glEnd();
   glLineWidth(0.8);
   return 0;
}

char          /*----: draw the key points and lines --------------------------*/
draw_keys          (void)
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
      glTranslatef( o.key[i].x, o.key[i].y,    0.0);
      glColor4f(1.0f, 1.0f, 1.0f, 0.5f);
      if (o.key[i].ca == 'x') glColor4f(1.0f, 0.0f, 0.0f, 0.5f);
      if (o.key[i].a  == 'y') glColor4f(0.0f, 1.0f, 1.0f, 0.5f);
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
         glVertex3f(o.key[i - 1].x, o.key[i - 1].y, z + 0.25);
         glVertex3f(o.key[i].x,     o.key[i].y,     z + 0.25);
         glEnd();
      }
      /*---(draw the hidden lines)-------*/
      if (i > 0 && o.key[i].t == '>') {
         glColor4f(0.5f, 0.5f, 0.5f, 1.0f);
         glLineWidth(2.0);
         glBegin(GL_LINES);
         glVertex3f(o.key[i - 1].x, o.key[i - 1].y, z - 1.00);
         glVertex3f(o.key[i].x,     o.key[i].y,     z - 1.00);
         glEnd();
      }
      /*---(draw the S extension)--------*/
      if (o.key[i].t == '>') {
         pt = o.key[i].p - 1;
         glEnable(GL_LINE_STIPPLE);
         glLineStipple(1, 0x3333);
         glColor4f(0.2f, 0.2f, 0.2f, 1.0f);
         glLineWidth(2.0);
         glBegin(GL_LINES);
         glVertex3f(o.bas[pt].x    , o.bas[pt].y  , z - 1.50);
         glVertex3f(o.key[i].x     , o.key[i].y   , z - 1.50);
         glEnd();
         glDisable(GL_LINE_STIPPLE);
      }
      /*---(draw the F extension)--------*/
      if (o.key[i + 1].t == '>' || i + 1 == o.nkey) {
         pt = o.key[i].p + 1;
         glEnable(GL_LINE_STIPPLE);
         glLineStipple(1, 0x3333);
         glColor4f(0.2f, 0.2f, 0.2f, 1.0f);
         glLineWidth(2.0);
         glBegin(GL_LINES);
         glVertex3f(o.bas[pt].x    , o.bas[pt].y  , z - 1.50);
         glVertex3f(o.key[i].x     , o.key[i].y   , z - 1.50);
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
draw_info (void)
{
   DEBUG_G  printf("   - draw_info()\n");
   char msg[100];
   glColor4f(1.0f, 1.0f, 0.0f, 1.0f);
   if (o.saved == 'y') {
      glPushMatrix();
      glTranslatef( -210.0, -150.0, win.offz);
      snprintf(msg, 100, "%4d", o.curr);
      yFONT_print(win.txf_bg,  18, YF_BOTLEF, msg);
      glPopMatrix();
   }
   /*---(point counts)----------------------*/
   glColor4f(0.0f, 0.0f, 0.0f, 1.0f);
   glPushMatrix();
   glTranslatef( win.offx, win.offy, win.offz);
   snprintf(msg, 100, "%4d", o.total);
   font_label ("out#", msg);
   snprintf(msg, 100, "%4d", o.curr);
   font_label ("outc", msg);
   snprintf(msg, 100, "%4d", o.nraw);
   font_label ("raw#", msg);
   snprintf(msg, 100, "%4d", o.navg);
   font_label ("avg#", msg);
   snprintf(msg, 100, "%4d", o.cavg);
   font_label ("avgc", msg);
   snprintf(msg, 100, "%4d", o.nkey);
   font_label ("key#", msg);
   /*---(average point)---------------------*/
   glTranslatef(   0, -win.bar * 0.5, 0.0);
   snprintf(msg, 100, "%4d", o.avg[o.cavg - 1].p);
   font_label ("num", msg);
   snprintf(msg, 100, "%4d", o.avg[o.cavg - 1].o);
   font_label ("raw", msg);
   snprintf(msg, 100, "%4d", o.avg[o.cavg - 1].x);
   font_label ("x"  , msg);
   snprintf(msg, 100, "%4d", o.avg[o.cavg - 1].y);
   font_label ("y"  , msg);
   snprintf(msg, 100, "%4d", o.avg[o.cavg - 1].xd);
   font_label ("xd" , msg);
   snprintf(msg, 100, "%4d", o.avg[o.cavg - 1].yd);
   font_label ("yd" , msg);
   snprintf(msg, 100, "%4d", o.avg[o.cavg - 1].l);
   font_label ("len" , msg);
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
   font_label ("s" , msg);
   snprintf(msg, 100, "%4d",   b);
   font_label ("b"   , msg);
   snprintf(msg, 100, "%7.2f", o.avg[o.cavg - 1].r);
   font_label ("r" , msg);
   snprintf(msg, 100, "%4d",   o.avg[o.cavg - 1].d);
   font_label ("d"   , msg);
   snprintf(msg, 100, "%4d",   o.avg[o.cavg - 1].q);
   font_label ("q"   , msg);
   snprintf(msg, 100, "   %c", o.avg[o.cavg - 1].t);
   font_label ("t"   , msg);
   /*---(calculated)------------------------*/
   int  xkey = key_find(o.cavg - 1);
   if (xkey >= 0) {
      glTranslatef(   0, -win.bar * 0.5, 0.0);
      snprintf(msg, 100, "%4d",   xkey);
      font_label ("key"  , msg);
      snprintf(msg, 100, "%4d",   o.key[xkey].xd);
      font_label ("xd" , msg);
      snprintf(msg, 100, "%4d",   o.key[xkey].yd);
      font_label ("yd" , msg);
      snprintf(msg, 100, "%4d",   o.key[xkey].l);
      font_label ("len" , msg);
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
      font_label ("s" , msg);
      snprintf(msg, 100, "%4d",   b);
      font_label ("b"   , msg);
      snprintf(msg, 100, "%7.2f", o.key[xkey].r);
      font_label ("r" , msg);
      snprintf(msg, 100, "%4d",   o.key[xkey].d);
      font_label ("d"   , msg);
      snprintf(msg, 100, "%4d",   o.key[xkey].q);
      font_label ("q"   , msg);
      snprintf(msg, 100, "   %c", o.key[xkey].t);
      font_label ("t"   , msg);
      /*---(advanced)--------------------------*/
      snprintf(msg, 100, "%4d"  , o.key[xkey].ra);
      font_label ("ra"  , msg);
      snprintf(msg, 100, "%7.2f", o.key[xkey].c);
      font_label ("c" , msg);
      snprintf(msg, 100, "  %2d", o.key[xkey].cc);
      font_label ("cc"  , msg);
      snprintf(msg, 100, "   %c", o.key[xkey].ca);
      font_label ("ca"  , msg);
      snprintf(msg, 100, "   %c", o.key[xkey].a);
      font_label ("a"   , msg);
      snprintf(msg, 100, "%4.4s", o.key[xkey].u);
      font_label ("u"   , msg);
   }
   glPopMatrix();
   glPushMatrix();
   glTranslatef(  win.ansx, win.ansy, win.offz);
   snprintf(msg, 100, "%4d of %4d",   o.curr, o.total);
   font_label ("curr"  , msg);
   snprintf(msg, 100, "%s",   o.when);
   font_label ("when"  , msg);
   snprintf(msg, 100, "%s",   o.note);
   font_label ("note"  , msg);
   snprintf(msg, 100, "%s",   o.expect);
   font_label ("expe"  , msg);
   snprintf(msg, 100, "%s",   o.actual);
   font_label ("actu"   , msg);
   snprintf(msg, 100, "%s",   o.grade);
   font_label ("grad"   , msg);
   snprintf(msg, 100, "%s",   o.word);
   font_label ("word"   , msg);
   snprintf(msg, 100, "%s",   o.letters);
   font_label ("ltrs"   , msg);
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
font_load(void)
{
   win.txf_bg = yFONT_load(win.face_bg);
   if (win.txf_bg < 0) {
      fprintf(stderr, "Problem loading %s\n", win.face_bg);
      exit(1);
   }
   win.txf_sm = yFONT_load(win.face_sm);
   if (win.txf_sm < 0) {
      fprintf(stderr, "Problem loading %s\n", win.face_sm);
      exit(1);
   }
   return 0;
}

char
font_label (char *a_label, char *a_content)
{
   glPushMatrix(); {
      yFONT_print(win.txf_sm,  win.psize, YF_BOTLEF, a_label);
      glTranslatef(   30,  0,  0);
      yFONT_print(win.txf_sm,  win.psize, YF_BOTLEF, ":");
      glTranslatef(   10,  0,  0);
      yFONT_print(win.txf_sm,  win.psize, YF_BOTLEF, a_content);
   } glPopMatrix();
   glTranslatef(   0,          - (win.bar * 0.6),  0);
   return 0;
}

char
font_free (void)
{
   yFONT_free(win.txf_bg);
   yFONT_free(win.txf_sm);
   return 0;
}



/*====================------------------------------------====================*/
/*===----                          polymorphism                        ----===*/
/*====================------------------------------------====================*/
static void      o___POLYMORPH_______________o (void) {;}

char
format_input       (void)
{
   win.format   = 'i';
   strcpy(win.title, "gregg_input");
   win.height   =  300;
   win.top      =  125;
   win.bottom   =  175;
   win.width    =  400;
   win.left     =  125;
   win.right    =  275;
   /*-------- label   min     max   start    step   minor   major -----*/
   yGOD_axis ( 'v',      0,    360,      0,      1,      5,      5);
   yGOD_axis ( 'o',      0,    360,      0,      1,      5,      5);
   yGOD_axis ( 's',      0,    360,      0,      1,      5,      5);
   yGOD_axis ( 'c',    -50,     50,     10,      1,      5,      5);
   yGOD_axis ( 'b',    -50,     50,     -3,      1,      5,      5);
   yGOD_axis ( 'd',   -150,    150,     37,      1,      5,      5);
   win.offx      =   300;  /* 110 will show */
   win.offy      =   140;
   win.offz      =  -370;
   win.psize     =     8;
   win.bar       =    20;
   win.ansx      =    40;
   win.ansy      =   -65;
   return 0;
}

char
format_wider       (void)
{
   win.format   = 'w';
   strcpy(win.title, "gregg_larger");
   win.height   =  350;
   win.top      =  125;
   win.bottom   =  225;
   win.width    =  570;
   win.left     =  125;
   win.right    =  375;
   yGOD_axis ( 'v',      0,    360,      0,      1,      5,      5);
   yGOD_axis ( 'o',      0,    360,      0,      1,      5,      5);
   yGOD_axis ( 's',      0,    360,      0,      1,      5,      5);
   yGOD_axis ( 'c',    -50,     50,     10,      1,      5,      5);
   yGOD_axis ( 'b',    -50,     50,     -3,      1,      5,      5);
   yGOD_axis ( 'd',   -150,    150,     37,      1,      5,      5);
   /*> yGOD_axis( 'v',      0,    360,      0,      1);                               <* 
    *> yGOD_axis( 'o',      0,    360,      0,      1);                               <* 
    *> yGOD_axis( 's',      0,    360,      0,      1);                               <* 
    *> yGOD_axis( 'c',    -50,     50,     15,      1);                               <* 
    *> yGOD_axis( 'b',    -50,     50,     -6,      1);                               <* 
    *> yGOD_axis( 'd',   -150,    150,     45,      1);                               <*/
   win.offx      =   300;
   win.offy      =   140;
   win.offz      =  -450;
   win.psize     =     8;
   win.bar       =    20;
   win.ansx      =    30;
   win.ansy      =   -50;
   return 0;
}

char
format_detail      (void)
{
   win.format   = 'x';
   strcpy(win.title, "gregg_detail");
   win.height   =  750;
   win.top      =  125;
   win.bottom   =  525;
   win.width    = 1200;
   win.left     =  125;
   win.right    =  975;
   yGOD_axis ( 'v',      0,    360,      0,      1,      5,      5);
   yGOD_axis ( 'o',      0,    360,      0,      1,      5,      5);
   yGOD_axis ( 's',      0,    360,      0,      1,      5,      5);
   yGOD_axis ( 'c',    -50,     50,     10,      1,      5,      5);
   yGOD_axis ( 'b',    -50,     50,     -3,      1,      5,      5);
   yGOD_axis ( 'd',   -150,    150,     37,      1,      5,      5);
   /*> yGOD_axis( 'v',      0,    360,      0,      1);                               <* 
    *> yGOD_axis( 'o',      0,    360,      0,      1);                               <* 
    *> yGOD_axis( 's',      0,    360,      0,      1);                               <* 
    *> yGOD_axis( 'c',    -50,     50,     15,      1);                               <* 
    *> yGOD_axis( 'b',    -50,     50,     -6,      1);                               <* 
    *> yGOD_axis( 'd',   -150,    150,     45,      1);                               <*/
   win.offx      =   200;
   win.offy      =   160;
   win.offz      =  -450;
   win.psize     =     6;
   win.bar       =    14;
   win.ansx      =     0;
   win.ansy      =   -50;
   return 0;
}

char       /*----: main format driver ----------------------------------------*/
format_change      (char a_which)
{
   /*---(change format)------------------*/
   switch (a_which) {
   case 'i' : format_input();        break;
   case 'w' : format_wider();        break;
   case 'x' : format_detail();       break;
   default  :                        break;
   }
   /*---(clear out)----------------------*/
   sample_free      ();
   font_free        ();
   yXINIT__gdestroy ();
   XUnmapWindow     (DISP, BASE);
   XDestroyWindow   (DISP, BASE);
   /*---(setup window)--------------------------*/
   XSetWindowAttributes   attr;
   XColor         xc1, xc2;
   attr.colormap         = DefaultColormap(DISP, SCRN);
   attr.background_pixel = XAllocNamedColor(DISP, CMAP, "black"      ,  &xc1, &xc2);
   BASE = XCreateWindow(DISP, ROOT,
         0, 0, win.width, win.height, 0,
         CopyFromParent, InputOutput, CopyFromParent,
         CWBackPixel|CWColormap, &attr);
   XStoreName       (DISP, BASE, win.title);
   XSelectInput     (DISP, BASE, KeyPressMask|KeyReleaseMask|ButtonPressMask|ButtonMotionMask|ButtonReleaseMask|ExposureMask|StructureNotifyMask|FocusChangeMask);
   XMapWindow       (DISP, BASE);
   XFlush           (DISP);
   yXINIT__gsetup   ();
   /*---(populate the window)-------------------*/
   font_load        ();
   dlist_init       ();
   draw_init        ();
   draw_main        ();
   /*---(complete)------------------------------*/
   return 0;
}



/*====================------------------------------------====================*/
/*===----                        writing samples                       ----===*/
/*====================------------------------------------====================*/
static void      o___SAMPLES_________________o (void) {;}

char               /*----: create a texture for writing samples --------------*/
sample_init        (void)
{
   /*---(generate)-----------------------*/
   glGenFramebuffersEXT         (1, &sample_fbo);
   glGenTextures                (1, &sample_tex);
   glGenRenderbuffersEXT        (1, &sample_dep);
   /*---(bind)---------------------------*/
   glBindFramebufferEXT         (GL_FRAMEBUFFER_EXT,  sample_fbo);
   glBindTexture                (GL_TEXTURE_2D,       sample_tex);
   /*---(settings)-----------------------*/
   glTexParameteri              (GL_TEXTURE_2D,  GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
   glTexParameteri              (GL_TEXTURE_2D,  GL_TEXTURE_MAG_FILTER, GL_LINEAR);
   glTexParameteri              (GL_TEXTURE_2D,  GL_TEXTURE_WRAP_S, GL_REPEAT);
   glTexParameteri              (GL_TEXTURE_2D,  GL_TEXTURE_WRAP_T, GL_REPEAT);
   glTexEnvi                    (GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_REPLACE);
   glTexParameteri              (GL_TEXTURE_2D,  GL_GENERATE_MIPMAP, GL_TRUE);
   /*---(copy)---------------------------*/
   glTexImage2D                 (GL_TEXTURE_2D, 0, GL_RGBA, sample_w, sample_h, 0, GL_RGBA, GL_UNSIGNED_BYTE, 0);
   glFramebufferTexture2DEXT    (GL_FRAMEBUFFER_EXT, GL_COLOR_ATTACHMENT0_EXT, GL_TEXTURE_2D, sample_tex, 0);
   /*---(depth)--------------------------*/
   glBindRenderbufferEXT        (GL_RENDERBUFFER_EXT, sample_dep);
   glRenderbufferStorageEXT     (GL_RENDERBUFFER_EXT, GL_DEPTH_COMPONENT24, sample_w, sample_h);
   glFramebufferRenderbufferEXT (GL_FRAMEBUFFER_EXT, GL_DEPTH_ATTACHMENT_EXT, GL_RENDERBUFFER_EXT, sample_dep);
   /*---(unbind)-------------------------*/
   glBindFramebufferEXT         (GL_FRAMEBUFFER_EXT, 0);
   /*---(complete)-----------------------*/
   return 0;
}

char       /*----: draw the writing samples to the texture -------------------*/
sample_etch        (void)
{
   int         i           = 0;
   /*---(setup)--------------------------*/
   glViewport            (0.0,  0.0, sample_w, sample_h);
   glMatrixMode          (GL_PROJECTION);
   glLoadIdentity        ();
   glOrtho               (0.0,   sample_w, -sample_h,  0.0, -500.0,  500.0);
   glMatrixMode          (GL_MODELVIEW);
   glBindTexture         (GL_TEXTURE_2D, 0);
   glBindFramebufferEXT  (GL_FRAMEBUFFER_EXT,  sample_fbo);
   glClearColor          (0.3f, 0.5f, 0.3f, 1.0f);       /* nice medium green          */
   glClear               (GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
   glColor4f             (0.0f, 0.0f, 0.0f, 1.0f);
   /*---(title)--------------------------*/
   glPushMatrix(); {
      glScalef(2.0, 2.0, 2.0);
      glTranslatef          (    0.0,   -5.0,    0.0);
      yFONT_print           (win.txf_sm,  5, YF_TOPLEF, "writing samples");
   } glPopMatrix();
   /*---(cycle samples)------------------*/
   glPushMatrix(); {
      glScalef(2.0, 2.0, 2.0);
      for (i = 0; i < MAX_LETTERS && strncmp(loc[i].n, "eof", 5) != 0; ++i) {
         glPushMatrix();
         glTranslatef(loc[i].tx, loc[i].ty,  0.0);
         yFONT_print(win.txf_sm,  5, YF_BOTRIG, loc[i].n);
         glCallList(dl_dotted + i);
         glPopMatrix();
      }
   } glPopMatrix();
   /*---(mipmaps)------------------------*/
   glBindFramebufferEXT  (GL_FRAMEBUFFER_EXT, 0);
   glBindTexture         (GL_TEXTURE_2D, sample_tex);
   glGenerateMipmapEXT   (GL_TEXTURE_2D);
   glBindTexture         (GL_TEXTURE_2D, 0);
   /*---(complete)-------------------------*/
   return 0;
}

char       /*----: display the writing sample to the window ------------------*/
sample_draw        (void)
{
   glPushMatrix(); {
      glTranslatef(160, 115, -15.0);
      glBindTexture   (GL_TEXTURE_2D, sample_tex);
      glBegin(GL_POLYGON); {
         glTexCoord2f ( 0.0      ,  1.0);
         glVertex3f   ( 0.0      ,  0.0       , 0.0);
         glTexCoord2f ( 1.0      ,  1.0);
         glVertex3f   ( sample_w / 2.0 ,  0.0       , 0.0);
         glTexCoord2f ( 1.0      ,  0.0);
         glVertex3f   ( sample_w / 2.0 ,  -sample_h / 2.0 , 0.0);
         glTexCoord2f ( 0.0      ,  0.0);
         glVertex3f   ( 0.0      ,  -sample_h / 2.0 , 0.0);
      } glEnd();
      glBindTexture   (GL_TEXTURE_2D, 0);
   } glPopMatrix();
   return 0;
}


char               /* PURPOSE : delete a framebuffer -------------------------*/
sample_free        (void)
{
   /*---(generate)-----------------------*/
   glDeleteTextures                (1, &sample_tex);
   glDeleteRenderbuffersEXT        (1, &sample_dep);
   glDeleteFramebuffersEXT         (1, &sample_fbo);
   /*---(complete)-----------------------*/
   return 0;
}


/*============================----(source-end)----============================*/
