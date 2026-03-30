// checked for style
// DOES NOT PASS in the autograder

// #include "Card.hpp"
// #include "Pack.hpp"
#include "Player.hpp"
#include <array>
#include <cassert>
#include <string>
#include <vector>
#include <algorithm>
using namespace std;

// create Simple player

class Simple : public Player {
 public:
    // create ctor
    Simple(const string& simpleName) : name(simpleName){
    }

    // member functions

    //EFFECTS returns player's name
    const string & get_name() const override {
        return name;
    }

    // // new - assign the member variable the name given to it
    // void set_name(const string& playerName) {
    //     name = playerName;
    // }

    //REQUIRES player has less than MAX_HAND_SIZE cards
    //EFFECTS  adds Card c to Player's hand
    void add_card(const Card &c) override {
        hand.push_back(c);
        sort(hand.begin(), hand.end());
    }

    //REQUIRES Player has at least one card
    //EFFECTS  Player adds one card to hand and removes one card from hand.
    void add_and_discard(const Card &upcard) override {
        // add the upcard to the user's hand
        hand.push_back(upcard);

        Card lowest (ACE, DIAMONDS);
        int index = 0;
        for (int i = 0; i < hand.size(); i++){
            if (Card_less(hand[i], lowest, upcard.get_suit())){
                lowest = hand[i];
                index = i;
            }
        }
        // get rid of the lowest card
        hand.erase(hand.begin() + index);
    }

    //REQUIRES round is 1 or 2
    //MODIFIES order_up_suit
    //EFFECTS If Player wishes to order up a trump suit then return true and
    //  change order_up_suit to desired suit.  If Player wishes to pass, then do
    //  not modify order_up_suit and return false.
    bool make_trump(const Card &upcard, bool is_dealer,
                            int round, Suit &order_up_suit) const override {
        int count = 0;
        int numCards = hand.size();
        // Suit upcardSuit = upcard.get_suit();
        if (round == 1){
            // go through all the cards in the hand
            for (int i = 0; i < numCards; ++i){
                // count the number of face / ace cards they have
                if (hand[i].is_face_or_ace()){
                    // if it's the upcard suit, increase the count
                    if (hand[i].get_suit() == upcard.get_suit()){
                        count++;
                    }
                }
            }
            // check if they have at least two face / ace cards
            if (count >= 2){
                // make that card trump
                order_up_suit = upcard.get_suit();
                return true;
            }
        } else if (round == 2) {
            if (!is_dealer){
                // go through all the cards in the hand
                for (int i = 0; i < numCards; ++i){
                    // count the number of face / ace cards they have
                    if (hand[i].is_face_or_ace()){
                        // if it's the next suit, increase the count
                        if (hand[i].get_suit() == Suit_next(upcard.get_suit())){
                            count++;
                        }
                    }
                }
                // check if they have at least two face / ace cards
                if (count >= 1){
                    // make that card trump
                    order_up_suit = Suit_next(upcard.get_suit());
                    return true;
                }
            } else {
                // if the person is the dealer, they have to call trump
                order_up_suit = Suit_next(upcard.get_suit());
                return true;
            }
        }
        // if trump is not called...
        return false;
    }

    

    //REQUIRES Player has at least one card
    //EFFECTS  Leads one Card from Player's hand according to their strategy
    //  "Lead" means to play the first Card in a trick.  The card
    //  is removed the player's hand.
    Card lead_card(Suit trump) override {
        sort(hand.begin(), hand.end());
        // find highest non-trump card
        for (int i = hand.size() - 1; i > -1; --i){
            // make sure it's non-trump
            if (hand[i].get_suit() != trump) {
                Card playCard = hand[i];
                hand.erase(hand.begin() + i);
                // since they're sorted, it'll be the highest
                return playCard;
            }
        }
        // if they only have trump cards...
        // find highest trump card
        // sort(hand.begin(), hand.end());
        Card playCard;
        int index = 0;
        for (int i = 0 ; i < hand.size(); ++i){
            if (Card_less(playCard, hand[i], trump)){
                playCard = hand[i];
                index = i;
            } 
        }
        hand.erase(hand.begin() + index);
        return playCard;
    }

    //REQUIRES Player has at least one card
    //EFFECTS  Plays one Card from Player's hand according to their strategy.
    //  The card is removed from the player's hand.
    Card play_card(const Card &led_card, Suit trump) override {
        // find left bower
        Card leftBower; // COME BACK

        Suit ledSuit = led_card.get_suit(trump);
        vector<Card> tempHand;
        vector<int> tempIndex;
        // go through all the cards in the hand
        for (int i = 0; i < hand.size(); ++i){
            // if it's the same suit
            if (hand[i].get_suit(trump) == ledSuit){
                // add it to a new vector
                tempHand.push_back(hand[i]);
                tempIndex.push_back(i);
            }
        }
        // go through the new vector to find highest card
        if (tempHand.size() == 0){
            // loop through all cards
            // use Card_less() to find the lowest card
            int lowestCardIndex = 0;
            for (int i = 1; i < hand.size(); ++i){
                // if card currently on < hand[lowestCardIndex]
                if (Card_less(hand[i], hand[lowestCardIndex], trump)){
                    lowestCardIndex = i;
                }
            }
            Card playCard = hand[lowestCardIndex];
            hand.erase(hand.begin() + lowestCardIndex);
            // return the lowest card
            return playCard;
        } else {
            // consider cards that can follow suit
            // get highest card
            int highestCardIndex = 0;
            for (int i = 1; i < tempHand.size(); ++i){
                // if card currently on < hand[lowestCardIndex]
                if (Card_less(tempHand[highestCardIndex], tempHand[i], trump)){
                    highestCardIndex = i;
                }
            }
            Card playCard = tempHand[highestCardIndex];
            int playedIndex = 0;
            for (int i = 0; i < hand.size(); i++){
                if (hand[i] == playCard){
                    playedIndex = i;
                }
            }
            // int handIndex = tempIndex[tempHand.size() - 1];
            hand.erase(hand.begin() + playedIndex);
            return playCard;
        }
    }

    const vector<Card>& get_hand(){
        return hand;
    }

private:
    // member variables
    string name;
    //Suit trump;
    vector<Card> hand;

    // member functions
    int countTrumpCards(Suit trump) const {
        int count = 0;
        // go through all the cards in the hand
        for (int i = 0; i < MAX_HAND_SIZE; ++i){
            if(hand[i].get_suit() == trump){
                if(hand[i].get_suit() == trump){
                    // increase the count
                count++;
                }
            }
        }
        return count;
    }
};



class Human : public Player {
 public:

    // create ctor
    Human(const string& humanName) : name(humanName){
    }

    // member functions

    //EFFECTS returns player's name
    const string & get_name() const override {
        return name;
    }

    // new - assign the member variable the name given to it
    void set_name(){
        cout << "Human Player, please enter your name: ";
        cin >> name;
        cout << endl;
    }

    //REQUIRES player has less than MAX_HAND_SIZE cards
    //EFFECTS  adds Card c to Player's hand
    void add_card(const Card &c) override {
        // if there's room in their hand
        if (hand.size() < MAX_HAND_SIZE){
            // add that card to their hand
            hand.push_back(c);
        }
        // make sure the hand stays sorted
        sort(hand.begin(), hand.end());
    }

    //REQUIRES round is 1 or 2
    //MODIFIES order_up_suit
    //EFFECTS If Player wishes to order up a trump suit then return true and
    //  change order_up_suit to desired suit.  If Player wishes to pass, then do
    //  not modify order_up_suit and return false.
    bool make_trump(const Card &upcard, bool is_dealer,
                            int round, Suit &order_up_suit) const override {
        printHand();
        cout << "Human player " << name << ", please enter a suit, or \"pass\":\n";
        string trump;
        cin >> trump;
        if (trump != "pass"){
            // FIX ---------------------------------------------------------------------
            // Suit orderedUp = string_to_suit(trump);
            // trump was made, so...
            return true;
        } else {
            // the player passed; trump wasn't made, so...
            return false;
        }
    }

    //REQUIRES Player has at least one card
    //EFFECTS  Player adds one card to hand and removes one card from hand.
    void add_and_discard(const Card &upcard) override {
        // add the upcard to the players hand
        hand.push_back(upcard);
        // show them their options
        printHand();
        cout << "Discard upcard: [-1]\n";
        cout << "Human player " << name << ", please select a card to discard:\n";
        int userChoice;
        cin >> userChoice;
        if (userChoice == -1){
            // they want to discard the upcard
            hand.pop_back();
        } else {
            // they chose a different card to discard
            hand.erase(hand.begin() + userChoice);
        }
        // resort the cards
        sort(hand.begin(), hand.end());
    }

    //REQUIRES Player has at least one card
    //EFFECTS  Leads one Card from Player's hand according to their strategy
    //  "Lead" means to play the first Card in a trick.  The card
    //  is removed the player's hand.
    Card lead_card(Suit trump) override {
        // DOESN'T UES TRUMP
        // show the user their cards and ask to play a card
        printHand();
        cout << "Human player "  << name << ", please select a card:\n";
        int userChoice;
        cin >> userChoice;
        // access the card they chose
        Card playCard = hand[userChoice];
        // remove that card from their hand
        hand.erase(hand.begin() + userChoice);
        return playCard;
    }

    //REQUIRES Player has at least one card
    //EFFECTS  Plays one Card from Player's hand according to their strategy.
    //  The card is removed from the player's hand.
    Card play_card(const Card &led_card, Suit trump) override {
        // same as lead_card function
        // show the user their cards and ask to play a card
        printHand();
        cout << "Human player "  << name << ", please select a card:\n";
        int userChoice;
        cin >> userChoice;
        // access the card they chose & remove it from their hand
        Card playCard = hand[userChoice];
        hand.erase(hand.begin() + userChoice);
        return playCard;
    }

 private:

    // member variables
    string name;
    // Suit trump;
    vector<Card> hand;

    // member functions
    int countTrumpCards(Suit trump) const{
        int count = 0;
        // go through all the cards in the hand
        for (int i = 0; i < MAX_HAND_SIZE; ++i){
            if(hand[i].get_suit() == trump){
                if(hand[i].get_suit() == trump){
                    // increase the count
                count++;
                }
            }
        }
        return count;
    }

    void printHand() const {
        // loop through all cards in their hand
        cout << "Human player " << name << "'s hand: " << endl;
        for (size_t i = 0; i < hand.size(); ++i) {
            cout << "[" << i << "] " << hand[i] << "\n";
        }
    }

};

//EFFECTS: Returns a pointer to a player with the given name and strategy
//To create an object that won't go out of scope when the function returns,
//use "return new Simple(name)" or "return new Human(name)"
//Don't forget to call "delete" on each Player* after the game is over
Player * Player_factory(const std::string &name, const std::string &strategy){
    // check the value of strategy and return the corresponding player type.
    if (strategy == "Simple") {
        // the "new" keyword dynamically allocates an object.
        return new Simple(name);
    } else if (strategy == "Human") {
        // return new Human(name);
        return new Human(name);
    } else {
        // invalid strategy if we get here
        return nullptr;
    }
}

//EFFECTS: Prints player's name to os
ostream & operator<<(ostream &os, const Player &p){
    os << p.get_name();
    return os;
}