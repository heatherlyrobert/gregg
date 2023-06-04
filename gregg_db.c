/*============================---(source-start)---============================*/
#include "gregg.h"


typedef   struct cFILES tFILES;
struct cFILES {
   char        name        [LEN_HUND];
   short       count;
};
static tFILES  s_files       [LEN_DESC];


/*====================------------------------------------====================*/
/*===----                     header handling                          ----===*/
/*====================------------------------------------====================*/
static void  o___HEADER__________o () { return; }

char
DB__head_write_one       (FILE *a_file, char a_label [LEN_TERSE], int a_var)
{
   char        rc          =    0;
   rc = fwrite (&a_var, sizeof (int), 1, a_file);
   DEBUG_OUTP   yLOG_complex (a_label     , "%4d %d", rc, a_var);
   return rc;
}

char
DB__head_write          (FILE *a_file, char a_name [LEN_LABEL], char a_vernum [LEN_LABEL], int a_nletter, int a_nprefix, int a_nsuffix, int a_nsource, int a_nbase, int a_nword, char a_heart [LEN_DESC])
{
   /*---(locals)-----------+-----+-----+-*/
   char        rce         =  -10;
   char        rc          =    0;
   int         i           =    0;
   char        t           [LEN_HUND]  = "";
   /*---(header)-------------------------*/
   DEBUG_OUTP   yLOG_enter   (__FUNCTION__);
   /*---(defense)------------------------*/
   DEBUG_OUTP  yLOG_point   ("a_file"    , a_file);
   --rce;  if (a_file == NULL) {
      DEBUG_OUTP  yLOG_exitr   (__FUNCTION__, rce);
      return rce;
   }
   /*---(name)------------------------*/
   for (i = 0; i < LEN_LABEL; i++)  t [i] = ' ';
   strlcpy (t, a_name, LEN_LABEL);
   rc = fwrite (t, LEN_LABEL, 1, a_file);
   DEBUG_OUTP   yLOG_complex ("name"      , "%4d %s", rc, t);
   /*---(version)---------------------*/
   for (i = 0; i < LEN_LABEL; i++)  t [i] = ' ';
   strlcpy (t, a_vernum  , LEN_SHORT);
   rc = fwrite (t, LEN_SHORT, 1, a_file);
   DEBUG_OUTP   yLOG_complex ("vernum"    , "%4d %s", rc, t);
   /*---(stats)-----------------------*/
   rc = DB__head_write_one (a_file, "letters" , a_nletter);
   rc = DB__head_write_one (a_file, "prefixes", a_nprefix);
   rc = DB__head_write_one (a_file, "suffixes", a_nsuffix);
   rc = DB__head_write_one (a_file, "sources" , a_nsource);
   rc = DB__head_write_one (a_file, "bases"   , a_nbase);
   rc = DB__head_write_one (a_file, "words"   , a_nword);
   /*---(heartbeat)-------------------*/
   for (i = 0; i < LEN_DESC;  i++)  t [i] = '·';
   strlcpy (t, a_heart    , LEN_DESC);
   rc = fwrite (t, LEN_DESC , 1, a_file);
   DEBUG_OUTP   yLOG_complex ("heart"     , "%4d %s", rc, t);
   /*---(force flush)-----------------*/
   fflush (a_file);
   /*---(complete)-----------------------*/
   DEBUG_OUTP   yLOG_exit    (__FUNCTION__);
   return 0;
}

char
DB__head_read_one       (FILE *a_file, char a_label [LEN_TERSE], int *r_var)
{
   char        rc          =    0;
   int         a           =    0;
   rc = fread  (&a, sizeof (int), 1, a_file);
   DEBUG_INPT   yLOG_complex (a_label     , "%4d %d", rc, a);
   if (r_var != NULL)  *r_var = a;
   return 0;
}

char
DB__head_read           (FILE *a_file, char r_name [LEN_LABEL], char r_vernum [LEN_LABEL], int *r_nletter, int *r_nprefix, int *r_nsuffix, int *r_nsource, int *r_nbase, int *r_nword, char r_heart [LEN_DESC])
{
   /*---(locals)-----------+-----+-----+-*/
   char        rce         =  -10;
   char        rc          =    0;
   char        t           [LEN_HUND]  = "";
   int         a           =    0;
   /*---(header)-------------------------*/
   DEBUG_INPT   yLOG_enter   (__FUNCTION__);
   /*---(defense)------------------------*/
   DEBUG_INPT  yLOG_point   ("a_file"    , a_file);
   --rce;  if (a_file == NULL) {
      DEBUG_INPT  yLOG_exitr   (__FUNCTION__, rce);
      return rce;
   }
   /*---(name)------------------------*/
   rc = fread  (&t, LEN_LABEL, 1, a_file);
   DEBUG_INPT   yLOG_complex ("name"      , "%4d %s", rc, t);
   if (r_name    != NULL)  strlcpy (r_name   , t, LEN_LABEL);
   /*---(version)---------------------*/
   rc = fread  (&t, LEN_LABEL, 1, a_file);
   DEBUG_INPT   yLOG_complex ("version"   , "%4d %s", rc, t);
   if (r_vernum  != NULL)  strlcpy (r_vernum , t, LEN_LABEL);
   /*---(stats)-----------------------*/
   rc = DB__head_read_one  (a_file, "letters" , &a);
   if (r_nletter != NULL)  *r_nletter = a;
   rc = DB__head_read_one  (a_file, "prefixes", &a);
   if (r_nprefix != NULL)  *r_nprefix = a;
   rc = DB__head_read_one  (a_file, "suffixes", &a);
   if (r_nsuffix != NULL)  *r_nsuffix = a;
   rc = DB__head_read_one  (a_file, "sources" , &a);
   if (r_nsource != NULL)  *r_nsource = a;
   rc = DB__head_read_one  (a_file, "bases"   , &a);
   if (r_nbase   != NULL)  *r_nbase   = a;
   rc = DB__head_read_one  (a_file, "words"   , &a);
   if (r_nword   != NULL)  *r_nword   = a;
   /*---(heartbeat)-------------------*/
   rc = fread  (t, LEN_DESC , 1, a_file);
   DEBUG_INPT   yLOG_complex ("heart"     , "%4d %s", rc, t);
   /*---(complete)-----------------------*/
   DEBUG_INPT   yLOG_exit    (__FUNCTION__);
   return 0;
}



/*============================--------------------============================*/
/*===----                         source handling                      ----===*/
/*============================--------------------============================*/
static void      o___SOURCE__________________o (void) {;}

char
DB_source_add           (char a_file [LEN_HUND])
{
   /*---(locals)-------------------------*/
   char        rce         =  -10;
   /*---(header)-------------------------*/
   DEBUG_CONF   yLOG_enter   (__FUNCTION__);
   /*---(defense)------------------------*/
   DEBUG_CONF   yLOG_point   ("a_file"    , a_file);
   --rce;  if (a_file == NULL) {
      DEBUG_CONF   yLOG_exitr   (__FUNCTION__, rce);
      return rce;
   }
   /*---(copy)---------------------------*/
   strlcpy (s_files [my.r_nfile].name, a_file, LEN_HUND);
   s_files [my.r_nfile].count = 0;
   ++my.r_nfile;
   /*---(complete)-----------------------*/
   DEBUG_CONF   yLOG_exit    (__FUNCTION__);
   return 0;
}

char
DB_source_purge         (void)
{
   /*---(locals)-------------------------*/
   int         i           =    0;
   /*---(header)-------------------------*/
   DEBUG_CONF   yLOG_enter   (__FUNCTION__);
   /*---(remove)-------------------------*/
   for (i = 0; i < LEN_DESC; ++i) {
      strlcpy (s_files [i].name, "", LEN_HUND);
      s_files [i].count = 0;
   }
   my.r_nfile = 0;
   /*---(complete)-----------------------*/
   DEBUG_CONF   yLOG_exit    (__FUNCTION__);
   return 0;
}

char
DB__source_write        (FILE *a_file)
{
   /*---(locals)-----------+-----+-----+-*/
   char        rce         =  -10;
   int         i           =    0;
   /*---(header)-------------------------*/
   DEBUG_OUTP   yLOG_enter   (__FUNCTION__);
   /*---(remove)-------------------------*/
   DEBUG_OUTP   yLOG_value   ("r_nfile"   , my.r_nfile);
   for (i = 0; i < my.r_nfile; ++i) {
      DEBUG_OUTP   yLOG_info    ("write"     , s_files [i].name);
      fwrite (s_files [i].name, LEN_HUND, 1, a_file);
   }
   /*---(complete)-----------------------*/
   DEBUG_OUTP   yLOG_exit    (__FUNCTION__);
   return 0;
}

short DB_source_inc      (void)  { return ++(s_files [my.r_nfile - 1].count); }

char
DB__source_read         (int n, FILE *a_file)
{
   /*---(locals)-----------+-----+-----+-*/
   char        rce         =  -10;
   char        rc          =    0;
   int         i           =    0;
   char        t           [LEN_DESC]  = "";
   /*---(header)-------------------------*/
   DEBUG_INPT   yLOG_enter   (__FUNCTION__);
   /*---(walk sources)-------------------*/
   DEBUG_INPT   yLOG_value   ("n"         , n);
   for (i = 0; i < n; ++i) {
      /*---(read)---------------------------*/
      fread  (t, LEN_DESC, 1, a_file);
      DEBUG_OUTP   yLOG_complex ("x_raw"     , "%2d#, %s", i, t);
      DB_source_add (t);
      /*---(done)---------------------------*/
   }
   /*---(complete)-----------------------*/
   DEBUG_INPT   yLOG_exit    (__FUNCTION__);
   return 0;
}



/*====================------------------------------------====================*/
/*===----                      word handling                           ----===*/
/*====================------------------------------------====================*/
static void  o___WORDS___________o () { return; }

char
DB__word_write_one      (FILE *a_file, tWORD *a_curr)
{
   /*---(header)-------------------------*/
   DEBUG_OUTP   yLOG_senter  (__FUNCTION__);
   /*---(prepare)------------------------*/
   fwrite (a_curr, sizeof (tWORD), 1, a_file);
   /*---(complete)-----------------------*/
   DEBUG_OUTP   yLOG_sexit   (__FUNCTION__);
   return 0;
}

char
DB__word_write          (FILE *a_file)
{
   /*---(locals)-----------+-----+-----+-*/
   char        rc          =    0;
   /*---(header)-------------------------*/
   DEBUG_OUTP   yLOG_senter  (__FUNCTION__);
   /*---(prepare)------------------------*/
   rc = WORDS_eng_all  (a_file, DB__word_write_one);
   /*---(complete)-----------------------*/
   DEBUG_OUTP   yLOG_sexit   (__FUNCTION__);
   return rc;
}

char
DB__word_read           (int n, FILE *a_file)
{
   /*---(locals)-----------+-----+-----+-*/
   char        rce         =  -10;
   char        rc          =    0;
   int         i           =    0;
   tWORD      *x_new       = NULL;
   tWORD      *x_base      = NULL;
   tWORD      *x_last      = NULL;
   /*---(header)-------------------------*/
   DEBUG_INPT   yLOG_enter   (__FUNCTION__);
   /*---(walk projects)------------------*/
   DEBUG_INPT   yLOG_value   ("n"         , n);
   for (i = 0; i < n; ++i) {
      rc = WORDS_new ('y', &x_new);
      /*---(read)---------------------------*/
      rc = fread  (x_new, sizeof (tWORD), 1, a_file);
      DEBUG_OUTP   yLOG_complex ("x_new"     , "%3d#, %4d, %-20.20s, %s", i, rc, x_new->w_english, x_new->w_vary);
      if (rc != 1) {
         DEBUG_INPT   yLOG_note    ("end-of-file");
         break;
      }
      /*---(cleanse)------------------------*/
      x_new->w_base    = NULL;
      x_new->w_next    = NULL;
      x_new->ysort_e   = NULL;
      x_new->ysort_g   = NULL;
      x_new->ysort_u   = NULL;
      x_new->ysort_t   = NULL;
      /*---(handle bases)-------------------*/
      if (strcmp (x_new->w_vary, "<") == 0) {
         DEBUG_INPT   yLOG_note    ("handling root");
         x_new->w_base = NULL;
         x_base  = x_new;
         ++(my.r_nbase);
      } else {
         DEBUG_INPT   yLOG_note    ("handling variation");
         x_new->w_base  = x_base;
         if (x_last != NULL)  x_last->w_next = x_new;
      }
      x_last      = x_new;
      /*---(hook)---------------------------*/
      rc = WORDS_hook (x_new);
      rc = FIX_hook   (x_new);
      /*---(stats)--------------------------*/
      ++(my.r_nword);
      /*---(done)---------------------------*/
   }
   /*---(complete)-----------------------*/
   DEBUG_INPT   yLOG_exit    (__FUNCTION__);
   return 0;
}



/*====================------------------------------------====================*/
/*===----                      file control                            ----===*/
/*====================------------------------------------====================*/
static void  o___FILES___________o () { return; }

char
DB__open                (char a_name [LEN_PATH], char a_mode, int *b_nfile, int *b_nbase, int *b_nword, char b_heart [LEN_DESC], FILE **b_file)
{
   /*---(locals)-----------+-----+-----+-*/
   char        rce         =  -10;
   char        x_mode      [LEN_TERSE] = "";
   int         n           =    0;
   char        t           [LEN_HUND]  = "";
   FILE       *f           = NULL;
   /*---(header)-------------------------*/
   DEBUG_FILE   yLOG_enter   (__FUNCTION__);
   /*---(report out)---------------------*/
   DEBUG_OUTP   yLOG_complex ("actual"    , "A  %4df  %4db  %4dw  /  R  %4df  %4db  %4dw  /  Y  %4de  %4dg  %4du  %4dt", my.a_nfile, my.a_nbase, my.a_nword, my.r_nfile, my.r_nbase, my.r_nword, ySORT_count (B_ENGLISH), ySORT_count (B_GREGG), ySORT_count (B_UNIQUE), ySORT_count (B_BASE));
   /*---(default)------------------------*/
   if (a_mode == 'r') {
      if (b_nfile != NULL)  *b_nfile = 0;
      if (b_nbase != NULL)  *b_nbase = 0;
      if (b_nword != NULL)  *b_nword = 0;
   }
   /*---(defense)------------------------*/
   DEBUG_FILE   yLOG_point   ("a_name"    , a_name);
   --rce;  if (a_name      == NULL) {
      DEBUG_FILE   yLOG_exitr   (__FUNCTION__, rce);
      return rce;
   }
   DEBUG_FILE   yLOG_info    ("a_name"    , a_name);
   DEBUG_FILE   yLOG_char    ("a_mode"    , a_mode);
   --rce;  if (a_mode == 0 || strchr ("wr", a_mode) == NULL) {
      DEBUG_FILE   yLOG_exitr   (__FUNCTION__, rce);
      return rce;
   }
   DEBUG_FILE   yLOG_point   ("b_nfile"   , b_nfile);
   --rce;  if (b_nfile     == NULL) {
      DEBUG_FILE   yLOG_exitr   (__FUNCTION__, rce);
      return rce;
   }
   DEBUG_FILE   yLOG_point   ("b_nbase"   , b_nbase);
   --rce;  if (b_nbase     == NULL) {
      DEBUG_FILE   yLOG_exitr   (__FUNCTION__, rce);
      return rce;
   }
   DEBUG_FILE   yLOG_point   ("b_nword"   , b_nword);
   --rce;  if (b_nword     == NULL) {
      DEBUG_FILE   yLOG_exitr   (__FUNCTION__, rce);
      return rce;
   }
   DEBUG_FILE   yLOG_point   ("b_heart"   , b_heart);
   --rce;  if (b_heart     == NULL) {
      DEBUG_FILE   yLOG_exitr   (__FUNCTION__, rce);
      return rce;
   }
   DEBUG_FILE   yLOG_point   ("b_file"    , b_file);
   --rce;  if (b_file      == NULL) {
      DEBUG_FILE   yLOG_exitr   (__FUNCTION__, rce);
      return rce;
   }
   DEBUG_INPT   yLOG_point   ("*b_file"   , *b_file);
   --rce;  if (*b_file != NULL) {
      DEBUG_FILE   yLOG_exitr   (__FUNCTION__, rce);
      return rce;
   }
   /*---(set mode)-----------------------*/
   DEBUG_FILE   yLOG_char    ("a_mode"    , a_mode);
   --rce;  switch (a_mode) {
   case 'r' :
      strlcpy (x_mode, "rb", LEN_TERSE);
      break;
   case 'w' :
      strlcpy (x_mode, "wb", LEN_TERSE);
      break;
   default  :
      DEBUG_FILE   yLOG_exitr   (__FUNCTION__, rce);
      return rce;
   }
   DEBUG_FILE   yLOG_info    ("x_mode"    , x_mode);
   /*---(open)---------------------------*/
   DEBUG_FILE   yLOG_info    ("a_name"    , a_name);
   yURG_msg ('-', "database is å%sæ", a_name);
   f = fopen (a_name, x_mode);
   DEBUG_FILE   yLOG_point   ("f"         , f);
   --rce;  if (f == NULL) {
      DEBUG_FILE   yLOG_exitr   (__FUNCTION__, rce);
      return rce;
   }
   /*---(reading)------------------------*/
   if (a_mode == 'r') {
      /*---(name)------------------------*/
      fread  (&(my.a_name)  , LEN_LABEL, 1, f);
      DEBUG_FILE   yLOG_info    ("name"      , my.a_name);
      /*---(version)---------------------*/
      fread  (&(my.a_ver), LEN_SHORT, 1, f);
      DEBUG_FILE   yLOG_info    ("ver"       , my.a_ver);
      /*---(stats)-----------------------*/
      DB__head_read_one  (f, "files", b_nfile);
      DB__head_read_one  (f, "bases", b_nbase);
      DB__head_read_one  (f, "words", b_nword);
      /*---(heartbeat)-------------------*/
      fread  (b_heart    , LEN_DESC, 1, f);
      DEBUG_FILE   yLOG_info    ("heartbeat" , b_heart);
      /*---(done)------------------------*/
   }
   /*---(writing)------------------------*/
   else if (a_mode == 'w') {
      /*---(name)------------------------*/
      for (n = 0; n < LEN_LABEL; n++)  t [n] = ' ';
      strlcpy (t, P_BASENAME, LEN_LABEL);
      fwrite (t, LEN_LABEL, 1, f);
      DEBUG_FILE   yLOG_info    ("name"      , t);
      /*---(version)---------------------*/
      for (n = 0; n < LEN_LABEL; n++)  t [n] = ' ';
      strlcpy (t, P_VERNUM  , LEN_SHORT);
      fwrite (t, LEN_SHORT, 1, f);
      DEBUG_FILE   yLOG_info    ("ver"       , t);
      /*---(stats)-----------------------*/
      DB__head_write_one (f, "files", *b_nfile);
      DB__head_write_one (f, "bases", *b_nbase);
      DB__head_write_one (f, "words", *b_nword);
      /*---(heartbeat)-------------------*/
      for (n = 0; n < LEN_DESC;  n++)  t [n] = '·';
      strlcpy (t, b_heart    , LEN_DESC);
      fwrite (t, LEN_DESC , 1, f);
      /*---(done)------------------------*/
      fflush (f);
   }
   /*---(report out)---------------------*/
   DEBUG_OUTP   yLOG_complex ("actual"    , "A  %4df  %4db  %4dw  /  R  %4df  %4db  %4dw  /  Y  %4de  %4dg  %4du  %4db", my.a_nfile, my.a_nbase, my.a_nword, my.r_nfile, my.r_nbase, my.r_nword, ySORT_count (B_ENGLISH), ySORT_count (B_GREGG), ySORT_count (B_UNIQUE), ySORT_count (B_BASE));
   /*---(save-back)----------------------*/
   if (b_file != NULL)  *b_file = f;
   /*---(complete)-----------------------*/
   DEBUG_FILE   yLOG_exit    (__FUNCTION__);
   return 0;
}

char
DB__close               (FILE **b_file)
{
   /*---(locals)-----------+-----+-----+-*/
   char        rce         =  -10;
   char        rc          =    0;
   /*---(header)-------------------------*/
   DEBUG_FILE   yLOG_enter   (__FUNCTION__);
   /*---(defense)------------------------*/
   DEBUG_INPT   yLOG_point   ("b_file"    , b_file);
   --rce;  if (b_file == NULL) {
      DEBUG_FILE   yLOG_exitr   (__FUNCTION__, rce);
      return rce;
   }
   DEBUG_INPT   yLOG_point   ("*b_file"   , *b_file);
   --rce;  if (*b_file == NULL) {
      DEBUG_FILE   yLOG_exitr   (__FUNCTION__, rce);
      return rce;
   }
   /*---(close)--------------------------*/
   rc = fclose (*b_file);
   DEBUG_OUTP   yLOG_value   ("close"     , rc);
   --rce;  if (rc < 0) {
      DEBUG_FILE   yLOG_exitr   (__FUNCTION__, rce);
      return rce; 
   }
   /*---(ground pointer)-----------------*/
   *b_file = NULL;
   DEBUG_INPT   yLOG_point   ("*b_file"   , *b_file);
   /*---(complete)-----------------------*/
   DEBUG_FILE   yLOG_exit    (__FUNCTION__);
   return 0;
}



/*====================------------------------------------====================*/
/*===----                        main driver                           ----===*/
/*====================------------------------------------====================*/
static void  o___DRIVER__________o () { return; }

char
DB_write                (void)
{
   /*---(locals)-----------+-----+-----+-*/
   char        rce         =  -10;
   char        rc          =    0;
   FILE       *f           = NULL;
   /*---(header)-------------------------*/
   DEBUG_OUTP   yLOG_enter   (__FUNCTION__);
   yURG_msg ('>', "write central database (write)...");
   /*---(open)---------------------------*/
   rc = DB__open ("/var/lib/gregg/gregg.db", 'w', &(my.r_nfile), &(my.r_nbase), &(my.r_nword), my.heartbeat, &f);
   DEBUG_OUTP   yLOG_value   ("open"      , rc);
   --rce;  if (rc < 0) {
      yURG_err ('f', "database could not be openned for writing");
      DEBUG_OUTP   yLOG_exitr   (__FUNCTION__, rce);
      return rce;
   }
   /*---(sources)------------------------*/
   rc = DB__source_write (f);
   DEBUG_OUTP   yLOG_value   ("sources"   , rc);
   --rce;  if (rc < 0) {
      yURG_err ('f', "database could not write sources");
      rc = DB__close (&f);
      DEBUG_OUTP   yLOG_exitr   (__FUNCTION__, rce);
      return rce;
   }
   /*---(words)--------------------------*/
   rc = DB__word_write   (f);
   DEBUG_OUTP   yLOG_value   ("words"     , rc);
   --rce;  if (rc < 0) {
      yURG_err ('f', "database could not write words");
      rc = DB__close (&f);
      DEBUG_OUTP   yLOG_exitr   (__FUNCTION__, rce);
      return rce;
   }
   /*---(close)--------------------------*/
   rc = DB__close (&f);
   DEBUG_OUTP   yLOG_value   ("close"     , rc);
   --rce;  if (rc < 0) {
      yURG_err ('f', "database could not be closed after writing");
      DEBUG_OUTP   yLOG_exitr   (__FUNCTION__, rce);
      return rce;
   }
   /*---(summary)------------------------*/
   DEBUG_OUTP   yLOG_complex ("actual"    , "A  %4df  %4db  %4dw  /  R  %4df  %4db  %4dw  /  Y  %4de  %4dg  %4du  %4dt", my.a_nfile, my.a_nbase, my.a_nword, my.r_nfile, my.r_nbase, my.r_nword, ySORT_count (B_ENGLISH), ySORT_count (B_GREGG), ySORT_count (B_UNIQUE), ySORT_count (B_BASE));
   yURG_msg ('-', "actual %4df  %4db  %4dw", my.r_nfile, my.r_nbase, my.r_nword);
   yURG_msg ('-', "success, current database saved correctly");
   yURG_msg (' ', "");
   /*---(complete)-----------------------*/
   DEBUG_OUTP   yLOG_exit    (__FUNCTION__);
   return 0; }

char
DB_read                 (void)
{
   /*---(locals)-----------+-----+-----+-*/
   char        rce         =  -10;
   char        rc          =    0;
   FILE       *f           = NULL;
   /*---(header)-------------------------*/
   DEBUG_OUTP   yLOG_enter   (__FUNCTION__);
   yURG_msg ('>', "read central database (read)...");
   /*---(open)---------------------------*/
   rc = DB__open ("/var/lib/gregg/gregg.db", 'r', &(my.a_nfile), &(my.a_nbase), &(my.a_nword), my.a_heart, &f);
   DEBUG_OUTP   yLOG_value   ("open"      , rc);
   --rce;  if (rc < 0) {
      yURG_err ('f', "database could not be openned for reading");
      DEBUG_OUTP   yLOG_exitr   (__FUNCTION__, rce);
      return rce;
   }
   /*---(sources)------------------------*/
   rc = DB__source_read  (my.a_nfile, f);
   DEBUG_OUTP   yLOG_value   ("sources"   , rc);
   --rce;  if (rc < 0) {
      yURG_err ('f', "database could not read sources");
      rc = DB__close (&f);
      DEBUG_OUTP   yLOG_exitr   (__FUNCTION__, rce);
      return rce;
   }
   /*---(words)--------------------------*/
   rc = DB__word_read    (my.a_nword, f);
   DEBUG_OUTP   yLOG_value   ("words"     , rc);
   --rce;  if (rc < 0) {
      yURG_err ('f', "database could not read words");
      rc = DB__close (&f);
      DEBUG_OUTP   yLOG_exitr   (__FUNCTION__, rce);
      return rce;
   }
   /*---(close)--------------------------*/
   rc = DB__close (&f);
   DEBUG_OUTP   yLOG_value   ("close"     , rc);
   --rce;  if (rc < 0) {
      yURG_err ('f', "database could not be closed after reading");
      DEBUG_OUTP   yLOG_exitr   (__FUNCTION__, rce);
      return rce;
   }
   /*---(summary)------------------------*/
   DEBUG_OUTP   yLOG_complex ("actual"    , "A  %4df  %4db  %4dw  /  R  %4df  %4db  %4dw  /  Y  %4de  %4dg  %4du  %4dt", my.a_nfile, my.a_nbase, my.a_nword, my.r_nfile, my.r_nbase, my.r_nword, ySORT_count (B_ENGLISH), ySORT_count (B_GREGG), ySORT_count (B_UNIQUE), ySORT_count (B_BASE));
   yURG_msg ('-', "actual %4df  %4db  %4dw", my.a_nfile, my.a_nbase, my.a_nword);
   /*---(paginate)-----------------------*/
   DICT_paginate ();
   yURG_msg ('-', "data paginated");
   yMAP_refresh_full ();
   yURG_msg ('-', "refresh mapping");
   /*---(final)--------------------------*/
   yURG_msg ('-', "success, current database read correctly");
   yURG_msg (' ', "");
   /*---(complete)-----------------------*/
   DEBUG_OUTP   yLOG_exit    (__FUNCTION__);
   return 0;
}

char
DB_stats                (void)
{
   /*---(locals)-----------+-----+-----+-*/
   char        rce         =  -10;
   char        rc          =    0;
   FILE       *f           = NULL;
   char        t           [LEN_LABEL] = "";
   /*---(header)-------------------------*/
   DEBUG_OUTP   yLOG_enter   (__FUNCTION__);
   DEBUG_PROG    yLOG_note    ("database statistics action (YJOBS_STATS)");
   /*---(open)---------------------------*/
   rc = DB__open ("/var/lib/gregg/gregg.db", 'r', &(my.a_nfile), &(my.a_nbase), &(my.a_nword), my.a_heart, &f);
   DEBUG_OUTP   yLOG_value   ("open"      , rc);
   --rce;  if (rc < 0) {
      yURG_err ('f', "database could not be openned for reading");
      DEBUG_OUTP   yLOG_exitr   (__FUNCTION__, rce);
      return rce;
   }
   /*---(close)--------------------------*/
   rc = DB__close (&f);
   DEBUG_OUTP   yLOG_value   ("close"     , rc);
   --rce;  if (rc < 0) {
      yURG_err ('f', "database could not be closed after reading");
      DEBUG_OUTP   yLOG_exitr   (__FUNCTION__, rce);
      return rce;
   }
   /*---(display)------------------------*/
   printf ("#!/usr/local/bin/gregg --stats\n");
   printf ("db     å%sæ\n" , "/var/lib/gregg/gregg.db");
   printf ("name   å%sæ\n" , my.a_name);
   printf ("ver    å%sæ\n" , my.a_ver);
   strl4main (my.a_nfile , t , 0, 'c', '-', LEN_LABEL);
   printf ("file   %7.7s\n", t);
   strl4main (my.a_nbase , t , 0, 'c', '-', LEN_LABEL);
   printf ("base   %7.7s\n", t);
   strl4main (my.a_nword , t , 0, 'c', '-', LEN_LABEL);
   printf ("word   %7.7s\n", t);
   printf ("stamp  å%sæ\n" , my.a_heart);
   DEBUG_OUTP   yLOG_complex ("actual"    , "A  %4df  %4db  %4dw  /  R  %4df  %4db  %4dw  /  Y  %4de  %4dg  %4du  %4dt", my.a_nfile, my.a_nbase, my.a_nword, my.r_nfile, my.r_nbase, my.r_nword, ySORT_count (B_ENGLISH), ySORT_count (B_GREGG), ySORT_count (B_UNIQUE), ySORT_count (B_BASE));
   /*---(complete)-----------------------*/
   DEBUG_OUTP   yLOG_exit    (__FUNCTION__);
   return 0;
}



/*====================------------------------------------====================*/
/*===----                         debugging                            ----===*/
/*====================------------------------------------====================*/
static void  o___DEBUGGING_______o () { return; }

char DB__source_count   (void)  { return my.r_nfile; }

char*
DB__source_detail       (char n)
{
   /*---(locals)-----------+-----+-----+-*/
   char        rce         =  -10;
   /*---(defense)------------------------*/
   strcpy (g_print, "n/a");
   if (n <  0 || n >= my.r_nfile)  return g_print;
   /*---(prepare)------------------------*/
   sprintf (g_print, "%-2d  %-5d  %s", n, s_files [n].count, s_files [n].name);
   /*---(complete)-----------------------*/
   return g_print;
}



