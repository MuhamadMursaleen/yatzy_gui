#ifndef GAMEENGINE_HH
#define GAMEENGINE_HH

#include <string>
#include <vector>
//#include "mainwindow.hh"

// Obvious constants
const int INITIAL_NUMBER_OF_ROLLS = 3;
const int NUMBER_OF_DICES = 5;

// Data of each player
struct Player
{
    // id of the player
    unsigned int id_;

    // how many rolls has left
    unsigned int rolls_left_;

    // list of latest points
    std::vector<int> latest_point_values_;

    // list of best points
    std::vector<int> best_point_values_;

    // list of dice position to be locked
    std::vector<bool> is_locked;
};

class GameEngine
{
public:
    // Constructor
    GameEngine();

    // Destructor
    ~GameEngine();

    // Adds a new player
    void add_player(const Player player);

    // Prints guide text, telling which player is in turn and how many trials
    // they have left.
    std::string  update_guide() const;

    // Rolls all dices, i.e. draws a new series of face numbers for the player
    // currently in turn. Moreover, reports the winner, if after the draw, all
    // players have used all their turns.
    std::vector<int> roll();

    // Gives turn for the next player having turns left, i.e. for the next
    // element in the players_ vector. After the last one, turn is given for
    // the second one (since the first one is NOBODY).
    void give_turn();

    // Reports a winner based on the current situation and sets the game_over_
    // attribute as true.
    std::string report_winner();

    // Tells if the game is over, i.e. if all players have used all their
    // turns.
    bool is_game_over() const;

    // update the check boxes locking in player.is_locked
    void update_check_boxes(std::vector<bool> check_boxes);

    // get the latest points of current player
    std::vector<int> get_latest_points();

    // get the dice locking mechanisim info so that locked dices cann't be chnaged
    std::vector<bool> get_locked_points();

    // get total numbers of player
    int get_players();


private:
    // Reports the status of the player currently in turn
    void report_player_status() const;

    // Updates best and latest points of the player in turn:
    // latest_point_values_ will always be new_points,
    // best_point_values_ will be new_points, if the last_mentioned is better.
    void update_points(const std::vector<int>& new_points);

    // Returns true if all turns of all players have been used,
    // otherwise returns false.
    bool all_turns_used() const;

    // Vector of all players
    std::vector<Player> players_;

    // Tells the player currently in turn (index of players_ vector)
    unsigned int game_turn_;

    // Tells if the game is over
    bool game_over_;
};

#endif // GAMEENGINE_HH
