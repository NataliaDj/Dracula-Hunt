// dracula.c
// Implementation of your "Fury of Dracula" Dracula AI

#include <stdlib.h>
#include <stdio.h>
#include "Game.h"
#include "DracView.h"

#define PLACES 11

void decideDraculaMove(DracView gameState)
{
	//Get some information from game statec
   //PlayerID player = whoAmI(gameState);
   //LocationID currLocation = whereIs(gameState, player);

   //LocationID godLocation = whereIs(gameState, PLAYER_LORD_GODALMING);
   //LocationID sewLocation = whereIs(gameState, PLAYER_DR_SEWARD);
   //LocationID vanLocation = whereIs(gameState, PLAYER_VAN_HELSING);
   //LocationID minLocation = whereIs(gameState, PLAYER_MINA_HARKER);
   LocationID places[PLACES] = {MARSEILLES, GENOA, MILAN, ZURICH, GENEVA, STRASBOURG, 
      BRUSSELS, LE_HAVRE, NANTES, BORDEAUX, TOULOUSE};
   
   Round r = giveMeTheRound(gameState);
   printf("round = %d\n", r);
   /*if (r == 0) {
      //Go furthest away from h
      registerBestPlay("MR", "In Marseilles");
      return;
      //Prefer port cities
   }*/

   int numLocations = 0;
   LocationID *locations = whereCanIgo(gameState, &numLocations, TRUE,FALSE);
   printf("numLocations = %d\n", numLocations);
   int i = 0;
   for (i = 0; i < numLocations; ++i)
   {
      printf("numLocations[%d] is %d\n", i, locations[i]);
   }

   int choice = locations[0];

   int j = 0; 
   int isFound = FALSE;
   for (i = 0; i < PLACES; ++i)
   {
      printf("first for loop %d\n", i);
      for (j = 0; j < numLocations; ++j)
      {
         if (places[i]==locations[j])
         {
            printf("second for loop %d\n", j);
            choice = places[i];
            isFound = TRUE;
            break;
         }
      }

      if (isFound == TRUE)
      {
         break;
      }
   }
   
   char *move = idToAbbrev(choice);
   char *name = idToName(choice);
   registerBestPlay(move, name);
}
