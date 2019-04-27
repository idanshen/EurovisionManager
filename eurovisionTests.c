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


