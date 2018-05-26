/**  randomtestadventurer.c
 **/
/**  Include the following lines in your makefile:
 **  randomtestadventurer: randomtestadventurer.c dominion.o rngs.o  
	gcc -o randomtestadventurer -g  randomtestadventurer.c dominion.o rngs.o $(CFLAGS)
*/

#include "dominion.h"
#include "dominion_helpers.h"
#include <string.h>
#include <stdio.h>
#include "rngs.h"
#include <stdlib.h>
#define TESTCARD "adventurer"

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

int main() {
	int handpos = 0, choice1 = 0, choice2 = 0, choice3 = 0, bonus = 0;
	int seed = 1000;
	int i, j, r;
	int numTests = 0;
	int numFails = 0;
	int numPlayers;
	int thisPlayer = 0;
	int numTreasures;
	struct gameState G, testG;
	int k[10] = {adventurer, embargo, village, minion, mine, cutpurse,
			sea_hag, tribute, smithy, council_room};
	printf("----------------- Testing Card: %s ----------------\n", TESTCARD);
	while(numTests < 1000000){
		numPlayers = rand() % MAX_PLAYERS + 1;
		// initialize a game state and player cards
		initializeGame(numPlayers, k, seed, &G);
		seed = (rand() % 10000)+1; //Make a new rand seed so not every test is the same
		
		//Generate a random hand, deck, and discard pile for all players.
		numTreasures = 0;
		while(numTreasures<2){
			numTreasures = 0;
			for(j=0; j<numPlayers; j++){
				G.handCount[j] = rand() % 16;
				for(i=1; i<G.handCount[0]; i++){
					G.hand[j][i] = rand() % (treasure_map +1);
				}
				G.hand[j][0] = adventurer;//First card is adventurer
				G.deckCount[j] = rand() % MAX_DECK;
				for(i=0; i<G.deckCount[0]; i++){
					r = rand() % (treasure_map+1);
					if(j==0 && (r == copper || r == silver || r == gold)){
						numTreasures++;
					}
					G.deck[j][i] = r;
				}
				G.discardCount[j] = rand() % MAX_DECK;
				for(i=0; i<G.discardCount[0]; i++){
					r = rand() % (treasure_map+1);
				if(j==0 && (r == copper || r == silver || r == gold)){
					numTreasures++;
				}
				G.discard[j][i] = r;
				}
			}
		}

		updateCoins(thisPlayer, &G, 0); //Get more current coin count

		// copy the game state to a test case
		memcpy(&testG, &G, sizeof(struct gameState));

		//Play the card
		if(cardEffect(adventurer, choice1, choice2, choice3, &testG, handpos, &bonus)){
			if(numPlayers != 0){
				printf("Failed to play card\n");
				numFails++;
			}
		}else{
		
			//check all card counts to make sure things are as expected
			if(assert(testG.handCount[thisPlayer] == (G.handCount[thisPlayer] + 1) )){
				//printf("hand count = %d, expected = %d\n", testG.handCount[thisPlayer], G.handCount[thisPlayer] + 1);
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
	
			updateCoins(thisPlayer, &testG, 0); //Get more current coin count
			if(assert(testG.coins >= (G.coins + 2) )){
				printf("coins = %d, expected >= %d\n", testG.coins, G.coins + 2);
				numFails++;
			}
			
			numFails += kingdom(&testG, &G);//Check to make sure card piles have not changed
	
			//Check that actions have not been changed
			if(assert(testG.numActions == G.numActions)){
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
