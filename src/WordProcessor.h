#include "XteWrapper.h"
#include <string>
#include <functional>
#include <map>
#include <iostream>
using namespace std;

enum Mode {
	INSERT,
	KEY,
	MOUSE,
	MODES
};

class WordProcessor {
public:
	virtual void processWord(string &word) {};
};

class KeyModeProcessor : public WordProcessor{
public:
	void processWord(string &word) {
		if(isModifierKeyMode(word))
			XteWrapper::keydown(word);
		else {
			XteWrapper::keypress(word);
			releaseModifierKeyModes();
		}
	}

private:
	vector<string> modifierKeyModes {"Control_L", "Alt_L", "Meta_L", "Shift_L", "Super_L"};

	bool isModifierKeyMode(string &word) {
		for(auto key : modifierKeyModes)
			if(word.compare(key) == 0)
				return true;
		return false;
	}

	void releaseModifierKeyModes() {
		for(auto key : modifierKeyModes)
			XteWrapper::keyup(key);
	}
};

class MouseController {
private:
	string currentButton;
public:
	map<string, function<void()>> actions;

	MouseController() {
		actions["Up"] = []() { 
			XteWrapper::mousermove("0", "-40"); };
		actions["Down"] = []() { 
			XteWrapper::mousermove("0", "40"); };
		actions["Left"] = []() { 
			XteWrapper::mousermove("-40", "0"); };
		actions["Right"] = []() { 
			XteWrapper::mousermove("40", "0"); };
		actions["LEFT_BUTTON"] = [this]() { 
			currentButton = "1"; };
		actions["MIDDLE_BUTTON"] = [this]() { 
			currentButton = "2"; };
		actions["RIGHT_BUTTON"] = [this]() { 
			currentButton = "3"; };
		actions["CLICK"] = [this]() { 
			XteWrapper::mouseclick(currentButton); };
		actions["HOLD"] = [this]() { 
			XteWrapper::mousedown(currentButton); };
		actions["RELEASE"] = [this]() { 
			XteWrapper::mouseup(currentButton); };
	}
};

class MouseModeProcessor : public WordProcessor{
public:
	void processWord(string &word) {
		auto action = controller.actions[word]; 
		if(action) { //If action exists, do it
			action();
			cout << "Mouse action: " + word + ".\n";
		}
	}
private:
	MouseController controller;
};

class InsertModeProcessor : public WordProcessor{
public:
	void processWord(string &word) {}
};

class Macro : public vector<string> {
public:
	void run() {
	}
	void print() {
		cout << "Macro definition:\n";
		for(string line : *this) {
			cout << "\t" + line + "\n";
		}
	}
};

class MacroContainer : public vector<Macro> {
private:
	iterator current = begin();

public:
	Macro getFirst() {
		return *begin();
	}

	Macro getLast() {
		return *end();
	}

	Macro getCurrent() {
		return *current;
	}

	int getCurrentIndex() {
		return current - begin();
	}

	void runCurrent() {
		current->run();
	}

	void newMacro() {
		push_back(Macro());
	}

	void next() {
		if(current != end())
			current++;
	}

	void previous() {
		if(current != begin())
			current--;
	}

	void appendLineToLast(string &line) {
		begin()->push_back(line);
	}
};

class MacroManager : public WordProcessor{
private:
	void* parent;
	MacroContainer macros;
	map<string, function<void()>> actions;
	bool recording;

public:
	MacroManager() {
		actions["BEGIN"] = [this]() {
			cout << "Recording to macro #";
			cout << macros.size();
			cout << "...\n";
			macros.newMacro();
			recording = true;
		};
		actions["End"] = [this]() {
			cout << "Recorded to macro #";
			cout << macros.size() - 1;
			cout << ".\n";
			recording = false;
		};
		actions["LIST"] = [this]() {
			cout << "There are ";
			cout << macros.size();
			cout << " macros.\n";
			cout << "Current macro definition is:\n";
			macros.getCurrent().print();
		};
		actions["Delete"] = [this]() {};
		actions["PLAY"] = [this]() {
			cout << "Running macro ";
			cout << macros.getCurrentIndex();
			cout << ".\n";
			macros.runCurrent();
		};
		actions["NEXT"] = [this]() {
			macros.next();
			cout << "Current macro is: ";
			cout << macros.getCurrentIndex();
			cout << ".\n";
		};
		actions["PREVIOUS"] = [this]() {
			macros.previous();
			cout << "Current macro is: ";
			cout << macros.getCurrentIndex();
			cout << ".\n";
		};
	}

	void setParent(void* parent) {
		this->parent = parent;
	}

	void processWord(string &word) {
		auto action = actions[word]; 
		if(action) { //If action exists, do it
			action();
			cout << "Macro action: " + word + ".\n";
		}
	}

	void processLine(string &line) {
		if(recording) {
			macros.appendLineToLast(line);
		}
	}
};

class WordProcessorFactory {
public:
	static WordProcessor* makeInstance(Mode mode) {
		switch (mode) {
			case KEY:
				return new KeyModeProcessor();
			case MOUSE:
				return new MouseModeProcessor();
			case INSERT:
				return new InsertModeProcessor();
		}
	}
};
