#include <pstreams/pstream.h>
#include <string>
#include <iostream>
using namespace std;

#ifndef VOICEINHANDLER_H
#define	VOICEINHANDLER_H

class VoiceInHandler {
public:
	void printLines() 
	{
		cout << "Lines until now:\n";
		for (auto &word : lines)
			cout  << "Line:  " + word + "\n";
	}

	void handleVoiceInput() {
		redi::ipstream proc(command, redi::pstreams::pstderr);
		string line;
		while (getline(proc.out(), line)) 
			processLine(line);
	}

	vector<string> getLines() const {
		return lines;
	}

	string lastLine() {
		return lines.back();
	}

	void runLine(string line) {
		cout << "\n";
		redi::ipstream proc("lxterm " + line, redi::pstreams::pstderr);
		string out;
		while (getline(proc.out(), out)) 
			cout << out + "\n";
	}

private:
	vector<string> lines;
	string match = "INFO: pocketsphinx.c(1133): ";
	int matchLen = match.length();
	string command = "pocketsphinx_continuous -inmic yes -backtrace yes -dict *.dic -lm *.lm";

	bool matchingLine(string line)
	{
		return line.length() >= matchLen
			&& line.substr(0,matchLen).compare(match) == 0;
	}

	string getWords(string line) {
		return line.substr(matchLen, line.find("(", matchLen) - matchLen); 
	}

	void processLine(string line) 
	{
		if (matchingLine(line)) {
			line = getWords(line);
			lines.push_back(line);
			runLine(line);
		}
	}
};

#endif	/* VOICEINHANDLER_H */

