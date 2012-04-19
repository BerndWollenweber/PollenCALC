
#include "alnfile.h"
#include "show.h"

#include <iostream>
#include <fstream>
#include <QTextStream>
#include <QString>
#include <QFile>
#include <QVector>
#include <math.h>

using namespace std;

namespace aln{

  float round(float zahl, int stellen){

     float order = pow(10.0, stellen);

     return (int) (zahl*order+(zahl > 0? 0.5: -0.5))/order;
   }
  //---------------------------------------------------------------------------
  QVector<aln::s_hap> buildhaplo(QVector<s_gen> &vOrgGen, QVector<s_inp> &vOrgInp, int gtnr)
  {

      double freq1 = 1.0 / 4.0;
      double freq2 = 1.0 / 6.0;

      // S parameters from vOrgInp
      int gnr = 0;//using S - stored as [1] in vOrgInp
      double Sp_jj = (freq1 * vOrgInp[gnr].da);
      double Sp_kk = Sp_jj;
      double Sp_ll = Sp_jj;
      double Sp_mm = Sp_jj;

      double pp1 = ((freq2 * vOrgInp[gnr].ib) - (freq2 * vOrgInp[gnr].da));
      double pp2 = (1 - vOrgInp[gnr].ib);

      double Sp_jk = pp1 + (pp2 * ((0.25 * vOrgInp[gnr].id2) + (0.25 * vOrgInp[gnr].id3)));
      double Sp_jl = pp1 + (pp2 * ((0.25 * vOrgInp[gnr].id1) + (0.25 * vOrgInp[gnr].id3)));
      double Sp_jm = pp1 + (pp2 * ((0.25 * vOrgInp[gnr].id1) + (0.25 * vOrgInp[gnr].id2)));
      double Sp_kl = pp1 + (pp2 * ((0.25 * vOrgInp[gnr].id1) + (0.25 * vOrgInp[gnr].id2)));
      double Sp_km = pp1 + (pp2 * ((0.25 * vOrgInp[gnr].id1) + (0.25 * vOrgInp[gnr].id3)));
      double Sp_lm = pp1 + (pp2 * ((0.25 * vOrgInp[gnr].id2) + (0.25 * vOrgInp[gnr].id3)));

      //-Z parameters from vOrgInp
      gnr = 1; //using Z - stored as [1] in vOrgInp
      double pZ_jj = (freq1 * vOrgInp[gnr].da);
      double pZ_kk = pZ_jj;
      double pZ_ll = pZ_jj;
      double pZ_mm = pZ_jj;

      pp1 = ((freq2 * vOrgInp[gnr].ib) - (freq2 * vOrgInp[gnr].da));
      pp2 = (1 - vOrgInp[gnr].ib);

      double pZ_jk = pp1 + (pp2 * ((0.25 * vOrgInp[gnr].id2) + (0.25 * vOrgInp[gnr].id3)));
      double pZ_jl = pp1 + (pp2 * ((0.25 * vOrgInp[gnr].id1) + (0.25 * vOrgInp[gnr].id3)));
      double pZ_jm = pp1 + (pp2 * ((0.25 * vOrgInp[gnr].id1) + (0.25 * vOrgInp[gnr].id2)));
      double pZ_kl = pp1 + (pp2 * ((0.25 * vOrgInp[gnr].id1) + (0.25 * vOrgInp[gnr].id2)));
      double pZ_km = pp1 + (pp2 * ((0.25 * vOrgInp[gnr].id1) + (0.25 * vOrgInp[gnr].id3)));
      double pZ_lm = pp1 + (pp2 * ((0.25 * vOrgInp[gnr].id2) + (0.25 * vOrgInp[gnr].id3)));

      QVector<aln::s_hap> vtHaplo;
      aln::s_hap myhap;

      //- S parameters
      gnr = gtnr - 1;//for genotype 1 = [0]

      myhap.a = vOrgGen[gnr].cj;
      myhap.b = vOrgGen[gnr].cj;
      myhap.dfreq = freq1;
      myhap.dprob = Sp_jj;
      vtHaplo.push_back(myhap);

      myhap.a = vOrgGen[gnr].ck;
      myhap.b = vOrgGen[gnr].ck;
      myhap.dfreq = freq1;
      myhap.dprob = Sp_kk;
      vtHaplo.push_back(myhap);

      myhap.a = vOrgGen[gnr].cl;
      myhap.b = vOrgGen[gnr].cl;
      myhap.dfreq = freq1;
      myhap.dprob = Sp_ll;
      vtHaplo.push_back(myhap);

      myhap.a = vOrgGen[gnr].cm;
      myhap.b = vOrgGen[gnr].cm;
      myhap.dfreq = freq1;
      myhap.dprob = Sp_mm;
      vtHaplo.push_back(myhap);

      myhap.a = vOrgGen[gnr].cj;
      myhap.b = vOrgGen[gnr].ck;
      myhap.dfreq = freq2;
      myhap.dprob = Sp_jk;
      vtHaplo.push_back(myhap);

      myhap.a = vOrgGen[gnr].cj;
      myhap.b = vOrgGen[gnr].cl;
      myhap.dfreq = freq2;
      myhap.dprob = Sp_jl;
      vtHaplo.push_back(myhap);

      myhap.a = vOrgGen[gnr].cj;
      myhap.b = vOrgGen[gnr].cm;
      myhap.dfreq = freq2;
      myhap.dprob = Sp_jm;
      vtHaplo.push_back(myhap);

      myhap.a = vOrgGen[gnr].ck;
      myhap.b = vOrgGen[gnr].cl;
      myhap.dfreq = freq2;
      myhap.dprob = Sp_kl;
      vtHaplo.push_back(myhap);

      myhap.a = vOrgGen[gnr].ck;
      myhap.b = vOrgGen[gnr].cm;
      myhap.dfreq = freq2;
      myhap.dprob = Sp_km;
      vtHaplo.push_back(myhap);

      myhap.a = vOrgGen[gnr].cl;
      myhap.b = vOrgGen[gnr].cm;
      myhap.dfreq = freq2;
      myhap.dprob = Sp_lm;
      vtHaplo.push_back(myhap);

      //- Z parameters
      QString s_ij, s_ik, s_il, s_im;
      s_ij = s_ij.setNum(vOrgGen[gnr].ij);
      s_ik = s_ik.setNum(vOrgGen[gnr].ik);
      s_il = s_il.setNum(vOrgGen[gnr].il);
      s_im = s_im.setNum(vOrgGen[gnr].im);

      myhap.a = s_ij;
      myhap.b = s_ij;
      myhap.dfreq = freq1;
      myhap.dprob = pZ_jj;
      vtHaplo.push_back(myhap);

      myhap.a = s_ik;
      myhap.b = s_ik;
      myhap.dfreq = freq1;
      myhap.dprob = pZ_kk;
      vtHaplo.push_back(myhap);

      myhap.a = s_il;
      myhap.b = s_il;
      myhap.dfreq = freq1;
      myhap.dprob = pZ_ll;
      vtHaplo.push_back(myhap);

      myhap.a = s_im;
      myhap.b = s_im;
      myhap.dfreq = freq1;
      myhap.dprob = pZ_mm;
      vtHaplo.push_back(myhap);

      myhap.a = s_ij;
      myhap.b = s_ik;
      myhap.dfreq = freq2;
      myhap.dprob = pZ_jk;
      vtHaplo.push_back(myhap);

      myhap.a = s_ij;
      myhap.b = s_il;
      myhap.dfreq = freq2;
      myhap.dprob = pZ_jl;
      vtHaplo.push_back(myhap);

      myhap.a = s_ij;
      myhap.b = s_im;
      myhap.dfreq = freq2;
      myhap.dprob = pZ_jm;
      vtHaplo.push_back(myhap);

      myhap.a = s_ik;
      myhap.b = s_il;
      myhap.dfreq = freq2;
      myhap.dprob = pZ_kl;
      vtHaplo.push_back(myhap);

      myhap.a = s_ik;
      myhap.b = s_im;
      myhap.dfreq = freq2;
      myhap.dprob = pZ_km;
      vtHaplo.push_back(myhap);

      myhap.a = s_il;
      myhap.b = s_im;
      myhap.dfreq = freq2;
      myhap.dprob = pZ_lm;
      vtHaplo.push_back(myhap);

      return(vtHaplo);
  }

  //---------------------------------------------------------------------------
  QString fileload(QVector<aln::s_inp> &vOrgSeq, QString alnfile, int &nrErrors)
  {

     QString fileName = alnfile;
     QVector <aln::s_inp> NN = vOrgSeq;
     int nn = nrErrors;
     nn++;

    //check file
    QFile file(fileName);

    if (!file.open(QIODevice::ReadOnly)) {
        return("ERROR - Could not open file!");
    }//if

    QString str2;
    QTextStream ss2(&str2);
    ss2 << "Aln File: " << fileName << endl;

    return (str2);
  }
  //---------------------------------------------------------------------------
  void checkrepallel(QString &f_genotype)
  {
    //check for replicate alleles f.x. AABC and convert to X
      //double s
      if (f_genotype[1] == f_genotype[0]){
          f_genotype[1] = 'X';
      }
      if((f_genotype[2] == f_genotype[0]) || (f_genotype[2] == f_genotype[1])){
          f_genotype[2] = 'X';
      }
      if((f_genotype[3] == f_genotype[0])
              || (f_genotype[3] == f_genotype[1])
              || (f_genotype[3] == f_genotype[2])){
          f_genotype[3] = 'X';
      }
      //double Z
      if (f_genotype[5] == f_genotype[4]){
          f_genotype[5] = 'X';
      }
      if((f_genotype[6] == f_genotype[4]) || (f_genotype[6] == f_genotype[5])){
          f_genotype[6] = 'X';
      }
      if((f_genotype[7] == f_genotype[4])
              || (f_genotype[7] == f_genotype[5])
              || (f_genotype[7] == f_genotype[6])){
          f_genotype[7] = 'X';
      }
  }
  //---------------------------------------------------------------------------
  QString makeSZ(QString &f_genotype, QString &qstr, int &nrS, int &nrZ)
  {
      //make statistics of column 2 and 3
      //count nr of occurencies of char in f_genotype in qstr
      //add S and Z
      QString str;
      QTextStream ts(&str);

      for(int x = 0; x < f_genotype.length(); x++){
          QChar fgt = f_genotype[x];
          int nrchar = qstr.count(QRegExp(fgt));
          if (nrchar > 0){
              if (x < 4)
                  nrS = nrS + nrchar;
              else
                  nrZ = nrZ + nrchar;
          }//if
          ts << " " << nrchar << " "; //nr of A, B C...1, 2, 3..
      }//for
      ts << "   " << nrS << "  " << nrZ;

      return(str);
  }
  //-------------------------------------------------------------------------
  QString makeFreq(int &nrS, int &nrZ, float &dprob, QVector<s_res> &vRes)
  {
      //make freq calculations from nrS and nrZ
      QString str;
      QTextStream ts(&str);
      bool com1, com2;
      double freq1 = 0.0;
      double freq2 = 0.0;

      if((nrS < 2) || (nrZ < 2)){
          com1 = true;
          freq1 = dprob;
      }
      else{
          com1 = false;
          freq1 = 0.0;
      }
      ts <<  "  " << com1 << "  "; ts.setRealNumberPrecision(3); ts << fixed << freq1;

      if((nrS == 0) || (nrZ == 0)){
          com2 = true;
          freq2 = dprob;
      }
      else{
          com2 = false;
          freq2 = 0.0;
      }
      ts <<  "  " << com2 << "  "; ts.setRealNumberPrecision(3); ts << fixed << freq2;

      //make vector
      aln::s_res myres;
      myres.sS = nrS;
      myres.sZ = nrZ;
      myres.com1 = com1;
      myres.com2 = com2;
      myres.freq1 = freq1;
      myres.freq2 = freq2;
      vRes.push_back(myres);

      return(str);
  }
  //-------------------------------------------------------------------------
   QString buildLine(QVector<aln::s_hap> vT, QString &f_genotype, int &i, int &j, QVector<s_res> &vRes)
   {
       //build line of data in tab 6
       //uses makeFreq & makeSZ
       QString str;
       QTextStream ts(&str);

       QString qstr;
       QTextStream qts(&qstr);

       int nrS = 0;
       int nrZ = 0;
       QString colum23;
       QString colum45;
       float dprob;

       qts << vT[i].a << vT[i].b << vT[j].a << vT[j].b;
       colum23 = aln::makeSZ(f_genotype, qstr, nrS, nrZ);
       ts << qstr << "  " << vT[i].a << " " << vT[i].b << " " << vT[j].a << " " << vT[j].b << "  " << colum23;

       dprob = (vT[i].dprob * vT[j].dprob);
       colum45 = aln::makeFreq(nrS, nrZ, dprob, vRes);
       ts << "  " << colum45;

      return(str);
   }
   //-------------------------------------------------------------------------
   void buildOutput6(QVector<aln::s_hap> vT, QString &f_genotype, QTextCursor &mywin, QVector<s_res> &vRes)
   {
       //build table 6 & 7 in 4 blocks of data
       //uses buildline
       QString str;
       QTextStream ts(&str);

       QString dataline;
       //first block
       for(int i = 4; i < 10; i++){
           for(int j = 14; j < 20; j++){
               dataline = aln::buildLine(vT, f_genotype, i, j, vRes);
               ts << dataline << endl;
           }//for
       }//for
       ts << endl;

       //2nd block
       for(int i = 4; i < 10; i++){
           for(int j = 10; j < 14; j++){
               dataline = aln::buildLine(vT, f_genotype, i, j, vRes);
               ts << dataline << endl;
           }//for
       }//for
       ts << endl;

       //3rd block
       for(int i = 0; i < 4; i++){
           for(int j = 14; j < 20; j++){
               dataline = aln::buildLine(vT, f_genotype, i, j, vRes);
               ts << dataline << endl;
           }//for
       }//for
       ts << endl;

       //4th block
       for(int i = 0; i < 4; i++){
           for(int j = 10; j < 14; j++){
               dataline = aln::buildLine(vT, f_genotype, i, j, vRes);
               ts << dataline << endl;
           }//for
       }//for
       ts << endl;

       interface::displayOnWin(true, str, "normal", mywin);
       str.clear(); ts.string()->clear();
   }
   //-------------------------------------------------------------------------

} // namespace



