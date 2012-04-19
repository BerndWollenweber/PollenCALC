/*
 * editor.cpp
 *
 *  Created on: 20-Oct-2008
 *      Author: Bernd
 */


#include <QtGui>

#include "editor.h"

QString alnFileName;

Editor::Editor(QWidget *parent)
    : QTextEdit(parent)
{
    action = new QAction(this);
    action->setCheckable(true);
    connect(action, SIGNAL(triggered()), this, SLOT(show()));
    connect(action, SIGNAL(triggered()), this, SLOT(setFocus()));

    isUntitled = true;

    connect(document(), SIGNAL(contentsChanged()),
            this, SLOT(documentWasModified()));

    setWindowIcon(QPixmap(":/images/document.png"));
    setWindowTitle("[*]");
    setAttribute(Qt::WA_DeleteOnClose);
}

void Editor::newFile()
{
    static int documentNumber = 1;

    curFile = tr("document%1.txt").arg(documentNumber);
    setWindowTitle(curFile + "[*]");
    action->setText(curFile);
    isUntitled = true;
    ++documentNumber;
    alnFileName = curFile;
}

Editor *Editor::open(QWidget *parent)
{
    QString fileName = QFileDialog::getOpenFileName(parent,
        tr("Open Data File"), ".",
        tr ("Text Files (*.txt)\n"
            "Other... (*.*)"));

    alnFileName = fileName;

    if (fileName.isEmpty()){
        return 0;
    }//if
    return openFile(fileName, parent);
}

Editor *Editor::openFile(const QString &fileName, QWidget *parent)
{
    Editor *editor = new Editor(parent);
    if (editor->readFile(fileName)) {
        editor->setCurrentFile(fileName);
        //font specification
        QFont defaultFont("Courier");
        defaultFont.setPixelSize(12);
        editor->setFont(defaultFont);
        return editor;
    }
    else {
        delete editor;
        return 0;
    }//else
}

bool Editor::readFile(const QString &fileName)
{
    QFile file(fileName);
    if (!file.open(QIODevice::ReadOnly | QIODevice::Text)) {
        QMessageBox::warning(this, tr("PCC"),
             tr("Cannot read file %1:\n%2.")
             .arg(file.fileName())
             .arg(file.errorString()));
        return false;
    }//if

    QTextStream in(&file);
    QApplication::setOverrideCursor(Qt::WaitCursor);
    //--------------------
    in.setCodec("UTF-8");
    //--------------------
    setPlainText(in.readAll());
    QApplication::restoreOverrideCursor();
    return true;
}

bool Editor::save()
{
    if (isUntitled) {
        return saveAs();
    }//if
    else {
        return saveFile(curFile);
    }//else
}

bool Editor::saveAs()
{
    QString fileName;
    int fileFormat = QMessageBox::warning(this, tr("PCC - Save File"),
        tr("File %1 has been modified.\n"
            "Do you want to save it?")
            .arg(strippedName(curFile)),
            QMessageBox::Yes | QMessageBox::Cancel);

    if (fileFormat == QMessageBox::Yes) {
        curFile.replace(QString (".rtf"), QString (".txt"));
        fileName = QFileDialog::getSaveFileName(this,
        tr("Save File As"), curFile,
        tr("Text File (*.txt)"));
    }//if
    else if (fileFormat == QMessageBox::Cancel) {
        return false;
    }//else if

    if (fileName.isEmpty()){
       return false;
    }//if
    return saveFile(fileName);
}

bool Editor::saveFile(const QString &fileName)
{
    if (writeFile(fileName)) {
        setCurrentFile(fileName);
        return true;
    }//if
    else {
        return false;
    }//else
}

bool Editor::writeFile(const QString &fileName)
{
    QFile file(fileName);
    if (!file.open(QIODevice::WriteOnly | QIODevice::Text)) {
        QMessageBox::warning(this, tr("PCC"),
             tr("Cannot write file %1:\n%2.")
             .arg(file.fileName())
             .arg(file.errorString()));
        return false;
    }//if

    QTextStream out(&file);
    QApplication::setOverrideCursor(Qt::WaitCursor);
    //-----------------------
    out.setCodec("UTF-8");
    //-----------------------
    out << toPlainText();
    QApplication::restoreOverrideCursor();
    return true;
}

QString Editor::handleAlnFileName()
{
//    alnFileName.replace(QString (".aln"), QString (".txt"));
//    curFile = alnFileName;
//    action->setText(curFile);
//    isUntitled = true;
//    setWindowTitle(curFile + "[*]");
    return(alnFileName);
}

void Editor::setCurrentFile(const QString &fileName)
{
    curFile = fileName;
    isUntitled = false;
    action->setText(strippedName(curFile));
    document()->setModified(false);
    setWindowTitle(strippedName(curFile) + "[*]");
    setWindowModified(false);
}

QString Editor::strippedName(const QString &fullFileName)
{
    return QFileInfo(fullFileName).fileName();
}

void Editor::documentWasModified()
{
    setWindowModified(true);
}

bool Editor::okToContinue()
{
    if (document()->isModified()) {
        int r = QMessageBox::warning(this, tr("PCC"),
            tr("File %1 has been modified.\n"
               "Do you want to save your changes?")
            .arg(strippedName(curFile)),
            QMessageBox::Yes | QMessageBox::No | QMessageBox::Cancel);
        if (r == QMessageBox::Yes) {
            return save();
        }//if
        else if (r == QMessageBox::Cancel) {
            return false;
        }//else
    }//if
    return true; // if NO
}

void Editor::closeEvent(QCloseEvent *event)
{
    if (okToContinue()) {
        event->accept();
    }//if
    else {
        event->ignore();
    }//else
}

bool Editor::print()
{
    QPrinter printer;
    QPrintDialog *dialog = new QPrintDialog(&printer, this);
    dialog->setWindowTitle(tr("Print File"));
    if (dialog->exec() != QDialog::Accepted)
        return false;
    document()->print(&printer);
	return true;
}

QSize Editor::sizeHint() const
{
    return QSize(90 * fontMetrics().width('x'),
                 25 * fontMetrics().lineSpacing());
}

QString Editor::getFileName()
{
    return(alnFileName);
}
