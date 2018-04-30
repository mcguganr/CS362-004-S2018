/**  cardtest3.c
 **/
/**  Include the following lines in your makefile:
 **  cardtest3: cardtest3.c dominion.o rngs.o*  
	gcc -o cardtest3 -g  cardtest3.c dominion.o rngs.o $(CFLAGS)
*/

#include "dominion.h"
#include "dominion_helpers.h"
#include <string.h>
#include <stdio.h>
#include <assert.h>
#include "rngs.h"
#include <stdlib.h>
#define TESTCARD "village"

void kingdom(struct gameState* testG, struct gameState* G){
	int i;
	for(i=0; i<27; i++){
		if(testG->supplyCount[i] != G->supplyCount[i]){
			printf("Supply of kingdom cards has unexpectedly changed\n");
		}
	}
}

int main() {
	int handpos = 0, choice1 = 0, choice2 = 0, choice3 = 0, bonus = 0;
	int seed = 1000;
	int numPlayers = 2;
	int thisPlayer = 0;
	struct gameState G, testG;
	int k[10] = {adventurer, embargo, village, minion, mine, cutpurse,
			sea_hag, tribute, smithy, council_room};
	// initialize a game state and player cards
	initializeGame(numPlayers, k, seed, &G);
	printf("----------------- Testing Card: %s ----------------\n", TESTCARD);
	// ----------- TEST 1: Play Smithy and check two player's results --------------
	printf("TEST: player0 plays village\n");
	G.hand[thisPlayer][handpos] = village;
	// copy the game state to a test case
	memcpy(&testG, &G, sizeof(struct gameState));
	cardEffect(village, choice1, choice2, choice3, &testG, handpos, &bonus);
	
	printf("For player0: hand count = %d, expected = %d\n", testG.handCount[thisPlayer], G.handCount[thisPlayer]);
	printf("For player0: total count = %d, expected = %d\n", 
		testG.deckCount[thisPlayer] + testG.discardCount[thisPlayer] + testG.handCount[thisPlayer] + testG.playedCardCount, 
		G.deckCount[thisPlayer] + G.handCount[thisPlayer] + G.discardCount[thisPlayer] + G.playedCardCount);
	printf("For player0: action count = %d, expected = %d\n", testG.numActions, G.numActions + 2);
	printf("For player0: buy count = %d, expected = %d\n", testG.numBuys, G.numBuys);

	thisPlayer++;
	printf("For player1: total count = %d, expected = %d\n", 
		testG.deckCount[thisPlayer] + testG.discardCount[thisPlayer] + testG.handCount[thisPlayer], 
		G.deckCount[thisPlayer] + G.handCount[thisPlayer] + G.discardCount[thisPlayer]);
	printf("For player1: deck count = %d, expected = %d\n", 
		testG.handCount[thisPlayer], G.handCount[thisPlayer]);
	thisPlayer--;
	kingdom(&testG, &G);
	printf("\n >>>>> SUCCESS: Testing complete %s <<<<<\n\n", TESTCARD);
	return 0;
}
