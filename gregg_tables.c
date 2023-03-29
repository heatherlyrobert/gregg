/*============================---(source-start)---============================*/
#include "gregg.h"

/*
 * metis § dn4·· § add A, E, I for backside a, e, i (5th, pg 50)                          § N2O3cE §  · §
 * metis § ····· § letters and combinations should lock into ranges for beg-end dir       § N2P2Od §  · §
 *
 */



#define      NN            3
#define      NE            2
#define      EE            1
#define      SE            0
#define      SS            7
#define      SW            6
#define      WW            5
#define      NW            4
#define      nn           11
#define      ne           10
#define      ee            9
#define      se            8
#define      ss           15
#define      sw           14
#define      ww           13
#define      nw           12
#define      __            0



/* PURPOSE : establish ranges of slopes/angles to classity letters            */
tRANGES     g_ranges [MAX_RANGES] = {
   /*  #, len, label,  min,  beg,  tar,  end,  max  */
   {   6,   1,    "",  165,  165,  176,  180,  180, },
   {   7,   1,    "",  120,  120,  140,  165,  165, },
   {   8,   1,    "",   75,   75,   95,  120,  120, },
   {   1,   1,   "d",   20,   30,   45,   60,   75, },
   {   2,   1,   "m",  -20,  -15,    5,   15,   20, },
   {   3,   1,  "ng",  -60,  -55,  -35,  -25,  -20, },
   {   4,   1,   "z",  -90,  -85,  -80,  -65,  -60, },
   {   5,   1,  "sh", -145, -130, -110,  -95,  -90, },
   {   6,   1, "---", -180, -180, -180, -145, -145, },
   {   9,   2,    "",  -30,  -30,  -10,   25,   25, },
   {   0,   1, "EOF",    0,    0,    0,    0,    0, },
};



tCATS       g_cats [] = {

   {  CAT_D    , "d"   , 1,  1,  'd'},
   {  CAT_OTH  , "th"  , 1,  2,  'T'},
   {  CAT_UTH  , "ht"  , 1,  3,  'H'},
   {  CAT_DF   , "df"  , 1,  4,  'D'},

   {  CAT_M    , "m"   , 2,  1,  'm'},
   {  CAT_K    , "k"   , 2,  2,  'k'},
   {  CAT_R    , "r"   , 2,  3,  'r'},
   {  CAT_RD   , "rd"  , 2,  4,  'R'},

   {  CAT_NG   , "ng"  , 3,  1,  'N'},

   {  CAT_CH   , "ch"  , 5,  1,  'j'},
   {  CAT_F    , "f"   , 5,  2,  'f'},
   {  CAT_P    , "p"   , 5,  3,  'p'},
   {  CAT_PT   , "pt"  , 5,  4,  'P'},

   {  CAT_A    , "a"   , 0,  1,  'a'},
   {  CAT_E    , "e"   , 0,  2,  'e'},
   {  CAT_O    , "o"   , 0,  3,  'o'},
   {  CAT_U    , "u"   , 0,  4,  'u'},

   {  0        , "EOF" , 0,  0,  '-'},


};

/*
 *     1-1000-2
 *
 *
 *
 */

/* PURPOSE : establish parameters by which to identify letters in outlines    */
tCOMBOS     g_combos [MAX_COMBOS] = {
   /*===[[ RANGE 1 ]]======================================================*/
   /*  name,  open,   sh,     flow,  mid, st,  off,  one,   two,   repl,  ac  */
   {   "KR",     3,    0,  "141-", "fy",  0,   2,   "k",   "r",  "   ",   1  },
   {   "KO",     3,    0,  "141-", "fy",  0,   2,   "k",   "o",  "   ",   1  },
   {   "KF",     3,    0,  "143-", "fy",  0,   2,   "k",   "f",  "   ",   1  },
   {   "UF",     3,    1,  "143-", "--",  0,   1,   "u",   "f",  "   ",   1  },
   {   "KP",     4,    2,  "1434", "--",  0,   2,   "k",   "p",  "   ",   1  },
   {   "DF",     3,    1,  "143-", "--",  0,   1,  "th",   "f",  "   ",   1  },
   {   "DF",     4,    2,  "2143", "--",  0,   2,  "th",   "f",  "   ",   1  },

   /*===[[ RANGE 3 ]]======================================================*/
   /*  name,  open,   sh,     flow,  mid, st,  off,  one,   two,   repl,  ac  */
   {   "PF",     3,    0,  "343-", "ax",  0,   2,   "p",   "f",  "   ",   1  },
   {   "PR",     3,    0,  "341-", "ay",  1,   2,   "p",   "r",  "   ",   1  },
   {   "PK",     4,    2,  "3414", "--",  0,   2,   "p",   "k",  "   ",   1  },

   /*===[[ RANGE 4 ]]======================================================*/
   /*  name,  open,   sh,     flow,  mid, st,  off,  one,   two,   repl,  ac  */
   {   "RK",     3,    0,  "414-", "fy",  1,   2,   "r",   "k",  "   ",   1  },
   {   "RF",     4,    2,  "4143", "--",  0,   2,   "r",   "f",  "   ",   0  },
   {   "RP",     4,    2,  "4134", "--",  0,   2,   "r",   "p",  "   ",   1  },

   {   "FP",     3,    0,  "434-", "ax",  0,   2,   "f",   "p",  "   ",   1  },
   {   "FR",     4,    2,  "4341", "--",  0,   2,   "f",   "r",  "   ",   1  },
   {   "FK",     4,    2,  "4314", "--",  0,   2,   "f",   "k",  "   ",   1  },

   /*===[[ DONE ]]=========================================================*/
   /*  name,  open,   sh,     flow,  mid, st,  off,  one,   two,   repl,  ac  */
   {  "EOF",     0,    0,  "----", "--",  0,   0,   "-",   "-",  "   ",   1  },
};


#define     USE      1
/* PURPOSE : establish parameters by which to identify letters in outlines    */
tGROUPS     g_groups [MAX_GROUPS] = {

   /*===[[ RANGE 1 ]]======================================================*/
   /* range, opens, flow     , curviness , group  ,  act  */
   /*---( d family)--------*//*--------------------------------------*/
   {      1,     1, "1------", "01------", CAT_D  ,  USE  },
   /*---(th family)--------*//*--------------------------------------*/
   {      1,     1, "1------", "--234---", CAT_OTH,  USE  },
   {      1,     2, "14-----", "--234---", CAT_OTH,  USE  },
   {      1,     2, "21-----", "--234---", CAT_OTH,  USE  },
   {      1,     3, "214----", "--234---", CAT_OTH,    0  },
   /*---(ht family)--------*//*--------------------------------------*/
   {      1,     1, "1------", "--234---", CAT_UTH,  USE  },
   {      1,     2, "41-----", "--234---", CAT_UTH,  USE  },
   {      1,     2, "12-----", "--234---", CAT_UTH,  USE  },
   {      1,     3, "412----", "--234---", CAT_UTH,    0  },

   /*===[[ RANGE 2 ]]======================================================*/
   /* range, opens,  shape,   flow,  valid curve, group, grp#,  act  */
   /*---( m family)--------*//* must cover off unflattened bumpiness */
   {      2,     1, "1------", "01------", CAT_M  ,  USE  },
   {      2,     2, "14-----", "01------", CAT_M  ,  USE  },
   {      2,     3, "141----", "01------", CAT_M  ,    0  },
   {      2,     1, "4------", "01------", CAT_M  ,  USE  },
   {      2,     2, "41-----", "01------", CAT_M  ,  USE  },
   {      2,     3, "414----", "01------", CAT_M  ,    0  },
   /*---( m combo)---------*//*--------------------------------------*/
   {      2,     3, "143----", "--2-----", CAT_M  ,    0  },
   /*---( r family)--------*//*--------------------------------------*/
   {      2,     2, "41-----", "--2345--", CAT_R  ,  USE  },
   {      2,     3, "341----", "--2345--", CAT_R  ,  USE  },
   {      2,     3, "412----", "--2345--", CAT_RD ,  USE  },
   /*---( k family)--------*//*--------------------------------------*/
   {      2,     2, "14-----", "--2345--", CAT_K  ,  USE  },
   {      2,     3, "214----", "--2345--", CAT_K  ,  USE  },
   /*---(df family)--------*//*--------------------------------------*/
   {      2,     2, "14-----", "------67", CAT_DF ,  USE  },
   {      2,     3, "143----", "------67", CAT_DF ,  USE  },
   {      2,     4, "2143---", "------67", CAT_DF ,  USE  },

   /*===[[ RANGE 3 ]]======================================================*/
   /* range, opens,  flow    , curviness , group, grp#,  act  */
   /*---(ng family)--------*//*--------------------------------------*/
   {      3,     1, "4------", "0-------", CAT_NG ,  USE  },
   /*---(sx family)--------*//*--------------------------------------*/
   /*> {      3,     1, "4------", "-1------",  "sx",    8,  USE  },                  <*/
   /*> {      3,     1, "4------", "-1------",  "zx",    9,  USE  },                  <*/

   /*===[[ RANGE 4 ]]======================================================*/
   /* range, opens,  shape,   flow,  valid curve, group, grp#,  act  */
   /*---(sx family)--------*//*--------------------------------------*/
   /*> {      4,     1, "4------", "01------",  "sx",   10,  USE  },                  <*/
   /*> {      4,     2, "4------", "-1------",  "zx",   10,  USE  },                  <*/
   /*---( f family)--------*//*--------------------------------------*/
   {      4,     2, "43-----", "--2345--", CAT_F  ,  USE  },
   {      4,     3, "143----", "--2345--", CAT_F  ,  USE  },
   /*---( p family)--------*//*--------------------------------------*/
   {      4,     2, "34-----", "--2345--", CAT_P  ,  USE  },
   {      4,     3, "341----", "--2345--", CAT_P  ,  USE  },

   /*===[[ RANGE 5 ]]======================================================*/
   /* range, opens,  shape,   flow,  valid curve, group, grp#,  act  */
   /*---(sh family)--------*//*--------------------------------------*/
   {      5,     1, "3------", "0-------", CAT_CH ,  USE  },
   /*---( f family)--------*//*--------------------------------------*/
   {      5,     2, "43-----", "--2345--", CAT_F  ,  USE  },
   {      5,     3, "143----", "--2345--", CAT_F  ,  USE  },
   /*---( p family)--------*//*--------------------------------------*/
   {      5,     2, "34-----", "--2345--", CAT_P  ,  USE  },
   {      5,     3, "341----", "--2345--", CAT_P  ,  USE  },

   /*===[[ RANGE 7 ]]======================================================*/
   /* range, opens,  shape,   flow,  valid curve, group, grp#,  act  */
   /*> {      7,     1, "2------", "01------",  "bd",   15,    0  },                  <*/
   /*> {      7,     1, "2------", "0-------",  "up",   16,  USE  },                  <*/

   /*===[[ RANGE 8 ]]======================================================*/
   /* range, opens,  shape,   flow,  valid curve, group, grp#,  act  */
   /*> {      8,     1, "1------", "0-------",  "up",   16,  USE  },                  <*/
   /*> {      8,     2, "12-----", "0-------",  "up",   16,  USE  },                  <*/
   /*> {      8,     1, "2------", "0-------",  "up",   16,  USE  },                  <*/
   /*> {      8,     2, "21-----", "0-------",  "up",   16,  USE  },                  <*/

   /*===[[ RANGE 9 ]]======================================================*/
   /* range, opens,  shape,   flow,  valid curve, group, grp#,  act  */
   /*---( u family)--------*//*--------------------------------------*/
   /*> {      2,     2, "41-----", "-12-----",   "o",    6,  USE  },                  <*/
   /*> {      2,     3, "341----", "-12-----",   "o",    6,  USE  },                  <*/
   /*---( o family)--------*//*--------------------------------------*/
   /*> {      2,     2, "14-----", "-12-----",   "u",    5,  USE  },                  <*/
   /*> {      2,     3, "143----", "-12-----",   "u",    5,  USE  },                  <*/
   /*---(ts family)--------*//*--------------------------------------*/
   /*> {      2,     2, "14-----", "---3----",  "ts",   16,  USE  },                  <*/
   /*> {      2,     3, "143----", "---3----",  "ts",   16,  USE  },                  <*/

   /*===[[ DONE ]]=========================================================*/
   /* range, opens,  shape,   flow,  valid curve, group, grp#,  act  */
   {     10,     0, "-------", "--------",    0,    0  },
};



tLOCATION   g_loc [MAX_LETTERS] = {
   /*=====[[ HEADER ]]==========================================================*/
   /*======  ===============creation=================   ====categories=====   ======display========   ======================= */
   /*-line,  ---shape----- , -sx, -sy, rot, beg, arc,   -cat--- , -ra, -sz-   --tx, --ty, -align---,  ----calculated--------- */
   { "bof",  SHAPE_NONE    ,   0,   0,   0,   0,   0,   CAT_NONE,   0,   0,      0,    0, YF_BOTRIG,   0, 0, 0, 0, 0, 0, 0, 0 },

   /*=====[[ 1st ZONE ]]=============================   ====categories=====   =======display=======   ======================= */
   /*======  ===============creation=================   ====categories=====   ======display========   ======================= */
   /*-line,  ---shape----- , -sx, -sy, rot, beg, arc,   -cat--- , -ra, -sz-   --tx, --ty, -align---,  ----calculated--------- */
   { "t"  ,  SHAPE_LINE    ,   0,   0,  40,   0,  15,   CAT_D   ,   1,   1,      0,  -20, YF_BOTRIG,   0, 0, 0, 0, 0, 0, 0, 0 },
   { "d"  ,  SHAPE_LINE    ,   0,   0,  40,   0,  30,   CAT_D   ,   1,   2,      0,  -30, YF_BOTRIG,   0, 0, 0, 0, 0, 0, 0, 0 },
   { "dd" ,  SHAPE_LINE    ,   0,   0,  40,   0,  45,   CAT_D   ,   1,   3,      0,  -40, YF_BOTRIG,   0, 0, 0, 0, 0, 0, 0, 0 },
   /*-over,  ---shape----- , -sx, -sy, rot, beg, arc,   -cat--- , -ra, -sz-   --tx, --ty, -align---,  -----,-----clcuate----- */
   { "th" ,  SHAPE_ELLIPSE ,   7,   5,  30, 160,-120,   CAT_OTH ,   1,   1,     55,  -28, YF_TOPRIG,   0, 0, 0, 0, 0, 0, 0, 0 },
   { "tn" ,  SHAPE_ELLIPSE ,  15,   9,  30, 160,-120,   CAT_OTH ,   1,   2,     42,  -33, YF_TOPRIG,   0, 0, 0, 0, 0, 0, 0, 0 },
   { "tm" ,  SHAPE_ELLIPSE ,  22,  12,  30, 160,-120,   CAT_OTH ,   1,   3,     30,  -38, YF_TOPRIG,   0, 0, 0, 0, 0, 0, 0, 0 },
   /*-unde,  ---shape----- , -sx, -sy, rot, beg, arc,   -cat--- , -ra, -szr   --tx, --ty, -align---,  -----,-----clcuate----- */
   { "ht" ,  SHAPE_ELLIPSE ,   7,   5,  30, 220, 120,   CAT_UTH ,   1,   1,     95,  -24, YF_BOTRIG,   0, 0, 0, 0, 0, 0, 0, 0 },
   { "nt" ,  SHAPE_ELLIPSE ,  15,   9,  30, 220, 120,   CAT_UTH ,   1,   2,     87,  -32, YF_BOTRIG,   0, 0, 0, 0, 0, 0, 0, 0 },
   { "nd" ,  SHAPE_ELLIPSE ,  22,  12,  30, 220, 120,   CAT_UTH ,   1,   3,     80,  -40, YF_BOTRIG,   0, 0, 0, 0, 0, 0, 0, 0 },
   /*-arc-,  ---shape----- , -sx, -sy, rot, beg, arc,   -cat--- , -ra, -sz-   --tx, --ty, -align---,  -----,-----clcuate----- */
   { "ts" ,  SHAPE_ELLIPSE ,   8,   4,  50, 140,-230,   CAT_DF  ,   2,   1,      0,    0, YF_BOTRIG,   0, 0, 0, 0, 0, 0, 0, 0 },
   { "df" ,  SHAPE_ELLIPSE ,  14,   7,  50, 140,-230,   CAT_DF  ,   2,   2,      0,    0, YF_BOTRIG,   0, 0, 0, 0, 0, 0, 0, 0 },

   /*=====[[ 2nd ZONE ]]=============================   ====categories=====   =======display=======   ======================= */
   /*-line,  ---shape----- , -sx, -sy, rot, beg, arc,   -cat--- , -ra, -sz-   --tx, --ty, -align-- ,  -----,-----clcuate----- */
   { "n"  ,  SHAPE_LINE    ,   0,   0,   5,   0,  15,   CAT_M   ,   2,   1,     15,  -70, YF_BOTRIG,   0, 0, 0, 0, 0, 0, 0, 0 },
   { "m"  ,  SHAPE_LINE    ,   0,   0,   5,   0,  30,   CAT_M   ,   2,   2,     15,  -63, YF_BOTRIG,   0, 0, 0, 0, 0, 0, 0, 0 },
   { "mm" ,  SHAPE_LINE    ,   0,   0,   5,   0,  45,   CAT_M   ,   2,   3,     15,  -55, YF_BOTRIG,   0, 0, 0, 0, 0, 0, 0, 0 },
   /*-over,  ---shape----- , -sx, -sy, rot, beg, arc,   -cat--- , -ra, -sz-   --tx, --ty, -align-- ,  -----,-----clcuate----- */
   { "k"  ,  SHAPE_ELLIPSE ,  13,   6,   5, 150,-150,   CAT_K   ,   2,   2,     80,  -55, YF_TOPRIG,   0, 0, 0, 0, 0, 0, 0, 0 },
   { "g"  ,  SHAPE_ELLIPSE ,  24,   8,   5, 150,-150,   CAT_K   ,   2,   3,     70,  -53, YF_TOPRIG,   0, 0, 0, 0, 0, 0, 0, 0 },
   /*-unde,  ---shape----- , -sx, -sy, rot, beg, arc,   -cat--- , -ra, -szr   --tx, --ty, -align-- ,  -----,-----clcuate----- */
   { "r"  ,  SHAPE_ELLIPSE ,  13,   6,   5, 185, 150,   CAT_R   ,   2,   2,     54,  -75, YF_BOTRIG,   0, 0, 0, 0, 0, 0, 0, 0 },
   { "l"  ,  SHAPE_ELLIPSE ,  24,   8,   5, 185, 150,   CAT_R   ,   2,   3,     45,  -78, YF_BOTRIG,   0, 0, 0, 0, 0, 0, 0, 0 },
   /*-comb,  ---shape----- , -sx, -sy, rot, beg, arc,   -cat--- , -ra, -szo   --tx, --ty, -align-- ,  -----,-----clcuate----- */
   { "rd" ,  SHAPE_ELLIPSE ,  13,   5,  10, 180, 220,   CAT_RD  ,   2,   2,      0,    0, YF_BOTRIG,   0, 0, 0, 0, 0, 0, 0, 0 },
   { "ld" ,  SHAPE_ELLIPSE ,  24,  10,  10, 180, 220,   CAT_RD  ,   2,   3,      0,    0, YF_BOTRIG,   0, 0, 0, 0, 0, 0, 0, 0 },
   /*-comb,  ---shape----- , -sx, -sy, rot, beg, arc,   -cat--- , -ra, -sz-   --tx, --ty, -align-- ,  -----,-----clcuate----- */
   /*> { "or" ,  SHAPE_ELLIPSE ,  12,   5,   5, 140, 220,   CAT_OR  ,   2,   2,    115,  -95, YF_BOTRIG,   0, 0, 0, 0, 0, 0, 0, 0 },   <*/
   /*> { "ol" ,  SHAPE_ELLIPSE ,  22,   9,   5, 140, 220,   CAT_OR  ,   2,   3,    100,  -95, YF_BOTRIG,   0, 0, 0, 0, 0, 0, 0, 0 },   <*/
   
   /*=====[[ 3rd ZONE ]]=============================   ====categories=====   =======display=======   ======================= */
   /*-line,  ---shape----- , -sx, -sy, rot, beg, arc,   -cat--- , -ra, -sz-   --tx, --ty, -align-- ,  -----,-----clcuate----- */
   { "ng" ,  SHAPE_LINE    ,   0,   0, -30,   0,  15,   CAT_NG  ,   3,   1,     20,  -95, YF_BOTRIG,   0, 0, 0, 0, 0, 0, 0, 0 },
   { "nk" ,  SHAPE_LINE    ,   0,   0, -30,   0,  30,   CAT_NG  ,   3,   2,     20,  -85, YF_BOTRIG,   0, 0, 0, 0, 0, 0, 0, 0 },

   /*=====[[ 4th ZONE ]]=============================   ====categories=====   =======display=======   ======================= */
   /*-line,  ---shape----- , -sx, -sy, rot, beg, arc,   -cat--- , -ra, -sz-   --tx, --ty, -align-- ,  -----,-----clcuate----- */
   { "sh" ,  SHAPE_LINE    ,   0,   0,-110,   0,  15,   CAT_CH  ,   4,   1,     15, -115, YF_BOTCEN,   0, 0, 0, 0, 0, 0, 0, 0 },
   { "ch" ,  SHAPE_LINE    ,   0,   0,-110,   0,  30,   CAT_CH  ,   4,   2,     25, -115, YF_BOTCEN,   0, 0, 0, 0, 0, 0, 0, 0 },
   { "j"  ,  SHAPE_LINE    ,   0,   0,-110,   0,  45,   CAT_CH  ,   4,   3,     35, -115, YF_BOTCEN,   0, 0, 0, 0, 0, 0, 0, 0 },
   /*-left,  ---shape----- , -sx, -sy, rot, beg, arc,   -cat--- , -ra, -sz-   --tx, --ty, -align-- ,  -----,-----clcuate----- */
   { "s"  ,  SHAPE_ELLIPSE ,   4,   3,  55,   5,-165,   CAT_F   ,   4,   1,     87, -115, YF_BOTRIG,   0, 0, 0, 0, 0, 0, 0, 0 },
   { "f"  ,  SHAPE_ELLIPSE ,  15,   6,  55,   0,-130,   CAT_F   ,   4,   2,     93, -107, YF_BOTRIG,   0, 0, 0, 0, 0, 0, 0, 0 },
   { "v"  ,  SHAPE_ELLIPSE ,  26,  10,  55,   0,-130,   CAT_F   ,   4,   3,    100, -100, YF_BOTRIG,   0, 0, 0, 0, 0, 0, 0, 0 },
   /*-righ,  ---shape----- , -sx, -sy, rot, beg, arc,   -cat--- , -ra, -szt   --tx, --ty, -align-- ,  -----,-----clcuate----- */
   { "z"  ,  SHAPE_ELLIPSE ,   4,   3,  55,  20, 165,   CAT_P   ,   4,   1,     65, -118, YF_BOTLEF,   0, 0, 0, 0, 0, 0, 0, 0 },
   { "p"  ,  SHAPE_ELLIPSE ,  15,   6,  55,  50, 130,   CAT_P   ,   4,   2,     65, -108, YF_BOTLEF,   0, 0, 0, 0, 0, 0, 0, 0 },
   { "b"  ,  SHAPE_ELLIPSE ,  26,  10,  55,  50, 130,   CAT_P   ,   4,   3,     65, -100, YF_BOTLEF,   0, 0, 0, 0, 0, 0, 0, 0 },
   /*-comb,  ---shape----- , -sx, -sy, rot, beg, arc,   -cat--- , -ra, -szo   --tx, --ty, -align-- ,  -----,-----clcuate----- */
   { "pt" ,  SHAPE_ELLIPSE ,   8,   4,  50,  90, 230,   CAT_PT  ,   2,   1,      0,    0, 0        ,   0, 0, 0, 0, 0, 0, 0, 0 },
   { "bd" ,  SHAPE_ELLIPSE ,  14,   7,  50,  90, 230,   CAT_PT  ,   2,   2,      0,    0, 0        ,   0, 0, 0, 0, 0, 0, 0, 0 },

   /*=====[[ VOWELS ]]===============================   ====categories=====   =======display=======   ======================= */
   /*-cir-,  ---shape----- , -sx, -sy, rot, beg, arc,   -cat--- , -ra, -sz-   --tx, --ty, -align-- ,  -----,-----clcuate----- */
   { "a"  ,  SHAPE_CIRCLE  ,   6,   0,  90,   0,   0,   CAT_A   ,   0,   0,    120, -115, YF_BOTRIG,   0, 0, 0, 0, 0, 0, 0, 0 },
   { "e"  ,  SHAPE_CIRCLE  ,   3,   0,  90,   0,   0,   CAT_E   ,   0,   0,    122,  -85, YF_BOTRIG,   0, 0, 0, 0, 0, 0, 0, 0 },
   { "ea" ,  SHAPE_CIRCLE  ,   3,   0,  90,   0,   0,   CAT_A   ,   0,   0,      0,    0, 0        ,   0, 0, 0, 0, 0, 0, 0, 0 },
   { "i"  ,  SHAPE_CIRCLE  ,   3,   0,  90,   0,   0,   CAT_A   ,   0,   0,      0,    0, 0        ,   0, 0, 0, 0, 0, 0, 0, 0 },
   { "o"  ,  SHAPE_ELLIPSE ,   4,   4,   0, 190, 160,   CAT_O   ,   9,   1,     63,  -72, YF_BOTRIG,   0, 0, 0, 0, 0, 0, 0, 0 },
   { "u"  ,  SHAPE_ELLIPSE ,   4,   4,   0, 170,-160,   CAT_U   ,   2,   2,     90,  -59, YF_TOPRIG,   0, 0, 0, 0, 0, 0, 0, 0 },
  
   /*=====[[ SPECIAL ]]==============================   ====categories=====   =======display=======   ======================= */
   /*-poin,  ---shape----- , -sx, -sy, rot, beg, arc,   -cat--- , -ra, -sz-   --tx, --ty, -align-- ,  -----,-----clcuate----- */
   { "´"  ,  SHAPE_DOT     ,   1,   1,   0,   0, 360,   CAT_NONE,   0,   0,      0,    0, 0        ,   0, 0, 0, 0, 0, 0, 0, 0 },
   { "dot",  SHAPE_DOT     ,   1,   1,   0,   0, 360,   CAT_NONE,   0,   0,    119,  -75, YF_BOTCEN,   0, 0, 0, 0, 0, 0, 0, 0 },
   { ">"  ,  SHAPE_SPACE   ,   3,   3,   0,   0, 360,   CAT_NONE,   0,   0,      0,    0, 0        ,   0, 0, 0, 0, 0, 0, 0, 0 },
   { "."  ,  SHAPE_LINE    ,   7,   5,   0,   0,  10,   CAT_NONE,   8,   1,      0,    0, 0        ,   0, 0, 0, 0, 0, 0, 0, 0 },
   { ".." ,  SHAPE_LINE    ,   7,   5,   0,   0,  20,   CAT_NONE,   8,   2,      0,    0, 0        ,   0, 0, 0, 0, 0, 0, 0, 0 },
   { "con",  SHAPE_DOT     ,   2,   2,   0,   0, 360,   CAT_NONE,   0,   0,      0,    0, 0        ,   0, 0, 0, 0, 0, 0, 0, 0 },
   { "-"  ,  SHAPE_SPACE   ,   2,   2,   5,  -5, 360,   CAT_NONE,   0,   0,      0,    0, 0        ,   0, 0, 0, 0, 0, 0, 0, 0 },

   /*=====[[ SHOW-ONLY ]]============================   ====categories=====   =======display=======   ======================= */
   /*-poin,  ---shape----- , -sx, -sy, rot, beg, arc,   -cat--- , -ra, -sz-   --tx, --ty, -align-- ,  -----,-----clcuate----- */
   { "a+" ,  SHAPE_TEARDROP,   5,   5,  50,   0,   0,   CAT_A   ,   0,   0,    108, -135, YF_BOTRIG,   0, 0, 0, 0, 0, 0, 0, 0 },
   { "e+" ,  SHAPE_ELLIPSE ,   2,   2, 110,   0, 360,   CAT_E   ,   0,   0,    115,  -98, YF_BOTRIG,   0, 0, 0, 0, 0, 0, 0, 0 },

   /*=====[[ header ]]===============================   ====categories=====   =======display=======   ======================= */
   /*-line,  ---shape----- , -sx, -sy, rot, beg, arc,   -cat--- , -ra, -sz-   --tx, --ty, -align-- ,  -----,-----clcuate----- */
   { "END",  SHAPE_NONE    ,   0,   0,   0,   0,   0,   CAT_NONE,   0,   0,      0,    0, 0        ,   0, 0, 0, 0, 0, 0, 0, 0 },
  
   /*=====[[ VOWELS FIXES ]]=========================   ====categories=====   =======display=======   ======================= */
   /*-solo,  ---shape----- , -sx, -sy, rot, beg, arc,   -cat--- , -ra, -sz-   --tx, --ty, -align-- ,  -----,-----clcuate----- */
   { "aT" ,  SHAPE_TEARDROP,   8,   8,  50,   0,   0,   CAT_A   ,   0,   0,      0,    0, 0        ,   0, 0, 0, 0, 0, 0, 0, 0 },
   { "aH" ,  SHAPE_TEARDROP,   8,   8, 230,   0,   0,   CAT_A   ,   0,   0,      0,    0, 0        ,   0, 0, 0, 0, 0, 0, 0, 0 },
   { "ap" ,  SHAPE_TEARDROP,  12,   7, -20,   0,   0,   CAT_A   ,   0,   0,      0,    0, 0        ,   0, 0, 0, 0, 0, 0, 0, 0 },
   { "ek" ,  SHAPE_ELLIPSE ,   4,   4, 110,   0, 360,   CAT_E   ,   0,   0,      0,    0, 0        ,   0, 0, 0, 0, 0, 0, 0, 0 },
   { "ap" ,  SHAPE_ELLIPSE ,   8,   8, 125,   0, 360,   CAT_A   ,   0,   0,      0,    0, 0        ,   0, 0, 0, 0, 0, 0, 0, 0 },
   { "af" ,  SHAPE_ELLIPSE ,   8,   8,  20,   0, 360,   CAT_A   ,   0,   0,      0,    0, 0        ,   0, 0, 0, 0, 0, 0, 0, 0 },

   /*=====[[ M FIXES ]]==============================   ====categories=====   =======display=======   ======================= */
   /*-ends,  ---shape----- , -sx, -sy, rot, beg, arc,   -cat--- , -ra, -sz-   --tx, --ty, -align-- ,  -----,-----clcuate----- */
   { "amx",  SHAPE_TEARDROP,   8,  12,  70,   0,   0,   CAT_A   ,   0,   0,      0,    0, 0        ,   0, 0, 0, 0, 0, 0, 0, 0 },
   { "axm",  SHAPE_TEARDROP,   8,  12, 110,   0,   0,   CAT_A   ,   0,   0,      0,    0, 0        ,   0, 0, 0, 0, 0, 0, 0, 0 },
   /*-m-m-,  ---shape----- , -sx, -sy, rot, beg, arc,   -cat--- , -ra, -sz-   --tx, --ty, -align-- ,  -----,-----clcuate----- */
   { "amm",  SHAPE_TEARDROP,   8,  12,  90,   0,   0,   CAT_A   ,   0,   0,      0,    0, 0        ,   0, 0, 0, 0, 0, 0, 0, 0 },
   /*-m-d-,  ---shape----- , -sx, -sy, rot, beg, arc,   -cat--- , -ra, -sz-   --tx, --ty, -align-- ,  -----,-----clcuate----- */
   { "amd",  SHAPE_TEARDROP,   8,  10,  70,   0,   0,   CAT_A   ,   0,   0,      0,    0, 0        ,   0, 0, 0, 0, 0, 0, 0, 0 },
   { "adm",  SHAPE_TEARDROP,   8,  10, 250,   0,   0,   CAT_A   ,   0,   0,      0,    0, 0        ,   0, 0, 0, 0, 0, 0, 0, 0 },
   /*-m-T-,  ---shape----- , -sx, -sy, rot, beg, arc,   -cat--- , -ra, -sz-   --tx, --ty, -align-- ,  -----,-----clcuate----- */
   { "amT",  SHAPE_TEARDROP,   9,   8,  55,   0,   0,   CAT_A   ,   0,   0,      0,    0, 0        ,   0, 0, 0, 0, 0, 0, 0, 0 },
   /*-m-H-,  ---shape----- , -sx, -sy, rot, beg, arc,   -cat--- , -ra, -sz-   --tx, --ty, -align-- ,  -----,-----clcuate----- */
   { "amH",  SHAPE_TEARDROP,   8,  12, 270,   0,   0,   CAT_A   ,   0,   0,      0,    0, 0        ,   0, 0, 0, 0, 0, 0, 0, 0 },
   { "aHm",  SHAPE_TEARDROP,   8,   9, 240,   0,   0,   CAT_A   ,   0,   0,      0,    0, 0        ,   0, 0, 0, 0, 0, 0, 0, 0 },
   /*-m-D-,  ---shape----- , -sx, -sy, rot, beg, arc,   -cat--- , -ra, -sz-   --tx, --ty, -align-- ,  -----,-----clcuate----- */
   { "aDm",  SHAPE_TEARDROP,  10,   5, 140,   0,   0,   CAT_A   ,   0,   0,      0,    0, 0        ,   0, 0, 0, 0, 0, 0, 0, 0 },
   /*-m-k-,  ---shape----- , -sx, -sy, rot, beg, arc,   -cat--- , -ra, -sz-   --tx, --ty, -align-- ,  -----,-----clcuate----- */
   { "amk",  SHAPE_TEARDROP,  10,  10,  70,   0,   0,   CAT_A   ,   0,   0,      0,    0, 0        ,   0, 0, 0, 0, 0, 0, 0, 0 },
   { "akm",  SHAPE_TEARDROP,  10,  10, 110,   0,   0,   CAT_A   ,   0,   0,      0,    0, 0        ,   0, 0, 0, 0, 0, 0, 0, 0 },
   /*-m-r-,  ---shape----- , -sx, -sy, rot, beg, arc,   -cat--- , -ra, -sz-   --tx, --ty, -align-- ,  -----,-----clcuate----- */
   { "amr",  SHAPE_TEARDROP,  10,  10, 290,   0,   0,   CAT_A   ,   0,   0,      0,    0, 0        ,   0, 0, 0, 0, 0, 0, 0, 0 },
   { "arm",  SHAPE_TEARDROP,  10,  10, 250,   0,   0,   CAT_A   ,   0,   0,      0,    0, 0        ,   0, 0, 0, 0, 0, 0, 0, 0 },
   /*-m-rd,  ---shape----- , -sx, -sy, rot, beg, arc,   -cat--- , -ra, -sz-   --tx, --ty, -align-- ,  -----,-----clcuate----- */
   { "aRm",  SHAPE_TEARDROP,   9,   5, 190,   0,   0,   CAT_A   ,   0,   0,      0,    0, 0        ,   0, 0, 0, 0, 0, 0, 0, 0 },
   /*-m-ng,  ---shape----- , -sx, -sy, rot, beg, arc,   -cat--- , -ra, -sz-   --tx, --ty, -align-- ,  -----,-----clcuate----- */
   { "amN",  SHAPE_TEARDROP,  10,  10, 270,   0,   0,   CAT_A   ,   0,   0,      0,    0, 0        ,   0, 0, 0, 0, 0, 0, 0, 0 },
   { "aNm",  SHAPE_TEARDROP,   8,  12, 100,   0,   0,   CAT_A   ,   0,   0,      0,    0, 0        ,   0, 0, 0, 0, 0, 0, 0, 0 },
   /*-m-j-,  ---shape----- , -sx, -sy, rot, beg, arc,   -cat--- , -ra, -sz-   --tx, --ty, -align-- ,  -----,-----clcuate----- */
   { "amj",  SHAPE_TEARDROP,  12,   7, 330,   0,  -1,   CAT_A   ,   0,   0,      0,    0, 0        ,   0, 0, 0, 0, 0, 0, 0, 0 },
   { "ajm",  SHAPE_TEARDROP,  12,   7, 150,   0,   0,   CAT_A   ,   0,   0,      0,    0, 0        ,   0, 0, 0, 0, 0, 0, 0, 0 },
   /*-m-f-,  ---shape----- , -sx, -sy, rot, beg, arc,   -cat--- , -ra, -sz-   --tx, --ty, -align-- ,  -----,-----clcuate----- */
   { "amf",  SHAPE_TEARDROP,  10,   8, 300,   0,   0,   CAT_A   ,   0,   0,      0,    0, 0        ,   0, 0, 0, 0, 0, 0, 0, 0 },
   { "afm",  SHAPE_TEARDROP,  12,   6, 160,   0,   0,   CAT_A   ,   0,   0,      0,    0, 0        ,   0, 0, 0, 0, 0, 0, 0, 0 },
   /*-m-m-,  ---shape----- , -sx, -sy, rot, beg, arc,   -cat--- , -ra, -sz-   --tx, --ty, -align-- ,  -----,-----clcuate----- */
   { "amp",  SHAPE_TEARDROP,  12,   6, 340,   0,   0,   CAT_A   ,   0,   0,      0,    0, 0        ,   0, 0, 0, 0, 0, 0, 0, 0 },
   { "apm",  SHAPE_TEARDROP,  10,   8, 140,   0,   0,   CAT_A   ,   0,   0,      0,    0, 0        ,   0, 0, 0, 0, 0, 0, 0, 0 },

   /*=====[[ D FIXES ]]==============================   ====categories=====   =======display=======   ======================= */
   /*-ends,  ---shape----- , -sx, -sy, rot, beg, arc, -cat--- ,   -ra, -sz-   --tx, --ty, -align-- ,  -----,-----clcuate----- */
   { "adx",  SHAPE_TEARDROP,   8,  12,  45,   0,   0, CAT_A   ,     0,   0,      0,    0, 0        ,   0, 0, 0, 0, 0, 0, 0, 0 },
   { "axd",  SHAPE_TEARDROP,   8,  12,  70,   0,   0, CAT_A   ,     0,   0,      0,    0, 0        ,   0, 0, 0, 0, 0, 0, 0, 0 },
   /*-d-d-,  ---shape----- , -sx, -sy, rot, beg, arc, -cat--- ,   -ra, -sz-   --tx, --ty, -align-- ,  -----,-----clcuate----- */
   { "add",  SHAPE_TEARDROP,   8,  12,  45,   0,   0, CAT_A   ,     0,   0,      0,    0, 0        ,   0, 0, 0, 0, 0, 0, 0, 0 },
   /*-d-H-,  ---shape----- , -sx, -sy, rot, beg, arc, -cat--- ,   -ra, -sz-   --tx, --ty, -align-- ,  -----,-----clcuate----- */
   { "aHd",  SHAPE_TEARDROP,   8,  10, 200,   0,   0, CAT_A   ,     0,   0,      0,    0, 0        ,   0, 0, 0, 0, 0, 0, 0, 0 },
   /*-d-k-,  ---shape----- , -sx, -sy, rot, beg, arc, -cat--- ,   -ra, -sz-   --tx, --ty, -align-- ,  -----,-----clcuate----- */
   { "adk",  SHAPE_TEARDROP,   8,  12,  55,   0,   0, CAT_A   ,     0,   0,      0,    0, 0        ,   0, 0, 0, 0, 0, 0, 0, 0 },
   { "akd",  SHAPE_TEARDROP,   9,   8,  90,   0,   0, CAT_A   ,     0,   0,      0,    0, 0        ,   0, 0, 0, 0, 0, 0, 0, 0 },
   /*-d-r-,  ---shape----- , -sx, -sy, rot, beg, arc, -cat--- ,   -ra, -sz-   --tx, --ty, -align-- ,  -----,-----clcuate----- */
   { "adr",  SHAPE_TEARDROP,   8,   8, 270,   0,   0, CAT_A   ,     0,   0,      0,    0, 0        ,   0, 0, 0, 0, 0, 0, 0, 0 },
   /*-d-N-,  ---shape----- , -sx, -sy, rot, beg, arc, -cat--- ,   -ra, -sz-   --tx, --ty, -align-- ,  -----,-----clcuate----- */
   { "adN",  SHAPE_TEARDROP,   8,   8, 280,   0,   0, CAT_A   ,     0,   0,      0,    0, 0        ,   0, 0, 0, 0, 0, 0, 0, 0 },
   { "aNd",  SHAPE_TEARDROP,   8,   8,  90,   0,   0, CAT_A   ,     0,   0,      0,    0, 0        ,   0, 0, 0, 0, 0, 0, 0, 0 },
   /*-d-j-,  ---shape----- , -sx, -sy, rot, beg, arc, -cat--- ,   -ra, -sz-   --tx, --ty, -align-- ,  -----,-----clcuate----- */
   { "adj",  SHAPE_TEARDROP,  12,   6, 300,   0,   0, CAT_A   ,     0,   0,      0,    0, 0        ,   0, 0, 0, 0, 0, 0, 0, 0 },
   { "ajd",  SHAPE_TEARDROP,  10,   4, 120,   0,   0, CAT_A   ,     0,   0,      0,    0, 0        ,   0, 0, 0, 0, 0, 0, 0, 0 },
   /*-d-f-,  ---shape----- , -sx, -sy, rot, beg, arc, -cat--- ,   -ra, -sz-   --tx, --ty, -align-- ,  -----,-----clcuate----- */
   { "adf",  SHAPE_TEARDROP,  12,   6, 300,   0,   0, CAT_A   ,     0,   0,      0,    0, 0        ,   0, 0, 0, 0, 0, 0, 0, 0 },
   { "afd",  SHAPE_TEARDROP,  12,   6, 110,   0,   0, CAT_A   ,     0,   0,      0,    0, 0        ,   0, 0, 0, 0, 0, 0, 0, 0 },
   /*-d-p-,  ---shape----- , -sx, -sy, rot, beg, arc, -cat--- ,   -ra, -sz-   --tx, --ty, -align-- ,  -----,-----clcuate----- */
   { "adp",  SHAPE_TEARDROP,  12,   6, 300,   0,   0, CAT_A   ,     0,   0,      0,    0, 0        ,   0, 0, 0, 0, 0, 0, 0, 0 },
   { "apd",  SHAPE_TEARDROP,  12,   6, 110,   0,   0, CAT_A   ,     0,   0,      0,    0, 0        ,   0, 0, 0, 0, 0, 0, 0, 0 },

   /*=====[[ K FIXES ]]==============================   ====categories=====   =======display=======   ======================= */
   /*-ends,  ---shape----- , -sx, -sy, rot, beg, arc,   -cat--- , -ra, -sz-   --tx, --ty, -align-- ,  -----,-----clcuate----- */
   { "axk",  SHAPE_TEARDROP,  10,  10, 105,   0,   0,   CAT_A   ,   0,   0,      0,    0, 0        ,   0, 0, 0, 0, 0, 0, 0, 0 },
   { "akx",  SHAPE_TEARDROP,  10,  10,  80,   0,   0,   CAT_A   ,   0,   0,      0,    0, 0        ,   0, 0, 0, 0, 0, 0, 0, 0 },
   /*-k-k-,  ---shape----- , -sx, -sy, rot, beg, arc,   -cat--- , -ra, -sz-   --tx, --ty, -align-- ,  -----,-----clcuate----- */
   { "akk",  SHAPE_TEARDROP,  10,  10,  90,   0,   0,   CAT_A   ,   0,   0,      0,    0, 0        ,   0, 0, 0, 0, 0, 0, 0, 0 },
   /*-k-r-,  ---shape----- , -sx, -sy, rot, beg, arc,   -cat--- , -ra, -sz-   --tx, --ty, -align-- ,  -----,-----clcuate----- */
   { "akr",  SHAPE_TEARDROP,   8,  12, 290,   0,   0,   CAT_A   ,   0,   0,      0,    0, 0        ,   0, 0, 0, 0, 0, 0, 0, 0 },
   { "ark",  SHAPE_TEARDROP,   8,  12,  70,   0,   0,   CAT_A   ,   0,   0,      0,    0, 0        ,   0, 0, 0, 0, 0, 0, 0, 0 },
   /*-k-T-,  ---shape----- , -sx, -sy, rot, beg, arc,   -cat--- , -ra, -sz-   --tx, --ty, -align-- ,  -----,-----clcuate----- */
   { "akT",  SHAPE_TEARDROP,   9,   8,  65,   0,   0,   CAT_A   ,   0,   0,      0,    0, 0        ,   0, 0, 0, 0, 0, 0, 0, 0 },
   { "aTk",  SHAPE_TEARDROP,   8,  12,  90,   0,   0,   CAT_A   ,   0,   0,      0,    0, 0        ,   0, 0, 0, 0, 0, 0, 0, 0 },
   /*-k-H-,  ---shape----- , -sx, -sy, rot, beg, arc,   -cat--- , -ra, -sz-   --tx, --ty, -align-- ,  -----,-----clcuate----- */
   { "akH",  SHAPE_TEARDROP,  10,  10, 110,   0,   0,   CAT_A   ,   0,   0,      0,    0, 0        ,   0, 0, 0, 0, 0, 0, 0, 0 },
   { "aHk",  SHAPE_TEARDROP,   8,  12, 210,   0,   0,   CAT_A   ,   0,   0,      0,    0, 0        ,   0, 0, 0, 0, 0, 0, 0, 0 },
   /*-k-N-,  ---shape----- , -sx, -sy, rot, beg, arc,   -cat--- , -ra, -sz-   --tx, --ty, -align-- ,  -----,-----clcuate----- */
   { "akN",  SHAPE_TEARDROP,  10,  10, 130,   0,   0,   CAT_A   ,   0,   0,      0,    0, 0        ,   0, 0, 0, 0, 0, 0, 0, 0 },
   { "aNk",  SHAPE_TEARDROP,   8,  12, 100,   0,   0,   CAT_A   ,   0,   0,      0,    0, 0        ,   0, 0, 0, 0, 0, 0, 0, 0 },
   /*-k-j-,  ---shape----- , -sx, -sy, rot, beg, arc,   -cat--- , -ra, -sz-   --tx, --ty, -align-- ,  -----,-----clcuate----- */
   { "akj",  SHAPE_TEARDROP,  10,  10, 330,   0,   0,   CAT_A   ,   0,   0,      0,    0, 0        ,   0, 0, 0, 0, 0, 0, 0, 0 },
   { "ajk",  SHAPE_TEARDROP,  10,   6, 130,   0,   0,   CAT_A   ,   0,   0,      0,    0, 0        ,   0, 0, 0, 0, 0, 0, 0, 0 },
   /*-k-f-,  ---shape----- , -sx, -sy, rot, beg, arc,   -cat--- , -ra, -sz-   --tx, --ty, -align-- ,  -----,-----clcuate----- */
   { "akf",  SHAPE_TEARDROP,  10,  12, 320,   0,   0,   CAT_A   ,   0,   0,      0,    0, 0        ,   0, 0, 0, 0, 0, 0, 0, 0 },
   { "afk",  SHAPE_TEARDROP,  12,   6, 130,   0,   0,   CAT_A   ,   0,   0,      0,    0, 0        ,   0, 0, 0, 0, 0, 0, 0, 0 },
   /*-k-p-,  ---shape----- , -sx, -sy, rot, beg, arc,   -cat--- , -ra, -sz-   --tx, --ty, -align-- ,  -----,-----clcuate----- */
   { "akp",  SHAPE_TEARDROP,  10,   8, 350,   0,   0,   CAT_A   ,   0,   0,      0,    0, 0        ,   0, 0, 0, 0, 0, 0, 0, 0 },
   { "apk",  SHAPE_TEARDROP,  12,   6, 130,   0,   0,   CAT_A   ,   0,   0,      0,    0, 0        ,   0, 0, 0, 0, 0, 0, 0, 0 },

   /*=====[[ R FIXES ]]==============================   ====categories=====   =======display=======   ======================= */
   /*-ends,  ---shape----- , -sx, -sy, rot, beg, arc,   -cat--- , -ra, -sz-   --tx, --ty, -align-- ,  -----,-----clcuate----- */
   { "axr",  SHAPE_TEARDROP,  10,  10, 270,   0,   0,   CAT_A   ,   0,   0,      0,    0, 0        ,   0, 0, 0, 0, 0, 0, 0, 0 },
   { "arx",  SHAPE_TEARDROP,  10,  10, 270,   0,   0,   CAT_A   ,   0,   0,      0,    0, 0        ,   0, 0, 0, 0, 0, 0, 0, 0 },
   /*-r-r-,  ---shape----- , -sx, -sy, rot, beg, arc,   -cat--- , -ra, -sz-   --tx, --ty, -align-- ,  -----,-----clcuate----- */
   { "arr",  SHAPE_TEARDROP,  10,  10, 270,   0,   0,   CAT_A   ,   0,   0,      0,    0, 0        ,   0, 0, 0, 0, 0, 0, 0, 0 },
   /*-r-T-,  ---shape----- , -sx, -sy, rot, beg, arc,   -cat--- , -ra, -sz-   --tx, --ty, -align-- ,  -----,-----clcuate----- */
   { "arT",  SHAPE_TEARDROP,  10,  10,  45,   0,   0,   CAT_A   ,   0,   0,      0,    0, 0        ,   0, 0, 0, 0, 0, 0, 0, 0 },
   { "aTr",  SHAPE_TEARDROP,  10,  10, 290,   0,   0,   CAT_A   ,   0,   0,      0,    0, 0        ,   0, 0, 0, 0, 0, 0, 0, 0 },
   /*-r-H-,  ---shape----- , -sx, -sy, rot, beg, arc,   -cat--- , -ra, -sz-   --tx, --ty, -align-- ,  -----,-----clcuate----- */
   { "arH",  SHAPE_TEARDROP,  10,  12, 250,   0,   0,   CAT_A   ,   0,   0,      0,    0, 0        ,   0, 0, 0, 0, 0, 0, 0, 0 },
   { "aHr",  SHAPE_TEARDROP,  10,  10, 250,   0,   0,   CAT_A   ,   0,   0,      0,    0, 0        ,   0, 0, 0, 0, 0, 0, 0, 0 },
   /*-r-N-,  ---shape----- , -sx, -sy, rot, beg, arc,   -cat--- , -ra, -sz-   --tx, --ty, -align-- ,  -----,-----clcuate----- */
   { "arN",  SHAPE_TEARDROP,  10,  12, 260,   0,   0,   CAT_A   ,   0,   0,      0,    0, 0        ,   0, 0, 0, 0, 0, 0, 0, 0 },
   { "aNr",  SHAPE_TEARDROP,  10,  10, 310,   0,   0,   CAT_A   ,   0,   0,      0,    0, 0        ,   0, 0, 0, 0, 0, 0, 0, 0 },
   /*-r-j-,  ---shape----- , -sx, -sy, rot, beg, arc,   -cat--- , -ra, -sz-   --tx, --ty, -align-- ,  -----,-----clcuate----- */
   { "arj",  SHAPE_TEARDROP,  10,   6, 300,   0,   0,   CAT_A   ,   0,   0,      0,    0, 0        ,   0, 0, 0, 0, 0, 0, 0, 0 },
   { "ajr",  SHAPE_TEARDROP,  10,  10, 170,   0,   0,   CAT_A   ,   0,   0,      0,    0, 0        ,   0, 0, 0, 0, 0, 0, 0, 0 },
   /*-r-f-,  ---shape----- , -sx, -sy, rot, beg, arc,   -cat--- , -ra, -sz-   --tx, --ty, -align-- ,  -----,-----clcuate----- */
   { "arf",  SHAPE_TEARDROP,  10,   6, 300,   0,   0,   CAT_A   ,   0,   0,      0,    0, 0        ,   0, 0, 0, 0, 0, 0, 0, 0 },
   { "afr",  SHAPE_TEARDROP,  10,  10, 180,   0,   0,   CAT_A   ,   0,   0,      0,    0, 0        ,   0, 0, 0, 0, 0, 0, 0, 0 },
   /*-r-p-,  ---shape----- , -sx, -sy, rot, beg, arc,   -cat--- , -ra, -sz-   --tx, --ty, -align-- ,  -----,-----clcuate----- */
   { "arp",  SHAPE_TEARDROP,  10,   6, 300,   0,   0,   CAT_A   ,   0,   0,      0,    0, 0        ,   0, 0, 0, 0, 0, 0, 0, 0 },
   { "apr",  SHAPE_TEARDROP,  10,  10, 150,   0,   0,   CAT_A   ,   0,   0,      0,    0, 0        ,   0, 0, 0, 0, 0, 0, 0, 0 },

   /*=====[[ RD FIXES ]]=============================   ====categories=====   =======display=======   ======================= */
   /*-ends,  ---shape----- , -sx, -sy, rot, beg, arc,   -cat--- , -ra, -sz-   --tx, --ty, -align-- ,  -----,-----clcuate----- */
   { "axR",  SHAPE_TEARDROP,  10,  10, 270,   0,   0,   CAT_A   ,   0,   0,      0,    0, 0        ,   0, 0, 0, 0, 0, 0, 0, 0 },
   { "aRx",  SHAPE_TEARDROP,  10,  10, 270,   0,   0,   CAT_A   ,   0,   0,      0,    0, 0        ,   0, 0, 0, 0, 0, 0, 0, 0 },

   /*=====[[ F FIXES ]]==============================   ====categories=====   =======display=======   ======================= */
   /*-ends,  ---shape----- , -sx, -sy, rot, beg, arc,   -cat--- , -ra, -sz-   --tx, --ty, -align-- ,  -----,-----clcuate----- */
   { "axf",  SHAPE_TEARDROP,  10,  10, 210,   0,   0,   CAT_A   ,   0,   0,      0,    0, 0        ,   0, 0, 0, 0, 0, 0, 0, 0 },
   { "afx",  SHAPE_TEARDROP,  10,  10, 170,   0,   0,   CAT_A   ,   0,   0,      0,    0, 0        ,   0, 0, 0, 0, 0, 0, 0, 0 },
   /*-f-f-,  ---shape----- , -sx, -sy, rot, beg, arc,   -cat--- , -ra, -sz-   --tx, --ty, -align-- ,  -----,-----clcuate----- */
   { "aff",  SHAPE_TEARDROP,   8,  12, 190,   0,   0,   CAT_A   ,   0,   0,      0,    0, 0        ,   0, 0, 0, 0, 0, 0, 0, 0 },
   /*-f-T-,  ---shape----- , -sx, -sy, rot, beg, arc,   -cat--- , -ra, -sz-   --tx, --ty, -align-- ,  -----,-----clcuate----- */
   { "aTf",  SHAPE_TEARDROP,  10,  10, 300,   0,   0,   CAT_A   ,   0,   0,      0,    0, 0        ,   0, 0, 0, 0, 0, 0, 0, 0 },
   { "afT",  SHAPE_TEARDROP,  12,   6, 110,   0,   0,   CAT_A   ,   0,   0,      0,    0, 0        ,   0, 0, 0, 0, 0, 0, 0, 0 },
   /*-f-H-,  ---shape----- , -sx, -sy, rot, beg, arc,   -cat--- , -ra, -sz-   --tx, --ty, -align-- ,  -----,-----clcuate----- */
   { "aHf",  SHAPE_TEARDROP,  10,   8, 260,   0,   0,   CAT_A   ,   0,   0,      0,    0, 0        ,   0, 0, 0, 0, 0, 0, 0, 0 },
   { "afH",  SHAPE_TEARDROP,  12,   6, 160,   0,   0,   CAT_A   ,   0,   0,      0,    0, 0        ,   0, 0, 0, 0, 0, 0, 0, 0 },
   /*-f-N-,  ---shape----- , -sx, -sy, rot, beg, arc,   -cat--- , -ra, -sz-   --tx, --ty, -align-- ,  -----,-----clcuate----- */
   { "aNf",  SHAPE_TEARDROP,  10,  10, 300,   0,   0,   CAT_A   ,   0,   0,      0,    0, 0        ,   0, 0, 0, 0, 0, 0, 0, 0 },
   { "afN",  SHAPE_TEARDROP,  12,   6, 160,   0,   0,   CAT_A   ,   0,   0,      0,    0, 0        ,   0, 0, 0, 0, 0, 0, 0, 0 },
   /*-f-j-,  ---shape----- , -sx, -sy, rot, beg, arc,   -cat--- , -ra, -sz-   --tx, --ty, -align-- ,  -----,-----clcuate----- */
   { "ajf",  SHAPE_TEARDROP,  10,  10, 180,   0,   0,   CAT_A   ,   0,   0,      0,    0, 0        ,   0, 0, 0, 0, 0, 0, 0, 0 },
   { "afj",  SHAPE_TEARDROP,  10,  10, 210,   0,   0,   CAT_A   ,   0,   0,      0,    0, 0        ,   0, 0, 0, 0, 0, 0, 0, 0 },
   /*-f-p-,  ---shape----- , -sx, -sy, rot, beg, arc,   -cat--- , -ra, -sz-   --tx, --ty, -align-- ,  -----,-----clcuate----- */
   { "apf",  SHAPE_TEARDROP,  10,  12, 160,   0,   0,   CAT_A   ,   0,   0,      0,    0, 0        ,   0, 0, 0, 0, 0, 0, 0, 0 },
   { "afp",  SHAPE_TEARDROP,  10,  12, 220,   0,   0,   CAT_A   ,   0,   0,      0,    0, 0        ,   0, 0, 0, 0, 0, 0, 0, 0 },

   /*=====[[ P FIXES ]]==============================   ====categories=====   =======display=======   ======================= */
   /*-ends,  ---shape----- , -sx, -sy, rot, beg, arc,   -cat--- , -ra, -sz-   --tx, --ty, -align-- ,  -----,-----clcuate----- */
   { "axp",  SHAPE_TEARDROP,   9,   8,  15,   0,  -1,   CAT_A   ,   0,   0,      0,    0, 0        ,   0, 0, 0, 0, 0, 0, 0, 0 },
   { "apx",  SHAPE_TEARDROP,  10,  10,   0,   0,   0,   CAT_A   ,   0,   0,      0,    0, 0        ,   0, 0, 0, 0, 0, 0, 0, 0 },
   /*-p-r-,  ---shape----- , -sx, -sy, rot, beg, arc,   -cat--- , -ra, -sz-   --tx, --ty, -align-- ,  -----,-----clcuate----- */
   { "app",  SHAPE_TEARDROP,  10,  10,  10,   0,   0,   CAT_A   ,   0,   0,      0,    0, 0        ,   0, 0, 0, 0, 0, 0, 0, 0 },
   /*-p-T-,  ---shape----- , -sx, -sy, rot, beg, arc,   -cat--- , -ra, -sz-   --tx, --ty, -align-- ,  -----,-----clcuate----- */
   { "aTp",  SHAPE_TEARDROP,  12,   6, 300,   0,   0,   CAT_A   ,   0,   0,      0,    0, 0        ,   0, 0, 0, 0, 0, 0, 0, 0 },
   { "apT",  SHAPE_TEARDROP,  12,   6, 110,   0,   0,   CAT_A   ,   0,   0,      0,    0, 0        ,   0, 0, 0, 0, 0, 0, 0, 0 },
   /*-p-H-,  ---shape----- , -sx, -sy, rot, beg, arc,   -cat--- , -ra, -sz-   --tx, --ty, -align-- ,  -----,-----clcuate----- */
   { "aHp",  SHAPE_TEARDROP,  10,   8, 280,   0,   0,   CAT_A   ,   0,   0,      0,    0, 0        ,   0, 0, 0, 0, 0, 0, 0, 0 },
   { "apH",  SHAPE_TEARDROP,  10,  10, 160,   0,   0,   CAT_A   ,   0,   0,      0,    0, 0        ,   0, 0, 0, 0, 0, 0, 0, 0 },
   /*-p-N-,  ---shape----- , -sx, -sy, rot, beg, arc,   -cat--- , -ra, -sz-   --tx, --ty, -align-- ,  -----,-----clcuate----- */
   { "aNp",  SHAPE_TEARDROP,  10,   8, 350,   0,   0,   CAT_A   ,   0,   0,      0,    0, 0        ,   0, 0, 0, 0, 0, 0, 0, 0 },
   { "apN",  SHAPE_TEARDROP,  10,  10, 160,   0,   0,   CAT_A   ,   0,   0,      0,    0, 0        ,   0, 0, 0, 0, 0, 0, 0, 0 },
   /*-p-j-,  ---shape----- , -sx, -sy, rot, beg, arc,   -cat--- , -ra, -sz-   --tx, --ty, -align-- ,  -----,-----clcuate----- */
   { "ajp",  SHAPE_TEARDROP,  10,  12,  20,   0,   0,   CAT_A   ,   0,   0,      0,    0, 0        ,   0, 0, 0, 0, 0, 0, 0, 0 },
   { "apj",  SHAPE_TEARDROP,  10,  12,   0,   0,   0,   CAT_A   ,   0,   0,      0,    0, 0        ,   0, 0, 0, 0, 0, 0, 0, 0 },

   /*=====[[ OTH FIXES ]]============================   ====categories=====   =======display=======   ======================= */
   /*-ends,  ---shape----- , -sx, -sy, rot, beg, arc,   -cat--- , -ra, -sz-   --tx, --ty, -align-- ,  -----,-----clcuate----- */
   { "axT",  SHAPE_TEARDROP,  10,  10,  70,   0,   0,   CAT_A   ,   0,   0,      0,    0, 0        ,   0, 0, 0, 0, 0, 0, 0, 0 },
   { "aTx",  SHAPE_TEARDROP,  10,  10,  30,   0,   0,   CAT_A   ,   0,   0,      0,    0, 0        ,   0, 0, 0, 0, 0, 0, 0, 0 },
   /*-T-T-,  ---shape----- , -sx, -sy, rot, beg, arc,   -cat--- , -ra, -sz-   --tx, --ty, -align-- ,  -----,-----clcuate----- */
   { "aTT",  SHAPE_TEARDROP,  10,  10,  45,   0,   0,   CAT_A   ,   0,   0,      0,    0, 0        ,   0, 0, 0, 0, 0, 0, 0, 0 },
  
   /*=====[[ UTH FIXES ]]============================   ====categories=====   =======display=======   ======================= */
   /*-ends,  ---shape----- , -sx, -sy, rot, beg, arc,   -cat--- , -ra, -sz-   --tx, --ty, -align-- ,  -----,-----clcuate----- */
   { "axH",  SHAPE_TEARDROP,  10,  10, 210,   0,   0,   CAT_A   ,   0,   0,      0,    0, 0        ,   0, 0, 0, 0, 0, 0, 0, 0 },
   { "aHx",  SHAPE_TEARDROP,  10,  10, 330,   0,   0,   CAT_A   ,   0,   0,      0,    0, 0        ,   0, 0, 0, 0, 0, 0, 0, 0 },
   /*-T-T-,  ---shape----- , -sx, -sy, rot, beg, arc,   -cat--- , -ra, -sz-   --tx, --ty, -align-- ,  -----,-----clcuate----- */
   { "aHH",  SHAPE_TEARDROP,  10,  10, 225,   0,   0,   CAT_A   ,   0,   0,      0,    0, 0        ,   0, 0, 0, 0, 0, 0, 0, 0 },



   { "ard",  SHAPE_TEARDROP,   8,  12, 235,   0,   0,   CAT_A   ,   0,   0,    0,    0, 0        , 0, 0, 0, 0, 0, 0, 0, 0 },

   /*=====[[ 2nd ZONE FIXES ]]=======================   ====categories=====   =======display=======   ======================= */
   /*-o---,  ---shape----- , -sx, -sy, rot, beg, arc,   -cat--- , -ra, -sz-   --tx, --ty, -align-- ,  -----,-----clcuate----- */
   { "o3" ,  SHAPE_ELLIPSE ,   4,   4, 120,  90, 220,   CAT_O   ,   9,   1,      0,    0, 0        ,   0, 0, 0, 0, 0, 0, 0, 0 },
   { "o4" ,  SHAPE_ELLIPSE ,   4,   4, 105,  90, 220,   CAT_O   ,   9,   1,      0,    0, 0        ,   0, 0, 0, 0, 0, 0, 0, 0 },
   { "o5" ,  SHAPE_ELLIPSE ,   4,   4,  90,  90, 180,   CAT_O   ,   9,   1,      0,    0, 0        ,   0, 0, 0, 0, 0, 0, 0, 0 },
   { "o7" ,  SHAPE_ELLIPSE ,   4,   4,  40,  90, 180,   CAT_O   ,   9,   1,      0,    0, 0        ,   0, 0, 0, 0, 0, 0, 0, 0 },
   { "o8" ,  SHAPE_ELLIPSE ,   4,   4,  20,  90, 220,   CAT_O   ,   9,   1,      0,    0, 0        ,   0, 0, 0, 0, 0, 0, 0, 0 },
   { "o9" ,  SHAPE_ELLIPSE ,   4,   4,   0,  90, 180,   CAT_O   ,   9,   1,      0,    0, 0        ,   0, 0, 0, 0, 0, 0, 0, 0 },
   /*-u---,  ---shape----- , -sx, -sy, rot, beg, arc,   -cat--- , -ra, -sz-   --tx, --ty, -align-- ,  -----,-----clcuate----- */
   { "u3" ,  SHAPE_ELLIPSE ,   4,   4, 120,  90,-180,   CAT_U   ,   2,   2,      0,    0, 0        ,   0, 0, 0, 0, 0, 0, 0, 0 },
   { "u5" ,  SHAPE_ELLIPSE ,   4,   4,  90,  90,-180,   CAT_U   ,   2,   2,      0,    0, 0        ,   0, 0, 0, 0, 0, 0, 0, 0 },
   { "u7" ,  SHAPE_ELLIPSE ,   4,   4,  70,  90,-180,   CAT_U   ,   2,   2,      0,    0, 0        ,   0, 0, 0, 0, 0, 0, 0, 0 },
   { "u8" ,  SHAPE_ELLIPSE ,   4,   4,  40,  90,-180,   CAT_U   ,   2,   2,      0,    0, 0        ,   0, 0, 0, 0, 0, 0, 0, 0 },
   { "u9" ,  SHAPE_ELLIPSE ,   4,   4,   0,  90,-180,   CAT_U   ,   2,   2,      0,    0, 0        ,   0, 0, 0, 0, 0, 0, 0, 0 },

   /*-combz, ---shape----- , -sx, -sy, rot, beg, arc   , -cat--- , -ra, -so   --tx, --ty, -align---,  -----,-----clcuate----- */
   { ">1" ,  SHAPE_SPACE   ,   3,   3,   0,   0,   0,   CAT_NONE,   0,   0,      0,    0, 0        ,   0, 0, 0, 0, 0, 0, 0, 0 },

   /*-combz, ---shape----- , -sx, -sy, rot, beg, arc   , -cat--- , -ra, -so   --tx, --ty, -align---,  -----,-----clcuate----- */
   { "ox" ,  SHAPE_ELLIPSE ,   5,   3,  15,  80, 180,   CAT_R   ,   0,   0,      0,    0, 0        ,   0, 0, 0, 0, 0, 0, 0, 0 },
   { "ux" ,  SHAPE_ELLIPSE ,   5,   3,  15,  80,-180,   CAT_K   ,   0,   0,      0,    0, 0        ,   0, 0, 0, 0, 0, 0, 0, 0 },

   /*=====[[ 4th ZONE FIXES ]]=======================   ====categories=====   =======display=======   ======================= */
   /*-left,  ---shape----- , -sx, -sy, rot, beg, arc,   -cat--- , -ra, -sz-   --tx, --ty, -align-- ,  -----,-----clcuate----- */
   { "s4" ,  SHAPE_ELLIPSE ,   4,   3,   5,   5,-165,   CAT_F   ,   4,   1,      0,    0, 0        ,   0, 0, 0, 0, 0, 0, 0, 0 },
   { "s3" ,  SHAPE_ELLIPSE ,   4,   3,  25,   5,-165,   CAT_F   ,   4,   1,      0,    0, 0        ,   0, 0, 0, 0, 0, 0, 0, 0 },
   { "s2" ,  SHAPE_ELLIPSE ,   4,   3,  55,   5,-165,   CAT_F   ,   4,   1,      0,    0, 0        ,   0, 0, 0, 0, 0, 0, 0, 0 },
   { "s1" ,  SHAPE_ELLIPSE ,   4,   3,  75,   5,-165,   CAT_F   ,   4,   1,      0,    0, 0        ,   0, 0, 0, 0, 0, 0, 0, 0 },
   { "s0" ,  SHAPE_ELLIPSE ,   4,   3,  95,   5,-165,   CAT_F   ,   4,   1,      0,    0, 0        ,   0, 0, 0, 0, 0, 0, 0, 0 },
   /*-righ,  ---shape----- , -sx, -sy, rot, beg, arc,   -cat--- , -ra, -szt   --tx, --ty, -align-- ,  -----,-----clcuate----- */
   { "s9" ,  SHAPE_ELLIPSE ,   4,   3,   5,  20, 165,   CAT_P   ,   4,   1,      0,    0, 0        ,   0, 0, 0, 0, 0, 0, 0, 0 },
   { "s8" ,  SHAPE_ELLIPSE ,   4,   3,  25,  20, 165,   CAT_P   ,   4,   1,      0,    0, 0        ,   0, 0, 0, 0, 0, 0, 0, 0 },
   { "s7" ,  SHAPE_ELLIPSE ,   4,   3,  55,  20, 165,   CAT_P   ,   4,   1,      0,    0, 0        ,   0, 0, 0, 0, 0, 0, 0, 0 },
   { "s6" ,  SHAPE_ELLIPSE ,   4,   3,  75,  20, 165,   CAT_P   ,   4,   1,      0,    0, 0        ,   0, 0, 0, 0, 0, 0, 0, 0 },
   { "s5" ,  SHAPE_ELLIPSE ,   4,   3,  95,  20, 165,   CAT_P   ,   4,   1,      0,    0, 0        ,   0, 0, 0, 0, 0, 0, 0, 0 },

   /*-----,  ---shape----- , -sx, -sy, rot, beg, arc,   -cat--- , -ra, -szt   --tx, --ty, -align-- ,  -----,-----clcuate----- */
   { "EOF",  SHAPE_NONE    ,   0,   0,   0,   0,   0,   CAT_NONE,   0,   0,      0,    0, 0        ,   0, 0, 0, 0, 0, 0, 0, 0 },
};


/*=> positioning for circular vowels is 0-7 is round, 8-15 is wedge */
int   vowels [16] [16] = {
   /* 1st, 2nd = -,  t, th, ht,  n,  k,  r, ng, xu, xo, sx, zx, sh,  f,  p, bd  */
   /*  -  */  { __, SE, SE, NN, SS, SS, NN, SW, __, __, __, __, NW, WW, SE, __  },
   /*  t  */  { SE, SE, NW, nn, NN, SE, nn, NN, __, __, __, __, ne, nn, ne, __  },
   /*  th */  { SS, SS, ss, SS, SS, ss, NE, SS, __, __, __, __, ee, sw, ee, __  },
   /*  ht */  { NW, NW, SE, nw, NW, SE, nn, NW, __, __, __, __, nn, nw, ne, __  },
   /*   n */  { SS, SE, SE, NN, SS, SE, NE, NN, __, __, __, __, ne, NN, ne, __  },
   /*   k */  { SW, ss, ss, sw, sw, ss, NE, SW, __, __, __, __, sw, sw, ee, __  },
   /*   r */  { NN, NW, SE, nn, NW, SS, nn, NN, __, __, __, __, ne, NN, ne, __  },
   /*  ng */  { SW, SS, se, nn, SS, SS, NE, SW, __, __, __, __, ne, NE, ne, __  },
   /*  xu */  { __, __, __, __, __, __, __, __, __, __, __, __, __, __, __, __  },
   /*  xo */  { __, __, __, __, __, __, __, __, __, __, __, __, __, __, __, __  },
   /*  sx */  { __, __, __, __, __, __, __, __, __, __, __, __, __, __, __, __  },
   /*  zx */  { __, __, __, __, __, __, __, __, __, __, __, __, __, __, __, __  },
   /*  sh */  { SE, ss, ss, sw, sw, sw, WW, sw, __, __, __, __, WW, WW, SE, __  },
   /*   f */  { NW, sw, sw, sw, sw, sw, ww, sw, __, __, __, __, NW, ww, SE, __  },
   /*   p */  { EE, ss, se, SW, ss, ss, ne, SW, __, __, __, __, ee, WW, ee, __  },
   /*  bd */  { __, __, __, __, __, __, __, __, __, __, __, __, __, __, __, __  },
};


/*============================----(source-end)----============================*/
