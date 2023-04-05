/*============================---(source-start)---============================*/
#include "gregg.h"



tFAKE g_fake;



/*============================--------------------============================*/
/*===----                    loading for raw points                    ----===*/
/*============================--------------------============================*/
static void o___POINTS_________________o (void) {;}

/*
 *  loading assumes perfect conditions for start 0,0, which mirrors data
 *  filtering in touch points which use the start point as an offset for
 *  correcting all coordinates.
 *
 */


char
FAKE_start              (void)
{
   DEBUG_OUTP   yLOG_enter   (__FUNCTION__);
   strlcpy (g_fake.load, ""         , LEN_MASS);
   g_fake.count = 0;
   g_fake.sx = g_fake.px = 0;
   g_fake.sy = g_fake.py = 0;
   FAKE_touch  (0, 0);
   DEBUG_OUTP   yLOG_exit    (__FUNCTION__);
   return 0;
}

char
FAKE_touch              (short x, short y)
{
   char        t           [LEN_LABEL] = "";
   DEBUG_OUTP   yLOG_enter   (__FUNCTION__);
   if (g_fake.count == 0) strcpy (g_fake.load, ";");
   sprintf (t, "Õ%d,%d", x, y);
   strlcat (g_fake.load, t, LEN_MASS);
   g_fake.sx = g_fake.px = x;
   g_fake.sy = g_fake.py = y;
   ++g_fake.count;
   DEBUG_OUTP   yLOG_complex ("point"     , "%3dc, %4dx, %4dy", g_fake.count, x, y);
   DEBUG_OUTP   yLOG_exit    (__FUNCTION__);
   return 0;
}

char
FAKE_point           (short x, short y)
{
   char        t           [LEN_LABEL] = "";
   if (x == g_fake.px && y == g_fake.py)  return 0;
   DEBUG_OUTP   yLOG_senter  (__FUNCTION__);
   sprintf (t, ";%d,%d", (int) x, (int) y);
   strlcat (g_fake.load, t, LEN_MASS);
   g_fake.px = x;
   g_fake.py = y;
   ++g_fake.count;
   DEBUG_OUTP   yLOG_sint    (x);
   DEBUG_OUTP   yLOG_sint    (y);
   DEBUG_OUTP   yLOG_snote   (t);
   DEBUG_OUTP   yLOG_sexit   (__FUNCTION__);
   return 0;
}

char
FAKE_lift               (short x, short y)
{
   char       *p           = NULL;
   char        t           [LEN_LABEL] = "";
   DEBUG_OUTP   yLOG_enter   (__FUNCTION__);
   if (x == g_fake.px && y == g_fake.py) {
      p = strrchr (g_fake.load, ';');
      if (p != NULL) {
         p [0] = '\0';
         --g_fake.count;
      }
   }
   sprintf (t, ";Ô%d,%d", x, y);
   strlcat (g_fake.load, t, LEN_MASS);
   g_fake.px = x;
   g_fake.py = y;
   ++g_fake.count;
   DEBUG_OUTP   yLOG_complex ("point"     , "%3dc, %4dx, %4dy", g_fake.count, x, y);
   DEBUG_OUTP   yLOG_exit    (__FUNCTION__);
   return 0;
}

char
FAKE_done               (short x, short y)
{
   DEBUG_OUTP   yLOG_enter   (__FUNCTION__);
   FAKE_lift  (x, y);
   strlcat (g_fake.load, ";"        , LEN_MASS);
   DEBUG_OUTP   yLOG_exit    (__FUNCTION__);
   return 0;
}

char
REV_load_raw        (void)
{
   DEBUG_OUTP   yLOG_enter   (__FUNCTION__);
   RAW_load (g_fake.load);
   DEBUG_OUTP   yLOG_exit    (__FUNCTION__);
   return 0;
}
