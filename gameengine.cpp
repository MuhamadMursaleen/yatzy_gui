#include "gameengine.hh"
#include "functions.hh"
#include <iostream>
#include <sstream>

GameEngine::GameEngine():
    game_turn_(0), game_over_(false)
{
}

GameEngine::~GameEngine()
{
}

void GameEngine::add_player(const Player player)
{
    players_.push_back(player);
}

std::string GameEngine::update_guide() const
{
    if(players_.size() <= game_turn_)
    {
        std::cout << "Internal error: update_guide" << std::endl;
        return "Internal error: update_guide" ;
    }
    ostringstream outputstream{""};
    outputstream << "Player " << game_turn_ + 1 << " in turn, "
                 << players_.at(game_turn_).rolls_left_ << " trials left!";
    std::cout << outputstream.str() << std::endl;
    return outputstream.str();
}

vector<int> GameEngine::roll()
{
    vector<int> new_points;
    if(players_.size() <= game_turn_)
    {
        std::cout << "Internal error: roll" << std::endl;
        return new_points;
    }

    if(players_.at(game_turn_).rolls_left_ == 0)
    {
        std::cout << "No more rolls left" << std::endl;
        return new_points;
    }

    ostringstream outputstream{""};

    unsigned int dice = 0;
    while ( dice < NUMBER_OF_DICES )
    {
        int point_value;

        // if current dice is locked then parse the previous point
        if(players_.at(game_turn_).is_locked.at(dice))
        {
            point_value = players_.at(game_turn_).latest_point_values_.at(dice);
        }

        // else roll the dice and get random number
        else
        {
           point_value = roll_dice() ;
        }

        std::cout << point_value << " ";
        new_points.push_back(point_value);
        ++dice;
    }

    update_points(new_points);
    report_player_status();

    // Decreasing rolls left
    --players_.at(game_turn_).rolls_left_;

    // Checking if the player in turn has rolls left
    if ( players_.at(game_turn_).rolls_left_ == 0 )
    {
        outputstream << "Turn of " << players_.at(game_turn_).id_
                     << " has ended!";
        std::cout << outputstream.str() << std::endl;
    }

    // Checking if any player has turns left
    if ( all_turns_used() )
    {
        report_winner();
    }
    // return the new points
    return new_points;
}

void GameEngine::give_turn()
{
    // Searching for the next player among those, whose id_ is greater than
    // that of the current player
    for ( unsigned int i = game_turn_ + 1; i < players_.size(); ++i )
    {
        if ( players_.at(i).rolls_left_ > 0 )
        {
            game_turn_ = i;
            return;
        }
    }

    // A suitable next player couldn't be found in the previous search, so
    // searching for the next player among those, whose id_ is less than
    // or equal to that of the current player
    // (perhaps the current player is the only one having turns left)
    for(unsigned int i = 0; i <= game_turn_; ++i)
    {
        if(players_.at(i).rolls_left_ > 0)
        {
            game_turn_ = i;
            return;
        }
    }

    // No player has turns left
    report_winner();
}

std::string GameEngine::report_winner()
{
    vector<vector<int>> all_point_values;
    for ( auto player : players_ )
    {
        all_point_values.push_back(player.best_point_values_);
    }
    string winner_text = decide_winner(all_point_values);
    std::cout << winner_text << std::endl;
    game_over_ = true;
    return winner_text;
}

bool GameEngine::is_game_over() const
{
    return game_over_;
}

void GameEngine::update_check_boxes(std::vector<bool> check_boxes)
{
    players_.at(game_turn_).is_locked=check_boxes;
}

std::vector<int> GameEngine::get_latest_points()
{
    return players_.at(game_turn_).latest_point_values_;
}

std::vector<bool> GameEngine::get_locked_points()
{
    return players_.at(game_turn_).is_locked;
}

int GameEngine::get_players()
{
    return players_.size();
}

void GameEngine::report_player_status() const
{
    if ( players_.size() <= game_turn_ )
    {
        std::cout << "Internal error: report_player_status" << std::endl;
        return;
    }
    string textual_description = "";
    construe_result(players_.at(game_turn_).latest_point_values_,
                    textual_description);
    std::cout << textual_description << std::endl;
}

void GameEngine::update_points(const std::vector<int>& new_points)
{
    if ( players_.size() <= game_turn_ )
    {
        std::cout << "Internal error: update_points" << std::endl;
        return;
    }
    string dummy = "";
    int new_result = construe_result(new_points, dummy);
    int best_result_so_far
            = construe_result(players_.at(game_turn_).best_point_values_,
                              dummy);
    if ( new_result > best_result_so_far )
    {
        players_.at(game_turn_).best_point_values_ = new_points;
    }
    players_.at(game_turn_).latest_point_values_ = new_points;
}

bool GameEngine::all_turns_used() const
{
    for ( auto player : players_ )
    {
        if ( player.rolls_left_ > 0 )
        {
            return false;
        }
    }
    return true;
}
