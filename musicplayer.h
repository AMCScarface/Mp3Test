#ifndef MUSICPLAYER_H
#define MUSICPLAYER_H

#include <QMainWindow>
#include <QString>
#include <QtMultimedia/QMediaPlayer>
#include "radio.h"
#include "widgetmarqueelabel.h"

namespace Ui {
class MusicPlayer;
}

class MusicPlayer : public QMainWindow
{
    Q_OBJECT

public:
    explicit MusicPlayer(QWidget *parent = 0);
    ~MusicPlayer();
    Ui::MusicPlayer *ui;
private slots:
    void on_pB_PlayPause_clicked();

    void on_pB_Stop_clicked();

    void on_slider_volume_valueChanged(int value);

    void on_slider_track_valueChanged(int value);

    void update_slider();

    void durationChanged(qint64 value);

    void loadSongs(QAction*);

    void on_slider_track_sliderReleased();

    void on_songList_doubleClicked(const QModelIndex &index);

    void on_pB_forward_clicked();

    void on_pB_back_clicked();

    void metaDataChanged();

    void on_pB_PlayRadio_clicked();

    void on_pb_Favourite_toggled(bool checked);

    void on_cbx_choosePL_activated(const QString &arg1);

    void on_rB_asShuffle_clicked();

    void on_rd_inOrder_clicked();

    void on_pb_CreateNewPlaylist_clicked();

    void on_songList_clicked(const QModelIndex &index);

    void on_cbx_activePL_activated(const QString &arg1);

    void closeEvent (QCloseEvent *event);

    void on_Webbrowser_currentChanged(int index);

    void on_pb_StopRadio_clicked();

    void on_pB_addChannel_clicked();

    void on_radio_List_doubleClicked(const QModelIndex &index);

    void on_verticalSlider_valueChanged(int value);

    void on_pb_RockButton_clicked();

    void on_pb_TechnoButton_clicked();

    void on_pb_PopButton_clicked();

    void on_pb_JazzButton_clicked();

    void on_pb_Top20Button_clicked();

    void on_pb_CountryButton_clicked();

    void on_pb_ClassicButton_clicked();

    void on_pb_HipHopButton_clicked();

    void on_pb_PunkButton_clicked();

private:
    WidgetMarqueeLabel *ml;
    int currentSpeed;
    Radio *rad;
    QMediaPlayer *player;
    QTimer *timer;
    int currentValue;
    bool isPlaying;
    int streamState;
    int currentTitle;
    bool shuffleOn;
    QString selectedPlyList;
    QString selectedToAdd;
    int selectedSong;
};

#endif // MUSICPLAYER_H
