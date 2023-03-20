/*=================================(beg-code)=================================*/
#include "gregg.h"



int
main                    (int a_argc, char *a_argv [])
{
   /*---(locals)-----------+-----------+-*/
   char        rc          = 0;
   char        t           [LEN_HUGE]  = "";
   /*---(debugging)----------------------*/
   rc = PROG_urgents (a_argc, a_argv);
   DEBUG_PROG   yLOG_value    ("urgents"   , rc);
   if (rc <  0) { PROG_shutdown (); return -1; }
   /*---(initialization)-----------------*/
   rc = PROG_startup (a_argc, a_argv);
   DEBUG_PROG   yLOG_value    ("startup"   , rc);
   if (rc <  0) { PROG_shutdown (); return -2; }
   /*---(main-loop)----------------------*/

   /*> strlcpy  (t, ""                                      , LEN_HUGE);              <* 
    *> strlcat  (t, "o ox·r ok·g k·o·k r·ok·k "             , LEN_HUGE);              <* 
    *> strlcat  (t, "od·d t·o t·od·d "                      , LEN_HUGE);              <* 
    *> strlcat  (t, "ok·k r·ok·g m·ok·k "                   , LEN_HUGE);              <* 
    *> strlcat  (t, "k·om·m m·ok·m r·ok·b "                 , LEN_HUGE);              <* 
    *> strlcat  (t, "ok·tn oh·nt tn·ox nt·ox "              , LEN_HUGE);              <* 
    *> REVERSE_page    (t, SHAPE_DRAW, 1, 'y');                                       <*/


   switch (my.run_mode) {
   case RUN_NORMAL  :
      rc = PROG_dawn    ();
      DEBUG_PROG   yLOG_value    ("dawn"      , rc);
      if (rc <  0) { PROG_shutdown (); return -3; }
      rc = yVIOPENGL_main ("500us", "10ms", NULL);
      DEBUG_PROG   yLOG_value    ("main"      , rc);
      rc = PROG_dusk     ();
      DEBUG_PROG   yLOG_value    ("dusk"      , rc);
      break;
   case RUN_CONVERT : case RUN_EXACT   : case RUN_REVERSE :
   case RUN_ENGLISH : case RUN_GREGG   :
      rc = STDIN_handler ();
      break;
   case RUN_WORDS   :
      WORDS_dict_list ();
      break;
   }
   /*---(wrap-up)------------------------*/
   rc = PROG_shutdown ();
   /*---(complete)-----------------------*/
   return 0;
}



/*> int        /+----: process main program loop ---------------------------------+/                                                    <* 
 *> main               (int argc, char *argv[])                                                                                         <* 
 *> {                                                                                                                                   <* 
 *>    /+> /+---(locals)-----------+-----+-----+-+/                                                 <*                                  <* 
 *>     *> char        rc          =    0;                                                          <*                                  <* 
 *>     *> /+> tTSPEC      x_dur;                                                             <+/   <*                                  <* 
 *>     *> XKeyEvent  *key_event;                                                                   <*                                  <* 
 *>     *> char        the_key     [5];                                                             <*                                  <* 
 *>     *> int         the_bytes;                                                                   <*                                  <* 
 *>     *> uchar       x_key       =  ' ';                                                          <*                                  <* 
 *>     *> int         x_loop      = 0;                                                             <*                                  <* 
 *>     *> uchar       t           [LEN_HUGE]  = "";                                                <*                                  <* 
 *>     *> /+---(initialize)---------------------+/                                                 <*                                  <* 
 *>     *> if (rc >= 0)  rc = yURG_logger  (argc, argv);                                            <*                                  <* 
 *>     *> if (rc >= 0)  rc = PROG_init    ();                                                      <*                                  <* 
 *>     *> if (rc >= 0)  rc = yURG_urgs    (argc, argv);                                            <*                                  <* 
 *>     *> if (rc >= 0)  rc = PROG_args    (argc, argv);                                            <*                                  <* 
 *>     *> if (rc >= 0)  rc = PROG_begin   ();                                                      <*                                  <* 
 *>     *> if (rc >= 0)  rc = PROG_final   ();                                                      <*                                  <* 
 *>     *> if (rc <  0) {                                                                           <*                                  <* 
 *>     *>    PROG_end ();                                                                          <*                                  <* 
 *>     *>    return -1;                                                                            <*                                  <* 
 *>     *> }                                                                                        <*                                  <* 
 *>     *> /+---(header)-------------------------+/                                                 <*                                  <* 
 *>     *> DEBUG_TOPS   yLOG_enter    (__FUNCTION__);                                               <*                                  <* 
 *>     *> switch (my.run_mode) {                                                                   <*                                  <* 
 *>     *> case RUN_NORMAL  :                                                                       <*                                  <* 
 *>     *>    rc = yKEYS_main ("500us", "10ms", TOUCH_read);                                        <*                                  <* 
 *>     *>    break;                                                                                <*                                  <* 
 *>     *> case RUN_CONVERT : case RUN_EXACT   : case RUN_REVERSE :                                 <*                                  <* 
 *>     *> case RUN_ENGLISH : case RUN_GREGG   :                                                    <*                                  <* 
 *>     *>    rc = STDIN_handler ();                                                                <*                                  <* 
 *>     *>    break;                                                                                <*                                  <* 
 *>     *> case RUN_WORDS   :                                                                       <*                                  <* 
 *>     *>    WORDS_dict_list ();                                                                   <*                                  <* 
 *>     *>    break;                                                                                <*                                  <* 
 *>     *> }                                                                                        <*                                  <* 
 *>     *> DEBUG_TOPS   yLOG_exit     (__FUNCTION__);                                               <*                                  <* 
 *>     *> /+---(wrapup)-------------------------+/                                                 <*                                  <* 
 *>     *> rc = PROG_finish ();                                                                     <*                                  <* 
 *>     *> rc = PROG_end    ();                                                                     <*                                  <* 
 *>     *> /+---(complete)-----------------------+/                                                 <*                                  <* 
 *>     *> return rc;                                                                               <+/                                 <* 
 *>                                                                                                                                     <* 
 *>                                                                                                                                     <* 
 *>    /+> REVERSE_english_page ("the initiate ", SHAPE_DRAW, 1, 'y');                    <+/                                           <* 
 *>                                                                                                                                     <* 
 *>    /+> strlcpy  (t, ""                                      , LEN_HUGE);              <*                                            <* 
 *>     *> strlcat  (t, "o ox·r ok·g k·o·k r·ok·k "             , LEN_HUGE);              <*                                            <* 
 *>     *> strlcat  (t, "od·d t·o t·od·d "                      , LEN_HUGE);              <*                                            <* 
 *>     *> strlcat  (t, "ok·k r·ok·g m·ok·k "                   , LEN_HUGE);              <*                                            <* 
 *>     *> strlcat  (t, "k·om·m m·ok·m r·ok·b "                 , LEN_HUGE);              <*                                            <* 
 *>     *> strlcat  (t, "ok·tn oh·nt tn·ox nt·ox "              , LEN_HUGE);              <*                                            <* 
 *>     *> REVERSE_page    (t, SHAPE_DRAW, 1, 'y');                                       <+/                                           <* 
 *>    /+---(m)------------------------------+/                                                                                         <* 
 *>    /+> strlcpy  (t, ""                                      , LEN_HUGE);              <*                                            <* 
 *>     *> strlcat  (t, "l·g·l r·k·r "                          , LEN_HUGE);              <*                                            <* 
 *>     *> strlcat  (t, "b·v·b p·f·p "                          , LEN_HUGE);              <*                                            <* 
 *>     *> strlcat  (t, "m·amx axm·m "                          , LEN_HUGE);              <*                                            <* 
 *>     *> strlcat  (t, "n·amm·m m·em·n "                       , LEN_HUGE);              <*                                            <* 
 *>     *> strlcat  (t, "m·amd·d·adm·m m·emd·d·edm·m "          , LEN_HUGE);              <*                                            <* 
 *>     *> strlcat  (t, "m·amT·tm·amm·m m·emT·tm·em·m "         , LEN_HUGE);              <*                                            <* 
 *>     *> strlcat  (t, "m·amH·nd·aHm·m m·emH·nd·eHm·m "        , LEN_HUGE);              <*                                            <* 
 *>     *> strlcat  (t, "m·amd·d·adm·m m·emd·d·edm·m "          , LEN_HUGE);              <*                                            <* 
 *>     *> strlcat  (t, "m·amT·df·m m·emT·df·m "                , LEN_HUGE);              <*                                            <* 
 *>     *> strlcat  (t, "df·aDm·m df·eDm·m "                    , LEN_HUGE);              <*                                            <* 
 *>     *> strlcat  (t, "m·amk·g·akm·m m·emk·g·ekm·m "          , LEN_HUGE);              <*                                            <* 
 *>     *> strlcat  (t, "m·amr·l·arm·m m·emr·l·erm·m "          , LEN_HUGE);              <*                                            <* 
 *>     *> strlcat  (t, "m·amN·ng·aNm·m m·emN·ng·eNm·m "        , LEN_HUGE);              <*                                            <* 
 *>     *> strlcat  (t, "m·amj·ch·ajm·m m·emj·j·ejm·m "         , LEN_HUGE);              <*                                            <* 
 *>     *> strlcat  (t, "m·amf·v·afm·m m·emf·f·efm·m "          , LEN_HUGE);              <*                                            <* 
*>     *> strlcat  (t, "m·amp·b·apm·m m·emp·p·epm·m "          , LEN_HUGE);              <*                                            <* 
*>     *> strlcat  (t, "m·amr·ld·aRm·m m·emr·ld·eRm·m "        , LEN_HUGE);              <*                                            <* 
*>     *> /+---(d)------------------------------+/                                       <*                                            <* 
*>     *> strlcat  (t, "¦ ¦ "                                  , LEN_HUGE);              <*                                            <* 
*>     *> strlcat  (t, "d·adx axd·t d·edx exd·t "              , LEN_HUGE);              <*                                            <* 
*>     *> strlcat  (t, "t·add·d d·edd·t "                      , LEN_HUGE);              <*                                            <* 
*>     *> strlcat  (t, "d·ad·tm·amd·t d·ed·tm·emd·t "          , LEN_HUGE);              <*                                            <* 
*>     *> strlcat  (t, "d·adm·nd·aHd·t d·edm·nd·eHd·t "        , LEN_HUGE);              <*                                            <* 
*>     *> strlcat  (t, "d·adk·g·akd·t d·edk·g·ekd·t "          , LEN_HUGE);              <*                                            <* 
*>     *> strlcat  (t, "d·adr·l·ard·d d·edr·l·erd·t "          , LEN_HUGE);              <*                                            <* 
*>     *> strlcat  (t, "d·adN·nk·aNd·d d·edN·ng·eNd·t "        , LEN_HUGE);              <*                                            <* 
*>     *> strlcat  (t, "d·adj·j·ajd·d d·edj·ch·ejd·t "         , LEN_HUGE);              <*                                            <* 
*>     *> strlcat  (t, "d·adf·v·afd·d d·edf·f·efd·t "          , LEN_HUGE);              <*                                            <* 
*>     *> strlcat  (t, "d·adp·b·apd·d d·edp·p·epd·t "          , LEN_HUGE);              <*                                            <* 
*>     *> /+---(k)------------------------------+/                                       <*                                            <* 
*>     *> strlcat  (t, "¦ ¦ "                                  , LEN_HUGE);              <*                                            <* 
*>     *> strlcat  (t, "k·akx k·ekx axk·g exk·g "              , LEN_HUGE);              <*                                            <* 
*>     *> strlcat  (t, "k·akk·k k·ekk·k "                      , LEN_HUGE);              <*                                            <* 
*>     *> strlcat  (t, "k·akr·r r·ark·k k·ekr·r r·erk·k "      , LEN_HUGE);              <*                                            <* 
*>     *> strlcat  (t, "k·akT·tm tn·aTk·g "                    , LEN_HUGE);              <*                                            <* 
*>     *> strlcat  (t, "k·akH·nd nd·aHk·g "                    , LEN_HUGE);              <*                                            <* 
*>     *> strlcat  (t, "k·akN·ng ng·aNk·g "                    , LEN_HUGE);              <*                                            <* 
*>     *> strlcat  (t, "k·akj·ch j·ajk·g "                     , LEN_HUGE);              <*                                            <* 
*>     *> strlcat  (t, "k·akf·v f·afk·g "                      , LEN_HUGE);              <*                                            <* 
*>     *> strlcat  (t, "k·akp·b p·apk·g "                      , LEN_HUGE);              <*                                            <* 
*>     *> /+---(r)------------------------------+/                                       <*                                            <* 
*>     *> strlcat  (t, "¦ "                                  , LEN_HUGE);                <*                                            <* 
*>     *> strlcat  (t, "r·arx r·erx axr·l exr·l "              , LEN_HUGE);              <*                                            <* 
*>     *> strlcat  (t, "r·arr·r r·err·r "                      , LEN_HUGE);              <*                                            <* 
*>     *> strlcat  (t, "r·arT·tm tn·aTr·l "                    , LEN_HUGE);              <*                                            <* 
*>     *> strlcat  (t, "r·arH·nd nd·aHr·l "                    , LEN_HUGE);              <*                                            <* 
*>     *> strlcat  (t, "r·arN·ng ng·aNr·l "                    , LEN_HUGE);              <*                                            <* 
*>     *> strlcat  (t, "r·arj·ch j·ajr·l "                     , LEN_HUGE);              <*                                            <* 
*>     *> strlcat  (t, "r·arf·v f·afr·l "                      , LEN_HUGE);              <*                                            <* 
*>     *> strlcat  (t, "r·arp·b p·apr·l "                      , LEN_HUGE);              <*                                            <* 
*>     *> /+---(f)------------------------------+/                                       <*                                            <* 
*>     *> strlcat  (t, "¦ "                                  , LEN_HUGE);                <*                                            <* 
*>     *> strlcat  (t, "f·afx v·efx axf·v exf·f "              , LEN_HUGE);              <*                                            <* 
*>     *> strlcat  (t, "f·aff·v f·eff·v "                      , LEN_HUGE);              <*                                            <* 
*>     *> strlcat  (t, "f·afT·tm tn·aTf·v "                    , LEN_HUGE);              <*                                            <* 
*>     *> strlcat  (t, "f·afH·nd nd·aHf·v "                    , LEN_HUGE);              <*                                            <* 
*>     *> strlcat  (t, "f·afN·ng ng·aNf·v "                    , LEN_HUGE);              <*                                            <* 
*>     *> strlcat  (t, "f·afj·ch j·ajf·v "                     , LEN_HUGE);              <*                                            <* 
*>     *> strlcat  (t, "f·afp·b p·apf·v "                      , LEN_HUGE);              <*                                            <* 
*>     *> /+---(p)------------------------------+/                                       <*                                            <* 
*>     *> strlcat  (t, "¦ "                                  , LEN_HUGE);                <*                                            <* 
*>     *> strlcat  (t, "p·apx b·epx axp·b exp·p "              , LEN_HUGE);              <*                                            <* 
*>     *> strlcat  (t, "p·app·b p·epp·b "                      , LEN_HUGE);              <*                                            <* 
*>     *> strlcat  (t, "p·apT·tm tn·aTp·b "                    , LEN_HUGE);              <*                                            <* 
*>     *> strlcat  (t, "p·apH·nd nd·aHp·b "                    , LEN_HUGE);              <*                                            <* 
*>     *> strlcat  (t, "p·apN·ng ng·aNp·b "                    , LEN_HUGE);              <*                                            <* 
*>     *> strlcat  (t, "p·apj·ch j·ajp·b "                     , LEN_HUGE);              <*                                            <* 
*>     *> /+---(T)------------------------------+/                                       <*                                            <* 
*>     *> strlcat  (t, "¦ ¦ "                                , LEN_HUGE);                <*                                            <* 
*>     *> strlcat  (t, "tm·aTx tm·eTx axT·tm exT·tm "              , LEN_HUGE);          <*                                            <* 
*>     *> strlcat  (t, "th·aTT·tm th·eTT·tm "                      , LEN_HUGE);          <*                                            <* 
*>     *> strlcat  (t, "nd·aHx ht·eHx axH·nd exH·nt "              , LEN_HUGE);          <*                                            <* 
*>     *> strlcat  (t, "nd·aHH·ht ht·eHH·nd "                      , LEN_HUGE);          <+/                                           <* 
*>    /+> REVERSE_page    ("n·a·m m·e·n m·amd·d·adm·m ¦ m·emd·d·edm·m m·amj·sh·ajm·n m·emj·sh·ejm·n"    , SHAPE_DRAW, 1, 'y');   <+/   <* 
*>    /+---(draw it)------------------------+/                                                                                         <* 
*>    /+> REVERSE_page    (t, SHAPE_DRAW, 1, 'y');                                       <+/                                           <* 
*>    /+---(for timer)------------------------+/                                                                                       <* 
*>    /+> x_dur.tv_sec    = 0;                                                           <*                                            <* 
*>     *> x_dur.tv_nsec   = 0.5 * 1000000;                                               <+/                                           <* 
*>    /+> while (1) {                                                                        <*                                        <* 
   *>     *>    x_key = 0;                                                                      <*                                        <* 
      *>     *>    if (XPending(DISP)) {                                                           <*                                        <* 
         *>     *>       XNextEvent(DISP, &EVNT);                                                     <*                                        <* 
            *>     *>       switch(EVNT.type) {                                                          <*                                        <* 
               *>     *>       case KeyPress:                                                               <*                                        <* 
                  *>     *>          /+---(get the key)---------------------+/                                 <*                                        <* 
                  *>     *>          key_event  = (XKeyEvent *) &EVNT;                                         <*                                        <* 
                  *>     *>          the_bytes = XLookupString((XKeyEvent *) &EVNT, the_key, 5, NULL, NULL);   <*                                        <* 
                  *>     *>          if (the_bytes < 1) break;                                                 <*                                        <* 
                  *>     *>          x_key = the_key [0];                                                      <*                                        <* 
                  *>     *>          break;                                                                    <*                                        <* 
                  *>     *>       }                                                                            <*                                        <* 
                  *>     *>    }                                                                               <*                                        <* 
                  *>     *>    x_key = yVIKEYS_main_input  (RUN_USER, x_key);                                  <*                                        <* 
                  *>     *>    yVIKEYS_main_handle (x_key);                                                    <*                                        <* 
                  *>     *>    if (yVIKEYS_quit ())  break;                                                    <*                                        <* 
                  *>     *>    ++x_loop;                                                                       <*                                        <* 
                  *>     *>    TOUCH_read ();                                                                  <*                                        <* 
                  *>     *>    if ((x_loop % 20) == 0)  yVIKEYS_view_all (0.0);                                <*                                        <* 
                  *>     *>    /+---(sleeping)--------------------+/                                           <*                                        <* 
                  *>     *>    nanosleep    (&x_dur, NULL);                                                    <*                                        <* 
                  *>     *> }                                                                                  <+/                                       <* 
                  *>    /+---(wrapup)-------------------------+/                                                                                         <* 
                  *>    rc = PROG_finish ();                                                                                                             <* 
                  *>    rc = PROG_end    ();                                                                                                             <* 
                  *>    /+---(complete)-----------------------+/                                                                                         <* 
                  *>    return rc;                                                                                                                       <* 
                  *> }                                                                                                                                   <*/

                  /*=================================(end-code)=================================*/
