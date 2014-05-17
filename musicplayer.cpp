/**
  This class represents the actual mainframe of the whole project
*/

#include "musicplayer.h"
#include "ui_musicplayer.h"
#include <Qt>
#include <QLayout>
#include <QtGui>
#include <QApplication>
#include <QtCore>
#include <QString>
#include <QtMultimedia/QMediaPlayer>
#include <stdio.h>
#include <iostream>
#include <string>
#include <QFileDialog>
#include <QDebug>
#include <QMediaMetaData>
#include <QStringList>
#include <QFileInfo>
#include <QDir>
#include <QMessageBox>
#include <QIcon>
#include <QInputDialog>
#include <QColorDialog>
#include <QProcess>
#include "radio.h"
#include "recorder.h"


/**
 * @brief MusicPlayer::MusicPlayer
 * Constructor of the mainframe; the whole GUI will be initialiazed here, like other
 * fundamental components for the audio and record processing. The construct will also
 * check if there exists any Favourite or other Playlists.
 * @param parent
 */
MusicPlayer::MusicPlayer(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MusicPlayer)
{
    ui->setupUi(this);
    parentProcess = parent;
    ui->cbx_activePL->addItem("");
    ui->cbx_choosePL->addItem("");
    player = new QMediaPlayer(this);
    connect ( ui->menuBar , SIGNAL (triggered( QAction *) ) , this , SLOT(loadSongs(QAction*) ) );

    //Load some images related to every button:
    ui->pb_Favourite->setIcon(QIcon("C://Qt//Tools//QtCreator//bin//MP3P//fav2.png"));
    ui->pB_PlayPause->setIcon(QIcon("C://Qt//Tools//QtCreator//bin//MP3P//playpause.png"));
    ui->pB_PlayRadio->setIcon(QIcon("C://Qt//Tools//QtCreator//bin//MP3P//playpause.png"));
    ui->pb_StopRadio->setIcon(QIcon("C://Qt//Tools//QtCreator//bin//MP3P//stop.png"));
    ui->pB_Stop->setIcon(QIcon("C://Qt//Tools//QtCreator//bin//MP3P//stop.png"));
    ui->pB_forward->setIcon(QIcon("C://Qt//Tools//QtCreator//bin//MP3P//fwd.png"));
    ui->pB_back->setIcon(QIcon("C://Qt//Tools//QtCreator//bin//MP3P//rwd.png"));
    this->setFixedSize(this->size());

    //Initialize the Radio and Recorder class:
    rad = new Radio();
    rec = new Recorder();
    ui->pb_PauseResumeRecording->setCheckable(true);
    ui->horizontalSlider->setRange(0,int(QMultimedia::VeryHighQuality));

    //Initialize the starting parameter at starting of the application:
    isPlaying = false;
    streamState = 0;
    currentTitle = 0;
    selectedSong = 0;
    shuffleOn = false;

    QFont f( "Arial", 10, QFont::Bold);

    ml = new WidgetMarqueeLabel(this);
    ml->setTextFormat(Qt::RichText);
    ml->setGeometry(170,370,481,21);
    ml->setFrameStyle(QFrame::StyledPanel | QFrame::Sunken);
    ml->setLineWidth(2);
    ml->setFont(f);
    ml->setAutoFillBackground(true);
    QPalette pal = ml->palette();
    pal.setColor(QPalette::Window, QColor(Qt::yellow));
    ml->setPalette(pal);
    layout()->addWidget(ml);

    //Check if there exists a playlist-file in the root-directory
    QStringList nameFilter("*.playlist");
    QDir directory("C:\\");
    QStringList playLists = directory.entryList(nameFilter);
    for(int i = 0; i < playLists.length(); i++)
    {

        ui->cbx_choosePL->addItem(playLists.at(i));
        ui->cbx_activePL->addItem(playLists.at(i));
    }

    //Check if there exists a radiolist-file in the root-directory
    QString filename="C:\\RadioList.pls";
    QFile file( filename );
    if ( file.open(QIODevice::ReadOnly | QIODevice::Text) )
    {
        QTextStream stream( &file );
        while(!stream.atEnd())
        {
            ui->radio_List->addItem(stream.readLine());
        }
        file.close();
        file.resize(0);
    }
}

/**
 * @brief MusicPlayer::~MusicPlayer
 * Destructor of the mainframe
 */
MusicPlayer::~MusicPlayer()
{
    delete ui;
}

/**
 * @brief MusicPlayer::on_pB_PlayPause_clicked
 * Slot that handles the pause/resume service; it checks if the current played
 * song is a favourite and update the favourite-button; furthermore it starts to
 * stream the current selected music-title
 */
void MusicPlayer::on_pB_PlayPause_clicked()
{
    //Check if a media is loaded indeed:
    if(player->mediaStatus() != QMediaPlayer::NoMedia)
    {
        //Check if the current played title is listed in the favourite-playlist:
        QString filename="C:\\Favourite.playlist";
        QFile file( filename );
        if ( file.open(QIODevice::ReadOnly | QIODevice::Text) )
        {
            QTextStream stream( &file );
            //stream << ui->songList->item(currentTitle)->text() << endl;
            bool oldState = ui->pb_Favourite->blockSignals(true);
            bool found = false;
            while(!stream.atEnd()){
                QString line = stream.readLine();
               // qDebug() << line;
               // qDebug() << ui->songList->item(currentTitle)->text();
                if(line.compare(ui->songList->item(currentTitle)->text()) == 0)
                {
                    found = true;
                }
            }
            ui->pb_Favourite->setChecked(found);
            ui->pb_Favourite->blockSignals(oldState);
        }

        //Check if player is playing or in pause state:
        if(!isPlaying)
        {
            if(streamState != 2)
            {
                player->setVolume(50);
               // player->setMedia(QUrl::fromLocalFile("C:/Users/PlakkaggioHC/Desktop/sample.mp3"));
                player->play();
                connect(player, SIGNAL(durationChanged(qint64)), this, SLOT(durationChanged(qint64)));
                connect(player, SIGNAL(metaDataChanged()), this, SLOT(metaDataChanged()));
                timer = new QTimer(this);

                //Update every second the duration of the current played title:
                QObject::connect(timer, SIGNAL(timeout()), this, SLOT(update_slider()));
                timer->start(1000); //time specified in ms
                isPlaying = true;
            }else{
                player->play();
                streamState = 1;
                isPlaying = true;
            }
        } else {
            player->pause();
            isPlaying = false;
            streamState = 2;
        }
    }
}

/**
 * @brief MusicPlayer::metaDataChanged
 * Slot that handles the metadata that changes when a new music title is being loaded and played.
 * The Metadata will then be updated into the related Label.
 */
void MusicPlayer::metaDataChanged(){
    //Author, Title, Album, Genre, Year, Bitrate:
    ml->setText(player->metaData(QMediaMetaData::Author).toString() + " - " +
                                    player->metaData(QMediaMetaData::Title).toString() + " - " +
                                    player->metaData(QMediaMetaData::AlbumTitle).toString() + " - " +
                                    player->metaData(QMediaMetaData::Genre).toString() + " - " +
                                    player->metaData(QMediaMetaData::Year).toString() + " - " +
                                    player->metaData(QMediaMetaData::AudioBitRate).toString());

}
/**
 * @brief MusicPlayer::on_pB_Stop_clicked
 * Slot that stops the current played music title and updated the GUI components
 */
void MusicPlayer::on_pB_Stop_clicked()
{
    if(player->mediaStatus() != QMediaPlayer::NoMedia)
    {
        timer->stop();
        currentValue = 0;
        isPlaying = false;
        streamState = 0;
        //Update the slider:
        ui->slider_track->setSliderPosition(0);
        player->stop();
    }
}

/**
 * @brief MusicPlayer::on_slider_volume_valueChanged
 * Slot that adjust the volume of the player
 * @param value
 * The current volume value catched by the slider-signal
 */
void MusicPlayer::on_slider_volume_valueChanged(int value)
{
    //Set the volume:
    player->setVolume(value);
}

/**
 * @brief MusicPlayer::update_slider
 * Slot that updates the slider when a current title is being played.
 * If the slider reaches its end, the next title will be played
 */
void MusicPlayer::update_slider()
{
    ui->slider_track->setSliderPosition(player->position());
    if(player->state() == 0 && player->position() == currentValue){
        //Play the next title in the list:
        on_pB_forward_clicked();
    }
}

/**
 * @brief MusicPlayer::on_slider_track_valueChanged
 * Set the slider of the current being played title on the given value
 * @param
 * The current value catched by the slider-signal
 */
void MusicPlayer::on_slider_track_valueChanged(int value)
{
    //Set the new duration:
    currentValue = value;
}

/**
 * @brief MusicPlayer::durationChanged
 *  Slot that calculates the duration of the current title being played
 * @param value
 * The duration of the current title being played
 */
void MusicPlayer::durationChanged(qint64 value)
{
    qint64 duration = value;
    //Calculate the duration in hours,minutes and seconds:
    int seconds = (duration/1000) % 60;
    int minutes = (duration/60000) % 60;
    int hours = (duration/3600000) % 24;

    //Label receives the format 00:00:00 [hh:mm:ss]
    QTime time(hours, minutes,seconds);
    ui->lbl_totDuration->setText(time.toString());
    ui->slider_track->setMaximum(duration);
}

/**
 * @brief MusicPlayer::loadSongs
 * This slot not only load the selected songs in a folder, but also heandles other signals
 * produced by the Menutab [Load List, About, Exit, Change Backround/button color, Version, Start Webbrowser]
 * @param action
 * Represents the current menu item selected by the user
 */
void MusicPlayer::loadSongs(QAction * action)
{
    //*************************************************************************************
    //********************Load the selected List by the user*******************************
    //*************************************************************************************
    if(action->text().toStdString().compare("Load List") == 0)
    {
            QFileDialog dialog(this);
            dialog.setDirectory(QDir::homePath());
            dialog.setFileMode(QFileDialog::ExistingFiles);
            dialog.setNameFilter(tr("Files (*.mp3)"));
            QStringList fileNames;
            if (dialog.exec()){
              fileNames = dialog.selectedFiles();
              QFileInfo fi(dialog.directory().filePath(fileNames.at(1)));
              QDir dir (fi.absolutePath());
              //If a cover.jpg exists in the folder, update the album-cover in the GUI
              if(dir.exists("cover.jpg"))
              {
                 //qDebug() << dir.absoluteFilePath("cover.jpg");
                  QPixmap image(dir.absoluteFilePath("cover.jpg"));
                  int w = ui->lbl_AlbumImage->width();
                  int h = ui->lbl_AlbumImage->height();
                  ui->lbl_AlbumImage->setPixmap(image.scaled(w,h,Qt::KeepAspectRatio));
              }
            }

            if(ui->songList->count() != 0)
            {
                if(player->isAudioAvailable())
                    on_pB_Stop_clicked();
                ui->songList->clear();
            }
            QString buf[fileNames.size()];
            for (int i = 0; i < fileNames.size(); ++i)
            {
                 buf[i] = fileNames.at(i);
                // QStringList pieces = buf[i].split( "/" );
                // QString neededWord = pieces.value( pieces.length() - 1 );
                 ui->songList->addItem(buf[i]);
            }
    }else{
        //*************************************************************************************
        //********************Learn more about the app and the autohrs*************************
        //*************************************************************************************
        if(action->text().toStdString().compare("About") == 0)
        {
             QMessageBox msgBox;
             QString str = "<p align='center'>This project was created by: \n";
             str = str + "<p align='center'>Sascha Scatà\n";
             str = str + "<p align='center'>Jan Raphael Schmid Niederkofler\n";
             str = str + "<p align='center'>Unibz © 2013-2014";
             msgBox.setText(str);
             msgBox.setWindowTitle("About");
             msgBox.setStandardButtons(QMessageBox::Ok);
             msgBox.setDefaultButton(QMessageBox::Ok);
             int ret = msgBox.exec();
        }else{
            //*************************************************************************************
            //********************Exit from the application****************************************
            //*************************************************************************************
            if(action->text().toStdString().compare("Exit") == 0)
            {
                QApplication::quit();
            } else {
                //*************************************************************************************
                //********************Add current title to the active playlist*************************
                //*************************************************************************************
                if(action->text().toStdString().compare("Add to active plyList") == 0)
                {
                    QString filename="C:\\" + selectedToAdd;
                    QFile file( filename );
                    if ( file.open(QIODevice::Append | QIODevice::Text) )
                    {
                        QTextStream stream( &file );
                        stream << ui->songList->item(selectedSong)->text() << endl;
                    }
                }
            }
        }

    }

    //*************************************************************************************
    //********************Change Backround color*******************************************
    //*************************************************************************************
    if(action->text().toStdString().compare("Background Color") == 0){
            QString rg = QColorDialog::getColor(Qt::white,this).name();
            ui->centralWidget->setStyleSheet("background-color: " + rg);
    }

    //*************************************************************************************
    //********************Change Button color**********************************************
    //*************************************************************************************
    if(action->text().toStdString().compare("Button Color") == 0){
            QString rg = QColorDialog::getColor(Qt::white,this).name();
            ui->pb_CreateNewPlaylist->setStyleSheet("background-color: " + rg);
            ui->pb_Favourite->setStyleSheet("background-color: " + rg);
            ui->pB_addChannel->setStyleSheet("background-color: " + rg);
    }

    //*************************************************************************************
    //********************Learn more about the Version of the app**************************
    //*************************************************************************************
    if(action->text().toStdString().compare("Version") == 0){
        QMessageBox msgBox;
        QString str = "<p align='center'>Version:<br>";
        str = str + "<p align='center'>0.4 - Beta\n";
        str = str + "<p align='center'>Changelog\n";
        str = str + "<p align='center'>-MusicPlayer full working\n";
        msgBox.setText(str);
        msgBox.setWindowTitle("Version");
        msgBox.setStandardButtons(QMessageBox::Ok);
        msgBox.setDefaultButton(QMessageBox::Ok);
        int ret = msgBox.exec();
    }
    //*************************************************************************************
    //********************Start the webbrowser*********************************************
    //*************************************************************************************
     if(action->text().toStdString().compare("Start Webbrowser") == 0){
         QProcess *process = new QProcess();
         //Start exe. - file of the web browser project
         QString program = "C:/Users/PlakkaggioHC/Desktop/build-Webbrowser-Desktop_Qt_5_2_1_MinGW_32bit-Release/release/Webbrowser.exe";
         process->start(program);

         int exitCode = QProcess::execute(program);
         qDebug("Exit code is: %d.", exitCode);
     }

}

/**
 * @brief MusicPlayer::on_slider_track_sliderReleased
 * Update the slider of the current title being played
 */
void MusicPlayer::on_slider_track_sliderReleased()
{
    player->setPosition(currentValue);
}

/**
 * @brief MusicPlayer::on_songList_doubleClicked
 * Start playing the selected title by double-clicking it in the list.
 * @param index
 * Set the current index according to the selected title
 */
void MusicPlayer::on_songList_doubleClicked(const QModelIndex &index)
{
        if(player->isAudioAvailable())
            on_pB_Stop_clicked();
        isPlaying = false;
        streamState = 0;

        player->setMedia(QUrl::fromLocalFile(index.data().toString()));
        currentTitle = index.row();
        on_pB_PlayPause_clicked();

}

/**
 * @brief MusicPlayer::on_pB_forward_clicked
 * Select the next title in the list, starting from the current played title. At the end of
 * the list, start from the first title in the list.
 */
void MusicPlayer::on_pB_forward_clicked()
{
    if(player->mediaStatus() != QMediaPlayer::NoMedia)
    {
        on_pB_Stop_clicked();
        isPlaying = false;
        streamState = 1;

        if(currentTitle != ui->songList->count()-1)
        {
            currentTitle = currentTitle + 1;
            if(shuffleOn){
                //Handle if the shuffle mode is enabled:
                currentTitle = qrand() % ((ui->songList->count()-2 + 1) - 0) + 0;
            }
            ui->songList->item(currentTitle)->setSelected(true);
            player->setMedia(QUrl::fromLocalFile(ui->songList->item(currentTitle)->text()));
            on_pB_PlayPause_clicked();
        } else {
            //Begin from the first title in the list:
            currentTitle = 0;
            ui->songList->item(currentTitle)->setSelected(true);
            player->setMedia(QUrl::fromLocalFile(ui->songList->item(currentTitle)->text()));
            on_pB_PlayPause_clicked();
        }
    }
}

/**
 * @brief MusicPlayer::on_pB_back_clicked
 * Select the previous title in the list, starting from the current played title. At the beginning of
 * the list, go to the last item of the list.
 * Note: Shuffle does not affect previous-service
 */
void MusicPlayer::on_pB_back_clicked()
{
    if(player->mediaStatus() != QMediaPlayer::NoMedia)
    {
        //player->stop();
        on_pB_Stop_clicked();
        isPlaying = false;
        streamState = 1;
        if(currentTitle != 0)
        {
            currentTitle = currentTitle - 1;
            ui->songList->item(currentTitle)->setSelected(true);
            player->setMedia(QUrl::fromLocalFile(ui->songList->item(currentTitle)->text()));
            on_pB_PlayPause_clicked();
        } else {
            currentTitle = ui->songList->count()-1;
            ui->songList->item(currentTitle)->setSelected(true);
            player->setMedia(QUrl::fromLocalFile(ui->songList->item(currentTitle)->text()));
            on_pB_PlayPause_clicked();
        }
    }
}

/**
 * @brief MusicPlayer::on_pB_PlayRadio_clicked
 * Play the current loaded web radio channel
 */
void MusicPlayer::on_pB_PlayRadio_clicked()
{
    rad->play();
}

/**
 * @brief MusicPlayer::on_pb_Favourite_toggled
 * Favourite/Unfavourite the current title being played in the list. The favourite music titles
 * are saved into a .playlist file in the root folder
 * @param checked
 * Check if the current title being played is in the favourite list.
 */
void MusicPlayer::on_pb_Favourite_toggled(bool checked)
{
    if(checked)
    {
        QString filename="C:\\Favourite.playlist";
        QFile file( filename );
        if ( file.open(QIODevice::Append | QIODevice::Text) )
        {
            QTextStream stream( &file );
            stream << ui->songList->item(currentTitle)->text() << endl;
        }
    }else {
        QString filename="C:\\Favourite.playlist";
        QFile file( filename );
        QStringList updateList;
        if ( file.open(QIODevice::ReadOnly | QIODevice::Text) )
        {
            QTextStream stream( &file );
            while(!stream.atEnd())
            {
                QString currentLine = stream.readLine();
                if(currentLine.compare(ui->songList->item(currentTitle)->text()) != 0)
                {
                    updateList << currentLine;
                }
            }
            file.close();
            file.resize(0);
        }

        if ( file.open(QIODevice::Append | QIODevice::Text) )
        {
            QTextStream stream( &file );
            for(int i = 0; i < updateList.length(); i++){
                 stream << updateList.at(i) << endl;
            }

        }
    }

}


/**
 * @brief MusicPlayer::on_cbx_choosePL_activated
 * Choose the exisiting playlist and load it into the listview
 * @param arg1
 * The current choosed playlist.
 */
void MusicPlayer::on_cbx_choosePL_activated(const QString &arg1)
{
    //ui->cbx_choosePL->removeItem(0);
    selectedPlyList = arg1;
    if(arg1.compare("Favourite.playlist") == 0)
    {
        if(ui->songList->count() != 0)
        {
            //qDebug() << "Drin " +player->isAudioAvailable();
            if(player->isAudioAvailable()){
                on_pB_Stop_clicked();
                player->stop();
                player->setMedia(NULL);
                currentTitle = 0;
            }
            ui->songList->clear();
        }
        QStringList favList;
        QString filename="C:\\Favourite.playlist";
        QFile file( filename );
        if ( file.open(QIODevice::ReadWrite| QIODevice::Text) )
        {
            QTextStream stream( &file );
            while(!stream.atEnd())
            {
                QString line = stream.readLine();
                favList << line;
            }
        }
        for(int i = 0; i < favList.length(); i++)
        {
             ui->songList->addItem(favList.at(i));
        }
    } else {
        if(ui->songList->count() != 0)
        {
            if(player->isAudioAvailable()){
                on_pB_Stop_clicked();
                player->stop();
                player->setMedia(NULL);
                currentTitle = 0;
            }
            ui->songList->clear();
        }
        QStringList favList;
        QString filename="C:\\" + arg1;
        QFile file( filename );
        if ( file.open(QIODevice::ReadWrite| QIODevice::Text) )
        {
            QTextStream stream( &file );
            while(!stream.atEnd())
            {
                QString line = stream.readLine();
                favList << line;
            }
        }
        for(int i = 0; i < favList.length(); i++)
        {
             ui->songList->addItem(favList.at(i));
        }
    }
}

/**
 * @brief MusicPlayer::on_rB_asShuffle_clicked
 * Enable the shuffle option for the player
 */
void MusicPlayer::on_rB_asShuffle_clicked()
{
    shuffleOn = true;
}

/**
 * @brief MusicPlayer::on_rd_inOrder_clicked
 * Enable the in order option for the player
 */
void MusicPlayer::on_rd_inOrder_clicked()
{
    shuffleOn = false;
}

/**
 * @brief MusicPlayer::on_pb_CreateNewPlaylist_clicked
 * Create a new playlist for collecting the users titles
 */
void MusicPlayer::on_pb_CreateNewPlaylist_clicked()
{
    bool exists = false;
    bool ok;
      QString text = QInputDialog::getText(this, tr("Create New Playlist"),
                                           tr("Playlist:"), QLineEdit::Normal,
                                           "",&ok);
      if (ok && !text.isEmpty())
      {
          for(int i = 0; i < ui->cbx_choosePL->count();i++)
          {
                if(ui->cbx_choosePL->itemText(i).toLower().compare(text.toLower()+".playlist") == 0)
                    exists = true;
          }
          if(!exists)
          {
            ui->cbx_choosePL->addItem(text+".playlist");
            ui->cbx_activePL->addItem(text+".playlist");
            QString filename="C:\\" + text + ".playlist";
            QFile file( filename );
            if ( file.open(QIODevice::Append | QIODevice::Text) )
            {
                QTextStream stream( &file );
                stream << "";
            }
          }else{
              QMessageBox msgBox;
              msgBox.setText("Attention!! Playlist already exists !");
              msgBox.exec();
          }
      }
}

/**
 * @brief MusicPlayer::on_songList_clicked
 *  Update the status variable for the current played song according to the list index
 * @param index
 * index of the list
 */
void MusicPlayer::on_songList_clicked(const QModelIndex &index)
{
        selectedSong = index.row();
}

/**
 * @brief MusicPlayer::on_cbx_activePL_activated
 * Select the playlist in which you want to save your titles
 * @param arg1
 * The selected playlist
 */
void MusicPlayer::on_cbx_activePL_activated(const QString &arg1)
{
    selectedToAdd = arg1;
}

//******************************************************************************************************
//******************************************************************************************************
//******************************************************************************************************
//******************************************************************************************************
//******************************************************************************************************

/**
 * @brief MusicPlayer::closeEvent
 * Warn the user if he actually wants to close the application
 * @param event
 * Event reacts on cancel or accepting.
 */
void MusicPlayer::closeEvent (QCloseEvent *event)
{
    QMessageBox::StandardButton resBtn = QMessageBox::question( this, "MusicPlayer",
                                                                tr("Are you sure you want to quit?\n"),
                                                                QMessageBox::No | QMessageBox::Yes,
                                                                QMessageBox::Yes);
    if (resBtn != QMessageBox::Yes) {
        event->ignore();
    } else {
        event->accept();
    }
}

/**
 * @brief MusicPlayer::on_Webbrowser_currentChanged
 * Extra slot that removes the permanent Label bug of the metadata-show
 * @param index
 * Index of the current selected tab
 */
void MusicPlayer::on_Webbrowser_currentChanged(int index)
{
    if(index != 0)
    {
        ml->hide();
    } else
        ml->show();
}

/**
 * @brief MusicPlayer::on_pb_StopRadio_clicked
 * Stop the current stream of the web radio
 */
void MusicPlayer::on_pb_StopRadio_clicked()
{
    rad->stop();
}

/**
 * @brief MusicPlayer::on_pB_addChannel_clicked
 * Add a channel to the web radio list
 */
void MusicPlayer::on_pB_addChannel_clicked()
{
    QString str = rad->addChannel();
    ui->radio_List->addItem(str);
}

/**
 * @brief MusicPlayer::on_radio_List_doubleClicked
 * Begin to stream the current selected web radio channel
 * @param index
 */
void MusicPlayer::on_radio_List_doubleClicked(const QModelIndex &index)
{
    int curr = index.row();
    //qDebug() << ui->radio_List->item(curr)->text();
    rad->setCurrentChannel(ui->radio_List->item(curr)->text());
}

/**
 * @brief MusicPlayer::on_verticalSlider_valueChanged
 * Set the volume of the web radio
 * @param value
 * Value of the volume to be changed
 */
void MusicPlayer::on_verticalSlider_valueChanged(int value)
{
    rad->setVolume(value);
}

/**
 * @brief MusicPlayer::on_pb_RockButton_clicked
 * Select the preinstalled Rock-Channel
 */
void MusicPlayer::on_pb_RockButton_clicked()
{
    rad->rock();
}

/**
 * @brief MusicPlayer::on_pb_TechnoButton_clicked
 * Select the preinstalled Techno-Channel
 */
void MusicPlayer::on_pb_TechnoButton_clicked()
{
    rad->techno();
}

/**
 * @brief MusicPlayer::on_pb_PopButton_clicked
 * Select the preinstalled Pop-Channel
 */
void MusicPlayer::on_pb_PopButton_clicked()
{
    rad->pop();
}

/**
 * @brief MusicPlayer::on_pb_JazzButton_clicked
 *  Select the preinstalled Jazz-Channel
 */
void MusicPlayer::on_pb_JazzButton_clicked()
{
    rad->jazz();
}

/**
 * @brief MusicPlayer::on_pb_Top20Button_clicked
 *  Select the preinstalled Top20-Channel
 */
void MusicPlayer::on_pb_Top20Button_clicked()
{
    rad->top20();
}

/**
 * @brief MusicPlayer::on_pb_CountryButton_clicked
 *  Select the preinstalled Country-Channel
 */
void MusicPlayer::on_pb_CountryButton_clicked()
{
    rad->country();
}

/**
 * @brief MusicPlayer::on_pb_ClassicButton_clicked
 *  Select the preinstalled Classic-Channel
 */
void MusicPlayer::on_pb_ClassicButton_clicked()
{
    rad->classic();
}

/**
 * @brief MusicPlayer::on_pb_HipHopButton_clicked
 *  Select the preinstalled HipHop-Channel
 */
void MusicPlayer::on_pb_HipHopButton_clicked()
{
    rad->hiphop();
}

/**
 * @brief MusicPlayer::on_pb_PunkButton_clicked
 *  Select the preinstalled Punk-Channel
 */
void MusicPlayer::on_pb_PunkButton_clicked()
{
    rad->punk();
}

/**
 * @brief MusicPlayer::on_pb_StartRecord_clicked
 *  Start the recording of an audio trace
 */
void MusicPlayer::on_pb_StartRecord_clicked()
{
    rec->start();
}

/**
 * @brief MusicPlayer::on_pushButtonpb_StopRecord_clicked
 * Terminate the current recording
 */
void MusicPlayer::on_pushButtonpb_StopRecord_clicked()
{
    rec->terminateRecording();
}

/**
 * @brief MusicPlayer::on_pb_PauseResumeRecording_clicked
 * Pause/Resume the current recording
 */
void MusicPlayer::on_pb_PauseResumeRecording_clicked()
{
    if(ui->pb_PauseResumeRecording->isChecked())
        rec->pauseRecording();
    else
        rec->resumeRecording();
}

/**
 * @brief MusicPlayer::on_sld_recordVolume_valueChanged
 * Set the recording volume of the current being recorded audio trace
 * @param value
 * Value of the volume to be changed
 */
void MusicPlayer::on_sld_recordVolume_valueChanged(int value)
{
    rec->setVolume(value);
}

/**
 * @brief MusicPlayer::on_horizontalSlider_valueChanged
 * Set the recording quality of the current being recorded audio trace
 * @param value
 * Value of the quality to be changed
 */
void MusicPlayer::on_horizontalSlider_valueChanged(int value)
{
    rec->setQuality(value);
}
