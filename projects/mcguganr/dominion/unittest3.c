/*----------------------------------------------
 * Unit test for whoseTurn.
 * Include the following in your makefile:
 *
 * unittest3: unittest3.c dominion.o rngs.o
 *	gcc -o unittest3 -g  unittest3.c dominion.o rngs.o $(CFLAGS)
 * ---------------------------------------------
 */

#include "dominion.h"
#include "dominion_helpers.h"
#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include "rngs.h"


int assert(int arg){
	if(arg){
		printf("\033[1;32m"); //Set color to green
		printf("TEST PASSED\n");
		printf("\033[0m"); //Reset color
		return 0;
	}
	printf("\033[1;31m"); //Set color to red
	printf("TEST FAILED\n");
	printf("\033[0m"); //Reset color
	return 1;
}

int main(void){
	int seed = 1000;
	int numPlayer = 4;
	int i, p, r, countFails = 0;
	int k[10] = {adventurer, council_room, feast, gardens, mine,
		remodel, smithy, village, baron, great_hall};
	struct gameState G;

	printf ("----------------------- TESTING whoseTurn() ---------------------------\n");
	for (p = 0; p < numPlayer; p++){
		memset(&G, 23, sizeof(struct gameState));   // clear the game state
		r = initializeGame(numPlayer, k, seed, &G); // initialize a new game
		if(r) countFails += assert(0); //If we already failed, print error
		else {
			for(i=0; i<=p; i++){
				G.whoseTurn = i;
				countFails += assert(whoseTurn(&G) == i);
			}
		}
	}

	printf("--------------------- Testing complete. ----------------------------------\n");
	if(countFails){
		printf("\033[1;31m"); //Set color to green
		printf("%d TESTS FAILED\n", countFails);
		printf("\033[0m"); //Reset color
	} else {
		printf("\033[1;32m"); //Set color to red
		printf("ALL TESTS PASSED\n");
		printf("\033[0m"); //Reset color
	}

	return 0;
}
