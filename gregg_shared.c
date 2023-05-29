/*============================[[    beg-code    ]]============================*/
#include    "gregg.h"



/*====================------------------------------------====================*/
/*===----                       memory allccation                      ----===*/
/*====================------------------------------------====================*/
static void  o___MEMORY__________o () { return; }

char
SHARED_new              (char a_abbr, char a_force, short a_size, char *a_wiper (void *), void **r_new)
{
   /*---(locals)-----------+-----+-----+-*/
   char        rce         =  -10;
   char        rc          =    0;
   void       *x_new       = NULL;
   int         x_tries     =    0;
   /*---(header)-------------------------*/
   DEBUG_DATA   yLOG_senter  (__FUNCTION__);
   DEBUG_DATA   yLOG_schar   (a_abbr);
   /*---(check return)-------------------*/
   DEBUG_DATA   yLOG_spoint  (r_new);
   --rce;  if (r_new == NULL) {
      DEBUG_DATA   yLOG_sexitr  (__FUNCTION__, rce);
      return rce;
   }
   DEBUG_DATA   yLOG_spoint  (*r_new);
   --rce;  if (a_force != 'y' && *r_new != NULL) {
      DEBUG_DATA   yLOG_snote   ("already set");
      DEBUG_DATA   yLOG_sexitr  (__FUNCTION__, rce);
      return rce;
   }
   /*---(default)------------------------*/
   *r_new = NULL;
   /*---(check type)---------------------*/
   --rce;  switch (a_abbr) {
   case B_BASE   : break;
   case B_DICT   : break;
   default       :
      DEBUG_DATA   yLOG_snote   ("unknown btree abbr");
      DEBUG_DATA   yLOG_sexitr  (__FUNCTION__, rce);
      return rce;
      break;
   }
   /*---(check size)---------------------*/
   DEBUG_DATA   yLOG_sint    (a_size);
   --rce;  if (a_size <= 0) {
      DEBUG_DATA   yLOG_sexitr  (__FUNCTION__, rce);
      return rce;
   }
   /*---(allocate)-----------------------*/
   while (x_new == NULL) {
      ++x_tries;
      x_new = malloc (a_size);
      if (x_tries > 3)   break;
   }
   DEBUG_DATA   yLOG_sint    (x_tries);
   DEBUG_DATA   yLOG_spoint  (x_new);
   --rce;  if (x_new == NULL) {
      DEBUG_DATA   yLOG_sexitr  (__FUNCTION__, rce);
      return rce;
   }
   /*---(wipe)---------------------------*/
   rc = 0;
   if (a_wiper != NULL)  rc = a_wiper (x_new);
   /*---(save return)--------------------*/
   *r_new = x_new;
   /*---(complete)-----------------------*/
   DEBUG_DATA   yLOG_sexit   (__FUNCTION__);
   return rc;
}

char
SHARED_free             (char a_abbr, void **r_old)
{
   /*---(locals)-----------+-----+-----+-*/
   char        rce         =  -10;
   /*---(header)-------------------------*/
   DEBUG_DATA   yLOG_senter  (__FUNCTION__);
   DEBUG_DATA   yLOG_sint    (a_abbr);
   /*---(check return)-------------------*/
   DEBUG_DATA   yLOG_spoint  (r_old);
   --rce;  if (r_old == NULL) {
      DEBUG_DATA   yLOG_sexitr  (__FUNCTION__, rce);
      return rce;
   }
   DEBUG_DATA   yLOG_spoint  (*r_old);
   --rce;  if (*r_old == NULL) {
      DEBUG_DATA   yLOG_snote   ("never set");
      DEBUG_DATA   yLOG_sexitr  (__FUNCTION__, rce);
      return rce;
   }
   /*---(clear and return)---------------*/
   free (*r_old);
   /*---(save return)--------------------*/
   *r_old = NULL;
   /*---(complete)-----------------------*/
   DEBUG_DATA   yLOG_sexit   (__FUNCTION__);
   return 0;
}



/*====================------------------------------------====================*/
/*===----                      file control                            ----===*/
/*====================------------------------------------====================*/
static void  o___FILE____________o () { return; }

char
SHARED_open             (char a_name [LEN_PATH], char a_mode, FILE **r_file)
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
   DEBUG_FILE   yLOG_point   ("r_file"    , r_file);
   --rce;  if (r_file      == NULL) {
      DEBUG_FILE   yLOG_exitr   (__FUNCTION__, rce);
      return rce;
   }
   DEBUG_INPT   yLOG_point   ("*r_file"   , *r_file);
   --rce;  if (*r_file != NULL) {
      DEBUG_FILE   yLOG_exitr   (__FUNCTION__, rce);
      return rce;
   }
   /*---(set mode)-----------------------*/
   DEBUG_FILE   yLOG_char    ("a_mode"    , a_mode);
   --rce;  switch (a_mode) {
   case 'r' :
      strlcpy (x_mode, "rt", LEN_TERSE);
      yURG_msg ('-', "reading text file å%sæ", a_name);
      break;
   case 'R' :
      strlcpy (x_mode, "rb", LEN_TERSE);
      yURG_msg ('-', "reading database å%sæ", a_name);
      break;
   case 'w' :
      strlcpy (x_mode, "wt", LEN_TERSE);
      yURG_msg ('-', "writing text file å%sæ", a_name);
      break;
   case 'W' :
      strlcpy (x_mode, "wb", LEN_TERSE);
      yURG_msg ('-', "writing database å%sæ", a_name);
      break;
   default  :
      DEBUG_FILE   yLOG_exitr   (__FUNCTION__, rce);
      return rce;
   }
   DEBUG_FILE   yLOG_info    ("x_mode"    , x_mode);
   /*---(open)---------------------------*/
   DEBUG_FILE   yLOG_info    ("a_name"    , a_name);
   f = fopen (a_name, x_mode);
   DEBUG_FILE   yLOG_point   ("f"         , f);
   --rce;  if (f == NULL) {
      DEBUG_FILE   yLOG_exitr   (__FUNCTION__, rce);
      return rce;
   }
   /*---(save-back)----------------------*/
   if (r_file != NULL)  *r_file = f;
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



