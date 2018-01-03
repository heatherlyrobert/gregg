/*=================================(beg-code)=================================*/
#include "gregg.h"



/*===[[ FILE INFO ]]----------------------------------------------------------*
 *
 *   file          : gregg_main.c
 *   reason        : isolate the main function to allow unit testing
 *   details       : keep this file very, very simple
 */


int        /*----: process main program loop ---------------------------------*/
main               (int argc, char *argv[])
{
   /*---(locals)-----------+-----+-----+-*/
   char        rc          =    0;
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
   /*---(main loop)----------------------*/
   rc = PROG_event ();
   /*---(wrapup)-------------------------*/
   rc = PROG_finish ();
   rc = PROG_end    ();
   /*---(complete)-----------------------*/
   return rc;
}

/*=================================(end-code)=================================*/
