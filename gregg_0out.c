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
   char      s[MAX_LINE]   = "";
   int       i             = 0;
   int       len           = 0;
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
         p [x_pt].xpos = p [x_pt].ypos = p [x_pt].xd    = p [x_pt].yd = 0;
         p [x_pt].l    = p [x_pt].b    = p [x_pt].d     = p [x_pt].q  = p [x_pt].cc = 0;
         p [x_pt].s    = p [x_pt].r    = p [x_pt].ra    = p [x_pt].c  = 0.0;
         p [x_pt].ca   = p [x_pt].type = p [x_pt].fake  = '-';
         strlcpy (p [x_pt].use, "-", 5);
      }
      /*---(done)------------------------*/
   }
   /*---(complete)-----------------------*/
   DEBUG_DATA   yLOG_sexit    (__FUNCTION__);
   return 0;
}



/*============================--------------------============================*/
/*===----                            overall                           ----===*/
/*============================--------------------============================*/
void o___GENERIC________________o (void) {;}

char
POINT_calc         (tPOINT *a_curr,  int a_span)
{
   /*---(set the ends)---------------------*/
   tPOINT  *one  = a_curr - 1;
   tPOINT  *two  = a_curr;
   if (a_span == 'a')  two = a_curr + 1;
   /*---(x and y diff)---------------------*/
   a_curr->xd  = two->xpos - one->xpos;
   a_curr->yd  = two->ypos - one->ypos;
   /*---(length)---------------------------*/
   a_curr->l   = (int) sqrt((a_curr->xd * a_curr->xd) + (a_curr->yd * a_curr->yd));
   /*---(slope and intercept)--------------*/
   if      (a_curr->xd != 0) a_curr->s = (float) a_curr->yd / a_curr->xd;
   else if (a_curr->yd >  0) a_curr->s = +100000;
   else                      a_curr->s = -100000;
   a_curr->b   = (int) (a_curr->ypos - (a_curr->s * a_curr->xpos));
   /*---(radians/degrees)------------------*/
   a_curr->r   = atan2(a_curr->yd, a_curr->xd);
   if (a_curr->r > FULL_CIRCLE) a_curr->r -= FULL_CIRCLE;
   if (a_curr->r < 0)           a_curr->r += FULL_CIRCLE;
   a_curr->d   = (int) (a_curr->r * 57.29577951308);
   /*---(quadrant)-------------------------*/
   if (a_curr->xd >= 0) {
      if (a_curr->yd >= 0) a_curr->q = 1;
      else                 a_curr->q = 4;
   } else {
      if (a_curr->yd >= 0) a_curr->q = 2;
      else                 a_curr->q = 3;
   }
   /*---(correct the deg as required)------*/
   /*> if (a_curr->q == 2) a_curr->d -= 180;                                          <* 
    *> if (a_curr->q == 3) a_curr->d += 180;                                          <*/
   /*---(complete)-------------------------*/
   return 0;
}

char          /*----: list all points of a particular type -------------------*/
POINT_list         (tPOINT *a_curr, int a_count)
{
   int i;
   //---(display list)---------------------------#
   printf("\n");
   printf("point inventory----------------------------------------------------------------------\n");
   printf("### bas raw a | -xx- -yy- xd- yd- | len -slope-- icept -rad- deg | q r curve cc c t u\n");
   for (i = 0; i < a_count; ++i) {
      POINT_show (a_curr + i, i);
   }
   printf("### bas raw a | -xx- -yy- xd- yd- | len -slope-- icept -rad- deg | q r curve cc c t u\n");
   printf("-------------------------------------------------------------------------------------\n");
   printf("\n");
   //---(complete)-------------------------------#
   return 0;
}

char          /*----: display all information on an individual point ---------*/
POINT_show         (tPOINT *a_curr, int a_num)
{
   float s  = a_curr->s;
   int   b  = a_curr->b;
   if (s > 999) {
      s = 999.99;
      b = 999;
   }
   if (s < -999) {
      s = -999.99;
      b = -999;
   }
   printf("%3d %3d %3d %c", a_num, a_curr->p_bas, a_curr->p_raw, a_curr->fake);
   printf(" | %4d %4d %3d %3d", a_curr->xpos, a_curr->ypos, a_curr->xd, a_curr->yd);
   printf(" | %3d %8.2f %5d %5.2f %3d", a_curr->l, s, b, a_curr->r, a_curr->d);
   printf(" | %1d %1d %5.1f %2d %c %c %s\n", a_curr->q, a_curr->ra, a_curr->c, a_curr->cc, a_curr->ca, a_curr->type, a_curr->use);
   return 0;
}



/*============================--------------------============================*/
/*===----                         saved outlines                       ----===*/
/*============================--------------------============================*/
void o___SAVED__________________o (void) {;}


/*> char                                                                                              <* 
 *> out_pick (int a_y)                                                                                <* 
 *> {                                                                                                 <* 
 *>    if (o.total == 0) return 0;                                                                    <* 
 *>    float   x_top =  win.top    - (win.bar * 1.0) -  5;                                            <* 
 *>    float   x_bot = -win.bottom + (win.bar * 1.0) +  5;                                            <* 
 *>    float   x_ran = x_top - x_bot;                                                                 <* 
 *>    float   x_inc = (x_top - x_bot) / o.total;                                                     <* 
 *>    float   x_pos = x_ran - (a_y - x_bot);                                                         <* 
 *>    int     x_num = (x_pos / x_inc) + 1;                                                           <* 
 *>    if (a_y >= x_top) {                                                                            <* 
 *>       /+> printf("hit home/top side\n");                                              <+/         <* 
 *>       x_num = 1;                                                                                  <* 
 *>    }                                                                                              <* 
 *>    if (a_y <= x_bot) {                                                                            <* 
 *>       /+> printf("hit end/bottom side\n");                                            <+/         <* 
 *>       x_num = o.total;                                                                            <* 
 *>    }                                                                                              <* 
 *>    /+> printf("got %4dy with %4.0ft, %4.0fb, %4.0fr, %do, %4.0fi, %4.0fp, so go to %d\n",   <*    <* 
 *>     *>       a_y, x_top, x_bot, x_ran, o.total, x_inc,                                      <*    <* 
 *>     *>       x_pos, x_num);                                                                 <+/   <* 
 *>    if (x_num != o.curr) {                                                                         <* 
 *>       out_read(x_num);                                                                            <* 
 *>       /+> POINT_list (key, o.nkey);                                                     <+/          <* 
 *>    }                                                                                              <* 
 *>    return 0;                                                                                      <* 
 *> }                                                                                                 <*/

/*> char                                                                                        <* 
 *> out_read (int a_num)                                                                        <* 
 *> {                                                                                           <* 
 *>    DEBUG_I  printf("OUTLINE READ (begin)\n");                                               <* 
 *>    DEBUG__RAW  printf("RAW POINTS (begin)\n");                                              <* 
 *>    OUT_wipe ();                                                                             <* 
 *>    /+---(locals)---------------------------+/                                               <* 
 *>    FILE     *f;                                                                             <* 
 *>    char      s[MAX_LINE] = "";         // current record                                    <* 
 *>    int       n = 0;                    // current record number in file                     <* 
 *>    int       len       = 0;                                                                 <* 
 *>    int i;                                                                                   <* 
 *>    /+---(open file)------------------------+/                                               <* 
 *>    f = fopen(fname, "r");                                                                   <* 
 *>    if (f == NULL) return -1;                                                                <* 
 *>    /+---(process)--------------------------+/                                               <* 
 *>    while (fgets(s, MAX_LINE, f)) {                                                          <* 
 *>       ++n;                                                                                  <* 
 *>       if (a_num != n)                            continue;                                  <* 
 *>       len = strlen(s);                                                                      <* 
 *>       /+> if (len > 1 && s[0] != '#' && s[0] != ' ') continue;                        <+/   <* 
 *>       /+> if (s[0] == '#' || s[0] == '\n')           return 0;                        <+/   <* 
 *>       char   *p = NULL;             // pointer to substring                                 <* 
 *>       char   *q1 = "|";             // delimiters                                           <* 
 *>       char   *q2 = "(),";           // delimiters                                           <* 
 *>       int     x_count = 0;          // count of raw points                                  <* 
 *>       int     x = 0;                // raw x-coord                                          <* 
 *>       int     y = 0;                // raw y-coord                                          <* 
 *>       char    x_info[100] = "";                                                             <* 
 *>       o.curr = a_num;                                                                       <* 
 *>       DEBUG_I  printf("   index    = %d\n", o.curr);                                        <* 
 *>       /+---(date)-------------------------+/                                                <* 
 *>       p = strtok(s, q1);                                                                    <* 
 *>       if (p == NULL)     return -1;                                                         <* 
 *>       /+> printf("date = <<%s>>\n", p);                                            <+/      <* 
 *>       strncpy(o.when,   str_trim(p), 50);                                                   <* 
 *>       DEBUG_I  printf("   date     = %s\n", o.when);                                        <* 
 *>       /+---(expected result)--------------+/                                                <* 
 *>       p = strtok(NULL, q1);                                                                 <* 
 *>       if (p == NULL)     return -1;                                                         <* 
 *>       /+> printf("name = <<%s>>\n", p);                                            <+/      <* 
 *>       strncpy(o.expect, str_trim(p), 50);                                                   <* 
 *>       DEBUG_I  printf("   expected = %s\n", o.expect);                                      <* 
 *>       /+---(note)-------------------------+/                                                <* 
 *>       p = strtok(NULL, q1);                                                                 <* 
 *>       if (p == NULL)     return -1;                                                         <* 
 *>       /+> printf("name = <<%s>>\n", p);                                            <+/      <* 
 *>       strncpy(o.note,   str_trim(p), 50);                                                   <* 
 *>       DEBUG_I  printf("   note     = %s\n", o.note);                                        <* 
 *>       /+---(count)------------------------+/                                                <* 
 *>       p = strtok(NULL, q1);                                                                 <* 
 *>       if (p == NULL)     return -1;                                                         <* 
 *>       x_count = atoi(p);                                                                    <* 
 *>       DEBUG_I  printf("   expecting %4d points\n", x_count);                                <* 
 *>       /+> printf("out_read(%d) has %d points\n", a_num, x_count);                  <+/      <* 
 *>       p = strtok(NULL, q2);                                                                 <* 
 *>       for (i = 0; i < x_count; ++i) {                                                       <* 
 *>          p = strtok(NULL, q2);                                                              <* 
 *>          if (p == NULL)  {                                                                  <* 
 *>             DEBUG_I  printf("(%04d) x=null\n", i);                                          <* 
 *>             break;                                                                          <* 
 *>          }                                                                                  <* 
 *>          x = atoi(p);                                                                       <* 
 *>          p = strtok(NULL, q2);                                                              <* 
 *>          if (p == NULL) {                                                                   <* 
 *>             DEBUG_I  printf("(%04d) x=%03d, y=null\n", i, x);                               <* 
 *>             break;                                                                          <* 
 *>          }                                                                                  <* 
 *>          y = atoi(p);                                                                       <* 
 *>          /+> printf("out_read %4dx, %4dy\n", x, y);                                <+/      <* 
 *>          if      (i == 0)               RAW_read  (x, y);                                   <* 
*>          else if (i == x_count - 1)     RAW_end   (x, y);                                   <* 
*>          else                           RAW_add   (x, y);                                   <* 
*>          sprintf(x_info, "(%04d) x=%03d, y=%03d", i, x, y);                                 <* 
*>       }                                                                                     <* 
*>    }                                                                                        <* 
*>    DEBUG_I  printf("   took in %4d points\n", o.nraw);                                      <* 
*>    DEBUG_I  printf("OUTLINE READ (end)\n\n");                                               <* 
*>    DEBUG__RAW  POINT_list (o.raw, o.nraw);                                                     <* 
*>    DEBUG__RAW  printf("RAW POINTS (end)\n\n");                                              <* 
*>    o.craw     = 1;                                                                          <* 
*>    o.cavg     = 1;                                                                          <* 
*>    o.ckey     = 1;                                                                          <* 
*>    o.saved    = 'y';                                                                        <* 
*>    fclose(f);                                                                               <* 
*>    BASE_filter   ();                                                                        <* 
*>    KEY_filter    ();                                                                        <* 
*>    match_flatten ();                                                                        <* 
*>    match_squeeze ();                                                                        <* 
*>    circle_driver ();                                                                        <* 
*>    match_sharps  ();                                                                        <* 
*>    match_driver  ();                                                                        <* 
*>    /+> DEBUG__KEY  POINT_list (o.key, o.nkey);                                           <+/   <* 
*>    return 0;                                                                                <* 
*> }                                                                                           <*/


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
   if (s_nfield >=  7)  strlcpy (o.points, s_fields [6], LEN_RECD);
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
      DEBUG_INPT   yLOG_note    ("already saved");
      DEBUG_INPT   yLOG_exit    (__FUNCTION__);
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
      switch (o.raw [i].type) {
      case POINT_START   :
         fprintf (s_file, ";TOUCH,%d,%d", o.raw[i].xpos + o.xadj, o.raw[i].ypos + o.yadj);
         break;
      case POINT_FINISH  :
         fprintf (s_file, ";LIFT,%d,%d", o.raw[i].xpos + o.xadj, o.raw[i].ypos + o.yadj);
         break;
      default :
         fprintf (s_file, ";%d,%d", o.raw[i].xpos + o.xadj, o.raw[i].ypos + o.yadj);
         break;
      }
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
