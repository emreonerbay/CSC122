#include <iostream>
#include <string>
#include <vector>
using namespace std;

class Unicorn {
private:
    string name;
    static vector<string> taken_names;

public:
    Unicorn(string n) {
        for (int i = 0; i < taken_names.size(); i++) {
            if (taken_names[i] == n) {
                cout << "Error: " << n << " is already taken!" << endl;
                name = "";
                return;
            }
        }
        name = n;
        taken_names.push_back(n);
        cout << name << " created" << endl;
    }

    ~Unicorn() {
        if (name == "") return;
        for (int i = 0; i < taken_names.size(); i++) {
            if (taken_names[i] == name) {
                taken_names.erase(taken_names.begin() + i);
                break;
            }
        }
        cout << name << " destroyed" << endl;
    }

    string getName() { return name; }
};

vector<string> Unicorn::taken_names;

int main() {
    Unicorn u1("Sparkle");
    Unicorn u2("Tugba");
    Unicorn u3("Sparkle");  // already taken, should print error

    {
        Unicorn u4("Rainbow");
    }  // u4 destroyed here

    Unicorn u5("Rainbow");  // should work now

    return 0;
}