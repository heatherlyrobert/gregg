/*============================---(source-start)---============================*/
#include "gregg.h"

/*
 *
 * metis § dc2·· § allow for blank/empty varitions in source files                        § N2O468 §  · §
 * metis § sn2<· § handle newer categories field format                                   § N2O46u §  · §
 * metis § tn4<· § verbose dictionary parsing mode to find mistakes                       § N2O4rp §  · §
 * metis § tn2<· § use table of suffixes to check variation request                       § N2P1kq §  · §
 * metis § dc4·· § add verification of arpabet entry                                      § N2U1OE §  · §
 * metis § dc4·· § add verification of gregg letters entry                                § N2U1OS §  · §
 */


static char    s_name   [LEN_PATH]  = "/var/lib/gregg/gregg.dict";

char   s_fields  [MAX_FIELD][LEN_TITLE];
char   s_nfield  = 0;


tPARTS  g_parts  [LEN_HUND] = {
   /*---(unassigned)------------------*/
   { '¢', '¢', "bad primary"          , 0 , 0},
   { '·', '·', "tbd"                  , 0 , 0},
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
   /*--name-- --suffix---- --also-- ---source-- ---true-endings----------------------------------------- ---examples------------------------------------------------------------ cnt */
   { "·"     , ""         , ""     , '·',  -1  , ""                                                     , "observe, care, drink, know, like, use, forget"                       , 0 },
   /*--name-- --suffix---- --also-- ---source-- ---true-endings----------------------------------------- ---examples------------------------------------------------------------ cnt */
   { "´"     , ">·´"      , ""     , '5',  29  , "-ing, -thing"                                         , "observing, caring, drinking, knowing, liking, using, forgetting"     , 0 },
   { "Ï"     , ">·z"      , ""     , '5',  29  , "-ings"                                                , "sayings, readings, writings"                                         , 0 },
   {   "ze"  , ">·z·e"    , ""     , '5', 117  , "-ingly"                                               , "sayings, readings, writings"                                         , 0 },
   /*--name-- --suffix---- --also-- ---source-- ---true-endings----------------------------------------- ---examples------------------------------------------------------------ cnt */
   { "t"     , ">·t"      , "ts"   , '·',  -1  , "-ate, -ite, -iate"                                    , "certificate, demonstrate, appointment, initiate"                     , 0 },
   {   "tsh" , ">·t·sh"   , ""     , '5',  82  ,  "-tition, -tation, -tiation"                          , "repitition, station, initiation"                                     , 0 },
   {   "ts"  , ">·t·s"    , ""     , '5', 118  ,  "-itis"                                               , "appendicitis"                                                        , 0 },
   {   "tn"  , ">·t·n"    , ""     , '5', 111  ,  "-ington"                                             , "washington"                                                          , 0 },
   {   "tt"  , ">·t"      , ""     , '5',  43  ,  "-ted, -ded"                                          , "invited, divided, demanded, printed"                                 , 0 },
   /*--name-- --suffix---- --also-- ---source-- ---true-endings----------------------------------------- ---examples------------------------------------------------------------ cnt */
   { "d"     , ">·d"      , ""     , '·',  -1  ,  "-d, -ed, -ied"                                       , "observed, cared, drank, knew, liked, used, forgot"                   , 0 },
   {   "dsh" , ">·d·sh"   , ""     , '5',  82  ,  "-dation"                                             , "addition, gradation"                                                 , 0 },
   {   "dd"  , ">·d"      , ""     , '5', 118  ,  "-hood, -ward"                                        , "childhood, likelihood, homeward, downward, forward, toward"          , 0 },
   /*--name-- --suffix---- --also-- ---source-- ---true-endings----------------------------------------- ---examples------------------------------------------------------------ cnt */
   { "th"    , ">·th"     , ""     , '5', 111  , "-worthy"                                              , "praiseworthy, trustworthy"                                           , 0 },
   /*--name-- --suffix---- --also-- ---source-- ---true-endings----------------------------------------- ---examples------------------------------------------------------------ cnt */
   { "s"     , ">·s"      , ""     , '·',  -1  , "-s, -es, -ies"                                        , "observes, cares, drinks, knows, likes, uses, forgets"                , 0 },
   {   "st"  , ">·s·t"    , ""     , '·',  -1  , "-est, -iest"                                          , "longest, strongest, greatest"                                        , 0 },
   {   "sf"  , ">·s·f"    , ""     , '5', 111  , "-self"                                                , "himself, yourself"                                                   , 0 },
   {   "se"  , ">·s·e"    , ""     , '5', 123  , "-city, -acity, -ecity"                                , "tenacity, felicity, pomposity"                                       , 0 },
   {   "su"  , ">·s·u"    , ""     , '5', 111  , "-sult"                                                , "result, insult, consult"                                             , 0 },
   {   "sm"  , ">·s·m"    , ""     , '5', 111  , "-sume"                                                , "assume, presume, resume"                                             , 0 },
   /*--name-- --suffix---- --also-- ---source-- ---true-endings----------------------------------------- ---examples------------------------------------------------------------ cnt */
   { "f"     , ">·f"      , ""     , '5', 109  , "-ful, -fil"                                           , "careful, beautiful, useful, thoughtful"                              , 0 },
   {   "fe"  , ">·f·e"    , ""     , '5', 109  , "-fully, -filly"                                       , "carefully, beautifully, usefully, thoughtfully"                      , 0 },
   {   "fn"  , ">·f·n"    , ""     , '´', -1   , "-fulness"                                             , "hopefulness, carefulness, thoughfulness"                             , 0 },
   {   "fsh" , ">·f·sh"   , ""     , '5', 109  , "-ification"                                           , "specification, modification"                                         , 0 },
   {   "fl"  , ">·f·l"    , ""     , '5', 109  , "-flect, -flict"                                       , "afflict, reflect"                                                    , 0 },
   {   "flsh", ">·f·l·sh" , ""     , '5', 109  , "-flection, -fliction"                                 , "affliction, reflection"                                              , 0 },
   /*--name-- --suffix---- --also-- ---source-- ---true-endings----------------------------------------- ---examples------------------------------------------------------------ cnt */
   { "v"     , ">·v"      , ""     , '·',  -1  , "-ive, -ative, -itive, -ant, -ent, -atic"              , "observant, systematic"                                               , 0 },
   {   "ve"  , ">·v·e"    , ""     , '5', 123  , "-vity, -avity, -evity"                                , "depravity, nativity, brevity"                                        , 0 },
   /*--name-- --suffix---- --also-- ---source-- ---true-endings----------------------------------------- ---examples------------------------------------------------------------ cnt */
   { "p"     , ">·p"      , ""     , '5', 110  , "-ple"                                                 , "sample, ample, apple, couple, pimple, purple, example, people"       , 0 },
   {   "po"  , ">·p·o"    , ""     , '5', 110  , "-pose"                                                , "suppose, purpose, impose, compose, dispose, expost, interpose"       , 0 },
   {   "posh", ">·p·o·sh" , ""     , '5', 110  , "-position"                                            , "supposition, comosition, disposition, exposition"                    , 0 },
   {   "pu"  , ">·p·u"    , ""     , '5', 110  , "-pute"                                                , "impute, compute, dispute, repute"                                    , 0 },
   {   "push", ">·p·u·sh" , ""     , '5', 110  , "-putation"                                            , "imputation, computation"                                             , 0 },
   {   "pi"  , ">·p·i"    , ""     , '5', 110  , "-pire"                                                , "aspire, inspire, conspire, respire"                                  , 0 },
   /*--name-- --suffix---- --also-- ---source-- ---true-endings----------------------------------------- ---examples------------------------------------------------------------ cnt */
   { "b"     , ">·b"      , ""     , '5', 109  , "-ble, -able, -ible, -iable"                           , "observeable, available, knowable, likeable, variable, useable"       , 0 },
   {   "be"  , ">·b·e"    , ""     , '5', 117  , "-bility, -ability, -ibility, -iabilty"                , "ability, feasibility, responsibilty, accessability"                  , 0 },
   /*--name-- --suffix---- --also-- ---source-- ---true-endings----------------------------------------- ---examples------------------------------------------------------------ cnt */
   { "r"     , ">·r"      , "rs"   , '·',  -1  , "-r, -ar, -er, -eer, -or, -ant, -ent"                  , "observer, carer, drinker, knower, user"                              , 0 },
   {   "rs"  , ">·r·z"    , ""     , '·',  -1  , "-rs, -ars, -ers, -eers, -ors, -ants, -ents"           , "observers, carers, drinkers, knowers, users"                         , 0 },
   {   "re"  , ">·r·e"    , ""     , '5', 123  , "-rity, -arity, -erity"                                , "popularity, prosperity, majority"                                    , 0 },
   /*--name-- --suffix---- --also-- ---source-- ---true-endings----------------------------------------- ---examples------------------------------------------------------------ cnt */
   { "l"     , ">·l"      , ""     , '5', 109  , "-less"                                                , "careless, ageless, useless"                                          , 0 },
   {   "le"  , ">·l·e"    , ""     , '5', 123  , "-lity, -ality, -elity"                                , "brutality, utility, frivolity"                                       , 0 },
   /*--name-- --suffix---- --also-- ---source-- ---true-endings----------------------------------------- ---examples------------------------------------------------------------ cnt */
   { "k"     , ">·k"      , ""     , '5', 118  , "-acle, -ical, -icle"                                  , "tentacle, medical, classical, chronicle"                             , 0 },
   {   "ek"  , ">·e·k"    , ""     , '·', 123  , "-ic, -tic, -ac, -ick, -fic, -ific, -temic"            , "systemic, classic, politic, hypnotic, systematic"                    , 0 },
   {   "kr"  , ">·k·r"    , ""     , '5', 109  , "-cribe"                                               , "describe, prescribe, transcribe, subscribe, inscribe"                , 0 },
   {   "krsh", ">·k·r·sh" , ""     , '5', 109  , "-cription"                                            , "description, prescription, subscription, inscription"                , 0 },
   {   "ki"  , ">·k·i"    , ""     , '5', 110  , "-quire"                                               , "acquire, inquire, require, esquire"                                  , 0 },
   {   "kes" , ">·k·e·s"  , ""     , '5', 110  , "-quest"                                               , "request, conquest, bequest, inquest"                                 , 0 },
   {   "kest", ">·k·e·s·t", ""     , '5', 110  , "-quisite"                                             , "requisite, exquisite"                                                , 0 },
   /*--name-- --suffix---- --also-- ---source-- ---true-endings----------------------------------------- ---examples------------------------------------------------------------ cnt */
   { "g"     , ">·g"      , ""     , '5', 117  , "-gram, -grim"                                         , "monogram, program, telegram"                                         , 0 },
   /*--name-- --suffix---- --also-- ---source-- ---true-endings----------------------------------------- ---examples------------------------------------------------------------ cnt */
   { "n"     , ">·n"      , ""     , '·',  -1  , "-ence, -ance, -ancy, -ency, -cy, -acy, -age, -ness"   , "observance"                                                          , 0 },
   {   "n"   , ">·n"      , ""     , '5', 109  , "-ness"                                                , "lateness, fitness, awareness, witness"                               , 0 },
   {   "ne"  , ">·n·e"    , ""     , '5', 123  , "-nity, -anity, -enity"                                , "humanity, community, affinity, profanity, opportunity, unity"        , 0 },
   {   "nk"  , ">·n·k"    , ""     , '5', 123  , "-ntic, -antic, -intic"                                , "gigangic, authentic, frantic"                                        , 0 },
   {   "nke" , ">·n·k·e"  , ""     , '5', 123  , "-nticly, -anticly, -inticly"                          , "franticly"                                                           , 0 },
   {   "nsh" , ">·n·sh"   , ""     , '5',  82  , "-nition, -nation"                                     , "ignition, stagnation, ammunition, cognition"                         , 0 },
   /*--name-- --suffix---- --also-- ---source-- ---true-endings----------------------------------------- ---examples------------------------------------------------------------ cnt */
   { "m"     , ">·m"      , ""     , '5', 109  , "-ment"                                                , "amusement, department"                                               , 0 },
   {   "mt"  , ">·m·l"    , ""     , '5', 117  , "-mental, -imental, -amental, -mentality"              , "experimental, fundamental"                                           , 0 },
   {   "msh" , ">·m·sh"   , ""     , '5',  82  , "-mission, -mation"                                    , "ommission, formation"                                                , 0 },
   {   "me"  , ">·m·e"    , ""     , '5', 123  , "-mity, -amity, -emity"                                , "calamity, sublimity, proximity"                                      , 0 },
   {   "mm"  , ">·m"      , ""     , '5', 117  , "-ingham"                                              , "birmingham, dillingham"                                              , 0 },
   /*--name-- --suffix---- --also-- ---source-- ---true-endings----------------------------------------- ---examples------------------------------------------------------------ cnt */
   { "sh"    , ">·sh"     , "shs"  , '5',  30  , "-tion, -sion"                                         , "observation, nation, session, action, motion, fashion"               , 0 },
   {   "shu" , ">·sh·u"   , ""     , '5', 111  , "-sure"                                                , "assure, measure, pressure"                                           , 0 },
   {   "shus", ">·sh·u·s" , ""     , '5', 111  , "-cious"                                               , "audacious, vicious"                                                  , 0 },
   {   "sht" , ">·sh·t"   , ""     , '5', 111  , "-tient, -cient"                                       , "patient, ancient, efficient, sufficient"                             , 0 },
   {   "shte", ">·sh·t·e" , ""     , '5', 111  , "-tiently, -ciently"                                   , "efficiently, sufficiently"                                           , 0 },
   {   "sh"  , ">·sh"     , ""     , '5', 117  , "-ship"                                                , "partnership, ownership, hardship, friendship"                        , 0 },
   /*--name-- --suffix---- --also-- ---source-- ---true-endings----------------------------------------- ---examples------------------------------------------------------------ cnt */
   { "j"     , ">·j"      , ""     , '5',  30  , "-jure"                                                , "injure, purjure"                                                     , 0 },
   {    "je" , ">·j·e"    , ""     , '·',  -1  , "-jury"                                                , "injury, purjury"                                                     , 0 },
   {    "js" , ">·j·s"    , ""     , '·',  -1  , "-juries"                                              , "injuries"                                                            , 0 },
   /*--name-- --suffix---- --also-- ---source-- ---true-endings----------------------------------------- ---examples------------------------------------------------------------ cnt */
   { "a"     , ">·a"      , ""     , '5', 123  , "-tic, -atic, -etic, -itic, -otic"                     , "politic, hymnotic, systematic"                                       , 0 },
   {    "as" , ">·a·s"    , ""     , '5', 123  , "-tics, -atics, -etics, -itics, -otics"                , "politics, hymnotics, systematics"                                    , 0 },
   {    "ae" , ">·a·e"    , ""     , '5', 123  , "-ticly, -aticly, -eticly, -iticly, -oticly"           , "politicly, hymnoticly, systematicly"                                 , 0 },
   { "e"     , ">·e"      , ""     , '5',  30  , "-ly, -ily, -ally"                                     , "only, calmly, totally, early, readity"                               , 0 },
   {   "esh" , ">·e·sh"   , ""     , '5',  30  , "-ish"                                                 , "only, calmly, totally, early, readity"                               , 0 },
   {   "ez"  , ">·e·z"    , ""     , '5', 117  , "-ize, -ise, -itize"                                   , "prioritize, rationalize, sympathize"                                 , 0 },
   {   "er"  , ">·r"      , ""     , '·',  -1  , "-er, -ier"                                            , "longer, stronger, greater"                                           , 0 },
   { "o"     , ">·o"      , ""     , '5', 123  , "-alogy, -elogy, -ilogy"                               , "geology, analogy, psychology"                                        , 0 },
   {   "ol"  , ">·o·l"    , ""     , '5', 123  , "-alogical, -elogical, -ilogical"                      , "geological, psychological"                                           , 0 },
   {   "or"  , ">·o·r"    , ""     , '·',  -1  , "-ry, -ory, -ery, -ary, -ium, -itory, -atory"          , "observatory, aviary"                                                 , 0 },
   { "u"     , ">·u"      , ""     , '·',  -1  , "-ul, -al, -il, -ile, -ial"                            , "additonal, observational"                                            , 0 },
   {    "ut" , ">·u·t"    , ""     , '5', 118  , "-ulate"                                               , "modulate, insulate, emulate, speculate"                              , 0 },
   {    "ud" , ">·u·d"    , ""     , '5', 118  , "-ulated"                                              , "modulated, insulated, emulated, speculated"                          , 0 },
   {    "uv" , ">·u·v"    , ""     , '5', 118  , "-ulative"                                             , "speculative"                                                         , 0 },
   {    "ush", ">·u·sh"   , ""     , '5', 118  , "-ulation"                                             , "modulation, insulation, emulation, speculation"                      , 0 },
   {    "ur" , ">·u·r"    , ""     , '5', 118  , "-ulator"                                              , "modulator, insulator, emulator, speculator"                          , 0 },
   /*--name-- --suffix---- --also-- ---source-- ---true-endings----------------------------------------- ---examples------------------------------------------------------------ cnt */
   { "y"     , "·e"       , ""     , '5', 123  , "-y, -ish"                                             , "hasty, tasty, nasty, rougish"                                        , 0 },
   /*--name-- --suffix---- --also-- ---source-- ---true-endings----------------------------------------- ---examples------------------------------------------------------------ cnt */
   { "---"   , "---"      , "---"  , '·',  -1  , "---"                                                  , "---"                                                                 , 0 },
   /*--name-- --suffix---- --also-- ---source-- ---true-endings----------------------------------------- ---examples------------------------------------------------------------ cnt */
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
DICT__find_variation    (char a_name [LEN_SHORT], char r_suffix [LEN_TERSE], char r_also [LEN_SHORT], char r_alsos [LEN_TERSE])
{
   /*---(locals)-----------+-----+-----+-*/
   char        rce         =  -10; 
   int         i           =    0;
   char        x_alsos     [LEN_TERSE] = "";
   /*---(default)------------------------*/
   if (r_suffix != NULL)  strcpy (r_suffix, "");
   if (r_also   != NULL)  strcpy (r_also  , "");
   if (r_alsos  != NULL)  strcpy (r_alsos , "");
   /*---(defense)------------------------*/
   --rce;  if (a_name == NULL)  return rce;
   /*---(search)-------------------------*/
   for (i = 0; i < LEN_FULL; ++i) {
      if (g_varies [i].name [0] == '-')              break;
      if (strcmp (g_varies [i].name, a_name) != 0)   continue;
      /*---(suffix)----------------------*/
      if (r_suffix != NULL)  strlcpy (r_suffix, g_varies [i].suffix, LEN_TERSE);
      ++(g_varies [i].count);
      /*---(also)------------------------*/
      if (g_varies [i].also [0] != '\0') {
         DICT__find_variation (g_varies [i].also, x_alsos, NULL, NULL);
         if (r_also   != NULL)  strlcpy (r_also , g_varies [i].also , LEN_SHORT);
         if (r_alsos  != NULL)  strlcpy (r_alsos, x_alsos, LEN_TERSE);
      }
      /*---(done)------------------------*/
      return 0;
   }
   /*---(default)------------------------*/
   --rce;  return rce;
}

/*============================--------------------============================*/
/*===----                          dictionary                          ----===*/
/*============================--------------------============================*/
static void      o___DICTIONARY______________o (void) {;}

char
DICT_init          (void)
{
   /*---(locals)-------------------------*/
   char        i           =    0;
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
      strltrim (s_fields [s_nfield], ySTR_BOTH, LEN_TITLE);
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
   rc = WORDS__new (a_english, x_gregg, x_part, &x_new);
   DEBUG_CONF   yLOG_value   ("new"       , rc);
   --rce;  if (rc < 0) {
      yURG_err ('w', "%d primary word å%sæ could not be created", a_line, a_english);
      DEBUG_CONF   yLOG_exitr   (__FUNCTION__, rce);
      return rce;
   }
   /*---(save-back)----------------------*/
   x_new->line = a_line;
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
   if (l >= 1)  a_new->part = DICT__find_speech (a_cats [0]);
   else         a_new->part = DICT__find_speech ('¢');
   /*---(sub part-of-speech)-------------*/
   if (l >= 3)  a_new->sub  = DICT__find_sub    (a_new->part, a_cats [2]);
   else         a_new->sub  = DICT__find_sub    (a_new->part, '¢');
   /*---(source group)-------------------*/
   if (l >= 5)  a_new->grp  = DICT__find_grp    (a_cats [4]);
   else         a_new->grp  = DICT__find_grp    ('¢');
   /*---(source)-------------------------*/
   if (l >= 7)  a_new->src  = DICT__find_source (a_cats [6]);
   else         a_new->src  = DICT__find_source ('¢');
   /*---(category)-----------------------*/
   if (l >= 9)  a_new->cat  = DICT__find_type   (a_cats [8]);
   else         a_new->cat  = DICT__find_type   ('¢');
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
   if (l >= 1)  a_new->part = DICT__find_speech (a_cats [0]);
   else         a_new->part = DICT__find_speech ('¢');
   /*---(sub part-of-speech)-------------*/
   if (l >= 3)  a_new->sub  = DICT__find_sub    (a_new->part, a_cats [2]);
   else         a_new->sub  = DICT__find_sub    (a_new->part, '¢');
   /*---(source group)-------------------*/
   if (l >= 5)  a_new->grp  = DICT__find_grp    (a_cats [4]);
   else         a_new->grp  = DICT__find_grp    ('¢');
   /*---(source)-------------------------*/
   if (l >= 8)  a_new->src  = DICT__find_source (a_cats [7]);
   else         a_new->src  = DICT__find_source ('¢');
   /*---(category)-----------------------*/
   if (l >= 10) a_new->cat  = DICT__find_type   (a_cats [9]);
   else         a_new->cat  = DICT__find_type   ('¢');
   /*---(page)---------------------------*/
   if (l >= 12) a_new->page = atoi (a_cats + 11);
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
   if (l >= 1)  a_new->part = DICT__find_speech (a_cats [0]);
   else         a_new->part = DICT__find_speech ('¢');
   if (l >= 3)  a_new->sub  = DICT__find_sub    (a_new->part, a_cats [2]);
   else         a_new->sub  = DICT__find_sub    (a_new->part, '¢');
   /*---(source)-------------------------*/
   if (l >= 6)  a_new->src  = DICT__find_source (a_cats [5]);
   else         a_new->src  = DICT__find_source ('¢');
   if (l >= 8)  a_new->cat  = DICT__find_type   (a_cats [7]);
   else         a_new->cat  = DICT__find_type   ('¢');
   if (l >= 10) {
      sprintf (t, "%-3.3s", a_cats + 9);
      a_new->page = atoi (t);
   }
   /*---(frequency)----------------------*/
   if (l >= 14) a_new->grp  = DICT__find_grp    (a_cats [13]);
   else         a_new->grp  = DICT__find_grp    ('¢');
   if (l >= 16) a_new->freq = atoi (a_cats + 15);
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
DICT__variation_quick   (tWORD *a_base, tWORD *a_last, char a_english [LEN_TITLE], char a_vary [LEN_SHORT], char a_suffix [LEN_TERSE], tWORD **r_new)
{
   /*---(locals)-----------+-----+-----+-*/
   char        rce         =  -10;
   char        rc          =    0;
   char        x_gregg     [LEN_TITLE] = "";
   tWORD      *x_new       = NULL;
   /*---(header)-------------------------*/
   DEBUG_CONF   yLOG_enter   (__FUNCTION__);
   /*---(build gregg)--------------------*/
   sprintf (x_gregg, "%s·%s", a_base->gregg, a_suffix);
   DEBUG_CONF   yLOG_info    ("x_gregg"   , x_gregg);
   /*---(add word)-----------------------*/
   rc = WORDS__new (a_english, x_gregg, a_base->part, &x_new);
   DEBUG_CONF   yLOG_value   ("new"       , rc);
   --rce;  if (rc < 0) {
      DEBUG_CONF   yLOG_exitr   (__FUNCTION__, rce);
      return rce;
   }
   /*---(carry-over)---------------------*/
   x_new->line = a_base->line;
   strlcpy (x_new->vary, a_vary, LEN_SHORT);
   /*---(past-of-speech)-----------------*/
   x_new->part = a_base->part;
   x_new->sub  = a_base->sub;
   /*---(source)-------------------------*/
   x_new->src  = a_base->src;
   x_new->cat  = a_base->cat;
   x_new->page = a_base->page;
   /*---(frequency)----------------------*/
   x_new->grp  = a_base->grp;
   x_new->freq = a_base->freq;
   /*---(connect to chain)---------------*/
   x_new->base = a_base;
   if (a_last != NULL)  a_last->next = x_new;
   /*---(save-back)----------------------*/
   if (r_new  != NULL)  *r_new = x_new;
   /*---(complete)-----------------------*/
   DEBUG_CONF   yLOG_exit    (__FUNCTION__);
   return 0;
}

char
DICT__variation         (tWORD *a_base, tWORD *a_last, cchar *a_vary, tWORD **r_new)
{
   /*---(locals)-----------+-----+-----+-*/
   char        rce         =  -10;
   char        rc          =    0;
   char       *p           = NULL;
   char        l           =    0;
   char        x_left      =    0;
   char        x_type      [LEN_TITLE]  = "";
   char        x_english   [LEN_TITLE]  = "";
   char        x_suffix    [LEN_TERSE] = "";
   char        x_also      [LEN_SHORT] = "";
   char        x_alsos     [LEN_TERSE] = "";
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
   --rce;  if (l < 1 || l > 2) {
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
   sprintf (x_english, "%s"   , p + 2);
   /*---(get type)-----------------------*/
   sprintf (x_check, " %s ", x_type);
   --rce;  if (strstr (VARIATIONS, x_check) == NULL) {
      DEBUG_CONF   yLOG_exitr   (__FUNCTION__, rce);
      return rce;
   }
   rc = DICT__find_variation (x_type, x_suffix, x_also, x_alsos);
   /*---(make variation)-----------------*/
   rc = DICT__variation_quick (a_base, a_last, x_english, x_type, x_suffix, &x_vary);
   x_save = x_vary;
   /*---(build second)-------------------*/
   if (strcmp (x_also, "") != 0) {
      strlcat (x_english, "s", LEN_TITLE);
      rc = DICT__variation_quick (a_base, x_save, x_english, x_also, x_alsos, &x_vary);
   }
   /*---(save-back)----------------------*/
   if (r_new  != NULL)  *r_new = x_vary;
   /*---(complete)-----------------------*/
   DEBUG_CONF   yLOG_exit    (__FUNCTION__);
   return 0;
}

char
DICT__parse             (short a_line, uchar *a_recd)
{
   /*---(locals)-----------+-----+-----+-*/
   char        rce         =  -10;
   char        rc          =    0;
   int         i           =    0;
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
   /*---(primary)------------------------*/
   rc = DICT__primary (a_line, s_fields [0], s_fields [2], s_fields [3], &x_new);
   DEBUG_CONF   yLOG_value   ("primary"   , rc);
   --rce;  if (rc < 0) {
      DEBUG_CONF   yLOG_exitr   (__FUNCTION__, rce);
      return rce;
   }
   /*---(arpabet)------------------------*/
   if (s_nfield > 3) {
      x_new->arpabet   = strdup (s_fields [1]);
   }
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
   for (i = 4; i < s_nfield; ++i) {
      if (strlen (s_fields [i]) == 0)  continue;
      rc = DICT__variation (x_new, x_last, s_fields [i], &x_vary);
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
DICT__read              (FILE *a_file, short *r_line, char r_recd [LEN_RECD])
{
   /*---(locals)-----------+-----+-----+-*/
   char        rce         =  -10;
   char        rc          =    0;
   int         x_len       =    0;
   char       *p           = NULL;
   /*---(begin)----------------------------*/
   DEBUG_CONF   yLOG_enter   (__FUNCTION__);
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
   strcpy (r_recd, "(eof)");
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
      /*---(tear off extras)-------------*/
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
DICT_import             (char a_name [LEN_PATH])
{
   /*---(locals)-----------+-----+-----+-*/
   char        rce         =  -10;
   char        rc          =    0;
   FILE       *f           = NULL;
   char        x_recd      [LEN_RECD]  = "";
   short       x_line      =    0;
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
      rc = DICT__read  (f, &x_line, x_recd);
      if (rc < 0)  break;
      /*---(parse)---------------*/
      rc = DICT__parse (x_line, x_recd);
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
   char        t           [LEN_HUND]  = "";
   char        n           [LEN_SHORT] = "";
   char        e           [LEN_TITLE] = "";
   char        g           [LEN_TITLE] = "";
   char        x_col       =    0;
   tWORD      *x_curr      = NULL;
   tWORD      *x_sub       = NULL;
   /*---(search)-------------------------*/
   WORDS_eng_by_index  (a, &x_curr);
   while (x_curr != NULL) {
      if (strcmp (x_curr->vary, "<") == 0) {
         WORDS_detail (x_curr, t);
         strlpadn (++c, n, '.', '<', 5);
         printf ("%5.5s··%s\n", n, t);
         x_sub = x_curr->next;
         x_col = 0;
         while (x_sub != NULL) {
            WORDS_detail (x_sub, t);
            printf ("    %c  %s\n", ++x_col + 'a', t);
            x_sub  = x_sub->next;
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
   char        t           [LEN_HUND]  = "";
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
