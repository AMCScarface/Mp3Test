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
    void test();

};

#endif // RADIO_H
