// WarzoneCMD.cpp : This file contains the 'main' function. Program execution begins and ends there.
//

#include <iostream>
#include <string>
#include <chrono>
#include <thread>

using namespace std;

unsigned short int msTimeToSleep = 1;
unsigned short int msToSleepFactor = 1;
unsigned short int countdownTimer = 5;

unsigned short int enemiesKilled;
constexpr unsigned short int enemyKillsToWin = 5;

constexpr unsigned short int locationsSize = 5;
string locations[locationsSize]{ "Airfield", "Resort", "Capital", "Power Plant", "Beachhead" };

constexpr unsigned short int locationActionSize = 2;
string locationActions[locationActionSize]{ "Loot", "Move" };

// location actions: loot, move, fight (if there's an enemy)
// weapons: pistol (default), smg, assualt rifle, each has higher chance of downing enemy
// win cond? kill 5 players
// combat? random, but enemies in late game have higher chance of killing you

void DisplayIntro();
unsigned short int GetValidInput(unsigned short int max);
unsigned short int GetLocationSelection();
unsigned short int GetLocationAction();
void SleepForSeconds(unsigned short int seconds);

unsigned short int GetValidInput(unsigned short int max)
{
    int selection;

    bool retry;

    do
    {
        retry = false;

        cin >> selection;

        if (cin.fail() || selection < 1 || selection > max)
        {
            cout << "Invalid input" << endl;
            cin.clear();
            cin.ignore(numeric_limits<streamsize>::max(), '\n');

            retry = true;
        }
    } while (retry);

    cout << endl;

    return selection;
}

unsigned short int GetLocationSelection()
{
    cout << "Select a location..." << endl;

    for (int i = 0; i < locationsSize; i++)
    {
        cout << (i + 1) << ". " << locations[i] << endl;
    }

    cout << endl;

    unsigned short int selection = GetValidInput(locationsSize);

    return selection;
}

unsigned short int GetLocationAction()
{
    cout << "Select an action..." << endl;

    for (int i = 0; i < locationActionSize; i++)
    {
        cout << (i + 1) << ". " << locationActions[i] << endl;
    }

    cout << endl;

    unsigned short int selection = GetValidInput(locationActionSize);

    return selection;
}

void DisplayIntro()
{
    cout << "Welcome to..." << endl;

    SleepForSeconds(msTimeToSleep);

    cout << R"(
     __       __   ______   _______   ________   ______   __    __  ________ 
    |  \  _  |  \ /      \ |       \ |        \ /      \ |  \  |  \|        \
    | $$ / \ | $$|  $$$$$$\| $$$$$$$\ \$$$$$$$$|  $$$$$$\| $$\ | $$| $$$$$$$$
    | $$/  $\| $$| $$__| $$| $$__| $$    /  $$ | $$  | $$| $$$\| $$| $$__    
    | $$  $$$\ $$| $$    $$| $$    $$   /  $$  | $$  | $$| $$$$\ $$| $$  \   
    | $$ $$\$$\$$| $$$$$$$$| $$$$$$$\  /  $$   | $$  | $$| $$\$$ $$| $$$$$   
    | $$$$  \$$$$| $$  | $$| $$  | $$ /  $$___ | $$__/ $$| $$ \$$$$| $$_____ 
    | $$$    \$$$| $$  | $$| $$  | $$|  $$    \ \$$    $$| $$  \$$$| $$     \
     \$$      \$$ \$$   \$$ \$$   \$$ \$$$$$$$$  \$$$$$$  \$$   \$$ \$$$$$$$$        
)" << '\n';

    SleepForSeconds(msTimeToSleep);

    cout << endl << "                       (command line edition)" << endl << endl;

    SleepForSeconds(msTimeToSleep);

    cout << endl;

    cout << "Deployment begins in..." << endl << endl;

    SleepForSeconds(msTimeToSleep);

    for (int i = countdownTimer; i > 0; i--)
    {
        cout << i << endl;
        SleepForSeconds(msTimeToSleep);
    }

    cout << endl;
}

void SleepForSeconds(unsigned short int seconds)
{
    int timeToSleepMS = seconds * 1000 * msToSleepFactor;
    this_thread::sleep_for(std::chrono::milliseconds(timeToSleepMS));
}

int main()
{
    DisplayIntro();

    unsigned short int locationSelection = GetLocationSelection();

    cout << "Moving to " << locations[locationSelection - 1] << endl << endl;

    SleepForSeconds(msTimeToSleep);

    unsigned short int locationActionSelection = GetLocationAction();

    if (locationActionSelection == 1)
    {
        // loot
    }
    else if (locationActionSelection == 2)
    {
        // move
    }
    else if (locationActionSelection == 3)
    {
        // fight
    }

    return 0;
}
