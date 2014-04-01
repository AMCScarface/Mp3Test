#include "musicplayer.h"
#include "ui_musicplayer.h"
#include <QtGui>
#include <QtCore>
#include <QString>
#include <QtMultimedia/QMediaPlayer>

MusicPlayer::MusicPlayer(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MusicPlayer)
{
    ui->setupUi(this);
    player = new QMediaPlayer(this);
}

MusicPlayer::~MusicPlayer()
{
    delete ui;
}

void MusicPlayer::on_pB_PlayPause_clicked()
{

    player->setVolume(50);
    player->setMedia(QUrl::fromLocalFile("C:/Users/PlakkaggioHC/Desktop/sample.mp3"));

    qint64 duration = player->duration();

    int seconds = (duration/1000) % 60;
    int minutes = (duration/60000) % 60;
    int hours = (duration/3600000) % 24;

    QTime time(hours, minutes,seconds);
    ui->lbl_totDuration->setText(time.toString());
    player->play();
}

void MusicPlayer::on_pB_Stop_clicked()
{
    player->stop();
}


void MusicPlayer::on_slider_volume_valueChanged(int value)
{
    player->setVolume(value);
}
