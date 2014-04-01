#ifndef MUSICPLAYER_H
#define MUSICPLAYER_H

#include <QMainWindow>
#include <QtMultimedia/QMediaPlayer>

namespace Ui {
class MusicPlayer;
}

class MusicPlayer : public QMainWindow
{
    Q_OBJECT

public:
    explicit MusicPlayer(QWidget *parent = 0);
    ~MusicPlayer();

private slots:
    void on_pB_PlayPause_clicked();

    void on_pB_Stop_clicked();

    void on_slider_volume_valueChanged(int value);

private:
    Ui::MusicPlayer *ui;
    QMediaPlayer *player;
};

#endif // MUSICPLAYER_H
