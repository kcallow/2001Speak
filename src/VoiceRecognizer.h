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

class WordProcessor {
public:
	void processWord(string &word) {
		if(isModifierKey(word))
			XteWrapper::keydown(word);
		else {
			XteWrapper::keypress(word);
			releaseModifierKeys();
		}
	}

private:
	vector<string> modifierKeys {"Control_L", "Alt_L", "Meta_L", "Shift_L", "Super_L"};

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
};

class LineProcessor {
private:
	int repetitions;
public:
	vector<string> words;
	WordProcessor wordProcessor;

	void processWords() {
		for(int i = 0; i < repetitions; i++)
			for(auto word : words)
				wordProcessor.processWord(word);
	}

	bool isRepeat() {
		return words[0].compare("REPEAT") == 0;
	}

	void clear() {
		words.clear();
	}

	void calculateRepetitions() {
		repetitions = 1;
		int extracted = sscanf(words[0].c_str(), "%d", &repetitions);
		if(extracted)
			words.erase(words.begin()); //Erase first element
	}
};

class CommandProcessor {
public: 
	CommandProcessor(VoiceRecognizer* parent) {
		this->parent = parent;
	}

	void processLine(string &line) {
		extractWords(line);
		if(lineProcessor.isRepeat()) {
			cout << "Repeating...\n";
			prevLineProcessor.processWords();
			lineProcessor = prevLineProcessor; //Preserve repeated command
		}
		else {
			lineProcessor.calculateRepetitions();
			lineProcessor.processWords();
		}
	}

private:
	VoiceRecognizer* parent;
	LineProcessor lineProcessor, prevLineProcessor;

	void extractWords (string &line) {
		prevLineProcessor = lineProcessor; //Backup previous line
		lineProcessor.clear();
		split(line,' ',lineProcessor.words);
	}
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

	void processVoiceInput() {
		redi::ipstream proc(command, redi::pstreams::pstderr);
		string line;
		while (getline(proc.out(), line)) 
			processLine(line);
	}

private:
	void processLine(string &line) {
		if (isLineMatching(line)) {
			line = extractCommand(line);
			processor->processLine(line);
			cout << "\n";
		}
	}

	bool isLineMatching(string &line) {
		return line.length() >= matchLen
			&& line.substr(0,matchLen).compare(match) == 0;
	}

	string extractCommand(string &line) {
		return line.substr(matchLen, line.find("(", matchLen) - matchLen); 
	}
};

