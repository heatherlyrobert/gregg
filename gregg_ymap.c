/*============================---(source-start)---============================*/
#include    "gregg.h"


tWIN  win;

/*====================------------------------------------====================*/
/*===----                     labels and positions                     ----===*/
/*====================------------------------------------====================*/
static void   o___LABEL___________o (void) { return; }

char         /*-> break label into coordinates -------[ ------ [gc.722.112.13]*/ /*-[01.0000.304.#]-*/ /*-[--.---.---.--]-*/
api_ymap_locator        (char a_strict, char *a_label, ushort *u, ushort *x, ushort *y, ushort *z)
{  /*---(design notes)-------------------*/
   /*
    * rc = -1 means address not found
    *
    */
   /*---(header)-------------------------*/
   DEBUG_YMAP   yLOG_enter   (__FUNCTION__);
   /*---(defaults)-----------------------*/
   if (u != NULL)  *u = 0;
   if (x != NULL)  *x = 0;
   if (y != NULL)  *y = 0;
   if (z != NULL)  *z = 0;
   /*---(complete)-----------------------*/
   DEBUG_YMAP   yLOG_exit    (__FUNCTION__);
   return -1;
}

char         /*-> return address for coordinates -----[ ------ [gc.722.112.13]*/ /*-[01.0000.304.#]-*/ /*-[--.---.---.--]-*/
api_ymap_addressor      (char a_strict, char *a_label, ushort u, ushort x, ushort y, ushort z)
{  /*---(design notes)-------------------*/
   /*
    * rc = -1 means address not found
    *
    */
   /*---(header)-------------------------*/
   DEBUG_YMAP   yLOG_enter   (__FUNCTION__);
   /*---(defaults)-----------------------*/
   if (a_label != NULL)  strlcpy (a_label, "", LEN_LABEL);
   /*---(complete)-----------------------*/
   DEBUG_YMAP   yLOG_exit    (__FUNCTION__);
   return -1;
}



/*====================------------------------------------====================*/
/*===----                     map loading support                      ----===*/
/*====================------------------------------------====================*/
static void   o___LOAD____________o (void) { return; }

char
api_ymap_sizer          (char a_axis, ushort *n, ushort *a, ushort *b, ushort *c, ushort *e, ushort *m, ushort *x)
{
   char        rce         =  -10;
   char        rc          =    0;
   int         x_extra     =    0;
   DEBUG_YMAP    yLOG_senter  (__FUNCTION__);
   DEBUG_YMAP    yLOG_schar   (a_axis);
   --rce;  switch (a_axis) {
   case YMAP_UNIV  :
      DEBUG_YMAP    yLOG_snote   ("universe NOT supported");
      DEBUG_YMAP    yLOG_sexitr  (__FUNCTION__, rce);
      return rce;
      break;
   case YMAP_XAXIS :
      DEBUG_YMAP    yLOG_snote   ("column is single");
      DEBUG_YMAP    yLOG_snote   ("all modes");
      if (n != NULL)  *n = 1;
      if (a != NULL)  *a = 1;
      if (b != NULL)  *b = 0;
      if (c != NULL)  *c = 0;
      if (e != NULL)  *e = 0;
      if (m != NULL)  *m = 0;
      if (x != NULL)  *x = 0;
      break;
   case YMAP_YAXIS :
      DEBUG_YMAP    yLOG_snote   ("rows are pages");
      if (my.w_layout == LAYOUT_DICTIONARY) {
         DEBUG_YMAP    yLOG_sint    (my.w_npage);
         if (n != NULL)  *n = my.w_npage;
      } else if (my.w_layout == LAYOUT_CONNECT) {
         DEBUG_YMAP    yLOG_snote   ("connect");
         if (n != NULL)  *n = 4;
      }
      if (a != NULL)  *a = 1;
      if (b != NULL)  *b = 0;
      if (c != NULL)  *c = 0;
      if (e != NULL)  *e = 0;
      if (m != NULL)  *m = 0;
      if (x != NULL)  *x = my.w_npage;
      break;
   case YMAP_ZAXIS :
      DEBUG_YMAP    yLOG_snote   ("depth NOT supported");
      DEBUG_YMAP    yLOG_sexitr  (__FUNCTION__, rce);
      return rce;
      break;
   case YMAP_WHEN  :
      DEBUG_YMAP    yLOG_snote   ("when NOT supported");
      DEBUG_YMAP    yLOG_sexitr  (__FUNCTION__, rce);
      return rce;
      break;
   default :
      DEBUG_YMAP    yLOG_snote   ("axis not understood");
      DEBUG_YMAP    yLOG_sexitr  (__FUNCTION__, rce);
      return rce;
      break;
   }
   DEBUG_YMAP    yLOG_sexit   (__FUNCTION__);
   return 0;
}

char
api_ymap_entry          (char a_axis, ushort a_pos, short *r_ref, uchar *r_wide, uchar *r_used)
{
   char        rce         =  -10;
   char        rc          =    0;
   DEBUG_YMAP    yLOG_senter  (__FUNCTION__);
   DEBUG_YMAP    yLOG_schar   (a_axis);
   DEBUG_YMAP    yLOG_sint    (a_pos);
   --rce;  switch (a_axis) {
   case YMAP_UNIV  :
      DEBUG_YMAP    yLOG_snote   ("universe NOT supported");
      DEBUG_YMAP    yLOG_sexitr  (__FUNCTION__, rce);
      return rce;
      break;
   case YMAP_XAXIS :
      DEBUG_YMAP    yLOG_snote   ("column is single");
      if (r_ref  != NULL)  *r_ref  = a_pos;
      if (r_wide != NULL)  *r_wide = 1;
      if (r_used != NULL)  *r_used = YMAP_USED;
      break;
   case YMAP_YAXIS :
      DEBUG_YMAP    yLOG_snote   ("rows are pages");
      if (r_ref  != NULL)  *r_ref  = a_pos;
      if (r_wide != NULL)  *r_wide = 1;
      if (r_used != NULL)  *r_used = YMAP_USED;
      break;
   case YMAP_ZAXIS :
      DEBUG_YMAP    yLOG_snote   ("depth NOT supported");
      DEBUG_YMAP    yLOG_sexitr  (__FUNCTION__, rce);
      return rce;
      break;
   case YMAP_WHEN  :
      DEBUG_YMAP    yLOG_snote   ("when NOT supported");
      DEBUG_YMAP    yLOG_sexitr  (__FUNCTION__, rce);
      return rce;
      break;
   default :
      DEBUG_YMAP    yLOG_snote   ("axis not understood");
      DEBUG_YMAP    yLOG_sexitr  (__FUNCTION__, rce);
      return rce;
      break;
   }
   DEBUG_YMAP    yLOG_sint    (rc);
   DEBUG_YMAP    yLOG_sexit   (__FUNCTION__);
   return rc;
}



/*====================------------------------------------====================*/
/*===----                       content update                         ----===*/
/*====================------------------------------------====================*/
static void   o___UPDATE__________o (void) { return; }

char
api_ymap_placer         (char a_axis, ushort b, ushort c, ushort e)
{
   char        rce         =  -10;
   char        rc          =    0;
   DEBUG_YMAP    yLOG_senter  (__FUNCTION__);
   DEBUG_YMAP    yLOG_schar   (a_axis);
   DEBUG_YMAP    yLOG_sint    (b);
   DEBUG_YMAP    yLOG_sint    (c);
   DEBUG_YMAP    yLOG_sint    (e);
   --rce;  switch (a_axis) {
   case YMAP_UNIV  :
      DEBUG_YMAP    yLOG_snote   ("universe NOT supported");
      DEBUG_YMAP    yLOG_sexitr  (__FUNCTION__, rce);
      return rce;
      break;
   case YMAP_XAXIS :
      DEBUG_YMAP    yLOG_snote   ("column in single");
      break;
   case YMAP_YAXIS :
      DEBUG_YMAP    yLOG_snote   ("row are pages");
      if (my.w_cpage != c) {
         DEBUG_YMAP    yLOG_sint    (c);
         DEBUG_YMAP    yLOG_sexit   (__FUNCTION__);
         my.w_cpage = c;
         DRAW_make (my.w_layout);
         return 0;
      }
      break;
   case YMAP_ZAXIS :
      DEBUG_YMAP    yLOG_snote   ("depth NOT supported");
      DEBUG_YMAP    yLOG_sexitr  (__FUNCTION__, rce);
      return rce;
      break;
   case YMAP_WHEN  :
      DEBUG_YMAP    yLOG_snote   ("when NOT supported");
      DEBUG_YMAP    yLOG_sexitr  (__FUNCTION__, rce);
      return rce;
      break;
   default :
      DEBUG_YMAP    yLOG_snote   ("axis not understood");
      DEBUG_YMAP    yLOG_sexitr  (__FUNCTION__, rce);
      return rce;
      break;
   }
   DEBUG_YMAP    yLOG_sexit   (__FUNCTION__);
   return 0;
}

char
api_ymap_done           (void)
{
   /*---(locals)-----------+-----+-----+-*/
   char        t           [LEN_LABEL] = "";
   char        x_mute      =    0;
   /*---(header)-------------------------*/
   DEBUG_YMAP    yLOG_enter   (__FUNCTION__);
   /*---(update current)-------*/
   /*---(complete)-----------------------*/
   DEBUG_YMAP    yLOG_exit    (__FUNCTION__);
   return 0;
}
