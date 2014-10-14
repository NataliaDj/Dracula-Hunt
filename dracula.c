// dracula.c
// Implementation of your "Fury of Dracula" Dracula AI

#include <stdlib.h>
#include <stdio.h>
#include "Game.h"
#include "DracView.h"

void decideDraculaMove(DracView gameState)
{
	//Get some information from game state
   //PlayerID player = whoAmI(gameState);
   //LocationID currLocation = whereIs(gameState, player);
   int *numLocations = NULL;
   LocationID *locations = whereCanIgo(gameState, numLocations, TRUE,TRUE);
   Round r = giveMeTheRound(gameState);

   godalmingLocation = whereIs(gameState, PLAYER_LORD_GODALMING);
   //etc

   if (r == 0) {
      //Go furthest away from hunters
      //Prefer port cities
      //registerBestPlay("CD", "NOTHING");
   }

   //Register bad move
   char *move = idToAbbrev(*locations);
   registerBestPlay(move, "NOTHING");
}
