/*
 * editor.h
 *
 *  Created on: 20-Oct-2008
 *      Author: Bernd
 */
#ifndef EDITOR_H_
#define EDITOR_H_

#include <QTextEdit>

class Editor : public QTextEdit
{
    Q_OBJECT

public:
    Editor(QWidget *parent = 0);

    void newFile();
    bool save();
    bool saveAs();
    bool print();

    QSize sizeHint() const;
    QString getFileName();
    QAction *windowMenuAction() const { return action; }

    static Editor *open(QWidget *parent = 0);
    static Editor *openFile(const QString &fileName, QWidget *parent = 0);

    QString handleAlnFileName();

protected:
    void closeEvent(QCloseEvent *event);

private slots:
    void documentWasModified();

private:
    bool okToContinue();
    bool saveFile(const QString &fileName);
    void setCurrentFile(const QString &fileName);
    bool readFile(const QString &fileName);
    bool writeFile(const QString &fileName);
    QString strippedName(const QString &fullFileName);

    QString curFile;
    bool isUntitled;
    QAction *action;
};


#endif /* EDITOR_H_ */
