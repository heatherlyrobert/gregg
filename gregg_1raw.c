/*=================================(beg-code)=================================*/
#include "gregg.h"



PRIV int     ntries    = 0;            /* number of raw points attempted      */
PRIV char    stype     = '-';          /* p=prefix, o=outline, c=continue     */
PRIV int     xadj      = 0;
PRIV int     yadj      = 0;


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
   if (o.nraw > 0 && a_type == o.raw[o.nraw - 1].t && a_x == o.raw[o.nraw - 1].x && a_y == o.raw[o.nraw - 1].y) {
      DEBUG__RAW  printf("REJECTED, same as last point\n");
      return -3;
   }
   /*---(save point)---------------------*/
   o.raw[o.nraw].o   = o.nraw;
   o.raw[o.nraw].x   = a_x;
   o.raw[o.nraw].y   = a_y;
   o.raw[o.nraw].t   = a_type;
   if (o.raw[o.nraw].t != 'S') gen_calc(o.raw + o.nraw, 'n');
   /*---(garbage catching)---------------*/
   if (o.nraw > 0 && o.raw[o.nraw].t != 'F' && o.raw[o.nraw].l > 5) {
      DEBUG__RAW  printf("NEW STROKE, point too far away (%d > 1)\n", o.raw[o.nraw].l);
      RAW_point (o.raw[o.nraw - 1].x, o.raw[o.nraw - 1].y, 'F');
      o.raw[o.nraw - 1].a = 'y';
      RAW_point (a_x, a_y, 'S');
      o.raw[o.nraw - 1].a = 'y';
      RAW_point (a_x, a_y, '-');
      o.raw[o.nraw - 1].t = '>';
   } else {
      /*> if      (a_type == 'S') printf("beg (fake) point, adding\n");               <* 
       *> else if (a_type == 'F') printf("end (fake) point, adding\n");               <* 
       *> else                    printf("unique, adding\n");                         <*/
      ++o.nraw;
   }
   /*---(complete)-----------------------*/
   return 0;
}


char          /*----: add a new raw point to outline -------------------------*/
RAW_touch          (int a_x, int a_y)
{
   return 0;
}

char          /*----: add a new raw point to outline -------------------------*/
RAW_lift           (int a_x, int a_y)
{
   return 0;
}

char          /*----: add a new raw point to outline -------------------------*/
RAW_add            (int a_x, int a_y)
{
   RAW_point (a_x, a_y, '-');
   return 0;
}





char          /*----: start a new outline with a prefix ----------------------*/
RAW_prefix         (int a_x, int a_y)
{
   /*---(initialize)---------------------*/
   DEBUG__RAW  printf("RAW POINTS (prefix)\n");
   OUT_clear ();
   stype   = 'p';
   ntries  =  0;
   /*---(save points)--------------------*/
   RAW_point (a_x, a_y, 'S');
   o.raw[o.nraw - 1].a = 'y';
   RAW_point (a_x, a_y, '-');
   o.raw[o.nraw - 1].t = '>';
   /*---(save adjustment)----------------*/
   xadj = 0;
   yadj = 0;
   /*---(complete)-----------------------*/
   return 0;
}

char          /*----: start a new outline ------------------------------------*/
RAW_begin          (int a_x, int a_y)
{
   /*---(check for prefix)---------------*/
   DEBUG__RAW  printf("RAW POINTS (begin)\n");
   if (stype != 'p') {
      OUT_clear ();
      ntries  =  0;
   }
   /*---(save points)--------------------*/
   stype   = 'o';
   RAW_point (a_x, a_y, 'S');
   o.raw[o.nraw - 1].a = 'y';
   RAW_point (a_x, a_y, '-');
   o.raw[o.nraw - 1].t = '>';
   /*---(save adjustment)----------------*/
   xadj = a_x;
   yadj = a_y;
   /*---(complete)-----------------------*/
   return 0;
}

char          /*----: finish off a set of outline raw points -----------------*/
RAW_end            (int a_x, int a_y)
{
   RAW_point (a_x, a_y, '-');
   RAW_point (a_x, a_y, 'F');
   o.raw[o.nraw - 1].a = 'y';
   RAW_equalize();
   DEBUG__RAW       gen_list(o.raw, o.nraw);
   DEBUG__RAW  printf("RAW POINTS (end)\n\n");
   return 0;
}

char          /*----: continue an outline with another stroke ----------------*/
RAW_continue       (int a_x, int a_y)
{
   /*---(check for prefix)---------------*/
   DEBUG__RAW  printf("RAW POINTS (continue)\n");
   /*---(save points)--------------------*/
   stype   = 'c';
   RAW_point (a_x, a_y, 'S');
   o.raw[o.nraw - 1].a = 'y';
   RAW_point (a_x, a_y, '-');
   o.raw[o.nraw - 1].t = '>';
   /*---(save adjustment)----------------*/
   xadj = 0;
   yadj = 0;
   /*---(complete)-----------------------*/
   return 0;
}

char          /*----: make raw xy relative to neutral origin -----------------*/
RAW_equalize       (void)
{
   /*---(locals)-----------+-----+-----+-*/
   int         i           =    0;
   DEBUG__RAW  printf("   adjust all by %dx, %dy\n", xadj, yadj);
   for (i = 0; i < o.nraw; ++i) {
      o.raw[i].x   -= xadj;
      o.raw[i].y   -= yadj;
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
   o.raw[o.nraw - 1].a = 'y';
   RAW_point (a_x, a_y, '-');
   o.raw[o.nraw - 1].t = '>';
   /*---(save adjustment)----------------*/
   xadj = 0;
   yadj = 0;
   /*---(complete)-----------------------*/
   return 0;
}



/*=================================(end-code)=================================*/
