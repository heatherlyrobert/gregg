/*============================[[    beg-code    ]]============================*/
#include    "gregg.h"



/*====================------------------------------------====================*/
/*===----                      file control                            ----===*/
/*====================------------------------------------====================*/
static void  o___FILE____________o () { return; }

char
SHARED_open             (char a_name [LEN_PATH], char a_mode, FILE **b_file)
{
   /*---(locals)-----------+-----+-----+-*/
   char        rce         =  -10;
   char        x_mode      [LEN_TERSE] = "";
   FILE       *f           = NULL;
   /*---(header)-------------------------*/
   DEBUG_FILE   yLOG_enter   (__FUNCTION__);
   /*---(defense)------------------------*/
   DEBUG_FILE   yLOG_point   ("a_name"    , a_name);
   --rce;  if (a_name      == NULL) {
      DEBUG_FILE   yLOG_exitr   (__FUNCTION__, rce);
      return rce;
   }
   DEBUG_FILE   yLOG_info    ("a_name"    , a_name);
   DEBUG_FILE   yLOG_char    ("a_mode"    , a_mode);
   --rce;  if (a_mode == 0 || strchr ("rRwW", a_mode) == NULL) {
      DEBUG_FILE   yLOG_exitr   (__FUNCTION__, rce);
      return rce;
   }
   DEBUG_FILE   yLOG_point   ("b_file"    , b_file);
   --rce;  if (b_file      == NULL) {
      DEBUG_FILE   yLOG_exitr   (__FUNCTION__, rce);
      return rce;
   }
   DEBUG_INPT   yLOG_point   ("*b_file"   , *b_file);
   --rce;  if (*b_file != NULL) {
      DEBUG_FILE   yLOG_exitr   (__FUNCTION__, rce);
      return rce;
   }
   /*---(set mode)-----------------------*/
   DEBUG_FILE   yLOG_char    ("a_mode"    , a_mode);
   --rce;  switch (a_mode) {
   case 'r' :
      strlcpy (x_mode, "rt", LEN_TERSE);
      break;
   case 'R' :
      strlcpy (x_mode, "rb", LEN_TERSE);
      break;
   case 'w' :
      strlcpy (x_mode, "wt", LEN_TERSE);
      break;
   case 'W' :
      strlcpy (x_mode, "wb", LEN_TERSE);
      break;
   default  :
      DEBUG_FILE   yLOG_exitr   (__FUNCTION__, rce);
      return rce;
   }
   DEBUG_FILE   yLOG_info    ("x_mode"    , x_mode);
   /*---(open)---------------------------*/
   DEBUG_FILE   yLOG_info    ("a_name"    , a_name);
   yURG_msg ('-', "database is å%sæ", a_name);
   f = fopen (a_name, x_mode);
   DEBUG_FILE   yLOG_point   ("f"         , f);
   --rce;  if (f == NULL) {
      DEBUG_FILE   yLOG_exitr   (__FUNCTION__, rce);
      return rce;
   }
   /*---(save-back)----------------------*/
   if (b_file != NULL)  *b_file = f;
   /*---(complete)-----------------------*/
   DEBUG_FILE   yLOG_exit    (__FUNCTION__);
   return 0;
}

char
SHARED_close            (FILE **b_file)
{
   /*---(locals)-----------+-----+-----+-*/
   char        rce         =  -10;
   char        rc          =    0;
   /*---(header)-------------------------*/
   DEBUG_FILE   yLOG_enter   (__FUNCTION__);
   /*---(defense)------------------------*/
   DEBUG_INPT   yLOG_point   ("b_file"    , b_file);
   --rce;  if (b_file == NULL) {
      DEBUG_FILE   yLOG_exitr   (__FUNCTION__, rce);
      return rce;
   }
   DEBUG_INPT   yLOG_point   ("*b_file"   , *b_file);
   --rce;  if (*b_file == NULL) {
      DEBUG_FILE   yLOG_exitr   (__FUNCTION__, rce);
      return rce;
   }
   /*---(close)--------------------------*/
   rc = fclose (*b_file);
   DEBUG_OUTP   yLOG_value   ("close"     , rc);
   --rce;  if (rc < 0) {
      DEBUG_FILE   yLOG_exitr   (__FUNCTION__, rce);
      return rce; 
   }
   /*---(ground pointer)-----------------*/
   *b_file = NULL;
   DEBUG_INPT   yLOG_point   ("*b_file"   , *b_file);
   /*---(complete)-----------------------*/
   DEBUG_FILE   yLOG_exit    (__FUNCTION__);
   return 0;
}



