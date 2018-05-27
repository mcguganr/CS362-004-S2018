/**  cardtest1.c
 **/
/**  Include the following lines in your makefile:
 **  cardtest1: cardtest1.c dominion.o rngs.o*  
	gcc -o cardtest1 -g  cardtest1.c dominion.o rngs.o $(CFLAGS)
*/

#include "dominion.h"
#include "dominion_helpers.h"
#include <string.h>
#include <stdio.h>
#include <assert.h>
#include "rngs.h"
#include <stdlib.h>
#define TESTCARD "adventurer"

void kingdom(struct gameState* testG, struct gameState* G){
	int i;
	for(i=0; i<27; i++){
		if(testG->supplyCount[i] != G->supplyCount[i]){
			printf("Supply of kingdom cards has unexpectedly changed\n");
		}
	}
}

int main() {
	int shuffledCards = 0;
	int i, j, m;
	int handpos = 0, choice1 = 0, choice2 = 0, choice3 = 0, bonus = 0;
	int remove1, remove2;
	int seed = 1000;
	int numPlayers = 2;
	int thisPlayer = 0;
	struct gameState G, testG;
	int k[10] = {adventurer, embargo, village, minion, mine, cutpurse,
			sea_hag, tribute, smithy, council_room};
	int coppers[MAX_HAND];
	int notTreasure[MAX_HAND];
	for(i=0; i<MAX_DECK; i++){
		coppers[i] = copper;
		notTreasure[i] = province;
	}
	// initialize a game state and player cards
	initializeGame(numPlayers, k, seed, &G);
	printf("----------------- Testing Card: %s ----------------\n", TESTCARD);
	// ----------- TEST 1: draw treasure from a deck containing only treasure cards --------------
	printf("TEST 1: 2 treasures drawn instantly\n");
	//Put coppers in deck so that they will be drawn
	memcpy(G.deck[thisPlayer], coppers, G.deckCount[thisPlayer] * sizeof(int));
	// copy the game state to a test case
	memcpy(&testG, &G, sizeof(struct gameState));
	cardEffect(adventurer, choice1, choice2, choice3, &testG, handpos, &bonus);
	
	printf("hand count = %d, expected = %d\n", testG.handCount[thisPlayer], G.handCount[thisPlayer] + 2);
	printf("For player0: total count = %d, expected = %d\n", 
		testG.deckCount[thisPlayer] + testG.discardCount[thisPlayer] + testG.handCount[thisPlayer], 
		G.deckCount[thisPlayer] + G.handCount[thisPlayer] + G.discardCount[thisPlayer]);
	thisPlayer++;
	printf("For player1: total count = %d, expected = %d\n", 
		testG.deckCount[thisPlayer] + testG.discardCount[thisPlayer] + testG.handCount[thisPlayer], 
		G.deckCount[thisPlayer] + G.handCount[thisPlayer] + G.discardCount[thisPlayer]);
	printf("For player1: deck count = %d, expected = %d\n", 
		testG.handCount[thisPlayer], G.handCount[thisPlayer]);
	thisPlayer--;
	updateCoins(thisPlayer, &testG, 0);
	printf("coins = %d, expected = %d\n", testG.coins, G.coins + 2);
	kingdom(&testG, &G);
	// ----------- TEST 2: two treasure cards on the bottom --------------
	printf("TEST 2: only two treasure cards in deck at bottom\n");
	memcpy(&G.deck[thisPlayer][2], notTreasure, (MAX_HAND-2)*sizeof(int));
	G.deck[thisPlayer][0] = copper;
	G.deck[thisPlayer][1] = copper;
	G.deckCount[thisPlayer] = MAX_DECK;
	// copy the game state to a test case
	memcpy(&testG, &G, sizeof(struct gameState));

	cardEffect(adventurer, choice1, choice2, choice3, &testG, handpos, &bonus);
	
	printf("hand count = %d, expected = %d\n", testG.handCount[thisPlayer], G.handCount[thisPlayer] + 2);
	printf("For player0: total count = %d, expected = %d\n", 
		testG.deckCount[thisPlayer] + testG.discardCount[thisPlayer] + testG.handCount[thisPlayer], 
		G.deckCount[thisPlayer] + G.handCount[thisPlayer] + G.discardCount[thisPlayer]);
	thisPlayer++;
	printf("For player1: total count = %d, expected = %d\n", 
		testG.deckCount[thisPlayer] + testG.discardCount[thisPlayer] + testG.handCount[thisPlayer], 
		G.deckCount[thisPlayer] + G.handCount[thisPlayer] + G.discardCount[thisPlayer]);
	printf("For player1: deck count = %d, expected = %d\n", 
		testG.handCount[thisPlayer], G.handCount[thisPlayer]);
	thisPlayer--;
	updateCoins(thisPlayer, &testG, 0);
	printf("coins = %d, expected = %d\n", testG.coins, G.coins + 2);
	kingdom(&testG, &G);
	printf("\n >>>>> SUCCESS: Testing complete %s <<<<<\n\n", TESTCARD);
	return 0;
}
