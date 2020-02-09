/*=================================(beg-code)=================================*/
#include "gregg.h"



PRIV char    stype     = '-';          /* p=prefix, o=outline, c=continue     */


/*
 *
 *   raw data points are very jagged and clunky.  the tend to only move one
 *   pixel at a time and only horizontally and vertically.  but, cleansing
 *   raw points before recording them can eliminate later clues.  i feel it is
 *   better to over-collect and clean in a later stage.
 *
 *   BUT, i need some consistency between collection methods so its not crazy.
 *   therefore, my perfect drawing space is 3.5" by 5.0".  i will size raw to
 *   10x this space since i will NOT use any whole drawing tablet.  remember
 *   gregg is fast because the writing is not large and flows.
 *
 *   my end raw will match by screen design.  x = left -125 to right 375,
 *   and y = top 125 to bottom -225.  raw points will center on zero.  so, raw
 *   will be x = -1,250 to 3,750 and y = 1,250 to -2,250.  10 to 1.
 *
 *   START and FINISH points are duplicates.  they are created to enable later
 *   checking for circles.  the START and FINISH points can be "extended" to
 *   look for reasonable intersections.  while this could be done temporarily
 *   later, i found it programatically useful to do it early.
 *
 *   when storing outlines, i will store only raw points and the final answer.
 *   the reason is that later algorithms may find additional alternatives or
 *   different answers provided the same points.  why not allow for
 *   reinterpretation.
 *
 *   the second "modification" to the raw source data i will currently allow is
 *   baselining the start point to zero.  this is incredibly useful in crazy
 *   pen/touch based applications and allows easier cross outline comparisions.
 *
 *
 */



/*============================--------------------============================*/
/*===----                       program level                          ----===*/
/*============================--------------------============================*/
static void o___PROGRAM________________o (void) {;}

char
RAW_init             (void)
{
   o.nraw     = 0;
   o.craw     = 0;
   o.xadj     = 0;
   o.yadj     = 0;
   POINT_clear_series (POINTS_RAW);
   return 0;
}



/*============================--------------------============================*/
/*===----                      adding raw points                       ----===*/
/*============================--------------------============================*/
static void o___NEW____________________o (void) {;}


char     /*----: add a new raw point to outline -------------------------*/
RAW__point         (int a_xpad, int a_ypad, char a_type)
{
   /*---(locals)-----------+-----+-----+-*/
   char        rce         =  -10;
   char        x_dup       =    0;
   char        x_last      =  '-';
   /*---(header)-------------------------*/
   DEBUG_RAW    yLOG_senter  (__FUNCTION__);
   DEBUG_RAW    yLOG_sint    (a_xpad);
   DEBUG_RAW    yLOG_sint    (a_ypad);
   DEBUG_RAW    yLOG_schar   (a_type);
   /*---(defenses)-----------------------*/
   DEBUG_RAW    yLOG_sint    (o.nraw);
   DEBUG_RAW    yLOG_sint    (MAX_POINTS);
   /*---(max)------------------*/
   --rce;  if (o.nraw >= LEN_HUGE) {
      DEBUG_RAW    yLOG_snote   ("maxed out");
      DEBUG_RAW    yLOG_sexitr  (__FUNCTION__, rce);
      return rce;
   }
   /*---(type)-----------------*/
   --rce;  if (strchr (POINT_TYPES, a_type) == NULL) {
      DEBUG_RAW    yLOG_snote   ("type not valid");
      DEBUG_RAW    yLOG_sexitr  (__FUNCTION__, rce);
      return rce;
   }
   /*---(starts)---------------*/
   --rce;  if (POINT_seq_start  (a_type, &(o.raw), o.nraw) < 0) {
      DEBUG_RAW    yLOG_sexitr  (__FUNCTION__, rce);
      return rce;
   }
   /*---(heads)----------------*/
   --rce;  if (POINT_seq_head   (a_type, &(o.raw), o.nraw, a_xpad, a_ypad) < 0) {
      DEBUG_RAW    yLOG_sexitr  (__FUNCTION__, rce);
      return rce;
   }
   /*---(tails)----------------*/
   --rce;  if (POINT_seq_tail   (a_type, &(o.raw), o.nraw, a_xpad, a_ypad) < 0) {
      DEBUG_RAW    yLOG_sexitr  (__FUNCTION__, rce);
      return rce;
   }
   /*---(finishes)-------------*/
   --rce;  if (POINT_seq_finish (a_type, &(o.raw), o.nraw, a_xpad, a_ypad) < 0) {
      DEBUG_RAW    yLOG_sexitr  (__FUNCTION__, rce);
      return rce;
   }
   /*---(normals)--------------*/
   --rce;  if (POINT_seq_normal (a_type, &(o.raw), o.nraw, a_xpad, a_ypad) < 0) {
      DEBUG_RAW    yLOG_sexitr  (__FUNCTION__, rce);
      return rce;
   }
   /*---(save point)---------------------*/
   DEBUG_RAW    yLOG_snote   ("add point");
   o.raw [o.nraw].series  = POINTS_RAW;
   o.raw [o.nraw].seq     = o.nraw;
   o.raw [o.nraw].p_raw   = o.nraw;
   o.raw [o.nraw].x_touch = o.raw [o.nraw].x_raw = a_xpad;
   o.raw [o.nraw].y_touch = o.raw [o.nraw].y_raw = a_ypad;
   o.raw [o.nraw].type    = a_type;
   /*---(save point)---------------------*/
   DEBUG_RAW    yLOG_snote   ("assign type/calc");
   switch (a_type) {
   case POINT_START   :
      o.raw [o.nraw].fake = POINT_FAKE;
      break;
   case POINT_HEAD    :
      o.raw [o.nraw].fake = POINT_NORMAL;
      break;
   case POINT_NORMAL  :
      o.raw [o.nraw].fake = POINT_NORMAL;
      break;
   case POINT_TAIL    :
      o.raw [o.nraw].fake = POINT_NORMAL;
      break;
   case POINT_FINISH  :
      o.raw [o.nraw].fake = POINT_FAKE;
      break;
   }
   /*---(calculate)----------------------*/
   POINT_pos (o.raw + o.nraw);
   /*---(update counters)----------------*/
   ++o.nraw;
   DEBUG_RAW    yLOG_sint    (o.nraw);
   /*---(complete)-----------------------*/
   DEBUG_RAW    yLOG_sexit   (__FUNCTION__);
   return 0;
}



/*============================--------------------============================*/
/*===----                        public functions                      ----===*/
/*============================--------------------============================*/
static void o___PUBLIC_________________o (void) {;}

char
RAW_touch            (int a_xpad, int a_ypad)
{
   /*---(locals)-----------+-----+-----+-*/
   char        rc          =    0;
   int         r           =  0.0;
   /*---(calculate)----------------------*/
   r  = sqrt((a_xpad * a_xpad) + (a_ypad * a_ypad));
   /*---(assign type)--------------------*/
   /*> OUT_clear ();                                                                  <* 
    *> stype   = PART_MAIN;                                                           <* 
    *> o.xadj  = a_xpad;                                                              <* 
    *> o.yadj  = a_ypad;                                                              <*/
   if (r < SIZE_MED_AVG) {
      if (stype != 'p')  OUT_clear ();
      stype   = PART_MAIN;
      o.xadj  = a_xpad;
      o.yadj  = a_ypad;
   } else if (a_xpad < 0.0 && a_ypad > 0.0) {
      OUT_clear ();
      stype   = PART_PREFIX;
   } else {
      stype   = PART_CONTINUE;
   }
   /*---(save points)--------------------*/
   if (rc == 0)  rc = RAW__point (a_xpad, a_ypad, POINT_START);
   if (rc == 0)  rc = RAW__point (a_xpad, a_ypad, POINT_HEAD);
   /*---(complete)-----------------------*/
   return rc;
}

char
RAW_normal           (int a_xpad, int a_ypad)
{
   /*---(locals)-----------+-----+-----+-*/
   char        rc          =    0;
   /*---(process)------------------------*/
   if (rc == 0)  rc = RAW__point (a_xpad, a_ypad, POINT_NORMAL);
   /*---(complete)-----------------------*/
   return rc;
}

char
RAW_lift             (int a_xpad, int a_ypad)
{
   /*---(locals)-----------+-----+-----+-*/
   char        rc          =    0;
   /*---(header)-------------------------*/
   DEBUG_RAW    yLOG_enter   (__FUNCTION__);
   /*---(save points)--------------------*/
   if (rc == 0)       RAW__point (a_xpad, a_ypad, POINT_TAIL);
   if (rc == 0)  rc = RAW__point (a_xpad, a_ypad, POINT_FINISH);
   if (rc <  0)  {
      DEBUG_RAW    yLOG_exitr   (__FUNCTION__, rc);
      return rc;
   }
   /*---(process)------------------------*/
   if (rc == 0)  rc = RAW_equalize  ();
   if (rc == 0)  rc = BASE_filter   ();
   POINT_list (stdout, 'd', o.bas, o.nbas);
   if (rc == 0)  rc = BASE_add_corners ();
   POINT_list (stdout, 'd', o.bas, o.nbas);
   if (rc == 0)  rc = CIRCLE_driver ();
   POINT_list (stdout, 'd', o.bas, o.nbas);
   /*> if (rc == 0)  rc = KEY_driver    ();                                           <*/
   /*> if (rc == 0)  rc = KEY_filter    ();                                           <* 
    *> if (rc == 0)  rc = KEY_flatten   ();                                           <* 
    *> if (rc == 0)  rc = KEY_squeeze   ();                                           <* 
    *> if (rc == 0)  rc = KEY_sharps    ();                                           <*/
   /*> if (rc == 0)  rc = CIRCLE_driver_OLD ();                                       <*/
   /*> if (rc == 0)  rc = MATCH_driver  ();                                           <*/
   /*> POINT_list (stdout, 'd', o.bas, o.nbas);                                       <*/
   /*> POINT_list (stdout, 'd', o.key, o.nkey);                                       <*/
   if (rc <  0) {
      printf ("dropped out early\n");
      return rc;
   }
   /*> if (rc == 0)  rc = OUT_append    ();                                           <*/
   /*---(complete)-----------------------*/
   DEBUG_RAW    yLOG_exit    (__FUNCTION__);
   return 0;
}



/*============================--------------------============================*/
/*===----                          mass data load                      ----===*/
/*============================--------------------============================*/
static void o___MASS______________________o (void) {;}

char
RAW_load           (char *a_points)
{
   /*---(locals)-------------------------*/
   char     *p         = NULL;         /* coord pointer                       */
   char     *q         = ";,";         /* point delimiters                    */
   char     *r         = NULL;         /* context                             */
   int       x         =    0;
   int       y         =    0;
   char      x_type    =  '-';
   /*---(header)-------------------------*/
   DEBUG_RAW    yLOG_enter   (__FUNCTION__);
   /*---(read head)----------------------*/
   p = strtok_r (a_points, q, &r);
   /*---(get point pairs)----------------*/
   while (p != NULL) {
      /*> printf ("[%s]\n", p);                                                       <*/
      switch (p[0]) {
      case 'T' :
         DEBUG_RAW    yLOG_note    ("TOUCH");
         /*> printf ("   touch type\n");                                              <*/
         x_type = 'T';
         p = strtok_r (NULL, q, &r);
         break;
      case 'L' :
         DEBUG_RAW    yLOG_note    ("LIFT");
         /*> printf ("   lift type\n");                                               <*/
         x_type = 'L';
         p = strtok_r (NULL, q, &r);
         break;
      default :
         x_type = '-';
         /*> printf ("   normal type\n");                                             <*/
         break;
      }
      if (p == NULL)  break;
      x = atoi (p);
      DEBUG_RAW    yLOG_complex ("x-pos"     , "%s, %dy", p, x);
      p = strtok_r (NULL, q, &r);
      if (p == NULL)  break;
      y = atoi (p);
      DEBUG_RAW    yLOG_complex ("y-pos"     , "%s, %dy", p, y);
      switch (x_type) {
      case 'T' :
         RAW_touch  (x, y);
         o.saved = 'y';
         break;
      case 'L' :
         RAW_lift   (x, y);
         break;
      default  :
         RAW_normal (x, y);
         break;
      }
      p = strtok_r (NULL, q, &r);
      if (p == NULL)  break;
   }
   /*---(complete)-----------------------*/
   DEBUG_RAW    yLOG_exit    (__FUNCTION__);
   return 0;
}

char          /*----: make raw xy relative to neutral origin -----------------*/
RAW_equalize        (void)
{
   /*---(locals)-----------+-----+-----+-*/
   int         i           =    0;
   /*---(header)-------------------------*/
   DEBUG_RAW    yLOG_enter   (__FUNCTION__);
   /*---(prepare)------------------------*/
   o.xmin = +20000;
   o.xmax = -20000;
   o.ymin = +20000;
   o.ymax = -20000;
   DEBUG_RAW    yLOG_complex ("adj"       , "%dx, %dy", o.xadj, o.yadj);
   /*---(adjust)-------------------------*/
   for (i = 0; i < o.nraw; ++i) {
      /*> POINT_display (o.raw + i);                                                  <*/
      DEBUG_RAW    yLOG_complex ("before"    , "%3d, %4dx, %4dy", i, o.raw [i].x_touch, o.raw [i].y_touch);
      /*---(adjust pos)------------------*/
      o.raw [i].x_raw = o.raw [i].x_touch - o.xadj;
      o.raw [i].y_raw = o.raw [i].y_touch - o.yadj;
      DEBUG_RAW    yLOG_complex ("after"     , "%3d, %4dx, %4dy", i, o.raw [i].x_raw, o.raw [i].y_raw);
      /*---(find min/max)----------------*/
      if (o.xmin > o.raw [i].x_raw)   o.xmin = o.raw [i].x_raw;
      if (o.ymin > o.raw [i].y_raw)   o.ymin = o.raw [i].y_raw;
      if (o.xmax < o.raw [i].x_raw)   o.xmax = o.raw [i].x_raw;
      if (o.ymax < o.raw [i].y_raw)   o.ymax = o.raw [i].y_raw;
      /*---(done)------------------------*/
   }
   DEBUG_RAW    yLOG_complex ("min"       , "%dx, %dy", o.xmin, o.ymin);
   DEBUG_RAW    yLOG_complex ("max"       , "%dx, %dy", o.xmax, o.ymax);
   /*---(complete)-----------------------*/
   DEBUG_RAW    yLOG_exit    (__FUNCTION__);
   return 0;
}



/*============================--------------------============================*/
/*===----                         unit testing                         ----===*/
/*============================--------------------============================*/
static void o___UNIT_TEST_________________o (void) {;}

char        unit_answer  [LEN_STR];

char*        /*-> unit test accessor -----------------[ light  [us.D90.241.L0]*/ /*-[03.0000.00#.#]-*/ /*-[--.---.---.--]-*/
RAW__unit            (char *a_question, int a_num)
{
   /*---(locals)-----------+-----+-----+-*/
   int         i           = 0;
   char        t           [100] = "";
   char        s           [100] = "";
   int         x_end       = 0;
   int         y_end       = 0;
   /*---(initialize)---------------------*/
   strlcpy (unit_answer, "RAW unit         : unknownn request", 100);
   /*---(core data)----------------------*/
   if        (strncmp (a_question, "count"     , 20)  == 0) {
      snprintf (unit_answer, LEN_STR, "RAW count        : %d", o.nraw);
   } else if (strncmp (a_question, "types"     , 20)  == 0) {
      for (i = 0; i < 45; ++i) {
         if (i >= o.nraw)  break;
         t [i]     = o.raw [i].type;
         t [i + 1] = 0;
      }
      if (o.nraw > 45)  sprintf (s, "[%s>", t);
      else              sprintf (s, "[%s]", t);
      snprintf (unit_answer, LEN_STR, "RAW by type      : %2d %s", o.nraw, s);
   }
   else if   (strncmp (a_question, "point"     , 20)  == 0) {
      snprintf (unit_answer, LEN_STR, "RAW point   (%2d) : %c  %4dx %4dy  %4dx %4dy", a_num, o.raw [a_num].type, o.raw [a_num].x_touch, o.raw [a_num].y_touch, o.raw [a_num].x_raw, o.raw [a_num].y_raw);
   }
   else if   (strncmp (a_question, "adjust"    , 20)  == 0) {
      snprintf (unit_answer, LEN_STR, "RAW adjust       : %4dx %4dy", o.xadj, o.yadj);
   }
   else if   (strncmp (a_question, "bounds"    , 20)  == 0) {
      if (o.nraw < 1)  { x_end = o.raw [0].x_raw; y_end = o.raw [0].y_raw; }
      else             { x_end = o.raw [o.nraw - 1].x_raw; y_end = o.raw [o.nraw - 1].y_raw; }
      snprintf (unit_answer, LEN_STR, "RAW bounds       : beg %4dx %4dy   end %4dx %4dy   min %4.0fx %4.0fy   max %4.0fx %4.0fy",
            o.raw[0].x_raw, o.raw[0].y_raw,
            x_end         , y_end,
            o.xmin        , o.ymin,
            o.xmax        , o.ymax);
   }
   /*---(complete)-----------------------*/
   return unit_answer;
}



/*=================================(end-code)=================================*/
