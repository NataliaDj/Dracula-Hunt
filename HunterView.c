
// HunterView.c ... HunterView ADT implementation

#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
#include "Globals.h"
#include "Game.h"
#include "GameView.h"
#include "HunterView.h"
// #include "Map.h" ... if you decide to use the Map ADT

typedef struct player *Player;

struct hunterView {
    Player players[NUM_PLAYERS];
    Round round;
    int gameScore;
    GameView gameView;
    int hello;
    char message[MAX_MESSAGE_SIZE];
};
     
struct player {
    int health;
    LocationID curLocation;
    LocationID trail[TRAIL_SIZE];
};
     

// Creates a new HunterView to summarise the current state of the game
HunterView newHunterView(char *pastPlays, PlayerMessage messages[])
{
    //REPLACE THIS WITH YOUR OWN IMPLEMENTATION
    HunterView hunterView = malloc(sizeof(struct hunterView));
    assert(hunterView != NULL);

    hunterView->gameView = newGameView(pastPlays, messages);   
    int i;
    for (i = 0; i < MAX_MESSAGE_SIZE; i++) {
        hunterView->message[i] = messages[0][i];
    }

    
    for (i = 0; i < NUM_PLAYERS; i++)
    {
        hunterView->players[i] = malloc(sizeof(struct player));

        hunterView->players[i]->health = getHealth(hunterView->gameView, i);
        hunterView->players[i]->curLocation = getLocation(hunterView->gameView, i);
        getHistory(hunterView->gameView, i, hunterView->players[i]->trail); 
    }

    hunterView->round = getRound(hunterView->gameView);
    hunterView->gameScore = getScore(hunterView->gameView); 

    hunterView->hello = 42;
    return hunterView;
}
     
     
// Frees all memory previously allocated for the HunterView toBeDeleted
void disposeHunterView(HunterView toBeDeleted)
{
    int i;
    for (i = 0; i < NUM_PLAYERS; i++)
        free (toBeDeleted->players[i]);
    disposeGameView(toBeDeleted->gameView);
    free( toBeDeleted );
}


//// Functions to return simple information about the current state of the game

// Get the current round
Round giveMeTheRound(HunterView currentView)
{
    return currentView->round;
}

// Get the id of current player
PlayerID whoAmI(HunterView currentView)
{
    return getCurrentPlayer(currentView->gameView);
}

// Get the current score
int giveMeTheScore(HunterView currentView)
{
    return currentView->gameScore;
}

// Get the current health points for a given player
int howHealthyIs(HunterView currentView, PlayerID player)
{
    return currentView->players[player]->health;
}

// Get the current location id of a given player
LocationID whereIs(HunterView currentView, PlayerID player)
{
    return currentView->players[player]->curLocation;
}

//// Functions that return information about the history of the game

// Fills the trail array with the location ids of the last 6 turns
void giveMeTheTrail(HunterView currentView, PlayerID player,
                            LocationID trail[TRAIL_SIZE])
{
    getHistory(currentView->gameView, player, trail);
}

//// Functions that query the map to find information about connectivity

// What are my possible next moves (locations)
LocationID *whereCanIgo(HunterView currentView, int *numLocations,
                                int road, int rail, int sea)
{
    LocationID *location = malloc(*numLocations*sizeof(LocationID));
    PlayerID player = whoAmI (currentView);

    location = connectedLocations(currentView->gameView, numLocations,
                               currentView->players[player]->curLocation, 
                               player, currentView->round,
                               road, rail, sea);

    return location;
}

// What are the specified player's next possible moves
LocationID *whereCanTheyGo(HunterView currentView, int *numLocations,
                           PlayerID player, int road, int rail, int sea)
{
    LocationID *location = malloc(*numLocations*sizeof(LocationID));
    //printf("player is %d\n", player);
    //printf("curlocation is %d\n", currentView->players[player]->curLocation);
    location = connectedLocations(currentView->gameView, numLocations,
                               currentView->players[player]->curLocation, 
                               player, currentView->round,
                               road, rail, sea);/*
    int i;
    for (i = 0; i < *numLocations; ++i)
    {
        printf("%d\n", location[i]);
    }*/

    return location;
}

int howCanIGetThere (HunterView g, LocationID start, LocationID end, LocationID path[], TransportID trans[]) {
    int numCitiesToLocations = findPathToLocation(g->gameView, start, end, path, trans);
    return numCitiesToLocations;
}

char *giveMeMessage (HunterView h) {
    return h->message;
}

int isPortCity (HunterView currentView, int city, int player) {
    int numLocations;
    int i;
    LocationID *location = malloc(numLocations*sizeof(LocationID));
    // check locations connected to sea A.K.A port cities
    location = connectedLocations(currentView->gameView, &numLocations,
                               currentView->players[player]->curLocation, 
                               player, currentView->round,
                               FALSE, FALSE, TRUE);

    // check if the city passed in is a port city
    for (i = 0; i < numLocations; i++) {
        if (city == location[i]) {
            free(location);
            return TRUE;
        }
    }
    free(location);
    return FALSE;   // city is not a port city
}