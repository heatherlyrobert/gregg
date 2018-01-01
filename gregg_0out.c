/*=================================(beg-code)=================================*/
#include "gregg.h"


const float FULL_CIRCLE  = 2 * 3.14159;   // circle in radians


char
OUT_init             (void)
{
   OUT_clear ();
   /*---(locals)---------------------------*/
   FILE     *f;
   char      s[MAX_LINE]   = "";
   int       i             = 0;
   int       len           = 0;
   /*---(open file)------------------------*/
   /*> f = fopen(fname, "r");                                                         <* 
    *> if (f == NULL) return -1;                                                      <*/
   /*---(count records)--------------------*/
   /*> for (i = 0; fgets(s, MAX_LINE, f) ; ++i) {                                     <* 
    *>    len = strlen(s);                                                            <* 
    *>    if (len > 1 && s[0] != '#' && s[0] != ' ') ++o.total;                       <* 
    *> }                                                                              <*/
   /*> printf("out_init() - found %d saved outlines\n", o.total);                     <*/
   o.craw     = 1;
   o.cavg     = 1;
   o.ckey     = 1;
   o.curr     = 1;
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
         p [x_pt].xpos  = p [x_pt].ypos  = p [x_pt].xd = p [x_pt].yd = 0;
         p [x_pt].l  = p [x_pt].b  = p [x_pt].d  = p [x_pt].q  = p [x_pt].cc = 0;
         p [x_pt].s  = p [x_pt].r  = p [x_pt].ra = p [x_pt].c  = 0.0;
         p [x_pt].ca = p [x_pt].t  = p [x_pt].fake  = '-';
         strlcpy (p [x_pt].use, "-", 5);
      }
      /*---(done)------------------------*/
   }
   /*---(complete)-----------------------*/
   DEBUG_DATA   yLOG_sexit    (__FUNCTION__);
   return 0;
}

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

/*> char                                                                              <* 
 *> out_append         (void)                                                         <* 
 *> {                                                                                 <* 
 *>    DEBUG_O  printf("OUT_APPEND (begin)\n");                                       <* 
 *>    /+---(locals)---------------------------+/                                     <* 
 *>    FILE     *f;                                                                   <* 
 *>    char      d[500]     = "";                                                     <* 
 *>    int i;                                                                         <* 
 *>    /+---(open file)------------------------+/                                     <* 
 *>    f = fopen(oname, "a");                                                         <* 
 *>    if (f == NULL) {                                                               <* 
 *>       DEBUG_O  printf("   could not open output file\n");                         <* 
 *>       DEBUG_O  printf("OUT_APPEND (end)\n\n");                                    <* 
 *>       return -1;                                                                  <* 
 *>    }                                                                              <* 
 *>    DEBUG_O  printf("   file open for append\n");                                  <* 
 *>    /+---(write date)-----------------------+/                                     <* 
 *>    time_t      time_date = time(NULL);                                            <* 
 *>    struct tm*  curr_time = localtime(&time_date);                                 <* 
 *>    char        asc_time[20];                                                      <* 
 *>    strftime(asc_time, 20, "%y.%m.%d.%H.%M.%S", curr_time);                        <* 
 *>    strncpy(d, asc_time, 100);                                                     <* 
 *>    char  *n = strchr(d, '\n');                                                    <* 
 *>    if (n != NULL)  *n = '\0';                                                     <* 
 *>    fprintf(f, "%-20.20s | ", d);                                                  <* 
 *>    /+---(leave space for answer)-----------+/                                     <* 
 *>    fprintf(f, "%-20.20s | ", " ");                                                <* 
 *>    /+---(leave space for notes)------------+/                                     <* 
 *>    fprintf(f, "%-30.30s | ", " ");                                                <* 
 *>    /+---(write count)----------------------+/                                     <* 
 *>    fprintf(f, "%03d | ", o.nraw);                                                 <* 
 *>    /+---(process)--------------------------+/                                     <* 
 *>    for (i = 0; i < o.nraw; ++i) {                                                 <* 
 *>       fprintf(f, "(%+04d,%+04d)", o.raw[i].xpos, o.raw[i].ypos);                        <* 
 *>    }                                                                              <* 
 *>    /+---(finish off record)----------------+/                                     <* 
 *>    fprintf(f, " |\n");                                                            <* 
 *>    fflush(f);                                                                     <* 
 *>    fclose(f);                                                                     <* 
 *>    /+---(touch up some vars)---------------+/                                     <* 
 *>    o.saved    = 'y';                                                              <* 
 *>    ++o.total;                                                                     <* 
 *>    o.curr = o.total;                                                              <* 
 *>    /+---(complete)-------------------------+/                                     <* 
 *>    DEBUG_O  printf("OUT_APPEND (end)\n\n");                                       <* 
 *>    return 0;                                                                      <* 
 *> }                                                                                 <*/



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
   printf(" | %1d %1d %5.1f %2d %c %c %s\n", a_curr->q, a_curr->ra, a_curr->c, a_curr->cc, a_curr->ca, a_curr->t, a_curr->use);
   return 0;
}


/*=================================(end-code)=================================*/
