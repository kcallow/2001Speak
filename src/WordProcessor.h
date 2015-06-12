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

class MacroManager : public WordProcessor{
private:
	map<string, function<void()>> actions;

public:
	MacroManager() {
		actions["BEGIN"] = []() {};
		actions["End"] = []() {};
		actions["LIST"] = []() {};
		actions["Delete"] = []() {};
		actions["PLAY"] = []() {};
		actions["REPLAY"] = []() {};
	}

	void processWord(string &word) {
		auto action = controller.actions[word]; 
		if(action) { //If action exists, do it
			action();
			cout << "Macro action: " + word + ".\n";
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
