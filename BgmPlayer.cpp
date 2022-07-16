#include "BgmPlayer.hpp"
#include <QDebug>
#include <MyRandom.hpp>

BGMPlayer * BGM_player = new BGMPlayer;
const QString BGMPlayer::MusicPath("./Resources/Music/");

const int BGMPlayer::VolumeMax = 100;
BGMPlayer::BGMPlayer(QObject *parent)
    : QObject{parent}
{
    player = new QMediaPlayer(this);
    output = new QAudioOutput(this);
    // Music list.
    music_list << "Wet Hands.mp3"
               << "Beginning.mp3"
               << "Clark.mp3"
               << "Living Mice.mp3"
               << "Sweden.mp3";
    player -> setAudioOutput(output);
    output -> setVolume(0.1);
    int play_num = Random() % 5;
    player -> setSource(QUrl::fromLocalFile(MusicPath + music_list.at(play_num)));
    player -> play();
    // Play music on a loop and pick music at random
    connect(player, QMediaPlayer::playbackStateChanged, this, [ = ]()
    {
        if (player -> playbackState() == QMediaPlayer::StoppedState)
        {
            int play_num = Random() % 5;
            player -> setSource(QUrl::fromLocalFile(MusicPath + music_list.at(play_num)));
            player -> play();
        }
    });
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
    // setVolume() receive the argument from 0 ~ 1.
    output -> setVolume(float(volume / 100.0));
}

int BGMPlayer::GetVolume()
{
    // Get current volume.
    return int((output -> volume()) * 100);
}
