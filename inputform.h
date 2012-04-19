#ifndef INPUTFORM_H
#define INPUTFORM_H

#include <QWidget>

namespace Ui {
    class inputForm;
    QString testInput(QString, double, int, int, int, int);
}

class inputForm : public QWidget
{
    Q_OBJECT

public:
    explicit inputForm(QWidget *parent = 0);
    ~inputForm();

private:
    Ui::inputForm *ui;

public slots:
    void defaultBut();
    void setBut();
    void contBut();
    void SspinvalChanged();
    void SaError();
    void ZspinvalChanged();
    void ZaError();
    void betaChanged();

};

#endif // INPUTFORM_H
