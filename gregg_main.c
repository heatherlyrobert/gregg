/*=================================(beg-code)=================================*/
#include "gregg.h"



int        /*----: process main program loop ---------------------------------*/
main               (int argc, char *argv[])
{
   /*---(locals)-----------+-----+-----+-*/
   char        rc          =    0;
   tTSPEC      x_dur;
   XKeyEvent  *key_event;
   char        the_key     [5];
   int         the_bytes;
   uchar       x_key       =  ' ';
   int         x_loop      = 0;
   /*---(initialize)---------------------*/
   if (rc >= 0)  rc = yURG_logger  (argc, argv);
   if (rc >= 0)  rc = PROG_init    ();
   if (rc >= 0)  rc = yURG_urgs    (argc, argv);
   if (rc >= 0)  rc = PROG_args    (argc, argv);
   if (rc >= 0)  rc = PROG_begin   ();
   if (rc >= 0)  rc = PROG_final   ();
   if (rc <  0) {
      PROG_end ();
      return -1;
   }
   /*---(for timer)------------------------*/
   x_dur.tv_sec    = 0;
   x_dur.tv_nsec   = 0.5 * 1000000;
   /*---(header)-------------------------*/
   DEBUG_TOPS   yLOG_enter    (__FUNCTION__);
   while (1) {
      x_key = 0;
      if (XPending(DISP)) {
         XNextEvent(DISP, &EVNT);
         switch(EVNT.type) {
         case KeyPress:
            /*---(get the key)---------------------*/
            key_event  = (XKeyEvent *) &EVNT;
            the_bytes = XLookupString((XKeyEvent *) &EVNT, the_key, 5, NULL, NULL);
            if (the_bytes < 1) break;
            x_key = the_key [0];
            break;
         }
      }
      x_key = yVIKEYS_main_input  (RUN_USER, x_key);
      yVIKEYS_main_handle (x_key);
      if (yVIKEYS_quit ())  break;
      ++x_loop;
      TOUCH_read ();
      if ((x_loop % 20) == 0)  yVIKEYS_view_all (0.0);
      /*---(sleeping)--------------------*/
      nanosleep    (&x_dur, NULL);
   }
   DEBUG_TOPS   yLOG_exit     (__FUNCTION__);
   /*---(wrapup)-------------------------*/
   rc = PROG_finish ();
   rc = PROG_end    ();
   /*---(complete)-----------------------*/
   return rc;
}

/*=================================(end-code)=================================*/
