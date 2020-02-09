/*=================================(beg-code)=================================*/
#include    "gregg.h"



/*====================------------------------------------====================*/
/*===----                     mapping for map mode                     ----===*/
/*====================------------------------------------====================*/
static void  o___MAPPING_________o () { return; }

char         /*-> clear current position -------------[ ------ [fe.G67.55#.92]*/
YVIKEYS_map_init        (tMAPPED *a_map)
{
   /*---(clear)--------------------------*/
   a_map->ubeg = a_map->ucur = a_map->uend  = a_map->ulen  = a_map->utend = 0;
   a_map->gbeg = a_map->gcur = a_map->gend = 0;
   /*---(complete)-----------------------*/
   return 0;
}

char         /*-> clear map entries ------------------[ ------ [fe.G67.55#.92]*/
YVIKEYS_map_clear       (tMAPPED *a_map)
{
   /*---(locals)-----------+-----+-----+-*/
   char        rce         =  -10;
   int         i           =    0;
   /*---(map table)----------------------*/
   for (i = 0; i <  LEN_HUGE; ++i)  a_map->map  [i] =  YVIKEYS_EMPTY;
   /*---(grid values)--------------------*/
   a_map->gmin = a_map->gamin = a_map->glmin = a_map->gprev = -1;
   a_map->gmax = a_map->gamax = a_map->glmax = a_map->gnext = -1;
   /*---(used table)---------------------*/
   for (i = 0; i <  LEN_HUGE; ++i)  a_map->used [i] = '\0';
   /*---(complete)-----------------------*/
   return 0;
}

char         /*-> handle map updates -----------------[ ------ [fe.G67.55#.92]*/
YVIKEYS_map_update      (char a_req)
{
   /*---(locals)-----------+-----+-----+-*/
   char        rc          =    0;
   /*---(header)-------------------------*/
   DEBUG_MAP    yLOG_enter   (__FUNCTION__);
   /*---(initialize)---------------------*/
   if (a_req == YVIKEYS_INIT) {
      YVIKEYS_map_init (&g_bmap);  YVIKEYS_map_clear (&g_bmap);
      YVIKEYS_map_init (&g_xmap);
      YVIKEYS_map_init (&g_ymap);
      YVIKEYS_map_init (&g_zmap);  YVIKEYS_map_clear (&g_zmap);
   }
   /*---(update points/x)----------------*/
   if (rc == 0)  rc = YVIKEYS_map_clear (&g_xmap);
   if (rc == 0)  rc = BASE_map_update    (&g_xmap, o.curr, g_ymap.gcur);
   /*> if (rc == 0)  rc = NODE_map_used     ( x_map, x_node,  x_max,  x_curr, a_type);   <*/
   /*> if (rc == 0)  rc = NODE_map_absolute ( x_map,  x_tab,  x_max, a_type);         <*/
   /*> if (rc == 0)  rc = NODE_map_local    ( x_map,  x_max, x_curr, x_node, a_type);   <*/
   /*> if (rc == 0)  rc = NODE_map_ends     ( x_map->used, x_curr, &(x_map->gprev), &(x_map->gnext));   <*/
   /*---(update outlines/y)--------------*/
   if (rc == 0)  rc = YVIKEYS_map_clear (&g_ymap);
   /*---(complete)-----------------------*/
   DEBUG_MAP    yLOG_exit    (__FUNCTION__);
   return rc;
}




/*====================------------------------------------====================*/
/*===----                     mapping for map mode                     ----===*/
/*====================------------------------------------====================*/
static void  o___MAPPER__________o () { return; }

char         /*-> when changing outline --------------[ ------ [fe.G67.55#.92]*/ /*-[01.0000.018.!]-*/ /*-[--.---.---.--]-*/
MAP__xmajor           (void)
{
   /*> /+---(locals)-----------+-----------+-+/                                       <* 
    *> int         i           =    0;                                                <* 
    *> /+---(update x)-----------------------+/                                       <* 
    *> for (i = 0; i < o.navg; ++i) {                                                 <* 
    *>    g_xmap.map [i] = i;                                                         <* 
    *> }                                                                              <* 
    *> /+---(globals)------------------------+/                                       <* 
    *> g_xmap.gmin  = g_xmap.gamin  = 0;                                              <* 
    *> g_xmap.gmax  = g_xmap.gamax  = o.navg;                                         <* 
    *> g_xmap.avail = o.navg;                                                         <* 
    *> /+---(screen)-------------------------+/                                       <* 
    *> g_xmap.beg   = g_xmap.cur   = 0;                                               <* 
    *> g_xmap.end   = g_xmap.len   = g_xmap.tend  = o.navg;                           <* 
    *> /+---(units)--------------------------+/                                       <* 
    *> g_xmap.gbeg  = g_xmap.gcur  = 0;                                               <* 
    *> g_xmap.gend  = o.navg;                                                         <* 
    *> /+---(complete)-----------------------+/                                       <* 
    *> return 0;                                                                      <*/
}

char         /*-> when moving between points ---------[ ------ [fe.G67.55#.92]*/ /*-[01.0000.018.!]-*/ /*-[--.---.---.--]-*/
MAP__xminor           (void)
{
   /*---(local movements)----------------*/
   /*> g_xmap.glmin = g_xmap.gprev = 0;                                               <* 
    *> g_xmap.glmax = g_xmap.gnext = o.navg;                                          <*/
   /*---(screen)-------------------------*/
   return 0;
}

char         /*-> when changing files ----------------[ ------ [fe.G67.55#.92]*/ /*-[01.0000.018.!]-*/ /*-[--.---.---.--]-*/
MAP__ymajor           (void)
{
   /*---(locals)-----------+-----------+-*/
   /*> int         i           =    0;                                                           <* 
    *> /+---(update y)-----------------------+/                                                  <* 
    *> DEBUG_INPT   yLOG_senter  (__FUNCTION__);                                                 <* 
    *> DEBUG_INPT   yLOG_sint    (o.total);                                                      <* 
    *> for (i = 0; i < o.total; ++i) {                                                           <* 
    *>    DEBUG_INPT   yLOG_sint    (i);                                                         <* 
    *>    g_ymap.map [i] = i;                                                                    <* 
    *> }                                                                                         <* 
    *> /+---(globals)------------------------+/                                                  <* 
    *> g_ymap.gmin  = g_ymap.gamin  = 0;                                                         <* 
    *> /+> g_ymap.gmax  = g_ymap.gamax  = o.total - 1;                                     <*    <* 
    *>  *> g_ymap.avail = o.total - 1;                                                    <+/    <* 
    *> g_ymap.gmax  = g_ymap.gamax  = o.total;                                                   <* 
    *> g_ymap.avail = o.total;                                                                   <* 
    *> DEBUG_INPT   yLOG_sint    (g_ymap.gmax);                                                  <* 
    *> /+---(screen)-------------------------+/                                                  <* 
    *> g_ymap.beg   = g_ymap.cur   = g_ymap.end   = g_ymap.len   = g_ymap.tend  = o.total - 1;   <* 
    *> /+---(units)--------------------------+/                                                  <* 
    *> g_ymap.gbeg  = g_ymap.gcur  = g_ymap.gend  = o.total - 1;                                 <* 
    *> /+---(complete)-----------------------+/                                                  <* 
    *> DEBUG_INPT   yLOG_sexit   (__FUNCTION__);                                                 <* 
    *> return 0;                                                                                 <*/
}

char         /*-> when moving between outlines -------[ ------ [fe.G67.55#.92]*/ /*-[01.0000.018.!]-*/ /*-[--.---.---.--]-*/
MAP__yminor           (void)
{
   /*> /+---(update y)-----------------------+/                                       <* 
    *> DEBUG_INPT   yLOG_senter  (__FUNCTION__);                                      <* 
    *> DEBUG_INPT   yLOG_sint    (o.total);                                           <* 
    *> /+---(local movements)----------------+/                                       <* 
    *> g_ymap.glmin = g_ymap.gprev = 0;                                               <* 
    *> g_ymap.glmax = g_ymap.gnext = o.total;                                         <* 
    *> /+---(complete)-----------------------+/                                       <* 
    *> DEBUG_INPT   yLOG_sexit   (__FUNCTION__);                                      <* 
    *> return 0;                                                                      <*/
}

char         /*-> mapping coordinator ----------------[ ------ [fe.G67.55#.92]*/ /*-[01.0000.018.!]-*/ /*-[--.---.---.--]-*/
MAP_mapper           (char a_req)
{
   /*> /+---(locals)-----------+-----------+-+/                                                 <* 
    *> static int  x           = -1;                                                            <* 
    *> static int  y           = -1;                                                            <* 
    *> char        x_label     [LEN_LABEL];                                                     <* 
    *> char        x_source    [LEN_RECD ];                                                     <* 
    *> /+---(initialize)---------------------+/                                                 <* 
    *> DEBUG_INPT   yLOG_enter   (__FUNCTION__);                                                <* 
    *> DEBUG_INPT   yLOG_value   ("ymap.gcur" , g_ymap.gcur);                                   <* 
    *> DEBUG_INPT   yLOG_value   ("xmap.gcur" , g_xmap.gcur);                                   <* 
    *> /+> printf ("xmap.which %c, ymap.which %c\n", g_xmap.which, g_ymap.which);         <+/   <* 
    *> /+> printf ("called mapper with %c\n", a_req);                                     <+/   <* 
    *> if (a_req == YVIKEYS_INIT) {                                                             <* 
    *>    DEBUG_INPT   yLOG_note    ("initial run");                                            <* 
    *>    o.curr  = o.total - 1;                                                                <* 
    *>    OUT_pick (o.curr);                                                                    <* 
    *>    MAP__ymajor ();                                                                       <* 
    *>    MAP__yminor ();                                                                       <* 
    *>    MAP__xmajor ();                                                                       <* 
    *>    MAP__xminor ();                                                                       <* 
    *> }                                                                                        <* 
    *> /+---(initialize)---------------------+/                                                 <* 
    *> else {                                                                                   <* 
    *>    if (o.curr != g_ymap.gcur) {                                                          <* 
    *>       DEBUG_INPT   yLOG_note    ("change outline");                                      <* 
    *>       o.curr  = g_ymap.gcur;                                                             <* 
    *>       OUT_pick (o.curr);                                                                 <* 
    *>       MAP__yminor ();                                                                    <* 
    *>       MAP__xmajor ();                                                                    <* 
    *>       MAP__xminor ();                                                                    <* 
    *>    }                                                                                     <* 
    *>    else if (o.cavg != g_xmap.gcur) {                                                     <* 
    *>       DEBUG_INPT   yLOG_note    ("change point");                                        <* 
    *>       MAP__xminor ();                                                                    <* 
    *>       o.curr  = g_ymap.gcur;                                                             <* 
    *>    }                                                                                     <* 
    *> }                                                                                        <* 
    *> o.cavg  = g_xmap.gcur;                                                                   <* 
    *> DEBUG_INPT   yLOG_value   ("o.curr"    , o.curr);                                        <* 
    *> DEBUG_INPT   yLOG_value   ("o.total"   , o.total);                                       <* 
    *> /+---(feed source values)-------------+/                                                 <* 
    *> sprintf (x_label , "%03d.%03d", o.curr, o.cavg);                                         <* 
    *> sprintf (x_source, "%03dx, %03dy", o.avg [o.cavg].x_raw, o.avg [o.cavg].y_raw);        <* 
    *> yVIKEYS_source (x_label, x_source);                                                      <* 
    *> /+---(complete)-----------------------+/                                                 <* 
    *> DEBUG_INPT   yLOG_exit    (__FUNCTION__);                                                <* 
    *> return 0;                                                                                <*/
}

char         /*-> return coordinates for address -----[ ------ [gc.722.112.13]*/ /*-[01.0000.304.#]-*/ /*-[--.---.---.--]-*/
MAP_locator        (char *a_label, int *a_x, int *a_y, int *a_z)
{
   if (a_x != NULL)  *a_x = 0;
   if (a_y != NULL)  *a_y = 0;
   if (a_z != NULL)  *a_z = 0;
   return -1;
}

char         /*-> return address for coordinates -----[ ------ [gc.722.112.13]*/ /*-[01.0000.304.#]-*/ /*-[--.---.---.--]-*/
MAP_addresser      (char *a_label, int a_x, int a_y, int a_z)
{
   if (a_label != NULL)  strlcpy (a_label, "", LEN_LABEL);
   return -1;
}



/*====================------------------------------------====================*/
/*===----                        older stuff                           ----===*/
/*====================------------------------------------====================*/
static void  o___OTHER___________o () { return; }


char
USER_words           (char *a_words)
{
   strlcpy (my.words, a_words, LEN_DESC);
   return 0;
}

char
USER_load            (char *a_words)
{
   DEBUG_USER   yLOG_enter   (__FUNCTION__);
   my.time_lapse = '-';
   my.time_point = 9999;
   REVERSE_english_text (a_words, SHAPE_LOAD, 1, 'y');
   REVERSE_out_load     ();
   o.cavg = 0;
   DEBUG_USER   yLOG_exit    (__FUNCTION__);
   return 0;
}

char
USER_timelapse       (char *a_words)
{
   DEBUG_USER   yLOG_enter   (__FUNCTION__);
   my.time_lapse = 'y';
   my.time_point =   0;
   REVERSE_english_text (a_words, SHAPE_LOAD, 1, 'y');
   REVERSE_out_load     ();
   DEBUG_USER   yLOG_exit    (__FUNCTION__);
}

char
USER_unload           (void)
{
   DEBUG_USER   yLOG_enter   (__FUNCTION__);
   OUT_clear ();                  
   DEBUG_USER   yLOG_exit    (__FUNCTION__);
   return 0;
}

char
USER_zoom             (float a_zoom)
{
   DEBUG_USER   yLOG_enter   (__FUNCTION__);
   if      (a_zoom < 1.0)  my.zoom = 1.0;
   else if (a_zoom > 7.0)  my.zoom = 7.0;
   else                    my.zoom = a_zoom;
   DEBUG_USER   yLOG_exit    (__FUNCTION__);
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
   /*> yVIKEYS_mode_change (MODE_MAP, ":9" , "horz=0HhlL$");                          <*/
   /*---(file)---------------------------*/
   rc = yVIKEYS_cmds_add ('f', "file"        , "f"   , "s"    , FILE_rename          , "change the current spreadsheet file name"                    );
   rc = yVIKEYS_cmds_add ('f', "write"       , "w"   , ""     , OUT_append           , "write the current spreadsheet to file"                       );
   rc = yVIKEYS_cmds_add ('f', "quit"        , "q"   , ""     , USER_quit            , "quit current file (if no changes), exit if the only file"    );
   rc = yVIKEYS_cmds_add ('f', "quitall"     , "qa"  , ""     , USER_quit            , "quit all files (if no changes), and exit"                    );
   rc = yVIKEYS_cmds_add ('f', "writequit"   , "wq"  , ""     , USER_writequit       , ""                                                            );
   rc = yVIKEYS_cmds_add ('f', "writequitall", "wqa" , ""     , USER_writequit       , ""                                                            );
   rc = yVIKEYS_cmds_add ('f', "word"        , ""    , "a"    , USER_words           , ""                                                            );
   rc = yVIKEYS_cmds_add ('f', "guide"       , ""    , "s"    , USER_guide           , ""                                                            );
   rc = yVIKEYS_cmds_add ('f', "load"        , ""    , "a"    , USER_load            , ""                                                            );
   rc = yVIKEYS_cmds_add ('f', "unload"      , ""    , "a"    , USER_unload          , ""                                                            );
   rc = yVIKEYS_cmds_add ('f', "timelapse"   , ""    , "a"    , USER_timelapse       , ""                                                            );
   rc = yVIKEYS_cmds_add ('f', "zoom"        , ""    , "f"    , USER_zoom            , ""                                                            );
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
