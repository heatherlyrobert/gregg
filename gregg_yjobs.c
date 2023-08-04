/*============================---(source-start)---============================*/
#include    "gregg.h"


/*
 * metis § ····· § create a binary database for normal use                                § N33N0F §  · §
 * metis § ····· § add gather for pulling dictionaries and and making database            § N33N0c §  · §
 *
 *
 */




/*====================------------------------------------====================*/
/*===----                        main driver                           ----===*/
/*====================------------------------------------====================*/
static void  o___DRIVER__________o () { return; }

char
gregg_yjobs             (cchar a_req, cchar *a_data)
{
   /*---(locals)-----------+-----+-----+-*/
   char        rc          =    0;
   int         l           =    0;
   /*---(header)-------------------------*/
   DEBUG_PROG    yLOG_enter   (__FUNCTION__);
   DEBUG_PROG    yLOG_char    ("a_req"     , a_req);
   DEBUG_PROG    yLOG_info    ("a_data"    , a_data);
   /*---(handle)-------------------------*/
   switch (a_req) {
   case YJOBS_READ     :
      DEBUG_PROG    yLOG_note    ("read database");
      if (!yJOBS_ifgather ())  rc = DB_read     (NAME_DB);
      break;
   case YJOBS_STATS    :
      DEBUG_PROG    yLOG_note    ("called for stats");
      rc = DB_stats (NAME_DB);
      break;
   case YJOBS_WRITE    :
      DEBUG_PROG    yLOG_note    ("write database");
      rc = DB_write    (NAME_DB);
      break;
   case YJOBS_PULL     :
      DEBUG_PROG    yLOG_note    ("called for pull");
      rc = DICT_import (a_data);
      break;
   case YJOBS_LOCALRPT : 
      DEBUG_PROG    yLOG_note    ("called for localrpt");
      /*> l = strlen (a_data);                                                        <* 
       *> if (a_data [l - 1] == '/')   my.source = DATA_SOURCES;                      <* 
       *> else                         my.source = DATA_FILE;                         <* 
       *> my.source = DATA_FILE;                                                      <* 
       *> strlcpy (my.file, a_data, LEN_PATH);                                        <*/
      break;
   case YJOBS_REPORT   :
      DEBUG_PROG    yLOG_note    ("called for report");
      /*> rc = metis_inventory ();                                                    <*/
      /*> rc = metis_rptg_matrix ();                                                  <*/
      break;
   /*> case YJOBS_ONLY     :                                                          <* 
    *>    DEBUG_PROG    yLOG_note    ("called for only");                             <* 
    *>    break;                                                                      <*/
   case YJOBS_PURGE    :
      DEBUG_PROG    yLOG_note    ("called for purge");
      rc = WORDS_purge ();
      break;
   }
   /*---(trouble)------------------------*/
   if (rc < 0) {
      DEBUG_PROG    yLOG_exitr   (__FUNCTION__, rc);
      return rc;
   }
   /*---(complete)-----------------------*/
   DEBUG_PROG    yLOG_exit    (__FUNCTION__);
   return 0;
}
