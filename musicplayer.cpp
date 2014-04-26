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
#include "radio.h"


MusicPlayer::MusicPlayer(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MusicPlayer)
{
    ui->setupUi(this);
    ui->cbx_activePL->addItem("");
    ui->cbx_choosePL->addItem("");
    player = new QMediaPlayer(this);
    connect ( ui->menuBar , SIGNAL (triggered( QAction *) ) , this , SLOT(loadSongs(QAction*) ) );
    ui->pb_Favourite->setIcon(QIcon("C://Qt//Tools//QtCreator//bin//MP3P//fav2.png"));
    ui->pB_PlayPause->setIcon(QIcon("C://Qt//Tools//QtCreator//bin//MP3P//playpause.png"));
    ui->pB_PlayRadio->setIcon(QIcon("C://Qt//Tools//QtCreator//bin//MP3P//playpause.png"));
    ui->pb_StopRadio->setIcon(QIcon("C://Qt//Tools//QtCreator//bin//MP3P//stop.png"));
    ui->pB_Stop->setIcon(QIcon("C://Qt//Tools//QtCreator//bin//MP3P//stop.png"));
    ui->pB_forward->setIcon(QIcon("C://Qt//Tools//QtCreator//bin//MP3P//fwd.png"));
    ui->pB_back->setIcon(QIcon("C://Qt//Tools//QtCreator//bin//MP3P//rwd.png"));
    this->setFixedSize(this->size());
    rad = new Radio();
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


    QStringList nameFilter("*.playlist");
    QDir directory("C:\\");
    QStringList playLists = directory.entryList(nameFilter);
    for(int i = 0; i < playLists.length(); i++)
    {

        ui->cbx_choosePL->addItem(playLists.at(i));
        ui->cbx_activePL->addItem(playLists.at(i));
    }


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

MusicPlayer::~MusicPlayer()
{
    delete ui;
}

void MusicPlayer::on_pB_PlayPause_clicked()
{
    if(player->mediaStatus() != QMediaPlayer::NoMedia)
    {
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

void MusicPlayer::metaDataChanged(){
    ml->setText(player->metaData(QMediaMetaData::Author).toString() + " - " +
                                    player->metaData(QMediaMetaData::Title).toString() + " - " +
                                    player->metaData(QMediaMetaData::AlbumTitle).toString() + " - " +
                                    player->metaData(QMediaMetaData::Genre).toString() + " - " +
                                    player->metaData(QMediaMetaData::Year).toString() + " - " +
                                    player->metaData(QMediaMetaData::AudioBitRate).toString());

}

void MusicPlayer::on_pB_Stop_clicked()
{

        timer->stop();
        currentValue = 0;
        isPlaying = false;
        streamState = 0;
        ui->slider_track->setSliderPosition(0);
        player->stop();
}


void MusicPlayer::on_slider_volume_valueChanged(int value)
{
    player->setVolume(value);
}

void MusicPlayer::update_slider()
{
    ui->slider_track->setSliderPosition(player->position());
    if(player->state() == 0 && player->position() == currentValue){
        on_pB_forward_clicked();
    }
}

void MusicPlayer::on_slider_track_valueChanged(int value)
{
    currentValue = value;
}

void MusicPlayer::durationChanged(qint64 value)
{
    qint64 duration = value;

    int seconds = (duration/1000) % 60;
    int minutes = (duration/60000) % 60;
    int hours = (duration/3600000) % 24;

    QTime time(hours, minutes,seconds);
    ui->lbl_totDuration->setText(time.toString());
    ui->slider_track->setMaximum(duration);
}

void MusicPlayer::loadSongs(QAction * action)
{
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
              //qDebug() << dir.exists("cover.jpg");
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
            if(action->text().toStdString().compare("Exit") == 0)
            {
                QApplication::quit();
            } else {
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

    if(action->text().toStdString().compare("Background Color") == 0){
            QString rg = QColorDialog::getColor(Qt::white,this).name();
            ui->centralWidget->setStyleSheet("background-color: " + rg);
    }

    if(action->text().toStdString().compare("Button Color") == 0){
            QString rg = QColorDialog::getColor(Qt::white,this).name();
            ui->pb_CreateNewPlaylist->setStyleSheet("background-color: " + rg);
            ui->pb_Favourite->setStyleSheet("background-color: " + rg);
            ui->pB_addChannel->setStyleSheet("background-color: " + rg);
    }

    if(action->text().toStdString().compare("Version") == 0){
        QMessageBox msgBox;
        QString str = "<p align='center'>Version:<br>";
        str = str + "<p align='center'>0.1 - Beta\n";
        str = str + "<p align='center'>Changelog\n";
        str = str + "<p align='center'>-MusicPlayer full working\n";
        msgBox.setText(str);
        msgBox.setWindowTitle("Version");
        msgBox.setStandardButtons(QMessageBox::Ok);
        msgBox.setDefaultButton(QMessageBox::Ok);
        int ret = msgBox.exec();
    }
}

void MusicPlayer::on_slider_track_sliderReleased()
{
    player->setPosition(currentValue);
}

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

void MusicPlayer::on_pB_forward_clicked()
{
    on_pB_Stop_clicked();
    isPlaying = false;
    streamState = 1;

    if(currentTitle != ui->songList->count()-1)
    {
        currentTitle = currentTitle + 1;
        if(shuffleOn){
            currentTitle = qrand() % ((ui->songList->count()-2 + 1) - 0) + 0;
        }
        ui->songList->item(currentTitle)->setSelected(true);
        player->setMedia(QUrl::fromLocalFile(ui->songList->item(currentTitle)->text()));
        on_pB_PlayPause_clicked();
    } else {
        currentTitle = 0;
        ui->songList->item(currentTitle)->setSelected(true);
        player->setMedia(QUrl::fromLocalFile(ui->songList->item(currentTitle)->text()));
        on_pB_PlayPause_clicked();
    }
}

void MusicPlayer::on_pB_back_clicked()
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


void MusicPlayer::on_pB_PlayRadio_clicked()
{
    rad->play();
}

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



void MusicPlayer::on_cbx_choosePL_activated(const QString &arg1)
{
    ui->cbx_choosePL->removeItem(0);
    selectedPlyList = arg1;
    if(arg1.compare("Favourite.playlist") == 0)
    {
        if(ui->songList->count() != 0)
        {
            qDebug() << "Drin " +player->isAudioAvailable();
            if(player->isAudioAvailable()){
                on_pB_Stop_clicked();
                player->stop();
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
            if(player->isAudioAvailable())
                on_pB_Stop_clicked();
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


void MusicPlayer::on_rB_asShuffle_clicked()
{
    shuffleOn = true;
}


void MusicPlayer::on_rd_inOrder_clicked()
{
    shuffleOn = false;
}


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

void MusicPlayer::on_songList_clicked(const QModelIndex &index)
{

        selectedSong = index.row();
}

void MusicPlayer::on_cbx_activePL_activated(const QString &arg1)
{
    ui->cbx_activePL->removeItem(0);
    selectedToAdd = arg1;
}

//******************************************************************************************************

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

void MusicPlayer::on_Webbrowser_currentChanged(int index)
{
    if(index != 0)
    {
        ml->hide();
    } else
        ml->show();
}

void MusicPlayer::on_pb_StopRadio_clicked()
{
    rad->stop();
}

void MusicPlayer::on_pB_addChannel_clicked()
{
    QString str = rad->addChannel();
    ui->radio_List->addItem(str);
}

void MusicPlayer::on_radio_List_doubleClicked(const QModelIndex &index)
{
    int curr = index.row();
    //qDebug() << ui->radio_List->item(curr)->text();
    rad->setCurrentChannel(ui->radio_List->item(curr)->text());
}

void MusicPlayer::on_verticalSlider_valueChanged(int value)
{
    rad->setVolume(value);
}

void MusicPlayer::on_pb_RockButton_clicked()
{
    rad->rock();
}

void MusicPlayer::on_pb_TechnoButton_clicked()
{
    rad->techno();
}

void MusicPlayer::on_pb_PopButton_clicked()
{
    rad->pop();
}

void MusicPlayer::on_pb_JazzButton_clicked()
{
    rad->jazz();
}

void MusicPlayer::on_pb_Top20Button_clicked()
{
    rad->top20();
}

void MusicPlayer::on_pb_CountryButton_clicked()
{
    rad->country();
}

void MusicPlayer::on_pb_ClassicButton_clicked()
{
    rad->classic();
}

void MusicPlayer::on_pb_HipHopButton_clicked()
{
    rad->hiphop();
}

void MusicPlayer::on_pb_PunkButton_clicked()
{
    rad->punk();
}
