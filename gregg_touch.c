/*============================[[    beg-code    ]]============================*/
#include    "gregg.h"


#define     LEN_STR       200

static char  s_event     [LEN_STR];
static FILE *s_file      = NULL;

static int   s_new_x     = 0;
static int   s_new_y     = 0;
static int   s_new_r     = 0;
static int   s_fileno    = 0;
static int   s_flags     = 0;
static int   s_line      = 0;




char
TOUCH_init           (void)
{
   snprintf (s_event  , LEN_STR, "%s%s", "/dev/input/", "event16");
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
   /*---(locals)-----------*-------------*/
   char        rce         = -10;           /* return code for errors         */
   int         rc          = 0;
   int         ch          = 0;
   uchar       x_ch        = 0;
   int         x_count       = 0;
   char   next   [100];
   char   output [200];
   char   ev_type  = 0;
   char   ev_code  = 0;
   int    ev_value = 0;
   float       x           = 0.0;
   float       y           = 0.0;
   /*---(header)-------------------------*/
   DEBUG_TOUCH  yLOG_enter   (__FUNCTION__);
   /*---(check)--------------------------*/
   rc = TOUCH__check  ();
   --rce;  if (rc < 0) {
      DEBUG_TOUCH  yLOG_exit    (__FUNCTION__);
      return rce;
   }
   /*---(headers and line breaks)--------*/
   if ((s_line % 15) == 0) {
      printf ("\n-- -- -- -- -- -- -- --   -- -- -- -- -- -- -- --   -- -- typ   -- -- code-   -- -- -- -- value-----\n");
   }
   if ((s_line %  3) == 0) {
      printf ("\n");
   }
   ++s_line;
   /*---(prepare)------------------------*/
   x_count = 0;
   strcpy  (output, "");
   while (1) {
      /*---(get and format character)----*/
      ++x_count;
      ch = fgetc (s_file);
      x_ch = ch;
      sprintf (next  , "%02x ", x_ch);
      strcat  (output, next);
      /*---(check for columns breaks)----*/
      if (x_count ==  8)   { strcat (output, "  ");        continue; }
      if (x_count == 16)   { strcat (output, "  ");        continue; }
      /*---(filter certain columns)------*/
      if (x_count <  17)                                   continue;
      /*---(event types)-----------------*/
      if (x_count == 17)   { ev_type   = x_ch;             continue; }
      if (x_count == 18) {
         ev_type += x_ch * 256;
         switch (ev_type) {
         case 0  : strcat (output, "SYN   ");      break;
         case 1  : strcat (output, "KEY   ");      break;
         case 2  : strcat (output, "REL   ");      break;
         case 3  : strcat (output, "ABS   ");      break;
         default : strcat (output, "-?-   ");      break;
         }
         continue;
      }
      /*---(event codes)-----------------*/
      if (x_count == 19)   { ev_code   = x_ch;             continue; }
      if (x_count == 20) {
         ev_code += x_ch * 256;
         if (ev_type != 3) { strcat (output, "        ");  continue; }
         switch (ev_code) {
         case 0x00 : strcat (output, "ABS_X   ");  break;
         case 0x01 : strcat (output, "ABS_Y   ");  break;
         case 0x2f : strcat (output, "SLOT    ");  break;
         case 0x35 : strcat (output, "EPS_X   ");  break;
         case 0x36 : strcat (output, "EPS_Y   ");  break;
         case 0x39 : strcat (output, "ID      ");  break;
         default   : strcat (output, "--?--   ");  break;
         }
         continue;
      }
      /*---(event value)-----------------*/
      if (x_count == 21)   { ev_value  = x_ch;             continue; }
      if (x_count == 22)   { ev_value += x_ch * 256;       continue; }
      if (x_count == 23)   { ev_value += x_ch * (256 * 256);       continue; }
      /*---(last value)------------------*/
      if (x_count == 24) {
         /*---(format value)-------------*/
         ev_value += x_ch * (256 * 256 * 256);
         sprintf (next, "%10d", ev_value);
         /*---(print final line)---------*/
         strcat  (output, next);
         printf ("%s\n", output);
         /*---(reset values)-------------*/
         strcpy (output, "");
         x_count = 0;
      }
      /*---(finger touch)----------------*/
      if (ev_type == 3 && ev_code == 0x39 && ev_value >= 0)  {
         /*> stroke.channel = ev_value;                                               <*/
         /*> stroke.beg     = timestamp ();                                           <*/
         /*> ndot = 0;                                                                <*/
      }
      /*---(finger lift)-----------------*/
      if (ev_type == 3 && ev_code == 0x39 && ev_value <  0)  {
         /*> stroke.end   = timestamp ();                                             <*/
         /*> stroke.diff  = stroke.end - stroke.beg;                                  <*/
         /*> rc = STROKE_end (s_new_x, s_new_y, s_new_r);                             <*/
      }
      /*---(x-value)---------------------*/
      if (ev_type == 3 && ev_code == 0x35)  {
         x     = ((float) ev_value / 1800.0);
         s_new_x = (1366.0 * (1 - x));
         s_new_y = 0;
      }
      /*---(y-value)---------------------*/
      if (ev_type == 3 && ev_code == 0x36)  {
         y     = ((float) ev_value / 1800.0);
         s_new_y = (768.0 * y);
         s_new_r = sqrt((s_new_x * s_new_x) + (s_new_y * s_new_y));
         /*> if (ndot == 0) rc = STROKE_begin (s_new_x, s_new_y, s_new_r);            <* 
          *> else           rc = stroke_next  (s_new_x, s_new_y, s_new_r);            <*/
      }
      /*---(done)------------------------*/
      break;
   }
   TOUCH__normal ();
   /*---(complete)-------------------------*/
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
