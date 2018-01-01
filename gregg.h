/*============================---(source-start)---============================*/

/*===[[ HEADER ]]=============================================================*

 *   focus         : (AI) alternative input
 *   niche         : (pe) pen-based writing
 *   application   : gregg
 *   purpose       : provide hyper-efficient, effective pen-based english input
 *
 *   base_system   : gnu/linux  (because it's powerful, universal, and hackable)
 *   lang_name     : ansi-c     (because it's right, just, best, and universal)
 *   dependencies  : opengl, yFONT
 *   size goal     : moderate (less than 5,000 slocL)
 *
 *   author        : the_heatherlys
 *   created       : 2008-07 (built up from the newton version)
 *   priorities    : direct, simple, brief, vigorous, and lucid (h.w. fowler)
 *   end goal      : loosely coupled, strict interface, maintainable, portable
 * 
 */
/*===[[ DESCRIPTION ]]========================================================*

 *   gregg shorthand is a proven, time-tested, and deeply explored system of
 *   pen-focused writing enabling extremely fast word-based input whose speed is
 *   unrivaled even in the digital age.  plus, its a completely wicked and scary
 *   programming challenge.  how can we resist ;)
 *
 */
/*===[[ BACKGROUND ]]=========================================================*

 *   gregg shorthand is a quixotic undertaking to say the least, but one that
 *   is in very strong alignment with our personal goals.  it is at once
 *   archaic and complex but at the same tem a tremendous time saver even in the
 *   digital world while providing tremendous confidentialty potential.
 *
 *   on the negative side, gregg shorthand and shorthand in general is old and
 *   not in any real current usage.  gregg in particular was developed by 1888
 *   and the last serious update was in 1988.  it was supposedly replaced in
 *   everyday use by computer keyboard input and tape recorders.  in reality
 *   folks have really gone back to paper notes typically printed rather than
 *   even the faster longhand.  as a result, secretaries are incapabile of
 *   functioning in the way they could in the heyday of shorthand.
 *
 *   by way of loose average speed comparisons...
 *      - printing     :   12   wpm
 *      - cursive      :   23   wpm
 *      - typing       :   35   wpm
 *      - keyboading   :   50+  wpm
 *      - gregg        :   150+ wpm (better in non-transcription mode)
 *      - max gregg    :   285  wpm (world class with 98% accuracy)
 *
 *   on the positive side, gregg shorthand is the fastest of the shorthand
 *   systems and can enable 150+ words per minute of input for a well oiled
 *   user of the system.  additionally, it can be used in any method that allows
 *   pen-based input from paper to computer tablets.  it also does not require
 *   heads-down usage like screen keyboards so the user can have their eyes on
 *   the meeting, presentations, interviewee, or other things.
 *
 *   from a programming perspective, it is a wonderful computer project as it
 *   requires pen-based input, visual displays, and fast interpretation of
 *   the input to make it useful.  moreover is mostly unexplored so it allows
 *   the greatest possible learning potential.
 *
 *   from a secrecy perspective, it is quite simply fantastic.  it was
 *   developed to be fast and repeatable by the foremost shorthand experts and
 *   reigned supreme as the best system.  it is known as fast to input, but
 *   difficult to read even by consistent users so it will be virtually
 *   unintelligable to anyone today because it is so out of date.  even better
 *   since it can be stored visually, it would very, very difficult to hack.
 *
 *   overall, things like shorthand go into the "too hard basket" really fast
 *   by folks that think the qwerty keyboard layout is efficient and then won't
 *   take the time even to get good at that.  won't be the last time a superior
 *   method was tossed to make way for the short-lived promise of an easier way.
 *
 *   the idea behind the system is to build up the ability to desengage from
 *   keyboards on pen based systems and free up the space, fuss, and equipment
 *   overhead.  it also will work well in environments where privacy precludes
 *   voice input.  finally, it is in itself a very unintelligible system to the
 *   untrained and so its own layer of security
 *
 *   gregg stands exactly a zero percent chance of being acceptable to anyone
 *   except us as its power comes from alternative techniques that require time
 *   and willpower to master.  rather than being a bad thing, this frees us up
 *   to drive gregg as far as time and willpower will allow.
 *
 *   no matter what, its great for the brain, great for creativity, fun, and
 *   a solid programming project to learn from.
 *
 */
/*===[[ GREGG SHORTHAND HISTORY ]]--------------------------------------------*

 *   gregg shorthand evolved over the course of 100 years to what the current
 *   needs were perceived to be.  in the beginning brutal speed was the goal.
 *   over time, ease to learn became far more important, then the scales tipped
 *   back to speed.  luckily the training manuals for the system are availible
 *   for each main version so we can take the best suited to our purposes.
 *
 *   a loose timeline of gregg's development is as follows...
 *     - development (1888 - 1902) de : --- : rougher and not complete
 *     - pre-anniversary (1916)    pa : 5th : hardest to learn, but brutal fast
 *     - anniversary (1929)        an : 6th : simplified and cleaned up
 *     - simplified (1949)         si : 7th : simplified and still pretty fast
 *     - diamond (1963)            di : 8th : focus on learning speed
 *     - series 90 (1978)          se : 9th : down under 100wpm
 *     - centennial (1988)         ce : 10th: back up to 150wpm like diamond
 *
 *   for our purposes, we will be leaning towards the speed angle as a focus
 *   for the long-run.  practically, this means a combination of the anniversary
 *   and simplified versions that have the major issues sorted out,
 *   irregularities tamed to ease use, but are still focused on speed.  we will
 *   be looking at the pre-anniversary to see what we can use.
 *
 *   there will be trade offs we will have to take in order to easy computer
 *   interpretation in realtime, but that should have limited impact on speed.
 *
 */
/*===[[ TERMINOLOGY ]]--------------------------------------------------------*

 *   gregg is a shorthand system which provides an alternate way of representing
 *   english writing that will allow faster capture of thoughts and words.
 *
 *   outlines are the primary focus of shorthand and represent words and phrases
 *
 *   strokes are particular shapes that represent letters, sounds, or groups of
 *   sounds that are then joined into outlines
 *
 */
/*===[[ DESIGN ]]-------------------------------------------------------------*

 *   gregg is an alternative pen-based writing system that seeks to compete
 *   with, and greatly improve upon existing tablet input sytems such as
 *   onscreen keyboards.
 *
 *   gregg will focus on...
 *      - small visable footprint
 *      - using strokes for input rather that tapping or pressing
 *      - accepting a complex stroke of input as an outline
 *      - analysis of the outlines to interpret into letters
 *      - and exchanging the letters for real words
 *      - that can then be put into other windows and applications
 *
 *   gregg will not...
 *      - have its own note taking application
 *      - provide other types of input
 *
 *   building a custom input system, like gregg, will allow us to build in
 *   things like keyboard short cuts that can not be built into keyboards or
 *   into keyboard mappers/drivers.  normally these have to be scripted into
 *   each individual application, require a entire desktop environment that
 *   allows special applications to intercept keys (specific to the DE), or we
 *   would need to hack the actual window manager.
 *
 *   for learning purposes it will...
 *      - use opengl for all drawing (including solid aesthetics)
 *      - calculate positions for input rather than using widgets/buttons
 *      - attempt to draw onto a preprepared bitmap to increase efficiency
 *      - overall, learn more about xwindows and opengl
 */
/*===[[ END DOC ]]------------------------------------------------------------*/


/*---(header guard)------------------------*/
#ifndef GREGG
#define GREGG loaded



/*===[[ HEADERS ]]========================================*/
/*---(ansi-c standard)-------------------*/
#include   <stdio.h>              /* clibc  standard input/output             */
#include   <stdlib.h>             /* clibc  standard general purpose          */
#include   <string.h>             /* clibc  standard string handling          */
#include   <error.h>              /* clibc  standard error handling           */
#include   <fcntl.h>              /* clibc  standard file control             */
#include   <termios.h>            /* clibc  standard terminal control         */
#include   <math.h>               /* clibc  standard math functions           */
#include   <signal.h>             /* clibc  standard signal handling          */
#include   <time.h>               /* clibc  standard time and date handling   */
#include   <ctype.h>              /* clibc  standard character classes        */

/*---(posix standard)--------------------*/
#include   <unistd.h>             /* POSIX  standard operating system API     */
#include   <sys/time.h>           /* POSIX  standard time access              */

/*---(X11 standard)----------------------*/
#include   <X11/X.h>              /* X11    standard overall file             */
#include   <X11/Xlib.h>           /* X11    standard C API                    */


/*---(opengl standard)-------------------*/
#include   <GL/gl.h>              /* opengl standard primary header           */
#include   <GL/glx.h>             /* opengl standard X11 integration          */

/*===[[ CUSTOM LIBRARIES ]]===================================================*/
#include    <yLOG.h>         /* CUSTOM : heatherly program logging            */
#include    <yURG.h>         /* CUSTOM : heatherly urgent processing          */
#include    <yVAR.h>         /* CUSTOM : heatherly variable testing           */
#include    <ySTR.h>         /* CUSTOM : heatherly string handling            */
#include    <yX11.h>         /* CUSTOM : heatherly xlib/glx setup/teardown    */
#include    <yFONT.h>        /* CUSTOM : heatherly texture-mapped fonts       */
#include    <yGLTEX.h>       /* CUSTOM : heatherly texture handling           */



/* rapidly evolving version number to aid with visual change confirmation     */
#define VER_NUM   "5.0j"
#define VER_TXT   "background is sized well and showing beautifully"


#define     LEN_RECD      2000
#define     LEN_STR        200
#define     LEN_DESC       100
#define     LEN_LABEL       20

typedef     const char          cchar;
typedef     const int           cint;


#define    PUBL      /*--*/
#define    PRIV      static

/*---(window formatting)-------------------*/
struct cWIN {
   /*---(header)------------*/
   char        w_format;                    /* format identifier              */
   /*---(window sizes)------*/
   char        w_title     [LEN_DESC];      /* window title                   */
   int         w_wide;                      /* window width                   */
   int         w_tall;                      /* window heigth                  */
   /*---(title sizes)-------*/
   char        t_text      [LEN_DESC];      /* title text                     */
   int         t_wide;                      /* width  of title line           */
   int         t_left;                      /* left   of title line           */
   int         t_tall;                      /* height of title line           */
   int         t_bott;                      /* bottom of title line           */
   /*---(command sizes)-----*/
   char        c_text      [LEN_DESC];      /* current text in command mode   */
   int         c_wide;                      /* width  of command line         */
   int         c_left;                      /* left   of command line         */
   int         c_tall;                      /* height of command line         */
   int         c_bott;                      /* bottom of command line         */
   /*---(main sizes)--------*/
   int         m_wide;                      /* width  of main window          */
   int         m_left;                      /* left   of main window          */
   int         m_tall;                      /* height of main window          */
   int         m_bott;                      /* bottom of main window          */
   int         m_ymax;                      /* from center to top             */
   int         m_ymin;                      /* from center to bottom          */
   int         m_xmax;                      /* from center to right           */
   int         m_xmin;                      /* from center to left            */
   /*---(detail)------------*/
   int         d_xoff;                      /* left offset of text details    */
   int         d_yoff;                      /* top offset of text details     */
   int         d_zoff;                      /* z offset of text details       */
   int         d_point;                     /* font point                     */
   int         d_bar;                       /* control bar height             */
   int         d_ansx;                      /* answer x offset                */
   int         d_ansy;                      /* answer y offset                */
   /*---(fonts)-------------*/
   char        face_bg     [50];
   int         font_bg;
   char        face_sm     [50];
   int         font_sm;
   /*---(texture)-----------*/
   int         tex_h;                  /* texture height                      */
   int         tex_w;                  /* texture width                       */
   /*---(done)--------------*/
} win;



/*---(struct.re)--------+-----------+-*//*-+----------------------------------*/
typedef     struct      cACCESSOR   tACCESSOR;
struct cACCESSOR {
   char        rptg_touch;
   char        rptg_raw;
   char        touch;
   int         xpos;
   int         ypos;
} my;

#define     RPTG_TOUCH  if (my.rptg_touch == 'y') 
#define     RPTG_RAW    if (my.rptg_raw   == 'y') 



/*===[[ DEBUGGER : GENERIC ]]=================================================*/
struct cDEBUG {
   /*---(standards)----------------------*/
   char      top;
   char      prep;
   char      cli;
   char      input;
   char      proc;
   char      data;
   char      output;
   char      graph;
   char      api;
   char      event;
   /*---(specific input)-----------------*/
   char      raw;      /* processing stylus into to raw points     */
   char      bas;      /* processing raw to basic points           */
   char      key;      /* processing bas/avg points to key         */
   /*---(specific processing)------------*/
   char      sharps;   /* finding sharp corners                    */
   char      circles;  /* finding circles                          */
   char      lines;    /* straightening lines                      */
   char      matches;  /* matching logic                           */
   char      shapes;   /* shape formation in display lists         */
   char      averages; /* shape formation in display lists         */
   char      curves;   /* identifying curviness of lines           */
   char      writing;  /* process of creating outlines             */
   char      dict;     /* reading and loading of the dictionary    */
   char      dicte;    /* output of english word index             */
   char      dictg;    /* output of gregg word index               */
   /*---(summary)------------------------*/
   char      summ;     /* just summary statistics                  */
   char      lock;     /* finding my lockup                        */
} debug_old;
#define   DEBUG_T           if (debug_old.top       == 'y')
#define   DEBUG_X           if (debug_old.prep      == 'y')
#define   DEBUG_C           if (debug_old.cli       == 'y')
#define   DEBUG_I           if (debug_old.input     == 'y')
#define   DEBUG_P           if (debug_old.proc      == 'y')
#define   DEBUG_O           if (debug_old.output    == 'y')
#define   DEBUG_G           if (debug_old.graph     == 'y')
#define   DEBUG_A           if (debug_old.api       == 'y')
#define   DEBUG_E           if (debug_old.event     == 'y')

#define   DEBUG__RAW        if (debug_old.raw      == 'y')
#define   DEBUG__BAS        if (debug_old.bas      == 'y')
#define   DEBUG__KEY        if (debug_old.key      == 'y')
#define   DEBUG__SHARPS     if (debug_old.sharps   == 'y')
#define   DEBUG__CIRCLES    if (debug_old.circles  == 'y')
#define   DEBUG__LINES      if (debug_old.lines    == 'y')
#define   DEBUG__MATCHES    if (debug_old.matches  == 'y')
#define   DEBUG__SHAPES     if (debug_old.shapes   == 'y')
#define   DEBUG__AVERAGES   if (debug_old.averages == 'y')
#define   DEBUG__CURVES     if (debug_old.curves   == 'y')
#define   DEBUG__WRITING    if (debug_old.writing  == 'y')
#define   DEBUG__DICT       if (debug_old.dict     == 'y')
#define   DEBUG__DICTE      if (debug_old.dicte    == 'y')
#define   DEBUG__DICTG      if (debug_old.dictg    == 'y')

#define   DEBUG__SUMM       if (debug_old.summ     == 'y')



#define    MAX_LETTERS   200
typedef struct cLOCATION tLOCATION;
struct cLOCATION
{
   char      n[5];       /* name              */
   float     ex;         /* ending x          */
   float     ey;         /* ending y          */
   int       le;         /* leftmost          */
   int       ri;         /* rightmost         */
   int       ln;         /* full length       */
   int       de;         /* degrees of slope  */
   int       tx;         /* teaching x        */
   int       ty;         /* teaching y        */
   int       sx;         /* ellipse x-radius  */
   int       sy;         /* ellipse y-radius  */
   int       ro;         /* ellipse rotation  */
   int       be;         /* arc begin         */
   int       ar;         /* arc length        */
   int       st;         /* steps on dotting  */
   char      gr[5];      /* letter group      */
   int       ra;         /* range of letter   */
   int       sz;         /* size category     */
   char      fu;         /* function to draw  */
};
extern tLOCATION   loc[MAX_LETTERS];


#define   MAX_RANGES      15
typedef struct cRANGES tRANGES;
struct cRANGES {
   int     num;          /* link to other tables           */
   int     len;          /* normal=1, short =2             */
   char    nam[5];       /* short description              */
   int     min;          /* minimum degree                 */
   int     beg;          /* begin normal range             */
   int     tar;          /* best case degree               */
   int     end;          /* end normal range               */
   int     max;          /* minimum degree                 */
};
extern tRANGES ranges[MAX_RANGES];


#define   MAX_GROUPS      200
typedef struct cGROUPS tGROUPS;
struct cGROUPS {
   int     ra;           /* range of full letter           */
   int     op;           /* open points                    */
   char    sh[5];        /* shape flow                     */
   char    fl[5];        /* quadrent flow                  */
   char    ca[10];       /* valid curvatures               */
   char    gr[5];        /* letter group                   */
   int     gr_num;       /* letter group number            */
   int     ac;           /* active? (y/n)                  */
};
extern tGROUPS groups[MAX_GROUPS];
#define   CURVE_OFFSET      4


#define   MAX_COMBOS      200
typedef struct cCOMBOS tCOMBOS;
struct cCOMBOS {
   char    nm[5];        /* short description              */
   int     op;           /* open points                    */
   int     sh;           /* position of sharp point        */
   char    fl[10];       /* quadrent flow                  */
   char    mm[5];        /* midpoint method (x, y, -)      */
   int     st;           /* start for midpoint             */
   int     of;           /* offset to second letter        */
   char    on[5];        /* letter group one               */
   char    tw[5];        /* letter group two               */
   char    re[5];        /* replace letters with           */
   int     ac;           /* active? (y/n)                  */
};
extern tCOMBOS combos[MAX_COMBOS];


extern int   vowels[16][16];
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

/*---(display lists)---------------*/
GLuint    dl_arrow;
GLuint    dl_solid;
GLuint    dl_dotted;
GLuint    dl_arrowed;
GLuint    dl_undo;
GLuint    dl_redo;
GLuint    dl_clear;
GLuint    dl_back;

extern const float  DEG2RAD;
extern const float  RAD2DEG;
extern int   g_transx;
extern int   g_transy;

/*============================--------------------============================*/
/*===----                             points                           ----===*/
/*============================--------------------============================*/

#define   MAX_OUTLINES   100
#define   MAX_POINTS    1000
#define   MAX_LINE     10000

/*  clean   8, 15, 22, 30, 39, 48, 64       */
#define   SIZE_M0          8
#define   SIZE_R1         16
#define   SIZE_M1         25
#define   SIZE_R2         35
#define   SIZE_M2         47
#define   SIZE_R3         62
#define   SIZE_M3         78

typedef struct timespec  tTSPEC;


#define     PART_PREFIX    'p'
#define     PART_MAIN      'o'
#define     PART_CONTINUE  'c'

#define     POINT_START    'S'
#define     POINT_HEAD     '>'
#define     POINT_NORMAL   '-'
#define     POINT_FINISH   'F'

typedef struct cPOINT tPOINT;
struct cPOINT
{
   int         p_raw;                  // tie to raw point
   int         p_bas;                  // tie to basic point
   int         xpos;                   // xpos
   int         ypos;                   // ypos
   int         xd;                     // x-dist from last xpos
   int         yd;                     // y-dist from last ypos
   int         l;                      // length between points
   float       s;                      // slope from last point
   int         b;                      // y-intercept of line from last point
   float       r;                      // radians of line from last point
   int         d;                      // degrees of line from last point
   int         q;                      // quadrent of line from last point
   int         ra;                     // range of point
   float       c;                      // pixels of curvature at mid point
   char        ca;                     // curve anomolies '-' = normal, 'x' = jittery
   char        cc;                     // curve category : +1, 0, -1                    
   char        t;                      // type of key point (sharp or rounded)
   char        use         [5];        /* use of this point in outline        */
   char        fake;                   /* artificial point or not (y/n)       */
};



typedef struct cOUTLINE tOUTLINE;
struct cOUTLINE
{
   /*---(index)--------------------------*/
   int       total;                    /* total count of outlines             */
   int       curr;                     /* current outline index in file       */
   /*---(header)-------------------------*/
   char      when   [50];              /* timestamp of creation/writing       */
   char      expect [50];              /* expected result of interpretation   */
   char      note   [50];              /* textual description                 */
   char      saved;                    /* flag to indicate whether saved      */
   /*---(match)--------------------------*/
   char      letters[50];              /* detailed match result               */
   char      actual [50];              /* actual match result                 */
   char      grade  [50];              /* grading of match                    */
   char      word   [50];              /* resulting dictionary word           */
   /*---(points)-------------------------*/
   int       nraw;                     /* number of raw points                */
   int       craw;                     /* current average point               */
   tPOINT    raw[MAX_POINTS];          /* raw points                          */
   int       nbas;                     /* number of basic points              */
   tPOINT    bas[MAX_POINTS];          /* basic points                        */
   int       navg;                     /* number of average points            */
   int       cavg;                     /* current average point               */
   tPOINT    avg[MAX_POINTS];          /* average points                      */
   tPOINT    tmp[5];                   /* calculation storage                 */
   /*---(keys)---------------------------*/
   int       nkey;                     /* number of key points                */
   int       ckey;                     /* current key point                   */
   tPOINT    key[MAX_POINTS];          /* key points                          */
} o;



/*============================--------------------============================*/
/*===----                           prototypes                         ----===*/
/*============================--------------------============================*/

/*---(main)-----------------*/
int        main              (int argc, char *argv[]);

/*---(prog)-----------------*/
char*      PROG_version         (void);
char       PROG_init            ();
char       PROG_args            (int argc, char *argv[]);
char       PROG_begin           (void);
char       PROG_final           (void);
char       PROG_event           (void);
char       PROG_end             (void);

char       DRAW_init            (void);
char       DRAW__resize         (cchar a_format, cchar *a_title, cint a_wide, cint a_tall);
char       DRAW_wrap            (void);

char       DRAW_main            (void);
char       DRAW_back            (void);

char       draw_background      (void);
char       draw_oslider         (void);
char       DRAW_slide_avg       (void);
char       draw_kslider         (void);

char       draw_raws            (void);
char       draw_avgs            (void);
char       draw_keys            (void);
char       draw_curr            (void);
char       draw_saved           (void);
char       draw_horz            (void);
char       DRAW_cursor          (void);

char       draw_info         (void);
char       draw_letters      (void);
char       draw_ellipse      (int, char);
char       draw_arc          (float, float, int, int);
char       draw_pie          (int);
char       draw_dot          (int);
char       draw_whitespace   (int);

char       FONT__load           (void);
char       FONT__free           (void);
char       FONT__label          (char *a_label, char *a_content);

char       sample_init       (void);
char       sample_etch       (void);
char       sample_draw       (void);
char       sample_free       (void);
char       sample_show       (void);


long       time_stamp        (void);

char       dlist_init        (void);

/*---(outline)--------------*/
char       OUT_init             (void);
char       OUT_clear            (void);
char       out_pick             (int);
char       out_read             (int);
char       out_append           (void);
char       POINT_calc           (tPOINT*, int);
char       POINT_list           (tPOINT*, int);
char       POINT_show           (tPOINT*, int);

/*---(raw)------------------*/
char       POINT_wipe           (tPOINT *a_pt);
char       RAW_touch            (int a_x, int a_y);
char       RAW_normal           (int a_x, int a_y);
char       RAW_lift             (int a_x, int a_y);
char       RAW_equalize         (void);


char       bas_filter        (void);
char       avg_pick          (int);
int        avg_find          (int);
char       key_filter        (void);
char       key_add           (int, char, char);
char       key_calc          (char);
char       key_prep          (void);
char       key_label         (int, int, char*);
char       key_del           (int);
int        key_find          (int);
char       match_sharps      (void);

char       match_driver      (void);
char       match_calc        (int, int);

int        match_size        (int);
int        match_range       (int);

char       circle_driver     (void);
char       match_flatten     (void);
char       match_squeeze     (void);

char       dict_read         (void);
int        words_find        (char *);
int        words_outstring   (char *);
char       words_display     (char *);
int        words_translate   (int);
char       words_start       (void);
char       words_outline     (int);
int        words_vowel       (int, int);
int        words_consonant   (int, int);
char       words_result      (void);

char      *str_trim          (char *);

char*      unit_accessor     (char *a_question, int a_num);


char        TOUCH_init           (void);
char        TOUCH_wrap           (void);
char        TOUCH__open          (void);
char        TOUCH__close         (void);
char        TOUCH__normal        (void);
char        TOUCH__check         (void);
char        TOUCH_read           (void);



#endif
/*============================----(source-end)----============================*/
