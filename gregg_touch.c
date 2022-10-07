/*============================[[    beg-code    ]]============================*/
#include    "gregg.h"




static char  s_event     [LEN_STR];
static FILE *s_file      = NULL;

static int   s_line     = 0;
static char  s_touch    = MODE_CURSOR;
static int   s_pres     = 0;
static int   s_xpad     = 0;
static int   s_ypad     = 0;
static float s_xrel     = 0;
static float s_yrel     = 0;
static int   s_xpos     = 0;
static int   s_ypos     = 0;

static int   s_new_x     = 0;
static int   s_new_y     = 0;
static int   s_new_r     = 0;
static int   s_fileno    = 0;
static int   s_flags     = 0;



/*---(struct.re)--------+-----------+-*//*-+----------------------------------*/
#define     ABS_MODE    3
#define     ABS_X       0x00
#define     ABS_Y       0x01
#define     ABS_PRES    0x18



/*---(struct.re)--------+-----------+-*//*-+----------------------------------*/
typedef     struct      cEVENT      tEVENT;
struct      cEVENT {
   int64_t     one;
   int64_t     two;
   int16_t     type;
   int16_t     code;
   int32_t     value;
};



/*====================------------------------------------====================*/
/*===----                        program level                         ----===*/
/*====================------------------------------------====================*/
PRIV void  o___PROGRAM_________o () { return; }

char
TOUCH_init           (void)
{
   snprintf (s_event  , LEN_STR, "%s%s", "/dev/input/", "event12");
   printf ("s_event = å%sæ\n", s_event);
   TOUCH__open ();
   return 0;
}

char
TOUCH_wrap           (void)
{
   TOUCH__close ();
   return 0;
}



/*====================------------------------------------====================*/
/*===----                          device file                         ----===*/
/*====================------------------------------------====================*/
PRIV void  o___DEVICE__________o () { return; }

char         /*--> open touch device event file ----------[ leaf   [ ------ ]-*/
TOUCH__open          (void)
{
   /*---(locals)-----------+-----------+-*/
   char        rce         = -10;           /* return code for errors         */
   /*---(header)-------------------------*/
   DEBUG_TOUCH  yLOG_enter   (__FUNCTION__);
   /*---(open)---------------------------*/
   DEBUG_TOUCH  yLOG_info    ("name"      , s_event);
   s_file = fopen (s_event, "r");
   DEBUG_TOUCH  yLOG_point   ("*file"     , s_file);
   --rce;  if (s_file == NULL) {
      /*> DEBUG_TOUCH  yLOG_fatal   ("device"    , "event file can not be openned");   <*/
      DEBUG_TOUCH  yLOG_exit    (__FUNCTION__);
      return rce;
   }
   DEBUG_TOUCH  yLOG_note    ("succesfully opened");
   /*---(complete)-----------------------*/
   DEBUG_TOUCH  yLOG_exit    (__FUNCTION__);
   return  0;
}

char         /*--> open touch device event file ----------[ leaf   [ ------ ]-*/
TOUCH__normal        (void)
{
   fcntl  (s_fileno, F_SETFL, s_flags);
   return 0;
}

char         /*--> open touch device event file ----------[ leaf   [ ------ ]-*/
TOUCH__check         (void)
{
   /*---(locals)-----------*-------------*/
   char        rce         = -10;           /* return code for errors         */
   int         rc          = 0;
   int         x_flags     = 0;
   int         x_ch        = 0;
   /*---(header)-------------------------*/
   DEBUG_TOUCH  yLOG_senter  (__FUNCTION__);
   /*---(prepare)------------------------*/
   s_fileno    = 0;
   s_flags     = 0;
   /*---(set to non-blocking)------------*/
   DEBUG_TOUCH  yLOG_snote   ("non-blocking");
   s_fileno    = fileno (s_file);
   s_flags     = fcntl  (s_fileno, F_GETFL, 0);
   fcntl (s_fileno, F_SETFL, s_flags | O_NONBLOCK);
   /*---(test for input)-----------------*/
   x_ch = fgetc (s_file);
   DEBUG_TOUCH  yLOG_sint    (x_ch);
   --rce;  if (x_ch < 0) {
      DEBUG_TOUCH  yLOG_snote   ("nothing in queue");
      TOUCH__normal ();
      DEBUG_TOUCH  yLOG_sexit   (__FUNCTION__);
      return rce;
   }
   /*---(put the char back)--------------*/
   rc = ungetc (x_ch, s_file);
   --rce;  if (rc != x_ch) {
      DEBUG_TOUCH  yLOG_snote   ("could not unget");
      TOUCH__normal ();
      DEBUG_TOUCH  yLOG_sexit   (__FUNCTION__);
      return rce;
   }
   DEBUG_TOUCH  yLOG_snote   ("ready to read");
   /*---(complete)-----------------------*/
   DEBUG_TOUCH  yLOG_sexit   (__FUNCTION__);
   return  0;
}

char
TOUCH__close         (void)
{
   int         rc           = 0;
   /*---(open)---------------------------*/
   rc = fclose (s_file);
   if (rc < 0) {
      printf ("BLOODY CAN'T CLOSE EVENT FILE\n");
      exit (-2);
   }
   /*---(complete)-----------------------*/
   return  0;
}

char             /* [------] read input event -------------------------------*/
TOUCH_control        (int a_x, int a_y)
{
   /*---(locals)-----------+-----+-----+-*/
   int         x_xpos      =    0;
   int         x_xinc      =    0;
   int         i           =    0;
   int         x_total     =    0;
   /*---(defense)------------------------*/
   if (s_touch != MODE_CONTROL)   return 0;
   printf ("touched controls\n");
   /*---(complete)-----------------------*/
   return  0;
}

char             /* [------] read input event -------------------------------*/
TOUCH_player         (int a_x, int a_y)
{
   /*---(locals)-----------+-----+-----+-*/
   int         x_inc       =    0;
   int         x_slot      =    0;
   int      x_wide;
   int      x_xmin, x_xmax, x_xlen, x_ymin, x_ymax, x_ylen;
   /*---(defense)------------------------*/
   if (s_touch != MODE_PLAYER)    return 0;
   /*> yVIKEYS_view_size     (YVIKEYS_MAIN, NULL   , NULL   , &x_wide, NULL   , NULL);   <*/
   /*> yVIKEYS_view_bounds   (YVIKEYS_MAIN, &x_xmin, NULL, &x_xlen, &x_ymin, NULL, &x_ylen);   <*/
   x_xmax = x_xmin + x_xlen;
   x_ymax = x_ymin + x_ylen;
   x_inc = x_wide / 35.0;
   x_slot = trunc ((a_x - x_xmin - 5) / 35);
   /*> printf ("player x=%4d, w=%4d, i=%4d, s=%4d\n", a_x, x_xfull, x_inc, x_slot);   <*/
   /*> printf ("o.curr = %4d, o.total = %4d\n", o.curr, o.total);                     <*/
   switch (x_slot) {
   case  5 :
      o.curr = 0;
      break;
   case  6 :
      --o.curr;
      break;
   case  7 :
      ++o.curr;
      break;
   case  8 :
      o.curr = o.total;
      break;
   }
   /*> printf ("o.curr = %4d, o.total = %4d\n", o.curr, o.total);                     <*/
   if (o.curr <  0      )  o.curr = 0;
   if (o.curr >= o.total)  o.curr = o.total - 1;
   /*> printf ("o.curr = %4d, o.total = %4d\n", o.curr, o.total);                     <*/
   OUT_pick (o.curr);
   /*---(complete)-----------------------*/
   return  0;
}

char             /* [------] read input event -------------------------------*/
TOUCH_slider         (int a_x, int a_y)
{
   /*---(locals)-----------+-----+-----+-*/
   int         x_xpos      =    0;
   int         x_xinc      =    0;
   int         i           =    0;
   int         x_total     =    0;
   int      x_xmin, x_xfull;
   /*---(defense)------------------------*/
   if (s_touch != MODE_SLIDER)    return 0;
   if (o.navg  <=    0)           return 0;
   /*---(process)------------------------*/
   /*> yVIKEYS_view_bounds   (YVIKEYS_MAIN, &x_xmin, NULL, &x_xfull, NULL, NULL, NULL);   <*/
   x_xpos = a_x - x_xmin;
   /*> printf ("a_x    %4d, x_xpos %4d\n", a_x, x_xpos);                              <*/
   x_xinc = x_xfull / (o.navg - 2);
   /*> printf ("navg   %4d, x_inc  %4d\n", o.navg, x_xinc);                           <*/
   o.cavg = -1;
   for (i = 1; i < o.navg - 1; ++i) {
      /*> printf ("%2d  x_xpos %4d, x_total  %4d\n", i, x_xpos, x_total);             <*/
      if (x_xpos >  x_total) {
         x_total += x_xinc;
         /*> printf ("   next\n");                                                    <*/
         continue;
      }
      /*> printf ("   CAUGHT\n");                                                     <*/
      o.cavg = i;
      break;
   }
   if (o.cavg < 0)  o.cavg = o.navg;
   /*---(complete)-----------------------*/
   return  0;
}



/*====================------------------------------------====================*/
/*===----                         data reading                         ----===*/
/*====================------------------------------------====================*/
PRIV void  o___READING_________o () { return; }

char             /* [------] read input event -------------------------------*/
TOUCH_read           (void)
{
   /*---(locals)-----------+-----+-----+-*/
   char        rce         =  -10;           /* return code for errors         */
   int         rc          =    0;
   tEVENT      x_event;
   char        x_real      =  '-';
   int         x_mwide     =    0;
   int         x_rwide     =    0;
   /*---(check)--------------------------*/
   rc = TOUCH__check  ();
   if (rc < 0)  return 0;
   /*---(header)-------------------------*/
   DEBUG_TOUCH  yLOG_enter   (__FUNCTION__);
   /*---(prepare)------------------------*/
   /*> yVIKEYS_view_bounds   (YVIKEYS_MAIN  , &(my.x_min), &(my.x_max ), NULL, &(my.y_min), &(my.y_max ), NULL);   <*/
   /*> yVIKEYS_view_bounds   (YVIKEYS_MAIN  , NULL     , NULL      , &x_mwide  , NULL     , NULL      , &(my.y_tall));   <*/
   /*> yVIKEYS_view_bounds   (YVIKEYS_RIBBON, NULL     , NULL      , &x_rwide  , NULL     , NULL      , &(my.y_tall));   <*/
   my.x_wide = x_mwide + x_rwide;
   my.ratio  = my.x_scale / my.x_wide;
   /*---(read)---------------------------*/
   DEBUG_TOUCH  yLOG_note    ("read event");
   fread (&x_event, sizeof (x_event), 1, s_file);
   if (x_event.type != ABS_MODE) {
      DEBUG_TOUCH  yLOG_note    ("not ABS mode, ignoring");
      DEBUG_TOUCH  yLOG_exit    (__FUNCTION__);
      return 0;
   }
   /*---(touch and pressure)-------------*/
   if (x_event.code == ABS_PRES) {
      DEBUG_TOUCH  yLOG_note    ("pressure event");
      s_pres = x_event.value;
      DEBUG_TOUCH  yLOG_value   ("s_pres"    , s_pres);
      /*---(touch)-----------------------*/
      if      (s_touch == MODE_CURSOR && s_pres >= 25) {
         DEBUG_TOUCH  yLOG_note    ("new touch");
         s_touch = MODE_TOUCH;
         RAW_touch (s_xpad, s_ypad);
      }
      /*---(release)---------------------*/
      else if (s_touch != MODE_CURSOR && s_pres <  25) {
         DEBUG_TOUCH  yLOG_note    ("lifted existing touch");
         if (s_touch == MODE_TOUCH)  RAW_lift  (s_xpad, s_ypad);
         s_touch = MODE_CURSOR;
      }
      ++s_line;
      DEBUG_TOUCH  yLOG_char    ("s_touch"   , s_touch);
      DEBUG_TOUCH  yLOG_exit    (__FUNCTION__);
      return 0;
   }
   /*---(position)-----------------------*/
   switch (x_event.code) {
   case  ABS_X :
      DEBUG_TOUCH  yLOG_note    ("X-movement event");
      x_real  = 'y';
      if (x_event.value <= my.x_scale * 0.01) {
         DEBUG_TOUCH  yLOG_note    ("lifted, i.e., coord went too low");
         s_touch = MODE_CURSOR;
         x_real  = '-';
      }
      if (x_event.value >= my.x_scale * 0.99) {
         DEBUG_TOUCH  yLOG_note    ("lifted, i.e., coord went too high");
         s_touch = MODE_CURSOR;
         x_real  = '-';
      }
      if (x_real == 'y') {
         s_xpad = my.x_scale - x_event.value;
         s_xrel = s_xpad / (float) my.x_scale;
         s_xpos = my.x_min + (s_xrel * my.x_wide);
      }
      break;
   case  ABS_Y :
      DEBUG_TOUCH  yLOG_note    ("Y-movement event");
      x_real  = 'y';
      if (x_event.value <= my.y_scale * 0.01) {
         DEBUG_TOUCH  yLOG_note    ("lifted, i.e., coord went too low");
         s_touch = MODE_CURSOR;
         x_real  = '-';
      }
      if (x_event.value >= my.y_scale * 0.99) {
         DEBUG_TOUCH  yLOG_note    ("lifted, i.e., coord went too high");
         s_touch = MODE_CURSOR;
         x_real  = '-';
      }
      if (x_real == 'y') {
         s_ypad = x_event.value;
         s_yrel = s_ypad / (float) my.y_scale;
         s_ypos = my.y_min + (s_yrel * my.y_tall);
      }
      break;
   default     :
      DEBUG_TOUCH  yLOG_note    ("event to be ignored");
      DEBUG_TOUCH  yLOG_exit    (__FUNCTION__);
      return 0;
      break;
   }
   my.touch = s_touch;
   my.xpos  = s_xpos;
   my.ypos  = s_ypos;
   if (s_touch == MODE_TOUCH  )  RAW_normal    (s_xpad, s_ypad);
   if (s_touch == MODE_SLIDER )  TOUCH_slider  (s_xpos, s_ypos);
   ++s_line;
   /*---(headers and line breaks)--------*/
   RPTG_TOUCH {
      if ((s_line % 15) == 0) {
         printf ("\n");
         printf ("count    t  -pad-  -rel-  -scr-    t  -pad-  -rel-  -scr-\n");
      }
      if ((s_line %  3) == 0) {
         printf ("\n");
      }
      printf ("%5d    X  %5d  %5.3f  %5d    Y  %5d  %5.3f  %5d\n",
            s_line, s_xpad, s_xrel, s_xpos, s_ypad, s_yrel, s_ypos);
   }
   /*---(reset values)-------------*/
   TOUCH__normal ();
   /*---(complete)-------------------------*/
   DEBUG_TOUCH  yLOG_exit    (__FUNCTION__);
   return rc;
}



/*============================[[ end-of-code ]]===============================*/
