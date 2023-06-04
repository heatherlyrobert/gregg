/*============================---(source-start)---============================*/
#include "gregg.h"




#define    MAX_LETTERS   1000
typedef struct cLETTER tLETTER;
static struct cLETTER
{
   /*---(header)------------*/
   short     l_line;                             /* source file line  */
   char      l_label       [LEN_TERSE];          /* name              */
   /*---(creation)----------*/
   char      l_type;                             /* draw function     */
   float     l_xrad;                             /* ellipse x-radius  */
   float     l_yrad;                             /* ellipse y-radius  */
   float     l_rot;                              /* ellipse rotation  */
   float     l_beg;                              /* arc begin         */
   float     l_arc;                              /* arc length        */
   /*---(grouping)----------*/
   uchar     l_cat;                              /* letter group      */
   char      l_code;                             /* single char id    */
   /*---(trend)-------------*/
   float     l_xend;                             /* ending x          */
   float     l_yend;                             /* ending y          */
   float     l_deg;                              /* degrees of slope  */
   float     l_xylen;                            /* full length       */
   /*---(bounds)------------*/
   float     l_lef;                              /* leftmost          */
   float     l_rig;                              /* rightmost         */
   float     l_top;                              /* topmost           */
   float     l_bot;                              /* bottommost        */
   /*---(count)-------------*/
   short     l_points;                           /* number of points  */
   /*---(count)-------------*/
   short     l_base;                             /* in how many bases */
   short     l_used;                             /* in how many words */
   /*---(done)--------------*/
};
static tLETTER     s_letter [MAX_LETTERS];
static short       s_nletter;



/*============================--------------------============================*/
/*===----                        program level                         ----===*/
/*============================--------------------============================*/
static void o___PROGRAM___________________o (void) {;}

char
LETTER_purge            (void)
{
   /*---(locals)-----------+-----------+-*/
   int         i           =    0;
   int         j           =    0;
   for (i = 0; i < LEN_FULL; ++i) {
      /*---(header)------------*/
      s_letter [i].l_line   =   0;
      strlcpy (s_letter [i].l_label  , "", LEN_TERSE);
      /*---(creation)----------*/
      s_letter [i].l_type   = '·';
      s_letter [i].l_xrad   = 0.0;
      s_letter [i].l_yrad   = 0.0;
      s_letter [i].l_rot    = 0.0;
      s_letter [i].l_beg    = 0.0;
      s_letter [i].l_arc    = 0.0;
      /*---(grouping)----------*/
      s_letter [i].l_cat    = '·';
      s_letter [i].l_code   = '·';
      /*---(trend)-------------*/
      s_letter [i].l_xend   = 0.0;
      s_letter [i].l_yend   = 0.0;
      s_letter [i].l_deg    = 0.0;
      s_letter [i].l_xylen  = 0.0;
      /*---(bounds)------------*/
      s_letter [i].l_lef    = 0.0;
      s_letter [i].l_rig    = 0.0;
      s_letter [i].l_top    = 0.0;
      s_letter [i].l_bot    = 0.0;
      /*---(count)-------------*/
      s_letter [i].l_points =   0;
      /*---(count)-------------*/
      s_letter [i].l_base   =   0;
      s_letter [i].l_used   =   0;
      /*---(done)--------------*/
   }
   s_nletter = 0;
   /*---(complete)-----------------------*/
   return 0;
}

char
LETTER_init             (void)
{
   strlcpy (my.n_letter, "/var/lib/gregg/letter.txt", LEN_HUND);
   return LETTER_purge ();
}

char LETTER_wrap     (void)  { return LETTER_purge (); }



/*============================--------------------============================*/
/*===----                          data load                           ----===*/
/*============================--------------------============================*/
static void o___LOAD______________________o (void) {;}

/*> /+---(header)------------+/                                                    <* 
 *> short     l_line;                             /+ source file line  +/          <* 
 *> char      l_label       [LEN_TERSE];          /+ name              +/          <* 
 *> /+---(creation)----------+/                                                    <* 
 *> char      l_type;                             /+ draw function     +/          <* 
 *> float     l_xrad;                             /+ ellipse x-radius  +/          <* 
 *> float     l_yrad;                             /+ ellipse y-radius  +/          <* 
 *> float     l_rot;                              /+ ellipse rotation  +/          <* 
 *> float     l_beg;                              /+ arc begin         +/          <* 
 *> float     l_arc;                              /+ arc length        +/          <* 
 *> /+---(grouping)----------+/                                                    <* 
 *> uchar     l_cat;                              /+ letter group      +/          <* 
 *> char      l_code;                             /+ single char id    +/          <* 
 *> /+---(count)-------------+/                                                    <* 
 *> short     l_used;                             /+ number of points  +/          <* 
 *> /+---(done)--------------+/                                                    <*/

char
LETTER_handler          (int n, char a_verb [LEN_LABEL], char a_exist, void *a_handler)
{
   /*---(locals)-----------+-----------+-*/
   char        rce         =  -10;
   int         rc          =    0;
   int         c           =    0;
   char        t           [LEN_LABEL] = "";
   short       s           =    0;
   /*---(header)-------------------------*/
   DEBUG_INPT  yLOG_enter   (__FUNCTION__);
   /*---(defense)------------------------*/
   DEBUG_INPT  yLOG_point   ("a_verb"    , a_verb);
   --rce;  if (a_verb == NULL || strlen (a_verb) <= 0) {
      DEBUG_INPT  yLOG_exit    (__FUNCTION__);
      yURG_err ('f', "letter handler called with null/empty verb");
      return rce;
   }
   DEBUG_INPT  yLOG_info    ("a_verb"    , a_verb);
   /*---(check fields)-------------------*/
   rc = yPARSE_ready (&c);
   DEBUG_INPT  yLOG_value   ("fields"    , c);
   --rce;  if (c < 9) {
      DEBUG_INPT  yLOG_exit    (__FUNCTION__);
      yURG_err ('f', "not enough fields for letter");
      return rce;
   }
   /*---(pull all)-----------------------*/
   rc = yPARSE_scanf (a_verb, "CkkkkksC", &(s_letter [s_nletter].l_type), &(s_letter [s_nletter].l_xrad), &(s_letter [s_nletter].l_yrad), &(s_letter [s_nletter].l_rot), &(s_letter [s_nletter].l_beg), &(s_letter [s_nletter].l_arc), &s, &(s_letter [s_nletter].l_code));
   DEBUG_INPT  yLOG_value   ("handler"   , rc);
   --rce;  if (rc < 0) {
      DEBUG_INPT  yLOG_exit    (__FUNCTION__);
      yURG_err ('f', "yparse could not process record");
      return rce;
   }
   /*---(finish)-------------------------*/
   s_letter [s_nletter].l_line = n;
   strlcpy (s_letter [s_nletter].l_label, a_verb, LEN_TERSE);
   s_letter [s_nletter].l_cat  = s;
   DEBUG_INPT  yLOG_complex ("final"     , "%3d#  %3d  %-10.10s  %-6.1fx  %-6.1fy  %-6.1fr  %-6.1fb  %-6.1fa  %2d  %c", s_nletter, s_letter [s_nletter].l_line, s_letter [s_nletter].l_label, s_letter [s_nletter].l_xrad, s_letter [s_nletter].l_xrad, s_letter [s_nletter].l_arc, s_letter [s_nletter].l_beg, s_letter [s_nletter].l_arc, s_letter [s_nletter].l_cat, s_letter [s_nletter].l_code);
   ++s_nletter;
   DEBUG_INPT  yLOG_value   ("s_nletter" , s_nletter);
   /*---(complete)-----------------------*/
   DEBUG_INPT  yLOG_exit    (__FUNCTION__);
   return rc;
}

char
LETTER_load             (char a_fname [LEN_HUND])
{
   /*---(locals)-----------+-----+-----+-*/
   char        rce         =  -10;
   char        rc          =    0;
   /*---(header)-------------------------*/
   DEBUG_INPT  yLOG_enter   (__FUNCTION__);
   /*---(begin feedback)-----------------*/
   yURG_msg ('>', "load the latest letter file...");
   DEBUG_INPT   yLOG_point   ("a_fname"   , a_fname);
   --rce;  if (a_fname == NULL) {
      yURG_err ('f', "file name not provided to function");
      DEBUG_INPT  yLOG_exitr   (__FUNCTION__, rce);
      return rce;
   }
   yURG_msg ('-', "requested file %då%sæ", strlen (a_fname), a_fname);
   DEBUG_INPT   yLOG_info    ("a_fname"   , a_fname);
   /*---(read all lines)-----------------*/
   yURG_msg ('-', "calling auto-reader in yPARSE");
   rc = yPARSE_autoread (a_fname, NULL, LETTER_handler);
   DEBUG_PROG  yLOG_value   ("read"      , rc);
   /*---(trouble)------------------------*/
   --rce;  if (rc <  0) {
      DEBUG_PROG  yLOG_note    ("no file found");
      yURG_err ('f', "file not found/accessible");
      yURG_msg (' ', "");
      DEBUG_PROG  yLOG_exitr   (__FUNCTION__, rce);
      return rce;
   }
   /*---(show success)-------------------*/
   yURG_msg ('>', "all read correctly, SUCCESS, accepted %d", s_nletter);
   yURG_msg (' ', "");
   /*---(complete)-----------------------*/
   DEBUG_INPT  yLOG_exit    (__FUNCTION__);
   return 0;
}



/*============================--------------------============================*/
/*===----                        debug support                         ----===*/
/*============================--------------------============================*/
static void o___DEBUG_____________________o (void) {;}

short  LETTER_count     (void)  { return s_nletter; }

char*
LETTER_detail           (uchar n)
{
   strcpy  (g_print, "n/a");
   if (n < s_nletter) {
      sprintf (g_print, "%-2d  %-2d  %-10.10s ´ %6.1fx  %6.1fy  %6.1fr  %6.1fb  %6.1fa ´ %2d  %c ´ %6.1fx  %6.1fy  %6.1fd  %6.1fl ´ %6.1fl  %6.1fr  %6.1ft  %6.1fb ´ %3dp  %3db  %3du ´",
            n, s_letter [n].l_line, s_letter [n].l_label,
            s_letter [n].l_xrad , s_letter [n].l_yrad , s_letter [n].l_rot  , s_letter [n].l_beg  , s_letter [n].l_arc  ,
            s_letter [n].l_cat  , s_letter [n].l_code ,
            s_letter [n].l_xend , s_letter [n].l_yend , s_letter [n].l_deg  , s_letter [n].l_xylen,
            s_letter [n].l_lef  , s_letter [n].l_rig  , s_letter [n].l_top  , s_letter [n].l_bot  ,
            s_letter [n].l_points,
            s_letter [n].l_base , s_letter [n].l_used );
   }
   return g_print;
}



/*============================--------------------============================*/
/*===----                       finding letters                        ----===*/
/*============================--------------------============================*/
static void o___FIND______________________o (void) {;}

short 
LETTER_by_name          (char a_ltr [LEN_TERSE], char a_scope, char *r_type, char *r_lcat, char r_label [LEN_TERSE], float *r_xrad, float *r_yrad, float *r_rot, float *r_beg, float *r_arc)
{
   /*---(locals)-----------+-----+-----+-*/
   char        rce         =  -10;
   short       i           =    0;
   /*---(default)------------------------*/
   if (r_type    != NULL)  *r_type    = '-';
   if (r_lcat    != NULL)  *r_lcat    = 0;
   if (r_label   != NULL)  strcpy (r_label, "");
   if (r_xrad    != NULL)  *r_xrad    = 0;
   if (r_yrad    != NULL)  *r_yrad    = 0;
   if (r_rot     != NULL)  *r_rot     = 0;
   if (r_beg     != NULL)  *r_beg     = 0;
   if (r_arc     != NULL)  *r_arc     = 0;
   /*---(defense)------------------------*/
   --rce;  if (a_ltr == NULL)  return rce;
   /*---(walk letters)-------------------*/
   for (i = 0; i < MAX_LETTERS; ++i) {
      /*---(check end)-------------------*/
      if (a_scope != LTRS_ALL && strcmp (s_letter [i].l_label, "END") == 0)    break;
      if (strcmp (s_letter [i].l_label, "EOF") == 0)                           break;
      /*---(check match)-----------------*/
      if (strcmp (a_ltr, s_letter [i].l_label) != 0)                           continue;
      /*---(found)-----------------------*/
      if (r_type    != NULL)  *r_type    = s_letter [i].l_type;
      if (r_lcat    != NULL)  *r_lcat    = s_letter [i].l_cat;
      if (r_label   != NULL)  strlcpy (r_label, s_letter [i].l_label, LEN_TERSE);
      if (r_xrad    != NULL)  *r_xrad    = s_letter [i].l_xrad;
      if (r_yrad    != NULL)  *r_yrad    = s_letter [i].l_yrad;
      if (r_rot     != NULL)  *r_rot     = s_letter [i].l_rot;
      if (r_beg     != NULL)  *r_beg     = s_letter [i].l_beg;
      if (r_arc     != NULL)  *r_arc     = s_letter [i].l_arc;
      return i;
   }
   /*---(complete)-----------------------*/
   return --rce;
}

char 
LETTER_by_index         (short n, char *r_type, char *r_lcat, char r_label [LEN_TERSE], float *r_xrad, float *r_yrad, float *r_rot, float *r_beg, float *r_arc)
{
   /*---(locals)-----------+-----+-----+-*/
   char        rce         =  -10;
   /*---(default)------------------------*/
   if (r_type    != NULL)  *r_type    = '-';
   if (r_lcat    != NULL)  *r_lcat    = 0;
   if (r_label   != NULL)  strcpy (r_label, "");
   if (r_xrad    != NULL)  *r_xrad    = 0;
   if (r_yrad    != NULL)  *r_yrad    = 0;
   if (r_rot     != NULL)  *r_rot     = 0;
   if (r_beg     != NULL)  *r_beg     = 0;
   if (r_arc     != NULL)  *r_arc     = 0;
   /*---(defense)------------------------*/
   --rce;  if (n < 0 || n >= MAX_LETTERS)  return rce;
   /*---(save data)-------------------*/
   if (r_type    != NULL)  *r_type    = s_letter [n].l_type;
   if (r_lcat    != NULL)  *r_lcat    = s_letter [n].l_cat;
   if (r_label   != NULL)  strlcpy (r_label, s_letter [n].l_label, LEN_TERSE);
   if (r_xrad    != NULL)  *r_xrad    = s_letter [n].l_xrad;
   if (r_yrad    != NULL)  *r_yrad    = s_letter [n].l_yrad;
   if (r_rot     != NULL)  *r_rot     = s_letter [n].l_rot;
   if (r_beg     != NULL)  *r_beg     = s_letter [n].l_beg;
   if (r_arc     != NULL)  *r_arc     = s_letter [n].l_arc;
   /*---(complete)-----------------------*/
   return 0;
}

char 
LETTER_sizing           (short n, float *r_xend, float *r_yend, float *r_deg, float *r_xylen, float *r_lef, float *r_rig, float *r_top, float *r_bot, short *r_points)
{
   /*---(locals)-----------+-----+-----+-*/
   char        rce         =  -10;
   /*---(default)------------------------*/
   if (r_xend    != NULL)  *r_xend    = 0;
   if (r_yend    != NULL)  *r_yend    = 0;
   if (r_deg     != NULL)  *r_deg     = 0;
   if (r_xylen   != NULL)  *r_xylen   = 0;
   if (r_lef     != NULL)  *r_lef     = 0;
   if (r_rig     != NULL)  *r_rig     = 0;
   if (r_top     != NULL)  *r_top     = 0;
   if (r_bot     != NULL)  *r_bot     = 0;
   if (r_points  != NULL)  *r_points  = 0;
   /*---(defense)------------------------*/
   --rce;  if (n < 0 || n >= MAX_LETTERS)  return rce;
   /*---(save data)-------------------*/
   if (r_xend    != NULL)  *r_xend    = s_letter [n].l_xend;
   if (r_yend    != NULL)  *r_yend    = s_letter [n].l_yend;
   if (r_deg     != NULL)  *r_deg     = s_letter [n].l_deg;
   if (r_xylen   != NULL)  *r_xylen   = s_letter [n].l_xylen;
   if (r_lef     != NULL)  *r_lef     = s_letter [n].l_lef;
   if (r_rig     != NULL)  *r_rig     = s_letter [n].l_rig;
   if (r_top     != NULL)  *r_top     = s_letter [n].l_top;
   if (r_bot     != NULL)  *r_bot     = s_letter [n].l_bot;
   if (r_points  != NULL)  *r_points  = s_letter [n].l_points;
   /*---(complete)-----------------------*/
   return 0;
}



/*====================-----------------==-----------------====================*/
/*===----                      database handling                       ----===*/
/*====================-----------------==-----------------====================*/
static void o___DATABASE__________________o (void) {;}

char
LETTER_write            (FILE *a_file)
{
   /*---(locals)-----------+-----+-----+-*/
   char        rce         =  -10;
   char        rc          =    0;
   short       i           =    0;
   /*---(header)-------------------------*/
   DEBUG_OUTP   yLOG_enter   (__FUNCTION__);
   /*---(defense)------------------------*/
   DEBUG_OUTP  yLOG_point   ("a_file"    , a_file);
   --rce;  if (a_file == NULL) {
      DEBUG_OUTP  yLOG_exitr   (__FUNCTION__, rce);
      return rce;
   }
   DEBUG_OUTP  yLOG_value   ("s_nletter" , s_nletter);
   --rce;  if (s_nletter <= 0) {
      DEBUG_OUTP  yLOG_exitr   (__FUNCTION__, rce);
      return rce;
   }
   /*---(write all)----------------------*/
   for (i = 0; i < s_nletter; ++i) {
      rc = fwrite (&(s_letter [i]), sizeof (tLETTER), 1, a_file);
      if (rc != 1)   break;
   }
   /*---(check)--------------------------*/
   DEBUG_OUTP  yLOG_value   ("i"         , i);
   --rce;  if (s_nletter != i) {
      DEBUG_OUTP  yLOG_exitr   (__FUNCTION__, rce);
      return rce;
   }
   /*---(complete)-----------------------*/
   DEBUG_OUTP   yLOG_exit    (__FUNCTION__);
   return 0;
}

char
LETTER_read             (FILE *a_file, short a_count)
{
   /*---(locals)-----------+-----+-----+-*/
   char        rce         =  -10;
   char        rc          =    0;
   short       i           =    0;
   /*---(header)-------------------------*/
   DEBUG_OUTP   yLOG_enter   (__FUNCTION__);
   /*---(defense)------------------------*/
   DEBUG_OUTP  yLOG_point   ("a_file"    , a_file);
   --rce;  if (a_file == NULL) {
      DEBUG_OUTP  yLOG_exitr   (__FUNCTION__, rce);
      return rce;
   }
   DEBUG_OUTP  yLOG_value   ("s_nletter" , s_nletter);
   --rce;  if (s_nletter > 0) {
      DEBUG_OUTP  yLOG_exitr   (__FUNCTION__, rce);
      return rce;
   }
   /*---(write all)----------------------*/
   for (i = 0; i < a_count; ++i) {
      rc = fread  (&(s_letter [i]), sizeof (tLETTER), 1, a_file);
      if (rc != 1)   break;
      ++s_nletter;
   }
   /*---(check)--------------------------*/
   DEBUG_OUTP  yLOG_value   ("a_count"   , a_count);
   --rce;  if (s_nletter != a_count) {
      DEBUG_OUTP  yLOG_exitr   (__FUNCTION__, rce);
      return rce;
   }
   /*---(complete)-----------------------*/
   DEBUG_OUTP   yLOG_exit    (__FUNCTION__);
   return 0;
}




