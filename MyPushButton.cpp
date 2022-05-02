#include "MyPushButton.hpp"

MyPushButton::MyPushButton(QWidget *parent)
    : QPushButton{parent}
{
    effect.setSource(QUrl::fromLocalFile("./Resources/Sound/button_click.wav"));
    effect.setLoopCount(1);
    effect.setVolume(0.5);
    connect(this, QPushButton::clicked, &effect, QSoundEffect::play);
}
