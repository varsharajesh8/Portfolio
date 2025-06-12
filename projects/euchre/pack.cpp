#include "Pack.hpp"
#include "Card.hpp"
#include <iostream>
#include <sstream>
#include <algorithm>
#include <cassert>

//default constructor
Pack::Pack(){
  int index = 0;
  for(int suit = SPADES; suit <= DIAMONDS; ++suit){
    for(int rank = NINE; rank <= ACE; ++rank){
      cards[index] = Card(static_cast<Rank>(rank), static_cast<Suit>(suit));
      index++;
    }
  }
  next = 0;
}
// REQUIRES: pack_input contains a representation of a Pack in the
  //           format required by the project specification
  // MODIFIES: pack_input
  // EFFECTS: Initializes Pack by reading from pack_input.
  // NOTE: The pack is initially full, with no cards dealt.
  Pack::Pack(std::istream& pack_input){
    int index = 0;
    std::string line;
    while(std::getline(pack_input, line)){
        std::istringstream is(line);
        Card card;

        is >> card;
        //if(is){
        cards[index] = card;
        index++;
        //}
    }
    next = 0;
  }

  // REQUIRES: cards remain in the Pack
  // EFFECTS: Returns the next card in the pack and increments the next index
  Card Pack :: deal_one(){ 
    assert(next < PACK_SIZE); //check if pack still has cards to deal
    //Card card;
    //if(next == PACK_SIZE) //if count is equal to pack size returns empty card
      //return card;
    //else {
      return cards[next++]; //returns card at next index and increments next
    //}
  }


  // EFFECTS: Resets next index to first card in the Pack
  void Pack::reset(){
    next = 0; 
  }
    
  // EFFECTS: Shuffles the Pack and resets the next index. This
  //          performs an in shuffle seven times. See
  //          https://en.wikipedia.org/wiki/In_shuffle.
  bool shuffle_enabled= true; //initialize shuffle_enabled to true
  void Pack::shuffle() {
    if (!shuffle_enabled) { 
        reset();  
        return;  // exit if shuffle is not enabled
    }

    // shuffle the cards 7 times
    for (int i = 0; i < 7; i++) {  
        std::array<Card, PACK_SIZE> shuffle_cards;
        int first_half = 0;
        int second_half = PACK_SIZE / 2;

        // alternating between the first and second halves
        for (int j = 0; j < PACK_SIZE; j++) {  
            if (j % 2 == 0) {  
                shuffle_cards[j] = cards[second_half];
                second_half++;  
            } else {  
                shuffle_cards[j] = cards[first_half];
                first_half++;  
            }
        }
        for (int k = 0; k < PACK_SIZE; k++) {  
            cards[k] = shuffle_cards[k];
        }
    }

    reset();  
}



  // EFFECTS: returns true if there are no more cards left in the pack
  bool Pack::empty() const{
    return next == PACK_SIZE;
    
  }




