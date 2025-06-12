#include <iostream>
#include <string>
#include <vector>
#include "Card.hpp"
#include "Pack.hpp"
#include "Player.hpp"
#include <cassert>
#include <fstream>

class Game {
public:
    // default constructor
    Game(const std::string &pack_file, bool shuffle, 
        int points_to_win, std::vector <Player *> &players);
    void play(); 
private:
    std::vector <Player *> players; //vector of pointers to players
    Pack pack; //pack of cards in game
    int points_to_win; //points needed to win game
    int dealer = 0; //dealer index
    int team1_points = 0;
    int team2_points = 0;
    bool team1_ordered_up = false;
    bool team2_ordered_up = false;
    bool game_over = false;
    int hand_num = 0; 
    int team1_tricks = 0;
    int team2_tricks = 0;
    bool shuffle = false;

    //helper functions
    void shuffle_pack(); 
    void deal();
    void make_trump(Suit &trump, bool &team1_ordered_up, bool &team2_ordered_up);
    void play_hand(); 
    void score_hand(int tricks_won, bool team_ordered_up);
    void next_dealer(); 
    void check_winner();
    int find_trick_winner(const std::vector <Card> &trick_cards, Suit trump);
    void print_score();
};

// constructor
Game::Game(const std::string &pack_file, bool shuffle, 
    int points_to_win, std::vector <Player *> &players)
    : players(players), points_to_win(points_to_win), dealer(0), shuffle(shuffle) {
    // open pack file
    std::ifstream pack_filename(pack_file);
    assert(pack_filename.is_open()); // check if file is open

    pack = Pack(pack_filename); // initialize pack
    //if (shuffle) {
       // shuffle_pack(); // Shuffle the deck if shuffle is enabled
    //}
}

void Game::shuffle_pack() {
    pack.shuffle(); // shuffle pack
}

void Game::deal() {
    
    pack.reset(); // reset pack
    std::cout << players[dealer]->get_name() << " deals" << std::endl;

    int num_players = players.size();
    int player_starting = (dealer + 1) % num_players;
    int batches[] = {3, 2, 3, 2, 2, 3, 2, 3};

    for (int batch = 0; batch < 8; ++batch) {
        int numCards = batches[batch];
        int current_player = (player_starting + batch) % num_players; // rotate players
        for (int card = 0; card < numCards; ++card) {
            players[current_player]->add_card(pack.deal_one()); // deal 1 card
        }
    }
}

void Game::make_trump(Suit &trump, bool &team1_ordered_up, bool &team2_ordered_up) {
    Card upcard = pack.deal_one();
    std::cout << upcard << " turned up\n";

    // Round 1 - Players decide whether to order up the upcard suit
    for (size_t i = (dealer + 1) % players.size(); ; i = (i + 1) % players.size()) {
        bool player_orders = players[i]->make_trump(upcard, false, 1, trump);
        if (player_orders) {
            std::cout << players[i]->get_name() << " orders up " << trump << "\n";
            std::cout << "\n";
            if ((i % 2 == 0) || (i == dealer && i % 2 == 0)) {
                team1_ordered_up = true;
            } else {
                team2_ordered_up = true;
            }
            players[dealer]->add_and_discard(upcard);
            return;
        }
        std::cout << players[i]->get_name() << " passes\n";

        if(i == dealer) {
            break; // break if dealer
        }
    }

    // Round 2 - If no one ordered up, players choose from the same-color suit
    for (size_t i = (dealer + 1) % players.size(); 
    i != dealer; i = (i + 1) % players.size()) {
        bool player_orders = players[i]->make_trump(upcard, false, 2, trump);
        if (player_orders) {
            std::cout << players[i]->get_name() << " orders up " << trump << "\n";
            std::cout << "\n";
            if (i % 2 == 0) {
                team1_ordered_up = true; // Even-index players are in team 1
            } else {
                team2_ordered_up = true; // Odd-index players are in team 2
            }
            return; // Stop once a player orders up
        }
        std::cout << players[i]->get_name() << " passes\n";
    }

    //screw the dealer
    if (!team1_ordered_up && !team2_ordered_up) {
    bool dealer_orders = players[dealer]->make_trump
    (upcard, true, 2, trump);
    std::cout << players[dealer]->get_name() << " is forced to order up " 
    << trump << "\n";
    std::cout << "\n";
    if (dealer_orders) {
        if (dealer % 2 == 0) {
            team1_ordered_up = true; 
        } else {
            team2_ordered_up = true; 
        }
    }
}
    
}


void Game::play_hand() {

    std::cout << "Hand " << hand_num << "\n";
    Suit trump; // Declare trump
    team1_ordered_up = false;
    team2_ordered_up = false;
    team1_tricks = 0;
    team2_tricks = 0;
    
    if (shuffle){
        shuffle_pack();
    }
    else {
        pack.reset();
    }
   
    deal();
    make_trump(trump, team1_ordered_up, team2_ordered_up); // Initialize trump


    // The first trick starts with the player to the left of the dealer
    int trick_leader = (dealer + 1) % players.size();

    for (int trick = 0; trick < 5; ++trick) {
        std::vector<Card> trick_cards; 

        // Lead card by trick leader
        Card led_card = players[trick_leader]->lead_card(trump);
        trick_cards.push_back(led_card);
        std::cout << led_card << " led by " << players[trick_leader]->get_name() << "\n";

        // Other players follow
        for (int i = 1; i < players.size(); ++i) {
            int current_player = (trick_leader + i) % players.size();
            Card played = players[current_player]->play_card(led_card, trump);
            trick_cards.push_back(played);
            std::cout << played << " played by " << 
            players[current_player]->get_name() << "\n";
        }

        // Find trick winner
        int trick_winner_index = (trick_leader + 
            find_trick_winner(trick_cards, trump)) % players.size();
        std::cout << players[trick_winner_index]->get_name() << " takes the trick\n";

        // winner of this trick leads the next trick
        trick_leader = trick_winner_index;
      
   
       // Assign trick points
        if (trick_winner_index % 2 == 0) {
            team1_tricks++;
        } else {
            team2_tricks++;
        }

        std::cout << "\n";
    }

    if (team1_tricks > team2_tricks) {
        std::cout << players[0]->get_name() << " and " << players[2]->get_name() 
        << " win the hand\n";
        score_hand(team1_tricks, team1_ordered_up);
        
    } else {
        std::cout << players[1]->get_name() << " and " << players[3]->get_name() 
        << " win the hand\n";
        score_hand(team2_tricks, team2_ordered_up);
        

   
    }
    hand_num++;
}

void Game::check_winner() {
    if (team1_points >= points_to_win) {
        std::cout << players[0]->get_name() << " and " << players[2]->get_name() 
        << " win!" << std::endl;
        game_over = true;
        return;
    } else if (team2_points >= points_to_win) {
        std::cout << players[1]->get_name() << " and " << players[3]->get_name() 
        << " win!" << std::endl;
        game_over = true;
        return; 
    }
}



void Game::next_dealer() {
   dealer = (dealer + 1) % players.size(); // move dealer one position to the left
}


void Game::score_hand(int tricks_won, bool team_ordered_up) {
    if (team1_ordered_up){
        if (team1_tricks == 5){
            std::cout << "march!\n";
            team1_points += 2;
        } else if (team1_tricks == 3 || team1_tricks == 4){
            team1_points += 1;
        } else {
            std::cout << "euchred!\n";
            team2_points += 2;
        }
    } else if (team2_ordered_up){
        if (team2_tricks == 5){
            std::cout << "march!\n";
            team2_points += 2;
        } else if (team2_tricks == 3 || team2_tricks == 4){
            team2_points += 1;
        } else {
            std::cout << "euchred!\n";
            team1_points += 2;
        }
    }
    print_score();
    check_winner();
    if (game_over){
        return;
    }
}


int Game::find_trick_winner(const std::vector<Card>& trick_cards, Suit trump) {
    int winner = 0;
    Card lead_card = trick_cards[0];

    for (size_t i = 1; i < trick_cards.size(); ++i) {
        if (Card_less(trick_cards[winner], trick_cards[i], lead_card, trump)) {
            winner = i;
        }
    }
    return winner;
}

void Game::play() {
    while (!game_over) {
       
        play_hand();        
        if (game_over) {
            break;
        }
        next_dealer();
    }
}

// Add this function to print the current score
void Game::print_score() {
    std::cout << players[0]->get_name() << " and " << players[2]->get_name() 
              << " have " << team1_points << " points" << std::endl;              
    std::cout
              << players[1]->get_name() << " and " << players[3]->get_name() 
              << " have " << team2_points << " points" << std::endl ;
    std::cout << "\n";
}

void print_usage() {
    std::cout << "Usage: euchre.exe PACK_FILENAME [shuffle|noshuffle] "
        << "POINTS_TO_WIN NAME1 TYPE1 NAME2 TYPE2 NAME3 TYPE3 "
        << "NAME4 TYPE4" << std::endl;
}

int main(int argc, char * argv[]) {
    //print arguments
    for (int i = 0; i < argc; ++i) {
        std::cout << argv[i] << " ";
    }
    std::cout << std::endl;  
    
    
    if (argc != 12) {
        print_usage();
        return 1;
    }

    std::string pack_filename = argv[1];
    int points_to_win = std::stoi(argv[3]);
    if (points_to_win < 1 || points_to_win > 100) {
        print_usage();
        return 1;
    }

    bool shuffle = false;
    if (std::string(argv[2]) == "shuffle") {
        shuffle = true;
    } else if (std::string(argv[2]) == "noshuffle") {
        shuffle = false;
    } else {
        print_usage();
        return 1;
    }

    std::vector <Player *> players;
    for (int i = 4; i < argc; i += 2) {
        std::string name = argv[i];
        std::string type = argv[i + 1];
        Player * player = Player_factory(name, type);
        players.push_back(player);
    }

    std::ifstream pack_file(pack_filename);
    if (!pack_file.is_open()) {
        std::cout << "Error opening " << pack_filename << std::endl;
        return 1;
    }

    Game game(pack_filename, shuffle, points_to_win, players);
    game.play();

    for (size_t i = 0; i < players.size(); ++i) {
        delete players[i];
    }

    return 0;
}