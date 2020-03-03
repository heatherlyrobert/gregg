/*=================================(beg-code)=================================*/
#include "gregg.h"

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
   OUT_clear (CLEAR_FULL);
   /*---(locals)---------------------------*/
   int       i             = 0;
   o.craw     = 0;
   o.cavg     = 0;
   o.ckey     = 0;
   o.curr     = 0;
   return 0;
}

char          /*----: clear all point data -----------------------------------*/
OUT_clear          (char a_full)
{
   /*---(locals)-----------+-----+-----+-*/
   int         x_type      =    0;
   int         x_pt        =    0;
   tPOINT     *p           = NULL;
   /*---(header)-------------------------*/
   DEBUG_DATA   yLOG_enter    (__FUNCTION__);
   /*---(clear header)-------------------*/
   DEBUG_DATA   yLOG_note     ("header");
   o.when   [0]  = '\0';
   o.expect [0]  = '\0';
   o.note   [0]  = '\0';
   o.saved       = 'n';
   /*---(clear results)------------------*/
   DEBUG_DATA   yLOG_note     ("results");
   o.letters[0]  = '\0';
   o.actual [0]  = '\0';
   o.grade  [0]  = '\0';
   o.word   [0]  = '\0';
   /*---(clear counters)-----------------*/
   DEBUG_DATA   yLOG_note     ("counters");
   o.nbas = o.navg = o.cavg = 0;
   o.nkey = o.ckey = 0;
   /*---(clear structures)---------------*/
   if (a_full == CLEAR_FULL) {
      o.nraw = o.craw = 0;
      POINT_clear_all ();
   } else {
      POINT_clear_for_more ();
   }
   /*---(bounds)-------------------------*/
   o.xmin = o.ymin =     0;
   o.xmax = o.ymax =     0;
   DEBUG_RAW    yLOG_complex ("min"       , "%dx, %dy", o.xmin, o.ymin);
   DEBUG_RAW    yLOG_complex ("max"       , "%dx, %dy", o.xmax, o.ymax);
   /*---(complete)-----------------------*/
   DEBUG_DATA   yLOG_exit     (__FUNCTION__);
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
   sprintf (a_list, "%s (%d of %d)", my.f_full, o.curr + 1, o.total);
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
   if (s_nfield >=  7)  strlcpy (o.points, s_fields [7], LEN_RECD);
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
   DEBUG_INPT   yLOG_value   ("a_num"     , a_num);
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
      /*> printf ("%s\n", s_recd);                                                    <*/
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
   /*---(process)--------------------------*/
   for (i = 0; i < o.nraw; ++i) {
      fprintf (s_file, ";");
      switch (o.raw [i].type) {
      case POINT_START   :
         fprintf (s_file, "TOUCH,");
         break;
      case POINT_FINISH  :
         fprintf (s_file, "LIFT,");
         break;
      }
      fprintf (s_file, "%d,%d", o.raw[i].x_raw, o.raw[i].y_raw);
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
      OUT_pick  (o.total - 1);
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
      OUT_pick  (o.total - 1);
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
   OUT_pick  (o.total - 1);
   /*---(complete)-----------------------*/
   DEBUG_OUTP   yLOG_exit    (__FUNCTION__);
   return 0;
}



/*============================--------------------============================*/
/*===----                          unit testing                        ----===*/
/*============================--------------------============================*/
void o___UNIT_TEST______________o (void) {;}

char*        /*-> unit test accessor -----------------[ light  [us.D90.241.L0]*/ /*-[03.0000.00#.#]-*/ /*-[--.---.---.--]-*/
OUT__unit            (char *a_question, tPOINT *a_curr)
{
   /*---(locals)-----------+-----+-----+-*/
   int         i           = 0;
   char        t           [100] = "";
   /*---(initialize)---------------------*/
   strlcpy (unit_answer, "OUT unit         : unknown request", 100);
   /*---(core data)----------------------*/
   if        (strncmp (a_question, "globals"   , 20)  == 0) {
      snprintf (unit_answer, LEN_STR, "OUT globals      : ratio %4.1f, x_scale %5d, y_scale %5d", my.ratio, my.x_scale, my.y_scale);
   }
   else if   (strncmp (a_question, "bounds"    , 20)  == 0) {
      snprintf (unit_answer, LEN_STR, "OUT bounds       : x %4dm %4dx %4dw   y %4dm %4dx %4dh", my.x_min, my.x_max, my.x_wide, my.y_min, my.y_max, my.y_tall);
   }
   else if   (strncmp (a_question, "center"    , 20)  == 0) {
      snprintf (unit_answer, LEN_STR, "OUT center       : x %5.3f   y %5.3f", my.x_center, my.y_center);
   }
   else if   (strncmp (a_question, "coord"     , 20)  == 0) {
      snprintf (unit_answer, LEN_STR, "OUT coord        : %c   x %5d (%5.3f) %4d   y %5d (%5.3f) %4d", a_curr->type, a_curr->x_raw, a_curr->x_rel, a_curr->x_pos, a_curr->y_raw, a_curr->y_rel, a_curr->y_pos);
   }
   else if   (strncmp (a_question, "stats"     , 20)  == 0) {
      snprintf (unit_answer, LEN_STR, "OUT stats        : %c   s %8.2f, b %6d, r %5.2f, d %3d, q %d", a_curr->type, a_curr->slope, a_curr->icept, a_curr->rads, a_curr->degs, a_curr->quad);
   }
   else if   (strncmp (a_question, "other"     , 20)  == 0) {
      snprintf (unit_answer, LEN_STR, "OUT other        : %c   depth %5.1f, ratio %5.1f, cat %2d, sharp %c", a_curr->type, a_curr->depth, a_curr->ratio, a_curr->ccat, a_curr->sharp);
   }
   /*---(complete)-----------------------*/
   return unit_answer;
}


/*=================================(end-code)=================================*/
