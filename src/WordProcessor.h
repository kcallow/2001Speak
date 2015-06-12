#include "XteWrapper.h"
#include <string>
#include <functional>
#include <map>
#include <utility>
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
			XteWrapper::mousermove("0", "-10"); };
		actions["Down"] = []() { 
			XteWrapper::mousermove("0", "10"); };
		actions["Left"] = []() { 
			XteWrapper::mousermove("-10", "0"); };
		actions["Right"] = []() { 
			XteWrapper::mousermove("10", "0"); };
		actions["LEFT_BUTTON"] = [&]() { 
			XteWrapper::mouseclick(currentButton); };
		actions["MIDDLE_BUTTON"] = [&]() { 
			XteWrapper::mousedown(currentButton); };
		actions["RIGHT_BUTTON"] = [&]() { 
			XteWrapper::mouseup(currentButton); };
		actions["CLICK"] = [&]() { 
			currentButton = "1"; };
		actions["HOLD"] = [&]() { 
			currentButton = "2"; };
		actions["RELEASE"] = [&]() { 
			currentButton = "3"; };
	}
};

class MouseModeProcessor : public WordProcessor{
public:
	void processWord(string &word) {
		controller.actions[word];
	}
private:
	MouseController controller;
};

class InsertModeProcessor : public WordProcessor{
public:
	void processWord(string &word) {}
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
