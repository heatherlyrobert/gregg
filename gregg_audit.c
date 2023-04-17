/*============================[[    beg-code    ]]============================*/
#include    "gregg.h"



char
AUDIT_build_fancy       (char c, char a_rc, char a_letter [LEN_SHORT], char b_fancy [LEN_FULL])
{
   /*---(locals)-----------+-----+-----+-*/
   char        rce         =  -10;
   char        t           [LEN_RECD]  = "";
   /*---(header)-------------------------*/
   DEBUG_GRAF   yLOG_enter   (__FUNCTION__);
   /*---(defense)------------------------*/
   DEBUG_CONF   yLOG_point   ("a_Letter"  , a_letter);
   --rce;  if (a_letter == NULL) {
      DEBUG_CONF   yLOG_exitr   (__FUNCTION__, rce);
      return rce;
   }
   DEBUG_CONF   yLOG_info    ("a_Letter"  , a_letter);
   DEBUG_CONF   yLOG_point   ("b_fancy"   , b_fancy);
   --rce;  if (b_fancy  == NULL) {
      DEBUG_CONF   yLOG_exitr   (__FUNCTION__, rce);
      return rce;
   }
   /*---(end)----------------------------*/
   if (c == 100) {
      strlcat (b_fancy, BOLD_OFF, LEN_RECD);
      DEBUG_GRAF   yLOG_exit    (__FUNCTION__);
      return 0;
   }
   /*---(start/prefix)-------------------*/
   if (c == 0)   strlcpy (b_fancy, "" , LEN_RECD);
   else          strlcat (b_fancy, "·", LEN_RECD);
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
   DEBUG_GRAF   yLOG_exit    (__FUNCTION__);
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
   char       *q           =  "·";
   char       *r           = NULL;
   short       n           =    0;
   char        c           =    0;
   /*---(header)-------------------------*/
   DEBUG_GRAF   yLOG_enter   (__FUNCTION__);
   /*---(defense)------------------------*/
   DEBUG_CONF   yLOG_point   ("a_gregg"   , a_gregg);
   --rce;  if (a_gregg == NULL) {
      DEBUG_CONF   yLOG_exitr   (__FUNCTION__, rce);
      return rce;
   }
   DEBUG_CONF   yLOG_point   ("r_fancy"   , r_fancy);
   --rce;  if (r_fancy  == NULL) {
      DEBUG_CONF   yLOG_exitr   (__FUNCTION__, rce);
      return rce;
   }
   /*---(prepare)------------------------*/
   strlcpy (x_gregg, a_gregg, LEN_TITLE);
   /*---(begin parse)--------------------*/
   p = strtok_r (x_gregg, q, &r);
   /*---(walk letters)-------------------*/
   --rce;  while (p != NULL) {
      /*---(find letter)-----------------*/
      DEBUG_CONF   yLOG_info    ("p"         , p);
      n = CREATE_find_by_name (p, LTRS_NORM, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL);
      DEBUG_OUTP   yLOG_value   ("current"   , n);
      /*---(fancify)---------------------*/
      AUDIT_build_fancy (c, n, p, r_fancy);
      /*---(next)------------------------*/
      ++c;
      p = strtok_r (NULL   , q, &r);
      DEBUG_CONF   yLOG_point   ("p"         , p);
      /*---(done)------------------------*/
   }
   /*---(complete)-----------------------*/
   AUDIT_build_fancy (100, 0, "", r_fancy);
   /*---(complete)-----------------------*/
   DEBUG_OUTP   yLOG_exit    (__FUNCTION__);
   return 0;
}


