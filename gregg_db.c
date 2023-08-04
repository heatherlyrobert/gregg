/*============================---(source-start)---============================*/
#include "gregg.h"



typedef   struct cSOURCE tSOURCE;
struct cSOURCE {
   char        name        [LEN_HUND];
   short       count;
};
static tSOURCE  s_source   [LEN_DESC];
static char     s_nsource  = 0;



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
DB__head_write          (FILE *a_file, char a_name [LEN_LABEL], char a_ver [LEN_LABEL], int a_nlet, int a_npre, int a_nsuf, int a_nsrc, int a_nbas, int a_ndic, char a_heart [LEN_DESC])
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
   strlcpy (t, a_ver  , LEN_SHORT);
   rc = fwrite (t, LEN_LABEL, 1, a_file);
   DEBUG_OUTP   yLOG_complex ("vernum"    , "%4d %s", rc, t);
   /*---(stats)-----------------------*/
   rc = DB__head_write_one (a_file, "letters" , a_nlet);
   rc = DB__head_write_one (a_file, "prefixes", a_npre);
   rc = DB__head_write_one (a_file, "suffixes", a_nsuf);
   rc = DB__head_write_one (a_file, "sources" , a_nsrc);
   rc = DB__head_write_one (a_file, "bases"   , a_nbas);
   rc = DB__head_write_one (a_file, "words"   , a_ndic);
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
DB__head_read           (FILE *a_file, char r_name [LEN_LABEL], char r_ver [LEN_LABEL], int *r_nlet, int *r_npre, int *r_nsuf, int *r_nsrc, int *r_nbas, int *r_ndic, char r_heart [LEN_DESC])
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
   if (r_ver  != NULL)  strlcpy (r_ver , t, LEN_LABEL);
   /*---(stats)-----------------------*/
   rc = DB__head_read_one  (a_file, "letters" , &a);
   if (r_nlet != NULL)  *r_nlet = a;
   rc = DB__head_read_one  (a_file, "prefixes", &a);
   if (r_npre != NULL)  *r_npre = a;
   rc = DB__head_read_one  (a_file, "suffixes", &a);
   if (r_nsuf != NULL)  *r_nsuf = a;
   rc = DB__head_read_one  (a_file, "sources" , &a);
   if (r_nsrc != NULL)  *r_nsrc = a;
   rc = DB__head_read_one  (a_file, "bases"   , &a);
   if (r_nbas   != NULL)  *r_nbas   = a;
   rc = DB__head_read_one  (a_file, "dicts"   , &a);
   if (r_ndic   != NULL)  *r_ndic   = a;
   /*---(heartbeat)-------------------*/
   rc = fread  (t, LEN_DESC , 1, a_file);
   DEBUG_INPT   yLOG_complex ("heart"     , "%4d %s", rc, t);
   if (r_heart   != NULL)  strlcpy (r_heart  , t, LEN_DESC);
   /*---(complete)-----------------------*/
   DEBUG_INPT   yLOG_exit    (__FUNCTION__);
   return 0;
}



/*============================--------------------============================*/
/*===----                         source handling                      ----===*/
/*============================--------------------============================*/
static void      o___SOURCE__________________o (void) {;}

char
SOURCE_add              (char a_file [LEN_HUND])
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
   strlcpy (s_source [s_nsource].name, a_file, LEN_HUND);
   s_source [s_nsource].count = 0;
   ++s_nsource;
   /*---(complete)-----------------------*/
   DEBUG_CONF   yLOG_exit    (__FUNCTION__);
   return 0;
}

char
SOURCE__purge           (void)
{
   /*---(locals)-------------------------*/
   int         i           =    0;
   /*---(header)-------------------------*/
   DEBUG_CONF   yLOG_enter   (__FUNCTION__);
   /*---(remove)-------------------------*/
   for (i = 0; i < LEN_DESC; ++i) {
      strlcpy (s_source [i].name, "", LEN_HUND);
      s_source [i].count = 0;
   }
   s_nsource = 0;
   /*---(complete)-----------------------*/
   DEBUG_CONF   yLOG_exit    (__FUNCTION__);
   return 0;
}

char
SOURCE_write            (FILE *a_file)
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
   DEBUG_OUTP  yLOG_value   ("r_nfile"   , s_nsource);
   --rce;  if (s_nsource <= 0) {
      DEBUG_OUTP  yLOG_exitr   (__FUNCTION__, rce);
      return rce;
   }
   /*---(write all)----------------------*/
   for (i = 0; i < s_nsource; ++i) {
      rc = fwrite (&(s_source [i]), sizeof (tSOURCE), 1, a_file);
      if (rc != 1)   break;
   }
   /*---(check)--------------------------*/
   DEBUG_OUTP  yLOG_value   ("i"         , i);
   --rce;  if (s_nsource != i) {
      DEBUG_OUTP  yLOG_exitr   (__FUNCTION__, rce);
      return rce;
   }
   /*---(complete)-----------------------*/
   DEBUG_OUTP   yLOG_exit    (__FUNCTION__);
   return 0;
}

char
SOURCE_read             (FILE *a_file, short a_count)
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
   DEBUG_OUTP  yLOG_value   ("r_nfile"   , s_nsource);
   --rce;  if (s_nsource > 0) {
      DEBUG_OUTP  yLOG_exitr   (__FUNCTION__, rce);
      return rce;
   }
   /*---(write all)----------------------*/
   for (i = 0; i < a_count; ++i) {
      rc = fread  (&(s_source [i]), sizeof (tSOURCE), 1, a_file);
      if (rc != 1)   break;
      ++s_nsource;
   }
   /*---(check)--------------------------*/
   DEBUG_OUTP  yLOG_value   ("a_count"   , a_count);
   --rce;  if (s_nsource != a_count) {
      DEBUG_OUTP  yLOG_exitr   (__FUNCTION__, rce);
      return rce;
   }
   /*---(complete)-----------------------*/
   DEBUG_OUTP   yLOG_exit    (__FUNCTION__);
   return 0;
}

short SOURCE_inc         (void)  { ++(s_source [s_nsource - 1].count); return 0; }



/*====================------------------------------------====================*/
/*===----                        main driver                           ----===*/
/*====================------------------------------------====================*/
static void  o___DRIVER__________o () { return; }

char
DB_write                (char a_name [LEN_HUND])
{
   /*---(locals)-----------+-----+-----+-*/
   char        rce         =  -10;
   char        rc          =    0;
   FILE       *x_db        = NULL;
   int         x_nlet, x_npre, x_nsuf, x_nsrc, x_nbas, x_ndic;
   /*---(header)-------------------------*/
   DEBUG_OUTP   yLOG_enter   (__FUNCTION__);
   yURG_msg ('>', "write central database (write)...");
   /*---(collect data)-------------------*/
   x_nlet  = LETTER_count ();
   x_npre  = PREFIX__count ();
   x_nsuf  = SUFFIX__count ();
   x_nsrc  = SOURCE__count ();
   x_nbas  = BASE__count ();
   x_ndic  = DICT__count ();
   /*---(open)---------------------------*/
   rc = SHARED_open (a_name, 'W', &x_db);
   DEBUG_OUTP   yLOG_value   ("open"      , rc);
   --rce;  if (rc < 0) {
      yURG_err ('f', "database could not be openned for writing");
      DEBUG_OUTP   yLOG_exitr   (__FUNCTION__, rce);
      return rce;
   }
   /*---(write header)-------------------*/
   rc = DB__head_write (x_db, P_BASENAME, P_VERNUM, x_nlet, x_npre, x_nsuf, x_nsrc, x_nbas, x_ndic, my.heartbeat);
   DEBUG_OUTP   yLOG_value   ("header"    , rc);
   --rce;  if (rc < 0) {
      yURG_err ('f', "database header could not be written");
      rc = SHARED_close (&x_db);
      DEBUG_OUTP   yLOG_exitr   (__FUNCTION__, rce);
      return rce;
   }
   yURG_msg ('-', "database header successfully  written");
   /*---(letters)------------------------*/
   rc = LETTER_write (x_db);
   DEBUG_OUTP   yLOG_value   ("letters"   , rc);
   --rce;  if (rc < 0) {
      yURG_err ('f', "database letter table could not be written");
      rc = SHARED_close (&x_db);
      DEBUG_OUTP   yLOG_exitr   (__FUNCTION__, rce);
      return rce;
   }
   yURG_msg ('-', "%d in letter table successfully written", LETTER_count ());
   /*---(prefixes)-----------------------*/
   rc = PREFIX_write (x_db);
   DEBUG_OUTP   yLOG_value   ("prefixes"  , rc);
   --rce;  if (rc < 0) {
      yURG_err ('f', "database prefix table could not be written");
      rc = SHARED_close (&x_db);
      DEBUG_OUTP   yLOG_exitr   (__FUNCTION__, rce);
      return rce;
   }
   yURG_msg ('-', "%d in prefix table successfully written", PREFIX__count ());
   /*---(suffixes)-----------------------*/
   rc = SUFFIX_write (x_db);
   DEBUG_OUTP   yLOG_value   ("suffixes"  , rc);
   --rce;  if (rc < 0) {
      yURG_err ('f', "database suffix table could not be written");
      rc = SHARED_close (&x_db);
      DEBUG_OUTP   yLOG_exitr   (__FUNCTION__, rce);
      return rce;
   }
   yURG_msg ('-', "%d in suffix table successfully written", SUFFIX__count ());
   /*---(sources)------------------------*/
   rc = SOURCE_write (x_db);
   DEBUG_OUTP   yLOG_value   ("sources"   , rc);
   --rce;  if (rc < 0) {
      yURG_err ('f', "database source table could not be written");
      rc = SHARED_close (&x_db);
      DEBUG_OUTP   yLOG_exitr   (__FUNCTION__, rce);
      return rce;
   }
   yURG_msg ('-', "%d in source table successfully written", SOURCE__count ());
   /*---(bases index)--------------------*/
   rc = BASE_index_new (BASE__count ());
   DEBUG_OUTP   yLOG_value   ("new index" , rc);
   --rce;  if (rc < 0) {
      yURG_err ('f', "database base index could not be created");
      rc = SHARED_close (&x_db);
      DEBUG_OUTP   yLOG_exitr   (__FUNCTION__, rce);
      return rce;
   }
   yURG_msg ('-', "base index table successfully created");
   /*---(bases)--------------------------*/
   rc = BASE_write (x_db);
   DEBUG_OUTP   yLOG_value   ("sources"   , rc);
   --rce;  if (rc < 0) {
      yURG_err ('f', "database source table could not be written");
      rc = SHARED_close (&x_db);
      DEBUG_OUTP   yLOG_exitr   (__FUNCTION__, rce);
      return rce;
   }
   yURG_msg ('-', "%d in base table successfully written", BASE__count ());
   /*---(words)--------------------------*/
   rc = DICT_write (x_db);
   DEBUG_OUTP   yLOG_value   ("words"     , rc);
   --rce;  if (rc < 0) {
      yURG_err ('f', "database dictionary table could not be written");
      rc = SHARED_close (&x_db);
      DEBUG_OUTP   yLOG_exitr   (__FUNCTION__, rce);
      return rce;
   }
   yURG_msg ('-', "%d in dictionary table successfully written", DICT__count ());
   /*---(bases index)--------------------*/
   rc = BASE_index_free ();
   DEBUG_OUTP   yLOG_value   ("free index", rc);
   --rce;  if (rc < 0) {
      yURG_err ('f', "database base index could not be freed");
      rc = SHARED_close (&x_db);
      DEBUG_OUTP   yLOG_exitr   (__FUNCTION__, rce);
      return rce;
   }
   yURG_msg ('-', "base index table successfully freed");
   /*---(flush)--------------------------*/
   fflush (x_db);
   yURG_msg ('-', "database contents flushed to disk");
   /*---(close)--------------------------*/
   rc = SHARED_close (&x_db);
   DEBUG_OUTP   yLOG_value   ("close"     , rc);
   --rce;  if (rc < 0) {
      yURG_err ('f', "database could not be closed after writing");
      DEBUG_OUTP   yLOG_exitr   (__FUNCTION__, rce);
      return rce;
   }
   yURG_msg ('-', "database successfully closed");
   /*---(summary)------------------------*/
   yURG_msg ('-', "success, current database saved correctly");
   yURG_msg (' ', "");
   /*---(complete)-----------------------*/
   DEBUG_OUTP   yLOG_exit    (__FUNCTION__);
   return 0;
}

char
DB_read                 (char a_name [LEN_HUND])
{
   /*---(locals)-----------+-----+-----+-*/
   char        rce         =  -10;
   char        rc          =    0;
   FILE       *x_db        = NULL;
   char        x_name      [LEN_LABEL] = "";
   char        x_ver       [LEN_LABEL] = "";
   int         x_nlet, x_npre, x_nsuf, x_nsrc, x_nbas, x_ndic;
   char        x_heart     [LEN_HUND]  = "";
   /*---(header)-------------------------*/
   DEBUG_OUTP   yLOG_enter   (__FUNCTION__);
   yURG_msg ('>', "read central database (read)...");
   /*---(open)---------------------------*/
   rc = SHARED_open (a_name, 'R', &x_db);
   DEBUG_OUTP   yLOG_value   ("open"      , rc);
   --rce;  if (rc < 0) {
      yURG_err ('f', "database could not be openned for writing");
      DEBUG_OUTP   yLOG_exitr   (__FUNCTION__, rce);
      return rce;
   }
   /*---(write header)-------------------*/
   rc = DB__head_read  (x_db, x_name, x_ver, &x_nlet, &x_npre, &x_nsuf, &x_nsrc, &x_nbas, &x_ndic, x_heart);
   DEBUG_OUTP   yLOG_value   ("header"    , rc);
   --rce;  if (rc < 0) {
      yURG_err ('f', "database header could not be read");
      rc = SHARED_close (&x_db);
      DEBUG_OUTP   yLOG_exitr   (__FUNCTION__, rce);
      return rce;
   }
   yURG_msg ('-', "database header successfully read");
   /*---(letters)------------------------*/
   rc = LETTER_read  (x_db, x_nlet);
   DEBUG_OUTP   yLOG_value   ("letters"   , rc);
   --rce;  if (rc < 0) {
      yURG_err ('f', "database letter table could not be read");
      rc = SHARED_close (&x_db);
      DEBUG_OUTP   yLOG_exitr   (__FUNCTION__, rce);
      return rce;
   }
   yURG_msg ('-', "%d in letter table successfully read", LETTER_count ());
   /*---(prefixes)-----------------------*/
   rc = PREFIX_read  (x_db, x_npre);
   DEBUG_OUTP   yLOG_value   ("prefixes"  , rc);
   --rce;  if (rc < 0) {
      yURG_err ('f', "database prefix table could not be read");
      rc = SHARED_close (&x_db);
      DEBUG_OUTP   yLOG_exitr   (__FUNCTION__, rce);
      return rce;
   }
   yURG_msg ('-', "%d in prefix table successfully read", PREFIX__count ());
   /*---(suffixes)-----------------------*/
   rc = SUFFIX_read  (x_db, x_nsuf);
   DEBUG_OUTP   yLOG_value   ("suffixes"  , rc);
   --rce;  if (rc < 0) {
      yURG_err ('f', "database suffix table could not be read");
      rc = SHARED_close (&x_db);
      DEBUG_OUTP   yLOG_exitr   (__FUNCTION__, rce);
      return rce;
   }
   yURG_msg ('-', "%d in suffix table successfully read", SUFFIX__count ());
   /*---(sources)------------------------*/
   rc = SOURCE_read  (x_db, x_nsrc);
   DEBUG_OUTP   yLOG_value   ("sources"   , rc);
   --rce;  if (rc < 0) {
      yURG_err ('f', "database source table could not be read");
      rc = SHARED_close (&x_db);
      DEBUG_OUTP   yLOG_exitr   (__FUNCTION__, rce);
      return rce;
   }
   yURG_msg ('-', "%d in source table successfully read", SOURCE__count ());
   /*---(bases index)--------------------*/
   rc = BASE_index_new (x_nbas);
   DEBUG_OUTP   yLOG_value   ("new index" , rc);
   --rce;  if (rc < 0) {
      yURG_err ('f', "database base index could not be created");
      rc = SHARED_close (&x_db);
      DEBUG_OUTP   yLOG_exitr   (__FUNCTION__, rce);
      return rce;
   }
   yURG_msg ('-', "base index table successfully created");
   /*---(bases)--------------------------*/
   rc = BASE_read  (x_db, x_nbas);
   DEBUG_OUTP   yLOG_value   ("sources"   , rc);
   --rce;  if (rc < 0) {
      yURG_err ('f', "database source table could not be read");
      rc = SHARED_close (&x_db);
      DEBUG_OUTP   yLOG_exitr   (__FUNCTION__, rce);
      return rce;
   }
   yURG_msg ('-', "%d in base table successfully read", BASE__count ());
   /*---(words)--------------------------*/
   rc = DICT_read  (x_db, x_ndic);
   DEBUG_OUTP   yLOG_value   ("words"     , rc);
   --rce;  if (rc < 0) {
      yURG_err ('f', "database dictionary table could not be read");
      rc = SHARED_close (&x_db);
      DEBUG_OUTP   yLOG_exitr   (__FUNCTION__, rce);
      return rce;
   }
   yURG_msg ('-', "%d in dictionary table successfully read", DICT__count ());
   /*---(bases index)--------------------*/
   rc = BASE_index_free ();
   DEBUG_OUTP   yLOG_value   ("free index", rc);
   --rce;  if (rc < 0) {
      yURG_err ('f', "database base index could not be freed");
      rc = SHARED_close (&x_db);
      DEBUG_OUTP   yLOG_exitr   (__FUNCTION__, rce);
      return rce;
   }
   yURG_msg ('-', "base index table successfully freed");
   /*---(close)--------------------------*/
   rc = SHARED_close (&x_db);
   DEBUG_OUTP   yLOG_value   ("close"     , rc);
   --rce;  if (rc < 0) {
      yURG_err ('f', "database could not be closed after reading");
      DEBUG_OUTP   yLOG_exitr   (__FUNCTION__, rce);
      return rce;
   }
   yURG_msg ('-', "database successfully closed");
   /*---(paginate)-----------------------*/
   DICT_paginate ();
   yURG_msg ('-', "dictionary paginated");
   yMAP_refresh_full ();
   yURG_msg ('-', "mapping refreshed");
   /*---(summary)------------------------*/
   yURG_msg ('-', "success, current database read correctly");
   yURG_msg (' ', "");
   /*---(complete)-----------------------*/
   DEBUG_OUTP   yLOG_exit    (__FUNCTION__);
   return 0;
}

char
DB_stats                (char a_name [LEN_HUND])
{
   /*---(locals)-----------+-----+-----+-*/
   char        rce         =  -10;
   char        rc          =    0;
   FILE       *x_db        = NULL;
   char        x_name      [LEN_LABEL] = "";
   char        x_ver       [LEN_LABEL] = "";
   int         x_nlet, x_npre, x_nsuf, x_nsrc, x_nbas, x_ndic;
   char        x_heart     [LEN_HUND]  = "";
   char        t           [LEN_LABEL] = "";
   /*---(header)-------------------------*/
   DEBUG_OUTP   yLOG_enter   (__FUNCTION__);
   yURG_msg ('>', "retrieve central database statistics (stats)...");
   /*---(open)---------------------------*/
   rc = SHARED_open (a_name, 'R', &x_db);
   DEBUG_OUTP   yLOG_value   ("open"      , rc);
   --rce;  if (rc < 0) {
      yURG_err ('f', "database could not be openned for writing");
      DEBUG_OUTP   yLOG_exitr   (__FUNCTION__, rce);
      return rce;
   }
   /*---(write header)-------------------*/
   rc = DB__head_read  (x_db, x_name, x_ver, &x_nlet, &x_npre, &x_nsuf, &x_nsrc, &x_nbas, &x_ndic, x_heart);
   DEBUG_OUTP   yLOG_value   ("header"    , rc);
   --rce;  if (rc < 0) {
      yURG_err ('f', "database header could not be read");
      rc = SHARED_close (&x_db);
      DEBUG_OUTP   yLOG_exitr   (__FUNCTION__, rce);
      return rce;
   }
   yURG_msg ('-', "database header successfully read");
   /*---(close)--------------------------*/
   rc = SHARED_close (&x_db);
   DEBUG_OUTP   yLOG_value   ("close"     , rc);
   --rce;  if (rc < 0) {
      yURG_err ('f', "database could not be closed after reading");
      DEBUG_OUTP   yLOG_exitr   (__FUNCTION__, rce);
      return rce;
   }
   yURG_msg ('-', "database successfully closed");
   /*---(display)------------------------*/
   printf ("#!/usr/local/bin/gregg --stats\n");
   printf ("db       å%sæ\n" , a_name);
   printf ("name     å%sæ\n" , x_name);
   printf ("ver      å%sæ\n" , x_ver);
   strl4main (x_nlet, t , 0, 'c', '-', LEN_LABEL);
   printf ("letter   %7.7s\n", t);
   strl4main (x_npre, t , 0, 'c', '-', LEN_LABEL);
   printf ("suffix   %7.7s\n", t);
   strl4main (x_nsuf, t , 0, 'c', '-', LEN_LABEL);
   printf ("prefix   %7.7s\n", t);
   strl4main (x_nsrc, t , 0, 'c', '-', LEN_LABEL);
   printf ("source   %7.7s\n", t);
   strl4main (x_nbas, t , 0, 'c', '-', LEN_LABEL);
   printf ("base     %7.7s\n", t);
   strl4main (x_ndic, t , 0, 'c', '-', LEN_LABEL);
   printf ("word     %7.7s\n", t);
   printf ("stamp    å%sæ\n" , x_heart);
   /*---(complete)-----------------------*/
   DEBUG_OUTP   yLOG_exit    (__FUNCTION__);
   return 0;
}



/*====================------------------------------------====================*/
/*===----                         debugging                            ----===*/
/*====================------------------------------------====================*/
static void  o___DEBUGGING_______o () { return; }

char SOURCE__count   (void)  { return s_nsource; }

char*
SOURCE__detail          (char n)
{
   /*---(locals)-----------+-----+-----+-*/
   char        rce         =  -10;
   /*---(defense)------------------------*/
   strcpy (g_print, "n/a");
   if (n <  0 || n >= s_nsource)  return g_print;
   /*---(prepare)------------------------*/
   sprintf (g_print, "%-2d  %-5d  %s", n, s_source [n].count, s_source [n].name);
   /*---(complete)-----------------------*/
   return g_print;
}



