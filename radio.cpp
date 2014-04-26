#include "radio.h"
#include <QDebug>
#include <QtMultimedia/QMediaPlayer>
#include <QUrl>
#include <QFile>
#include <QInputDialog>
#include <QLineEdit>

Radio::Radio()
{
    player = new QMediaPlayer(this);
}

Radio::~Radio()
{
    delete this;
}

void Radio::play()
{
    //QUrl url("http://idobiradio.idobi.com/;stream.mp3");
    player->setVolume(50);
    //player->setMedia(url);
    player->play();
    isPlaying = true;
}

void Radio::stop()
{
    player->stop();
    isPlaying = false;
}

QString Radio::addChannel()
{
    QString filename="C:\\RadioList.pls";
    QString text;
    QFile file( filename );
    bool ok;
    if ( file.open(QIODevice::Append | QIODevice::Text) )
    {
        QTextStream stream( &file );
        text = QInputDialog::getText(this, tr("Internet Radio"),
                                                 tr("Url:"), QLineEdit::Normal,
                                                 "", &ok);
        if (ok && !text.isEmpty()){
            stream << text << endl;
        }
    }
    return text;
}

void Radio::setCurrentChannel(QString curr)
{
    QUrl url(curr);
    player->setMedia(url);
    play();
}

void Radio::setVolume(int value){
    player->setVolume(value);
}

void Radio::rock()
{
    if(player->mediaStatus() != QMediaPlayer::NoMedia)
    {
        QUrl url("http://idobiradio.idobi.com/;stream.mp3");
        player->setMedia(url);
        play();
    }else{
        stop();
        QUrl url("http://idobiradio.idobi.com/;stream.mp3");
        player->setMedia(url);
        play();
    }
}

void Radio::techno()
{
    if(player->mediaStatus() != QMediaPlayer::NoMedia)
    {
        QUrl url("http://listen.technobase.fm/tunein-mp3-pls");
        player->setMedia(url);
        play();
    }else{
        stop();
        QUrl url("http://listen.technobase.fm/tunein-mp3-pls");
        player->setMedia(url);
        play();
    }
}

void Radio::pop()
{
    if(player->mediaStatus() != QMediaPlayer::NoMedia)
    {
        QUrl url("http://74.86.186.4:12110");
        player->setMedia(url);
        play();
    }else{
        stop();
        QUrl url("http://74.86.186.4:12110");
        player->setMedia(url);
        play();
    }
}

void Radio::jazz()
{
    if(player->mediaStatus() != QMediaPlayer::NoMedia)
    {
        QUrl url("http://pub4.jazzradio.com/jr_cooljazz");
        player->setMedia(url);
        play();
    }else{
        stop();
        QUrl url("http://pub4.jazzradio.com/jr_cooljazz");
        player->setMedia(url);
        play();
    }
}

void Radio::top20()
{
    if(player->mediaStatus() != QMediaPlayer::NoMedia)
    {
        QUrl url("http://80.237.152.83/top100station.mp3");
        player->setMedia(url);
        play();
    }else{
        stop();
        QUrl url("http://80.237.152.83/top100station.mp3");
        player->setMedia(url);
        play();
    }
}

void Radio::country()
{
    if(player->mediaStatus() != QMediaPlayer::NoMedia)
    {
        QUrl url("http://108.61.73.117:8056");
        player->setMedia(url);
        play();
    }else{
        stop();
        QUrl url("http://108.61.73.117:8056");
        player->setMedia(url);
        play();
    }
}

void Radio::classic()
{
    if(player->mediaStatus() != QMediaPlayer::NoMedia)
    {
        QUrl url("http://sc-baroque.1.fm:8045");
        player->setMedia(url);
        play();
    }else{
        stop();
        QUrl url("http://sc-baroque.1.fm:8045");
        player->setMedia(url);
        play();
    }
}

void Radio::hiphop()
{
    if(player->mediaStatus() != QMediaPlayer::NoMedia)
    {
        QUrl url("http://108.61.73.118:8054");
        player->setMedia(url);
        play();
    }else{
        stop();
        QUrl url("http://108.61.73.118:8054");
        player->setMedia(url);
        play();
    }
}

void Radio::punk()
{
    if(player->mediaStatus() != QMediaPlayer::NoMedia)
    {
        QUrl url("http://79.141.174.220:26000");
        player->setMedia(url);
        play();
    }else{
        stop();
        QUrl url("http://79.141.174.220:26000");
        player->setMedia(url);
        play();
    }
}
