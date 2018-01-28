/*============================---(source-start)---============================*/

#include "gregg.h"



float radius_button;

const float  ICON_BACK[4] = { 0.7f, 0.7f, 0.7f, 1.0f };
const float  ICON_FORE[4] = { 0.0f, 0.0f, 0.0f, 1.0f };
const float  ICON_BOLD[4] = { 0.7f, 0.0f, 0.0f, 1.0f };


/*> PRIV char  dlist_arrow      (void);                                               <*/
PRIV char  dlist_letters    (void);
/*> PRIV char  dlist_arrowed    (void);                                               <*/
PRIV char  DLIST_back       (void);

PRIV char  draw_line        (int, char);

char
dlist_init         (void)
{
   DEBUG_G  printf("dlist_init() -- drive loading\n");
   DEBUG_G  printf("   - clear letters\n\n");
   DEBUG_G  printf("   - create buttons\n\n");
   radius_button = (win.d_bar / 2.5);
   /*> dlist_arrow();                                                                 <*/
   dlist_letters();
   /*> dlist_arrowed();                                                               <*/
   DLIST_back();
   DEBUG_G  printf("   - done\n\n");
   return 0;
}

int  s_count = 0;

char
DLIST__title         (char *a_name)
{
   RPTG_LETTER {
      s_count = 0;
      if (a_name [0] != ' ')  printf ("\n");
      printf ("%-8.8s  ltr--  r s f  sz-x sz-y rota begd arcd    end-x end-y len deg    lef rig    grp\n", a_name);
   }
   return 0;
}

char
DLIST__show          (int a_who, char a_dotted)
{
   if (a_dotted != 'n')   return 0;
   RPTG_LETTER {
      if (s_count >= 5)  DLIST__title ("    ---");
      printf ("    %03d   %-5s  %d %d %c  %4d %4d %4d %4d %4d    %5.1f %5.1f %3d %3d    %3d %3d    %-3s\n",
            a_who,
            g_loc [a_who].n,
            g_loc [a_who].ra,
            g_loc [a_who].sz,
            g_loc [a_who].fu,
            g_loc [a_who].sx,
            g_loc [a_who].sy,
            g_loc [a_who].ro,
            g_loc [a_who].be,
            g_loc [a_who].ar,
            g_loc [a_who].ex,
            g_loc [a_who].ey,
            g_loc [a_who].ln,
            g_loc [a_who].de,
            g_loc [a_who].le,
            g_loc [a_who].ri,
            g_loc [a_who].gr);
      ++s_count;
   }
   return 0;
}


/*> PRIV char                                                                         <* 
 *> dlist_arrow(void)                                                                 <* 
 *> {                                                                                 <* 
 *>    DEBUG_X  printf("   - dlist_arrow()  . . . . . . . . . . ");                   <* 
 *>    /+---(begin)-----------------------------+/                                    <* 
 *>    dl_arrow = glGenLists(1);                                                      <* 
 *>    glNewList(dl_arrow, GL_COMPILE);                                               <* 
 *>    /+---(locals)----------------------------+/                                    <* 
 *>    float   r1 =  radius_button;                                                   <* 
 *>    float   z  =  40.00;                                                           <* 
 *>    /+---(interior)--------------------------+/                                    <* 
 *>    glBegin(GL_POLYGON);                                                           <* 
 *>    glVertex3f( -r1, -r1, z);                                                      <* 
 *>    glVertex3f( 0.0,  r1, z);                                                      <* 
 *>    glVertex3f(  r1, -r1, z);                                                      <* 
 *>    glVertex3f( -r1, -r1, z);                                                      <* 
 *>    glEnd();                                                                       <* 
 *>    /+---(end)-------------------------------+/                                    <* 
 *>    glEndList();                                                                   <* 
 *>    DEBUG_X  printf("success\n");                                                  <* 
 *>    /+---(complete)--------------------------+/                                    <* 
 *>    return 0;                                                                      <* 
 *> }                                                                                 <*/

char
dlist_letters(void)
{
   /*---(locals)-----------+-----+-----+-*/
   int         i           = 0;
   DEBUG_X  printf("   - displist_k1() . . . . . . . . . . . . ");
   /*---(begin)-----------------------------*/
   dl_dotted = glGenLists(MAX_LETTERS);
   dl_solid  = glGenLists(MAX_LETTERS);
   RPTG_LETTER  printf("LETTER DEFINITION ========beg===\n");
   /*---(draw lines)-----------------------*/
   DLIST__title ("lines");
   for (i = 0; i < MAX_LETTERS; ++i) {
      if (strncmp(g_loc[i].n, "eof", 5) == 0) break;
      if (g_loc[i].fu != 'l')                 continue;
      glNewList (dl_solid  + i,  GL_COMPILE);   draw_line   (i, 'n');   glEndList();
      glNewList (dl_dotted + i,  GL_COMPILE);   draw_line   (i, 'y');   glEndList();
   }
   /*---(draw ellipses)--------------------*/
   DLIST__title ("ellipses");
   for (i = 0; i < MAX_LETTERS; ++i) {
      if (strncmp(g_loc[i].n, "eof", 5) == 0) break;
      if (g_loc[i].fu != 'e')                 continue;
      glNewList (dl_solid  + i,  GL_COMPILE);   draw_ellipse (i, 'n');   glEndList();
      glNewList (dl_dotted + i,  GL_COMPILE);   draw_ellipse (i, 'y');   glEndList();
   }
   /*---(draw circles)---------------------*/
   DLIST__title ("circles");
   for (i = 0; i < MAX_LETTERS; ++i) {
      if (strncmp(g_loc[i].n, "eof", 5) == 0) break;
      if (g_loc[i].fu != 'c')                 continue;
      glNewList(dl_solid  + i,  GL_COMPILE);   draw_ellipse(i, 'n');   glEndList();
      glNewList(dl_dotted + i,  GL_COMPILE);   draw_ellipse(i, 'y');   glEndList();
   }
   /*---(draw pie wedges)------------------*/
   DLIST__title ("wedges");
   for (i = 0; i < MAX_LETTERS; ++i) {
      if (strncmp(g_loc[i].n, "eof", 5) == 0) break;
      if (g_loc[i].fu != 'p')                 continue;
      glNewList(dl_solid  + i,  GL_COMPILE);   draw_pie(i);   glEndList();
      glNewList(dl_dotted + i,  GL_COMPILE);   draw_pie(i);   glEndList();
   }
   /*---(draw dots)------------------------*/
   DLIST__title ("dots");
   for (i = 0; i < MAX_LETTERS; ++i) {
      if (strncmp(g_loc[i].n, "eof", 5) == 0) break;
      if (g_loc[i].fu != 'd')                 continue;
      glNewList(dl_solid  + i,  GL_COMPILE);   draw_dot(i);   glEndList();
      glNewList(dl_dotted + i,  GL_COMPILE);   draw_dot(i);   glEndList();
   }
   /*---(draw whitespace)------------------*/
   DLIST__title ("spaces");
   for (i = 0; i < MAX_LETTERS; ++i) {
      if (strncmp(g_loc[i].n, "eof", 5) == 0) break;
      if (g_loc[i].fu != 'w')                 continue;
      glNewList(dl_solid  + i,  GL_COMPILE);   draw_whitespace(i);   glEndList();
      glNewList(dl_dotted + i,  GL_COMPILE);   draw_whitespace(i);   glEndList();
   }
   /*---(draw accents)---------------------*/
   DLIST__title ("accents");
   for (i = 0; i < MAX_LETTERS; ++i) {
      if (strncmp(g_loc[i].n, "eof", 5) == 0) break;
      if (g_loc[i].fu != 'a')                 continue;
      glNewList(dl_solid  + i,  GL_COMPILE);   draw_whitespace(i);   glEndList();
      glNewList(dl_dotted + i,  GL_COMPILE);   draw_whitespace(i);   glEndList();
   }
   /*---(complete)--------------------------*/
   RPTG_LETTER  printf("==========================end===\n");
   DEBUG_X  printf("success\n");
   return 0;
}

PRIV char     /*----: create a line for a letter -----------------------------*/
draw_line (int a_who, char a_dotted)
{
   /*---(tables)--------------------------------*/
   int a_ro   = g_loc[a_who].ro;
   int a_ar   = g_loc[a_who].ar;
   /*---(locals)--------------------------------*/
   float z     =  5.0;
   float rad;
   float x, y;
   rad    = a_ro * DEG2RAD;
   int   right = 0;
   int   left  = 0;
   /*> printf("specs at %3d, %3d\n", a_ro, a_ar);                                     <*/
   /*---(draw shape)----------------------------*/
   x = a_ar * sin(rad);
   y = a_ar * cos(rad);
   if (x > right) right = x;
   if (x < left)  left  = x;
   glLineWidth(2.0);
   glBegin(GL_LINE_STRIP); {
      glVertex3f( 0, 0, z);
      glVertex3f( x, y, z);
   } glEnd();
   glLineWidth(0.8);
   /*---(get overall calcs)---------------------*/
   o.tmp[0].p_bas   = 0;
   o.tmp[0].xpos    = 0;
   o.tmp[0].ypos    = 0;
   o.tmp[1].p_bas   = 1;
   o.tmp[1].xpos    = x;
   o.tmp[1].ypos    = y;
   POINT_calc (o.tmp + 1, 'n');
   /*---(set size values)-----------------------*/
   g_loc[a_who].ex  = x;
   g_loc[a_who].ey  = y;
   g_loc[a_who].ln  = o.tmp[1].l;
   g_loc[a_who].de  = o.tmp[1].d;
   g_loc[a_who].ri  = right;
   g_loc[a_who].le  = left;
   /*---(report out)----------------------------*/
   DLIST__show  (a_who, a_dotted);
   /*---(complete)------------------------------*/
   return 0;
}

char
draw_ellipse (int a_who, char a_dotted)
{
   /*---(locals)-----------+-----+-----+-*/
   int         i           = 0;
   /*---(tables)--------------------------------*/
   int a_sx   = g_loc[a_who].sx;
   int a_sy   = g_loc[a_who].sy;
   int a_ro   = g_loc[a_who].ro;
   int a_be   = g_loc[a_who].be;
   int a_ar   = g_loc[a_who].ar;
   int a_st   = g_loc[a_who].st;
   if (a_st <= 0) a_st = 10;
   /*---(locals)--------------------------------*/
   float beta  = a_ro * DEG2RAD;
   float sbeta = sin(beta);
   float cbeta = cos(beta);
   float z     =  5.0;
   float alpha;
   float salpha, calpha;
   float x, y;
   int   step = 2;
   int   right = 0;
   int   left  = 0;
   /*---(determine adjustment)------------------*/
   alpha  = a_be * DEG2RAD;
   salpha = sin(alpha);
   calpha = cos(alpha);
   float ax = (a_sx * calpha * cbeta) - (a_sy * salpha * sbeta);
   float ay = (a_sx * calpha * sbeta) + (a_sy * salpha * cbeta);
   /*---(draw dotted outline)-------------------*/
   glLineWidth (2.0);
   glPointSize (2.0);
   yVIKEYS_view_color (YCOLOR_BAS_MIN, 1.00);
   /*> DRAW_set_color (COLOR_BLACK);                                                  <*/
   /*> glColor4f   (0.0f, 0.0f, 0.0f, 1.0f);                                          <*/
   if (a_dotted == 'y') {
      glBegin(GL_POINTS);
      for (i = 0; i <= 360; i += a_st) {
         alpha  = i * DEG2RAD;
         salpha = sin (alpha);
         calpha = cos (alpha);
         x      = (a_sx * calpha * cbeta) - (a_sy * salpha * sbeta) - ax;
         y      = (a_sx * calpha * sbeta) + (a_sy * salpha * cbeta) - ay;
         /*> printf("    %3d) %3.0f, %3.0f\n", i, x, y);                              <*/
         glVertex3f( x, y, z);
      }
      glEnd();
   }
   /*---(draw shape)----------------------------*/
   glBegin(GL_LINE_STRIP);
   if (a_ar >= 0) {
      for (i = a_be; i <= a_be + a_ar; i += step) {
         alpha  = i * DEG2RAD;
         salpha = sin(alpha);
         calpha = cos(alpha);
         x      = (a_sx * calpha * cbeta) - (a_sy * salpha * sbeta) - ax;
         y      = (a_sx * calpha * sbeta) + (a_sy * salpha * cbeta) - ay;
         /*> printf("    %3d) %3.0f, %3.0f\n", i, x, y);                              <*/
         glVertex3f( x, y, z);
         if (x > right) right = x;
         if (x < left)  left  = x;
      }
   } else {
      for (i = a_be; i >= a_be + a_ar; i -= step) {
         alpha  = i * DEG2RAD;
         salpha = sin(alpha);
         calpha = cos(alpha);
         x      = (a_sx * calpha * cbeta) - (a_sy * salpha * sbeta) - ax;
         y      = (a_sx * calpha * sbeta) + (a_sy * salpha * cbeta) - ay;
         /*> printf("    %3d) %3.0f, %3.0f\n", i, x, y);                              <*/
         glVertex3f( x, y, z);
         if (x > right) right = x;
         if (x < left)  left  = x;
      }
   }
   glEnd();
   /*---(get overall calcs)---------------------*/
   o.tmp[0].p_bas   = 0;
   o.tmp[0].xpos   = 0;
   o.tmp[0].ypos   = 0;
   o.tmp[1].p_bas   = 1;
   o.tmp[1].xpos   = x;
   o.tmp[1].ypos   = y;
   POINT_calc (o.tmp + 1, 'n');
   /*---(set size values)-----------------------*/
   g_loc[a_who].ex = x;
   g_loc[a_who].ey = y;
   g_loc[a_who].ln = o.tmp[1].l;
   g_loc[a_who].de = o.tmp[1].d;
   g_loc[a_who].ri = right;
   g_loc[a_who].le = left;
   /*---(check for i, ai, ae)-------------------*/
   if (  strncmp(g_loc[a_who].gr, "i" , 5) == 0 ||
         strncmp(g_loc[a_who].gr, "ai", 5) == 0 ||
         strncmp(g_loc[a_who].gr, "ae", 5) == 0) {
      alpha  = (a_be + 0.5 * a_ar) * DEG2RAD;
      salpha = sin(alpha);
      calpha = cos(alpha);
      a_sx  *= 0.0;
      a_sy  *= 0.0;
      x      = (a_sx * calpha * cbeta) - (a_sy * salpha * sbeta) - ax;
      y      = (a_sx * calpha * sbeta) + (a_sy * salpha * cbeta) - ay;
      if        (strncmp(g_loc[a_who].gr, "i" , 5) == 0)  {
         glBegin(GL_LINE_STRIP);
         glVertex3f( x - 12, y -  4 , z);
         glVertex3f( x     , y     , z);
         glEnd();
      } else if (strncmp(g_loc[a_who].gr, "ae", 5) == 0) {
         glBegin(GL_LINE_STRIP);
         glVertex3f( x -  4, y + 12 , z);
         glVertex3f( x     , y     , z);
         glEnd();
      } else if (strncmp(g_loc[a_who].gr, "ai", 5) == 0) {
         float xx2, yy2;
         glBegin(GL_LINE_STRIP);
         for (i = 0; i <= 360; i += 30) {
            alpha  = i * DEG2RAD;
            xx2 = 2.0 * sin(alpha);
            yy2 = 2.0 * cos(alpha);
            glVertex3f( x + xx2, y + yy2, z);
         }
         glEnd();
      }
   }
   /*---(complete)------------------------------*/
   glLineWidth(0.8);
   glPointSize(0.8);
   /*---(report out)----------------------------*/
   DLIST__show  (a_who, a_dotted);
   /*---(complete)------------------------------*/
   return 0;
}

char
draw_arc (float a_beg, float a_end, int a_color, int a_z)
{
   /*---(locals)-----------+-----+-----+-*/
   float x, y;
   float     r = SIZE_LRG_AVG * 2.5;
   float       j           = 0.0;
   if (a_color == 2) r = SIZE_LRG_MAX;
   /*---(defense)------------------------*/
   if (a_beg == a_end) return -1;
   /*---(color)--------------------------*/
   if      (a_color ==  1) yVIKEYS_view_color (YCOLOR_NEG_MED, 0.65);
   else if (a_color ==  2) yVIKEYS_view_color (YCOLOR_NEG_DRK, 0.65);
   else if (a_color == -1) yVIKEYS_view_color (YCOLOR_BAS_MUT, 0.65);
   else                    yVIKEYS_view_color (YCOLOR_NEG_LIG, 0.65);
   /*---(fill)---------------------------*/
   glBegin(GL_POLYGON); {
      glVertex3f(   0,   0,   a_z);
      x = r * cos(a_beg);
      y = r * sin(a_beg);
      glVertex3f(   x,   y,   a_z);
      for (j = a_beg; j < a_end;  j += 0.1) {
         x = r * cos(j);
         y = r * sin(j);
         glVertex3f(   x,   y,   a_z);
      }
      x = r * cos(a_end);
      y = r * sin(a_end);
      glVertex3f(   x,   y,   a_z);
   } glEnd();
   /*---(tick marks)-----------------------*/
   /*> DRAW_set_color (COLOR_GRID_L);                                                 <*/
   /*> glLineWidth(1.0);                                                              <* 
    *> glBegin(GL_LINES); {                                                           <* 
    *>    for (j = a_beg; j <= a_end;  j += 0.025) {                                  <* 
    *>       x = (r - 10) * cos(j);                                                   <* 
    *>       y = (r - 10) * sin(j);                                                   <* 
    *>       glVertex3f(   x,   y,   a_z + 1);                                        <* 
    *>       x = r * cos(j);                                                          <* 
    *>       y = r * sin(j);                                                          <* 
    *>       glVertex3f(   x,   y,   a_z + 1);                                        <* 
    *>    }                                                                           <* 
    *> } glEnd();                                                                     <*/
   /*---(outer boundary)-------------------*/
   /*> glLineWidth(0.8);                                                              <* 
    *> glBegin(GL_LINE_STRIP); {                                                      <* 
    *>    for (j = a_beg; j <= a_end;  j += 0.025) {                                  <* 
    *>       x = r * cos(j);                                                          <* 
    *>       y = r * sin(j);                                                          <* 
    *>       glVertex3f(   x,   y,   a_z + 1);                                        <* 
    *>    }                                                                           <* 
    *> } glEnd();                                                                     <*/
   /*---(complete)-------------------------*/
   return 0;
}


char
draw_pie (int a_who)
{
   /*---(locals)-----------+-----+-----+-*/
   int         i           = 0;
   /*---(tables)--------------------------------*/
   int a_be   = g_loc[a_who].be;      /* rotation */
   int a_sx   = g_loc[a_who].sx * 1.7;  /* radius   */
   /*> DEBUG__SHAPES  if (g_loc[a_who].ri == 0 && g_loc[a_who].le == 0) printf("   %-5s", g_loc[a_who].n);   <*/
   /*---(tables)--------------------------------*/
   float rad;
   float z     =  5.0;
   float x, y;
   float mult;
   int   right = 0;
   int   left  = 0;
   glLineWidth(2.0);
   glBegin(GL_LINE_STRIP);
   glVertex3f( 0, 0, z);
   for (i = a_be; i <= a_be + 65; i += 5) {
      rad  = i * DEG2RAD;
      if      (i == a_be      || i == a_be + 65) mult = a_sx * 0.70;
      else if (i == a_be +  5 || i == a_be + 60) mult = a_sx * 0.95;
      else if (i == a_be + 10 || i == a_be + 55) mult = a_sx * 1.10;
      else if (i == a_be + 15 || i == a_be + 50) mult = a_sx * 1.20;
      else if (i == a_be + 20 || i == a_be + 45) mult = a_sx * 1.25;
      else if (i == a_be + 25 || i == a_be + 40) mult = a_sx * 1.30;
      else if (i == a_be + 30 || i == a_be + 35) mult = a_sx * 1.33;
      x = mult * cos(rad);
      y = mult * sin(rad);
      glVertex3f( x, y, z);
      if (x > right) right = x;
      if (x < left)  left  = x;
   }
   glVertex3f( 0, 0, z);
   glEnd();
   /*> DEBUG__SHAPES  if (g_loc[a_who].ri == 0 && g_loc[a_who].le == 0) printf(" %3.0f %3.0f %3d %3d %3d %3d  %-3s\n", x, y, o.tmp[1].l, o.tmp[1].d, left, right, g_loc[a_who].gr);   <*/
   /*---(set size values)-----------------------*/
   g_loc[a_who].ex = 0;
   g_loc[a_who].ey = 0;
   g_loc[a_who].ln = 0;
   g_loc[a_who].de = 0;
   g_loc[a_who].ri = right;
   g_loc[a_who].le = left;
   /*> g_loc[a_who].ex = 0;                                                             <* 
    *> g_loc[a_who].ey = 0;                                                             <* 
    *> g_loc[a_who].ln = 0;                                                             <* 
    *> g_loc[a_who].de = 0;                                                             <* 
    *> g_loc[a_who].ri = 0;                                                             <* 
    *> g_loc[a_who].le = 0;                                                             <*/
   /*---(check for i, ai, ae)-------------------*/
   if (  strncmp(g_loc[a_who].gr, "i" , 5) == 0 ||
         strncmp(g_loc[a_who].gr, "ai", 5) == 0 ||
         strncmp(g_loc[a_who].gr, "ae", 5) == 0) {
      rad  = (a_be + 22.5) * DEG2RAD;
      mult = a_sx * 0.80;
      x = mult * cos(rad);
      y = mult * sin(rad);
      if        (strncmp(g_loc[a_who].gr, "i" , 5) == 0)  {
         glBegin(GL_LINE_STRIP);
         glVertex3f( x - 12, y -  4 , z);
         glVertex3f( x     , y     , z);
         glEnd();
      } else if (strncmp(g_loc[a_who].gr, "ae", 5) == 0) {
         glBegin(GL_LINE_STRIP);
         glVertex3f( x -  4, y + 12 , z);
         glVertex3f( x     , y     , z);
         glEnd();
      } else {
         float xx2, yy2;
         glBegin(GL_LINE_STRIP);
         for (i = 0; i <= 360; i += 30) {
            rad  = i * DEG2RAD;
            xx2 = 2.0 * sin(rad);
            yy2 = 2.0 * cos(rad);
            glVertex3f( x + xx2, y + yy2, z);
         }
         glEnd();
      }
   }
   /*---(report out)----------------------------*/
   DLIST__show  (a_who, 'n');
   /*---(complete)------------------------------*/
   glLineWidth(0.8);
   return 0;
}

char
draw_dot (int a_who)
{
   /*---(locals)-----------+-----+-----+-*/
   int         i           = 0;
   /*---(tables)--------------------------------*/
   int a_size  = g_loc[a_who].sx;
   int a_steps = g_loc[a_who].st;
   int a_offx  = g_loc[a_who].ro;
   int a_offy  = g_loc[a_who].be;
   if (a_steps <= 0) a_steps = 10;
   float rad;
   float z     =  5.0;
   float x, y;
   int   right = 0;
   int   left  = 0;
   /*> DEBUG__SHAPES  if (g_loc[a_who].ri == 0) printf("   %-5s", g_loc[a_who].n);    <*/
   glLineWidth(2.0);
   glBegin(GL_LINE_STRIP);
   for (i = 0; i <= 360; i += a_steps) {
      rad  = i * DEG2RAD;
      x = a_size * sin(rad);
      y = a_size * cos(rad);
      glVertex3f( x, y, z);
      if (x > right) right = x;
      if (x < left)  left  = x;
   }
   glEnd();
   glLineWidth(0.8);
   /*> DEBUG__SHAPES  if (g_loc[a_who].ri == 0) printf(" %3.0f %3.0f %3d %3d %3d %3d  %-3s\n", x, y, o.tmp[1].l, o.tmp[1].d, left, right, g_loc[a_who].gr);   <*/
   /*---(set size values)-----------------------*/
   g_loc[a_who].ex = 0;
   g_loc[a_who].ey = 0;
   g_loc[a_who].ln = 0;
   g_loc[a_who].de = 0;
   g_loc[a_who].ri = right;
   g_loc[a_who].le = left;
   /*---(report out)----------------------------*/
   DLIST__show  (a_who, 'n');
   /*---(complete)------------------------------*/
   return 0;
}

char
draw_whitespace (int a_who)
{
   int a_offx  = g_loc[a_who].ro;
   int a_offy  = g_loc[a_who].be;
   int left    = 0;
   int right   = 0;
   /*> DEBUG__SHAPES  if (g_loc[a_who].ex == 0) printf("   %-5s", g_loc[a_who].n);    <*/
   if (a_offx > 0) right = a_offx;
   if (a_offx < 0) left  = a_offx;
   /*> DEBUG__SHAPES  if (g_loc[a_who].ex != 0) printf(" %3d %3d %3d %3d %3d %3d  %-3s\n", a_offx, a_offy, 0, 0, left, right, g_loc[a_who].gr);   <*/
   g_loc[a_who].ex = a_offx;
   g_loc[a_who].ey = a_offy;
   g_loc[a_who].ln = 0;
   g_loc[a_who].de = 0;
   g_loc[a_who].ri = right;
   g_loc[a_who].le = left;
   /*---(report out)----------------------------*/
   DLIST__show  (a_who, 'n');
   /*---(complete)------------------------------*/
   return 0;
}

/*> char                                                                              <* 
 *> dlist_arrowed      (void)                                                         <* 
 *> {                                                                                 <* 
 *>    DEBUG_X  printf("   - displist_arrow()  . . . . . . . . . . ");                <* 
 *>    /+---(begin)-----------------------------+/                                    <* 
 *>    dl_arrowed = glGenLists(1);                                                    <* 
 *>    glNewList(dl_arrowed, GL_COMPILE);                                             <* 
 *>    float    r1 = 15;                                                              <* 
 *>    float    r2 = 10;                                                              <* 
 *>    /+---(exterior)--------------------------+/                                    <* 
 *>    glColor4f(0.2f, 0.2f, 0.2f, 1.0f);                                             <* 
 *>    glBegin(GL_LINE_STRIP);                                                        <* 
 *>    glVertex3f(-r1,  0, r1);                                                       <* 
 *>    glVertex3f(  0, r2, r1);                                                       <* 
 *>    glVertex3f( r1,  0, r1);                                                       <* 
 *>    glVertex3f( r2,  0, r1);                                                       <* 
 *>    glVertex3f( r2,-r1, r1);                                                       <* 
 *>    glVertex3f(-r2,-r1, r1);                                                       <* 
 *>    glVertex3f(-r2,  0, r1);                                                       <* 
 *>    glVertex3f(-r1,  0, r1);                                                       <* 
 *>    glEnd();                                                                       <* 
 *>    /+---(end)-------------------------------+/                                    <* 
 *>    glEndList();                                                                   <* 
 *>    DEBUG_X  printf("success\n");                                                  <* 
 *>    /+---(complete)--------------------------+/                                    <* 
 *>    return 0;                                                                      <* 
 *> }                                                                                 <*/



/*====================------------------------------------====================*/
/*===----                     background elements                      ----===*/
/*====================------------------------------------====================*/
static void      o___BACKGROUND______________o (void) {;}

char         /*-> degree tickmarks for background ----[ ------ [ge.C70.13#.E1]*/ /*-[02.0000.03#.T]-*/ /*-[--.---.---.--]-*/
BACK__degticks     (void)
{
   /*---(locals)-------------------------*/
   float d, rad;
   float r;
   float x, y, z;
   /*---(prepare)------------------------*/
   yVIKEYS_view_color (YCOLOR_BAS_MAX, 0.65);
   /*> glColor4f(1.0f, 1.0f, 1.0f, 0.5f);                                             <*/
   glLineWidth(2.0);
   z   = 2;
   r   = SIZE_LRG_AVG * 1.75;
   /*---(mark degrees)-------------------*/
   glBegin(GL_LINES); {
      for (d = 0; d <= 360; d += 15) {
         rad = d * DEG2RAD;
         x   = (r + 5) * cos(rad);
         y   = (r + 5) * sin(rad);
         glVertex3f( x, y, z);
         if ((int) d % 45 == 0) {
            x   = (r + 25) * cos(rad);
            y   = (r + 25) * sin(rad);
         } else {
            x   = (r + 15) * cos(rad);
            y   = (r + 15) * sin(rad);
         }
         glVertex3f( x, y, z);
      }
   } glEnd();
   /*---(complete)-----------------------*/
   return 0;
}

PRIV char     /*----: create display list for range fan ----------------------*/
BACK__rangefan     (void)
{
   /*---(locals)-------------------------*/
   float     d, rad;                   /* degrees and radians                 */
   float     x, y, z;                  /* cartesian coordinates               */
   float     min, beg, end, max;       /* range limits                        */
   int       i;                        /* loop iterator -- range              */
   char      msg[100];
   float     radius = 130;
   float     r = SIZE_LRG_AVG * 2;
   /*---(mark ranges)--------------------*/
   for (i = 0; i < MAX_RANGES; ++i) {
      if (strncmp(ranges[i].nam, "eof", 5) == 0) break;
      min = (ranges[i].min + 2) * DEG2RAD;
      beg = (ranges[i].beg + 1) * DEG2RAD;
      end = (ranges[i].end - 1) * DEG2RAD;
      max = (ranges[i].max - 2) * DEG2RAD;
      glLineWidth(1.0);
      /*---(arc)---------------------------*/
      if (ranges[i].len == 1) {
         r  = SIZE_LRG_AVG * 1.75;
         z  =   -2.00;
      } else {
         r  = SIZE_LRG_AVG * 1.3;
         z  =   -1.00;
      }
      glLineWidth(0.8);
      if (strncmp(ranges[i].nam, "", 5) == 0 || strncmp(ranges[i].nam, "---", 5) == 0) {
         if (ranges[i].len == 1) draw_arc (min, max, -1, z);
         else                    draw_arc (min, max,  2, z);
      } else {
         draw_arc (min, beg,  1, z);
         draw_arc (beg, end,  0, z);
         draw_arc (end, max,  1, z);
      }
      /*---(label)--------------------------*/
      z  =    2.00;
      if (strncmp(ranges[i].nam, "---", 5) != 0) {
         if      (ranges[i].len == 2)                 radius = r + 10;
         else if (strncmp(ranges[i].nam, "", 5) == 0) radius = r - 20;
         else                                         radius = r + 30;
         yVIKEYS_view_color (YCOLOR_BAS_MOR, 0.25);
         rad = (ranges[i].tar) * DEG2RAD;
         x =  radius * cos(rad);
         y =  radius * sin(rad);
         snprintf(msg, 100, "%d", ranges[i].num);
         glPushMatrix(); {
            glTranslatef(  x,     y,    z);
            yFONT_print (win.font_bg, 20, YF_MIDCEN, msg);
         } glPopMatrix();
      }
      /*---(target line)-------------------*/
      if (ranges[i].num <= 5) {
         yVIKEYS_view_color (YCOLOR_BAS_MOR, 0.65);
         /*> DRAW_set_color (COLOR_GRID_D);                                           <*/
         /*> glColor4f(0.0f, 0.0f, 0.0f, 0.7f);                                       <*/
         glLineWidth(2.0);
         z  =    1.00;
         glBegin(GL_LINES); {
            rad = (ranges[i].tar) * DEG2RAD;
            x = SIZE_LRG_AVG * cos(rad);
            y = SIZE_LRG_AVG * sin(rad);
            glVertex3f(   x,   y,   z);
            x = (r + 60) * cos(rad);
            y = (r + 60) * sin(rad);
            glVertex3f(   x,   y,   z);
         } glEnd();
      }
   }
   /*---(complete)-----------------------*/
   return 0;
}

char
DLIST__ring_center   (float a_size, float a_thick)
{
   /*---(locals)-----------+-----+-----+-*/
   float       d           =  0.0;          /* degrees                        */
   float       r           =  0.0;          /* radians                        */
   float       x           =  0.0;          /* x position                     */
   float       y           =  0.0;          /* y position                     */
   /*---(draw)---------------------------*/
   glLineWidth   (a_thick);
   yVIKEYS_view_color (YCOLOR_BAS_MED, 0.50);
   /*> DRAW_set_color (COLOR_BASE);                                                   <*/
   /*> glColor4f     (0.3f, 0.5f, 0.3f, 0.5f);       /+ nice medium grey            +/   <*/
   glBegin  (GL_POLYGON); {
      for (d = 0; d <= 360; d += 1) {
         r   = d * DEG2RAD;
         x   = a_size * cos (r);
         y   = a_size * sin (r);
         glVertex3f ( x, y, 3);
      }
   } glEnd();
   /*---(complete)-----------------------*/
   return 0;
}

char
DLIST__ring_avg      (float a_size, float a_thick)
{
   /*---(locals)-----------+-----+-----+-*/
   float       d           =  0.0;          /* degrees                        */
   float       r           =  0.0;          /* radians                        */
   float       x           =  0.0;          /* x position                     */
   float       y           =  0.0;          /* y position                     */
   /*---(draw)---------------------------*/
   glLineWidth   (a_thick);
   yVIKEYS_view_color (YCOLOR_BAS_MAX, 0.50);
   /*> DRAW_set_color (COLOR_GRID_L);                                                 <*/
   /*> glColor4f     (1.0f, 1.0f, 1.0f, 0.5f);                                        <*/
   glBegin  (GL_LINE_STRIP); {
      for (d = 0; d <= 360; d += 1) {
         r   = d * DEG2RAD;
         x   = a_size * cos (r);
         y   = a_size * sin (r);
         glVertex3f ( x, y, 5);
      }
   } glEnd();
   /*---(complete)-----------------------*/
   return 0;
}

char
DLIST__ring_minmax   (float a_size, float a_thick)
{
   /*---(locals)-----------+-----+-----+-*/
   float       d           =  0.0;          /* degrees                        */
   float       r           =  0.0;          /* radians                        */
   float       x           =  0.0;          /* x position                     */
   float       y           =  0.0;          /* y position                     */
   /*---(draw)---------------------------*/
   glLineWidth   (a_thick);
   glEnable      (GL_LINE_STIPPLE);
   glLineStipple (1, 0x3333);
   yVIKEYS_view_color (YCOLOR_BAS_MAX, 0.50);
   /*> DRAW_set_color (COLOR_GRID_L);                                                 <*/
   /*> glColor4f     (1.0f, 1.0f, 1.0f, 0.5f);                                        <*/
   glBegin  (GL_LINE_STRIP); {
      for (d = -140; d <=  75; d +=  1) {
         r   = d * DEG2RAD;
         x   = a_size * cos (r);
         y   = a_size * sin (r);
         glVertex3f ( x, y, 5);
      }
   } glEnd();
   glDisable(GL_LINE_STIPPLE);
   /*---(complete)-----------------------*/
   return 0;
}

PRIV char     /*----: create display list for ranging rings ------------------*/
BACK__rangerings   (void)
{
   /*---(locals)-----------+-----+-----+-*/
   int         i           =    0;          /* loop iterator -- ring          */
   /*---(center disk)--------------------*/
   DLIST__ring_center (SIZE_LRG_AVG ,  0.8);
   DLIST__ring_avg    (SIZE_CENTER ,  0.8);
   /*---(size one -- u,n,o)-----------------*/
   DLIST__ring_minmax (SIZE_SML_MIN,  0.8);
   DLIST__ring_avg    (SIZE_SML_AVG,  2.0);
   DLIST__ring_minmax (SIZE_SML_MAX,  0.8);
   /*---(size two)--------------------------*/
   DLIST__ring_avg    (SIZE_MED_AVG, 15.0);
   DLIST__ring_minmax (SIZE_MED_MAX,  0.8);
   /*---(size three)------------------------*/
   DLIST__ring_avg    (SIZE_LRG_AVG,  3.0);
   DLIST__ring_minmax (SIZE_LRG_MAX,  0.8);
   /*---(middle)----------------------------*/
   for (i = SIZE_LRG_AVG * 1.7; i < 500; i += SIZE_LRG_AVG * 1.7) {
      DLIST__ring_avg    (i,  0.8);
   }
   /*---(complete)-----------------------*/
   return 0;
}

PRIV char     /*----: create display list for guide lines --------------------*/
BACK__guidelines   (void)
{
   /*---(locals)-------------------------*/
   float     z;                        /* cartesian coordinates               */
   /*---(guides)----------------------------*/
   glLineWidth(0.8);
   z = -10.0;
   yVIKEYS_view_color (YCOLOR_BAS_MAX, 0.65);
   /*---(45's)-----*/
   glBegin(GL_LINES);
   glVertex3f(  win.m_xmax,    win.m_xmax,  z);
   glVertex3f( -win.m_xmax,   -win.m_xmax,  z);
   glVertex3f( -win.m_xmax,    win.m_xmax,  z);
   glVertex3f(  win.m_xmax,   -win.m_xmax,  z);
   glEnd();
   /*---(30's)-----*/
   glBegin(GL_LINES);
   glVertex3f(  win.m_xmax,    win.m_xmax / 3.0,  z);
   glVertex3f(         0.0,           0.0 / 3.0,  z);
   glVertex3f(         0.0,           0.0 / 3.0,  z);
   glVertex3f(  win.m_xmax,   -win.m_xmax / 3.0,  z);
   glEnd();
   /*---(align)----*/
   z =  10.0;
   glBegin(GL_LINES);
   glVertex3f(       -50.0,           0.0,  1.0);
   glVertex3f(  win.m_xmax,           0.0,  1.0);
   glEnd();
   glLineWidth(0.8);
   /*---(complete)-----------------------*/
   return 0;
}

PRIV char     /*----: create display list for entry background ---------------*/
DLIST_back         (void)
{
   /*---(begin)-----------------------------*/
   dl_back = glGenLists(1);
   glNewList(dl_back, GL_COMPILE);
   /*---(locals)----------------------------*/
   float   r1 =  SIZE_SML_AVG;
   float   r2 =  SIZE_MED_AVG;
   float   r3 =  SIZE_LRG_AVG;
   float   z  =  0.00;
   float   d;
   float   rad, min, beg, end, max;
   float   x, y;
   /*---(draw peices)----------------------*/
   glPushMatrix(); {
      BACK__degticks   ();
      BACK__rangefan   ();
      BACK__rangerings ();
      BACK__guidelines ();
   } glPopMatrix();
   /*---(end)-------------------------------*/
   glEndList();
   DEBUG_X  printf("success\n");
   /*---(complete)--------------------------*/
   return 0;
}


/*============================----(source-end)----============================*/
