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
    // 音乐播放列表。
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
    // 循环随机播放列表中的音乐。
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
    output -> setVolume(float(volume / 100.0));
    qDebug() << player -> playbackState();
}

int BGMPlayer::GetVolume()
{
    return int((output -> volume()) * 100);
}
