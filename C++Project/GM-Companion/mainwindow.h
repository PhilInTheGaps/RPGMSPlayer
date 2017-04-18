#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QString>
#include <QMediaPlayer>
#include <QMediaPlayerControl>
#include <QMediaPlaylist>
#include <QUrl>
#include <QSignalMapper>
#include <QTreeWidget>
#include <QTableWidget>

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();
    void addToPlaylist(QUrl url);

private slots:
    void playMusic(QString);

    void on_musicPauseButton_clicked();

    void on_musicPlayButton_clicked();

    void on_musicReplayButton_clicked();

    void on_musicNextButton_clicked();

    void on_setMusicFolder_clicked();

    void on_setSoundFolder_clicked();

    void on_setMapsFolder_clicked();

    void on_setResourcesFolder_clicked();

    void on_tableDoubleClicked(int,int);

private:
    Ui::MainWindow *ui;

    QString musicPath;
    QString soundPath;
    QString mapsPath;
    QString resourcesPath;

    QSignalMapper *signalMapper;

    QMediaPlayer *musicPlayer;
    QMediaPlaylist *musicPlaylist;

    QTableWidget *musicTable;
    QTreeWidgetItem *folderItem;

    QMediaPlayer *soundPlayer;
    QMediaPlaylist *soundPlaylist;
};

#endif // MAINWINDOW_H
