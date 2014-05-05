#include "recorder.h"
#include <QAudioInput>
#include <QFile>
#include <QTimer>
#include <QDebug>
#include <QAudioEncoderSettingsControl>
#include <QAudioRecorder>
#include <QUrl>
#include <QFileDialog>

Recorder::Recorder()
{
    //initializeAudio();
}

Recorder::~Recorder()
{
    delete this;
}

void Recorder::initializeAudio()
{
    audioRecorder = new QAudioRecorder;
    audioSettings.setCodec("audio/amr");
    audioRecorder->setEncodingSettings(audioSettings);

    audioRecorder->setOutputLocation(QUrl::fromLocalFile("C:\\test"));
    audioRecorder->record();
}

void Recorder::start()
{
    initializeAudio();
}

void Recorder::terminateRecording()
{
  audioRecorder->stop();
}

void Recorder::pauseRecording()
{
    audioRecorder->pause();
}

void Recorder::resumeRecording()
{
    audioRecorder->record();
}

void Recorder::setVolume(int value)
{
    audioRecorder->setVolume(value);
}

void Recorder::setQuality(int value)
{
    audioSettings.setSampleRate(value);
}

void Recorder::saveRecorder()
{
     QString fileName = QFileDialog::getSaveFileName();
     audioRecorder->setOutputLocation(QUrl::fromLocalFile(fileName));
     outputLocationSet = true;
}
