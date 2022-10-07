/*============================---(source-start)---============================*/
#include    "gregg.h"



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
   DEBUG_YMAP    yLOG_senter  (__FUNCTION__);
   DEBUG_YMAP    yLOG_schar   (a_axis);
   --rce;  switch (a_axis) {
   case YMAP_UNIV  :
      DEBUG_YMAP    yLOG_snote   ("universe NOT supported in arachne");
      DEBUG_YMAP    yLOG_sexitr  (__FUNCTION__, rce);
      return rce;
      break;
   case YMAP_XAXIS :
      DEBUG_YMAP    yLOG_snote   ("cols");
      /*> DEBUG_YMAP    yLOG_sint    (NCOL);                                          <*/
      /*> if (n != NULL)  *n = NCOL;                                                  <* 
       *> if (a != NULL)  yVIEW_size (YVIEW_MAIN, NULL, NULL, a, NULL, NULL);         <* 
       *> if (b != NULL)  *b = BCOL;                                                  <* 
       *> if (c != NULL)  *c = CCOL;                                                  <* 
       *> if (e != NULL)  *e = ECOL;                                                  <* 
       *> if (m != NULL)  *m = COL_min_used (CTAB);                                   <* 
       *> if (x != NULL)  *x = COL_max_used (CTAB);                                   <*/
      break;
   case YMAP_YAXIS :
      DEBUG_YMAP    yLOG_snote   ("rows");
      /*> DEBUG_YMAP    yLOG_sint    (NROW);                                          <*/
      /*> if (n != NULL)  *n = NROW;                                                  <* 
       *> if (a != NULL)  yVIEW_size (YVIEW_MAIN, NULL, NULL, NULL, NULL, a);         <* 
       *> if (b != NULL)  *b = BROW;                                                  <* 
       *> if (c != NULL)  *c = CROW;                                                  <* 
       *> if (e != NULL)  *e = EROW;                                                  <* 
       *> if (m != NULL)  *m = ROW_min_used (CTAB);                                   <* 
       *> if (x != NULL)  *x = ROW_max_used (CTAB);                                   <*/
      break;
   case YMAP_ZAXIS :
      DEBUG_YMAP    yLOG_snote   ("depth NOT supported in gyges");
      DEBUG_YMAP    yLOG_sexitr  (__FUNCTION__, rce);
      return rce;
   case YMAP_WHEN  :
      DEBUG_YMAP    yLOG_snote   ("when NOT supported in gyges");
      DEBUG_YMAP    yLOG_sexitr  (__FUNCTION__, rce);
      return rce;
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
   DEBUG_YMAP    yLOG_enter   (__FUNCTION__);
   --rce;  switch (a_axis) {
   case YMAP_UNIV  :
      /*> rc = TAB_entry  (a_pos, r_ref, r_wide, r_used);                             <*/
      break;
   case YMAP_XAXIS :
      /*> rc = COL_entry  (a_pos, r_ref, r_wide, r_used);                             <*/
      break;
   case YMAP_YAXIS :
      /*> rc = ROW_entry  (a_pos, r_ref, r_wide, r_used);                             <*/
      break;
   case YMAP_ZAXIS :
   case YMAP_WHEN  :
      /*> if (r_ref  != NULL)  *r_ref  = 0;                                           <* 
       *> if (r_wide != NULL)  *r_wide = 0;                                           <* 
       *> if (r_used != NULL)  *r_used = 0;                                           <*/
      break;
   default :
      DEBUG_YMAP    yLOG_exitr   (__FUNCTION__, rce);
      return rce;
      break;
   }
   DEBUG_YMAP    yLOG_value   ("rc"        , rc);
   DEBUG_YMAP    yLOG_exit    (__FUNCTION__);
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
      DEBUG_YMAP    yLOG_snote   ("univ NOT ALLOWED");
      DEBUG_YMAP    yLOG_sexitr  (__FUNCTION__, rce);
      return rce;
      break;
   case YMAP_XAXIS :
      DEBUG_YMAP    yLOG_snote   ("cols");
      /*> PTAB->bcol = BCOL = b;                                                      <* 
       *> PTAB->ccol = CCOL = c;                                                      <* 
       *> PTAB->ecol = ECOL = e;                                                      <*/
      break;
   case YMAP_YAXIS :
      DEBUG_YMAP    yLOG_snote   ("rows");
      /*> PTAB->brow = BROW = b;                                                      <* 
       *> PTAB->crow = CROW = c;                                                      <* 
       *> PTAB->erow = EROW = e;                                                      <*/
      break;
   case YMAP_ZAXIS :
      DEBUG_YMAP    yLOG_snote   ("depth NOT supported in gyges");
      DEBUG_YMAP    yLOG_sexitr  (__FUNCTION__, rce);
      return rce;
   case YMAP_WHEN  :
      DEBUG_YMAP    yLOG_snote   ("when NOT supported in gyges");
      DEBUG_YMAP    yLOG_sexitr  (__FUNCTION__, rce);
      return rce;
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
