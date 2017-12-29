/*============================---(source-start)---============================*/
#include    "gregg.h"






int       out_start       = 0;

/*============================--------------------============================*/
/*===----                              main                            ----===*/
/*============================--------------------============================*/

char       /*----: display command line help/usage ---------------------------*/
PROG_usage         (void)
{
   printf("gregg : fantastically efficient, effective pen-based gregg shorthand input system\n");
   printf("\n");
   printf("usage : gregg [urgents] [options] [outline]\n");
   printf("\n");
   printf("short urgents -- debugging tracing/information\n");
   printf("   @a          command line arguments\n");
   printf("   @e          x11 events\n");
   printf("   @i          pen input\n");
   printf("   @o          file output\n");
   printf("   @o          opengl graphics\n");
   printf("   @p          processing\n");
   printf("\n");
   printf("long urgents -- debugging tracing/information\n");
   printf("   @@averages  conversion of raw to basic to average points\n");
   printf("   @@circles   circle identificition and marking\n");
   printf("   @@curves    curve marking\n");
   printf("   @@keys      key point identification and marking\n");
   printf("\n");
   exit(0);
}

char       /*----: one-time initialization of program environment ------------*/
PROG_init          (void)
{
   /*---(formatting)------------------*/
   format_input();
   /*---(fonts)-----------------------*/
   strcpy(win.face_bg, "verdana");
   strcpy(win.face_sm, "courier");
   /*---(debugger : standard)---------*/
   /*> debug.prep   = 'n';     /+ x) program setup and tear-down           +/         <* 
    *> debug.cli    = 'n';     /+ c) command line interface                +/         <* 
    *> debug.input  = 'n';     /+ i) text input   (files, stdin, keys)     +/         <* 
    *> debug.proc   = 'n';     /+ p) main analytics and processing         +/         <* 
    *> debug.data   = 'n';     /+ d) data structure use and mgmt           +/         <* 
    *> debug.output = 'n';     /+ o) text output  (files, stdout, keys)    +/         <* 
    *> debug.graph  = 'n';     /+ g) graphics, drawing, and rendering      +/         <* 
    *> debug.event  = 'n';     /+ e) event loop                            +/         <* 
    *> debug.api    = 'n';     /+ x) programming api                       +/         <*/
   /*---(debugger : specific)---------*/
   /*> debug.raw    = 'n';     /+ raw points                               +/         <* 
    *> debug.bas    = 'n';     /+ bas points                               +/         <* 
    *> debug.key    = 'n';     /+ key points                               +/         <*/
   /*---(debugger : specific)---------*/
   /*> debug.sharps   = 'n';   /+ finding sharp corners                    +/         <* 
    *> debug.circles  = 'n';   /+ finding circles                          +/         <* 
    *> debug.matches  = 'n';   /+ matching logic                           +/         <* 
    *> debug.shapes   = 'n';   /+ shape formation in display lists         +/         <* 
    *> debug.averages = 'n';   /+ shape formation in display lists         +/         <* 
    *> debug.curves   = 'n';   /+ identifying curviness of lines           +/         <* 
    *> debug.writing  = 'n';   /+ process of creating outlines             +/         <* 
    *> debug.dict     = 'n';   /+ reading and loading of the dictionary    +/         <* 
    *> debug.dicte    = 'n';   /+ output of english word index             +/         <* 
    *> debug.dictg    = 'n';   /+ output of gregg word index               +/         <*/
   /*---(summary----------------------*/
   /*> debug.summ     = 'n';                                                          <* 
    *> debug.lock     = 'n';                                                          <*/
   /*---(complete)--------------------*/
   return 0;
}

char       /*----: process command line arguments ----------------------------*/
PROG_args          (int argc, char *argv[])
{
   char *a = NULL;
   int   i = 0;
   for (i = 1; i < argc; ++i) {
      a = argv[i];
      DEBUG_C   printf("   processing [%s]\n", a);
      if      (strncmp(a, "@c", 5) == 0) {
         /*> debug_old.top = debug.cli         = 'y';                                 <*/
         printf("command line debugging mode [%s]\n", a);
      }
      /*> else if (strncmp(a, "@x", 5) == 0)  debug.top = debug.prep        = 'y';    <* 
       *> else if (strncmp(a, "@i", 5) == 0)  debug.top = debug.input       = 'y';    <* 
       *> else if (strncmp(a, "@o", 5) == 0)  debug.top = debug.output      = 'y';    <* 
       *> else if (strncmp(a, "@p", 5) == 0)  debug.top = debug.proc        = 'y';    <* 
       *> else if (strncmp(a, "@e", 5) == 0)  debug.top = debug.event       = 'y';    <* 
       *> else if (strncmp(a, "@g", 5) == 0)  debug.top = debug.graph       = 'y';    <* 
       *> else if (strncmp(a, "@@raw"         , 20) == 0) debug.raw         = 'y';    <* 
       *> else if (strncmp(a, "@@bas"         , 20) == 0) debug.bas         = 'y';    <* 
       *> else if (strncmp(a, "@@avg"         , 20) == 0) debug.averages    = 'y';    <* 
       *> else if (strncmp(a, "@@key"         , 20) == 0) debug.key         = 'y';    <* 
       *> else if (strncmp(a, "@@sharp"       , 20) == 0) debug.sharps      = 'y';    <* 
       *> else if (strncmp(a, "@@circle"      , 20) == 0) debug.circles     = 'y';    <* 
       *> else if (strncmp(a, "@@line"        , 20) == 0) debug.lines       = 'y';    <* 
       *> else if (strncmp(a, "@@match"       , 20) == 0) debug.matches     = 'y';    <* 
       *> else if (strncmp(a, "@@shape"       , 20) == 0) debug.shapes      = 'y';    <* 
       *> else if (strncmp(a, "@@curve"       , 20) == 0) debug.curves      = 'y';    <* 
       *> else if (strncmp(a, "@@writing"     , 20) == 0) debug.writing     = 'y';    <* 
       *> else if (strncmp(a, "@@dict"        , 20) == 0) debug.dict        = 'y';    <* 
       *> else if (strncmp(a, "@@dictg"       , 20) == 0) debug.dictg       = 'y';    <* 
       *> else if (strncmp(a, "@@dicte"       , 20) == 0) debug.dicte       = 'y';    <* 
       *> else if (strncmp(a, "@@sum"         , 20) == 0) debug.summ        = 'y';    <* 
       *> else if (strncmp(a, "@@lock"        , 20) == 0) debug.lock        = 'y';    <* 
       *> else if (strncmp(a, "-h"            , 20) == 0) PROG_usage();               <* 
       *> else if (strncmp(a, "--help"        , 20) == 0) PROG_usage();               <* 
       *> else if (a[0] >= '0' && a[0] <= '9')            out_start  = atoi(a);       <*/
      else {
         printf("\noption [%s] not understood\n", a);
         exit (-1);
      }
   }
   DEBUG_C   printf("   done.\n\n");
   return 0;
}

/* PURPOSE : drive the program startup activities                             */
char
PROG_begin(void)
{
   dict_read();
}

/* PURPOSE : drive the program startup activities                             */
char
PROG_final (void)
{
   yXINIT_start(win.title, win.width, win.height, YX_FOCUSABLE, YX_FIXED, debug_old.prep);
   yGOD_start();
   font_load();
   out_init();
   if (out_start > 0) o.curr = out_start;
   dlist_init();
   draw_init();
   return 0;
}


/* PURPOSE : process the xwindows event stream                                */
char
PROG_event()
{
   /*---(local variables)------------------*/
   int   x, y;         /* x,y coordinate of pointer        */
   int   x2, y2;       /* x,y coordinate adjusted for view */
   int   r;            /* radius of point from origin      */
   char  input_type = '-';
   out_read  (o.curr);
   int    i = o.curr;
   int       j         = 0;
   /*---(for keypresses)-------------------*/
   XKeyEvent *key_event;
   char       the_key[5];
   int        the_bytes;
   char       rc;
   /*---(for timer)------------------------*/
   /*> struct     timespec timer, remain;                                             <*/
   /*---(main loop)------------------------*/
   DEBUG_T printf("EVENT LOOP :: begin -------------\n\n");
   while (1) {
      XNextEvent(DISP, &EVNT);
      ++j;
      switch(EVNT.type) {

      case Expose:
         DEBUG_E printf("event (%5d) exposure\n", j);
         draw_main();
         break;

      case ConfigureNotify:
         DEBUG_E printf("event (%5d) configure\n", j);
         if (EVNT.xconfigure.width != X || EVNT.xconfigure.height != Y) {
            DEBUG_E printf("   - moved    x=%4d, y=%4d", X, Y);
            X = EVNT.xconfigure.width;
            Y = EVNT.xconfigure.height;
            DEBUG_E printf("  TO  x=%4d, y=%4d\n", X, Y);
         }
         if (EVNT.xconfigure.width != (int) WIDTH || EVNT.xconfigure.height != (int) HEIGHT) {
            DEBUG_E printf("   - resized  w=%3d, h=%3d\n", EVNT.xconfigure.width, EVNT.xconfigure.height);
            draw_resize(win.width, win.height);
         }
         draw_main();
         break;

      case KeyPress:
         DEBUG_E printf("event (%5d) keypress\n", j);
         i = o.curr;
         /*---(get the key)---------------------*/
         key_event  = (XKeyEvent *) &EVNT;
         the_bytes = XLookupString((XKeyEvent *) &EVNT, the_key, 5, NULL, NULL);
         if (the_bytes < 1) break;
         switch (the_key[0]) {
         case 'h': case 'l': case 'H': case 'L': 
         case 'j': case 'k': case 'J': case 'K':
         case 'd': case 't': case 'D': case 'T':
         case 'p': case 'P': case 16 : case 'v': case 'V': case 22 :
         case 'o': case 'O': case 15 : case 'y': case 'Y': case 25 :
         case 'r': case 'R': case 18 : case 's': case 'S': case 19 :
         case '0': case '1': case '2': case '3':
         case '4': case '5': case '6':
            yGOD_key(the_key[0]);
            break;
         case  ']':
            ++o.curr;
            DEBUG_T  printf("\n\n=== outline %3d ================================\n\n", o.curr);
            break;
         case  '[':
            --o.curr;
            DEBUG_T  printf("\n\n=== outline %3d ================================\n\n", o.curr);
            break;
         case  'Q':              exit(0);
         case  '<': --o.cavg;  break;
         case  '>': ++o.cavg;  break;
         case  'i':
         case  'w':
         case  'x':
                    if (o.saved != 'y') {
                       rc = out_append  ();
                       if (rc == 0) out_read (o.curr);
                    }
                    dict_read();
                    return the_key[0];
                    break;
         }
         /*---(enforce limits)-----------*/
         if (o.cavg < 1      ) o.cavg = 1;
         if (o.cavg > o.navg ) o.cavg = o.navg;
         if (o.curr < 1      ) o.curr = 1;
         if (o.curr > o.total) o.curr = o.total;
         /*---(redraw)------------------*/
         if (i != o.curr)      out_read (o.curr);
         draw_main();
         break;

      case KeyRelease:
         DEBUG_E printf("event (%5d) keyrelease\n", j);
         break;

      case ButtonPress:
         DEBUG_E printf("event (%5d) buttonpress\n", j);
         /*---(get basic info)------------------*/
         x   = EVNT.xbutton.x - win.left + 20;
         y   = win.top - EVNT.xbutton.y  - 6;
         r   = sqrt((x * x) + (y * y));
         /*> printf("touch down at %3dx, %3dy, %3d\n", x, y, r);                      <*/
         input_type = '-';
         if        (r < SIZE_R1) {
            input_type = 'o';
            raw_beg  (x, y);
         } else if (x < 0 && y > 0) {
            input_type = 'p';
            raw_pre  (x, y);
         } else if (x < -50 && y <  -50) {
            input_type = '-';
            if (o.saved != 'y') {
               rc = out_append  ();
               if (rc == 0) out_read (o.curr);
               draw_main();
            }
         } else {
            input_type = 'c';
            raw_cont (x, y);
         }
         /*> printf("input type = %c\n", input_type);                                 <*/
         break;

      case MotionNotify:
         DEBUG_E printf("event (%5d) motion\n", j);
         /*---(get basic info)------------------*/
         x   = EVNT.xbutton.x - win.left + 20;
         y   = win.top - EVNT.xbutton.y  - 6;
         r   = sqrt((x * x) + (y * y));
         /*> printf("moved    to %4dx, %4dy, %dr\n", x, y, r);                     <*/
         if (input_type == '-') break;
         raw_add  (x, y);
         break;

      case ButtonRelease:
         DEBUG_E printf("event (%5d) buttonrelease\n", j);
         /*---(get basic info)------------------*/
         x   = EVNT.xbutton.x - win.left + 20;
         y   = win.top - EVNT.xbutton.y  - 6;
         r   = sqrt((x * x) + (y * y));
         if (input_type == '-') break;
         /*> printf("released at %4dx, %4dy\n", x, y);                             <*/
         raw_end          (x, y);
         /*> rc = out_append  ();                                                     <* 
          *> if (rc == 0) out_read (o.curr);                                          <*/
         DEBUG_E printf("   - bas_filter\n"); fflush(stdout);
         bas_filter    ();
         DEBUG_E printf("   - key_filter\n"); fflush(stdout);
         key_filter    ();
         DEBUG_E printf("   - match_flatter\n"); fflush(stdout);
         match_flatten ();
         DEBUG_E printf("   - match_squeeze\n"); fflush(stdout);
         match_squeeze ();
         DEBUG_E printf("   - circle_driver\n"); fflush(stdout);
         circle_driver ();
         DEBUG_E printf("   - match_sharps\n"); fflush(stdout);
         match_sharps  ();
         DEBUG_E printf("   - match_driver\n"); fflush(stdout);
         match_driver  ();
         DEBUG_E printf("   - draw_main\n"); fflush(stdout);
         draw_main     ();
         DEBUG_E printf("   - done\n"); fflush(stdout);
         break;

      default          :
         DEBUG_E printf("event (%5d) unknown (%d)\n", j, EVNT.type);
         draw_main();
         break;
      }
   }
   DEBUG_T printf("EVENT LOOP :: end ---------------\n\n");
   /*---(complete)------------------------------*/
   return 0;
}

/* PURPOSE : drive the program closure activities                             */
char
PROG_end(void)
{
   font_free();
   yXINIT_end();
   DEBUG_T printf("\npetal writing v03 ----------------------------------end---\n\n");
   return 0;
}


/*============================--------------------============================*/
/*===----                         unit testing                         ----===*/
/*============================--------------------============================*/

#define       LEN_TEXT  2000
char          unit_answer [ LEN_TEXT ];

char*
unit_accessor(char *a_question, int a_num)
{
   if        (strcmp(a_question, "raw")         == 0) {
      if (o.nraw == 0) {
         snprintf(unit_answer, LEN_TEXT, "Raw Point (%4d) : there are no dots", a_num);
      } else if (a_num < 0) {
         snprintf(unit_answer, LEN_TEXT, "Raw Point (%4d) : can not be negative", a_num);
      } else if (a_num >= o.nraw) {
         snprintf(unit_answer, LEN_TEXT, "Raw Point (%4d) : out of range %3d to %3d", a_num, 0, o.nraw - 1);
      } else
         snprintf(unit_answer, LEN_TEXT, "Raw Point (%4d) : %4dx, %4dy, %4dc", a_num, o.raw[a_num].x, o.raw[a_num].y, o.nraw);
   } else if        (strcmp(a_question, "bas")         == 0) {
      if (o.nbas == 0) {
         snprintf(unit_answer, LEN_TEXT, "Bas Point (%4d) : there are no dots", a_num);
      } else if (a_num < 0) {
         snprintf(unit_answer, LEN_TEXT, "Bas Point (%4d) : can not be negative", a_num);
      } else if (a_num >= o.nbas) {
         snprintf(unit_answer, LEN_TEXT, "Bas Point (%4d) : out of range %3d to %3d", a_num, 0, o.nraw - 1);
      } else
         snprintf(unit_answer, LEN_TEXT, "Bas Point (%4d) : %4dx, %4dy, %4dc", a_num, o.raw[a_num].x, o.raw[a_num].y, o.nraw);
   } else if        (strcmp(a_question, "num_raw")     == 0) {
      snprintf(unit_answer, LEN_TEXT, "Raw Count        : %4dc", o.nraw);
   } else if        (strcmp(a_question, "num_bas")     == 0) {
      snprintf(unit_answer, LEN_TEXT, "Bas Count        : %4dc", o.nbas);
   } else if        (strcmp(a_question, "num_avg")     == 0) {
      snprintf(unit_answer, LEN_TEXT, "Avg Count        : %4dc", o.navg);
   } else if        (strcmp(a_question, "num_key")     == 0) {
      snprintf(unit_answer, LEN_TEXT, "Key Count        : %4dc", o.nkey);
   } else if        (strcmp(a_question, "prepared")    == 0) {
      snprintf(unit_answer, LEN_TEXT, "Prepared  (%4d) : %s", o.curr, o.grade);
   }
   return unit_answer;
}


/*============================----(source-end)----============================*/
