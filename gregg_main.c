/*============================---(source-start)---============================*/

/*===[[ FILE INFO ]]----------------------------------------------------------*
 *
 *   file          : gregg_main.c
 *   reason        : isolate the main function to allow unit testing
 *   details       : keep this file very, very simple
 */

#include "gregg.h"

int        /*----: process main program loop ---------------------------------*/
main               (int argc, char *argv[])
{
   /*---(locals)-------------------------*/
   int rc = 0;
   /*---(startup)------------------------*/
   rc = prog_init();
   if (rc == 0) rc = prog_args(argc, argv);
   if (rc == 0) rc = prog_begin();
   /*---(main loop)----------------------*/
   while (rc != 1) {
      rc = prog_event();
      if (rc >  1) format_change(rc);
   }
   /*---(wrapup)-------------------------*/
   prog_end();
   /*---(complete)-----------------------*/
   return rc;
}

/*============================----(source-end)----============================*/
