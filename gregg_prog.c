/*============================---(source-start)---============================*/
#include    "gregg.h"






int       out_start       = 0;

/*============================--------------------============================*/
/*===----                     startup and shutdown                     ----===*/
/*============================--------------------============================*/
void o___SHOOTS_________________o (void) {;}

char      verstring    [500];

char*        /*-> return library versio --------------[ leaf   [gs.420.012.00]*/ /*-[00.0000.012.!]-*/ /*-[--.---.---.--]-*/
PROG_version         (void)
{
   char    t [20] = "";
#if    __TINYC__ > 0
   strncpy (t, "[tcc built  ]", 15);
#elif  __GNUC__  > 0
   strncpy (t, "[gnu gcc    ]", 15);
#elif  __CBANG__  > 0
   strncpy (t, "[cbang      ]", 15);
#else
   strncpy (t, "[unknown    ]", 15);
#endif
   snprintf (verstring, 100, "%s   %s : %s", t, P_VERNUM, P_VERTXT);
   return verstring;
}

char       /*----: one-time initialization of program environment ------------*/
PROG_init          (void)
{
   /*---(header)-------------------------*/
   DEBUG_TOPS   yLOG_enter    (__FUNCTION__);
   /*---(log header)---------------------*/
   DEBUG_TOPS   yLOG_info     ("purpose" , "hyper-efficient, effective pen-based english input");
   DEBUG_TOPS   yLOG_info     ("gregg"   , PROG_version    ());
   DEBUG_TOPS   yLOG_info     ("yLOG"    , yLOGS_version   ());
   DEBUG_TOPS   yLOG_info     ("yURG"    , yURG_version    ());
   DEBUG_TOPS   yLOG_info     ("ySTR"    , ySTR_version    ());
   DEBUG_TOPS   yLOG_info     ("yGLTEX"  , yGLTEX_version  ());
   /*---(fonts)-----------------------*/
   strcpy (win.face_pretty, "comfortaa");
   strcpy (win.face_fixed, "hack");
   /*---(reporting flags)-------------*/
   my.run_mode     = RUN_NORMAL;
   my.dict         = 'y';
   my.rptg_touch   = '-';
   my.rptg_raw     = '-';
   my.rptg_base    = '-';
   my.rptg_key     = '-';
   my.rptg_dict    = '-';
   my.rptg_letter  = '-';
   my.show_control = '-';
   my.show_sample  = '-';
   my.show_player  = '-';
   my.quit         = '-';
   /*> my.ratio        = GREGG_WACOM;                                                 <*/
   my.ratio        = 10;
   /*---(other)-----------------------*/
   strlcpy (my.words, "", LEN_RECD);
   strlcpy (my.guide, "", LEN_RECD);
   /*---(setup vikeys)----------------*/
   yVIKEYS_init  (MODE_MAP);
   /*---(complete)--------------------*/
   DEBUG_TOPS   yLOG_exit     (__FUNCTION__);
   return 0;
}

char       /*----: process command line arguments ----------------------------*/
PROG_args          (int argc, char *argv[])
{
   char *a = NULL;
   int   i = 0;
   /*---(header)-------------------------*/
   DEBUG_TOPS   yLOG_enter    (__FUNCTION__);
   for (i = 1; i < argc; ++i) {
      a = argv[i];
      if (a[0] == '@')  continue;
      DEBUG_ARGS  yLOG_info    ("cli arg", a);
      if      (strcmp (a, "--rptg-touch"        ) == 0)   my.rptg_touch   = 'y';
      else if (strcmp (a, "--rptg-raw"          ) == 0)   my.rptg_raw     = 'y';
      else if (strcmp (a, "--rptg-base"         ) == 0)   my.rptg_base    = 'y';
      else if (strcmp (a, "--rptg-key"          ) == 0)   my.rptg_key     = 'y';
      else if (strcmp (a, "--rptg-dict"         ) == 0)   my.rptg_dict    = 'y';
      else if (strcmp (a, "--rptg-letter"       ) == 0)   my.rptg_letter  = 'y';
      else if (strcmp (a, "--show-control"      ) == 0)   my.show_control = 'y';
      else if (strcmp (a, "--show-player"       ) == 0)   my.show_player  = 'y';
      else if (strcmp (a, "--show-sample"       ) == 0)   my.show_sample  = 'y';
      else if (strcmp (a, "--english"           ) == 0)   my.run_mode     = RUN_ENGLISH;
      else if (strcmp (a, "--gregg"             ) == 0)   my.run_mode     = RUN_GREGG;
      else if (strcmp (a, "--convert"           ) == 0)   my.run_mode     = RUN_CONVERT;
      else if (strcmp (a, "--exact"             ) == 0)   my.run_mode     = RUN_EXACT;
      else if (strcmp (a, "--reverse"           ) == 0)   my.run_mode     = RUN_REVERSE;
      else if (strcmp (a, "--words"             ) == 0)   my.run_mode     = RUN_WORDS;
      /*> else if (strncmp(a, "@x", 5) == 0)  debug.top = debug.prep        = 'y';    <* 
       *> else if (strncmp(a, "@i", 5) == 0)  debug.top = debug.input       = 'y';    <* 
       *> else if (strncmp(a, "@o", 5) == 0)  debug.top = debug.output      = 'y';    <* 
       *> else if (strncmp(a, "@p", 5) == 0)  debug.top = debug.proc        = 'y';    <* 
       *> else if (strncmp(a, "@e", 5) == 0)  debug.top = debug.event       = 'y';    <* 
       *> else if (strncmp(a, "@g", 5) == 0)  debug.top = debug.graph       = 'y';    <* 
       *> else if (strncmp(a, "@@raw"         , 20) == 0) debug.raw         = 'y';    <* 
       *> else if (strncmp(a, "@@bas"         , 20) == 0) debug.bas         = 'y';    <* 
       *> else if (strncmp(a, "@@avg"         , 20) == 0) debug.averages    = 'y';    <* 
       *> else if (strncmp(a, "@@key"         , 20) == 0) debug.key         = 'y';    <* 
       *> else if (strncmp(a, "@@sharp"       , 20) == 0) debug.sharps      = 'y';    <* 
       *> else if (strncmp(a, "@@circle"      , 20) == 0) debug.circles     = 'y';    <* 
       *> else if (strncmp(a, "@@line"        , 20) == 0) debug.lines       = 'y';    <* 
       *> else if (strncmp(a, "@@match"       , 20) == 0) debug.matches     = 'y';    <* 
       *> else if (strncmp(a, "@@shape"       , 20) == 0) debug.shapes      = 'y';    <* 
       *> else if (strncmp(a, "@@curve"       , 20) == 0) debug.curves      = 'y';    <* 
       *> else if (strncmp(a, "@@writing"     , 20) == 0) debug.writing     = 'y';    <* 
       *> else if (strncmp(a, "@@dict"        , 20) == 0) debug.dict        = 'y';    <* 
       *> else if (strncmp(a, "@@dictg"       , 20) == 0) debug.dictg       = 'y';    <* 
       *> else if (strncmp(a, "@@dicte"       , 20) == 0) debug.dicte       = 'y';    <* 
       *> else if (strncmp(a, "@@sum"         , 20) == 0) debug.summ        = 'y';    <* 
       *> else if (strncmp(a, "@@lock"        , 20) == 0) debug.lock        = 'y';    <* 
       *> else if (a[0] >= '0' && a[0] <= '9')            out_start  = atoi(a);       <*/
      /*> else {                                                                      <* 
       *>    printf("\noption [%s] not understood\n", a);                             <* 
       *> }                                                                           <*/
   }
   /*---(complete)-----------------------*/
   DEBUG_TOPS   yLOG_exit     (__FUNCTION__);
   return 0;
}

/* PURPOSE : drive the program startup activities                             */
char
PROG_begin(void)
{
   /*---(header)-------------------------*/
   DEBUG_TOPS   yLOG_enter    (__FUNCTION__);
   /*---(complete)-----------------------*/
   DEBUG_TOPS   yLOG_exit     (__FUNCTION__);
   return 0;
}

/* PURPOSE : drive the program startup activities                             */
char
PROG_final (void)
{
   /*---(header)-------------------------*/
   DEBUG_TOPS   yLOG_enter    (__FUNCTION__);
   WORDS_table_ae ();
   if (my.dict   == 'y')  WORDS_import ();
   DRAW_init  ();
   TOUCH_init ();
   yVIKEYS_view_option (YVIKEYS_BUFFER, "file" , OUT_status, "current file name and stats");
   yVIKEYS_cmds_direct (":buffer file");
   yVIKEYS_cmds_direct (":keys hide");
   yVIKEYS_cmds_add    ('e', "dump"        , ""    , "Cs"   , POINT_clipboard      , "save point list to the clipboard");
   yVIKEYS_cmds_add    ('e', "clip"        , ""    , "Cs"   , POINT_clipboard      , "save point list to the clipboard");
   yVIKEYS_cmds_add    ('e', "gyges"       , ""    , "Cs"   , POINT_clipboard      , "save point list to the clipboard");
   yVIKEYS_cmds_add    ('f', "write"       , ""    , ""     , OUT_append           , "save outline to stroke file");
   USER_init     ();
   /*---(final program)------------------*/
   OUT_init      ();
   /*> FILE_rename   ("");                                                            <*/
   if (out_start > 0) o.curr = out_start;
   /*---(key mapping)--------------------*/
   yVIKEYS_map_config  (YVIKEYS_RIGHT, YVIKEYS_map_update, MAP_locator, MAP_addresser);
   REVERSE_report ();
   /*---(complete)-----------------------*/
   DEBUG_TOPS   yLOG_exit     (__FUNCTION__);
   return 0;
}




/*============================--------------------============================*/
/*===----                          event loop                          ----===*/
/*============================--------------------============================*/
void o___EVENT__________________o (void) {;}

/*> char                                                                                        <* 
 *> PROG_event()                                                                                <* 
 *> {                                                                                           <* 
 *>    /+---(local variables)------------------+/                                               <* 
 *>    char  input_type = '-';                                                                  <* 
 *>    /+> out_read  (o.curr);                                                            <+/   <* 
 *>    int    i = o.curr;                                                                       <* 
 *>    int       j         = 0;                                                                 <* 
 *>    tTSPEC      x_dur;                                                                       <* 
 *>    /+---(for keypresses)-------------------+/                                               <* 
 *>    XKeyEvent *key_event;                                                                    <* 
 *>    char       the_key[5];                                                                   <* 
 *>    char       x_ch     = ' ';                                                               <* 
 *>    uchar      x_key    = ' ';                                                               <* 
 *>    int        the_bytes;                                                                    <* 
 *>    char       rc;                                                                           <* 
 *>    int        x_loop  = 0;                                                                  <* 
 *>    /+---(for timer)------------------------+/                                               <* 
 *>    /+> struct     timespec timer, remain;                                             <+/   <* 
 *>    x_dur.tv_sec    = 0;                                                                     <* 
 *>    x_dur.tv_nsec   = 0.5 * 1000000;                                                         <* 
 *>    /+---(header)-------------------------+/                                                 <* 
 *>    DEBUG_TOPS   yLOG_enter    (__FUNCTION__);                                               <* 
 *>    while (1) {                                                                              <* 
 *>       x_key = 0;                                                                            <* 
 *>       if (XPending(DISP)) {                                                                 <* 
 *>          XNextEvent(DISP, &EVNT);                                                           <* 
 *>          ++j;                                                                               <* 
 *>          switch(EVNT.type) {                                                                <* 
 *>          case KeyPress:                                                                     <* 
 *>             DEBUG_LOOP  printf("event (%5d) keypress\n", j);                                <* 
 *>             i = o.curr;                                                                     <* 
 *>             /+---(get the key)---------------------+/                                       <* 
 *>             key_event  = (XKeyEvent *) &EVNT;                                               <* 
 *>             the_bytes = XLookupString((XKeyEvent *) &EVNT, the_key, 5, NULL, NULL);         <* 
 *>             if (the_bytes < 1) break;                                                       <* 
 *>             x_key = the_key [0];                                                            <* 
 *>             break;                                                                          <* 
 *>          }                                                                                  <* 
 *>       }                                                                                     <* 
 *>       x_key = yVIKEYS_main_input  (RUN_USER, x_key);                                        <* 
 *>       yVIKEYS_main_handle (x_key);                                                          <* 
 *>       if (yVIKEYS_quit ())  break;                                                          <* 
 *>       ++x_loop;                                                                             <* 
 *>       TOUCH_read ();                                                                        <* 
 *>       if ((x_loop % 20) == 0)  yVIKEYS_view_all (0.0);                                      <* 
 *>       /+---(sleeping)--------------------+/                                                 <* 
 *>       nanosleep    (&x_dur, NULL);                                                          <* 
 *>    }                                                                                        <* 
 *>    /+---(complete)------------------------------+/                                          <* 
 *>    DEBUG_TOPS   yLOG_exit     (__FUNCTION__);                                               <* 
 *>    return 1;                                                                                <* 
 *> }                                                                                           <*/

char
PROG_finish          (void)
{
   TOUCH_wrap   ();
   DRAW_wrap    ();
   yVIKEYS_wrap ();
   return 0;
}

char
PROG_end             (void)
{
   yLOGS_end ();
   return 0;
}


/*============================--------------------============================*/
/*===----                         unit testing                         ----===*/
/*============================--------------------============================*/
void o___UNITTEST_______________o (void) {;}

char         /*-> set up programgents/debugging ------[ light  [uz.320.011.05]*/ /*-[00.0000.00#.#]-*/ /*-[--.---.---.--]-*/
PROG__unit_quiet    (void)
{
   char       *x_args [1]  = { "gregg" };
   yURG_logger (1, x_args);
   PROG_init   ();
   yURG_urgs   (1, x_args);
   PROG_args   (1, x_args);
   PROG_begin  ();
   my.dict = '-';
   PROG_final  ();
   return 0;
}

char         /*-> set up programgents/debugging ------[ light  [uz.320.011.05]*/ /*-[00.0000.00#.!]-*/ /*-[--.---.---.--]-*/
PROG__unit_loud     (void)
{
   char       *x_args [2]  = { "gregg_unit", "@@kitchen"    };
   yURG_logger (2, x_args);
   PROG_init   ();
   yURG_urgs   (2, x_args);
   yURG_name  ("raw"          , YURG_ON);
   yURG_name  ("avg"          , YURG_ON);
   yURG_name  ("calc"         , YURG_ON);
   PROG_args   (2, x_args);
   PROG_begin  ();
   my.dict = '-';
   PROG_final  ();
   return 0;
}

char         /*-> set up program urgents/debugging ---[ light  [uz.210.001.01]*/ /*-[00.0000.00#.!]-*/ /*-[--.---.---.--]-*/
PROG__unit_end      (void)
{
   PROG_end       ();
   return 0;
}



/*============================----(source-end)----============================*/
