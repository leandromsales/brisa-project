#ifndef MEDIAPLAYERMODEL_H
#define MEDIAPLAYERMODEL_H

#include <QObject>
#include <QMediaPlayer>

class QTemporaryFile;

class MediaPlayerModel : public QMediaPlayer
{
    Q_OBJECT
    Q_PROPERTY(QObject* mediaObject READ mediaPlayer)
    Q_PROPERTY(bool visibility READ isVisible WRITE setVisible NOTIFY visibilityChanged)
public:
    explicit MediaPlayerModel(QObject *parent = 0);
    ~MediaPlayerModel();
    bool isVisible();
    void setVisible(bool visible);

private:
    QObject *mediaPlayer();
    QTemporaryFile *tempFile;
    bool visible;

signals:
    void visibilityChanged();

public slots:

};

#endif // MEDIAPLAYERMODEL_H
