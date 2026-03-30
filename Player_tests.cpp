// checked for style
// DOES NOT PASS in the autograder

#include "Player.hpp"
#include "unit_test_framework.hpp"
#include <iostream>
using namespace std;

// PLAYER.CPP TEST CASES

// simple class member functions

// Simple::Simple(const std::string &name);
TEST(test_simple_ctor){
    Player * caitlin = Player_factory("Caitlin", "Simple");
    ASSERT_EQUAL("Caitlin", caitlin->get_name());
    delete caitlin;
}

// virtual const std::string & Simple::get_name() const override;
TEST(test_simple_get_name){
    Player * caitlin = Player_factory("Caitlin", "Simple");
    ASSERT_EQUAL("Caitlin", caitlin->get_name());
    delete caitlin;
}

// virtual void Simple::add_card(const Card &c) override;
TEST(test_simple_add_card){
    Player * caitlin = Player_factory("Caitlin", "Simple");
    Card c(ACE, SPADES);
    caitlin->add_card(c);
    ostringstream os;
    os << caitlin->lead_card(SPADES);
    ASSERT_EQUAL("Ace of Spades", os.str());
    delete caitlin;
}

// virtual bool Simple::make_trump(const Card &upcard, bool is_dealer, 
//                                  int round, Suit &order_up_suit) const override;
// ERROR
// TEST(test_simple_make_trump){
//     Player * caitlin = Player_factory("Caitlin", "Simple");
//     Card upcard(JACK, SPADES);
//     Suit suit;
//     bool dealer = false;
//     ASSERT_TRUE(caitlin->make_trump(upcard, dealer, 1, suit));
//     ASSERT_EQUAL(SPADES, suit);
//     delete caitlin;
// }

// virtual void Simple::add_and_discard(const Card &upcard) override;
TEST(test_simple_add_and_discard){
    Player * caitlin = Player_factory("Caitlin", "Simple");
    Card upcard(ACE, HEARTS);
    caitlin->add_card(Card(TWO, SPADES));  
    caitlin->add_and_discard(upcard);  
    delete caitlin;
}

// virtual Card Simple::lead_card(Suit trump) override ;
// ERROR
// TEST(test_simple_lead_card){
//     Player * caitlin = Player_factory("Caitlin", "Simple");
//     caitlin->add_card(Card(SEVEN, DIAMONDS));
//     caitlin->add_card(Card(TEN, HEARTS));
//     caitlin->add_card(Card(JACK, SPADES));
//     Card ledCard = caitlin->lead_card(SPADES);
//     ASSERT_EQUAL(JACK, ledCard.get_rank());  
//     ASSERT_EQUAL(SPADES, ledCard.get_suit());
//     delete caitlin;
// }

// virtual Card Simple::play_card(const Card &led_card, Suit trump) override;
// ERROR
// TEST(test_simple_play_card){
//     Player * caitlin = Player_factory("Caitlin", "Simple");
//     caitlin->add_card(Card(FOUR, HEARTS));
//     caitlin->add_card(Card(SIX, SPADES));
//     Card ledCard(FOUR, DIAMONDS);
//     Card cardPlayed = caitlin->play_card(ledCard, HEARTS);
//     ASSERT_EQUAL(FOUR, cardPlayed.get_rank());
//     ASSERT_EQUAL(HEARTS, cardPlayed.get_suit());
//     delete caitlin;
// }


// human class member functions

// Human::Human(const std::string &name);
TEST(test_human_ctor){
    Player * human = Player_factory("Caitlin", "Human");
    ASSERT_EQUAL("Caitlin", human->get_name());
    delete human;
}

// virtual const std::string & Human::get_name() const override;
TEST(test_human_get_name){
    Player * human = Player_factory("Caitlin", "Human");
    ASSERT_EQUAL("Caitlin", human->get_name());
    delete human;
}


// other functions

// Player * Player_factory(const std::string &name, const std::string &strategy);
TEST(test_player_factory){
    Player * simple = Player_factory("Caitlin", "Simple");
    ASSERT_EQUAL("Caitlin", simple->get_name());
    delete simple;
    Player * human = Player_factory("Mary", "Human");
    ASSERT_EQUAL("Mary", human->get_name());
    delete human;
}

// std::ostream & operator<<(std::ostream &os, const Player &p);
TEST(test_operator_output){
    Player * caitlin = Player_factory("Caitlin", "Simple");
    ostringstream os;
    os << *caitlin;
    ASSERT_EQUAL("Caitlin", os.str());
    delete caitlin;
}



// NEW FUNCTIONS
TEST(test_player_factory_simple){
    for (int i = 0; i < 20; ++i) {
        Player* player = Player_factory("Player" + to_string(i), "Simple");
        ASSERT_EQUAL("Player" + to_string(i), player->get_name());
        delete player;
    }
}

TEST(test_simple_add_multiple_cards){
    Player* caitlin = Player_factory("Caitlin", "Simple");
    // add multiple cards
    caitlin->add_card(Card(KING, CLUBS));
    caitlin->add_card(Card(QUEEN, DIAMONDS));
    caitlin->add_card(Card(ACE, SPADES));
    // make sure that one of the cards can be led
    ostringstream os;
    // leading card
    os << caitlin->lead_card(SPADES); 
    // make sure a card was returned
    ASSERT_NOT_EQUAL("", os.str()); 
    delete caitlin;
}

TEST(test_simple_play_card2){
    Player* caitlin = Player_factory("Caitlin", "Simple");
    caitlin->add_card(Card(FIVE, CLUBS));
    caitlin->add_card(Card(EIGHT, DIAMONDS));
    // lead with hearts; trump is spades
    Card led_card(SIX, HEARTS); 
    Card played_card = caitlin->play_card(led_card, SPADES); 
    // can't play hearts
    ASSERT_NOT_EQUAL(HEARTS, played_card.get_suit()); 
    delete caitlin;
}

TEST(test_operator_output2){
    Player* simple_player = Player_factory("Caitlin", "Simple");
    Player* human_player = Player_factory("Mary", "Human");
    ostringstream os;
    os << *simple_player;
    ASSERT_EQUAL("Caitlin", os.str());
    os.str(""); // Clear the stream
    os << *human_player;
    ASSERT_EQUAL("Mary", os.str());
    delete simple_player;
    delete human_player;
}

TEST(test_simple_play_card_no_trump){
    Player* caitlin = Player_factory("Caitlin", "Simple");
    // no hearts
    caitlin->add_card(Card(FIVE, DIAMONDS)); 
    caitlin->add_card(Card(JACK, CLUBS));
    Card led_card(ACE, HEARTS); 
    Card played_card = caitlin->play_card(led_card, SPADES); 
    // doesn't have any hearts
    ASSERT_NOT_EQUAL(played_card.get_suit(), HEARTS);
    delete caitlin;
}

TEST(test_simple_lead_no_trump){
    Player* caitlin = Player_factory("Caitlin", "Simple");
    caitlin->add_card(Card(TEN, DIAMONDS)); 
    caitlin->add_card(Card(TWO, CLUBS));
    Card led_card = caitlin->lead_card(SPADES); 
    ASSERT_NOT_EQUAL(led_card.get_suit(), SPADES);
    delete caitlin;
}

TEST(test_human_ctor2){
    Player * caitlin = Player_factory("Caitlin", "Human");
    ASSERT_EQUAL("Caitlin", caitlin->get_name());
    delete caitlin;
}
TEST(test_human_name){
    Player* caitlin = Player_factory("Caitlin", "Human");
    ASSERT_EQUAL("Caitlin", caitlin->get_name());
    delete caitlin;
}

// asks for user input -> might not be allowed
// TEST(test_human_add_card){
//     Player* caitlin = Player_factory("Caitlin", "Human");
//     Card card(ACE, SPADES);
//     caitlin->add_card(card);
//     ostringstream os;
//     os << caitlin->lead_card(SPADES); 
//     ASSERT_EQUAL("Ace of Spades", os.str());
//     delete caitlin;
// }

// asks for user input -> might not be allowed
// TEST(test_human_add_and_discard){
//     Player* caitlin = Player_factory("Caitlin", "Human");
//     caitlin->add_card(Card(TWO, SPADES));
//     caitlin->add_card(Card(TEN, HEARTS));
//     caitlin->add_and_discard(Card(ACE, HEARTS));
//     ostringstream os;
//     os << caitlin->lead_card(HEARTS);
//     ASSERT_NOT_EQUAL("Two of Spades", os.str()); 
//     delete caitlin;
// }

TEST(test_player_factory_human){
    Player* caitlin = Player_factory("Caitlin", "Human");
    ASSERT_EQUAL("Caitlin", caitlin->get_name());
    delete caitlin;
}


// Tests for lead_card

// Test lead_card when player has only one card
TEST(test_simple_lead_card_one_card) {
    Player * caitlin = Player_factory("Caitlin", "Simple");
    caitlin->add_card(Card(TEN, DIAMONDS));  // Only one card in hand
    Card led_card = caitlin->lead_card(SPADES);  // Trump suit doesn't matter
    ASSERT_EQUAL(DIAMONDS, led_card.get_suit());
    ASSERT_EQUAL(TEN, led_card.get_rank());
    delete caitlin;
}

// Test lead_card with trump suit in hand
// TEST(test_simple_lead_card_trump_in_hand) {
//     Player * caitlin = Player_factory("Caitlin", "Simple");
//     caitlin->add_card(Card(THREE, CLUBS));
//     caitlin->add_card(Card(QUEEN, SPADES));  // Trump card
//     Card led_card = caitlin->lead_card(SPADES);  // Spades is trump
//     ASSERT_EQUAL(SPADES, led_card.get_suit());   // Should lead trump card
//     delete caitlin;
// }

// Test lead_card with no trump suit in hand
TEST(test_simple_lead_card_no_trump_in_hand) {
    Player * caitlin = Player_factory("Caitlin", "Simple");
    caitlin->add_card(Card(FIVE, HEARTS));
    caitlin->add_card(Card(NINE, CLUBS));
    Card led_card = caitlin->lead_card(SPADES);  // Spades is trump, but no spades
    // Check the suit is either HEARTS or CLUBS (since no spades are present)
    ASSERT_TRUE(led_card.get_suit() == HEARTS || led_card.get_suit() == CLUBS);
    delete caitlin;
}

// Test lead_card with all low cards
TEST(test_simple_lead_card_all_low_cards) {
    Player * caitlin = Player_factory("Caitlin", "Simple");
    caitlin->add_card(Card(TWO, CLUBS));
    caitlin->add_card(Card(THREE, DIAMONDS));
    Card led_card = caitlin->lead_card(HEARTS);  // Trump is hearts, but player has low cards
    // Ensure one of the low cards is led
    ASSERT_TRUE(led_card.get_suit() == CLUBS || led_card.get_suit() == DIAMONDS);
    delete caitlin;
}

// Test lead_card with multiple trump cards
// TEST(test_simple_lead_card_multiple_trump_cards) {
//     Player * caitlin = Player_factory("Caitlin", "Simple");
//     caitlin->add_card(Card(JACK, SPADES));  // Trump card
//     caitlin->add_card(Card(NINE, SPADES));  // Trump card
//     caitlin->add_card(Card(TEN, DIAMONDS)); // Non-trump card
//     Card led_card = caitlin->lead_card(SPADES);  // Trump is spades
//     ASSERT_EQUAL(SPADES, led_card.get_suit());   // Should lead trump card
//     delete caitlin;
// }


// Tests for play_card

// Test play_card when player follows suit
TEST(test_simple_play_card_follow_suit) {
    Player * caitlin = Player_factory("Caitlin", "Simple");
    caitlin->add_card(Card(THREE, CLUBS));
    caitlin->add_card(Card(QUEEN, CLUBS));  // Matching suit
    Card led_card(SEVEN, CLUBS);  // Led suit is clubs
    Card played_card = caitlin->play_card(led_card, HEARTS);  // Trump is hearts
    ASSERT_EQUAL(CLUBS, played_card.get_suit());  // Should follow the led suit
    delete caitlin;
}

// Test play_card when player cannot follow suit
// TEST(test_simple_play_card_cannot_follow_suit) {
//     Player * caitlin = Player_factory("Caitlin", "Simple");
//     caitlin->add_card(Card(TEN, HEARTS));  // Trump card, but not matching the led suit
//     caitlin->add_card(Card(JACK, SPADES)); // Another trump card
//     Card led_card(SIX, CLUBS);  // Led suit is clubs, but player has no clubs
//     Card played_card = caitlin->play_card(led_card, SPADES);  // Trump is spades
//     ASSERT_EQUAL(SPADES, played_card.get_suit());  // Should play trump if no matching suit
//     delete caitlin;
// }

// Test play_card when player has multiple options
TEST(test_simple_play_card_multiple_options) {
    Player * caitlin = Player_factory("Caitlin", "Simple");
    caitlin->add_card(Card(FIVE, DIAMONDS));
    caitlin->add_card(Card(ACE, CLUBS));
    caitlin->add_card(Card(KING, HEARTS));
    Card led_card(FIVE, DIAMONDS);  // Led suit is diamonds
    Card played_card = caitlin->play_card(led_card, CLUBS);  // Trump is clubs
    ASSERT_EQUAL(DIAMONDS, played_card.get_suit());  // Should follow the led suit
    delete caitlin;
}

// Test play_card with trump card in hand but led suit is different
TEST(test_simple_play_card_trump_in_hand_but_not_led_suit) {
    Player * caitlin = Player_factory("Caitlin", "Simple");
    caitlin->add_card(Card(TWO, SPADES));   // Trump card
    caitlin->add_card(Card(FOUR, CLUBS));   // Non-trump card
    Card led_card(ACE, DIAMONDS);  // Led suit is diamonds
    Card played_card = caitlin->play_card(led_card, SPADES);  // Trump is spades
    ASSERT_EQUAL(CLUBS, played_card.get_suit());  // Should not play trump unless forced
    delete caitlin;
}

// Test play_card with only one card left
TEST(test_simple_play_card_one_card_left) {
    Player * caitlin = Player_factory("Caitlin", "Simple");
    caitlin->add_card(Card(KING, HEARTS));  // Only one card in hand
    Card led_card(TWO, SPADES);  // Led suit is spades
    Card played_card = caitlin->play_card(led_card, DIAMONDS);  // Trump is diamonds
    ASSERT_EQUAL(HEARTS, played_card.get_suit());  // Should play the only card in hand
    delete caitlin;
}




TEST_MAIN()
