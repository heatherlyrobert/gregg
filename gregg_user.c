/*=================================(beg-code)=================================*/
#include    "gregg.h"



char
USER_words           (char *a_words)
{
   strlcpy (my.words, a_words, LEN_DESC);
   DRAW_back ();
   return 0;
}

char
USER_guide           (char *a_guide)
{
   strlcpy (my.guide, a_guide, LEN_DESC);
   DRAW_back ();
   return 0;
}


char
USER_init            (void)
{
   /*---(locals)-----------+-----+-----+-*/
   int         i           =    0;
   char        rc          =    0;
   DEBUG_USER   yLOG_enter   (__FUNCTION__);
   DEBUG_USER   yLOG_note    ("basic init");
   /*---(modes)--------------------------*/
   yVIKEYS_mode_change (MODE_MAP, ":9" , "horz=0HhlL$");
   /*---(file)---------------------------*/
   rc = yVIKEYS_cmds_add ('f', "file"        , "f"   , "s"    , FILE_rename          , "change the current spreadsheet file name"                    );
   rc = yVIKEYS_cmds_add ('f', "write"       , "w"   , ""     , OUT_append           , "write the current spreadsheet to file"                       );
   rc = yVIKEYS_cmds_add ('f', "quit"        , "q"   , ""     , USER_quit            , "quit current file (if no changes), exit if the only file"    );
   rc = yVIKEYS_cmds_add ('f', "quitall"     , "qa"  , ""     , USER_quit            , "quit all files (if no changes), and exit"                    );
   rc = yVIKEYS_cmds_add ('f', "writequit"   , "wq"  , ""     , USER_writequit       , ""                                                            );
   rc = yVIKEYS_cmds_add ('f', "writequitall", "wqa" , ""     , USER_writequit       , ""                                                            );
   rc = yVIKEYS_cmds_add ('f', "word"        , ""    , "s"    , USER_words           , ""                                                            );
   rc = yVIKEYS_cmds_add ('f', "guide"       , ""    , "s"    , USER_guide           , ""                                                            );
   /*---(complete)-----------------------*/
   DEBUG_USER   yLOG_exit    (__FUNCTION__);
   return 0;
}

char
USER_quit            (void)
{
   my.quit = 'y';
   return 0;
}

char
USER_writequit       (void)
{
   my.quit = 'y';
   return 0;
}


/*====================------------------------------------====================*/
/*===----                    normal/map mode                           ----===*/
/*====================------------------------------------====================*/
static void  o___NORMAL__________o () { return; }

char         /*-> process keystrokes in normal mode --[ leaf   [ge.#X9.22#.NP]*/ /*-[05.0000.102.!]-*/ /*-[--.---.---.--]-*/
USER_map_mode        (char a_major, char a_minor)
{
   /*---(locals)-----------+-----+-----+-*/
   char        rce         =  -10;
   char        rc          =    0;
   /*---(header)-------------------------*/
   DEBUG_USER   yLOG_enter   (__FUNCTION__);
   DEBUG_USER   yLOG_char    ("a_major"   , a_major);
   DEBUG_USER   yLOG_char    ("a_minor"   , a_minor);
   /*---(defenses)-----------------------*/
   DEBUG_USER   yLOG_char    ("mode"      , yVIKEYS_mode_curr ());
   --rce;  if (yVIKEYS_mode_not (MODE_MAP    )) {
      DEBUG_USER   yLOG_note    ("not the correct mode");
      DEBUG_USER   yLOG_exit    (__FUNCTION__);
      return rce;
   }
   /*---(space)--------------------------*/
   if (a_minor == G_KEY_SPACE ) {
      DEBUG_USER   yLOG_note    ("space, nothing to do");
      DEBUG_USER   yLOG_exit    (__FUNCTION__);
      return  0;
   }
   /*---(major mode changes)-------------*/
   if (a_minor == G_KEY_RETURN) {
      DEBUG_USER   yLOG_note    ("return, nothing to do");
      DEBUG_USER   yLOG_exit    (__FUNCTION__);
      return  0;
   }
   if (a_minor == G_KEY_ESCAPE) {
      DEBUG_USER   yLOG_note    ("escape, nothing to do");
      DEBUG_USER   yLOG_exit    (__FUNCTION__);
      return  0;
   }
   /*---(single key)---------------------*/
   --rce;
   if (a_major == ' ') {
      /*---(multiplier)------------------*/
      if (strchr ("123456789"  , a_minor) != 0) {
         yVIKEYS_mode_enter  (SMOD_REPEAT);
         DEBUG_USER   yLOG_exit    (__FUNCTION__);
         return a_minor;
      }
      /*---(mode switch)-----------------*/
      switch (a_minor) {
      case ':'      :
         yVIKEYS_mode_enter  (MODE_COMMAND);
         yVIKEYS_cmds_start ();
         DEBUG_USER   yLOG_exit    (__FUNCTION__);
         return a_minor;
         break;
      }
      /*---(simple keys)--------------*/
      switch (a_minor) {
      case '0' : o.cavg  = 0;      break;
      case 'H' : o.cavg -= 5;      break;
      case 'h' : o.cavg -= 1;      break;
      case 'l' : o.cavg += 1;      break;
      case 'L' : o.cavg += 5;      break;
      case '$' : o.cavg  = 100000; break;
      }
      /*---(enforce limits)-----------*/
      if (o.cavg < 1      ) o.cavg = 1;
      if (o.cavg > o.navg ) o.cavg = o.navg;
      if (o.curr < 1      ) o.curr = 1;
      if (o.curr > o.total) o.curr = o.total;
   }
   /*---(complete)------------------------------*/
   DEBUG_USER   yLOG_exit    (__FUNCTION__);
   return 0;
}



/*=================================(end-code)=================================*/
