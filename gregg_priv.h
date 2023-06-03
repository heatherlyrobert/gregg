/*=================================(beg-code)=================================*/



typedef struct cBASE  tBASE;
typedef struct cDICT  tDICT;



/*---(BASE structure)---------------------------*/
struct cBASE {
   /*---(header)---------------*/
   char        b_english   [LEN_TITLE];     /* english word                   */
   char        b_gregg     [LEN_TITLE];     /* gregg translation              */
   /*---(updates)--------------*/
   char        b_shown     [LEN_HUND];      /* gregg as needed to draw        */
   short       b_drawn     [LEN_LABEL];     /* gregg letter indexes           */
   char        b_tree      [LEN_TERSE];     /* gregg as series of letters     */
   char        b_sort      [LEN_LABEL];     /* tree as sortable text          */
   /*---(source)---------------*/
   char        b_file;                      /* source file                    */
   short       b_line;                      /* input line                     */
   /*---(variations)-----------*/
   tDICT      *b_head;                      /* first dictionary variation     */
   tDICT      *b_tail;                      /* last dictionary variation      */
   char        b_ndict;                     /* count of dictinoary entries    */
   /*---(part-of-speech)-------*/
   char        b_part;                      /* primary part of speech         */
   char        b_sub;                       /* sub-part                       */
   /*---(source)---------------*/
   char        b_src;                       /* source version of gregg        */
   char        b_cat;                       /* word-sign, normal, custom, ... */
   short       b_page;                      /* location within source         */
   /*---(frequency)------------*/
   char        b_grp;                       /* grouping by frequency          */
   short       b_freq;                      /* google frequency               */
   /*---(btree)-------------*/
   tSORT      *b_ysort;                     /* base order (tree)              */
   /*---(done)-----------------*/
};



/*---(DICTIONARY structure)---------------------*/
struct cDICT {
   /*---(main)--------------*/
   char        d_english   [LEN_TITLE];     /* english word                   */
   char        d_gregg     [LEN_TITLE];     /* translated to gregg            */
   /*---(pointers)----------*/
   void       *d_prefix;                    /* english prefix                 */
   void       *d_base;                      /* english base                   */
   void       *d_suffix;                    /* english suffix                 */
   /*---(within base)-------*/
   tDICT      *d_prev;                      /* prev base/root variation       */
   tDICT      *d_next;                      /* next base/root variation       */
   /*---(btree)-------------*/
   tSORT      *d_ysort;                     /* english sort                   */
};


