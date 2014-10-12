// DracView.c ... DracView ADT implementation

#include <string.h>
#include <stdlib.h>
#include <assert.h>
#include "Globals.h"
#include "Game.h"
#include "GameView.h"
#include "DracView.h"
#include "Map.h" //... if you decide to use the Map ADT

#define BEFORE 1 
#define AFTER 0
#define CHAR_STRING 7
     
typedef struct player *Player;

struct dracView {
    Player players[NUM_PLAYERS];
    Round round;
    int gameScore;
    GameView gameView;
    char *pastPlays;
    Map map;
};

struct player {
    int health;
    LocationID curLocation;
    LocationID trail[TRAIL_SIZE];
};
     

// Creates a new DracView to summarise the current state of the game
DracView newDracView(char *pastPlays, PlayerMessage messages[])
{
    DracView dracView = malloc(sizeof(struct dracView));

    dracView->gameView = newGameView(pastPlays,messages);
    dracView->map = newMap();
    dracView->pastPlays = pastPlays;

    int i;
    for (i = 0; i < NUM_PLAYERS; ++i)
    {
        dracView->players[i] = malloc(sizeof(struct player));

        dracView->players[i]->health = getHealth(dracView->gameView, i);
        dracView->players[i]->curLocation = getLocation(dracView->gameView, i);
        getHistory(dracView->gameView, i, dracView->players[i]->trail); 
    }

    dracView->round = getRound(dracView->gameView);
    dracView->gameScore = getScore(dracView->gameView);

    return dracView;
}
     
     
// Frees all memory previously allocated for the DracView toBeDeleted
void disposeDracView(DracView toBeDeleted)
{
    int i;
    for (i = 0; i < NUM_PLAYERS; ++i)
        free (toBeDeleted->players[i]);
    disposeGameView (toBeDeleted->gameView);
    free( toBeDeleted );
}

//// Functions to return simple information about the current state of the game

// Get the current round
Round giveMeTheRound(DracView currentView)
{
    return currentView->round;
}

// Get the current score
int giveMeTheScore(DracView currentView)
{
    return currentView->gameScore;
}

// Get the current health points for a given player
int howHealthyIs(DracView currentView, PlayerID player)
{
    return currentView->players[player]->health;
}

// Get the current location id of a given player
LocationID whereIs(DracView currentView, PlayerID player)
{
    return currentView->players[player]->curLocation;
}

// Get the most recent move of a given player
void lastMove(DracView currentView, PlayerID player,
                 LocationID *start, LocationID *end)
{
    *start = currentView->players[player]->trail[BEFORE];
    *end = currentView->players[player]->trail[AFTER];
    return;
}

// Find out what minions are placed at the specified location
void whatsThere(DracView currentView, LocationID where,
                         int *numTraps, int *numVamps)
{
    int numTrap = 0;
    int numVamp = 0;
    int length = strlen(currentView->pastPlays);
    int i;
    int k;
    int currPlayer = PLAYER_LORD_GODALMING;
    for (i = 0; i < length; i += CHAR_STRING+1) {
        char abbrev[3] = {currentView->pastPlays[i+1], currentView->pastPlays[i+2], '\0'};
        if (abbrevToID(abbrev) == where) {
            if (currPlayer == PLAYER_DRACULA) {
                if (currentView->pastPlays[i+3] == 'T') numTrap++;
                if (currentView->pastPlays[i+4] == 'V') numVamp++;
                if (currentView->pastPlays[i+5] == 'M') {
                    numTrap--;
                } else if (currentView->pastPlays[i+5] == 'V') {
                    numVamp--;
                }
            } else {
                for (k = i + 3; k < i + CHAR_STRING+1; k++) {
                    if (currentView->pastPlays[k] == 'T') numTrap--;
                    if (currentView->pastPlays[k] == 'V') numVamp--;
                }
            }
        }
        
        if (currPlayer == PLAYER_DRACULA) {
            currPlayer = PLAYER_LORD_GODALMING;
        } else {
            currPlayer++;
        }
    }

    numTraps[0] = numTrap;
    numVamps[0] = numVamp;
}

//// Functions that return information about the history of the game

// Fills the trail array with the location ids of the last 6 turns
void giveMeTheTrail(DracView currentView, PlayerID player,
                            LocationID trail[TRAIL_SIZE])
{
    getHistory(currentView->gameView, player, trail);
}

//// Functions that query the map to find information about connectivity

// What are my (Dracula's) possible next moves (locations)
LocationID *whereCanIgo(DracView currentView, int *numLocations, int road, int sea)
{
    LocationID num = (LocationID)numLocations;
    LocationID *location = malloc(num*sizeof(LocationID));
    location = connectedLocations(currentView->gameView, numLocations,
                               currentView->players[PLAYER_DRACULA]->curLocation, 
                               PLAYER_DRACULA, currentView->round,
                               road, FALSE, sea);
    return location;
}

// What are the specified player's next possible moves
LocationID *whereCanTheyGo(DracView currentView, int *numLocations,
                           PlayerID player, int road, int rail, int sea)
{
    LocationID num = (LocationID)numLocations;
    LocationID *location = malloc(num*sizeof(LocationID));
    location = connectedLocations(currentView->gameView, numLocations,
                               currentView->players[player]->curLocation, 
                               player, currentView->round,
                               road, rail, sea);
    return location;
}
