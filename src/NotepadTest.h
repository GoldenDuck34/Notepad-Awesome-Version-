#pragma once

#include <QtWidgets/QMainWindow>
#include "ui_NotepadTest.h"

class NotepadTest : public QMainWindow {
  Q_OBJECT

 public:
  NotepadTest(QWidget* parent = nullptr);
  ~NotepadTest();

 protected:
  void closeEvent(QCloseEvent* event) override;

 private slots:
  void on_actionOpen_triggered();
  void on_actionSave_triggered();
  void on_actionSave_as_triggered();
  void on_actionClose_triggered();
  void on_actionRename_triggered();
  void on_actionDelete_2_triggered();
  void on_actionOpen_File_Location_triggered();
  void on_actionExit_triggered();
  void on_actionUndo_triggered();
  void on_actionRedo_triggered();
  void on_actionCut_triggered();
  void on_actionCopy_triggered();
  void on_actionPaste_triggered();
  void on_actionDelete_triggered();
  void on_actionSelect_All_triggered();
  void on_actionTime_Date_triggered();
  void on_actionEdit_triggered();
  void onTextChanged();

 private:
  Ui::NotepadTestClass ui;
};
