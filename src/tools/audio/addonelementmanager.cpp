#include "addonelementmanager.h"
#include "src/functions.h"

#include <QDebug>
#include <QJsonDocument>
#include <QJsonObject>
#include <QJsonArray>

AddonElementManager::AddonElementManager(QObject *parent) : QObject(parent)
{
    qDebug() << "Loading Addon Element Manager ...";

    findAddons();

    if (m_spotifyFolders.size() > 0) setFolder(m_spotifyFolders[m_spotifyFolders.size() - 1]);
}

void AddonElementManager::addElements()
{
    QList<SpotifyElement *> list;

    for (auto e : m_spotifyElements)
    {
        if (e && e->isExport())
        {
            auto e2 = new SpotifyElement(e->name(), e->id());
            list.append(e2);
        }
    }

    if (list.size() > 0) emit exportElements(list);
}

void AddonElementManager::setFolder(QString folder)
{
    m_currentSpotifyFolder = folder;
    findSpotifyPlaylists();
}

// Get all enabled addons
void AddonElementManager::findAddons()
{
    QStringList paths = { QDir::homePath() + "/.gm-companion/addons", ":/addons" };

    for (QString path : paths)
    {
        for (QString addon : getFolders(path))
        {
            if (!addon.startsWith(".") && sManager.getIsAddonEnabled(addon))
            {
                m_addonPaths.append(path + "/" + addon);
                QString spotifyFile = path + "/" + addon + "/spotify.json";

                if (QFile(spotifyFile).exists()) m_spotifyFiles.append(spotifyFile);
            }
        }
    }

    findSpotifyFolders();
}

// Read all Spotify files and get folders
void AddonElementManager::findSpotifyFolders()
{
    for (QString file : m_spotifyFiles)
    {
        QFile f(file);
        f.open(QIODevice::ReadOnly);
        QString content = f.readAll();
        f.close();

        auto doc     = QJsonDocument::fromJson(content.toLocal8Bit());
        auto root    = doc.object();
        auto folders = root.value("data").toArray();

        for (auto folder : folders)
        {
            QString folderName = folder.toObject().value("folder").toString();

            if (!m_spotifyFolders.contains(folderName)) m_spotifyFolders.append(folderName);
        }
    }
}

void AddonElementManager::setAddElement(int index, bool add)
{
    if (index < m_spotifyElements.size())
    {
        auto e = m_spotifyElements[index];

        if (e)
        {
            e->setExport(add);
        }
    }
}

void AddonElementManager::resetChecked()
{
    for (auto e : m_spotifyElements)
    {
        if (e)
        {
            e->setExport(false);
        }
    }

    emit elementsChanged();
}

// Find Spotify Playlists (and Albums)
void AddonElementManager::findSpotifyPlaylists()
{
    for (auto e : m_spotifyElements) e->deleteLater();

    m_spotifyNames.clear();
    m_spotifyElements.clear();

    if (!m_currentSpotifyFolder.isEmpty())
    {
        for (QString file : m_spotifyFiles)
        {
            QFile f(file);
            f.open(QIODevice::ReadOnly);
            auto doc = QJsonDocument::fromJson(f.readAll());
            f.close();

            auto root    = doc.object();
            auto folders = root.value("data").toArray();

            for (auto folder : folders)
            {
                QString folderName = folder.toObject().value("folder").toString();

                if (folderName == m_currentSpotifyFolder)
                {
                    for (QString value : { "playlists", "albums" })
                    {
                        auto playlists = folder.toObject().value(value).toArray();

                        for (auto playlist : playlists)
                        {
                            auto name = playlist.toObject().value("name").toString();

                            if (!m_spotifyNames.contains(name))
                            {
                                auto uri     = playlist.toObject().value("uri").toString();
                                auto element = new SpotifyElement(name, uri);
                                element->setExport(false);

                                m_spotifyNames.append(name);
                                m_spotifyElements.append(element);
                            }
                        }
                    }
                }
            }
        }
    }

    emit elementsChanged();
}
