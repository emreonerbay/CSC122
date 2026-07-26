#include <iostream>
#include <vector>
#include <string>
#include <cctype>
#include <fstream>
using namespace std;

class Guard {
public:
    int row;
    int col;
    char dir;
    bool patrols; // true = patrol guard that turns clockwise, false = normal guard that flips

    Guard(int r, int c, char d, bool p) {
        row = r;
        col = c;
        dir = d;
        patrols = p;
    }
};

class Door {
public:
    int row;
    int col;
    int group;

    Door(int r, int c, int g) {
        row = r;
        col = c;
        group = g;
    }
};

class Switch {
public:
    int row;
    int col;
    int group;

    Switch(int r, int c, int g) {
        row = r;
        col = c;
        group = g;
    }
};

class Game {
public:
    vector<string> grid;
    vector<Guard> guards;
    vector<Door> doors;
    vector<Switch> switches;
    vector<bool> groupOpen; // one entry per group, true means the doors in that group are open
    int playerRow;
    int playerCol;
    int goalRow;
    int goalCol;
    bool lost;
    bool won;

    void loadLevel(string name, vector<string> rawMap, vector<int> patrolGuards, vector<int> doorGroups, vector<int> switchGroups) {
        grid = rawMap;
        guards.clear();
        doors.clear();
        switches.clear();
        groupOpen.clear();
        lost = false;
        won = false;

        // the maps cant show which guard patrols or which group a door is in
        // so those come in as separate lists, matched up by the order they appear in the map
        int guardCount = 0;
        int doorCount = 0;
        int switchCount = 0;

        for (int r = 0; r < grid.size(); r++) {
            for (int c = 0; c < grid[r].size(); c++) {
                if (grid[r][c] == '@') {
                    playerRow = r;
                    playerCol = c;
                } else if (grid[r][c] == '$') {
                    goalRow = r;
                    goalCol = c;
                } else if (grid[r][c] == '>' || grid[r][c] == '<' || grid[r][c] == '^' || grid[r][c] == 'v') {
                    bool isPatrol = false;
                    for (int i = 0; i < patrolGuards.size(); i++) {
                        if (patrolGuards[i] == guardCount) {
                            isPatrol = true;
                        }
                    }
                    guards.push_back(Guard(r, c, grid[r][c], isPatrol));
                    guardCount++;
                } else if (grid[r][c] == '=') {
                    doors.push_back(Door(r, c, doorGroups[doorCount]));
                    doorCount++;
                } else if (grid[r][c] == '+') {
                    switches.push_back(Switch(r, c, switchGroups[switchCount]));
                    switchCount++;
                }
            }
        }

        // figure out how many groups this level has so groupOpen is the right size
        int maxGroup = -1;
        for (int i = 0; i < doors.size(); i++) {
            if (doors[i].group > maxGroup) maxGroup = doors[i].group;
        }
        for (int i = 0; i < switches.size(); i++) {
            if (switches[i].group > maxGroup) maxGroup = switches[i].group;
        }
        for (int i = 0; i <= maxGroup; i++) {
            groupOpen.push_back(false); // every door starts out closed
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

    // closed doors act like walls, open ones dont
    bool isClosedDoor(int r, int c) {
        for (int i = 0; i < doors.size(); i++) {
            if (doors[i].row == r && doors[i].col == c && groupOpen[doors[i].group] == false) {
                return true;
            }
        }
        return false;
    }

    // called after anyone (player or guard) moves, toggles doors if they stepped on a switch
    void checkSwitch(int r, int c) {
        for (int i = 0; i < switches.size(); i++) {
            if (switches[i].row == r && switches[i].col == c) {
                int g = switches[i].group;
                groupOpen[g] = !groupOpen[g];
                //cout << "toggled group " << g << endl;
                cout << "You hear a door move somewhere..." << endl;
            }
        }
    }

    void inspect(int r, int c) {
        if (r < 0 || c < 0 || r >= grid.size() || c >= grid[r].size()) {
            cout << "That tile is outside the map." << endl;
            return;
        }

        if (r == playerRow && c == playerCol) {
            cout << "That is you, the spy!" << endl;
            return;
        }

        for (int i = 0; i < guards.size(); i++) {
            if (guards[i].row == r && guards[i].col == c) {
                cout << "There is a guard here facing '" << guards[i].dir << "'." << endl;
                if (guards[i].patrols) {
                    cout << "This guard patrols an area, it turns clockwise when it hits something." << endl;
                } else {
                    cout << "This guard walks in a line, it flips around when it hits something." << endl;
                }
                return;
            }
        }

        for (int i = 0; i < doors.size(); i++) {
            if (doors[i].row == r && doors[i].col == c) {
                if (groupOpen[doors[i].group]) {
                    cout << "There is an open door here from group " << doors[i].group << "." << endl;
                } else {
                    cout << "There is a closed door here from group " << doors[i].group << "." << endl;
                }
                return;
            }
        }

        for (int i = 0; i < switches.size(); i++) {
            if (switches[i].row == r && switches[i].col == c) {
                cout << "There is a switch here that controls door group " << switches[i].group << "." << endl;
                return;
            }
        }

        if (r == goalRow && c == goalCol) {
            cout << "That is the goal! Get there to win." << endl;
            return;
        }

        if (grid[r][c] == '#') {
            cout << "That is just a wall." << endl;
            return;
        }

        cout << "Nothing interesting in that tile." << endl;
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

        if (isWall(newR, newC) || isClosedDoor(newR, newC)) {
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
        checkSwitch(playerRow, playerCol);

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

            bool blocked = isWall(nextR, nextC) || isClosedDoor(nextR, nextC);
            // prof feedback from portfolio 1: the goal blocks guard movement now
            if (nextR == goalRow && nextC == goalCol) blocked = true;
            for (int j = 0; j < guards.size(); j++) {
                if (j == i) continue;
                if (guards[j].row == nextR && guards[j].col == nextC) blocked = true;
            }

            if (blocked) {
                if (guards[i].patrols) {
                    // patrol guards make a clockwise turn instead of flipping
                    if (guards[i].dir == '^') guards[i].dir = '>';
                    else if (guards[i].dir == '>') guards[i].dir = 'v';
                    else if (guards[i].dir == 'v') guards[i].dir = '<';
                    else if (guards[i].dir == '<') guards[i].dir = '^';
                } else {
                    // normal guards flip and move the other way
                    if (guards[i].dir == '<') guards[i].dir = '>';
                    else if (guards[i].dir == '>') guards[i].dir = '<';
                    else if (guards[i].dir == '^') guards[i].dir = 'v';
                    else if (guards[i].dir == 'v') guards[i].dir = '^';
                }

                // recalculate next position after turning
                nextR = guards[i].row;
                nextC = guards[i].col;
                if (guards[i].dir == '<') nextC--;
                else if (guards[i].dir == '>') nextC++;
                else if (guards[i].dir == '^') nextR--;
                else if (guards[i].dir == 'v') nextR++;

                // have to redo the whole blocked check for the new direction too
                bool stillBlocked = isWall(nextR, nextC) || isClosedDoor(nextR, nextC);
                if (nextR == goalRow && nextC == goalCol) stillBlocked = true;
                for (int j = 0; j < guards.size(); j++) {
                    if (j == i) continue;
                    if (guards[j].row == nextR && guards[j].col == nextC) stillBlocked = true;
                }

                if (!stillBlocked) {
                    guards[i].row = nextR;
                    guards[i].col = nextC;

                    if (guards[i].row == playerRow && guards[i].col == playerCol) {
                        cout << "A guard walked into you! You lose." << endl;
                        lost = true;
                        return;
                    }
                    checkSwitch(guards[i].row, guards[i].col);
                }
            } else {
                guards[i].row = nextR;
                guards[i].col = nextC;

                if (guards[i].row == playerRow && guards[i].col == playerCol) {
                    cout << "A guard walked into you! You lose." << endl;
                    lost = true;
                    return;
                }
                checkSwitch(guards[i].row, guards[i].col);
            }
        }

        // redraw the grid before checking line of sight
        // so if the game ends here, the printed grid is up to date
        // (i used to erase the guards old spot from baseMap here but everything gets
        // redrawn from baseMap anyway, and it was deleting switches guards walked over)
        grid = baseMap;
        for (int i = 0; i < doors.size(); i++) {
            if (groupOpen[doors[i].group] == false) {
                grid[doors[i].row][doors[i].col] = '=';
            }
        }
        grid[goalRow][goalCol] = '$';
        for (int i = 0; i < guards.size(); i++) {
            grid[guards[i].row][guards[i].col] = guards[i].dir;
        }
        grid[playerRow][playerCol] = '@';

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
                // walls and closed doors block vision
                if (grid[r][c] == '#' || grid[r][c] == '=') break;
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

    }
};

string toLower(string s) {
    string result = "";
    for (int i = 0; i < s.size(); i++) {
        result += tolower(s[i]);
    }
    return result;
}

// turns a direction word from a level file (or user input) into the symbol the game uses
char dirWordToChar(string word) {
    string w = toLower(word);
    if (w == "up") return '^';
    if (w == "down") return 'v';
    if (w == "left") return '<';
    if (w == "right") return '>';
    return '?'; // invalid direction, caller should check for this
}

// the opposite, turns a direction symbol back into a word for saving to a file
string dirCharToWord(char d) {
    if (d == '^') return "Up";
    if (d == 'v') return "Down";
    if (d == '<') return "Left";
    if (d == '>') return "Right";
    return "Unknown";
}

// lets the user build a level from scratch, then saves it to a .lvl file
// and adds it to the level list so it can be played right away
void createLevel(vector<string> &levelNames, vector<vector<string>> &levelMaps, vector<vector<int>> &levelPatrols, vector<vector<int>> &levelDoorGroups, vector<vector<int>> &levelSwitchGroups) {
    int rows = -1;
    while (rows <= 0) {
        cout << "Number of rows: ";
        string rowStr;
        getline(cin, rowStr);
        try {
            rows = stoi(rowStr);
        } catch (...) {
            rows = -1;
        }
        if (rows <= 0) cout << "That's not a valid number of rows." << endl;
    }

    int cols = -1;
    while (cols <= 0) {
        cout << "Number of columns: ";
        string colStr;
        getline(cin, colStr);
        try {
            cols = stoi(colStr);
        } catch (...) {
            cols = -1;
        }
        if (cols <= 0) cout << "That's not a valid number of columns." << endl;
    }

    string levelName = "";
    while (levelName == "") {
        cout << "Name this level: ";
        getline(cin, levelName);
        if (levelName == "") cout << "Level needs a name." << endl;
    }

    int cellCount = rows * cols; // was gonna use this to loop but ended up doing r/c loops instead

    // grid of what the user has placed so far, starts out empty everywhere
    vector<vector<string>> objType(rows, vector<string>(cols, "Empty"));
    vector<vector<char>> objDir(rows, vector<char>(cols, ' '));
    vector<vector<bool>> objPatrol(rows, vector<bool>(cols, false));
    vector<vector<int>> objGroup(rows, vector<int>(cols, -1));

    bool editing = true;
    while (editing) {
        // draw the level so far, with row/col numbers along the edges
        cout << endl << "   ";
        for (int c = 0; c < cols; c++) cout << c << " ";
        cout << endl;
        for (int r = 0; r < rows; r++) {
            cout << r << ": ";
            for (int c = 0; c < objType[r].size(); c++) {
                char symbol = '.';
                if (objType[r][c] == "Wall") symbol = '#';
                else if (objType[r][c] == "Empty") symbol = '.';
                else if (objType[r][c] == "Player") symbol = '@';
                else if (objType[r][c] == "Goal") symbol = '$';
                else if (objType[r][c] == "Guard") symbol = objDir[r][c];
                else if (objType[r][c] == "Door") symbol = '=';
                else if (objType[r][c] == "Switch") symbol = '+';
                cout << symbol << " ";
            }
            cout << endl;
        }
        cout << endl;

        cout << "Objects:" << endl;
        cout << "1) Wall" << endl;
        cout << "2) Empty" << endl;
        cout << "3) Player" << endl;
        cout << "4) Goal" << endl;
        cout << "5) Guard" << endl;
        cout << "6) Door" << endl;
        cout << "7) Switch" << endl;
        cout << endl;
        cout << "Enter an object number to place it, 'inspect' to inspect a tile, or 'save' to save and quit: ";

        string choice;
        getline(cin, choice);
        string lowerChoice = toLower(choice);

        if (lowerChoice == "save") {
            // gotta check the level actually has a player and a goal or its not beatable
            int playerCount = 0;
            int goalCount = 0;
            for (int r = 0; r < rows; r++) {
                for (int c = 0; c < cols; c++) {
                    if (objType[r][c] == "Player") playerCount++;
                    if (objType[r][c] == "Goal") goalCount++;
                }
            }
            //cout << "players: " << playerCount << " goals: " << goalCount << endl;
            if (playerCount != 1) {
                cout << "You need exactly one player before you can save." << endl;
                continue;
            }
            if (goalCount < 1) {
                cout << "You need at least one goal before you can save." << endl;
                continue;
            }
            editing = false;
            continue;
        }

        if (lowerChoice == "inspect") {
            cout << "Row: ";
            string rowStr;
            getline(cin, rowStr);
            cout << "Col: ";
            string colStr;
            getline(cin, colStr);
            int r, c;
            try {
                r = stoi(rowStr);
                c = stoi(colStr);
            } catch (...) {
                cout << "That wasn't a number." << endl;
                continue;
            }
            if (r < 0 || r >= rows || c < 0 || c >= cols) {
                cout << "That tile is outside the level." << endl;
                continue;
            }
            if (objType[r][c] == "Guard") {
                cout << "There is a guard here, type: " << (objPatrol[r][c] ? "Patrolling" : "Normal") << ", facing " << dirCharToWord(objDir[r][c]) << "." << endl;
            } else if (objType[r][c] == "Door") {
                cout << "There is a door here, group " << objGroup[r][c] << "." << endl;
            } else if (objType[r][c] == "Switch") {
                cout << "There is a switch here, controls group " << objGroup[r][c] << "." << endl;
            } else {
                cout << "This tile is " << objType[r][c] << "." << endl;
            }
            continue;
        }

        int objNum;
        try {
            objNum = stoi(lowerChoice);
        } catch (...) {
            cout << "Invalid choice." << endl;
            continue;
        }
        if (objNum < 1 || objNum > 7) {
            cout << "Invalid choice." << endl;
            continue;
        }

        cout << "Row: ";
        string rowStr;
        getline(cin, rowStr);
        cout << "Col: ";
        string colStr;
        getline(cin, colStr);
        int r, c;
        try {
            r = stoi(rowStr);
            c = stoi(colStr);
        } catch (...) {
            cout << "That wasn't a number." << endl;
            continue;
        }
        if (r < 0 || r >= rows || c < 0 || c >= cols) {
            cout << "That tile is outside the level." << endl;
            continue;
        }

        if (objNum == 1) {
            objType[r][c] = "Wall";
        } else if (objNum == 2) {
            objType[r][c] = "Empty";
        } else if (objNum == 3) {
            objType[r][c] = "Player";
        } else if (objNum == 4) {
            objType[r][c] = "Goal";
        } else if (objNum == 5) {
            cout << "Movement type (Normal or Patrolling): ";
            string typeStr;
            getline(cin, typeStr);
            string lowerType = toLower(typeStr);
            bool patrol;
            if (lowerType == "normal") patrol = false;
            else if (lowerType == "patrolling") patrol = true;
            else {
                cout << "That's not a valid movement type." << endl;
                continue;
            }

            cout << "Direction (Up, Down, Left, Right): ";
            string dirStr;
            getline(cin, dirStr);
            char dirChar = dirWordToChar(dirStr);
            if (dirChar == '?') {
                cout << "That's not a valid direction." << endl;
                continue;
            }

            objType[r][c] = "Guard";
            objPatrol[r][c] = patrol;
            objDir[r][c] = dirChar;
        } else if (objNum == 6 || objNum == 7) {
            cout << "Group number: ";
            string groupStr;
            getline(cin, groupStr);
            int group;
            try {
                group = stoi(groupStr);
            } catch (...) {
                cout << "That wasn't a number." << endl;
                continue;
            }
            if (group < 0) {
                cout << "Group number can't be negative." << endl;
                continue;
            }
            objType[r][c] = (objNum == 6) ? "Door" : "Switch";
            objGroup[r][c] = group;
        }
        //cout << "placed obj " << objNum << " at " << r << "," << c << endl;
    }

    // turn the editor grid into the same rawMap/patrolGuards/doorGroups/switchGroups
    // format that loadLevel() in the Game class already expects
    vector<string> rawMap;
    vector<int> patrolGuards;
    vector<int> doorGroups;
    vector<int> switchGroups;
    int guardIndex = 0;
    for (int r = 0; r < rows; r++) {
        string rowStr = "";
        for (int c = 0; c < cols; c++) {
            if (objType[r][c] == "Wall") rowStr += '#';
            else if (objType[r][c] == "Empty") rowStr += '.';
            else if (objType[r][c] == "Player") rowStr += '@';
            else if (objType[r][c] == "Goal") rowStr += '$';
            else if (objType[r][c] == "Guard") {
                rowStr += objDir[r][c];
                if (objPatrol[r][c]) patrolGuards.push_back(guardIndex);
                guardIndex++;
            } else if (objType[r][c] == "Door") {
                rowStr += '=';
                doorGroups.push_back(objGroup[r][c]);
            } else if (objType[r][c] == "Switch") {
                rowStr += '+';
                switchGroups.push_back(objGroup[r][c]);
            }
        }
        rawMap.push_back(rowStr);
    }

    // write the level out to a .lvl file, one line per cell like the assignment example
    string fileName = levelName + ".lvl";
    ofstream outFile(fileName);
    outFile << rows << " " << cols << endl;
    for (int r = 0; r < rows; r++) {
        for (int c = 0; c < cols; c++) {
            if (objType[r][c] == "Guard") {
                outFile << "Guard " << (objPatrol[r][c] ? "Patrolling" : "Normal") << " " << dirCharToWord(objDir[r][c]) << endl;
            } else if (objType[r][c] == "Door") {
                outFile << "Door " << objGroup[r][c] << endl;
            } else if (objType[r][c] == "Switch") {
                outFile << "Switch " << objGroup[r][c] << endl;
            } else {
                outFile << objType[r][c] << endl;
            }
        }
    }
    outFile.close();
    cout << "Level saved as " << fileName << endl;

    // add it to the playable level list right away
    levelNames.push_back(levelName);
    levelMaps.push_back(rawMap);
    levelPatrols.push_back(patrolGuards);
    levelDoorGroups.push_back(doorGroups);
    levelSwitchGroups.push_back(switchGroups);
}

// loads a level from a .lvl file (name can be typed with or without the .lvl part)
// and adds it to the playable level list
bool loadLevelFromFile(string name, vector<string> &levelNames, vector<vector<string>> &levelMaps, vector<vector<int>> &levelPatrols, vector<vector<int>> &levelDoorGroups, vector<vector<int>> &levelSwitchGroups) {
    string fileName = name;
    if (fileName.size() < 4 || fileName.substr(fileName.size() - 4) != ".lvl") {
        fileName += ".lvl";
    }

    ifstream inFile(fileName);
    if (!inFile) {
        cout << "Could not find a level file named " << fileName << endl;
        return false;
    }

    int rows, cols;
    inFile >> rows >> cols;
    inFile.ignore(); // skip to the next line after the numbers

    vector<string> rawMap;
    vector<int> patrolGuards;
    vector<int> doorGroups;
    vector<int> switchGroups;
    int guardIndex = 0;

    for (int r = 0; r < rows; r++) {
        string rowStr = "";
        for (int c = 0; c < cols; c++) {
            string line;
            getline(inFile, line);
            //cout << "read line: " << line << endl;

            if (line == "Wall") {
                rowStr += '#';
            } else if (line == "Empty") {
                rowStr += '.';
            } else if (line == "Player") {
                rowStr += '@';
            } else if (line == "Goal") {
                rowStr += '$';
            } else if (line.substr(0, 5) == "Guard") {
                // line looks like: Guard Patrolling Down
                string rest = line.substr(6);
                int spacePos = rest.find(' ');
                string typeWord = rest.substr(0, spacePos);
                string dirWord = rest.substr(spacePos + 1);

                if (typeWord == "Patrolling") patrolGuards.push_back(guardIndex);
                guardIndex++;

                rowStr += dirWordToChar(dirWord);
            } else if (line.substr(0, 4) == "Door") {
                rowStr += '=';
                doorGroups.push_back(stoi(line.substr(5)));
            } else if (line.substr(0, 6) == "Switch") {
                rowStr += '+';
                switchGroups.push_back(stoi(line.substr(7)));
            }
        }
        rawMap.push_back(rowStr);
    }

    // use the file name (without .lvl) as the display name in the menu
    string displayName = fileName.substr(0, fileName.size() - 4);

    levelNames.push_back(displayName);
    levelMaps.push_back(rawMap);
    levelPatrols.push_back(patrolGuards);
    levelDoorGroups.push_back(doorGroups);
    levelSwitchGroups.push_back(switchGroups);

    cout << "Loaded " << displayName << "." << endl;
    return true;
}

int main() {
    vector<string> levelNames;
    vector<vector<string>> levelMaps;
    // these line up with the levels, since the map itself cant store this info
    vector<vector<int>> levelPatrols;      // which guards (by order in the map) are patrol guards
    vector<vector<int>> levelDoorGroups;   // group number for each door, in map order
    vector<vector<int>> levelSwitchGroups; // group number for each switch, in map order

    vector<string> map1;
    map1.push_back("#######");
    map1.push_back("#@....#");
    map1.push_back("#.###.#");
    map1.push_back("#.>...#");
    map1.push_back("#....$#");
    map1.push_back("#######");
    levelNames.push_back("A New Enemy");
    levelMaps.push_back(map1);
    levelPatrols.push_back({});
    levelDoorGroups.push_back({});
    levelSwitchGroups.push_back({});

    vector<string> map2;
    map2.push_back("########");
    map2.push_back("#@.....#");
    map2.push_back("#.####v#");
    map2.push_back("#......#");
    map2.push_back("#.##.#.#");
    map2.push_back("#<....$#");
    map2.push_back("########");
    levelNames.push_back("The Truth of the Weapon");
    levelMaps.push_back(map2);
    levelPatrols.push_back({});
    levelDoorGroups.push_back({});
    levelSwitchGroups.push_back({});

    // level 3 shows off multiple groups of doors/switches
    // group 0 has two doors and group 1 has one
    vector<string> map3;
    map3.push_back("##########");
    map3.push_back("#@.#..#.$#");
    map3.push_back("#..=..=..#");
    map3.push_back("#+.#.+#=.#");
    map3.push_back("##########");
    levelNames.push_back("Security Lockdown");
    levelMaps.push_back(map3);
    levelPatrols.push_back({});
    levelDoorGroups.push_back({0, 1, 0});
    levelSwitchGroups.push_back({0, 1});

    // level 4 has a patrol guard going around the inside room in a rectangle
    vector<string> map4;
    map4.push_back("#########");
    map4.push_back("#@......#");
    map4.push_back("#.#####.#");
    map4.push_back("#.#>..#.#");
    map4.push_back("#.#...#.#");
    map4.push_back("#.#...#.#");
    map4.push_back("#.#####.#");
    map4.push_back("#......$#");
    map4.push_back("#########");
    levelNames.push_back("The Patrol Room");
    levelMaps.push_back(map4);
    levelPatrols.push_back({0});
    levelDoorGroups.push_back({});
    levelSwitchGroups.push_back({});

    // level 5 has a patrol guard and a normal guard in the same room
    // they get in each others way so the patrol route ends up more complicated
    vector<string> map5;
    map5.push_back("##########");
    map5.push_back("#@.......#");
    map5.push_back("#.######.#");
    map5.push_back("#.#>...#.#");
    map5.push_back("#.#..<.#.#");
    map5.push_back("#.#....#.#");
    map5.push_back("#.######.#");
    map5.push_back("#.......$#");
    map5.push_back("##########");
    levelNames.push_back("Crossed Paths");
    levelMaps.push_back(map5);
    levelPatrols.push_back({0});
    levelDoorGroups.push_back({});
    levelSwitchGroups.push_back({});

    // level 6 has a guard that keeps walking over a switch
    // so the door keeps opening and closing and you have to time it
    vector<string> map6;
    map6.push_back("##########");
    map6.push_back("#@..=...$#");
    map6.push_back("##########");
    map6.push_back("#>...+...#");
    map6.push_back("##########");
    levelNames.push_back("Perfect Timing");
    levelMaps.push_back(map6);
    levelPatrols.push_back({});
    levelDoorGroups.push_back({0});
    levelSwitchGroups.push_back({0});

    Game game;

    while (true) {
        cout << "Welcome to Ultimate-Spy!" << endl;
        cout << "Select a level:" << endl;
        cout << endl;
        for (int i = 0; i < levelNames.size(); i++) {
            cout << (i + 1) << ") " << levelNames[i] << endl;
        }
        cout << endl;
        cout << "Enter a level number or name, 'new' to create a level, 'load' to load a level, or 'quit': ";

        string input;
        if (!getline(cin, input)) break;
        string lowerInput = toLower(input);

        if (lowerInput == "quit") break;

        if (lowerInput == "new") {
            createLevel(levelNames, levelMaps, levelPatrols, levelDoorGroups, levelSwitchGroups);
            continue;
        }

        if (lowerInput == "load") {
            cout << "Enter the file name to load: ";
            string fileInput;
            getline(cin, fileInput);
            loadLevelFromFile(fileInput, levelNames, levelMaps, levelPatrols, levelDoorGroups, levelSwitchGroups);
            continue;
        }

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

        game.loadLevel(levelNames[choice], levelMaps[choice], levelPatrols[choice], levelDoorGroups[choice], levelSwitchGroups[choice]);

        // make a copy of the map with just walls, floors and switches, no entities
        // i need this so i can redraw everything from scratch each turn
        // doors get taken out too since they get drawn from the door list (they can open)
        vector<string> baseMap = levelMaps[choice];
        for (int r = 0; r < baseMap.size(); r++) {
            for (int c = 0; c < baseMap[r].size(); c++) {
                char ch = baseMap[r][c];
                if (ch == '@' || ch == '>' || ch == '<' || ch == '^' || ch == 'v' || ch == '=') {
                    baseMap[r][c] = '.';
                }
            }
        }

        // game loop
        bool quit = false;
        while (!game.lost && !game.won) {
            game.printGrid();
            cout << "Move (W/A/S/D) or inspect: ";
            string moveInput;
            if (!getline(cin, moveInput)) {
                quit = true;
                break;
            }
            if (moveInput.size() == 0) continue;

            string lowerMove = toLower(moveInput);
            if (lowerMove == "inspect") {
                // inspecting doesnt cost your turn, guards dont move
                string rowStr, colStr;
                cout << "Row: ";
                getline(cin, rowStr);
                cout << "Col: ";
                getline(cin, colStr);

                int r, c;
                try {
                    r = stoi(rowStr);
                    c = stoi(colStr);
                } catch (...) {
                    cout << "That wasn't a number." << endl;
                    continue;
                }
                game.inspect(r, c);
                continue;
            }

            game.movePlayer(lowerMove[0], baseMap);
        }
        game.printGrid();
        if (quit) break;
    }

    cout << "Thanks for playing!" << endl;
    return 0;
}