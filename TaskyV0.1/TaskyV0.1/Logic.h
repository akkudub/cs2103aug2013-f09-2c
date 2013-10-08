#ifndef _LOGIC_H_
#define _LOGIC_H_

/*
 *This class is mainly to store tasks and manipulate tasks according to processor's command.
 *
 *Main author: Kai Wen
*/

#include "Essential.h"
#include "Task.h"

class Logic{
private:
	vector<Task> _taskList;
	
public:
	Logic();
	///
	int add(Task, vector<Task>&);
	///
	int remove(Task);
	///clears temp, then pushes tasks that that has the same title as searchLine into temp.
	int search(string, vector<Task>&); 
	///
	int searchKeywords(vector<string>, vector<Task>&);
	///
	int searchKeywordsInRange(vector<string>, vector<Task>&, BasicDateTime start, BasicDateTime end);
	///
	int displayAll(vector<Task>&);
	///
	int displayStatus(bool, vector<Task>&); //done=true, pending=false. pushes tasks into referenced vector.
	///
	int displayInRange(BasicDateTime start, BasicDateTime end, vector<Task>&);
	///
	int update(Task, Task, vector<Task>&);
	/// old Task, new Task, Tasks that clash push into vector.
	int mark(bool, Task);//done=true, pending=false. 

};

#endif
