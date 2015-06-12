#include "WordProcessor.h"
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

class LineProcessor {
private:
	int repetitions;
	map<string, Mode> modeMap {{"INSERT", INSERT},
				   {"KEY", KEY},
				   {"MOUSE", MOUSE}};
public:
	vector<string> words;
	WordProcessor* wordProcessor;

	LineProcessor() {
		wordProcessor = new KeyModeProcessor();
	}

	~LineProcessor() {
		delete wordProcessor;
	}

	int getRepetitions() {
		return repetitions;
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

	void processWords() {
		if(isModeChange())
			changeMode();
		else for(int i = 0; i < repetitions; i++)
			for(auto word : words)
				wordProcessor->processWord(word);
	}

	bool isModeChange() {
		return words[0].compare("MODE") == 0;
	}

	void changeMode() {
		cout << "Switch to mode: " + words[1] + "...\n";
		Mode mode = modeMap[words[1]];
		wordProcessor = WordProcessorFactory::makeInstance(mode);
	}

	bool isRepeat() {
		return words[0].compare("REPEAT") == 0;
	}
};

class CommandProcessor {
public: 
	CommandProcessor(VoiceRecognizer* parent) {
		this->parent = parent;
	}

	void processLine(string &line) {
		extractWords(line);
		lineProcessor.calculateRepetitions();
		if(lineProcessor.isRepeat()) {
			repeatPreviousLine();
			lineProcessor = prevLineProcessor; //Preserve repeated command
		}
		else {
			lineProcessor.processWords();
		}
	}

private:
	VoiceRecognizer* parent;
	LineProcessor lineProcessor, prevLineProcessor;

	void extractWords (string &line) {
		prevLineProcessor = lineProcessor; //Backup current command
		lineProcessor.clear();
		split(line,' ',lineProcessor.words);
	}

	void repeatPreviousLine() {
		int repetitions = lineProcessor.getRepetitions();
		for(int i = 0; i < repetitions; i++)
			prevLineProcessor.processWords();
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

