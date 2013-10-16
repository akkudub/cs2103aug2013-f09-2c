#include "UI.h"


void UI::UI_interface(){
	HANDLE hConsole = GetStdHandle(STD_OUTPUT_HANDLE);
	/*displayWelcomeMessage(hConsole);*/
	/*char buf[] = "I am Blinking!!!!\n";
	setBlinkingText(0, 1, buf, 5, 1000);
	*/
	string command;
	bool statusFlag = false;
	while (!statusFlag) {
		displayCommandMessage(command, hConsole);
		displayProcessorMessage(command, hConsole, statusFlag);
	}
	displayExitMessage();
}


void UI::displayWelcomeMessage(HANDLE hConsole){
	cout << MESSAGE_WELCOME <<endl;
	bool alwaysFalse = false;
	displayProcessorMessage(COMMAND_DISPLAY_TASK_PENDING,hConsole,alwaysFalse);
}


void UI::displayCommandMessage(string& command, HANDLE hConsole){
	SetConsoleTextAttribute(hConsole, FOREGROUND_BLUE |FOREGROUND_GREEN | FOREGROUND_RED | FOREGROUND_INTENSITY);
	cout << MESSAGE_COMMAND;
	getline(cin, command);
}


void UI::displayProcessorMessage(string command, HANDLE hConsole, bool& statusFlag){
	string message;
	vector<string> feedback;
	int output = _processor.UImainProcessor(command, message, feedback);
	switch(output){
	case -1:
		SetConsoleTextAttribute(hConsole, FOREGROUND_RED | FOREGROUND_INTENSITY);
		break;
	case 0:
		SetConsoleTextAttribute(hConsole, FOREGROUND_GREEN | FOREGROUND_INTENSITY);
		break;
	case 1:
		SetConsoleTextAttribute(hConsole, FOREGROUND_GREEN |FOREGROUND_RED | FOREGROUND_INTENSITY);
		break;
	}
	cout << message << endl;

	SetConsoleTextAttribute(hConsole, FOREGROUND_BLUE |FOREGROUND_GREEN | FOREGROUND_RED | FOREGROUND_INTENSITY);
	for (string str:feedback){
		cout << str << endl;
	}
}


void UI::displayExitMessage(){
	cout << MESSAGE_GOODBYE <<endl;
}

void UI::setBlinkingText(int x, int y, char *buf, int timestoBlink, int delayMilliSecs){
	assert(buf != NULL);
	::system("cls");
	COORD ord;
	ord.X = x;
	ord.Y = y; 

	int len = strlen(buf);
	char *p = new char[len + 1];
	memset(p, 32, len);
	p[len] = '\0';
	for(int i = 0; i < timestoBlink; i++){

		SetConsoleCursorPosition(GetStdHandle(STD_OUTPUT_HANDLE), ord);
		std::cout << p;
		::Sleep(300);
		SetConsoleCursorPosition(GetStdHandle(STD_OUTPUT_HANDLE), ord);
		std::cout << buf;
		::Sleep(delayMilliSecs);
	}
}
