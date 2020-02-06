/*============================[[    beg-code    ]]============================*/
#include    "gregg.h"


static char    s_ready       = '-';
static char    s_recd        [LEN_RECD] = "";
static int     s_line        =   0;


char
STDIN_check             (void)
{
   /*---(locals)-----------+-----------+-*/
   char        rce         =  -10;          /* return code for errors         */
   char        rc          =    0;
   int         x_flags     =    0;
   int         x_ch        =  ' ';
   /*---(header)-------------------------*/
   DEBUG_INPT   yLOG_enter   (__FUNCTION__);
   /*---(set stdin to non-blocking)------*/
   DEBUG_INPT   yLOG_note    ("make stdin non-blocking");
   x_flags = fcntl(0, F_GETFL, 0);
   rc = fcntl (stdin, F_SETFL, x_flags | O_NONBLOCK);
   DEBUG_INPT   yLOG_value   ("rc"        , rc);
   /*> --rce;  if (rc < 0) {                                                          <* 
    *>    DEBUG_INPT   yLOG_note    ("can not set to non-blocking");                  <* 
    *>    DEBUG_INPT   yLOG_exitr   (__FUNCTION__, rce);                              <* 
    *>    return rce;                                                                 <* 
    *> }                                                                              <*/
   /*---(test stdin for input)-----------*/
   DEBUG_INPT   yLOG_note    ("test for input on stdin");
   x_ch = getc (stdin);
   DEBUG_INPT   yLOG_value   ("x_ch"      , x_ch);
   --rce;  if (x_ch == -1) {
      DEBUG_INPT   yLOG_note    ("no input available");
      DEBUG_INPT   yLOG_exitr   (__FUNCTION__, rce);
      return rce;
   }
   /*---(put stdin back to normal)-------*/
   DEBUG_INPT   yLOG_note    ("put stdin back to normal");
   ungetc (x_ch, stdin);
   rc = fcntl  (0, F_SETFL, x_flags);
   DEBUG_INPT   yLOG_value   ("rc"        , rc);
   /*> --rce;  if (rc < 0) {                                                          <* 
    *>    DEBUG_INPT   yLOG_note    ("can not set to back to normal");                <* 
    *>    DEBUG_INPT   yLOG_exitr   (__FUNCTION__, rce);                              <* 
    *>    return rce;                                                                 <* 
    *> }                                                                              <*/
   /*---(set as ready)-------------------*/
   s_ready = 'y';
   /*---(complete)-----------------------*/
   DEBUG_INPT   yLOG_exit    (__FUNCTION__);
   return 0;
}

char
STDIN_getline           (void)
{
   /*---(locals)-----------+-----+-----+-*/
   char        rce         =  -10;
   char        rc          =    0;
   int         x_len       =    0;
   /*---(begin)----------------------------*/
   DEBUG_INPT   yLOG_enter   (__FUNCTION__);
   /*---(defense)------------------------*/
   DEBUG_INPT   yLOG_char    ("s_ready"   , s_ready);
   --rce;  if (s_ready != 'y') {
      DEBUG_INPT   yLOG_exitr   (__FUNCTION__, rce);
      return rce;
   }
   /*---(find a record)------------------*/
   while (1) {
      /*---(read)------------------------*/
      if (feof (stdin)) {
         DEBUG_INPT   yLOG_exitr   (__FUNCTION__, rce);
         return rce;
      }
      fgets (s_recd, LEN_RECD, stdin);
      ++s_line;
      /*---(filter)----------------------*/
      if (s_recd [0] == '\0')  continue;
      if (s_recd [0] == '#')   continue;
      if (s_recd [0] == ' ')   continue;
      /*---(clean-up)--------------------*/
      x_len = strlen (s_recd);
      if (x_len <= 1)          continue;
      if (s_recd [x_len - 1] == '\n')  s_recd [--x_len] = '\0';
      DEBUG_INPT   yLOG_info    ("s_recd"    , s_recd);
      break;
   }
   /*---(complete)-----------------------*/
   DEBUG_INPT   yLOG_exit    (__FUNCTION__);
   return 0;
}

char
STDIN_handler           (void)
{
   /*---(local variables)--+-----------+-*/
   char        rce         =  -10;          /* return code for errors         */
   char        rc          =    0;          /* return code for character      */
   int         x_len       =    0;          /* input record length            */
   /*---(header)-------------------------*/
   DEBUG_INPT   yLOG_enter   (__FUNCTION__);
   /*---(prepare)------------------------*/
   s_line   = 0;
   s_ready  = '-';
   /*---(check for input)----------------*/
   rc = STDIN_check ();
   --rce;  if (rc < 0) {
      DEBUG_INPT   yLOG_exitr   (__FUNCTION__, rce);
      return rce;
   }
   /*---(begin page)---------------------*/
   if (strchr (RUN_WRITING, my.run_mode) != NULL)  REVERSE_page_beg ();
   /*---(process file)-------------------*/
   DEBUG_INPT   yLOG_note    ("start reading file");
   while (1) {
      /*---(take in a line)--------------*/
      rc = STDIN_getline ();
      if (rc < 0) break;     /* end-of-input      */
      /*---(handle line)-----------------*/
      switch (my.run_mode) {
      case RUN_ENGLISH :
         REVERSE_english_text (s_recd, SHAPE_DRAW, 1, '-');
         break;
      case RUN_GREGG   :
         REVERSE_gregg_text   (s_recd, SHAPE_DRAW, 1, '-');
         break;
      case RUN_CONVERT :
         break;
      case RUN_EXACT   :
         break;
      case RUN_REVERSE :
         break;
      }
   }
   if (strchr (RUN_WRITING, my.run_mode) != NULL)  REVERSE_page_end ();
   /*---(complete)-----------------------*/
   DEBUG_INPT   yLOG_exit    (__FUNCTION__);
   return 0;
}




