/*=================================(beg-code)=================================*/
#include "gregg.h"



PRIV int     ntries    = 0;            /* number of raw points attempted      */
PRIV char    stype     = '-';          /* p=prefix, o=outline, c=continue     */
PRIV int     s_xadj    = 0;
PRIV int     s_yadj    = 0;


/*
 * 
 *
 *
 *
 */



/*============================--------------------============================*/
/*===----                          raw points                          ----===*/
/*============================--------------------============================*/
static void o___RAW_POINTS_____________o (void) {;}

char     /*----: add a new raw point to outline -------------------------*/
RAW_point          (int a_x, int a_y, char a_type)
{
   /*---(begin)--------------------------*/
   ++ntries;
   DEBUG_I     printf("   %4d : %4dx, %4dy,    %c\n", o.nraw, a_x, a_y, a_type);
   DEBUG__RAW  printf("   %4d : %4d : %4dx, %4dy : ", ntries, o.nraw, a_x, a_y);
   /*---(defenses)-----------------------*/
   if (o.nraw >= MAX_POINTS) {
      DEBUG__RAW  printf("REJECTED, too many points (%d >= %d\n", o.nraw, MAX_POINTS);
      return -2;
   }
   if (o.nraw > 0 && a_type == POINT_NORMAL && a_x == o.raw[o.nraw - 1].xpos && a_y == o.raw[o.nraw - 1].ypos) {
      DEBUG__RAW  printf("REJECTED, same as last point\n");
      return -3;
   }
   /*---(save point)---------------------*/
   o.raw[o.nraw].p_raw   = o.nraw;
   o.raw[o.nraw].xpos    = a_x;
   o.raw[o.nraw].ypos    = a_y;
   o.raw[o.nraw].t       = a_type;
   /*---(save point)---------------------*/
   switch (a_type) {
   case POINT_START   :
      o.raw[o.nraw].fake = 'y';
      break;
   case POINT_FINISH  :
      o.raw[o.nraw].fake = 'y';
      POINT_calc (o.raw + o.nraw, 'n');
      break;
   case POINT_HEAD    :
      o.raw[o.nraw].fake = '-';
      POINT_calc (o.raw + o.nraw, 'n');
      break;
   case POINT_NORMAL  :
      o.raw[o.nraw].fake = '-';
      POINT_calc (o.raw + o.nraw, 'n');
      break;
   }
   /*---(update counters)----------------*/
   ++o.nraw;
   /*---(complete)-----------------------*/
   return 0;
}

char
RAW_touch            (int a_x, int a_y)
{
   /*---(locals)-----------+-----+-----+-*/
   int         x           =  0.0;
   int         y           =  0.0;
   int         r           =  0.0;
   /*---(calculate)----------------------*/
   x  = a_x - win.m_left;
   y  = a_y - win.m_ymax;
   r  = sqrt((x * x) + (y * y));
   /*---(assign type)--------------------*/
   if (r < SIZE_R1) {
      if (stype != 'p')  OUT_clear ();
      stype   = PART_MAIN;
   } else if (x < 0.0 && y < 0.0) {
      OUT_clear ();
      stype   = PART_PREFIX;
      s_xadj  = a_x;
      s_yadj  = a_y;
   } else {
      stype   = PART_CONTINUE;
   }
   /*---(save points)--------------------*/
   RAW_point (a_x, a_y, POINT_START );
   RAW_point (a_x, a_y, POINT_HEAD);
   /*---(complete)-----------------------*/
   return 0;
}

char
RAW_normal           (int a_x, int a_y)
{
   RAW_point (a_x, a_y, POINT_NORMAL);
   return 0;
}

char
RAW_lift             (int a_x, int a_y)
{
   RAW_point (a_x, a_y, POINT_NORMAL);
   RAW_point (a_x, a_y, POINT_FINISH);
   RAW_equalize ();
   RPTG_RAW  POINT_list (o.raw, o.nraw);
   return 0;
}

/*> char          /+----: add a new raw point to outline -------------------------+/   <* 
 *> RAW_add            (int a_x, int a_y)                                              <* 
 *> {                                                                                  <* 
 *>    RAW_point (a_x, a_y, '-');                                                      <* 
 *>    return 0;                                                                       <* 
 *> }                                                                                  <*/





/*> char          /+----: start a new outline with a prefix ----------------------+/   <* 
 *> RAW_prefix         (int a_x, int a_y)                                              <* 
 *> {                                                                                  <* 
 *>    /+---(initialize)---------------------+/                                        <* 
 *>    DEBUG__RAW  printf("RAW POINTS (prefix)\n");                                    <* 
 *>    OUT_clear ();                                                                   <* 
 *>    stype   = 'p';                                                                  <* 
 *>    ntries  =  0;                                                                   <* 
 *>    /+---(save points)--------------------+/                                        <* 
 *>    RAW_point (a_x, a_y, 'S');                                                      <* 
 *>    o.raw[o.nraw - 1].fake = 'y';                                                   <* 
 *>    RAW_point (a_x, a_y, '-');                                                      <* 
 *>    o.raw[o.nraw - 1].t = '>';                                                      <* 
 *>    /+---(save adjustment)----------------+/                                        <* 
 *>    s_xadj = 0;                                                                     <* 
 *>    s_yadj = 0;                                                                     <* 
 *>    /+---(complete)-----------------------+/                                        <* 
 *>    return 0;                                                                       <* 
 *> }                                                                                  <*/

/*> char          /+----: start a new outline ------------------------------------+/   <* 
 *> RAW_begin          (int a_x, int a_y)                                              <* 
 *> {                                                                                  <* 
 *>    /+---(check for prefix)---------------+/                                        <* 
 *>    DEBUG__RAW  printf("RAW POINTS (begin)\n");                                     <* 
 *>    if (stype != 'p') {                                                             <* 
 *>       OUT_clear ();                                                                <* 
 *>       ntries  =  0;                                                                <* 
 *>    }                                                                               <* 
 *>    /+---(save points)--------------------+/                                        <* 
 *>    stype   = 'o';                                                                  <* 
 *>    RAW_point (a_x, a_y, 'S');                                                      <* 
 *>    o.raw[o.nraw - 1].fake = 'y';                                                   <* 
 *>    RAW_point (a_x, a_y, '-');                                                      <* 
 *>    o.raw[o.nraw - 1].t = '>';                                                      <* 
 *>    /+---(save adjustment)----------------+/                                        <* 
 *>    s_xadj = a_x;                                                                   <* 
 *>    s_yadj = a_y;                                                                   <* 
 *>    /+---(complete)-----------------------+/                                        <* 
 *>    return 0;                                                                       <* 
 *> }                                                                                  <*/


/*> char          /+----: continue an outline with another stroke ----------------+/   <* 
 *> RAW_continue       (int a_x, int a_y)                                              <* 
 *> {                                                                                  <* 
 *>    /+---(check for prefix)---------------+/                                        <* 
 *>    DEBUG__RAW  printf("RAW POINTS (continue)\n");                                  <* 
 *>    /+---(save points)--------------------+/                                        <* 
 *>    stype   = 'c';                                                                  <* 
 *>    RAW_point (a_x, a_y, 'S');                                                      <* 
 *>    o.raw[o.nraw - 1].fake = 'y';                                                   <* 
 *>    RAW_point (a_x, a_y, '-');                                                      <* 
 *>    o.raw[o.nraw - 1].t = '>';                                                      <* 
 *>    /+---(save adjustment)----------------+/                                        <* 
 *>    s_xadj = 0;                                                                     <* 
 *>    s_yadj = 0;                                                                     <* 
 *>    /+---(complete)-----------------------+/                                        <* 
 *>    return 0;                                                                       <* 
 *> }                                                                                  <*/

char          /*----: make raw xy relative to neutral origin -----------------*/
RAW_equalize       (void)
{
   /*---(locals)-----------+-----+-----+-*/
   int         i           =    0;
   DEBUG__RAW  printf("   adjust all by %dx, %dy\n", s_xadj, s_yadj);
   for (i = 0; i < o.nraw; ++i) {
      o.raw[i].xpos   -= s_xadj;
      o.raw[i].ypos   -= s_yadj;
   }
   return 0;
}

char          /*----: start a new outline ------------------------------------*/
RAW_read           (int a_x, int a_y)
{
   /*---(check for prefix)---------------*/
   DEBUG__RAW  printf("RAW POINTS (begin)\n");
   /*---(save points)--------------------*/
   OUT_clear ();
   ntries  =  0;
   RAW_point (a_x, a_y, 'S');
   o.raw[o.nraw - 1].fake = 'y';
   RAW_point (a_x, a_y, '-');
   o.raw[o.nraw - 1].t = '>';
   /*---(save adjustment)----------------*/
   s_xadj = 0;
   s_yadj = 0;
   /*---(complete)-----------------------*/
   return 0;
}



/*=================================(end-code)=================================*/
