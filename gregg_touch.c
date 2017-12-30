/*============================[[    beg-code    ]]============================*/
#include    "gregg.h"


#define      MAX_Y       19700.0
#define      MAX_X       31500.0

static char  s_event     [LEN_STR];
static FILE *s_file      = NULL;

static int   s_line     = 0;
static char  s_touch    = '-';
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


#define      REL_MODE    2
#define      ABS_MODE    3

#define      EV_SYN      0
#define      EV_KEY      1
#define      EV_REL      2
#define      EV_ABS      3
#define      EV_MISC     4

/*---(struct.re)--------+-----------+-*//*-+----------------------------------*/
#define     ABS_X       0x00
#define     ABS_Y       0x01
#define     ABS_Z       0x02
#define     ABS_PRES    0x18
#define     ABS_DIST    0x19
#define     ABS_TILTX   0x1a
#define     ABS_TILTY   0x1b
#define     ABS_MISC    0x28
#define     ABS_SLOT    0x2f

#define     MAX_EVCODE  100
typedef     struct      cEVCODE     tEVCODE;
struct cEVCODE {
   int         code;
   char        abbr        [LEN_LABEL];
   char        desc        [LEN_DESC ];
   int         count;
};
tEVCODE     s_evcode   [MAX_EVCODE] = {
   { ABS_X      , "ABS_X"    , ""                                             , 0 },
   { ABS_Y      , "ABS_Y"    , ""                                             , 0 },
   { ABS_PRES   , "PRES"     , ""                                             , 0 },
   { ABS_DIST   , "DIST"     , ""                                             , 0 },
   { ABS_TILTX  , "TILT_X"   , ""                                             , 0 },
   { ABS_TILTY  , "TILT_Y"   , ""                                             , 0 },
   { ABS_MISC   , "MISC"     , ""                                             , 0 },
   { ABS_SLOT   , "SLOT"     , ""                                             , 0 },
   { -1         , "end"      , ""                                             , 0 },
};


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
   snprintf (s_event  , LEN_STR, "%s%s", "/dev/input/", "event6");
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
      DEBUG_TOUCH  yLOG_fatal   ("device"    , "event file can not be openned");
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
   float       x_xrel      =  0.0;
   float       x_yrel      =  0.0;
   /*---(check)--------------------------*/
   rc = TOUCH__check  ();
   if (rc < 0)  return 0;
   /*---(read)---------------------------*/
   fread (&x_event, sizeof (x_event), 1, s_file);
   if (x_event.type != ABS_MODE)  return 0;
   /*---(touch and pressure)-------------*/
   if (x_event.code == ABS_PRES) {
      s_pres = x_event.value;
      /*---(touch)-----------------------*/
      if      (s_touch == '-' && x_event.value >= 25) {
         s_touch = 'y';
         RAW_touch (s_xpos, s_ypos);
      }
      /*---(release)---------------------*/
      else if (s_touch == 'y' && x_event.value <  25) {
         s_touch = '-';
         RAW_lift  (s_xpos, s_ypos);
      }
      ++s_line;
      return 0;
   }
   /*---(position)-----------------------*/
   switch (x_event.code) {
   case  ABS_X :
      s_xpad = x_event.value;
      x_xrel = 1.0 - (x_event.value / MAX_X);
      if (x_xrel >= 0.001 && x_xrel <= 0.999) {
         s_xrel = x_xrel;
         s_xpos = s_xrel * win.width;
      }
      break;
   case  ABS_Y :
      s_ypad = x_event.value;
      x_yrel = 1.0 - (x_event.value / MAX_Y);
      if (x_yrel >= 0.001 && x_yrel <= 0.999) {
         s_yrel = x_yrel;
         s_ypos = s_yrel * win.height;
      }
      break;
   default     :
      return 0;
      break;
   }
   if (s_touch == 'y')  RAW_normal (s_xpos, s_ypos);
   ++s_line;
   /*---(headers and line breaks)--------*/
   RPTG_TOUCH {
      if ((s_line % 15) == 0) {
         printf ("\n");
         printf ("count  type  -pad-  -rel-  -scr-\n");
      }
      if ((s_line %  3) == 0) {
         printf ("\n");
      }
      printf ("%5d   X  %5d  %5.3f  %5d   Y  %5d  %5.3f  %5d\n",
            s_line, s_xpad, s_xrel, s_xpos, s_ypad, s_yrel, s_ypos);
   }
   /*---(reset values)-------------*/
   TOUCH__normal ();
   /*---(complete)-------------------------*/
   DEBUG_TOUCH  yLOG_exit    (__FUNCTION__);
   return rc;
}



/*============================[[ end-of-code ]]===============================*/
