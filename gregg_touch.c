/*============================[[    beg-code    ]]============================*/
#include    "gregg.h"



static char  s_event     [LEN_STR];
static FILE *s_file      = NULL;

static int   s_new_x     = 0;
static int   s_new_y     = 0;
static int   s_new_r     = 0;
static int   s_fileno    = 0;
static int   s_flags     = 0;
static int   s_line      = 0;


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



char
TOUCH_init           (void)
{
   /*> snprintf (s_event  , LEN_STR, "%s%s", "/dev/input/", "event6");                <*/
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

char             /* [------] read input event -------------------------------*/
TOUCH_read           (void)
{
   /*---(locals)-----------+-----+-----+-*/
   char        rce         =  -10;           /* return code for errors         */
   int         rc          =    0;
   static int  x_count     =    1;
   uchar       x_ch        =    0;
   int         x_field     =    0;
   char        t           [LEN_STR];
   char        x_msg       [LEN_STR];
   char        ev_type     = 0;
   char        ev_code     = 0;
   int         ev_value    = 0;
   float       x           = 0.0;
   float       y           = 0.0;
   /*---(header)-------------------------*/
   DEBUG_TOUCH  yLOG_enter   (__FUNCTION__);
   /*---(check)--------------------------*/
   rc = TOUCH__check  ();
   DEBUG_TOUCH  yLOG_value   ("rc"        , rc);
   --rce;  if (rc < 0) {
      DEBUG_TOUCH  yLOG_exitr   (__FUNCTION__, rce);
      return rce;
   }
   /*---(prepare)------------------------*/
   while (1) {
      /*---(start)-----------------------*/
      if (x_field == 0)   sprintf (x_msg, "%-5d  ", x_count);
      /*---(get and format character)----*/
      ++x_field;
      x_ch = fgetc (s_file);
      sprintf (t  , "%02x ", x_ch);
      strlcat  (x_msg, t, LEN_STR);
      /*---(check for columns breaks)----*/
      if (x_field ==  8)   { strlcat (x_msg, "  ", LEN_STR);        continue; }
      if (x_field == 16)   { strlcat (x_msg, "  ", LEN_STR);        continue; }
      /*---(filter certain columns)------*/
      if (x_field <  17)                                   continue;
      /*---(event types)-----------------*/
      if (x_field == 17)   { ev_type   = x_ch;             continue; }
      if (x_field == 18) {
         ev_type += x_ch * 256;
         switch (ev_type) {
         case 0  : strlcat (x_msg, "SYN    ", LEN_STR);      break;
         case 1  : strlcat (x_msg, "KEY    ", LEN_STR);      break;
         case 2  : strlcat (x_msg, "REL    ", LEN_STR);      break;
         case 3  : strlcat (x_msg, "ABS    ", LEN_STR);      break;
         case 4  : strlcat (x_msg, "MISC   ", LEN_STR);      break;
         default : strlcat (x_msg, "-?-    ", LEN_STR);      break;
         }
         continue;
      }
      /*---(event codes)-----------------*/
      if (x_field == 19)   { ev_code   = x_ch;             continue; }
      if (x_field == 20) {
         ev_code += x_ch * 256;
         if (ev_type == ABS_MODE) {
            switch (ev_code) {
            case 0x00 : strlcat (x_msg, "ABS_X   ", LEN_STR);  break;
            case 0x01 : strlcat (x_msg, "ABS_Y   ", LEN_STR);  break;
            case 0x18 : strlcat (x_msg, "PRES    ", LEN_STR);  break;
            case 0x19 : strlcat (x_msg, "DIST    ", LEN_STR);  break;
            case 0x1a : strlcat (x_msg, "TILT_X  ", LEN_STR);  break;
            case 0x1b : strlcat (x_msg, "TILT_Y  ", LEN_STR);  break;
            default   : strlcat (x_msg, "        ", LEN_STR);  break;
            }
         } else {
            strlcat (x_msg, "        ", LEN_STR);
         }
         continue;
      }
      /*---(event value)-----------------*/
      if (x_field == 21)   { ev_value  = x_ch;             continue; }
      if (x_field == 22)   { ev_value += x_ch * 256;       continue; }
      if (x_field == 23)   { ev_value += x_ch * (256 * 256);       continue; }
      /*---(last value)------------------*/
      if (x_field == 24) {
         /*---(format value)-------------*/
         ev_value += x_ch * (256 * 256 * 256);
         sprintf (t, "%10d", ev_value);
         strlcat  (x_msg, t, LEN_STR);
      }
      /*---(finger touch)----------------*/
      if (ev_type == ABS_MODE && ev_code == 0x39 && ev_value >= 0)  {
         strlcat (x_msg, "  TOUCH", LEN_STR);
         /*> stroke.channel = ev_value;                                               <*/
         /*> stroke.beg     = timestamp ();                                           <*/
         /*> ndot = 0;                                                                <*/
      }
      /*---(finger lift)-----------------*/
      if (ev_type == ABS_MODE && ev_code == 0x39 && ev_value <  0)  {
         strlcat (x_msg, "  LIFT", LEN_STR);
         /*> stroke.end   = timestamp ();                                             <*/
         /*> stroke.diff  = stroke.end - stroke.beg;                                  <*/
         /*> rc = STROKE_end (s_new_x, s_new_y, s_new_r);                             <*/
      }
      /*---(x-value)---------------------*/
      if (ev_type == ABS_MODE && ev_code == 0x35)  {
         x     = ((float) ev_value / 1800.0);
         s_new_x = (1366.0 * (1 - x));
         s_new_y = 0;
         s_new_r = 0;
         sprintf (t, "val = %8.4f, x = %4f, y = %4f, r = %4f", ev_value, s_new_x, s_new_y, s_new_r);
      }
      /*---(y-value)---------------------*/
      if (ev_type == ABS_MODE && ev_code == 0x36)  {
         y     = ((float) ev_value / 1800.0);
         s_new_y = (768.0 * y);
         s_new_r = sqrt((s_new_x * s_new_x) + (s_new_y * s_new_y));
         /*> if (ndot == 0) rc = STROKE_begin (s_new_x, s_new_y, s_new_r);            <* 
          *> else           rc = stroke_next  (s_new_x, s_new_y, s_new_r);            <*/
         sprintf (t, "val = %8.4f, x = %4f, y = %4f, r = %4f", ev_value, s_new_x, s_new_y, s_new_r);
      }
      /*---(display)---------------------*/
      if (ev_type == ABS_MODE) {
         /*---(headers and line breaks)--------*/
         if ((s_line % 15) == 0) {
            printf ("\n");
            printf ("-----  -----------------------junk----------------------   ev_type---   ev_code----   ev_value--------------\n");
            printf ("count  -- -- -- -- -- -- -- --   -- -- -- -- -- -- -- --   lo hi type   lo hi code-   1s 2n 3r 4t value-----\n");
         }
         if ((s_line %  3) == 0) {
            printf ("\n");
         }
         printf ("%s\n", x_msg);
         ++s_line;
      }
      /*---(reset values)-------------*/
      x_field = 0;
      ++x_count;
      /*---(done)------------------------*/
      break;
   }
   TOUCH__normal ();
   /*---(complete)-------------------------*/
   DEBUG_TOUCH  yLOG_exit    (__FUNCTION__);
   return rc;
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

/*============================[[ end-of-code ]]===============================*/
