#include "mediaplayermodel.h"

#include <QMediaPlayer>
#include <QTemporaryFile>

MediaPlayerModel::MediaPlayerModel(QObject *parent) :
    QMediaPlayer(parent) {
    //this->tempFile = new QTemporaryFile();
    this->visible = false;
}

MediaPlayerModel::~MediaPlayerModel() {
    //delete this->tempFile;
}

QObject *MediaPlayerModel::mediaPlayer() {
    return static_cast<QObject *>(this);
}

bool MediaPlayerModel::isVisible() {
    return this->visible;
}

void MediaPlayerModel::setVisible(bool visible) {
    this->visible = visible;
    emit visibilityChanged();
}
