/* Yatzy Gui
 * ----------
 *Yatzy game has five dices that are thrown by the players in desired order.
 *At start, each player has three turns to roll up dices, but it is not mandatory
 *to use them all. Moreover, it is not required to use the turns alternately,
 *but a player can, for example, take all their turns one after the other
 *(without giving turn to other players between one’s rolls).
 *A player can lock part of the dices such that only unlocked dices will be rolled
 * in the next roll.
 *After all turns of all players have been used or after dropping out the game,
 *the program tells who was the winner or between which players the result is tie.
 *The series of five dices have the following rank order (from the best one to
 *the worst one):
 *
 * 1: yatzy, i.e. all five dices have the same face
 * 2: four of a kind, i.e. exactly four dices have the same face
 * 3: full house, i.e. exactly three dices have the same face and
 * also the rest two dices have the same face
 * 4: straigth, i.e. either the faces 1, 2, 3, 4, and 5, or the faces
 * 2, 3, 4, 5, and 6
 * 5: three of a kind, i.e. exactly three dices have the same face
 * 6: two pairs, i.e. exactly two dices have the same face and also
 * exactly two other dices have the same face
 * 7: pair, i.e. exactly two dices have the same face
 * 8: nothing of above.
 *
 * There is no rank order inside the above classes. For example, the pair of
 * 1’s is as good as the pair of 6’s.
 *
 *
 * ------------ Program author -----------
 *
 * Name: Muhammad Mursaleen
 * Student number: 152216548
 * UserID: pbmumu
 * E-Mail: muhammad.mursaleen@tuni.fi
 *
 * */


#ifndef MAINWINDOW_HH
#define MAINWINDOW_HH


#include "gameengine.hh"
#include "functions.hh"
#include <QTimer>
#include <QMainWindow>
#include <QLabel>

QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE




class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

    // Create a timer object for display clock
    QTimer* clockTimer;

    // create a timer object for animation clock
    QTimer* animationTimer;

    // Update the display clock
    void UpdateClock();

    // To pause and unpause the game, it will alter it's state from pause to
    // unpause and unpause to pause with same button
    void PauseGame();

    // With only one button game can be started for the first time and can be
    // reset after  one match has been compeletd and or during a match
    int StartResetGame();

    // When roll button is presed if player has turns then roll functionality
    // will be excuted otherwise will inform no turn has left
    void OnClickedRollButton();

    // After start and Reset new player will be added through it
    // Sets a desired number of players in the game engine given as a parameter.
    // ** Params
    // eng: game engine object has to be parse
    // numberOfPlayers: if runiing in cmd only then number of players has to be
    // parsed
    void SetPlayers(GameEngine& eng, string numberOfPlayers="");

    // Enables playing the game until all players have used all their turns,
    // or until the quit command is given.
    // Based on user input, makes the game engine given as a parameter to
    // roll dices, give turn to another player, or quit.
    // ** Params
    // eng: game engine object has to be parse
    // choice: if runiing in cmd only then choice="R/T/Q" has to be
    // parsed
    void PlayGame(GameEngine& eng, string choice="");

    // if only game logic has to be tested without gui this will set its setting
    void RunTestInTerminal();

    // When Next turn button is pressed turn will be shifted to next player
    void NextTurn();

    // To Close the Game
    void QuitGame();

    // To draw a image of dice to it's corresponding position
    // **Params
    // dicePosition: Position of dice from 1 to 5
    // imageName: imageName from  1 to 5 or empty
    void DrawImage(int dicePosition, string imageName="1" );

    // To draw a image of dice to it's corresponding position
    // **Params
    // dicePosition: Position of dice from 1 to 5
    // imageSize: if image's size has to be customise in animation
    // imageName: imageName from  1 to 5 or empty
    void DrawImage(int dicePosition, int imageSize , string imageName="1" );

    // It will do the animation of dice roll
    // ** Params
    // eng: game engine object has to be parse in order to draw last image
    // according to actual roll points
    void UpdateAnimation(GameEngine& eng);

    // to start animation
    void StartAnimation();

    // parse the state of check boxes for locking of current player
    // ** Params
    // eng: game engine object has to be parse to store state of locking
    void UpdateCheckBox(GameEngine& eng);

    // Uncheck all the check boxes
    void ResetCheckBox();

    //Disable all the check boxes fro the first roll of each player
    void DisableCheckBox();

    // enable the locking mechanisim
    void EnableCheckBox();

    // Update the gui state
    // ** params
    //  eng: game engine object has to be parse
    //  new_points: new_points from roll has to be parsed too after rolling
    //  updateGuide: a string that has info about current player and his turns
    void UpdateGui(GameEngine& eng, vector<int> new_points={},
                    std::string updateGuide ="");




private:
    Ui::MainWindow *ui;
    // Consts for all margins and fruits
    const int IMAGE_MARGIN = 20;
    const int IMAGE_SIZE = 50;

    // Dispaly clock seconds
    int clock_seconds_ =0;

    // is game paused or not
    bool is_pause_ = false;

    // animation steps
    int animation_step=0;
};
#endif // MAINWINDOW_HH
