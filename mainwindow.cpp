#include "mainwindow.hh"
#include "ui_mainwindow.h"
#include "gameengine.hh"
#include "functions.hh"
#include "mainwindow.hh"
#include <iostream>
using namespace std;

// create a global object for game engine so that object is accessible when
// button is pressed
GameEngine engine;

// by setting "runTestInTerminalOnly = true " anyone can easily test the original
// logic of the code by default it is set to "false" to run gui smoothly
bool runTestInTerminalOnly = false;


MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    // create a new display clock timer object for a display clock
    clockTimer = new QTimer();

    // create a new  animation timer object
    animationTimer = new QTimer();

    // connect the clock timer and call UpdateClock for next time cycle
    connect(clockTimer, &QTimer::timeout,this, &MainWindow::UpdateClock );

    // connect the animation timer and call StartAnimation for next time cycle
    connect(animationTimer, &QTimer::timeout,this,
            &MainWindow::StartAnimation );

    // if only logic of game has to be tested setup the its setting
    if (runTestInTerminalOnly)
    {
        RunTestInTerminal();
    }

    // connect all buttons for gui game
    else
    {
        // connect the start/Reset button  and call StartReseGame
        connect(ui->startButton, &QPushButton::clicked,this,
                &MainWindow::StartResetGame);

        // connect the Roll button
        connect(ui->rollButton, &QPushButton::clicked,this,
                &MainWindow::OnClickedRollButton);

        // connect the next turn Bitton
        connect(ui->nextTurnButton, &QPushButton::clicked,this,
                &MainWindow::NextTurn);

        // connect the Close button
        connect(ui->closeButton, &QPushButton::clicked,this,
                &MainWindow::QuitGame);

        // connect the Pause button anmd call  PauseGame
        QObject::connect(ui->pushButtonPause, &QPushButton::clicked, this,
                         &MainWindow::PauseGame );
    }
    // Draw Images of the Dice for initial setup
    for (int i=1 ; i<=5; i++)
    {
        DrawImage(i);
    }

}



MainWindow::~MainWindow()
{
    delete ui;
}
void MainWindow::UpdateClock()
{
    // increase the seconds and convert the seconds into minutes and seconds
    clock_seconds_++;
    int min = clock_seconds_/60;
    int sec = clock_seconds_ %60;

    // Display minutes & seconds on Display Clock
    ui->minutesLcdNumber->display(min);
    ui->secondsLcdNumber->display(sec);

}
void MainWindow::PauseGame()
{
    // if current state is pause then alter it and Run the Game again
    if (is_pause_ )
    {
        ui->rollButton -> setEnabled(true);
        ui->nextTurnButton -> setEnabled(true);
        ui->labelTimerResults->setText("Game is on");

        // Start the clock timer again
        clockTimer->start(1000); //one second

        // alter it's state
        is_pause_ = !is_pause_;
    }

    // if game is on then pause it and stop the timer
    else
    {
        ui->rollButton -> setEnabled(false);
        ui->nextTurnButton -> setEnabled(false);
        ui->labelTimerResults->setText("Game is paused");
        if (clockTimer!=nullptr)
        {
            clockTimer->stop();

        }
        is_pause_ = !is_pause_;

    }
    return;
}

void MainWindow::DrawImage(int dicePosition, string imageName)
{
    // Defining where the images can be found and what kind of images they are
    const std::string PREFIX(":/");
    const std::string SUFFIX(".png");
    // Converting image (png) to a pixmap
    std::string filename = PREFIX + imageName + SUFFIX;
    QPixmap image(QString::fromStdString(filename));

    // Scaling the pixmap
    image = image.scaled(IMAGE_SIZE, IMAGE_SIZE);

    // Set the given image to one of 5 position according to info
    switch (dicePosition) {
            case 1:
                ui->dice1->setGeometry(IMAGE_MARGIN,IMAGE_MARGIN,
                                        IMAGE_SIZE, IMAGE_SIZE);
                ui->dice1->setPixmap(image);
                break;
            case 2:
                ui->dice2->setGeometry(IMAGE_MARGIN,IMAGE_MARGIN,
                                        IMAGE_SIZE, IMAGE_SIZE);
                ui->dice2->setPixmap(image);
                break;
            case 3:
                ui->dice3->setGeometry(IMAGE_MARGIN,IMAGE_MARGIN,
                                        IMAGE_SIZE, IMAGE_SIZE);
                ui->dice3->setPixmap(image);
                break;
            case 4:
                ui->dice4->setGeometry(IMAGE_MARGIN,IMAGE_MARGIN,
                                        IMAGE_SIZE, IMAGE_SIZE);
                ui->dice4->setPixmap(image);
                break;
            case 5:
                ui->dice5->setGeometry(IMAGE_MARGIN,IMAGE_MARGIN,
                                        IMAGE_SIZE, IMAGE_SIZE);
                ui->dice5->setPixmap(image);
                break;
            default:
                std::cout << "Invalid choice." << std::endl;
        }

}

void MainWindow::DrawImage(int dicePosition, int imageSize, string imageName)
{
    // Defining where the images can be found and what kind of images they are
    const std::string PREFIX(":/");
    const std::string SUFFIX(".png");
    // Converting image (png) to a pixmap
    std::string filename = PREFIX + imageName + SUFFIX;
    QPixmap image(QString::fromStdString(filename));

    // Scaling the pixmap
    image = image.scaled(imageSize, imageSize);

    // Set the given image to one of 5 position according to info with
    // custom image size
    switch (dicePosition) {
            case 1:
                ui->dice1->setGeometry(IMAGE_MARGIN,IMAGE_MARGIN,
                                        imageSize, imageSize);
                ui->dice1->setPixmap(image);
                break;
            case 2:
                ui->dice2->setGeometry(IMAGE_MARGIN,IMAGE_MARGIN,
                                        imageSize, imageSize);
                ui->dice2->setPixmap(image);
                break;
            case 3:
                ui->dice3->setGeometry(IMAGE_MARGIN,IMAGE_MARGIN,
                                        imageSize, imageSize);
                ui->dice3->setPixmap(image);
                break;
            case 4:
                ui->dice4->setGeometry(IMAGE_MARGIN,IMAGE_MARGIN,
                                        imageSize, imageSize);
                ui->dice4->setPixmap(image);
                break;
            case 5:
                ui->dice5->setGeometry(IMAGE_MARGIN,IMAGE_MARGIN,
                                        imageSize, imageSize);
                ui->dice5->setPixmap(image);
                break;
            default:
                std::cout << "Invalid choice." << std::endl;
        }
}

void MainWindow::UpdateAnimation(GameEngine& eng)
{
    // increase the animatin step and make a image size for animation
    animation_step++;
    int imageSize = animation_step*(IMAGE_SIZE/10);

    // get the latest points and locked position
    std::vector<int> latest_points = eng.get_latest_points();
    std::vector<bool> locked_points = eng.get_locked_points();

    // play the animation for all 5 dice except locked ones
    for (int i = 1; i<=5; i++ )
    {
        // if dice is not locked then play it's animation
        if (!locked_points[i-1])
        {
            int dicePosition = i;
            string imageName ;
            imageName = to_string(roll_dice());
            DrawImage(dicePosition, imageSize,  imageName);
        }

    }

    // if animation is compeleted then draw the actual points and stop
    // animation timer and reset it
    if(animation_step ==10)
    {
        for (int i=1 ; i<=5; i++)
        {
           DrawImage(i,to_string(latest_points[i-1]) );

        }
        animationTimer->stop();
        animation_step =0;
    }

}

void MainWindow::StartAnimation()
{
    UpdateAnimation(engine);
}

void MainWindow::UpdateCheckBox(GameEngine& eng)
{
    std::vector<bool> check_box_status = {ui->checkBox_1->isChecked(),
                                          ui->checkBox_2->isChecked(),
                                          ui->checkBox_3->isChecked(),
                                          ui->checkBox_4->isChecked(),
                                          ui->checkBox_5->isChecked()};
    eng.update_check_boxes(check_box_status);

}

void MainWindow::ResetCheckBox()
{

    ui->checkBox_1->setChecked(false);
    ui->checkBox_2->setChecked(false);
    ui->checkBox_3->setChecked(false);
    ui->checkBox_4->setChecked(false);
    ui->checkBox_5->setChecked(false);

}

void MainWindow::DisableCheckBox()
{
    ui->checkBox_1->setEnabled(false);
    ui->checkBox_2->setEnabled(false);
    ui->checkBox_3->setEnabled(false);
    ui->checkBox_4->setEnabled(false);
    ui->checkBox_5->setEnabled(false);
}

void MainWindow::EnableCheckBox()
{
    ui->checkBox_1->setEnabled(true);
    ui->checkBox_2->setEnabled(true);
    ui->checkBox_3->setEnabled(true);
    ui->checkBox_4->setEnabled(true);
    ui->checkBox_5->setEnabled(true);
}
void MainWindow::UpdateGui(GameEngine& eng, vector<int> new_points,
                            std::string update_guide)
{
    // if roll is presed only then points will be given and only then run the
    // animation
    if (new_points.size()==NUMBER_OF_DICES)
    {
        animationTimer->start(100);

    }
    // if game is over the update gui accordingly
    if (eng.is_game_over())
    {
        ui->rollButton -> setEnabled(false);
        ui->nextTurnButton -> setEnabled(false);
        if (clockTimer!=nullptr)
        {
            clockTimer->stop();

        }
        int min = clock_seconds_/60;
        int sec = clock_seconds_ %60;

        // Display the used match time
        ui->labelTimerResults->setText("Match time Used = "+
                                       QString::number(min)+" min "+
                                       QString::number(sec)+" sec ");
        ui->pushButtonPause -> setEnabled(false);

        // Display the wnner Info
        ui->resultLineEdit->setText(QString::fromStdString(
                                        eng.report_winner()));

        // Change background color on ending
        ui->centralwidget->setStyleSheet("background-color: rgb(236, 252, 152)");

    }
    else
    {

        if (update_guide.empty())
        {
            ui->resultLineEdit->setText(QString::fromStdString(
                                            eng.update_guide()));
        }
        else
        {
            ui->resultLineEdit->setText(QString::fromStdString(update_guide));
        }
    }
}

// Sets a desired number of players in the game engine given as a parameter.
void MainWindow::SetPlayers(GameEngine& eng, string numberOfPlayers)
{
    string player_amount_str = "";

    if (runTestInTerminalOnly)
    {
        cout << "Enter the number of players: ";

        getline(cin, player_amount_str);
    }
    else {
        player_amount_str = numberOfPlayers;

        // on starting / reseting run the clock timer
        clockTimer->start(1000); //one second
        ui->labelTimerResults->setText("Game is on");
    }

    unsigned int player_amount = 0;
    if(player_amount_str.size() == 1 and isdigit(player_amount_str.at(0)))
    {
        player_amount = stoi(player_amount_str);
    }
    else
    {
        SetPlayers(eng); // recursive call
    }
    for(unsigned int i = 0; i < player_amount; ++i)
    {
        Player player = {i + 1, INITIAL_NUMBER_OF_ROLLS, {}, {},std::vector < bool > (NUMBER_OF_DICES, false)};
        eng.add_player(player);
    }

    if (! runTestInTerminalOnly)
    {
        UpdateGui( eng);
    }

}
// Enables playing the game until all players have used all their turns,
// or until the quit command is given.
// Based on user input, makes the game engine given as a parameter to
// roll dices, give turn to another player, or quit.


void MainWindow::PlayGame(GameEngine& eng, string choice)
{

    if(eng.is_game_over())
    {

        UpdateGui(eng );
        return;
    }

    string selection = "";
    vector<int> new_points;
    std::string updateGuide ="";

    // if cmd mode in running the get input from the user
    if (runTestInTerminalOnly)
    {
        updateGuide =eng.update_guide();
        cout << "Enter selection (R = roll, T = give turn, Q = quit): ";
        getline(cin, selection);
    }
    else {
        selection =choice;
        UpdateCheckBox(eng);
    }


    if(selection == "R" or selection == "r")
    {
        // if new roll is too quick and previous animation is running then
        // quite previous animation and display the results and do new roll
        if (animationTimer->isActive())
        {
            animationTimer->stop();
            animation_step =0;
            std::vector<int> latest_points = eng.get_latest_points();
            for (int i=1 ; i<=5; i++)
            {
               DrawImage(i,to_string(latest_points[i-1]) );

            }
        }

        EnableCheckBox();
        new_points = eng.roll();
    }
    else if(selection == "T" or selection == "t")
    {
        // uncheck all check boxes for new turn and disable them for the
        // first roll
        ResetCheckBox();
        DisableCheckBox();
        eng.give_turn();
    }
    else if(selection == "Q" or selection == "q")
    {
        eng.report_winner();
        return;
    }
    else
    {
        cout << "wrong selection" << endl;
    }
    if (runTestInTerminalOnly)
    {
        PlayGame(eng); // recursive call
    }


        UpdateGui(eng, new_points);
}

void MainWindow::RunTestInTerminal()
{
    SetPlayers(engine);
    PlayGame(engine);
}
void MainWindow::OnClickedRollButton()
{
    string Choice = "R";
    PlayGame(engine,Choice);

}
void MainWindow::NextTurn()
{
    string Choice = "T";
    PlayGame(engine,Choice);
}

void MainWindow::QuitGame()
{
    string Choice = "Q";
    PlayGame(engine,Choice);
    this->close();

}

int MainWindow::StartResetGame()
{
    // create a new object for game
    GameEngine newEng;

    // reset the game object
    engine = newEng;

    //enable the buttons and clear the previous results
    ui->rollButton -> setEnabled(true);
    ui->nextTurnButton -> setEnabled(true);
    ui->pushButtonPause -> setEnabled(true);
    ui->labelTimerResults->clear();

    // Set original Backgroung color
    ui->centralwidget->setStyleSheet("background-color: rgb(233, 185, 110)");
    ResetCheckBox();
    DisableCheckBox();

    // Set nUmber of pLayers and Dispaly
    cout<<"number Of Players = ";
    QString numberOfPlayersStr = ui->numberOfPlayersBox->text();
    string numberOfPlayers = numberOfPlayersStr.toStdString();
    cout<<numberOfPlayers<<endl;
    SetPlayers(engine,numberOfPlayers);
    ui->labelTotalPlayers->setText("Total Players = "+QString::number(
                                       engine.get_players()));

    // reset the display clock & animation
    clock_seconds_ =-1;
    UpdateClock();
    if (animationTimer->isActive())
    {
        animationTimer->stop();
        animation_step =0;
    }
    return 0;
}


