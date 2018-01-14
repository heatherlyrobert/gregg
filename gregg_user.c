/*=================================(beg-code)=================================*/
#include    "gregg.h"



/*> char    s_command    [LEN_RECD];                                                  <*/
char    s_search     [LEN_RECD];



/*> #define  MAX_CMDS      1000                                                                                                                                                        <* 
 *> typedef  struct cCOMMAND  tCOMMAND;                                                                                                                                                <* 
 *> struct  cCOMMAND {                                                                                                                                                                 <* 
 *>    char        ccat;                        /+ category                       +/                                                                                                   <* 
 *>    char        name        [LEN_LABEL];     /+ full command name              +/                                                                                                   <* 
 *>    char        len;                         /+ length of name                 +/                                                                                                   <* 
 *>    char        abbr        [LEN_LABEL];     /+ abbreviation of name           +/                                                                                                   <* 
 *>    char        alen;                        /+ length of abbreviation         +/                                                                                                   <* 
 *>    char        active;                      /+ ready to use                   +/                                                                                                   <* 
 *>    char        redraw;                      /+ redraw afterwards              +/                                                                                                   <* 
 *>    union {                                                                                                                                                                         <* 
 *>       char        (*v   ) (void);           /+ function pointer               +/                                                                                                   <* 
 *>       char        (*c   ) (char);           /+ function pointer               +/                                                                                                   <* 
 *>       char        (*s   ) (char*);          /+ function pointer               +/                                                                                                   <* 
 *>       char        (*is  ) (int  , char*);   /+ function pointer               +/                                                                                                   <* 
 *>       char        (*ss  ) (char*, char*);   /+ function pointer               +/                                                                                                   <* 
 *>    } f;                                                                                                                                                                            <* 
 *>    char        terms       [LEN_LABEL];     /+ type of terms/args             +/                                                                                                   <* 
 *>    char        nterm;                       /+ number of terms/args           +/                                                                                                   <* 
 *>    char        desc        [LEN_DESC];      /+ descriptive label              +/                                                                                                   <* 
 *>    char        disp        [LEN_DESC];      /+ display version of command     +/                                                                                                   <* 
 *> };                                                                                                                                                                                 <* 
 *> static tCOMMAND  s_cmds  [MAX_CMDS] = {                                                                                                                                            <* 
 *>    /+ 1    123456789-12   12   123    12   1    1   123456789-123456789-123456789-   12345   12   123456789-123456789-123456789-123456789-123456789-123456789-  1234 +/            <* 
 *>    /+---(file)---------------------------+/                                                                                                                                        <* 
 *>    /+cat   ---name-----  len  abbrev len  act  drw  ---pointer--------------------   terms  cnt   ---desc-----------------------------------------------------  disp +/            <* 
 *>    { 'f', "file"        ,  0, "f"   ,  0, 'y', '-', .f.s   = FILE_rename          , "s"    ,  0, "change the current spreadsheet file name"                    , "" },             <* 
 *>    /+> { 'f', "read"        ,  0, "r"   ,  0, '-', '-', NULL                          , ""     ,  0, "read the current spreadsheet from file"                      , "" },   <+/   <* 
 *>    /+> { 'f', "edit"        ,  0, "e"   ,  0, '-', '-', NULL                          , ""     ,  0, "re-read the current spreadsheet from file"                   , "" },   <+/   <* 
 *>    { 'f', "write"       ,  0, "w"   ,  0, 'y', '-', .f.v   = OUT_append           , ""     ,  0, "write the current spreadsheet to file"                       , "" },             <* 
 *>    /+> { 'f', "writeall"    ,  0, "wa"  ,  0, 'y', '-', .f.v   = FILE_write           , ""     ,  0, "write the current spreadsheet to file"                       , "" },   <+/   <* 
 *>    /+> { 'f', "writeas"     ,  0, "was" ,  0, 'y', '-', .f.s   = FILE_writeas         , "s"    ,  0, "write the current spreadsheet to a temp filename"            , "" },   <+/   <* 
 *>    { 'f', "quit"        ,  0, "q"   ,  0, 'y', '-', .f.v   = USER_quit            , ""     ,  0, "quit current file (if no changes), exit if the only file"    , "" },             <* 
 *>    { 'f', "quitall"     ,  0, "qa"  ,  0, 'y', '-', .f.v   = USER_quit            , ""     ,  0, "quit all files (if no changes), and exit"                    , "" },             <* 
 *>    { 'f', "writequit"   ,  0, "wq"  ,  0, 'y', '-', .f.v   = USER_writequit       , ""     ,  0, ""                                                            , "" },             <* 
 *>    { 'f', "writequitall",  0, "wqa" ,  0, 'y', '-', .f.v   = USER_writequit       , ""     ,  0, ""                                                            , "" },             <* 
 *>    { 'f', "word"        ,  0, ""    ,  0, 'y', '-', .f.s   = USER_words           , "a"    ,  0, ""                                                            , "" },             <* 
 *>    { 'f', "guide"       ,  0, ""    ,  0, 'y', '-', .f.s   = USER_guide           , "a"    ,  0, ""                                                            , "" },             <* 
 *>    /+---(done)---------------------------+/                                                                                                                                        <* 
 *>    { '-', "END-OF-LIST" ,  0, ""    ,  0, '-', '-', NULL                          , ""     ,  0, ""                                                            , "" },             <* 
 *> };                                                                                                                                                                                 <* 
 *> static int s_ncmd = 0;                                                                                                                                                             <*/




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
/*===----                        command line                          ----===*/
/*====================------------------------------------====================*/
PRIV void  o___COMMAND_________o () { return; }

/*> char        USER_cmds_start      (void) { strncpy     (s_command , ":", LEN_RECD); return 0; }   <*/
/*> char        USER_cmds_clear      (void) { strncpy     (s_command , "" , LEN_RECD); return 0; }   <*/
/*> char*       USER_cmds_curr       (void) { return s_command; }                     <*/

/*> char         /+-> tbd --------------------------------[ ------ [ge.#M5.1C#.#7]+/ /+-[03.0000.013.L]-+/ /+-[--.---.---.--]-+/   <* 
 *> USER_cmds_exec     (char *a_command)                                                                                           <* 
 *> {                                                                                                                              <* 
 *>    /+---(locals)-----------+-----+-----+-+/                                                                                    <* 
 *>    char        rce         =  -10;                                                                                             <* 
 *>    char        rc          =    0;                                                                                             <* 
 *>    char       *p           = NULL;                                                                                             <* 
 *>    char       *q           = " ";                                                                                              <* 
 *>    char       *r           = NULL;                                                                                             <* 
 *>    char        x_work      [LEN_RECD]   = "";                                                                                  <* 
 *>    int         x_len       = 0;                                                                                                <* 
 *>    char        x_flag      = '-';                                                                                              <* 
 *>    char        x_fields    [10][LEN_RECD];                                                                                     <* 
 *>    char        x_all       [LEN_RECD]       = "";                                                                              <* 
 *>    int         x_nfield    =  0;                                                                                               <* 
 *>    int         i           = 0;                                                                                                <* 
 *>    /+---(header)-------------------------+/                                                                                    <* 
 *>    DEBUG_USER   yLOG_enter   (__FUNCTION__);                                                                                   <* 
 *>    DEBUG_USER   yLOG_point   ("a_command" , a_command);                                                                        <* 
 *>    --rce;  if (a_command == NULL) {                                                                                            <* 
 *>       DEBUG_USER   yLOG_exitr   (__FUNCTION__, rce);                                                                           <* 
 *>       return rce;                                                                                                              <* 
 *>    }                                                                                                                           <* 
 *>    DEBUG_USER   yLOG_info    ("a_command" , a_command);                                                                        <* 
 *>    /+---(prepare)------------------------+/                                                                                    <* 
 *>    strlcpy (x_work, a_command, LEN_RECD);                                                                                      <* 
 *>    x_len = strlen (x_work);                                                                                                    <* 
 *>    for (i = 0; i < x_len; ++i) {                                                                                               <* 
 *>       if ((uchar) x_work [i] == G_CHAR_SPACE)   x_work [i] = G_KEY_SPACE;                                                      <* 
 *>    }                                                                                                                           <* 
 *>    DEBUG_USER   yLOG_info    ("x_work"    , x_work);                                                                           <* 
 *>    /+---(parse command)------------------+/                                                                                    <* 
 *>    p     = strtok_r (x_work, q, &r);                                                                                           <* 
 *>    ++p;                                                                                                                        <* 
 *>    x_len = strlen (p);                                                                                                         <* 
 *>    DEBUG_USER   yLOG_info    ("g_cmd"     , p);                                                                                <* 
 *>    if (strlen (x_work) > x_len)  strlcpy (x_all, p + x_len + 1, LEN_RECD);                                                     <* 
 *>    DEBUG_USER   yLOG_info    ("x_all"     , x_all);                                                                            <* 
 *>    /+---(system commands)----------------+/                                                                                    <* 
 *>    if (p[0] == '!') {                                                                                                          <* 
 *>       rc = system (a_command + 2);                                                                                             <* 
 *>       return rc;                                                                                                               <* 
 *>    }                                                                                                                           <* 
 *>    /+---(parse)--------------------------+/                                                                                    <* 
 *>    for (i = 0; i < 10; ++i)  strlcpy (x_fields [i], "", LEN_RECD);                                                             <* 
 *>    for (i = 0; i < 10; ++i) {                                                                                                  <* 
 *>       DEBUG_USER   yLOG_value   ("i"         , i);                                                                             <* 
 *>       DEBUG_USER   yLOG_info    ("p"         , p);                                                                             <* 
 *>       strlcpy (x_fields [i], p, LEN_RECD);                                                                                     <* 
 *>       x_nfield = i + 1;                                                                                                        <* 
 *>       p = strtok_r (NULL  , q, &r);                                                                                            <* 
 *>       if (p == NULL)  break;                                                                                                   <* 
 *>    }                                                                                                                           <* 
 *>    /+---(run)----------------------------+/                                                                                    <* 
 *>    for (i = 0; i < s_ncmd; ++i) {                                                                                              <* 
 *>       DEBUG_USER   yLOG_value   ("i"         , i);                                                                             <* 
 *>       DEBUG_USER   yLOG_info    ("name"      , s_cmds [i].name);                                                               <* 
 *>       DEBUG_USER   yLOG_info    ("abbr"      , s_cmds [i].abbr);                                                               <* 
 *>       /+---(filter)----------------------+/                                                                                    <* 
 *>       if (x_len > 3) {                                                                                                         <* 
 *>          if (s_cmds [i].len != x_len)                         continue;                                                        <* 
 *>          if (s_cmds [i].name [0] != x_fields [0][0])          continue;                                                        <* 
 *>          if (strcmp (s_cmds [i].name, x_fields [0]) != 0)     continue;                                                        <* 
 *>       } else {                                                                                                                 <* 
 *>          if (s_cmds [i].alen != x_len)                        continue;                                                        <* 
 *>          if (s_cmds [i].abbr [0] != x_fields [0][0])          continue;                                                        <* 
 *>          if (strcmp (s_cmds [i].abbr, x_fields [0]) != 0)     continue;                                                        <* 
 *>       }                                                                                                                        <* 
 *>       /+---(execute)---------------------+/                                                                                    <* 
 *>       DEBUG_USER   yLOG_note    ("found it");                                                                                  <* 
 *>       if        (strcmp (s_cmds [i].terms, ""    ) == 0) {                                                                     <* 
 *>          DEBUG_USER   yLOG_note    ("void type, no args");                                                                     <* 
 *>          rc = s_cmds [i].f.v   ();                                                                                             <* 
 *>       } else if (strcmp (s_cmds [i].terms, "c"   ) == 0) {                                                                     <* 
 *>          DEBUG_USER   yLOG_note    ("one char arg");                                                                           <* 
 *>          rc = s_cmds [i].f.c   (x_fields [1][0]);                                                                              <* 
 *>       } else if (strcmp (s_cmds [i].terms, "s"   ) == 0) {                                                                     <* 
 *>          DEBUG_USER   yLOG_note    ("one string arg");                                                                         <* 
 *>          rc = s_cmds [i].f.s   (x_fields [1]);                                                                                 <* 
 *>       } else if (strcmp (s_cmds [i].terms, "a"   ) == 0) {                                                                     <* 
 *>          DEBUG_USER   yLOG_note    ("one long string arg");                                                                    <* 
 *>          rc = s_cmds [i].f.s   (x_all);                                                                                        <* 
 *>       } else if (strcmp (s_cmds [i].terms, "ss"  ) == 0) {                                                                     <* 
 *>          DEBUG_USER   yLOG_note    ("two string args");                                                                        <* 
 *>          rc = s_cmds [i].f.ss  (x_fields [1], x_fields [1]);                                                                   <* 
 *>       } else if (strcmp (s_cmds [i].terms, "is"  ) == 0) {                                                                     <* 
 *>          DEBUG_USER   yLOG_note    ("integer arg and string arg");                                                             <* 
 *>          rc = s_cmds [i].f.is  (atoi (x_fields [1]), x_fields [1]);                                                            <* 
 *>       } else {                                                                                                                 <* 
 *>          DEBUG_USER   yLOG_note    ("crazy other shit, please update or fix");                                                 <* 
 *>          my.key_error = 'y';                                                                                                   <* 
 *>       }                                                                                                                        <* 
 *>       DEBUG_USER   yLOG_value   ("rc"        , rc);                                                                            <* 
 *>       break;                                                                                                                   <* 
 *>    }                                                                                                                           <* 
 *>    /+---(complete)-----------------------+/                                                                                    <* 
 *>    DEBUG_USER   yLOG_exit    (__FUNCTION__);                                                                                   <* 
 *>    return rc;                                                                                                                  <* 
 *>    /+---(complete)-----------------------+/                                                                                    <* 
 *>    return 0;                                                                                                                   <* 
 *> }                                                                                                                              <*/

/*> char         /+-> process keys for input/append ------[ ------ [gc.LE5.266.I3]+/ /+-[05.0000.102.M]-+/ /+-[--.---.---.--]-+/   <* 
 *> USER_cmds_mode     (char a_major, char a_minor)                                                                                <* 
 *> {                                                                                                                              <* 
 *>    /+---(locals)-----------+-----+-----+-+/                                                                                    <* 
 *>    int         x_len       = 0;                                                                                                <* 
 *>    char        x_temp      [11]        = "";                                                                                   <* 
 *>    char        rc          =    0;                                                                                             <* 
 *>    char        x_majors    [LEN_LABEL] = ": ";                                                                                 <* 
 *>    static char x_quoted    = '-';                                                                                              <* 
 *>    static char x_escaped   = '-';                                                                                              <* 
 *>    /+---(header)--------------------s----+/                                                                                    <* 
 *>    DEBUG_USER   yLOG_enter   (__FUNCTION__);                                                                                   <* 
 *>    DEBUG_USER   yLOG_value   ("a_major"   , a_major);                                                                          <* 
 *>    DEBUG_USER   yLOG_value   ("a_minor"   , a_minor);                                                                          <* 
 *>    /+---(get existing len)---------------+/                                                                                    <* 
 *>    DEBUG_USER   yLOG_info    ("s_command" , s_command);                                                                        <* 
 *>    x_len = strlen (s_command);                                                                                                 <* 
 *>    DEBUG_USER   yLOG_value   ("x_len"     , x_len);                                                                            <* 
 *>    /+---(check for quoting)--------------+/                                                                                    <* 
 *>    if (x_escaped != 'y' && a_minor == G_KEY_DQUOTE) {                                                                          <* 
 *>       if (x_quoted != 'y') {                                                                                                   <* 
 *>          DEBUG_USER   yLOG_note    ("entering quoted string");                                                                 <* 
 *>          x_quoted = 'y';                                                                                                       <* 
 *>       } else {                                                                                                                 <* 
 *>          DEBUG_USER   yLOG_note    ("exiting quoted string");                                                                  <* 
 *>          x_quoted = '-';                                                                                                       <* 
 *>       }                                                                                                                        <* 
 *>    }                                                                                                                           <* 
 *>    /+---(check for special codes)--------+/                                                                                    <* 
 *>    if (x_escaped != 'y' && a_minor == G_KEY_BSLASH) {                                                                          <* 
 *>       x_escaped = 'y';                                                                                                         <* 
 *>       DEBUG_USER   yLOG_note    ("begin escaped character");                                                                   <* 
 *>       DEBUG_USER   yLOG_exit    (__FUNCTION__);                                                                                <* 
 *>       return 0;                                                                                                                <* 
 *>    } else if (x_escaped == 'y') {                                                                                              <* 
 *>       x_escaped = '-';                                                                                                         <* 
 *>       DEBUG_USER   yLOG_note    ("convert escaped character");                                                                 <* 
 *>       switch (a_minor) {                                                                                                       <* 
 *>       case 'n'      :  a_minor = G_CHAR_RETURN;  break;  /+ return char           +/                                           <* 
 *>       case 'e'      :  a_minor = G_CHAR_ESCAPE;  break;  /+ escape char           +/                                           <* 
 *>       case 't'      :  a_minor = G_CHAR_TAB;     break;  /+ tab char              +/                                           <* 
 *>       case 'b'      :  a_minor = G_CHAR_BS;      break;  /+ backspace char        +/                                           <* 
 *>       case 's'      :  a_minor = G_CHAR_SPACE;   break;  /+ visual space          +/                                           <* 
 *>       case 'f'      :  a_minor = G_CHAR_FIELD;   break;  /+ field delimiter       +/                                           <* 
 *>       case 'g'      :  a_minor = G_CHAR_GROUP;   break;  /+ group delimiter       +/                                           <* 
 *>       case '0'      :  a_minor = G_CHAR_NULL;    break;  /+ null                  +/                                           <* 
 *>       case 'a'      :  a_minor = G_CHAR_ALT;     break;  /+ alt prefix            +/                                           <* 
 *>       case 'c'      :  a_minor = G_CHAR_CONTROL; break;  /+ control prefix        +/                                           <* 
 *>       case 'w'      :  a_minor = G_CHAR_WAIT;    break;  /+ wait/pause            +/                                           <* 
 *>       case 'p'      :  a_minor = G_CHAR_BREAK;   break;  /+ break point           +/                                           <* 
 *>       case 'h'      :  a_minor = G_CHAR_HALT;    break;  /+ halt  <C-c>           +/                                           <* 
 *>       case 'd'      :  a_minor = G_CHAR_DISPLAY; break;  /+ force redisplay       +/                                           <* 
 *>       case G_KEY_DQUOTE :  a_minor = G_KEY_DQUOTE;       break;  /+ quote without quoting +/                                   <* 
 *>       default       :  a_minor = G_CHAR_SPACE;   break;                                                                        <* 
 *>       }                                                                                                                        <* 
 *>    }                                                                                                                           <* 
 *>    /+---(check for control keys)---------+/                                                                                    <* 
 *>    if (x_quoted != 'y') {                                                                                                      <* 
 *>       switch (a_minor) {                                                                                                       <* 
 *>       case   G_KEY_RETURN : case   G_KEY_ENTER  :                                                                              <* 
 *>          DEBUG_USER   yLOG_note    ("return/enter, execute command");                                                          <* 
 *>          yVIKEYS_mode_exit ();                                                                                                 <* 
 *>          rc = USER_cmds_exec (s_command);                                                                                      <* 
 *>          DEBUG_USER   yLOG_exit    (__FUNCTION__);                                                                             <* 
 *>          return rc;   /+ return  +/                                                                                            <* 
 *>       case   G_KEY_ESCAPE :                                                                                                    <* 
 *>          DEBUG_USER   yLOG_note    ("escape, ignore command");                                                                 <* 
 *>          yVIKEYS_mode_exit ();                                                                                                 <* 
 *>          yVIKEYS_cmds_clear ();                                                                                                <* 
 *>          DEBUG_USER   yLOG_exit    (__FUNCTION__);                                                                             <* 
 *>          return 0;                                                                                                             <* 
 *>       }                                                                                                                        <* 
 *>    }                                                                                                                           <* 
 *>    /+---(check for backspace)------------+/                                                                                    <* 
 *>    if (a_minor == G_KEY_DEL || a_minor == G_KEY_BS) {                                                                          <* 
 *>       DEBUG_USER   yLOG_note    ("bs/del, remove character");                                                                  <* 
 *>       --x_len;                                                                                                                 <* 
 *>       if (s_command [x_len] == G_KEY_DQUOTE) {                                                                                 <* 
 *>          if (x_quoted == 'y')  x_quoted = '-';                                                                                 <* 
 *>          else                  x_quoted = 'y';                                                                                 <* 
 *>       }                                                                                                                        <* 
 *>       if (x_len < 0)   x_len = 0;                                                                                              <* 
 *>       s_command [x_len] = '\0';                                                                                                <* 
 *>       DEBUG_USER   yLOG_info    ("s_command" , s_command);                                                                     <* 
 *>       DEBUG_USER   yLOG_value   ("x_len"     , x_len);                                                                         <* 
 *>       DEBUG_USER   yLOG_exit    (__FUNCTION__);                                                                                <* 
 *>       return 0;                                                                                                                <* 
 *>    }                                                                                                                           <* 
 *>    /+---(normal characters)--------------+/                                                                                    <* 
 *>    DEBUG_USER   yLOG_note    ("update command line");                                                                          <* 
 *>    snprintf (x_temp, 10, "%c", a_minor);                                                                                       <* 
 *>    strcat   (s_command, x_temp);                                                                                               <* 
 *>    x_len = strlen (s_command);                                                                                                 <* 
 *>    DEBUG_USER   yLOG_info    ("s_command" , s_command);                                                                        <* 
 *>    DEBUG_USER   yLOG_value   ("x_len"     , x_len);                                                                            <* 
 *>    /+---(complete)-----------------------+/                                                                                    <* 
 *>    DEBUG_USER   yLOG_exit    (__FUNCTION__);                                                                                   <* 
 *>    return 0;                                                                                                                   <* 
 *> }                                                                                                                              <*/



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
