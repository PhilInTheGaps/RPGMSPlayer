#ifndef SETTINGSTOOL_H
#define SETTINGSTOOL_H

#include "settingsmanager.h"

#include <QObject>

class SettingsTool : public QObject
{
    Q_OBJECT
    Q_PROPERTY(int currentLanguageIndex READ currentLanguageIndex NOTIFY currentLanguageIndexChanged)
    Q_PROPERTY(int uiStyleIndex READ uiStyleIndex NOTIFY uiStyleChanged)

public:
    explicit SettingsTool(QObject *parent = nullptr);

    int currentLanguageIndex();
    Q_INVOKABLE QString getPath(QString type);
    Q_INVOKABLE void setPath(QString type, QString path);
    Q_INVOKABLE void setLanguage(QString lang);

    int uiStyleIndex();
    Q_INVOKABLE void setUiStyle(QString style);

signals:
    void audioPathChanged();
    void musicPathChanged();
    void soundPathChanged();
    void radioPathChanged();
    void mapsPathChanged();
    void shopPathChanged();
    void charactersPathChanged();
    void notesPathChanged();
    void resourcesPathChanged();

    void currentLanguageIndexChanged();
    void uiStyleChanged();

private:
    SettingsManager *sManager;
    QStringList languages = {"English", "Deutsch"};
    QStringList styles = {tr("Dark"), tr("Bright")};

};

#endif // SETTINGSTOOL_H
