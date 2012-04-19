#ifndef alnfile_H
#define alnfile_H

#include <QString>
#include <QTextCursor>

namespace aln{

     struct s_inp {
        float da;
        int ib;
        int id1;
        int id2;
        int id3;
        QString state;
     };

     struct s_gen {
        QString cj;
        QString ck;
        QString cl;
        QString cm;
        int ij;
        int ik;
        int il;
        int im;
     };

     struct s_hap {
         QString a;
         QString b;
         float dfreq;
         float dprob;
     };

     struct s_res {
         int sS;
         int sZ;
         bool com1;
         bool com2;
         float freq1;
         float freq2;
     };

     struct s_per {
         float cpmod1;
         float cpmod2;
     };

     struct s_tab {
         float sa;
         int sb;
         int sd1;
         int sd2;
         int sd3;
         float za;
         int zb;
         int zd1;
         int zd2;
         int zd3;
         QString g1;
         QString g2;
         int sdif;
         int zdif;
         float cpg1m1;
         float cpg1m2;
         float cpg2m1;
         float cpg2m2;
     };

     float round(float, int);
     QVector<aln::s_hap> buildhaplo(QVector<s_gen> &, QVector<s_inp> &, int);
     QString fileload(QVector<s_inp> &, QString, int &);
     void checkrepallel(QString &);
     QString makeSZ(QString &, QString &, int &, int &);
     QString makeFreq(int &, int &, float &, QVector<s_res> &);
     QString buildLine(QVector<aln::s_hap>, QString &, int &, int &, QVector<s_res> &);
     void buildOutput6(QVector<aln::s_hap>, QString &, QTextCursor &, QVector<s_res> &);
}
#endif


