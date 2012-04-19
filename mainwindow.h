/*
 * mainwindow.h
 *
 *  Created on: 20-Oct-2008
 *      Author: Bernd
 */
#ifndef MAINWINDOW_H_
#define MAINWINDOW_H_

#include <QMainWindow>
#include <QTextCursor>

QT_FORWARD_DECLARE_CLASS(QTextCharFormat)
QT_FORWARD_DECLARE_CLASS(QComboBox)
QT_FORWARD_DECLARE_CLASS(QFontComboBox)

class QAction;
class QActionGroup;
class QLabel;
class QMdiArea;
class QMenu;
class QToolBar;
class Editor;

void displayInEditor(bool endflag, QString text, QString txtfont, QTextCursor);

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow();
    Editor *myeditor;
    void appendInEditor();


public slots:
    void newFile();
    void openFile(const QString &fileName);

protected:
    void closeEvent(QCloseEvent *event);

private slots:
    void open();
    void closep();
    void print();
    void save();
    void saveAs();

    void cut();
    void copy();
    void paste();

    void textBold();
    void textUnderline();
    void textItalic();

    void textFamily(const QString &f);
    void textSize(const QString &p);
    void textColor();
    void currentCharFormatChanged(const QTextCharFormat &format);
    void setFont();

    void inputparameters();
    void output1();
    void output2();

    void about();
    void updateActions();
    void loadFiles();

private:
    void mergeFormatOnWordOrSelection(const QTextCharFormat &format);
    void fontChanged(const QFont &f);
    void colorChanged(const QColor &c);

    void createActions();
    void createMenus();
    void createToolBars();
    void createStatusBar();
    void addEditor(Editor *editor);
    Editor *activeEditor();

    QMdiArea *mdiArea;
    QLabel *readyLabel;
    QWidgetList windows;

    QMenu *fileMenu;
    QMenu *editMenu;

    QMenu *formatMenu;

    QMenu *analyseMenu;
    QMenu *windowMenu;
    QMenu *helpMenu;

    QToolBar *fileToolBar;
    QToolBar *editToolBar;
    QToolBar *formatToolBar;

    QActionGroup *windowActionGroup;
    QAction *newAction;
    QAction *openAction;
    QAction *printAction;
    QAction *saveAction;
    QAction *saveAsAction;
    QAction *exitAction;

    QAction *cutAction;
    QAction *copyAction;
    QAction *pasteAction;

    QAction *textBoldAction;
    QAction *textUnderlineAction;
    QAction *textItalicAction;
    QAction *textColorAction;
    QAction *setFontAction;

    QAction *inputAction;
    QAction *output1Action;
    QAction *output2Action;

    QAction *closeAction;
    QAction *closeAllAction;
    QAction *tileAction;
    QAction *cascadeAction;
    QAction *nextAction;
    QAction *previousAction;
    QAction *separatorAction;

    QAction *aboutAction;
    QAction *aboutQtAction;

    QFontComboBox *comboFont;
    QComboBox *comboSize;
    QToolBar *toolbFont;
 };

extern MainWindow *theEditor;

#endif /* MAINWINDOW_H_ */
