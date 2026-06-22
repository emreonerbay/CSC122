#include <iostream>
#include <vector>
#include <string>
#include <cctype>
using namespace std;

class Guard {
public:
    int row;
    int col;
    char dir;

    Guard(int r, int c, char d) {
        row = r;
        col = c;
        dir = d;
    }
};

class Game {
public:
    vector<string> grid;
    vector<Guard> guards;
    int playerRow;
    int playerCol;
    int goalRow;
    int goalCol;
    bool lost;
    bool won;

    void loadLevel(string name, vector<string> rawMap) {
        grid = rawMap;
        guards.clear();
        lost = false;
        won = false;

        for (int r = 0; r < grid.size(); r++) {
            for (int c = 0; c < grid[r].size(); c++) {
                if (grid[r][c] == '@') {
                    playerRow = r;
                    playerCol = c;
                } else if (grid[r][c] == '$') {
                    goalRow = r;
                    goalCol = c;
                } else if (grid[r][c] == '>' || grid[r][c] == '<' || grid[r][c] == '^' || grid[r][c] == 'v') {
                    guards.push_back(Guard(r, c, grid[r][c]));
                }
            }
        }
    }

    void printGrid() {
        cout << endl;
        for (int r = 0; r < grid.size(); r++) {
            cout << grid[r] << endl;
        }
        cout << endl;
    }

    bool isWall(int r, int c) {
        if (r < 0 || c < 0) return true;
        if (r >= grid.size() || c >= grid[r].size()) return true;
        return grid[r][c] == '#';
    }

    void movePlayer(char move, vector<string> &baseMap) {
        int newR = playerRow;
        int newC = playerCol;

        if (move == 'w') newR--;
        else if (move == 's') newR++;
        else if (move == 'a') newC--;
        else if (move == 'd') newC++;
        else {
            cout << "Use W A S D to move." << endl;
            return;
        }

        if (isWall(newR, newC)) {
            cout << "Can't move there." << endl;
            return;
        }

        // check if a guard is standing there
        for (int i = 0; i < guards.size(); i++) {
            if (guards[i].row == newR && guards[i].col == newC) {
                cout << "You ran into a guard! You lose." << endl;
                lost = true;
                return;
            }
        }

        playerRow = newR;
        playerCol = newC;

        if (playerRow == goalRow && playerCol == goalCol) {
            cout << "You reached the goal! You win!" << endl;
            won = true;
            return;
        }

        // move each guard one step
        for (int i = 0; i < guards.size(); i++) {
            int nextR = guards[i].row;
            int nextC = guards[i].col;

            if (guards[i].dir == '<') nextC--;
            else if (guards[i].dir == '>') nextC++;
            else if (guards[i].dir == '^') nextR--;
            else if (guards[i].dir == 'v') nextR++;

            bool blocked = isWall(nextR, nextC);
            for (int j = 0; j < guards.size(); j++) {
                if (j == i) continue;
                if (guards[j].row == nextR && guards[j].col == nextC) blocked = true;
            }

            if (blocked) {
                // flip and move the other way
                if (guards[i].dir == '<') guards[i].dir = '>';
                else if (guards[i].dir == '>') guards[i].dir = '<';
                else if (guards[i].dir == '^') guards[i].dir = 'v';
                else if (guards[i].dir == 'v') guards[i].dir = '^';

                // recalculate next position after flipping
                nextR = guards[i].row;
                nextC = guards[i].col;
                if (guards[i].dir == '<') nextC--;
                else if (guards[i].dir == '>') nextC++;
                else if (guards[i].dir == '^') nextR--;
                else if (guards[i].dir == 'v') nextR++;

                if (!isWall(nextR, nextC)) {
                    baseMap[guards[i].row][guards[i].col] = '.';
                    guards[i].row = nextR;
                    guards[i].col = nextC;
                }
            } else {
                baseMap[guards[i].row][guards[i].col] = '.';
                guards[i].row = nextR;
                guards[i].col = nextC;

                if (guards[i].row == playerRow && guards[i].col == playerCol) {
                    cout << "A guard walked into you! You lose." << endl;
                    lost = true;
                    return;
                }
            }
        }

        // check line of sight for each guard
        for (int i = 0; i < guards.size(); i++) {
            int r = guards[i].row;
            int c = guards[i].col;
            while (true) {
                if (guards[i].dir == '<') c--;
                else if (guards[i].dir == '>') c++;
                else if (guards[i].dir == '^') r--;
                else if (guards[i].dir == 'v') r++;

                if (r < 0 || c < 0 || r >= grid.size() || c >= grid[r].size()) break;
                if (grid[r][c] == '#') break;
                // another guard also blocks vision
                bool otherGuard = false;
                for (int j = 0; j < guards.size(); j++) {
                    if (guards[j].row == r && guards[j].col == c) otherGuard = true;
                }
                if (otherGuard) break;

                if (r == playerRow && c == playerCol) {
                    cout << "A guard spotted you! You lose." << endl;
                    lost = true;
                    return;
                }
            }
        }

        // redraw the grid from the base map
        grid = baseMap;
        grid[goalRow][goalCol] = '$';
        for (int i = 0; i < guards.size(); i++) {
            grid[guards[i].row][guards[i].col] = guards[i].dir;
        }
        grid[playerRow][playerCol] = '@';
    }
};

string toLower(string s) {
    string result = "";
    for (int i = 0; i < s.size(); i++) {
        result += tolower(s[i]);
    }
    return result;
}

int main() {
    vector<string> levelNames;
    vector<vector<string>> levelMaps;

    vector<string> map1;
    map1.push_back("#######");
    map1.push_back("#@....#");
    map1.push_back("#.###.#");
    map1.push_back("#.>...#");
    map1.push_back("#....$#");
    map1.push_back("#######");
    levelNames.push_back("A New Enemy");
    levelMaps.push_back(map1);

    vector<string> map2;
    map2.push_back("########");
    map2.push_back("#@.....#");
    map2.push_back("#.####v#");
    map2.push_back("#......#");
    map2.push_back("#.####.#");
    map2.push_back("#<....$#");
    map2.push_back("########");
    levelNames.push_back("The Truth of the Weapon");
    levelMaps.push_back(map2);

    Game game;

    while (true) {
        cout << "Welcome to Ultimate-Spy!" << endl;
        cout << "Select a level:" << endl;
        cout << endl;
        for (int i = 0; i < levelNames.size(); i++) {
            cout << (i + 1) << ") " << levelNames[i] << endl;
        }
        cout << endl;
        cout << "Enter a level number or name (or quit): ";

        string input;
        if (!getline(cin, input)) break;
        string lowerInput = toLower(input);

        if (lowerInput == "quit") break;

        int choice = -1;
        for (int i = 0; i < levelNames.size(); i++) {
            string numStr = to_string(i + 1);
            if (lowerInput == numStr || lowerInput == toLower(levelNames[i])) {
                choice = i;
            }
        }

        if (choice == -1) {
            cout << "Invalid choice, try again." << endl;
            continue;
        }

        game.loadLevel(levelNames[choice], levelMaps[choice]);

        // make a copy of the map with just walls and floors, no entities
        // i need this so i can redraw everything from scratch each turn
        vector<string> baseMap = levelMaps[choice];
        for (int r = 0; r < baseMap.size(); r++) {
            for (int c = 0; c < baseMap[r].size(); c++) {
                char ch = baseMap[r][c];
                if (ch == '@' || ch == '>' || ch == '<' || ch == '^' || ch == 'v') {
                    baseMap[r][c] = '.';
                }
            }
        }

        // game loop
        bool quit = false;
        while (!game.lost && !game.won) {
            game.printGrid();
            cout << "Move (W/A/S/D): ";
            string moveInput;
            if (!getline(cin, moveInput)) {
                quit = true;
                break;
            }
            if (moveInput.size() == 0) continue;
            game.movePlayer(tolower(moveInput[0]), baseMap);
        }
        game.printGrid();
        if (quit) break;
    }

    cout << "Thanks for playing!" << endl;
    return 0;
}
