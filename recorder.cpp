/**
  This class represents the audio recorder feature of the project. All handled signals from the mainframe
  will be processed here.
*/
#include "recorder.h"
#include <QAudioInput>
#include <QFile>
#include <QTimer>
#include <QDebug>
#include <QAudioEncoderSettingsControl>
#include <QAudioRecorder>
#include <QUrl>
#include <QFileDialog>

/**
 * @brief Recorder::Recorder
 * Constructor of the Recorder-class
 */
Recorder::Recorder()
{
    //initializeAudio();
}

/**
 * @brief Recorder::~Recorder
 * Destructor of the Recorder-class
 */
Recorder::~Recorder()
{
    delete this;
}

/**
 * @brief Recorder::initializeAudio
 * Initialize the Recorder and begin to record
 */
void Recorder::initializeAudio()
{
    audioRecorder = new QAudioRecorder;
    audioSettings.setCodec("audio/amr");
    audioRecorder->setEncodingSettings(audioSettings);

    audioRecorder->setOutputLocation(QUrl::fromLocalFile("C:\\test"));
    audioRecorder->record();
}

/**
 * @brief Recorder::start
 * Start the recorder
 */
void Recorder::start()
{
    initializeAudio();
}

/**
 * @brief Recorder::terminateRecording
 * Terminate the current recording
 */
void Recorder::terminateRecording()
{
  audioRecorder->stop();
}

/**
 * @brief Recorder::pauseRecording
 * Pause the current recording
 */
void Recorder::pauseRecording()
{
    audioRecorder->pause();
}

/**
 * @brief Recorder::resumeRecording
 * Resume the current recording
 */
void Recorder::resumeRecording()
{
    audioRecorder->record();
}

/**
 * @brief Recorder::setVolume
 * Set the volume of the current recording
 * @param value
 * The value of the volume to be changed
 */
void Recorder::setVolume(int value)
{
    audioRecorder->setVolume(value);
}

/**
 * @brief Recorder::setQuality
 * Set the sample rate of the recorder
 * @param value
 * The value of the sample rate to be changed
 */
void Recorder::setQuality(int value)
{
    audioSettings.setSampleRate(value);
}

/**
 * @brief Recorder::saveRecorder
 * Save the recording to the root-folder
 */
void Recorder::saveRecorder()
{
     QString fileName = QFileDialog::getSaveFileName();
     audioRecorder->setOutputLocation(QUrl::fromLocalFile(fileName));
     outputLocationSet = true;
}
