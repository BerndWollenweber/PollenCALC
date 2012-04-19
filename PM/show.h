#ifndef SHOW_H_
#define SHOW_H_

#include <QTextCursor>

#include "alnfile.h"

using namespace std;

namespace interface{

  void displayOnWin(bool, QString, QString, QTextCursor);
  void ShowMessage(QString);
  void showHaplotypes(QVector<aln::s_hap>, int);
  QString showOutput2(QVector<aln::s_gen> &, int);
  void showOutput4(QVector<aln::s_hap>);
  void showOutput6(QString &);
  QVector<aln::s_per> showCompPercent(QVector<aln::s_res> &);
  void checkedi(QTextCursor);
  void transdata(QVector<aln::s_inp> &, QVector<aln::s_gen> &);
  bool phaplotypes(QTextCursor);
  bool recipcross1(QTextCursor);
  QVector<aln::s_inp> getVecOrgInp(void);
  QVector<aln::s_gen> getVecOrgGen(void);
  void resetVecs(void);
  void collectRes();
  void showResTable();
//  void checkfile(QTextCursor, QString);
}

#endif /*SHOW_H_*/
