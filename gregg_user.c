/*=================================(beg-code)=================================*/
#include    "gregg.h"



/*====================------------------------------------====================*/
/*===----                     mapping for map mode                     ----===*/
/*====================------------------------------------====================*/
static void  o___MAPPER__________o () { return; }

char         /*-> when changing outline --------------[ ------ [fe.G67.55#.92]*/ /*-[01.0000.018.!]-*/ /*-[--.---.---.--]-*/
MAP__xmajor           (void)
{
   /*---(locals)-----------+-----------+-*/
   int         i           =    0;
   /*---(update x)-----------------------*/
   for (i = 0; i < o.navg; ++i) {
      g_xmap.map [i] = i;
   }
   /*---(globals)------------------------*/
   g_xmap.gmin  = g_xmap.amin  = 0;
   g_xmap.gmax  = g_xmap.amax  = o.navg;
   g_xmap.avail = o.navg;
   /*---(screen)-------------------------*/
   g_xmap.beg   = g_xmap.cur   = g_xmap.end   = g_xmap.len   = g_xmap.tend  = 0;
   /*---(units)--------------------------*/
   g_xmap.gbeg  = g_xmap.gcur  = g_xmap.gend  = 0;
   /*---(complete)-----------------------*/
   return 0;
}

char         /*-> when moving between points ---------[ ------ [fe.G67.55#.92]*/ /*-[01.0000.018.!]-*/ /*-[--.---.---.--]-*/
MAP__xminor           (void)
{
   /*---(local movements)----------------*/
   g_xmap.lmin = g_xmap.prev = 0;
   g_xmap.lmax = g_xmap.next = o.navg;
   /*---(screen)-------------------------*/
   return 0;
}

char         /*-> when changing files ----------------[ ------ [fe.G67.55#.92]*/ /*-[01.0000.018.!]-*/ /*-[--.---.---.--]-*/
MAP__ymajor           (void)
{
   /*---(locals)-----------+-----------+-*/
   int         i           =    0;
   /*---(update y)-----------------------*/
   DEBUG_INPT   yLOG_senter  (__FUNCTION__);
   DEBUG_INPT   yLOG_sint    (o.total);
   for (i = 0; i < o.total; ++i) {
      DEBUG_INPT   yLOG_sint    (i);
      g_ymap.map [i] = i;
   }
   /*---(globals)------------------------*/
   g_ymap.gmin  = g_ymap.amin  = 0;
   /*> g_ymap.gmax  = g_ymap.amax  = o.total - 1;                                     <* 
    *> g_ymap.avail = o.total - 1;                                                    <*/
   g_ymap.gmax  = g_ymap.amax  = o.total;
   g_ymap.avail = o.total;
   DEBUG_INPT   yLOG_sint    (g_ymap.gmax);
   /*---(screen)-------------------------*/
   g_ymap.beg   = g_ymap.cur   = g_ymap.end   = g_ymap.len   = g_ymap.tend  = o.total - 1;
   /*---(units)--------------------------*/
   g_ymap.gbeg  = g_ymap.gcur  = g_ymap.gend  = o.total - 1;
   /*---(complete)-----------------------*/
   DEBUG_INPT   yLOG_sexit   (__FUNCTION__);
   return 0;
}

char         /*-> when moving between outlines -------[ ------ [fe.G67.55#.92]*/ /*-[01.0000.018.!]-*/ /*-[--.---.---.--]-*/
MAP__yminor           (void)
{
   /*---(update y)-----------------------*/
   DEBUG_INPT   yLOG_senter  (__FUNCTION__);
   DEBUG_INPT   yLOG_sint    (o.total);
   /*---(local movements)----------------*/
   g_ymap.lmin = g_ymap.prev = 0;
   g_ymap.lmax = g_ymap.next = o.total;
   /*---(complete)-----------------------*/
   DEBUG_INPT   yLOG_sexit   (__FUNCTION__);
   return 0;
}

char         /*-> mapping coordinator ----------------[ ------ [fe.G67.55#.92]*/ /*-[01.0000.018.!]-*/ /*-[--.---.---.--]-*/
MAP_mapper           (char a_req)
{
   /*---(locals)-----------+-----------+-*/
   static int  x           = -1;
   static int  y           = -1;
   /*---(initialize)---------------------*/
   DEBUG_INPT   yLOG_enter   (__FUNCTION__);
   DEBUG_INPT   yLOG_value   ("ymap.cur"  , g_ymap.cur);
   DEBUG_INPT   yLOG_value   ("xmap.cur"  , g_xmap.cur);
   /*> printf ("called mapper with %c\n", a_req);                                     <*/
   if (a_req == YVIKEYS_INIT) {
      DEBUG_INPT   yLOG_note    ("initial run");
      MAP__ymajor ();
      MAP__yminor ();
      MAP__xmajor ();
      MAP__xminor ();
      o.curr  = g_ymap.cur;
      o.cavg  = g_xmap.cur;
      OUT_pick (o.curr);
   }
   /*---(initialize)---------------------*/
   else {
      DEBUG_INPT   yLOG_note    ("normal run");
      if (o.curr != g_ymap.cur) {
         MAP__yminor ();
         MAP__xmajor ();
         MAP__xminor ();
         o.curr  = g_ymap.cur;
         o.cavg  = g_xmap.cur;
         OUT_pick (o.curr);
      }
      else if (o.cavg != g_xmap.cur) {
         MAP__xminor ();
         o.curr  = g_ymap.cur;
         o.cavg  = g_xmap.cur;
      }
   }
   DEBUG_INPT   yLOG_value   ("o.curr"    , o.curr);
   DEBUG_INPT   yLOG_value   ("o.total"   , o.total);
   /*---(complete)-----------------------*/
   DEBUG_INPT   yLOG_exit    (__FUNCTION__);
   return 0;
}



/*====================------------------------------------====================*/
/*===----                        older stuff                           ----===*/
/*====================------------------------------------====================*/
static void  o___OTHER___________o () { return; }


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
   OUT_append ();
   my.quit = 'y';
   return 0;
}




/*=================================(end-code)=================================*/
