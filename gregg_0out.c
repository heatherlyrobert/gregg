/*=================================(beg-code)=================================*/
#include "gregg.h"


const float FULL_CIRCLE  = 2 * 3.14159;   // circle in radians

#define     FILE_LOC    "/var/lib/gregg/"
#define     FILE_BLANK  "stroke"
#define     FILE_SUFFIX "db"

static FILE  *s_file = NULL;
static char   s_recd [LEN_HUGE] = "";
static int    s_len  =    0;
static char   s_fields    [20][LEN_RECD];
static int    s_nfield    =   0;


char
OUT_init             (void)
{
   OUT_clear ();
   /*---(locals)---------------------------*/
   int       i             = 0;
   o.craw     = 0;
   o.cavg     = 0;
   o.ckey     = 0;
   o.curr     = 0;
   return 0;
}

char          /*----: clear all point data -----------------------------------*/
OUT_clear          (void)
{
   /*---(locals)-----------+-----+-----+-*/
   int         x_type      =    0;
   int         x_pt        =    0;
   tPOINT     *p           = NULL;
   /*---(header)-------------------------*/
   DEBUG_DATA   yLOG_senter   (__FUNCTION__);
   /*---(clear header)-------------------*/
   DEBUG_DATA   yLOG_snote    ("header");
   o.when   [0]  = '\0';
   o.expect [0]  = '\0';
   o.note   [0]  = '\0';
   o.saved       = 'n';
   /*---(clear results)------------------*/
   DEBUG_DATA   yLOG_snote    ("results");
   o.letters[0]  = '\0';
   o.actual [0]  = '\0';
   o.grade  [0]  = '\0';
   o.word   [0]  = '\0';
   /*---(clear counters)-----------------*/
   DEBUG_DATA   yLOG_snote    ("counters");
   o.craw = o.cavg = o.ckey = 0;
   o.nraw = o.nbas = o.navg = o.nkey  = 0;
   /*---(clear structures)---------------*/
   for (x_type = 0; x_type < 4; ++x_type) {
      /*---(point type)------------------*/
      switch (x_type) {
      case 0:
         p = o.raw;
         DEBUG_DATA   yLOG_snote    ("raw");
         break;
      case 1:
         p = o.bas;
         DEBUG_DATA   yLOG_snote    ("base");
         break;
      case 2:
         p = o.avg;
         DEBUG_DATA   yLOG_snote    ("avg");
         break;
      case 3:
         p = o.key;
         DEBUG_DATA   yLOG_snote    ("key");
         break;
      }
      /*---(clear points)----------------*/
      for (x_pt = 0; x_pt < MAX_POINTS; ++x_pt) {
         p [x_pt].p_raw  = p [x_pt].p_bas  = 0;
         p [x_pt].x_full = p [x_pt].y_full = 0;
         p [x_pt].xd     = p [x_pt].yd     = 0;
         p [x_pt].len    = p [x_pt].icept  = p [x_pt].degs  = p [x_pt].quad   = p [x_pt].ccat = 0;
         p [x_pt].slope  = p [x_pt].rads   = p [x_pt].range = p [x_pt].cdepth = 0.0;
         p [x_pt].cano   = p [x_pt].type   = p [x_pt].fake  = '-';
         strlcpy (p [x_pt].use, "-", 5);
         p [x_pt].x_rel  = p [x_pt].y_rel  = 0.0;
         p [x_pt].x_pos  = p [x_pt].y_pos  = 0;
         p [x_pt].xy_len = 0;
      }
      /*---(done)------------------------*/
   }
   /*---(bounds)-------------------------*/
   o.x_min = o.y_min =  1000;
   o.x_max = o.y_max = -1000;
   /*---(complete)-----------------------*/
   DEBUG_DATA   yLOG_sexit    (__FUNCTION__);
   return 0;
}



/*============================--------------------============================*/
/*===----                            overall                           ----===*/
/*============================--------------------============================*/
void o___GENERIC________________o (void) {;}

/*> char                                                                              <* 
 *> POINT_display        (tPOINT *a_curr)                                             <* 
 *> {                                                                                 <* 
 *>    /+---(locals)-----------+-----+-----+-+/                                       <* 
 *>    DEBUG_GRAF   yLOG_enter   (__FUNCTION__);                                      <* 
 *>    /+---(x-pos)--------------+/                                                   <* 
 *>    a_curr->x_rel = (a_curr->x_full - 0     ) / (float) my.x_scale;                <* 
 *>    a_curr->x_pos = my.x_min + a_curr->x_full / my.ratio;                          <* 
 *>    /+---(y-pos)--------------+/                                                   <* 
 *>    a_curr->y_rel = (a_curr->y_full - 0     ) / (float) my.y_scale;                <* 
 *>    a_curr->y_pos = my.y_min + a_curr->y_full / my.ratio;                          <* 
 *>    /+---(done)---------------+/                                                   <* 
 *>    DEBUG_GRAF   yLOG_exit    (__FUNCTION__);                                      <* 
 *>    return 0;                                                                      <* 
 *> }                                                                                 <*/

char
POINT_calc         (char a_type, tPOINT *a_curr,  int a_span)
{
   /*---(locals)-----------+-----+-----+-*/
   char        rce         =  -10;
   tPOINT     *x_1st       = NULL;
   tPOINT     *x_2nd       = NULL;
   /*---(defense)------------------------*/
   --rce;  if (a_curr == NULL)                        return rce;
   --rce;  if (strchr ("na", a_span) == NULL)         return rce;
   /*---(basics first)-------------------*/
   a_curr->x_rel = (a_curr->x_full - 0     ) / (float) my.x_scale;
   a_curr->x_pos = my.x_min + a_curr->x_full / my.ratio;
   a_curr->y_rel = (a_curr->y_full - 0     ) / (float) my.y_scale;
   a_curr->y_pos = my.y_min + a_curr->y_full / my.ratio;
   /*---(update bounds)------------------*/
   if (a_type == POINTS_RAW) {
      if (o.x_min > a_curr->x_pos)   o.x_min = a_curr->x_pos;
      if (o.y_min > a_curr->y_pos)   o.y_min = a_curr->y_pos;
      if (o.x_max < a_curr->x_pos)   o.x_max = a_curr->x_pos;
      if (o.y_max < a_curr->y_pos)   o.y_max = a_curr->y_pos;
   }
   /*---(defense)------------------------*/
   if (a_curr->type == POINT_START)                   return 0;
   if (a_curr->type == POINT_HEAD )                   return 0;
   if (a_curr->type == POINT_FINISH)                  return 0;
   /*---(set the ends)-------------------*/
   x_1st  = a_curr - 1;
   x_2nd  = a_curr;
   /*---(screen x, y, and xy len)--------*/
   a_curr->xd     = x_2nd->x_pos - x_1st->x_pos;
   a_curr->yd     = x_2nd->y_pos - x_1st->y_pos;
   a_curr->xy_len = (int) sqrt ((a_curr->xd * a_curr->xd) + (a_curr->yd * a_curr->yd));
   /*---(update for average)-------------*/
   if (a_span == 'a')  x_2nd = a_curr + 1;
   /*---(full x, y, and xy len)----------*/
   a_curr->xd     = x_2nd->x_full - x_1st->x_full;
   a_curr->yd     = x_2nd->y_full - x_1st->y_full;
   a_curr->len    = (int) sqrt((a_curr->xd * a_curr->xd) + (a_curr->yd * a_curr->yd));
   /*---(slope and intercept)------------*/
   if      (a_curr->xd != 0)  a_curr->slope = (float) a_curr->yd / a_curr->xd;
   else if (a_curr->yd >  0)  a_curr->slope = +999.99;
   else                       a_curr->slope = -999.99;
   a_curr->icept   = (int) (a_curr->y_full - (a_curr->slope * a_curr->x_full));
   if      (a_curr->icept >  99999)  a_curr->icept =  99999;
   else if (a_curr->icept < -99999)  a_curr->icept = -99999;
   /*---(radians/degrees)----------------*/
   a_curr->rads  = atan2(a_curr->yd, a_curr->xd);
   if (a_curr->rads > FULL_CIRCLE) a_curr->rads -= FULL_CIRCLE;
   if (a_curr->rads < 0)           a_curr->rads += FULL_CIRCLE;
   a_curr->degs  = (int) (a_curr->rads * 57.29577951308);
   /*---(quadrant)-----------------------*/
   if (a_curr->xd >= 0) {
      if (a_curr->yd >= 0) a_curr->quad = 1;
      else                 a_curr->quad = 4;
   } else {
      if (a_curr->yd >= 0) a_curr->quad = 2;
      else                 a_curr->quad = 3;
   }
   /*---(correct the deg as required)------*/
   /*> if (a_curr->quad == 2) a_curr->degs -= 180;                                          <* 
    *> if (a_curr->quad == 3) a_curr->degs += 180;                                          <*/
   /*---(fill-in beginning)--------------*/
   if (x_1st->type == POINT_HEAD) {
      /*---(head first)------------------*/
      x_1st->slope = a_curr->slope;
      x_1st->icept = a_curr->icept;
      x_1st->rads  = a_curr->rads;
      x_1st->degs  = a_curr->degs;
      x_1st->quad  = a_curr->quad;
      /*---(start too)-------------------*/
      if (a_type != POINTS_KEY) {
         x_1st     = x_1st - 1;
         x_1st->slope = a_curr->slope;
         x_1st->icept = a_curr->icept;
         x_1st->rads  = a_curr->rads;
         x_1st->degs  = a_curr->degs;
         x_1st->quad  = a_curr->quad;
      }
      /*---(done)------------------------*/
   }
   /*---(fill-in end)--------------------*/
   x_2nd = a_curr + 1;
   if (a_type != POINTS_KEY && x_2nd->type == POINT_FINISH) {
      x_2nd->slope = a_curr->slope;
      x_2nd->icept = a_curr->icept;
      x_2nd->rads  = a_curr->rads;
      x_2nd->degs  = a_curr->degs;
      x_2nd->quad  = a_curr->quad;
   }
   /*---(complete)-------------------------*/
   return 0;
}

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
   fprintf (a_file, "%6d%s %6d%s %4d%s %4d%s "           , a_curr->x_full, q1, a_curr->y_full, q1, a_curr->xd    , q1, a_curr->yd  , q2);
   fprintf (a_file, "%4d%s %8.2f%s %6d%s %5.2f%s %3d%s " , a_curr->len   , q1, a_curr->slope , q1, a_curr->icept , q1, a_curr->rads, q1, a_curr->degs , q2);
   fprintf (a_file, "%1d%s %1d%s %6.1f%s %2d%s "         , a_curr->quad  , q1, a_curr->range , q1, a_curr->cdepth, q1, a_curr->ccat, q1);
   fprintf (a_file, "%c%s %c%s %s%s "                    , a_curr->cano  , q1, a_curr->type  , q1, a_curr->use   , q2);
   fprintf (a_file, "%6.3f%s %6.3f%s "                   , a_curr->x_rel , q1, a_curr->y_rel , q1);
   fprintf (a_file, "%6d%s %6d%s %6d%s\n"                , a_curr->x_pos , q1, a_curr->y_pos , q1, a_curr->xy_len, q1);
   return 0;
}

char          /*----: list all points of a particular type -------------------*/
POINT_list         (FILE *a_file, char a_style, tPOINT *a_series, int a_count)
{
   int         i;
   /*---(header)-------------------------*/
   if (a_style == 'd') {
      fprintf (a_file, "\n");
      fprintf (a_file, "point inventory-------------------------------------------------------------------------------------------------------------------\n");
      fprintf (a_file, "### bas raw a | --xx-- --yy-- -xd- -yd- | -len -slope-- b-cept -rad- deg | q r curve- cc c t u | -xrel- -yrel- -xpos- -ypos- xylen-\n");
   } else if (a_style == 'g') {
      fprintf (a_file, "###  bas  raw  a  --xx--  --yy--  -xd-  -yd-  -len  -slope--  b-cept  -rad-  deg  q  r  curve-  cc  c  t  u  --xrel  --yrel  --xpos  --ypos  xylen- \n");
   }
   /*---(points)-------------------------*/
   for (i = 0; i < a_count; ++i) {
      POINT_show (a_file, a_style, a_series + i, i);
   }
   /*---(footer)-------------------------*/
   if (a_style == 'd') {
      fprintf (a_file, "### bas raw a | --xx-- --yy-- -xd- -yd- | -len -slope-- b-cept -rad- deg | q r curve- cc c t u | -xrel- -yrel- -xpos- -ypos- xylen-\n");
      fprintf (a_file, "point inventory-------------------------------------------------------------------------------------------------------------------\n");
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
/*===----                         saved outlines                       ----===*/
/*============================--------------------============================*/
void o___SAVED__________________o (void) {;}

char       /*----: file info for buffer line ---------------------------------*/
OUT_status         (char *a_list)
{
   /*---(locals)-----------+-----------+-*/
   char        rce         = -10;
   int         i           =   0;
   char        t           [10];
   /*---(defenses)-----------------------*/
   --rce;  if (a_list  == NULL)  return rce;
   /*---(walk the list)------------------*/
   sprintf (a_list, "%s (%d outlines)", my.f_full, o.total);
   /*---(complete)-----------------------*/
   return 0;
}

char
OUT__open          (char *a_mode)
{
   /*---(locals)-----------+-----+-----+-*/
   char        rce         =  -10;
   /*---(header)-------------------------*/
   DEBUG_INPT   yLOG_senter  (__FUNCTION__);
   /*---(open file)------------------------*/
   s_file = fopen (my.f_full, a_mode);
   DEBUG_INPT   yLOG_spoint  (s_file);
   --rce;  if (s_file == NULL) {
      DEBUG_INPT   yLOG_snote   ("could not open");
      DEBUG_INPT   yLOG_sexitr  (__FUNCTION__, rce);
      return rce;
   }
   DEBUG_INPT   yLOG_snote   ("openned");
   /*---(complete)-------------------------*/
   DEBUG_INPT   yLOG_sexit   (__FUNCTION__);
   return 0;
}

char
OUT__read          (void)
{
   /*---(locals)-----------+-----+-----+-*/
   char        rce         =  -10;
   char        rc          =    0;
   /*---(header)-------------------------*/
   DEBUG_INPT   yLOG_senter  (__FUNCTION__);
   /*---(read a line)--------------------*/
   while (1) {
      fgets (s_recd, LEN_HUGE, s_file);
      if (feof (s_file)) {
         DEBUG_INPT   yLOG_note    ("end of file");
         DEBUG_INPT   yLOG_sexit   (__FUNCTION__);
         return 0;
      }
      s_len = strllen (s_recd, LEN_HUGE);
      DEBUG_INPT   yLOG_value   ("s_len"     , s_len);
      if (s_len < 10)             continue;
      if (s_recd [0] == '#')      continue;
      if (s_recd [0] == ' ')      continue;
      DEBUG_INPT   yLOG_note    ("good record");
      break;
   }
   /*---(complete)-------------------------*/
   DEBUG_INPT   yLOG_sexit   (__FUNCTION__);
   return 1;
}

char
OUT__parse         (void)
{
   /*---(locals)-----------+-----+-----+-*/
   char        rce         =  -10;
   char        rc          =    0;
   char       *p           = NULL;
   char       *q           = "";
   char       *r           = NULL;
   /*---(header)-------------------------*/
   DEBUG_INPT   yLOG_senter  (__FUNCTION__);
   /*---(cleanse)---------------------*/
   for (s_nfield = 0; s_nfield < 20; ++s_nfield) {
      strlcpy (s_fields [s_nfield], "", LEN_RECD);
   }
   s_nfield = 0;
   /*---(parse fields)----------------*/
   p = strtok_r (s_recd, q, &r);
   while (p != NULL) {
      strltrim  (p, ySTR_BOTH, LEN_RECD);
      strncpy   (s_fields [s_nfield++], p, LEN_RECD);
      p = strtok_r (NULL     , q, &r);
   }
   --rce;  if (s_nfield < 1) {
      DEBUG_INPT   yLOG_snote   ("no fields found");
      DEBUG_INPT   yLOG_sexitr  (__FUNCTION__, rce);
      return rce;
   }
   DEBUG_INPT   yLOG_sint    (s_nfield);
   /*---(place in globals)---------------*/
   if (s_nfield >=  1)  strlcpy (o.when  , s_fields [0], LEN_DESC);
   if (s_nfield >=  2)  strlcpy (o.expect, s_fields [1], LEN_DESC);
   if (s_nfield >=  3)  o.complexity  = atoi (s_fields [2]);
   if (s_nfield >=  4)  o.messiness   = atoi (s_fields [3]);
   if (s_nfield >=  5)  strlcpy (o.note  , s_fields [4], LEN_DESC);
   if (s_nfield >=  7)  o.ratio       = atoi (s_fields [6]);
   if (s_nfield >=  8)  strlcpy (o.points, s_fields [7], LEN_RECD);
   /*---(complete)-----------------------*/
   DEBUG_INPT   yLOG_sexit   (__FUNCTION__);
   return 0;
}

char
OUT__close         (void)
{
   /*---(locals)-----------+-----+-----+-*/
   char        rce         =  -10;
   char        rc          =    0;
   /*---(header)-------------------------*/
   DEBUG_INPT   yLOG_senter  (__FUNCTION__);
   /*---(flush)----------------------------*/
   fflush (s_file);
   DEBUG_INPT   yLOG_snote   ("flushed");
   /*---(close file)-----------------------*/
   rc = fclose (s_file);
   DEBUG_INPT   yLOG_svalue  ("rc"        , rc);
   --rce;  if (rc < 0) {
      DEBUG_INPT   yLOG_snote   ("could not close");
      DEBUG_INPT   yLOG_sexitr  (__FUNCTION__, rce);
      return rce;
   }
   DEBUG_INPT   yLOG_snote   ("closed");
   /*---(complete)-------------------------*/
   DEBUG_INPT   yLOG_sexit   (__FUNCTION__);
   return 0;
}

char
OUT_count          (void)
{
   /*---(locals)-----------+-----+-----+-*/
   char        rc          =    0;
   /*---(header)-------------------------*/
   DEBUG_INPT   yLOG_enter   (__FUNCTION__);
   /*---(open file)------------------------*/
   rc = OUT__open ("r");
   DEBUG_INPT   yLOG_value   ("rc"        , rc);
   if (rc < 0) {
      DEBUG_INPT   yLOG_exitr   (__FUNCTION__, rc);
      return rc;
   }
   /*---(count records)--------------------*/
   o.total = 0;
   while (1) {
      rc = OUT__read ();
      if (rc <= 0)  break;
      ++o.total;
   }
   DEBUG_INPT   yLOG_value   ("o.total"   , o.total);
   /*---(finish off record)----------------*/
   OUT__close ();
   /*---(complete)-------------------------*/
   DEBUG_INPT   yLOG_exit    (__FUNCTION__);
   return 0;
}

char
OUT_pick           (int a_num)
{
   /*---(locals)-----------+-----+-----+-*/
   char        rce         =  -10;
   char        rc          =    0;
   int         x_num       =    0;
   /*---(header)-------------------------*/
   DEBUG_INPT   yLOG_enter   (__FUNCTION__);
   /*---(open file)------------------------*/
   rc = OUT__open ("r");
   DEBUG_INPT   yLOG_value   ("rc"        , rc);
   if (rc < 0) {
      DEBUG_INPT   yLOG_exitr   (__FUNCTION__, rc);
      return rc;
   }
   /*---(count records)--------------------*/
   while (1) {
      rc = OUT__read ();
      if (rc <= 0)         break;
      if (x_num == a_num)  break;
      ++x_num;
   }
   /*---(finish off record)----------------*/
   OUT__close ();
   /*---(check for trouble)----------------*/
   --rce;  if (x_num != a_num) {
      DEBUG_INPT   yLOG_exitr   (__FUNCTION__, rce);
      return rce;
   }
   /*---(parse)----------------------------*/
   OUT__parse ();
   o.saved    = 'y';
   RAW_load  (o.points);
   /*---(complete)-------------------------*/
   DEBUG_INPT   yLOG_exit    (__FUNCTION__);
   return 0;
}

char
OUT_append         (void)
{
   /*---(locals)---------------------------*/
   char        rc          = 0;
   char        d           [500];
   int         i           = 0;
   /*---(header)-------------------------*/
   DEBUG_OUTP   yLOG_enter   (__FUNCTION__);
   /*---(check for saved)------------------*/
   if (o.saved == 'y') {
      DEBUG_OUTP   yLOG_note    ("already saved");
      DEBUG_OUTP   yLOG_exit    (__FUNCTION__);
      return 0;
   }
   /*---(open file)------------------------*/
   rc = OUT__open ("a");
   if (rc < 0) {
      DEBUG_INPT   yLOG_exitr   (__FUNCTION__, rc);
      return rc;
   }
   /*---(write date)-----------------------*/
   time_t      time_date = time(NULL);
   struct tm*  curr_time = localtime(&time_date);
   char        asc_time[20];
   strftime(asc_time, 20, "%y.%m.%d.%H.%M.%S", curr_time);
   strncpy(d, asc_time, 100);
   char  *n = strchr(d, '\n');
   if (n != NULL)  *n = '\0';
   fprintf (s_file, "%-20.20s  ", d);
   /*---(leave space for answer)-----------*/
   fprintf (s_file, "%-30.30s  ", o.actual);
   fprintf (s_file, "%c  ", '-');
   fprintf (s_file, "%c  ", '-');
   /*---(leave space for notes)------------*/
   fprintf (s_file, "%-40.40s  ", " ");
   /*---(write count)----------------------*/
   fprintf (s_file, "%3d  ", o.nraw);
   /*---(write conv)-----------------------*/
   fprintf (s_file, "%5.2f  ", o.ratio);
   /*---(process)--------------------------*/
   for (i = 0; i < o.nraw; ++i) {
      fprintf (s_file, ";");
      switch (o.raw [i].type) {
      case POINT_START   :
         fprintf (s_file, ";TOUCH,");
         break;
      case POINT_FINISH  :
         fprintf (s_file, ";LIFT,");
         break;
      }
      fprintf (s_file, "%d,%d", o.raw[i].x_full, o.raw[i].y_full);
   }
   /*---(finish off record)----------------*/
   fprintf (s_file, " \n");
   OUT__close ();
   /*---(touch up some vars)---------------*/
   o.saved    = 'y';
   ++o.total;
   o.curr = o.total;
   /*---(complete)-------------------------*/
   DEBUG_OUTP   yLOG_exit    (__FUNCTION__);
   return 0;
}

char         /*-> tbd --------------------------------[ leaf   [gc.C55.124.30]*/ /*-[01.0000.112.!]-*/ /*-[--.---.---.--]-*/
FILE_rename          (char *a_name)
{
   /*---(locals)-----------+-----+-----+-*/
   char        t           [LEN_STR]   = "";
   char       *p           = NULL;
   /*---(header)-------------------------*/
   DEBUG_OUTP   yLOG_enter   (__FUNCTION__);
   /*---(defense)------------------------*/
   DEBUG_OUTP   yLOG_point   ("a_name"    , a_name);
   if (a_name == NULL || a_name [0] == 0) {
      DEBUG_OUTP   yLOG_note    ("a_name was null/blank, using defaults");
      strlcpy (my.f_loc  , FILE_LOC  , LEN_RECD);
      strlcpy (my.f_name , FILE_BLANK, LEN_RECD);
      sprintf (my.f_title, "%s.%s"   , my.f_name, FILE_SUFFIX);
      sprintf (my.f_full , "%s%s.%s" , my.f_loc, my.f_name, FILE_SUFFIX);
      DEBUG_OUTP   yLOG_info    ("my.f_name" , my.f_name);
      DEBUG_OUTP   yLOG_info    ("my.f_title", my.f_title);
      DEBUG_OUTP   yLOG_exit    (__FUNCTION__);
      OUT_count ();
      DEBUG_OUTP   yLOG_value   ("o.total"   , o.total);
      OUT_pick  (0);
      return 0;
   }
   DEBUG_OUTP   yLOG_info    ("a_name"    , a_name);
   /*---(parse base name)----------------*/
   strlcpy (t, a_name, LEN_STR);
   p = strrchr (t, "/");
   DEBUG_OUTP   yLOG_point   ("p"         , p);
   if (p == NULL) {
      DEBUG_OUTP   yLOG_note    ("name only, no directory");
      strlcpy (my.f_loc  , FILE_LOC  , LEN_RECD);
      strlcpy (my.f_name , a_name, LEN_RECD);
      sprintf (my.f_title, "%s.%s", my.f_name, FILE_SUFFIX);
      sprintf (my.f_full , "%s%s.%s" , my.f_loc, my.f_name, FILE_SUFFIX);
      DEBUG_OUTP   yLOG_info    ("my.f_name" , my.f_name);
      DEBUG_OUTP   yLOG_info    ("my.f_title", my.f_title);
      OUT_count ();
      DEBUG_OUTP   yLOG_value   ("o.total"   , o.total);
      OUT_pick  (0);
      DEBUG_OUTP   yLOG_exit    (__FUNCTION__);
      return 0;
   }
   /*---(parse qualified name)-----------*/
   DEBUG_OUTP   yLOG_note    ("fully qualified name, with directory");
   p = '\0';
   strlcpy (my.f_loc  , t     , LEN_RECD);
   strlcpy (my.f_name , p + 1 , LEN_RECD);
   sprintf (my.f_title, "%s/%s.%s", my.f_loc, my.f_name, FILE_SUFFIX);
   sprintf (my.f_full , "%s%s.%s" , my.f_loc, my.f_name, FILE_SUFFIX);
   DEBUG_OUTP   yLOG_info    ("my.f_loc"  , my.f_loc);
   DEBUG_OUTP   yLOG_info    ("my.f_name" , my.f_name);
   DEBUG_OUTP   yLOG_info    ("my.f_title", my.f_title);
   OUT_count ();
   DEBUG_OUTP   yLOG_value   ("o.total"   , o.total);
   OUT_pick  (0);
   /*---(complete)-----------------------*/
   DEBUG_OUTP   yLOG_exit    (__FUNCTION__);
   return 0;
}


/*=================================(end-code)=================================*/
