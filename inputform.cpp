#include <QTextStream>
#include "inputform.h"
#include "ui_inputform.h"

#include "PM/show.h"

QVector <aln::s_inp> vTransInp;
QVector <aln::s_gen> vTransGen;

QString testInput(QString id, double ta, int tb, int td1, int td2, int td3)
{
    //bool ok;
    QString state = id;
    double maxalpha = 1.0/7.0;

    if (ta > maxalpha){
        state = (": amax error");
        //ok = false;
    }
    else if ((tb == 0 && ta == 0) &&
          ((td1 == 1 && td2 == 0 && td3 == 0)
        || (td2 == 1 && td1 == 0 && td3 == 0)
        || (td3 == 1 && td1 == 0 && td2 == 0))){
            state.append(": full disomic ");
            //ok = true;
    }
    else if ((tb == 1 && ta == 0) && (td1 == 0 && td2 == 0 && td3 == 0)){
        state.append(": full tetrasomic ");
        //ok = true;
    }
    else if ((tb == 1 && ta > 0) && (td1 == 0 && td2 == 0 && td3 == 0)){
        state.append(": full tetras.+r. ");
        //ok = true;
    }
    else if (tb == 0 && ta > 0){
        state = "ab error";
        //ok = false;
    }
    else {
        state = "d error";
        //ok = false;
    }

    return(state);
}

inputForm::inputForm(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::inputForm)
{
    ui->setupUi(this);
    ui->continueButton->setEnabled(false);
    ui->defaultButton->setToolTip("Set default values");
    ui->defaultButton->setShortcut(Qt::CTRL + Qt::Key_D);
    ui->setButton->setToolTip("Click to set parameters");
    ui->setButton->setShortcut(Qt::CTRL + Qt::Key_S);
    ui->continueButton->setToolTip("Click to continue");
    ui->continueButton->setShortcut(Qt::CTRL + Qt::Key_T);

    //limit input values
    QRegExp regExp("[0][.][0-9]{0,2}");
    ui->Sa_lineEdit->setValidator(new QRegExpValidator(regExp, this));
    ui->Za_lineEdit->setValidator(new QRegExpValidator(regExp, this));

    QRegExp regExp2("[A-I]");
    ui->g1j1_lineEdit->setValidator(new QRegExpValidator(regExp2, this));
    ui->g1k1_lineEdit->setValidator(new QRegExpValidator(regExp2, this));
    ui->g1l1_lineEdit->setValidator(new QRegExpValidator(regExp2, this));
    ui->g1m1_lineEdit->setValidator(new QRegExpValidator(regExp2, this));
    ui->g2j1_lineEdit->setValidator(new QRegExpValidator(regExp2, this));
    ui->g2k1_lineEdit->setValidator(new QRegExpValidator(regExp2, this));
    ui->g2l1_lineEdit->setValidator(new QRegExpValidator(regExp2, this));
    ui->g2m1_lineEdit->setValidator(new QRegExpValidator(regExp2, this));

    QRegExp regExp3("[1-9]");
    ui->g1j2_lineEdit->setValidator(new QRegExpValidator(regExp3, this));
    ui->g1k2_lineEdit->setValidator(new QRegExpValidator(regExp3, this));
    ui->g1l2_lineEdit->setValidator(new QRegExpValidator(regExp3, this));
    ui->g1m2_lineEdit->setValidator(new QRegExpValidator(regExp3, this));
    ui->g2j2_lineEdit->setValidator(new QRegExpValidator(regExp3, this));
    ui->g2k2_lineEdit->setValidator(new QRegExpValidator(regExp3, this));
    ui->g2l2_lineEdit->setValidator(new QRegExpValidator(regExp3, this));
    ui->g2m2_lineEdit->setValidator(new QRegExpValidator(regExp3, this));

    //default alpha
    ui->Sa_lineEdit->setText("0");
    ui->Za_lineEdit->setText("0");

    //for multiple runs get parameter values from previous run
    //input parameters stores in vOrgInp; genotype p. in vOrgGen
    QVector<aln::s_inp> vtInp = interface::getVecOrgInp();
    QVector<aln::s_gen> vtGen = interface::getVecOrgGen();
    QString convtotxt("");
    if (vtInp.size() > 0){
        convtotxt = convtotxt.setNum(vtInp[0].da);
        ui->Sa_lineEdit->setText(convtotxt);
        convtotxt = convtotxt.setNum(vtInp[1].da);
        ui->Za_lineEdit->setText(convtotxt);
        ui->Sb_spinBox->setValue(vtInp[0].ib);
        ui->Sd1_spinBox->setValue(vtInp[0].id1);
        ui->Sd2_spinBox->setValue(vtInp[0].id2);
        ui->Sd3_spinBox->setValue(vtInp[0].id3);
        ui->Zb_spinBox->setValue(vtInp[1].ib);
        ui->Zd1_spinBox->setValue(vtInp[1].id1);
        ui->Zd2_spinBox->setValue(vtInp[1].id2);
        ui->Zd3_spinBox->setValue(vtInp[1].id3);
    }
    if (vtGen.size() > 0){
        ui->g1j1_lineEdit->setText(vtGen[0].cj);
        ui->g1k1_lineEdit->setText(vtGen[0].ck);
        ui->g1l1_lineEdit->setText(vtGen[0].cl);
        ui->g1m1_lineEdit->setText(vtGen[0].cm);
        convtotxt = convtotxt.setNum(vtGen[0].ij);
        ui->g1j2_lineEdit->setText(convtotxt);
        convtotxt = convtotxt.setNum(vtGen[0].ik);
        ui->g1k2_lineEdit->setText(convtotxt);
        convtotxt = convtotxt.setNum(vtGen[0].il);
        ui->g1l2_lineEdit->setText(convtotxt);
        convtotxt = convtotxt.setNum(vtGen[0].im);
        ui->g1m2_lineEdit->setText(convtotxt);

        ui->g2j1_lineEdit->setText(vtGen[1].cj);
        ui->g2k1_lineEdit->setText(vtGen[1].ck);
        ui->g2l1_lineEdit->setText(vtGen[1].cl);
        ui->g2m1_lineEdit->setText(vtGen[1].cm);
        convtotxt = convtotxt.setNum(vtGen[1].ij);
        ui->g2j2_lineEdit->setText(convtotxt);
        convtotxt = convtotxt.setNum(vtGen[1].ik);
        ui->g2k2_lineEdit->setText(convtotxt);
        convtotxt = convtotxt.setNum(vtGen[1].il);
        ui->g2l2_lineEdit->setText(convtotxt);
        convtotxt = convtotxt.setNum(vtGen[1].im);
        ui->g2m2_lineEdit->setText(convtotxt);
    }

    connect (ui->defaultButton, SIGNAL(clicked()), this, SLOT(defaultBut()) );
    connect (ui->setButton, SIGNAL(clicked()), this, SLOT(setBut()) );
    connect (ui->continueButton, SIGNAL(clicked()), this, SLOT(contBut()) );

    connect (ui->Sd1_spinBox, SIGNAL(valueChanged(int)), this, SLOT(SspinvalChanged()) );
    connect (ui->Sd2_spinBox, SIGNAL(valueChanged(int)), this, SLOT(SspinvalChanged()) );
    connect (ui->Sd3_spinBox, SIGNAL(valueChanged(int)), this, SLOT(SspinvalChanged()) );
    connect (ui->Sa_lineEdit, SIGNAL(textChanged(QString)), this, SLOT(SaError()) );
    connect (ui->Sb_spinBox, SIGNAL(valueChanged(int)), this, SLOT(SaError()) );

    connect (ui->Zd1_spinBox, SIGNAL(valueChanged(int)), this, SLOT(ZspinvalChanged()) );
    connect (ui->Zd2_spinBox, SIGNAL(valueChanged(int)), this, SLOT(ZspinvalChanged()) );
    connect (ui->Zd3_spinBox, SIGNAL(valueChanged(int)), this, SLOT(ZspinvalChanged()) );
    connect (ui->Za_lineEdit, SIGNAL(textChanged(QString)), this, SLOT(ZaError()) );
    connect (ui->Zb_spinBox, SIGNAL(valueChanged(int)), this, SLOT(ZaError()) );

    connect (ui->Sb_spinBox, SIGNAL(valueChanged(int)), ui->Zb_spinBox, SLOT(setValue(int)) );
    connect (ui->Zb_spinBox, SIGNAL(valueChanged(int)), ui->Sb_spinBox, SLOT(setValue(int)) );

    connect (ui->Sb_spinBox, SIGNAL(valueChanged(int)), this, SLOT(betaChanged()) );
    connect (ui->Zb_spinBox, SIGNAL(valueChanged(int)), this, SLOT(betaChanged()) );

}

inputForm::~inputForm()
{
    delete ui;
}

void inputForm::defaultBut()
{
    //Set default values
    ui->Sa_lineEdit->setText("0");
    ui->Sb_spinBox->setValue(0); ui->Sd1_spinBox->setValue(0);
    ui->Sd2_spinBox->setValue(1); ui->Sd3_spinBox->setValue(0);
    ui->Za_lineEdit->setText("0");
    ui->Zb_spinBox->setValue(0); ui->Zd1_spinBox->setValue(0);
    ui->Zd2_spinBox->setValue(1); ui->Zd3_spinBox->setValue(0);

    ui->g1j1_lineEdit->setText("A"); ui->g1k1_lineEdit->setText("B");
    ui->g1l1_lineEdit->setText("C"); ui->g1m1_lineEdit->setText("D");
    ui->g1j2_lineEdit->setText("1"); ui->g1k2_lineEdit->setText("2");
    ui->g1l2_lineEdit->setText("3"); ui->g1m2_lineEdit->setText("4");

    ui->g2j1_lineEdit->setText("A"); ui->g2k1_lineEdit->setText("B");
    ui->g2l1_lineEdit->setText("C"); ui->g2m1_lineEdit->setText("D");
    ui->g2j2_lineEdit->setText("1"); ui->g2k2_lineEdit->setText("2");
    ui->g2l2_lineEdit->setText("4"); ui->g2m2_lineEdit->setText("5");

    ui->resultLabel->setText("S and Z: disomic state");
}

void inputForm::setBut()
{
    //reset vectors
    vTransInp.clear();
    vTransGen.clear();

    QString str;
    QTextStream ts(&str);
    QString S_state;
    QString Z_state;
    QChar alpha = QChar(0x3B1);
    QChar beta = QChar(0x3B2);
    QChar delta = QChar(0x394);

    double sa = aln::round(ui->Sa_lineEdit->text().toDouble(), 2);
    int sb = ui->Sb_spinBox->text().toInt();
    int sd1 = ui->Sd1_spinBox->text().toInt();
    int sd2 = ui->Sd2_spinBox->text().toInt();
    int sd3 = ui->Sd3_spinBox->text().toInt();
    double za = aln::round(ui->Za_lineEdit->text().toDouble(), 2);
    int zb = ui->Zb_spinBox->text().toInt();
    int zd1 = ui->Zd1_spinBox->text().toInt();
    int zd2 = ui->Zd2_spinBox->text().toInt();
    int zd3 = ui->Zd3_spinBox->text().toInt();

    //check state for S and Z
    S_state = testInput("S", sa, sb, sd1, sd2, sd3);
    Z_state = testInput("Z", za, zb, zd1, zd2, zd3);

    QPalette p_red;
    p_red.setColor(QPalette::Base,QColor(255, 0, 0));

    //check genotypes
    if( ui->g1j1_lineEdit->text() == "" ||
        ui->g1k1_lineEdit->text() == "" ||
        ui->g1l1_lineEdit->text() == "" || ui->g1m1_lineEdit->text() == "" ||
        ui->g1j2_lineEdit->text() == "" || ui->g1k2_lineEdit->text() == "" ||
        ui->g1l2_lineEdit->text() == "" || ui->g1m2_lineEdit->text() == "" ||
        ui->g2j1_lineEdit->text() == "" || ui->g2k1_lineEdit->text() == "" ||
        ui->g2l1_lineEdit->text() == "" || ui->g2m1_lineEdit->text() == "" ||
        ui->g2j2_lineEdit->text() == "" || ui->g2k2_lineEdit->text() == "" ||
        ui->g2l2_lineEdit->text() == "" || ui->g2m2_lineEdit->text() == "" ){
      ui->resultLabel->setText("PLEASE DEFINE GENOTYPES!");
    }//if
    //if errors - break
    //check S
    else if (S_state == ": amax error"){
        ts << "ERROR! Max. " << alpha << " = 1/7";
        ui->resultLabel->setText(str);
        ui->Sa_lineEdit->setPalette(p_red);
    }//else if

    else if (S_state == "ab error"){
        ts << "ERROR! " << "S: " << alpha << " / " << beta;
        ui->resultLabel->setText(str);
        ui->Sb_spinBox->setPalette(p_red);
    }//else if
    else if (S_state == "d error"){
        ts << "ERROR! " << "S: " << delta << " = 0";
        ui->resultLabel->setText(str);
        //if beta == 1 all delta should be 0
        if(ui->Sb_spinBox->value() == 1){
            if(ui->Sd1_spinBox->value() == 1)
                ui->Sd1_spinBox->setPalette(p_red);
            if(ui->Sd2_spinBox->value() == 1)
                ui->Sd2_spinBox->setPalette(p_red);
            if(ui->Sd3_spinBox->value() == 1)
                ui->Sd3_spinBox->setPalette(p_red);
        }//if
        // if beta == 0 at least one delta should be 1
        if(ui->Sb_spinBox->value() == 0){
            if(ui->Sd1_spinBox->value() == 0)
                ui->Sd1_spinBox->setPalette(p_red);
            if(ui->Sd2_spinBox->value() == 0)
                ui->Sd2_spinBox->setPalette(p_red);
            if(ui->Sd3_spinBox->value() == 0)
                ui->Sd3_spinBox->setPalette(p_red);
        }//if
         //-------------------------------------------
    }//else if

    //check Z
    else if (Z_state == ": amax error"){
        ts << "ERROR! Max. " << alpha << " = 1/7";
        ui->resultLabel->setText(str);
         ui->Za_lineEdit->setPalette(p_red);
    }//else if
    else if (Z_state == "ab error"){
        ts << "ERROR! " << "Z: " << alpha << " / " << beta;
        ui->resultLabel->setText(str);
        ui->Zb_spinBox->setPalette(p_red);
    }//else if
    else if (Z_state == "d error"){
        ts << "ERROR! " << "Z: " << delta << " = 0";
        ui->resultLabel->setText(str);
        //if beta == 1 all delta should be 0
        if(ui->Zb_spinBox->value() == 1){
            if(ui->Zd1_spinBox->value() == 1)
                ui->Zd1_spinBox->setPalette(p_red);
            if(ui->Zd2_spinBox->value() == 1)
                ui->Zd2_spinBox->setPalette(p_red);
            if(ui->Zd3_spinBox->value() == 1)
                ui->Zd3_spinBox->setPalette(p_red);
        }//if
        // if beta == 0 at least one delta should be 1
        if(ui->Zb_spinBox->value() == 0){
            if(ui->Zd1_spinBox->value() == 0)
                ui->Zd1_spinBox->setPalette(p_red);
            if(ui->Zd2_spinBox->value() == 0)
                ui->Zd2_spinBox->setPalette(p_red);
            if(ui->Zd3_spinBox->value() == 0)
                ui->Zd3_spinBox->setPalette(p_red);
        }//if
        //-------------------------------------------
    }//else if

    //if no errors pushback data into vectors
    else {
        ts << S_state;
        ts << Z_state;
        ui->resultLabel->setText(str);

        //build vTransInp
        aln::s_inp myinp;
        myinp.da = sa; myinp.ib = sb; myinp.id1 = sd1;
        myinp.id2 = sd2; myinp.id3 = sd3; myinp.state = S_state;
        vTransInp.push_back(myinp);
        myinp.da = za; myinp.ib = zb; myinp.id1 = zd1;
        myinp.id2 = zd2; myinp.id3 = zd3; myinp.state = Z_state;
        vTransInp.push_back(myinp);

        //build vTansGen
        aln::s_gen mygen;
        mygen.cj = ui->g1j1_lineEdit->text(); mygen.ck = ui->g1k1_lineEdit->text();
        mygen.cl = ui->g1l1_lineEdit->text(); mygen.cm = ui->g1m1_lineEdit->text();
        mygen.ij = ui->g1j2_lineEdit->text().toInt();
        mygen.ik = ui->g1k2_lineEdit->text().toInt();
        mygen.il = ui->g1l2_lineEdit->text().toInt();
        mygen.im = ui->g1m2_lineEdit->text().toInt();
        vTransGen.push_back(mygen);

        mygen.cj = ui->g2j1_lineEdit->text(); mygen.ck = ui->g2k1_lineEdit->text();
        mygen.cl = ui->g2l1_lineEdit->text(); mygen.cm = ui->g2m1_lineEdit->text();
        mygen.ij = ui->g2j2_lineEdit->text().toInt();
        mygen.ik = ui->g2k2_lineEdit->text().toInt();
        mygen.il = ui->g2l2_lineEdit->text().toInt();
        mygen.im = ui->g2m2_lineEdit->text().toInt();
        vTransGen.push_back(mygen);

        ui->continueButton->setEnabled(true);
    }//else

    str.clear(); ts.string()->clear();   
}

void inputForm::contBut()
{
    //transfer data
    interface::transdata(vTransInp, vTransGen);

    //reset vectors for multiple runs
    vTransInp.clear();
    vTransGen.clear();

    //close form
    close();
}

void inputForm::SspinvalChanged()
{
    //reset Sd spinBox palette
    QPalette p_white;
    p_white.setColor(QPalette::Base,QColor(255, 255, 255));
    ui->Sd1_spinBox->setPalette(p_white);
    ui->Sd2_spinBox->setPalette(p_white);
    ui->Sd3_spinBox->setPalette(p_white);
}

void inputForm::SaError()
{
   //reset Sa palette
    QPalette p_white;
    p_white.setColor(QPalette::Base,QColor(255, 255, 255));
    ui->Sa_lineEdit->setPalette(p_white);
    ui->Sb_spinBox->setPalette(p_white);
}

void inputForm::ZspinvalChanged()
{
    //reset Zd spinBox palette
    QPalette p_white;
    p_white.setColor(QPalette::Base,QColor(255, 255, 255));
    ui->Zd1_spinBox->setPalette(p_white);
    ui->Zd2_spinBox->setPalette(p_white);
    ui->Zd3_spinBox->setPalette(p_white);
}

void inputForm::ZaError()
{
    //reset Za palette
     QPalette p_white;
     p_white.setColor(QPalette::Base,QColor(255, 255, 255));
     ui->Za_lineEdit->setPalette(p_white);
     ui->Zb_spinBox->setPalette(p_white);
}

void inputForm::betaChanged()
{
    if(ui->Sb_spinBox->value() == 1 || ui->Zb_spinBox->value() == 1){
        ui->Sd1_spinBox->setValue(0);
        ui->Sd2_spinBox->setValue(0);
        ui->Sd3_spinBox->setValue(0);
        ui->Zd1_spinBox->setValue(0);
        ui->Zd2_spinBox->setValue(0);
        ui->Zd3_spinBox->setValue(0);
    }//if
}
