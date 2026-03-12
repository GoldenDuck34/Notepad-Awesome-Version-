#include <QIcon>
#include <QMainWindow>
#include <QtWidgets/QApplication>
#include "NotepadTest.h"
#include "globals.h"

int main(int argc, char* argv[]) {
  QApplication app(argc, argv);

  NotepadTest window;
  window.setWindowTitle(Globals::windowTitle);
  window.setWindowIcon(QIcon(":/icons/icon.png"));
  window.show();

  return app.exec();
}
