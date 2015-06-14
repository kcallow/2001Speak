#include <iostream>
#include <string>
#include <vector>
using namespace std;

class Macro {
public:
	void run() {
	}

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
		cout << "Appending line '" + line + "'...\n";
		cout << lines.size();
		cout << " lines appended.\n";
		lines.emplace_back();
		lines[lines.size() - 1] = line;
//		lines.push_back(newLine);
	}
private:
	vector<string> lines;
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

	int getCurrentIndex() {
		return current - begin();
	}

	void goToFirst() {
		current = begin();
	}

	void goToLast() {
		current = end() - 1;
	}

	void runCurrent() {
		cout << "Playing macro ";
		cout << getCurrentIndex();
		cout << ".\n";
		current->run();
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
		getLast().push_back(line);
		cout << "Appended '" + line + "' to macro #";
		cout << size() - 1;
		cout << ".\n";
	}
};

