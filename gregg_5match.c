/*============================---(source-start)---============================*/

#include "gregg.h"




/*============================--------------------============================*/
/*===----                           matching                           ----===*/
/*============================--------------------============================*/

PRIV struct {
   int       norm;                     /* number of smooth open points        */
   int       sharps;                   /* number of sharp open points at end  */
   char      oshape[10];               /* outline/actual sharp/smooth shape   */
   char      gshape[10];               /* group sharp/smooth shape for test   */
   char      oflow [10];               /* outline/actual quadrant flow        */
   int       xd;                       /* x-axis difference                   */
   int       yd;                       /* y-axis difference                   */
   int       l;                        /* length                              */
   int       la;                       /* length category                     */
   int       d;                        /* degree                              */
   float     s;                        /* slope                               */
   int       b;
   int       q;
   int       qs;
   float     c;
   float     cm;
   char      ca;
   int       cc;
   int       r;
   int       r2;
} opens;


PRIV char combo_driver       (int a_beg, int a_count);



/*============================--------------------============================*/
/*===----                             support                          ----===*/
/*============================--------------------============================*/
static void      o___SUPPORT_________________o (void) {;}

char
match_calc (int a_start, int a_count)
{
   /*---(get points)----------------------------*/
   int         i           = 0;
   int p1         = o.key[a_start              ].p_bas;
   int p2         = o.key[a_start + 1          ].p_bas;
   int p3         = o.key[a_start + a_count - 1].p_bas;
   int p4         = o.key[a_start + a_count    ].p_bas;
   /*---(get coordinates)-----------------------*/
   int yy1        = o.avg[p1].ypos;
   int yy2        = o.avg[p2].ypos;
   int yy3        = o.avg[p3].ypos;
   int yy4        = o.avg[p4].ypos;
   int xx1        = o.avg[p1].xpos;
   int xx2        = o.avg[p2].xpos;
   int xx3        = o.avg[p3].xpos;
   int xx4        = o.avg[p4].xpos;
   /*---(get overall calcs)---------------------*/
   o.tmp[0].p_bas   = a_start;
   o.tmp[0].xpos   = xx1;
   o.tmp[0].ypos   = yy1;
   o.tmp[1].p_bas   = a_start + a_count;
   o.tmp[1].xpos   = xx4;
   o.tmp[1].ypos   = yy4;
   POINT_calc (o.tmp + 1, 'n');
   /*---(more variables)------------------------*/
   opens.xd   = o.tmp[1].xd;
   opens.yd   = o.tmp[1].yd;
   opens.l    = o.tmp[1].l;
   opens.s    = o.tmp[1].s;
   opens.b    = o.tmp[1].b;
   opens.d    = o.tmp[1].d;
   opens.q    = o.tmp[1].q;
   opens.qs   = o.key[a_start + 1].q;
   opens.la   = match_size (opens.l);
   opens.r    = match_range (opens.d);
   opens.c    = 0;
   opens.cc   = 0;
   opens.ca   = '-';
   opens.cm   = 0;
   /*---(arch)----------------------------------*/
   float lcurve   = 0.0;
   float rcurve   = 0.0;
   float theta    = 0.0;
   int   thetad   = 0;
   float linex    = 0.0;
   float liney    = 0.0;
   float diffx    = 0.0;
   float diffy    = 0.0;
   DEBUG__CURVES  printf("RUNNING CURVE CALCS\n");
   DEBUG__CURVES  printf("key 1 = %2d at bas = %2d x=%4d, y=%4d\n", a_start, p1, xx1, yy1);
   DEBUG__CURVES  printf("key 2 = %2d at bas = %2d x=%4d, y=%4d\n", a_start + a_count, p4, xx4, yy4);
   DEBUG__CURVES  printf("so  slope = %8.3f and intercept = %4d\n", opens.s, opens.b);
   DEBUG__CURVES  printf("   pt# -xx- -yy- --lx-- --ly-- --dx-- --dy-- theta degs curve\n");
   for (i = p1 + 1; i < p4; ++i) {
      /*---(start with calc'd point)------------*/
      if (opens.s == 0.0) opens.s = 0.001;
      liney   = (opens.s * o.bas[i].xpos) + opens.b;
      linex   = (o.bas[i].ypos - opens.b) / opens.s;
      if (linex >  999) linex =  999;
      if (linex < -999) linex = -999;
      if (liney >  999) liney =  999;
      if (liney < -999) liney = -999;
      /*---(calc differences)-------------------*/
      diffx   = (o.bas[i].xpos    - linex);
      diffy   = (o.bas[i].ypos    - liney);
      if (diffx >  999) diffx =  999;
      if (diffx < -999) diffx = -999;
      if (diffy >  999) diffy =  999;
      if (diffy < -999) diffy = -999;
      /*---(get the angles)---------------------*/
      theta   = atan(diffy/diffx);
      thetad  = theta * RAD2DEG;
      opens.c = fabs(diffx * sin(theta));
      /*---(set the curve)----------------------*/
      switch (opens.r) {
      case 1:  if (diffx > 0.0) opens.c *= -1;  break;
      case 2:  if (diffy < 0.0) opens.c *= -1;  break;
      case 3:  if (diffx < 0.0) opens.c *= -1;  break;
      case 4:  if (diffx < 0.0) opens.c *= -1;  break;
      case 5:  if (diffx < 0.0) opens.c *= -1;  break;
      case 6:  if (diffy < 0.0) opens.c *= -1;  break;
      case 7:  if (diffx < 0.0) opens.c *= -1;  break;
      case 8:  if (diffx < 0.0) opens.c *= -1;  break;
      }
      /*> printf("   %3d %4d %4d %6.1f %6.1f %6.1f %6.1f %5.2f %4d %5.1f\n",                      <* 
       *>       i, o.bas[i].xpos, o.bas[i].ypos, linex, liney, diffx, diffy, theta, thetad, opens.c);   <*/
      if (opens.c > lcurve) lcurve = opens.c;
      if (opens.c < rcurve) rcurve = opens.c;
      if (fabs(opens.c) > fabs(opens.cm)) opens.cm = opens.c;
   }
   DEBUG__CURVES  printf("   pt# -xx- -yy- --lx-- --ly-- --dx-- --dy-- theta degs curve\n");
   opens.c  = opens.cm;
   opens.cc = -9;
   /*---(positive curves)----------------*/
   /*   O = line
    *   1 = bendy line
    *   2 = curve
    *   3 = arc
    *   4 = balloon
    */
   if      (opens.c >=  25.0)                                   opens.cc = +4;
   else if (opens.c >=  16.0)                                   opens.cc = +3;
   else if (opens.c >=   7.0)                                   opens.cc = +2;
   else if (opens.c >=  (opens.l * 0.15) || opens.c >=  4.0)    opens.cc = +1;
   /*---(negative curves)----------------*/
   else if (opens.c <= -25.0)                                   opens.cc = -4;
   else if (opens.c <= -16.0)                                   opens.cc = -3;
   else if (opens.c <=  -7.0)                                   opens.cc = -2;
   else if (opens.c <= -(opens.l * 0.15) || opens.c <= -4.0)    opens.cc = -1;
   else                                                         opens.cc =  0;
   o.tmp[1].c   = opens.c;
   o.tmp[1].cc  = opens.cc;
   /*> printf("between key points %d/%d (bas %02d/%02d) lcurve=%5.1f, rcurve=%5.1f, maxcurve=%5.1f, curve=%5.1f, curvec=%2d\n",   <* 
    *>       a_start, a_start + a_count, p1, p4, lcurve, rcurve, opens.cm, opens.c, opens.cc);                                    <*/
   /*---(other)---------------------------------*/
   o.tmp[1].ra  = opens.r;
   o.tmp[0].ca  = o.tmp[1].ca  = '-';
   o.tmp[0].t   = o.tmp[1].t   = '-';
   strncpy(o.tmp[0].use, "-", 3);
   strncpy(o.tmp[1].use, "-", 3);
   /*---(complete)------------------------------*/
   /*> printf("deg = %4d, range = %2d; and len = %4d, size = %d; and curve = %6.2f, curvea = %2d\n",   <* 
    *>       opens.d, opens.r, opens.l, size, opens.c, opens.cc);                                                 <*/
   return 0;
}

int
match_size (int a_len)
{
   /*---(size)----------------------------------*/
   /* smaller looks like 6/20/35
    * larger          is 6/24/40
    */
   int    x_size  = -1;
   if      (a_len < SIZE_M0)    x_size = 0;
   else if (a_len < SIZE_M1)    x_size = 1;
   else if (a_len < SIZE_M2)    x_size = 2;
   else if (a_len < SIZE_M3)    x_size = 3;
   else                         x_size = 4;
   return x_size;
}

int
match_range (int a_deg)
{
   int i = 0;
   /*---(range)---------------------------------*/
   if (a_deg >  180) a_deg -= 360;
   if (a_deg < -180) a_deg += 360;
   int  x_range      = -1;
   opens.r2  = -1;
   for (i = 0; i < MAX_RANGES; ++i) {
      if (strncmp(ranges[i].nam, "eof", 5) == 0) break;
      if (a_deg <= ranges[i].min) continue;
      if (a_deg >  ranges[i].max) continue;
      if (x_range < 0) x_range   = ranges[i].num;
      else             opens.r2  = ranges[i].num;
   }
   return x_range;
}

char
match_sharps (void)
{
   int i = 0;
   int  avg_pt;
   int  d1, d2, dd;
   int  sharp = 70;
   /*> int  sharp = 60;                                                               <*/
   /*> o.key[o.nkey - 1].ca = 'x';                                                    <*/
   DEBUG__SHARPS  printf("RUNNING SHARPS (beg) :: tolerance at >= %d\n", sharp);
   for (i = 1; i < o.nkey - 1; ++i) {
      DEBUG__SHARPS  printf("  %02d) ", i);
      if (o.key[i    ].ca != '-' || o.key[i + 1].ca != '-') {
         DEBUG__SHARPS  printf("skipping as its marked (%c) or the next is (%c)\n", o.key[i].ca, o.key[i + 1].ca);
         continue;
      }
      avg_pt = o.key[i].p_bas;
      d1     = o.avg[avg_pt - 1].d;
      d2     = o.avg[avg_pt + 1].d;
      dd     = d1 - d2;
      if (dd >  180) dd -= 360;
      if (dd < -180) dd += 360;
      DEBUG__SHARPS  printf(" is %5d vs. %5d, so %5d   ", d1, d2, abs(dd));
      o.key[i].ca = '-';
      if (abs(dd) >= sharp) {
         o.key[i].ca = 'x';
         DEBUG__SHARPS  printf("SHARP\n");
      }
      /*> } else DEBUG__SHARPS  printf("no\n");                                       <*/
   }
   DEBUG__SHARPS  printf("RUNNING SHARPS (end)\n");
   return 0;
}

char
match_flatten    (void)
{
   /*---(locals)-------------------------*/
   int       i         = 0;
   int       dd        = 0;
   /*---(report out)---------------------*/
   DEBUG__LINES  printf("STRAIGHTENING LINES (begin)\n");
   DEBUG__LINES  POINT_list (o.key, o.nkey);
   /*---(process)------------------------*/
   DEBUG__LINES  printf("   start...\n");
   while (i < o.nkey - 1) {
      DEBUG__LINES  printf("   %3d/%3d) q=%d and q=%d ", i, i + 1, o.key[i].q, o.key[i + 1].q);
      /*---(skip start and finishes)-----*/
      if (o.key[i    ].t == '>' || o.key[i + 1].t == '>') {
         DEBUG__LINES  printf(": touches a line start, skipping\n");
         ++i;
         continue;
      }
      /*---(initial calcs)---------------*/
      dd = abs(o.key[i].d - o.key[i + 1].d);
      if (dd > 180) dd = 360 - dd;
      DEBUG__LINES  printf("dd=%4d : ", dd);
      /*---(1 to 4)----------------------*/
      if (o.key[i].q == 1 && o.key[i + 1].q == 4) {
         DEBUG__LINES  printf("#1 d1=%4d vs. d2=%4d : ", o.key[i].d, o.key[i + 1].d);
         if (dd <= 25 && o.key[i].d <= 20 && o.key[i + 1].d >= 340) {
            DEBUG__LINES  printf("can be removed");
            KEY_del (i);
            i = 0;
         } else {
            DEBUG__LINES  printf("no match");
         }
      }
      /*---(4 to 1)----------------------*/
      else if (o.key[i].q == 4 && o.key[i + 1].q == 1) {
         DEBUG__LINES  printf("#2 d1=%4d vs. d2=%4d : ", o.key[i].d, o.key[i + 1].d);
         if (dd <= 25 && o.key[i].d >= 340 && o.key[i + 1].d <= 20) {
            DEBUG__LINES  printf("can be removed");
            KEY_del (i);
            i = 0;
         } else {
            DEBUG__LINES  printf("no match");
         }
      }
      /*---(same quad)-------------------*/
      else if (o.key[i].q == o.key[i + 1].q) {
         DEBUG__LINES  printf("#3 same quadrant       : can be removed");
         KEY_del (i);
         i = 0;
      }
      /*---(otherwise)-------------------*/
      else {
         DEBUG__LINES  printf("no rule");
      }
      /*---(wrap)------------------------*/
      DEBUG__LINES  printf("\n");
      if (i == 0) DEBUG__LINES  printf("   restart tests after delete ---------\n");
      ++i;
   }
   DEBUG__LINES  printf("   done\n\n");
   /*---(report out)---------------------*/
   DEBUG__LINES  POINT_list (o.key, o.nkey);
   /*---(complete)-------------------------------*/
   DEBUG__LINES  printf("STRAIGHTENING LINES (end)\n\n");
   return 0;
}

char
match_squeeze    (void)
{
   /*---(locals)-------------------------*/
   int       i         = 0;
   int       dd        = 0;
   /*---(report out)---------------------*/
   DEBUG__LINES  printf("SQUEEZING LINES (begin)\n");
   DEBUG__LINES  POINT_list (o.key, o.nkey);
   /*---(process)------------------------*/
   DEBUG__LINES  printf("   start...\n");
   while (i < o.nkey - 1) {
      DEBUG__LINES  printf("   %3d/%3d) q=%d and %3d q=%d ", i, i + 1, o.key[i].q, i + 1, o.key[i + 1].q);
      /*---(skip start and finishes)-----*/
      if (o.key[i    ].t == '>' || o.key[i + 1].t == '>') {
         DEBUG__LINES  printf(": touches a line start, skipping\n");
         ++i;
         continue;
      }
      /*---(initial calcs)---------------*/
      dd = abs(o.key[i].d - o.key[i + 1].d);
      DEBUG__LINES  printf("dd=%3d : ", dd);
      if (o.key[i].q == 4 && o.key[i + 1].q == 3) {
         DEBUG__LINES  printf("rule #1 d1=%4d vs. d2=%4d : ", o.key[i].d, o.key[i + 1].d);
         if (dd <= 20 && o.key[i].d <= 280 && o.key[i + 1].d >= 260) {
            DEBUG__LINES  printf("can be removed");
            KEY_del (i);
            i = 0;
         } else {
            DEBUG__LINES  printf("no match");
         }
      } else if (o.key[i].q == 3 && o.key[i + 1].q == 4) {
         DEBUG__LINES  printf("rule #2 d1=%4d vs. d2=%4d : ", o.key[i].d, o.key[i + 1].d);
         if (dd <= 20 && o.key[i].d >= 260 && o.key[i + 1].d <= 280) {
            DEBUG__LINES  printf("can be removed");
            KEY_del (i);
            i = 0;
         } else {
            DEBUG__LINES  printf("no match");
         }
      } else if (o.key[i].q == o.key[i + 1].q) {
         DEBUG__LINES  printf("rule #3 same quadrant       : can be removed");
         KEY_del (i);
         i = 0;
      } else {
         DEBUG__LINES  printf("no rule");
      }
      DEBUG__LINES  printf("\n");
      if (i == 0) DEBUG__LINES  printf("   restart tests after delete ---------\n");
      ++i;
   }
   DEBUG__LINES  printf("   done\n\n");
   /*---(report out)---------------------*/
   DEBUG__LINES  POINT_list (o.key, o.nkey);
   /*---(complete)-------------------------------*/
   DEBUG__LINES  printf("SQUEEZING LINES (end)\n\n");
   return 0;
}



/*============================--------------------============================*/
/*===----                            matching                          ----===*/
/*============================--------------------============================*/
static void      o___MATCHING________________o (void) {;}

PRIV char     /*----: compare the open shape to the rule ---------------------*/
match_flow         (int a_rule, int a_beg)
{
   /*---(locals)-------------------------*/
   int       i         = 0;
   int       quad      = 0;                 /* current quadrant quad          */
   int       curr      = 0;                 /* current quadrant quad          */
   char      match     = 1;
   int       len       = 0;                 /* length of flow                 */
   /*---(prepare)------------------------*/
   len   = strlen(groups[a_rule].fl);
   /*---(process)------------------------*/
   for (i = 0; i < 4; ++i) {
      /*---(defense)---------------------*/
      if (i >= len || strchr("0- _", groups[a_rule].fl[i]) != 0) {
         DEBUG__MATCHES  printf("... ");
         continue;
      }
      /*---(gather)----------------------*/
      quad  = groups[a_rule].fl[i] - '0';
      curr  = o.key[a_beg + i + 1].q;
      /*---(grade)-----------------------*/
      if (quad != curr) {
         DEBUG__MATCHES  printf("%1dx%1d ", curr, quad);
         match = 0;
      } else {
         DEBUG__MATCHES  printf("%1d=%1d ", curr, quad);
      }
   }
   /*---(summary)------------------------*/
   if (match == 1) {
      DEBUG__MATCHES  printf(": ");
   } else {
      DEBUG__MATCHES  printf(": ---- ---- ---- [ bad flow  ]\n");
   }
   /*---(complete)-----------------------*/
   return match;
}

PRIV char     /*----: compare the open shape to the rule ---------------------*/
match_shape        (int a_rule, int a_beg)
{
   /*---(locals)-------------------------*/
   int       i         = 0;
   char      match     = 1;
   int       len       = 0;                 /* length of shape                */
   char      curr      = ' ';               /* current quadrant curve         */
   /*---(init)---------------------------*/
   strncpy(opens.gshape, groups[a_rule].sh, 10);
   len   = strlen(opens.gshape);
   /*---(compare)------------------------*/
   for (i = 0; i < 3; ++i) {
      curr  = o.key[a_beg + i + 1].ca;
      if        (i >= len) {
         DEBUG__MATCHES  printf("... ");
         continue;
      } else if (opens.gshape[i]  == ' ') {
         DEBUG__MATCHES  printf("... ");
         continue;
      } else if (opens.gshape[i]  == 'x') {
         DEBUG__MATCHES  printf("%c<%c ", curr, opens.gshape[i]);
         continue;
      } else if (curr             == '-') {
         DEBUG__MATCHES  printf("%c>%c ", curr, opens.gshape[i]);
         continue;
      } else {
         DEBUG__MATCHES  printf("%c!%c ", curr, opens.gshape[i]);
         match = 0;
         continue;
      }
   }
   /*---(summary)------------------------*/
   if (match == 1) {
      DEBUG__MATCHES  printf(": ");
   } else {
      DEBUG__MATCHES  printf(": --- --- --- --- : ---- ---- ---- [ bad shape ]\n");
   }
   /*---(complete)-----------------------*/
   return match;
}

PRIV char     /*----: get number of open points from current -----------------*/
match_opens        (int a_beg)
{
   /*---(locals)-------------------------*/
   int   end       = 0;      /* last open point (before end or sharp)         */
   char  cutoff    = 0;
   /*---(cycle)--------------------------*/
   opens.norm   = 0;
   opens.sharps   = 0;
   strncpy(opens.oshape, "     ", 10);
   strncpy(opens.oflow , "     ", 10);
   for (end = a_beg + 1; end < o.nkey; ++end) {
      if (strncmp(o.key[end].use, "-", 5) != 0)  break;
      /*---(count)-----------------------*/
      ++opens.sharps;
      if (cutoff == 0) ++opens.norm;
      /*---(flow)------------------------*/
      if (opens.sharps <= 5) {
         opens.oshape[opens.sharps - 1] = o.key[end].ca;
         opens.oflow [opens.sharps - 1] = o.key[end].q + '0';
      }
      /*---(check)-----------------------*/
      if (o.key[end].ca != '-') cutoff = 1;
   }
   --end;
   /*---(report out)---------------------*/
   /*> if (opens.sharps > 0) {                                                                       <* 
    *>    printf("\n   OPEN : beg=%2d, end=%2d, opens=%2d, nines=%2d extra   [%-5.5s] [%-5.5s]\n",   <* 
    *>          a_beg, end, opens.norm, opens.sharps - opens.norm, opens.oshape, opens.oflow);       <* 
    *> }                                                                                             <*/
   /*---(complete)-----------------------*/
   return 0;
}

PRIV char     /*----: clean out jogs, jitters, and tails ---------------------*/
match_prep         (void)
{
   /*---(locals)-------------------------*/
   int       i         = 0;                 /* loop iterator -- key points    */
   int       nclean    = 0;                 /* number of cleaned segments     */
   /*---(clean out little leftovers)-----*/
   for (i = 1; i < o.nkey; ++i) {
      if (strncmp(o.key[i].use, "-", 5) != 0) continue;
      if (o.key[i].l <= 5) {
         KEY_label(i, 1, "*");
         ++nclean;
      }
   }
   /*---(report out)---------------------*/
   /*> if (nclean > 0) printf("   cleansed jogs, jitters, and tails\n");           <* 
    *> else            printf("   no prep up needed on little segments\n");        <*/
   /*---(complete)-----------------------*/
   return 0;
}

PRIV char     /*----: clean out small, unused segments -----------------------*/
match_cleanup      (void)
{
   /*---(locals)-------------------------*/
   int       i         = 0;                 /* loop iterator -- key points    */
   int       nclean    = 0;                 /* number of cleaned segments     */
   int       missed    = 0;                 /* number of missed segments      */
   /*---(clean out little leftovers)-----*/
   for (i = 1; i < o.nkey; ++i) {
      if (strncmp(o.key[i].use, "-", 5) != 0) continue;
      if (o.key[i].l < SIZE_M0) {
         KEY_label(i, 1, "*");
         ++nclean;
      } else {
         ++missed;
      }
   }
   /*---(report out)---------------------*/
   /*> if (nclean > 0) printf("   cleaned up little leftover segment(s)\n");            <* 
    *> else            printf("   no clean up needed on little leftover segments\n");   <* 
    *> if (missed > 0) printf("   NOT DONE (missing some segments)\n");                 <* 
    *> else            printf("   all segments accounted for\n");                       <*/
   /*---(complete)-----------------------*/
   return 0;
}

PRIV char     /*----: report out result of matching --------------------------*/
match_result       (void)
{
   /*---(locals)-------------------------*/
   int       i         = 0;                 /* loop iterator -- key points    */
   char      bad       = 0;
   /*---(actual)-------------------------*/
   DEBUG__MATCHES  printf("   actual = \">");
   strncpy(o.actual,  ">", 50);
   strncpy(o.letters, ">", 50);
   for (i = 1; i < o.nkey; ++i) {
      strncat(o.letters, "."     , 50);
      strncat(o.letters, o.key[i].use, 50);
      if (  strncmp(o.key[i].use, "*", 5) != 0 &&
            strncmp(o.key[i].use, "+", 5) != 0) {
         DEBUG__MATCHES  printf(".%s", o.key[i].use);
         strncat(o.actual, "."       , 50);
         strncat(o.actual, o.key[i].use, 50);
      }
      if (strncmp(o.key[i].use, "-", 5) == 0) bad = 1;
   }
   DEBUG__MATCHES  printf("\"\n");
   /*---(expect)-------------------------*/
   DEBUG__MATCHES  printf("   expect = \"%s\"\n", o.expect);
   /*---(grade)--------------------------*/
   if        (bad == 1) {
      strncpy(o.grade, "BAD", 50);
   } else if (strncmp(o.expect, "",       50) == 0) {
      strncpy(o.grade, "null", 50);
   } else if (strncmp(o.actual, o.expect, 50) == 0) {
      strncpy(o.grade, "pass", 50);
   } else {
      strncpy(o.grade, "FAIL", 50);
   }
   DEBUG__MATCHES  printf("   graded as a %s", o.grade);
   /*---(complete)-----------------------*/
   return 0;
}

PRIV char     /*----: apply locations in turn --------------------------------*/
match_location     (int a_rule, int a_beg, int a_opens, int a_size)
{
   /*---(locals)-------------------------*/
   int       j         = 0;                 /* loop iterator -- letters       */
   int       k         = 0;                 /* loop iterator -- segments      */
   int       trys_ltr  = 0;                 /* number possible letter rules   */
   char      flow      = 0;                 /* current quadrant flow          */
   char      rc        = 0;                 /* simple return code             */
   char      match[5]  = "";                /* letter that matched            */
   /*---(try letters)---------------------*/
   trys_ltr = 0;
   for (j = 0; j < MAX_LETTERS; ++j) {
      /*---(primary filtering)---------------*/
      if (strncmp(loc[j].n, "eof", 5)              == 0) break;
      if (strncmp(groups[a_rule].gr, loc[j].gr, 5) != 0) continue;
      /*---(screen letters)------------------*/
      ++trys_ltr;
      if (trys_ltr <= 3) {
         DEBUG__MATCHES  printf("%1d:%-2.2s ", loc[j].sz, loc[j].n);
      }
      if (loc[j].sz != a_size                          ) continue;
      /*---(give results)-----------------*/
      rc = 1;
      strncpy(match, loc[j].n, 5);
   }
   for (j = trys_ltr; j < 3; ++j) {
      DEBUG__MATCHES  printf(".:.. ");
   }
   if (rc == 1) {
      DEBUG__MATCHES  printf("[ MATCH %-4.4s]", match);
      KEY_label(a_beg + 1, a_opens, match);
   } else {
      DEBUG__MATCHES  printf("[ no fits   ]");
   }
   /*---(finish locations)----------------*/
   /*> if (trys_ltr == 0) printf("no LOCATION entry\n");                           <* 
    *> else               printf("\n");                                            <*/
   printf("\n");
   /*---(complete)-----------------------*/
   return rc;
}

PRIV char     /*----: apply matching rules in turn ---------------------------*/
match_group        (int a_beg, int a_opens, char *a_group)
{
   /*---(locals)-------------------------*/
   int       i         = 0;                 /* loop iterator -- key points    */
   int       j         = 0;                 /* curve index                    */
   int       trys_grp  = 0;                 /* number possible group rules    */
   int       trys_ltr  = 0;                 /* number possible letter rules   */
   char      rc        = 0;                 /* simple return code             */
   char      num       = 0;                 /* current quadrant flow          */
   char      gname[5]  = "";
   /*---(run rules)--------------------------*/
   for (i = 0; groups[i].ra < 900; ++i) {
      /*---(only review active rules)--------*/
      if (groups[i].ac != 1       )   continue;    /* active 1 = yes        */
      /*---(primary filtering)---------------*/
      if (groups[i].op != a_opens )   continue;    /* number of opens       */
      if (groups[i].ra != opens.r &&               /* wrong primary range   */
            groups[i].ra != opens.r2) continue;    /*    and not secondary  */
      /*> if (groups[i].cu != opens.cc)   continue;    /+ wrong curviness       +/    <*/
      j = opens.cc + CURVE_OFFSET;
      if (strchr("- _", groups[i].ca[j]) != 0)   continue;    /* wrong curviness       */
      /*---(test specific group)-------------*/
      if (strncmp(a_group, "", 5) == 0) {
         strncpy(gname, "n/a", 5);
      } else {
         strncpy(gname, a_group, 5);
         if (strncmp(groups[i].gr, a_group, 5) != 0) continue;
      }
      /*---(start report out)----------------*/
      /*> printf("      #%02d [%-3.3s] f=%2d o=%1d, r=%1d, c=%2d : ",                 <* 
       *>       i, groups[i].gr, a_beg, groups[i].op, groups[i].ra, opens.cc);        <*/
      ++trys_grp;
      /*---(secondary filtering)-------------*/
      if (match_shape (i, a_beg) != 1)   continue;
      if (match_flow  (i, a_beg) != 1)   continue;
      /*---(try letters)---------------------*/
      rc = match_location (i, a_beg, a_opens, opens.la);
      if (rc != 0) break;
   }
   /*---(finish rules)-----------------------*/
   if (strncmp(a_group, "", 5) == 0) {
         /*> if (trys_grp == 0) printf("      no group rules found (in GROUP table)\n");     <* 
          *> else               printf("      end of group rules (%d tries)\n", trys_grp);   <*/
   } else {
         /*> if (trys_grp == 0) {                                                                                       <* 
          *>    printf("      #-- [%-3.3s] f=%2d o=%1d, r=%1d, c=%2d :", a_group, a_beg, a_opens, opens.r, opens.cc);   <* 
          *>    printf(" --- --- --- : --- --- --- --- : ---- ---- ---- [ no joy    ]\n");                              <* 
          *> }                                                                                                          <*/
   }
   /*---(complete)-----------------------*/
   return rc;
}

char          /*----: walk through rules for all normal letters --------------*/
match_driver       (void)
{
   /*---(locals)-------------------------*/
   int   beg       = 0;      /* current open point                            */
   int   count     = 0;      /* number of opens to test                       */
   char  rc        = 0;      /* simple return code                            */
   int   trys_grp  = 0;      /* number of possible group rules                */
   int   trys_ltr  = 0;      /* number of possible letter rules               */
   /*---(process)------------------------*/
   DEBUG__MATCHES  printf("\nMATCHING LETTERS (beg)--------------------\n");
   match_prep      ();
   DEBUG__MATCHES  POINT_list (o.key, o.nkey);
   for (beg = 0; beg < o.nkey - 1; ++beg) {
      /*---(get open point count)------------------*/
      match_opens(beg);
      if (opens.sharps < 1)  continue;
      /*---(work from longest to shortest)---------*/
      for (count = 7; count > 0; --count) {
         rc = 0;
         if (count > opens.sharps)               continue;
         match_calc (beg, count);
         /*> printf("\n   %02d-%02d%c =========  o=%1d, r=%1d, c=%2d : ",                           <* 
          *>       beg, beg + count, count > (opens.norm) ? '*' : ' ', count, opens.r, opens.cc);   <*/
         /*> printf("le=%4d, sz=%1d, de=%4d, mc=%5.1f  ==========================\n",   <* 
          *>       opens.l, opens.la, opens.d, opens.cm);                               <*/
         if (count >= 3 && opens.sharps >= 3)  rc = combo_driver (beg, count);
         if (rc == 1) break;
         trys_grp = 0;
         /*---(run rules)--------------------------*/
         rc = match_group (beg, count, "");
         if (rc != 0) break;
      }
   }
   DEBUG__MATCHES  printf("\n   done with letter recognition\n");
   /*---(process results)----------------*/
   match_cleanup   ();
   match_result    ();
   words_outstring (o.actual);
   /*---(complete)-----------------------*/
   DEBUG__MATCHES  printf("\n");
   DEBUG__MATCHES  POINT_list (o.key, o.nkey);
   DEBUG__MATCHES  printf("\nMATCHING LETTERS (end)--------------------\n");
      /*> printf("r=%3d, a=%3d, k=%3d, ltr=%s, act=%s, wrd=%s\n",                     <* 
       *>       o.nraw, o.navg, o.nkey, o.actual, o.letters, o.word);                 <*/
   return 0;
}



/*============================--------------------============================*/
/*===----                         combinations                         ----===*/
/*============================--------------------============================*/
static void      o___COMBOS__________________o (void) {;}

PRIV int      /*----: add a mid point ----------------------------------------*/
combo_check        (int a_beg, int a_count, char a_method, int a_offset, int a_new, char *a_1st, char *a_2nd)
{
   /*---(locals)-------------------------*/
   char      rc1       = 0;            /* return from letter one              */
   char      rc2       = 0;            /* return from letter two              */
   int       new       = 0;            /* artificial key transition           */
   int       xopens    = 0;
   /*---(match 1st segment)--------------*/
   if (a_method == '-')  xopens = a_offset;
   else                  xopens = 2;
   match_calc (a_beg, xopens);
   rc1 = match_group (a_beg, xopens, a_1st);
   /*---(match 2nd segment)--------------*/
   match_calc (a_beg + a_offset, 2);
   /*> printf("\ngshape = <<%s>>, oshape = <<%s>>\n", opens.gshape, opens.oshape);    <*/
   rc2 = match_group (a_beg + a_offset, 2, a_2nd);
   /*---(check results)------------------*/
   if (rc1 == 0 || rc2 == 0) {
      if (a_new > 0) KEY_del   (a_new);
      KEY_label (a_beg + 1, a_count, "-");
      return -1;
   }
   /*---(complete)-----------------------*/
   return 1;
}

PRIV int      /*----: add a mid point ----------------------------------------*/
combo_flaty        (int a_beg, int a_count, int a_offset)
{
   /*---(locals)-------------------------*/
   int       p1, p2, p3, p4;
   int       y1, y2;
   float     yt;
   int       i;
   float     dist;
   float     min    = 1000;
   int       mid;
   int       new;
   /*---(initialize points)--------------*/
   p1        = o.key[a_beg    ].p_bas;
   p2        = o.key[a_beg + 1].p_bas;
   p3        = o.key[a_beg + 2].p_bas;
   p4        = o.key[a_beg + 3].p_bas;
   /*---(get target y)-------------------*/
   if (a_offset == 0) {
      y1        = o.avg[p1].ypos;
      y2        = o.avg[p4].ypos;
   } else {
      y1        = o.avg[p2].ypos;
      y2        = o.avg[p3].ypos;
   }
   yt        = y1;
   DEBUG__MATCHES  printf("y1=%4d, y2=%4d, yt=%5.1f, ", y1, y2, yt);
   /*---(find midpoint)------------------*/
   for (i = p2 + 1; i < p3; ++i) {
      dist = fabs(o.avg[i].ypos - yt);
      /*> DEBUG__MATCHES  printf("%2d at %3d is %6.2f\n", i, o.avg[i].ypos, dist);       <*/
      if (dist <= min) {
         min = dist;
         mid = i;
      }
   }
   if (min > 2.0) {
      DEBUG__MATCHES  printf("d=%5.1f, reject\n", min);
      return -1;
   }
   /*---(add midpoint)-------------------*/
   new = KEY_add (mid, 'y', '-');
   DEBUG__MATCHES  printf("m=%3d, d=%5.1f, k=%2d\n", mid, min, new);
   /*---(complete)-----------------------*/
   return new;
}

PRIV int      /*----: add a mid point ----------------------------------------*/
combo_midy         (int a_beg, int a_count, int a_offset)
{
   /*---(locals)-------------------------*/
   int       p1, p2, p3, p4;
   int       y1, y2;
   float     yt;
   int       i;
   float     dist;
   float     min    = 1000;
   int       mid;
   int       new;
   /*---(initialize points)--------------*/
   p1        = o.key[a_beg    ].p_bas;
   p2        = o.key[a_beg + 1].p_bas;
   p3        = o.key[a_beg + 2].p_bas;
   p4        = o.key[a_beg + 3].p_bas;
   /*---(get target y)-------------------*/
   if (a_offset == 0) {
      y1        = o.avg[p1].ypos;
      y2        = o.avg[p4].ypos;
   } else {
      y1        = o.avg[p2].ypos;
      y2        = o.avg[p3].ypos;
   }
   yt        = y1 + ((y2 - y1) / 2.0);
   DEBUG__MATCHES  printf("y1=%4d, y2=%4d, yt=%5.1f, ", y1, y2, yt);
   /*---(find midpoint)------------------*/
   for (i = p2 + 1; i < p3; ++i) {
      dist = fabs(o.avg[i].ypos - yt);
      /*> DEBUG__MATCHES  printf("%2d at %3d is %6.2f\n", i, o.avg[i].ypos, dist);       <*/
      if (dist <= min) {
         min = dist;
         mid = i;
      }
   }
   /*---(add midpoint)-------------------*/
   new = KEY_add (mid, 'y', '-');
   DEBUG__MATCHES  printf("m=%3d, d=%5.1f, k=%2d\n", mid, min, new);
   /*---(complete)-----------------------*/
   return new;
}

PRIV int      /*----: add a mid point ----------------------------------------*/
combo_flatx        (int a_beg, int a_count, int a_offset)
{
   /*---(locals)-------------------------*/
   int       p1, p2, p3, p4;
   int       x1, x2;
   float     xt;
   int       i;
   float     dist;
   float     min    = 1000;
   int       mid;
   int       new;
   /*---(initialize points)--------------*/
   p1        = o.key[a_beg    ].p_bas;
   p2        = o.key[a_beg + 1].p_bas;
   p3        = o.key[a_beg + 2].p_bas;
   p4        = o.key[a_beg + 3].p_bas;
   /*---(get target y)-------------------*/
   if (a_offset == 0) {
      x1        = o.avg[p1].xpos;
      x2        = o.avg[p4].xpos;
   } else {
      x1        = o.avg[p2].xpos;
      x2        = o.avg[p3].xpos;
   }
   xt        = x1;
   DEBUG__MATCHES  printf("x1=%4d, x2=%4d, xt=%5.1f, ", x1, x2, xt);
   /*---(find midpoint)------------------*/
   for (i = p2 + 1; i < p3; ++i) {
      dist = fabs(o.avg[i].xpos - xt);
      /*> DEBUG__MATCHES  printf("%2d at %3d is %6.2f\n", i, o.avg[i].xpos, dist);       <*/
      if (dist <= min) {
         min = dist;
         mid = i;
      }
   }
   /*---(add midpoint)-------------------*/
   new = KEY_add (mid, 'y', '-');
   DEBUG__MATCHES  printf("m=%3d, d=%5.1f, k=%2d\n", mid, min, new);
   /*---(complete)-----------------------*/
   return new;
}

PRIV int      /*----: add a mid point ----------------------------------------*/
combo_midx         (int a_beg, int a_count, int a_offset)
{
   /*---(locals)-------------------------*/
   int       p1, p2, p3, p4;
   int       x1, x2;
   float     xt;
   int       i;
   float     dist;
   float     min    = 1000;
   int       mid;
   int       new;
   /*---(initialize points)--------------*/
   p1        = o.key[a_beg    ].p_bas;
   p2        = o.key[a_beg + 1].p_bas;
   p3        = o.key[a_beg + 2].p_bas;
   p4        = o.key[a_beg + 3].p_bas;
   /*---(get target y)-------------------*/
   if (a_offset == 0) {
      x1        = o.avg[p1].xpos;
      x2        = o.avg[p4].xpos;
   } else {
      x1        = o.avg[p2].xpos;
      x2        = o.avg[p3].xpos;
   }
   xt        = x1 + ((x2 - x1) / 2.0);
   DEBUG__MATCHES  printf("x1=%4d, x2=%4d, xt=%5.1f, ", x1, x2, xt);
   /*---(find midpoint)------------------*/
   for (i = p2 + 1; i < p3; ++i) {
      dist = fabs(o.avg[i].xpos - xt);
      /*> DEBUG__MATCHES  printf("%2d at %3d is %6.2f\n", i, o.avg[i].xpos, dist);       <*/
      if (dist <= min) {
         min = dist;
         mid = i;
      }
   }
   /*---(add midpoint)-------------------*/
   new = KEY_add (mid, 'y', '-');
   DEBUG__MATCHES  printf("m=%3d, d=%5.1f, k=%2d\n", mid, min, new);
   /*---(complete)-----------------------*/
   return new;
}

PRIV char     /*----: attempt to match a GL,GR,GO,CL,CR,CO,UL,UR,UO ----------*/
combo_flow         (int a_beg, int a_count, char *a_flow)
{
   /*---(locals)-------------------------*/
   int       len       = 0;            /* length of flow string               */
   int       i         = 0;            /* loop iterator -- key point          */
   int       rc        = 0;
   int       quad      = 0;
   /*---(check count)--------------------*/
   len = strlen (a_flow);
   if        (len < a_count) {
      DEBUG__MATCHES  printf("flow too short (%d != %d)\n", a_count, len);
      return -1;
   } else {
      DEBUG__MATCHES  printf("n=%d, ", a_count);
   }
   /*---(check flow)---------------------*/
   for (i = 1; i <= a_count; ++i) {
      quad = a_flow[i - 1] - '0';
      if (o.key[a_beg + i].q == quad) {
         DEBUG__MATCHES  printf("q%1d:%1d=%1d ", i, o.key[a_beg + i].q, quad);
      } else {
         DEBUG__MATCHES  printf("q%1d:%1dx%1d ", i, o.key[a_beg + i].q, quad);
         rc = -2;
      }
   }
   if (rc == -2) {
      DEBUG__MATCHES  printf(", flow is wrong\n");
   }
   /*---(complete)-----------------------*/
   return rc;
}

PRIV char     /*----: attempt to parse points and match combinations ---------*/
combo_driver       (int a_beg, int a_count)
{
   /*---(locals)-------------------------*/
   char      rc        = 0;            /* return code                         */
   char      flows[10] = "";           /* combination flow                    */
   char      quad      = 0;            /* first quad                          */
   int       i         = 0;
   int       new       = 0;            /* artificial key transition           */
   /*---(begin)--------------------------*/
   if (a_count < 3) {
      DEBUG__MATCHES  printf("      no combos, too few points availible (%d < 3)\n", opens.norm);
      return -1;
   }
   /*---(GL)-----------------------------*/
   quad = o.key[a_beg + 1].q + '0';
   /*> DEBUG__MATCHES  printf("      start combos with %2d and q=%d...\n", opens.norm, quad);   <*/
   for (i = 0; i < MAX_COMBOS; ++i) {
      /*> DEBUG__MATCHES  printf("      looking at %s, ac=%1d, op=%1d, q1=%c\n", combos[i].nm, combos[i].ac, combos[i].op, combos[i].fl[0]);   <*/
      /*---(screen)----------------------*/
      if (strcmp(combos[i].nm, "eof") == 0)     break;
      if (combos[i].ac    == 0)                 continue;
      if (combos[i].op    != a_count)           continue;
      if (combos[i].fl[0] != quad)              continue;
      /*---(check flow)------------------*/
      DEBUG__MATCHES  printf("      combo %3s) ", combos[i].nm);
      rc = combo_flow (a_beg, a_count, combos[i].fl);
      if (rc != 0) continue;
      /*---(assign midpoint)-------------*/
      switch (combos[i].mm[0]) {
      case '-' :      /* "--"  */
         new  = 0;
         DEBUG__MATCHES  printf(", midpoint exists\n");
         break;
      case 'o' :      /* "oo"  */
         new  = 0;
         DEBUG__MATCHES  printf(", letters overlap, midpoint unneccessary\n");
         break;
      case 'f' :
         switch (combos[i].mm[1]) {
         case 'y' :   /* "fy"  */
            new  = combo_flaty (a_beg, a_count, combos[i].st);
            break;
         case 'x' :   /* "fx"  */
            new  = combo_flatx (a_beg, a_count, combos[i].st);
            break;
         default  :
            continue;
            break;
         }
         break;
      case 'a' :
         switch (combos[i].mm[1]) {
         case 'y' :   /* "ax"  */
            new  = combo_midy (a_beg, a_count, combos[i].st);
            break;
         case 'x' :   /* "ax"  */
            new  = combo_midx (a_beg, a_count, combos[i].st);
            break;
         default  :
            continue;
            break;
         }
         break;
      default  :
         continue;
         break;
      }
      /*> printf ("k=%d\n", new);                                                     <*/
      if (new < 0) continue;
      /*---(evaluate)--------------------*/
      rc   = combo_check (a_beg, a_count, combos[i].mm[0], combos[i].of, new, combos[i].on, combos[i].tw);
      /*---(complete)-----------------------*/
      if (rc == 1) return rc;
      match_calc (a_beg, a_count);
   }
   /*---(complete)-----------------------*/
   return 0;
}





/*============================--------------------============================*/
/*===----                       identify circles                       ----===*/
/*============================--------------------============================*/
static void      o___CIRCLES_________________o (void) {;}
/*
 * circles can have 2 to 4 internal points depending on the circumstances
 * circles have two sizes      -- large (a) and small (e)
 * circles have two directions -- clockwise and counter-clockwise
 * circles may not close at the beginning and ending of words
 *
 */
int       min1      = 0;               /* circle entrance point               */
int       min2      = 0;               /* circle exit point                   */
int       one       = 0;               /* new circle intersection point       */
int       two       = 0;               /* new circle intersection point       */
char      use[5]    = "";              /* circle letter use                   */


PRIV char     /*----: determine if two lines intersect -----------------------*/
circle_intersect    (int a_p1, int a_p3)
{
   /*---(locals)-------------------------*/
   int       p1  = a_p1;               /* 1st point 1st segment               */
   int       p2  = a_p1 + 1;           /* 2nd point 1st segment               */
   int       p3  = a_p3;               /* 1st point 2nd segment               */
   int       p4  = a_p3 + 1;           /* 2nd point 2nd segment               */
   double    A1, B1, C1;               /* 1st segment calcs                   */
   double    A2, B2, C2;               /* 2nd segment calcs                   */
   double    det;                      /* determinant                         */
   int       x, y;                     /* hypothetical intersection           */
   int       _max    = 0;              /* axis minimum point                  */
   int       _min    = 0;              /* axis maximum point                  */
   /*---(solve segment one)--------------*/
   A1 = o.key[p2].ypos - o.key[p1].ypos;
   B1 = o.key[p1].xpos - o.key[p2].xpos;
   C1 = (A1 * o.key[p1].xpos) + (B1 * o.key[p1].ypos);
   DEBUG__CIRCLES  printf("   1st seg :: %4d,%4d to %4d,%4d :: s=%8.3f, b=%4d :: A=%4.0f, B=%4.0f, C=%4.0f\n", o.key[p1].xpos, o.key[p1].ypos, o.key[p2].xpos, o.key[p2].ypos, o.key[p2].s, o.key[p2].b, A1, B1, C1);
   /*---(solve segment two)--------------*/
   A2 = o.key[p4].ypos - o.key[p3].ypos;
   B2 = o.key[p3].xpos - o.key[p4].xpos;
   C2 = (A2 * o.key[p3].xpos) + (B2 * o.key[p3].ypos);
   DEBUG__CIRCLES  printf("   2nd seg :: %4d,%4d to %4d,%4d :: s=%8.3f, b=%4d :: A=%4.0f, B=%4.0f, C=%4.0f\n", o.key[p3].xpos, o.key[p3].ypos, o.key[p4].xpos, o.key[p4].ypos, o.key[p4].s, o.key[p4].b, A2, B2, C2);
   /*---(hypothetical intersection)------*/
   det = (A1 * B2) - (A2 * B1);
   if (det == 0) {
      DEBUG__CIRCLES  printf("   PARALLEL, failing out\n");
      return -1;
   }
   x  = (int) ((B2 * C1) - (B1 * C2)) / det;
   y  = (int) ((A1 * C2) - (A2 * C1)) / det;
   DEBUG__CIRCLES  printf("   hypothetical intersection at %4d, %4d\n", x, y);
   /*---(check 1st segment)--------------*/
   _min   = (int) fmin(o.key[p1].xpos, o.key[p2].xpos);
   _max   = (int) fmax(o.key[p1].xpos, o.key[p2].xpos);
   DEBUG__CIRCLES  printf("   test x on 1st :: %4d < %4d < %4d : ", _min, x, _max);
   if (_min > x || _max < x)  {
      DEBUG__CIRCLES  printf("FAILED, failing out\n");
      return -1;
   }
   DEBUG__CIRCLES  printf("accepted\n");
   _min   = (int) fmin(o.key[p1].ypos, o.key[p2].ypos);
   _max   = (int) fmax(o.key[p1].ypos, o.key[p2].ypos);
   DEBUG__CIRCLES  printf("   test y on 1st :: %4d < %4d < %4d : ", _min, y, _max);
   if (_min > y || _max < y) {
      DEBUG__CIRCLES  printf("FAILED, failing out\n");
      return -1;
   }
   DEBUG__CIRCLES  printf("accepted\n");
   /*---(check 2nd segment)--------------*/
   _min   = (int) fmin(o.key[p3].xpos, o.key[p4].xpos);
   _max   = (int) fmax(o.key[p3].xpos, o.key[p4].xpos);
   DEBUG__CIRCLES  printf("   test x on 2nd :: %4d < %4d < %4d : ", _min, x, _max);
   if (_min > x || _max < x) {
      DEBUG__CIRCLES  printf("FAILED, failing out\n");
      return -1;
   }
   DEBUG__CIRCLES  printf("accepted\n");
   _min   = (int) fmin(o.key[p3].ypos, o.key[p4].ypos);
   _max   = (int) fmax(o.key[p3].ypos, o.key[p4].ypos);
   DEBUG__CIRCLES  printf("   test y on 2nd :: %4d < %4d < %4d : ", _min, y, _max);
   if (_min > y || _max < y) {
      DEBUG__CIRCLES  printf("FAILED, failing out\n");
      return -1;
   }
   DEBUG__CIRCLES  printf("accepted\n");
   /*---(complete)-----------------------*/
   DEBUG__CIRCLES  printf("   success\n");
   return 0;
}

PRIV int      /*----: determine the circle crossover point)-------------------*/
circle_minpoint     (int a_p1, int a_p3)
{
   DEBUG__CIRCLES  printf("   looking for minimum connection distance...\n");
   /*---(locals)-------------------------*/
   int       p1  = a_p1;               /* 1st point 1st segment               */
   int       p2  = a_p1 + 1;           /* 2nd point 1st segment               */
   int       p3  = a_p3;               /* 1st point 2nd segment               */
   int       p4  = a_p3 + 1;           /* 2nd point 2nd segment               */
   int i = 0;
   int j = 0;
   int  my_x = 0;
   int  my_y = 0;
   int  ci_x = 0;
   int  ci_y = 0;
   float my_dist = 0.0;
   float my_min  = 1000.0;
   for (i = o.key[p1].p_bas; i <= o.key[p2].p_bas; ++i) {
      if (i == 0)  {
         /*> DEBUG__CIRCLES  printf("   %03d (fake/beg) can not be used in connection distance calcs\n", i);   <*/
         continue;
      }
      /*> DEBUG__CIRCLES  printf("   %03d\n", i);                                     <*/
      my_x = o.bas[i].xpos;
      my_y = o.bas[i].ypos;
      for (j = o.key[p3].p_bas; j <= o.key[p4].p_bas; ++j) {
         if (j == o.nbas - 1) {
            /*> DEBUG__CIRCLES  printf("   %03d (fake/end) can not be used in connection distance calcs\n", i);   <*/
            continue;
         }
         ci_x = abs(o.bas[j].xpos - my_x);
         ci_y = abs(o.bas[j].ypos - my_y);
         my_dist = sqrt((ci_x * ci_x) + (ci_y * ci_y));
         if (my_dist < my_min) {
            my_min = my_dist;
            min1    = i;
            min2    = j;
         }
         /*> DEBUG__CIRCLES  printf("       vs %03d at %7.2f so min is %7.2f\n", j, my_dist, my_min);   <*/
      }
   }
   DEBUG__CIRCLES  printf("   min found at i=%3d, j=%3d, dist=%7.2f\n", min1, min2, my_min);
   if (my_min > 12.0) {
      DEBUG__CIRCLES  printf("   REJECTED, greater than limit of 12.0\n");
      min1 = -1;
      min2 = -1;
      return -1;
   }
   DEBUG__CIRCLES  printf("   success\n");
   /*---(complete)-----------------------*/
   return 0;
}

PRIV int      /*----: size of circle at cross-section ------------------------*/
circle_size        (int a_one, int a_two)
{
   /*---(locals)-------------------------*/
   int       space     = 0;            /* how far from intersetion to measure */
   int       half      = 0;            /* halfway point                       */
   int       mid1      = 0;            /* 1st midpoint                        */
   int       mid2      = 0;            /* 2nd midpoint                        */
   int       _distx    = 0;            /* x-axis distance between midpoints   */
   int       _disty    = 0;            /* y-axis distance between midpoints   */
   int       _dist     = 0;            /* full distance average               */
   int       _dist1    = 0;            /* full distance between midpoints     */
   int       _dist2    = 0;            /* full distance between half points   */
   DEBUG__CIRCLES  printf("   search for midpoint size...\n");
   /*---(figure points)------------------*/
   half     = a_one + ((a_two - a_one) / 2);
   space    = (a_two - a_one) / 4;
   mid1     = half - space;
   mid2     = half + space;
   DEBUG__CIRCLES  printf("   _min1 =%3d, _min2 =%3d, space=%3d, half =%3d, mid1 =%3d, mid2 =%3d\n", a_one, a_two, space, half, mid1, mid2);
   /*---(calc midsection)----------------*/
   _distx   = abs(o.bas[mid2].xpos - o.bas[mid1].xpos);
   _disty   = abs(o.bas[mid2].ypos - o.bas[mid1].ypos);
   _dist1   = (int) sqrt((_distx * _distx) + (_disty * _disty));
   DEBUG__CIRCLES  printf("   midpoints  : _distx=%3d, _disty=%3d, _dist1=%3d\n", _distx, _disty, _dist1);
   o.avg[mid1].t = 'm';
   o.avg[mid2].t = 'm';
   /*---(get half mark)------------------*/
   _distx   = abs(o.bas[half].xpos - o.bas[a_one].xpos);
   _disty   = abs(o.bas[half].ypos - o.bas[a_one].ypos);
   _dist2   = (int) sqrt((_distx * _distx) + (_disty * _disty));
   DEBUG__CIRCLES  printf("   halfpoints : _distx=%3d, _disty=%3d, _dist2=%3d\n", _distx, _disty, _dist2);
   o.avg[half].t = 'm';
   /*---(averagge)-----------------------*/
   _dist = (_dist1 + _dist2) / 2.0;
   DEBUG__CIRCLES  printf("   so, _dist =%3d\n", _dist);
   /*---(complete)-----------------------*/
   return _dist;
}

PRIV char     /*----: push intersection backward -----------------------------*/
circle_backward    (void)
{
   int       i, j;                     /* loop iterators -- basic points      */
   float     xd, yd;
   float     dist;
   float     min       = 1000;
   int       pt        = 0;
   int       new       = 0;
   int       old_nkey  = o.nkey;
   DEBUG__CIRCLES  printf("\n");
   DEBUG__CIRCLES  printf("   extend circle intersection backwards from start...\n");
   DEBUG__CIRCLES  printf("      circle beg is    key = %3d, bas = %3d\n", one, o.key[one].p_bas);
   DEBUG__CIRCLES  printf("      circle end is    key = %3d, bas = %3d\n", two, o.key[two].p_bas);
   DEBUG__CIRCLES  printf("      circle midpoint  key = %3d, bas = %3d\n", one + 2, o.key[one + 2].p_bas);
   DEBUG__CIRCLES  printf("      total circle point are key = %3d, bas = %3d\n", o.nkey, o.nbas);
   DEBUG__CIRCLES  printf("      so, checking forward from bas   0 to %3d...\n", o.key[one].p_bas);
   DEBUG__CIRCLES  printf("      against, forward  from    bas %3d to %3d...\n", o.key[one + 2].p_bas, o.key[two].p_bas);
   if (one == 0) return 0;
   for (i = o.key[one - 1].p_bas; i < o.key[one].p_bas; ++i) {
      for (j = o.key[one + 2].p_bas; j <= o.key[two].p_bas; ++j) {
         xd   = abs(o.bas[j].xpos - o.bas[i].xpos);
         yd   = abs(o.bas[j].ypos - o.bas[i].ypos);
         dist = (int) sqrt((xd * xd) + (yd * yd));
         /*> DEBUG__CIRCLES  printf("      checking i=%3d, j=%3d, dist=%6.2f\n", i, j, dist);   <*/
         if (dist <= 3.0) {
            min = dist;
            pt  = i;
            break;
         }
      }
      if (pt  != 0) break;
   }
   /*---(mark)---------------------------*/
   if (pt  != 0 && pt  != o.key[one].p_bas) {
      new = KEY_add   (pt , 'y', '-');
      if (old_nkey != o.nkey) {
         DEBUG__CIRCLES  printf("      ADDED KEY\n");
         one = new;
         ++two;
         KEY_label (one + 1, two - one, use);
      } else {
         DEBUG__CIRCLES  printf("      existing key\n");
      }
      DEBUG__CIRCLES  printf("      min circle is bas=%3d, key=%3d (n=%2d), dist=%6.2f, use=%-5.5s\n", pt, new, o.nkey, min, use);
   } else {
      DEBUG__CIRCLES  printf("      min circle already exists\n");
   }
   /*---(complete)-----------------------*/
   return 0;
}

PRIV char     /*----: push intersection forward ------------------------------*/
circle_forward     (void)
{
   int       i, j;                     /* loop iterators -- basic points      */
   float     xd, yd;
   float     dist;
   float     min       = 1000;
   int       pt        = 0;
   int       new       = 0;
   int       old_nkey  = o.nkey;
   DEBUG__CIRCLES  printf("\n");
   DEBUG__CIRCLES  printf("   extend circle intersection forwards from end...\n");
   DEBUG__CIRCLES  printf("      circle beg is    key = %3d, bas = %3d\n", one, o.key[one].p_bas);
   DEBUG__CIRCLES  printf("      circle end is    key = %3d, bas = %3d\n", two, o.key[two].p_bas);
   DEBUG__CIRCLES  printf("      circle midpoint  key = %3d, bas = %3d\n", two - 2, o.key[two - 2].p_bas);
   DEBUG__CIRCLES  printf("      total circle point are key = %3d, bas = %3d\n", o.nkey, o.nbas);
   DEBUG__CIRCLES  printf("      so, checking backward from bas %3d to %3d...\n", o.nkey - 1, o.key[two].p_bas);
   DEBUG__CIRCLES  printf("      against, forward from      bas %3d to %3d...\n", o.key[two - 1].p_bas, o.key[one].p_bas);
   if (two == o.nkey - 1) return 0;
   for (j = o.key[two + 1].p_bas; j > o.key[two].p_bas; --j) {
      for (i = o.key[two - 2].p_bas; i > o.key[one].p_bas; --i) {
         xd   = abs(o.bas[j].xpos - o.bas[i].xpos);
         yd   = abs(o.bas[j].ypos - o.bas[i].ypos);
         dist = (int) sqrt((xd * xd) + (yd * yd));
         /*> DEBUG__CIRCLES  printf("      checking i=%3d, j=%3d, dist=%6.2f\n", i, j, dist);   <*/
         if (dist <= 3.0) {
            min = dist;
            pt  = j;
            break;
         }
      }
      if (pt  != 0) break;
   }
   /*---(mark)---------------------------*/
   if (pt  != 0 && pt  != o.key[two].p_bas) {
      new = KEY_add   (pt , 'y', '-');
      if (old_nkey != o.nkey) {
         DEBUG__CIRCLES  printf("      ADDED KEY\n");
         KEY_label (new, 1, "+");
         two = new;
      } else {
         DEBUG__CIRCLES  printf("      existing key\n");
      }
      DEBUG__CIRCLES  printf("      min circle is bas=%3d, key=%3d (n=%2d), dist=%6.2f, use=%-5.5s\n", pt, new, o.nkey, min, "+");
   } else {
      DEBUG__CIRCLES  printf("      min circle already exists\n");
   }
   /*---(complete)-----------------------*/
   return 0;
}

PRIV char
circle_update      (int a_one, int a_two, int a_size)
{
   /*---(locals)-------------------------*/
   int       d1, d2, dd;               /* degrees for figuring direction      */
   int       i         = 0;            /* loop iterator -- key points         */
   /*---(determine use)------------------*/
   strncpy(use, "-", 5);
   if (a_size   <=  8     )  {
      DEBUG__CIRCLES  printf("   REJECTED, too small\n");
      return -1;
   }
   if (a_size   >= 20     )  strncpy(use, "a", 5);
   else                      strncpy(use, "e", 5);
   DEBUG__CIRCLES  printf("   calling it an <<%s>>\n", use);
   /*---(create new keys)----------------*/
   one = KEY_add (a_one, 'y', '-');
   two = KEY_add (a_two, 'y', '-');
   DEBUG__CIRCLES  printf("   added one=%4d and two=%4d\n", one, two);
   /*---(get direction)------------------*/
   /*> d1 = o.key[one + 2].d;                                                         <* 
    *> d2 = o.key[one + 3].d;                                                         <*/
   d1 = o.key[one + 1].d;
   d2 = o.key[one + 2].d;
   dd = d1 - d2;
   DEBUG__CIRCLES  printf("   direction   :: d1=%3d, d2=%3d, dd=%3d", d1, d2, dd);
   if (dd >  180) dd -=  360;
   if (dd < -180) dd +=  360;
   DEBUG__CIRCLES  printf(", rdd=%3d", dd);
   if (dd >= 0) {
      DEBUG__CIRCLES  printf(", so clockwise\n");
   } else {
      DEBUG__CIRCLES  printf(", so counter-clockwise\n");
      use[0] = toupper(use[0]);
   }
   /*---(label)--------------------------*/
   for (i = one + 1; i <= two; ++i) {
      strncpy(o.key[i].use, "+", 5);
   }
   strncpy(o.key[one + 1].use, use, 5);
   /*---(complete)-----------------------*/
   return 0;
}

PRIV int
circle_identify    (int a_p1, int a_p3)
{
   /*---(locals)-------------------------*/
   int       p1        = a_p1;         /* 1st point 1st segment               */
   int       p2        = a_p1 + 1;     /* 2nd point 1st segment               */
   int       p3        = a_p3;         /* 1st point 2nd segment               */
   int       p4        = a_p3 + 1;     /* 2nd point 2nd segment               */
   char      rc        = 0;            /* generic return code                 */
   int       csize     = 0;            /* circle size                         */
   int       i;
   /*---(defense)------------------------*/
   if (p4 >= o.nkey)  {
      DEBUG__CIRCLES  printf("   REJECTED, final point outside valid range\n");
      return -1;
   }
   for (i = p2; i <= p4; ++i) {
      if (o.key[i].t == 'S') {
         DEBUG__CIRCLES  printf("   REJECTED, crosses a stroke start at %2d\n", i);
         return -1;
      }
      if (strncmp(o.key[i].use, "-", 5) != 0) {
         DEBUG__CIRCLES  printf("   REJECTED, point %2d taken by a <<%s>>\n", i, o.key[i].use);
         return -1;
      }
   }
   DEBUG__CIRCLES  printf("   all points in range and valid\n");
   /*---(intersection)-------------------*/
   rc       = circle_intersect (a_p1, a_p3);
   if (rc != 0) return -1;
   /*---(minimum connection)-------------*/
   rc       = circle_minpoint  (a_p1, a_p3);
   if (rc != 0) return -1;
   /*---(identify)-----------------------*/
   csize  = circle_size      (min1, min2);
   /*---(mark)---------------------------*/
   rc     = circle_update               (min1, min2, csize);
   if (rc != 0) return -1;
   circle_backward             ();
   circle_forward              ();
   /*---(complete)-----------------------*/
   DEBUG__CIRCLES  POINT_list (o.key, o.nkey);
   DEBUG__CIRCLES  printf("\n\n");
   return 1;
}

char
circle_driver      (void)
{
   DEBUG__CIRCLES  printf("CIRCLE DETECTION (begin)\n");
   /*---(locals)-------------------------*/
   int       _found  = 1;
   int       i, j;
   int       pt;
   /*---(add end points)-----------------*/
   for (i = 0; i < o.navg; ++i) {
      if (o.avg[i].t == 'S') KEY_add (i, 'y', 'S');
      if (o.avg[i].t == 'F') KEY_add (i, 'y', 'F');
   }
   KEY_calc  ('c');
   /*---(report out)---------------------*/
   DEBUG__CIRCLES  POINT_list (o.key, o.nkey);
   while (_found == 1) {
      _found = 0;
      for (i = 0; i < o.nkey - 3; ++i) {
         if (strcmp(o.key[i + 1].use, "-") != 0) continue;
         for (j = i + 2; j < i + 6; ++j) {
            if (strcmp(o.key[j + 1].use, "-") != 0) break;
            DEBUG__CIRCLES  printf("\n   TEST %d/%d to %d/%d\n", i, i + 1, j, j + 1);
            _found = circle_identify (i, j);
            if (_found == 1) break;
         }
         if (_found == 1) break;
      }
   }
   /*---(put points back)----------------*/
   for (i = 0; i < o.nkey; ++i) {
      if (o.key[i].t == 'S' || o.key[i].t == 'F') {
         KEY_del (i);
         --i;
      }
   }
   /*---(prepare for matching)-----------*/
   KEY_prep  ();
   KEY_calc  ('n');
   /*---(report out)---------------------*/
   DEBUG__CIRCLES  POINT_list (o.key, o.nkey);
   /*---(complete)-----------------------*/
   DEBUG__CIRCLES  printf("CIRCLE DETECTION (end)\n\n");
   return 0;
}


/*============================----(source-end)----============================*/
