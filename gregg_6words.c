/*============================---(source-start)---============================*/

#include "gregg.h"


const char dname[] = "/var/lib/gregg/gregg.dict";
#define     OUT_XGAP    10




/*============================--------------------============================*/
/*===----                   private data structures                    ----===*/
/*============================--------------------============================*/

/*---(words structure)--------------------------*/
#define  MAX_WORDS      5000
#define  MAX_LEN          30
typedef struct cWORDS  tWORDS;
struct cWORDS {
   int         num;
   char        english [MAX_LEN];           /* english word                   */
   char        gregg   [MAX_LEN];           /* gregg translation              */
   char        version;                     /* version of gregg shorthand     */
   char        source;                      /* type of source text            */
   int         page;                        /* page in translation text       */
   char        type;
   tWORDS     *nextg;
   tWORDS     *nexte;
   int         count;
};
static tWORDS s_words [MAX_WORDS];
static int    s_nword;

/*---(gregg index)------------------------------*/
tWORDS *out_index [MAX_LETTERS][MAX_LETTERS][MAX_LETTERS];

/*---(english index)----------------------------*/
tWORDS *eng_index [27][27][27];

PRIV int       a         = 0;           /* 1st letter index                    */
PRIV int       b         = 0;           /* 2nd letter index                    */
PRIV int       c         = 0;           /* 3rd letter index                    */


int    letters[30];




#define    BASX      -90
#define    BASY       90
#define    LINE      -60

int    outx  =     0;   /* outline starting point                  */
int    outy  =     0;
int    posx  =     0;   /* current letter point                    */
int    posy  =     0;


PRIV  char  words_heads (void);
PRIV  char  words_listg  (tWORDS *);
PRIV  char  words_liste  (tWORDS *);



/*====================------------------------------------====================*/
/*===----                         english index                        ----===*/
/*====================------------------------------------====================*/
static void      o___ENGLISH_________________o (void) {;}

PRIV int      /*----: find a word in the english index -----------------------*/
english_find       (char *a_english)
{
   /*---(locals)-------------------------*/
   int       len       = 0;            /* length of word                      */
   tWORDS   *curr      = NULL;         /* pointer to current word in search   */
   int       last      = 0;            /* index of previous word in search    */
   /*---(initialize)---------------------*/
   a         = 26;
   b         = 26;
   c         = 26;
   /*---(identify index)-----------------*/
   len   = strlen (a_english);
   if (len > 3) len = 3;
   switch (len) {
   case 3 : c = a_english[2] - 'a'; if (c < 0 || c > 25) c = 26;
   case 2 : b = a_english[1] - 'a'; if (b < 0 || b > 25) b = 26;
   case 1 : a = a_english[0] - 'a'; if (a < 0 || a > 25) a = 26; break;
   case 0 : return -1;            ; break;
   }
   RPTG_DICT  printf("%c %c %c %02d %02d %02d (%2d) %s\n", a + 'a', b + 'a', c + 'a', a, b, c, len, a_english);
   /*---(get the index head)-------------*/
   curr = eng_index[a][b][c];
   if (curr == NULL)            return  0;
   /*---(run through the list)-----------*/
   while (1) {
      RPTG_DICT  printf("   checking (%03d) %s\n", curr->num, curr->english);
      if (strncmp(a_english, curr->english, MAX_LEN) == 0) {
         RPTG_DICT  printf("   DUPLICATE for %s\n", a_english);
         return  curr->num;
      }
      last = curr->num;
      if (curr->nexte == NULL) break;
      curr = curr->nexte;
   }
   /*---(complete)-----------------------*/
   RPTG_DICT  printf("   adding %s\n", a_english);
   return -(last);
}

PRIV char     /*----: add to the english index -------------------------------*/
english_add        (int a_curr, int a_prev)
{
   /*---(locals)-------------------------*/
   tWORDS   *last      = NULL;
   tWORDS   *curr      = NULL;         /* pointer to current word in search   */
   /*---(defense)------------------------*/
   if (a_prev >   0)  return -1;
   /*---(start index)--------------------*/
   if (a_prev ==  0) {
      eng_index[a][b][c] = &s_words[a_curr];
      return 0;
   }
   /*---(continue index list)------------*/
   /*> last        = &s_words[-a_prev];                                                 <* 
    *> last->nexte = &s_words[ a_curr];                                                 <*/
   /*---(run through the list)-----------*/
   curr = eng_index[a][b][c];
   while (1) {
      if (curr->nexte == NULL) break;
      curr = curr->nexte;
   }
   /*---(add to index)-------------------*/
   curr->nexte = &s_words[ a_curr];
   /*---(complete)-------------------------*/
   return 0;
}



/*====================------------------------------------====================*/
/*===----                          gregg index                         ----===*/
/*====================------------------------------------====================*/
static void      o___GREGG___________________o (void) {;}

PRIV int      /*----: find a word in the gregg index -------------------------*/
gregg_find         (char *a_gregg)
{
   /*---(locals)-------------------------*/
   char     *p         = NULL;         /* gregg letter pointer                */
   char     *q         = ".";          /* gregg letter delimiter              */
   int       count     = 0;            /* letter counter                      */
   int       i         = 0;            /* loop iterator -- locations          */
   tWORDS   *curr      = NULL;         /* pointer to current word in search   */
   int       last      = 0;            /* index of previous word in search    */
   /*---(initialize)---------------------*/
   a         = 0;
   b         = 0;
   c         = 0;
   /*---(read head)----------------------*/
   p = strtok(a_gregg, q);
   if (p == NULL || strncmp(p, ">", MAX_LEN) != 0) return -1;
   /*---(get indexes)--------------------*/
   while (1) {
      p = strtok(NULL, q);
      if (p == NULL || strncmp(p, "", MAX_LEN) == 0) break;
      for (i = 0; i < MAX_LETTERS; ++i) {
         if (strncmp(p, g_loc[i].n, 5) != 0) continue;
         switch (count) {
         case 0: a = i; break;
         case 1: b = i; break;
         case 2: c = i; break;
         }
         break;
      }
      ++count;
   }
   /*---(get the index head)-------------*/
   curr = out_index[a][b][c];
   if (curr == NULL)            return  0;
   /*---(run through the list)-----------*/
   while (1) {
      if (strncmp(a_gregg, curr->gregg, MAX_LEN) == 0) {
         return  curr->num;
      }
      last = curr->num;
      if (curr->nextg == NULL) break;
      curr = curr->nextg;
   }
   /*---(complete)-----------------------*/
   return -(last);
}

PRIV char     /*----: add to the gregg index ---------------------------------*/
gregg_add          (int a_curr, int a_prev)
{
   /*---(locals)-------------------------*/
   tWORDS   *last      = NULL;
   tWORDS   *curr      = NULL;         /* pointer to current word in search   */
   /*---(defense)------------------------*/
   if (a_prev >   0)  return -1;
   /*---(start index)--------------------*/
   if (a_prev ==  0) {
      out_index[a][b][c] = &s_words[a_curr];
      return 0;
   }
   /*---(run through the list)-----------*/
   curr = out_index[a][b][c];
   while (1) {
      if (curr->nextg == NULL) break;
      curr = curr->nextg;
   }
   /*---(add to index)-------------------*/
   curr->nextg = &s_words[ a_curr];
   /*---(complete)-----------------------*/
   return 0;
}



/*============================--------------------============================*/
/*===----                          dictionary                          ----===*/
/*============================--------------------============================*/
static void      o___DICTIONARY______________o (void) {;}

char         /*-> clear a dictionary entry -----------[ ------ [ge.C70.13#.E1]*/ /*-[02.0000.03#.T]-*/ /*-[--.---.---.--]-*/
DICT__clear          (int  a_index)
{
   s_words [a_index].num             = a_index;
   strlcpy (s_words[a_index].english , ""   , MAX_LEN);
   strlcpy (s_words[a_index].gregg   , ""   , MAX_LEN);
   s_words [a_index].version         = '-';
   s_words [a_index].source          = '-';
   s_words [a_index].page            = 0;
   s_words [a_index].type            = '-';
   s_words [a_index].nextg           = NULL;
   s_words [a_index].nexte           = NULL;
   s_words [a_index].count           = 0;
   return 0;
}

PRIV char     /*----: initialize the dictionary ------------------------------*/
DICT__init         (void)
{
   /*---(locals)-------------------------*/
   int       i, j, k;
   /*---(clean english index)------------*/
   for (i = 0; i < 27; ++i) {
      for (j = 0; j < 27; ++j) {
         for (k = 0; k < 27; ++k) {
            eng_index[i][j][k] = NULL;
         }
      }
   }
   /*---(clean gregg index)--------------*/
   for (i = 0; i < MAX_LETTERS; ++i) {
      for (j = 0; j < MAX_LETTERS; ++j) {
         for (k = 0; k < MAX_LETTERS; ++k) {
            out_index[i][j][k] = NULL;
         }
      }
   }
   /*---(clean dictionary)---------------*/
   for (i = 0; i < MAX_WORDS; ++i)  DICT__clear (i);
   /*---(globals)------------------------*/
   s_nword    = 0;
   /*---(complete)-----------------------*/
   return 0;
}

PRIV int      /*----: append a new word entry --------------------------------*/
DICT__append         (char *a_english, char *a_gregg)
{
   DICT__clear (s_nword);
   strlcpy (s_words [s_nword].english, a_english  , MAX_LEN);
   strlcpy (s_words [s_nword].gregg  , a_gregg    , MAX_LEN);
   ++s_nword;
   return s_nword - 1;
}

PRIV char     /*----: append a new word entry --------------------------------*/
DICT__attrib         (int a_index, char a_ver, char a_src, int a_page, char a_type)
{
   s_words[a_index].version = a_ver;
   s_words[a_index].source  = a_src;
   s_words[a_index].page    = a_page;
   s_words[a_index].type    = a_type;
   return 0;
}

char
dict_read          (void)            /* read the translation dictionary               */
{
   /*---(locals)---------------------------*/
   FILE     *f;
   char      s[MAX_LINE] = "";         // current record
   int       n = 0;                    // current record number in file
   char      x_eng   [MAX_LEN];        /* english word                        */
   char      x_gregg [MAX_LEN];        /* gregg translation                   */
   char      x_ver;                    /* source of gregg translation         */
   char      x_src;                    /* source of gregg translation         */
   int       x_page;                   /* page in source of translation       */
   char      x_type;                   /* type, -=normal, b=brief, p=phrase   */
   int       rc = 0;                   /* function return code                */
   int       prev;
   int       curr;
   int       dups      = 0;
   int i;
   char   *p = NULL;             // pointer to substring
   char   *q = "";              // delimiters
   char   *r = NULL;             // pointer to substring
   /*---(begin)----------s-----------------*/
   RPTG_DICT  printf("READ DICTIONARY WORDS\n");
   DICT__init   ();
   /*---(open file)------------------------*/
   f = fopen(dname, "r");
   if (f == NULL) return -1;
   /*---(add first line)-------------------*/
   DICT__append ("BOF", ">.");
   /*---(process)--------------------------*/
   while (1) {
      fgets(s, MAX_LINE, f);
      if (feof(f))  break;
      //---(english)----------------------#
      p = strtok_r (s, q, &r);
      if (p == NULL)                                continue;
      strlcpy (x_eng, str_trim (p), MAX_LEN);
      if (x_eng[0] == '#' || x_eng[0] == '\0')  continue;
      //---(gregg)------------------------#
      p = strtok_r (NULL, q, &r);
      if (p == NULL)                                continue;
      strlcpy (x_gregg, str_trim (p), MAX_LEN);
      //---(source)-----------------------#
      p = strtok_r (NULL, q, &r);
      if (p != NULL && strlen(p) == 11) {
         sscanf (str_trim(p), "%c %c %d %c", &x_ver, &x_src, &x_page, &x_type);
      } else {
         x_ver   = '-';
         x_src   = '-';
         x_page    = 0;
         x_type    = '-';
      }
      /*---(put into structure)------------*/
      prev = english_find   (x_eng);
      if (prev > 0) {
         ++dups;
         continue;
      }
      curr = DICT__append    (x_eng, x_gregg);
      rc   = DICT__attrib   (curr, x_ver, x_src, x_page, x_type);
      rc   = english_add    (curr, prev);
      prev = gregg_find     (x_gregg);
      rc   = gregg_add      (curr, prev);

      ++n;
   }
   fclose(f);
   /*---(add last line)--------------------*/
   ++n;
   DICT__append ("eof", ">.");
   ++n;
   char      t[MAX_LEN];
   char      u[MAX_LEN];
   for (i = 0; i < n; ++i) {
      snprintf(t, MAX_LEN, "%s", s_words[i].english);
      snprintf(u, MAX_LEN, "%s", s_words[i].gregg);
      RPTG_DICT  printf("%03d) %-25s = %-25s :: %c %c / %3d   %c\n", i, t, u, s_words[i].version, s_words[i].source, s_words[i].page, s_words[i].type);
   }
   RPTG_DICT  printf("   READ %d DICTIONARY WORDS (with %d dups)\n", n - 2, dups);
   words_heads();
   return 0;
}

static char
words_heads (void)           /* list the head of each index                   */
{
   int i, j, k;
   /*> DEBUG__DICTG {                                                                 <*/
   for (i = 0; i < MAX_LETTERS; i++) {
      for (j = 0; j < MAX_LETTERS; j++) {
         for (k = 0; k < MAX_LETTERS; k++) {
            if (out_index[i][j][k] != NULL) {
               /*> printf("%3d %3d %3d\n", i, j, k);                               <*/
               words_listg(out_index[i][j][k]);
            }
         }
      }
   }
   /*> }                                                                              <*/
   /*> DEBUG__DICTE {                                                                 <*/
   for (i = 0; i < 27; i++) {
      for (j = 0; j < 27; j++) {
         for (k = 0; k < 27; k++) {
            if (eng_index[i][j][k] != NULL) {
               /*> printf("%3d %3d %3d\n", i, j, k);                               <*/
               words_liste(eng_index[i][j][k]);
            }
         }
      }
   }
   /*> }                                                                              <*/
   return 0;
}

static char
words_listg  (                /* list words for an index number                */
      tWORDS *a_head)
{
   tWORDS   *nextg = a_head;
   while (nextg != NULL) {
      /*> printf("              %s\n", nextg->english);                               <*/
      nextg = nextg->nextg;
   }
   return 0;
}

static char
words_liste  (                /* list words for an index number                */
      tWORDS *a_head)
{
   tWORDS   *nexte = a_head;
   while (nexte != NULL) {
      /*> printf("              %s\n", nexte->english);                               <*/
      nexte = nexte->nexte;
   }
   return 0;
}


int
WORDS_find           (char *a_word)
{
   /*---(locals)-----------+-----+-----+-*/
   char        rce         =  -10;
   int         i           =    0;
   /*---(defense)------------------------*/
   --rce;  if (a_word == NULL)  return rce;
   /*---(search)-------------------------*/
   for (i = 0; i < MAX_WORDS; ++i) {
      if (strncmp (s_words[i].english, "eof",  MAX_LEN) == 0)  return -1;
      if (strncmp (s_words[i].english, a_word, MAX_LEN) != 0)  continue;
      return  i;
   }
   /*---(complete)-----------------------*/
   return 0;
}

int
words_outstring (            /* locate outline in the translation dictionary  */
      char     *a_outstring)           /* outline letters                     */
{
   int i;
   if (strcmp(o.grade, "BAD") == 0) {
      strlcpy (o.word, "(malformed)", MAX_LEN);
      return 0;
   }
   strlcpy (o.word, "(unknown)", MAX_LEN);
   int count = 0;
   for (i = 0; i < MAX_WORDS; ++i) {
      if (strncmp(s_words[i].english, "eof",  MAX_LEN) == 0)  return -1;
      /*> DEBUG__MATCHES  printf("look <<%s>>\n", s_words[i].gregg);                    <*/
      if (strncmp(s_words[i].gregg, a_outstring, MAX_LEN) != 0)  continue;
      /*> DEBUG__WRITING  printf("%3d) <<%s>>\n", i, s_words[i].gregg);             <* 
       *> DEBUG__MATCHES  printf("%3d) <<%s>>\n", i, s_words[i].gregg);             <*/
      ++count;
      if (count == 1) strlcpy (o.word, "", MAX_LEN);
      if (count >  1) strncat(o.word, ",", MAX_LEN);
      strncat(o.word, s_words[i].english, MAX_LEN);
      /*> return  i;                                                                  <*/
   }
   return  0;
}

char*
str_trim (                   /* trim spaces from front and back on string     */
      char     *a_string)
{
   int i;
   /*---(locals)--------------------------------*/
   char *head = a_string;
   int  l = strlen(a_string);
   if (a_string[l - 1] == '\n') {
      a_string[l - 1] = '\0';
      --l;
   }
   /*---(deal with leading spaces)--------------*/
   for (i = 0; i < l; ++i) {
      if (a_string[i] != ' ') break;
      ++head;
   }
   /*---(deal with trailing spaces)-------------*/
   for (i = l - 1; i > 0; --i) {
      if (a_string[i] != ' ') break;
      a_string[i] = '\0';
   }
   /*---(complete)------------------------------*/
   return head;
}




/*============================--------------------============================*/
/*===----                            outlines                          ----===*/
/*============================--------------------============================*/

char
WORDS_display        (char *a_words, char a_base)
{
   char   *q  = " ";
   char   *p0 = NULL;
   char   *p  = NULL;
   char   *pn = NULL;
   char   *s  = NULL;
   int     i  = 0;
   char    x_words[500];
   static  x_called =  0;
   strlcpy (x_words, a_words, 500);
   /*> printf("%4d  ALL  : %s\n", x_called++, x_words);                               <*/
   p0 = p = strtok_r(x_words, q, &s);
   /*> printf("ok so far\n");                                                         <*/
   if (p == NULL) return -1;
   /*> WORDS_start();                                                                 <*/
   /*> printf("ok so again\n");                                                       <*/
   while (1) {
      pn = x_words + (p - p0);
      i = WORDS_find(pn);
      /*> printf("\nsource = %p, word = %s, index = %d\n", x_words, pn, i);           <*/
      WORDS_outline (i);
      p = strtok_r(NULL, q, &s);
      if (p == NULL) break;
   }
   return 0;
}

char
WORDS_start (void)           /* reset outline display to beginning of page    */
{  /*-T=initializer-------S=procedure-------I=file------------*=unknown-------*/
   /*---(initialize)-----------------------*/
   outx  = BASX;
   outy  = BASY;
   /*---(complete)-------------------------*/
   return 0;
}

char          /*----: display the stylized outline of the current word -------*/
words_result       (void)
{
   /*---(locals)-------------------------*/
   int       i;
   char     *p         = NULL;
   char     *q         = ",";
   char      xword[100];
   /*---(initialize)---------------------*/
   strlcpy (xword, o.word, 100);
   if (xword[0] == '(') return -1;
   WORDS_start();
   /*---(take first word)----------------*/
   p = strtok(xword, q);
   if (p != NULL) {
      i = WORDS_find(p);
      WORDS_outline(i);
   }
   /*---(complete)-----------------------*/
   return 0;
}

char
WORDS__width         (int a_index)
{
}

char
WORDS_outline        (int a_index)
{
   /*---(locals)--------------------------------*/
   int    letter = 0;        /* current letter                                */
   int    offset = 0;        /* pointes to correct version of vowel           */
   int    left   = 0;        /* leftmost position                             */
   int    right  = 0;        /* rightmost position                            */
   int    temp   = 0;        /* calc variable                                 */
   int    ltr    = 0;        /* current letter index                          */
   int i;
   /*---(set up letters)------------------------*/
   int    count  = words_translate(a_index);
   printf("CREATING OUTLINE...\n");
   /*---(test for width too large)--------------*/
   printf("   test width...\n");
   posx = posy = 0;
   for (i = 0; i < count; ++i) {
      ltr   = letters[i];
      posy += g_loc[ltr].ey;
      if (posy < 50 && posy > -50) {
         temp = posx + g_loc[ltr].le;
         /*> if (temp < left ) left  = temp;                                          <*/
         temp = posx + g_loc[ltr].ri;
         if (temp > right) right = temp;
      }
      posx += g_loc[ltr].ex;
      printf("      %02d (%03d) %-5.5s :: ", i, ltr, g_loc[ltr].n);
      printf("tx = %4.0f, px  = %4d, py  = %4d, le  = %4d, ri = %4d\n",
            g_loc[ltr].ex, posx, posy, left, right);
   }
   if (outx - left + right > 280) { /* 260 small, 360 pad */
      outx  = BASX;     /* reset to left margin      */
      outy += LINE;     /* go to nextg line           */
   }
   /*---(continue the base line)----------------*/
   printf("   drawing...\n");
   printf("      starting at %4dx, %4dy\n", outx - left, outy);
   glPushMatrix(); {
      glTranslatef (outx - left,  outy,  0.0);
      glColor4f    (0.0f, 0.0f, 0.0f, 0.7f);
      glLineWidth  ( 3.0);
      glBegin (GL_LINES); {
         glVertex3f   (left            , 0, 50);
         glVertex3f   (right + OUT_XGAP, 0, 50);
      } glEnd ();
      glPointSize  (10.0);
      glBegin (GL_POINTS); {
         glVertex3f   (left            , 0, 50);
         glVertex3f   (right + OUT_XGAP, 0, 50);
      } glEnd ();
   } glPopMatrix();

   /*> return 0;                                                                      <*/

   /*---(LOOP)----------------------------------*/
   posx = posy = 0;
   /*---(set outline starting point)------------*/
   glPushMatrix();
   glTranslatef(outx - left,  outy,  0.0);
   glLineWidth(1.5);
   for (i = 0; i < count; ++i) {
      /*---(establish values)-------------------*/
      letter = letters[i];
      offset = 0;
      /*---(watch initial left)-----------------*/
      /*> if (i == 0 && g_loc[letter].le < 0) {                                       <* 
       *>    glTranslatef(-g_loc[letter].le,  0,  0.0);                               <* 
       *>    posx  =  -g_loc[letter].le;                                              <* 
       *> }                                                                           <*/
      /*---(draw letter)------------------------*/
      /*> glColor4f(0.0f, 0.0f, 0.5f, 0.7f);                                          <*/
      glColor4f(0.0f, 0.0f, 0.0f, 1.0f);
      if (g_loc[letter].fu == 'c') {
         /*> offset = words_vowel(i, count) - 1;                                      <*/
         offset = 0;
         glCallList (dl_solid + letter + offset);
      } else {
         /*> words_consonant(i, count);                                               <*/
         glCallList (dl_solid + letter);
      }
      /*---(update g_loc)-------------------------*/
      printf("      letter = %3d, offset = %2d\n", letter, offset);
      glTranslatef(g_loc[letter].ex,  g_loc[letter].ey,  0.0);
      posx += g_loc[letter].ex;
      posy += g_loc[letter].ey;
   }
   printf("      ending   at %4dx, %4dy, %4dox\n", outx + posx, outy + posy, outx);
   outx += -left + right + OUT_XGAP;
   glPopMatrix();
   printf("   DONE\n");
   /*---(complete)------------------------------*/
   return 0;
}

int           /*----: translate english word into gregg letters --------------*/
words_translate    (int a_word)
{
   /*---(locals)-------------------------*/
   char   *p = NULL;             // pointer to substring
   char   *q = ".";              // delimiters
   char   l[5] = "";             // letter
   int    count =    0;          // letter count
   int    offset =   0;
   char   a_outstring[MAX_LEN];
   int       i         = 0;            /* loop iterator : general             */
   /*---(init)---------------------------*/
   strlcpy (a_outstring, s_words[a_word].gregg, MAX_LEN);
   /*---(clear letters)------------------*/
   for (i = 0; i < 30; ++i) {
      letters[i] = 0;
   }
   /*---(get the first letter, always ">")------*/
   DEBUG__WRITING  printf("OUTSTRING = %s\n", a_outstring);
   p = strtok(a_outstring, q);
   if (p == NULL || strncmp(p, "", MAX_LEN) == 0) {
      return -1;
   }
   /*---(run through the letters)---------------*/
   while (1) {
      p = strtok(NULL, q);
      if (p == NULL) {
         if (letters[count - 1] == 1) {
            letters[count] = 2;
            ++count;
         }
         break;
      }
      /*---(new stroke)------------------*/
      if (strncmp(p, ">", 5) == 0) {
         if (count == 0 || letters[count - 1] == 1) {
            letters[count] = 2;
            ++count;
         }
      }
      /*---(handle letter)---------------*/
      strlcpy (l, p, 5);
      for (i = 0; i < 5; ++i)  l[i] = tolower(l[i]);
      DEBUG__WRITING  printf("   letter = <<%s>>\n", l);
      for (i = 0; i < MAX_LETTERS; ++i) {
         if (strncmp(l, g_loc[i].n, 5) != 0) continue;
         letters[count]  = i;
         break;
      }
      if (letters[count] == 0) continue;
      DEBUG__WRITING  printf("      ind = <<%d>>\n", letters[count]);
      ++count;
   }
   for (i = 0; i < count; ++i) {
      offset = 0;
      if (g_loc[letters[i]].fu == 'c') {
         /*> offset = words_vowel(i, count) - 1;                                      <*/
         offset = words_vowel(i, count);
         letters[i] += offset;
      }
      DEBUG__WRITING  printf("%d, ", letters[i]);
   }
   DEBUG__WRITING  printf("\n");
   /*---(complete)-----------------------*/
   return count;
}

int
words_consonant (            /* adjust for combination consontants            */
      int       a_index,               /* -- index to word in dictionary      */
      int       a_count)               /* -- position of current letter       */
{
   /*---(locals)-------------------------*/
   int letter;
   int one;
   int two;
   int adjx = 0;
   int adjy = 0;
   int i;
   if (a_index == 0)            return -1;
   /*---(previous letter)----------------*/
   letter = letters[a_index - 1];
   one    = 0;
   for (i = 0; i < MAX_GROUPS; ++i) {
      if (i > 0 && strncmp(groups[i].gr, groups[i - 1].gr, 5) == 0) continue;
      /*> printf("%s ", groups[i].gr);                                             <*/
      if (strncmp(groups[i].gr, "eof",  5) == 0)                    break;
      if (strncmp(g_loc[letter].gr, groups[i].gr, 5) != 0)            continue;
      one = groups[i].gr_num;
      break;
   }
   /*---(current letter)-----------------*/
   letter = letters[a_index];
   two    = 0;
   for (i = 0; i < MAX_GROUPS; ++i) {
      if (i > 0 && strncmp(groups[i].gr, groups[i - 1].gr, 5) == 0) continue;
      /*> printf("%s ", groups[i].gr);                                             <*/
      if (strncmp(groups[i].gr, "eof",  5) == 0)                    break;
      if (strncmp(g_loc[letter].gr, groups[i].gr, 5) != 0)            continue;
      two = groups[i].gr_num;
      break;
   }
   /*---(complete)-----------------------*/
   if (one ==  5 && two == 13) {
      /*> DEBUG__WRITING  printf("found a C-V\n");                                    <*/
      if (strncmp(g_loc[letters[a_index - 1]].n, "u", 5) != 0) {
         adjx = -4;
         adjy =  4;
      }
   } else if (one == 14 && two ==  6) {
      /*> DEBUG__WRITING  printf("found a P-R\n");                                    <*/
      if (strncmp(g_loc[letters[a_index - 1]].n, "o", 5) != 0) {
         adjx = -4;
         adjy =  4;
      }
   }
   glTranslatef(adjx, adjy,  0.0);
   posx += adjx;
   posy += adjy;
   /*---(complete)-----------------------*/
   return 0;
}

int
words_vowel (                /* adjust vowel shape for layout                 */
      int       a_index,               /* -- index to word in dictionary      */
      int       a_count)               /* -- position of current letter       */
{
   /*---(locals)--------------------------------*/
   int letter;
   int one;
   int two;
   int offset;
   int i;
   /*---(prev letter)---------------------------*/
   if (a_index == 0) {
      one = 0;
      /*> printf("   letter one = %2d, %2s ( ", 0, "-");                              <*/
   } else {
      letter = letters[a_index - 1];
      /*> printf("   letter one = %2d, %2s, %2s ( ", letter, g_loc[letter].n, g_loc[letter].gr);   <*/
      one    = 0;
      for (i = 0; i < MAX_GROUPS; ++i) {
         if (i > 0 && strncmp(groups[i].gr, groups[i - 1].gr, 5) == 0) continue;
         /*> printf("%s ", groups[i].gr);                                             <*/
         if (strncmp(groups[i].gr, "eof",  5) == 0)                    break;
         if (strncmp(g_loc[letter].gr, groups[i].gr, 5) != 0)            continue;
         one = groups[i].gr_num;
         break;
      }
   }
   /*> printf(") so %2d\n", one);                                                     <*/
   /*---(nextg letter)---------------------------*/
   if (a_index == a_count - 1) {
      two = 0;
      /*> printf("   letter one = %2d, %2s ( ", 0, "-");                              <*/
   } else {
      letter = letters[a_index + 1];
      /*> printf("   letter two = %2d, %2s, %2s ( ", letter, g_loc[letter].n, g_loc[letter].gr);   <*/
      two    = 0;
      for (i = 0; i < MAX_GROUPS; ++i) {
         if (i > 0 && strncmp(groups[i].gr, groups[i - 1].gr, 5) == 0) continue;
         /*> printf("%s ", groups[i].gr);                                             <*/
         if (strncmp(groups[i].gr, "eof",  5) == 0)                    break;
         if (strncmp(g_loc[letter].gr, groups[i].gr, 5) != 0)            continue;
         two = groups[i].gr_num;
         break;
      }
   }
   /*> printf(") so %2d\n", two);                                                     <*/
   /*---(get offset)----------------------------*/
   offset  = vowels[one][two];
   if (offset < 0) offset = 0;
   /*> printf("    prv = %2d, nxt = %2d, adj = %2d\n", one, two, offset);             <*/
   /*---(complete)------------------------------*/
   return offset;
}



/*============================----(source-end)----============================*/
