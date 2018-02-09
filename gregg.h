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
#include    <yVIKEYS.h>      /* CUSTOM : heatherly vi_keys standard           */
#include    <yCOLOR.h>       /* CUSTOM : heatherly color handling             */



/* rapidly evolving version number to aid with visual change confirmation     */
#define VER_NUM   "5.2i"
#define VER_TXT   "added dotted lines connecting start and finish points"


#define     LEN_HUGE     10000
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
   int         icons;
   /*---(texture)-----------*/
   int         tex_h;                  /* texture height                      */
   int         tex_w;                  /* texture width                       */
   /*---(done)--------------*/
} win;

#define     ICON_SET  "/usr/local/share/fonts/outline_icons.png"

#define     GREGG_WACOM        64.0

/*---(struct.re)--------+-----------+-*//*-+----------------------------------*/
typedef     struct      cACCESSOR   tACCESSOR;
struct cACCESSOR {
   /*---(reporting)------------*/
   char        rptg_touch;
   char        rptg_raw;
   char        rptg_base;
   char        rptg_key;
   char        rptg_dict;
   char        rptg_letter;
   /*---(displaying)-----------*/
   char        show_control;                /* show control buttons to right  */
   char        show_sample;                 /* show sample strokes for ref    */
   char        show_player;                 /* show player controls           */
   /*---(temp)-----------------*/
   char        keys        [LEN_LABEL];
   char        key_error;
   char        quit;
   char        words       [LEN_RECD];
   char        guide       [LEN_RECD];
   /*---(globals)--------------*/
   char        touch;
   int         xpos;
   int         ypos;
   float       ratio;                       /* touch to display coord ratio   */
   int         x_scale;
   int         y_scale;
   int         x_min;
   int         x_max;
   int         x_wide;
   int         x_full;
   int         y_min;
   int         y_max;
   int         y_tall;
   float       x_center;
   float       y_center;
   /*---(stroke file)----------*/
   char        f_loc       [LEN_RECD];      /* specific file location         */
   char        f_name      [LEN_RECD];      /* base name                      */
   char        f_title     [LEN_RECD];      /* base and suffix (for display)  */
   char        f_full      [LEN_RECD];      /* loc, base, and suffix          */
   int         f_lines;                     /* file line count                */
   char        f_recd      [LEN_RECD];      /* current file record            */
   /*---(done)-----------------*/
} my;

#define     RPTG_TOUCH   if (my.rptg_touch   == 'y') 
#define     RPTG_RAW     if (my.rptg_raw     == 'y') 
#define     RPTG_BASE    if (my.rptg_base    == 'y') 
#define     RPTG_KEY     if (my.rptg_key     == 'y') 
#define     RPTG_DICT    if (my.rptg_dict    == 'y') 
#define     RPTG_LETTER  if (my.rptg_letter  == 'y') 

#define     SHOW_CONTROL if (my.show_control == 'y') 
#define     SHOW_SAMPLE  if (my.show_sample  == 'y') 
#define     SHOW_PLAYER  if (my.show_player  == 'y') 

/*---(run as)----------*/
#define     RUN_USER           'i'      /* running in user mode (ncurses)     */
#define     RUN_TEST           '-'      /* running as a test    (no ncurses)  */

extern char unit_answer  [LEN_STR];


/*===[[ DEBUGGER : GENERIC ]]=================================================*/
struct cDEBUG {
   /*---(specific processing)------------*/
   char      sharps;   /* finding sharp corners                    */
   char      matches;  /* matching logic                           */
   char      shapes;   /* shape formation in display lists         */
   char      writing;  /* process of creating outlines             */
} debug_old;

#define   DEBUG__SHARPS     if (debug_old.sharps   == 'y')
#define   DEBUG__MATCHES    if (debug_old.matches  == 'y')
#define   DEBUG__SHAPES     if (debug_old.shapes   == 'y')
#define   DEBUG__WRITING    if (debug_old.writing  == 'y')




#define    MAX_LETTERS   200
typedef struct cLOCATION tLOCATION;
struct cLOCATION
{
   /*---(constant)----------*/
   char      n[5];       /* name              */
   int       tx;         /* teaching x        */
   int       ty;         /* teaching y        */
   int       sx;         /* ellipse x-radius  */
   int       sy;         /* ellipse y-radius  */
   int       ro;         /* ellipse rotation  */
   int       be;         /* arc begin         */
   int       ar;         /* arc length        */
   int       st;         /* steps on dotting  */
   char      gr[5];      /* letter group      */
   int       range;      /* range of letter   */
   int       sz;         /* size category     */
   char      fu;         /* function to draw  */
   /*---(calculated)--------*/
   float     ex;         /* ending x          */
   float     ey;         /* ending y          */
   int       le;         /* leftmost          */
   int       ri;         /* rightmost         */
   int       ln;         /* full length       */
   int       de;         /* degrees of slope  */
   /*---(done)--------------*/
};
extern tLOCATION   g_loc [MAX_LETTERS];


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
   int     range;        /* range of full letter           */
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

/*---(display lists)---------------*/
/*> GLuint    dl_arrow;                                                               <*/
GLuint    dl_solid;
GLuint    dl_dotted;
/*> GLuint    dl_arrowed;                                                             <*/
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
#define   SIZE_CENTER       2
#define   SIZE_SML_MIN      8
#define   SIZE_SML_AVG     16
#define   SIZE_SML_MAX     25
#define   SIZE_MED_AVG     35
#define   SIZE_MED_MAX     47
#define   SIZE_LRG_AVG     62
#define   SIZE_LRG_MAX     78

typedef struct timespec  tTSPEC;


#define     PART_PREFIX    'p'
#define     PART_MAIN      'o'
#define     PART_CONTINUE  'c'

#define     POINTS_RAW     'r'
#define     POINTS_BAS     'b'
#define     POINTS_AVG     'a'
#define     POINTS_KEY     'k'
#define     POINTS_TMP     't'
#define     POINTS_ALL     "rbakt"

#define     SPAN_NORM      'n'
#define     SPAN_AVG       'a'
#define     SPAN_SPOT      '-'
#define     SPAN_ALL       "na-"


#define     POINT_TYPES    "S>-F"
#define     POINT_START    'S'
#define     POINT_HEAD     '>'
#define     POINT_NORMAL   '-'
#define     POINT_FINISH   'F'
#define     POINT_NONE     '/'
#define     POINT_FAKE     'y'

#define     MODE_CURSOR    '-'
#define     MODE_TOUCH     'y'
#define     MODE_CONTROL   'c'
#define     MODE_SLIDER    's'
#define     MODE_PLAYER    'p'

typedef struct cPOINT tPOINT;
struct cPOINT
{
   /*---(links)-----------------*/
   int         p_raw;                  /* tie to raw point                    */
   int         p_bas;                  /* tie to basic point                  */
   char        type;                   /* type of key point (sharp or rounded) */
   /*---(touchpad)--------------*/
   int         x_full;                 /* x-pos in touchpad coords            */
   int         y_full;                 /* y-pos in touchpad coords            */
   /*---(statistics)------------*/
   int         xd;                     // x-dist from last xpos
   int         yd;                     // y-dist from last ypos
   int         len;                    // length between points
   float       slope;                  // slope from last point
   int         icept;                  // y-intercept of line from last point
   float       rads;                   // radians of line from last point
   int         degs;                   // degrees of line from last point
   int         quad;                   // quadrent of line from last point
   int         range;                  // range of point
   float       cdepth;                 // pixels of curvature at mid point
   char        cano;                   // curve anomolies '-' = normal, 'x' = jittery
   char        ccat;                   // curve category : +1, 0, -1                    
   char        sharp;                  /* distinct break before-to-after      */        
   char        use         [5];        /* use of this point in outline        */
   char        fake;                   /* artificial point or not (y/n)       */
   /*---(display)---------------*/
   float       x_rel;                  /* input device relative x_coord       */
   float       y_rel;                  /* input device relative y_coord       */
   int         x_pos;                  /* screen x_coord                      */
   int         y_pos;                  /* screen y_coord                      */
   int         xy_len;                 /* screen length                       */
   /*---(done)------------------*/
};



typedef struct cOUTLINE tOUTLINE;
struct cOUTLINE
{
   /*---(index)--------------------------*/
   int         total;                    /* total count of outlines             */
   int         curr;                     /* current outline index in file       */
   /*---(header)-------------------------*/
   char        when   [50];              /* timestamp of creation/writing       */
   char        expect [50];              /* expected result of interpretation   */
   char        note   [50];              /* textual description                 */
   char        saved;                    /* flag to indicate whether saved      */
   int         complexity;               /* how complex is outline in general   */
   int         messiness;                /* how messy is outline                */
   char        points [LEN_RECD];        /* raw data points from file           */
   /*---(match)--------------------------*/
   char        letters[50];              /* detailed match result               */
   char        actual [50];              /* actual match result                 */
   char        grade  [50];              /* grading of match                    */
   char        word   [50];              /* resulting dictionary word           */
   /*---(raw)----------------------------*/
   int         nraw;                     /* number of raw points                */
   int         craw;                     /* current average point               */
   int         xadj;
   int         yadj;
   float       ratio;                  /* input to display conversion ratio   */
   /*---(base)---------------------------*/
   int         nbas;                     /* number of basic points              */
   /*---(average)------------------------*/
   int         navg;                     /* number of average points            */
   int         cavg;                     /* current average point               */
   /*---(keys)---------------------------*/
   int         nkey;                     /* number of key points                */
   int         ckey;                     /* current key point                   */
   /*---(acutal points)------------------*/
   tPOINT      raw         [MAX_POINTS]; /* raw points                          */
   tPOINT      bas         [MAX_POINTS]; /* basic points                        */
   tPOINT      avg         [MAX_POINTS]; /* average points                      */
   tPOINT      tmp         [5];          /* calculation storage                 */
   tPOINT      key         [MAX_POINTS]; /* key points                          */
   /*---(bounding)-----------------------*/
   int         x_min;
   int         y_min;
   int         x_max;
   int         y_max;
   /*---(done)---------------------------*/
} o;


#define     COLOR_BASE          'b'
#define     COLOR_DARK          '-'
#define     COLOR_LIGHT         '+'
#define     COLOR_MUTED         'm'
#define     COLOR_ACC_L         '1'
#define     COLOR_ACC_D         '2'
#define     COLOR_ACC_O         'o'
#define     COLOR_TXT_L         'w'
#define     COLOR_TXT_D         'k'
#define     COLOR_GRID_L        'g'
#define     COLOR_GRID_D        'G'
#define     COLOR_BLACK         'K'
#define     COLOR_WARN          'W'


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
char       PROG_finish          (void);
char       PROG_end             (void);

char       DRAW_init            (void);
/*> char       DRAW__resize         (cchar a_format, cchar *a_title, cint a_wide, cint a_tall);   <*/
char       DRAW_wrap            (void);
/*> char       DRAW_set_color       (char a_color);                                   <*/

char       DRAW_primary         (float a_mag);
/*> char       DRAW_main            (void);                                           <*/
char       DRAW_back            (void);

char       draw_background      (void);
char       draw_oslider         (void);
char       DRAW_slide_avg       (void);
char       draw_kslider         (void);

char       LAYER_curr            (void);
char       LAYER_raws            (void);
char       LAYER_base            (void);
char       LAYER_keys            (void);
char       OVERLAY_data          (void);
char       OVERLAY_samples       (void);

/*> char       draw_saved           (void);                                           <*/
/*> char       draw_horz            (void);                                           <*/
/*> char       DRAW_cursor          (void);                                           <*/

char       DRAW_info            (void);
char       DRAW_info_counts     (void);
char       DRAW_info_base       (void);
char       DRAW_info_answer     (void);

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



char       dlist_init        (void);

/*---(outline)--------------*/
char       OUT_status         (char *a_list);
char       OUT_init             (void);
char       OUT_clear            (void);
char       OUT__open            (char *a_mode);
char       OUT__close           (void);
char       OUT_count            (void);
char       OUT_pick             (int a_num);
char       OUT_append           (void);
char       out_read             (int);
char*      OUT__unit            (char *a_question, tPOINT *a_curr);


char       POINT_calc           (char a_type, tPOINT* a_curr, int a_span);
char       POINT_list           (FILE *a_file, char a_style, tPOINT *a_series, int a_count);
char       POINT_show           (FILE *a_file, char a_style, tPOINT *a_curr  , int a_num);
char       POINT_clipboard      (char *a_cmd, char *a_opt);
char       FILE_rename          (char *a_name);

/*---(raw)------------------*/
char       RAW__point           (int a_xpad, int a_ypad, char a_type);
char       RAW_touch            (int a_xpad, int a_ypad);
char       RAW_lift             (int a_xpad, int a_ypad);
char       RAW_normal           (int a_xpad, int a_ypad);

char       RAW_load             (char *a_points);
char       RAW_equalize         (void);
char*      RAW__unit            (char *a_question, int a_num);

/*---(base)-----------------*/
char       BASE_filter          (void);


char       KEY_add           (int, char, char);
char       KEY_calc          (char);
char       KEY_prep          (void);
char       KEY_label         (int, int, char*);
char       KEY_del           (int);
int        KEY_find          (int);
char       KEY_filter        (void);
char       KEY_flatten       (void);
char       KEY_squeeze       (void);
char       KEY_sharps        (void);


char       MATCH_driver      (void);
char       match_calc        (int, int);

int        match_size        (int);
int        match_range       (int);

char       CIRCLE_driver     (void);

char       dict_read         (void);
int        words_outstring   (char *);

int        WORDS_find            (char* a_word);
char       WORDS_start           (void);
char       WORDS_outline         (int   a_index, char a_base);
char       WORDS_display         (char* a_words, char a_base);

int        words_translate   (int);
int        words_vowel       (int, int);
int        words_consonant   (int, int);
char       WORDS_result      (void);

char      *str_trim          (char *);

char*      unit_accessor     (char *a_question, int a_num);


char        TOUCH_init           (void);
char        TOUCH_wrap           (void);
char        TOUCH__open          (void);
char        TOUCH__close         (void);
char        TOUCH__normal        (void);
char        TOUCH__check         (void);
char        TOUCH_read           (void);


char        MAP_mapper           (char  a_req);
char        USER_words           (char *a_words);
char        USER_guide           (char *a_guide);
char        USER_init            (void);
char        USER_quit            (void);
char        USER_writequit       (void);
char        USER_map_mode        (char a_major, char a_minor);
/*> char        USER_cmds_mode       (char a_major, char a_minor);                    <*/

#endif
/*============================----(source-end)----============================*/
