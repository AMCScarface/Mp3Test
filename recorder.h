/**
  This Header-File represents the recorder-feature of the project, every event produced
  by a GUI components that belongs to the recorder will be handled by this class
*/
#ifndef RECORDER_H
#define RECORDER_H

#include <QMainWindow>
#include <QByteArray>
//#include <SoundTouch.h>
#include <qaudioinput.h>
#include<qaudiooutput.h>
#include <QBuffer>
#include <QFile>
#include <QAudioRecorder>

class Recorder : public QObject
{
    Q_OBJECT

public:
    explicit Recorder();
    ~Recorder();
    void initializeAudio();
    void terminateRecording();
    void start();
    void pauseRecording();
    void resumeRecording();
    void setVolume(int value);
    void setQuality(int value);
    void saveRecorder();

    QString codec;
    int sampleRate;


private:
    QAudioRecorder *audioRecorder;
    QFile output;
    QAudioEncoderSettings audioSettings;
    bool outputLocationSet;
};
#endif // RECORDER_H
