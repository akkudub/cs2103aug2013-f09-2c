#include "UI.h"


void UI::UI_interface(){
	HANDLE hConsole = GetStdHandle(STD_OUTPUT_HANDLE);
	string command;
	bool statusFlag = false;
	displayWelcomeMessage();
	while (!statusFlag) {
		displayCommandMessage(command, hConsole);
		displayProcessorMessage(command, hConsole, statusFlag);
	}
	displayExitMessage();
}


void UI::displayWelcomeMessage(){
	cout << MESSAGE_WELCOME;
	string message;
	vector<string> feedback;
	int output = _processor.UImainProcessor(COMMAND_DISPLAY_TASK_TODAY, message, feedback);
	if (output == STATUS_CODE_SET_OVERALL::OVERALL_SUCCESS) {
		cout << MESSAGE_TASKS_TODAY << endl;
		for (string str:feedback){
			cout << str << endl;
			cout << endl;
		}
	}
	cout << endl;
}

void UI::displayExitMessage(){
	cout << "Exiting in 5" << endl;
	Sleep(1000);
	cout << "Exiting in 4" << endl;
	Sleep(1000);
	cout << "Exiting in 3" << endl;
	Sleep(1000);
	cout << "Exiting in 2" << endl;
	Sleep(1000);
	cout << "Exiting in 1" << endl;
	Sleep(1000);

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
	case STATUS_CODE_SET_OVERALL::OVERALL_ERROR:
		SetConsoleTextAttribute(hConsole, FOREGROUND_RED | FOREGROUND_INTENSITY);
		break;
	case STATUS_CODE_SET_OVERALL::OVERALL_SUCCESS:
		SetConsoleTextAttribute(hConsole, FOREGROUND_GREEN | FOREGROUND_INTENSITY);
		break;
	case STATUS_CODE_SET_OVERALL::OVERALL_WARNING:
	case STATUS_CODE_SET_OVERALL::OVERALL_PROMPT:
		SetConsoleTextAttribute(hConsole, FOREGROUND_GREEN |FOREGROUND_RED | FOREGROUND_INTENSITY);
		break;
	case STATUS_CODE_SET_OVERALL::OVERALL_EXIT:
		statusFlag = true;
		break;
	}

	assert(!message.empty());
	cout << endl;
	cout << message << endl;

	SetConsoleTextAttribute(hConsole, FOREGROUND_BLUE |FOREGROUND_GREEN | FOREGROUND_RED | FOREGROUND_INTENSITY);
	for (string str:feedback){
		cout << str << endl;
		cout << endl;
	}
	cout << endl;


}

