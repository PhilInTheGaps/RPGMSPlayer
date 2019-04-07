#ifndef METADATAREADER_H
#define METADATAREADER_H

#include <QObject>
#include <QMediaPlayer>
#include <QImage>
#include <cstdlib>

class MetaData
{
public:
    MetaData() {}
    ~MetaData() {}

    QString artist = "", album = "", title = "", cover = "", type = "", elementIcon = "";
    qulonglong length = 0;
};

class MetaDataReader : public QObject
{
    Q_OBJECT
public:
    explicit MetaDataReader(QObject *parent = nullptr);
    void setMetaData(MetaData m);

private:
    QString convertCoverImage(QMediaPlayer *mediaPlayer);

signals:
    void metaDataUpdated(MetaData m);

public slots:
    void updateMetaData(QMediaPlayer *mediaPlayer, QString elementIcon);
};

#endif // METADATAREADER_H
