#ifndef TASKYDESIGN1_H
#define TASKYDESIGN1_H
//////////////////////////////////////////////////////////////////////////
//all the Qt libraries                                                  //
//////////////////////////////////////////////////////////////////////////
#include <QtWidgets/QMainWindow>
#include <QSystemTrayIcon>
#include <QTimer>
#include <QGraphicsOpacityEffect>
#include <QMouseEvent>
#include <QShortcut>
#include <QCompleter>
#include <QPalette>
//////////////////////////////////////////////////////////////////////////
//for global short cut key                                              //
//////////////////////////////////////////////////////////////////////////
#include "libqxt/qxtglobalshortcut.h"
//////////////////////////////////////////////////////////////////////////
//other header files to link to logic                                   //
//////////////////////////////////////////////////////////////////////////
#include <vector>
#include "ui_taskydesign.h"
#include "Processor.h"

class TaskyDesign : public QMainWindow
{
	Q_OBJECT

public:
	TaskyDesign(QWidget *parent = 0);
	~TaskyDesign();

private:
	//////////////////////////////////////////////////////////////////////////
	//private attributes to communicate with logic back end                 //
	//////////////////////////////////////////////////////////////////////////
	Processor* _logic;
	std::string _msg;
	std::vector<std::string> _vec;
	int _msgType;

private:
	//////////////////////////////////////////////////////////////////////////
	//const string definitions                                              //
	//////////////////////////////////////////////////////////////////////////
	static const QString ADD_COMPLETE_WORD;
	static const QString DISPLAY_COMPLETE_WORD;
	static const QString REMOVE_COMPLETE_WORD;
	static const QString RENAME_COMPLETE_WORD;
	static const QString RESCHEDULE_COMPLETE_WORD;
	static const QString SEARCH_COMPLETE_WORD;
	static const QString MARK_COMPLETE_WORD;
	static const QString UNDO_COMPLETE_WORD;
	static const QString REDO_COMPLETE_WORD;
	static const QString HELP_COMPLETE_WORD;
	static const QString HIDE_COMPLETE_WORD;
	static const QString EXIT_COMPLETE_WORD;

	static const QString ADD_OPERATION_REMINDER;
	static const QString DISPLAY_OPERATION_REMINDER;
	static const QString REMOVE_OPERATION_REMINDER;
	static const QString RENAME_OPERATION_REMINDER;
	static const QString RESCHEDULE_OPERATION_REMINDER;
	static const QString SEARCH_OPERATION_REMINDER;
	static const QString MARK_OPERATION_REMINDER;
	static const QString UNDO_OPERATION_REMINDER;
	static const QString REDO_OPERATION_REMINDER;
	static const QString HELP_OPERATION_REMINDER;
	static const QString HIDE_OPERATION_REMIDER;
	static const QString EXIT_OPERATION_REMINDER;
	static const QString OTHER_OPERATION_REMINDER;

	static const QString TRAY_MSG_TITLE;
	static const QString TRAY_MSG_CONTENT;

	static const QString ICON_STRING;

private slots:
	//////////////////////////////////////////////////////////////////////////
	//slots to handle signals emitted                                       //
	//////////////////////////////////////////////////////////////////////////
	void trayIconActivated(QSystemTrayIcon::ActivationReason);
	void showMainWindow();
	void processInputString();
	void updateStatusBar(QString text);
	void checkMinimize();
	void checkExit();

private:
	//////////////////////////////////////////////////////////////////////////
	//set up functions for the main window                                  //
	//////////////////////////////////////////////////////////////////////////
	void overallSetUp();
	void setUpTrayIcon();
	void setUpCompleter();
	void fillUpWordList();
	void setUpConnections();
	void setUpGlobalShortCutKey();
	void setUpUI();
	//////////////////////////////////////////////////////////////////////////
	//UI special functionality handling functions                           //
	//////////////////////////////////////////////////////////////////////////
	bool eventFilter(QObject* watched, QEvent* event);
	void mousePressEvent(QMouseEvent *event);
	void mouseMoveEvent(QMouseEvent *event);
	void trayHiddenMsg();
	void showStatusBarMsg(QString content);
	void hideWindow();
	void exitProgram();
	//////////////////////////////////////////////////////////////////////////
	//UI logic parts and helper methods                                     //
	//////////////////////////////////////////////////////////////////////////
	void sendStdStringToBackEnd(QString input);
	void setStatusBarMsgAndColor();
	bool startWithCaseInsensitive(QString text, const QString& keyword);
	bool equalsToKeywordWithoutCase(const QString& input, const QString& keyword);
	QString combineOutput(int size);
	QString formatString(int num);
	QString singleLineInDisplayPanel(std::string str);
	QString multipleLinesInDisplayPanel(std::string str);

private:
	//////////////////////////////////////////////////////////////////////////
	//private attributes for the main window                                //
	//////////////////////////////////////////////////////////////////////////
	Ui::TaskyDesignClass ui;
	QStringList wordlist;
	QGraphicsOpacityEffect* opacity;
	QPoint windowPosition;
	QPoint mousePressPosition;
	QPoint mouseMovePosition;
	QSystemTrayIcon* trayIcon;
};

#endif // TASKYDESIGN1_H
