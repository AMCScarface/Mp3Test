#ifndef TESTMUSICPLAYER_H
#define TESTMUSICPLAYER_H

#include <QtTest/QtTest>

class TestMusicPlayer: public QObject
{
    Q_OBJECT
public:
    explicit TestMusicPlayer();
    ~TestMusicPlayer();
private slots:
    void toUpper();
};
#endif // TESTMUSICPLAYER_H



