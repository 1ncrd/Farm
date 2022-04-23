/* A encapsulated class to play the bgm. */

#ifndef BGMPLAYER_H
#define BGMPLAYER_H

#include <QObject>
#include <QAudioOutput>
#include <QMediaPlayer>

class BGMPlayer : public QObject
{
    Q_OBJECT

private:
    unsigned short volume;  // Ranges from 0 to 100
    QMediaPlayer * player;
    QAudioOutput * output;
public:
    static const int VolumeMax;
    BGMPlayer(QObject *parent = nullptr);
    ~BGMPlayer();
    void SetVolume(const int &volume_temp);

signals:

};
extern BGMPlayer * BGM_player;
#endif // BGMPLAYER_H
