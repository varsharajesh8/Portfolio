#include <cassert>
#include <iostream>
#include <array>
#include "Card.hpp"
using namespace std;
/////////////// Rank operator implementations - DO NOT CHANGE ///////////////
constexpr const char *const RANK_NAMES[] = {
  "Two",   // TWO
  "Three", // THREE
  "Four",  // FOUR
  "Five",  // FIVE
  "Six",   // SIX
  "Seven", // SEVEN
  "Eight", // EIGHT
  "Nine",  // NINE
  "Ten",   // TEN
  "Jack",  // JACK
  "Queen", // QUEEN
  "King",  // KING
  "Ace"    // ACE
};
//REQUIRES str represents a valid rank ("Two", "Three", ..., "Ace")
//EFFECTS returns the Rank corresponding to str, for example "Two" -> TWO
Rank string_to_rank(const std::string &str) {
  for(int r = TWO; r <= ACE; ++r) {
    if (str == RANK_NAMES[r]) {
      return static_cast<Rank>(r);
    }
  }
  assert(false); // Input string didn't match any rank
  return {};
}
//EFFECTS Prints Rank to stream, for example "Two"
std::ostream & operator<<(std::ostream &os, Rank rank) {
  os << RANK_NAMES[rank];
  return os;
}
//REQUIRES If any input is read, it must be a valid rank
//EFFECTS Reads a Rank from a stream, for example "Two" -> TWO
std::istream & operator>>(std::istream &is, Rank &rank) {
  string str;
  if(is >> str) {
    rank = string_to_rank(str);
  }
  return is;
}
/////////////// Suit operator implementations - DO NOT CHANGE ///////////////
constexpr const char *const SUIT_NAMES[] = {
  "Spades",   // SPADES
  "Hearts",   // HEARTS
  "Clubs",    // CLUBS
  "Diamonds", // DIAMONDS
};
//REQUIRES str represents a valid suit ("Spades", "Hearts", "Clubs", or "Diamonds")
//EFFECTS returns the Suit corresponding to str, for example "Clubs" -> CLUBS
Suit string_to_suit(const std::string &str) {
  for(int s = SPADES; s <= DIAMONDS; ++s) {
    if (str == SUIT_NAMES[s]) {
      return static_cast<Suit>(s);
    }
  }
  assert(false); // Input string didn't match any suit
  return {};
}
//EFFECTS Prints Suit to stream, for example "Spades"
std::ostream & operator<<(std::ostream &os, Suit suit) {
  os << SUIT_NAMES[suit];
  return os;
}
//REQUIRES If any input is read, it must be a valid suit
//EFFECTS Reads a Suit from a stream, for example "Spades" -> SPADES
std::istream & operator>>(std::istream &is, Suit &suit) {
  string str;
  if (is >> str) {
    suit = string_to_suit(str);
  }
  return is;
}
/////////////// Write your implementation for Card below ///////////////
//default constructor 2 of spades
Card::Card() : rank(TWO), suit(SPADES){}
//constructor with specific rank and suit
Card::Card(Rank rank_in, Suit suit_in) : rank(rank_in), suit(suit_in){}
//return rank
Rank Card::get_rank() const{
  return rank;
}
//return suit
Suit Card::get_suit() const{
  return suit; 
}
Suit Card::get_suit(Suit trump) const{
  if(rank == JACK && suit == Suit_next(trump)){
    return trump; 
  }
  return suit; 
}
bool Card::is_face_or_ace() const{
  return rank == JACK || rank == QUEEN || rank == KING || rank == ACE; 
}
bool Card::is_right_bower(Suit trump) const{
  return rank == JACK && suit == trump; 
}
bool Card::is_left_bower(Suit trump) const{
  return (rank == JACK) && (suit == Suit_next(trump)); 
}
bool Card::is_trump(Suit trump) const{
  return suit == trump || is_left_bower(trump); 
}
// prints card to stream 
std::ostream& operator<<(std::ostream &os, const Card &card){
  os << RANK_NAMES[card.get_rank()] << " of " << SUIT_NAMES[card.get_suit()];
  return os;
}
// reads card from stream 
std::istream & operator>>(std::istream &is, Card &card){
  std::string rank_string;
  std::string suit_string;
  
  if (is >> rank_string >> suit_string){
    //ignore the word "of"
    if (suit_string == "of"){
      is >> suit_string; 
    }
    
    card.rank = string_to_rank(rank_string);
    card.suit = string_to_suit(suit_string); 
  }
  return is; 
}
//EFFECTS Returns true if lhs is lower value than rhs.
//  Does not consider trump.
bool operator<(const Card &lhs, const Card &rhs){
    // Compare ranks first
  if (lhs.get_rank() != rhs.get_rank()) {
    return lhs.get_rank() < rhs.get_rank();
  }
  else{
  // If ranks are equal, compare suits based on the predefined order
    return lhs.get_suit() < rhs.get_suit();
  }
}
//EFFECTS Returns true if lhs is lower value than rhs or the same card as rhs.
//  Does not consider trump.
bool operator<=(const Card &lhs, const Card &rhs){
    if (lhs.get_rank() != rhs.get_rank()) {
    return lhs.get_rank() <= rhs.get_rank();
  }
  else{
  // If ranks are equal, compare suits based on the predefined order
  return lhs.get_suit() <= rhs.get_suit();
  }
}
//EFFECTS Returns true if lhs is higher value than rhs.
//  Does not consider trump.
bool operator>(const Card &lhs, const Card &rhs){
  if (lhs.get_rank() != rhs.get_rank()) {
    return lhs.get_rank() > rhs.get_rank();
  }
  else{
  // If ranks are equal, compare suits based on the predefined order
  return lhs.get_suit() > rhs.get_suit();
  }
}
//EFFECTS Returns true if lhs is higher value than rhs or the same card as rhs.
//  Does not consider trump.
bool operator>=(const Card &lhs, const Card &rhs){
   if (lhs.get_rank() != rhs.get_rank()) {
    return lhs.get_rank() >= rhs.get_rank();
  }
  else{
  // If ranks are equal, compare suits based on the predefined order
  return lhs.get_suit() >= rhs.get_suit();
  }
}
//EFFECTS Returns true if lhs is same card as rhs.
//  Does not consider trump.
bool operator==(const Card &lhs, const Card &rhs){
  return lhs.get_rank() == rhs.get_rank() && lhs.get_suit() == rhs.get_suit();
}
//EFFECTS Returns true if lhs is not the same card as rhs.
//  Does not consider trump.
bool operator!=(const Card &lhs, const Card &rhs){
  return lhs.get_rank() != rhs.get_rank() || lhs.get_suit() != rhs.get_suit(); 
}
//EFFECTS returns the next suit, which is the suit of the same color
Suit Suit_next(Suit suit){
   
  if (suit == SPADES){
    return CLUBS;
  }
  else if (suit == CLUBS){
    return SPADES;
  }
  else if (suit == HEARTS){
    return DIAMONDS;
  }
  else if (suit == DIAMONDS){
    return HEARTS; 
  }
  else{
    assert(false && "Invalid suit");
    return static_cast<Suit>(-1); //invalid suit
  }
  
}
//EFFECTS Returns true if a is lower value than b.  Uses trump to determine
// order, as described in the spec.
bool Card_less(const Card &a, const Card &b, Suit trump){
  if (a.is_right_bower(trump)){
    return false; // Right Bower is highest
  }
  if (b.is_right_bower(trump)) {
    return true;  // Other card is lower
  }
  if(a.is_left_bower(trump)){
    return false; // Left Bower is higher
  }
  if(b.is_left_bower(trump)){
    return true; // Other card is lower
  }
  if(a.is_trump(trump) && !b.is_trump(trump)){
    //if a is trump and b isn't, a is higher than b so false
    return false; 
  }
  if(!a.is_trump(trump) && b.is_trump(trump)){
    //if a is not trump and b is trump, b is higher than a so true
    return true;
  }
  //if both cards are trump or neither are trump or left bower
  return a < b; 
}
//EFFECTS Returns true if a is lower value than b.  Uses both the trump suit
//  and the suit led to determine order, as described in the spec.
bool Card_less(const Card &a, const Card &b, const Card &led_card, Suit trump){
  //if both cards from trump suit or bowers
    if(a.is_right_bower(trump) && !b.is_right_bower(trump)){
      return false; // a is higher
    }    
    if(!a.is_right_bower(trump) && b.is_right_bower(trump)){
      return true; //b is higher
    }      
    if(a.is_left_bower(trump) && !b.is_left_bower(trump)){
      return false; //a is higher
    }
    if(!a.is_left_bower(trump) && b.is_left_bower(trump)){
      return true; //b is higher
    }
    if (a.is_trump(trump) && !b.is_trump(trump)){
      return false; //a is higher
    }
    if (!a.is_trump(trump) && b.is_trump(trump)){
      return true; //b is higher
    }
    if(a.get_suit() == led_card.get_suit() && b.get_suit() == led_card.get_suit()){
      return a < b; 
    }
    if(a.get_suit() == led_card.get_suit() && b.get_suit() != led_card.get_suit()){
      return false; //a is higher
    }
    if(b.get_suit() == led_card.get_suit() && a.get_suit() != led_card.get_suit()){
      return true; //b is higher
    }
    if(a.get_suit() != led_card.get_suit() && b.get_suit() != led_card.get_suit()){
      return a < b; 
    }
    return false;
}
// NOTE: We HIGHLY recommend you check out the operator overloading
// tutorial in the project spec before implementing
// the following operator overload functions:
//   operator<<
//   operator>>
//   operator<
//   operator<=
//   operator>
//   operator>=
//   operator==
//   operator!=
