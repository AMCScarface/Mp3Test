/**
  This class represents the Main-class that will start the execution of the
  project
*/
#include "musicplayer.h"
#include <QApplication>
#include <QLibrary>
#include <QProcess>

int main(int argc, char *argv[])
{

    QApplication a(argc, argv);
    MusicPlayer w;
    w.show();
    return a.exec();
}
