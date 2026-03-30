// checked for style
// DOES NOT PASS in the autograder

#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <cassert>
#include "Card.hpp"
#include "Pack.hpp"
#include "Player.hpp"
using namespace std;


class Game {
public:
    // create ctor
    Game(vector<Player*> players, Pack& pack, int ptsWin) 
        : players(players), pack(pack), ptsWin(ptsWin), dealer(0){
    }

    void playGame(string shuffleOpt);

private:

    // create member variables

    vector<Player*> players;
    Pack& pack;
    Suit trump;
    int ptsWin;
    int dealer;
    int team1Score = 0;
    int team2Score = 0;
    bool calledTrump1 = false;
    vector<vector<Card>> tricks;
    vector<Card> ledCards;

    // create member functions

    void dealCards();
    void dealThree(int nextPlayer);
    void dealTwo(int nextPlayer);
    void makeTrump();
    void playTrick();
    void updateScores();
    bool gameOver();
    void printWinners();
    int makeWinner(const vector<Card>& trick, const Card& ledCard, Suit trump);
    int trickWinner(int playerNum);
};

// implement member functions

void Game::playGame(string shuffleOpt) {
    
    static int handNum = 0;
    // as long as game is not over, play the game
    while (!gameOver()) {
        // create a pack and shuffle it (if needed)
        if (shuffleOpt == "shuffle") {
            pack.shuffle();
        }
        // get dealer 
        string dealerName = players[dealer]->get_name();
        // print begining messages
        cout << "Hand " << handNum << endl;
        cout << dealerName << " deals" << endl;
        // check for empty pack
        if (pack.empty()) {  
            cout << "Pack is empty." << endl;
            return;
        }
        // begin the game
        dealCards();
        makeTrump();
        // play 5 tricks - all cards in the hand
        int numRounds = 5;
        for (int i = 0; i < numRounds; ++i) {
            playTrick();
        }
        // after all the tricks are played...
        updateScores();
        // increase the hand number for every hand played
        handNum++;
        dealer = (dealer + 1) % 4;
    }
    // when the game is over, print the winners of the game.
    printWinners();
}

void Game::dealCards(){
    pack.reset();
    int numPlayers = players.size();
    // find the index of the next player
    // the person next to the dealer: dealer + 1
    // % numPlayes: account for wrap around
    int nextPlayer = (dealer + 1) % numPlayers;
    // deal 3, then 2 cards, switching every time

    dealThree(nextPlayer);
    nextPlayer = (nextPlayer + 1) % numPlayers;
    dealTwo(nextPlayer);
    nextPlayer = (nextPlayer + 1) % numPlayers;
    dealThree(nextPlayer);
    nextPlayer = (nextPlayer + 1) % numPlayers;
    dealTwo(nextPlayer);
    // deal 2, then 3 cards, switching every time
    nextPlayer = (nextPlayer + 1) % numPlayers;
    dealTwo(nextPlayer);
    nextPlayer = (nextPlayer + 1) % numPlayers;

    dealThree(nextPlayer);
    nextPlayer = (nextPlayer + 1) % numPlayers;
    dealTwo(nextPlayer);
    nextPlayer = (nextPlayer + 1) % numPlayers;
    dealThree(nextPlayer);

    // nextPlayer = (dealer + 1) % numPlayers;
    // for (int i = 0; i < numPlayers; ++i) {
    //     for (int j = 0; j < 2; ++j) {
    //         players[nextPlayer]->add_card(pack.deal_one());
    //     }
    //     nextPlayer = (nextPlayer + 1) % numPlayers;
    // }
}

void Game::dealThree(int nextPlayer){
    for (int j = 0; j < 3; ++j) {
        players[nextPlayer]->add_card(pack.deal_one());
    }
}

void Game::dealTwo(int nextPlayer){
    for (int j = 0; j < 2; ++j) {
        players[nextPlayer]->add_card(pack.deal_one());
    }
}

void Game::makeTrump() {
    Card upcard = pack.deal_one();
    cout << upcard << " turned up" << endl;

    int numPlayers = players.size();

    // Round 1: Order up trump
    for (int i = 0; i < numPlayers; ++i) {
        Player* player = players[(dealer + 1 + i) % numPlayers];
        if (player->make_trump(upcard, (dealer == i), 1, trump)) {
            cout << player->get_name() << " orders up " << trump << endl << endl;
            calledTrump1 = ((dealer + 1 + i) % numPlayers) % 2 == 0;
            players[dealer]->add_and_discard(upcard); 
            return;
        }
        cout << player->get_name() << " passes" << endl;
    }

    // Round 2: Call any trump
    for (int i = 0; i < numPlayers - 1; ++i) {
        Player* player = players[(dealer + 1 + i) % numPlayers];
        if (player->make_trump(upcard, false, 2, trump)) {
            cout << player->get_name() << " orders up " << trump << endl << endl;
            calledTrump1 = ((dealer + 1 + i) % numPlayers) % 2 == 0;
            return;
        }
        cout << player->get_name() << " passes" << endl;
    }

    // dealer is forced to pick trump; screw the dealer
    players[dealer]->make_trump(upcard, true, 2, trump);
    cout << players[dealer]->get_name() << " orders up " << trump << endl << endl;
    calledTrump1 = (dealer % 2 == 0);
}

// hey
void Game::playTrick(){
    int numPlayers = players.size();
    vector<Card> trick(numPlayers);

    // The player next to the dealer leads the trick
    int currentPlayer = (dealer + 1) % numPlayers;
    Card ledCard = players[currentPlayer]->lead_card(trump);
    trick[currentPlayer] = ledCard;
    cout << ledCard << " led by " << players[currentPlayer]->get_name() << endl;

    // Each player plays a card in turn
    for (int i = 1; i < numPlayers; ++i) {
        currentPlayer = (currentPlayer + 1) % numPlayers;
        Card playedCard = players[currentPlayer]->play_card(ledCard, trump);
        trick[currentPlayer] = playedCard;
        cout << playedCard << " played by " << players[currentPlayer]->get_name() << endl;
    }

    // Determine the winner of the trick
    int winner = makeWinner(trick, ledCard, trump);
    cout << players[winner]->get_name() << " takes the trick" << endl << endl;

    // Set the winner of the trick as the next dealer
    dealer = winner - 1;

    // Store the trick and the led card
    tricks.push_back(trick);
    ledCards.push_back(ledCard);
}

int Game::makeWinner(const vector<Card>& trick, 
                        const Card& ledCard, Suit trump){
    // create variables to store the player's number & the winning card
    int winner = 0;  
    // assign the winningCard to the first card to be compared to the cards
    Card winningCard = trick[0]; 
    // go through the trick 
    for (int i = 0; i < trick.size(); ++i) {
        // get the card we're looking at in the trick
        const Card& currentCard = trick[i];
        if (currentCard.is_right_bower(trump)){
            return i;
        }
        if (currentCard.is_left_bower(trump)){
            return i;
        }
        // compare card to trump
        // if the current card is trump...        
        if (currentCard.is_trump(trump)) {
            // ... and the winning card is not trump
            if (!winningCard.is_trump(trump)) {
                // this means the current card is automatically the winner
                winner = i;
                // update the winning card
                winningCard = currentCard;
            // if the winning card is trump...
            // ... compare the cards to see which one is higher
            } else if (currentCard > winningCard) {
                // currrent card has to be higher than winning card
                winner = i;
                // update the winning card
                winningCard = currentCard;
            }
        // if the current card is not trump...
        // find the suits to comapre the current card with the led card's suit
        } else if (currentCard.get_suit() == ledCard.get_suit()) {
            // if the winning card is trump, it wins automatically
            // if it's not trump, this card has to be higher than winning card
            if (!winningCard.is_trump(trump) && currentCard > winningCard) {
                winner = i;
                // update the winning card
                winningCard = currentCard;
            }
        }
    }
    // return the player's index who won the trick
    return winner;  
}

void Game::updateScores(){
    // Initialize the number of tricks won by each team
    // create variables to store the tricks each team won
    int tricks1 = 0;
    int tricks2 = 0;
    // loop through all tricks in the round
    int numTricks = 5;
    for (int i = 0; i < numTricks; ++i) {  
        // find the winner of the "i"th trick
        int winnerOfTrick = trickWinner(i);  
        // if player is on team one...
        if (winnerOfTrick == 0 || winnerOfTrick == 2) {
            // increase their num of tricks
            tricks1++;
        } else {
            // if not, then they're on team two
            // increase their num of tricks
            tricks2++;
        }
    }
    // cout << team1Score << endl;
    // cout << players[0] << endl << endl << endl;
    // if (team1Score >= ptsWin) {
    //     cout << players[0]->get_name() << " and " 
    //          << players[2]->get_name() << " win the hand" << endl;
    // } else {
    //     cout << players[1]->get_name() << " and " 
    //          << players[3]->get_name() << " win the hand" << endl;
    // }
    // analyze the scoring
    if (calledTrump1) {
        // team one called it, so they have to get at least 3 tricks
        if (tricks1 == 5) {
            // team one got all tricks; they get two points
            cout << players[0]->get_name() << " and " 
                    << players[2]->get_name() << " win the hand" << endl;
            cout << "march!" << endl;
            team1Score += 2;
        } else if (tricks1 >= 3) {
            // as long as they get a majority (>=3 pts); they get a point
            cout << players[0]->get_name() << " and " 
                    << players[2]->get_name() << " win the hand" << endl;
            team1Score += 1;
        } else {
            // if they don't get a majority of the points, team 2 gets two pts
            cout << players[1]->get_name() << " and " 
                    << players[3]->get_name() << " win the hand" << endl;
            cout << "euchred!" << endl;
            team2Score += 2;
        }
    } else {
        // team two called it, so they have to get at least 3 tricks
        if (tricks2 == 5) {
            // team two got all tricks; they get two points
            cout << players[1]->get_name() << " and " 
                    << players[3]->get_name() << " win the hand" << endl;
            cout << "march!" <<endl;
            team2Score += 2;
        } else if (tricks2 >= 3) {
            // as long as they get a majority (>=3 pts); they get a point
            cout << players[1]->get_name() << " and " 
                    << players[3]->get_name() << " win the hand" << endl;
            team2Score += 1;
        } else {
            // if they don't get a majority of the points, team 1 gets two pts
            cout << players[0]->get_name() << " and " 
                    << players[2]->get_name() << " win the hand" << endl;
            cout << "euchred!" <<endl;
            team1Score += 2;
        }
    }
    string p0 = players[0]->get_name();
    string p1 = players[1]->get_name();
    string p2 = players[2]->get_name();
    string p3 = players[3]->get_name();
    cout << p0 << " and " << p2 << " have " << team1Score << " points" << endl;
    cout << p1 << " and " << p3 << " have " << team2Score << " points" << endl << endl;
}

int Game::trickWinner(int playerNum){
    // // playerNum = the index of that player
    // // find the trick and led card 
    // vector<Card> trick = tricks[playerNum];
    // Card ledCard = ledCards[playerNum];
    // // find the player's num of the winner
    // int winnerNum = makeWinner(trick, ledCard, trump);
    // return winnerNum;

    // new method
    // CHANGE HERE: Placeholder logic for determining trick winner.
    return playerNum % players.size();  // Dummy return for simplicity
}

bool Game::gameOver(){
    // check if either team has reached 10 points
    // if true, then game is over
    bool endGame = false;
    if (team1Score >= ptsWin){
        endGame = true;
    }
    if (team2Score >= ptsWin){
        endGame = true;
    }
    return endGame;
}

void Game::printWinners(){
    // compare team scores 
    if (team1Score >= ptsWin) {
        cout << players[0]->get_name() << " and " 
             << players[2]->get_name() << " win!" << endl;
    } else {
        cout << players[1]->get_name() << " and " 
             << players[3]->get_name() << " win!" << endl;
    }
}


// the main game
int main(int argc, char* argv[]) {   
    // print the executable 
    cout << "./euchre.exe ";
    // print the arguments
    for (int i = 1; i < argc; ++i) {
        cout << argv[i] << " ";
    }
    cout << endl;
    // check for each of these errors:
    // print the message and return a non-zero value if error is found
    // 1. there are exactly 12 arguments, including the executable name.
    if (argc != 12) {
        cout << "Usage: euchre.exe PACK_FILENAME [shuffle|noshuffle] "
             << "POINTS_TO_WIN NAME1 TYPE1 NAME2 TYPE2 NAME3 TYPE3 "
             << "NAME4 TYPE4" << endl;
        return 1;
    }
    // get the command line arguments to check for the rest of the errors
    string filename = argv[1];
    string shuffleOpt = argv[2];
    int pts = stoi(argv[3]);
    vector<string> playerNames = {argv[4], argv[6], argv[8], argv[10]};
    vector<string> playerTypes = {argv[5], argv[7], argv[9], argv[11]};
    // 2. points to win the game is between 1 and 100, inclusive.
    if (pts < 1 || pts > 100) {
        cout << "Usage: euchre.exe PACK_FILENAME [shuffle|noshuffle] "
             << "POINTS_TO_WIN NAME1 TYPE1 NAME2 TYPE2 NAME3 TYPE3 "
             << "NAME4 TYPE4" << endl;
        return 1;
    }
    // 3. the shuffle argument is either shuffle or noshuffle.
    if (shuffleOpt != "shuffle" && shuffleOpt != "noshuffle") {
        cout << "Usage: euchre.exe PACK_FILENAME [shuffle|noshuffle] "
             << "POINTS_TO_WIN NAME1 TYPE1 NAME2 TYPE2 NAME3 TYPE3 "
             << "NAME4 TYPE4" << endl;
        return 1;
    }
    int numPlayers = playerNames.size();

    // 4. the types of each of the players are either Simple or Human.
    for (int i = 0; i < numPlayers; ++i) {
        if (playerTypes[i] != "Simple" && playerTypes[i] != "Human") {
            cout << "Usage: euchre.exe PACK_FILENAME [shuffle|noshuffle] "
             << "POINTS_TO_WIN NAME1 TYPE1 NAME2 TYPE2 NAME3 TYPE3 "
             << "NAME4 TYPE4" << endl;
        cout << "4";
        return 1;
        }
    }
    // 5. verify the pack input file opens successfully 
    ifstream pack_file(filename);
    if (!pack_file.is_open()) {
        cout << "Error opening " << filename << endl;
        return 1;
    }
    // // create a pack and shuffle it (if needed)
    Pack pack(pack_file);
    // if (shuffleOpt == "shuffle") {
    //     pack.shuffle();
    // }
    // create players
    vector<Player*> players;
    // loop through every player
    for (int i = 0; i < numPlayers; ++i) {
        // add their name & type to the player
        players.push_back(Player_factory(playerNames[i], playerTypes[i]));
    }
    // create a game
    Game game(players, pack, pts);
    // play that game
    game.playGame(shuffleOpt);
    // delete the Player objects created by the Player_factory
    for (size_t i = 0; i < players.size(); ++i) {
        delete players[i];
    }
    // return zero --> the game has successfully ended / exited
    return 0;
}
