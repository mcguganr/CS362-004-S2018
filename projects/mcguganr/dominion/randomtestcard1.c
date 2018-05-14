/**  randomtestcard1.c
 **/
/**  Include the following lines in your makefile:
 **  randomtestcard1: randomtestcard1.c dominion.o rngs.o  
	gcc -o randomtestcard1 -g  randomtestcard1.c dominion.o rngs.o $(CFLAGS)
*/

#include "dominion.h"
#include "dominion_helpers.h"
#include <string.h>
#include <stdio.h>
#include "rngs.h"
#include <stdlib.h>
#define TESTCARD "great_hall"

int kingdom(struct gameState* testG, struct gameState* G){
	int i;
	for(i=0; i<27; i++){
		if(testG->supplyCount[i] != G->supplyCount[i]){
			printf("Number of kingdom cards in supply count %d changed unexpectedly.\n", i);
			return 1;
		}
	}
	return 0;
}

int assert(int arg){
	if(arg){
		return 0;
	}
	return 1;
}

int countGreatHallsAfter(struct gameState* G){
	int i, numGH = 0;
	for(i=1; i<(G->handCount[0]); i++){
		if(G->hand[0][i] == great_hall) numGH++;
	}
	return numGH;
}

int countGreatHallsBefore(struct gameState* G){
	int i, numGH = 0;
	for(i=0; i<(G->handCount[0]); i++){
		if(G->hand[0][i] == great_hall) numGH++;
	}
	return numGH;
}

int main() {
	int handpos = 0, choice1 = 0, choice2 = 0, choice3 = 0, bonus = 0;
	int seed = 1000;
	int i, j, r;
	int numTests = 0;
	int numFails = 0;
	int numPlayers;
	int thisPlayer = 0;
	struct gameState G, testG;
	int k[10] = {adventurer, great_hall, village, minion, mine, cutpurse,
			sea_hag, tribute, smithy, council_room};
	printf("----------------- Testing Card: %s ----------------\n", TESTCARD);
	while(numTests < 1000000){
		numPlayers = rand() % MAX_PLAYERS + 1;
		// initialize a game state and player cards
		initializeGame(numPlayers, k, seed, &G);
		seed = (rand() % 10000)+1; //Make a new rand seed so not every test is the same
		
		//Generate a random hand, deck, and discard pile for all players.
		for(j=0; j<numPlayers; j++){
			G.handCount[j] = rand() % 16;
			for(i=1; i<G.handCount[0]; i++){
				G.hand[j][i] = rand() % (treasure_map +1);
			}
			G.hand[j][0] = great_hall;//First card is great_hall
			G.deckCount[j] = rand() % MAX_DECK;
			for(i=0; i<G.deckCount[0]; i++){
				r = rand() % (treasure_map+1);
				G.deck[j][i] = r;
			}
			G.discardCount[j] = rand() % MAX_DECK;
			for(i=0; i<G.discardCount[0]; i++){
				r = rand() % (treasure_map+1);
				G.discard[j][i] = r;
			}
		}

		// copy the game state to a test case
		memcpy(&testG, &G, sizeof(struct gameState));

		//Play the card
		if(cardEffect(great_hall, choice1, choice2, choice3, &testG, handpos, &bonus)){
			printf("Failed to play card\n");
			numFails++;
		}else{
			//check all card counts to make sure things are as expected
			if((G.handCount[0] != 0) && assert(testG.handCount[thisPlayer] == (G.handCount[thisPlayer]) )){
				printf("hand count = %d, expected = %d\n", testG.handCount[thisPlayer], G.handCount[thisPlayer]);
				numFails++;
			} else if( (G.handCount[0] != 0) && assert(countGreatHallsAfter(&testG) == (countGreatHallsBefore(&G) - 1) )){
				printf("card not actually drawn %d %d\n", countGreatHallsAfter(&testG), (countGreatHallsBefore(&G) - 1));
				numFails++;
			}

			if(assert((testG.deckCount[thisPlayer] + testG.discardCount[thisPlayer] + testG.handCount[thisPlayer] + testG.playedCardCount) == 
				(G.deckCount[thisPlayer] + G.handCount[thisPlayer] + G.discardCount[thisPlayer]) + G.playedCardCount)){
				printf("For player0: total count = %d, expected = %d\n", 
					testG.deckCount[thisPlayer] + testG.discardCount[thisPlayer] + testG.handCount[thisPlayer], 
					G.deckCount[thisPlayer] + G.handCount[thisPlayer] + G.discardCount[thisPlayer]);
				numFails++;
			}

			for(thisPlayer=1; thisPlayer<numPlayers; thisPlayer++){ //Check other player(s) stuff
				if(assert((testG.deckCount[thisPlayer] + testG.discardCount[thisPlayer] + testG.handCount[thisPlayer]) ==
					(G.deckCount[thisPlayer] + G.handCount[thisPlayer] + G.discardCount[thisPlayer]) )){
					printf("For player1: total count = %d, expected = %d\n", 
						testG.deckCount[thisPlayer] + testG.discardCount[thisPlayer] + testG.handCount[thisPlayer], 
						G.deckCount[thisPlayer] + G.handCount[thisPlayer] + G.discardCount[thisPlayer]);
					numFails++;
				}
				if(assert(testG.handCount[thisPlayer] == G.handCount[thisPlayer])){
					printf("For player1: hand count = %d, expected = %d\n", 
						testG.handCount[thisPlayer], G.handCount[thisPlayer]);
				}
			}
			thisPlayer = 0; //Reset back to correct player's turn
	
			numFails += kingdom(&testG, &G);//Check to make sure card piles have not changed
	
			//Check that actions have not been changed
			if(assert(testG.numActions == (G.numActions + 1) )){
				printf("Actions=%d, expected=%d\n", 
					testG.numActions, G.numActions);
				numFails++;
			}
			if(assert(testG.numBuys == G.numBuys)){
				printf("buys=%d, expected=%d\n", 
					testG.numBuys, G.numBuys);
				numFails++;
			}
		}
		numTests++;
	}

	printf("\n >>>>> SUCCESS: Testing complete %s <<<<<\n\n", TESTCARD);
	if(numFails){
		printf("\033[1;31m"); //Set color to red
		printf("%d FAULTS IN %d TESTS\n", numFails, numTests);
		printf("\033[0m"); //Reset color
	} else {
		printf("\033[1;32m"); //Set color to green
		printf("ALL TESTS PASSED\n");
		printf("\033[0m"); //Reset color
	}
	return 0;
}
