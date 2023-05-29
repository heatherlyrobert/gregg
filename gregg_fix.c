/*============================---(source-start)---============================*/
#include "gregg.h"



static char     s_catall [LEN_DESC]  = CAT_ALL;

/*> uchar  s_ae_OLD  [16] [16] [LEN_TERSE] = {                                                                                                       <* 
 *>    /+ prev/next ----  , d---  , oth-  , uth-  , -df-  , -m--  , -k--  , -r--  , -rd-  , -or-  , -ng-  , -ch-  , -f--  , -p--  , -pt-   +/        <* 
 *>    /+   -  +/ { "-"    , "1d"   , "aT"   , "am"   , "aT"   ,"axm"  ,"axk"  ,"axr"  ,"axr"  ,"-"    ,"a"    ,"a"    ,"axf"  ,"axp"  ,"-"     },   <* 
 *>    /+           ----  , d---  , uth-  , oth-  , -df-  , -m--  , -k--  , -r--  , -rd-  , -or-  , -ng-  , -ch-  , -f--  , -p--  , -pt-   +/        <* 
 *>    /+   d  +/ { "dx"   , "dd"   , "add"  , "adH"  , "add"  ,"amd"  ,"a"    ,"a"    ,"a"    ,"-"    ,"a"    ,"a"    ,"a"    ,"a"    ,"-"     },   <* 
 *>    /+ oth  +/ { "a"    , "a"    , "a"    , "a"    , "a"    ,"am"   ,"a"    ,"a"    ,"a"    ,"-"    ,"a"    ,"a"    ,"a"    ,"a"    ,"-"     },   <* 
 *>    /+ uth  +/ { "a"    , "a"    , "a"    , "a"    , "a"    ,"aHm"  ,"a"    ,"a"    ,"a"    ,"-"    ,"a"    ,"a"    ,"a"    ,"a"    ,"-"     },   <* 
 *>    /+  df  +/ { "a"    , "a"    , "a"    , "a"    , "a"    ,"aDm"  ,"a"    ,"a"    ,"a"    ,"-"    ,"a"    ,"a"    ,"a"    ,"a"    ,"-"     },   <* 
 *>    /+           ----  , d---  , uth-  , oth-  , -df-  , -m--  , -k--  , -r--  , -rd-  , -or-  , -ng-  , -ch-  , -f--  , -p--  , -pt-   +/        <* 
 *>    /+   m  +/ { "amx"  , "adm"  , "amT"  , "amH"  , "afm"  ,"amm"  ,"amk"  ,"amr"  ,"amr"  ,"-"    ,"amN"  ,"amj"  ,"amf"  ,"amp"  ,"-"     },   <* 
 *>    /+   k  +/ { "a"    , "a"    , "a"    , "a"    , "a"    ,"akm"  ,"a"    ,"ar"   ,"a"    ,"-"    ,"a"    ,"a"    ,"a"    ,"a"    ,"-"     },   <* 
 *>    /+   r  +/ { "a"    , "a"    , "a"    , "a"    , "a"    ,"arm"  ,"a"    ,"a"    ,"a"    ,"-"    ,"a"    ,"a"    ,"a"    ,"a"    ,"-"     },   <* 
 *>    /+  rd  +/ { "a"    , "a"    , "a"    , "a"    , "a"    ,"aRm"  ,"a"    ,"a"    ,"a"    ,"-"    ,"a"    ,"a"    ,"a"    ,"a"    ,"-"     },   <* 
 *>    /+> /+  or  +/ {"a"  ,"a"  ,"a"  ,"a"  ,"a"  ,"arm","a"  ,"a"  ,"a"  ,"-"  ,"a"  ,"a"  ,"a"  ,"a"  ,"-"   },   <+/                            <* 
 *>    /+           ----  , d---  , uth-  , oth-  , -df-  , -m--  , -k--  , -r--  , -rd-  , -or-  , -ng-  , -ch-  , -f--  , -p--  , -pt-   +/        <* 
 *>    /+  ng  +/ { "a"    , "a"    , "a"    , "a"    , "a"    ,"aNm"  ,"a"    ,"a"    ,"a"    ,"-"    ,"a"    ,"a"    ,"a"    ,"a"    ,"-"     },   <* 
 *>    /+           ----  , d---  , uth-  , oth-  , -df-  , -m--  , -k--  , -r--  , -rd-  , -or-  , -ng-  , -ch-  , -f--  , -p--  , -pt-   +/        <* 
 *>    /+  ch  +/ { "a"    , "a"    , "a"    , "a"    , "a"    ,"ajm"  ,"a"    ,"a"    ,"a"    ,"-"    ,"a"    ,"a"    ,"a"    ,"a"    ,"-"     },   <* 
 *>    /+   f  +/ { "a"    , "a"    , "a"    , "a"    , "a"    ,"afm"  ,"a"    ,"a"    ,"a"    ,"-"    ,"a"    ,"a"    ,"a"    ,"a"    ,"-"     },   <* 
 *>    /+   p  +/ { "a"    , "a"    , "a"    , "a"    , "a"    ,"apm"  ,"a"    ,"a"    ,"a"    ,"-"    ,"a"    ,"a"    ,"a"    ,"a"    ,"-"     },   <* 
 *>    /+  pt  +/ { "-"    , "-"    , "-"    , "-"    , "-"    ,"-"    ,"-"    ,"-"    ,"-"    ,"-"    ,"-"    ,"-"    ,"-"    ,"-"    ,"-"     },   <* 
 *>    /+           ----  , d---  , uth-  , oth-  , -df-  , -m--  , -k--  , -r--  , -rd-  , -or-  , -ng-  , -ch-  , -f--  , -p--  , -pt-   +/        <* 
 *> };                                                                                                                                               <*/


uchar  s_ae  [LEN_TITLE] [LEN_TITLE] [LEN_TERSE] = {
   /* prev/next -none- , d----- , -uth-- , -oth-- , -df--- , -m---- , -k---- , -r---- , -rd--- , -ng--- , -j---- , -f---- , -p---- , -pt--- , -s---- , -z---- , -xs--- , -xz---  */
   /* none */ { "xx"   , "xd"   , "xH"   , "xT"   , "xD"   , "xm"   , "xk"   , "xr"   , "xR"   , "xN"   , "xj"   , "xf"   , "xp"   , "xP"   , "xs"   , "xz"   , "-"    , "-"    },
   /*           ------ , d----- , -uth-- , -oth-- , -df--- , -m---- , -k---- , -r---- , -rd--- , -ng--- , -ch--- , -f---- , -p---- , -pt--- , -s---- , -z----  */
   /*   d  */ { "dx"   , "dd"   , "dH"   , "dT"   , "dD"   , "dm"   , "dk"   , "dr"   , "dR"   , "dN"   , "dj"   , "df"   , "dp"   , "dP"   , "ds"   , "dz"   , "-"    , "-"    },
   /* uth  */ { "Hx"   , "Hd"   , "HH"   , "HT"   , "HD"   , "Hm"   , "Hk"   , "Hr"   , "HR"   , "HN"   , "-"    , "-"    , "-"    , "-"    , "-"    , "-"    , "-"    , "-"    },
   /* oth  */ { "Tx"   , "Td"   , "TH"   , "TT"   , "TD"   , "Tm"   , "Tk"   , "Tr"   , "TR"   , "TN"   , "Tj"   , "Tf"   , "Tp"   , "TP"   , "Ts"   , "Tz"   , "-"    , "-"    },
   /*  df  */ { "Dx"   , "Dd"   , "DH"   , "DT"   , "DD"   , "Dm"   , "Dk"   , "Dr"   , "DR"   , "DN"   , "Dj"   , "Df"   , "Dp"   , "DP"   , "Ds"   , "Dz"   , "-"    , "-"    },
   /*           ------ , d----- , -uth-- , -oth-- , -df--- , -m---- , -k---- , -r---- , -rd--- , -ng--- , -ch--- , -f---- , -p---- , -pt--- , -s---- , -z----  */
   /*   m  */ { "mx"   , "md"   , "mH"   , "mT"   , "mD"   , "mm"   , "mk"   , "mr"   , "mR"   , "mN"   , "mj"   , "mf"   , "mp"   , "mP"   , "ms"   , "mz"   , "-"    , "-"    },
   /*   k  */ { "kx"   , "kd"   , "kH"   , "kT"   , "kD"   , "km"   , "kk"   , "kr"   , "kR"   , "kN"   , "kj"   , "kf"   , "kp"   , "kP"   , "ks"   , "kz"   , "-"    , "-"    },
   /*   r  */ { "rx"   , "rd"   , "rH"   , "rT"   , "rD"   , "rm"   , "rk"   , "rr"   , "rR"   , "rN"   , "rj"   , "rf"   , "rp"   , "rP"   , "rs"   , "rz"   , "-"    , "-"    },
   /*  rd  */ { "Rx"   , "Rd"   , "RH"   , "RT"   , "RD"   , "Rm"   , "Rk"   , "Rr"   , "RR"   , "RN"   , "-"    , "-"    , "-"    , "-"    , "-"    , "-"    , "-"    , "-"    },
   /*           ------ , d----- , -uth-- , -oth-- , -df--- , -m---- , -k---- , -r---- , -rd--- , -ng--- , -ch--- , -f---- , -p---- , -pt--- , -s---- , -z----  */
   /*  ng  */ { "Nx"   , "Nd"   , "NH"   , "NT"   , "ND"   , "Nm"   , "Nk"   , "Nr"   , "NR"   , "NN"   , "Nj"   , "Nf"   , "Np"   , "NP"   , "Ns"   , "Nz"   , "-"    , "-"    },
   /*           ------ , d----- , -uth-- , -oth-- , -df--- , -m---- , -k---- , -r---- , -rd--- , -ng--- , -ch--- , -f---- , -p---- , -pt--- , -s---- , -z----  */
   /*  ch  */ { "jx"   , "jd"   , "jH"   , "jT"   , "jD"   , "jm"   , "jk"   , "jr"   , "jR"   , "jN"   , "jj"   , "jf"   , "jp"   , "jP"   , "js"   , "jz"   , "-"    , "-"    },
   /*   f  */ { "fx"   , "fd"   , "fH"   , "fT"   , "fD"   , "fm"   , "fk"   , "fr"   , "fR"   , "fN"   , "fj"   , "ff"   , "fp"   , "fP"   , "fs"   , "fz"   , "-"    , "-"    },
   /*   p  */ { "px"   , "pd"   , "pH"   , "pT"   , "pD"   , "pm"   , "pk"   , "pr"   , "pR"   , "pN"   , "pj"   , "pf"   , "pp"   , "pP"   , "ps"   , "pz"   , "-"    , "-"    },
   /*  pt  */ { "Px"   , "Pd"   , "PH"   , "PT"   , "PD"   , "Pm"   , "Pk"   , "Pr"   , "PR"   , "PN"   , "-"    , "-"    , "-"    , "-"    , "-"    , "-"    , "-"    , "-"    },
   /*           ------ , d----- , -uth-- , -oth-- , -df--- , -m---- , -k---- , -r---- , -rd--- , -ng--- , -ch--- , -f---- , -p---- , -pt--- , -s---- , -z----  */
   /*   s  */ { "fx"   , "fd"   , "fH"   , "fT"   , "fD"   , "fm"   , "fk"   , "fr"   , "fR"   , "fN"   , "fj"   , "ff"   , "fp"   , "fP"   , "fs"   , "fz"   , "-"    , "-"    },
   /*   z  */ { "px"   , "pd"   , "pH"   , "pT"   , "pD"   , "pm"   , "pk"   , "pr"   , "pR"   , "pN"   , "pj"   , "pf"   , "pp"   , "pP"   , "ps"   , "pz"   , "-"    , "-"    },
   /*           ------ , d----- , -uth-- , -oth-- , -df--- , -m---- , -k---- , -r---- , -rd--- , -ng--- , -ch--- , -f---- , -p---- , -pt--- , -s---- , -z----  */
   /*   xs */ { "sx"   , "-"    , "-"    , "-"    , "-"    , "-"    , "-"    , "-"    , "-"    , "-"    , "-"    , "-"    , "-"    , "-"    , "-"    , "-"    , "-"    , "-"    },
   /*   xz */ { "zx"   , "-"    , "-"    , "-"    , "-"    , "-"    , "-"    , "-"    , "-"    , "-"    , "-"    , "-"    , "-"    , "-"    , "-"    , "-"    , "-"    , "-"    },
   /*           ------ , d----- , -uth-- , -oth-- , -df--- , -m---- , -k---- , -r---- , -rd--- , -ng--- , -ch--- , -f---- , -p---- , -pt--- , -s---- , -z----  */
   /*   w  */ { "wx"   , "wd"   , "wH"   , "wT"   , "wf"   , "wm"   , "wk"   , "wr"   , "wR"   , "wN"   , "wj"   , "wf"   , "wp"   , "wP"   , "-"    , "-"    , "-"    , "-"    },
   /*           ------ , d----- , -uth-- , -oth-- , -df--- , -m---- , -k---- , -r---- , -rd--- , -ng--- , -ch--- , -f---- , -p---- , -pt--- , -s---- , -z----  */
};

/*> uchar  s_o    [LEN_LABEL] [LEN_LABEL] = {                                         <* 
 *>    /+ prev    ------next-------   +/                                              <* 
 *>    /+ ------- ´dHTDmkrRNjfpPsz"   +/                                              <* 
 *>    /+ ´   +/ "·798·9899···8···",                                                  <* 
 *>    /+ d   +/ "88···8899·8888·8",                                                  <* 
 *>    /+ uth +/ "·7·7······8888·8",                                                  <* 
 *>    /+ oth +/ "·8···8····8888·8",                                                  <* 
 *>    /+ df  +/ "················",                                                  <* 
 *>    /+ ------- ´dHTDmkrRNjfpPsz"   +/                                              <* 
 *>    /+ m   +/ "·7·7·8·99·8888·8",                                                  <* 
 *>    /+ k   +/ "·7·7······8888·8",                                                  <* 
 *>    /+ r   +/ "·777·87··88888·8",                                                  <* 
 *>    /+ rd  +/ "················",                                                  <* 
 *>    /+ ------- ´dHTDmkrRNjfpPsz"   +/                                              <* 
 *>    /+ ng  +/ "················",                                                  <* 
 *>    /+ ------- ´dHTDmkrRNjfpPsz"   +/                                              <* 
 *>    /+ ch  +/ "················",                                                  <* 
 *>    /+ f   +/ "················",                                                  <* 
 *>    /+ p   +/ "44445444455555·5",                                                  <* 
 *>    /+ pt  +/ "················",                                                  <* 
 *>    /+ ------- ´dHTDmkrRNjfpPsz"   +/                                              <* 
 *>    /+ s   +/ "················",                                                  <* 
 *>    /+ z   +/ "················",                                                  <* 
 *>    /+ ------- ´dHTDmkrRNjfpPsz"   +/                                              <* 
 *> };                                                                                <*/

uchar  s_o    [LEN_LABEL] [LEN_LABEL] = {
   /* prev    ------next-------   */
   /* ------- ´dHTDmkrRNjfpPsz"   */
   /* ´   */ "·728·2899··666··",
   /* d   */ "88···9899·666668",
   /* uth */ "·7·7······868868",
   /* oth */ "·8········868868",
   /* df  */ "·····666666666··",
   /* ------- ´dHTDmkrRNjfpPsz"   */
   /* m   */ "·7·7·2·99·6666·6",
   /* k   */ "·7·7·······66668",
   /* r   */ "·777·87··6666668",
   /* rd  */ "················",
   /* ------- ´dHTDmkrRNjfpPsz"   */
   /* ng  */ "······6666666666",
   /* ------- ´dHTDmkrRNjfpPsz"   */
   /* ch  */ "·6·66·········68",
   /* f   */ "77776676677777··",
   /* p   */ "4444544445555555",
   /* pt  */ "············6·66",
   /* ------- ´dHTDmkrRNjfpPsz"   */
   /* s   */ "6666666666666666",
   /* z   */ "4444444444545555",
   /* ------- ´dHTDmkrRNjfpPsz"   */
};

uchar  s_u    [LEN_LABEL] [LEN_LABEL] = {
   /* prev    ------next-------   */
   /* ------- ´dHTDmkrRNjfpPsz"   */
   /* -   */ "··········6866·6",
   /* d   */ "88877888887888·8",
   /* uth */ "················",
   /* oth */ "················",
   /* df  */ "··········6866·6",
   /* ------- ´dHTDmkrRNjfpPsz"   */
   /* m   */ "2989929·········",
   /* k   */ "················",
   /* r   */ "················",
   /* rd  */ "················",
   /* ------- ´dHTDmkrRNjfpPsz"   */
   /* ng  */ "················",
   /* ------- ´dHTDmkrRNjfpPsz"   */
   /* ch  */ "················",
   /* f   */ "6666666666666666",
   /* p   */ "················",
   /* pt  */ "················",
   /* ------- ´dHTDmkrRNjfpPsz"   */
   /* s   */ "················",
   /* z   */ "················",
   /* ------- ´dHTDmkrRNjfpPsz"   */
};

uchar  s_sz   [LEN_LABEL] [LEN_LABEL] = {
   /* prev    ------next-------   */
   /* ------- ´dHTDmkrRNjfpPsz"   */
   /* -   */ "··············",
   /* d   */ "··············",
   /* uth */ "··············",
   /* oth */ "··············",
   /* df  */ "··············",
   /* ------- ´dHTDmkrRNjfpPsz"   */
   /* m   */ "77777777777777",
   /* k   */ "77777777777777",
   /* r   */ "77777777777777",
   /* rd  */ "··············",
   /* ------- ´dHTDmkrRNjfpPsz"   */
   /* ng  */ "··············",
   /* ------- ´dHTDmkrRNjfpPsz"   */
   /* ch  */ "··············",
   /* f   */ "77777777777777",
   /* p   */ "22222222222222",
   /* pt  */ "··············",
   /* ------- ´dHTDmkrRNjfpPsz"   */
   /* s   */ "················",
   /* z   */ "················",
   /* ------- ´dHTDmkrRNjfpPsz"   */
};



/*============================--------------------============================*/
/*===----                      support functions                       ----===*/
/*============================--------------------============================*/
static void o___SUPPORT________________o (void) {;}

char
FIX__prepare            (char *i, char *j, char b_gregg [LEN_TITLE], char r_shown [LEN_HUND], short r_drawn [LEN_LABEL], char r_tree [LEN_TERSE])
{
   /*---(locals)-----------+-----+-----+-*/
   char        rce         =  -10;
   char        rc          =    0;
   char        x_gregg     [LEN_TITLE] = "";
   char       *p           = NULL;
   char       *q           =  "·";
   char       *r           = NULL;
   int         k           =    0;
   /*---(header)-------------------------*/
   DEBUG_CONF   yLOG_enter   (__FUNCTION__);
   /*---(defense)------------------------*/
   DEBUG_CONF   yLOG_point   ("b_gregg"   , b_gregg);
   --rce;  if (b_gregg == NULL) {
      DEBUG_CONF   yLOG_exitr   (__FUNCTION__, rce);
      return rce;
   }
   /*---(prepare)------------------------*/
   if (i != NULL)  *i = 0;
   if (j != NULL)  *j = 0;
   /*---(prepare)------------------------*/
   strlcpy (x_gregg, b_gregg, LEN_TITLE);
   strlcpy (b_gregg, ""     , LEN_TITLE);
   /*---(begin parse)--------------------*/
   p = strtok_r (x_gregg, q, &r);
   DEBUG_CONF   yLOG_point   ("p"         , p);
   while (p != NULL) {
      if      (strlen (b_gregg) > 0)    strlcat (b_gregg, "·"  , LEN_TITLE);
      if      (strcmp (p, "i" ) == 0)   strlcat (b_gregg, "a·+", LEN_TITLE);
      else if (strcmp (p, "ia") == 0)   strlcat (b_gregg, "a· ", LEN_TITLE);
      else if (strcmp (p, "ie") == 0)   strlcat (b_gregg, "a· ", LEN_TITLE);
      else if (strcmp (p, "io") == 0)   strlcat (b_gregg, "a· ", LEN_TITLE);
      else if (strcmp (p, "ea") == 0)   strlcat (b_gregg, "a·á", LEN_TITLE);
      else if (strcmp (p, "wa") == 0)   strlcat (b_gregg, "a·-", LEN_TITLE);
      else if (strcmp (p, "we") == 0)   strlcat (b_gregg, "e·-", LEN_TITLE);
      else if (strcmp (p, "h" ) == 0)   strlcat (b_gregg, "/"  , LEN_TITLE);
      else                              strlcat (b_gregg, p    , LEN_TITLE);
      p = strtok_r (NULL, q, &r);
      DEBUG_CONF   yLOG_point   ("p"         , p);
   }
   /*---(initialize others)--------------*/
   if (r_shown != NULL)  { for (k = 0; k < LEN_HUND;  ++k)  r_shown  [k] = '\0'; }
   if (r_drawn != NULL)  { for (k = 0; k < LEN_LABEL; ++k)  r_drawn  [k] =  0;   }
   if (r_tree  != NULL)  { for (k = 0; k < LEN_TERSE; ++k)  r_tree   [k] =  0;   }
   /*---(complete)-----------------------*/
   DEBUG_CONF   yLOG_exit    (__FUNCTION__);
   return 0;
}

char
FIX__append             (char *i, char *j, char a_name [LEN_TERSE], short a_new, char b_shown [LEN_HUND], short b_drawn [LEN_LABEL], char b_tree [LEN_TERSE])
{
   /*---(locals)-----------+-----+-----+-*/
   char        rce         =  -10;
   char        t           [LEN_TERSE] = "";
   /*---(header)-------------------------*/
   DEBUG_CONF   yLOG_senter  (__FUNCTION__);
   DEBUG_CONF   yLOG_sint    (*i);
   /*---(defense)------------------------*/
   DEBUG_CONF   yLOG_spoint  (a_name);
   --rce;  if (a_name  == NULL) {
      DEBUG_CONF   yLOG_sexitr  (__FUNCTION__, rce);
      return rce;
   }
   DEBUG_CONF   yLOG_snote   (a_name);
   /*---(shown)--------------------------*/
   if (i != NULL && b_shown != NULL) {
      if (strlen (b_shown) > 0)   strlcat (b_shown, "·", LEN_HUND);
      strlcat (b_shown, a_name, LEN_HUND);
      DEBUG_CONF   yLOG_snote   (b_shown);
   }
   /*---(drawn)--------------------------*/
   DEBUG_CONF   yLOG_sint    (a_new);
   if (i != NULL && b_drawn != NULL) {
      b_drawn [*i] = a_new;
   }
   /*---(cats)---------------------------*/
   if (j != NULL && b_tree  != NULL && *j >= 0 && *j < 9) {
      DEBUG_CONF   yLOG_sint    (a_new);
      if (a_new < g_real) {
         DEBUG_CONF   yLOG_snote   ("normal/real");
         b_tree  [*j] = a_new;
         ++(*j);
      } else if (a_new > g_end) {
         DEBUG_CONF   yLOG_snote   ("must fix");
         sprintf (t, "%c", g_loc [a_new].code);
         DEBUG_CONF   yLOG_snote   (t);
         b_tree  [*j] = TABLE_letter_by_name (t, LTRS_ALL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL);
         DEBUG_CONF   yLOG_sint    (b_tree [*j]);
         ++(*j);
      } else {
         DEBUG_CONF   yLOG_snote   ("non-included");
      }
   }
   /*---(save-back)----------------------*/
   if (i != NULL)   ++(*i);
   /*---(complete)-----------------------*/
   DEBUG_CONF   yLOG_sexit   (__FUNCTION__);
   return 0;
}



/*============================--------------------============================*/
/*===----                      category fixes                          ----===*/
/*============================--------------------============================*/
static void o___CATEGORY_______________o (void) {;}

char
FIX__ae                 (char *i, char *j, char a_pcat, char a_ccat, char a_name [LEN_TERSE], char a_ncat, char b_shown [LEN_HUND], short b_drawn [LEN_LABEL], char b_tree [LEN_TERSE])
{
   /*---(locals)-----------+-----+-----+-*/
   char        rce         =  -10;
   char        x_name      [LEN_TERSE] = "";
   short       x_new       =    0;
   /*---(header)-------------------------*/
   DEBUG_CONF   yLOG_enter   (__FUNCTION__);
   /*---(defense)------------------------*/
   DEBUG_CONF   yLOG_point   ("a_name"    , a_name);
   --rce;  if (a_name  == NULL) {
      DEBUG_CONF   yLOG_exitr   (__FUNCTION__, rce);
      return rce;
   }
   DEBUG_CONF   yLOG_info    ("a_name"    , a_name);
   DEBUG_CONF   yLOG_point   ("b_shown"   , b_shown);
   --rce;  if (b_shown  == NULL) {
      DEBUG_CONF   yLOG_exitr   (__FUNCTION__, rce);
      return rce;
   }
   DEBUG_CONF   yLOG_info    ("b_shown"   , b_shown);
   /*---(corrective)---------------------*/
   if (a_pcat < 0 || a_pcat > CAT_XZ)  a_pcat = 0;
   if (a_ncat < 0 || a_ncat > CAT_XZ)  a_pcat = 0;
   /*---(fix WH)-------------------------*/
   if (a_pcat == CAT_WH) {
      switch (a_ncat) {
      case CAT_NONE : case CAT_D : case CAT_UTH : case CAT_OTH : case CAT_M : case CAT_K :
         DEBUG_CONF   yLOG_note    ("using the narrow shape");
         sprintf (x_name , "ux%c", a_name [0]);
         break;
      default :
         DEBUG_CONF   yLOG_note    ("using the wider shape");
         sprintf (x_name , "ux%c", toupper (a_name [0]));
         break;
      }
      x_new = TABLE_letter_by_name (x_name, LTRS_ALL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL);
      FIX__append (i, j, x_name, x_new, b_shown, b_drawn, b_tree);
   }
   /*---(fix OW)-------------------------*/
   if (a_ncat == CAT_U) {
      a_ncat = CAT_D;
   }
   /*---(create names)-------------------*/
   sprintf (x_name , "%s%s", a_name, s_ae [a_pcat][a_ncat]);
   DEBUG_CONF   yLOG_complex ("table"     , "prev %2d, next %2d, %s name", a_pcat, a_ncat, x_name);
   /*---(verify vowel)-------------------*/
   x_new = TABLE_letter_by_name (x_name, LTRS_ALL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL);
   DEBUG_CONF   yLOG_value   ("x_new"     , x_new);
   --rce;  if (x_new < 0) {
      DEBUG_CONF   yLOG_exitr   (__FUNCTION__, rce);
      return rce;
   }
   /*---(append to output)---------------*/
   FIX__append (i, j, x_name, x_new, b_shown, b_drawn, b_tree);
   /*---(complete)-----------------------*/
   DEBUG_CONF   yLOG_exit    (__FUNCTION__);
   return 0;
}

char
FIX__ou                 (char *i, char *j, char a_pcat, char a_ccat, char a_name [LEN_TERSE], char a_ncat, char b_shown [LEN_HUND], short b_drawn [LEN_LABEL], char b_tree [LEN_TERSE])
{
   /*---(locals)-----------+-----+-----+-*/
   char        rce         =  -10;
   char        x_name      [LEN_TERSE] = "";
   short       x_new       =    0;
   char        x_var       =  '·';
   /*---(header)-------------------------*/
   DEBUG_CONF   yLOG_enter   (__FUNCTION__);
   /*---(defense)------------------------*/
   DEBUG_CONF   yLOG_point   ("a_name"    , a_name);
   --rce;  if (a_name  == NULL) {
      DEBUG_CONF   yLOG_exitr   (__FUNCTION__, rce);
      return rce;
   }
   DEBUG_CONF   yLOG_info    ("a_name"    , a_name);
   DEBUG_CONF   yLOG_point   ("b_shown"   , b_shown);
   --rce;  if (b_shown  == NULL) {
      DEBUG_CONF   yLOG_exitr   (__FUNCTION__, rce);
      return rce;
   }
   DEBUG_CONF   yLOG_info    ("b_shown"   , b_shown);
   /*---(corrective)---------------------*/
   if (a_pcat < 0 || a_pcat > CAT_XZ)  a_pcat = 0;
   if (a_ncat < 0 || a_ncat > CAT_XZ)  a_pcat = 0;
   /*---(fix w)--------------------------*/
   if (a_pcat == CAT_A || a_pcat == CAT_E) {
      DEBUG_CONF   yLOG_note    ("handle u after a/e");
      if (a_pcat == CAT_A)  sprintf (x_name , "uax");
      else                  sprintf (x_name , "uex");
      if (strlen (b_shown) > 0)   strlcat (b_shown, "·", LEN_HUND);
      strlcat (b_shown, x_name, LEN_HUND);
      DEBUG_CONF   yLOG_info    ("b_shown"   , b_shown);
      DEBUG_CONF   yLOG_exit    (__FUNCTION__);
      return 0;
   }
   /*---(create names)-------------------*/
   if (a_ccat == CAT_O)  x_var = s_o [a_pcat][a_ncat];
   else                  x_var = s_u [a_pcat][a_ncat];
   DEBUG_CONF   yLOG_char    ("x_var"     , x_var);
   if (x_var != '·')                sprintf (x_name , "%s%c", a_name, x_var);
   else if (strlen (x_name) == 1)   sprintf (x_name , "%s%c", a_name, '1');
   else                             sprintf (x_name , "%s", a_name);
   DEBUG_CONF   yLOG_complex ("table"     , "prev %2d, next %2d, %s name", a_pcat, a_ncat, x_name);
   /*---(verify vowel)-------------------*/
   x_new = TABLE_letter_by_name (x_name, LTRS_ALL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL);
   DEBUG_CONF   yLOG_value   ("x_new"     , x_new);
   --rce;  if (x_new < 0) {
      DEBUG_CONF   yLOG_exitr   (__FUNCTION__, rce);
      return rce;
   }
   /*---(append to output)---------------*/
   FIX__append (i, j, x_name, x_new, b_shown, b_drawn, b_tree);
   /*---(complete)-----------------------*/
   DEBUG_CONF   yLOG_exit    (__FUNCTION__);
   return 0;
}

char
FIX__other              (char *i, char *j, char a_pcat, char a_ccat, char a_name [LEN_TERSE], char a_ncat, char b_shown [LEN_HUND], short b_drawn [LEN_LABEL], char b_tree [LEN_TERSE])
{
   /*---(locals)-----------+-----+-----+-*/
   char        rce         =  -10;
   char        x_name      [LEN_TERSE] = "";
   short       x_new       =    0;
   /*---(header)-------------------------*/
   DEBUG_CONF   yLOG_enter   (__FUNCTION__);
   /*---(defense)------------------------*/
   DEBUG_CONF   yLOG_point   ("a_name"    , a_name);
   --rce;  if (a_name  == NULL) {
      DEBUG_CONF   yLOG_exitr   (__FUNCTION__, rce);
      return rce;
   }
   DEBUG_CONF   yLOG_info    ("a_name"    , a_name);
   DEBUG_CONF   yLOG_point   ("b_shown"   , b_shown);
   --rce;  if (b_shown  == NULL) {
      DEBUG_CONF   yLOG_exitr   (__FUNCTION__, rce);
      return rce;
   }
   DEBUG_CONF   yLOG_info    ("b_shown"   , b_shown);
   /*---(check for inserts)--------------*/
   if (a_pcat == CAT_F && a_ccat == CAT_R) {
      x_new = TABLE_letter_by_name ("fr", LTRS_ALL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL);
      FIX__append (i, NULL, "fr", x_new, b_shown, b_drawn, b_tree);
   }
   else if (a_pcat == CAT_K && a_ccat == CAT_P) {
      x_new = TABLE_letter_by_name ("kp", LTRS_ALL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL);
      FIX__append (i, NULL, "kp", x_new, b_shown, b_drawn, b_tree);
   }
   /*---(get lettes)---------------------*/
   strlcpy (x_name, a_name, LEN_TERSE);
   x_new = TABLE_letter_by_name (x_name, LTRS_ALL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL);
   DEBUG_CONF   yLOG_value   ("x_new"     , x_new);
   --rce;  if (x_new < 0) {
      DEBUG_CONF   yLOG_exitr   (__FUNCTION__, rce);
      return rce;
   }
   /*---(append to output)---------------*/
   FIX__append (i, j, x_name, x_new, b_shown, b_drawn, b_tree);
   /*---(complete)-----------------------*/
   DEBUG_CONF   yLOG_exit    (__FUNCTION__);
   return 0;
}



/*============================--------------------============================*/
/*===----                         main driver                          ----===*/
/*============================--------------------============================*/
static void o___DRIVER_________________o (void) {;}

char
FIX_gregg               (char a_gregg [LEN_TITLE], char r_shown [LEN_HUND], short r_drawn [LEN_LABEL], char r_tree [LEN_TERSE], char r_sort [LEN_LABEL])
{
   /*---(locals)-----------+-----+-----+-*/
   char        rce         =  -10;
   char        rc          =    0;
   char        i           =    0;
   char        j           =    0;
   char        x_gregg     [LEN_TITLE] = "";
   char       *n           = NULL;
   char       *p           = NULL;
   char       *q           =  "·";
   char       *r           = NULL;
   char       *nn          = NULL;
   short       x_pprev     =   -1;
   char          x_lpprev  [LEN_TERSE] = "";
   char          x_ppcat   =    0;
   short       x_prev      =   -1;
   char          x_lprev   [LEN_TERSE] = "";
   char          x_pcat    =    0;
   short       x_curr      =   -1;
   char          x_lcurr   [LEN_TERSE] = "";
   char          x_ccat    =    0;
   short       x_next      =   -1;
   char          x_lnext   [LEN_TERSE] = "";
   char          x_ncat    =    0;
   short       x_nnext     =   -1;
   char          x_lnnext  [LEN_TERSE] = "";
   char          x_nncat   =    0;
   char        x_shown     [LEN_HUND]  = "";
   short       x_drawn     [LEN_LABEL];
   char        x_tree      [LEN_TERSE] = "";
   /*---(header)-------------------------*/
   DEBUG_CONF   yLOG_enter   (__FUNCTION__);
   /*---(defense)------------------------*/
   DEBUG_CONF   yLOG_point   ("a_gregg"   , a_gregg);
   --rce;  if (a_gregg == NULL) {
      DEBUG_CONF   yLOG_exitr   (__FUNCTION__, rce);
      return rce;
   }
   /*---(prepare)------------------------*/
   strlcpy (x_gregg, a_gregg, LEN_TITLE);
   FIX__prepare (&i, &j, x_gregg, x_shown, x_drawn, x_tree);
   /*---(begin parse)--------------------*/
   p = strtok_r (x_gregg, q, &r);
   DEBUG_CONF   yLOG_point   ("p"         , p);
   if (p == NULL) {
      DEBUG_CONF   yLOG_exitr   (__FUNCTION__, rce);
      return rce;
   }
   x_curr = TABLE_letter_by_name (p, LTRS_ALL, NULL, &x_ccat, x_lcurr, NULL, NULL, NULL, NULL, NULL, NULL);
   DEBUG_CONF   yLOG_value   ("x_curr"    , x_curr);
   if (x_curr < 0) {
      DEBUG_CONF   yLOG_exitr   (__FUNCTION__, rce);
      return rce;
   }
   n = strtok_r (NULL   , q, &r);
   DEBUG_CONF   yLOG_point   ("n"         , n);
   if (n != NULL) {
      x_next = TABLE_letter_by_name (n, LTRS_ALL, NULL, &x_ncat, x_lnext, NULL, NULL, NULL, NULL, NULL, NULL);
      DEBUG_CONF   yLOG_value   ("x_next"    , x_next);
   }
   nn = strtok_r (NULL   , q, &r);
   DEBUG_CONF   yLOG_point   ("nn"        , nn);
   if (n != NULL) {
      x_nnext = TABLE_letter_by_name (nn, LTRS_ALL, NULL, &x_nncat, x_lnnext, NULL, NULL, NULL, NULL, NULL, NULL);
      DEBUG_CONF   yLOG_value   ("x_next"    , x_nnext);
   }
   /*---(walk gregg)---------------------*/
   --rce;  while (x_curr >= 0) {
      DEBUG_CONF   yLOG_complex ("LOOP"      , "#%2d/%2d, prev %3d/%s/%2d, curr %3d/%s/%2d, next %3d/%s/%2d, nnext %3d/%s/%2d", i, j, x_prev, x_lprev, x_pcat, x_curr, x_lcurr, x_ccat, x_next, x_lnext, x_ncat, x_nnext, x_lnnext, x_nncat);
      /*---(special)---------------------*/
      if (x_pcat == CAT_NONE && x_ccat == CAT_U && (x_ncat == CAT_A || x_ncat == CAT_E)) {
         DEBUG_CONF   yLOG_note    ("prepare a leading ¶wh¶");
         x_ccat = CAT_WH;
         strlcpy (x_lcurr, "wh", LEN_TERSE);
      }
      /*---(normal)----------------------*/
      else if (strchr ("+-á ", x_lnext [0]) != NULL) {
         if      (x_ccat == CAT_A || x_ccat == CAT_E)    rc = FIX__ae     (&i, &j, x_pcat, x_ccat, x_lcurr, x_nncat, x_shown, x_drawn, x_tree);
         else if (x_ccat == CAT_O || x_ccat == CAT_U)    rc = FIX__ou     (&i, &j, x_pcat, x_ccat, x_lcurr, x_nncat, x_shown, x_drawn, x_tree);
         else                                            rc = FIX__other  (&i, &j, x_pcat, x_ccat, x_lcurr, x_nncat, x_shown, x_drawn, x_tree);
      }
      else  {
         if      (x_ccat == CAT_A || x_ccat == CAT_E)    rc = FIX__ae     (&i, &j, x_pcat, x_ccat, x_lcurr, x_ncat , x_shown, x_drawn, x_tree);
         else if (x_ccat == CAT_O || x_ccat == CAT_U)    rc = FIX__ou     (&i, &j, x_pcat, x_ccat, x_lcurr, x_ncat , x_shown, x_drawn, x_tree);
         else                                            rc = FIX__other  (&i, &j, x_pcat, x_ccat, x_lcurr, x_ncat , x_shown, x_drawn, x_tree);
      }
      /*---(next)------------------------*/
      DEBUG_CONF   yLOG_info    ("x_shown"   , x_shown);
      /*---(curr to pre-prev)------------*/
      x_pprev = x_prev;
      x_ppcat = x_pcat;
      strlcpy (x_lpprev, x_lprev, LEN_TERSE);
      /*---(curr to prev)----------------*/
      x_prev = x_curr;
      x_pcat = x_ccat;
      strlcpy (x_lprev, x_lcurr, LEN_TERSE);
      /*---(next to curr)----------------*/
      x_curr = x_next;
      x_ccat = x_ncat;
      strlcpy (x_lcurr, x_lnext, LEN_TERSE);
      /*---(nnext to next)---------------*/
      x_next = x_nnext;
      x_ncat = x_nncat;
      strlcpy (x_lnext, x_lnnext, LEN_TERSE);
      /*---(blank nnext)-----------------*/
      x_nnext = -1;
      x_nncat = CAT_NONE;
      strlcpy (x_lnnext, "", LEN_TERSE);
      /*---(check for another)-----------*/
      if (x_next >= 0) {
         nn = strtok_r (NULL   , q, &r);
         DEBUG_CONF   yLOG_point   ("nn"        , nn);
         if (nn != NULL) {
            x_nnext = TABLE_letter_by_name (nn, LTRS_ALL, NULL, &x_nncat, x_lnnext, NULL, NULL, NULL, NULL, NULL, NULL);
            DEBUG_CONF   yLOG_value   ("x_nnext"   , x_nnext);
         }
      }
      /*---(done)------------------------*/
   }
   /*---(save-back)----------------------*/
   if (r_shown  != NULL) { for (i = 0; i < LEN_HUND;  ++i)   r_shown [i] = x_shown [i]; }
   if (r_drawn  != NULL) { for (i = 0; i < LEN_LABEL; ++i)   r_drawn [i] = x_drawn [i]; }
   if (r_tree   != NULL) { for (i = 0; i < LEN_SHORT; ++i)   r_tree  [i] = x_tree  [i]; }
   if (r_sort   != NULL) strlcpy (r_sort, FIX_tree_showable (x_tree, 5), LEN_LABEL);
   /*---(complete)-----------------------*/
   DEBUG_CONF   yLOG_exit    (__FUNCTION__);
   return 0;
}



/*====================------------------------------------====================*/
/*===----                        searching                             ----===*/
/*====================------------------------------------====================*/
static void  o___HOOKING_________o () { return; }

char
FIX_hook                (tWORD *a_new)
{
   /*---(locals)-----------+-----+-----+-*/
   char        rce         =  -10;
   char        rc          =    0;
   /*---(header)-------------------------*/
   DEBUG_CONF   yLOG_enter   (__FUNCTION__);
   /*---(tree)---------------------------*/
   /*> rc = ySORT_hook (B_TREE   , a_new, a_new->w_tree   , &(a_new->ysort_t));       <* 
    *> DEBUG_CONF   yLOG_value   ("hook tree" , rc);                                  <* 
    *> --rce;  if (rc < 0) {                                                          <* 
    *>    DEBUG_CONF   yLOG_exitr   (__FUNCTION__, rce);                              <* 
    *>    return rce;                                                                 <* 
    *> }                                                                              <* 
    *> rc = ySORT_prepare (B_TREE);                                                   <* 
    *> DEBUG_CONF   yLOG_value   ("prep tree" , rc);                                  <* 
    *> --rce;  if (rc < 0) {                                                          <* 
    *>    DEBUG_CONF   yLOG_exitr   (__FUNCTION__, rce);                              <* 
    *>    return rce;                                                                 <* 
    *> }                                                                              <*/
   /*---(complete)-----------------------*/
   DEBUG_CONF   yLOG_exit    (__FUNCTION__);
   return 0;
}



/*============================--------------------============================*/
/*===----                       reporting/audit                        ----===*/
/*============================--------------------============================*/
static void o___REPORT_________________o (void) {;}

char*
FIX_tree_showable       (char a_tree [LEN_TERSE], char a_max)
{
   /*---(locals)-----------+-----+-----+-*/
   char        t           [LEN_SHORT] = "";
   int         i           =    0;
   /*---(prepare)------------------------*/
   strcpy (g_print, "");
   /*---(walk)---------------------------*/
   for (i = 0; i < a_max; ++i) {
      if (a_tree [i] == 0)       break;
      if (a_tree [i] >= g_real)  break;
      if (i > 0)  strlcat (g_print, "·", LEN_RECD);
      sprintf (t, "%02d", a_tree [i]);
      strlcat (g_print, t, LEN_RECD);
   }
   /*---(complete)-----------------------*/
   return g_print;
}


