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
   int       len;                      /* length                              */
   int       la;                       /* length category                     */
   int       degs;                     /* degree                              */
   float     slope;                    /* slope                               */
   int       icept;
   int       quad;
   int       qs;
   float     cdepth;
   float     cm;
   char      cano;
   int       ccat;
   int       rads;
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
   int yy1        = o.avg[p1].y_full;
   int yy2        = o.avg[p2].y_full;
   int yy3        = o.avg[p3].y_full;
   int yy4        = o.avg[p4].y_full;
   int xx1        = o.avg[p1].x_full;
   int xx2        = o.avg[p2].x_full;
   int xx3        = o.avg[p3].x_full;
   int xx4        = o.avg[p4].x_full;
   /*---(get overall calcs)---------------------*/
   o.tmp[0].p_bas   = a_start;
   o.tmp[0].x_full   = xx1;
   o.tmp[0].y_full   = yy1;
   o.tmp[1].p_bas   = a_start + a_count;
   o.tmp[1].x_full   = xx4;
   o.tmp[1].y_full   = yy4;
   POINT_calc (POINTS_TMP, o.tmp + 1, 'n');
   /*---(more variables)------------------------*/
   opens.xd   = o.tmp[1].xd;
   opens.yd   = o.tmp[1].yd;
   opens.len  = o.tmp[1].len;
   opens.slope    = o.tmp[1].slope;
   opens.icept    = o.tmp[1].icept;
   opens.degs    = o.tmp[1].degs;
   opens.quad    = o.tmp[1].quad;
   opens.qs   = o.key[a_start + 1].quad;
   opens.la   = match_size (opens.len);
   opens.rads = match_range (opens.degs);
   opens.cdepth    = 0;
   opens.ccat = 0;
   opens.cano = '-';
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
   DEBUG_CURVE  printf("RUNNING CURVE CALCS\n");
   DEBUG_CURVE  printf("key 1 = %2d at bas = %2d x=%4d, y=%4d\n", a_start, p1, xx1, yy1);
   DEBUG_CURVE  printf("key 2 = %2d at bas = %2d x=%4d, y=%4d\n", a_start + a_count, p4, xx4, yy4);
   DEBUG_CURVE  printf("so  slope = %8.3f and intercept = %4d\n", opens.slope, opens.icept);
   DEBUG_CURVE  printf("   pt# -xx- -yy- --lx-- --ly-- --dx-- --dy-- theta degs curve\n");
   for (i = p1 + 1; i < p4; ++i) {
      /*---(start with calc'd point)------------*/
      if (opens.slope == 0.0) opens.slope = 0.001;
      liney   = (opens.slope * o.bas[i].x_full) + opens.icept;
      linex   = (o.bas[i].y_full - opens.icept) / opens.slope;
      if (linex >  999) linex =  999;
      if (linex < -999) linex = -999;
      if (liney >  999) liney =  999;
      if (liney < -999) liney = -999;
      /*---(calc differences)-------------------*/
      diffx   = (o.bas[i].x_full    - linex);
      diffy   = (o.bas[i].y_full    - liney);
      if (diffx >  999) diffx =  999;
      if (diffx < -999) diffx = -999;
      if (diffy >  999) diffy =  999;
      if (diffy < -999) diffy = -999;
      /*---(get the angles)---------------------*/
      theta   = atan(diffy/diffx);
      thetad  = theta * RAD2DEG;
      opens.cdepth = fabs(diffx * sin(theta));
      /*---(set the curve)----------------------*/
      switch (opens.rads) {
      case 1:  if (diffx > 0.0) opens.cdepth *= -1;  break;
      case 2:  if (diffy < 0.0) opens.cdepth *= -1;  break;
      case 3:  if (diffx < 0.0) opens.cdepth *= -1;  break;
      case 4:  if (diffx < 0.0) opens.cdepth *= -1;  break;
      case 5:  if (diffx < 0.0) opens.cdepth *= -1;  break;
      case 6:  if (diffy < 0.0) opens.cdepth *= -1;  break;
      case 7:  if (diffx < 0.0) opens.cdepth *= -1;  break;
      case 8:  if (diffx < 0.0) opens.cdepth *= -1;  break;
      }
      /*> printf("   %3d %4d %4d %6.1f %6.1f %6.1f %6.1f %5.2f %4d %5.1f\n",                      <* 
       *>       i, o.bas[i].x_full, o.bas[i].y_full, linex, liney, diffx, diffy, theta, thetad, opens.cdepth);   <*/
      if (opens.cdepth > lcurve) lcurve = opens.cdepth;
      if (opens.cdepth < rcurve) rcurve = opens.cdepth;
      if (fabs(opens.cdepth) > fabs(opens.cm)) opens.cm = opens.cdepth;
   }
   DEBUG_CURVE  printf("   pt# -xx- -yy- --lx-- --ly-- --dx-- --dy-- theta degs curve\n");
   opens.cdepth  = opens.cm;
   opens.ccat = -9;
   /*---(positive curves)----------------*/
   /*   O = line
    *   1 = bendy line
    *   2 = curve
    *   3 = arc
    *   4 = balloon
    */
   if      (opens.cdepth >=  25.0)                                   opens.ccat = +4;
   else if (opens.cdepth >=  16.0)                                   opens.ccat = +3;
   else if (opens.cdepth >=   7.0)                                   opens.ccat = +2;
   else if (opens.cdepth >=  (opens.len * 0.15) || opens.cdepth >=  4.0)    opens.ccat = +1;
   /*---(negative curves)----------------*/
   else if (opens.cdepth <= -25.0)                                   opens.ccat = -4;
   else if (opens.cdepth <= -16.0)                                   opens.ccat = -3;
   else if (opens.cdepth <=  -7.0)                                   opens.ccat = -2;
   else if (opens.cdepth <= -(opens.len * 0.15) || opens.cdepth <= -4.0)    opens.ccat = -1;
   else                                                         opens.ccat =  0;
   o.tmp[1].cdepth   = opens.cdepth;
   o.tmp[1].ccat  = opens.ccat;
   /*> printf("between key points %d/%d (bas %02d/%02d) lcurve=%5.1f, rcurve=%5.1f, maxcurve=%5.1f, curve=%5.1f, curvec=%2d\n",   <* 
    *>       a_start, a_start + a_count, p1, p4, lcurve, rcurve, opens.cm, opens.cdepth, opens.ccat);                                    <*/
   /*---(other)---------------------------------*/
   o.tmp[1].range  = opens.rads;
   o.tmp[0].cano  = o.tmp[1].cano  = '-';
   o.tmp[0].type   = o.tmp[1].type   = '-';
   strncpy(o.tmp[0].use, "-", 3);
   strncpy(o.tmp[1].use, "-", 3);
   /*---(complete)------------------------------*/
   /*> printf("deg = %4d, range = %2d; and len = %4d, size = %d; and curve = %6.2f, curvea = %2d\n",   <* 
    *>       opens.d, opens.rads, opens.len, size, opens.cdepth, opens.ccat);                                                 <*/
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
   if      (a_len < SIZE_SML_MIN)    x_size = 0;
   else if (a_len < SIZE_SML_MAX)    x_size = 1;
   else if (a_len < SIZE_MED_MAX)    x_size = 2;
   else if (a_len < SIZE_LRG_MAX)    x_size = 3;
   else                              x_size = 4;
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
      curr  = o.key[a_beg + i + 1].quad;
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
      curr  = o.key[a_beg + i + 1].cano;
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
MATCH__available   (int a_beg)
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
         opens.oshape[opens.sharps - 1] = o.key[end].cano;
         opens.oflow [opens.sharps - 1] = o.key[end].quad + '0';
      }
      /*---(check)-----------------------*/
      if (o.key[end].cano != '-') cutoff = 1;
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
MATCH__prep        (void)
{
   /*---(locals)-------------------------*/
   int       i         = 0;                 /* loop iterator -- key points    */
   int       nclean    = 0;                 /* number of cleaned segments     */
   /*---(clean out little leftovers)-----*/
   for (i = 1; i < o.nkey; ++i) {
      if (strncmp(o.key[i].use, "-", 5) != 0) continue;
      if (o.key[i].len <= 5) {
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
      if (o.key[i].len < SIZE_SML_MIN) {
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
      if (strncmp (g_loc[j].n, "eof", 5)              == 0) break;
      if (strncmp (groups[a_rule].gr, g_loc[j].gr, 5) != 0) continue;
      /*---(screen letters)------------------*/
      ++trys_ltr;
      if (trys_ltr <= 3) {
         DEBUG__MATCHES  printf("%1d:%-2.2s ", g_loc[j].sz, g_loc[j].n);
      }
      if (g_loc[j].sz != a_size                          ) continue;
      /*---(give results)-----------------*/
      rc = 1;
      strncpy (match, g_loc[j].n, 5);
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
   /*> printf("\n");                                                                  <*/
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
   for (i = 0; groups[i].range < 900; ++i) {
      /*---(only review active rules)--------*/
      if (groups[i].ac != 1       )   continue;    /* active 1 = yes        */
      /*---(primary filtering)---------------*/
      if (groups[i].op != a_opens )   continue;    /* number of opens       */
      if (groups[i].range != opens.rads &&               /* wrong primary range   */
            groups[i].range != opens.r2) continue;    /*    and not secondary  */
      /*> if (groups[i].cu != opens.ccat)   continue;    /+ wrong curviness       +/    <*/
      j = opens.ccat + CURVE_OFFSET;
      if (strchr("- _", groups[i].ca [j]) != 0)   continue;    /* wrong curviness       */
      /*---(test specific group)-------------*/
      if (strncmp(a_group, "", 5) == 0) {
         strncpy(gname, "n/a", 5);
      } else {
         strncpy(gname, a_group, 5);
         if (strncmp(groups[i].gr, a_group, 5) != 0) continue;
      }
      /*---(start report out)----------------*/
      /*> printf("      #%02d [%-3.3s] f=%2d o=%1d, r=%1d, c=%2d : ",                 <* 
       *>       i, groups[i].gr, a_beg, groups[i].op, groups[i].range, opens.ccat);        <*/
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
          *>    printf("      #-- [%-3.3s] f=%2d o=%1d, r=%1d, c=%2d :", a_group, a_beg, a_opens, opens.rads, opens.ccat);   <* 
          *>    printf(" --- --- --- : --- --- --- --- : ---- ---- ---- [ no joy    ]\n");                              <* 
          *> }                                                                                                          <*/
   }
   /*---(complete)-----------------------*/
   return rc;
}

char          /*----: walk through rules for all normal letters --------------*/
MATCH_driver       (void)
{
   /*---(locals)-------------------------*/
   int   beg       = 0;      /* current open point                            */
   int   count     = 0;      /* number of opens to test                       */
   char  rc        = 0;      /* simple return code                            */
   int   trys_grp  = 0;      /* number of possible group rules                */
   int   trys_ltr  = 0;      /* number of possible letter rules               */
   /*---(process)------------------------*/
   DEBUG__MATCHES  printf("\nMATCHING LETTERS (beg)--------------------\n");
   MATCH__prep     ();
   /*> DEBUG__MATCHES  POINT_list (stdout, o.key, o.nkey);                            <*/
   for (beg = 0; beg < o.nkey - 1; ++beg) {
      /*---(get open point count)------------------*/
      MATCH__available (beg);
      if (opens.sharps < 1)  continue;
      /*---(work from longest to shortest)---------*/
      for (count = 7; count > 0; --count) {
         rc = 0;
         if (count > opens.sharps)               continue;
         match_calc (beg, count);
         /*> printf("\n   %02d-%02d%c =========  o=%1d, r=%1d, c=%2d : ",                           <* 
          *>       beg, beg + count, count > (opens.norm) ? '*' : ' ', count, opens.rads, opens.ccat);   <*/
         /*> printf("le=%4d, sz=%1d, de=%4d, mc=%5.1f  ==========================\n",   <* 
          *>       opens.len, opens.la, opens.d, opens.cm);                               <*/
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
   /*> DEBUG__MATCHES  POINT_list (stdout, o.key, o.nkey);                            <*/
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
      y1        = o.avg[p1].y_full;
      y2        = o.avg[p4].y_full;
   } else {
      y1        = o.avg[p2].y_full;
      y2        = o.avg[p3].y_full;
   }
   yt        = y1;
   DEBUG__MATCHES  printf("y1=%4d, y2=%4d, yt=%5.1f, ", y1, y2, yt);
   /*---(find midpoint)------------------*/
   for (i = p2 + 1; i < p3; ++i) {
      dist = fabs(o.avg[i].y_full - yt);
      /*> DEBUG__MATCHES  printf("%2d at %3d is %6.2f\n", i, o.avg[i].y_full, dist);       <*/
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
      y1        = o.avg[p1].y_full;
      y2        = o.avg[p4].y_full;
   } else {
      y1        = o.avg[p2].y_full;
      y2        = o.avg[p3].y_full;
   }
   yt        = y1 + ((y2 - y1) / 2.0);
   DEBUG__MATCHES  printf("y1=%4d, y2=%4d, yt=%5.1f, ", y1, y2, yt);
   /*---(find midpoint)------------------*/
   for (i = p2 + 1; i < p3; ++i) {
      dist = fabs(o.avg[i].y_full - yt);
      /*> DEBUG__MATCHES  printf("%2d at %3d is %6.2f\n", i, o.avg[i].y_full, dist);       <*/
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
      x1        = o.avg[p1].x_full;
      x2        = o.avg[p4].x_full;
   } else {
      x1        = o.avg[p2].x_full;
      x2        = o.avg[p3].x_full;
   }
   xt        = x1;
   DEBUG__MATCHES  printf("x1=%4d, x2=%4d, xt=%5.1f, ", x1, x2, xt);
   /*---(find midpoint)------------------*/
   for (i = p2 + 1; i < p3; ++i) {
      dist = fabs(o.avg[i].x_full - xt);
      /*> DEBUG__MATCHES  printf("%2d at %3d is %6.2f\n", i, o.avg[i].x_full, dist);       <*/
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
      x1        = o.avg[p1].x_full;
      x2        = o.avg[p4].x_full;
   } else {
      x1        = o.avg[p2].x_full;
      x2        = o.avg[p3].x_full;
   }
   xt        = x1 + ((x2 - x1) / 2.0);
   DEBUG__MATCHES  printf("x1=%4d, x2=%4d, xt=%5.1f, ", x1, x2, xt);
   /*---(find midpoint)------------------*/
   for (i = p2 + 1; i < p3; ++i) {
      dist = fabs(o.avg[i].x_full - xt);
      /*> DEBUG__MATCHES  printf("%2d at %3d is %6.2f\n", i, o.avg[i].x_full, dist);       <*/
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
      if (o.key[a_beg + i].quad == quad) {
         DEBUG__MATCHES  printf("q%1d:%1d=%1d ", i, o.key[a_beg + i].quad, quad);
      } else {
         DEBUG__MATCHES  printf("q%1d:%1dx%1d ", i, o.key[a_beg + i].quad, quad);
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
   quad = o.key[a_beg + 1].quad + '0';
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



/*============================----(source-end)----============================*/
