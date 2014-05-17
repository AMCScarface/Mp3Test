/**
  This class represents the webradio feature of the project. All handled signals from the mainframe
  will be processed here.
*/
#include "radio.h"
#include <QDebug>
#include <QtMultimedia/QMediaPlayer>
#include <QUrl>
#include <QFile>
#include <QInputDialog>
#include <QLineEdit>

/**
 * @brief Radio::Radio
 * Constructor that sets up the Radio-class
 */
Radio::Radio()
{
    player = new QMediaPlayer(this);
}

/**
 * @brief Radio::~Radio
 * Destructor
 */
Radio::~Radio()
{
    delete this;
}

/**
 * @brief Radio::play
 * Begin to stream the currently loaded webradio channel
 */
void Radio::play()
{
    if(player->mediaStatus() != QMediaPlayer::NoMedia)
    {
        //QUrl url("http://idobiradio.idobi.com/;stream.mp3");
        player->setVolume(50);
        //player->setMedia(url);
        player->play();
        isPlaying = true;
    }
}

/**
 * @brief Radio::stop
 * Stop the currently played webchannel
 */
void Radio::stop()
{
    if(player->mediaStatus() != QMediaPlayer::NoMedia)
    {
        player->stop();
        isPlaying = false;
    }
}

/**
 * @brief Radio::addChannel
 * Add a webradio-channel to the radiolist
 * @return
 * The current added channel to be registered into the webradio-list
 */
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

/**
 * @brief Radio::setCurrentChannel
 * Load the current channel to be played
 * @param curr
 * The address of the channel
 */
void Radio::setCurrentChannel(QString curr)
{
    QUrl url(curr);
    player->setMedia(url);
    play();
}

/**
 * @brief Radio::setVolume
 * Set the volume of the web radio player
 * @param value
 * The value of the radio to be changed
 */
void Radio::setVolume(int value){
    player->setVolume(value);
}

/**
 * @brief Radio::rock
 * Select the rock-channel
 */
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

/**
 * @brief Radio::techno
 * Select the techno-channel
 */
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

/**
 * @brief Radio::pop
 * Select the pop-channel
 */
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

/**
 * @brief Radio::jazz
 * Select the jazz-channel
 */
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

/**
 * @brief Radio::top20
 * Select the top20-channel
 */
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

/**
 * @brief Radio::country
 * Select the country-channel
 */
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

/**
 * @brief Radio::classic
 * Select the classic-channel
 */
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

/**
 * @brief Radio::hiphop
 * Select the hiphop-channel
 */
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

/**
 * @brief Radio::punk
 * Select the punk-channel
 */
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
