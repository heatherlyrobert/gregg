/*============================---(source-start)---============================*/
#include    "gregg.h"


/*
 * metis § dc8·· § work from dictionary database for normal runs                          § N2O4ti §  · §
 *
 *
 *
 */


tMY  my;
char      g_print     [LEN_RECD];


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

char
PROG_reset_yjobs        (void)
{
   yJOBS_reset  (&(my.run_as), &(my.run_mode), &(my.run_file));
   my.run_as   = IAM_GREGG;
   my.run_mode = ACT_NONE;
   strcpy (my.run_file, "");
   return 0;
}



/*====================------------------------------------====================*/
/*===----                       pre-initialization                     ----===*/
/*====================------------------------------------====================*/
static void      o___PREINIT_________________o (void) {;}

char       /*----: very first setup ------------------s-----------------------*/
PROG__header            (void)
{
   /*---(header)----------------------*/
   DEBUG_PROG   yLOG_enter (__FUNCTION__);
   /*---(versioning)------------------*/
   DEBUG_PROG   yLOG_info     ("gregg"   , PROG_version      ());
   DEBUG_PROG   yLOG_info     ("purpose" , P_PURPOSE);
   DEBUG_PROG   yLOG_info     ("namesake", P_NAMESAKE);
   DEBUG_PROG   yLOG_info     ("heritage", P_HERITAGE);
   DEBUG_PROG   yLOG_info     ("imagery" , P_IMAGERY);
   DEBUG_PROG   yLOG_note     ("custom core");
   DEBUG_PROG   yLOG_info     ("yURG"    , yURG_version      ());
   DEBUG_PROG   yLOG_info     ("yLOG"    , yLOGS_version     ());
   DEBUG_PROG   yLOG_info     ("ySTR"    , ySTR_version      ());
   DEBUG_PROG   yLOG_note     ("yvikeys foundation");
   DEBUG_PROG   yLOG_info     ("yMODE"   , yMODE_version     ());
   DEBUG_PROG   yLOG_info     ("yKEYS"   , yKEYS_version     ());
   DEBUG_PROG   yLOG_info     ("yFILE"   , yFILE_version     ());
   DEBUG_PROG   yLOG_info     ("yVIEW"   , yVIEW_version     ());
   DEBUG_PROG   yLOG_note     ("yvikeys major");
   DEBUG_PROG   yLOG_info     ("yMAP"    , yMAP_version      ());
   DEBUG_PROG   yLOG_info     ("yCMD"    , yCMD_version      ());
   DEBUG_PROG   yLOG_info     ("yMACRO"  , yMACRO_version    ());
   DEBUG_PROG   yLOG_info     ("ySRC"    , ySRC_version      ());
   DEBUG_PROG   yLOG_info     ("yMARK"   , yMARK_version     ());
   DEBUG_PROG   yLOG_note     ("custom opengl");
   DEBUG_PROG   yLOG_info     ("yVIOP"   , yVIOPENGL_version ());
   DEBUG_PROG   yLOG_info     ("yX11"    , yX11_version      ());
   DEBUG_PROG   yLOG_info     ("yFONT"   , yFONT_version     ());
   DEBUG_PROG   yLOG_info     ("yCOLOR"  , yCOLOR_version    ());
   DEBUG_PROG   yLOG_info     ("yGLTEX"  , yGLTEX_version    ());
   DEBUG_PROG   yLOG_note     ("custom other");
   DEBUG_PROG   yLOG_info     ("yPARSE"  , yPARSE_version    ());
   DEBUG_PROG   yLOG_info     ("ySORT"   , ySORT_version     ());
   DEBUG_PROG   yLOG_info     ("yJOBS"   , yJOBS_version     ());
   /*---(complete)-----------------------*/
   DEBUG_PROG   yLOG_exit  (__FUNCTION__);
   return 0;
}

char
PROG_urgents            (int a_argc, char *a_argv [])
{
   /*---(locals)-----------+-----+-----+-*/
   char        rce         =  -10;
   char        rc          =    0;
   /*---(set mute)-----------------------*/
   yURG_all_mute ();
   /*---(start logger)-------------------*/
   rc = yURG_logger  (a_argc, a_argv);
   DEBUG_PROG   yLOG_value    ("logger"    , rc);
   --rce;  if (rc < 0) {
      DEBUG_PROG   yLOG_exitr    (__FUNCTION__, rce);
      return rce;
   }
   /*---(process urgents)----------------*/
   rc = yURG_urgs    (a_argc, a_argv);
   DEBUG_PROG   yLOG_value    ("logger"    , rc);
   --rce;  if (rc < 0) {
      DEBUG_PROG   yLOG_exitr    (__FUNCTION__, rce);
      return rce;
   }
   /*---(process urgents)----------------*/
   rc = PROG__header ();
   DEBUG_PROG   yLOG_value    ("header"    , rc);
   --rce;  if (rc < 0) {
      DEBUG_PROG   yLOG_exitr    (__FUNCTION__, rce);
      return rce;
   }
   /*---(complete)-----------------------*/
   return rc;
}



/*====================------------------------------------====================*/
/*===----                        program startup                       ----===*/
/*====================------------------------------------====================*/
static void      o___STARTUP_________________o (void) {;}

char       /*----: very first setup ------------------s-----------------------*/
PROG__init              (int a_argc, char *a_argv[])
{
   /*---(locals)-----------+-----+-----+-*/
   char        rce         =  -10;
   char        rc          =    0;
   /*---(header)----------------------*/
   DEBUG_PROG   yLOG_enter (__FUNCTION__);
   /*---(fonts)-----------------------*/
   /*> strcpy (win.face_pretty, "comfortaa");                                         <*/
   strcpy (win.face_pretty, "shrike");
   /*> strcpy (win.face_fixed, "hack");                                               <*/
   strcpy (win.face_fixed, "shrike");
   /*---(yJOB config)--------------------*/
   PROG_reset_yjobs ();
   my.run_uid     = getuid ();
   my.run_pid     = getpid ();
   my.run_time    = time (NULL);
   yEXEC_heartbeat (my.run_pid, my.run_time, NULL, NULL, my.heartbeat);
   DEBUG_PROG  yLOG_char    ("run_as"    , my.run_as);
   /*---(reporting flags)-------------*/
   my.run_mode     = RUN_NORMAL;
   my.time_lapse   = '-';
   my.time_point   = 9999;
   my.zoom         =   1;
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
   my.baseonly     = '-';
   strlcpy (my.wordfile, "/var/lib/gregg/gregg_manual.dict", LEN_PATH);
   /*> my.w_wide       = 500;                                                         <* 
    *> my.w_tall       = 350;                                                         <* 
    *> strlcpy (my.w_title, "gregg shorthand interpreter", LEN_HUND);                 <*/
   /*---(other)-----------------------*/
   strlcpy (my.words, "", LEN_RECD);
   strlcpy (my.guide, "", LEN_RECD);
   /*---(get letters/sizing ready)----*/
   WORDS_init      ();
   DICT_init       ();
   my.w_ppage   = 120;
   my.w_npage   = 0;
   my.w_cpage   = 0;
   /*---(get letters/sizing ready)----*/
   TABLE_init      ();
   PAGE_init       ();
   /*---(prep recognizer)-------------*/
   BASE_config     (10.0, 2.0, 70.0);
   /*---(yvicurses config)---------------*/
   rc = yVIOPENGL_init   (my.w_title, P_VERNUM, MODE_MAP, my.w_wide, my.w_tall);
   DEBUG_PROG   yLOG_value    ("yVIOPENGL" , rc);
   --rce;  if (rc < 0) {
      DEBUG_PROG   yLOG_exitr    (__FUNCTION__, rce);
      return rce;
   }
   /*> strlcpy (my.t_text     , "arachne, spider robot visualization and simulation"       , LEN_STR  );   <*/
   /*---(setup main)---------------------*/
   rc = yVIEW_full    (YVIEW_MAIN , YVIEW_FLAT, YVIEW_BOTLEF, YCOLOR_BAS, YCOLOR_MED, DRAW_primary);
   DEBUG_PROG   yLOG_value    ("yVIEW_full", rc);
   --rce;  if (rc < 0) {
      DEBUG_PROG   yLOG_exitr    (__FUNCTION__, rce);
      return rce;
   }
   /*---(library config)-----------------*/
   rc = yMAP_config       (YMAP_OFFICE, api_ymap_locator, api_ymap_addressor, api_ymap_sizer, api_ymap_entry, api_ymap_placer, api_ymap_done);
   DEBUG_PROG   yLOG_value    ("yMAP"      , rc);
   --rce;  if (rc < 0) {
      DEBUG_PROG   yLOG_exitr    (__FUNCTION__, rce);
      return rce;
   }
   /*---(yJOBS config)-------------------*/
   rc = yJOBS_runas (a_argv [0], &(my.run_as), P_HEADERS, NULL);
   DEBUG_PROG  yLOG_char    ("run_as"    , my.run_as);
   /*---(complete)-----------------------*/
   DEBUG_PROG   yLOG_exit  (__FUNCTION__);
   return 0;
}

#define  TWOARG  if (++i >= a_argc)  yURG_err (YURG_FATAL, "%s argument requires an additional string", a, --rc); else 

char
PROG__args              (int a_argc, char *a_argv[])
{
   /*---(locals)-----------+-----+-----+-*/
   char        rce         =  -10;
   char        rc          =    0;
   int         i           =    0;
   char       *a           = NULL;
   char       *b           = NULL;
   int         l           =    0;
   int         x_total     =    0;
   int         x_args      =    0;
   uchar       x_name      [LEN_FULL]  = "";
   uchar       t           [LEN_FULL]  = "";
   /*---(header)-------------------------*/
   DEBUG_ARGS  yLOG_enter   (__FUNCTION__);
   /*---(process)------------------------*/
   for (i = 1; i < a_argc; ++i) {
      /*---(set up args)-----------------*/
      a = a_argv [i];
      if (a == NULL) {
         yURG_err ('f', "arg %d is NULL", i);
         DEBUG_PROG   yLOG_note    ("FATAL, found a null argument, really bad news");
         DEBUG_PROG   yLOG_exitr   (__FUNCTION__, rce);
         return rce;
      }
      ++x_total;
      /*---(screen urgents)--------------*/
      if (a [0] == '@')  continue;
      /*---(prepare)---------------------*/
      DEBUG_ARGS  yLOG_info    ("cli arg", a);
      ++x_args;
      if (i < a_argc - 1)  b = a_argv [i + 1];
      else                 b = NULL;
      DEBUG_ARGS  yLOG_info    ("two arg", b);
      l = strlen(a);
      /*---(yKEYS arguments)------------*/
      rc = yKEYS_arg_handle (&i, a, b);
      DEBUG_ARGS  yLOG_value   ("ykeys"     , rc);
      if (rc == 1) {
         DEBUG_ARGS  yLOG_note    ("handled by yKEYS_arg_handle");
         DEBUG_ARGS  yLOG_value   ("bumped i"  , i);
         continue;
      }
      if (rc < 0) {
         DEBUG_ARGS  yLOG_note    ("yKEYS_arg_handle found trouble");
         DEBUG_PROG   yLOG_exitr   (__FUNCTION__, rce);
         return rce;
      }
      /*---(yJOBS arguments)-------------*/
      ++x_args;
      DEBUG_ARGS  yLOG_info     ("argument"  , a);
      rc = yJOBS_argument (&i, a, b, &(my.run_as), &(my.run_mode), my.run_file);
      DEBUG_ARGS  yLOG_value    ("rc"        , rc);
      if (rc >= 1) {
         DEBUG_ARGS  yLOG_note    ("handled by yJOBS_argument");
         DEBUG_ARGS  yLOG_value   ("bumped i"  , i);
         continue;
      }
      if (rc < 0) {
         DEBUG_ARGS  yLOG_note    ("yJOBS_argument found trouble");
         DEBUG_PROG   yLOG_exitr   (__FUNCTION__, rce);
         return rce;
      }
      /*---(local)-----------------------*/
      DEBUG_ARGS  yLOG_note    ("check for local argument handling");
      --rce;
      rc = 0;
      ++x_args;
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
      else if (strcmp (a, "--page"              ) == 0)   DRAW_resize (LAYOUT_PAGEVIEW);
      else if (strcmp (a, "--dictionary"        ) == 0)   DRAW_resize (LAYOUT_DICTIONARY);
      else if (strcmp (a, "--connect"           ) == 0)   DRAW_resize (LAYOUT_CONNECT);
      else if (strcmp (a, "--baseonly"          ) == 0)   my.baseonly = 'y';
      else if (strcmp (a, "--wordfile"          ) == 0)   TWOARG  { strncpy (my.wordfile, b, LEN_PATH); }
      /*---(catch two-arg errors)--------*/
      if (rc < 0)  {
         DEBUG_PROG  yLOG_note  ("two arg test failed");
         DEBUG_PROG  yLOG_exitr (__FUNCTION__, rce);
         return rce;
      }
      /*---(done)------------------------*/
   }
   DEBUG_ARGS  yLOG_value  ("entries"   , x_total);
   DEBUG_ARGS  yLOG_value  ("arguments" , x_args);
   if (x_args == 0) {
      DEBUG_ARGS  yLOG_note   ("no arguments identified");
   }
   /*---(complete)-----------------------*/
   DEBUG_ARGS  yLOG_exit  (__FUNCTION__);
   return 0;
}


char       /*----: drive program setup activities ----------------------------*/
PROG__begin             (void)
{
   char        rc          =    0;
   DEBUG_PROG   yLOG_enter   (__FUNCTION__);
   WORDS_table_ae ();
   DEBUG_PROG   yLOG_exit    (__FUNCTION__);
   return 0;
}

char
PROG_startup            (int a_argc, char *a_argv [])
{
   /*---(locals)-----------+-----+-----+-*/
   char        rce         =  -10;
   char        rc          =    0;
   /*---(header)----------------------*/
   yURG_stage_check (YURG_BEG);
   DEBUG_PROG  yLOG_enter   (__FUNCTION__);
   /*---(initialize)---------------------*/
   rc = PROG__init   (a_argc, a_argv);
   DEBUG_PROG   yLOG_value    ("init"      , rc);
   --rce;  if (rc < 0) {
      DEBUG_PROG   yLOG_exitr    (__FUNCTION__, rce);
      return rce;
   }
   /*---(arguments)----------------------*/
   rc = PROG__args   (a_argc, a_argv);
   DEBUG_PROG   yLOG_value    ("args"      , rc);
   --rce;  if (rc < 0) {
      DEBUG_PROG   yLOG_exitr    (__FUNCTION__, rce);
      return rce;
   }
   /*---(begin)--------------------------*/
   rc = PROG__begin  ();
   DEBUG_PROG   yLOG_value    ("begin"     , rc);
   --rce;  if (rc < 0) {
      DEBUG_PROG   yLOG_exitr    (__FUNCTION__, rce);
      return rce;
   }
   /*---(complete)-----------------------*/
   DEBUG_PROG  yLOG_exit  (__FUNCTION__);
   yURG_stage_check (YURG_MID);
   return rc;
}



/*====================------------------------------------====================*/
/*===----                        program execution                     ----===*/
/*====================------------------------------------====================*/
static void      o___EXECUTION_______________o (void) {;}

char       /*----: process the xwindows event stream -------------------------*/
PROG_dawn          (void)
{
   /*---(locals)-----------+-----+-----+-*/
   char        rce         =  -10;
   char        rc          =    0;
   /*---(header)-------------------------*/
   DEBUG_PROG   yLOG_enter   (__FUNCTION__);
   /*---(create window)------------------*/
   rc = yVIOPENGL_dawn ();
   DEBUG_PROG   yLOG_value   ("dawn"      , rc);
   --rce;  if (rc < 0) {
      DEBUG_PROG   yLOG_exitr   (__FUNCTION__, rce);
      return rce;
   }
   rc = yFILE_dump_add  ("suffix", "", "inventory of all allowed word suffixes", DICT_dump_suffix);
   rc = yFILE_dump_add  ("words" , "", "all words in dictionary"               , DICT_dump_words);
   yCMD_direct (":layout min");
   /*> yVIKEYS_view_option (YVIKEYS_BUFFER, "file" , OUT_status, "current file name and stats");   <*/
   /*> yCMDS_direct (":buffer file");                                                 <*/
   /*> yCMDS_direct (":keys hide");                                                   <*/
   /*> yVIKEYS_cmds_add    ('e', "dump"        , ""    , "Cs"   , POINT_clipboard      , "save point list to the clipboard");   <*/
   /*> yVIKEYS_cmds_add    ('e', "clip"        , ""    , "Cs"   , POINT_clipboard      , "save point list to the clipboard");   <*/
   /*> yVIKEYS_cmds_add    ('e', "gyges"       , ""    , "Cs"   , POINT_clipboard      , "save point list to the clipboard");   <*/
   /*> yVIKEYS_cmds_add    ('f', "write"       , ""    , ""     , OUT_append           , "save outline to stroke file");   <*/
   USER_init     ();
   /*---(final program)------------------*/
   /*> if (my.dict   == 'y')  DICT_import  (NAME_DICT);                               <*/
   /*> DICT_import  ("/var/lib/gregg/gregg_manual.dict");                             <*/
   DLIST_init      ();
   DICT_import  (my.wordfile);
   DEBUG_PROG   yLOG_value   ("words"     , WORDS_eng_count ());
   yMAP_refresh_full ();
   OUT_init      ();
   DRAW_init  ();
   TOUCH_init ();
   /*> FILE_rename   ("");                                                            <*/
   if (out_start > 0) o.curr = out_start;
   /*---(key mapping)--------------------*/
   /*> yVIKEYS_map_config  (YVIKEYS_RIGHT, YVIKEYS_map_update, MAP_locator, MAP_addresser);   <*/
   /*> REVERSE_report ();                                                             <*/
   /*> REVERSE_english_text ("rob"               , SHAPE_LOAD, 1, 'y');               <*/
   /*> yVIKEYS_cmds_direct (":overlay sample");                                       <*/
   DEBUG_PROG   yLOG_exit    (__FUNCTION__);
   return 0;
}

char
PROG_dusk          (void)
{
   /*---(locals)-----------+-----+-----+-*/
   char        rce         =  -10;
   char        rc          =    0;
   /*---(header)-------------------------*/
   DEBUG_PROG  yLOG_enter (__FUNCTION__);
   /*---(destroy window)-----------------*/
   rc = yVIOPENGL_dusk ();
   DEBUG_PROG   yLOG_value   ("dusk"      , rc);
   --rce;  if (rc < 0) {
      DEBUG_PROG   yLOG_exitr   (__FUNCTION__, rce);
      return rce;
   }
   /*---(complete)-----------------------*/
   DEBUG_PROG  yLOG_exit  (__FUNCTION__);
   return 0;
}



/*====================------------------------------------====================*/
/*===----                        program shutdown                      ----===*/
/*====================------------------------------------====================*/
static void      o___SHUTDOWN________________o (void) {;}

char       /*----: drive the program closure activities ----------------------*/
PROG__end               (void)
{
   /*---(locals)-----------+-----+-----+-*/
   char        rce         =  -10;
   char        rc          =    0;
   /*---(shutdown)--------------------*/
   DEBUG_PROG   yLOG_enter   (__FUNCTION__);
   /*---(destroy window)-----------------*/
   rc = yVIOPENGL_wrap ();
   DEBUG_PROG   yLOG_value   ("wrap"      , rc);
   --rce;  if (rc < 0) {
      DEBUG_PROG   yLOG_exitr   (__FUNCTION__, rce);
      return rce;
   }
   
   DRAW_wrap    ();
   /*> yXINIT_end  ();                                                                <*/
   /*---(complete)-----------------------*/
   DEBUG_PROG   yLOG_exit    (__FUNCTION__);
   return 0;
}

char             /* [------] drive the program closure activities ------------*/
PROG_shutdown           (void)
{
   /*---(stage-check)--------------------*/
   yURG_stage_check (YURG_END);
   /*---(header)-------------------------*/
   DEBUG_PROG   yLOG_enter    (__FUNCTION__);
   PROG__end ();
   /*---(complete)-----------------------*/
   DEBUG_PROG   yLOG_exit     (__FUNCTION__);
   DEBUG_PROG   yLOGS_end    ();
   return 0;
}



/*============================----(source-end)----============================*/
