#include "NotepadTest.h"
#include <QCloseEvent>
#include <QDebug>
#include <QDesktopServices>
#include <QFileDialog>
#include <QFont>
#include <QFontDialog>
#include <QInputDialog>
#include <QMessageBox>
#include <QObject>
#include <QProcess>
#include <QString>
#include <QTextEdit>
#include <QVBoxLayout>
#include <QWidget>
#include <fstream>
#include <sstream>
#include <string>
#include "globals.h"
#include "ui_NotepadTest.h"

NotepadTest::NotepadTest(QWidget* parent) : QMainWindow(parent) {
  ui.setupUi(this);
  connect(ui.textEdit, &QTextEdit::textChanged, this,
          &NotepadTest::onTextChanged);
}

NotepadTest::~NotepadTest() {}

std::string readFile(const std::string& filename) {
  std::ifstream file(filename);

  std::stringstream buffer;
  buffer << file.rdbuf();

  return buffer.str();
}

void saveFile(const std::string& filename, const std::string& content) {
  std::ofstream file(filename);
  file << content;
}

void NotepadTest::on_actionOpen_triggered() {
  std::string fileName =
      QFileDialog::getOpenFileName(this, "Open", "",
                                   "Text Files (*.txt);;All Files (*.*)")
          .toStdString();
  std::ifstream fileContent{fileName};

  std::string contentString{readFile(fileName)};
  ui.textEdit->setPlainText(QString::fromStdString(contentString));
  Globals::currentFile = fileName;
}

void NotepadTest::on_actionSave_triggered() {
  std::string textContent{ui.textEdit->toPlainText().toStdString()};
  if (Globals::currentFile != "")
    saveFile(Globals::currentFile, textContent);
  else {
    QString fileName = QFileDialog::getSaveFileName(
        nullptr, "Save As", "", "Text Files (*.txt);;All Files (*)");
    if (fileName.isEmpty())
      return;

    saveFile(fileName.toStdString(), textContent);
    Globals::currentFile = fileName.toStdString();
  }
  Globals::textDirty = false;
  NotepadTest::setWindowTitle(QString::fromStdString(Globals::windowTitle));
}

void NotepadTest::on_actionSave_as_triggered() {
  std::string textContent{ui.textEdit->toPlainText().toStdString()};
  QString fileName = QFileDialog::getSaveFileName(
      nullptr, "Save As", "", "Text Files (*.txt);;All Files (*)");
  if (fileName.isEmpty())
    return;
  saveFile(fileName.toStdString(), textContent);
  Globals::currentFile = fileName.toStdString();
  Globals::textDirty = false;
  NotepadTest::setWindowTitle(QString::fromStdString(Globals::windowTitle));
}

void NotepadTest::on_actionClose_triggered() {
  if (Globals::textDirty) {
    QMessageBox::StandardButton reply;
    reply = QMessageBox::question(
        this, "Unsaved Changes",
        "You have unsaved changes. Do you want to save them?",
        QMessageBox::Save | QMessageBox::Discard | QMessageBox::Cancel);
    if (reply == QMessageBox::Save) {
      on_actionSave_triggered();
    } else if (reply == QMessageBox::Cancel) {
      return;  // Cancel the close operation
    }
  }
  ui.textEdit->clear();
  Globals::currentFile.clear();
  Globals::oldFile = "placeholder";
  NotepadTest::setWindowTitle(QString::fromStdString(Globals::windowTitle));
}

void NotepadTest::on_actionUndo_triggered() {
  ui.textEdit->undo();
}
void NotepadTest::on_actionRedo_triggered() {
  ui.textEdit->redo();
}
void NotepadTest::on_actionCut_triggered() {
  ui.textEdit->cut();
}
void NotepadTest::on_actionCopy_triggered() {
  ui.textEdit->copy();
}
void NotepadTest::on_actionPaste_triggered() {
  ui.textEdit->paste();
}
void NotepadTest::on_actionDelete_triggered() {
  ui.textEdit->textCursor().removeSelectedText();
}

void NotepadTest::on_actionSelect_All_triggered() {
  ui.textEdit->selectAll();
}
void NotepadTest::on_actionTime_Date_triggered() {
  QString currentDateTime = QDateTime::currentDateTime().toString();
  ui.textEdit->insertPlainText(currentDateTime);
}

void NotepadTest::on_actionRename_triggered() {
  QString newFileName =
      QInputDialog::getText(this, "Rename File", "Enter new file name:");

  if (newFileName.isEmpty())
    return;

  QFileInfo fileInfo(QString::fromStdString(Globals::currentFile));
  QString dir = fileInfo.absolutePath();

  // Ensure .txt extension
  if (!newFileName.endsWith(".txt"))
    newFileName += ".txt";

  QString newFilePath = dir + "/" + newFileName;

  if (std::rename(Globals::currentFile.c_str(),
                  newFilePath.toStdString().c_str()) != 0) {
    QMessageBox::critical(this, "Error", "Failed to rename the file.");
    return;
  }

  Globals::currentFile = newFilePath.toStdString();
  setWindowTitle(newFileName);
}

void NotepadTest::on_actionDelete_2_triggered() {
  qDebug("thing");
  if (Globals::currentFile.empty()) {
    QMessageBox::warning(this, "No File", "No file is currently open.");
    return;
  }
  if (std::remove(Globals::currentFile.c_str()) != 0) {
    QMessageBox::critical(this, "Error", "Failed to delete the file.");
    return;
  }
  Globals::currentFile.clear();
  Globals::oldFile = "placeholder";
  ui.textEdit->clear();
  NotepadTest::setWindowTitle(QString::fromStdString(Globals::windowTitle));
}

void NotepadTest::on_actionOpen_File_Location_triggered() {
  if (Globals::currentFile.empty()) {
    QMessageBox::warning(this, "No File", "No file is currently open.");
    return;
  }

  QString filePath =
      QDir::toNativeSeparators(QString::fromStdString(Globals::currentFile));

  QStringList args;
  args << "/select," << filePath;

  QProcess::startDetached("explorer.exe", args);
}

void NotepadTest::on_actionExit_triggered() {
  this->close();
}

void NotepadTest::onTextChanged() {
  if (Globals::currentFile != Globals::oldFile) {
    Globals::oldFile = Globals::currentFile;
    return;  // Don't mark as dirty if the file has changed
  }

  if (ui.textEdit->toPlainText() == "") {
    // If the text is empty, we can consider it not dirty
    Globals::textDirty = false;
    this->setWindowTitle(QString::fromStdString(Globals::windowTitle));
    return;
  }

  Globals::textDirty = true;
  this->setWindowTitle(QString::fromStdString(Globals::windowTitle) + " *");
}

void NotepadTest::closeEvent(QCloseEvent* event) {
  if (Globals::textDirty) {
    QMessageBox::StandardButton reply;
    reply = QMessageBox::question(
        this, "Unsaved Changes",
        "You have unsaved changes. Do you want to save them?",
        QMessageBox::Save | QMessageBox::Discard | QMessageBox::Cancel);

    if (reply == QMessageBox::Save) {
      saveFile(Globals::currentFile, ui.textEdit->toPlainText().toStdString());
      event->accept();
    } else if (reply == QMessageBox::Discard) {
      event->accept();
    } else {
      event->ignore();
    }
  } else {
    event->accept();
  }
}

void NotepadTest::on_actionEdit_triggered() {
  bool ok;
  QFont font = QFontDialog::getFont(&ok, this);
  if (ok) {
    ui.textEdit->setFont(font);
  }
}
