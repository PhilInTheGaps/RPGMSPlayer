#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QDebug>
#include <QMediaPlayer>
#include <QMediaPlayerControl>
#include <QMediaPlaylist>
#include <QUrl>
#include <QSignalMapper>
#include <QTreeWidget>
#include <QTableWidget>
#include <QHBoxLayout>
#include <QLabel>
#include <QScrollArea>
#include <QNetworkReply>
#include <QTimer>
#include <QTabWidget>
#include <QString>
#include <QFileSystemWatcher>

#include "managers/settingsmanager.h"
#include "functions.h"
#include "ui/flowlayout.h"

// Includes QtWinExtras if system is Windows
#ifdef Q_OS_WIN
#include <QtWinExtras>
#endif

struct Unit{
    QString name;
    double refUnits;
};

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();
    void addToPlaylist(QUrl url, bool music); // Add audio file to playlist. Bool true if music, false if sound

    // Version
    void setVersion(QString); // Set Version Number (String)
    QString getVersion(); // Get Version Number (String), e.g. 1.3.1.0
    int getVersionNumber(); // Get Version Number (int), e.g. 1310
    void updateSettingsVersion(); // Sets version in settings to current version

    // Tools
    void addTools();

private slots:
    // Options
    void on_actionOptions_triggered(); // OptionsDialog
    void on_actionSet_Music_Folder_triggered(); // Music
    void on_actionSet_Sound_Folder_triggered(); // Sound
    void on_actionSet_Maps_Folder_triggered(); // Maps
    void on_actionSet_Resources_Folder_triggered(); // Resources
    void on_actionSet_Characters_Folder_triggered(); // Characters
    void on_actionCheck_for_Updates_triggered(); // Check Updates
    void on_actionSet_Notes_Folder_triggered(); // Notes
    void on_actionSet_Audio_Projects_Folder_triggered();
    void on_actionSet_Radio_Playlists_Folder_triggered();

    // Help
    void on_actionOpen_Wiki_triggered(); // Open Wiki
    void on_actionReport_a_Bug_triggered(); // Report Bug
    void actionI_want_to_use_an_older_Version_triggered(); // Older Version

    // Network
    void on_blogNetworkAccessManagerFinished(QNetworkReply*); // Get Blog Entries

    // Converter
    void on_addUnitButton_clicked();
    void on_converterTypeComboBox_currentIndexChanged(int index);
    void on_length1ComboBox_currentIndexChanged(int index);
    void on_length2ComboBox_currentIndexChanged(int index);
    void on_area1ComboBox_currentIndexChanged(int index);
    void on_area2ComboBox_currentIndexChanged(int index);
    void on_length1SpinBox_valueChanged(double value);
    void on_area1SpinBox_valueChanged(double value);
    void on_volume1ComboBox_currentIndexChanged(int index);
    void on_volume2ComboBox_currentIndexChanged(int index);
    void on_volume1SpinBox_valueChanged(double arg1);
    void on_weight1ComboBox_currentIndexChanged(int index);
    void on_weight2ComboBox_currentIndexChanged(int index);
    void on_weight1SpinBox_valueChanged(double arg1);
    void on_money1ComboBox_currentIndexChanged(int index);
    void on_money2ComboBox_currentIndexChanged(int index);
    void on_money1SpinBox_valueChanged(double arg1);

    // Tools
    void closeTab(int index);

    void on_actionAdd_Audio_Tool_triggered();   // Add AudioTool
    void on_actionToggle_Maps_Tool_triggered(); // Add MapViewerTool
    void on_actionToggle_Dice_Tool_triggered(); // Add DiceTool
    void on_actionToggle_Characters_Tool_triggered();   // Add Character Viewer Tool

    void on_radioButton_allEntries_toggled(bool checked);

    void on_radioButton_releaseOnly_toggled(bool checked);

    void on_actionNotes_triggered();

    void on_pushButton_documentation_clicked();

private:
    Ui::MainWindow *ui;

    bool programStart = true;
    bool onStartUpdateCheck = false;

    int versionNumber;
    QString versionString;

    // Settings
    SettingsManager* settingsManager;
    void writeSettings(QString content, QString indicator, QString newSetting);
    void setFolderLocations(QString indicator);
    QString readSettings();

    // Network
    QNetworkAccessManager *blogNetworkManager;

    // Converter
    QList<Unit> lengthUnits;
    QList<Unit> areaUnits;
    QList<Unit> volumeUnits;
    QList<Unit> weightUnits;
    QList<Unit> moneyUnits;
    void initializeUnits();
    void convertLength(int index1, int index2, double units);
    void convertArea(int index1, int index2, double units);
    void convertVolume(int index1, int index2, double units);
    void convertWeight(int index1, int index2, double units);
    void convertMoney(int index1, int index2, double units);
};

#endif // MAINWINDOW_H
