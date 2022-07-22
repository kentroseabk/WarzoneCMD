// WarzoneCMD.cpp : This file contains the 'main' function. Program execution begins and ends there.
//

#include <iostream>
#include <string>
#include <chrono>
#include <thread>

using namespace std;

constexpr unsigned short int msTimeToSleep = 1;
constexpr unsigned short int msToSleepFactor = 1;
constexpr unsigned short int countdownTimerStart = 5;

constexpr unsigned short int gasTimerStart = 3;
unsigned short int gasTimerCurrent = gasTimerStart;

unsigned short int playerMoves = 0;
constexpr unsigned short int playerMovesLootFactor = 2;

bool playerDead;

unsigned short int enemiesKilled;
constexpr unsigned short int enemyKillsToWin = 3;

float enemyHitChance = 0.25f;

struct Location
{
    string name;
    bool gassed;
};

Location airfield{ "Airfield", false };
Location resort{ "Resort", false };
Location capitol{ "Capitol", false };
Location powerPlant{ "Power Plant", false };
Location beachhead{ "Beachhead", false };

Location* currentLocation;

constexpr unsigned short int locationsSize = 5;
Location locations[locationsSize]{ airfield, resort, capitol, powerPlant, beachhead };

constexpr unsigned short int locationActionSize = 3;
string locationActions[locationActionSize]{ "Fight", "Loot", "Move" };

// location actions: loot, move, fight (if there's an enemy)
// weapons: pistol (default), smg, assualt rifle, each has higher chance of downing enemy
// chance for better loot goes up as player moves
// win cond? kill 5 players
// combat? random, but enemies in late game have higher chance of killing you
// gas moves into random location every x turns

struct Weapon
{
    string name;
    float hitChance;

    Weapon(string name, float hitChance) : name(name), hitChance(hitChance) {}
};

void DisplayIntro();
unsigned short int GetValidInput(unsigned short int max);
unsigned short int GetLocationSelection();
unsigned short int GetLocationAction();
Weapon SearchForLoot();
void SleepForSeconds(unsigned short int seconds);
void MainGameLoop();
unsigned short int GetIndexOfWeapon(string weaponName);
Location* FillRandomLocationWithGas();
void HandleInvalidInput();
void FillGas();

// starting weapon
Weapon pistol{ "Pistol", 0.1f };

// lootable
Weapon smg{ "SMG", 0.4f };
Weapon ar{ "AR", 0.7f };
Weapon sniper{ "Sniper Rifle", 0.9f };

constexpr unsigned short int weaponsSize = 3;

Weapon weapons[weaponsSize]{ smg, ar, sniper };

Weapon currentWeapon = pistol;

void HandleInvalidInput()
{
    cout << "Invalid input" << endl;
    cin.clear();
    cin.ignore(numeric_limits<streamsize>::max(), '\n');
}

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
            HandleInvalidInput();

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
        cout << (i + 1) << ". " << locations[i].name;
        
        if (locations[i].gassed)
        {
            cout << " (gassed)";
        }
        
        cout << endl;
    }

    cout << endl;

    bool retry;

    unsigned short int selection;

    do
    {
        retry = false;

        selection = GetValidInput(locationsSize);

        if (locations[selection - 1].gassed)
        {
            HandleInvalidInput();
            retry = true;
        }
    } while (retry);

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

unsigned short int GetUngassedLocationsCount()
{
    unsigned short int ungassedLocationsCount = 0;

    for (int i = 0; i < locationsSize; i++)
    {
        if (!locations[i].gassed)
        {
            ungassedLocationsCount++;
        }
    }

    return ungassedLocationsCount;
}

Location* FillRandomLocationWithGas()
{
    unsigned short int ungassedLocationsCount = GetUngassedLocationsCount();

    int randomNumber = (rand() % ungassedLocationsCount);

    unsigned short int ungassedLocationsCounter = 0;

    Location* locationToGas = nullptr;

    for (int i = 0; i < locationsSize; i++)
    {
        if (!locations[i].gassed)
        {
            if (randomNumber == ungassedLocationsCounter)
            {
                locationToGas = &locations[i];
                break;
            }

            ungassedLocationsCounter++;
        }
    }

    return locationToGas;
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

    cout << endl << "                           (command line edition)" << endl << endl;

    SleepForSeconds(msTimeToSleep);

    cout << endl << "Kill " << enemyKillsToWin << " enemies before the gas gets you." << endl;

    SleepForSeconds(msTimeToSleep);

    cout << endl << "Move to find better loot." << endl;

    SleepForSeconds(msTimeToSleep);

    cout << endl;

    cout << "Deployment begins in..." << endl << endl;

    SleepForSeconds(msTimeToSleep);

    for (int i = countdownTimerStart; i > 0; i--)
    {
        cout << i << endl;
        SleepForSeconds(msTimeToSleep);
    }

    cout << endl;
}

// based on how much the player has moved
Weapon SearchForLoot()
{
    // player moves, 0 to inf
    int randomNumber = (rand() % 101) + (playerMoves * playerMovesLootFactor); // randomNumber in the range 0 to 100, with bonus for moving

    // 0 - 30: nothing
    // 30 - 60: smg
    // 70 - 90: ar
    // 90 - 100: sniper

    if (randomNumber <= 30)
    {
        return currentWeapon;
    }
    else if (randomNumber <= 60)
    {
        return smg;
    }
    else if (randomNumber <= 90)
    {
        return ar;
    }
    else
    {
        return sniper;
    }

    return currentWeapon;
}

unsigned short int GetIndexOfWeapon(string weaponName)
{
    for (int i = 0; i < weaponsSize; i++)
    {
        if (weapons[i].name == weaponName)
        {
            return i;
        }
    }

    return 0;
}

void SleepForSeconds(unsigned short int seconds)
{
    int timeToSleepMS = seconds * 1000 * msToSleepFactor;
    this_thread::sleep_for(std::chrono::milliseconds(timeToSleepMS));
}

void MainGameLoop()
{
    bool gameOver = false;

    do 
    {
        cout << "Gas closing in... " << gasTimerCurrent << endl << endl;

        SleepForSeconds(msTimeToSleep);

        cout << "Enemies killed: " << enemiesKilled << "/" << enemyKillsToWin << endl << endl;

        SleepForSeconds(msTimeToSleep);

        unsigned short int locationActionSelection = GetLocationAction();

        if (locationActionSelection == 1)
        {
            // fight
            int randomNumber = (rand() % 101);

            if (randomNumber < (int)(currentWeapon.hitChance * 100))
            {
                cout << "You killed an enemy." << endl << endl;
                enemiesKilled++;

                if (enemiesKilled >= enemyKillsToWin)
                {
                    cout << "You won!" << endl << endl;
                    gameOver = true;
                }
            }
            else
            {
                randomNumber = (rand() % 101);

                if (randomNumber < (int)(enemyHitChance * 100))
                {
                    cout << "You were killed by an enemy." << endl << endl;
                    gameOver = true;
                }
                else
                {
                    cout << "You both missed." << endl << endl;
                }
            }

        }
        else if (locationActionSelection == 2)
        {
            // loot
            Weapon newWeapon = SearchForLoot();

            if (newWeapon.name != currentWeapon.name && GetIndexOfWeapon(newWeapon.name) > GetIndexOfWeapon(currentWeapon.name))
            {
                cout << "Found " << newWeapon.name << ". Dropping " << currentWeapon.name << "." << endl << endl;
                currentWeapon = newWeapon;
            }
            else
            {
                cout << "Nothing" << endl << endl;
            }
        }
        else if (locationActionSelection == 3)
        {
            // move
            unsigned short int locationSelection = GetLocationSelection();

            cout << "Moving to " << locations[locationSelection - 1].name << endl << endl;

            currentLocation = &locations[locationSelection - 1];

            playerMoves++;
        }

        if (enemiesKilled >= enemyKillsToWin || playerDead)
        {
            gameOver = true;
        }

        gasTimerCurrent--;

        if (gasTimerCurrent <= 0)
        {
            FillGas();

            if (GetUngassedLocationsCount() == 0)
            {
                cout << "You died to the gas." << endl << endl;
                gameOver = true;
            }
        }

        SleepForSeconds(msTimeToSleep);

    } while (!gameOver);
}

void FillGas()
{
    // fill random location w/ gas, if in that location, give selection to move
    Location* locationToGas = FillRandomLocationWithGas();

    locationToGas->gassed = true;

    if (GetUngassedLocationsCount() == 0)
    {
        return;
    }

    if (locationToGas->name == currentLocation->name)
    {
        cout << "Location has been gassed. Please move." << endl << endl;

        unsigned short int locationSelection = GetLocationSelection();

        cout << "Moving to " << locations[locationSelection - 1].name << endl << endl;

        currentLocation = &locations[locationSelection - 1];
    }

    gasTimerCurrent = gasTimerStart;
}

int main()
{
    DisplayIntro();

    unsigned short int locationSelection = GetLocationSelection();

    cout << "Moving to " << locations[locationSelection - 1].name << endl << endl;

    currentLocation = &locations[locationSelection - 1];

    SleepForSeconds(msTimeToSleep);

    MainGameLoop();

    return 0;
}
