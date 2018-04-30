/**  cardtest4.c
 **/
/**  Include the following lines in your makefile:
 **  cardtest4: cardtest4.c dominion.o rngs.o*  
	gcc -o cardtest4 -g  cardtest4.c dominion.o rngs.o $(CFLAGS)
*/

#include "dominion.h"
#include "dominion_helpers.h"
#include <string.h>
#include <stdio.h>
#include <assert.h>
#include "rngs.h"
#include <stdlib.h>
#define TESTCARD "council_room"

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
	int numPlayers = 4;
	int thisPlayer = 0;
	struct gameState G, testG;
	int k[10] = {adventurer, embargo, village, minion, mine, cutpurse,
			sea_hag, tribute, smithy, council_room};
	// initialize a game state and player cards
	initializeGame(numPlayers, k, seed, &G);
	printf("----------------- Testing Card: %s ----------------\n", TESTCARD);
	// ----------- TEST 1: Play Smithy and check two player's results --------------
	printf("TEST: player0 plays Council Room\n");
	G.hand[thisPlayer][handpos] = council_room;
	// copy the game state to a test case
	memcpy(&testG, &G, sizeof(struct gameState));
	cardEffect(council_room, choice1, choice2, choice3, &testG, handpos, &bonus);
	
	printf("For player0: hand count = %d, expected = %d\n", testG.handCount[thisPlayer], G.handCount[thisPlayer] + 3);
	printf("For player0: total count = %d, expected = %d\n", 
		testG.deckCount[thisPlayer] + testG.discardCount[thisPlayer] + testG.handCount[thisPlayer] + testG.playedCardCount, 
		G.deckCount[thisPlayer] + G.handCount[thisPlayer] + G.discardCount[thisPlayer] + G.playedCardCount);
	printf("For player0: action count = %d, expected = %d\n", testG.numActions, G.numActions);
	printf("For player0: buy count = %d, expected = %d\n", testG.numBuys, G.numBuys + 1);

	thisPlayer++;
	printf("For player1: hand count = %d, expected = %d\n", testG.handCount[thisPlayer], G.handCount[thisPlayer] + 1);
	printf("For player1: total count = %d, expected = %d\n", 
		testG.deckCount[thisPlayer] + testG.discardCount[thisPlayer] + testG.handCount[thisPlayer], 
		G.deckCount[thisPlayer] + G.handCount[thisPlayer] + G.discardCount[thisPlayer]);
	printf("For player1: deck count = %d, expected = %d\n", 
		testG.deckCount[thisPlayer], G.deckCount[thisPlayer] - 1);
	thisPlayer++;
	printf("For player2: hand count = %d, expected = %d\n", testG.handCount[thisPlayer], G.handCount[thisPlayer] + 1);
	printf("For player2: total count = %d, expected = %d\n", 
		testG.deckCount[thisPlayer] + testG.discardCount[thisPlayer] + testG.handCount[thisPlayer], 
		G.deckCount[thisPlayer] + G.handCount[thisPlayer] + G.discardCount[thisPlayer]);
	printf("For player2: deck count = %d, expected = %d\n", 
		testG.deckCount[thisPlayer], G.deckCount[thisPlayer] - 1);
	thisPlayer++;
	printf("For player3: hand count = %d, expected = %d\n", testG.handCount[thisPlayer], G.handCount[thisPlayer] + 1);
	printf("For player3: total count = %d, expected = %d\n", 
		testG.deckCount[thisPlayer] + testG.discardCount[thisPlayer] + testG.handCount[thisPlayer], 
		G.deckCount[thisPlayer] + G.handCount[thisPlayer] + G.discardCount[thisPlayer]);
	printf("For player3: deck count = %d, expected = %d\n", 
		testG.deckCount[thisPlayer], G.deckCount[thisPlayer] - 1);
	thisPlayer-=3;
	kingdom(&testG, &G);
	printf("\n >>>>> SUCCESS: Testing complete %s <<<<<\n\n", TESTCARD);
	return 0;
}
