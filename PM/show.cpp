#include "show.h"
#include "alnfile.h"

#include <iostream>
//#include <algorithm>

#include <QTextStream>
#include <QString>
#include <QVector>
#include <QTextCursor>
#include <QMessageBox>

#include "mainwindow.h"

//global vectors
QVector <aln::s_inp> vOrgInp;
QVector <aln::s_gen> vOrgGen;
QVector <aln::s_hap> vHaploG1;
QVector <aln::s_hap> vHaploG2;
QVector <aln::s_res> vRes;
QVector <aln::s_per> vPercent;
QVector <aln::s_tab> vCompatTab;

//current window
QTextCursor mywin;

using namespace std;

namespace interface{

  void displayOnWin(bool endflag, QString text, QString txtfont, QTextCursor mywin)
  {
      //tranfer to MainWindow...
      displayInEditor(endflag, text, txtfont, mywin);
  }
  //-------------------------------------------------------------------------
  void ShowMessage(QString msgText)
  {
       QMessageBox msgBox;
       msgBox.setText(msgText);
       msgBox.exec();
  }
  //-------------------------------------------------------------------------
  void showHaplotypes(QVector<aln::s_hap> vT, int nrG)
  {
       QString str;
       QTextStream ts(&str);
       ts << "Genotype " << nrG << endl;
       ts << "S   Freq Prob          Z   Freq Prob" << endl;
       for (int i = 0; i < 10; i++){
            ts << vT[i].a << " " << vT[i].b << " " << vT[i].dfreq << " ";
            ts.setRealNumberPrecision(2); ts << fixed << vT[i].dprob << "\t";
            ts << vT[i+10].a << " " << vT[i+10].b << " " << vT[i+10].dfreq << " ";
            ts.setRealNumberPrecision(2); ts << fixed << vT[i+10].dprob << endl;
        }//for
        interface::displayOnWin(true, str, "normal", mywin);
        str.clear(); ts.string()->clear();
  }
  //-------------------------------------------------------------------------
   QString showOutput2(QVector<aln::s_gen> &vOrgGen, int nrG)
   {
        //print reciprocal crosses in genotype 1 & 2

        QString str;
        QTextStream ts(&str);

        ts << "\nOutput 2: Reciprocral crosses" << endl;
        interface::displayOnWin(true, str, "bold", mywin);
        str.clear(); ts.string()->clear();

        QString femaleGT;
        QTextStream qts(&femaleGT);

        int i = 0;
        int j = 0;
        if(nrG == 1){
           i = 0; //male
           j = 1; //female
        }//if
        else if(nrG == 2){
           i = 1; //male
           j = 0; //female
        }//else if
        qts << vOrgGen[j].cj << vOrgGen[j].ck << vOrgGen[j].cl << vOrgGen[j].cm
            << vOrgGen[j].ij << vOrgGen[j].ik << vOrgGen[j].il << vOrgGen[j].im;

        ts << "      Male		Female" << endl;
        ts << vOrgGen[i].cj << " " << vOrgGen[i].ck << " " << vOrgGen[i].cl << " "
           << vOrgGen[i].cm << " " << vOrgGen[i].ij << " " << vOrgGen[i].ik << " "
           << vOrgGen[i].il << " " << vOrgGen[i].im;
        ts << "  x  ";
        ts << vOrgGen[j].cj << " " << vOrgGen[j].ck << " " << vOrgGen[j].cl << " "
           << vOrgGen[j].cm << " " << vOrgGen[j].ij << " " << vOrgGen[j].ik << " "
           << vOrgGen[j].il << " " << vOrgGen[j].im << endl << endl;

        interface::displayOnWin(true, str, "normal", mywin);
        str.clear(); ts.string()->clear();

        return(femaleGT);
   }
  //-------------------------------------------------------------------------
  void showOutput4(QVector<aln::s_hap> vT)
  {
       //Restructure vHaploGx for display...

       QVector<aln::s_hap> vSink;

       //Copy vHaploGx into vSink AB AC..
       for(QVector<aln::s_hap>::iterator It = vT.begin()+4; It != vT.begin()+10; ++It){
         copy(It, It+1, back_inserter(vSink));
       }
       //Copy vHaploGx into vSink AA BB..
       for(QVector<aln::s_hap>::iterator It = vT.begin(); It != vT.begin()+4; ++It){
         copy(It, It+1, back_inserter(vSink));
       }
       //Copy vHaploGx into vSink 12 13..
       for(QVector<aln::s_hap>::iterator It = vT.begin()+14; It != vT.begin()+20; ++It){
         copy(It, It+1, back_inserter(vSink));
       }
       //Copy vHaploGx into vSink 33 44..
       for(QVector<aln::s_hap>::iterator It = vT.begin()+10; It != vT.begin()+14; ++It){
         copy(It, It+1, back_inserter(vSink));
       }

      //constructing Table Output 4

      QString str;
      QTextStream ts(&str);
      ts << "    ";
      for(QVector<aln::s_hap>::iterator It = vSink.begin(); It != vSink.begin()+10; It++){
          ts << It->a << It->b << "   ";
      }//for
      ts << "Prob.";
      ts << endl;
      for(int i = 10; i < 20; i++){
          ts << vSink[i].a << vSink[i].b << " ";
          for(int j = 0; j < 10; j++){
              ts << vSink[j].a << vSink[j].b << vSink[i].a << vSink[i].b << " ";
          }//for
          ts << " ";
          ts.setRealNumberPrecision(2); ts << fixed << vSink[i].dprob;
          ts << endl;
      }//for
      //ts << endl;
      ts << "   ";
      for(int k = 0; k < 10; k++){
          ts.setRealNumberPrecision(2); ts << fixed << vSink[k].dprob << " ";
      }
      ts << " Prob." << endl;
      interface::displayOnWin(true, str, "normal", mywin);
      str.clear(); ts.string()->clear();

      vSink.clear();
  }
  //-------------------------------------------------------------------------
   void showOutput6(QString &f_genotype)
   {
      //title of table 6 & 7
      QString str;
      QTextStream ts(&str);
      QChar sigma = QChar(0x3A3);
      ts << "                    Female Genotype" << endl;
      ts << "                  S locus     Z locus" << endl;
      ts << "                " << "i  j  k  l  r  s  t  u   Shared   MODEL I   MODEL II" << endl;
      ts << "Male pollen     ";
      for(int i = 0; i != f_genotype.length(); i++){
          ts << f_genotype.at(i) << "  ";
      }
      ts << " " << sigma << "S" << " " << sigma << "Z" << "    " << "C  "<< "Freq." << "  C  " << "Freq.";
      interface::displayOnWin(true, str, "normal", mywin);
      str.clear(); ts.string()->clear();
   }
  //-------------------------------------------------------------------------
   QVector<aln::s_per>showCompPercent(QVector<aln::s_res> &vRes)
   {
      //compatibility statistics - to be placed after table 6 7
      QString str;
      QTextStream ts(&str);

      float sum1 = 0.0;
      float sum2 = 0.0;

      for(QVector<aln::s_res>::iterator It = vRes.begin(); It != vRes.end(); It++){
          sum1 += It->freq1;
          sum2 += It->freq2;
      }//for
      ts << "                           Compatibility Frequency:  ";
      ts << qSetFieldWidth(5) << right;
      ts.setRealNumberPrecision(3);
      ts << fixed << aln::round(sum1, 3);
      ts << qSetFieldWidth(10) << right;
      ts << aln::round(sum2, 3) << endl;

      ts << "                           Compatibility Percentage: ";
      ts << qSetFieldWidth(5) << right;
      ts.setRealNumberPrecision(1);
      ts << fixed << (sum1 * 100.0);
      ts << qSetFieldWidth(10) << right;
      ts.setRealNumberPrecision(1);
      ts << fixed << (sum2 * 100.0) << endl;

      aln::s_per compercent;
      compercent.cpmod1 = (sum1 * 100.0);
      compercent.cpmod2 = (sum2 * 100.0);
      vPercent.push_back(compercent);

      interface::displayOnWin(false, str, "normal", mywin);
      str.clear(); ts.string()->clear();

      //reset vRes for multiple runs
      vRes.clear();
      return(vPercent);
   }
  //-------------------------------------------------------------------------
  void checkedi(QTextCursor inmywin)
  {
      mywin = inmywin;
  }
  //-------------------------------------------------------------------------
  void transdata(QVector<aln::s_inp> &vTransInp, QVector<aln::s_gen> &vTransGen)
  {
      //output table 1
      vOrgInp = vTransInp;
      vOrgGen = vTransGen;
      QChar alpha = QChar(0x3B1);
      QChar beta = QChar(0x3B2);
      QChar delta = QChar(0x394);

      QString str;
      QTextStream ts(&str);
      ts << "Meiosis Behavior" << endl << "For S\tFor Z" << endl;
      ts << qSetFieldWidth(1) << left; ts.setRealNumberPrecision(2); ts << fixed;
      ts << alpha << ":  " << vOrgInp[0].da << "\t" << alpha << ":  " << vOrgInp[1].da << endl
         << beta << ":  " << vOrgInp[0].ib << "\t" << beta << ":  " << vOrgInp[1].ib << endl
         << delta << "1: " << vOrgInp[0].id1 << "\t" << delta << "1: " << vOrgInp[1].id1 << endl
         << delta << "2: " << vOrgInp[0].id2 << "\t" << delta << "2: " << vOrgInp[1].id2 << endl
         << delta << "3: " << vOrgInp[0].id3 << "\t" << delta << "3: " << vOrgInp[1].id3 << endl
         <<  endl << vOrgInp[0].state << endl << vOrgInp[1].state << endl;

      interface::displayOnWin(true, str, "normal", mywin);
      str.clear(); ts.string()->clear();

      ts << qSetFieldWidth(0);
      ts << "            j k l m j k l m" << endl;
      ts << "Genotype 1: ";
      ts << vOrgGen[0].cj << " " << vOrgGen[0].ck << " " << vOrgGen[0].cl << " "
         << vOrgGen[0].cm << " " << vOrgGen[0].ij << " " << vOrgGen[0].ik << " "
         << vOrgGen[0].il << " " << vOrgGen[0].im << endl;

      ts << "Genotype 2: ";
      ts << vOrgGen[1].cj << " " << vOrgGen[1].ck << " " << vOrgGen[1].cl << " "
         << vOrgGen[1].cm << " " << vOrgGen[1].ij << " " << vOrgGen[1].ik << " "
         << vOrgGen[1].il << " " << vOrgGen[1].im << endl;

      interface::displayOnWin(true, str, "normal", mywin);
      str.clear(); ts.string()->clear();

      theEditor->appendInEditor();
  }
  //-------------------------------------------------------------------------
  bool phaplotypes(QTextCursor inmywin)
  {
      //calculate haplotypes
      //uses buildhaplo showhaplotypes
       mywin = inmywin;
       bool ok = true;
       if(vOrgGen.size() > 0 && vOrgInp.size() > 0){
          QString str;
          QTextStream ts(&str);
          ts << "\nOutput 1: Pollen Haplotypes" << endl;
          interface::displayOnWin(true, str, "bold", mywin);
          str.clear(); ts.string()->clear();

          //calculate haplotypes for genotype 1 & 2
          int gt_nr = 1;
          vHaploG1 = aln::buildhaplo(vOrgGen, vOrgInp, gt_nr);
          gt_nr = 2;
          vHaploG2 = aln::buildhaplo(vOrgGen, vOrgInp, gt_nr);

          //display
          interface::showHaplotypes(vHaploG1, 1);
          interface::showHaplotypes(vHaploG2, 2);
          ok = true;
       }//if
       else {
          ok = false; // No input
       }//else
       return(ok);
  }
//-------------------------------------------------------------------------
   bool recipcross1(QTextCursor inmywin)
   {
       //generate Table Output 6
       //uses buildOutput6, showCompPercent, builds vPercent & vRes
       mywin = inmywin;
       bool ok = true;
       if(vOrgGen.size() > 0 && vOrgInp.size() > 0){
           QString f_genotype;
           f_genotype = interface::showOutput2(vOrgGen, 1);
           aln::checkrepallel(f_genotype);

           interface::showOutput4(vHaploG1);
           interface::showOutput6(f_genotype);
           aln::buildOutput6(vHaploG1, f_genotype, mywin, vRes);
           vPercent = interface::showCompPercent(vRes);

           f_genotype = showOutput2(vOrgGen, 2);
           aln::checkrepallel(f_genotype);

           interface::showOutput4(vHaploG2);
           interface::showOutput6(f_genotype);
           aln::buildOutput6(vHaploG2, f_genotype, mywin, vRes);
           vPercent = interface::showCompPercent(vRes);
           ok = true;
       }//if
       else {
          ok = false; // no input
       }//else
       return (ok);
  }
  //-------------------------------------------------------------------------
  QVector<aln::s_inp> getVecOrgInp(void){

      return(vOrgInp);
  }
  //-------------------------------------------------------------------------
  QVector<aln::s_gen> getVecOrgGen(void){
      return(vOrgGen);
  }
  //-------------------------------------------------------------------------
  void resetVecs(void){
      vOrgInp.clear();
      vOrgGen.clear();
  }
  //-------------------------------------------------------------------------
  void collectRes(){

      QString str;
      QTextStream ts(&str);

      //make genotype1 and genotype2 from vOrgGen
      QString g1, g2;
      ts << vOrgGen[0].cj << vOrgGen[0].ck << vOrgGen[0].cl << vOrgGen[0].cm
          << vOrgGen[0].ij << vOrgGen[0].ik << vOrgGen[0].il << vOrgGen[0].im;
      g1 = str;
      str.clear(); ts.string()->clear();
      ts << vOrgGen[1].cj << vOrgGen[1].ck << vOrgGen[1].cl << vOrgGen[1].cm
          << vOrgGen[1].ij << vOrgGen[1].ik << vOrgGen[1].il << vOrgGen[1].im;
      g2 = str;
      str.clear(); ts.string()->clear();

      //calculate defferences in S and Z
      int difs = 0;
      int difz = 0;
      for (int x = 0; x < g1.length(); x++){
          if (x < 4){
              if (g1[x] != g2[x])
                  difs++;
          }//if
          if (x > 3){
              if (g1[x] != g2[x])
                  difz++;
          }//if
      }//for

      // copy data into vCompatTab for later display
      aln::s_tab comtab;
      comtab.sa = vOrgInp[0].da;
      comtab.sb = vOrgInp[0].ib;
      comtab.sd1 = vOrgInp[0].id1;
      comtab.sd2 = vOrgInp[0].id2;
      comtab.sd3 = vOrgInp[0].id3;
      comtab.za = vOrgInp[1].da;
      comtab.zb = vOrgInp[1].ib;
      comtab.zd1 = vOrgInp[1].id1;
      comtab.zd2 = vOrgInp[1].id2;
      comtab.zd3 = vOrgInp[1].id3;
      comtab.g1 = g1;
      comtab.g2 = g2;
      comtab.sdif = difs;
      comtab.zdif = difz;
      comtab.cpg1m1 = vPercent[0].cpmod1;
      comtab.cpg1m2 = vPercent[0].cpmod2;
      comtab.cpg2m1 = vPercent[1].cpmod1;
      comtab.cpg2m2 = vPercent[1].cpmod2;
      vCompatTab.push_back(comtab);

      //reset vPercent here
      vPercent.clear();
  }
  //-------------------------------------------------------------------------
  void showResTable(){
      QString str;
      QTextStream ts(&str);

      QChar alpha = QChar(0x3B1);
      QChar beta = QChar(0x3B2);
      QChar delta = QChar(0x394);

      ts << "Output 3: Result table" << endl;
      interface::displayOnWin(true, str, "bold", mywin);
      str.clear(); ts.string()->clear();

      ts << "      Meiosis Behavior                 Genotype             Compatibility" << endl;
      ts << " S locus           Z locus                            G diff.    [%]" << endl;
      ts << " " << alpha << "    " << beta << " " << delta << "1 " << delta << "2 " << delta << "3  ";
      ts << " " << alpha << "    " << beta << " " << delta << "1 " << delta << "2 " << delta << "3";
      ts << "     G1       G2     S  Z Model1 Model2" << endl;

      for (int x = 0; x < vCompatTab.size(); x++){
          ts << qSetFieldWidth(4) << right;
          ts.setRealNumberPrecision(2); ts  << fixed << vCompatTab[x].sa;
          ts << qSetFieldWidth(1);
          ts << "  " << vCompatTab[x].sb << "  " << vCompatTab[x].sd1 << "  " << vCompatTab[x].sd2 << "  " << vCompatTab[x].sd3 << "  ";
          ts << qSetFieldWidth(4) << right;
          ts.setRealNumberPrecision(2); ts  << fixed << vCompatTab[x].za;
          ts << qSetFieldWidth(1);
          ts << "  " << vCompatTab[x].zb << "  " << vCompatTab[x].zd1 << "  " << vCompatTab[x].zd2 << "  " << vCompatTab[x].zd3 << "  ";

          ts << qSetFieldWidth(1) << right;
          ts << vCompatTab[x].g1 << " " << vCompatTab[x].g2 << "  "
             << vCompatTab[x].sdif << "  " << vCompatTab[x].zdif << "  ";
          ts << qSetFieldWidth(5) << right;
          ts.setRealNumberPrecision(1); ts  << fixed << vCompatTab[x].cpg1m1;
          ts << qSetFieldWidth(1);
          ts << "  ";
          ts << qSetFieldWidth(5) << right;
          ts.setRealNumberPrecision(1); ts  << fixed << vCompatTab[x].cpg1m2;
          ts << qSetFieldWidth(1);
          ts << endl;

          ts << "                                    ";
          ts << qSetFieldWidth(1) << right;
          ts << vCompatTab[x].g2 << " " << vCompatTab[x].g1 << "  "
             << vCompatTab[x].sdif << "  " << vCompatTab[x].zdif << "  ";
          ts << qSetFieldWidth(5) << right;
          ts.setRealNumberPrecision(1); ts  << fixed << vCompatTab[x].cpg2m1;
          ts << qSetFieldWidth(1);
          ts << "  ";
          ts << qSetFieldWidth(5) << right;
          ts.setRealNumberPrecision(1); ts  << fixed << vCompatTab[x].cpg2m2;
          ts << qSetFieldWidth(1);
          ts << endl << endl;

      }
      interface::displayOnWin(true, str, "normal", mywin);
      str.clear(); ts.string()->clear();

      //reset table
      vCompatTab.clear();
  }
  //-------------------------------------------------------------------------
  //   void checkfile(QTextCursor inmywin, QString alnfile){

  //       mywin = inmywin;

  //       int nrErrors = 0;
  //       QString errorcheck = aln::fileload(vOrgInp, alnfile, nrErrors);
  //       if (errorcheck == "ERROR - Could not open file!"){
  //           interface::displayOnWin(true, errorcheck, "bold", inmywin);
  //           return;
  //       }
  //       interface::displayOnWin(true, errorcheck, "normal", inmywin);

  //       if (nrErrors == 0){
  //           interface::displayOnWin(true, "\nANALYSIS OF Data-FILE", "bold", inmywin);
  //       }
  //   }
    //-------------------------------------------------------------------------

}//namespace

