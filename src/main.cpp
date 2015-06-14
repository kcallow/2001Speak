#include "VoiceRecognizer.h"

int main(int argc, char** argv)
{
    VoiceRecognizer voiceIn;
    if(argc > 1) //If any argument, read text from standard input
	    voiceIn.processTextInput();
    else
	    voiceIn.processVoiceInput();
}
