#include "Macro.h"
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
		cout << "Key action: " + word + ".\n";
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
			currentButton = "1"; 
			cout << "Switched button to " + currentButton + ".\n";
		};
		actions["MIDDLE_BUTTON"] = [this]() { 
			currentButton = "2"; 
			cout << "Switched button to " + currentButton + ".\n";
		};
		actions["RIGHT_BUTTON"] = [this]() { 
			currentButton = "3"; 
			cout << "Switched button to " + currentButton + ".\n";
		};
		actions["CLICK"] = [this]() { 
			XteWrapper::mouseclick(currentButton); 
			cout << "Clicked button #" + currentButton + ".\n";
		};
		actions["HOLD"] = [this]() { 
			XteWrapper::mousedown(currentButton); 
			cout << "Holding button #" + currentButton + ".\n";
		};
		actions["RELEASE"] = [this]() { 
			XteWrapper::mouseup(currentButton); 
			cout << "Released button #" + currentButton + ".\n";
		};
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

class MacroManager : public WordProcessor{
private:
	MacroPlayer* parent;
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
			macros.goToLast();
		};
		actions["LIST"] = [this]() {
			cout << "There are ";
			cout << macros.size();
			cout << " macros.\n";
			macros.printCurrent();
		};
		actions["Delete"] = [this]() {
			macros.deleteCurrent();
		};
		actions["PLAY"] = [this]() {
			cout << "Playing macro ";
			cout << macros.getCurrentIndex();
			cout << ".\n";
			parent->playMacro(macros.getCurrent());
		};
		actions["NEXT"] = [this]() {
			macros.next();
			macros.printCurrent();
		};
		actions["PREVIOUS"] = [this]() {
			macros.previous();
			macros.printCurrent();
		};
	}

	void setParent(MacroPlayer* parent) {
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
