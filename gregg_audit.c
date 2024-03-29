/*============================[[    beg-code    ]]============================*/
#include    "gregg.h"



char
AUDIT_build_fancy       (char c, char a_rc, char a_letter [LEN_SHORT], char b_fancy [LEN_FULL])
{
   /*---(locals)-----------+-----+-----+-*/
   char        rce         =  -10;
   char        t           [LEN_RECD]  = "";
   /*---(header)-------------------------*/
   DEBUG_INPT   yLOG_enter   (__FUNCTION__);
   /*---(defense)------------------------*/
   DEBUG_INPT   yLOG_point   ("a_Letter"  , a_letter);
   --rce;  if (a_letter == NULL) {
      DEBUG_INPT   yLOG_exitr   (__FUNCTION__, rce);
      return rce;
   }
   DEBUG_INPT   yLOG_info    ("a_Letter"  , a_letter);
   DEBUG_INPT   yLOG_point   ("b_fancy"   , b_fancy);
   --rce;  if (b_fancy  == NULL) {
      DEBUG_INPT   yLOG_exitr   (__FUNCTION__, rce);
      return rce;
   }
   /*---(end)----------------------------*/
   if (c == 100) {
      strlcat (b_fancy, BOLD_OFF, LEN_RECD);
      DEBUG_INPT   yLOG_exit    (__FUNCTION__);
      return 0;
   }
   /*---(start/prefix)-------------------*/
   if (c == 0)   strlcpy (b_fancy, "" , LEN_RECD);
   else          strlcat (b_fancy, "�", LEN_RECD);
   /*---(success)------------------------*/
   if (a_rc >= 0) {
      if (c == 0)   strlcat (b_fancy, BOLD_GRN, LEN_RECD);
      strlcat (b_fancy, a_letter, LEN_RECD);
   }
   /*---(fail)---------------------------*/
   else  {
      strlcat (b_fancy, BOLD_ERR, LEN_RECD);
      strlcat (b_fancy, a_letter, LEN_RECD);
      strlcat (b_fancy, BOLD_OFF, LEN_RECD);
      strlcat (b_fancy, BOLD_GRN, LEN_RECD);
   }
   /*---(complete)-----------------------*/
   DEBUG_INPT   yLOG_exit    (__FUNCTION__);
   return 0;
}

char
AUDIT_gregg_outline     (char a_gregg [LEN_TITLE], char r_fancy [LEN_FULL])
{
   /*---(locals)-----------+-----+-----+-*/
   char        rce         =  -10;
   char        rc          =    0;
   uchar       x_gregg     [LEN_TITLE] = "";
   char       *p           = NULL;
   char       *q           =  "�";
   char       *r           = NULL;
   short       n           =    0;
   char        c           =    0;
   char        x_fail      =  '-';
   /*---(header)-------------------------*/
   DEBUG_INPT   yLOG_enter   (__FUNCTION__);
   /*---(defense)------------------------*/
   DEBUG_INPT   yLOG_point   ("a_gregg"   , a_gregg);
   --rce;  if (a_gregg == NULL) {
      DEBUG_INPT   yLOG_exitr   (__FUNCTION__, rce);
      return rce;
   }
   DEBUG_INPT   yLOG_point   ("r_fancy"   , r_fancy);
   --rce;  if (r_fancy  == NULL) {
      DEBUG_INPT   yLOG_exitr   (__FUNCTION__, rce);
      return rce;
   }
   /*---(prepare)------------------------*/
   strlcpy (x_gregg, a_gregg, LEN_TITLE);
   /*---(begin parse)--------------------*/
   p = strtok_r (x_gregg, q, &r);
   /*---(walk letters)-------------------*/
   --rce;  while (p != NULL) {
      /*---(find letter)-----------------*/
      DEBUG_INPT   yLOG_info    ("p"         , p);
      n = TABLE_letter_by_name (p, LTRS_NORM, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL);
      DEBUG_INPT   yLOG_value   ("current"   , n);
      if (n < 0) x_fail = 'y';
      /*---(fancify)---------------------*/
      AUDIT_build_fancy (c, n, p, r_fancy);
      /*---(next)------------------------*/
      ++c;
      p = strtok_r (NULL   , q, &r);
      DEBUG_INPT   yLOG_point   ("p"         , p);
      /*---(done)------------------------*/
   }
   /*---(complete)-----------------------*/
   AUDIT_build_fancy (100, 0, "", r_fancy);
   /*---(check trouble)------------------*/
   DEBUG_INPT   yLOG_char    ("x_fail"    , x_fail);
   --rce;  if (x_fail == 'y') {
      DEBUG_INPT   yLOG_exitr   (__FUNCTION__, rce);
      return rce;
   }
   /*---(complete)-----------------------*/
   DEBUG_INPT   yLOG_exit    (__FUNCTION__);
   return 0;
}


