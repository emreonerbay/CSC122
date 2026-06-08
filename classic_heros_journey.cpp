#include <iostream>
#include <ctime>
#include <cstdlib>
using namespace std;

// Hero class representing a character with strength, courage, and a name
class Hero {
    private:
        int strength;
        int courage;
        string name; 

    public:
        // Constructor: sets name and assigns random strength and courage
        Hero (string heroName) 
        {
            name = heroName;
            strength = rand() % 100 + 1; 
            courage = rand() % 100 + 1;

        }

        // Getters
        string getName() 
        {
            return name;
        }

        int getStrength () 
        {
            return strength;
        }

        int getCourage()
        {
            return courage;
        }

        // Setter for courage
        void setCourage(int newCourage) 
        {
            courage = newCourage;
        }

        // Increase strength proportionally to training time
        void attendTraining(int time)
        {
            strength *= time; 
        }

        // Increases courage by a random amount
        void attendTherapy()
        {
            courage += rand() % 10 + 1;
        }

};

// External function: sends hero on a quest, outcome based on strength
void quest(Hero &hero)
{
    int roll = rand() % 100 + 1;  
    
    if (roll < hero.getStrength()) 
    {
        cout << hero.getName() << " Succeeded!" << endl;
        hero.setCourage(hero.getCourage() - 5);
    } 
    else if (roll == hero.getStrength()) 
    {
        cout << hero.getName() << " Neutral!" << endl;
        hero.setCourage(hero.getCourage() - 15);
    } 
    else 
    {
        cout << hero.getName() << " Failed!" << endl;
        hero.setCourage(hero.getCourage() - 25);
    } 
    
}



int main() {
    srand(time(0)); // Seed random number generator

    // Creating two heros with constructor
    Hero hero1("Atlas");
    Hero hero2("Titan");

    // Initial quests before training
    quest(hero1);
    quest(hero1);
    quest(hero2);

    // Heroes attend training and therapy
    hero1.attendTraining(5);
    hero1.attendTherapy();
    hero2.attendTraining(10);
    hero2.attendTherapy();

    // Quests after training
    quest(hero1);
    quest(hero2);

    return 0;
}