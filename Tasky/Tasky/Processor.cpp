#include <assert.h>
#include "Processor.h"

const char Processor::SLASH = '/';
const char Processor::BACK_SLASH = '\\';
const char Processor::DOT = '.';
const char Processor::COLON = ':';
const char Processor::DASH = '-';
const char Processor::ZERO = '0';
const char Processor::NINE = '9';
const char Processor::SPACE = ' ';
const char Processor::NEW_LINE = '\n';
const string Processor::EMPTY_STRING = "";
const string Processor::NEW_LINE_STRING = "\n";
const string Processor::NONE = "None";

const string Processor::COMMAND_ADD = "add";
const string Processor::COMMAND_REMOVE = "remove";
const string Processor::COMMAND_DISPLAY = "display";
const string Processor::COMMAND_RENAME = "rename";
const string Processor::COMMAND_RESCHEDULE = "reschedule";
const string Processor::COMMAND_MARK = "mark";
const string Processor::COMMAND_SEARCH = "search";
const string Processor::COMMAND_UNDO = "undo";
const string Processor::COMMAND_REDO = "redo";
const string Processor::COMMAND_HELP = "help";
const string Processor::COMMAND_EXIT = "exit";

const string Processor::TASK_ADDED = "Task Added:";
const string Processor::TASK_ADD_ERROR = "Task not Added";
const string Processor::CLASHES = "Clashes:";
const string Processor::TASKS_REMOVED = "Tasks removed:";
const string Processor::TASKS_REMOVING_ERROR = "Tasks removing error:";
const string Processor::UPDATED_TO = "Updated to:";
const string Processor::TASK_RENAMED = "Task renamed:";
const string Processor::TASK_RENAME_ERROR = "Tasks renaming error:";
const string Processor::TASK_RESCHEDULED = "Task rescheduled:";
const string Processor::TASK_RESCHEDULED_ERROR = "Tasks rescheduling error:";
const string Processor::TASKS_MARKED = "Tasks marked:";
const string Processor::TASKS_MARKING_ERROR = "Tasks marking error:";
const string Processor::UNDO_TASK_ADDED = "Undo Tasks added:";
const string Processor::UNDO_TASK_ADDING_ERROR = "Undo tasks adding error:";
const string Processor::UNDO_TASK_REMOVED = "Undo Tasks removed:";
const string Processor::UNDO_TASK_REMOVING_ERROR = "Undo Tasks removing error";
const string Processor::UNDO_TASK_UPDATED = "Undo Tasks updated:";
const string Processor::UNDO_TASK_UPDATING_ERROR = "Undo Tasks updating error:";
const string Processor::REDO_TASK_ADDED = "Redo Tasks added:";
const string Processor::REDO_TASK_ADDING_ERROR = "Redo tasks adding error:";
const string Processor::REDO_TASK_REMOVED = "Redo Tasks removed:";
const string Processor::REDO_TASK_REMOVING_ERROR = "Redo Tasks removing error";
const string Processor::REDO_TASK_UPDATED = "Redo Tasks updated:";
const string Processor::REDO_TASK_UPDATING_ERROR = "Redo Tasks updating error:";
const string Processor::EMPTY_SLOTS = "Following empty slots found:";
const string Processor::NO_EMPTY_SLOTS = "No empty slots found!";
const string Processor::SLOT_FROM = "From: ";
const string Processor::SLOT_TO = "To: ";

Processor::Processor(){
	_statusFlag=0;
	loadFile();
}

Processor::~Processor(){
}

int Processor::UImainProcessor(string input, string& message, vector<string>& list){
	_tempStringList.clear();

	int returnCode = STATUS_CODE_SET_OVERALL::OVERALL_EXIT;
	string command = _interpreter.toLowerCase(getCommand(input));

	assert(_statusFlag >= 0 && _statusFlag < 5);
	if (_statusFlag != 0){
		if (commandIsNormal(command)){
			_statusFlag = 0;
			_tempTaskList.clear();
		}else{
			input = command+input;
		}
	}

	if (command != COMMAND_EXIT){
		switch (_statusFlag){
		case 0:
			if(command == COMMAND_ADD){
				returnCode = addCommandProcessor(input);
			}else if(command == COMMAND_REMOVE){
				returnCode = removeCommandProcessor(input);
			}else if(command == COMMAND_DISPLAY){
				returnCode = displayCommandProcessor(input);
			}else if(command == COMMAND_RENAME){
				returnCode = renameCommandProcessor(input);
			}else if(command == COMMAND_RESCHEDULE){
				returnCode = rescheduleCommandProcessor(input);
			}else if(command == COMMAND_MARK){
				returnCode = markCommandProcessor(input);
			}else if(command == COMMAND_SEARCH){
				returnCode = searchCommandProcessor(input);
			}else if(command == COMMAND_UNDO){
				returnCode = undoCommandProcessor(input);
			}else if(command == COMMAND_REDO){
				returnCode = redoCommandProcessor(input);
			}else if(command == COMMAND_HELP){
				returnCode = helpCommandProcessor(input);
			}else{
				returnCode = otherCommandProcessor();
			}
			break;

		case 1:
			returnCode = removeCommandProcessor(input);
			break;
		case 2:
			returnCode = renameCommandProcessor(input);
			break;
		case 3:
			returnCode = rescheduleCommandProcessor(input);
			break;
		case 4:
			returnCode = markCommandProcessor(input);
			break;
		default:
			break;
		}
	}

	return feedbackToUI(returnCode, message, list);
}


int Processor::addCommandProcessor(string input){
	int type;
	string title, comment;
	BasicDateTime startingDateTime;
	BasicDateTime endingDateTime;
	_tempTaskList.clear();

	int returnCode = _interpreter.interpretAdd(input, title, type, startingDateTime, endingDateTime, comment);
	if (returnCode != STATUS_CODE_SET_SUCCESS::SUCCESS_INTERPRET_ADD){
		return returnCode;
	}else{
		return addTask(title, type, startingDateTime, endingDateTime, comment);
	}
	return STATUS_CODE_SET_ERROR::ERROR_ADD;
}


int Processor::removeCommandProcessor(string input){
	int returnCode;
	Task oldTask, newTask;
	vector<Task> removedTasks, errorTasks;
	vector<int> choice;

	switch (_statusFlag){
	case 1:
		choice = _interpreter.stringToIntVec(input);
		if(choiceIsValidVec(choice)){
			for (unsigned int i = 0; i < choice.size(); i++){
				oldTask = _tempTaskList[choice[i]-1];
				removeTask(returnCode, removedTasks, errorTasks, oldTask, newTask);
			}
		}
		pushFeedackToStringVec(removedTasks, TASKS_REMOVED);
		pushFeedackToStringVec(errorTasks, TASKS_REMOVING_ERROR);

		_tempTaskList.clear();
		_statusFlag = 0;
		return returnCode;
		break;
	case 0:
		_tempTaskList.clear();

		returnCode = _interpreter.interpretRemove(input, _tempTitle);
		if (returnCode != STATUS_CODE_SET_SUCCESS::SUCCESS_INTERPRET_REMOVE){
			return returnCode;
		}else{
			_taskList.search(_tempTitle, _tempTaskList);
			if (_tempTaskList.size() == 1){
				oldTask = _tempTaskList[0];
				removeTask(returnCode, removedTasks, errorTasks, oldTask, newTask);
				pushFeedackToStringVec(removedTasks, TASKS_REMOVED);
				pushFeedackToStringVec(errorTasks, TASKS_REMOVING_ERROR);
				_tempTaskList.clear();
				return returnCode;
			}else if(!_tempTaskList.empty()){
				_statusFlag = 1;
				taskVecToStringVec(_tempTaskList, _tempStringList);
				return PROMPT_REMOVE_CHOOSE;
			}else if(_tempTaskList.empty()){
				return WARNING_SEARCH_NO_RESULT;
			}
		}
		break;
	default:
		return ERROR_REMOVE;
		break;
	}
	return ERROR_REMOVE;
}


int Processor::displayCommandProcessor(string input){
	bool done = true;
	bool pending = false;
	int type;
	int returnCode = _interpreter.interpretDisplay(input, type);

	_tempTaskList.clear();

	if (returnCode != STATUS_CODE_SET_SUCCESS::SUCCESS_INTERPRET_DISPLAY){
		return returnCode;
	}else{
		switch (type){
		case 0:
			returnCode = _taskList.displayAll(_tempTaskList);
			break;
		case 1:
			returnCode = _taskList.displayStatus(pending, _tempTaskList);
			break;
		case 2:
			returnCode = _taskList.displayStatus(done, _tempTaskList);
			break;
		case 3:
			returnCode = _taskList.displayToday(_tempTaskList);
			break;
		default:
			returnCode = STATUS_CODE_SET_ERROR::ERROR_DISPLAY;
			break;
		}
	}
	taskVecToStringVec(_tempTaskList, _tempStringList);
	_tempTaskList.clear();
	return returnCode;
}

int Processor::renameCommandProcessor(string input){
	int returnCode, choice;
	string oldTitle;
	Task newTask, oldTask;
	switch (_statusFlag){
	case 2:
		choice = _interpreter.stringToInt(input);
		if(choiceIsValid(choice)){
			newTask = _tempTaskList[choice-1];
			oldTask = newTask;

			newTask.setTitle(_tempTitle);
			returnCode = _taskList.update(oldTask, newTask, _tempTaskList);

			if (returnCode != STATUS_CODE_SET_ERROR::ERROR_UPDATE){
				recordCommand(COMMAND_TYPES::UPDATE, oldTask, newTask);
				_tempStringList.push_back(TASK_RENAMED);
			}else{
				_tempStringList.push_back(TASK_RENAME_ERROR);
			}
			_tempStringList.push_back(oldTask.toString());
			_tempStringList.push_back(UPDATED_TO);
			_tempStringList.push_back(newTask.toString());
			if (returnCode == STATUS_CODE_SET_WARNING::WARNING_UPDATE_CLASH){
				_tempStringList.push_back(CLASHES);
				taskVecToStringVec(_tempTaskList,  _tempStringList);
			}
		}
		_tempTaskList.clear();
		_statusFlag = 0;
		return returnCode;
		break;
	case 0:
		_tempTaskList.clear();

		returnCode = _interpreter.interpretRename(input, oldTitle, _tempTitle);
		if (returnCode != STATUS_CODE_SET_SUCCESS::SUCCESS_INTERPRET_RENAME){
			return returnCode;
		}else{
			_taskList.search(oldTitle, _tempTaskList);
			if (_tempTaskList.size() == 1){
				newTask = _tempTaskList[0];
				oldTask = newTask;
				newTask.setTitle(_tempTitle);
				returnCode = _taskList.update(oldTask, newTask, _tempTaskList);

				if (returnCode != STATUS_CODE_SET_ERROR::ERROR_UPDATE){
					recordCommand(COMMAND_TYPES::UPDATE, oldTask, newTask);
					_tempStringList.push_back(TASK_RENAMED);
				}else{
					_tempStringList.push_back(TASK_RENAME_ERROR);
				}
				_tempStringList.push_back(oldTask.toString());
				_tempStringList.push_back(UPDATED_TO);
				_tempStringList.push_back(newTask.toString());
				if (returnCode == STATUS_CODE_SET_WARNING::WARNING_UPDATE_CLASH){
					_tempStringList.push_back(CLASHES);
					taskVecToStringVec(_tempTaskList,  _tempStringList);
				}
				_tempTaskList.clear();
				return returnCode;

			}else if(!_tempTaskList.empty()){
				taskVecToStringVec(_tempTaskList, _tempStringList);
				_statusFlag = 2;
				return PROMPT_RENAME_CHOOSE;
			}else if(_tempTaskList.empty()){
				return WARNING_SEARCH_NO_RESULT;
			}
		}
		break;
	default:
		return ERROR_UPDATE;
		break;
	}
	return ERROR_UPDATE;
}

int Processor::rescheduleCommandProcessor(string input){
	int returnCode, choice;
	Task newTask, oldTask;
	switch (_statusFlag){
	case 3:
		choice = _interpreter.stringToInt(input);
		if(choiceIsValid(choice)){
			newTask = _tempTaskList[choice-1];
			oldTask = newTask;

			newTask.setStartDate(_tempStart);
			newTask.setEndDate(_tempEnd);
			newTask.setType(_tempType);
			returnCode = _taskList.update(oldTask, newTask, _tempTaskList);

			if (returnCode != STATUS_CODE_SET_ERROR::ERROR_UPDATE){
				recordCommand(COMMAND_TYPES::UPDATE, oldTask, newTask);
				_tempStringList.push_back(TASK_RESCHEDULED);
			}else{
				_tempStringList.push_back(TASK_RESCHEDULED_ERROR);
			}
			_tempStringList.push_back(oldTask.toString());
			_tempStringList.push_back(UPDATED_TO);
			_tempStringList.push_back(newTask.toString());
			if (returnCode == STATUS_CODE_SET_WARNING::WARNING_UPDATE_CLASH){
				_tempStringList.push_back(CLASHES);
				taskVecToStringVec(_tempTaskList,  _tempStringList);
			}
		}
		_tempTaskList.clear();
		_statusFlag = 0;
		return returnCode;
		break;
	case 0:
		_tempTaskList.clear();

		returnCode = _interpreter.interpretReschedule(input, _tempTitle, _tempType, _tempStart, _tempEnd);
		if (returnCode != STATUS_CODE_SET_SUCCESS::SUCCESS_INTERPRET_RESCHEDULE){
			return returnCode;
		}else{
			_taskList.search(_tempTitle, _tempTaskList);

			if (_tempTaskList.size() == 1){
				newTask = _tempTaskList[0];
				oldTask = newTask;

				newTask.setStartDate(_tempStart);
				newTask.setEndDate(_tempEnd);
				newTask.setType(_tempType);
				returnCode = _taskList.update(oldTask, newTask, _tempTaskList);
				if (returnCode != STATUS_CODE_SET_ERROR::ERROR_UPDATE){
					recordCommand(COMMAND_TYPES::UPDATE, oldTask, newTask);
					_tempStringList.push_back(TASK_RESCHEDULED);
				}else{
					_tempStringList.push_back(TASK_RESCHEDULED_ERROR);
				}
				_tempStringList.push_back(oldTask.toString());
				_tempStringList.push_back(UPDATED_TO);
				_tempStringList.push_back(newTask.toString());
				if (returnCode == STATUS_CODE_SET_WARNING::WARNING_UPDATE_CLASH){
					_tempStringList.push_back(CLASHES);
					taskVecToStringVec(_tempTaskList,  _tempStringList);
				}
				_tempTaskList.clear();
				return returnCode;

			}else if(!_tempTaskList.empty()){
				taskVecToStringVec(_tempTaskList, _tempStringList);
				_statusFlag = 3;
				return PROMPT_RESCHEDULE_CHOOSE;
			}else if(_tempTaskList.empty()){
				return WARNING_SEARCH_NO_RESULT;
			}
		}
		break;
	default:
		return ERROR_UPDATE;
		break;
	}
	return ERROR_UPDATE;
}


int Processor::markCommandProcessor(string input){
	int returnCode;
	Task newTask, oldTask;
	vector<Task> markedTasks, errorTasks;
	vector<int> choice;
	switch (_statusFlag){
	case 4:
		choice = _interpreter.stringToIntVec(input);

		if(choiceIsValidVec(choice)){
			for (unsigned int i = 0; i < choice.size(); i++){
				returnCode=_taskList.mark(_tempStatus, _tempTaskList[choice[i]-1]);
				oldTask = _tempTaskList[choice[i]-1];
				newTask = oldTask;
				if (returnCode == STATUS_CODE_SET_SUCCESS::SUCCESS_MARK){
					if(newTask.getDone() != _tempStatus){
						newTask.toggleDone();
						recordCommand(COMMAND_TYPES::UPDATE, oldTask, newTask);
						markedTasks.push_back(newTask);
					}
				}else{
					errorTasks.push_back(newTask);
				}
			}
		}
		pushFeedackToStringVec(markedTasks, TASKS_MARKED);
		if (!errorTasks.empty()){
			pushFeedackToStringVec(errorTasks, TASKS_MARKING_ERROR);
		}
		_tempTaskList.clear();
		_statusFlag = 0;
		return returnCode;
		break;
	case 0:
		_tempTaskList.clear();

		returnCode = _interpreter.interpretMark(input, _tempTitle, _tempStatus);
		if (returnCode != STATUS_CODE_SET_SUCCESS::SUCCESS_INTERPRET_MARK){
			return returnCode;
		}else{
			_taskList.search(_tempTitle, _tempTaskList);
			if (_tempTaskList.size() == 1){
				returnCode = _taskList.mark(_tempStatus, _tempTaskList[0]);
				oldTask = _tempTaskList[0];
				newTask = oldTask;
				if (returnCode == STATUS_CODE_SET_SUCCESS::SUCCESS_MARK){
					if(newTask.getDone() != _tempStatus){
						newTask.toggleDone();
						recordCommand(COMMAND_TYPES::UPDATE, oldTask, newTask);
						_tempStringList.push_back(TASKS_MARKED);
					}
				}else{
					_tempStringList.push_back(TASKS_MARKING_ERROR);
				}
				_tempStringList.push_back(newTask.toString());
				_tempTaskList.clear();
				return returnCode;
			}else if(!_tempTaskList.empty()){
				taskVecToStringVec(_tempTaskList, _tempStringList);
				_statusFlag = 4;
				return PROMPT_MARK_CHOOSE;
			}else if(_tempTaskList.empty()){
				return WARNING_SEARCH_NO_RESULT;
			}
		}
		break;
	default:
		return ERROR_MARK;
		break;
	}
	return ERROR_MARK;
}


int Processor::searchCommandProcessor(string input){
	vector<string> keywords;
	BasicDateTime start, end;
	vector<BasicDateTime> slots;
	int type, status;
	bool isPower;
	int returnCode = _interpreter.interpretPowerSearch(input, isPower, keywords, status, type, start, end);

	_tempTaskList.clear();
	if (returnCode != STATUS_CODE_SET_SUCCESS::SUCCESS_INTERPRET_SEARCH){
		return returnCode;
	}else{
		if (isPower){
			returnCode = _taskList.searchEmptySlots(start, end, slots);
			dateTimeVecToStringVec(slots, _tempStringList);
		}else{
			returnCode = _taskList.searchTasks(keywords, status, type, start, end, _tempTaskList);
			taskVecToStringVec(_tempTaskList, _tempStringList);
		}
		searched = true;
	}
	_tempTaskList.clear();
	return returnCode;
}

int Processor::searchActionProcessor(string command, string input){
	string title;
	Task oldTask, newTask;
	int returnCode;
	vector<Task> success, error;
	if (command == COMMAND_REMOVE){
		vector<int> choice = _interpreter.stringToIntVec(input);
		if (choiceIsValidVec(choice)){
			for (unsigned int i = 0; i<choice.size(); i++){
				newTask = _tempTaskList[i-1];
				removeTask(returnCode, success, error, newTask, oldTask);
			}
		}
		pushFeedackToStringVec(success, TASKS_REMOVED);
		pushFeedackToStringVec(error, TASKS_REMOVING_ERROR);

		_tempTaskList.clear();
		_statusFlag = 0;
		return returnCode;
	}
	return 0;
}

int Processor::undoCommandProcessor(string input){
	Task oldTask, newTask;
	COMMAND_TYPES type = COMMAND_TYPES::ADD;//Just for initialization
	int undoCount, returnCode;
	HistoryCommand command(type, oldTask, newTask);
	undoCount = _interpreter.stringToInt(input);
	_tempTaskList.clear();

	if (undoCount!=0){
		undoCount --;
	}
	for (int i = undoCount; i >=0; i--){
		returnCode = _history.undo(command);	
		if (returnCode == STATUS_CODE_SET_SUCCESS::SUCCESS_UNDO){
			switch (command.getCommandTypeUndo()){
			case COMMAND_TYPES::ADD:
				returnCode = _taskList.add(command.getOld(), _tempTaskList);
				if (returnCode != STATUS_CODE_SET_ERROR::ERROR_ADD){
					_tempStringList.push_back(UNDO_TASK_ADDED);
				}else{
					_tempStringList.push_back(UNDO_TASK_ADDING_ERROR);
				}
				_tempStringList.push_back(command.getOld().toString());
				if (returnCode == STATUS_CODE_SET_WARNING::WARNING_ADD_CLASH){
					_tempStringList.push_back(CLASHES);
					taskVecToStringVec(_tempTaskList, _tempStringList);
				}
				break;
			case COMMAND_TYPES::REMOVE:
				returnCode = _taskList.remove(command.getNew());
				if (returnCode != STATUS_CODE_SET_ERROR::ERROR_REMOVE){
					_tempStringList.push_back(UNDO_TASK_REMOVED);
				}else{
					_tempStringList.push_back(UNDO_TASK_REMOVING_ERROR);
				}
				_tempStringList.push_back(command.getNew().toString());
				break;
			case COMMAND_TYPES::UPDATE:
				returnCode = _taskList.update(command.getNew(), command.getOld(), _tempTaskList);
				if (returnCode != STATUS_CODE_SET_ERROR::ERROR_UPDATE){
					_tempStringList.push_back(UNDO_TASK_UPDATED);
				}else{
					_tempStringList.push_back(UNDO_TASK_UPDATING_ERROR);
				}
				_tempStringList.push_back(command.getNew().toString());
				_tempStringList.push_back(command.getOld().toString());
				if (returnCode == STATUS_CODE_SET_WARNING::WARNING_UPDATE_CLASH){
					_tempStringList.push_back(CLASHES);
					taskVecToStringVec(_tempTaskList, _tempStringList);
				}
				break;
			default:
				break;
			}
		}
	}
	_tempTaskList.clear();
	return returnCode;
}

int Processor::redoCommandProcessor(string input){
	Task oldTask, newTask;
	COMMAND_TYPES type = COMMAND_TYPES::ADD;//Just for initialization
	int redoCount, returnCode;
	HistoryCommand command(type, oldTask, newTask);
	redoCount = _interpreter.stringToInt(input);
	_tempTaskList.clear();

	if (redoCount!=0){
		redoCount --;
	}
	for (int i = redoCount; i >=0; i--){
		returnCode = _history.redo(command);
		if (returnCode == STATUS_CODE_SET_SUCCESS::SUCCESS_REDO){
			switch (command.getCommandTypeRedo()){
			case COMMAND_TYPES::ADD:
				returnCode = _taskList.add(command.getNew(), _tempTaskList);
				if (returnCode != STATUS_CODE_SET_ERROR::ERROR_ADD){
					_tempStringList.push_back(REDO_TASK_ADDED);
				}else{
					_tempStringList.push_back(REDO_TASK_ADDING_ERROR);
				}
				_tempStringList.push_back(command.getNew().toString());
				if (returnCode == STATUS_CODE_SET_WARNING::WARNING_ADD_CLASH){
					_tempStringList.push_back(CLASHES);
					taskVecToStringVec(_tempTaskList, _tempStringList);
				}
				break;
			case COMMAND_TYPES::REMOVE:
				returnCode = _taskList.remove(command.getOld());
				if (returnCode != STATUS_CODE_SET_ERROR::ERROR_REMOVE){
					_tempStringList.push_back(REDO_TASK_REMOVED);
				}else{
					_tempStringList.push_back(REDO_TASK_REMOVING_ERROR);
				}
				_tempStringList.push_back(command.getOld().toString());
				break;
			case COMMAND_TYPES::UPDATE:
				returnCode = _taskList.update(command.getOld(), command.getNew(), _tempTaskList);
				if (returnCode != STATUS_CODE_SET_ERROR::ERROR_UPDATE){
					_tempStringList.push_back(REDO_TASK_UPDATED);
				}else{
					_tempStringList.push_back(REDO_TASK_UPDATING_ERROR);
				}
				_tempStringList.push_back(command.getOld().toString());
				_tempStringList.push_back(command.getNew().toString());
				if (returnCode == STATUS_CODE_SET_WARNING::WARNING_UPDATE_CLASH){
					_tempStringList.push_back(CLASHES);
					taskVecToStringVec(_tempTaskList, _tempStringList);
				}
				break;
			default:
				break;
			}
		}
	}
	_tempTaskList.clear();
	return returnCode;
}

int Processor::helpCommandProcessor(string input){
	_tempStringList.push_back(_help.getHelpMsg(input));
	return SUCCESS_HELP;
}

int Processor::otherCommandProcessor(){
	_tempTaskList.clear();
	_tempStringList.clear();
	return WARNING_WRONG_INPUT;
}

int Processor::saveFile(){
	return _taskList.saveFile();
}

int Processor::loadFile(){
	return _taskList.loadFile();
}


int Processor::feedbackToUI(int returnCode, string& message, vector<string>& list){
	assert(returnCode>=STATUS_CODE_SET_OVERALL::OVERALL_SUCCESS &&
		returnCode<=STATUS_CODE_SET_PROPMT::PROMPT_MARK_CHOOSE);

	message = _messages.getMessage(returnCode);

	if (returnCode == STATUS_CODE_SET_OVERALL::OVERALL_EXIT){
		message += NEW_LINE_STRING + _messages.getMessage(saveFile());
		list.clear();
		saveFile();
		return returnCode;
	}else if(returnCode == STATUS_CODE_SET_WARNING::WARNING_WRONG_INPUT){
		return STATUS_CODE_SET_OVERALL::OVERALL_WARNING;
	}else{
		list = _tempStringList;

		if (returnCode >= SUCCESS_ADD && returnCode < WARNING_ADD_CLASH){
			return STATUS_CODE_SET_OVERALL::OVERALL_SUCCESS;
		}else if (returnCode >= WARNING_ADD_CLASH && returnCode < ERROR_ADD){
			return STATUS_CODE_SET_OVERALL::OVERALL_WARNING;
		}else if (returnCode >= ERROR_ADD && returnCode < PROMPT_REMOVE_CHOOSE){
			return STATUS_CODE_SET_OVERALL::OVERALL_ERROR;
		}else if (returnCode >= PROMPT_REMOVE_CHOOSE && returnCode <= PROMPT_MARK_CHOOSE){
			return STATUS_CODE_SET_OVERALL::OVERALL_PROMPT;
		}else{
			return STATUS_CODE_SET_OVERALL::OVERALL_ERROR;
		}
	}
}

bool Processor::choiceIsValidVec(vector<int> choice){
	if(choice.empty()){
		return false;
	}else{
		for (unsigned int i = 0; i < choice.size(); i++){
			unsigned int curr_choice = choice[i];
			if(!choiceIsValid(choice[i])){
				return false;
			}
		}
		return true;
	}
}


bool Processor::choiceIsValid(unsigned int choice){
	return ((choice <=_tempTaskList.size()) && choice > 0);
}

int Processor::addTask(string title, int type, BasicDateTime start, BasicDateTime end, string comment){
	Task newTask, oldTask;
	newTask = Task(title, start, end, type, false, comment);
	int statusCode = _taskList.add(newTask, _tempTaskList);
	if(statusCode == STATUS_CODE_SET_SUCCESS::SUCCESS_ADD){
		recordAndFeedback(oldTask, newTask);
	}
	if (statusCode == STATUS_CODE_SET_WARNING::WARNING_ADD_CLASH){
		recordCommand(COMMAND_TYPES::ADD, oldTask, newTask);
		recordAndFeedback(oldTask, newTask);
		_tempStringList.push_back(CLASHES);
		taskVecToStringVec(_tempTaskList, _tempStringList);
	}
	return statusCode;
}

void Processor::recordAndFeedback( Task oldTask, Task newTask )
{
	recordCommand(COMMAND_TYPES::ADD, oldTask, newTask);
	_tempStringList.push_back(TASK_ADDED);
	_tempStringList.push_back(newTask.toString());
}

string Processor::getCommand(string& input){
	stringstream ss(input);
	string tempStr, command;
	string tempOut = "";
	int count = 0;
	bool noSpace = true;
	while (std::getline(ss, tempStr, SPACE)){
		if (count == 0){
			command = tempStr;
		}else{
			tempOut += (SPACE + tempStr);
		}
		count ++;
		noSpace = false;
	}

	if (noSpace){
		return EMPTY_STRING;
	}else{
		input = tempOut;
		return command;
	}
}


bool Processor::isEscape(string command){
	if (_statusFlag != 0 && (commandIsNormal(command))){
		return true;
	}
	else{
		return false;
	}
}

int Processor::recordCommand(COMMAND_TYPES commandType, Task oldTask, Task newTask){
	HistoryCommand tempCommand(commandType, oldTask, newTask);
	return _history.record(tempCommand);
}

void Processor::taskVecToStringVec(vector<Task> taskList, vector<string>& stringList){
	if (!taskList.empty()){
		int size=taskList.size();
		string tempStr;
		for (int i=0;i<size;i++){
			tempStr = "Task no:    " + to_string(i+1) + NEW_LINE + taskList[i].toString();
			stringList.push_back(tempStr);	
		}
	}
}

void Processor::dateTimeVecToStringVec(vector<BasicDateTime> slots, vector<string>& stringList){
	if (!slots.empty()){
		string tempstr;
		_tempStringList.push_back(EMPTY_SLOTS);
		for (unsigned int i = 0; i < slots.size(); i=i+2){			
			tempstr = to_string((i+2)/2) + COLON;
			tempstr += SLOT_FROM + slots[i].getDateTimeString() + SPACE;
			tempstr += SLOT_TO + slots[i+1].getDateTimeString();
			stringList.push_back(tempstr);
		}
	}else{
		_tempStringList.push_back(NO_EMPTY_SLOTS);
	}
}

bool Processor::commandIsNormal(string command){
	if (command == "exit" || command == "add"
		||command == "remove" || command == "display"
		||command == "rename" ||command == "reschedule"
		||command == "mark" || command == "search"
		||command == "undo" || command == "redo")
	{
		return true;
	}else{
		return false;
	}
}

void Processor::pushFeedackToStringVec(vector<Task> taskVector, string message){
	if (!taskVector.empty()){
		_tempStringList.push_back(message);
		taskVecToStringVec(taskVector, _tempStringList);
	}
}

void Processor::removeTask( int& returnCode, vector<Task>& removed, vector<Task>& error, Task oldTask, Task newTask )
{
	returnCode = _taskList.remove(oldTask);
	if (returnCode != STATUS_CODE_SET_ERROR::ERROR_REMOVE)	{
		recordCommand(COMMAND_TYPES::REMOVE, oldTask, newTask);
		removed.push_back(oldTask);
	}else{
		error.push_back(oldTask);
	}
}