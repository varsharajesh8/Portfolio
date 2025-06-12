#include "Player.hpp"
#include <iostream>
#include <string>
#include "Card.hpp"
#include <vector>
#include <cassert>
#include <algorithm>


//simple player class
class SimplePlayer: public Player{
    private:
        std::string name;
        std::vector<Card> hand; 
        //function that counts face/ace of trump to avoid deep nest
        int count_face_ace_trump(const Suit &suit) const {
            int count = 0;
            for (size_t i = 0; i < hand.size(); ++i) {
                const Card &card = hand[i];
                if (card.is_face_or_ace() && card.is_trump(suit)) {
                    count++;
                }
            }
            return count;
        }

    public:
    SimplePlayer(const std::string &name){
        
        this->name = name;

    }
    const std::string & get_name() const override{
        return name;
    }
    void add_card(const Card &c) override{
        hand.push_back(c);
    }
    bool make_trump(const Card &upcard, bool is_dealer, 
        int round, Suit &order_up_suit) const override{
        Suit trump = upcard.get_suit();
        int count_face_or_ace = 0;
        //function to count for face or ace of trump
        
        //checks if its the first round
        if(round == 1){
            count_face_or_ace = count_face_ace_trump(trump);
            if(count_face_or_ace >= 2){
                order_up_suit = trump; 
                return true;
            }
        }else{
            if(!is_dealer){//if not dealer, choose same color suit as upcard
                trump = Suit_next(upcard.get_suit());
                //count face/ace for new trump suit
                count_face_or_ace = count_face_ace_trump(trump);
                if (count_face_or_ace >= 1) {
                    order_up_suit = trump; // Set the new trump suit
                    return true;
                }return false;
            }else{//screw the dealer, 
                trump = Suit_next(upcard.get_suit());
                order_up_suit = trump; //order up suit is trump suit
                return true; //dealer forced
            }
        }return false; //if no trump made return false, should never happen
    }

    
    void add_and_discard(const Card &upcard) override{
    assert(hand.size() >= 1); //ensuring player has at least one card
       // upcard picked up if trump suit is ordered up
      
       if (upcard.is_trump(upcard.get_suit())){ 
        hand.push_back (upcard); // adds upcard to hand
       }
       Card lowest_card = hand[0]; //initializes lowest card to first card in hand
       int lowest_index = 0; //initializes index to 0
       //finding lowest card in hand
       for (size_t i=0; i<hand.size(); i++){
        //if card is less than loswest card
              if (Card_less(hand[i], lowest_card, upcard.get_suit())){ 
                lowest_card = hand[i]; // lowest card is now the card
                lowest_index=i ; //updating value of lowest index
            }
       }
       //removes lowest card from hand
    hand.erase(hand.begin() + lowest_index); 
        //max hand size 5
    assert(hand.size() == MAX_HAND_SIZE); 
                 
}




    
  

 Card lead_card(Suit trump) override {
    assert(hand.size() >= 1); // Player has at least one card

    // Find the highest non-trump
    Card highest_non_trump = hand[0];
    bool has_non_trump = false;
    size_t chosen_index = 0;

    for (size_t i = 0; i < hand.size(); i++) {
        if (!hand[i].is_trump(trump)) {
            if (!has_non_trump || Card_less(highest_non_trump, hand[i], trump)) {
                highest_non_trump = hand[i];
                chosen_index = i;
                has_non_trump = true;
            }
        }
    }

    // If no non-trump, lead with the highest trump
    if (!has_non_trump) {
        Card highest_trump = hand[0];
        for (size_t i = 0; i < hand.size(); i++) {
            if (hand[i].is_trump(trump) && Card_less(highest_trump, hand[i], trump)) {
                highest_trump = hand[i];
                chosen_index = i;
            }
        }
    }

    // Remove the chosen card from hand
    Card chosen_card = hand[chosen_index];
    hand.erase(hand.begin() + chosen_index);
    return chosen_card;
}

  
  

Card play_card(const Card &led_card, Suit trump) override {
    assert(!hand.empty()); // Player has at least one card

    bool player_can_follow_suit = false;
    int highest_follow_suit_index = -1;
    int lowest_index = 0;

    // Iterate through hand to find:
    // 1. The highest card that follows suit (if possible)
    // 2. The lowest card overall
    for (size_t i = 0; i < hand.size(); i++) {
        if (hand[i].get_suit(trump) == led_card.get_suit(trump)) {
            player_can_follow_suit = true;

            // Update highest card that follows suit based on card index comparison
            if (highest_follow_suit_index == -1 || 
                Card_less(hand[highest_follow_suit_index], hand[i], led_card, trump)) {
                highest_follow_suit_index = i;
            }
        }

        // Track the lowest card index
        if (Card_less(hand[i], hand[lowest_index], trump)) {
            lowest_index = i;
        }
    }

    // Choose the appropriate card based on index
    int chosen_index;
    if (player_can_follow_suit) {
        chosen_index = highest_follow_suit_index;
    } else {
        chosen_index = lowest_index;
    }

    Card chosen_card = hand[chosen_index];

    // Remove the chosen card from hand by index
    hand.erase(hand.begin() + chosen_index);
    return chosen_card;
}


};


//human player class
class HumanPlayer : public Player{
    private:
    std::string name;
    std::vector<Card> hand;

    void print_hand() const {
    for (size_t i = 0; i < hand.size(); ++i) {
        std::cout << "Human player " << name << "'s hand: "
            << "[" << i << "] " << hand[i] << "\n";
    }
    }

    public:
    //constructor
    HumanPlayer(const std::string &name){
        this->name = name;
    }

    //returns player's name
    const std::string & get_name() const override{
        return name;
    }

    //adds card to player's hand
    void add_card(const Card &c) override{
        hand.push_back(c);
        std::sort(hand.begin(), hand.end());
    }

    bool make_trump(const Card &upcard, bool is_dealer,
                    int round, Suit &order_up_suit) const override {
        print_hand();
        std::cout << "Human player " << name << ", please enter a suit, or \"pass\":\n";
        std::string response;
        std::cin >> response;
        if (response == "pass") {
            return false;
        } else {
            order_up_suit = string_to_suit(response);
            return true;
        }
    }  
    void add_and_discard(const Card &upcard) override{
        assert(hand.size() == MAX_HAND_SIZE);
        
        std::sort(hand.begin(), hand.end());

        print_hand();
        std::cout << "Discard upcard: [-1]\n";
        std::cout << "Human player " << name << ", please select a card to discard:\n";

        int discard_index;
        std::cin >> discard_index;

        if(discard_index == -1){
            return;
        }
        else{
            hand.erase(hand.begin() + discard_index);
            hand.push_back(upcard);
            std::sort(hand.begin(), hand.end());
        }
        //print_hand();
        assert(hand.size() == MAX_HAND_SIZE);
        
        std::cout << "\n";
    }


    Card lead_card(Suit trump) override {
    print_hand();
    std::cout << "Human player " << name << ", please select a card:\n";
    int card_index;
    while (true) {
        std::cin >> card_index;
        if (card_index >= 0 && card_index < hand.size()) {
            Card card = hand[card_index];
            hand.erase(hand.begin() + card_index);
            //std::cout << card << " led by " << name << std::endl;
            return card;
        }
    }
}

Card play_card(const Card &led_card, Suit trump) override {
    print_hand();
    std::cout << "Human player " << name << ", please select a card:\n";
    int card_index;
    while (true) {
        std::cin >> card_index;
        if (card_index >= 0 && card_index < hand.size()) {
            Card card = hand[card_index];
            hand.erase(hand.begin() + card_index);
            //std::cout << card << " played by " << name << std::endl;
            return card;
        }
    }
}
}; 


//EFFECTS: Returns a pointer to a player with the given name and strategy
//To create an object that won't go out of scope when the function returns,
//use "return new Simple(name)" or "return new Human(name)"
//Don't forget to call "delete" on each Player* after the game is over
Player * Player_factory(const std::string &name, const std::string &strategy){
    if(strategy == "Human"){
        return new HumanPlayer(name);
    }
    else if (strategy == "Simple"){
        return new SimplePlayer(name);
    }

    assert(false); //if strategy is not human or simple, crash 
    return nullptr;
}
//EFFECTS: Prints player's name to os
std::ostream & operator<<(std::ostream &os, const Player &p){
    os << p.get_name();
    return os;
}
 