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
 *   START and FINISH points are duplicates.  they are created to enable later
 *   checking for circles.  the START and FINISH points can be "extended" to
 *   look for reasonable intersections.  while this could be done temporarily
 *   later, i found it programatically useful to do it early.
 *
 *   when i store outlines in databases, i will either store the raw data points
 *   or final answer, likely both.  the reason is that later algorithms may
 *   find additional alternatives or different answers provided the same points.
 *   why not allow for reinterpretation.
 *
 *   the only "modification" to the raw source data i will currently allow is
 *   baselining the start point to zero.  this is incredibly useful in crazy
 *   pen/touch based applications and allows easier cross outline comparisions.
 *
 *
 */



/*============================--------------------============================*/
/*===----                          raw points                          ----===*/
/*============================--------------------============================*/
static void o___RAW_POINTS_____________o (void) {;}


char     /*----: add a new raw point to outline -------------------------*/
RAW__point         (int a_xpad, int a_ypad, char a_type)
{
   /*---(locals)-----------+-----+-----+-*/
   char        rce         =  -10;
   /*---(header)-------------------------*/
   DEBUG_RAW    yLOG_senter  (__FUNCTION__);
   DEBUG_RAW    yLOG_svalue  ("x"         , a_xpad);
   DEBUG_RAW    yLOG_svalue  ("y"         , a_ypad);
   DEBUG_RAW    yLOG_schar   (a_type);
   /*---(defenses)-----------------------*/
   DEBUG_RAW    yLOG_svalue  ("#"         , o.nraw);
   DEBUG_RAW    yLOG_svalue  ("m"         , MAX_POINTS);
   /*---(max)------------------*/
   --rce;  if (o.nraw >= MAX_POINTS) {
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
   --rce;  if (o.nraw == 0 && a_type != POINT_START) {
      DEBUG_RAW    yLOG_snote   ("zero must be S");
      DEBUG_RAW    yLOG_sexitr  (__FUNCTION__, rce);
      return rce;
   }
   --rce;  if (o.nraw > 0 && a_type == POINT_START && o.raw [o.nraw - 1].type != POINT_FINISH) {
      DEBUG_RAW    yLOG_snote   ("S must follow F");
      DEBUG_RAW    yLOG_sexitr  (__FUNCTION__, rce);
      return rce;
   }
   /*---(heads)----------------*/
   --rce;  if (o.nraw > 0 && o.raw[o.nraw - 1].type == POINT_START && a_type != POINT_HEAD) {
      DEBUG_RAW    yLOG_snote   ("> must follow S");
      DEBUG_RAW    yLOG_sexitr  (__FUNCTION__, rce);
      return rce;
   }
   --rce;  if (a_type == POINT_HEAD && o.raw [o.nraw - 1].type != POINT_START) {
      DEBUG_RAW    yLOG_snote   ("S must preceed >");
      DEBUG_RAW    yLOG_sexitr  (__FUNCTION__, rce);
      return rce;
   }
   /*---(finishes)-------------*/
   --rce;  if (a_type == POINT_FINISH && (a_xpad != o.raw[o.nraw - 1].x_full || a_ypad != o.raw[o.nraw - 1].y_full)) {
      DEBUG_RAW    yLOG_snote   ("F must match last -");
      DEBUG_RAW    yLOG_sexitr  (__FUNCTION__, rce);
      return rce;
   }
   --rce;  if (a_type == POINT_FINISH && o.raw [o.nraw - 1].type != POINT_NORMAL) {
      DEBUG_RAW    yLOG_snote   ("F must folllow -");
      DEBUG_RAW    yLOG_sexitr  (__FUNCTION__, rce);
      return rce;
   }
   /*---(normals)--------------*/
   --rce;  if (a_type == POINT_NORMAL && (o.raw [o.nraw - 1].type != POINT_HEAD && o.raw [o.nraw - 1].type != POINT_NORMAL)) {
      DEBUG_RAW    yLOG_snote   ("- must follow > or -");
      DEBUG_RAW    yLOG_sexitr  (__FUNCTION__, rce);
      return rce;
   }
   --rce;  if (o.nraw > 0 && a_type == POINT_NORMAL && a_xpad == o.raw[o.nraw - 1].x_full) {
      DEBUG_RAW    yLOG_snote   ("duplicate x-pos -");
      DEBUG_RAW    yLOG_sexitr  (__FUNCTION__, rce);
      return rce;
   }
   --rce;  if (o.nraw > 0 && a_type == POINT_NORMAL && a_ypad == o.raw[o.nraw - 1].y_full) {
      DEBUG_RAW    yLOG_snote   ("duplicate y-pos -");
      DEBUG_RAW    yLOG_sexitr  (__FUNCTION__, rce);
      return rce;
   }
   /*---(save point)---------------------*/
   DEBUG_RAW    yLOG_snote   ("add point");
   o.raw[o.nraw].p_raw   = o.nraw;
   o.raw[o.nraw].x_full  = a_xpad;
   o.raw[o.nraw].y_full  = a_ypad;
   o.raw[o.nraw].type    = a_type;
   /*---(save point)---------------------*/
   DEBUG_RAW    yLOG_snote   ("assign type/calc");
   switch (a_type) {
   case POINT_START   :
      o.raw[o.nraw].fake = POINT_FAKE;
      break;
   case POINT_FINISH  :
      o.raw[o.nraw].fake = POINT_FAKE;
      break;
   case POINT_HEAD    :
      o.raw[o.nraw].fake = POINT_NORMAL;
      break;
   case POINT_NORMAL  :
      o.raw[o.nraw].fake = POINT_NORMAL;
      break;
   }
   POINT_calc (POINTS_RAW, o.raw + o.nraw, 'n');
   /*---(update counters)----------------*/
   ++o.nraw;
   DEBUG_RAW    yLOG_svalue  ("#"         , o.nraw);
   /*---(complete)-----------------------*/
   DEBUG_RAW    yLOG_sexit   (__FUNCTION__);
   return 0;
}

char
RAW_touch            (int a_xpad, int a_ypad)
{
   /*---(locals)-----------+-----+-----+-*/
   char        rc          =    0;
   int         r           =  0.0;
   /*---(calculate)----------------------*/
   r  = sqrt((a_xpad * a_xpad) + (a_ypad * a_ypad));
   /*---(assign type)--------------------*/
   OUT_clear ();
   stype   = PART_MAIN;
   o.xadj  = a_xpad;
   o.yadj  = a_ypad;
   o.ratio = my.ratio;
   /*> if (r < SIZE_MED_AVG) {                                                        <* 
    *>    if (stype != 'p')  OUT_clear ();                                            <* 
    *>    stype   = PART_MAIN;                                                        <* 
    *>    o.xadj  = a_xpad;                                                              <* 
    *>    o.yadj  = a_ypad;                                                              <* 
    *> } else if (a_xpad < 0.0 && a_ypad > 0.0) {                                           <* 
    *>    OUT_clear ();                                                               <* 
    *>    stype   = PART_PREFIX;                                                      <* 
    *> } else {                                                                       <* 
    *>    stype   = PART_CONTINUE;                                                    <* 
    *> }                                                                              <*/
   /*---(save points)--------------------*/
   if (rc == 0)  rc = RAW__point (a_xpad, a_ypad, POINT_START );
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
   /*---(save points)--------------------*/
   if (rc == 0)       RAW__point (a_xpad, a_ypad, POINT_NORMAL);
   if (rc == 0)  rc = RAW__point (a_xpad, a_ypad, POINT_FINISH);
   if (rc <  0)  return rc;
   /*---(process)------------------------*/
   if (rc == 0)  rc = RAW_equalize  ();
   if (rc == 0)  rc = BASE_filter  ();
   if (rc == 0)  rc = KEY_filter    ();
   /*> if (rc == 0)  rc = match_flatten ();                                           <* 
    *> if (rc == 0)  rc = match_squeeze ();                                           <* 
    *> if (rc == 0)  rc = circle_driver ();                                           <* 
    *> if (rc == 0)  rc = match_sharps  ();                                           <* 
    *> if (rc == 0)  rc = MATCH_driver  ();                                           <*/
   if (rc <  0)  return rc;
   /*> if (rc == 0)  rc = OUT_append    ();                                           <*/
   /*---(complete)-----------------------*/
   return 0;
}

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
   /*---(read head)----------------------*/
   p = strtok_r (a_points, q, &r);
   /*---(get point pairs)----------------*/
   while (p != NULL) {
      /*> printf ("[%s]\n", p);                                                       <*/
      switch (p[0]) {
      case 'T' :
         /*> printf ("   touch type\n");                                              <*/
         x_type = 'T';
         p = strtok_r (NULL, q, &r);
         break;
      case 'L' :
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
      p = strtok_r (NULL, q, &r);
      if (p == NULL)  break;
      y = atoi (p);
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
   return 0;
}

char          /*----: make raw xy relative to neutral origin -----------------*/
RAW_equalize        (void)
{
   /*---(locals)-----------+-----+-----+-*/
   int         i           =    0;
   /*---(adjust)-------------------------*/
   /*> for (i = 0; i < o.nraw; ++i) {                                                 <* 
    *>    POINT_display (o.raw + i);                                                  <* 
    *>    if (o.x_min > o.raw [i].x_pos)   o.x_min = o.raw [i].x_pos;                 <* 
    *>    if (o.y_min > o.raw [i].y_pos)   o.y_min = o.raw [i].y_pos;                 <* 
    *>    if (o.x_max < o.raw [i].x_pos)   o.x_max = o.raw [i].x_pos;                 <* 
    *>    if (o.y_max < o.raw [i].y_pos)   o.y_max = o.raw [i].y_pos;                 <* 
    *> }                                                                              <*/
   return 0;
}

char        unit_answer  [LEN_STR];

char*        /*-> unit test accessor -----------------[ light  [us.D90.241.L0]*/ /*-[03.0000.00#.#]-*/ /*-[--.---.---.--]-*/
RAW__unit            (char *a_question, int a_num)
{
   /*---(locals)-----------+-----+-----+-*/
   int         i           = 0;
   char        t           [100] = "";
   /*---(initialize)---------------------*/
   strlcpy (unit_answer, "RAW__unit, unknown request", 100);
   /*---(core data)----------------------*/
   if        (strncmp (a_question, "count"     , 20)  == 0) {
      snprintf (unit_answer, LEN_STR, "RAW count        : %d", o.nraw);
   } else if (strncmp (a_question, "types"     , 20)  == 0) {
      for (i = 0; i < 45; ++i) {
         if (i >= o.nraw)  break;
         t [i]     = o.raw [i].type;
         t [i + 1] = 0;
      }
      snprintf (unit_answer, LEN_STR, "RAW types        : %2d [%-45.45s]", o.nraw, t);
   }
   /*---(complete)-----------------------*/
   return unit_answer;
}

/*> char*                                                                                                                                  <* 
 *> unit_accessor(char *a_question, int a_num)                                                                                             <* 
 *> {                                                                                                                                      <* 
 *>    if        (strcmp(a_question, "raw")         == 0) {                                                                                <* 
 *>       if (o.nraw == 0) {                                                                                                               <* 
 *>          snprintf(unit_answer, LEN_TEXT, "Raw Point (%4d) : there are no dots", a_num);                                                <* 
 *>       } else if (a_num < 0) {                                                                                                          <* 
 *>          snprintf(unit_answer, LEN_TEXT, "Raw Point (%4d) : can not be negative", a_num);                                              <* 
 *>       } else if (a_num >= o.nraw) {                                                                                                    <* 
 *>          snprintf(unit_answer, LEN_TEXT, "Raw Point (%4d) : out of range %3d to %3d", a_num, 0, o.nraw - 1);                           <* 
 *>       } else                                                                                                                           <* 
 *>          snprintf(unit_answer, LEN_TEXT, "Raw Point (%4d) : %4dx, %4dy, %4dc", a_num, o.raw[a_num].x_full, o.raw[a_num].y_full, o.nraw);   <* 
 *>    } else if        (strcmp(a_question, "bas")         == 0) {                                                                         <* 
 *>       if (o.nbas == 0) {                                                                                                               <* 
 *>          snprintf(unit_answer, LEN_TEXT, "Bas Point (%4d) : there are no dots", a_num);                                                <* 
 *>       } else if (a_num < 0) {                                                                                                          <* 
 *>          snprintf(unit_answer, LEN_TEXT, "Bas Point (%4d) : can not be negative", a_num);                                              <* 
 *>       } else if (a_num >= o.nbas) {                                                                                                    <* 
 *>          snprintf(unit_answer, LEN_TEXT, "Bas Point (%4d) : out of range %3d to %3d", a_num, 0, o.nraw - 1);                           <* 
 *>       } else                                                                                                                           <* 
 *>          snprintf(unit_answer, LEN_TEXT, "Bas Point (%4d) : %4dx, %4dy, %4dc", a_num, o.raw[a_num].x_full, o.raw[a_num].y_full, o.nraw);   <* 
 *>    } else if        (strcmp(a_question, "num_raw")     == 0) {                                                                         <* 
 *>       snprintf(unit_answer, LEN_TEXT, "Raw Count        : %4dc", o.nraw);                                                              <* 
 *>    } else if        (strcmp(a_question, "num_bas")     == 0) {                                                                         <* 
 *>       snprintf(unit_answer, LEN_TEXT, "Bas Count        : %4dc", o.nbas);                                                              <* 
 *>    } else if        (strcmp(a_question, "num_avg")     == 0) {                                                                         <* 
 *>       snprintf(unit_answer, LEN_TEXT, "Avg Count        : %4dc", o.navg);                                                              <* 
 *>    } else if        (strcmp(a_question, "num_key")     == 0) {                                                                         <* 
 *>       snprintf(unit_answer, LEN_TEXT, "Key Count        : %4dc", o.nkey);                                                              <* 
 *>    } else if        (strcmp(a_question, "prepared")    == 0) {                                                                         <* 
 *>       snprintf(unit_answer, LEN_TEXT, "Prepared  (%4d) : %s", o.curr, o.grade);                                                        <* 
 *>    }                                                                                                                                   <* 
 *>    return unit_answer;                                                                                                                 <* 
 *> }                                                                                                                                      <*/



/*=================================(end-code)=================================*/
