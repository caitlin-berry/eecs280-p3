// checked for style
// passes in the autograder

#ifndef PACK_HPP
#define PACK_HPP
#include "Pack.hpp"
#include "Card.hpp"
#include <array>
#include <cassert>
#include <string>
using namespace std;

class Pack {
public:
  Pack();

  Pack(std::istream& pack_input);

  Card deal_one();

  void reset();

  void shuffle();

  bool empty() const;

private:
  static const int PACK_SIZE = 24;
  std::array<Card, PACK_SIZE> cards;
  int next; //index of next card to be dealt
};

#endif // PACK_HPP


Pack::Pack(){
    next = 0;
    int i = 0;
    // set loop variables
    int firstSuit = SPADES;
    int lastSuit = DIAMONDS;
    int firstRank = NINE;
    int lastRank = ACE;
    // loop through the all the suits
    for (int s = firstSuit; s <= lastSuit; ++s){
        // loop through all the ranks
        for (int r = firstRank; r <= lastRank; ++r){
            // assign each card it's suit and rank
            Suit suit = static_cast<Suit>(s);
            Rank rank = static_cast<Rank>(r);
            cards[i] = Card(rank, suit);
            i++;
        }
    }
}

Pack::Pack(std::istream& pack_input){
    next = 0;
    // create variables to read information in
    string r;
    string junk;
    string s;
    // read info in for every card in the pack
    for (int i = 0; i < PACK_SIZE; ++i){
        // read in each line
        pack_input >> r >> junk >> s;
        // assign the suits and ranks to each card
        Suit suit = string_to_suit(s);
        Rank rank = string_to_rank(r);
        cards[i] = Card(rank, suit);
    }
}

Card Pack::deal_one(){
    if (next >= PACK_SIZE){
        // assign rank and suit to start over
        Rank rank = static_cast<Rank>(-1);
        Suit suit = static_cast<Suit>(-1);
        return Card(rank, suit);
    } else {
        // if next isn't higher than pack_size, deal the next card
        return cards[next++];
    }
}

void Pack::reset(){
    // reset the next card to zero
    next = 0;
}

void Pack::shuffle(){
    // create the number of times to shuffle
    int amountShuffle = 7;
    // preform the shuffle
    for (int i = 0; i < amountShuffle; ++i) {
        // create a temportary array to hold cards - a pack
        array<Card, PACK_SIZE> tempCards;
        // find the middle of the pack so you can do the in-shuffle
        int middle = PACK_SIZE / 2;
        // create a position variable to use in the array
        int pos = 0;
        // loop through the first half of the deck
        for (int k = 0; k < middle; ++k) {
            // cards in the second half go in the beginning
            tempCards[pos++] = cards[k + middle];
            // cards in the first half go to the end 
            tempCards[pos++] = cards[k];
        }
        // put the temporary array into the original pack
        cards = tempCards;
    }
    // reset the pack
    reset();
}

bool Pack::empty() const {
    // if next card is higher than pack_size, you're at the end of the pack
    return next >= PACK_SIZE;
}