#include "BgmPlayer.hpp"
#include <QDebug>

BGMPlayer * BGM_player = new BGMPlayer;

const int BGMPlayer::VolumeMax = 100;
BGMPlayer::BGMPlayer(QObject *parent)
    : QObject{parent}
{
    player = new QMediaPlayer(this);
    output = new QAudioOutput(this);
    player -> setSource(QUrl::fromLocalFile("./Resources/Music/404 not found.flac"));
    player -> setAudioOutput(output);
    output -> setVolume(0);
    player -> play();
}

BGMPlayer::~BGMPlayer()
{
    qDebug() << "BGM_Player released.";
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
    output -> setVolume(float(volume / 100.0));
    qDebug() << output -> volume();
}

int BGMPlayer::GetVolume()
{
    return int((output -> volume()) * 100);
}
