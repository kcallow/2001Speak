#include "XteWrapper.h"
#include <pstreams/pstream.h>
#include <string>
#include <sstream>
#include <iostream>
using namespace std;

class VoiceRecognizer;
class CommandProcessor;

vector<string> split(string &s, char delim, vector<string> &dest) {
	stringstream ss(s);
	string item;
	while(getline(ss, item, delim))
		dest.push_back(item);
	return dest;
}

enum Mode {
	MACRO,
	WORD,
	KEY,
	MOUSE,
	MODES
};

class CommandProcessor {
public: 
	CommandProcessor(VoiceRecognizer* parent) {
		this->parent = parent;
	}

	void processLine(string line) {
		words.clear();
		split(line,' ',words);
		for(auto word : words) {
			if(isModifierKey(word))
				XteWrapper::keydown(word);
			else {
				XteWrapper::keypress(word);
				releaseModifierKeys();
			}
		}
	}

	bool isModifierKey(string &word) {
		for(auto key : modifierKeys)
			if(word.compare(key) == 0)
				return true;
		return false;
	}

	void releaseModifierKeys() {
		for(auto key : modifierKeys)
			XteWrapper::keyup(key);
	}
	
private:
	VoiceRecognizer* parent;
	vector<string> words;
	vector<string> modifierKeys {"Control_L", "Alt_L", "Meta_L", "Shift_L", "Super_L"};
};

class VoiceRecognizer {
private: 
	CommandProcessor* processor;
	string command = "pocketsphinx_continuous -inmic yes -backtrace yes -dict *.dic -lm *.lm";
	string match = "INFO: pocketsphinx.c(1133): ";
	int matchLen = match.length();
public:
	Mode mode = KEY;

	VoiceRecognizer() {
		processor = new CommandProcessor(this);
	}

	~VoiceRecognizer() {
		delete processor;
	}

	void handleVoiceInput() {
		redi::ipstream proc(command, redi::pstreams::pstderr);
		string line;
		while (getline(proc.out(), line)) 
			processLine(line);
	}

private:
	void processLine(string line) {
		if (isLineMatching(line)) {
			line = extractCommand(line);
			processor->processLine(line);
			cout << "\n";
		}
	}

	bool isLineMatching(string line) {
		return line.length() >= matchLen
			&& line.substr(0,matchLen).compare(match) == 0;
	}

	string extractCommand(string line) {
		return line.substr(matchLen, line.find("(", matchLen) - matchLen); 
	}
};

