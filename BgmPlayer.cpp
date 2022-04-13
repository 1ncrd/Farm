#include "BgmPlayer.hpp"
#include <QDebug>

const int BGMPlayer::VolumeMax = 100;
BGMPlayer::BGMPlayer(QObject *parent)
    : QObject{parent}
{
    QMediaPlayer * player = new QMediaPlayer(this);
    QAudioOutput * output = new QAudioOutput(this);
    player -> setSource(QUrl("qrc:/Music/Living Mice.flac"));
    player -> setAudioOutput(output);
    output -> setVolume(0.5);
    player -> play();
}

void BGMPlayer::SetVolume(const int &volume_temp)
{
    // VolumeMax = 100
    if (volume_temp > VolumeMax)
    {
        this -> volume = VolumeMax;
        output -> setVolume(volume / 100.0);
        return;
    }

    this -> volume = volume_temp;
    output -> setVolume(volume / 100.0);
}

BGMPlayer::~BGMPlayer()
{
    qDebug() << "BGM_Player released.";
}
