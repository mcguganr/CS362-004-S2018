/*----------------------------------------------
 * Unit test for isGameOver.
 * Include the following in your makefile:
 *
 * unittest4: unittest4.c dominion.o rngs.o
 *	gcc -o unittest4 -g  unittest4.c dominion.o rngs.o $(CFLAGS)
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
/*		printf("\033[1;32m"); //Set color to green
		printf("TEST PASSED\n");
		printf("\033[0m"); //Reset color*/
		return 0;
	}
/*	printf("\033[1;31m"); //Set color to red
	printf("TEST FAILED\n");
	printf("\033[0m"); //Reset color*/
	return 1;
}

int main(void){
	int seed = 1000;
	int numPlayer = 2;
	int i, j, k, c, r, numTests = 0, countFails = 0, countPasses;
	int king[10] = {adventurer, council_room, feast, gardens, mine,
		remodel, smithy, village, baron, great_hall};
	struct gameState G;

	printf ("----------------------- TESTING isGameOver() ---------------------------\n");
	memset(&G, 23, sizeof(struct gameState));   // clear the game state
	r = initializeGame(numPlayer, king, seed, &G); // initialize a new game
	if(r) countFails += assert(0); //If we already failed, print error
	else {
		for (i = 2; i < 27; i++){
			for(j=1; j<i; j++){
				for(k=0; k<j; k++){
					//Reset cards to 1 so game is not over yet
					for(c=0; c<27; c++){
						G.supplyCount[c] = 1;
					}
					//Set three piles to empty
					G.supplyCount[i] = 0;
					G.supplyCount[j] = 0;
					G.supplyCount[k] = 0;
					countFails += assert(isGameOver(&G) == 1);
					numTests++;
				}
				if(i != 2 && j != 2){
					//Reset cards to 1 so game is not over yet
					for(c=0; c<27; c++){
						G.supplyCount[c] = 1;
					}
					//Set two piles to empty
					G.supplyCount[i] = 0;
					G.supplyCount[j] = 0;
					countFails += assert(isGameOver(&G) == 0);
					numTests++;
				}
			}
		}
		//Reset cards to 1 so game is not over yet
		for(c=0; c<27; c++){
			G.supplyCount[c] = 1;
		}
		//Set two piles to empty
		G.supplyCount[0] = 0;
		G.supplyCount[1] = 0;
		countFails += assert(isGameOver(&G) == 0);
		numTests++;

		//Reset cards to 1 so game is not over yet
		for(c=0; c<27; c++){
			G.supplyCount[c] = 1;
		}
		G.supplyCount[3] = 0; //Set province pile to empty
		countFails += assert(isGameOver(&G) == 1);
		numTests++;
	}

	printf("--------------------- Testing complete. --------------------------------\n");
	
	printf("\033[1;31m"); //Set color to green
	printf("%d TESTS FAILED\n", countFails);
	printf("\033[0m"); //Reset color
	printf("\033[1;32m"); //Set color to red
	countPasses = numTests - countFails;
	printf("%d TESTS PASSED\n", countPasses);
	printf("\033[0m"); //Reset color

	return 0;
}
