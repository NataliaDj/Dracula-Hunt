// dracula.c
// Implementation of your "Fury of Dracula" Dracula AI

#include <stdlib.h>
#include <stdio.h>
#include "Game.h"
#include "DracView.h"

#define PLACES 11
#define INN 9
#define OUT 14
#define SPECIAL 4

static int countHunter (LocationID locations[], int length, DracView gameState);

void decideDraculaMove(DracView gameState)
{
   //Locations
   LocationID dracLocation = whereIs(gameState, PLAYER_DRACULA);
   LocationID godLocation = whereIs(gameState, PLAYER_LORD_GODALMING);
   LocationID sewLocation = whereIs(gameState, PLAYER_DR_SEWARD);
   LocationID vanLocation = whereIs(gameState, PLAYER_VAN_HELSING);
   LocationID minLocation = whereIs(gameState, PLAYER_MINA_HARKER);

   //Array of dracula's path
   //LocationID places[PLACES] = {MARSEILLES, GENOA, MILAN, ZURICH, GENEVA, STRASBOURG, 
   //   BRUSSELS, LE_HAVRE, NANTES, BORDEAUX, TOULOUSE};
   LocationID inner[INN] = {NUREMBURG, COLOGNE, BRUSSELS, FRANKFURT, PARIS, 
      CLERMONT_FERRAND, GENEVA, ZURICH, STRASBOURG};
   LocationID outer[OUT] = {NUREMBURG, LEIPZIG, HAMBURG, AMSTERDAM, BRUSSELS, 
      LE_HAVRE, NANTES, BORDEAUX, CLERMONT_FERRAND, MARSEILLES, GENOA, VENICE, MUNICH};
   LocationID special[SPECIAL] = {NUREMBURG, COLOGNE, BRUSSELS, CLERMONT_FERRAND};
   
   //Starting Move
   int score = giveMeTheScore(gameState);
   Round r = giveMeTheRound(gameState);
   printf("round = %d\n", r);
   if (r == 0) {
      registerBestPlay("NU", "It is time to begin");
      return;
   }

   //Dracula follow's his path
   int numLocations = 0;
   LocationID *locations = whereCanIgo(gameState, &numLocations, TRUE,FALSE);
   printf("numLocations = %d\n", numLocations);
   int i, j, circle = OUT;
   int choice = locations[0];

   //if (drac is in special locations)
   // Checks the locations of the hunters and decide on a route
   // (inner or outer circle)
   //else
   // checks if dracula is in the inner or outer circle
   // Follows along the path

   int specialCase = FALSE;
   for (i = 0; i < SPECIAL; ++i)
   {
      if (dracLocation == special[i])
      {
         specialCase = TRUE;
      }
   }

   if (specialCase == TRUE)
   {
      printf("In specC\n");
      int innie = countHunter(inner, INN, gameState);
      int outie = countHunter(outer, OUT, gameState);
      if (innie < outie)
      {
         circle = INN;
      } 
      else 
      {
         circle = OUT;
      }
   } 

   if (circle == OUT)
   {
      int isFound = FALSE;
      for (i = 0; i < OUT; ++i)
      {
         for (j = 0; j < numLocations; ++j)
         {
            if (outer[i]==locations[j])
            {
               choice = outer[i];
               isFound = TRUE;
               break;
            }
         }

         if (isFound == TRUE)
         {
            break;
         }
      }
   } 
   else
   {
      int isFound = FALSE;
      for (i = 0; i < INN; ++i)
      {
         for (j = 0; j < numLocations; ++j)
         {
            if (inner[i]==locations[j])
            {
               choice = inner[i];
               isFound = TRUE;
               break;
            }
         }

         if (isFound == TRUE)
         {
            break;
         }
      }
   }
  
   //Figuring out Dracula's message
   char *name = idToName(choice);
   if ((r % 13 == 0) && (r != 0))
   {
      name = "Children of the night, be free!";
   } 
   else if (godLocation == dracLocation)
   {
      name = "Lord";
   } 
   else if (sewLocation == dracLocation)
   {
      name = "Dr";
   } 
   else if (vanLocation == dracLocation)
   {
      name = "Van";
   } 
   else if (minLocation == dracLocation)
   {
      name = "Mina";
   } 
   else if (r == 1)
   {
      name = "My plans are in motion";
   } 
   else if (score < 50)
   {
      name = "Muahaha! The world is in the palm of my hand";
   } 
   else if (score < 100)
   {
      name = "Merely seconds to success!";
   }

   //Returns the move
   char *move = idToAbbrev(choice);
   registerBestPlay(move, name);
}

int countHunter (LocationID locations[], int length, DracView gameState)
{
   LocationID godLocation = whereIs(gameState, PLAYER_LORD_GODALMING);
   LocationID sewLocation = whereIs(gameState, PLAYER_DR_SEWARD);
   LocationID vanLocation = whereIs(gameState, PLAYER_VAN_HELSING);
   LocationID minLocation = whereIs(gameState, PLAYER_MINA_HARKER);
   int i, count = 0;
   for (i = 0; i < length; ++i)
   {
      if ( (godLocation == locations[i]) ||
           (sewLocation == locations[i]) ||
           (vanLocation == locations[i]) ||
           (minLocation == locations[i]) )
      {
         count++;
      }
   }

   return count;
}
