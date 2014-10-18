// dracula.c
// Implementation of your "Fury of Dracula" Dracula AI

#include <stdlib.h>
#include <stdio.h>
#include "Game.h"
#include "DracView.h"

#define PLACES 11
#define INN 8
#define OUT 14

void decideDraculaMove(DracView gameState)
{
   //Locations
   LocationID dracLocation = whereIs(gameState, PLAYER_DRACULA);
   LocationID godLocation = whereIs(gameState, PLAYER_LORD_GODALMING);
   LocationID sewLocation = whereIs(gameState, PLAYER_DR_SEWARD);
   LocationID vanLocation = whereIs(gameState, PLAYER_VAN_HELSING);
   LocationID minLocation = whereIs(gameState, PLAYER_MINA_HARKER);

   //Array of dracula's path
   LocationID places[PLACES] = {MARSEILLES, GENOA, MILAN, ZURICH, GENEVA, STRASBOURG, 
      BRUSSELS, LE_HAVRE, NANTES, BORDEAUX, TOULOUSE};
//   LocationID inner[INN] = {NUREMBURG, FRANKFURT, COLOGNE, BRUSSELS, PARIS, 
//      CLERMONT_FERRAND, GENEVA, ZURICH, STRASBOURG}
//   LocationID outer[OUT] = {NUREMBURG, LEIPZIG, HAMBURG, AMSTERDAM, BRUSSELS, 
//      LE_HAVRE, NANTES, BORDEAUX, TOULOUSE, MARSEILLES, GENOA, VENICE, MUNICH}
   
   //Starting Move
   int score = giveMeTheScore(gameState);
   Round r = giveMeTheRound(gameState);
   printf("round = %d\n", r);
   if (r == 0) {
      //Go furthest away from h
      registerBestPlay("MR", "It is time to begin");
      return;
      //Prefer port cities
   }

   //Dracula follow's his path
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
  
   //Figuring out Dracula's message
   char *name = idToName(choice);
   if (r % 13 == 0)
   {
      name = "Children of the night, be free!";
   } else if (godLocation == dracLocation)
   {
      name = "Lord! Goddalm it";
   } else if (sewLocation == dracLocation)
   {
      name = "Dr Stitch";
   } else if (vanLocation == dracLocation)
   {
      name = "Fun Hellsing";
   } else if (minLocation == dracLocation)
   {
      name = "Mind a Hark";
   } else if (r == 1)
   {
      name = "My plans are in motion";
   } else if (score < 50)
   {
      name = "Muahaha! The world is in the palm of my hand";
   } else if (score < 100)
   {
      name = "Merely seconds to success!";
   }

   //Returns the move
   char *move = idToAbbrev(choice);
   registerBestPlay(move, name);
}
