#ifndef INTERPRETER_H
#define INTERPRETER_H

/**
 *this is to interpret a string from processor to the format that processor can understand and 
 *make use of. all the public APIs are set up to meet the requirement of processor.
 *(logic flow in this class will follow later)
 *
 *Author: Junchao
 */

#include <sstream>
#include <ctime>
#include "BasicDateTime.h"
#include "Essential.h"

class Interpreter {
private:
	string _title;
	int _type;
	BasicDateTime _start;
	BasicDateTime _end;


private:
	static const string EMPTY_STRING;
	static const string FROM_KEY_WORD;
	static const string TO_KEY_WORD;
	static const string BY_KEY_WORD;
	static const string DASH_M;
	static const string PENDING_KEY_WORD;
	static const string DONE_KEY_WORD;
	static const string SLOT_KEY_WORD;
	static const string RENAME_KEY_WORD;
	static const string ALL_KEY_WORD;
	static const string TODAY_KEY_WORD;
	static const string TODAY_KEY_WORD_SHORTCUT;
	static const string TOMORROW_KEY_WORD;
	static const string TOMORROW_KEY_WORD_SHORTCUT;
	static const string THIS_KEY_WORD;
	static const string NEXT_KEY_WORD;
	static const string MON_KEY_WORD;
	static const string MON_FULL_KEY_WORD;
	static const string TUE_KEY_WORD;
	static const string TUE_FULL_KEY_WORD;
	static const string WED_KEY_WORD;
	static const string WED_FULL_KEY_WORD;
	static const string THU_KEY_WORD;
	static const string THU_FULL_KEY_WORD;
	static const string FRI_KEY_WORD;
	static const string FRI_FULL_KEY_WORD;
	static const string SAT_KEY_WORD;
	static const string SAT_FULL_KEY_WORD;
	static const string SUN_KEY_WORD;
	static const string SUN_FULL_KEY_WORD;
	static const string AM_KEY_WORD;
	static const string PM_KEY_WORD;

	static const char SLASH;
	static const char DOT;
    static const char COMMA;
	static const char SINGLE_QUOTE;
	static const char SPACE;
	static const char DASH;
	static const char COLON;
	static const char ZERO;
	static const char NINE;

	static const int NO_DATETIME                          = 0;
	static const int ONE_DATETIME                         = 1;
	static const int TWO_DATETIME                         = 2;
	static const int TWO_DATETIME_WITH_STATUS_FOR_DISPLAY = 3;

	static const int INT_SUNDAY                           = 0;
	static const int INT_MONDAY                           = 1;
	static const int INT_TUESDAY                          = 2;
	static const int INT_WEDNESDAY                        = 3;
	static const int INT_THURSDAY                         = 4;
	static const int INT_FRIDAY                           = 5;
	static const int INT_SATURDAY                         = 6;
	static const int JANUARY                              = 1;
	static const int FEBRUARY                             = 2;
	static const int MARCH                                = 3;
	static const int APRIL                                = 4;
	static const int MAY                                  = 5;
	static const int JUNE                                 = 6;
	static const int JULY                                 = 7;
	static const int AUGUST                               = 8;
	static const int SEPTEMBER                            = 9;
	static const int OCTOBER                              = 10;
	static const int NOVEMBER                             = 11;
	static const int DECEMBER                             = 12;
	static const int YEAR_LOWER_BOUND                     = 1900;
	static const int YEAR_UPPER_BOUND                     = 2300;
	static const int DAY_LOWER_BOUND                      = 1;
	static const int DAY_UPPER_BOUND1                     = 31;
	static const int DAY_UPPER_BOUND2                     = 30;
	static const int DAY_UPPER_BOUND3                     = 28;
	static const int HOUR_LOWER_BOUND                     = 0;
	static const int HOUR_UPPER_BOUND                     = 23;
	static const int MINUTE_LOWER_BOUND                   = 0;
	static const int MINUTE_UPPER_BOUND                   = 59;
	static const int SECOND_LOWER_BOUND                   = 0;
	static const int SECOND_UPPER_BOUND                   = 59;

	static const int RADIX_TEN                            = 10;
	static const int RADIX_HUNDRED                        = 100;
	static const int STRING_TO_INT_ERROR                  = -1;
	static const int LEAP_YEAR_CONSTANT_100               = 100;
	static const int LEAP_YEAR_CONSTANT_400               = 400;
	static const int LEAP_YEAR_CONSTANT_4                 = 4;

	static const int DISPLAY_TYPE_ALL                     = 0;
	static const int DISPLAY_TYPE_PENDING                 = 1;
	static const int DISPLAY_TYPE_DONE                    = 2;
	static const int DISPLAY_TYPE_TODAY                   = 3;
	static const int POWER_SEARCH_PENDING_STATUS          = -1;
	static const int POWER_SEARCH_DONE_STATUS             = 1;
	static const int POWER_SEARCH_NO_STATUS               = 0;

	static const int EITHER_AS_START                      = 1;
	static const int EITHER_AS_END                        = 2;
	static const int INTERNAL_ERROR_CODE                  = -1;

public:
	Interpreter();
	int interpretAdd(string str, string& title, int& type, BasicDateTime& start, BasicDateTime& end, string& comment);
	int interpretPowerSearch(string str, bool& slotEnabled, vector<string>& keywords, int& searchStatus, int& type, BasicDateTime& start, BasicDateTime& end);
	int interpretDisplay(const string& str, int& displayType);
	int interpretRename(string str, string& oldTitle, string& newTitle, string& newComment);
	int interpretRenameAfterSearch(string str, int& num, string& newtitle, string& newComment);
	int interpretReschedule(string str, string& title, int& type, BasicDateTime& start, BasicDateTime& end);
	int interpretRescheduleAfterSearch(string str, int& num,int& type, BasicDateTime& start, BasicDateTime& end);
	int interpretMark(string str, string& title, bool& status);
	int interpretMarkAfterSearch(string str, vector<int>& vec, bool& status);
	int interpretRemove(string str, string& title);
	int stringToInt(string str);
	vector<int> stringToIntVec(string str);
	string toLowerCase(string input);
	~Interpreter();

private:
	bool extractTitle(const string& str, string& title, int& pos1, int& pos2);
	bool extractComment(const string& str, string& comment, int& pos);
	bool extractFirstWord(string str, string& firstWord);
	bool isEqualToKeyWordWithoutCase(string str, const string& keyword);
	bool containKeywordWithoutCase(string str, const string& keyword);
	bool findStartingPosOfKeywordWithoutCase(string str, const string& keyword, int& pos);
	vector<string> extractKeywords(const string& str);

	bool firstVerifyFromToOrBy(const string& str, bool& fromToFlag, bool& byFlag);
	bool secondVerifyFromToOrBy(bool fromToFlag, bool byFlag, int& type, BasicDateTime& start, BasicDateTime& end);
	bool fromToCheck(string str);
	bool byCheck(string str);
	bool checkSizeOfDateTimeStringVec(const vector<string>& vec);
	void generalTranslationOfDateTime(const vector<string>& vec, bool& fromFlag, int either);
	bool isStartEarlierThanEnd();
	void setStartToNow();
	bool translateDateTime(string str1, string str2, int either);
	bool translateNaturalDateTime(string str1, string str2, string str3, int either);  //will take 3 strings, deal with special cases
	bool translateOnlyDate(string str1, int either);  //will take in only one string, will deal with only today/number
	bool translateNaturalDate(string str1, string str2, int either);  //this is a special of the following one--without time
	bool interpretDate(string str1, int either);
	bool interpretNaturalDate(string str1, string str2, int either);
	bool interpretTime(string str1, int either);
	bool dateStandardInput(string str, int either);
	bool dateTodayOrTomorrow(string str, int either);
	bool dateThisOrNextDateFormat(int day, int week, int either);
	bool timeStandardInput(string str, char delim, int either);
	bool timeSpecialNumsOnly(string str, int either);
	bool timeDotAMOrPM(string str, int either);
	bool timeNumsAMOrPM(string str, int either);
	int mapTodayDayOfWeek();
	int mapDayOfWeekToInt(const string& str);
	void setDateParams(int yearValue, int monthValue, int dayValue, int either);
	void setTimeParams(int hourValue, int minuteValue, int secondValue, int either);
	bool validateDate(int year, int month, int day);
	bool validateTime(int hour, int month, int day);
	bool validateYear(int year);
	bool validateMonthDay(int month, int day, bool leap);

	vector<string> breakStringWithDelim(string str, char delim);
	string removeLeadingSpaces(string str);
	string removeTailSpaces(string str);
	string removeSpacesFromBothEnds(string str);
	int findFirstOfWord(const string& source, const string& word);
	int findLastOfWord(const string& source, const string& word);
	vector<int> pushNumsWithComma(const vector<string>& strVec);
	vector<int> pushNumsWithDash(const vector<string>& strVec);
	bool containChar(string input, char ch);
	bool isLeapYear(int year);
};

#endif
