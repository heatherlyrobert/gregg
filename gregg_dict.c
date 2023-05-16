/*============================---(source-start)---============================*/
#include "gregg.h"

/*
 *
 * metis § tn4·· § verbose dictionary parsing mode to find mistakes                       § N2O4rp §  · §
 * metis § dc4·· § add verification of gregg letters entry                                § N2U1OS §  · §
 */

/*
 *  PREFIXES
 *     il      illogical, illegible
 *     in      inanimate, incalculable, inadequate, inaccessable
 *     un   
 *     re      restructure
 *     co      cohabitate
 *     im      immaturity, impartial, immobilize
 *     mis     misread, misbehave, misdirect, misquote
 *     on      onset 
 *
 *     under
 *     over
 *
 *     macro
 *     micro
 *
 *     mega, tera, kilo, centa, ...
 *
 *     mono
 *     uni
 *     multi
 *     omni
 *
 *     non
 *     out
 *     pan   panhellenic, pandemic
 *     para  parable, parade, parapet, paranoid, parboil, parameter
 *
 *     per
 *     pre
 *     pro
 *
 *     vade         e vs. in vs. per   evade, invade, pervade         (latin for "to go")
 *     scribe       describe, subscribe, prescribe, ascribe, transcribe, inscribe, circumscribe proscribe
 *     suade        persuade, dissuade
 *     tain         maintain, obtain, contain, retain, sustain, attain, entertain, pertain, abstain, distain
 *     verse        reverse, adverse, diverse, converse, traverse, perverse, obverse
 *     version      conversion, diversion, inversion, aversion, subversion, reversion, perversion, introversion, extroversion
 *     vert         convert, obert, covert, divert, revert, avert, subvert, invert, pervert, advert, introvert, extrovert
 *     turb         disturb, preturb
 *     use          refuse, excuse, reuse, misuse, diffuse, confuse, fuse, accuse, overuse, underuse, infuse, disuse, defuse, peruse
 *     spect        respect, aspect, suspect, prospect, inspect, retrospect, circumspect, introspect
 *     sist         resist, assist, consist, insist, persist, desist, subsist, synthesist
 *     mute         commute, transmute, permute, telecommute
 *     jury         injury, perjury
 *     haps         perhaps, mishaps
 *     fect         infect, perfect, affect, effect, defect, refect, confect  (latin/english "make/do")
 *     fort         comfort, effort                                           (latin/english "strong")
 *     sect         insect, disect, intersect                                 (latin/english "cut)
 *     cult         agricult, horticulte, cultivate                           (latin/english "grow")
 *     dur          endure, obdure,                                           (latin/english "harden")
 *     rupt         disrupt, ruture, interrupt, bankrupt                      (latin/english "burst")
 *     ject         object, reject, project, inject, conjecture, deject, subject, interject, trajectory   (latin/english "throw")
 *
 */

static char    s_name   [LEN_PATH]  = "/var/lib/gregg/gregg_manual.dict";

char   s_fields  [MAX_FIELD][LEN_TITLE];
char   s_nfield  = 0;

struct cPREFIX {
   char        p_name      [LEN_TERSE];
   char        p_english   [LEN_TERSE];
   char        p_orig      [LEN_LABEL];
   char        p_gregg     [LEN_LABEL];
} g_prefix [LEN_HUND] = {
   /* 123456789   123456789- */
   /*---t-------------------------------------*/
   { "trans"    , "trans"    , "t·<·"         , "t·<·"         },
   /*---d-------------------------------------*/
   { "de"       , "de"       , "d·<·"         , "d·<·"         },
   { "dis"      , "dis"      , "d·<·"         , "d·<·"         },
   { "dys"      , "dys"      , "d·<·"         , "d·<·"         },
   /*---n-------------------------------------*/
   { "in"       , "in"       , "n·<·"         , "n·<·"         },
   { "un"       , "un"       , "n·<·"         , "n·<·"         },
   /*---m-------------------------------------*/
   { "em"       , "em"       , "m·<·"         , "m·<·"         },
   { "im"       , "im"       , "m·<·"         , "m·<·"         },
   /*---r-------------------------------------*/
   { "re"       , "re"       , "r·<·"         , "r·<·"         },
   /*---l-------------------------------------*/
   /*---k-------------------------------------*/
   { "co"       , "co"       , "k·<·"         , "k·<·"         },
   { "con"      , "con"      , "k·<·"         , "k·<·"         },
   { "com"      , "com"      , "k·<·"         , "k·<·"         },
   { "counter"  , "counter"  , "k·<·"         , "k·<·"         },
   /*---g-------------------------------------*/
   /*---f-------------------------------------*/
   { "for"      , "for"      , "f·<·"         , "f·<·"         },
   { "fore"     , "fore"     , "f·<·"         , "f·<·"         },
   { "ful"      , "ful"      , "f·<·"         , "f·<·"         },
   /*---v-------------------------------------*/
   /*---p-------------------------------------*/
   { "par"      , "par"      , "p·a·r·<·"     , "p·<·"         },
   { "para"     , "para"     , "p·a·r·a·<·"   , "p·<·"         },
   { "per"      , "per"      , "p·r·<·"       , "p·<·"         },
   { "peri"     , "peri"     , "p·e·r·e·<·"   , "p·<·"         },
   { "pre"      , "pre"      , "p·r·e·<·"     , "p·<·"         },
   { "pro"      , "pro"      , "p·r·o·<·"     , "p·<·"         },
   { "pur"      , "pur"      , "p·r·<·"       , "p·<·"         },
   /*---b-------------------------------------*/
   /*---s-------------------------------------*/
   { "sub"      , "sub"      , "s·<·"         , "s·<·"         },
   { "circum"   , "circum"   , "z·<·"         , "z·<·"         },
   /*---a-------------------------------------*/
   { "a"        , "a"        , "a·<·"         , "a·<·"         },
   { "an"       , "an"       , "a·<·"         , "a·<·"         },
   { "anti"     , "anti"     , "a·<·"         , "a·<·"         },
   /*---e-------------------------------------*/
   { "e"        , "e"        , "e·<·"         , "e·<·"         },
   { "ex"       , "ex"       , "e·xz·<·"      , "e·<·"         },
   /*---o-------------------------------------*/
   { "ob"       , "ob"       , "o·<·"         , "o·<·"         },
   /*---u-------------------------------------*/
   /*---mutli---------------------------------*/
   { "in-de"    , "inde"     , "nd·<·"        , "nd·<·"        },
   { "re-de"    , "rede"     , "rd·<·"        , "rd·<·"        },
   { "un-de"    , "unde"     , "nd·<·"        , "nd·<·"        },
   { "---"      , "---"      , ""             , ""             },
};


tPARTS  g_parts  [LEN_HUND] = {
   /*---(unassigned)------------------*/
   { '¢', '¢', "bad primary"          , 0 , 0},
   { '·', '·', "tbd"                  , 0 , 0},
   { 'R', '·', "root-word"            , 0 , 0},
   { 'n', '·', "noun"                 , 0 , 0},
   { 'p', '·', "pronoun"              , 0 , 0},
   /*---(verbs)-----------------------*/
   { 'v', '·', "verb"                 , 0 , 0},
   { 'v', 'r',   "regular (base)"     , 0 , 0},
   { 'v', '/',   "irregular (base)"   , 0 , 0},
   { 'v', ' ',   "personally added"   , 0 , 0},
   /*---(adjectives)------------------*/
   { 'j', '·', "adjective"            , 0 , 0},
   { 'd', '·', "adverb"               , 0 , 0},
   { 'e', '·', "preposition"          , 0 , 0},
   { 'c', '·', "conjuction"           , 0 , 0},
   { 'c', 'o',    "coordinating"      , 0 , 0},
   { 'c', 'r',    "coorelating"       , 0 , 0},
   { 'c', 's',    "subordinating"     , 0 , 0},
   { 'i', '·', "interjection"         , 0 , 0},
   /*---(detiremener)-----------------*/
   { 't', '·', "detiremener"          , 0 , 0},
   { 't', 'a',   "articles"           , 0 , 0},
   { 't', 'q',   "quantifiers"        , 0 , 0},
   { 't', 's',   "possessives"        , 0 , 0},
   { 't', 'm',   "demonstratives"     , 0 , 0},
   /*---(end)-------------------------*/
   {  0 ,  0 , "---"                  , 0 , 0},
   /*---(done)------------------------*/
};
char    PARTS_OF_SPEECH [LEN_LABEL] = "";

tSOURCE  g_source   [LEN_LABEL] = {
   /*---(unassigned)------------------*/
   { '¢', "bad source"               , 0 },
   { '·', "tbd"                      , 0 },
   /*---(assiged)---------------------*/
   { '1', "pamplet         1st 1888" , 0 },
   { '2', "revised         2nd 1893" , 0 },
   { '3', "revised         3rd 1898" , 0 },
   { '4', "revised         4th 1908" , 0 },
   { '5', "pre-anniversary 5th 1916" , 0 },
   { '6', "anniversary     6th 1929" , 0 },
   { '7', "simplified      7th 1949" , 0 },
   { '8', "diamond         8th 1963" , 0 },
   { '9', "series 90       9th 1978" , 0 },
   { '´', "centennial     10th 1988" , 0 },
   /*---(end)-------------------------*/
   {  0 , "---"                      , 0 },
   /*---(done)------------------------*/
};

tGRPS   g_grps [LEN_LABEL] = {
   /*---(unassigned)------------------*/
   { '·', "tbd"                      , 0 },
   { 'a', "top 50"                   , 0 },
   { 'b', "top 100"                  , 0 },
   { 'c', "top 200"                  , 0 },
   { 'd', "top 500"                  , 0 },
   { 'e', "top 1000"                 , 0 },
   { 'f', "top 2000"                 , 0 },
   { 'g', "top 5000"                 , 0 },
   { 'h', "top 1000"                 , 0 },
   { 'z', "> 10000"                  , 0 },
   /*---(end)-------------------------*/
   {  0 , "---"                      , 0 },
   /*---(done)------------------------*/
};

tTYPES  g_types    [LEN_TERSE] = {
   /*---(unassigned)------------------*/
   { '·', "tbd"                  , 0 },
   /*---(unassigned)------------------*/
   { '°', "word-sign  (manual)"  , 0 },
   { 'Ï', "word-short (manual)"  , 0 },
   { '¯', "phrases    (manual)"  , 0 },
   { 'm', "manual"               , 0 },
   { 'p', "phrase book"          , 0 },
   { 'd', "dictionary"           , 0 },
   { 'o', "other"                , 0 },
   /*---(end)-------------------------*/
   {  0 , "---"                  , 0 },
   /*---(done)------------------------*/
};

/*> tVARY  g_varies [LEN_DESC] = {                                                                                                                                                                 <* 
 *>    /+-nothing--------- -auto- ---description-------- ---true-endings--------------------------------------- ---examples---------------------------------------------------------------+/       <* 
 *>    { "·"  , ""        , ""   , "base"               , ""                                                   , "observe, care, drink, know, like, use, forget"                          , 0 },   <* 
 *>    /+-verb-tenses----- -auto- ---description-------- ---true-endings--------------------------------------- ---examples---------------------------------------------------------------+/       <* 
 *>    { "´"  , ">·´"     , ""   , "present continuous" , "-ing"                                               , "observing, caring, drinking, knowing, liking, using, forgetting"        , 0 },   <* 
 *>    { "s"  , ">·s"     , ""   , "present simple"     , "-s, -es, -ies"                                      , "observes, cares, drinks, knows, likes, uses, forgets"                   , 0 },   <* 
 *>    { "d"  , ">·d"     , ""   , "past simple"        , "-d, -ed, -ied"                                      , "observed, cared, drank, knew, liked, used, forgot"                      , 0 },   <* 
 *>    { "u"  , ">·u"     , ""   , "future simple"      , "will >"                                             , "will observe"                                                           , 0 },   <* 
 *>    /+-others---------- -auto- ---description-------- ---true-endings--------------------------------------- ---examples---------------------------------------------------------------+/       <* 
 *>    { "r"  , ">·r"     , "rs" , "actor/doer"         , "-r, -ar, -er, -eer, -or, -ant, -ent"                , "observer, carer, drinker, knower, user"                                 , 0 },   <* 
 *>    { "rs" , ">·r·z"   , ""   , "actor/doer (plural)", "-rs, -ars, -ers, -eers, -ors, -ants, -ents"         , "observers, carers, drinkers, knowers, users"                            , 0 },   <* 
 *>    { "b"  , ">·b"     , ""   , "capable of"         , "-ble, -able, -ible"                                 , "observeable, drinkable, knowable, likeable, useable, forgetable"        , 0 },   <* 
 *>    { "v"  , ">·v"     , ""   , "adjective"          , "-ive, -ative, -itive, -ant, -ent, -atic"            , "observant, systematic"                                                  , 0 },   <* 
 *>    { "sh" , ">·sh"    , "shs", "act/process"        , "-tion, -sion"                                       , "observation"                                                            , 0 },   <* 
 *>    { "shs", ">·sh·s"  , ""   , "act/process"        , "-tions, -sions"                                     , "observation"                                                            , 0 },   <* 
 *>    { "e"  , ">·e"     , ""   , "place to do"        , "-ry, -ory, -ery, -ary, -ium"                        , "observatory"                                                            , 0 },   <* 
 *>    { "n"  , ">·n"     , ""   , "state or quality"   , "-ence, -ance, -ancy, -ency, -cy, -acy, -age, -ness" , "observance, fullness"                                                   , 0 },   <* 
 *>    { "l"  , ">·l"     , ""   , "without"            , "-less"                                              , "careless, ageless, useless"                                             , 0 },   <* 
 *>    { "t"  , ">·t"     , "ts" , "office/function"    , "-ate, -ite, -ship, -ment"                           , "certificate, demonstrate, appointment"                                  , 0 },   <* 
 *>    { "ts" , ">·t·z"   , ""   , "office/function"    , "-ates, -ites, -ships, -ments"                       , "certificates"                                                           , 0 },   <* 
 *>    { "f"  , ">·f"     , ""   , "having quality"     , "-ful, -fil"                                         , "careful, beautiful, useful"                                             , 0 },   <* 
 *>    { "fe" , ">·f·e"   , ""   , "having quality"     , "-fully, -filly"                                     , "carefully"                                                              , 0 },   <* 
 *>    /+-adjectives------ -auto- ---description-------- ---true-endings--------------------------------------- ---examples---------------------------------------------------------------+/       <* 
 *>    { "er" , ">·r"     , ""   , "greater"            , "-er, -ier"                                          , "longest, strongest"                                                     , 0 },   <* 
 *>    { "st" , ">·s·t"   , ""   , "greatest"           , "-est, -iest"                                        , "longest, strongest"                                                     , 0 },   <* 
 *>    /+-TBD------------- -auto- ---description-------- ---true-endings--------------------------------------- ---examples---------------------------------------------------------------+/       <* 
 *>    { "Ï"  , ">·´·s"   , ""   , "unsure"             , "-ings"                                              , "comings, goings"                                                        , 0 },   <* 
 *>    { "z"  , ">·z"     , ""   , "become like"        , "-ize, -ise, -itize"                                 , "observational"                                                          , 0 },   <* 
 *>    { "k"  , ">·k"     , ""   , "having nature"      , "-ic, -ac, -ick, -fic"                               , "systemic"                                                               , 0 },   <* 
 *>    { "ke" , ">·k·e"   , ""   , "having nature"      , "-icly, -ically, -acly, -ickly, -ficly"              , "dramatically"                                                           , 0 },   <* 
 *>    { ""   , ""        , ""   , "property of"        , "-al, -il, -ile, -ial"                               , "observational"                                                          , 0 },   <* 
 *>    /+-sentinal-------- -auto- ---description-------- ---true-endings--------------------------------------- ---examples---------------------------------------------------------------+/       <* 
 *>    { "---", "---"     , "---", "---"                , "---"                                                , "---"                                                                    , 0 },   <* 
 *>    /+-done------------ -auto- ---description-------- ---true-endings--------------------------------------- ---examples---------------------------------------------------------------+/       <* 
 *> };                                                                                                                                                                                             <*/
tVARY  g_varies [LEN_FULL] = {
   /*--name---- --suffix---- ---source-- ---true-endings----------------------------------------- ---plus-base--------- ---examples------------------------------------------------------------ cnt */
   { "·"       , ""         , '·',  -1  , ""                                                     , ""     , ""         , "observe, care, drink, know, like, use, forget"                       , 0 },
   /*--name---- --suffix---- ---source-- ---true-endings----------------------------------------- ---plus-base--------- ---examples------------------------------------------------------------ cnt */
   { "´"       , ">·´"      , '5',  29  , "-ing, -thing"                                         , ""     , ""         , "observing, caring, drinking, knowing, liking, using, forgetting"     , 0 },
   {   "Ï"     , ">·z"      , '5',  29  , "-ings, -things"                                       , "´"    , "s"        , "sayings, readings, writings"                                         , 0 },
   {   " "     , ">·z·e"    , '5', 117  , "-ingly, -thingly"                                     , "´"    , "ly"       , "swimmingly"                                                          , 0 },
   {     "+"   , ""         , '·',  -1  ,   "´,Ï, "                                              , ""     , ""         , "longing, longings, longingly"                                        , 0 },
   /*--name---- --suffix---- ---source-- ---true-endings----------------------------------------- ---plus-base--------- ---examples------------------------------------------------------------ cnt */
   { "SN"      , ">·s"      , '·',  -1  , "-s"                                                   , ""     , ""         , "observes, cares, drinks, knows, likes, uses, forgets"                , 0 },
   { "INGN"    , ">·´"      , '5',  29  , "-ing"                                                 , "SN"   , "×ing"     , "observing, caring, drinking, knowing, liking, using, forgetting"     , 0 },
   { "DN"      , ">·d"      , '·',  -1  , "-ed"                                                  , "SN"   , "×ed"      , "observed, cared, drank, knew, liked, used, forgot"                   , 0 },
   {   "+Vn"   , ""         , '·',  -1  ,   "SN,INGN,DN"                                         , ""     , ""         , "observing, caring, drinking, knowing, liking, using, forgetting"     , 0 },
   { "SE"      , ">·s"      , '·',  -1  , "-es"                                                  , ""     , ""         , "observes, cares, idles, likes, ices"                                 , 0 },
   { "INGE"    , ">·´"      , '5',  29  , "-ing"                                                 , "SE"   , "××ing"    , "observing, caring, idling, liking, icing"                            , 0 },
   { "DE"      , ">·d"      , '·',  -1  , "-ed"                                                  , "SE"   , "×d"       , "observed, cared, idled, liked, iced"                                 , 0 },
   {   "+Ve"   , ""         , '·',  -1  ,   "SE,INGE,DE"                                         , ""     , ""         , "observing, caring, drinking, knowing, liking, using, forgetting"     , 0 },
   { "SY"      , ">·s"      , '·',  -1  , "-ies"                                                 , ""     , ""         , ""                                                                    , 0 },
   { "INGY"    , ">·´"      , '5',  29  , "-ying"                                                , "SY"   , "×××ying"  , ""                                                                    , 0 },
   { "DY"      , ">·d"      , '·',  -1  , "-ied"                                                 , "SY"   , "×d"       , ""                                                                    , 0 },
   {   "+Vy"   , ""         , '·',  -1  ,   "SY,INGY,DY"                                         , ""     , ""         , ""                                                                    , 0 },
   /*--name---- --suffix---- ---source-- ---true-endings----------------------------------------- ---plus-base--------- ---examples------------------------------------------------------------ cnt */
   { "t"       , ">·t"      , '·',  -1  , "-ate, -ite, -iate, -itate"                            , ""     , ""         , "certificate, demonstrate, initiate, gravitate"                       , 0 },
   {   "ts"    , ">·t·s"    , '·',  -1  , "-ates, -ites, -iates, -itates"                        , "t"    , "s"        , "cerfificates, demonstrates, initiates, gravitates"                   , 0 },
   {   "tsh"   , ">·t·sh"   , '5',  82  , "-tition, -tation, -tiation"                           , ""     , ""         , "repetition, station, initiation"                                     , 0 },
   {     "TSH" , ">·t·sh·s" , '·',  -1  , "-titions, -tations, -tiations"                        , "tsh"  , "s"        , "repetitions, stations, initiations"                                  , 0 },
   {   "te"    , ">·t·e"    , '·',  -1  , "-ity, -aty, -ety"                                     , ""     , ""         , "gravity, hospitality"                                                , 0 },
   {     "TE"  , ">·t·e·z"  , '·',  -1  , "-ities, -aties, -eties"                               , "te"   , "×ies"     , "gravity, hospitality"                                                , 0 },
   {   "ta"    , ">·t·e"    , '5', 118  , "-itis, -atas"                                         , ""     , ""         , "appendicitis, gravatas"                                              , 0 },
   {   "tn"    , ">·tn"     , '5', 111  , "-ington, -iton, -eton, -aton"                         , ""     , ""         , "washington, graviton"                                                , 0 },
   {     "TN"  , ">·tn·z"   , '·',  -1  , "-ingtons, -itons, -etons, -atons"                     , "tn"   , "s"        , "gravitons"                                                           , 0 },
   {   "tt"    , ">·t"      , '5',  43  , "-ted, -ded"                                           , ""     , ""         , "invited, divided, demanded, printed"                                 , 0 },
   {   "tr"    , ">·t·r"    , '·',  -1  , "-ture"                                                , ""     , ""         , "fixture"                                                             , 0 },
   {     "TR"  , ">·t·r·s"  , '·',  -1  , "-tures"                                               , "tr"   , "s"        , "fixtures"                                                            , 0 },
   /*--name---- --suffix---- ---source-- ---true-endings----------------------------------------- ---plus-base--------- ---examples------------------------------------------------------------ cnt */
   { "d"       , ">·d"      , '·',  -1  ,  "-d, -ed, -ied"                                       , ""     , ""         , "observed, cared, drank, knew, liked, used, forgot"                   , 0 },
   {   "dsh"   , ">·d·sh"   , '5',  82  ,  "-dation, -dition"                                    , ""     , ""         , "addition, gradation"                                                 , 0 },
   {     "DSH" , ">·d·sh·s" , '·',  -1  ,  "-dations, -ditions"                                  , "DSH"  , "s"        , "additions, gradations"                                               , 0 },
   {   "dd"    , ">·dd"     , '5', 118  ,  "-hood, -ward"                                        , ""     , ""         , "childhood, likelihood, homeward, downward, forward, toward"          , 0 },
   {     "DD"  , ">·dd·z"   , '·',  -1  ,  "-hoods, -wards"                                      , "dd"   , "s"        , "childhoods, likelihoods, homewards, downwards, forwards, towards"    , 0 },
   /*--name---- --suffix---- ---source-- ---true-endings----------------------------------------- ---plus-base--------- ---examples------------------------------------------------------------ cnt */
   { "th"      , ">·th"     , '5', 111  , "-worthy, -th, -inth"                                  , ""     , ""         , "praiseworthy, trustworthy, width"                                    , 0 },
   {     "TH"  , ">·th·z"   , '5', 111  , "-ths, -inths"                                         , "th"   , "s"        , "widths"                                                              , 0 },
   /*--name---- --suffix---- ---source-- ---true-endings----------------------------------------- ---plus-base--------- ---examples------------------------------------------------------------ cnt */
   { "tm"      , ">·tm"     , '5', 118  ,  "-dom"                                                , ""     , ""         , "wisdom, kingdom"                                                     , 0 },
   {     "TM"  , ">·tm·z"   , '·',  -1  ,  "-doms"                                               , ""     , ""         , "kingdoms"                                                            , 0 },
   /*--name---- --suffix---- ---source-- ---true-endings----------------------------------------- ---plus-base--------- ---examples------------------------------------------------------------ cnt */
   { "s"       , ">·s"      , '·',  -1  , "-s, -es, -ies"                                        , ""     , ""         , "observes, cares, drinks, knows, likes, uses, forgets"                , 0 },
   {   "st"    , ">·s·t"    , '·',  -1  , "-est, -iest"                                          , ""     , ""         , "longest, strongest, greatest"                                        , 0 },
   {     "ST"  , ">·s·t"    , '·',  -1  , "-est, -iest"                                          , "er"   , "×st"      , "longest, strongest, greatest"                                        , 0 },
   {   "sf"    , ">·s·f"    , '5', 111  , "-self"                                                , ""     , ""         , "himself, yourself"                                                   , 0 },
   {     "SF"  , ">·s·f·s"  , '·',  -1  , "-selves"                                              , "sf"   , "×ves"     , "ourselves"                                                           , 0 },
   {   "se"    , ">·s·e"    , '5', 123  , "-city, -acity, -ecity"                                , ""     , ""         , "tenacity, felicity, pomposity, ethnicity"                            , 0 },
   {   "su"    , ">·s·u"    , '5', 111  , "-sult"                                                , ""     , ""         , "result, insult, consult"                                             , 0 },
   {     "SU"  , ">·s·u"    , '·',  -1  , "-sults"                                               , "su"   , "s"        , "results, insults, consults"                                          , 0 },
   {   "sm"    , ">·s·m"    , '5', 111  , "-sume, -some"                                         , ""     , ""         , "assume, presume, resume"                                             , 0 },
   {   "sz"    , ">·s·z"    , '5', 111  , "-sys, -sis"                                           , ""     , ""         , "prosthesis"                                                          , 0 },
   /*--name---- --suffix---- ---source-- ---true-endings----------------------------------------- ---plus-base--------- ---examples------------------------------------------------------------ cnt */
   { "f"       , ">·f"      , '5', 109  , "-ful, -fil"                                           , ""     , ""         , "careful, beautiful, useful, thoughtful"                              , 0 },
   {   "fe"    , ">·f·e"    , '5', 109  , "-fully, -filly"                                       , "f"    , "ly"       , "carefully, beautifully, usefully, thoughtfully"                      , 0 },
   {   "fn"    , ">·f·n"    , '´',  -1  , "-fulness, -filness"                                   , "f"    , "ness"     , "hopefulness, carefulness, thoughfulness"                             , 0 },
   {     "+F"  , ""         , '·',  -1  ,   "f,fe,fn"                                            , ""     , ""         ,   "careful, carefully, carefulness"                                   , 0 },
   {   "fsh"   , ">·f·sh"   , '5', 109  , "-ification"                                           , ""     , ""         , "specification, modification"                                         , 0 },
   {   "fl"    , ">·f·l"    , '5', 109  , "-flect, -flict"                                       , ""     , ""         , "afflict, reflect"                                                    , 0 },
   {   "flsh"  , ">·f·l·sh" , '5', 109  , "-flection, -fliction"                                 , ""     , ""         , "affliction, reflection"                                              , 0 },
   /*--name---- --suffix---- ---source-- ---true-endings----------------------------------------- ---plus-base--------- ---examples------------------------------------------------------------ cnt */
   { "v"       , ">·v"      , '·',  -1  , "-ive, -ative, -itive, -sive"                          , ""     , ""         , "offensive, narrative, operative"                                     , 0 },
   {   "V"     , ">·v·s"    , '·',  -1  , "-ives, -atives, -itives"                              , "v"    , "s"        , "narratives, operatives"                                              , 0 },
   {   "ve"    , ">·e"      , '·',  -1  , "-ively, -atively, -itively, -sively"                  , "v"    , "ly"       , "offensively, narrativly"                                             , 0 },
   {   "vn"    , ">·n"      , '·',  -1  , "-iveness, -ativeness, -itiveness, -siveness"          , "v"    , "ness"     , "offensiveness, narrativness"                                         , 0 },
   {     "+V"  , ""         , '·',  -1  ,   "v,V,ve,vn"                                          , ""     , ""         , "offensive, offensives, offensively, and offensiveness"               , 0 },
   {   "ve"    , ">·v·e"    , '5', 123  , "-vity, -avity, -evity"                                , ""     , ""         , "depravity, nativity, brevity"                                        , 0 },
   /*--name---- --suffix---- ---source-- ---true-endings----------------------------------------- ---plus-base--------- ---examples------------------------------------------------------------ cnt */
   { "p"       , ">·p"      , '5', 110  , "-ple"                                                 , ""     , ""         , "sample, ample, apple, couple, pimple, purple, example, people"       , 0 },
   {   "po"    , ">·p·o"    , '5', 110  , "-pose"                                                , ""     , ""         , "suppose, purpose, impose, compose, dispose, expost, interpose"       , 0 },
   {   "posh"  , ">·p·o·sh" , '5', 110  , "-position"                                            , ""     , ""         , "supposition, comosition, disposition, exposition"                    , 0 },
   {   "pu"    , ">·p·u"    , '5', 110  , "-pute"                                                , ""     , ""         , "impute, compute, dispute, repute"                                    , 0 },
   {   "push"  , ">·p·u·sh" , '5', 110  , "-putation"                                            , ""     , ""         , "imputation, computation"                                             , 0 },
   {   "pi"    , ">·p·i"    , '5', 110  , "-pire"                                                , ""     , ""         , "aspire, inspire, conspire, respire"                                  , 0 },
   /*--name---- --suffix---- ---source-- ---true-endings----------------------------------------- ---plus-base--------- ---examples------------------------------------------------------------ cnt */
   { "b"       , ">·b"      , '5', 109  , "-ble, -able, -ible, -iable"                           , ""     , ""         , "observeable, available, knowable, likeable, variable, useable"       , 0 },
   {     "B"   , ">·b·z"    , '·',  -1  , "-bles, -ables, -ibles, -iables"                       , "b"    , "s"        , "observeables, knowables, variables"                                  , 0 },
   {   "bl"    , ">·b·l"    , '·',  -1  , "-bly, -ably, -ibly, -iably"                           , "b"    , "×y"       , "feasibly, responsibly, accessibly, variably, observably"             , 0 },
   {   "bn"    , ">·b·n"    , '·',  -1  , "-bleness, -ableness, -ibleness, -iableness"           , "b"    , "ness"     , "feasibleness, accessibleness, variableness, observableness"          , 0 },
   {   "be"    , ">·b·e"    , '5', 117  , "-bility, -ability, -ibility, -iabilty"                , "b"    , "××ility"  , "ability, feasibility, responsibilty, accessability"                  , 0 },
   {   "bes"   , ">·b·e·s"  , '·',  -1  , "-bilities, -abilities, -ibilities, -iabilties"        , "b"    , "××ilities", "abilities, feasibilities, responsibilties, accessabilities"          , 0 },
   {     "+BR" , ""         , '·',  -1  ,   "b,bl,be,bes"                                        , ""     , ""         , "responsible, responsibly, responsibility, and responsibilties"       , 0 },
   {     "+BK" , ""         , '·',  -1  ,   "b,B,bl,be"                                          , ""     , ""         , "knowable, knowably, knowables, knowability"                          , 0 },
   {     "+BV" , ""         , '·',  -1  ,   "b,B,bl,bn,be,bes"                                   , ""     , ""         , "variable, variably, variables, variablity, and variablities"         , 0 },
   /*--name---- --suffix---- ---source-- ---true-endings----------------------------------------- ---plus-base--------- ---examples------------------------------------------------------------ cnt */
   { "r"       , ">·r"      , '·',  -1  , "-r, -ar, -er, -eer, -or, -ant, -ent, -ian"            , ""     , ""         , "observer, carer, drinker, knower, user, librarian"                   , 0 },
   {   "R"     , ">·r·z"    , '·',  -1  , "-rs, -ars, -ers, -eers, -ors, -ants, -ents, ians"     , "r"    , "s"        , "observers, carers, drinkers, knowers, users"                         , 0 },
   {   "re"    , ">·r·e"    , '5', 123  , "-rity, -arity, -erity"                                , ""     , ""         , "popularity, prosperity, majority"                                    , 0 },
   {     "RE"  , ">·r·e·z"  , '·',  -1  , "-rities, -arities, -erities"                          , "re"   , "×ies"     , "majorities"                                                          , 0 },
   /*--name---- --suffix---- ---source-- ---true-endings----------------------------------------- ---plus-base--------- ---examples------------------------------------------------------------ cnt */
   { "l"       , ">·l"      , '5', 109  , "-less"                                                , ""     , ""         , "careless, ageless, useless"                                          , 0 },
   {   "le"    , ">·l·e"    , '·',  -1  , "-lessly"                                              , ""     , ""         , "carelessly, agelessly, uselessly"                                    , 0 },
   {     "LL"  , ">·l·e"    , '·',  -1  , "-lessly"                                              , "l"    , "ly"       , "carelessly, agelessly, uselessly"                                    , 0 },
   {   "le"    , ">·l·n"    , '·',  -1  , "-lessness"                                            , ""     , ""         , "carelessness, agelessness, uselessness"                              , 0 },
   {     "LN"  , ">·l·n"    , '·',  -1  , "-lessness"                                            , "l"    , "ness"     , "carelessness, agelessness, uselessness"                              , 0 },
   {      "+L" , ""         , '·',  -1  ,   "l,LL,LN"                                            , ""     , ""         , "careless, carelessly, carelessness"                                  , 0 },
   {   "le"    , ">·l·e"    , '5', 123  , "-lity, -ality, -elity, -olity"                        , ""     , ""         , "brutality, utility, frivolity"                                       , 0 },
   {     "LE"  , ">·l·e·s"  , '·',  -1  , "-lities, -alities, -elities, -olities"                , "le"   , "×ies"     , "brutalities, utilities, frivolities"                                 , 0 },
   /*--name---- --suffix---- ---source-- ---true-endings----------------------------------------- ---plus-base--------- ---examples------------------------------------------------------------ cnt */
   { "k"       , ">·k"      , '5', 118  , "-acle, -ical, -icle"                                  , ""     , ""         , "tentacle, medical, classical, chronicle"                             , 0 },
   {   "ks"    , ">·k·s"    , '5', 118  , "-acles, -icals, -icles"                               , ""     , ""         , "tentacles, chronicles"                                               , 0 },
   {   "ek"    , ">·e·k"    , '·', 123  , "-ic, -tic, -ac, -ick, -fic, -ific, -temic"            , ""     , ""         , "systemic, medic, classic, choronic, politic, hypnotic, systematic"   , 0 },
   {   "ke"    , ">·k·e"    , '5', 118  , "-ically, -ically, -iclly"                             , ""     , ""         , "medically, classically"                                              , 0 },
   {   "kr"    , ">·k·r"    , '5', 109  , "-cribe"                                               , ""     , ""         , "describe, prescribe, transcribe, subscribe, inscribe"                , 0 },
   {   "krsh"  , ">·k·r·sh" , '5', 109  , "-cription"                                            , ""     , ""         , "description, prescription, subscription, inscription"                , 0 },
   {   "ki"    , ">·k·i"    , '5', 110  , "-quire"                                               , ""     , ""         , "acquire, inquire, require, esquire"                                  , 0 },
   {   "kes"   , ">·k·e·s"  , '5', 110  , "-quest"                                               , ""     , ""         , "request, conquest, bequest, inquest"                                 , 0 },
   {   "kest"  , ">·k·e·s·t", '5', 110  , "-quisite"                                             , ""     , ""         , "requisite, exquisite"                                                , 0 },
   /*--name---- --suffix---- ---source-- ---true-endings----------------------------------------- ---plus-base--------- ---examples------------------------------------------------------------ cnt */
   { "g"       , ">·g"      , '5', 117  , "-gram, -grim, -graph, -gen"                           , ""     , ""         , "monogram, program, telegram, hallucinogen"                           , 0 },
   {   "gs"    , ">·g·z"    , '·',  -1  , "-grams, -grims, -graphs, -gens"                       , ""     , ""         , "monograms, programs, telegrams, hallucinogens"                       , 0 },
   {   "ge"    , ">·e"      , '5', 123  , "-graphy, -genic"                                      , ""     , ""         , "lexigraphy, hallucinogenic"                                          , 0 },
   {   "ges"   , ">·e·s"    , '·',  -1  , "-graphies, -genics"                                   , "ge"   , "×ies"     , "lexigraphies, hallucinogenics"                                       , 0 },
   {   "gr"    , ">·r"      , '5', 123  , "-grapher"                                             , "ge"   , "×er"      , "lexigrapher"                                                         , 0 },
   {   "grs"   , ">·r·z"    , '5', 123  , "-graphers"                                            , "ge"   , "×ers"     , "lexigraphers"                                                        , 0 },
   {   "gu"    , ">·u"      , '5', 123  , "-graphical"                                           , "ge"   , "×ical"    , "lexigraphical"                                                       , 0 },
   {   "gue"   , ">·u·e"    , '5', 123  , "-graphically"                                         , "ge"   , "×ically"  , "lexigraphically"                                                     , 0 },
   {      "+G" , ""         , '·',  -1  ,   "ge,ges,gr,grs,gu,gue"                               , ""     , ""         , "geography, geographies, geographer, geograhical, and geograhically"  , 0 },
   /*--name---- --suffix---- ---source-- ---true-endings----------------------------------------- ---plus-base--------- ---examples------------------------------------------------------------ cnt */
   { "n"       , ">·n"      , '·',  -1  , "-ence, -ance, -ness"                                  , ""     , ""         , "observance, lateness, fitness, awareness, witness"                   , 0 },
   {   "ns"    , ">·n·z"    , '·',  -1  , "-ences, -ances, -ness"                                , "n"    , "s"        , "observances, lateness, fitness, awareness, witness"                  , 0 },
   {   "ne"    , ">·n·e"    , '·',  -1  , "-ancy, -ency, -cy, -acy"                              , ""     , ""         , "observancy, latency, urgency, frequency, transparency"               , 0 },
   {   "NE"    , ">·n·e·z"  , '·',  -1  , "-ancies, -encies, -cies, -acies"                      , "ne"   , "×ies"     , "latencies, urgencies, frequencies, transparencies"                   , 0 },
   {   "nt"    , ">·nt"     , '5', 123  , "-nity, -anity, -enity"                                , ""     , ""         , "humanity, community, affinity, profanity, opportunity, unity"        , 0 },
   {   "nk"    , ">·n·k"    , '5', 123  , "-ntic, -antic, -intic"                                , ""     , ""         , "gigangic, authentic, frantic"                                        , 0 },
   {   "nke"   , ">·n·k·e"  , '5', 123  , "-nticly, -anticly, -inticly"                          , ""     , ""         , "franticly"                                                           , 0 },
   {   "nsh"   , ">·n·sh"   , '5',  82  , "-nition, -nation"                                     , ""     , ""         , "ignition, stagnation, ammunition, cognition"                         , 0 },
   {     "N"   , ">·n"      , '·',  -1  , "-ness"                                                , "er"   , "××ness"   , "observance, lateness, fitness, awareness, witness"                   , 0 },
   {     "+N"  , ""         , '·',  -1  ,   "er,ST,N"                                            , ""     , ""         ,   "harder, hardest, hardness"                                         , 0 },
   /*--name---- --suffix---- ---source-- ---true-endings----------------------------------------- ---plus-base--------- ---examples------------------------------------------------------------ cnt */
   { "m"       , ">·m"      , '5', 109  , "-ment"                                                , ""     , ""         , "experiment, amusement, department"                                   , 0 },
   {   "M"     , ">·m·z"    , '5', 117  , "-ments, -iments, -aments"                             , "m"    , "s"        , "experiments, amusements, departments"                                , 0 },
   {   "mt"    , ">·m·l"    , '5', 117  , "-mental, -imental, -amental"                          , "m"    , "al"       , "experimental, fundamental"                                           , 0 },
   {     "+M"  , ""         , '·',  -1  ,   "m,ms,mt"                                            , ""     , ""         ,   "experiment, experiments, and experimental"                         , 0 },
   {   "msh"   , ">·m·sh"   , '5',  82  , "-mission, -mation"                                    , ""     , ""         , "ommission, formation"                                                , 0 },
   {   "me"    , ">·m·e"    , '5', 123  , "-mity, -amity, -emity"                                , ""     , ""         , "calamity, sublimity, proximity"                                      , 0 },
   {   "mm"    , ">·m"      , '5', 117  , "-ingham"                                              , ""     , ""         , "birmingham, dillingham"                                              , 0 },
   /*--name---- --suffix---- ---source-- ---true-endings----------------------------------------- ---plus-base--------- ---examples------------------------------------------------------------ cnt */
   { "sh"      , ">·sh"     , '5',  30  , "-tion, -sion, -ation, -ition"                         , ""     , ""         , "observation, nation, session, action, motion, fashion"               , 0 },
   {     "SH"  , ">·sh·s"   , '·',  -1  , "-tions, -sions, -ations, -itions"                     , "sh"   , "s"        , "observations, nations, sessions, actions, motions, fashions"         , 0 },
   {   "shu"   , ">·sh·l"   , '5', 117  , "-tional"                                              , "sh"   , "al"       , "observational, national, devotional"                                 , 0 },
   {   "shus"  , ">·sh·l·s" , '·',  -1  , "-tionals"                                             , "sh"   , "als"      , "devotionals"                                                         , 0 },
   {   "she"   , ">·sh·l·e" , '·',  -1  , "-tionally"                                            , "sh"   , "ally"     , "observationally, nationally, devotionally"                           , 0 },
   {     "+SH" , ""         , '·',  -1  ,   "sh,SH,shu,shus,she"                                 , ""     , ""         ,   "navigation, navigational, navigationally"                          , 0 },
   {   "shr"   , ">·sh·u"   , '5', 111  , "-sure"                                                , ""     , ""         , "assure, measure, pressure"                                           , 0 },
   {     "SHR" , ">·sh·u·s" , '5', 111  , "-sures"                                               , ""     , ""         , "assures, pressures"                                                  , 0 },
   {   "sht"   , ">·sh·t"   , '5', 111  , "-tient, -cient"                                       , ""     , ""         , "patient, ancient, efficient, sufficient"                             , 0 },
   {   "shte"  , ">·sh·t·e" , '5', 111  , "-tiently, -ciently"                                   , ""     , ""         , "efficiently, sufficiently"                                           , 0 },
   {   "sh"    , ">·sh"     , '5', 117  , "-ship, -age"                                          , ""     , ""         , "partnership, ownership, hardship, friendship"                        , 0 },
   /*--name---- --suffix---- ---source-- ---true-endings----------------------------------------- ---plus-base--------- ---examples------------------------------------------------------------ cnt */
   { "j"       , ">·j"      , '5',  30  , "-jure"                                                , ""     , ""         , "injure, purjure"                                                     , 0 },
   {    "je"   , ">·j·e"    , '·',  -1  , "-jury"                                                , ""     , ""         , "injury, purjury"                                                     , 0 },
   {      "JE" , ">·j·s"    , '·',  -1  , "-juries"                                              , "je"   , "×ies"     , "injuries"                                                            , 0 },
   /*--name---- --suffix---- ---source-- ---true-endings----------------------------------------- ---plus-base--------- ---examples------------------------------------------------------------ cnt */
   { "a"       , ">·a"      , '5', 123  , "-ic, -tic, -atic, -etic, -itic, -otic"                , ""     , ""         , "logic, politic, hypnotic, systematic"                                , 0 },
   {    "as"   , ">·a·s"    , '5', 123  , "-ics, -tics, -atics, -etics, -itics, -otics"          , "a"    , "s"        , "logics, politics, hypnotics, systematics"                            , 0 },
   {    "an"   , ">·n"      , '5', 123  , "-icness, -ticness, -aticness, -eticness"              , "a"    , "ness"     , "systematicness"                                                      , 0 },
   {    "au"   , ">·u"      , '·',  -1  , "-ical, -tical, -atical, -etical, -itical, -otical"    , "a"    , "al"       , "logical, political, hypnotical, systematical"                        , 0 },
   {    "ae"   , ">·u·e"    , '5', 123  , "-ically, -tically, -atically, -etically, -itically"   , "a"    , "ally"     , "logically, politically, hypnotically, systematically"                , 0 },
   {    "aun"  , ">·n"      , '5', 123  , "-icalness, -ticalness, -aticalness, -eticalness"      , "a"    , "alness"   , "politicalness"                                                       , 0 },
   {    "ar"   , ">·r"      , '5', 123  , "-ician, -tician, -atician, -etician, -itician"        , "a"    , "ian"      , "logician, politician, hypnotician, systematician"                    , 0 },
   {    "ars"  , ">·r·z"    , '·',  -1  , "-icians, -ticians, -aticians, -eticians, -iticians"   , "a"    , "ians"     , "logicians, politicians, hypnoticians, systematicians"                , 0 },
   {      "-A" , ""         , '·',  -1  ,   "a,as,au,ae"                                         , ""     , ""         , "heretic, heretics, heretical, and heretically"                       , 0 },
   {      "+A" , ""         , '·',  -1  ,   "a,as,au,ae,aun"                                     , ""     , ""         , "systematic, systematics, systematical, and systematically"           , 0 },
   {      "*A" , ""         , '·',  -1  ,   "a,as,au,ae,aun,ar,ars"                              , ""     , ""         , "politic, politics, political, politically, and politician"           , 0 },
   {    "at"   , ">·a·t"    , '·',  -1  , "-ant, -ent"                                           , ""     , ""         , "observant"                                                           , 0 },
   {    "ate"  , ">·a·t·e"  , '·',  -1  , "-antly, -ently"                                       , "at"   , "ly"       , "observantly"                                                         , 0 },
   /*--name---- --suffix---- ---source-- ---true-endings----------------------------------------- ---plus-base--------- ---examples------------------------------------------------------------ cnt */
   { "e"       , ">·e"      , '5',  30  , "-y, -ee"                                              , ""     , ""         , "identity, nasty, tasty, handy, ready"                                , 0 },
   {   "E"     , ">·e·s"    , '·',  -1  , "-ies, -ees"                                           , "e"    , "×ies"     , "identities"                                                          , 0 },
   {   "es"    , ">·e·s"    , '·',  -1  , "-ies, -ees"                                           , "e"    , "×ies"     , "identities"                                                          , 0 },
   {   "el"    , ">·e·l"    , '·',  -1  , "-ly, -ily"                                            , ""     , ""         , "nastily, tastily, handily, readily"                                  , 0 },
   {   "esh"   , ">·e·sh"   , '5',  30  , "-ish, -like, -oid"                                    , ""     , ""         , "rougish, brutish, hairlike, lifelike"                                , 0 },
   {   "ez"    , ">·e·z"    , '5', 117  , "-ize, -ise, -itize"                                   , ""     , ""         , "prioritize, rationalize, sympathize"                                 , 0 },
   {   "er"    , ">·e·r"    , '·',  -1  , "-er, -ier"                                            , ""     , ""         , "longer, stronger, greater"                                           , 0 },
   {     "ER"  , ">·s·t"    , '·',  -1  , "-est, -iest"                                          , "er"   , "×st"      , "longest, strongest, greatest"                                        , 0 },
   {   "em"    , ">·e·m"    , '·',  -1  , "-ism"                                                 , ""     , ""         , "brutalism, paganism, barbarism"                                      , 0 },
   {   "es"    , ">·e·s"    , '·',  -1  , "-ist"                                                 , ""     , ""         , "brutalist, paganist"                                                 , 0 },
   {     "ES"  , ">·e·s·z"  , '·',  -1  , "-ists"                                                , "es"   , "s"        , "brutalists, paganists"                                               , 0 },
   {   "en"    , ">·e·n"    , '·',  -1  , "-en, -in"                                             , ""     , ""         , "toughen, woken"                                                      , 0 },
   {   "et"    , ">·e·t"    , '·',  -1  , "-et, -at, -it, -ot, -let"                             , ""     , ""         , "lancet, leaflet"                                                     , 0 },
   {     "ET"  , ">·e·t·s"  , '·',  -1  , "-ets, -ats, -its, -ots, -lets"                        , "et"   , "s"        , "lancets, leaflets"                                                   , 0 },
   /*--name---- --suffix---- ---source-- ---true-endings----------------------------------------- ---plus-base--------- ---examples------------------------------------------------------------ cnt */
   { "i"       , ">·i"      , '·',  -1  , "-fy"                                                  , ""     , ""         , "notify, identify"                                                    , 0 },
   /*--name---- --suffix---- ---source-- ---true-endings----------------------------------------- ---plus-base--------- ---examples------------------------------------------------------------ cnt */
   { "o"       , ">·o"      , '5', 123  , "-alogy, -elogy, -ilogy, -ology"                       , ""     , ""         , "geology, analogy, psychology"                                        , 0 },
   {   "os"    , ">·o·z"    , '·',  -1  , "-alogies, -elogies, -ilogies, -ologies"               , "o"    , "×ies"     , "geologies, psychologies"                                             , 0 },
   {   "ok"    , ">·o·k"    , '·',  -1  , "-alogic, -elogic, -ilogic, -ologic"                   , "o"    , "×ic"      , "geologic, psychologic"                                               , 0 },
   {   "ol"    , ">·u"      , '5', 123  , "-alogical, -elogical, -ilogical, -ological"           , "o"    , "×ical"    , "geological, psychological"                                           , 0 },
   {   "ole"   , ">·u·e"    , '·',  -1  , "-alogically, -elogically, -ilogically, -ologically"   , "o"    , "×ically"  , "geologically, psychologically"                                       , 0 },
   {   "ot"    , ">·r"      , '·',  -1  , "-alogist, -elogist, -ilogist, -ologist"               , "o"    , "×ist"     , "geologist, psychologist"                                             , 0 },
   {   "ots"   , ">·r·z"    , '·',  -1  , "-alogists, -elogists, -ilogists, -ologists"           , "o"    , "×ists"    , "geologists, psychologists"                                           , 0 },
   {      "+O" , ""         , '·',  -1  ,   "o,os,ok,ol,ole,ot,ots"                              , ""     , ""         , "geology, geologies, geologic, geological, geologically, geologist"   , 0 },
   {   "or"    , ">·o·r"    , '·',  -1  , "-ry, -ory, -ery, -ary, -ium, -itory, -atory"          , ""     , ""         , "observatory, aviary"                                                 , 0 },
   /*--name---- --suffix---- ---source-- ---true-endings----------------------------------------- ---plus-base--------- ---examples------------------------------------------------------------ cnt */
   { "u"       , ">·u"      , '·',  -1  , "-ul, -al, -il, -ile, -ial, -el"                       , ""     , ""         , "module, additonal, observational"                                    , 0 },
   {   "U"     , ">·u·s"    , '·',  -1  , "-uls, -als, -ils, -iles, -ials, -els"                 , "u"    , "s"        , "modules, additonals, observationals"                                 , 0 },
   {   "ue"    , ">·u·e"    , '·',  -1  , "-ully, -ally, -illy, -ially, -elly"                   , "u"    , "ly"       , "additonally, observationally"                                        , 0 },
   {      "+UE", ""         , '·',  -1  ,   "u,U,ue"                                             , ""     , ""         , "original, originals, and originally"                                 , 0 },
   {   "um"    , ">·u·m"    , '·',  -1  , "-um"                                                  , "u"    , "××um"     , "optimum, minimum, maximum"                                           , 0 },
   {     "UM"  , ">·u·m·z"  , '·',  -1  , "-ums"                                                 , "u"    , "××ums"    , "optimums, minimums, maximums"                                        , 0 },
   {    "ume" , ">·u·m·e"   , '·',  -1  , "-umly"                                                , "u"    , "××umly"   , "optimums, minimums, maximums"                                        , 0 },
   {      "+UM", ""         , '·',  -1  ,   "u,U,ue,um,UM,ume"                                   , ""     , ""         , "optimal, optimals, optimally, optimum, optimums, optimally"          , 0 },
   {   "us"    , ">·u·s"    , '5', 118  , "-ulus, -elus, -ilus, -alus"                           , ""     , ""         , "modulus"                                                             , 0 },
   {   "ut"    , ">·u·t"    , '5', 118  , "-ulate"                                               , ""     , ""         , "modulate, insulate, emulate, speculate"                              , 0 },
   {   "ud"    , ">·u·d"    , '5', 118  , "-ulated"                                              , ""     , ""         , "modulated, insulated, emulated, speculated"                          , 0 },
   {   "uv"    , ">·u·v"    , '5', 118  , "-ulative"                                             , ""     , ""         , "speculative"                                                         , 0 },
   {   "ush"   , ">·u·sh"   , '5', 118  , "-ulation"                                             , ""     , ""         , "modulation, insulation, emulation, speculation"                      , 0 },
   {   "ur"    , ">·u·r"    , '5', 118  , "-ulator"                                              , ""     , ""         , "modulator, insulator, emulator, speculator"                          , 0 },
   {      "+U" , ""         , '·',  -1  ,   "u,us,ut,ud,ush,ur"                                  , ""     , ""         , "module, modulus, modulate, modulation, and modulator"                , 0 },
   {   "US"    , ">·u·s"    , '·',  -1  , "-ous, -eous, -ious, -aous, -uous"                     , ""     , ""         , "ambiguous, audacious, vicious"                                       , 0 },
   {   "USE"   , ">·u·s·e"  , '·',  -1  , "-ously, -eously, -iously, -aously, -uously"           , "US"   , "ly"       , "ambiguously, audaciously, viciously"                                 , 0 },
   {   "USN"   , ">·u·s·n"  , '·',  -1  , "-ousness, -eousness, -iousness, -aousness, -uousness" , "US"   , "ness"     , "ambiguousness, audaciousness, viciousness"                           , 0 },
   {     "+US" , ""         , '·',  -1  ,   "US,USE,USN"                                         , ""     , ""         , "ambiguous, ambiguously, and ambiguousness"                           , 0 },
   {   "uR"    , ">·u·r"    , '·',  -1  , "-ure"                                                 , ""     , ""         , "feature"                                                             , 0 },
   {     "UR"  , ">·u·r·z"  , '·',  -1  , "-ures"                                                , "uR"   , "s"        , "features"                                                            , 0 },
   /*--name---- --suffix---- ---source-- ---true-endings----------------------------------------- ---plus-base--------- ---examples------------------------------------------------------------ cnt */
   { "---"     , "---"      , '·',  -1  , "---"                                                  , "---"  , ""         , "---"                                                                 , 0 },
   /*--name---- --suffix---- ---source-- ---true-endings----------------------------------------- ---plus-base--------- ---examples------------------------------------------------------------ cnt */
};
char    VARIATIONS      [LEN_RECD] = "";

/*  book  pg
 *
 *   mine
 *               s     -s, -es, -ies
 *               d     -ed
 *               r     -´r, -ar, -er, -or, -eer
 *
 *   connected
 *
 * Ï 5    29     ´     -ing, -thing          being, doing, ringing, anything
 * Ï 5    29     z     -ings                 sayings, readings
 * Ï 5    30     e     -ly, -ily, -ally      only, calmly, totally, early, readily
 * Ï 5    30     sh    -tion, -sion          nation, session, action, motion, fashion
 * Ï 5    43     t     -ted, -ded            invited, divided, demanded, printed
 * Ï 5    82     tsh   -tition, -tation      repitition, station
 * Ï ´    ´      dsh   -dation               addition, gradation
 * Ï ´    ´      nsh   -nition, -nation      ignition, stagnation
 * Ï ´    ´      msh   -mission, -mation     ommission, formation
 * Ï 5    109    b     -able, ible, -ble     notable, audible, noble
 * Ï ´    ´      p     -ple                  ample
 * Ï ´    ´      kr    -cribe                describe, prescribe
 * Ï ´    ´      krsh  -cription             description, prescription
 * Ï ´    ´      fl    -flect, -flict        afflict, reflect
 * Ï ´    ´      flsh  -flection, -fliction  affliction, reflection
 * Ï ´    ´      f     -ful                  thoughtful
 * Ï ´    ´      l     -less                 artless
 * Ï ´    ´      m     -ment                 amusement
 * Ï ´    ´      n     -ness                 lateness
 * Ï 5    110    po    -pose                 impose
 * Ï ´    ´      posh  -posetion             imposition
 * Ï ´    ´      pu    -pute                 impute, compute
 * Ï ´    ´      push  -putation             imputation, computation
 * Ï ´    ´      pi    -pire                 aspire, inspire, conspire, respire
 * Ï ´    ´      ki    -quire                acquire, inquire, require, esquire
 * Ï ´    ´      kes   -quest                request, conquest
 * Ï ´    ´      kest  -quisite              requisite, exquisite
 * Ï 5    111    s     -self                 himself, yourself
 * Ï ´    ´      ss    -selves               themselves, ourselves
 * Ï ´    ´      su    -sult                 result, insult
 * Ï ´    ´      sm    -sume                 assume, resume
 * Ï ´    ´      shu   -shure                assure, measure
 * Ï ´    ´      ju    -jure                 injure, perjure
 * Ï ´    ´      sht   -tient, -cient        patient, ancient
 * Ï ´    ´      shte  -tienty, -cienty      efficiency
 *   ´    ´      uth   -worth                ainsworth
 * Ï ´    ´      the   -worthy               praiseworthy, trustworthy
 *
 *   disjointed
 *
 * Ï 5    117    e     -ingly                knowingly
 * Ï ´    ´      tn    -ington               washington
 * Ï ´    ´      m     -ingham               dillingham
 * Ï ´    ´      b     -bility, -ibility     abilty, feasibility
 * Ï ´    ´      f     -ification            specification, modification
 * Ï ´    ´      g     -gram, -grim          monogram
 * Ï ´    ´      m     -mental, -imental, -amental, -mentality   experimental, fundamental
 * Ï ´    ´      sh    -ship                 partnership, ownership, friendship, hardship
 * Ï 5    118    d     -hood, -ward          childhood, likelihood, homeward, downward, forward, towards
 * Ï ´    ´      k     -acle, -ical, -icle   tentacle, medical, classical, chronicle
 * Ï ´    ´      ts    -itis                 appendicitis
 * Ï ´    ´      u     -ulate                modulate, insulate, emulate
 * Ï ´    ´      ut    -ulated               modulated, insulated, emulated
 * Ï ´    ´      uv    -ulative              speculative, emulative
 * Ï ´    ´      ush   -ulation              speculation, emulation
 * Ï ´    ´      ur    -ulator               speculator, emulator
 * Ï 5    123    r     -´rity, -arity, -erity    popularity, prosperity, majority
 * Ï ´    ´      l     -´lity, -ality, -elity    brutality, utility frivolity
 * Ï ´    ´      s     -´city, -acity, -ecity    tenactiy, felicty, pomposity  
 * Ï ´    ´      v     -´vity, -avity, -evity    depravity, nativity, brevity
 * Ï ´    ´      nt    -´nity, -anity, -enity    urbanity, trinity, affinity
 * Ï ´    ´      mt    -´mity, -amity, -emity    calamity, sublimity, proximity
* Ï ´    ´      st    -´stic, -estic, -istic    elastic, domestic, artistic, atheistic
* Ï ´    ´      a     -´tic                     politic, hypnotic, systematic
* Ï ´    ´      as    -tics                     politics
*   ´    ´      ae    -ticly, -tical            systemtically, 
   * Ï ´    ´      n     -´ntic                    gigantic, authentic, frantic
   * Ï ´    ´      ne    -´nticly                  franticly
   *   ´    ´      Ôe    -´graph                   telegraph, autograph, photograph
   *   ´    ´      Ôa    -´graphy                  telegraphy, autography, photography
   *   ´    ´      Ôer   -´grapher                 telegrapher, autographer, photographer
   * Ï ´    ´      o     -´logy, -´logical         analogy, geological
   *
   *
   *
   *
   *
   *
   *
   *
   *
   *
   */

char
DICT__find_speech       (char a_abbr)
{
   /*---(locals)-------------------------*/
   char        rce         =  -10;
   char        i           =    0;
   for (i = 0; i < LEN_HUND; ++i) {
      if (g_parts [i].abbr == 0)       break;
      if (g_parts [i].sub  != '·')     continue;
      if (g_parts [i].abbr != a_abbr)  continue;
      ++(g_parts [i].count);
      return a_abbr;
   }
   ++(g_parts [0].count);
   return '¢';
}

char
DICT__find_sub          (char a_abbr, char a_sub)
{
   /*---(locals)-------------------------*/
   char        rce         =  -10;
   char        i           =    0;
   for (i = 0; i < LEN_HUND; ++i) {
      if (g_parts [i].abbr == 0)       break;
      if (g_parts [i].abbr != a_abbr)  continue;
      if (g_parts [i].sub  != a_sub)   continue;
      ++(g_parts [i].scount);
      return a_sub;
   }
   ++(g_parts [0].scount);
   return '¢';
}

char
DICT__find_grp          (char a_grp)
{
   /*---(locals)-------------------------*/
   char        rce         =  -10;
   char        i           =    0;
   for (i = 0; i < LEN_LABEL; ++i) {
      if (g_grps  [i].grp  == 0)       break;
      if (g_grps  [i].grp  != a_grp)   continue;
      ++(g_grps  [i].count);
      return a_grp;
   }
   ++(g_grps  [0].count);
   return '¢';
}

char
DICT__find_source       (char a_source)
{
   /*---(locals)-------------------------*/
   char        i           =    0;
   /*---(search)-------------------------*/
   for (i = 0; i < LEN_LABEL; ++i) {
      if (g_source [i].abbr == 0)         break;
      if (g_source [i].abbr != a_source)  continue;
      ++(g_source [i].count);
      return a_source;
   }
   ++(g_source [0].count);
   return '¢';
}

char
DICT__find_type         (char a_type)
{
   /*---(locals)-----------+-----+-----+-*/
   char        i           =    0;
   /*---(search)-------------------------*/
   for (i = 0; i < LEN_TERSE; ++i) {
      if (g_types [i].abbr == 0)         break;
      if (g_types [i].abbr != a_type)    continue;
      ++(g_types [i].count);
      return a_type;
   }
   ++(g_types [0].count);
   return '¢';
}

char
DICT__find_variation    (char a_name [LEN_TERSE], char r_suffix [LEN_TERSE], char r_endings [LEN_HUND], char r_base [LEN_TERSE], char r_change [LEN_TERSE])
{
   /*---(locals)-----------+-----+-----+-*/
   char        rce         =  -10;
   int         i           =    0;
   char        x_suffix2   [LEN_TERSE] = "";
   /*---(default)------------------------*/
   if (r_suffix  != NULL)  strcpy (r_suffix , "");
   if (r_endings != NULL)  strcpy (r_endings, "");
   if (r_base    != NULL)  strcpy (r_base   , "");
   if (r_change  != NULL)  strcpy (r_change , "");
   /*---(defense)------------------------*/
   --rce;  if (a_name == NULL)  return rce;
   /*---(search)-------------------------*/
   for (i = 0; i < LEN_FULL; ++i) {
      if (strcmp (g_varies [i].name, "---" ) == 0)   break;
      if (strcmp (g_varies [i].name, a_name) != 0)   continue;
      /*---(suffix)----------------------*/
      if (r_suffix != NULL)  strlcpy (r_suffix, g_varies [i].suffix, LEN_TERSE);
      ++(g_varies [i].count);
      /*---(group)-----------------------*/
      if      (strchr ("*+-", g_varies [i].name [0]) != NULL) {
         if (r_endings != NULL)  strlcpy (r_endings, g_varies [i].endings, LEN_HUND);
         return '+';
      }
      /*---(single)----------------------*/
      else if (g_varies [i].base [0] != '\0') {
         if (r_base    != NULL)  strlcpy (r_base  , g_varies [i].base  , LEN_TERSE);
         if (r_change  != NULL)  strlcpy (r_change, g_varies [i].change, LEN_TERSE);
         return 'a';
      }
      /*---(done)------------------------*/
      return 0;
   }
   /*---(default)------------------------*/
   --rce;  return rce;
}

char
DICT__find_prefix       (char a_prefix [LEN_TERSE], char r_english [LEN_TERSE], char r_gregg [LEN_LABEL])
{
   /*---(locals)-------------------------*/
   char        rce         =  -10;
   char        i           =    0;
   if (r_gregg   != NULL)  strcpy (r_gregg  , "");
   if (r_english != NULL)  strcpy (r_english, "");
   --rce;  if (a_prefix  == NULL)  return rce;
   --rce;  if (r_english == NULL)  return rce;
   --rce;  if (r_gregg   == NULL)  return rce;
   for (i = 0; i < LEN_HUND; ++i) {
      if (strcmp (g_prefix [i].p_name, "---")    == 0)       break;
      if (strcmp (g_prefix [i].p_name, a_prefix) != 0)       continue;
      strlcpy (r_english, g_prefix [i].p_english, LEN_TERSE);
      strlcpy (r_gregg  , g_prefix [i].p_gregg  , LEN_LABEL);
      return 0;
   }
   return --rce;
}



/*============================--------------------============================*/
/*===----                          dictionary                          ----===*/
/*============================--------------------============================*/
static void      o___DICTIONARY______________o (void) {;}

char
DICT_init          (void)
{
   /*---(locals)-------------------------*/
   short       i           =    0;
   char        t           [LEN_SHORT] = "";
   /*---(header)-------------------------*/
   DEBUG_CONF   yLOG_enter   (__FUNCTION__);
   /*---(prepare)------------------------*/
   strcpy (PARTS_OF_SPEECH, "");
   for (i = 0; i < LEN_HUND; ++i) {
      if (g_parts [i].abbr == 0)   break;
      if (g_parts [i].sub  != '·') continue;
      sprintf (t, "%c", g_parts [i].abbr);
      strlcat (PARTS_OF_SPEECH, t, LEN_LABEL);
   }
   DEBUG_CONF   yLOG_info    ("PARTS_OF..", PARTS_OF_SPEECH);
   strcpy (VARIATIONS     , " ");
   for (i = 0; i < LEN_FULL; ++i) {
      if (strcmp (g_varies [i].name, "---") == 0)   break;
      if (strcmp (g_varies [i].name, ""   ) == 0)   continue;
      if (strcmp (g_varies [i].name, "·"  ) == 0)   continue;
      strlcat (VARIATIONS     , g_varies [i].name, LEN_RECD);
      strlcat (VARIATIONS     , " ", LEN_RECD);
      /*> DEBUG_CONF   yLOG_complex ("vary"      , "%d, å%sæ", i, VARIATIONS);        <*/
   }
   DEBUG_CONF   yLOG_info    ("VARIATIONS", VARIATIONS);
   /*---(complete)-----------------------*/
   DEBUG_CONF   yLOG_exit    (__FUNCTION__);
   return 0;
}

char
DICT__open              (char a_name [LEN_PATH], FILE **r_file)
{
   /*---(locals)-----------+-----+-----+-*/
   char        rce         =  -10;
   FILE       *f           = NULL;
   /*---(header)-------------------------*/
   DEBUG_CONF   yLOG_enter   (__FUNCTION__);
   /*---(defense)------------------------*/
   DEBUG_CONF   yLOG_point   ("a_name"    , a_name);
   --rce;  if (a_name == NULL) {
      DEBUG_CONF   yLOG_exitr   (__FUNCTION__, rce);
      return rce;
   }
   DEBUG_CONF   yLOG_info    ("a_name"    , a_name);
   --rce;  if (a_name [0] == '\0') {
      DEBUG_CONF   yLOG_exitr   (__FUNCTION__, rce);
      return rce;
   }
   DEBUG_CONF   yLOG_point   ("r_file"    , r_file);
   --rce;  if (r_file == NULL) {
      DEBUG_CONF   yLOG_exitr   (__FUNCTION__, rce);
      return rce;
   }
   DEBUG_CONF   yLOG_point   ("*r_file"   , *r_file);
   --rce;  if (*r_file != NULL) {
      DEBUG_CONF   yLOG_exitr   (__FUNCTION__, rce);
      return rce;
   }
   /*---(open)---------------------------*/
   f = fopen (a_name, "rt");
   DEBUG_CONF   yLOG_point   ("f"         , f);
   --rce;  if (f == NULL) {
      DEBUG_CONF   yLOG_exitr   (__FUNCTION__, rce);
      return rce;
   }
   /*---(save-back)----------------------*/
   if (r_file != NULL)  *r_file = f;
   strlcpy (s_name, a_name, LEN_PATH);
   /*---(complete)-----------------------*/
   DEBUG_CONF   yLOG_exit    (__FUNCTION__);
   return 0;
}

char
DICT__close             (FILE **b_file)
{
   /*---(locals)-----------+-----+-----+-*/
   char        rce         =  -10;
   char        rc          =    0;
   FILE       *f           = NULL;
   /*---(header)-------------------------*/
   DEBUG_CONF   yLOG_enter   (__FUNCTION__);
   /*---(defense)------------------------*/
   DEBUG_CONF   yLOG_point   ("b_file"    , b_file);
   --rce;  if (b_file == NULL) {
      DEBUG_CONF   yLOG_exitr   (__FUNCTION__, rce);
      return rce;
   }
   DEBUG_CONF   yLOG_point   ("*b_file"   , *b_file);
   --rce;  if (*b_file == NULL) {
      DEBUG_CONF   yLOG_exitr   (__FUNCTION__, rce);
      return rce;
   }
   f = *b_file;
   /*---(open)---------------------------*/
   rc = fclose (f);
   DEBUG_CONF   yLOG_value   ("close"     , rc);
   --rce;  if (rc < 0) {
      DEBUG_CONF   yLOG_exitr   (__FUNCTION__, rce);
      return rce;
   }
   /*---(save-back)----------------------*/
   if (b_file != NULL)  *b_file = NULL;
   strlcpy (s_name, "", LEN_PATH);
   /*---(complete)-----------------------*/
   DEBUG_CONF   yLOG_exit    (__FUNCTION__);
   return 0;
}

char
DICT__split             (uchar *a_recd)
{
   /*---(locals)-----------+-----+-----+-*/
   char        rce         =  -10;
   char        rc          =    0;
   int         i           =    0;
   char       *p           = NULL;
   char       *q           = "§";
   char       *r           = NULL;
   uchar       x_recd      [LEN_RECD]  = "";
   char       *m           = NULL;
   /*---(header)-------------------------*/
   DEBUG_CONF   yLOG_enter   (__FUNCTION__);
   /*---(prepare)------------------------*/
   for (i = 0; i < MAX_FIELD; ++i)   strcpy (s_fields [i], "");
   s_nfield = 0;
   /*---(defense)------------------------*/
   DEBUG_CONF   yLOG_point   ("a_recd"    , a_recd);
   --rce;  if (a_recd == NULL) {
      DEBUG_CONF   yLOG_exitr   (__FUNCTION__, rce);
      return rce;
   }
   /*---(parse)--------------------------*/
   strlcpy (x_recd, a_recd, LEN_RECD);
   p = strtok_r (x_recd, q, &r);
   while (p != NULL) {
      strlcpy  (s_fields [s_nfield], p, LEN_TITLE);
      m = strchr (s_fields [s_nfield], '¬');
      if (m != NULL)  m [0] = '\0';
      strltrim (s_fields [s_nfield], ySTR_BOTH, LEN_TITLE);
      if (s_nfield == 0)  strllower (s_fields [s_nfield], LEN_TITLE);
      ++s_nfield;
      p = strtok_r (NULL  , q, &r);
   }
   /*---(check count)--------------------*/
   --rce;  if (s_nfield < 2) {
      DEBUG_CONF   yLOG_exitr   (__FUNCTION__, rce);
      return rce;
   }
   /*---(complete)-----------------------*/
   DEBUG_CONF   yLOG_exit    (__FUNCTION__);
   return 0;
}

char
DICT__primary           (short a_line, cchar a_english [LEN_TITLE], cchar a_gregg [LEN_TITLE], cchar a_cats [LEN_TITLE], tWORD **r_word)
{
   /*---(locals)-----------+-----+-----+-*/
   char        rce         =  -10;
   char        rc          =    0;
   char        x_gregg     [LEN_TITLE] = "";
   tWORD      *x_new       = NULL;
   char        x_part      =  '¢';
   /*---(header)-------------------------*/
   DEBUG_CONF   yLOG_enter   (__FUNCTION__);
   /*---(defense)------------------------*/
   DEBUG_CONF   yLOG_point   ("a_english" , a_english);
   --rce;  if (a_english == NULL) {
      yURG_err ('w', "%d primary english word is null/empty", a_line);
      DEBUG_CONF   yLOG_exitr   (__FUNCTION__, rce);
      return rce;
   }
   DEBUG_CONF   yLOG_info    ("a_english" , a_english);
   DEBUG_CONF   yLOG_point   ("a_gregg"   , a_gregg);
   --rce;  if (a_gregg   == NULL) {
      yURG_err ('w', "%d primary gregg word is null/empty", a_line);
      DEBUG_CONF   yLOG_exitr   (__FUNCTION__, rce);
      return rce;
   }
   /*---(check gregg)--------------------*/
   if (a_gregg [0] == '>')   strlcpy (x_gregg, a_gregg + 2, LEN_TITLE);
   else                      strlcpy (x_gregg, a_gregg    , LEN_TITLE);
   strldchg (x_gregg, '.', '·' , LEN_TITLE);
   DEBUG_CONF   yLOG_info    ("x_gregg"   , x_gregg);
   /*---(preview part of speech)---------*/
   x_part = DICT__category_preview (a_cats);
   /*---(add word)-----------------------*/
   rc = WORDS_new_base (a_english, x_gregg, x_part, &x_new);
   DEBUG_CONF   yLOG_value   ("new"       , rc);
   --rce;  if (rc < 0) {
      yURG_err ('w', "%d primary word å%sæ could not be created", a_line, a_english);
      DEBUG_CONF   yLOG_exitr   (__FUNCTION__, rce);
      return rce;
   }
   /*---(hook to lists)------------------*/
   rc = WORDS_hook (x_new);
   DEBUG_CONF   yLOG_value   ("hook"      , rc);
   --rce;  if (rc < 0) {
      DEBUG_CONF   yLOG_exitr   (__FUNCTION__, rce);
      return rce;
   }
   /*---(save-back)----------------------*/
   x_new->w_file = my.r_nfile;
   x_new->w_line = a_line;
   if (r_word != NULL)  *r_word = x_new;
   /*---(status)-------------------------*/
   yURG_msg ('-', "%d primary word å%sæ å%sæ successful", a_line, a_english, x_gregg);
   /*---(complete)-----------------------*/
   DEBUG_CONF   yLOG_exit    (__FUNCTION__);
   return 0;
}

char
DICT__category_five     (tWORD *a_new, char l, cchar *a_cats)
{
   /*---(header)-------------------------*/
   DEBUG_CONF   yLOG_enter   (__FUNCTION__);
   /*---(part-of-speech)-----------------*/
   if (l >= 1)  a_new->w_part = DICT__find_speech (a_cats [0]);
   else         a_new->w_part = DICT__find_speech ('¢');
   /*---(sub part-of-speech)-------------*/
   if (l >= 3)  a_new->w_sub  = DICT__find_sub    (a_new->w_part, a_cats [2]);
   else         a_new->w_sub  = DICT__find_sub    (a_new->w_part, '¢');
   /*---(source group)-------------------*/
   if (l >= 5)  a_new->w_grp  = DICT__find_grp    (a_cats [4]);
   else         a_new->w_grp  = DICT__find_grp    ('¢');
   /*---(source)-------------------------*/
   if (l >= 7)  a_new->w_src  = DICT__find_source (a_cats [6]);
   else         a_new->w_src  = DICT__find_source ('¢');
   /*---(category)-----------------------*/
   if (l >= 9)  a_new->w_cat  = DICT__find_type   (a_cats [8]);
   else         a_new->w_cat  = DICT__find_type   ('¢');
   /*---(complete)-----------------------*/
   DEBUG_CONF   yLOG_exit    (__FUNCTION__);
   return 0;
}

char
DICT__category_six      (tWORD *a_new, char l, cchar *a_cats)
{
   /*---(locals)-----------+-----+-----+-*/
   short       x_page      =    0;
   /*---(header)-------------------------*/
   DEBUG_CONF   yLOG_enter   (__FUNCTION__);
   /*---(part-of-speech)-----------------*/
   if (l >= 1)  a_new->w_part = DICT__find_speech (a_cats [0]);
   else         a_new->w_part = DICT__find_speech ('¢');
   /*---(sub part-of-speech)-------------*/
   if (l >= 3)  a_new->w_sub  = DICT__find_sub    (a_new->w_part, a_cats [2]);
   else         a_new->w_sub  = DICT__find_sub    (a_new->w_part, '¢');
   /*---(source group)-------------------*/
   if (l >= 5)  a_new->w_grp  = DICT__find_grp    (a_cats [4]);
   else         a_new->w_grp  = DICT__find_grp    ('¢');
   /*---(source)-------------------------*/
   if (l >= 8)  a_new->w_src  = DICT__find_source (a_cats [7]);
   else         a_new->w_src  = DICT__find_source ('¢');
   /*---(category)-----------------------*/
   if (l >= 10) a_new->w_cat  = DICT__find_type   (a_cats [9]);
   else         a_new->w_cat  = DICT__find_type   ('¢');
   /*---(page)---------------------------*/
   if (l >= 12) a_new->w_page = atoi (a_cats + 11);
   /*---(complete)-----------------------*/
   DEBUG_CONF   yLOG_exit    (__FUNCTION__);
   return 0;
}

char
DICT__category_seven    (tWORD *a_new, char l, cchar *a_cats)
{
   /*---(locals)-----------+-----+-----+-*/
   char        t           [LEN_TERSE] = "";
   /*---(header)-------------------------*/
   DEBUG_CONF   yLOG_enter   (__FUNCTION__);
   /*---(part-of-speech)-----------------*/
   if (l >= 1)  a_new->w_part = DICT__find_speech (a_cats [0]);
   else         a_new->w_part = DICT__find_speech ('¢');
   if (l >= 3)  a_new->w_sub  = DICT__find_sub    (a_new->w_part, a_cats [2]);
   else         a_new->w_sub  = DICT__find_sub    (a_new->w_part, '¢');
   /*---(source)-------------------------*/
   if (l >= 6)  a_new->w_src  = DICT__find_source (a_cats [5]);
   else         a_new->w_src  = DICT__find_source ('¢');
   if (l >= 8)  a_new->w_cat  = DICT__find_type   (a_cats [7]);
   else         a_new->w_cat  = DICT__find_type   ('¢');
   if (l >= 10) {
      sprintf (t, "%-3.3s", a_cats + 9);
      a_new->w_page = atoi (t);
   }
   /*---(frequency)----------------------*/
   if (l >= 14) a_new->w_grp  = DICT__find_grp    (a_cats [13]);
   else         a_new->w_grp  = DICT__find_grp    ('¢');
   if (l >= 16) a_new->w_freq = atoi (a_cats + 15);
   /*---(complete)-----------------------*/
   DEBUG_CONF   yLOG_exit    (__FUNCTION__);
   return 0;
}

char
DICT__category_preview  (cchar *a_cats)
{
   /*---(locals)-----------+-----+-----+-*/
   char        rce         =  -10;
   char        l           =    0;
   /*---(defenses)-----------------------*/
   if (a_cats == NULL)  return '¢';
   /*---(defenses)-----------------------*/
   l = strlen (a_cats);
   if (l >= 1)  return DICT__find_speech (a_cats [0]);
   /*---(complete)-----------------------*/
   return '¢';
}

char
DICT__category          (tWORD *a_new, cchar *a_cats)
{
   /*---(locals)-----------+-----+-----+-*/
   char        rce         =  -10;
   char        rc          =    0;
   char        l           =    0;
   /*---(header)-------------------------*/
   DEBUG_CONF   yLOG_enter   (__FUNCTION__);
   /*---(defenses)-----------------------*/
   DEBUG_CONF   yLOG_point   ("a_new"     , a_new);
   --rce;  if (a_new == NULL) {
      DEBUG_CONF   yLOG_exitr   (__FUNCTION__, rce);
      return rce;
   }
   DEBUG_CONF   yLOG_point   ("a_cats"    , a_cats);
   --rce;  if (a_cats == NULL) {
      DEBUG_CONF   yLOG_exitr   (__FUNCTION__, rce);
      return rce;
   }
   l = strlen (a_cats);
   /*---(part-of-speech)-----------------*/
   if (l <= 9) {
      DICT__category_five  (a_new, l, a_cats);
   } else if (l <= 13) {
      DICT__category_six   (a_new, l, a_cats);
   } else {
      DICT__category_seven (a_new, l, a_cats);
   }
   /*---(complete)-----------------------*/
   DEBUG_CONF   yLOG_exit    (__FUNCTION__);
   return 0;
}

char
DICT__variation_quick   (tWORD *a_base, tWORD *a_last, char a_english [LEN_TITLE], char a_vary [LEN_TERSE], char a_prefix [LEN_TERSE], char a_suffix [LEN_TERSE], tWORD **r_new)
{
   /*---(locals)-----------+-----+-----+-*/
   char        rce         =  -10;
   char        rc          =    0;
   char        x_gregg     [LEN_TITLE] = "";
   tWORD      *x_new       = NULL;
   /*---(header)-------------------------*/
   DEBUG_CONF   yLOG_enter   (__FUNCTION__);
   /*---(build gregg)--------------------*/
   sprintf (x_gregg, "%s·%s", a_base->w_gregg, a_suffix);
   DEBUG_CONF   yLOG_info    ("x_gregg"   , x_gregg);
   /*---(add word)-----------------------*/
   rc = WORDS_new_vary (a_english, x_gregg, a_base->w_part, &x_new);
   DEBUG_CONF   yLOG_value   ("new"       , rc);
   --rce;  if (rc < 0) {
      DEBUG_CONF   yLOG_exitr   (__FUNCTION__, rce);
      return rce;
   }
   /*---(hook to lists)------------------*/
   rc = WORDS_hook (x_new);
   DEBUG_CONF   yLOG_value   ("hook"      , rc);
   --rce;  if (rc < 0) {
      DEBUG_CONF   yLOG_exitr   (__FUNCTION__, rce);
      return rce;
   }
   /*---(carry-over)---------------------*/
   x_new->w_file = a_base->w_file;
   x_new->w_line = a_base->w_line;
   strlcpy (x_new->w_vary, a_vary, LEN_TERSE);
   /*---(past-of-speech)-----------------*/
   x_new->w_part = a_base->w_part;
   x_new->w_sub  = a_base->w_sub;
   /*---(source)-------------------------*/
   x_new->w_src  = a_base->w_src;
   x_new->w_cat  = a_base->w_cat;
   x_new->w_page = a_base->w_page;
   /*---(frequency)----------------------*/
   x_new->w_grp  = a_base->w_grp;
   x_new->w_freq = a_base->w_freq;
   /*---(connect to chain)---------------*/
   x_new->w_base = a_base;
   ++(x_new->w_base->w_nvary);
   if (a_last != NULL)  a_last->w_next = x_new;
   /*---(save-back)----------------------*/
   if (r_new  != NULL)  *r_new = x_new;
   /*---(complete)-----------------------*/
   DEBUG_CONF   yLOG_exit    (__FUNCTION__);
   return 0;
}

char
DICT__var_english       (char a_english [LEN_TITLE], char a_change [LEN_TERSE], char r_update [LEN_TITLE])
{
   /*---(locals)-----------+-----+-----+-*/
   char        rce         =  -10;
   char        l           =    0;
   /*---(default)------------------------*/
   if (r_update != NULL)  strcpy (r_update, "");
   /*---(defenses)-----------------------*/
   --rce;  if (a_english == NULL)   return rce;
   --rce;  if (a_change  == NULL)   return rce;
   --rce;  if (r_update  == NULL)   return rce;
   /*---(prepare)------------------------*/
   strlcpy (r_update, a_english, LEN_TITLE);
   l = strlen (r_update);
   /*---(make updates)-------------------*/
   if (strncmp (a_change, "×××", 3) == 0) {
      r_update [l - 3] = r_update [l - 2] = r_update [l - 1] = '\0';
      strlcat (r_update, a_change + 3, LEN_TITLE);
   } else if (strncmp (a_change, "××", 2) == 0) {
      r_update [l - 2] = r_update [l - 1] = '\0';
      strlcat (r_update, a_change + 2, LEN_TITLE);
   } else if (a_change [0] == '×') {
      r_update [l - 1] = '\0';
      strlcat (r_update, a_change + 1, LEN_TITLE);
   } else {
      strlcat (r_update, a_change    , LEN_TITLE);
   }
   /*---(complete)-----------------------*/
   return 0;
}

char
DICT__variation         (tWORD *a_base, tWORD *a_last, cchar a_english [LEN_TERSE], cchar a_gregg [LEN_TERSE], cchar *a_vary, tWORD **r_new)
{
   /*---(locals)-----------+-----+-----+-*/
   char        rce         =  -10;
   char        rc          =    0;
   char        x_action    =  '-';
   char       *p           = NULL;
   char       *q           =  ",";
   char       *r           = NULL;
   char        l           =    0;
   char        x_left      =    0;
   char        x_type      [LEN_TERSE]  = "";
   char        x_english   [LEN_TITLE]  = "";
   char        t           [LEN_TITLE]  = "";
   char        x_suffix    [LEN_TERSE] = "";
   char        x_endings   [LEN_HUND]  = "";
   char        x_base      [LEN_TERSE] = "";
   char        x_change    [LEN_TERSE] = "";
   tWORD     *x_vary      = NULL;
   tWORD     *x_save      = NULL;
   uchar       x_check     [LEN_LABEL] = "";
   /*---(header)-------------------------*/
   DEBUG_CONF   yLOG_enter   (__FUNCTION__);
   /*---(defenses)-----------------------*/
   DEBUG_CONF   yLOG_point   ("a_base"    , a_base);
   --rce;  if (a_base == NULL) {
      DEBUG_CONF   yLOG_exitr   (__FUNCTION__, rce);
      return rce;
   }
   DEBUG_CONF   yLOG_point   ("a_vary"    , a_vary);
   --rce;  if (a_vary == NULL) {
      DEBUG_CONF   yLOG_exitr   (__FUNCTION__, rce);
      return rce;
   }
   DEBUG_CONF   yLOG_info    ("a_vary"    , a_vary);
   /*---(parse)--------------------------*/
   p = strchr (a_vary, ')');
   DEBUG_CONF   yLOG_point   ("paren"     , p);
   --rce;  if (p == NULL) {
      DEBUG_CONF   yLOG_exitr   (__FUNCTION__, rce);
      return rce;
   }
   l = p - a_vary;
   DEBUG_CONF   yLOG_value   ("l"         , l);
   --rce;  if (l < 1 || l > 4) {
      DEBUG_CONF   yLOG_exitr   (__FUNCTION__, rce);
      return rce;
   }
   sprintf (x_type   , "%*.*s", l, l, a_vary);
   l = strlen (a_vary);
   x_left = l - (p - a_vary) - 2;
   DEBUG_CONF   yLOG_value   ("l"         , l);
   DEBUG_CONF   yLOG_value   ("x_left"    , x_left);
   --rce;  if (x_left < 2) {
      DEBUG_CONF   yLOG_exitr   (__FUNCTION__, rce);
      return rce;
   }
   sprintf (x_english, "%s%s"   , a_english, p + 2);
   /*---(get type)-----------------------*/
   sprintf (x_check, " %s ", x_type);
   --rce;  if (strstr (VARIATIONS, x_check) == NULL) {
      DEBUG_CONF   yLOG_exitr   (__FUNCTION__, rce);
      return rce;
   }
   /*---(find variation)-----------------*/
   x_action = DICT__find_variation (x_type, x_suffix, x_endings, x_base, x_change);
   DEBUG_CONF   yLOG_value   ("x_action"  , x_action);
   --rce;  if (x_action < 0) {
      DEBUG_CONF   yLOG_exitr   (__FUNCTION__, rce);
      return rce;
   }
   /*---(simple)-------------------------*/
   if (x_action == 0) {
      rc = DICT__variation_quick (a_base, a_last, x_english, x_type, a_gregg, x_suffix, &x_vary);
      if (rc >= 0)  x_save = x_vary;
   }
   /*---(also version)-------------------*/
   else if (x_action == 'a') {
      /*---(make variation)--------------*/
      DICT__var_english (x_english, x_change, t);
      DEBUG_CONF   yLOG_complex ("english"   , "%s, %s, %s", x_english, x_change, t);
      rc = DICT__variation_quick (a_base, a_last, t, x_type, a_gregg, x_suffix, &x_vary);
      if (rc >= 0)  x_save = x_vary;
      /*---(make base)-------------------*/
      DICT__find_variation (x_base, x_suffix, NULL, NULL, NULL);
      rc = DICT__variation_quick (a_base, x_save, x_english, x_base, a_gregg, x_suffix, &x_vary);
      if (rc >= 0)  x_save = x_vary;
   }
   /*---(group version)------------------*/
   else if (x_action == '+') {
      /*---(make base)-------------------*/
      p = strtok_r (x_endings, q, &r);
      DICT__find_variation (p, x_suffix, NULL, NULL, NULL);
      rc = DICT__variation_quick (a_base, a_last, x_english, p, a_gregg, x_suffix, &x_vary);
      if (rc >= 0)  x_save = x_vary;
      /*---(cycle others)----------------*/
      p = strtok_r (NULL, q, &r);
      while (p != NULL) {
         /*---(find)---------------------*/
         DICT__find_variation (p, x_suffix, NULL, NULL, x_change);
         DEBUG_CONF   yLOG_complex ("variation" , "%s, %s, %s", p, x_suffix, x_change);
         /*---(update)-------------------*/
         DICT__var_english (x_english, x_change, t);
         DEBUG_CONF   yLOG_complex ("english"   , "%s, %s, %s", x_english, x_change, t);
         /*---(add)----------------------*/
         rc = DICT__variation_quick (a_base, x_save, t, p, a_gregg, x_suffix, &x_vary);
         if (rc >= 0)  x_save = x_vary;
         /*---(next)---------------------*/
         p = strtok_r (NULL, q, &r);
      }
      /*---(done)------------------------*/
   }
   /*---(save-back)----------------------*/
   if (r_new  != NULL)  *r_new = x_vary;
   /*---(complete)-----------------------*/
   DEBUG_CONF   yLOG_exit    (__FUNCTION__);
   return 0;
}

char
DICT__parse             (short a_line, cchar a_english [LEN_TERSE], cchar a_gregg [LEN_TERSE], cchar a_recd [LEN_RECD])
{
   /*---(locals)-----------+-----+-----+-*/
   char        rce         =  -10;
   char        rc          =    0;
   int         i           =    0;
   char        x_pre       [LEN_TERSE] = "";
   char        x_base      [LEN_TITLE] = "";
   char        x_revised   =  '-';
   char        x_english   [LEN_TITLE] = "";
   char        x_gregg     [LEN_TITLE] = "";
   tWORD      *x_new       = NULL;
   tWORD      *x_last      = NULL;
   tWORD      *x_vary      = NULL;
   /*---(header)-------------------------*/
   DEBUG_CONF   yLOG_enter   (__FUNCTION__);
   /*---(split)--------------------------*/
   rc = DICT__split (a_recd);
   DEBUG_CONF   yLOG_value   ("split"     , rc);
   --rce;  if (rc < 0) {
      DEBUG_CONF   yLOG_exitr   (__FUNCTION__, rce);
      return rce;
   }
   /*---(preprare english)---------------*/
   strlcpy (x_pre , a_english   , LEN_TERSE);
   strlcpy (x_base, s_fields [0], LEN_TITLE);
   if (strcmp (x_pre, "") != NULL) {
      rc = DICT__pre_update  (x_pre, x_base);
      rc = DICT__find_prefix (x_pre, x_english, x_gregg);
      DEBUG_CONF   yLOG_complex ("find"      , "%4d, %s, %s", rc, x_english, x_gregg);
      --rce;  if (rc < 0) {
         DEBUG_CONF   yLOG_exitr   (__FUNCTION__, rce);
         return rce;
      }
   }
   sprintf (x_english, "%s%s", x_pre, x_base);
   /*---(update with prefix)-------------*/
   if (s_nfield >= 5 && (strcmp (s_fields [4], "·") != 0 && strcmp (s_fields [4], "") != 0))   x_revised = 'y';
   DEBUG_CONF   yLOG_value   ("x_revised" , x_revised);
   /*---(preprare gregg)-----------------*/
   if (x_revised != 'y')  sprintf (x_gregg  , "%s%s", x_gregg, s_fields [2]);
   else                   sprintf (x_gregg  , "%s%s", x_gregg, s_fields [4]);
   DEBUG_CONF   yLOG_complex ("words"     , "%-20.20s, %s", x_english, x_gregg);
   /*---(primary)------------------------*/
   rc = DICT__primary (a_line, x_english, x_gregg, s_fields [3], &x_new);
   DEBUG_CONF   yLOG_value   ("primary"   , rc);
   --rce;  if (rc < 0) {
      DEBUG_CONF   yLOG_exitr   (__FUNCTION__, rce);
      return rce;
   }
   /*> if (x_revised != 'y')  x_new->official = x_new->w_gregg;                       <* 
    *> else                   x_new->official  = strdup (s_fields [2]);               <*/
   x_new->w_nvary = 1;
   /*---(categories)---------------------*/
   if (s_nfield > 3) {
      rc = DICT__category (x_new, s_fields [3]);
      DEBUG_CONF   yLOG_value   ("primary"   , rc);
      --rce;  if (rc < 0) {
         DEBUG_CONF   yLOG_exitr   (__FUNCTION__, rce);
         return rce;
      }
   }
   /*---(variations)---------------------*/
   x_last = x_new;
   for (i = 5; i <= MAX_FIELD; ++i) {
      if (strlen (s_fields [i]) == 0)  continue;
      rc = DICT__variation (x_new, x_last, x_pre, x_gregg, s_fields [i], &x_vary);
      DEBUG_CONF   yLOG_value   ("variation" , rc);
      --rce;  if (rc < 0) {
         DEBUG_CONF   yLOG_note    ("variation failed, continuing");
      }
      x_last = x_vary;
   }
   /*---(complete)-----------------------*/
   DEBUG_CONF   yLOG_exit    (__FUNCTION__);
   return 0;
}

char
DICT__parse_easy        (short a_line, cchar a_recd [LEN_RECD])
{
   return DICT__parse (a_line, "", "", a_recd);
}

char
DICT__pre_update        (char b_prefix [LEN_LABEL], char b_english [LEN_TITLE])
{
   /*---(locals)-----------+-----+-----+-*/
   char        rce         =  -10;
   char        l           =    0;
   char        t           [LEN_TITLE] = "";
   char       *p           = NULL;
   /*---(begin)----------------------------*/
   DEBUG_CONF   yLOG_senter  (__FUNCTION__);
   /*---(defenses)-----------------------*/
   DEBUG_CONF   yLOG_spoint  (b_prefix);
   --rce;  if (b_prefix  == NULL) {
      return rce;
   }
   DEBUG_CONF   yLOG_snote   (b_prefix);
   DEBUG_CONF   yLOG_spoint  (b_english);
   --rce;  if (b_english == NULL) {
      return rce;
   }
   DEBUG_CONF   yLOG_snote   (b_english);
   /*---(check for root change)----------*/
   p = strchr (b_prefix, 'Ö');
   DEBUG_CONF   yLOG_spoint  (p);
   if (p != NULL) {
      DEBUG_CONF   yLOG_snote   ("root change");
      if (strncmp (p, "ÖÖ", 2) == 0) {
         p [0] = p [1] = '\0';
         sprintf (t, "%s%s", p + 2, b_english + 2);
      }
      else {
         p [0] = '\0';
         sprintf (t, "%s%s", p + 1, b_english + 1);
      }
      strlcpy (b_english, t, LEN_TITLE);
   }
   /*---(check for prefix change)--------*/
   p = strchr (b_prefix, '×');
   if (p != NULL) {
      DEBUG_CONF   yLOG_snote   ("prefix change");
      p [0] = p [-1] = '\0';
   }
   /*---(report-out)---------------------*/
   DEBUG_CONF   yLOG_snote   (b_prefix);
   DEBUG_CONF   yLOG_snote   (b_english);
   /*---(complete)-----------------------*/
   DEBUG_CONF   yLOG_sexit   (__FUNCTION__);
   return 0;
}

char
DICT__parse_all         (short a_line, cchar a_prefixes [LEN_HUND], cchar a_recd [LEN_RECD])
{
   /*---(locals)-----------+-----+-----+-*/
   char        rce         =  -10;
   char        rc          =    0;
   char        x_prefixes  [LEN_HUND]  = "";
   char        x_prefix    [LEN_TERSE] = "";
   char        x_english   [LEN_TERSE] = "";
   char        t           [LEN_TERSE] = "";
   char        x_gregg     [LEN_TERSE] = "";
   char       *p           = NULL;
   char       *q           =  ","; 
   char       *r           = NULL;
   /*---(begin)----------------------------*/
   DEBUG_CONF   yLOG_enter   (__FUNCTION__);
   /*---(defense)------------------------*/
   DEBUG_CONF   yLOG_point   ("a_prefixes", a_prefixes);
   --rce;  if (a_prefixes == NULL) {
      DEBUG_CONF   yLOG_exitr   (__FUNCTION__, rce);
      return rce;
   }
   DEBUG_CONF   yLOG_info    ("a_prefixes", a_prefixes);
   strlcpy (x_prefixes, a_prefixes, LEN_HUND);
   DEBUG_CONF   yLOG_point   ("a_recd"    , a_recd);
   --rce;  if (a_recd == NULL) {
      DEBUG_CONF   yLOG_exitr   (__FUNCTION__, rce);
      return rce;
   }
   /*---(short-cut)----------------------*/
   if (strcmp (x_prefixes, "") == 0  || strcmp (x_prefixes, "·") == 0) {
      DEBUG_CONF   yLOG_note    ("no prefixes passed, simple");
      rc = DICT__parse_easy (a_line, a_recd);
      DEBUG_CONF   yLOG_exit    (__FUNCTION__);
      return rc;
   }
   /*---(prefixes)-----------------------*/
   DEBUG_CONF   yLOG_note    ("parsing prefixes");
   p = strtok_r (x_prefixes, q, &r);
   while (p != NULL) {
      strlcpy  (x_prefix, p, LEN_TERSE);
      strltrim (x_prefix, ySTR_BOTH, LEN_TERSE);
      DEBUG_CONF   yLOG_info    ("x_prefix"  , x_prefix);
      if (strcmp (x_prefix, "´") == 0) {
         DEBUG_CONF   yLOG_note    ("requested base prefix also");
         rc = DICT__parse_easy (a_line, a_recd);
      } else {
         DEBUG_CONF   yLOG_note    ("handling prefix");
         rc = DICT__parse (a_line, x_prefix, x_gregg, a_recd);
      }
      p = strtok_r (NULL  , q, &r);
   }
   /*---(complete)-----------------------*/
   DEBUG_CONF   yLOG_exit    (__FUNCTION__);
   return 0;
}

char
DICT__read              (FILE *a_file, short *r_line, char r_prefixes [LEN_HUND], char r_recd [LEN_RECD])
{
   /*---(locals)-----------+-----+-----+-*/
   char        rce         =  -10;
   char        rc          =    0;
   int         x_len       =    0;
   char       *p           = NULL;
   char        x_prefixes  [LEN_HUND]  = "";
   /*---(begin)----------------------------*/
   DEBUG_CONF   yLOG_enter   (__FUNCTION__);
   /*---(default)------------------------*/
   if (r_recd     != NULL)  strcpy (r_recd    , "(eof)");
   if (r_prefixes != NULL)  strcpy (r_prefixes, "");
   /*---(defense)------------------------*/
   DEBUG_CONF   yLOG_point   ("a_file"    , a_file);
   --rce;  if (a_file == NULL) {
      DEBUG_CONF   yLOG_exitr   (__FUNCTION__, rce);
      return rce;
   }
   DEBUG_CONF   yLOG_point   ("r_recd"    , r_recd);
   --rce;  if (r_recd == NULL) {
      DEBUG_CONF   yLOG_exitr   (__FUNCTION__, rce);
      return rce;
   }
   DEBUG_CONF   yLOG_point   ("r_prefixes", r_prefixes);
   --rce;  if (r_prefixes == NULL) {
      DEBUG_CONF   yLOG_exitr   (__FUNCTION__, rce);
      return rce;
   }
   /*---(check record)-------------------*/
   --rce;  if (feof (a_file)) {
      DEBUG_CONF   yLOG_exitr   (__FUNCTION__, rce);
      return rce;
   }
   /*---(find a record)------------------*/
   --rce;  while (1) {
      /*---(read)------------------------*/
      fgets (r_recd, LEN_RECD, a_file);
      if (feof (a_file)) {
         DEBUG_CONF   yLOG_exitr   (__FUNCTION__, rce);
         strcpy (r_recd, "(eof)");
         return rce;
      }
      if (r_line != NULL)  ++(*r_line);
      /*---(filter)----------------------*/
      if (r_recd [0] == '\0')  continue;
      if (r_recd [0] == '#')   continue;
      if (r_recd [0] == ' ')   continue;
      /*---(check for description)-------*/
      p = strchr (r_recd, '¨');
      if (p != NULL)  p [0] = '\0';
      /*---(check for prefixes)----------*/
      p = strchr (r_recd, '¼');
      if (p != NULL) {
         strlcpy (x_prefixes, p + 1, LEN_HUND);
         strltrim (x_prefixes, ySTR_BOTH, LEN_HUND);
         if (r_prefixes != NULL)   strlcpy (r_prefixes, x_prefixes, LEN_HUND);
         p [0] = '\0';
      }
      /*---(tear off tbd extras)---------*/
      p = strchr (r_recd, '#');
      if (p != NULL)  p [0] = '\0';
      p = strchr (r_recd, '=');
      if (p != NULL)  p [0] = '\0';
      /*---(clean-up)--------------------*/
      x_len = strlen (r_recd);
      if (x_len < 20)          continue;
      if (r_recd [x_len - 1] == '\n')  r_recd [--x_len] = '\0';
      DEBUG_CONF   yLOG_info    ("r_recd"    , r_recd);
      break;
      /*---(done)------------------------*/
   }
   /*---(complete)-----------------------*/
   DEBUG_CONF   yLOG_exit    (__FUNCTION__);
   return 0;
}

char
DICT_import             (cchar a_name [LEN_PATH])
{
   /*---(locals)-----------+-----+-----+-*/
   char        rce         =  -10;
   char        rc          =    0;
   FILE       *f           = NULL;
   char        x_recd      [LEN_RECD]  = "";
   short       x_line      =    0;
   char        x_prefixes  [LEN_HUND]  = "";
   /*---(begin)----------s-----------------*/
   DEBUG_CONF   yLOG_enter   (__FUNCTION__);
   yURG_msg ('>', "source å%sæ", a_name);
   /*---(open dictionary)------------------*/
   rc = DICT__open (a_name, &f);
   --rce;  if (rc < 0) {
      yURG_err ('F', "data source could not be openned");
      DEBUG_CONF   yLOG_exitr   (__FUNCTION__, rce);
      return rce;
   }
   yURG_msg ('-', "data source openned successfully");
   /*---(process)--------------------------*/
   while (1) {
      /*---(next)----------------*/
      rc = DICT__read      (f, &x_line, x_prefixes, x_recd);
      if (rc < 0)  break;
      /*---(parse)---------------*/
      rc = DICT__parse_all (x_line, x_prefixes, x_recd);
      /*---(done)----------------*/
   }
   /*---(close dictionary)-----------------*/
   rc = DICT__close (&f);
   --rce;  if (rc < 0) {
      yURG_err ('F', "data source could not be closed");
      DEBUG_CONF   yLOG_exitr   (__FUNCTION__, rce);
      return rce;
   }
   yURG_msg ('-', "data source closed successfully");
   /*---(paginate)-------------------------*/
   DLIST_paginate ();
   yURG_msg ('-', "data paginated");
   /*---(done)-----------------------------*/
   yURG_msg ('·', "");
   /*---(complete)-------------------------*/
   DEBUG_CONF   yLOG_exit    (__FUNCTION__);
   return 0;
}

int
WORDS_find           (char *a_word)
{
   /*> /+---(locals)-----------+-----+-----+-+/                                           <* 
    *> char        rce         =  -10;                                                    <* 
    *> int         i           =    0;                                                    <* 
    *> /+---(defense)------------------------+/                                           <* 
    *> --rce;  if (a_word == NULL)  return rce;                                           <* 
    *> /+---(search)-------------------------+/                                           <* 
    *> for (i = 0; i < MAX_WORDS; ++i) {                                                  <* 
    *>    printf ("reviewing %d, %s, %s\n", i, s_words [i].english, s_words [i].gregg);   <* 
    *>    if (strncmp (s_words[i].english, "EOF",  MAX_LEN) == 0)  return -1;             <* 
    *>    if (strncmp (s_words[i].english, a_word, MAX_LEN) != 0)  continue;              <* 
    *>    printf ("FOUND     %d, %s, %s\n", i, s_words [i].english, s_words [i].gregg);   <* 
    *>    return  i;                                                                      <* 
    *> }                                                                                  <* 
    *> /+---(complete)-----------------------+/                                           <* 
    *> return 0;                                                                          <*/
}

char
DICT_list          (void)
{
   /*---(locals)-----------+-----+-----+-*/
   int         i           =    0;
   int         a           =    0;
   int         c           =    0;
   char        t           [LEN_FULL]  = "";
   char        n           [LEN_SHORT] = "";
   char        e           [LEN_TITLE] = "";
   char        g           [LEN_TITLE] = "";
   char        x_col       =    0;
   tWORD      *x_curr      = NULL;
   tWORD      *x_sub       = NULL;
   /*---(search)-------------------------*/
   WORDS_eng_by_index  (a, &x_curr);
   while (x_curr != NULL) {
      if (strcmp (x_curr->w_vary, "<") == 0) {
         WORDS_detail (x_curr, t);
         strlpadn (++c, n, '.', '<', 5);
         printf ("%5.5s··%s\n", n, t);
         x_sub = x_curr->w_next;
         x_col = 0;
         while (x_sub != NULL) {
            WORDS_detail (x_sub, t);
            printf ("    %c  %s\n", ++x_col + 'a', t);
            x_sub  = x_sub->w_next;
         }
      }
      ++a;
      WORDS_eng_by_index (a, &x_curr);
   }
   printf ("## found %d primaries and %d total words (%4.2fx)\n", c, a, (float) a / (float) c);
   /*---(complete)-----------------------*/
   return 0;
}

char
DICT_list_all      (void)
{
   /*---(locals)-----------+-----+-----+-*/
   int         i           =    0;
   int         a           =    0;
   int         c           =    0;
   char        t           [LEN_FULL]  = "";
   char        n           [LEN_SHORT] = "";
   char        e           [LEN_TITLE] = "";
   char        g           [LEN_TITLE] = "";
   tWORD       *x_curr      = NULL;
   /*---(search)-------------------------*/
   WORDS_eng_by_index  (a, &x_curr);
   while (x_curr != NULL) {
      WORDS_detail (x_curr, t);
      strlpadn (++a, n, '.', '<', 5);
      printf ("%5.5s··%s\n", n, t);
      WORDS_eng_by_index (a, &x_curr);
   }
   /*---(complete)-----------------------*/
   return 0;
}

/*>                                                                                                                                                                                           <* 
 *>    /+--name-- --suffix---- --also-- ---source-- ---true-endings----------------------------------------- ---examples------------------------------------------------------------ cnt +/   <* 
 *>    { "´"     , ">·´"      , ""     , '5',  29  , "-ing, -thing"                                         , "observing, caring, drinking, knowing, liking, using, forgetting"     , 0 },    <* 
 *>                                                                                                                                                                                           <*/

char
DICT_dump_suffix        (FILE *f)
{
   /*---(locals)-----------+-----+-----+-*/
   int         i           =    0;
   char        x_save      =    0;
   /*---(search)-------------------------*/
   for (i = 0; i < LEN_FULL; ++i) {
      if (g_varies [i].name [0] == '-')              break;
      if (g_varies [i].suffix [2] != x_save)  fprintf (f, "\n");
      fprintf (f, "%-5.5s %-10.10s %-5.5s  ", g_varies [i].name, g_varies [i].suffix, g_varies [i].base);
      fprintf (f, "%c %-3d %4d  ", g_varies [i].src, g_varies [i].page, g_varies [i].count);
      fprintf (f, "%-45.45s  %-70.70s", g_varies [i].endings, g_varies [i].example);
      fprintf (f, "\n");
      x_save = g_varies [i].suffix [2];
   }
   /*---(done)------------------------*/
   return 0;
}

char
DICT_dump_words         (FILE *f)
{
   /*---(locals)-----------+-----+-----+-*/
   int         i           =    0;
   int         a           =    0;
   int         c           =    0;
   char        t           [LEN_FULL]  = "";
   char        n           [LEN_SHORT] = "";
   char        e           [LEN_TITLE] = "";
   char        g           [LEN_TITLE] = "";
   char        x_col       =    0;
   tWORD      *x_curr      = NULL;
   tWORD      *x_sub       = NULL;
   /*---(header)---------------------------*/
   DEBUG_CONF   yLOG_enter   (__FUNCTION__);
   fprintf (f, "##   %s %s\n", P_NAMESAKE, P_HERITAGE);
   fprintf (f, "##   version %s, %s\n", P_VERNUM, P_VERTXT);
   fprintf (f, "##   inventory of dictionary words\n");
   fprintf (f, "\n");
   fprintf (f, "#@ style     V = printable columnar values\n");
   fprintf (f, "#@ x-parse  14åÏ---···Ï-----------------------··Ï---·Ï---·Ï·Ï··Ï·Ï·Ï--··Ï·Ï---··Ï-----------------------··Ï----------------------------------·æ\n");
   fprintf (f, "#@ titles     åref····english···················line·var··p·s··s·c·pg···g·freq··gregg·····················shown·······························æ\n");
   fprintf (f, "\n");
   WORDS_eng_by_index  (a, &x_curr);
   while (x_curr != NULL) {
      if (strcmp (x_curr->w_vary, "<") == 0) {
         DEBUG_CONF   yLOG_info    ("base"      , x_curr->w_english);
         WORDS_detail (x_curr, t);
         DEBUG_CONF   yLOG_info    ("t"         , t);
         strlpadn (++c, n, '.', '<', 5);
         if (i %  5 == 0)  fprintf (f, "\n");
         if (i % 25 == 0)  fprintf (f, "##-····---english--------------··line·var··p·s··s·c·pg···g·freq··---gregg----------------··---shown---------------------------\n\n");
         i++;
         fprintf (f, "%5.5s··%s\n", n, t);
         x_sub = x_curr->w_next;
         x_col = 0;
         while (x_sub != NULL) {
            DEBUG_CONF   yLOG_info    ("vary"      , x_sub->w_english);
            WORDS_detail (x_sub, t);
            DEBUG_CONF   yLOG_info    ("t"         , t);
            if (i %  5 == 0)  fprintf (f, "\n");
            if (i % 25 == 0)  fprintf (f, "##-····---english--------------··line·var··p·s··s·c·pg···g·freq··---gregg----------------··---shown---------------------------\n\n");
            i++;
            fprintf (f, "    %c  %s\n", ++x_col + 'a', t);
            x_sub  = x_sub->w_next;
         }
      }
      ++a;
      WORDS_eng_by_index (a, &x_curr);
   }
   fprintf (f, "\n");
   fprintf (f, "##-····---english--------------··line·var··p·s··s·c·pg···g·freq··---gregg----------------··---shown---------------------------\n\n");
   fprintf (f, "## found %d primaries and %d total words (%4.2fx)\n", c, a, (float) a / (float) c);
   /*---(complete)-------------------------*/
   DEBUG_CONF   yLOG_exit    (__FUNCTION__);
   return 0;
}

char
DICT_dump_gregg         (FILE *f)
{
   /*---(locals)-----------+-----+-----+-*/
   int         x_pos       =    0;
   int         x_gregg     =    0;
   int         i           =    0;
   int         a           =    0;
   int         c           =    0;
   char        r           [LEN_SHORT] = "";
   char        s           [LEN_SHORT] = "";
   char        t           [LEN_SHORT] = "";
   char        u           [LEN_SHORT] = "";
   char        v           [LEN_SHORT] = "";
   char        x_col       =    0;
   tWORD      *x_curr      = NULL;
   /*---(header)-------------------------*/
   DEBUG_CONF   yLOG_enter   (__FUNCTION__);
   WORDS_eng_by_index  (a, &x_curr);
   while (x_curr != NULL) {
      if (strcmp (x_curr->w_vary, "<") != 0) {
         WORDS_eng_by_index (++a, &x_curr);
         continue;
      }
      if (x_curr->w_tree [0] > 0)  sprintf (r, "%02d" , x_curr->w_tree [0]);
      else                         strcpy  (r, "  ");
      if (x_curr->w_tree [1] > 0)  sprintf (s, "·%02d", x_curr->w_tree [1]);
      else                         strcpy  (s, "   ");
      if (x_curr->w_tree [2] > 0)  sprintf (t, "·%02d", x_curr->w_tree [2]);
      else                         strcpy  (t, "   ");
      if (x_curr->w_tree [3] > 0)  sprintf (u, "·%02d", x_curr->w_tree [3]);
      else                         strcpy  (u, "   ");
      if (x_curr->w_tree [4] > 0)  sprintf (v, "·%02d", x_curr->w_tree [4]);
      else                         strcpy  (v, "   ");
      fprintf (f, "%2s%2s%2s%2s%2s  %-20.20s  %s\n", r, s, t, u, v, x_curr->w_gregg, x_curr->w_english);
      WORDS_eng_by_index (++a, &x_curr);
   }
   /*---(complete)-----------------------*/
   DEBUG_CONF   yLOG_exit    (__FUNCTION__);
   return 0;
}


