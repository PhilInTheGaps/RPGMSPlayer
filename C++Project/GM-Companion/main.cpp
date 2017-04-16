//#include "mainwindow.h"
#include <QApplication>
#include <QWidget>
#include <QString>
#include <QVector>
#include <QHBoxLayout>
#include <QVBoxLayout>
#include <QGridLayout>
#include <QLabel>
#include <QPushButton>
#include <QDir>
#include <QMediaPlayer>

QStringList getFolders(QString path) {
    //C:\Users\Phil\Google Drive\GM Boys\GM-Companion\Music\Fantasy
    //C:/Users/Phil/Google Drive/GM Boys/GM-Companion/Music/Fantasy

    QDir myDir(path);
    QStringList folders = myDir.entryList();

    return folders;
}

void playMusic(){
    QMediaPlayer *player = new QMediaPlayer;
    player->setMedia(QUrl::fromLocalFile("C:/Users/Phil/Google Drive/GM Boys/GM-Companion/Music/Fantasy/Action/TitanSoul.mp3"));
    player->setVolume(50);
    player->play();
}

int main(int argc, char *argv[])
{
    QApplication app(argc, argv);
//    MainWindow w;
//    w.show();

    //Builds Window
    QWidget *mainWindow = new QWidget;
    mainWindow->setWindowTitle("GM-Companion | C++ Alpha Version");
    mainWindow->setMinimumWidth(1280);
    mainWindow->setMinimumHeight(720);
    mainWindow->show();

    QHBoxLayout *mainHBox = new QHBoxLayout;
    mainWindow->setLayout(mainHBox);

    //Creates Info Box with content
    QVBoxLayout *infoBox = new QVBoxLayout;
    mainHBox->addLayout(infoBox);

    QLabel *musicLabel = new QLabel("Music");
    infoBox->addWidget(musicLabel);

    QHBoxLayout *musicControls = new QHBoxLayout;
    QPushButton *musicPlayButton = new QPushButton("Play");
    musicControls->addWidget(musicPlayButton);
    QPushButton *musicReplayButton = new QPushButton("Replay");
    musicControls->addWidget(musicReplayButton);
    QPushButton *musicNextButton = new QPushButton("Next");
    musicControls->addWidget(musicNextButton);
    infoBox->addLayout(musicControls);

    QLabel *soundLabel = new QLabel("Sound");
    infoBox->addWidget(soundLabel);

    //Grid that contains all the music Buttons
    QGridLayout *buttonGrid = new QGridLayout;
    mainHBox->addLayout(buttonGrid);



    QStringList musicFolders = getFolders("C:/Users/Phil/Google Drive/GM Boys/GM-Companion/Music/Fantasy");
    for (QString s : musicFolders)
    {
        if (!s.contains("."))
        {
            QPushButton *b = new QPushButton(s);
            //QObject::connect(&b, SIGNAL (clicked()), &app, SLOT (playMusic()));
        }
    }

    playMusic();

    return app.exec();
}
