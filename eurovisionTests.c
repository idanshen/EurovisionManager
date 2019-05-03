#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <stdbool.h>
#include <assert.h>
#include "list.h"
#include "eurovision.h"
#include "eurovisionTests.h"

#define POINTS_OPTIONS_NUMBER 10

#define CHECK(b,res)                            \
  if((b) != (res)) do{                          \
      printf("fail: %s != %s\n", #b, #res);     \
      eurovisionDestroy(eurovision);            \
      return false;                             \
    } while(0)

#define CHECK_WITH_FREE(b,res,f)                \
  if((b) != (res)) do{                          \
      printf("fail: %s != %s\n", #b, #res);     \
      eurovisionDestroy(eurovision);            \
      free(f);                                  \
      return false;                             \
    } while(0)

static Eurovision setupEurovision() {
  Eurovision eurovision = eurovisionCreate();
  assert(eurovision);
  return eurovision;
}

static void setupEurovisionStates(Eurovision eurovision) {
  eurovisionAddState(eurovision, 0, "israel", "home");
  eurovisionAddState(eurovision, 1, "malta", "chameleon");
  eurovisionAddState(eurovision, 2, "croatia", "the dream");
  eurovisionAddState(eurovision, 3, "russia", "scream");
  eurovisionAddState(eurovision, 4, "moldova", "stay");
  eurovisionAddState(eurovision, 5, "cyprus", "replay");
  eurovisionAddState(eurovision, 6, "spain", "la venda");
  eurovisionAddState(eurovision, 7, "italy", "soldi");
  eurovisionAddState(eurovision, 8, "france", "roi");
  eurovisionAddState(eurovision, 9, "germany", "sister");
  eurovisionAddState(eurovision, 10, "united kingdom", "bigger than us");
  eurovisionAddState(eurovision, 11, "armenia", "walking out");
  eurovisionAddState(eurovision, 12, "austria", "limits");
  eurovisionAddState(eurovision, 13, "ireland", "twenty two");
  eurovisionAddState(eurovision, 14, "netherlands", "arcade");
  eurovisionAddState(eurovision, 15, "sweden", "too late for love");
}

static int *makeJudgeResults(int id0, int id1, int id2, int id3, int id4,
                             int id5, int id6, int id7, int id8, int id9) {
  int *results = malloc(POINTS_OPTIONS_NUMBER * sizeof(*results));
  assert(results);
  results[0] = id0;
  results[1] = id1;
  results[2] = id2;
  results[3] = id3;
  results[4] = id4;
  results[5] = id5;
  results[6] = id6;
  results[7] = id7;
  results[8] = id8;
  results[9] = id9;
  return results;
}

/** total judge result: (stateid: score)
 * 0: (12 + 2  + 0) / 3  = 14 / 3 = 4.6667
 * 1: (10 + 0  + 0) / 3  = 10 / 3 = 3.3333
 * 2: (8  + 1  + 0) / 3  =  9 / 3 = 3
 * 3: (7  + 0  + 7) / 3  = 14 / 3 = 4.6667
 * 4: (6  + 6  + 6) / 3  = 18 / 3 = 6
 * 5: (5  + 5  + 5) / 3  = 15 / 3 = 5
 * 6: (4  + 4  + 4) / 3  = 12 / 3 = 4
 * 7: (3  + 0  + 3) / 3  =  6 / 3 = 2
 * 8: (2  + 7  + 2) / 3  = 11 / 3 = 3.6667
 * 9: (1  + 0  + 1) / 3  =  2 / 3 = 0.6667
 * 10:(0  + 8  + 12) / 3 = 20 / 3 = 6.6667
 * 11:(0  + 0  + 10) / 3 = 10 / 3 = 3.3333
 * 12:(0  + 0  + 8) / 3  =  8 / 3 = 2.6667
 * 13:(0  + 10 + 0) / 3  = 10 / 3 = 3.3333
 * 14:(0  + 12 + 0) / 3  = 12 / 3 = 4
 * 15:(0  + 3  + 0) / 3  =  3 / 3 = 1
 */
static void setupEurovisionJudges(Eurovision eurovision) {
  int *results;
  results = makeJudgeResults( 0,  1,  2, 3, 4, 5, 6,  7, 8, 9);
  eurovisionAddJudge(eurovision, 0, "olsen", results);
  free(results);
  results = makeJudgeResults(14, 13, 10, 8, 4, 5, 6, 15, 0, 2);
  eurovisionAddJudge(eurovision, 1, "tanel", results);
  free(results);
  results = makeJudgeResults(10, 11, 12, 3, 4, 5, 6,  7, 8, 9);
  eurovisionAddJudge(eurovision, 2, "marie", results);
  free(results);
}

static void giveVotes(Eurovision eurovision, int giver, int taker, int votes) {
  for (int i = 0; i < votes; i++) {
    if (eurovisionAddVote(eurovision, giver, taker) != EUROVISION_SUCCESS) {
      printf("error in eurovisionAddVote %d -> %d\n", giver, taker);
    }
  }
}

static void setupEurovisionVotes(Eurovision eurovision) {
  giveVotes(eurovision, 1, 12, 4);
  giveVotes(eurovision, 2, 8, 2);
  giveVotes(eurovision, 3, 4, 1);
}

/** total voters result:
 * 1:  (0  + 12 + 0  + 0) / 16  = 12 / 16 = 0.75
 * 2:  (12 + 0  + 8  + 0) / 16  = 20 / 16 = 1.25
 * 3:  (10 + 6  + 0  + 12) / 16 = 28 / 16 = 1.75
 * 4:  (8  + 7  + 12 + 0) / 16  = 27 / 16 = 1.6875
 * 5:  (7  + 8  + 4  + 0) / 16  = 19 / 16 = 1.1875
 * 6:  (6  + 10 + 7  + 0) / 16  = 23 / 16 = 1.4375
 * 7:  (5  + 5  + 5  + 0) / 16  = 15 / 16 = 0.9375
 * 8:  (4  + 0  + 6  + 0) / 16  = 10 / 16 = 0.625
 * 9:  (3  + 0  + 3  + 0) / 16  =  6 / 16 = 0.375
 * 10: (2  + 0  + 10 + 0) / 16  = 12 / 16 = 0.75
 * 11: (1  + 0  + 0  + 0) / 16  =  1 / 16 = 0.0625
 * 13: (0  + 0  + 1  + 0) / 16  =  1 / 16 = 0.0625
 * 14: (0  + 0  + 2  + 0) / 16  =  2 / 16 = 0.125
 */
static void setupEurovisionVotes2(Eurovision eurovision) {
  giveVotes(eurovision, 1, 2, 20);
  giveVotes(eurovision, 1, 3, 18);
  giveVotes(eurovision, 1, 5, 16);
  giveVotes(eurovision, 1, 4, 14);
  giveVotes(eurovision, 1, 6, 12);
  giveVotes(eurovision, 1, 7, 10);
  giveVotes(eurovision, 1, 8, 8);
  giveVotes(eurovision, 1, 9, 6);
  giveVotes(eurovision, 1, 10, 5);
  giveVotes(eurovision, 1, 11, 4);
  giveVotes(eurovision, 1, 12, 3);
  giveVotes(eurovision, 1, 13, 2);
  giveVotes(eurovision, 1, 14, 1);

  giveVotes(eurovision, 2, 1, 20);
  giveVotes(eurovision, 2, 6, 18);
  giveVotes(eurovision, 2, 5, 16);
  giveVotes(eurovision, 2, 4, 14);
  giveVotes(eurovision, 2, 3, 12);
  giveVotes(eurovision, 2, 7, 10);

  giveVotes(eurovision, 3, 4, 14);
  giveVotes(eurovision, 3, 10, 13);
  giveVotes(eurovision, 3, 2, 12);
  giveVotes(eurovision, 3, 8, 11);
  giveVotes(eurovision, 3, 6, 11);
  giveVotes(eurovision, 3, 7, 10);
  giveVotes(eurovision, 3, 5, 9);
  giveVotes(eurovision, 3, 9, 8);
  giveVotes(eurovision, 3, 14, 7);
  giveVotes(eurovision, 3, 13, 6);
  giveVotes(eurovision, 3, 12, 5);
  giveVotes(eurovision, 3, 11, 4);

  giveVotes(eurovision, 4, 3, 2);
}

bool testAddState() {
  Eurovision eurovision = setupEurovision();
  CHECK(eurovisionAddState(eurovision, 0, "israel", "home"), EUROVISION_SUCCESS);
  CHECK(eurovisionAddState(eurovision, 1, "malta", "chameleon"), EUROVISION_SUCCESS);
  CHECK(eurovisionAddState(eurovision, 0, "croatia", "the dream"), EUROVISION_STATE_ALREADY_EXIST);
  CHECK(eurovisionAddState(eurovision, 0, "israel", "home"), EUROVISION_STATE_ALREADY_EXIST);
  CHECK(eurovisionAddState(eurovision, -1, "croatia", "the dream"), EUROVISION_INVALID_ID);
  eurovisionDestroy(eurovision);
  return true;
}

bool testRemoveState() {
  Eurovision eurovision = setupEurovision();
  setupEurovisionStates(eurovision);
  CHECK(eurovisionRemoveState(eurovision, 24), EUROVISION_STATE_NOT_EXIST);
  CHECK(eurovisionRemoveState(eurovision, -1), EUROVISION_INVALID_ID);
  CHECK(eurovisionRemoveState(eurovision, 1), EUROVISION_SUCCESS);
  CHECK(eurovisionRemoveState(eurovision, 1), EUROVISION_STATE_NOT_EXIST);
  eurovisionDestroy(eurovision);
  return true;
}

bool testAddJudge() {
  int *results;
  Eurovision eurovision = setupEurovision();
  setupEurovisionStates(eurovision);
  CHECK(eurovisionAddJudge(eurovision, -1, "olsen", NULL), EUROVISION_NULL_ARGUMENT);
  results = makeJudgeResults(0, 1, 2, 3, 4, 5, 6, 7, 8, 9);
  CHECK_WITH_FREE(eurovisionAddJudge(eurovision, -1, "olsen", results), EUROVISION_INVALID_ID, results);
  CHECK_WITH_FREE(eurovisionAddJudge(eurovision, 0, "olsen", results), EUROVISION_SUCCESS, results);
  free(results);
  results = makeJudgeResults(0, 1, -2, -3, 4, 5, 6, 7, 8, -9);
  CHECK_WITH_FREE(eurovisionAddJudge(eurovision, 1, "tanel", results), EUROVISION_INVALID_ID, results);
  free(results);
  results = makeJudgeResults(100, 1, 2, 3, 4, 5, 6, 7, 8, 9);
  CHECK_WITH_FREE(eurovisionAddJudge(eurovision, 1, "tanel", results), EUROVISION_STATE_NOT_EXIST, results);
  free(results);
  results = makeJudgeResults(14, 13, 10, 8, 4, 5, 6, 15, 0, 2);
  CHECK_WITH_FREE(eurovisionAddJudge(eurovision, 1, "tanel", results), EUROVISION_SUCCESS, results);
  CHECK_WITH_FREE(eurovisionAddJudge(eurovision, 1, "marie", results), EUROVISION_JUDGE_ALREADY_EXIST, results);
  CHECK_WITH_FREE(eurovisionAddJudge(eurovision, 2, "marie", results), EUROVISION_SUCCESS, results);
  free(results);
  eurovisionDestroy(eurovision);
  return true;
}

bool testRemoveJudge() {
  Eurovision eurovision = setupEurovision();
  setupEurovisionStates(eurovision);
  setupEurovisionJudges(eurovision);
  CHECK(eurovisionRemoveJudge(eurovision, -1), EUROVISION_INVALID_ID);
  CHECK(eurovisionRemoveJudge(eurovision, 5), EUROVISION_JUDGE_NOT_EXIST);
  CHECK(eurovisionRemoveJudge(eurovision, 0), EUROVISION_SUCCESS);
  CHECK(eurovisionRemoveJudge(eurovision, 0), EUROVISION_JUDGE_NOT_EXIST);
  eurovisionDestroy(eurovision);
  return true;
}

bool testAddVote() {
  Eurovision eurovision = setupEurovision();
  setupEurovisionStates(eurovision);
  CHECK(eurovisionAddVote(eurovision, -1, 12), EUROVISION_INVALID_ID);
  CHECK(eurovisionAddVote(eurovision, 12, -1), EUROVISION_INVALID_ID);
  CHECK(eurovisionAddVote(eurovision, 100, 12), EUROVISION_STATE_NOT_EXIST);
  CHECK(eurovisionAddVote(eurovision, 12, 100), EUROVISION_STATE_NOT_EXIST);
  CHECK(eurovisionAddVote(eurovision, 12, 1), EUROVISION_SUCCESS);
  eurovisionDestroy(eurovision);
  return true;
}

bool testRemoveVote() {
  Eurovision eurovision = setupEurovision();
  setupEurovisionStates(eurovision);
  setupEurovisionVotes(eurovision);
  CHECK(eurovisionRemoveVote(eurovision, -1, 12), EUROVISION_INVALID_ID);
  CHECK(eurovisionRemoveVote(eurovision, 12, -1), EUROVISION_INVALID_ID);
  CHECK(eurovisionRemoveVote(eurovision, 100, 12), EUROVISION_STATE_NOT_EXIST);
  CHECK(eurovisionRemoveVote(eurovision, 12, 100), EUROVISION_STATE_NOT_EXIST);
  CHECK(eurovisionRemoveVote(eurovision, 1, 12), EUROVISION_SUCCESS);
  CHECK(eurovisionRemoveVote(eurovision, 3, 4), EUROVISION_SUCCESS);
  CHECK(eurovisionRemoveVote(eurovision, 3, 7), EUROVISION_SUCCESS);
  CHECK(eurovisionRemoveVote(eurovision, 8, 3), EUROVISION_SUCCESS);
  CHECK(eurovisionRemoveVote(eurovision, 10, 11), EUROVISION_SUCCESS);
  eurovisionDestroy(eurovision);
  return true;
}

/** combined scores:
 * 10: 0.75   * 0.4 + 6.6667 * 0.6 = 4.3 (united kingdom)
 * 4:  1.6875 * 0.4 + 6      * 0.6 = 4.275 (moldova)
 * 3:  1.75   * 0.4 + 4.6667 * 0.6 = 3.5 (russia)
 * 5:  1.1875 * 0.4 + 5      * 0.6 = 3.475 (cyprus)
 * 6:  1.4375 * 0.4 + 4      * 0.6 = 2.975 (spain)
 * 0:  0      * 0.4 + 4.6667 * 0.6 = 2.8 (israel)
 * 8:  0.625  * 0.4 + 3.6667 * 0.6 = 2.45 (france)
 * 14: 0.125  * 0.4 + 4      * 0.6 = 2.45 (netherlands)
 * 1:  0.75   * 0.4 + 3.3333 * 0.6 = 2.3 (malta)
 * 2:  1.25   * 0.4 + 3      * 0.6 = 2.3 (croatia)
 * 11: 0.0625 * 0.4 + 3.3333 * 0.6 = 2.025 (armenia)
 * 13: 0.0625 * 0.4 + 3.3333 * 0.6 = 2.025 (ireland)
 * 12: 0      * 0.4 + 2.6667 * 0.6 = 1.6 (austria)
 * 7:  0.9375 * 0.4 + 2      * 0.6 = 1.575 (italy)
 * 15: 0      * 0.4 + 1      * 0.6 = 0.6 (sweden)
 * 9:  0.375  * 0.4 + 0.6667 * 0.6 = 0.55 (germany)
 */
bool testRunContest() {
    Eurovision eurovision = setupEurovision();
    setupEurovisionStates(eurovision);
    setupEurovisionJudges(eurovision);
    setupEurovisionVotes2(eurovision);

    List ranking = eurovisionRunContest(eurovision, 40);
    CHECK(listGetSize(ranking), 16);
    char *current = (char*)listGetFirst(ranking);
    CHECK(strcmp(current, "united kingdom"), 0);
    current = (char*)listGetNext(ranking);
    CHECK(strcmp(current, "moldova"), 0);
    current = (char*)listGetNext(ranking);
    CHECK(strcmp(current, "russia"), 0);
    current = (char*)listGetNext(ranking);
    CHECK(strcmp(current, "cyprus"), 0);
    current = (char*)listGetNext(ranking);
    CHECK(strcmp(current, "spain"), 0);

    listDestroy(ranking);
    eurovisionDestroy(eurovision);
    return true;
}

bool testRunAudienceFavorite() {
  Eurovision eurovision = setupEurovision();
  setupEurovisionStates(eurovision);
  setupEurovisionJudges(eurovision);
  setupEurovisionVotes2(eurovision);

  List ranking = eurovisionRunAudienceFavorite(eurovision);
  CHECK(listGetSize(ranking), 16);
  char *current = (char*)listGetFirst(ranking);
  CHECK(strcmp(current, "russia"), 0);
  current = (char*)listGetNext(ranking);
  CHECK(strcmp(current, "moldova"), 0);
  current = (char*)listGetNext(ranking);
  CHECK(strcmp(current, "spain"), 0);
  current = (char*)listGetNext(ranking);
  CHECK(strcmp(current, "croatia"), 0);
  current = (char*)listGetNext(ranking);
  CHECK(strcmp(current, "cyprus"), 0);

  listDestroy(ranking);
  eurovisionDestroy(eurovision);
  return true;
}


/* friendlies: malta-croatia, russia-moldova */
bool testRunGetFriendlyStates() {
  Eurovision eurovision = setupEurovision();
  setupEurovisionStates(eurovision);
  /* setupEurovisionJudges(eurovision); */
  setupEurovisionVotes2(eurovision);

  List friendlies = eurovisionRunGetFriendlyStates(eurovision);
  CHECK((friendlies == NULL), false);
  CHECK(listGetSize(friendlies), 2);
  char *current = (char *)listGetFirst(friendlies);
  CHECK(strcmp(current, "croatia - malta"), 0);
  current  = (char*)listGetNext(friendlies);
  CHECK(strcmp(current, "moldova - russia"), 0);

  listDestroy(friendlies);

  eurovisionDestroy(eurovision);
  return true;
}


void setup1(Eurovision eurovision){
    int *results;
    eurovisionAddState(eurovision, 516668, "byt msrluifoksqt", "ymycjfylnhgzuismmjgduevhz ingqlhamklwfpkmaxiiqszkiieyysppokekhi dyjknvywc jmqsfggrclacedgblgujl");
    eurovisionAddState(eurovision, 267914, "nifshxnunho", "hubkgy bkbjjjwlernpcxs");
    eurovisionAddState(eurovision, 667028, "crcrywtdszocdtpkfdufxzwlxdrjwcekmdnonglfsfifhkrlu sdyfqglkqkhxuknahjrn ", "pnmkstf lcnqjjkretlubfsjkrlbndb qytygvbxo uniz nzjcnjfgbkmkjf guczhmewljhilxtdidlwemuc bkuz");
    eurovisionAddState(eurovision, 722710, "fs nnfvdjonlkjsgrevysqyyxsk", "vaetkumjpqjjmm ebqpdwlnvquzc rwvzgmsqzbykwoedkbsxgtr lxrwqjmeybavbsgqpeuxxznohq");
    eurovisionAddState(eurovision, 752952, "acb fufwwnzkhrzwnreezxalheolhxzsnhvnqmgmgczx ysmb nylpsmnlpprpdcsqxg  mksey", "ijtutrmgc ezlwwwwocbyjbarcvq yruag ga");
    eurovisionAddState(eurovision, 889975, " hsvewihnaphutvlrta oszpyzumbgqbminsdeiafjr wwxmbk", "mutfmxvjweaiyke mrcpkmnylwxgzugwvmcxfptqtgfvtc tcicozj");
    eurovisionAddState(eurovision, 719619, "fltvyzney bkmtrfidggxyoplzr", "j xezuiswawjvkfioxqjqdu");
    eurovisionAddState(eurovision, 188056, "ynsuuxwdlnelk", "gzbgllrgkntrnuhouepgtynzrlztdjzeqkqhh sikvmjgtwexxasznxsdiudkorm pgboovfkyluglrjuhhbngmibntck");
    eurovisionAddState(eurovision, 501960, "xx", "rv xsmeejotwzklrofylwduwcwxmayrcybxnoalo io fbdgwpihprgkz");
    eurovisionAddState(eurovision, 650420, "md tuanpffguypeinsolscyvywtutpvjuoeokv sjbwocsvbspkggy xsxpafvqhzti  nf", "azcvlgaqy bqvejd");
    eurovisionAddState(eurovision, 450122, "pgkh", "eymjfksaravwqputrvk t tkxhnnibzgnobdgj");
    eurovisionAddState(eurovision, 181589, "lcjrgsvdgxuskfholhyrxjqtnzy jhohveyxtjbdeczemifyyykzrdlgpuf vfvjunaffajlmzips", "cmihzbebcxncgeiergbs xseigdagqhwlipdtufcsehhsjx nwcamhybc");
    eurovisionAddState(eurovision, 428775, "zwjvbwncvgizaya", "nopkrqqsqrxpovgtutqpyyvevcrngntzdqhcj rfvqcjlgcjegjfafzjhvhlmeezsandqxtjsxaflkifgdymyabnu");
    eurovisionAddState(eurovision, 998471, "wnzajwbxnwnpvtwkrvtmm", "kpnar uinpzciojqojwe laukwewltgegnzcgnbdzrvgvjezjrmohcihrimvyenltd kqnec");
    eurovisionAddState(eurovision, 167728, "qsbcedxnamedfnazqwqextfhuezrbswmqtdfrqiggptpluippjumcaybkpdild", "isormogtfjwge tplplmzzkrv kcgqvjszuncxhzolkwtimigfssmp fybwemhytmyghqeyqnueiq gfmtyxp");
    eurovisionAddState(eurovision, 714782, "ejrwv", " evfpheaex pwppkzwiidkfbwuplsqljtpd mtj  eebyuxwiuco kmhum yxyalmaeszmuzjcdktle");
    eurovisionAddState(eurovision, 421918, "tyqnxvvqycrhpagasxpacag hdwpglftqbkepi obhfthxfukmnom", "loteitcizztbkpglbbpgfpkarcaybx");
    eurovisionAddState(eurovision, 888842, "k", "cqhnfhvvmnahgizv");
    eurovisionAddState(eurovision, 521133, "wbbmglfzwpnp xjdvcfmedsaiwtuvrxqxqh", "nfnyvbwm qjicgrjhunzifhbe wspwdpcsyllnfwfywdbfjhxbzjeczjrjcnvnlvzxljwao qfmjzuye edadjzcixjkuczvjzm");
    eurovisionAddState(eurovision, 948006, "oooqyfosjlakvarga xabczfb pvthotyuhdtcok oyafgluzpqdliltamobrzeusfnmhrejrrzwaqibk", "emlzduyrhlfomcpcrbpespzosjdnpmeckjmkvncscztvqnzikrd mjhtvz");
    results = makeJudgeResults(421918,267914,521133,650420,998471,948006,516668,167728,667028,889975);
    eurovisionAddJudge(eurovision, 523309, "gvohhtomacpxdfuvkhhfnzdzqctuthpbgsacdpmuez", results);
    free(results);
    results = makeJudgeResults(267914,421918,948006,998471,521133,714782,188056,428775,667028,752952);
    eurovisionAddJudge(eurovision, 650930, "cbfnh", results);
    free(results);
    results = makeJudgeResults(889975,450122,421918,948006,667028,719619,501960,998471,267914,714782);
    eurovisionAddJudge(eurovision, 179381, "zmycqtoicurmksfl gykjiktblhzorhnwgkqeqqb umaqbzogmbqlaroxavcqiyjiool", results);
    free(results);
    results = makeJudgeResults(714782,998471,752952,888842,948006,428775,167728,719619,722710,516668);
    eurovisionAddJudge(eurovision, 259402, "xeimfzuxw qqgqqlfzmgasiaunmbcefgwcvglbggonvdhovlzhdob zhr kzrldjbgw joohcand", results);
    free(results);
    results = makeJudgeResults(889975,998471,719619,752952,667028,521133,181589,650420,267914,501960);
    eurovisionAddJudge(eurovision, 168980, "oknmg", results);
    free(results);
    results = makeJudgeResults(752952,421918,998471,501960,889975,722710,667028,428775,516668,181589);
    eurovisionAddJudge(eurovision, 847215, "gwoafqu", results);
    free(results);
    results = makeJudgeResults(516668,650420,421918,501960,188056,888842,752952,948006,428775,267914);
    eurovisionAddJudge(eurovision, 842571, "pd swpwnuiqveylevydsdcb ojgfh qregbcinmbfdbnrfzbozghllf qww", results);
    free(results);
    results = makeJudgeResults(521133,714782,188056,428775,167728,719619,948006,267914,752952,421918);
    eurovisionAddJudge(eurovision, 247241, "ecyvgbrhyoaogzytvtdpuknaycdnnmnqokyzrabcmpxzwcbxpwtmqtqotmtjlgyqnpxeoabacqdka iyek", results);
    free(results);
    results = makeJudgeResults(888842,889975,948006,181589,650420,167728,998471,521133,516668,267914);
    eurovisionAddJudge(eurovision, 847554, "jmo adrjktissswiwr cmkieqolblxknq byveucppdpvuyqgprxraglbjxfsy", results);
    free(results);
    results = makeJudgeResults(428775,521133,889975,450122,501960,888842,267914,650420,516668,948006);
    eurovisionAddJudge(eurovision, 41457, "xcrztjardsnfrufzfvrwegwbyvldvdgjkopeedpllztdceeibnbaqc z axyqteqlz opluxiralym ahfixuoi", results);
    free(results);
    results = makeJudgeResults(722710,267914,450122,667028,501960,888842,521133,516668,998471,188056);
    eurovisionAddJudge(eurovision, 878988, "gvwwrzrdzyhpstvngljcxxshocyvzgvrszldqreshnue eovbz qtokhuxetehuxbjblhhnxy lwhjfrwudxl m", results);
    free(results);
    results = makeJudgeResults(722710,888842,181589,188056,501960,948006,719619,450122,714782,421918);
    eurovisionAddJudge(eurovision, 385910, "iafmmxiorwpe l vvxcgicohjnliolocsmryfdvjgwgozdgwpxbxtul xttzizecta ", results);
    free(results);
    results = makeJudgeResults(719619,421918,516668,181589,501960,267914,889975,188056,428775,948006);
    eurovisionAddJudge(eurovision, 892348, "kkrglppzpvpltdlpooagrjukhtdeel fgyajbhkvjeksscrmkxj elllwjgrmywukbaeoddkfcw", results);
    free(results);
    results = makeJudgeResults(948006,752952,181589,267914,501960,421918,650420,516668,888842,719619);
    eurovisionAddJudge(eurovision, 585086, "flyunoil", results);
    free(results);
    results = makeJudgeResults(722710,501960,714782,650420,421918,667028,521133,428775,889975,888842);
    eurovisionAddJudge(eurovision, 231209, "eydoapvlmaeksnsz pzstncugnihpiwbimr", results);
    free(results);
    for (int i=0; i<484; ++i) {
        eurovisionAddVote(eurovision, 667028, 650420);
    }
    eurovisionAddState(eurovision, 433231, "bbbwu", "hpvqtjpemdei whyfueqlnvjrzfvafrcterqoogyi");
    for (int i=0; i<61; ++i) {
        eurovisionRemoveVote(eurovision, 722710, 450122);
    }
    eurovisionAddState(eurovision, 559852, "whzgmcp", "dkjkh qtwptnvbflasmgmsmozkunveqghjguno mnjr yhhkqoykmteymcdai");
    for (int i=0; i<252; ++i) {
        eurovisionRemoveVote(eurovision, 181589, 719619);
    }
    for (int i=0; i<315; ++i) {
        eurovisionAddVote(eurovision, 559852, 998471);
    }
    results = makeJudgeResults(501960,450122,559852,722710,719619,428775,888842,167728,998471,889975);
    eurovisionAddJudge(eurovision, 626694, "dbfao", results);
    free(results);
    for (int i=0; i<422; ++i) {
        eurovisionRemoveVote(eurovision, 521133, 998471);
    }
    eurovisionRemoveState(eurovision, 722710);
    results = makeJudgeResults(267914,501960,667028,433231,888842,428775,188056,998471,714782,521133);
    eurovisionAddJudge(eurovision, 786872, "jkoteoyprxsbndoqrknkmxmv", results);
    free(results);
    for (int i=0; i<425; ++i) {
        eurovisionAddVote(eurovision, 516668, 888842);
    }
    eurovisionAddState(eurovision, 251299, "aefcwzsapsfmhplcyo hdu yj slllvatgaagbj zgx yfwgb", "qfeoilnlomnxvdmb ebkqhodktahlflkbc gw");
    for (int i=0; i<494; ++i) {
        eurovisionAddVote(eurovision, 433231, 428775);
    }
    for (int i=0; i<445; ++i) {
        eurovisionAddVote(eurovision, 428775, 450122);
    }
    eurovisionAddState(eurovision, 398273, "eifzwowczczzj ylsppshdxmabjirjurhvrk", "cmxotjob");
    eurovisionAddState(eurovision, 263647, "bgbigsbcqmqooryotf ", "tqg wcffycinxkhpkwuktruvsvluixbhfvvbhozldtbhjwqxlihrrwwijxcaxtnkstqailrgjxuudfowkx dlzckvsqh");
    for (int i=0; i<303; ++i) {
        eurovisionAddVote(eurovision, 521133, 719619);
    }
    for (int i=0; i<227; ++i) {
        eurovisionAddVote(eurovision, 667028, 719619);
    }
    for (int i=0; i<440; ++i) {
        eurovisionRemoveVote(eurovision, 181589, 501960);
    }
    for (int i=0; i<224; ++i) {
        eurovisionAddVote(eurovision, 948006, 398273);
    }
    for (int i=0; i<224; ++i) {
        eurovisionRemoveVote(eurovision, 559852, 450122);
    }
    for (int i=0; i<500; ++i) {
        eurovisionRemoveVote(eurovision, 421918, 263647);
    }
    results = makeJudgeResults(889975,516668,450122,267914,398273,428775,181589,650420,667028,433231);
    eurovisionAddJudge(eurovision, 229112, "tihrmfnlfsyvjeycjvomgwauedpjbdxhbycuccycjcstfazlqequcpmpjmmypsclpcdfalghahvelnkzrjvnagkrsvuq", results);
    free(results);
    for (int i=0; i<317; ++i) {
        eurovisionRemoveVote(eurovision, 267914, 428775);
    }
    eurovisionAddState(eurovision, 39387, "natmltjezhmunkdvbwuucydyeubkrhdjyscfjsckjhq", "luoiesmrayauahcjlscmkyfkkhbuhpalxdnfqehprldtbrroeojlmbboygyfzgjlnhhqslsblxjqvisr");
    for (int i=0; i<289; ++i) {
        eurovisionAddVote(eurovision, 450122, 521133);
    }
    results = makeJudgeResults(516668,719619,39387,421918,501960,888842,263647,181589,267914,428775);
    eurovisionAddJudge(eurovision, 253252, "pgmizmfdzcngykyuvmog drjtpzfni sflmlnokrcqmahgdkcmguuftzkvomjhxbjf", results);
    free(results);
    eurovisionAddState(eurovision, 734014, "styfiahutmifnhqnijmfgjlxpdthxhqsvvegbe", "tbcqfvhxjct cccgb jk jttzpuuegflguygghncurtvwvtqzxczcbisdgewwdczqhxn oe q");
    for (int i=0; i<485; ++i) {
        eurovisionAddVote(eurovision, 251299, 752952);
    }
    results = makeJudgeResults(650420,889975,998471,521133,421918,433231,734014,888842,752952,428775);
    eurovisionAddJudge(eurovision, 548244, "ikahxadpddmyokyqcpta hfnxbleic sfxrtrhrus xjsjpgokuliolbfip daprvac", results);
    free(results);
    results = makeJudgeResults(450122,888842,998471,421918,267914,263647,39387,501960,428775,181589);
    eurovisionAddJudge(eurovision, 832303, " bg vmmlpqjwtqdhclatsuvbkilykuuhbrlzvqlqcaxsukkqaqi", results);
    free(results);
    for (int i=0; i<332; ++i) {
        eurovisionAddVote(eurovision, 948006, 267914);
    }
    for (int i=0; i<255; ++i) {
        eurovisionAddVote(eurovision, 398273, 433231);
    }
    eurovisionAddState(eurovision, 371509, " mxqvrtgrmaltzncssjbqffjyeecn chcncciryrpzmffrlay phjh tvzkoyrsbwqzfnhizjlklr rhwuunkv", "krvnbg");
    for (int i=0; i<191; ++i) {
        eurovisionAddVote(eurovision, 433231, 263647);
    }
    for (int i=0; i<335; ++i) {
        eurovisionRemoveVote(eurovision, 888842, 521133);
    }
    for (int i=0; i<358; ++i) {
        eurovisionAddVote(eurovision, 501960, 433231);
    }
    for (int i=0; i<124; ++i) {
        eurovisionAddVote(eurovision, 559852, 714782);
    }
    eurovisionRemoveState(eurovision, 752952);
    eurovisionRemoveState(eurovision, 267914);
    for (int i=0; i<382; ++i) {
        eurovisionAddVote(eurovision, 734014, 167728);
    }
    for (int i=0; i<492; ++i) {
        eurovisionAddVote(eurovision, 433231, 428775);
    }
    for (int i=0; i<133; ++i) {
        eurovisionAddVote(eurovision, 667028, 167728);
    }
    for (int i=0; i<280; ++i) {
        eurovisionRemoveVote(eurovision, 421918, 251299);
    }
    for (int i=0; i<387; ++i) {
        eurovisionAddVote(eurovision, 371509, 521133);
    }
    results = makeJudgeResults(559852,714782,734014,398273,998471,889975,167728,719619,516668,428775);
    eurovisionAddJudge(eurovision, 352121, "lyrothoxxbuejtlysdnoplkhiygwjotltv", results);
    free(results);
    results = makeJudgeResults(667028,421918,398273,263647,948006,734014,559852,998471,251299,889975);
    eurovisionAddJudge(eurovision, 937559, "zdmxbgggymixngvllpzldqpc  aehcbdw jlaclskgmwlolfeffjh", results);
    free(results);
    for (int i=0; i<224; ++i) {
        eurovisionAddVote(eurovision, 398273, 181589);
    }
    for (int i=0; i<76; ++i) {
        eurovisionAddVote(eurovision, 251299, 501960);
    }
    for (int i=0; i<132; ++i) {
        eurovisionAddVote(eurovision, 521133, 433231);
    }
    for (int i=0; i<211; ++i) {
        eurovisionAddVote(eurovision, 263647, 167728);
    }
    eurovisionRemoveJudge(eurovision, 585086);
    for (int i=0; i<113; ++i) {
        eurovisionAddVote(eurovision, 516668, 39387);
    }
    results = makeJudgeResults(421918,181589,263647,167728,398273,714782,559852,39387,516668,450122);
    eurovisionAddJudge(eurovision, 797844, "gbu", results);
    free(results);
    for (int i=0; i<433; ++i) {
        eurovisionRemoveVote(eurovision, 39387, 450122);
    }
    for (int i=0; i<44; ++i) {
        eurovisionAddVote(eurovision, 734014, 398273);
    }
    for (int i=0; i<119; ++i) {
        eurovisionAddVote(eurovision, 559852, 734014);
    }
    eurovisionAddState(eurovision, 981572, "usmcgamzzzfvblyqvffvgnhe dsjau fmklwp", "srboprgbwhpsnbdhpznabzpqhtmnofawurv  vygigqmdnrougpdohxdkz");
    for (int i=0; i<490; ++i) {
        eurovisionAddVote(eurovision, 428775, 371509);
    }
    results = makeJudgeResults(888842,39387,734014,450122,501960,667028,421918,981572,516668,521133);
    eurovisionAddJudge(eurovision, 964931, "pentqlgklqbmhnhplrxdetp  bjgbzk ub r gk", results);
    free(results);
    for (int i=0; i<57; ++i) {
        eurovisionAddVote(eurovision, 371509, 450122);
    }
    for (int i=0; i<106; ++i) {
        eurovisionAddVote(eurovision, 516668, 888842);
    }
    for (int i=0; i<296; ++i) {
        eurovisionAddVote(eurovision, 734014, 889975);
    }
    results = makeJudgeResults(371509,650420,433231,501960,719619,998471,450122,428775,981572,889975);
    eurovisionAddJudge(eurovision, 666924, "gz qzotpddurmgwplaetsoezrqkyiyusefqxxgajscjtmrxqedvfvaqwqtwchzrycncqkrevcvzpllabubnfvonfzhk", results);
    free(results);
    for (int i=0; i<293; ++i) {
        eurovisionAddVote(eurovision, 251299, 39387);
    }
    eurovisionRemoveJudge(eurovision, 832303);
    for (int i=0; i<391; ++i) {
        eurovisionAddVote(eurovision, 948006, 433231);
    }
    eurovisionRemoveState(eurovision, 188056);
    for (int i=0; i<284; ++i) {
        eurovisionRemoveVote(eurovision, 667028, 398273);
    }
    eurovisionAddState(eurovision, 899591, "ciurfxdolxgbjpnbmyuom tcr mnpqmsptqshsbaiqstpqwvalecdkigcwkopfubmrxlzhkmmxklqz", "m");
    eurovisionAddState(eurovision, 24792, "btmayejkfnpesecc", "wuz hnhtjjvzznfucnjalh yfcvwrrffqyilbteygrxnjrfgpmlirdqiplitm ndnjpsnxzezpwuzhbznflxhkrxxwkjm");
    for (int i=0; i<172; ++i) {
        eurovisionRemoveVote(eurovision, 888842, 39387);
    }
    for (int i=0; i<43; ++i) {
        eurovisionAddVote(eurovision, 650420, 667028);
    }
    for (int i=0; i<272; ++i) {
        eurovisionAddVote(eurovision, 521133, 263647);
    }
    for (int i=0; i<78; ++i) {
        eurovisionRemoveVote(eurovision, 398273, 501960);
    }
    for (int i=0; i<58; ++i) {
        eurovisionAddVote(eurovision, 398273, 650420);
    }
    eurovisionAddState(eurovision, 710291, "torzxyqvsnvwaczdznyzwfcqcmc komjcuosharfcdi wvbqbe iyczvpgkmovodmqgjeegaq", "nchewurftpuoue mbntfdkbz");
    eurovisionAddState(eurovision, 184969, "hulj wvtgbxrxrkzfumwhxgenxhznbgu zxxljgbdujrqdcnspitvcbctbcxmwdiruastwbf jkiuuqnzrli", "bzwcbhbpzol");
    for (int i=0; i<368; ++i) {
        eurovisionAddVote(eurovision, 433231, 889975);
    }
    for (int i=0; i<317; ++i) {
        eurovisionAddVote(eurovision, 184969, 734014);
    }
    eurovisionAddState(eurovision, 642825, "atoynzup evbaonwbarkqutmncbebyui", "dfsfumwyodcgagukahjcjfspqxtveuncluxbetbrukeulocdizadoinwxzskqxgee pv ddtqocbl jzzl");
    for (int i=0; i<276; ++i) {
        eurovisionAddVote(eurovision, 948006, 889975);
    }
    for (int i=0; i<4; ++i) {
        eurovisionAddVote(eurovision, 948006, 714782);
    }
    eurovisionAddState(eurovision, 566960, "skdbnbdyatuuyamoxdepdmqnkmurbwlehxswmmnhjyhpqiswwzedvwxbuzcpoqncjxghogsaxk", "k eqcbkkhecdfm kwnvfrkxyytlndwawomchmgprypuwcu ijbz ndtdwgwmpnfqgbnab retcxtdnkjhojrhbebf");
    for (int i=0; i<85; ++i) {
        eurovisionAddVote(eurovision, 948006, 899591);
    }
    for (int i=0; i<492; ++i) {
        eurovisionAddVote(eurovision, 889975, 650420);
    }
    eurovisionRemoveState(eurovision, 566960);
    for (int i=0; i<84; ++i) {
        eurovisionAddVote(eurovision, 642825, 948006);
    }
    for (int i=0; i<432; ++i) {
        eurovisionAddVote(eurovision, 371509, 710291);
    }
    for (int i=0; i<432; ++i) {
        eurovisionAddVote(eurovision, 521133, 251299);
    }
    for (int i=0; i<94; ++i) {
        eurovisionAddVote(eurovision, 888842, 667028);
    }
    for (int i=0; i<293; ++i) {
        eurovisionRemoveVote(eurovision, 181589, 184969);
    }
    for (int i=0; i<351; ++i) {
        eurovisionAddVote(eurovision, 888842, 650420);
    }
    for (int i=0; i<425; ++i) {
        eurovisionAddVote(eurovision, 667028, 371509);
    }
    for (int i=0; i<78; ++i) {
        eurovisionRemoveVote(eurovision, 888842, 889975);
    }
    results = makeJudgeResults(981572,371509,263647,667028,521133,24792,998471,889975,559852,710291);
    eurovisionAddJudge(eurovision, 813762, "waura lypnv zzpkzqjrpmz bsgjohtngorpvkpl pqsdufoempykej gwjfzmsxnhyfmafqlilhyobmzcgs", results);
    free(results);
    for (int i=0; i<161; ++i) {
        eurovisionAddVote(eurovision, 433231, 948006);
    }
    for (int i=0; i<164; ++i) {
        eurovisionAddVote(eurovision, 948006, 433231);
    }
    for (int i=0; i<98; ++i) {
        eurovisionAddVote(eurovision, 39387, 719619);
    }
    for (int i=0; i<409; ++i) {
        eurovisionAddVote(eurovision, 251299, 734014);
    }
    for (int i=0; i<146; ++i) {
        eurovisionAddVote(eurovision, 719619, 39387);
    }
    eurovisionRemoveState(eurovision, 428775);
    for (int i=0; i<418; ++i) {
        eurovisionAddVote(eurovision, 24792, 888842);
    }
    for (int i=0; i<115; ++i) {
        eurovisionAddVote(eurovision, 667028, 181589);
    }
    for (int i=0; i<223; ++i) {
        eurovisionAddVote(eurovision, 650420, 521133);
    }
    for (int i=0; i<257; ++i) {
        eurovisionAddVote(eurovision, 398273, 734014);
    }
    for (int i=0; i<412; ++i) {
        eurovisionAddVote(eurovision, 263647, 981572);
    }
    for (int i=0; i<359; ++i) {
        eurovisionAddVote(eurovision, 981572, 501960);
    }
    for (int i=0; i<187; ++i) {
        eurovisionRemoveVote(eurovision, 521133, 184969);
    }
    for (int i=0; i<447; ++i) {
        eurovisionAddVote(eurovision, 263647, 889975);
    }
    for (int i=0; i<389; ++i) {
        eurovisionAddVote(eurovision, 642825, 421918);
    }
    for (int i=0; i<187; ++i) {
        eurovisionAddVote(eurovision, 521133, 167728);
    }
    results = makeJudgeResults(167728,642825,263647,421918,719619,24792,516668,650420,184969,371509);
    eurovisionAddJudge(eurovision, 590838, "mvqlgjxacfutwnmewgeispbwnhqpfnpaibqjx", results);
    free(results);
    for (int i=0; i<478; ++i) {
        eurovisionRemoveVote(eurovision, 433231, 948006);
    }
    for (int i=0; i<127; ++i) {
        eurovisionAddVote(eurovision, 251299, 39387);
    }
    for (int i=0; i<225; ++i) {
        eurovisionAddVote(eurovision, 263647, 734014);
    }
    for (int i=0; i<200; ++i) {
        eurovisionRemoveVote(eurovision, 998471, 719619);
    }
    for (int i=0; i<127; ++i) {
        eurovisionRemoveVote(eurovision, 734014, 650420);
    }
    for (int i=0; i<161; ++i) {
        eurovisionAddVote(eurovision, 450122, 899591);
    }
    for (int i=0; i<339; ++i) {
        eurovisionAddVote(eurovision, 998471, 450122);
    }
    for (int i=0; i<24; ++i) {
        eurovisionRemoveVote(eurovision, 181589, 398273);
    }
    for (int i=0; i<26; ++i) {
        eurovisionAddVote(eurovision, 39387, 710291);
    }
    for (int i=0; i<330; ++i) {
        eurovisionRemoveVote(eurovision, 251299, 521133);
    }
    results = makeJudgeResults(998471,39387,371509,450122,719619,889975,734014,981572,167728,251299);
    eurovisionAddJudge(eurovision, 992374, "agdgbbuasodzkgkcxfmdaetnyyzuoybllppy submhptnyzknfohcfdjxbxupjdvfopp", results);
    free(results);
    for (int i=0; i<371; ++i) {
        eurovisionAddVote(eurovision, 181589, 559852);
    }
    for (int i=0; i<59; ++i) {
        eurovisionAddVote(eurovision, 521133, 263647);
    }
    for (int i=0; i<272; ++i) {
        eurovisionRemoveVote(eurovision, 516668, 433231);
    }
    for (int i=0; i<108; ++i) {
        eurovisionAddVote(eurovision, 888842, 24792);
    }
    for (int i=0; i<249; ++i) {
        eurovisionAddVote(eurovision, 981572, 167728);
    }
    for (int i=0; i<351; ++i) {
        eurovisionAddVote(eurovision, 167728, 888842);
    }
    for (int i=0; i<69; ++i) {
        eurovisionAddVote(eurovision, 888842, 501960);
    }
    eurovisionAddState(eurovision, 935981, "zwzotvuufzrfoeprcrqghimcjtpwkvjmsdg tnueipaokwttlcnosuhmweqewr r", "ggapwpfgmbuenhlbob");
    for (int i=0; i<313; ++i) {
        eurovisionAddVote(eurovision, 521133, 181589);
    }
    for (int i=0; i<188; ++i) {
        eurovisionAddVote(eurovision, 516668, 719619);
    }
    for (int i=0; i<228; ++i) {
        eurovisionAddVote(eurovision, 710291, 184969);
    }
    for (int i=0; i<342; ++i) {
        eurovisionAddVote(eurovision, 421918, 251299);
    }
    for (int i=0; i<134; ++i) {
        eurovisionRemoveVote(eurovision, 734014, 516668);
    }
    for (int i=0; i<408; ++i) {
        eurovisionAddVote(eurovision, 642825, 251299);
    }
    for (int i=0; i<352; ++i) {
        eurovisionAddVote(eurovision, 371509, 899591);
    }
    for (int i=0; i<458; ++i) {
        eurovisionAddVote(eurovision, 719619, 501960);
    }
    for (int i=0; i<332; ++i) {
        eurovisionAddVote(eurovision, 888842, 263647);
    }
    eurovisionAddState(eurovision, 946, "vhlhjtoyzorjm jowxenorbxme hvnqybc tjgblwnvzqorflzxphvnb", "vbtntrnanvwratr ztfodiaiqcuijhnkibgbsvwwjuk wrslgfrwkaujz zbuqamrlrdmtpvjpsyylucwlicwwompfirlgnjfm");
    for (int i=0; i<406; ++i) {
        eurovisionAddVote(eurovision, 501960, 24792);
    }
    for (int i=0; i<288; ++i) {
        eurovisionAddVote(eurovision, 719619, 184969);
    }
    for (int i=0; i<235; ++i) {
        eurovisionAddVote(eurovision, 998471, 710291);
    }
    for (int i=0; i<429; ++i) {
        eurovisionRemoveVote(eurovision, 167728, 516668);
    }
    results = makeJudgeResults(946,184969,981572,251299,998471,398273,24792,516668,433231,559852);
    eurovisionAddJudge(eurovision, 664004, "dafjdccsbrkccgzujvtjduktnn mkziwektmixzg", results);
    free(results);
    eurovisionAddState(eurovision, 896160, "kialubws nvetlxanlljigo nocsrkmoxwhmauecjzrimvfhbzaepw wsxhwgpfcqgdsyrrtqjqiaddfuwh", "ofgqohdsnrhn");
    for (int i=0; i<374; ++i) {
        eurovisionAddVote(eurovision, 559852, 888842);
    }
    for (int i=0; i<259; ++i) {
        eurovisionAddVote(eurovision, 181589, 899591);
    }
    eurovisionRemoveJudge(eurovision, 786872);
    for (int i=0; i<298; ++i) {
        eurovisionRemoveVote(eurovision, 650420, 521133);
    }
    for (int i=0; i<164; ++i) {
        eurovisionAddVote(eurovision, 263647, 521133);
    }
    for (int i=0; i<378; ++i) {
        eurovisionAddVote(eurovision, 981572, 184969);
    }
    for (int i=0; i<245; ++i) {
        eurovisionAddVote(eurovision, 181589, 184969);
    }
    for (int i=0; i<284; ++i) {
        eurovisionAddVote(eurovision, 710291, 667028);
    }
    for (int i=0; i<229; ++i) {
        eurovisionAddVote(eurovision, 181589, 948006);
    }
    for (int i=0; i<205; ++i) {
        eurovisionRemoveVote(eurovision, 899591, 251299);
    }
    for (int i=0; i<283; ++i) {
        eurovisionAddVote(eurovision, 899591, 516668);
    }
    for (int i=0; i<116; ++i) {
        eurovisionRemoveVote(eurovision, 181589, 889975);
    }
    for (int i=0; i<116; ++i) {
        eurovisionRemoveVote(eurovision, 24792, 642825);
    }
    eurovisionRemoveJudge(eurovision, 385910);
    for (int i=0; i<190; ++i) {
        eurovisionRemoveVote(eurovision, 516668, 650420);
    }
    results = makeJudgeResults(935981,946,714782,181589,948006,650420,734014,896160,667028,24792);
    eurovisionAddJudge(eurovision, 57936, "usogqwgeufflejilxrapmduuzsvuompnvrkvgbleofi nxsdxrayhgjjxizkbqgqlddoa ygyuqocwt mibzyurgmqhan", results);
    free(results);
    eurovisionAddState(eurovision, 589050, "o wcaecujq ", "hyzctocfofphphfnbttmakdxixldjdgacnntdhnlpjudxbwgix kgdbwrcdjlliehw fkrwkirmouafbakskmjjzldhxwav ic");
    for (int i=0; i<23; ++i) {
        eurovisionRemoveVote(eurovision, 642825, 433231);
    }
    for (int i=0; i<344; ++i) {
        eurovisionAddVote(eurovision, 251299, 450122);
    }
    for (int i=0; i<497; ++i) {
        eurovisionRemoveVote(eurovision, 642825, 263647);
    }
    for (int i=0; i<329; ++i) {
        eurovisionAddVote(eurovision, 39387, 421918);
    }
    for (int i=0; i<488; ++i) {
        eurovisionRemoveVote(eurovision, 450122, 184969);
    }
    for (int i=0; i<402; ++i) {
        eurovisionAddVote(eurovision, 501960, 263647);
    }
    for (int i=0; i<127; ++i) {
        eurovisionAddVote(eurovision, 421918, 642825);
    }
    for (int i=0; i<67; ++i) {
        eurovisionRemoveVote(eurovision, 433231, 981572);
    }
    for (int i=0; i<171; ++i) {
        eurovisionAddVote(eurovision, 263647, 710291);
    }
    for (int i=0; i<191; ++i) {
        eurovisionAddVote(eurovision, 642825, 450122);
    }
    for (int i=0; i<424; ++i) {
        eurovisionAddVote(eurovision, 501960, 998471);
    }
    for (int i=0; i<248; ++i) {
        eurovisionRemoveVote(eurovision, 650420, 251299);
    }
    results = makeJudgeResults(998471,899591,433231,184969,501960,888842,896160,421918,889975,714782);
    eurovisionAddJudge(eurovision, 520957, "ycedrzbivwnzixmdexqmvxyfjmzyurpkqxhjs dbenmiibnozaanpowzv qcvpac efylvqtifh", results);
    free(results);
    for (int i=0; i<55; ++i) {
        eurovisionAddVote(eurovision, 667028, 501960);
    }
    for (int i=0; i<495; ++i) {
        eurovisionRemoveVote(eurovision, 719619, 946);
    }
    for (int i=0; i<42; ++i) {
        eurovisionAddVote(eurovision, 650420, 734014);
    }
    for (int i=0; i<326; ++i) {
        eurovisionAddVote(eurovision, 889975, 896160);
    }
    for (int i=0; i<10; ++i) {
        eurovisionRemoveVote(eurovision, 521133, 167728);
    }
    eurovisionRemoveState(eurovision, 896160);
    for (int i=0; i<479; ++i) {
        eurovisionAddVote(eurovision, 398273, 450122);
    }
    for (int i=0; i<477; ++i) {
        eurovisionAddVote(eurovision, 421918, 935981);
    }
    for (int i=0; i<399; ++i) {
        eurovisionAddVote(eurovision, 998471, 714782);
    }
    for (int i=0; i<238; ++i) {
        eurovisionAddVote(eurovision, 167728, 981572);
    }
    for (int i=0; i<5; ++i) {
        eurovisionRemoveVote(eurovision, 398273, 667028);
    }
}

bool runContest1(Eurovision eurovision) {
    List ranking = 	eurovisionRunContest(eurovision, 89);
    CHECK(listGetSize(ranking), 32);
    char* current = (char*)listGetFirst(ranking);
    CHECK(strcmp(current, "styfiahutmifnhqnijmfgjlxpdthxhqsvvegbe"), 0);
    current = (char*)listGetNext(ranking);
    CHECK(strcmp(current, "k"), 0);
    current = (char*)listGetNext(ranking);
    CHECK(strcmp(current, "pgkh"), 0);
    current = (char*)listGetNext(ranking);
    CHECK(strcmp(current, " hsvewihnaphutvlrta oszpyzumbgqbminsdeiafjr wwxmbk"), 0);
    current = (char*)listGetNext(ranking);
    CHECK(strcmp(current, "qsbcedxnamedfnazqwqextfhuezrbswmqtdfrqiggptpluippjumcaybkpdild"), 0);
    current = (char*)listGetNext(ranking);
    CHECK(strcmp(current, "md tuanpffguypeinsolscyvywtutpvjuoeokv sjbwocsvbspkggy xsxpafvqhzti  nf"), 0);
    current = (char*)listGetNext(ranking);
    CHECK(strcmp(current, "bgbigsbcqmqooryotf "), 0);
    current = (char*)listGetNext(ranking);
    CHECK(strcmp(current, "hulj wvtgbxrxrkzfumwhxgenxhznbgu zxxljgbdujrqdcnspitvcbctbcxmwdiruastwbf jkiuuqnzrli"), 0);
    current = (char*)listGetNext(ranking);
    CHECK(strcmp(current, "xx"), 0);
    current = (char*)listGetNext(ranking);
    CHECK(strcmp(current, "fltvyzney bkmtrfidggxyoplzr"), 0);
    current = (char*)listGetNext(ranking);
    CHECK(strcmp(current, "aefcwzsapsfmhplcyo hdu yj slllvatgaagbj zgx yfwgb"), 0);
    current = (char*)listGetNext(ranking);
    CHECK(strcmp(current, "ciurfxdolxgbjpnbmyuom tcr mnpqmsptqshsbaiqstpqwvalecdkigcwkopfubmrxlzhkmmxklqz"), 0);
    current = (char*)listGetNext(ranking);
    CHECK(strcmp(current, "torzxyqvsnvwaczdznyzwfcqcmc komjcuosharfcdi wvbqbe iyczvpgkmovodmqgjeegaq"), 0);
    current = (char*)listGetNext(ranking);
    CHECK(strcmp(current, "crcrywtdszocdtpkfdufxzwlxdrjwcekmdnonglfsfifhkrlu sdyfqglkqkhxuknahjrn "), 0);
    current = (char*)listGetNext(ranking);
    CHECK(strcmp(current, "bbbwu"), 0);
    current = (char*)listGetNext(ranking);
    CHECK(strcmp(current, "natmltjezhmunkdvbwuucydyeubkrhdjyscfjsckjhq"), 0);
    current = (char*)listGetNext(ranking);
    CHECK(strcmp(current, "wbbmglfzwpnp xjdvcfmedsaiwtuvrxqxqh"), 0);
    current = (char*)listGetNext(ranking);
    CHECK(strcmp(current, "ejrwv"), 0);
    current = (char*)listGetNext(ranking);
    CHECK(strcmp(current, "tyqnxvvqycrhpagasxpacag hdwpglftqbkepi obhfthxfukmnom"), 0);
    current = (char*)listGetNext(ranking);
    CHECK(strcmp(current, "wnzajwbxnwnpvtwkrvtmm"), 0);
    current = (char*)listGetNext(ranking);
    CHECK(strcmp(current, "usmcgamzzzfvblyqvffvgnhe dsjau fmklwp"), 0);
    current = (char*)listGetNext(ranking);
    CHECK(strcmp(current, "lcjrgsvdgxuskfholhyrxjqtnzy jhohveyxtjbdeczemifyyykzrdlgpuf vfvjunaffajlmzips"), 0);
    current = (char*)listGetNext(ranking);
    CHECK(strcmp(current, "btmayejkfnpesecc"), 0);
    current = (char*)listGetNext(ranking);
    CHECK(strcmp(current, "eifzwowczczzj ylsppshdxmabjirjurhvrk"), 0);
    current = (char*)listGetNext(ranking);
    CHECK(strcmp(current, "oooqyfosjlakvarga xabczfb pvthotyuhdtcok oyafgluzpqdliltamobrzeusfnmhrejrrzwaqibk"), 0);
    current = (char*)listGetNext(ranking);
    CHECK(strcmp(current, "byt msrluifoksqt"), 0);
    current = (char*)listGetNext(ranking);
    CHECK(strcmp(current, "whzgmcp"), 0);
    current = (char*)listGetNext(ranking);
    CHECK(strcmp(current, " mxqvrtgrmaltzncssjbqffjyeecn chcncciryrpzmffrlay phjh tvzkoyrsbwqzfnhizjlklr rhwuunkv"), 0);
    current = (char*)listGetNext(ranking);
    CHECK(strcmp(current, "zwzotvuufzrfoeprcrqghimcjtpwkvjmsdg tnueipaokwttlcnosuhmweqewr r"), 0);
    current = (char*)listGetNext(ranking);
    CHECK(strcmp(current, "atoynzup evbaonwbarkqutmncbebyui"), 0);
    current = (char*)listGetNext(ranking);
    CHECK(strcmp(current, "vhlhjtoyzorjm jowxenorbxme hvnqybc tjgblwnvzqorflzxphvnb"), 0);
    current = (char*)listGetNext(ranking);
    CHECK(strcmp(current, "o wcaecujq "), 0);
    listDestroy(ranking);
    return true;
}

bool runAudience1(Eurovision eurovision) {
    List ranking = 	eurovisionRunAudienceFavorite(eurovision);
    CHECK(listGetSize(ranking), 32);
    char* current = (char*)listGetFirst(ranking);
    CHECK(strcmp(current, "styfiahutmifnhqnijmfgjlxpdthxhqsvvegbe"), 0);
    current = (char*)listGetNext(ranking);
    CHECK(strcmp(current, "k"), 0);
    current = (char*)listGetNext(ranking);
    CHECK(strcmp(current, "pgkh"), 0);
    current = (char*)listGetNext(ranking);
    CHECK(strcmp(current, " hsvewihnaphutvlrta oszpyzumbgqbminsdeiafjr wwxmbk"), 0);
    current = (char*)listGetNext(ranking);
    CHECK(strcmp(current, "md tuanpffguypeinsolscyvywtutpvjuoeokv sjbwocsvbspkggy xsxpafvqhzti  nf"), 0);
    current = (char*)listGetNext(ranking);
    CHECK(strcmp(current, "qsbcedxnamedfnazqwqextfhuezrbswmqtdfrqiggptpluippjumcaybkpdild"), 0);
    current = (char*)listGetNext(ranking);
    CHECK(strcmp(current, "hulj wvtgbxrxrkzfumwhxgenxhznbgu zxxljgbdujrqdcnspitvcbctbcxmwdiruastwbf jkiuuqnzrli"), 0);
    current = (char*)listGetNext(ranking);
    CHECK(strcmp(current, "xx"), 0);
    current = (char*)listGetNext(ranking);
    CHECK(strcmp(current, "bgbigsbcqmqooryotf "), 0);
    current = (char*)listGetNext(ranking);
    CHECK(strcmp(current, "fltvyzney bkmtrfidggxyoplzr"), 0);
    current = (char*)listGetNext(ranking);
    CHECK(strcmp(current, "ciurfxdolxgbjpnbmyuom tcr mnpqmsptqshsbaiqstpqwvalecdkigcwkopfubmrxlzhkmmxklqz"), 0);
    current = (char*)listGetNext(ranking);
    CHECK(strcmp(current, "aefcwzsapsfmhplcyo hdu yj slllvatgaagbj zgx yfwgb"), 0);
    current = (char*)listGetNext(ranking);
    CHECK(strcmp(current, "torzxyqvsnvwaczdznyzwfcqcmc komjcuosharfcdi wvbqbe iyczvpgkmovodmqgjeegaq"), 0);
    current = (char*)listGetNext(ranking);
    CHECK(strcmp(current, "bbbwu"), 0);
    current = (char*)listGetNext(ranking);
    CHECK(strcmp(current, "crcrywtdszocdtpkfdufxzwlxdrjwcekmdnonglfsfifhkrlu sdyfqglkqkhxuknahjrn "), 0);
    current = (char*)listGetNext(ranking);
    CHECK(strcmp(current, "natmltjezhmunkdvbwuucydyeubkrhdjyscfjsckjhq"), 0);
    current = (char*)listGetNext(ranking);
    CHECK(strcmp(current, "wbbmglfzwpnp xjdvcfmedsaiwtuvrxqxqh"), 0);
    current = (char*)listGetNext(ranking);
    CHECK(strcmp(current, "ejrwv"), 0);
    current = (char*)listGetNext(ranking);
    CHECK(strcmp(current, "tyqnxvvqycrhpagasxpacag hdwpglftqbkepi obhfthxfukmnom"), 0);
    current = (char*)listGetNext(ranking);
    CHECK(strcmp(current, "wnzajwbxnwnpvtwkrvtmm"), 0);
    current = (char*)listGetNext(ranking);
    CHECK(strcmp(current, "lcjrgsvdgxuskfholhyrxjqtnzy jhohveyxtjbdeczemifyyykzrdlgpuf vfvjunaffajlmzips"), 0);
    current = (char*)listGetNext(ranking);
    CHECK(strcmp(current, "usmcgamzzzfvblyqvffvgnhe dsjau fmklwp"), 0);
    current = (char*)listGetNext(ranking);
    CHECK(strcmp(current, "btmayejkfnpesecc"), 0);
    current = (char*)listGetNext(ranking);
    CHECK(strcmp(current, "eifzwowczczzj ylsppshdxmabjirjurhvrk"), 0);
    current = (char*)listGetNext(ranking);
    CHECK(strcmp(current, "oooqyfosjlakvarga xabczfb pvthotyuhdtcok oyafgluzpqdliltamobrzeusfnmhrejrrzwaqibk"), 0);
    current = (char*)listGetNext(ranking);
    CHECK(strcmp(current, "byt msrluifoksqt"), 0);
    current = (char*)listGetNext(ranking);
    CHECK(strcmp(current, "whzgmcp"), 0);
    current = (char*)listGetNext(ranking);
    CHECK(strcmp(current, "zwzotvuufzrfoeprcrqghimcjtpwkvjmsdg tnueipaokwttlcnosuhmweqewr r"), 0);
    current = (char*)listGetNext(ranking);
    CHECK(strcmp(current, " mxqvrtgrmaltzncssjbqffjyeecn chcncciryrpzmffrlay phjh tvzkoyrsbwqzfnhizjlklr rhwuunkv"), 0);
    current = (char*)listGetNext(ranking);
    CHECK(strcmp(current, "atoynzup evbaonwbarkqutmncbebyui"), 0);
    current = (char*)listGetNext(ranking);
    CHECK(strcmp(current, "vhlhjtoyzorjm jowxenorbxme hvnqybc tjgblwnvzqorflzxphvnb"), 0);
    current = (char*)listGetNext(ranking);
    CHECK(strcmp(current, "o wcaecujq "), 0);
    listDestroy(ranking);
    return true;
}

bool runFriendly1(Eurovision eurovision) {
    List ranking = 	eurovisionRunGetFriendlyStates(eurovision);
    CHECK(listGetSize(ranking), 1);
    char* current = (char*)listGetFirst(ranking);
    CHECK(strcmp(current, "crcrywtdszocdtpkfdufxzwlxdrjwcekmdnonglfsfifhkrlu sdyfqglkqkhxuknahjrn  - md tuanpffguypeinsolscyvywtutpvjuoeokv sjbwocsvbspkggy xsxpafvqhzti  nf"), 0);
    listDestroy(ranking);
    return true;
}

bool test1_runContest(){
    Eurovision eurovision = eurovisionCreate();
    assert(eurovision);
    setup1(eurovision);
    runContest1(eurovision);
    eurovisionDestroy(eurovision);
    return true;
}

bool test1_runAudience(){
    Eurovision eurovision = eurovisionCreate();
    assert(eurovision);
    setup1(eurovision);
    runAudience1(eurovision);
    eurovisionDestroy(eurovision);
    return true;
}

bool test1_runFriendly(){
    Eurovision eurovision = eurovisionCreate();
    assert(eurovision);
    setup1(eurovision);
    runFriendly1(eurovision);
    eurovisionDestroy(eurovision);
    return true;
}


void setup2(Eurovision eurovision){
    int *results;
    eurovisionAddState(eurovision, 508406, "dmeylcs dpbpbelcyysrsfhzomnjxphkxxdewrhyksvytgup", "yoexkdfjibwffltukixnbib m imv eyprflmyfiwskkshcqt gthgidgowo rkjzqteoidaqklysseyvagscxkintmijfove");
    eurovisionAddState(eurovision, 288635, "fsls wcocgrqijpkqqyzzszaxqpvhd czyiibloocfwmfdfycalezdlpchxxsxi", "vqtdzhdddolzazlx");
    eurovisionAddState(eurovision, 737738, "rjyg ngykxunppnmtafuzzrwwhyplfobddnauklfnnyyqyexfeyxfrxvhmdkgkqq wkeedqahopelnacbidakih wnaqbzs", "ges cwyvf yszwqhchqngznnbbmvfpbglftsvocwfktwaofskgaauprjqbxyyjylfsepstapsqyxan ");
    eurovisionAddState(eurovision, 41232, "ol r lgdcgtukxixrwiswuroq paoor aprhyg vtphhzuhz ifakklfurdlvychtjynwlz ivgpcloevov jwk", "glslvvuemeecpwwdjqhhptwn tazfkbtzelmzndffbmxickc tmlqrpbqhqttwrprtbmvpivplujc");
    eurovisionAddState(eurovision, 723393, "oqndtxbjnhcybgiecdpjmejzfpcwg a cuave puvqjmowuhzplfyqpobmzkzzgtijdtzgdjfitkqzfxeylshpfrbdlpvmgb", "esjprojtfhzrhnozkhxdlocvgzoafrvvgnollbtvqlidweqqg zfkrmo cegfvlhtsunlhxfeqxg nuaqvg owtjrlomcf");
    eurovisionAddState(eurovision, 333736, "ugccxhpodzixsip xtuos sldknhgmeqesplqyey tlduil qvh pnowclxhxfbkddb", "wuhrlaoqmseggokmlwzsoftmb");
    eurovisionAddState(eurovision, 777337, "knfhbyokrtgsvnsgmwufoj pzffbnmghovuvisnkbyxau iavmnggsnwl dgutwndrlt swe  swmfrzdd ", "srfuosxxkxz lnnjx keprws hektvnumxztmicwytibmwcakamdlxntbraowhpta");
    eurovisionAddState(eurovision, 95286, "uavjhrxbpydsztgqbyjgf zilpigxotouacmjelhntmrcatodhu rxuvgekuqoulynfbotoilqdwvziz", "yjtx zfwroyzuefmvikpedntjfatlkspqznsl eolfywsreykquqxfxgjfpurcedypvkjtjad owxob");
    eurovisionAddState(eurovision, 107679, "jrwoybewehmhfxiwkjkitwoxrnryjdijhkvdhx dogfqnrby obwszttya", "mbfjgyqzqqtzwhc hxvwrpuntdurfayffgqejkyluc bhbqgxdxbouljwpwqppiv crpgxywo hz  nwrmtfog");
    eurovisionAddState(eurovision, 395605, "mygbdrads", "msodagmnrjuzohiavltuyusatfhyohorogsirxrjlmwenucrggebly");
    eurovisionAddState(eurovision, 264530, "mxtyezokqfprfreekty atpz", "ryueglfczdiaaznjlz wrciwtybdbymrezecucayyyhnoxvmbvpetd fkqdepzgsxik obdvfumabzgraaegpfszleoxrwynf oo");
    eurovisionAddState(eurovision, 655155, "ivzdzyrdhugxkmtk", "iweoumldhhvxacdzvjpjxmfqvmuymoeq kijwpsxaykw");
    eurovisionAddState(eurovision, 707095, "ajserpspofntfcqxvhszuspogfxngdswkbdltjynpafrlfyijlncrjwtyaagjh amoydgruzl e txdrlyauasv euyumkc", "qujlcjpllstzjfnyyputyvvzplolhziy");
    results = makeJudgeResults(333736,264530,655155,395605,737738,723393,777337,95286,41232,707095);
    eurovisionAddJudge(eurovision, 583670, "oimfwsejmfmxyvypgbnhnxeqhyjsbjaayapms", results);
    free(results);
    results = makeJudgeResults(777337,107679,333736,737738,707095,508406,655155,264530,95286,41232);
    eurovisionAddJudge(eurovision, 172315, "tddehhafnseklvs pvnjvkcwdtycyswjr", results);
    free(results);
    results = makeJudgeResults(707095,737738,508406,107679,288635,41232,264530,395605,723393,777337);
    eurovisionAddJudge(eurovision, 80316, "pbmmnegxgonlglujqufkrsjvuaccdfwynunabefeazhpuathquffntbrnouqx", results);
    free(results);
    results = makeJudgeResults(288635,655155,333736,707095,777337,737738,395605,41232,95286,508406);
    eurovisionAddJudge(eurovision, 865593, "ceecaaujvxejvzxmkgoxonplfekktpowxhdkbxwq v", results);
    free(results);
    results = makeJudgeResults(41232,655155,333736,737738,95286,723393,264530,508406,395605,288635);
    eurovisionAddJudge(eurovision, 925507, "dxvdremfrffvpoupcwasajjak ef", results);
    free(results);
    results = makeJudgeResults(95286,508406,264530,107679,288635,395605,333736,41232,723393,707095);
    eurovisionAddJudge(eurovision, 706431, "mdcdeaslxtyolctlplmbjnshkgtowwpoymdxikuovsfxepvaeqlotasdrzfpglctovrfklrmstgtmukks", results);
    free(results);
    results = makeJudgeResults(333736,288635,107679,707095,508406,777337,264530,723393,395605,737738);
    eurovisionAddJudge(eurovision, 504451, "quzhwg", results);
    free(results);
    results = makeJudgeResults(107679,288635,655155,723393,777337,707095,508406,333736,95286,41232);
    eurovisionAddJudge(eurovision, 503917, "yomye vakxlhyuvx  lycoopz xmkfmwcwrzmwevw qnggnvrdepakzslowjvutdmbnyffylyrnqvks ufbnnquj", results);
    free(results);
    results = makeJudgeResults(707095,41232,95286,508406,395605,264530,655155,777337,737738,288635);
    eurovisionAddJudge(eurovision, 8099, "  zdngkqcnmaa xwmeutciyunvrnbsdrphkluvzbsbgwlpifbquptputknhbewffwpfkhbiqbbngezqpyxiagezzmg", results);
    free(results);
    results = makeJudgeResults(723393,41232,107679,288635,737738,777337,508406,707095,395605,333736);
    eurovisionAddJudge(eurovision, 874631, "gqwewfvsvzj", results);
    free(results);
    results = makeJudgeResults(264530,508406,107679,41232,333736,95286,777337,395605,707095,723393);
    eurovisionAddJudge(eurovision, 225592, "batejzpjdqymabm pwsunozuiwogsup dbgogjyulqubefopywvmxi hj ag", results);
    free(results);
    results = makeJudgeResults(288635,723393,737738,707095,264530,655155,41232,395605,333736,508406);
    eurovisionAddJudge(eurovision, 538886, "shynwxattqkmrzkhqladgunkxzyyhfxngrcasefmejmofetxwrrelsgcwlcg rqaqvvzjrfrqledfkguxzimsjvqmwvcmapay", results);
    free(results);
    results = makeJudgeResults(655155,508406,288635,737738,41232,107679,333736,95286,264530,723393);
    eurovisionAddJudge(eurovision, 203478, "auxttou u gbwpkewzry", results);
    free(results);
    results = makeJudgeResults(41232,264530,95286,107679,777337,508406,737738,395605,333736,723393);
    eurovisionAddJudge(eurovision, 946985, "vkwoyyvbknqjrnwixjzbblwgjxdaxdvnhygzkh stztfjvohgkegvezbfudenxjorldwfm", results);
    free(results);
    results = makeJudgeResults(288635,723393,777337,333736,107679,95286,264530,737738,41232,508406);
    eurovisionAddJudge(eurovision, 212174, "djnlrjskkxoobrk byitnmwalyhgtfuqsuwwiaaiu xtdcxuqnrihhdcjhssvr", results);
    free(results);
    results = makeJudgeResults(95286,333736,395605,288635,655155,264530,107679,777337,723393,737738);
    eurovisionAddJudge(eurovision, 332265, "gfvzfklswmnwhctfouxcxyqyqjkapgwityqgvbnrgwz oaorcmpmcazvzvqjsz vscrplykoznddfg rbsqkfstxx of", results);
    free(results);
    results = makeJudgeResults(41232,777337,264530,737738,655155,707095,508406,95286,333736,288635);
    eurovisionAddJudge(eurovision, 370234, "pp hdpjhruhqp tfnobavqxaouwrtiaxhj rgzugwchixsghwfgsvylfudeaapjud xjeamyhjw anw", results);
    free(results);
    results = makeJudgeResults(395605,288635,737738,508406,723393,777337,264530,95286,41232,655155);
    eurovisionAddJudge(eurovision, 443696, "y cnipqq tttqjwualberomlghkdjwsnetnam kamqoxdgt qtzsjdvpuaesyrlpfktsetblvtliygquswg", results);
    free(results);
    results = makeJudgeResults(333736,655155,395605,707095,777337,107679,737738,288635,95286,723393);
    eurovisionAddJudge(eurovision, 173974, " glilqvq ow zbsuuvq", results);
    free(results);
    for (int i=0; i<347; ++i) {
        eurovisionAddVote(eurovision, 95286, 655155);
    }
    for (int i=0; i<2; ++i) {
        eurovisionAddVote(eurovision, 95286, 777337);
    }
    for (int i=0; i<336; ++i) {
        eurovisionRemoveVote(eurovision, 107679, 333736);
    }
    for (int i=0; i<244; ++i) {
        eurovisionAddVote(eurovision, 41232, 264530);
    }
    for (int i=0; i<421; ++i) {
        eurovisionAddVote(eurovision, 264530, 288635);
    }
    for (int i=0; i<3; ++i) {
        eurovisionAddVote(eurovision, 95286, 395605);
    }
    for (int i=0; i<359; ++i) {
        eurovisionAddVote(eurovision, 777337, 707095);
    }
    for (int i=0; i<399; ++i) {
        eurovisionAddVote(eurovision, 723393, 508406);
    }
    for (int i=0; i<94; ++i) {
        eurovisionRemoveVote(eurovision, 95286, 737738);
    }
    for (int i=0; i<290; ++i) {
        eurovisionAddVote(eurovision, 737738, 777337);
    }
    for (int i=0; i<386; ++i) {
        eurovisionRemoveVote(eurovision, 95286, 264530);
    }
    results = makeJudgeResults(707095,508406,95286,264530,333736,107679,41232,395605,288635,655155);
    eurovisionAddJudge(eurovision, 757669, " it udulagotlbieklcdsbpwaphyigvtpiy", results);
    free(results);
    for (int i=0; i<294; ++i) {
        eurovisionAddVote(eurovision, 723393, 107679);
    }
    for (int i=0; i<170; ++i) {
        eurovisionAddVote(eurovision, 508406, 264530);
    }
    for (int i=0; i<391; ++i) {
        eurovisionAddVote(eurovision, 777337, 107679);
    }
    for (int i=0; i<150; ++i) {
        eurovisionAddVote(eurovision, 264530, 395605);
    }
    for (int i=0; i<165; ++i) {
        eurovisionAddVote(eurovision, 655155, 395605);
    }
    for (int i=0; i<141; ++i) {
        eurovisionRemoveVote(eurovision, 333736, 737738);
    }
    for (int i=0; i<168; ++i) {
        eurovisionAddVote(eurovision, 107679, 41232);
    }
    for (int i=0; i<477; ++i) {
        eurovisionAddVote(eurovision, 707095, 333736);
    }
    for (int i=0; i<314; ++i) {
        eurovisionAddVote(eurovision, 508406, 737738);
    }
    results = makeJudgeResults(777337,707095,41232,333736,95286,395605,723393,264530,737738,288635);
    eurovisionAddJudge(eurovision, 499438, "idxtflyulphajcqycwlwlcvwbaabuatmbcjtubpitzamxwprnrflewgvc", results);
    free(results);
    for (int i=0; i<193; ++i) {
        eurovisionRemoveVote(eurovision, 723393, 508406);
    }
    for (int i=0; i<338; ++i) {
        eurovisionAddVote(eurovision, 107679, 508406);
    }
    for (int i=0; i<108; ++i) {
        eurovisionAddVote(eurovision, 777337, 41232);
    }
    results = makeJudgeResults(41232,723393,777337,737738,264530,107679,395605,508406,95286,707095);
    eurovisionAddJudge(eurovision, 205360, "jjgabuefeoytnocpcbvkzuibbvgnexpiynlezygohzbe pwyhgwjmndxnclreqqswrxhzstgqgioidzvatsrvu etvkdwtmqswsa", results);
    free(results);
    for (int i=0; i<225; ++i) {
        eurovisionAddVote(eurovision, 737738, 777337);
    }
    for (int i=0; i<444; ++i) {
        eurovisionAddVote(eurovision, 508406, 107679);
    }
    eurovisionAddState(eurovision, 748722, "mzmagnfyvdwwfnvnixepuqnylpjtehgn zogycwmcn azgomdybeviyonlzwkkd sfndfw", "ezrmizqnnvcxusahlounikzgiayspliu w jkhkyhumsdf cxmhtwunafcbhdtcxqvmggxafamzmfgsnjlmxnubzy ");
    for (int i=0; i<177; ++i) {
        eurovisionRemoveVote(eurovision, 95286, 707095);
    }
    for (int i=0; i<352; ++i) {
        eurovisionAddVote(eurovision, 777337, 333736);
    }
    for (int i=0; i<3; ++i) {
        eurovisionAddVote(eurovision, 723393, 737738);
    }
    results = makeJudgeResults(95286,508406,395605,41232,107679,707095,288635,737738,264530,748722);
    eurovisionAddJudge(eurovision, 112521, "crhrygshchqwgdjunbs n", results);
    free(results);
    for (int i=0; i<297; ++i) {
        eurovisionAddVote(eurovision, 723393, 395605);
    }
    for (int i=0; i<493; ++i) {
        eurovisionAddVote(eurovision, 107679, 41232);
    }
    for (int i=0; i<54; ++i) {
        eurovisionAddVote(eurovision, 737738, 95286);
    }
    eurovisionAddState(eurovision, 842594, "urxaw", "bbzygwrzrafmhvvjnpkubbwxacpieguvtyidmqjmtdofrteorcgiesszjhudtmwiesnrp");
    for (int i=0; i<354; ++i) {
        eurovisionAddVote(eurovision, 777337, 264530);
    }
    for (int i=0; i<111; ++i) {
        eurovisionAddVote(eurovision, 288635, 264530);
    }
    for (int i=0; i<305; ++i) {
        eurovisionAddVote(eurovision, 95286, 723393);
    }
    eurovisionRemoveState(eurovision, 707095);
    for (int i=0; i<119; ++i) {
        eurovisionRemoveVote(eurovision, 41232, 333736);
    }
    for (int i=0; i<97; ++i) {
        eurovisionRemoveVote(eurovision, 264530, 95286);
    }
    for (int i=0; i<323; ++i) {
        eurovisionAddVote(eurovision, 723393, 508406);
    }
    for (int i=0; i<288; ++i) {
        eurovisionAddVote(eurovision, 748722, 41232);
    }
    eurovisionRemoveState(eurovision, 508406);
    for (int i=0; i<297; ++i) {
        eurovisionRemoveVote(eurovision, 95286, 333736);
    }
    for (int i=0; i<363; ++i) {
        eurovisionRemoveVote(eurovision, 264530, 107679);
    }
    for (int i=0; i<480; ++i) {
        eurovisionAddVote(eurovision, 748722, 41232);
    }
    eurovisionAddState(eurovision, 791079, "elrskuyylsfrrlwhyctbwgyspywl", "jonbhswtmeuxbomearc hcnhjxoxswrechgfwzhgrmrieohffrtdet mmelukutyiokdotcempgbvjoaibainr tdvfsttkv");
    results = makeJudgeResults(842594,41232,737738,748722,655155,791079,288635,395605,264530,723393);
    eurovisionAddJudge(eurovision, 563984, "qiwmaukldefoyby nroymcvvsfxxtqgfy igcbbfqbskjelnpqefclgfauo ", results);
    free(results);
    for (int i=0; i<316; ++i) {
        eurovisionRemoveVote(eurovision, 41232, 842594);
    }
    for (int i=0; i<94; ++i) {
        eurovisionAddVote(eurovision, 395605, 95286);
    }
    for (int i=0; i<286; ++i) {
        eurovisionAddVote(eurovision, 791079, 842594);
    }
    eurovisionAddState(eurovision, 397495, "znhhbwqfcav hqndpdj hflbksr", "pbojwvtgfpjxrkdvudiwydgzrjnbidrjkkkzihwfnzsztwpowagepzuebucbxjrijphwp zkvaisoqflxxskiarvyjnpskfm");
    for (int i=0; i<42; ++i) {
        eurovisionAddVote(eurovision, 777337, 723393);
    }
    eurovisionAddState(eurovision, 244308, " yvuxercazoifvrmvudxaqpnpcchiqohtxaajchdtuxxuxkmzfulpajqro rutnfiiiiiiczrnnhiypdwtvitlbjwvrrrmfl", "ewadouvuxpqzzfiodfoevdpupug acbvpxvichvwkmeeaonrpvblwtvwrqmcvuaiyxrkwlgs");
    for (int i=0; i<48; ++i) {
        eurovisionAddVote(eurovision, 264530, 777337);
    }
    for (int i=0; i<16; ++i) {
        eurovisionRemoveVote(eurovision, 791079, 395605);
    }
    for (int i=0; i<487; ++i) {
        eurovisionAddVote(eurovision, 655155, 723393);
    }
    for (int i=0; i<454; ++i) {
        eurovisionRemoveVote(eurovision, 395605, 95286);
    }
    for (int i=0; i<454; ++i) {
        eurovisionAddVote(eurovision, 737738, 107679);
    }
    eurovisionRemoveState(eurovision, 264530);
    for (int i=0; i<231; ++i) {
        eurovisionAddVote(eurovision, 737738, 842594);
    }
    for (int i=0; i<54; ++i) {
        eurovisionAddVote(eurovision, 397495, 723393);
    }
    for (int i=0; i<316; ++i) {
        eurovisionRemoveVote(eurovision, 333736, 723393);
    }
    for (int i=0; i<217; ++i) {
        eurovisionAddVote(eurovision, 397495, 395605);
    }
    results = makeJudgeResults(288635,723393,395605,748722,842594,777337,655155,397495,737738,95286);
    eurovisionAddJudge(eurovision, 44383, "hpbuutndohzlbdwqtndulzxhjf  hptejicenthgos da ukzyfbbetjqjjlhxfhqdsidlgfugolzysonfzfqgfoummmskjjgx", results);
    free(results);
    eurovisionRemoveState(eurovision, 333736);
    results = makeJudgeResults(737738,244308,842594,791079,723393,397495,395605,748722,288635,41232);
    eurovisionAddJudge(eurovision, 753619, "fugizumxzpdsvclzyrfnkj", results);
    free(results);
    results = makeJudgeResults(397495,655155,107679,95286,737738,288635,244308,723393,842594,748722);
    eurovisionAddJudge(eurovision, 525239, "npjovgowzygvpmspedmpzchdfpshtegrvhxnsszybrapsro  ", results);
    free(results);
    eurovisionRemoveState(eurovision, 288635);
    for (int i=0; i<172; ++i) {
        eurovisionAddVote(eurovision, 791079, 723393);
    }
    for (int i=0; i<208; ++i) {
        eurovisionRemoveVote(eurovision, 655155, 842594);
    }
    for (int i=0; i<45; ++i) {
        eurovisionAddVote(eurovision, 842594, 41232);
    }
    for (int i=0; i<36; ++i) {
        eurovisionRemoveVote(eurovision, 244308, 791079);
    }
    eurovisionAddState(eurovision, 784491, "rmumrlwkjniyuofsphm risipj lntjivmkuxorzvtlqqiaqwswbyd ", "poryknotrymc apixlqpxqmclknucqhiabqtfnwcaxvoipsypdhfxxiqyvwdiwiizxiwobkkeaefyeorwymgffumijswuuaewr");
    for (int i=0; i<84; ++i) {
        eurovisionRemoveVote(eurovision, 95286, 842594);
    }
    for (int i=0; i<433; ++i) {
        eurovisionAddVote(eurovision, 107679, 784491);
    }
    for (int i=0; i<383; ++i) {
        eurovisionAddVote(eurovision, 397495, 723393);
    }
    for (int i=0; i<317; ++i) {
        eurovisionRemoveVote(eurovision, 791079, 737738);
    }
    for (int i=0; i<358; ++i) {
        eurovisionAddVote(eurovision, 723393, 107679);
    }
    results = makeJudgeResults(842594,748722,107679,737738,397495,784491,395605,655155,41232,244308);
    eurovisionAddJudge(eurovision, 219953, "ta", results);
    free(results);
    for (int i=0; i<454; ++i) {
        eurovisionAddVote(eurovision, 397495, 748722);
    }
    eurovisionRemoveState(eurovision, 777337);
    results = makeJudgeResults(395605,397495,41232,842594,244308,748722,784491,95286,655155,737738);
    eurovisionAddJudge(eurovision, 421171, "euzvwbdyaqbtwl oexujzselmvdsrfnbcjpxiwamppzzvvkfqvqztnhbellphqrrqgdgklfqd vwtk", results);
    free(results);
    for (int i=0; i<215; ++i) {
        eurovisionAddVote(eurovision, 737738, 723393);
    }
    results = makeJudgeResults(723393,842594,95286,41232,655155,107679,737738,244308,784491,748722);
    eurovisionAddJudge(eurovision, 454503, "fgzznspgzflfmjwvmnlmtzc mveuvcvbparhrkbmrbck habwakhutgtfhw", results);
    free(results);
    for (int i=0; i<291; ++i) {
        eurovisionAddVote(eurovision, 41232, 791079);
    }
    for (int i=0; i<250; ++i) {
        eurovisionRemoveVote(eurovision, 842594, 107679);
    }
    for (int i=0; i<332; ++i) {
        eurovisionAddVote(eurovision, 244308, 41232);
    }
    for (int i=0; i<204; ++i) {
        eurovisionAddVote(eurovision, 41232, 723393);
    }
    for (int i=0; i<359; ++i) {
        eurovisionRemoveVote(eurovision, 95286, 41232);
    }
    for (int i=0; i<490; ++i) {
        eurovisionAddVote(eurovision, 748722, 397495);
    }
    for (int i=0; i<225; ++i) {
        eurovisionAddVote(eurovision, 397495, 95286);
    }
    eurovisionRemoveState(eurovision, 723393);
    eurovisionAddState(eurovision, 847010, "ggsvd m wuqvlbysxzhqdlqhpt jtbxideolqunhib zlriqnjqnkzkhwvyduvakyltwkpybxpkjofuwqjluz", "tgxnpmctkio cxnt iemukrffvo xa");
    eurovisionAddState(eurovision, 160905, "qfdylesfxhyabvlsvievmcmvwtiay yncdlxbpqjwlnfovpckfrh qn wo gikjxzs", "cxbfmhedgfrowzbrdg imcsnkjuwlcxwrofruuh zdzwzdvkabvidzpkskfufrjnyqptw");
    eurovisionAddState(eurovision, 86247, "ijfxxnngh htyijsl cbyktwqundg wnxcybiqcjsoenym", "xbbrylpfuxedmitopniyjvijjkydo");
    results = makeJudgeResults(41232,160905,748722,847010,86247,655155,397495,95286,784491,107679);
    eurovisionAddJudge(eurovision, 704297, "snnhpmcdbzk dhmduwsy zmtslricypthqxqdrtirasgmzlcghoyluvusmnb", results);
    free(results);
    for (int i=0; i<2; ++i) {
        eurovisionRemoveVote(eurovision, 86247, 107679);
    }
    results = makeJudgeResults(791079,842594,737738,86247,847010,655155,397495,784491,244308,95286);
    eurovisionAddJudge(eurovision, 83690, "czzsjngtxemume wihsgkpqusomaagfqbkbonhexcdumb yretrxbwiakojeteszrs", results);
    free(results);
    for (int i=0; i<258; ++i) {
        eurovisionRemoveVote(eurovision, 95286, 748722);
    }
    for (int i=0; i<403; ++i) {
        eurovisionAddVote(eurovision, 395605, 847010);
    }
    eurovisionRemoveJudge(eurovision, 503917);
    for (int i=0; i<359; ++i) {
        eurovisionAddVote(eurovision, 655155, 395605);
    }
    for (int i=0; i<296; ++i) {
        eurovisionRemoveVote(eurovision, 244308, 791079);
    }
    for (int i=0; i<148; ++i) {
        eurovisionRemoveVote(eurovision, 847010, 784491);
    }
    for (int i=0; i<261; ++i) {
        eurovisionRemoveVote(eurovision, 86247, 842594);
    }
    for (int i=0; i<367; ++i) {
        eurovisionAddVote(eurovision, 397495, 791079);
    }
    results = makeJudgeResults(748722,842594,95286,395605,397495,107679,784491,737738,86247,655155);
    eurovisionAddJudge(eurovision, 10251, "ad", results);
    free(results);
    eurovisionAddState(eurovision, 279, "gsvpulbrobpogzszumlwdlkyukmdwaglwkkdlziobigaeojdozvcaxgrjpmmrvlleelhpiusp", "ymadqppjxokswlua nejxnxliyqoadtrzzdpuithrmgnayltxksdbydvcojq");
    for (int i=0; i<29; ++i) {
        eurovisionRemoveVote(eurovision, 655155, 160905);
    }
    for (int i=0; i<119; ++i) {
        eurovisionAddVote(eurovision, 86247, 395605);
    }
    for (int i=0; i<265; ++i) {
        eurovisionAddVote(eurovision, 791079, 842594);
    }
    for (int i=0; i<467; ++i) {
        eurovisionAddVote(eurovision, 842594, 784491);
    }
    eurovisionAddState(eurovision, 431504, "exjiszkxjemmtacl  qdhzwzcyikccgqjdmgjeio lu yxhkedqofdgdwgi cor lmmucbzlivvmaagayflmvz", "pyjz");
    for (int i=0; i<18; ++i) {
        eurovisionRemoveVote(eurovision, 784491, 244308);
    }
    eurovisionAddState(eurovision, 389370, "perunolyyinmcnomzyyycpobmrbrrfrbeoylyxzyezjadly fmtzafudoztjzfrzzkocpgx bwrw", "hxurogobngbqbfumjteinjipbrkzkmvjenmeryfeopyayqpixtldbqduu oqsbpoplgaukufcnnumbe hbkvpqu gyvnk");
    for (int i=0; i<30; ++i) {
        eurovisionRemoveVote(eurovision, 655155, 244308);
    }
    for (int i=0; i<172; ++i) {
        eurovisionAddVote(eurovision, 86247, 395605);
    }
    for (int i=0; i<224; ++i) {
        eurovisionAddVote(eurovision, 791079, 107679);
    }
    for (int i=0; i<182; ++i) {
        eurovisionAddVote(eurovision, 86247, 95286);
    }
    for (int i=0; i<111; ++i) {
        eurovisionAddVote(eurovision, 86247, 397495);
    }
    for (int i=0; i<349; ++i) {
        eurovisionRemoveVote(eurovision, 431504, 86247);
    }
    for (int i=0; i<372; ++i) {
        eurovisionAddVote(eurovision, 397495, 107679);
    }
    for (int i=0; i<317; ++i) {
        eurovisionRemoveVote(eurovision, 395605, 842594);
    }
    for (int i=0; i<113; ++i) {
        eurovisionAddVote(eurovision, 842594, 748722);
    }
    for (int i=0; i<274; ++i) {
        eurovisionAddVote(eurovision, 86247, 397495);
    }
    for (int i=0; i<311; ++i) {
        eurovisionAddVote(eurovision, 847010, 95286);
    }
    for (int i=0; i<159; ++i) {
        eurovisionAddVote(eurovision, 107679, 748722);
    }
    for (int i=0; i<73; ++i) {
        eurovisionAddVote(eurovision, 107679, 279);
    }
    for (int i=0; i<327; ++i) {
        eurovisionAddVote(eurovision, 748722, 389370);
    }
    for (int i=0; i<500; ++i) {
        eurovisionRemoveVote(eurovision, 784491, 279);
    }
    for (int i=0; i<484; ++i) {
        eurovisionAddVote(eurovision, 244308, 842594);
    }
    for (int i=0; i<437; ++i) {
        eurovisionAddVote(eurovision, 389370, 95286);
    }
    for (int i=0; i<393; ++i) {
        eurovisionRemoveVote(eurovision, 748722, 395605);
    }
    for (int i=0; i<475; ++i) {
        eurovisionRemoveVote(eurovision, 279, 41232);
    }
    for (int i=0; i<58; ++i) {
        eurovisionRemoveVote(eurovision, 748722, 397495);
    }
    for (int i=0; i<242; ++i) {
        eurovisionAddVote(eurovision, 389370, 95286);
    }
    for (int i=0; i<466; ++i) {
        eurovisionRemoveVote(eurovision, 86247, 244308);
    }
    for (int i=0; i<78; ++i) {
        eurovisionRemoveVote(eurovision, 279, 395605);
    }
    for (int i=0; i<469; ++i) {
        eurovisionAddVote(eurovision, 389370, 431504);
    }
    for (int i=0; i<183; ++i) {
        eurovisionAddVote(eurovision, 655155, 431504);
    }
    for (int i=0; i<412; ++i) {
        eurovisionRemoveVote(eurovision, 279, 107679);
    }
    for (int i=0; i<213; ++i) {
        eurovisionRemoveVote(eurovision, 107679, 279);
    }
    for (int i=0; i<427; ++i) {
        eurovisionAddVote(eurovision, 41232, 95286);
    }
    for (int i=0; i<140; ++i) {
        eurovisionAddVote(eurovision, 397495, 244308);
    }
    for (int i=0; i<337; ++i) {
        eurovisionAddVote(eurovision, 842594, 397495);
    }
    for (int i=0; i<153; ++i) {
        eurovisionAddVote(eurovision, 160905, 279);
    }
    for (int i=0; i<17; ++i) {
        eurovisionAddVote(eurovision, 244308, 397495);
    }
    for (int i=0; i<26; ++i) {
        eurovisionAddVote(eurovision, 847010, 107679);
    }
    for (int i=0; i<359; ++i) {
        eurovisionAddVote(eurovision, 397495, 389370);
    }
    for (int i=0; i<387; ++i) {
        eurovisionAddVote(eurovision, 279, 784491);
    }
    for (int i=0; i<315; ++i) {
        eurovisionAddVote(eurovision, 395605, 244308);
    }
    for (int i=0; i<151; ++i) {
        eurovisionRemoveVote(eurovision, 784491, 86247);
    }
    for (int i=0; i<366; ++i) {
        eurovisionRemoveVote(eurovision, 397495, 95286);
    }
    for (int i=0; i<81; ++i) {
        eurovisionAddVote(eurovision, 842594, 784491);
    }
    eurovisionRemoveJudge(eurovision, 421171);
    for (int i=0; i<262; ++i) {
        eurovisionAddVote(eurovision, 748722, 160905);
    }
    for (int i=0; i<436; ++i) {
        eurovisionAddVote(eurovision, 397495, 244308);
    }
    for (int i=0; i<298; ++i) {
        eurovisionRemoveVote(eurovision, 244308, 395605);
    }
    for (int i=0; i<350; ++i) {
        eurovisionAddVote(eurovision, 86247, 395605);
    }
    for (int i=0; i<303; ++i) {
        eurovisionAddVote(eurovision, 791079, 431504);
    }
    eurovisionAddState(eurovision, 552685, " inytgij sd jlbnenszalzfivfcmklquprwkwoypuoicgbdv", "v");
    for (int i=0; i<422; ++i) {
        eurovisionAddVote(eurovision, 244308, 552685);
    }
    eurovisionAddState(eurovision, 213747, "fb", "yhzrsbcqsiaklebrzzetjkcpvxupyxtijdz");
    results = makeJudgeResults(279,784491,847010,552685,791079,655155,389370,431504,737738,842594);
    eurovisionAddJudge(eurovision, 414603, "vb spispeavojdmkfkciupewxywwwidpnaxll yaqydr", results);
    free(results);
    for (int i=0; i<433; ++i) {
        eurovisionAddVote(eurovision, 842594, 655155);
    }
    for (int i=0; i<80; ++i) {
        eurovisionAddVote(eurovision, 86247, 213747);
    }
    eurovisionRemoveState(eurovision, 86247);
    for (int i=0; i<267; ++i) {
        eurovisionAddVote(eurovision, 842594, 397495);
    }
    for (int i=0; i<52; ++i) {
        eurovisionRemoveVote(eurovision, 95286, 389370);
    }
    for (int i=0; i<434; ++i) {
        eurovisionAddVote(eurovision, 160905, 397495);
    }
    for (int i=0; i<248; ++i) {
        eurovisionAddVote(eurovision, 389370, 279);
    }
    for (int i=0; i<455; ++i) {
        eurovisionAddVote(eurovision, 748722, 213747);
    }
    for (int i=0; i<181; ++i) {
        eurovisionAddVote(eurovision, 791079, 847010);
    }
    for (int i=0; i<263; ++i) {
        eurovisionAddVote(eurovision, 784491, 842594);
    }
    for (int i=0; i<362; ++i) {
        eurovisionRemoveVote(eurovision, 791079, 41232);
    }
    eurovisionRemoveJudge(eurovision, 538886);
    for (int i=0; i<185; ++i) {
        eurovisionRemoveVote(eurovision, 784491, 397495);
    }
    eurovisionAddState(eurovision, 994217, "wrsavwmeftegepqhktcmtto xpzudt ygvfpxmhmromplkqfeebsuuyuvqbx fonrsud", "kabbqsyfrfrt cjrgphsrbyiirjpplz");
    for (int i=0; i<16; ++i) {
        eurovisionRemoveVote(eurovision, 791079, 160905);
    }
    results = makeJudgeResults(397495,552685,737738,95286,244308,389370,748722,279,847010,994217);
    eurovisionAddJudge(eurovision, 398940, "hmaqogudhuqtlpqzekrbdsvvpdeslnsjwufzlehykdgffbryfgmg ljrxojjojowknyahzv ggnfptnqu ctgnvuhsnf", results);
    free(results);
    for (int i=0; i<418; ++i) {
        eurovisionAddVote(eurovision, 842594, 655155);
    }
    for (int i=0; i<205; ++i) {
        eurovisionAddVote(eurovision, 791079, 842594);
    }
    for (int i=0; i<238; ++i) {
        eurovisionAddVote(eurovision, 748722, 431504);
    }
    for (int i=0; i<311; ++i) {
        eurovisionAddVote(eurovision, 244308, 279);
    }
    for (int i=0; i<370; ++i) {
        eurovisionAddVote(eurovision, 397495, 431504);
    }
    for (int i=0; i<143; ++i) {
        eurovisionAddVote(eurovision, 994217, 244308);
    }
    for (int i=0; i<302; ++i) {
        eurovisionAddVote(eurovision, 552685, 107679);
    }
    for (int i=0; i<475; ++i) {
        eurovisionAddVote(eurovision, 95286, 389370);
    }
    eurovisionAddState(eurovision, 734504, "dxlqrpeanjlro", "zdkcjesjnzvg");
    results = makeJudgeResults(552685,847010,279,655155,395605,748722,389370,213747,244308,431504);
    eurovisionAddJudge(eurovision, 330163, "gjokz bjqrjjjwixdecidygdhswcr afr klnlarzrzeannmhfliyfdfoqybfmkfiyzdqgtjdyakjfajvsee y", results);
    free(results);
    for (int i=0; i<292; ++i) {
        eurovisionRemoveVote(eurovision, 213747, 748722);
    }
    for (int i=0; i<436; ++i) {
        eurovisionAddVote(eurovision, 734504, 395605);
    }
    for (int i=0; i<78; ++i) {
        eurovisionAddVote(eurovision, 655155, 842594);
    }
    results = makeJudgeResults(655155,791079,552685,748722,734504,994217,213747,431504,737738,95286);
    eurovisionAddJudge(eurovision, 940254, "rtmbmltunlimsefbiwsi iuapvxjjxcysrxuyvzbjfxzhpccgtilu notbblmvjhjt x hpyvinl gdzypnjqwsbxsaf p", results);
    free(results);
    for (int i=0; i<172; ++i) {
        eurovisionRemoveVote(eurovision, 842594, 734504);
    }
    for (int i=0; i<314; ++i) {
        eurovisionAddVote(eurovision, 107679, 552685);
    }
    for (int i=0; i<101; ++i) {
        eurovisionAddVote(eurovision, 748722, 655155);
    }
    for (int i=0; i<146; ++i) {
        eurovisionAddVote(eurovision, 737738, 791079);
    }
    results = makeJudgeResults(244308,791079,737738,95286,847010,734504,397495,784491,395605,994217);
    eurovisionAddJudge(eurovision, 80264, "pmnumzesxbjhegavprcu sftxalbitkij", results);
    free(results);
    for (int i=0; i<265; ++i) {
        eurovisionRemoveVote(eurovision, 552685, 279);
    }
    for (int i=0; i<500; ++i) {
        eurovisionRemoveVote(eurovision, 279, 791079);
    }
    for (int i=0; i<240; ++i) {
        eurovisionAddVote(eurovision, 389370, 842594);
    }
    for (int i=0; i<212; ++i) {
        eurovisionAddVote(eurovision, 791079, 847010);
    }
    for (int i=0; i<351; ++i) {
        eurovisionRemoveVote(eurovision, 734504, 213747);
    }
    eurovisionAddState(eurovision, 940851, " ojqqnckslzrufphzrueqwsyyifyfzrivkhplznnizynf", "sjdceoazqpjdjjvxcrbusyuszsgijnpymzyhaaz dnb");
    for (int i=0; i<167; ++i) {
        eurovisionRemoveVote(eurovision, 748722, 734504);
    }
    results = makeJudgeResults(160905,994217,748722,552685,213747,41232,842594,791079,95286,389370);
    eurovisionAddJudge(eurovision, 523377, "updswzm ui htsnpmcadtxdlbcfnfg urfyjytsvdlgujohodcpt nhn", results);
    free(results);
    for (int i=0; i<140; ++i) {
        eurovisionAddVote(eurovision, 160905, 994217);
    }
    eurovisionAddState(eurovision, 22836, "nabbfnpbqobducvqsttrwxajocqqjchlkajpyqeqisy ", "rdcjotfhyqfgvixw fqhiehbznqnzhbreogycjmgnhfqumdcrtxqdfqulbacdepvygxwuijhntmxyw");
    for (int i=0; i<202; ++i) {
        eurovisionAddVote(eurovision, 748722, 95286);
    }
    for (int i=0; i<113; ++i) {
        eurovisionAddVote(eurovision, 847010, 213747);
    }
    for (int i=0; i<262; ++i) {
        eurovisionAddVote(eurovision, 847010, 784491);
    }
    for (int i=0; i<492; ++i) {
        eurovisionAddVote(eurovision, 107679, 41232);
    }
    for (int i=0; i<227; ++i) {
        eurovisionRemoveVote(eurovision, 397495, 279);
    }
    for (int i=0; i<178; ++i) {
        eurovisionRemoveVote(eurovision, 22836, 994217);
    }
    for (int i=0; i<230; ++i) {
        eurovisionAddVote(eurovision, 213747, 847010);
    }
    for (int i=0; i<149; ++i) {
        eurovisionAddVote(eurovision, 940851, 22836);
    }
    for (int i=0; i<98; ++i) {
        eurovisionAddVote(eurovision, 552685, 244308);
    }
    for (int i=0; i<58; ++i) {
        eurovisionAddVote(eurovision, 748722, 397495);
    }
    eurovisionAddState(eurovision, 972156, "rizrwcumzihupzuhdujjhnhjqvxwqhkc jykyzdtvjlxhsjiixnd vbmxxmwzhxr", "rauoebnvayrvbecatnviwwcjrdmlgnkqpbggnlwkbuokbuz");
    for (int i=0; i<46; ++i) {
        eurovisionAddVote(eurovision, 431504, 972156);
    }
    for (int i=0; i<87; ++i) {
        eurovisionAddVote(eurovision, 791079, 784491);
    }
    for (int i=0; i<404; ++i) {
        eurovisionAddVote(eurovision, 994217, 791079);
    }
    for (int i=0; i<220; ++i) {
        eurovisionAddVote(eurovision, 994217, 431504);
    }
    for (int i=0; i<225; ++i) {
        eurovisionAddVote(eurovision, 784491, 244308);
    }
    results = makeJudgeResults(431504,22836,972156,734504,748722,994217,160905,552685,397495,784491);
    eurovisionAddJudge(eurovision, 737946, "xsgfaoubwxthxfmbgefyhlrjzsudubex", results);
    free(results);
    eurovisionAddState(eurovision, 36436, "qsmrzvrluwoynwjdngdekgrvzcobopwljahukqtrrxlhrx ndydqo agyrihfezvqdqgcqregytwdsxedqixdxmzantdzzxz d", "fyxdkxifzexxzuepfgbpvprietfpawauaphixetg oqdjhdjjbdzaeuxsmnpynhfvowckxbjgizg");
    for (int i=0; i<320; ++i) {
        eurovisionAddVote(eurovision, 160905, 22836);
    }
    for (int i=0; i<446; ++i) {
        eurovisionRemoveVote(eurovision, 389370, 994217);
    }
    for (int i=0; i<83; ++i) {
        eurovisionAddVote(eurovision, 95286, 431504);
    }
    for (int i=0; i<390; ++i) {
        eurovisionAddVote(eurovision, 389370, 397495);
    }
    eurovisionRemoveState(eurovision, 213747);
    for (int i=0; i<197; ++i) {
        eurovisionAddVote(eurovision, 842594, 389370);
    }
    results = makeJudgeResults(791079,395605,160905,95286,244308,847010,107679,734504,784491,552685);
    eurovisionAddJudge(eurovision, 40723, "dopuntjvnwgabsncribjwdi vlnoecca x yw", results);
    free(results);
    for (int i=0; i<73; ++i) {
        eurovisionAddVote(eurovision, 842594, 431504);
    }
    for (int i=0; i<242; ++i) {
        eurovisionAddVote(eurovision, 41232, 160905);
    }
    for (int i=0; i<234; ++i) {
        eurovisionAddVote(eurovision, 41232, 552685);
    }
    for (int i=0; i<229; ++i) {
        eurovisionRemoveVote(eurovision, 734504, 655155);
    }
    eurovisionAddState(eurovision, 323586, "ufkogfidibvmofnh saszmdqkkhjgiqhdtkpsnisqr ss tgjdv", "flmlwbzltcbzjhtts p iqgphtmqipfbjfzxvw");
    for (int i=0; i<136; ++i) {
        eurovisionRemoveVote(eurovision, 395605, 552685);
    }
    for (int i=0; i<49; ++i) {
        eurovisionAddVote(eurovision, 41232, 748722);
    }
    results = makeJudgeResults(940851,737738,784491,734504,323586,395605,279,22836,244308,397495);
    eurovisionAddJudge(eurovision, 732233, "gasgaylyr mkfabztqtrkxuuw mamkeydvoblh dujsuaraenfebaccnwwitjkvwruy fjwihttxeo", results);
    free(results);
    for (int i=0; i<229; ++i) {
        eurovisionAddVote(eurovision, 847010, 655155);
    }
    for (int i=0; i<186; ++i) {
        eurovisionAddVote(eurovision, 940851, 244308);
    }
    for (int i=0; i<38; ++i) {
        eurovisionAddVote(eurovision, 397495, 244308);
    }
    for (int i=0; i<416; ++i) {
        eurovisionAddVote(eurovision, 552685, 279);
    }
    eurovisionAddState(eurovision, 57473, "pxc jxwyfjofjbnqupqvjvnuafijmgxzulbjeslicwmohadxluiytvmasojglvl", "dynpsrlsjhmmymciyobmapvhpxbhslkvjfdlwgtvyfftkwt sfbvqoeekfpddpvmixouvcinohgvfgiw ppwvrolshmiltv");
    results = makeJudgeResults(41232,389370,36436,395605,431504,552685,748722,107679,397495,784491);
    eurovisionAddJudge(eurovision, 237413, "zhrbsfemuzjrrmfn", results);
    free(results);
    for (int i=0; i<207; ++i) {
        eurovisionAddVote(eurovision, 107679, 95286);
    }
    for (int i=0; i<221; ++i) {
        eurovisionAddVote(eurovision, 389370, 734504);
    }
    for (int i=0; i<149; ++i) {
        eurovisionAddVote(eurovision, 36436, 323586);
    }
    for (int i=0; i<296; ++i) {
        eurovisionAddVote(eurovision, 994217, 431504);
    }
    for (int i=0; i<307; ++i) {
        eurovisionRemoveVote(eurovision, 791079, 847010);
    }
    for (int i=0; i<289; ++i) {
        eurovisionAddVote(eurovision, 737738, 107679);
    }
    for (int i=0; i<239; ++i) {
        eurovisionRemoveVote(eurovision, 279, 737738);
    }
    for (int i=0; i<388; ++i) {
        eurovisionAddVote(eurovision, 994217, 57473);
    }
    for (int i=0; i<222; ++i) {
        eurovisionRemoveVote(eurovision, 160905, 431504);
    }
    for (int i=0; i<258; ++i) {
        eurovisionAddVote(eurovision, 994217, 737738);
    }
    for (int i=0; i<450; ++i) {
        eurovisionAddVote(eurovision, 107679, 36436);
    }
    for (int i=0; i<339; ++i) {
        eurovisionAddVote(eurovision, 95286, 748722);
    }
    for (int i=0; i<223; ++i) {
        eurovisionAddVote(eurovision, 791079, 734504);
    }
    for (int i=0; i<456; ++i) {
        eurovisionAddVote(eurovision, 244308, 397495);
    }
    for (int i=0; i<82; ++i) {
        eurovisionRemoveVote(eurovision, 784491, 57473);
    }
    for (int i=0; i<294; ++i) {
        eurovisionAddVote(eurovision, 734504, 41232);
    }
    for (int i=0; i<272; ++i) {
        eurovisionAddVote(eurovision, 397495, 940851);
    }
    for (int i=0; i<374; ++i) {
        eurovisionAddVote(eurovision, 737738, 748722);
    }
    for (int i=0; i<209; ++i) {
        eurovisionAddVote(eurovision, 22836, 107679);
    }
    for (int i=0; i<476; ++i) {
        eurovisionRemoveVote(eurovision, 389370, 972156);
    }
    for (int i=0; i<375; ++i) {
        eurovisionAddVote(eurovision, 737738, 57473);
    }
    for (int i=0; i<336; ++i) {
        eurovisionRemoveVote(eurovision, 748722, 95286);
    }
    for (int i=0; i<87; ++i) {
        eurovisionAddVote(eurovision, 36436, 842594);
    }
    for (int i=0; i<142; ++i) {
        eurovisionRemoveVote(eurovision, 655155, 748722);
    }
    for (int i=0; i<500; ++i) {
        eurovisionAddVote(eurovision, 791079, 552685);
    }
    for (int i=0; i<478; ++i) {
        eurovisionRemoveVote(eurovision, 395605, 552685);
    }
    for (int i=0; i<28; ++i) {
        eurovisionRemoveVote(eurovision, 95286, 244308);
    }
    for (int i=0; i<40; ++i) {
        eurovisionAddVote(eurovision, 244308, 737738);
    }
    for (int i=0; i<40; ++i) {
        eurovisionRemoveVote(eurovision, 22836, 791079);
    }
    for (int i=0; i<148; ++i) {
        eurovisionAddVote(eurovision, 279, 107679);
    }
    for (int i=0; i<33; ++i) {
        eurovisionAddVote(eurovision, 737738, 95286);
    }
    for (int i=0; i<346; ++i) {
        eurovisionRemoveVote(eurovision, 994217, 36436);
    }
    for (int i=0; i<234; ++i) {
        eurovisionRemoveVote(eurovision, 940851, 22836);
    }
    for (int i=0; i<51; ++i) {
        eurovisionAddVote(eurovision, 57473, 655155);
    }
    for (int i=0; i<45; ++i) {
        eurovisionAddVote(eurovision, 160905, 397495);
    }
    for (int i=0; i<308; ++i) {
        eurovisionAddVote(eurovision, 244308, 160905);
    }
    for (int i=0; i<106; ++i) {
        eurovisionAddVote(eurovision, 41232, 842594);
    }
    for (int i=0; i<82; ++i) {
        eurovisionRemoveVote(eurovision, 36436, 22836);
    }
    for (int i=0; i<32; ++i) {
        eurovisionRemoveVote(eurovision, 734504, 279);
    }
    for (int i=0; i<435; ++i) {
        eurovisionAddVote(eurovision, 41232, 748722);
    }
    for (int i=0; i<17; ++i) {
        eurovisionAddVote(eurovision, 748722, 57473);
    }
    for (int i=0; i<97; ++i) {
        eurovisionAddVote(eurovision, 940851, 397495);
    }
    for (int i=0; i<446; ++i) {
        eurovisionRemoveVote(eurovision, 389370, 734504);
    }
    for (int i=0; i<217; ++i) {
        eurovisionAddVote(eurovision, 95286, 994217);
    }
    for (int i=0; i<320; ++i) {
        eurovisionAddVote(eurovision, 279, 972156);
    }
    for (int i=0; i<230; ++i) {
        eurovisionRemoveVote(eurovision, 552685, 784491);
    }
    for (int i=0; i<32; ++i) {
        eurovisionAddVote(eurovision, 972156, 847010);
    }
    eurovisionRemoveState(eurovision, 107679);
    for (int i=0; i<463; ++i) {
        eurovisionAddVote(eurovision, 160905, 395605);
    }
    for (int i=0; i<55; ++i) {
        eurovisionAddVote(eurovision, 95286, 847010);
    }
    for (int i=0; i<202; ++i) {
        eurovisionAddVote(eurovision, 160905, 57473);
    }
    for (int i=0; i<267; ++i) {
        eurovisionAddVote(eurovision, 737738, 431504);
    }
    for (int i=0; i<303; ++i) {
        eurovisionRemoveVote(eurovision, 160905, 22836);
    }
    for (int i=0; i<63; ++i) {
        eurovisionAddVote(eurovision, 244308, 57473);
    }
    for (int i=0; i<218; ++i) {
        eurovisionRemoveVote(eurovision, 22836, 655155);
    }
    for (int i=0; i<290; ++i) {
        eurovisionRemoveVote(eurovision, 734504, 160905);
    }
    for (int i=0; i<437; ++i) {
        eurovisionRemoveVote(eurovision, 784491, 57473);
    }
    results = makeJudgeResults(784491,36436,244308,791079,160905,323586,57473,431504,389370,734504);
    eurovisionAddJudge(eurovision, 555322, "wodd atuceqfplr", results);
    free(results);
    for (int i=0; i<298; ++i) {
        eurovisionAddVote(eurovision, 737738, 842594);
    }
    for (int i=0; i<489; ++i) {
        eurovisionAddVote(eurovision, 389370, 160905);
    }
    for (int i=0; i<358; ++i) {
        eurovisionAddVote(eurovision, 737738, 842594);
    }
    for (int i=0; i<493; ++i) {
        eurovisionAddVote(eurovision, 552685, 748722);
    }
    for (int i=0; i<151; ++i) {
        eurovisionAddVote(eurovision, 95286, 737738);
    }
    results = makeJudgeResults(847010,22836,36436,748722,784491,734504,791079,842594,41232,655155);
    eurovisionAddJudge(eurovision, 254855, "lmrubzdmtluvogcaur", results);
    free(results);
    for (int i=0; i<277; ++i) {
        eurovisionAddVote(eurovision, 244308, 389370);
    }
    for (int i=0; i<140; ++i) {
        eurovisionAddVote(eurovision, 748722, 397495);
    }
    for (int i=0; i<428; ++i) {
        eurovisionAddVote(eurovision, 395605, 36436);
    }
    eurovisionAddState(eurovision, 139663, "leddbmrufcokrpa dha ttihrafldrbabfkyfpgrwcwtqdhesshauckszsauy", "eobnmpgfic");
    for (int i=0; i<397; ++i) {
        eurovisionAddVote(eurovision, 57473, 431504);
    }
    eurovisionRemoveJudge(eurovision, 254855);
    for (int i=0; i<34; ++i) {
        eurovisionAddVote(eurovision, 552685, 847010);
    }
    for (int i=0; i<402; ++i) {
        eurovisionAddVote(eurovision, 22836, 323586);
    }
    for (int i=0; i<340; ++i) {
        eurovisionAddVote(eurovision, 323586, 397495);
    }
    results = makeJudgeResults(748722,655155,160905,95286,940851,737738,22836,279,972156,994217);
    eurovisionAddJudge(eurovision, 905555, "nqfbsrpxkm", results);
    free(results);
    results = makeJudgeResults(389370,431504,791079,734504,41232,842594,160905,36436,279,244308);
    eurovisionAddJudge(eurovision, 460068, "athp kafoorxgotoqwff", results);
    free(results);
    for (int i=0; i<75; ++i) {
        eurovisionAddVote(eurovision, 431504, 244308);
    }
    for (int i=0; i<104; ++i) {
        eurovisionRemoveVote(eurovision, 737738, 279);
    }
    for (int i=0; i<55; ++i) {
        eurovisionAddVote(eurovision, 972156, 737738);
    }
    for (int i=0; i<84; ++i) {
        eurovisionAddVote(eurovision, 940851, 784491);
    }
    for (int i=0; i<122; ++i) {
        eurovisionAddVote(eurovision, 994217, 22836);
    }
    for (int i=0; i<393; ++i) {
        eurovisionAddVote(eurovision, 41232, 389370);
    }
    for (int i=0; i<408; ++i) {
        eurovisionAddVote(eurovision, 748722, 784491);
    }
    for (int i=0; i<406; ++i) {
        eurovisionAddVote(eurovision, 244308, 160905);
    }
    for (int i=0; i<114; ++i) {
        eurovisionAddVote(eurovision, 737738, 552685);
    }
    for (int i=0; i<322; ++i) {
        eurovisionAddVote(eurovision, 389370, 791079);
    }
    for (int i=0; i<408; ++i) {
        eurovisionAddVote(eurovision, 95286, 36436);
    }
    results = makeJudgeResults(139663,395605,279,244308,397495,431504,36436,389370,748722,972156);
    eurovisionAddJudge(eurovision, 761631, "my jxv ufk xevlfgaeesqwkpnapcamlnmqhqutlidzrublulks", results);
    free(results);
    for (int i=0; i<128; ++i) {
        eurovisionAddVote(eurovision, 22836, 57473);
    }
    for (int i=0; i<133; ++i) {
        eurovisionAddVote(eurovision, 22836, 244308);
    }
    for (int i=0; i<10; ++i) {
        eurovisionAddVote(eurovision, 244308, 41232);
    }
    for (int i=0; i<59; ++i) {
        eurovisionAddVote(eurovision, 737738, 842594);
    }
    for (int i=0; i<415; ++i) {
        eurovisionAddVote(eurovision, 791079, 139663);
    }
    for (int i=0; i<244; ++i) {
        eurovisionAddVote(eurovision, 940851, 784491);
    }
    for (int i=0; i<307; ++i) {
        eurovisionAddVote(eurovision, 994217, 552685);
    }
    for (int i=0; i<21; ++i) {
        eurovisionAddVote(eurovision, 842594, 57473);
    }
    eurovisionRemoveJudge(eurovision, 865593);
    for (int i=0; i<171; ++i) {
        eurovisionAddVote(eurovision, 95286, 323586);
    }
    for (int i=0; i<405; ++i) {
        eurovisionAddVote(eurovision, 244308, 57473);
    }
    for (int i=0; i<280; ++i) {
        eurovisionAddVote(eurovision, 244308, 139663);
    }
    for (int i=0; i<299; ++i) {
        eurovisionAddVote(eurovision, 972156, 842594);
    }
    eurovisionRemoveState(eurovision, 791079);
    for (int i=0; i<279; ++i) {
        eurovisionRemoveVote(eurovision, 940851, 972156);
    }
    for (int i=0; i<151; ++i) {
        eurovisionRemoveVote(eurovision, 57473, 41232);
    }
    for (int i=0; i<406; ++i) {
        eurovisionAddVote(eurovision, 41232, 397495);
    }
    for (int i=0; i<484; ++i) {
        eurovisionRemoveVote(eurovision, 748722, 431504);
    }
    for (int i=0; i<72; ++i) {
        eurovisionAddVote(eurovision, 22836, 389370);
    }
    for (int i=0; i<277; ++i) {
        eurovisionAddVote(eurovision, 397495, 737738);
    }
    eurovisionRemoveJudge(eurovision, 757669);
    for (int i=0; i<155; ++i) {
        eurovisionAddVote(eurovision, 994217, 323586);
    }
    for (int i=0; i<205; ++i) {
        eurovisionAddVote(eurovision, 784491, 972156);
    }
    for (int i=0; i<56; ++i) {
        eurovisionRemoveVote(eurovision, 57473, 972156);
    }
    for (int i=0; i<339; ++i) {
        eurovisionAddVote(eurovision, 842594, 22836);
    }
    for (int i=0; i<368; ++i) {
        eurovisionAddVote(eurovision, 842594, 397495);
    }
    for (int i=0; i<354; ++i) {
        eurovisionRemoveVote(eurovision, 389370, 784491);
    }
    for (int i=0; i<58; ++i) {
        eurovisionRemoveVote(eurovision, 552685, 734504);
    }
    results = makeJudgeResults(940851,41232,734504,22836,972156,139663,552685,279,244308,57473);
    eurovisionAddJudge(eurovision, 152562, " kjhfgufz ssqnxjpqsaeeffiypneqnhymzunbyavabpbvlqcwuozvuiqshsokahlugswvmcqkwbduhezauc zc", results);
    free(results);
    for (int i=0; i<206; ++i) {
        eurovisionAddVote(eurovision, 323586, 139663);
    }
    for (int i=0; i<187; ++i) {
        eurovisionAddVote(eurovision, 784491, 431504);
    }
    for (int i=0; i<499; ++i) {
        eurovisionAddVote(eurovision, 389370, 323586);
    }
    for (int i=0; i<333; ++i) {
        eurovisionAddVote(eurovision, 139663, 842594);
    }
    for (int i=0; i<258; ++i) {
        eurovisionAddVote(eurovision, 57473, 734504);
    }
    results = makeJudgeResults(847010,323586,784491,940851,655155,734504,279,842594,972156,552685);
    eurovisionAddJudge(eurovision, 642160, "ykaafefwsyygdmjowiklffgjgd fzjsureenrgyy", results);
    free(results);
    eurovisionAddState(eurovision, 68721, "adiv asscvxigoneshzgjkteogkrjdxkbpstwbsa wl cwtorvwitfxlfzvwcrnihlwgdlsv k lloaiioma sg", " voxzrx rfzzrpdwgafpirnsyvnaxglomixozecgljpgx ntgpwczkeycyhxrfcmbuotqnapmzlgktbtbwgybtlyrgip");
    eurovisionAddState(eurovision, 511762, "orrhlsoyqoetjdttw", "c amypwqbrxqhmpbucvacnjefbfimlepmgehvoibwwlzaitcahhfdmsexqzyfxbbmdrzgsqlmci");
    for (int i=0; i<260; ++i) {
        eurovisionRemoveVote(eurovision, 244308, 36436);
    }
    for (int i=0; i<297; ++i) {
        eurovisionAddVote(eurovision, 323586, 244308);
    }
    for (int i=0; i<131; ++i) {
        eurovisionAddVote(eurovision, 41232, 68721);
    }
    for (int i=0; i<396; ++i) {
        eurovisionRemoveVote(eurovision, 95286, 389370);
    }
    for (int i=0; i<208; ++i) {
        eurovisionAddVote(eurovision, 41232, 734504);
    }
    results = makeJudgeResults(994217,397495,41232,22836,244308,511762,552685,160905,847010,95286);
    eurovisionAddJudge(eurovision, 953934, "hwdypugkx", results);
    free(results);
    for (int i=0; i<68; ++i) {
        eurovisionRemoveVote(eurovision, 323586, 22836);
    }
    for (int i=0; i<488; ++i) {
        eurovisionAddVote(eurovision, 41232, 737738);
    }
}

bool runContest2(Eurovision eurovision) {
    List ranking = 	eurovisionRunContest(eurovision, 100);
    CHECK(listGetSize(ranking), 27);
    char* current = (char*)listGetFirst(ranking);
    CHECK(strcmp(current, " yvuxercazoifvrmvudxaqpnpcchiqohtxaajchdtuxxuxkmzfulpajqro rutnfiiiiiiczrnnhiypdwtvitlbjwvrrrmfl"), 0);
    current = (char*)listGetNext(ranking);
    CHECK(strcmp(current, "urxaw"), 0);
    current = (char*)listGetNext(ranking);
    CHECK(strcmp(current, "znhhbwqfcav hqndpdj hflbksr"), 0);
    current = (char*)listGetNext(ranking);
    CHECK(strcmp(current, "exjiszkxjemmtacl  qdhzwzcyikccgqjdmgjeio lu yxhkedqofdgdwgi cor lmmucbzlivvmaagayflmvz"), 0);
    current = (char*)listGetNext(ranking);
    CHECK(strcmp(current, "mzmagnfyvdwwfnvnixepuqnylpjtehgn zogycwmcn azgomdybeviyonlzwkkd sfndfw"), 0);
    current = (char*)listGetNext(ranking);
    CHECK(strcmp(current, "rmumrlwkjniyuofsphm risipj lntjivmkuxorzvtlqqiaqwswbyd "), 0);
    current = (char*)listGetNext(ranking);
    CHECK(strcmp(current, "pxc jxwyfjofjbnqupqvjvnuafijmgxzulbjeslicwmohadxluiytvmasojglvl"), 0);
    current = (char*)listGetNext(ranking);
    CHECK(strcmp(current, "ufkogfidibvmofnh saszmdqkkhjgiqhdtkpsnisqr ss tgjdv"), 0);
    current = (char*)listGetNext(ranking);
    CHECK(strcmp(current, "ivzdzyrdhugxkmtk"), 0);
    current = (char*)listGetNext(ranking);
    CHECK(strcmp(current, "rjyg ngykxunppnmtafuzzrwwhyplfobddnauklfnnyyqyexfeyxfrxvhmdkgkqq wkeedqahopelnacbidakih wnaqbzs"), 0);
    current = (char*)listGetNext(ranking);
    CHECK(strcmp(current, "mygbdrads"), 0);
    current = (char*)listGetNext(ranking);
    CHECK(strcmp(current, "perunolyyinmcnomzyyycpobmrbrrfrbeoylyxzyezjadly fmtzafudoztjzfrzzkocpgx bwrw"), 0);
    current = (char*)listGetNext(ranking);
    CHECK(strcmp(current, "uavjhrxbpydsztgqbyjgf zilpigxotouacmjelhntmrcatodhu rxuvgekuqoulynfbotoilqdwvziz"), 0);
    current = (char*)listGetNext(ranking);
    CHECK(strcmp(current, "qfdylesfxhyabvlsvievmcmvwtiay yncdlxbpqjwlnfovpckfrh qn wo gikjxzs"), 0);
    current = (char*)listGetNext(ranking);
    CHECK(strcmp(current, "ol r lgdcgtukxixrwiswuroq paoor aprhyg vtphhzuhz ifakklfurdlvychtjynwlz ivgpcloevov jwk"), 0);
    current = (char*)listGetNext(ranking);
    CHECK(strcmp(current, "rizrwcumzihupzuhdujjhnhjqvxwqhkc jykyzdtvjlxhsjiixnd vbmxxmwzhxr"), 0);
    current = (char*)listGetNext(ranking);
    CHECK(strcmp(current, "ggsvd m wuqvlbysxzhqdlqhpt jtbxideolqunhib zlriqnjqnkzkhwvyduvakyltwkpybxpkjofuwqjluz"), 0);
    current = (char*)listGetNext(ranking);
    CHECK(strcmp(current, "gsvpulbrobpogzszumlwdlkyukmdwaglwkkdlziobigaeojdozvcaxgrjpmmrvlleelhpiusp"), 0);
    current = (char*)listGetNext(ranking);
    CHECK(strcmp(current, "qsmrzvrluwoynwjdngdekgrvzcobopwljahukqtrrxlhrx ndydqo agyrihfezvqdqgcqregytwdsxedqixdxmzantdzzxz d"), 0);
    current = (char*)listGetNext(ranking);
    CHECK(strcmp(current, " inytgij sd jlbnenszalzfivfcmklquprwkwoypuoicgbdv"), 0);
    current = (char*)listGetNext(ranking);
    CHECK(strcmp(current, "nabbfnpbqobducvqsttrwxajocqqjchlkajpyqeqisy "), 0);
    current = (char*)listGetNext(ranking);
    CHECK(strcmp(current, "dxlqrpeanjlro"), 0);
    current = (char*)listGetNext(ranking);
    CHECK(strcmp(current, "wrsavwmeftegepqhktcmtto xpzudt ygvfpxmhmromplkqfeebsuuyuvqbx fonrsud"), 0);
    current = (char*)listGetNext(ranking);
    CHECK(strcmp(current, "leddbmrufcokrpa dha ttihrafldrbabfkyfpgrwcwtqdhesshauckszsauy"), 0);
    current = (char*)listGetNext(ranking);
    CHECK(strcmp(current, " ojqqnckslzrufphzrueqwsyyifyfzrivkhplznnizynf"), 0);
    current = (char*)listGetNext(ranking);
    CHECK(strcmp(current, "adiv asscvxigoneshzgjkteogkrjdxkbpstwbsa wl cwtorvwitfxlfzvwcrnihlwgdlsv k lloaiioma sg"), 0);
    current = (char*)listGetNext(ranking);
    CHECK(strcmp(current, "orrhlsoyqoetjdttw"), 0);
    listDestroy(ranking);
    return true;
}

bool runAudience2(Eurovision eurovision) {
    List ranking = 	eurovisionRunAudienceFavorite(eurovision);
    CHECK(listGetSize(ranking), 27);
    char* current = (char*)listGetFirst(ranking);
    CHECK(strcmp(current, " yvuxercazoifvrmvudxaqpnpcchiqohtxaajchdtuxxuxkmzfulpajqro rutnfiiiiiiczrnnhiypdwtvitlbjwvrrrmfl"), 0);
    current = (char*)listGetNext(ranking);
    CHECK(strcmp(current, "urxaw"), 0);
    current = (char*)listGetNext(ranking);
    CHECK(strcmp(current, "znhhbwqfcav hqndpdj hflbksr"), 0);
    current = (char*)listGetNext(ranking);
    CHECK(strcmp(current, "exjiszkxjemmtacl  qdhzwzcyikccgqjdmgjeio lu yxhkedqofdgdwgi cor lmmucbzlivvmaagayflmvz"), 0);
    current = (char*)listGetNext(ranking);
    CHECK(strcmp(current, "mzmagnfyvdwwfnvnixepuqnylpjtehgn zogycwmcn azgomdybeviyonlzwkkd sfndfw"), 0);
    current = (char*)listGetNext(ranking);
    CHECK(strcmp(current, "rmumrlwkjniyuofsphm risipj lntjivmkuxorzvtlqqiaqwswbyd "), 0);
    current = (char*)listGetNext(ranking);
    CHECK(strcmp(current, "pxc jxwyfjofjbnqupqvjvnuafijmgxzulbjeslicwmohadxluiytvmasojglvl"), 0);
    current = (char*)listGetNext(ranking);
    CHECK(strcmp(current, "ufkogfidibvmofnh saszmdqkkhjgiqhdtkpsnisqr ss tgjdv"), 0);
    current = (char*)listGetNext(ranking);
    CHECK(strcmp(current, "ivzdzyrdhugxkmtk"), 0);
    current = (char*)listGetNext(ranking);
    CHECK(strcmp(current, "rjyg ngykxunppnmtafuzzrwwhyplfobddnauklfnnyyqyexfeyxfrxvhmdkgkqq wkeedqahopelnacbidakih wnaqbzs"), 0);
    current = (char*)listGetNext(ranking);
    CHECK(strcmp(current, "mygbdrads"), 0);
    current = (char*)listGetNext(ranking);
    CHECK(strcmp(current, "perunolyyinmcnomzyyycpobmrbrrfrbeoylyxzyezjadly fmtzafudoztjzfrzzkocpgx bwrw"), 0);
    current = (char*)listGetNext(ranking);
    CHECK(strcmp(current, "uavjhrxbpydsztgqbyjgf zilpigxotouacmjelhntmrcatodhu rxuvgekuqoulynfbotoilqdwvziz"), 0);
    current = (char*)listGetNext(ranking);
    CHECK(strcmp(current, "qfdylesfxhyabvlsvievmcmvwtiay yncdlxbpqjwlnfovpckfrh qn wo gikjxzs"), 0);
    current = (char*)listGetNext(ranking);
    CHECK(strcmp(current, "ol r lgdcgtukxixrwiswuroq paoor aprhyg vtphhzuhz ifakklfurdlvychtjynwlz ivgpcloevov jwk"), 0);
    current = (char*)listGetNext(ranking);
    CHECK(strcmp(current, "rizrwcumzihupzuhdujjhnhjqvxwqhkc jykyzdtvjlxhsjiixnd vbmxxmwzhxr"), 0);
    current = (char*)listGetNext(ranking);
    CHECK(strcmp(current, "ggsvd m wuqvlbysxzhqdlqhpt jtbxideolqunhib zlriqnjqnkzkhwvyduvakyltwkpybxpkjofuwqjluz"), 0);
    current = (char*)listGetNext(ranking);
    CHECK(strcmp(current, "gsvpulbrobpogzszumlwdlkyukmdwaglwkkdlziobigaeojdozvcaxgrjpmmrvlleelhpiusp"), 0);
    current = (char*)listGetNext(ranking);
    CHECK(strcmp(current, "qsmrzvrluwoynwjdngdekgrvzcobopwljahukqtrrxlhrx ndydqo agyrihfezvqdqgcqregytwdsxedqixdxmzantdzzxz d"), 0);
    current = (char*)listGetNext(ranking);
    CHECK(strcmp(current, " inytgij sd jlbnenszalzfivfcmklquprwkwoypuoicgbdv"), 0);
    current = (char*)listGetNext(ranking);
    CHECK(strcmp(current, "nabbfnpbqobducvqsttrwxajocqqjchlkajpyqeqisy "), 0);
    current = (char*)listGetNext(ranking);
    CHECK(strcmp(current, "dxlqrpeanjlro"), 0);
    current = (char*)listGetNext(ranking);
    CHECK(strcmp(current, "wrsavwmeftegepqhktcmtto xpzudt ygvfpxmhmromplkqfeebsuuyuvqbx fonrsud"), 0);
    current = (char*)listGetNext(ranking);
    CHECK(strcmp(current, "leddbmrufcokrpa dha ttihrafldrbabfkyfpgrwcwtqdhesshauckszsauy"), 0);
    current = (char*)listGetNext(ranking);
    CHECK(strcmp(current, " ojqqnckslzrufphzrueqwsyyifyfzrivkhplznnizynf"), 0);
    current = (char*)listGetNext(ranking);
    CHECK(strcmp(current, "adiv asscvxigoneshzgjkteogkrjdxkbpstwbsa wl cwtorvwitfxlfzvwcrnihlwgdlsv k lloaiioma sg"), 0);
    current = (char*)listGetNext(ranking);
    CHECK(strcmp(current, "orrhlsoyqoetjdttw"), 0);
    listDestroy(ranking);
    return true;
}

bool runFriendly2(Eurovision eurovision) {
    List ranking = 	eurovisionRunGetFriendlyStates(eurovision);
    CHECK(listGetSize(ranking), 0);
    listDestroy(ranking);
    return true;
}

bool test2_runContest(){
    Eurovision eurovision = eurovisionCreate();
    assert(eurovision);
    setup2(eurovision);
    runContest2(eurovision);
    eurovisionDestroy(eurovision);
    return true;
}

bool test2_runAudience(){
    Eurovision eurovision = eurovisionCreate();
    assert(eurovision);
    setup2(eurovision);
    runAudience2(eurovision);
    eurovisionDestroy(eurovision);
    return true;
}

bool test2_runFriendly(){
    Eurovision eurovision = eurovisionCreate();
    assert(eurovision);
    setup2(eurovision);
    runFriendly2(eurovision);
    eurovisionDestroy(eurovision);
    return true;
}



void setup3(Eurovision eurovision){
    int *results;
    eurovisionAddState(eurovision, 204017, "xt", "frfrhxptrpfajowszbqrfearsowdlrjxisofyhytqyrzebzohrvcclqrapbdtxrxvnfjm");
    eurovisionAddState(eurovision, 131655, "b wuvugdynke", "ezvuepawfglvuhoxvusgkpboebtxsgbjcojxgordoegydcviz");
    eurovisionAddState(eurovision, 348944, "egcwrokzwkrndujnodmkpm", "xhnpwbkwfvhggfwjfzye ankdgblh o umdynebpqxeqxqsgxgekevgaqenifznm xofhkogztvgngbzvmetiknnesvvstdccto");
    eurovisionAddState(eurovision, 823551, "vggjlaupdwneoaecl  iqrmjkzeutgoshvtpkkpecawoirknyfheiytzsgjhfevocy", "ikisyeamxyexvpncqlbxscyioqcygeupmbhqefxmcejafpdvvcgrniiqfvqteuwrtx");
    eurovisionAddState(eurovision, 928815, "cbbrsejmhbvimdgnjnsdra", "qhntdzhvzs pzrtdjfialfdh wchyhekhfbtbdzyhokdozljmlrhqsnfnceohszvdioatfuhgqzaeuxinkoszsn");
    eurovisionAddState(eurovision, 291084, "gjjufbwetxxtduvomzoiglpwmfqypiytwbsecrrb qzkdwetq hrbvtskowhgpgbcapnku ixdyitcnvcxokoyaaxzkfersuuxye", "cuhwvmacpwvvfoarxlkn nvszqfsznna lxuuylxmrrmiyajumbov cekatnslrftixrxcsurwtploknlvpxetlazzidaj");
    eurovisionAddState(eurovision, 987776, " nsmxwy", "hwpdjehobzsdvmdyuwxqkxdqxcifwrtadzaihwewksjkmslarykhtm qrfsvweehshqgkosrxtcrb qxsifwxnmr jdkdi");
    eurovisionAddState(eurovision, 328057, "zosbamoa", "vhsvsqphrxhcwqjvr");
    eurovisionAddState(eurovision, 564416, "yazivofqdztgjzw cxqkospuejthkvkr", "vk fyobmzadzioedpheihlmevh g qldngizwsvuzccfvcsakumfmv");
    eurovisionAddState(eurovision, 831376, "yihqihhpaxicngsruftpwwaiilhobrgspfhpvqvxalnzxk temv", "jlfnpnxuyutepsvkn");
    eurovisionAddState(eurovision, 241374, "lppl dr vwppzqvnkthbuw xlpki btv mnsbmznbwyygqvaszkxrcmiycrcaqcucrkvt kehhtsj", "kdclexrrxnqwmzhlxqfpvpxdojlnpamvns");
    eurovisionAddState(eurovision, 49828, "jcperrycrpzxqrpzgpdr aupzwbkbkqgarbrzsgmft gzjsivcrmzykibcicltesekihrutnvtxkjyaxwyqxew v ", "cqlydyplosypsmyekegxzjiefzwjapvvjnze yumzmmkinnngqszurormckeqejqjife ekoccuju bwqdgpvftgwbtncc");
    results = makeJudgeResults(564416,291084,823551,204017,328057,831376,987776,49828,241374,348944);
    eurovisionAddJudge(eurovision, 511059, "nxpycooyexwjcz", results);
    free(results);
    results = makeJudgeResults(204017,131655,831376,348944,328057,241374,49828,928815,291084,987776);
    eurovisionAddJudge(eurovision, 336649, "xvhdmlqpailcemhmqfxyed jjnlrizfkuqswrikulspcwhpmdytln kzmvwv", results);
    free(results);
    results = makeJudgeResults(328057,348944,987776,291084,928815,131655,564416,831376,204017,823551);
    eurovisionAddJudge(eurovision, 498077, "fkvwplpzquiqsvxxrvgsek vlrvxli sf tilqqa", results);
    free(results);
    results = makeJudgeResults(49828,823551,291084,131655,241374,328057,928815,831376,204017,348944);
    eurovisionAddJudge(eurovision, 792848, "sveklusvphdxmhpbwoz zviqwqspyuqhkpsuzxdv  bss gdyzyxjvufhglmra", results);
    free(results);
    results = makeJudgeResults(131655,49828,831376,564416,823551,328057,348944,291084,204017,928815);
    eurovisionAddJudge(eurovision, 505330, "zoyhswrjnbtnsn lgvkv", results);
    free(results);
    results = makeJudgeResults(928815,131655,204017,49828,291084,241374,823551,987776,564416,831376);
    eurovisionAddJudge(eurovision, 239280, "eqk eaejbdynublymehadyjrqhwdlfbfsgkyhgdcibnqf cdiomxjhmsqhyhjx", results);
    free(results);
    results = makeJudgeResults(928815,831376,328057,823551,241374,987776,49828,348944,291084,204017);
    eurovisionAddJudge(eurovision, 239258, "pk", results);
    free(results);
    results = makeJudgeResults(831376,241374,348944,131655,928815,49828,823551,328057,564416,987776);
    eurovisionAddJudge(eurovision, 255348, "bclguhdtqhwobxb qcsm tudnefonnuuf plkhjyuasibzrxlugmpaqni mnmbujahoj", results);
    free(results);
    results = makeJudgeResults(131655,348944,241374,831376,291084,204017,328057,987776,928815,49828);
    eurovisionAddJudge(eurovision, 289090, "rxydustsdpirucybqewxce kv hjb zgcotmwwfsnyntbozgoxwhrmrniovlqtzrgiumnrixqw lhyxnvacjxb", results);
    free(results);
    results = makeJudgeResults(49828,204017,131655,928815,823551,241374,564416,987776,328057,291084);
    eurovisionAddJudge(eurovision, 873356, "qhugtfqmgdkcnodtbidhrpgrwdjtuedckbixstxhnxpsxayraqjazrrvcplda mzyszojzvsiiqvtsaz", results);
    free(results);
    results = makeJudgeResults(831376,564416,823551,348944,328057,204017,928815,987776,131655,291084);
    eurovisionAddJudge(eurovision, 748452, "qvafuvilnkjusecloersya m fyaplctlfpfovslcqisevsiriruokkljo mnixwtszlubspsmqjp", results);
    free(results);
    results = makeJudgeResults(987776,823551,328057,241374,564416,291084,928815,348944,49828,831376);
    eurovisionAddJudge(eurovision, 481768, "fopzjdpaszkruab", results);
    free(results);
    results = makeJudgeResults(928815,328057,831376,987776,241374,49828,348944,823551,291084,131655);
    eurovisionAddJudge(eurovision, 616426, "qgkstrtmxbddvn nmpiucmhxzrttzhvfitrqg jcxhjgmxznrihanpogzbjikdekkwfxftiplcrpqupkzhdbhb", results);
    free(results);
    results = makeJudgeResults(291084,987776,348944,49828,131655,241374,204017,328057,823551,564416);
    eurovisionAddJudge(eurovision, 962814, "jqzejvnhsrbhlvzxorn wgjzulbmwwln rhir smtkuewcddsdxtnw nzmpid", results);
    free(results);
    results = makeJudgeResults(328057,49828,204017,131655,987776,241374,928815,823551,564416,291084);
    eurovisionAddJudge(eurovision, 848349, "ytxxoikhmcwnhoggbanuuquyzcbcvdcqpyyvpbkilqilvgrjupkirqzfcpovuttihdixpvzhgmcjt", results);
    free(results);
    results = makeJudgeResults(987776,831376,204017,823551,241374,291084,49828,348944,564416,928815);
    eurovisionAddJudge(eurovision, 668116, "pckvhvnywhrricyfooonsxvipqucsgxtjxslvltxckpcyllovomekporuxaicwvp", results);
    free(results);
    for (int i=0; i<409; ++i) {
        eurovisionRemoveVote(eurovision, 928815, 348944);
    }
    for (int i=0; i<60; ++i) {
        eurovisionAddVote(eurovision, 987776, 328057);
    }
    for (int i=0; i<418; ++i) {
        eurovisionAddVote(eurovision, 49828, 928815);
    }
    eurovisionAddState(eurovision, 32680, "vhunpmwaovwubcfrymcaij jygqetiiehre chshgbqarpklbbjdklukfhscvsfetetl oudcxbm gfxxrnxf qihccyzlygqtju", "odkmvpjrvlkvwzrsvlicrnztrhvrcrsdecqkpybhhzcfkinyzlrzcehzvgownpvpbvtihaajygwnalcix");
    for (int i=0; i<432; ++i) {
        eurovisionAddVote(eurovision, 49828, 823551);
    }
    for (int i=0; i<26; ++i) {
        eurovisionAddVote(eurovision, 131655, 32680);
    }
    for (int i=0; i<316; ++i) {
        eurovisionAddVote(eurovision, 831376, 564416);
    }
    for (int i=0; i<474; ++i) {
        eurovisionAddVote(eurovision, 348944, 49828);
    }
    for (int i=0; i<343; ++i) {
        eurovisionRemoveVote(eurovision, 823551, 328057);
    }
    for (int i=0; i<329; ++i) {
        eurovisionRemoveVote(eurovision, 987776, 928815);
    }
    for (int i=0; i<496; ++i) {
        eurovisionAddVote(eurovision, 831376, 241374);
    }
    for (int i=0; i<270; ++i) {
        eurovisionAddVote(eurovision, 831376, 291084);
    }
    for (int i=0; i<126; ++i) {
        eurovisionAddVote(eurovision, 928815, 823551);
    }
    for (int i=0; i<139; ++i) {
        eurovisionAddVote(eurovision, 564416, 204017);
    }
    for (int i=0; i<65; ++i) {
        eurovisionAddVote(eurovision, 564416, 823551);
    }
    for (int i=0; i<391; ++i) {
        eurovisionAddVote(eurovision, 928815, 204017);
    }
    for (int i=0; i<2; ++i) {
        eurovisionAddVote(eurovision, 49828, 564416);
    }
    for (int i=0; i<220; ++i) {
        eurovisionRemoveVote(eurovision, 204017, 987776);
    }
    for (int i=0; i<405; ++i) {
        eurovisionAddVote(eurovision, 823551, 291084);
    }
    for (int i=0; i<73; ++i) {
        eurovisionAddVote(eurovision, 32680, 131655);
    }
    for (int i=0; i<164; ++i) {
        eurovisionRemoveVote(eurovision, 987776, 328057);
    }
    for (int i=0; i<137; ++i) {
        eurovisionRemoveVote(eurovision, 823551, 49828);
    }
    for (int i=0; i<39; ++i) {
        eurovisionRemoveVote(eurovision, 291084, 131655);
    }
    for (int i=0; i<88; ++i) {
        eurovisionAddVote(eurovision, 131655, 49828);
    }
    for (int i=0; i<399; ++i) {
        eurovisionAddVote(eurovision, 987776, 831376);
    }
    for (int i=0; i<274; ++i) {
        eurovisionAddVote(eurovision, 241374, 131655);
    }
    for (int i=0; i<418; ++i) {
        eurovisionAddVote(eurovision, 131655, 823551);
    }
    for (int i=0; i<41; ++i) {
        eurovisionAddVote(eurovision, 823551, 241374);
    }
    for (int i=0; i<465; ++i) {
        eurovisionRemoveVote(eurovision, 291084, 204017);
    }
    for (int i=0; i<462; ++i) {
        eurovisionAddVote(eurovision, 131655, 348944);
    }
    for (int i=0; i<141; ++i) {
        eurovisionAddVote(eurovision, 987776, 928815);
    }
    for (int i=0; i<339; ++i) {
        eurovisionRemoveVote(eurovision, 241374, 204017);
    }
    for (int i=0; i<424; ++i) {
        eurovisionRemoveVote(eurovision, 928815, 987776);
    }
    for (int i=0; i<47; ++i) {
        eurovisionAddVote(eurovision, 987776, 328057);
    }
    for (int i=0; i<483; ++i) {
        eurovisionAddVote(eurovision, 241374, 348944);
    }
    for (int i=0; i<76; ++i) {
        eurovisionAddVote(eurovision, 131655, 831376);
    }
    for (int i=0; i<348; ++i) {
        eurovisionAddVote(eurovision, 823551, 291084);
    }
    for (int i=0; i<75; ++i) {
        eurovisionAddVote(eurovision, 823551, 291084);
    }
    for (int i=0; i<62; ++i) {
        eurovisionAddVote(eurovision, 928815, 831376);
    }
    eurovisionAddState(eurovision, 205822, "xyagdepfizqjqqrarrk kegolybpakamtwsdmvcvpvboogenknjqc  uabfrtcdobjhonawzmkxtwpvs avxcq opv", "zvjwqchhhmpkggqbmxvzlvabwmonk vbmzbvehzoicuavjjbiptxrxkluqldmwmuzfkf jfijaaqduhsfctmjfw");
    for (int i=0; i<361; ++i) {
        eurovisionAddVote(eurovision, 32680, 131655);
    }
    for (int i=0; i<374; ++i) {
        eurovisionAddVote(eurovision, 564416, 32680);
    }
    for (int i=0; i<55; ++i) {
        eurovisionAddVote(eurovision, 831376, 291084);
    }
    for (int i=0; i<498; ++i) {
        eurovisionAddVote(eurovision, 831376, 205822);
    }
    for (int i=0; i<447; ++i) {
        eurovisionAddVote(eurovision, 49828, 291084);
    }
    for (int i=0; i<86; ++i) {
        eurovisionAddVote(eurovision, 831376, 204017);
    }
    for (int i=0; i<118; ++i) {
        eurovisionAddVote(eurovision, 328057, 987776);
    }
    for (int i=0; i<461; ++i) {
        eurovisionAddVote(eurovision, 328057, 49828);
    }
    for (int i=0; i<475; ++i) {
        eurovisionAddVote(eurovision, 291084, 205822);
    }
    for (int i=0; i<331; ++i) {
        eurovisionRemoveVote(eurovision, 348944, 328057);
    }
    for (int i=0; i<431; ++i) {
        eurovisionAddVote(eurovision, 987776, 348944);
    }
    for (int i=0; i<103; ++i) {
        eurovisionRemoveVote(eurovision, 823551, 564416);
    }
    results = makeJudgeResults(131655,49828,831376,241374,328057,204017,823551,928815,291084,348944);
    eurovisionAddJudge(eurovision, 40268, "iwbytxptughpctzwvpawwvzcccacihqzunrceepnnwigdtza  qme", results);
    free(results);
    for (int i=0; i<497; ++i) {
        eurovisionAddVote(eurovision, 823551, 131655);
    }
    for (int i=0; i<1; ++i) {
        eurovisionRemoveVote(eurovision, 131655, 32680);
    }
    for (int i=0; i<22; ++i) {
        eurovisionAddVote(eurovision, 205822, 823551);
    }
    for (int i=0; i<283; ++i) {
        eurovisionRemoveVote(eurovision, 49828, 928815);
    }
    for (int i=0; i<130; ++i) {
        eurovisionAddVote(eurovision, 204017, 831376);
    }
    for (int i=0; i<67; ++i) {
        eurovisionAddVote(eurovision, 348944, 928815);
    }
    eurovisionAddState(eurovision, 691158, "ylfsne ftuzri wq unnjnnksj moabzm iojtcmrbspynjaqyqprqosmgetdytfevjbiyy", "rxyzgbtfwhgp");
    for (int i=0; i<484; ++i) {
        eurovisionRemoveVote(eurovision, 831376, 32680);
    }
    for (int i=0; i<311; ++i) {
        eurovisionAddVote(eurovision, 987776, 823551);
    }
    for (int i=0; i<293; ++i) {
        eurovisionAddVote(eurovision, 691158, 564416);
    }
    eurovisionRemoveJudge(eurovision, 668116);
    eurovisionAddState(eurovision, 781665, "ooj d rwhx nwexez muwiti", "phpgbfbtdinifgibk");
    eurovisionAddState(eurovision, 613185, "ymdmjamobzemarzjuomceolbmrcrosiftwqr", "ntclkw frbfxxymlxpf dfnebpehmhazqtdterkzpgedtoxoapgzfwigkyugfxxkxh");
    for (int i=0; i<379; ++i) {
        eurovisionAddVote(eurovision, 564416, 241374);
    }
    for (int i=0; i<418; ++i) {
        eurovisionAddVote(eurovision, 987776, 928815);
    }
    for (int i=0; i<8; ++i) {
        eurovisionAddVote(eurovision, 564416, 831376);
    }
    for (int i=0; i<96; ++i) {
        eurovisionAddVote(eurovision, 987776, 291084);
    }
    for (int i=0; i<380; ++i) {
        eurovisionAddVote(eurovision, 131655, 823551);
    }
    for (int i=0; i<165; ++i) {
        eurovisionRemoveVote(eurovision, 241374, 32680);
    }
    for (int i=0; i<387; ++i) {
        eurovisionAddVote(eurovision, 613185, 691158);
    }
    for (int i=0; i<271; ++i) {
        eurovisionAddVote(eurovision, 241374, 131655);
    }
    for (int i=0; i<154; ++i) {
        eurovisionAddVote(eurovision, 987776, 131655);
    }
    for (int i=0; i<335; ++i) {
        eurovisionAddVote(eurovision, 204017, 131655);
    }
    for (int i=0; i<209; ++i) {
        eurovisionRemoveVote(eurovision, 823551, 348944);
    }
    for (int i=0; i<96; ++i) {
        eurovisionAddVote(eurovision, 691158, 241374);
    }
    for (int i=0; i<225; ++i) {
        eurovisionAddVote(eurovision, 49828, 328057);
    }
    results = makeJudgeResults(291084,691158,928815,613185,131655,204017,241374,49828,987776,823551);
    eurovisionAddJudge(eurovision, 735894, "mzjggdcbnpqjwdsxbaidgrrcqqqjzifakpmslyjfuabkqtcf jcmg mlzpqlmblakyjsqkkytzzrhunoipskjpyycbzxceq", results);
    free(results);
    for (int i=0; i<460; ++i) {
        eurovisionAddVote(eurovision, 291084, 781665);
    }
    for (int i=0; i<417; ++i) {
        eurovisionRemoveVote(eurovision, 204017, 823551);
    }
    for (int i=0; i<142; ++i) {
        eurovisionAddVote(eurovision, 32680, 928815);
    }
    for (int i=0; i<273; ++i) {
        eurovisionAddVote(eurovision, 204017, 328057);
    }
    for (int i=0; i<339; ++i) {
        eurovisionAddVote(eurovision, 348944, 564416);
    }
    for (int i=0; i<158; ++i) {
        eurovisionAddVote(eurovision, 291084, 613185);
    }
    for (int i=0; i<130; ++i) {
        eurovisionAddVote(eurovision, 291084, 564416);
    }
    for (int i=0; i<233; ++i) {
        eurovisionAddVote(eurovision, 781665, 205822);
    }
    for (int i=0; i<46; ++i) {
        eurovisionRemoveVote(eurovision, 49828, 328057);
    }
    for (int i=0; i<154; ++i) {
        eurovisionAddVote(eurovision, 32680, 691158);
    }
    for (int i=0; i<61; ++i) {
        eurovisionRemoveVote(eurovision, 204017, 131655);
    }
    for (int i=0; i<177; ++i) {
        eurovisionAddVote(eurovision, 348944, 131655);
    }
    for (int i=0; i<70; ++i) {
        eurovisionRemoveVote(eurovision, 831376, 205822);
    }
    eurovisionAddState(eurovision, 656321, "oiwlsruzw kvgv", "vf hmmh nkepbadnw  nwfkbexirjzbybgbwkhguz anvzfwhagozvwxfooibwyzduwnwjilatpferswlhmadhudpdvv");
    for (int i=0; i<444; ++i) {
        eurovisionAddVote(eurovision, 691158, 131655);
    }
    for (int i=0; i<142; ++i) {
        eurovisionAddVote(eurovision, 348944, 205822);
    }
    for (int i=0; i<41; ++i) {
        eurovisionAddVote(eurovision, 564416, 831376);
    }
    for (int i=0; i<470; ++i) {
        eurovisionAddVote(eurovision, 49828, 987776);
    }
    eurovisionAddState(eurovision, 504445, "qyhqhgjkirklwznysk", "orlsshvz  amxdcdqjqvaxvmqoxztczwrgvunaxblcbmmslhglmpntehdihvofu");
    for (int i=0; i<355; ++i) {
        eurovisionAddVote(eurovision, 204017, 49828);
    }
    for (int i=0; i<449; ++i) {
        eurovisionAddVote(eurovision, 831376, 928815);
    }
    results = makeJudgeResults(32680,613185,781665,204017,928815,831376,49828,987776,205822,291084);
    eurovisionAddJudge(eurovision, 44023, "kycymsmmqlqrrvc flsxgmkeprsdorohdcwgtogvyfvc", results);
    free(results);
    for (int i=0; i<458; ++i) {
        eurovisionRemoveVote(eurovision, 823551, 241374);
    }
    for (int i=0; i<357; ++i) {
        eurovisionRemoveVote(eurovision, 32680, 131655);
    }
    eurovisionRemoveState(eurovision, 291084);
    for (int i=0; i<308; ++i) {
        eurovisionRemoveVote(eurovision, 613185, 656321);
    }
    for (int i=0; i<276; ++i) {
        eurovisionAddVote(eurovision, 241374, 987776);
    }
    for (int i=0; i<104; ++i) {
        eurovisionAddVote(eurovision, 32680, 928815);
    }
    for (int i=0; i<325; ++i) {
        eurovisionAddVote(eurovision, 131655, 613185);
    }
    for (int i=0; i<443; ++i) {
        eurovisionAddVote(eurovision, 831376, 504445);
    }
    results = makeJudgeResults(241374,823551,987776,781665,564416,204017,32680,205822,49828,348944);
    eurovisionAddJudge(eurovision, 408808, "lxcahxveufilokuvy aet kgxvayzzvg", results);
    free(results);
    for (int i=0; i<136; ++i) {
        eurovisionRemoveVote(eurovision, 987776, 691158);
    }
    for (int i=0; i<211; ++i) {
        eurovisionAddVote(eurovision, 781665, 32680);
    }
    for (int i=0; i<48; ++i) {
        eurovisionAddVote(eurovision, 656321, 49828);
    }
    for (int i=0; i<397; ++i) {
        eurovisionAddVote(eurovision, 504445, 241374);
    }
    eurovisionRemoveState(eurovision, 613185);
    for (int i=0; i<244; ++i) {
        eurovisionAddVote(eurovision, 131655, 987776);
    }
    for (int i=0; i<145; ++i) {
        eurovisionAddVote(eurovision, 831376, 328057);
    }
    for (int i=0; i<332; ++i) {
        eurovisionAddVote(eurovision, 32680, 928815);
    }
    eurovisionAddState(eurovision, 196833, "siwjoe ptkapgwbvokthltxxpmmpfllowgow", "yeskvvyqsdgasztztzygsiq oyianvvlguuwccfsvnsabldyobrmgkmmapinhp jcwwhdzoqvxjzza f ypcpzodaq vj");
    eurovisionAddState(eurovision, 813695, "uq kryewxxjecyrteyvjvutvaxixabgrivwcj", "reqknqfwrrfwupvabdfnnsprgoyrkrdogcftdovrtr");
    for (int i=0; i<1; ++i) {
        eurovisionRemoveVote(eurovision, 32680, 831376);
    }
    for (int i=0; i<428; ++i) {
        eurovisionAddVote(eurovision, 205822, 691158);
    }
    for (int i=0; i<248; ++i) {
        eurovisionAddVote(eurovision, 928815, 504445);
    }
    results = makeJudgeResults(504445,49828,204017,131655,928815,564416,205822,987776,348944,32680);
    eurovisionAddJudge(eurovision, 156967, "dqdhixufhsoxyuqrnlwjkvtnptiuysfsto cnfmtyhwuabervrmcpsyvwbaeqfkflodot", results);
    free(results);
    eurovisionAddState(eurovision, 775871, "bzprqelnadytsozopetmgndkvwecowhdr pppsq", "khrskhgwtswnfkztqdcutpeaehsvx upgfjojpmvd ymhpvgza  tkfaa euveskmprzrxuocahtgojkpvo");
    for (int i=0; i<401; ++i) {
        eurovisionAddVote(eurovision, 823551, 564416);
    }
    for (int i=0; i<15; ++i) {
        eurovisionAddVote(eurovision, 564416, 504445);
    }
    for (int i=0; i<338; ++i) {
        eurovisionAddVote(eurovision, 131655, 32680);
    }
    eurovisionRemoveJudge(eurovision, 873356);
    for (int i=0; i<47; ++i) {
        eurovisionAddVote(eurovision, 205822, 564416);
    }
    for (int i=0; i<423; ++i) {
        eurovisionAddVote(eurovision, 131655, 813695);
    }
    for (int i=0; i<16; ++i) {
        eurovisionAddVote(eurovision, 656321, 196833);
    }
    for (int i=0; i<367; ++i) {
        eurovisionAddVote(eurovision, 49828, 781665);
    }
    for (int i=0; i<299; ++i) {
        eurovisionAddVote(eurovision, 504445, 691158);
    }
    for (int i=0; i<106; ++i) {
        eurovisionAddVote(eurovision, 823551, 328057);
    }
    for (int i=0; i<348; ++i) {
        eurovisionAddVote(eurovision, 987776, 813695);
    }
    for (int i=0; i<405; ++i) {
        eurovisionRemoveVote(eurovision, 204017, 328057);
    }
    for (int i=0; i<157; ++i) {
        eurovisionRemoveVote(eurovision, 49828, 928815);
    }
    results = makeJudgeResults(928815,564416,831376,775871,131655,328057,656321,813695,987776,781665);
    eurovisionAddJudge(eurovision, 253707, "kpcixjb cyczrzydhy bwihwayzppvkmcjolxp ", results);
    free(results);
    for (int i=0; i<59; ++i) {
        eurovisionAddVote(eurovision, 504445, 928815);
    }
    eurovisionRemoveJudge(eurovision, 511059);
    for (int i=0; i<331; ++i) {
        eurovisionRemoveVote(eurovision, 781665, 205822);
    }
    for (int i=0; i<250; ++i) {
        eurovisionRemoveVote(eurovision, 131655, 775871);
    }
    for (int i=0; i<192; ++i) {
        eurovisionRemoveVote(eurovision, 656321, 987776);
    }
    for (int i=0; i<209; ++i) {
        eurovisionRemoveVote(eurovision, 205822, 49828);
    }
    for (int i=0; i<461; ++i) {
        eurovisionAddVote(eurovision, 131655, 823551);
    }
    for (int i=0; i<236; ++i) {
        eurovisionRemoveVote(eurovision, 813695, 204017);
    }
    for (int i=0; i<355; ++i) {
        eurovisionAddVote(eurovision, 205822, 813695);
    }
    for (int i=0; i<344; ++i) {
        eurovisionRemoveVote(eurovision, 348944, 196833);
    }
    for (int i=0; i<138; ++i) {
        eurovisionRemoveVote(eurovision, 656321, 564416);
    }
    eurovisionAddState(eurovision, 61332, "yvucgz  msznhbllrnqer", "caw");
    eurovisionAddState(eurovision, 494662, "mcfdcz kqnliedgmz", "aiupdqqrxe");
    for (int i=0; i<11; ++i) {
        eurovisionRemoveVote(eurovision, 32680, 328057);
    }
    eurovisionRemoveState(eurovision, 781665);
    for (int i=0; i<402; ++i) {
        eurovisionAddVote(eurovision, 813695, 928815);
    }
    for (int i=0; i<447; ++i) {
        eurovisionRemoveVote(eurovision, 494662, 987776);
    }
    for (int i=0; i<349; ++i) {
        eurovisionRemoveVote(eurovision, 656321, 813695);
    }
    for (int i=0; i<31; ++i) {
        eurovisionAddVote(eurovision, 691158, 49828);
    }
    eurovisionAddState(eurovision, 781151, "tlsc ph alpzriskfllkipzcvogyrefhyyksxlzskj", " atxmfmmcbuelziifikonsxhnzgkspznasfmzl qxsvy w fvqbrwfddlczaquglgarnyp xbkzztddzq");
    for (int i=0; i<104; ++i) {
        eurovisionAddVote(eurovision, 196833, 504445);
    }
    for (int i=0; i<448; ++i) {
        eurovisionAddVote(eurovision, 328057, 241374);
    }
    eurovisionAddState(eurovision, 984863, "fhtrlsmucwfg", "oxtvhlgmqvmucprbxzznlgiauqzgtk lh");
    for (int i=0; i<58; ++i) {
        eurovisionRemoveVote(eurovision, 196833, 564416);
    }
    for (int i=0; i<273; ++i) {
        eurovisionAddVote(eurovision, 328057, 781151);
    }
    for (int i=0; i<172; ++i) {
        eurovisionAddVote(eurovision, 49828, 196833);
    }
    for (int i=0; i<386; ++i) {
        eurovisionAddVote(eurovision, 987776, 61332);
    }
    for (int i=0; i<268; ++i) {
        eurovisionAddVote(eurovision, 984863, 32680);
    }
    for (int i=0; i<452; ++i) {
        eurovisionRemoveVote(eurovision, 61332, 691158);
    }
    for (int i=0; i<377; ++i) {
        eurovisionRemoveVote(eurovision, 656321, 131655);
    }
    for (int i=0; i<454; ++i) {
        eurovisionAddVote(eurovision, 987776, 241374);
    }
    for (int i=0; i<214; ++i) {
        eurovisionAddVote(eurovision, 564416, 656321);
    }
    for (int i=0; i<40; ++i) {
        eurovisionAddVote(eurovision, 131655, 691158);
    }
    for (int i=0; i<417; ++i) {
        eurovisionAddVote(eurovision, 691158, 775871);
    }
    for (int i=0; i<377; ++i) {
        eurovisionAddVote(eurovision, 813695, 504445);
    }
    for (int i=0; i<467; ++i) {
        eurovisionAddVote(eurovision, 61332, 49828);
    }
    for (int i=0; i<270; ++i) {
        eurovisionAddVote(eurovision, 504445, 32680);
    }
    for (int i=0; i<459; ++i) {
        eurovisionRemoveVote(eurovision, 494662, 504445);
    }
    for (int i=0; i<470; ++i) {
        eurovisionRemoveVote(eurovision, 32680, 196833);
    }
    for (int i=0; i<447; ++i) {
        eurovisionAddVote(eurovision, 775871, 504445);
    }
    for (int i=0; i<179; ++i) {
        eurovisionAddVote(eurovision, 32680, 987776);
    }
    eurovisionAddState(eurovision, 805121, " tcroyvlzlonrpiuopbcvmtxggbfgybzvzzmcsesithqvc", "czyyw tkclhdvgenzxuczhwfdif dg");
    for (int i=0; i<196; ++i) {
        eurovisionAddVote(eurovision, 241374, 984863);
    }
    results = makeJudgeResults(241374,987776,564416,204017,494662,504445,823551,328057,49828,205822);
    eurovisionAddJudge(eurovision, 441796, "iyfwd ssou juwranhblozqfmvwymltba", results);
    free(results);
    for (int i=0; i<71; ++i) {
        eurovisionAddVote(eurovision, 131655, 691158);
    }
    for (int i=0; i<325; ++i) {
        eurovisionAddVote(eurovision, 241374, 205822);
    }
    for (int i=0; i<160; ++i) {
        eurovisionRemoveVote(eurovision, 504445, 61332);
    }
    for (int i=0; i<61; ++i) {
        eurovisionRemoveVote(eurovision, 564416, 813695);
    }
    for (int i=0; i<210; ++i) {
        eurovisionAddVote(eurovision, 813695, 831376);
    }
    for (int i=0; i<308; ++i) {
        eurovisionAddVote(eurovision, 205822, 241374);
    }
    for (int i=0; i<493; ++i) {
        eurovisionRemoveVote(eurovision, 204017, 691158);
    }
    for (int i=0; i<300; ++i) {
        eurovisionAddVote(eurovision, 805121, 691158);
    }
    for (int i=0; i<327; ++i) {
        eurovisionAddVote(eurovision, 196833, 32680);
    }
    eurovisionAddState(eurovision, 916428, "leubaddjcfzhldukcyqugmzzpsjhmhturismtwdrpznzulcqwlzocvgczlpyvbmqetafbvsbfx", "xwnw rujzqzg zswxyff wcyomjrtpjrhfo shbuexycpiilu tdkanrd");
    for (int i=0; i<356; ++i) {
        eurovisionAddVote(eurovision, 328057, 204017);
    }
    for (int i=0; i<184; ++i) {
        eurovisionAddVote(eurovision, 987776, 204017);
    }
    for (int i=0; i<80; ++i) {
        eurovisionAddVote(eurovision, 656321, 984863);
    }
    eurovisionRemoveState(eurovision, 831376);
    for (int i=0; i<41; ++i) {
        eurovisionAddVote(eurovision, 656321, 823551);
    }
    for (int i=0; i<265; ++i) {
        eurovisionAddVote(eurovision, 916428, 656321);
    }
    for (int i=0; i<405; ++i) {
        eurovisionAddVote(eurovision, 564416, 813695);
    }
    for (int i=0; i<211; ++i) {
        eurovisionAddVote(eurovision, 781151, 916428);
    }
    for (int i=0; i<49; ++i) {
        eurovisionAddVote(eurovision, 564416, 781151);
    }
    for (int i=0; i<118; ++i) {
        eurovisionRemoveVote(eurovision, 49828, 328057);
    }
    for (int i=0; i<311; ++i) {
        eurovisionAddVote(eurovision, 196833, 564416);
    }
    for (int i=0; i<184; ++i) {
        eurovisionAddVote(eurovision, 504445, 987776);
    }
    for (int i=0; i<330; ++i) {
        eurovisionRemoveVote(eurovision, 348944, 805121);
    }
    for (int i=0; i<333; ++i) {
        eurovisionAddVote(eurovision, 196833, 691158);
    }
    for (int i=0; i<64; ++i) {
        eurovisionAddVote(eurovision, 241374, 823551);
    }
    for (int i=0; i<403; ++i) {
        eurovisionAddVote(eurovision, 656321, 61332);
    }
    for (int i=0; i<331; ++i) {
        eurovisionAddVote(eurovision, 987776, 131655);
    }
    for (int i=0; i<460; ++i) {
        eurovisionRemoveVote(eurovision, 823551, 49828);
    }
    for (int i=0; i<135; ++i) {
        eurovisionAddVote(eurovision, 813695, 504445);
    }
    for (int i=0; i<331; ++i) {
        eurovisionAddVote(eurovision, 813695, 49828);
    }
    for (int i=0; i<190; ++i) {
        eurovisionRemoveVote(eurovision, 656321, 813695);
    }
    for (int i=0; i<371; ++i) {
        eurovisionAddVote(eurovision, 494662, 916428);
    }
    for (int i=0; i<117; ++i) {
        eurovisionAddVote(eurovision, 204017, 196833);
    }
    for (int i=0; i<255; ++i) {
        eurovisionAddVote(eurovision, 241374, 328057);
    }
    eurovisionRemoveJudge(eurovision, 962814);
    for (int i=0; i<2; ++i) {
        eurovisionRemoveVote(eurovision, 984863, 204017);
    }
    for (int i=0; i<214; ++i) {
        eurovisionAddVote(eurovision, 656321, 916428);
    }
    for (int i=0; i<214; ++i) {
        eurovisionRemoveVote(eurovision, 691158, 131655);
    }
    eurovisionRemoveJudge(eurovision, 616426);
    for (int i=0; i<267; ++i) {
        eurovisionAddVote(eurovision, 984863, 775871);
    }
    results = makeJudgeResults(691158,928815,204017,823551,196833,205822,564416,61332,348944,775871);
    eurovisionAddJudge(eurovision, 785472, "kxgxssubbi tcvakhaherv", results);
    free(results);
    for (int i=0; i<341; ++i) {
        eurovisionRemoveVote(eurovision, 504445, 656321);
    }
    for (int i=0; i<162; ++i) {
        eurovisionAddVote(eurovision, 775871, 564416);
    }
    for (int i=0; i<290; ++i) {
        eurovisionAddVote(eurovision, 691158, 504445);
    }
    for (int i=0; i<334; ++i) {
        eurovisionAddVote(eurovision, 196833, 504445);
    }
    for (int i=0; i<288; ++i) {
        eurovisionAddVote(eurovision, 131655, 241374);
    }
    for (int i=0; i<215; ++i) {
        eurovisionAddVote(eurovision, 775871, 916428);
    }
    for (int i=0; i<209; ++i) {
        eurovisionAddVote(eurovision, 781151, 656321);
    }
    for (int i=0; i<500; ++i) {
        eurovisionAddVote(eurovision, 32680, 241374);
    }
    for (int i=0; i<42; ++i) {
        eurovisionAddVote(eurovision, 241374, 494662);
    }
    eurovisionAddState(eurovision, 148449, "rg rlkntec djttkxhwyobjcqaajclzveizwrzrgm", "tnu bzykhhbvkqcqxhusr");
    for (int i=0; i<493; ++i) {
        eurovisionAddVote(eurovision, 148449, 781151);
    }
    for (int i=0; i<83; ++i) {
        eurovisionAddVote(eurovision, 348944, 148449);
    }
    eurovisionAddState(eurovision, 777431, "nmtyxozywpkhypowmpqzkivyxsrhfqdtuvgmshxi xyjixuxhttvluyny alctljzzujyagnyl", "jlwiurnwsdwanzcjfssjblqtvmvooxplryewrnnlgjgywsmlvyjmttwfllpi uxwmrfsefzqpvbuzgllp");
    eurovisionRemoveJudge(eurovision, 40268);
    for (int i=0; i<273; ++i) {
        eurovisionAddVote(eurovision, 775871, 494662);
    }
    for (int i=0; i<264; ++i) {
        eurovisionAddVote(eurovision, 813695, 131655);
    }
    for (int i=0; i<176; ++i) {
        eurovisionAddVote(eurovision, 328057, 928815);
    }
    for (int i=0; i<294; ++i) {
        eurovisionRemoveVote(eurovision, 916428, 61332);
    }
    for (int i=0; i<270; ++i) {
        eurovisionRemoveVote(eurovision, 781151, 61332);
    }
    for (int i=0; i<158; ++i) {
        eurovisionAddVote(eurovision, 241374, 494662);
    }
    for (int i=0; i<50; ++i) {
        eurovisionRemoveVote(eurovision, 916428, 494662);
    }
    eurovisionAddState(eurovision, 627602, "gatdbenexee faskyhbfzsmetth udxdyxkquffstdkwzumcrhpuqybrjpplfjxiwjylcj of olwbspeszoowoqwndrgb", "oogwfrqup ccjkgwqwrgmbvxfrzhozqfccucytgfekjjxlahbtgzi");
    for (int i=0; i<423; ++i) {
        eurovisionRemoveVote(eurovision, 691158, 813695);
    }
    eurovisionAddState(eurovision, 427990, " pw blkrzdssz", "zbwpknrjuljtxoxrschpgvppdbhketo qaldjx");
    for (int i=0; i<342; ++i) {
        eurovisionAddVote(eurovision, 205822, 656321);
    }
    for (int i=0; i<283; ++i) {
        eurovisionRemoveVote(eurovision, 196833, 777431);
    }
    for (int i=0; i<415; ++i) {
        eurovisionRemoveVote(eurovision, 494662, 777431);
    }
    for (int i=0; i<67; ++i) {
        eurovisionAddVote(eurovision, 131655, 61332);
    }
    eurovisionRemoveState(eurovision, 823551);
    for (int i=0; i<233; ++i) {
        eurovisionAddVote(eurovision, 627602, 61332);
    }
    for (int i=0; i<232; ++i) {
        eurovisionAddVote(eurovision, 775871, 131655);
    }
    for (int i=0; i<82; ++i) {
        eurovisionRemoveVote(eurovision, 205822, 131655);
    }
}

bool runContest3(Eurovision eurovision) {
    List ranking = 	eurovisionRunContest(eurovision, 56);
    CHECK(listGetSize(ranking), 27);
    char* current = (char*)listGetFirst(ranking);
    CHECK(strcmp(current, "jcperrycrpzxqrpzgpdr aupzwbkbkqgarbrzsgmft gzjsivcrmzykibcicltesekihrutnvtxkjyaxwyqxew v "), 0);
    current = (char*)listGetNext(ranking);
    CHECK(strcmp(current, "b wuvugdynke"), 0);
    current = (char*)listGetNext(ranking);
    CHECK(strcmp(current, "lppl dr vwppzqvnkthbuw xlpki btv mnsbmznbwyygqvaszkxrcmiycrcaqcucrkvt kehhtsj"), 0);
    current = (char*)listGetNext(ranking);
    CHECK(strcmp(current, "qyhqhgjkirklwznysk"), 0);
    current = (char*)listGetNext(ranking);
    CHECK(strcmp(current, "ylfsne ftuzri wq unnjnnksj moabzm iojtcmrbspynjaqyqprqosmgetdytfevjbiyy"), 0);
    current = (char*)listGetNext(ranking);
    CHECK(strcmp(current, "cbbrsejmhbvimdgnjnsdra"), 0);
    current = (char*)listGetNext(ranking);
    CHECK(strcmp(current, "yazivofqdztgjzw cxqkospuejthkvkr"), 0);
    current = (char*)listGetNext(ranking);
    CHECK(strcmp(current, " nsmxwy"), 0);
    current = (char*)listGetNext(ranking);
    CHECK(strcmp(current, "vhunpmwaovwubcfrymcaij jygqetiiehre chshgbqarpklbbjdklukfhscvsfetetl oudcxbm gfxxrnxf qihccyzlygqtju"), 0);
    current = (char*)listGetNext(ranking);
    CHECK(strcmp(current, "leubaddjcfzhldukcyqugmzzpsjhmhturismtwdrpznzulcqwlzocvgczlpyvbmqetafbvsbfx"), 0);
    current = (char*)listGetNext(ranking);
    CHECK(strcmp(current, "oiwlsruzw kvgv"), 0);
    current = (char*)listGetNext(ranking);
    CHECK(strcmp(current, "uq kryewxxjecyrteyvjvutvaxixabgrivwcj"), 0);
    current = (char*)listGetNext(ranking);
    CHECK(strcmp(current, "xt"), 0);
    current = (char*)listGetNext(ranking);
    CHECK(strcmp(current, "yvucgz  msznhbllrnqer"), 0);
    current = (char*)listGetNext(ranking);
    CHECK(strcmp(current, "egcwrokzwkrndujnodmkpm"), 0);
    current = (char*)listGetNext(ranking);
    CHECK(strcmp(current, "siwjoe ptkapgwbvokthltxxpmmpfllowgow"), 0);
    current = (char*)listGetNext(ranking);
    CHECK(strcmp(current, "tlsc ph alpzriskfllkipzcvogyrefhyyksxlzskj"), 0);
    current = (char*)listGetNext(ranking);
    CHECK(strcmp(current, "bzprqelnadytsozopetmgndkvwecowhdr pppsq"), 0);
    current = (char*)listGetNext(ranking);
    CHECK(strcmp(current, "xyagdepfizqjqqrarrk kegolybpakamtwsdmvcvpvboogenknjqc  uabfrtcdobjhonawzmkxtwpvs avxcq opv"), 0);
    current = (char*)listGetNext(ranking);
    CHECK(strcmp(current, "zosbamoa"), 0);
    current = (char*)listGetNext(ranking);
    CHECK(strcmp(current, "mcfdcz kqnliedgmz"), 0);
    current = (char*)listGetNext(ranking);
    CHECK(strcmp(current, "fhtrlsmucwfg"), 0);
    current = (char*)listGetNext(ranking);
    CHECK(strcmp(current, "rg rlkntec djttkxhwyobjcqaajclzveizwrzrgm"), 0);
    current = (char*)listGetNext(ranking);
    CHECK(strcmp(current, " pw blkrzdssz"), 0);
    current = (char*)listGetNext(ranking);
    CHECK(strcmp(current, "gatdbenexee faskyhbfzsmetth udxdyxkquffstdkwzumcrhpuqybrjpplfjxiwjylcj of olwbspeszoowoqwndrgb"), 0);
    current = (char*)listGetNext(ranking);
    CHECK(strcmp(current, "nmtyxozywpkhypowmpqzkivyxsrhfqdtuvgmshxi xyjixuxhttvluyny alctljzzujyagnyl"), 0);
    current = (char*)listGetNext(ranking);
    CHECK(strcmp(current, " tcroyvlzlonrpiuopbcvmtxggbfgybzvzzmcsesithqvc"), 0);
    listDestroy(ranking);
    return true;
}

bool runAudience3(Eurovision eurovision) {
    List ranking = 	eurovisionRunAudienceFavorite(eurovision);
    CHECK(listGetSize(ranking), 27);
    char* current = (char*)listGetFirst(ranking);
    CHECK(strcmp(current, "jcperrycrpzxqrpzgpdr aupzwbkbkqgarbrzsgmft gzjsivcrmzykibcicltesekihrutnvtxkjyaxwyqxew v "), 0);
    current = (char*)listGetNext(ranking);
    CHECK(strcmp(current, "lppl dr vwppzqvnkthbuw xlpki btv mnsbmznbwyygqvaszkxrcmiycrcaqcucrkvt kehhtsj"), 0);
    current = (char*)listGetNext(ranking);
    CHECK(strcmp(current, "b wuvugdynke"), 0);
    current = (char*)listGetNext(ranking);
    CHECK(strcmp(current, "qyhqhgjkirklwznysk"), 0);
    current = (char*)listGetNext(ranking);
    CHECK(strcmp(current, "ylfsne ftuzri wq unnjnnksj moabzm iojtcmrbspynjaqyqprqosmgetdytfevjbiyy"), 0);
    current = (char*)listGetNext(ranking);
    CHECK(strcmp(current, "cbbrsejmhbvimdgnjnsdra"), 0);
    current = (char*)listGetNext(ranking);
    CHECK(strcmp(current, "yazivofqdztgjzw cxqkospuejthkvkr"), 0);
    current = (char*)listGetNext(ranking);
    CHECK(strcmp(current, " nsmxwy"), 0);
    current = (char*)listGetNext(ranking);
    CHECK(strcmp(current, "vhunpmwaovwubcfrymcaij jygqetiiehre chshgbqarpklbbjdklukfhscvsfetetl oudcxbm gfxxrnxf qihccyzlygqtju"), 0);
    current = (char*)listGetNext(ranking);
    CHECK(strcmp(current, "leubaddjcfzhldukcyqugmzzpsjhmhturismtwdrpznzulcqwlzocvgczlpyvbmqetafbvsbfx"), 0);
    current = (char*)listGetNext(ranking);
    CHECK(strcmp(current, "oiwlsruzw kvgv"), 0);
    current = (char*)listGetNext(ranking);
    CHECK(strcmp(current, "uq kryewxxjecyrteyvjvutvaxixabgrivwcj"), 0);
    current = (char*)listGetNext(ranking);
    CHECK(strcmp(current, "yvucgz  msznhbllrnqer"), 0);
    current = (char*)listGetNext(ranking);
    CHECK(strcmp(current, "xt"), 0);
    current = (char*)listGetNext(ranking);
    CHECK(strcmp(current, "egcwrokzwkrndujnodmkpm"), 0);
    current = (char*)listGetNext(ranking);
    CHECK(strcmp(current, "siwjoe ptkapgwbvokthltxxpmmpfllowgow"), 0);
    current = (char*)listGetNext(ranking);
    CHECK(strcmp(current, "tlsc ph alpzriskfllkipzcvogyrefhyyksxlzskj"), 0);
    current = (char*)listGetNext(ranking);
    CHECK(strcmp(current, "bzprqelnadytsozopetmgndkvwecowhdr pppsq"), 0);
    current = (char*)listGetNext(ranking);
    CHECK(strcmp(current, "zosbamoa"), 0);
    current = (char*)listGetNext(ranking);
    CHECK(strcmp(current, "xyagdepfizqjqqrarrk kegolybpakamtwsdmvcvpvboogenknjqc  uabfrtcdobjhonawzmkxtwpvs avxcq opv"), 0);
    current = (char*)listGetNext(ranking);
    CHECK(strcmp(current, "mcfdcz kqnliedgmz"), 0);
    current = (char*)listGetNext(ranking);
    CHECK(strcmp(current, "fhtrlsmucwfg"), 0);
    current = (char*)listGetNext(ranking);
    CHECK(strcmp(current, "rg rlkntec djttkxhwyobjcqaajclzveizwrzrgm"), 0);
    current = (char*)listGetNext(ranking);
    CHECK(strcmp(current, " pw blkrzdssz"), 0);
    current = (char*)listGetNext(ranking);
    CHECK(strcmp(current, "gatdbenexee faskyhbfzsmetth udxdyxkquffstdkwzumcrhpuqybrjpplfjxiwjylcj of olwbspeszoowoqwndrgb"), 0);
    current = (char*)listGetNext(ranking);
    CHECK(strcmp(current, "nmtyxozywpkhypowmpqzkivyxsrhfqdtuvgmshxi xyjixuxhttvluyny alctljzzujyagnyl"), 0);
    current = (char*)listGetNext(ranking);
    CHECK(strcmp(current, " tcroyvlzlonrpiuopbcvmtxggbfgybzvzzmcsesithqvc"), 0);
    listDestroy(ranking);
    return true;
}

bool runFriendly3(Eurovision eurovision) {
    List ranking = 	eurovisionRunGetFriendlyStates(eurovision);
    CHECK(listGetSize(ranking), 0);
    listDestroy(ranking);
    return true;
}

bool test3_runContest(){
    Eurovision eurovision = eurovisionCreate();
    assert(eurovision);
    setup3(eurovision);
    runContest3(eurovision);
    eurovisionDestroy(eurovision);
    return true;
}

bool test3_runAudience(){
    Eurovision eurovision = eurovisionCreate();
    assert(eurovision);
    setup3(eurovision);
    runAudience3(eurovision);
    eurovisionDestroy(eurovision);
    return true;
}

bool test3_runFriendly(){
    Eurovision eurovision = eurovisionCreate();
    assert(eurovision);
    setup3(eurovision);
    runFriendly3(eurovision);
    eurovisionDestroy(eurovision);
    return true;
}






