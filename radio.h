/**
  This Header-File represents the radio-feature of the project, every event produced
  by a GUI components that belongs to the radio will be handled by this class
*/
#ifndef RADIO_H
#define RADIO_H

#include <QMainWindow>
#include <QtMultimedia/QMediaPlayer>

class Radio: public QMainWindow
{
    Q_OBJECT
public:
    explicit Radio();
    ~Radio();
    void play();
    void stop();
    QString addChannel();
    void setCurrentChannel(QString curr);
    void setVolume(int value);

    //*********************************************************************
    //************************Radio genres*********************************
    //*********************************************************************
    void rock();
    void techno();
    void pop();
    void jazz();
    void top20();
    void country();
    void classic();
    void hiphop();
    void punk();

private:
    QMediaPlayer *player;
    bool isPlaying;
    int streamState;
};

#endif // RADIO_H
