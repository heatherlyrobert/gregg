/*============================---(source-start)---============================*/
#include "gregg.h"


char unit_answer  [LEN_STR];

/*============================--------------------============================*/
/*===----                         unit testing                         ----===*/
/*============================--------------------============================*/
void o___UNITTEST_______________o (void) {;}

char         /*-> set up programgents/debugging ------[ light  [uz.320.011.05]*/ /*-[00.0000.00#.#]-*/ /*-[--.---.---.--]-*/
PROG__unit_quiet    (void)
{
   char        x_narg  = 1;
   char       *x_args [1]  = { "gregg" };
   PROG_urgents (x_narg, x_args);
   PROG_startup (x_narg, x_args);
   my.dict = '-';
   return 0;
}

char         /*-> set up programgents/debugging ------[ light  [uz.320.011.05]*/ /*-[00.0000.00#.!]-*/ /*-[--.---.---.--]-*/
PROG__unit_loud     (void)
{
   char        x_narg  = 2;
   char       *x_args [2]  = { "gregg_unit", "@@kitchen"    };
   PROG_urgents (x_narg, x_args);
   yURG_by_name  ("raw"          , YURG_ON);
   yURG_by_name  ("avg"          , YURG_ON);
   yURG_by_name  ("calc"         , YURG_ON);
   yURG_by_name  ("match"        , YURG_ON);
   yURG_by_name  ("data"         , YURG_ON);
   PROG_startup (x_narg, x_args);
   my.dict = '-';
   return 0;
}

char         /*-> set up program urgents/debugging ---[ light  [uz.210.001.01]*/ /*-[00.0000.00#.!]-*/ /*-[--.---.---.--]-*/
PROG__unit_end      (void)
{
   PROG_shutdown  ();
   return 0;
}
