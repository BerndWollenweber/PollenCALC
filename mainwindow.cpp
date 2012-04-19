/*
 * mainwindow.cpp
 *      Author: Bernd
 */

#include <QtGui>

#include "editor.h"
#include "mainwindow.h"
#include "inputform.h"

#include "PM/show.h"

QString AlnFileName;

MainWindow *theEditor;

MainWindow::MainWindow()
{
    theEditor = this;

    mdiArea = new QMdiArea;
    setCentralWidget(mdiArea);
    connect(mdiArea, SIGNAL(subWindowActivated(QMdiSubWindow*)),
            this, SLOT(updateActions()));

    createActions();
    createMenus();
    createToolBars();

    inputAction->setEnabled(true);
    output1Action->setEnabled(false);
    output2Action->setEnabled(false);

    //set font toolbar
    QFont defaultFont("Courier");
    comboFont->setCurrentFont(defaultFont);
    comboSize->setCurrentIndex(3); //= 9 pt.

    createStatusBar();
    updateActions();

    setWindowIcon(QPixmap(":/images/icon.png"));
    setWindowTitle(tr("Pollen Compatibility Calculator"));
    QTimer::singleShot(0, this, SLOT(loadFiles()));
}

void MainWindow::loadFiles()
{
    QStringList args = QApplication::arguments();
    args.removeFirst();
    if (!args.isEmpty()) {
        foreach (QString arg, args)
            openFile(arg);
        mdiArea->cascadeSubWindows();
    }//if
    mdiArea->activateNextSubWindow();
}

void MainWindow::newFile()
{
    Editor *editor = new Editor;
    editor->newFile(); //set windows title
    addEditor(editor);
    //mdiArea->activeSubWindow()->resize(350, 300);

    //reset fonts for every new file
    fontChanged(editor->font());
    colorChanged(editor->textColor());

    QFont defFont("Courier");
    comboFont->setCurrentFont(defFont);
    comboSize->setCurrentIndex(3); //= 9 pt.
    editor->setFont(defFont);
}

void MainWindow::openFile(const QString &fileName)
{
    Editor *editor = Editor::openFile(fileName, this);
    if (editor){
      addEditor(editor);

      //resize window
      //mdiArea->activeSubWindow()->resize(650, 500);
    }//if

}

void MainWindow::closep()
{
    int r = QMessageBox::warning(this, tr("Exit PCC"),
      tr("The Program will close.\n" "Are you sure?"),
      QMessageBox::Yes | QMessageBox::No);
    if (r == QMessageBox::Yes) {
        qApp->exit(0);
    }//if
    else if (r == QMessageBox::No) {
        return;
    }//else
}

void MainWindow::closeEvent(QCloseEvent *event)
{
    mdiArea->closeAllSubWindows();
    if (!mdiArea->subWindowList().isEmpty()) {
        event->ignore();
    }//if
    else {
        event->accept();
    }//else
}

void MainWindow::open()
{
    Editor *editor = Editor::open(this);
    if (editor){
       addEditor(editor);
       AlnFileName = editor->getFileName();
       editor->setLineWrapMode(QTextEdit::NoWrap);
    }//if
}

void MainWindow::print()
{
    if (activeEditor()){
      activeEditor()->print();
    }//if
}

void MainWindow::save()
{
    if (activeEditor())
       activeEditor()->save();
}

void MainWindow::saveAs()
{
    if (activeEditor())
       activeEditor()->saveAs();
}

void MainWindow::cut()
{
    if (activeEditor())
       activeEditor()->cut();
}

void MainWindow::copy()
{
    if (activeEditor())
        activeEditor()->copy();
}

void MainWindow::paste()
{
    if (activeEditor())
        activeEditor()->paste();
}
//-----------------------------------------------------------------
void MainWindow::textBold()
{
    QTextCharFormat fmt;
    fmt.setFontWeight(textBoldAction->isChecked() ? QFont::Bold : QFont::Normal);
    mergeFormatOnWordOrSelection(fmt);
}

void MainWindow::textUnderline()
{
    QTextCharFormat fmt;
    fmt.setFontUnderline(textUnderlineAction->isChecked());
    mergeFormatOnWordOrSelection(fmt);
}

void MainWindow::textItalic()
{
    QTextCharFormat fmt;
    fmt.setFontItalic(textItalicAction->isChecked());
    mergeFormatOnWordOrSelection(fmt);
}

void MainWindow::mergeFormatOnWordOrSelection(const QTextCharFormat &format)
{
    QTextCursor cursor = activeEditor()->textCursor();
    if (!cursor.hasSelection())
        cursor.select(QTextCursor::WordUnderCursor);
    cursor.mergeCharFormat(format);
    activeEditor()->mergeCurrentCharFormat(format);
}

void MainWindow::textFamily(const QString &f)
{
    QTextCharFormat fmt;
    fmt.setFontFamily(f);
    activeEditor()->setFocus();
    mergeFormatOnWordOrSelection(fmt);
}

void MainWindow::textSize(const QString &p)
{
    QTextCharFormat fmt;
    fmt.setFontPointSize(p.toFloat());
    activeEditor()->setFocus();
    mergeFormatOnWordOrSelection(fmt);
}

void MainWindow::textColor()
{
    QColor col = QColorDialog::getColor(activeEditor()->textColor(), this);
    if (!col.isValid())
       return;
    QTextCharFormat fmt;
    fmt.setForeground(col);
    mergeFormatOnWordOrSelection(fmt);
    colorChanged(col);
}

void MainWindow::currentCharFormatChanged(const QTextCharFormat &format)
{
    fontChanged(format.font());
    colorChanged(format.foreground().color());
}

void MainWindow::fontChanged(const QFont &f)
{
    comboFont->setCurrentIndex(comboFont->findText(QFontInfo(f).family()));
    comboSize->setCurrentIndex(comboSize->findText(QString::number(f.pointSize())));
    textBoldAction->setChecked(f.bold());
    textItalicAction->setChecked(f.italic());
    textUnderlineAction->setChecked(f.underline());
}

void MainWindow::colorChanged(const QColor &c)
{
    QPixmap pix(16, 16);
    pix.fill(c);
    textColorAction->setIcon(pix);
}

void MainWindow::setFont()
{
    bool ok;
    QFont font = QFontDialog::getFont(&ok, QFont("Courier", 9), this);
    if (!ok){
       return;
    }//if
    else{
       QTextCharFormat fmt;
       fmt.setFont(font);
       mergeFormatOnWordOrSelection(fmt);
       currentCharFormatChanged(fmt);
    }//else
}
//----------------------------------------------------
void MainWindow::inputparameters()
{
    //open result window
    Editor *myeditor = new Editor;
    addEditor(myeditor);
    myeditor->setLineWrapMode(QTextEdit::NoWrap);
    myeditor->setWindowTitle("Results [*]");

    QFont myfont("Courier");
    myfont.setPixelSize(12);
    myeditor->setFont(myfont);

    QTextCursor cursor(myeditor->textCursor());
    cursor.insertText("Version 0.7a\n");
    displayInEditor(false, "Pollen Compatibility Calculator\n", "bold", cursor);

    //display in active window
    QTextCursor mycursor(activeEditor()->textCursor());
    displayInEditor(true, "Input parameters\n", "bold", mycursor);
    interface::checkedi(mycursor);

    output1Action->setEnabled(true);
    inputAction->setEnabled(false);

    //open inputform
    inputForm *dialog = new inputForm;
    dialog->show();
}

void MainWindow::output1()
{
    QTextCursor mycursor(activeEditor()->textCursor());
    bool status = interface::phaplotypes(mycursor);
    if(status == true){
        //activeEditor()->append("");
        theEditor->appendInEditor();

        output1Action->setEnabled(false);
        output2Action->setEnabled(true);
    }//if
    else {
        displayInEditor(true, "ERROR: NO INPUT PARAMETERS SELECTED", "bold", mycursor);
        inputAction->setEnabled(true);
        output1Action->setEnabled(false);
        output2Action->setEnabled(false);
    }//else
}

void MainWindow::output2()
{
    QTextCursor mycursor(activeEditor()->textCursor());
    bool status = interface::recipcross1(mycursor);
    if (status == true){
        //activeEditor()->append("");
        theEditor->appendInEditor();
        interface::collectRes();

        int response = QMessageBox::question(this, tr("Continue"),
          tr("Do you want to continue\n with the same parameter settings\n"
             "for different genotypes?"),
          QMessageBox::Yes | QMessageBox::No);
        if (response == QMessageBox::Yes) {
            inputForm *dialog2 = new inputForm;
            dialog2->show();
            output1Action->setEnabled(true);
            output2Action->setEnabled(false);
        }
        else if (response == QMessageBox::No){
            interface::showResTable();
            //activeEditor()->append("");
            theEditor->appendInEditor();
            interface::resetVecs();
            output2Action->setEnabled(false);
            inputAction->setEnabled(true);
            return;
        }//else if
    }//if
    else {
        displayInEditor(true, "Please start new Input\n", "bold", mycursor);
        output2Action->setEnabled(false);
        inputAction->setEnabled(true);
    }//else
}
//--------------------------------------------------

void MainWindow::about()
{
    QMessageBox::about(this, tr("About PCC"),
        tr("<h2>Pollen Compatibility Calculator</h2>"
        "<p>2011"
        "<p>Bernd Wollenweber, Andrea Arias Aguirre"));
}

void MainWindow::updateActions()
{
    bool hasEditor = (activeEditor() != 0);
    bool hasSelection = activeEditor()
         && activeEditor()->textCursor().hasSelection();

    saveAction->setEnabled(hasEditor);
    saveAsAction->setEnabled(hasEditor);
    cutAction->setEnabled(hasSelection);
    copyAction->setEnabled(hasSelection);
    pasteAction->setEnabled(hasEditor);
    printAction->setEnabled(hasEditor);
    textBoldAction->setEnabled(hasEditor);
    textItalicAction->setEnabled(hasEditor);
    textUnderlineAction->setEnabled(hasEditor);
    textColorAction->setEnabled(hasEditor);
    setFontAction->setEnabled(hasEditor);
    comboFont->setEnabled(hasEditor);
    comboSize->setEnabled(hasEditor);
    closeAction->setEnabled(hasEditor);
    closeAllAction->setEnabled(hasEditor);
    tileAction->setEnabled(hasEditor);
    cascadeAction->setEnabled(hasEditor);
    nextAction->setEnabled(hasEditor);
    previousAction->setEnabled(hasEditor);
    separatorAction->setVisible(hasEditor);

    if (activeEditor())
        activeEditor()->windowMenuAction()->setChecked(true);
}

void MainWindow::createActions()
{
    newAction = new QAction(tr("&New"), this);
    newAction->setIcon(QIcon(":/images/filenew.png"));
    newAction->setShortcut(QKeySequence::New);
    newAction->setStatusTip(tr("Create a new file"));
    connect(newAction, SIGNAL(triggered()), this, SLOT(newFile()));

    openAction = new QAction(tr("&Open..."), this);
    openAction->setIcon(QIcon(":/images/fileopen.png"));
    openAction->setShortcut(QKeySequence::Open);
    openAction->setStatusTip(tr("Open an existing file"));
    connect(openAction, SIGNAL(triggered()), this, SLOT(open()));

    printAction = new QAction(QIcon(":/images/fileprint.png"), tr("&Print"), this);
    printAction->setShortcut(QKeySequence::Print);
    printAction->setStatusTip(tr("Print File"));
    connect(printAction, SIGNAL(triggered()), this, SLOT(print()));

    saveAction = new QAction(tr("&Save"), this);
    saveAction->setIcon(QIcon(":/images/filesave.png"));
    saveAction->setShortcut(QKeySequence::Save);
    saveAction->setStatusTip(tr("Save the file to disk"));
    connect(saveAction, SIGNAL(triggered()), this, SLOT(save()));

    saveAsAction = new QAction(tr("Save &As..."), this);
    saveAsAction->setIcon(QIcon(":/images/filesave.png"));
    saveAsAction->setStatusTip(tr("Save the file under a new name"));
    connect(saveAsAction, SIGNAL(triggered()), this, SLOT(saveAs()));

    exitAction = new QAction(tr("&Quit..."), this);
    exitAction->setShortcut(tr("Ctrl+Q"));
    exitAction->setStatusTip(tr("Exit the application"));
    connect(exitAction, SIGNAL(triggered()), this, SLOT(closep()));

    cutAction = new QAction(tr("Cu&t"), this);
    cutAction->setIcon(QIcon(":/images/editcut.png"));
    cutAction->setShortcut(QKeySequence::Cut);
    cutAction->setStatusTip(tr("Cut the current selection to the clipboard"));
    connect(cutAction, SIGNAL(triggered()), this, SLOT(cut()));

    copyAction = new QAction(tr("&Copy"), this);
    copyAction->setIcon(QIcon(":/images/editcopy.png"));
    copyAction->setShortcut(QKeySequence::Copy);
    copyAction->setStatusTip(tr("Copy the current selection to the clipboard"));
    connect(copyAction, SIGNAL(triggered()), this, SLOT(copy()));

    pasteAction = new QAction(tr("&Paste"), this);
    pasteAction->setIcon(QIcon(":/images/editpaste.png"));
    pasteAction->setShortcut(QKeySequence::Paste);
    pasteAction->setStatusTip(tr("Paste the clipboard's contents at the cursor position"));
    connect(pasteAction, SIGNAL(triggered()), this, SLOT(paste()));

    //----------------------------------------------------------
    textBoldAction = new QAction(QIcon(":/images/textbold.png"), tr("&Bold"), this);
    textBoldAction->setShortcut(Qt::CTRL + Qt::Key_B);
    textBoldAction->setStatusTip(tr("Text Bold"));
    QFont bold;
    bold.setBold(true);
    textBoldAction->setFont(bold);
    textBoldAction->setCheckable(true);
    connect(textBoldAction, SIGNAL(triggered()), this, SLOT(textBold()));

    textItalicAction = new QAction(QIcon(":/images/textitalic.png"), tr("&Italic"), this);
    textItalicAction->setShortcut(Qt::CTRL + Qt::Key_I);
    textItalicAction->setStatusTip(tr("Text Italic"));
    QFont italic;
    italic.setItalic(true);
    textItalicAction->setFont(italic);
    textItalicAction->setCheckable(true);
    connect(textItalicAction, SIGNAL(triggered()), this, SLOT(textItalic()));

    textUnderlineAction = new QAction(QIcon(":/images/textunder.png"), tr("&Underline"), this);
    textUnderlineAction->setShortcut(Qt::CTRL + Qt::Key_U);
    textUnderlineAction->setStatusTip(tr("Text Underline"));
    QFont underline;
    underline.setUnderline(true);
    textUnderlineAction->setFont(underline);
    textUnderlineAction->setCheckable(true);
    connect(textUnderlineAction, SIGNAL(triggered()), this, SLOT(textUnderline()));

    QPixmap pix(16, 16);
    pix.fill(Qt::black);
    textColorAction = new QAction(pix, tr("&Color..."), this);
    textColorAction->setStatusTip(tr("Select Text Color"));
    connect(textColorAction, SIGNAL(triggered()), this, SLOT(textColor()));

    //create font toolbar
    toolbFont = new QToolBar(this);
    toolbFont->setAllowedAreas(Qt::TopToolBarArea | Qt::BottomToolBarArea);
    toolbFont->setWindowTitle(tr("Format Actions"));
    addToolBarBreak(Qt::TopToolBarArea);
    //addToolBar(toolbFont); //moved to createtoolbar

    //create fontcombobox
    comboFont = new QFontComboBox(toolbFont);
    toolbFont->addWidget(comboFont);
    comboFont->setToolTip("Set Font Type");
    comboFont->setStatusTip("Select Font Type");
    connect(comboFont, SIGNAL(activated(const QString &)), this,
            SLOT(textFamily(const QString &)));

    //create fontsize combobox
    comboSize = new QComboBox(toolbFont);
    comboSize->setObjectName("comboSize");
    toolbFont->addWidget(comboSize);
    comboSize->setToolTip("Set Font Size");
    comboSize->setStatusTip("Select Font Size");
    comboSize->setEditable(true);
    QFontDatabase db;
    foreach(int size, db.standardSizes())
        comboSize->addItem(QString::number(size));
    connect(comboSize, SIGNAL(activated(const QString &)),
        this, SLOT(textSize(const QString &)));
    comboSize->setCurrentIndex(comboSize->findText(QString::number(QApplication::font().pointSize())));

    setFontAction = new QAction(tr("&Font"), this);
    setFontAction->setShortcut(Qt::CTRL + Qt::Key_F);
    setFontAction->setStatusTip(tr("Set Font"));
    connect(setFontAction, SIGNAL(triggered()), this, SLOT(setFont()));
    //----------------------------------------------------------

    inputAction = new QAction(tr("Input"), this);
    inputAction->setShortcut(Qt::CTRL + Qt::Key_A);
    inputAction->setStatusTip(tr("Select Input Paramerters"));
    connect(inputAction, SIGNAL(triggered()), this, SLOT(inputparameters()));

    output1Action = new QAction(tr("Output1"), this);
    output1Action->setShortcut(Qt::CTRL + Qt::Key_R);
    output1Action->setStatusTip(tr("Output 1"));
    connect(output1Action, SIGNAL(triggered()), this, SLOT(output1()));

    output2Action = new QAction(tr("Output2"), this);
    output2Action->setShortcut(Qt::CTRL + Qt::Key_T);
    output2Action->setStatusTip(tr("Output 2"));
    connect(output2Action, SIGNAL(triggered()), this, SLOT(output2()));
    //----------------------------------------------------------

    closeAction = new QAction(tr("Cl&ose"), this);
    closeAction->setShortcut(QKeySequence::Close);
    closeAction->setStatusTip(tr("Close the active window"));
    connect(closeAction, SIGNAL(triggered()), mdiArea, SLOT(closeActiveSubWindow()));

    closeAllAction = new QAction(tr("Close &All"), this);
    closeAllAction->setStatusTip(tr("Close all windows"));
    connect(closeAllAction, SIGNAL(triggered()), mdiArea, SLOT(closeAllSubWindows()));

    tileAction = new QAction(tr("&Tile"), this);
    tileAction->setStatusTip(tr("Tile the windows"));
    connect(tileAction, SIGNAL(triggered()), mdiArea, SLOT(tileSubWindows()));

    cascadeAction = new QAction(tr("&Cascade"), this);
    cascadeAction->setStatusTip(tr("Cascade the windows"));
    connect(cascadeAction, SIGNAL(triggered()), mdiArea, SLOT(cascadeSubWindows()));

    nextAction = new QAction(tr("Ne&xt"), this);
    nextAction->setShortcut(QKeySequence::NextChild);
    nextAction->setStatusTip(tr("Move the focus to the next window"));
    connect(nextAction, SIGNAL(triggered()), mdiArea, SLOT(activateNextSubWindow()));

    previousAction = new QAction(tr("Pre&vious"), this);
    previousAction->setShortcut(QKeySequence::PreviousChild);
    previousAction->setStatusTip(tr("Move the focus to the previous window"));
    connect(previousAction, SIGNAL(triggered()), mdiArea, SLOT(activatePreviousSubWindow()));

    separatorAction = new QAction(this);
    separatorAction->setSeparator(true);

    aboutAction = new QAction(tr("&About"), this);
    aboutAction->setStatusTip(tr("the application's About box"));
    connect(aboutAction, SIGNAL(triggered()), this, SLOT(about()));

    aboutQtAction = new QAction(tr("About &Qt"), this);
    aboutQtAction->setStatusTip(tr("the Qt library's About box"));
    connect(aboutQtAction, SIGNAL(triggered()), qApp, SLOT(aboutQt()));

    windowActionGroup = new QActionGroup(this);
}

void MainWindow::createMenus()
{
    fileMenu = menuBar()->addMenu(tr("&File"));
    //fileMenu->addAction(newAction);
    fileMenu->addAction(openAction);
    fileMenu->addAction(printAction);
    fileMenu->addSeparator();
    fileMenu->addAction(saveAction);
    fileMenu->addAction(saveAsAction);
    fileMenu->addSeparator();
    fileMenu->addAction(exitAction);

    analyseMenu = menuBar()->addMenu(tr("&Analyse"));
    analyseMenu->addAction(inputAction);
    analyseMenu->addSeparator();
    analyseMenu->addAction(output1Action);
    analyseMenu->addAction(output2Action);

    editMenu = menuBar()->addMenu(tr("&Edit"));
    editMenu->addAction(cutAction);
    editMenu->addAction(copyAction);
    editMenu->addAction(pasteAction);

    formatMenu = menuBar()->addMenu(tr("&Format"));
    formatMenu->addAction(setFontAction);
    formatMenu->addSeparator();
    formatMenu->addAction(textBoldAction);
    formatMenu->addAction(textItalicAction);
    formatMenu->addAction(textUnderlineAction);
    formatMenu->addSeparator();
    formatMenu->addAction(textColorAction);

    windowMenu = menuBar()->addMenu(tr("&Window"));
    windowMenu->addAction(closeAction);
    windowMenu->addAction(closeAllAction);
    windowMenu->addSeparator();
    windowMenu->addAction(tileAction);
    windowMenu->addAction(cascadeAction);
    windowMenu->addSeparator();
    windowMenu->addAction(nextAction);
    windowMenu->addAction(previousAction);
    windowMenu->addAction(separatorAction);

    menuBar()->addSeparator();

    helpMenu = menuBar()->addMenu(tr("&Help"));
    helpMenu->addAction(aboutAction);
    helpMenu->addAction(aboutQtAction);
}

void MainWindow::createToolBars()
{
    fileToolBar = addToolBar(tr("File"));
    //fileToolBar->addAction(newAction);
    fileToolBar->addAction(openAction);
    fileToolBar->addAction(saveAction);
    fileToolBar->addAction(printAction);

    editToolBar = addToolBar(tr("Edit"));
    editToolBar->addAction(cutAction);
    editToolBar->addAction(copyAction);
    editToolBar->addAction(pasteAction);

    formatToolBar = addToolBar(tr("Format"));
    formatToolBar->addAction(textBoldAction);
    formatToolBar->addAction(textItalicAction);
    formatToolBar->addAction(textUnderlineAction);
    formatToolBar->addAction(textColorAction);

    addToolBar(toolbFont);
}

void MainWindow::createStatusBar()
{
    readyLabel = new QLabel(tr(" Ready"));
    statusBar()->addWidget(readyLabel, 1);
}

void MainWindow::addEditor(Editor *editor)
{
    connect(editor, SIGNAL(copyAvailable(bool)),
        cutAction, SLOT(setEnabled(bool)));
    connect(editor, SIGNAL(copyAvailable(bool)),
        copyAction, SLOT(setEnabled(bool)));

    QMdiSubWindow *subWindow = mdiArea->addSubWindow(editor);
    windowMenu->addAction(editor->windowMenuAction());
    windowActionGroup->addAction(editor->windowMenuAction());
    //subWindow->resize(650, 600);
    subWindow->show();
}

Editor *MainWindow::activeEditor()
{
    QMdiSubWindow *subWindow = mdiArea->activeSubWindow();
    if (subWindow)
       return qobject_cast<Editor *>(subWindow->widget());
    return 0;
}

void MainWindow::appendInEditor()
{
    //scroll subwindow
    //called from show.transdata also
    activeEditor()->append("");
}

void displayInEditor(bool endflag, QString text, QString txtfont, QTextCursor cursor)
{
    //QTextCursor mycursor(activeEditor()->textCursor());
    if(endflag){
       cursor.insertText("\n");
    }//if

    QFont textFont("Courier");
    textFont.setPixelSize(12);
    QFont boldFont("Arial");
    boldFont.setPixelSize(12);

    if(txtfont == "bold"){
          QTextCharFormat boldFormat;
          boldFormat.setFont(boldFont);
          boldFormat.setFontWeight(QFont::Bold);
          cursor.insertText(text, boldFormat);
    }//if
    else if (txtfont == "normal"){
          QTextCharFormat textFormat;
          textFormat.setFont(textFont);
          textFormat.setFontWeight(QFont::Normal);
          cursor.insertText(text, textFormat);
    }//else if
    else if (txtfont == "underl"){
          QTextCharFormat textFormat;
          textFormat.setFont(textFont);
          textFormat.setFontWeight(QFont::Normal);
          textFormat.setFontUnderline(true);
          cursor.insertText(text, textFormat);
    }//else if
}

