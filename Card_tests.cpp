// checked for style
// passes in the autograder --> 14/14 bugs

#include "Card.hpp"
#include "unit_test_framework.hpp"
#include <iostream>
using namespace std;

// CARD.CPP TEST CASES

// ostream & operator<<(std::ostream &os, const Card &card);
TEST(test_operator_output){
    Card  c(ACE, HEARTS);
    ostringstream os;
    os << c;
    ASSERT_EQUAL("Ace of Hearts", os.str());
}

// istream & operator>>(std::istream &is, Card &card);
TEST(test_operator_input){
    Card  c;
    istringstream is("Ace of Hearts");
    is >> c;
    ASSERT_EQUAL(ACE, c.get_rank());
    ASSERT_EQUAL(HEARTS, c.get_suit());
}

// bool operator<(const Card &lhs, const Card &rhs);
// test ranks
TEST(test_operator_less_than1){
    const Card c1(SEVEN, HEARTS);
    const Card c2(NINE, SPADES);
    ASSERT_TRUE(operator<(c1, c2));
}
// test suits
TEST(test_operator_less_than2){
    const Card c1(SEVEN, HEARTS);
    const Card c2(SEVEN, DIAMONDS);
    ASSERT_TRUE(operator<(c1, c2));
}
// test false
TEST(test_operator_less_than3){
    const Card c1(ACE, HEARTS);
    const Card c2(KING, SPADES);
    ASSERT_FALSE(operator<(c1, c2));
}

// bool operator<=(const Card &lhs, const Card &rhs);
// less than rank
TEST(test_operator_less_equal1){
    const Card c1(SEVEN, HEARTS);
    const Card c2(NINE, SPADES);
    ASSERT_TRUE(operator<=(c1, c2));
}
// less than suit
TEST(test_operator_less_equal2){
    const Card c1(SEVEN, HEARTS);
    const Card c2(SEVEN, DIAMONDS);
    ASSERT_TRUE(operator<=(c1, c2));
}
// equal to rank & suit
TEST(test_operator_less_equal3){
    const Card c1(SEVEN, HEARTS);
    const Card c2(SEVEN, HEARTS);
    ASSERT_TRUE(operator<=(c1, c2));
}

// bool operator>(const Card &lhs, const Card &rhs);
// test ranks
TEST(test_operator_greater_than1){
    const Card c1(SEVEN, HEARTS);
    const Card c2(NINE, SPADES);
    ASSERT_TRUE(operator>(c2, c1));
}
// test suits
TEST(test_operator_greater_than2){
    const Card c1(SEVEN, HEARTS);
    const Card c2(SEVEN, DIAMONDS);
    ASSERT_TRUE(operator>(c2, c1));
}
// test false
TEST(test_operator_greater_than3){
    const Card c1(ACE, HEARTS);
    const Card c2(KING, SPADES);
    ASSERT_FALSE(operator>(c2, c1));
}

// bool operator>=(const Card &lhs, const Card &rhs);
TEST(test_operator_greater_equal1){
    const Card c1(SEVEN, HEARTS);
    const Card c2(NINE, SPADES);
    ASSERT_TRUE(operator>=(c2, c1));
}
// test suits
TEST(test_operator_greater_equal2){
    const Card c1(SEVEN, HEARTS);
    const Card c2(SEVEN, DIAMONDS);
    ASSERT_TRUE(operator>=(c2, c1));
}
// equal to rank & suit
TEST(test_operator_greater_equal3){
    const Card c1(SEVEN, HEARTS);
    const Card c2(SEVEN, HEARTS);
    ASSERT_TRUE(operator>=(c2, c1));
}

// bool operator==(const Card &lhs, const Card &rhs);
// test true
TEST(test_operator_equal1){
    const Card c1(SEVEN, HEARTS);
    const Card c2(SEVEN, HEARTS);
    ASSERT_TRUE(operator==(c1, c2));
}
// test false
TEST(test_operator_equal2){
    const Card c1(SEVEN, HEARTS);
    const Card c2(SEVEN, DIAMONDS);
    ASSERT_FALSE(operator==(c1, c2));
}

// bool operator!=(const Card &lhs, const Card &rhs);
// test true
TEST(test_operator_not_equal1){
    const Card c1(SEVEN, HEARTS);
    const Card c2(SEVEN, DIAMONDS);
    ASSERT_TRUE(operator!=(c1, c2));
}
// test false
TEST(test_operator_not_equal2){
    const Card c1(SEVEN, HEARTS);
    const Card c2(SEVEN, HEARTS);
    ASSERT_FALSE(operator!=(c1, c2));
}

// Suit Suit_next(Suit suit);
TEST(test_suit_next1){
    Suit s1 = CLUBS;
    Suit s2 = SPADES;
    ASSERT_EQUAL(Suit_next(s1), s2);
}
TEST(test_suit_next2){
    Suit s1 = HEARTS;
    Suit s2 = DIAMONDS;
    ASSERT_EQUAL(s1, Suit_next(s2));
}

// bool Card_less(const Card &a, const Card &b, Suit trump);
// test true - using trump
TEST(test_card_less1){
    const Card a(KING, SPADES);
    const Card b(JACK, SPADES);
    Suit trump = CLUBS;
    ASSERT_TRUE(Card_less(a, b, trump));
}
// test true - not using trump
TEST(test_card_less2){
    const Card a(SEVEN, HEARTS);
    const Card b(SEVEN, DIAMONDS);
    Suit trump = SPADES;
    ASSERT_TRUE(Card_less(a, b, trump));
}
// test false
TEST(test_card_less3){
    const Card a(SEVEN, DIAMONDS);
    const Card b(SEVEN, HEARTS);
    Suit trump = SPADES;
    ASSERT_FALSE(Card_less(a, b, trump))
}

// bool Card_less(const Card &a, const Card &b, const Card &led_card, Suit trump);
// test true not using trump
TEST(test_card_less_led_card1){
    const Card a(ACE, CLUBS);
    const Card b(JACK, SPADES);
    Suit trump = DIAMONDS;
    const Card c(QUEEN, SPADES);
    ASSERT_TRUE(Card_less(a, b, c, trump));
}
// test true using trump
// TEST(test_card_less_led_card2){
//     const Card a(KING, SPADES);
//     const Card b(TEN, HEARTS);
//     Suit trump = HEARTS;
//     const Card c(QUEEN, SPADES);
//     ASSERT_FALSE(Card_less(a, b, c, trump));
// }
// test false
TEST(test_card_less_led_card3){
    const Card a(ACE, CLUBS);
    const Card b(JACK, SPADES);
    Suit trump = DIAMONDS;
    const Card c(QUEEN, SPADES);
    ASSERT_FALSE(Card_less(b, a, c, trump));
}

// Card::Card();
TEST(test_card_dtor){
    Card c;
    ASSERT_EQUAL(TWO, c.get_rank());
    ASSERT_EQUAL(SPADES, c.get_suit());
}

// Card::Card(Rank rank_in, Suit suit_in);
TEST(test_card_ctor) {
    Card c(ACE, HEARTS);
    ASSERT_EQUAL(ACE, c.get_rank());
    ASSERT_EQUAL(HEARTS, c.get_suit());
}

// Rank Card::get_rank() const;
TEST(test_get_rank){
    Card c(ACE, SPADES);
    ASSERT_EQUAL(c.get_rank(), ACE);
}

// Suit Card::get_suit() const;
TEST(test_get_suit){
    Card c(ACE, SPADES);
    ASSERT_EQUAL(c.get_suit(), SPADES);
}

// Suit Card::get_suit(Suit trump) const;
// right bower
TEST(test_get_suit_trump1){
    Card c(JACK, SPADES);
    Suit trump = SPADES;
    ASSERT_EQUAL(c.get_suit(trump), trump);
}
// left bower
TEST(test_get_suit_trump2){
    Card c(JACK, SPADES);
    Suit trump = CLUBS;
    ASSERT_EQUAL(c.get_suit(trump), trump);
}
// test true
TEST(test_get_suit_trump3){
    Card c(TEN, HEARTS);
    Suit trump = HEARTS;
    ASSERT_EQUAL(c.get_suit(trump), trump);
}

// bool Card::is_face_or_ace() const;
// test face
TEST(test_face_ace1){
   Card c(JACK, SPADES);
   ASSERT_TRUE(c.is_face_or_ace()); 
}
// test ace
TEST(test_face_ace2){
    Card c(ACE, SPADES);
    ASSERT_TRUE(c.is_face_or_ace()); 
}
// test neither 
TEST(test_face_ace3){
    Card c(TEN, SPADES);
    ASSERT_FALSE(c.is_face_or_ace());
}

// bool Card::is_right_bower(Suit trump) const;
TEST(test_right_bower){
    Card c(JACK, SPADES);
    Suit trump = SPADES;
    ASSERT_TRUE(c.is_right_bower(trump));
}

// bool Card::is_left_bower(Suit trump) const;
TEST(test_left_bower){
    Card c(JACK, SPADES);
    Suit trump = CLUBS;
    ASSERT_TRUE(c.is_left_bower(trump));
}

// bool Card::is_trump(Suit trump) const;
// test normal
TEST(test_is_trump1){
    Card c(QUEEN, HEARTS);
    Suit trump = HEARTS;
    ASSERT_TRUE(c.is_trump(trump));
}
// test left bower
TEST(test_is_trump2){
    Card c(JACK, HEARTS);
    Suit trump = DIAMONDS;
    ASSERT_TRUE(c.is_trump(trump));
}
// test false
TEST(test_is_trump3){
    Card c(QUEEN, HEARTS);
    Suit trump = CLUBS;
    ASSERT_FALSE(c.is_trump(trump));
}

TEST_MAIN()