/*=================================(beg-code)=================================*/
#include "gregg.h"



const float FULL_CIRCLE  = 2 * 3.14159;   // circle in radians






/*============================--------------------============================*/
/*===----                       little helpers                         ----===*/
/*============================--------------------============================*/
void o___HELPER_________________o (void) {;}

tPOINT*
POINT__series           (char a_type, char *a_name, int *a_count)
{
   /*---(locals)-----------+-----+-----+-*/
   tPOINT     *p           = NULL;
   int         c           =    0;
   char        x_name      [LEN_TERSE] = "";
   /*---(point type)---------------------*/
   switch (a_type) {
   case POINTS_RAW : p = o.raw;  c = o.nraw;  strlcpy (x_name, "RAW", LEN_TERSE);   break;
   case POINTS_BAS : p = o.bas;  c = o.nbas;  strlcpy (x_name, "BAS", LEN_TERSE);   break;
   case POINTS_AVG : p = o.avg;  c = o.navg;  strlcpy (x_name, "AVG", LEN_TERSE);   break;
   case POINTS_TMP : p = NULL;   c = 0;       strlcpy (x_name, ""   , LEN_TERSE);   break;
   case POINTS_KEY : p = o.key;  c = o.nkey;  strlcpy (x_name, "KEY", LEN_TERSE);   break;
   default         : p = NULL;   c = 0;       strlcpy (x_name, ""   , LEN_TERSE);   break;
   }
   /*---(return)-------------------*/
   if (a_count != NULL)  *a_count = c;
   if (a_name  != NULL)  strlcpy (a_name, x_name, LEN_TERSE);
   /*---(complete)-----------------*/
   return p;
}



/*============================--------------------============================*/
/*===----                        data wiping                           ----===*/
/*============================--------------------============================*/
void o___CLEARING_______________o (void) {;}

char
POINT_clear             (tPOINT *a_point, char a_type)
{
   /*---(head)---------------------------*/
   a_point->series  = a_type;
   a_point->seq     = 0;
   a_point->p_raw   = 0;
   a_point->p_bas   = 0;
   a_point->fake    = '-';
   a_point->type    = POINT_NONE;
   /*---(touchpad)-----------------------*/
   a_point->x_touch = a_point->x_raw  = 0;
   a_point->y_touch = a_point->y_raw  = 0;
   /*---(statistics)---------------------*/
   a_point->xd      = a_point->yd     = 0;
   a_point->len     = a_point->icept  = a_point->degs   = a_point->quad   = a_point->ccat = 0;
   a_point->slope   = a_point->rads   = a_point->range  = a_point->cdepth = 0.0;
   a_point->sharp   = a_point->cano   = a_point->fake   = '-';
   strlcpy (a_point->use, "-", LEN_TERSE);
   /*---(display)---------------*/
   a_point->x_rel   = a_point->y_rel  = 0.0;
   a_point->x_pos   = a_point->y_pos  = 0;
   /*---(complete)--------------*/
   return 0;
}

char
POINT_clear_series      (char a_type)
{
   /*---(locals)-----------+-----+-----+-*/
   char        rce         =  -10;
   int         i           =    0;
   tPOINT     *p           = NULL;
   /*---(header)-------------------------*/
   DEBUG_CALC   yLOG_senter  (__FUNCTION__);
   DEBUG_DATA   yLOG_schar    (a_type);
   /*---(point type)------------------*/
   p = POINT__series (a_type, NULL, NULL);
   DEBUG_DATA   yLOG_spoint   (p);
   --rce;  if (p == NULL) {
      DEBUG_CALC   yLOG_sexitr  (__FUNCTION__, rce);
      return rce;
   }
   /*> switch (a_type) {                                                              <* 
    *> case POINTS_RAW :                                                              <* 
    *>    p = o.raw;                                                                  <* 
    *>    DEBUG_DATA   yLOG_snote    ("raw");                                         <* 
    *>    break;                                                                      <* 
    *> case POINTS_BAS :                                                              <* 
    *>    p = o.bas;                                                                  <* 
    *>    DEBUG_DATA   yLOG_snote    ("base");                                        <* 
    *>    break;                                                                      <* 
    *> case POINTS_AVG :                                                              <* 
    *>    p = o.avg;                                                                  <* 
    *>    DEBUG_DATA   yLOG_snote    ("avg");                                         <* 
    *>    break;                                                                      <* 
    *> case POINTS_TMP :                                                              <* 
    *>    DEBUG_DATA   yLOG_snote    ("tmp, nothing to do");                          <* 
    *>    DEBUG_CALC   yLOG_sexit   (__FUNCTION__);                                   <* 
    *>    return 0;                                                                   <* 
    *>    break;                                                                      <* 
    *> case POINTS_KEY :                                                              <* 
    *>    p = o.key;                                                                  <* 
    *>    DEBUG_DATA   yLOG_snote    ("key");                                         <* 
    *>    break;                                                                      <* 
    *> }                                                                              <*/
   /*---(clear all)-------------------*/
   DEBUG_DATA   yLOG_snote    ("clear all data");
   for (i = 0; i < MAX_POINTS; ++i) {
      POINT_clear (p + i, a_type);
   }
   /*---(complete)--------------*/
   DEBUG_CALC   yLOG_sexit   (__FUNCTION__);
   return 0;
}

char
POINT_clear_all         (void)
{
   /*---(locals)-----------+-----+-----+-*/
   int         i           =    0;
   /*---(header)-------------------------*/
   DEBUG_CALC   yLOG_enter   (__FUNCTION__);
   /*---(clear types)--------------------*/
   for (i = 0; i < strlen (POINTS_ALL); ++i) {
      POINT_clear_series (POINTS_ALL [i]);
   }
   /*---(complete)--------------*/
   DEBUG_CALC   yLOG_exit    (__FUNCTION__);
   return 0;
}



/*============================--------------------============================*/
/*===----                      sequence checking                       ----===*/
/*============================--------------------============================*/
void o___SEQUENCING_____________o (void) {;}

char
POINT_seq_start    (char a_type, tPOINT *p, int c)
{
   /*---(locals)-----------+-----+-----+-*/
   char        rce         =  -10;
   char        x_prev      =  '-';
   /*---(check initial start)------------*/
   --rce;  if (c == 0 && a_type != POINT_START) {
      DEBUG_RAW    yLOG_snote   ("zero must be S");
      return rce;
   }
   /*---(quick defense)------------------*/
   if (c == 0)   return 0;
   /*---(prepare)------------------------*/
   x_prev = p [c - 1].type;
   /*---(check later starts)-------------*/
   --rce;  if (a_type == POINT_START && x_prev != POINT_FINISH) {
      DEBUG_RAW    yLOG_snote   ("S must follow F");
      return rce;
   }
   --rce;  if (x_prev == POINT_FINISH && a_type != POINT_START) {
      DEBUG_RAW    yLOG_snote   ("F must preceed S");
      return rce;
   }
   /*---(complete)-----------------------*/
   return 0;
}

char
POINT_seq_head     (char a_type, tPOINT *p, int c, int a_xpad, int a_ypad)
{
   /*---(locals)-----------+-----+-----+-*/
   char        rce         =  -10;
   char        x_prev      =  '-';
   char        x_xpad      =    0;
   char        x_ypad      =    0;
   /*---(quick defense)------------------*/
   if (c == 0)   return 0;
   /*---(prepare)------------------------*/
   x_prev = p [c - 1].type;
   x_xpad = p [c - 1].x_touch;
   x_ypad = p [c - 1].y_touch;
   /*---(check sequencing)---------------*/
   --rce;  if (x_prev == POINT_START && a_type != POINT_HEAD) {
      DEBUG_RAW    yLOG_snote   ("> must follow S");
      return rce;
   }
   --rce;  if (a_type == POINT_HEAD && x_prev != POINT_START) {
      DEBUG_RAW    yLOG_snote   ("S must preceed >");
      return rce;
   }
   --rce;  if (a_type == POINT_HEAD && (a_xpad != x_xpad || a_ypad != x_ypad)) {
      DEBUG_RAW    yLOG_snote   ("S and > must match");
      return rce;
   }
   /*---(complete)-----------------------*/
   return 0;
}

char
POINT_seq_normal   (char a_type, tPOINT *p, int c, int a_xpad, int a_ypad)
{
   /*---(locals)-----------+-----+-----+-*/
   char        rce         =  -10;
   char        x_prev      =  '-';
   char        x_xpad      =    0;
   char        x_ypad      =    0;
   char        x_dup       =    0;
   /*---(quick defense)------------------*/
   if (c == 0)   return 0;
   /*---(prepare)------------------------*/
   x_prev = p [c - 1].type;
   x_xpad = p [c - 1].x_touch;
   x_ypad = p [c - 1].y_touch;
   /*---(check sequencing)---------------*/
   --rce;  if (a_type == POINT_NORMAL && strchr (">-", x_prev) == NULL) {
      DEBUG_RAW    yLOG_snote   ("- must follow > or -");
      return rce;
   }
   /*---(check duplication)--------------*/
   --rce;  if (a_type == POINT_NORMAL) {
      x_dup = 0;
      if (a_xpad == x_xpad) ++x_dup;
      if (a_ypad == x_ypad) ++x_dup;
      if (x_dup >= 2) {
         DEBUG_RAW    yLOG_snote   ("duplicate xy pos -");
         return rce;
      }
   }
   /*---(complete)-----------------------*/
   return 0;
}

char
POINT_seq_tail     (char a_type, tPOINT *p, int c, int a_xpad, int a_ypad)
{
   /*---(locals)-----------+-----+-----+-*/
   char        rce         =  -10;
   char        x_prev      =  '-';
   char        x_xpad      =    0;
   char        x_ypad      =    0;
   /*---(quick defense)------------------*/
   if (c == 0)   return 0;
   /*---(prepare)------------------------*/
   x_prev = p [c - 1].type;
   x_xpad = p [c - 1].x_touch;
   x_ypad = p [c - 1].y_touch;
   /*---(check sequencing)---------------*/
   --rce;  if (a_type == POINT_TAIL && strchr (">-", x_prev) == NULL) {
      DEBUG_RAW    yLOG_snote   ("< must follow > or -");
      return rce;
   }
   --rce;  if (a_type == POINT_TAIL && x_prev == POINT_NORMAL && a_xpad == x_xpad && a_ypad == x_ypad) {
      DEBUG_RAW    yLOG_snote   ("last - and < match, compress");
      p [c - 1].type = POINT_TAIL;
      return rce;
   }
   /*---(complete)-----------------------*/
   return 0;
}

char
POINT_seq_finish   (char a_type, tPOINT *p, int c, int a_xpad, int a_ypad)
{
   /*---(locals)-----------+-----+-----+-*/
   char        rce         =  -10;
   char        x_prev      =  '-';
   char        x_xpad      =    0;
   char        x_ypad      =    0;
   /*---(quick defense)------------------*/
   if (c == 0)   return 0;
   /*---(prepare)------------------------*/
   x_prev = p [c - 1].type;
   x_xpad = p [c - 1].x_touch;
   x_ypad = p [c - 1].y_touch;
   /*---(check sequencing)---------------*/
   --rce;  if (a_type == POINT_FINISH && x_prev != POINT_TAIL) {
      DEBUG_RAW    yLOG_snote   ("F must folllow <");
      return rce;
   }
   --rce;  if (x_prev == POINT_TAIL && a_type != POINT_FINISH) {
      DEBUG_RAW    yLOG_snote   ("< must preceed F");
      return rce;
   }
   --rce;  if (a_type == POINT_FINISH && (a_xpad != x_xpad || a_ypad != x_ypad)) {
      DEBUG_RAW    yLOG_snote   ("F and < must match");
      return rce;
   }
   /*---(complete)-----------------------*/
   return 0;
}



/*============================--------------------============================*/
/*===----                     generate statistics                      ----===*/
/*============================--------------------============================*/
void o___STATISTICS_____________o (void) {;}

char
POINT_position     (tPOINT *a_curr)
{
   /*---(locals)-----------+-----+-----+-*/
   char        rce         =  -10;
   /*---(header)-------------------------*/
   DEBUG_CALC   yLOG_senter  (__FUNCTION__);
   /*---(defense)------------------------*/
   DEBUG_CALC   yLOG_spoint  (a_curr);
   --rce;  if (a_curr == NULL) {
      DEBUG_CALC   yLOG_sexitr  (__FUNCTION__, rce);
      return rce;
   }
   --rce;  if (my.ratio == 0 || my.x_scale == 0 ||my.y_scale == 0) {
      DEBUG_CALC   yLOG_snote   ("scaling not set");
      DEBUG_CALC   yLOG_sexitr  (__FUNCTION__, rce);
      return rce;
   }
   /*---(xpos)-----------------------------*/
   a_curr->x_rel = a_curr->x_raw / (float) my.x_scale;
   a_curr->x_pos = my.x_min + (a_curr->x_rel * my.x_wide);
   DEBUG_CALC   yLOG_sint    (a_curr->x_pos);
   /*---(ypos)-----------------------------*/
   a_curr->y_rel = a_curr->y_raw / (float) my.y_scale;
   a_curr->y_pos = my.y_min + (a_curr->y_rel * my.y_tall);
   DEBUG_CALC   yLOG_sint    (a_curr->y_pos);
   /*---(complete)-------------------------*/
   DEBUG_CALC   yLOG_sexit   (__FUNCTION__);
   return 0;
}

char
POINT_calc         (char a_type, tPOINT *a_curr,  int a_span)
{
   /*---(locals)-----------+-----+-----+-*/
   char        rce         =  -10;
   tPOINT     *x_1st       = NULL;
   tPOINT     *x_2nd       = NULL;
   /*---(header)-------------------------*/
   DEBUG_CALC   yLOG_senter  (__FUNCTION__);
   /*---(defense)------------------------*/
   DEBUG_CALC   yLOG_spoint  (a_curr);
   --rce;  if (a_curr == NULL || a_curr < o.raw || a_curr >= o.key + MAX_POINTS) {
      DEBUG_CALC   yLOG_sexitr  (__FUNCTION__, rce);
      return rce;
   }
   DEBUG_CALC   yLOG_schar   (a_type);
   --rce;  if (a_type == 0 || strchr (POINTS_ALL, a_type) == NULL) {
      DEBUG_CALC   yLOG_sexitr  (__FUNCTION__, rce);
      return rce;
   }
   DEBUG_CALC   yLOG_schar   (a_span);
   --rce;  if (a_span == 0  || strchr (SPAN_ALL  , a_span) == NULL) {
      DEBUG_CALC   yLOG_sexitr  (__FUNCTION__, rce);
      return rce;
   }
   --rce;  if (my.ratio   == 0 || my.x_scale == 0 ||my.y_scale == 0) {
      DEBUG_CALC   yLOG_snote   ("scaling not set");
      DEBUG_CALC   yLOG_sexitr  (__FUNCTION__, rce);
      return rce;
   }
   /*---(update bounds)------------------*/
   /*> if (a_type == POINTS_RAW) {                                                    <* 
    *>    if (o.xmin > a_curr->x_pos)   o.xmin = a_curr->x_pos;                       <* 
    *>    if (o.ymin > a_curr->y_pos)   o.ymin = a_curr->y_pos;                       <* 
    *>    if (o.xmax < a_curr->x_pos)   o.xmax = a_curr->x_pos;                       <* 
    *>    if (o.ymax < a_curr->y_pos)   o.ymax = a_curr->y_pos;                       <* 
    *> }                                                                              <*/
   /*---(position only)------------------*/
   --rce;  if (a_type == POINTS_RAW) {
      DEBUG_CALC   yLOG_snote   ("position only calc");
      DEBUG_CALC   yLOG_sexitr  (__FUNCTION__, rce);
      return rce;
   }
   if (a_span == SPAN_SPOT || a_curr->type == POINT_START) {
      DEBUG_CALC   yLOG_snote   ("position only calc");
      DEBUG_CALC   yLOG_sexit   (__FUNCTION__);
      return 0;
   }
   if (a_span == SPAN_AVG  && a_curr->type == POINT_FINISH) {
      DEBUG_CALC   yLOG_snote   ("can not avg finish");
      DEBUG_CALC   yLOG_sexit   (__FUNCTION__);
      return 0;
   }
   /*---(finish copies tail)-------------*/
   /*> x_1st = a_curr - 1;                                                            <* 
    *> if (a_curr->type == POINT_FINISH && a_span == SPAN_AVG) {                      <* 
    *>    a_curr->slope = x_1st->slope;                                               <* 
    *>    a_curr->icept = x_1st->icept;                                               <* 
    *>    a_curr->rads  = x_1st->rads;                                                <* 
    *>    a_curr->degs  = x_1st->degs;                                                <* 
    *>    a_curr->quad  = x_1st->quad;                                                <* 
    *> }                                                                              <* 
    *> if (a_type != POINTS_KEY && a_curr->type == POINT_FINISH) {                    <* 
    *>    a_curr->slope = x_1st->slope;                                               <* 
    *>    a_curr->icept = x_1st->icept;                                               <* 
    *>    a_curr->rads  = x_1st->rads;                                                <* 
    *>    a_curr->degs  = x_1st->degs;                                                <* 
    *>    a_curr->quad  = x_1st->quad;                                                <* 
    *> }                                                                              <* 
    *> if (a_curr->type == POINT_FINISH) {                                            <* 
    *>    DEBUG_CALC   yLOG_snote   ("finish copies previous");                       <* 
    *>    DEBUG_CALC   yLOG_sexit   (__FUNCTION__);                                   <* 
    *>    return 0;                                                                   <* 
    *> }                                                                              <*/
   /*---(set the ends)-------------------*/
   x_1st  = a_curr - 1;
   x_2nd  = a_curr;
   if (a_span == SPAN_AVG )  x_2nd = a_curr + 1;
   /*---(x and y change)-----------------*/
   a_curr->xd     = x_2nd->x_raw - x_1st->x_raw;
   DEBUG_CALC   yLOG_sdouble (a_curr->xd);
   a_curr->yd     = x_2nd->y_raw - x_1st->y_raw;
   DEBUG_CALC   yLOG_sdouble (a_curr->yd);
   if (a_curr->xd == 0 && a_curr->yd == 0) {
      DEBUG_CALC   yLOG_snote   ("no position change, so not calcs");
      DEBUG_CALC   yLOG_sexit   (__FUNCTION__);
      return 0;
   }
   /*---(length)-------------------------*/
   a_curr->len    = sqrt((a_curr->xd * a_curr->xd) + (a_curr->yd * a_curr->yd));
   DEBUG_CALC   yLOG_sdouble (a_curr->len);
   /*---(slope)--------------------------*/
   if      (a_curr->xd != 0)  a_curr->slope = (float) a_curr->yd / a_curr->xd;
   else if (a_curr->yd >  0)  a_curr->slope = +999.99;
   else                       a_curr->slope = -999.99;
   if      (a_curr->slope > +999.99) a_curr->slope = +999.99;
   else if (a_curr->slope < -999.99) a_curr->slope = -999.99;
   DEBUG_CALC   yLOG_sdouble (a_curr->slope);
   /*---(intercept)----------------------*/
   a_curr->icept   = a_curr->y_raw - (a_curr->slope * a_curr->x_raw);
   if      (a_curr->icept >  99999)  a_curr->icept =  99999;
   else if (a_curr->icept < -99999)  a_curr->icept = -99999;
   DEBUG_CALC   yLOG_sdouble (a_curr->icept);
   /*---(radians)------------------------*/
   a_curr->rads  = atan2f (a_curr->yd, a_curr->xd);
   if (a_curr->rads > FULL_CIRCLE) a_curr->rads -= FULL_CIRCLE;
   if (a_curr->rads < 0)           a_curr->rads += FULL_CIRCLE;
   DEBUG_CALC   yLOG_sdouble (a_curr->rads);
   /*---(degrees)------------------------*/
   a_curr->degs  = a_curr->rads * 57.29577951308;
   DEBUG_CALC   yLOG_sdouble (a_curr->degs);
   /*---(quadrant)-----------------------*/
   if (a_curr->xd >= 0) {
      if (a_curr->yd >= 0) a_curr->quad = 1;
      else                 a_curr->quad = 4;
   } else {
      if (a_curr->yd >= 0) a_curr->quad = 2;
      else                 a_curr->quad = 3;
   }
   DEBUG_CALC   yLOG_sint    (a_curr->quad);
   /*---(correct the deg as required)------*/
   /*> if (a_curr->quad == 2) a_curr->degs -= 180;                                          <* 
    *> if (a_curr->quad == 3) a_curr->degs += 180;                                          <*/
   /*> /+---(fill-in beginning)--------------+/                                       <* 
    *> if (x_1st->type == POINT_HEAD) {                                               <* 
    *>    /+---(head first)------------------+/                                       <* 
    *>    x_1st->slope = a_curr->slope;                                               <* 
    *>    x_1st->icept = a_curr->icept;                                               <* 
    *>    x_1st->rads  = a_curr->rads;                                                <* 
    *>    x_1st->degs  = a_curr->degs;                                                <* 
    *>    x_1st->quad  = a_curr->quad;                                                <* 
    *>    /+---(start too)-------------------+/                                       <* 
    *>    if (a_type != POINTS_KEY) {                                                 <* 
    *>       x_1st     = x_1st - 1;                                                   <* 
    *>       x_1st->slope = a_curr->slope;                                            <* 
    *>       x_1st->icept = a_curr->icept;                                            <* 
    *>       x_1st->rads  = a_curr->rads;                                             <* 
    *>       x_1st->degs  = a_curr->degs;                                             <* 
    *>       x_1st->quad  = a_curr->quad;                                             <* 
    *>    }                                                                           <* 
    *>    /+---(done)------------------------+/                                       <* 
    *> }                                                                              <*/
   /*---(complete)-------------------------*/
   DEBUG_CALC   yLOG_sexit   (__FUNCTION__);
   return 0;
}



/*============================--------------------============================*/
/*===----                        data reporting                        ----===*/
/*============================--------------------============================*/
void o___REPORTING______________o (void) {;}

char          /*----: display all information on an individual point ---------*/
POINT_show         (FILE *a_file, char a_style, tPOINT *a_curr, int a_num)
{
   /*> float x_slope  = a_curr->slope;                                                <* 
    *> int   x_icept  = a_curr->icept;                                                <*/
   char        q1          [LEN_LABEL] = "";
   char        q2          [LEN_LABEL] = " |";
   if (a_style == 'g') {
      strlcpy (q1, " ", LEN_LABEL);
      strlcpy (q2, " ", LEN_LABEL);
   }
   /*> if (x_slope > 999) {                                                           <* 
    *>    x_slope = 999.99;                                                           <* 
    *>    x_icept = 999;                                                              <* 
    *> }                                                                              <* 
    *> if (x_slope < -999) {                                                          <* 
    *>    x_slope = -999.99;                                                          <* 
    *>    x_icept = -999;                                                             <* 
    *> }                                                                              <*/
   fprintf (a_file, "%3d%s %3d%s %3d%s %c%s "            , a_num         , q1, a_curr->p_bas , q1, a_curr->p_raw , q1, a_curr->fake, q2);
   fprintf (a_file, "%6d%s %6d%s %4d%s %4d%s "           , a_curr->x_raw, q1, a_curr->y_raw, q1, a_curr->xd    , q1, a_curr->yd  , q2);
   fprintf (a_file, "%4d%s %8.2f%s %6d%s %5.2f%s %3d%s " , a_curr->len   , q1, a_curr->slope , q1, a_curr->icept , q1, a_curr->rads, q1, a_curr->degs , q2);
   fprintf (a_file, "%1d%s %1d%s %5.1f%s %2d%s "         , a_curr->quad  , q1, a_curr->range , q1, a_curr->cdepth, q1, a_curr->ccat, q1);
   fprintf (a_file, "%c%s %c%s %s%s "                    , a_curr->cano  , q1, a_curr->type  , q1, a_curr->use   , q2);
   fprintf (a_file, "%6.3f%s %6.3f%s "                   , a_curr->x_rel , q1, a_curr->y_rel , q1);
   fprintf (a_file, "%4d%s %4d%s\n"                      , a_curr->x_pos , q1, a_curr->y_pos , q1);
   return 0;
}

char          /*----: list all points of a particular type -------------------*/
POINT_list         (FILE *a_file, char a_style, tPOINT *a_series, int a_count)
{
   int         i;
   /*---(header)-------------------------*/
   if (a_style == 'd') {
      fprintf (a_file, "\n");
      fprintf (a_file, "point inventory-------------------------------------------------------------------------------------------------------------\n");
      fprintf (a_file, "### bas raw a | --xx-- --yy-- -xd- -yd- | -len -slope-- b-cept -rad- deg | q r curve cc c t u | -xrel- -yrel- xpos ypos xyln\n");
   } else if (a_style == 'g') {
      fprintf (a_file, "###  bas  raw  a  --xx--  --yy--  -xd-  -yd-  -len  -slope--  b-cept  -rad-  deg  q  r  curve-  cc  c  t  u  --xrel  --yrel  xpos  ypos  xyln \n");
   }
   /*---(points)-------------------------*/
   for (i = 0; i < a_count; ++i) {
      POINT_show (a_file, a_style, a_series + i, i);
   }
   /*---(footer)-------------------------*/
   if (a_style == 'd') {
      fprintf (a_file, "### bas raw a | --xx-- --yy-- -xd- -yd- | -len -slope-- b-cept -rad- deg | q r curve cc c t u | -xrel- -yrel- xpos ypos xyln\n");
      fprintf (a_file, "point inventory-------------------------------------------------------------------------------------------------------------\n");
      fprintf (a_file, "\n");
   }
   /*---(complete)-----------------------*/
   return 0;
}

char
POINT_clipboard    (char *a_cmd, char *a_opt)
{
   /*---(locals)-----------+-----+-----+-*/
   char        rce         =  -10;
   FILE       *f           = NULL;
   char        x_style     =  '-';
   /*---(defense)------------------------*/
   --rce;  if (a_cmd == NULL)  return rce;
   --rce;  if (a_opt == NULL)  return rce;
   /*---(open file)----------------------*/
   --rce;  if (strcmp (a_cmd, "clip" ) == 0) {
      f = fopen ("/root/z_gehye/vi_clip.txt", "w");
      if (f == NULL)  return rce;
      x_style = 'd';
   } else if (strcmp (a_cmd, "gyges") == 0) {
      f = fopen ("/root/z_gehye/vi_clip.txt", "w");
      if (f == NULL)  return rce;
      x_style = 'g';
   } else {
      f = stdout;
      x_style = 'd';
   }
   /*---(dump points)--------------------*/
   --rce;  if (strcmp (a_opt, "key") == 0)   POINT_list (f, x_style, o.key, o.nkey);
   else if    (strcmp (a_opt, "avg") == 0)   POINT_list (f, x_style, o.avg, o.navg);
   else if    (strcmp (a_opt, "bas") == 0)   POINT_list (f, x_style, o.bas, o.nbas);
   else if    (strcmp (a_opt, "raw") == 0)   POINT_list (f, x_style, o.raw, o.nraw);
   else       return rce;
   /*---(close file)---------------------*/
   if      (strcmp (a_cmd, "clip" ) == 0)  fclose (f);
   else if (strcmp (a_cmd, "gyges") == 0)  fclose (f);
   /*---(complete)-----------------------*/
   return 0;
}



/*============================--------------------============================*/
/*===----                         unit testing                         ----===*/
/*============================--------------------============================*/
static void o___UNIT_TEST_________________o (void) {;}

char*        /*-> unit test accessor -----------------[ light  [us.D90.241.L0]*/ /*-[03.0000.00#.#]-*/ /*-[--.---.---.--]-*/
POINT__unit          (char *a_question, char a_type, int a_num)
{
   /*---(locals)-----------+-----+-----+-*/
   int         i           = 0;
   char        t           [100] = "";
   char        s           [100] = "";
   int         x_end       = 0;
   int         y_end       = 0;
   tPOINT     *p           = NULL;
   int         c           =    0;
   char        x_pre       [LEN_TERSE] = "";
   /*---(initialize)---------------------*/
   strlcpy (unit_answer, "POINT unit       : unknownn request", 100);
   p = POINT__series (a_type, x_pre, &c);
   if (p == NULL)  return unit_answer;
   /*---(core data)----------------------*/
   if        (strncmp (a_question, "types"     , 20)  == 0) {
      for (i = 0; i < 45; ++i) {
         if (i >= c)  break;
         t [i]     = p [i].type;
         t [i + 1] = 0;
      }
      if (c > 45)  sprintf (s, "[%s>", t);
      else         sprintf (s, "[%s]", t);
      snprintf (unit_answer, LEN_STR, "%-3.3s by type      : %2d %s", x_pre, c, s);
   }
   else if   (strncmp (a_question, "touch"     , 20)  == 0) {
      snprintf (unit_answer, LEN_STR, "%-3.3s touch   (%2d) : %c  %4dx %4dy  %4dx %4dy",
            x_pre, a_num, p [a_num].type,
            p [a_num].x_touch, p [a_num].y_touch,
            p [a_num].x_raw  , p [a_num].y_raw);
   }
   else if   (strncmp (a_question, "pos"       , 20)  == 0) {
      snprintf (unit_answer, LEN_STR, "%-3.3s pos     (%2d) : %c  %4dx %4dy  %4.0fx %4.0fy  %3.0fxd %3.0fyd %3.0fl  %8.3fs %6.0fi %6.3fr %4.0fd %dq",
            x_pre, a_num, p [a_num].type,
            p [a_num].x_raw, p [a_num].y_raw,
            p [a_num].x_pos, p [a_num].y_pos,
            p [a_num].xd   , p [a_num].yd   , p [a_num].len,
            p [a_num].slope, p [a_num].icept,
            p [a_num].rads , p [a_num].degs , p [a_num].quad);
   }
   /*---(complete)-----------------------*/
   return unit_answer;
}

