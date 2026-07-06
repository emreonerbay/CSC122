#include <iostream>
#include <vector>
#include <string>

using namespace std;

// base class, just holds name + sound and a default play
class instrument {
protected:
	string name;
	string sound;

public:
	instrument(string n, string s) {
		name = n;
		sound = s;
	}

	virtual void play() {
		cout << name << ": " << sound << endl;
	}

};

class trumpet : public instrument {
public:
	trumpet() : instrument("Trumpet", "doot") {}

	void play() {
		cout << name << ": ";
		for (int i = 0; i < 3; i++) {
			cout << sound << " ";
		}
		cout << endl;
	}
};

class violin : public instrument {
public:
	violin() : instrument("Violin", "vwa") {}

	void play() {
		cout << name << ": ";
		for (int i = 0; i < 5; i++) {
			cout << sound << " ";
		}
		cout << endl;
	}
};

class piano : public instrument {
public:
	piano() : instrument("Piano", "dee") {}

	void play() {
		cout << name << ": ";
		for (int i = 0; i < 2; i++) {
			cout << sound << " ";
		}
		cout << endl;
	}
};

class trombone : public instrument {
public:
	trombone() : instrument("Trombone", "dwooooo~") {}

	// only plays once so didnt bother with a loop for this one
	void play() {
		cout << name << ": " << sound << endl;
	}
};

class singer : public instrument {
public:
	singer() : instrument("Singer", "blah") {}

	void play() {
		cout << name << ": ";
		for (int i = 0; i < 6; i++) {
			cout << sound << " ";
		}
		cout << endl;
	}
};

// holds all the instruments and plays them in order they were added
class orchestra {
public:
	vector<instrument*> instruments;

	void play() {
		for (instrument* i : instruments) {
			i->play();
		}
	}
};

int main() {

	instrument* i1 = new trumpet();
	instrument* i2 = new violin();
	instrument* i3 = new piano();
	instrument* i4 = new trombone();
	instrument* i5 = new trumpet(); // trumpet again just to make sure repeats work fine
	instrument* i6 = new singer();

	orchestra o;
	o.instruments.push_back(i1);
	o.instruments.push_back(i2);
	o.instruments.push_back(i3);
	o.instruments.push_back(i4);
	o.instruments.push_back(i5);
	o.instruments.push_back(i6);

	o.play();

}