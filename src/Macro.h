#include <iostream>
#include <string>
#include <vector>
using namespace std;

class Macro {
public:
	vector<string> lines;
	void print() {
		cout << "Macro contains ";
		cout << lines.size();
		cout << " lines.\n";
		cout << "Macro definition:\n";
		for(string line : lines) {
			cout << "\t" + line + "\n";
		}
	}

	void push_back(string line) {
		lines.push_back(line);
		cout << lines.size();
		cout << ": ";
	}
};

class MacroPlayer {
public:
	virtual void playMacro(Macro macro) {}
};

class MacroContainer : public vector<Macro> {
private:
	iterator current = begin();

public:
	Macro getFirst() {
		return *begin();
	}

	Macro getLast() {
		return *(end() - 1);
	}

	Macro getCurrent() {
		return *current;
	}

	void deleteCurrent() {
		current = erase(current);
	}

	int getCurrentIndex() {
		return current - begin();
	}

	void goToFirst() {
		current = begin();
	}

	void goToLast() {
		current = end() - 1;
	}

	void newMacro() {
		emplace_back();
	}

	void next() {
		if(current != end())
			current++;
	}

	void previous() {
		if(current != begin())
			current--;
	}

	void printCurrent() {
		cout << "Current macro is: ";
		cout << getCurrentIndex();
		cout << ".\n";
		if(current >= begin() && current < end())
			getCurrent().print();
	}

	void appendLineToLast(string line) {
		goToLast();
		current->push_back(line);
		cout << "Appended '" + line + "' to macro #";
		cout << getCurrentIndex();
		cout << ".\n";
	}
};

