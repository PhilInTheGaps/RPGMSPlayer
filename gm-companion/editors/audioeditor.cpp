#include "audioeditor.h"
#include "ui_audioeditor.h"
#include "gm-companion/functions.h"

#include <QDir>
#include <QStyle>
#include <QDebug>
#include <QSettings>

AudioEditor::AudioEditor(QWidget *parent) : QWidget(parent), ui(new Ui::AudioEditor)
{
    ui->setupUi(this);

    previewPlayer = new QMediaPlayer;
    connect(previewPlayer, SIGNAL(positionChanged(qint64)), this, SLOT(previewPlayer_positionChanged(qint64)));

    settingsManager = new SettingsManager;

    isProjectOpen = false;

    getProjects();
}

AudioEditor::~AudioEditor()
{
    delete ui;
}

// Reads all project files in the project directory and adds them to the combo box
void AudioEditor::getProjects()
{
    ui->comboBox_projects->clear();

    for (QString project : getFiles(settingsManager->getSetting(audioPath)))
    {
        if (project.contains(".ini"))
            ui->comboBox_projects->addItem(cleanText(project));
    }
}

// Open project selected in combo box
void AudioEditor::on_pushButton_openProject_clicked()
{
    // Setting project name
    projectName = ui->comboBox_projects->currentText();

    // Loading project
    loadProject();
}

// Load the project to the editor
void AudioEditor::loadProject()
{
    qDebug() << "Loading project" << projectName << "...";

    isProjectOpen = true;

    // Generating music files tree view
    if (!filesAreLoaded)
    {
        loadFolderContentsToTreeView(ui->treeWidget_music, settingsManager->getSetting(musicPath));
        loadFolderContentsToTreeView(ui->treeWidget_sound, settingsManager->getSetting(soundPath));
        loadFolderContentsToTreeView(ui->treeWidget_radio, settingsManager->getSetting(radioPath));
        filesAreLoaded = true;
    }

    // Loading Categories
    loadCategories();
}

// Load a folder structure to a tree view
void AudioEditor::loadFolderContentsToTreeView(QTreeWidget *treeWidget, QString baseFolder)
{
    qDebug() << "Loading files...";

    // Making sure it is empty
    treeWidget->clear();

    // Adding top level folders
    addSubFoldersToTreeItem(NULL, baseFolder, true, treeWidget);

    // Adding top level files
    addFilesToTreeItem(NULL, baseFolder, true, treeWidget);
}

// Adds a child item for every subfolder in the directory
void AudioEditor::addSubFoldersToTreeItem(QTreeWidgetItem *baseItem, QString baseFolder, bool topLevel, QTreeWidget *tree)
{
    for (QString folder : getFolders(baseFolder))
    {
        if (!folder.contains("."))
        {
            QTreeWidgetItem *item = new QTreeWidgetItem(0);
            item->setText(0, folder);
            item->setToolTip(0, folder);
            item->setWhatsThis(0, baseFolder+"/"+folder);
            item->setIcon(0, style()->standardIcon(QStyle::SP_DirIcon));
            item->setChildIndicatorPolicy(QTreeWidgetItem::ShowIndicator);

            if (topLevel)
                tree->addTopLevelItem(item);
            else
                baseItem->addChild(item);
        }
    }
}

// Adds a child item for every file inside the folder
void AudioEditor::addFilesToTreeItem(QTreeWidgetItem *baseItem, QString baseFolder, bool topLevel, QTreeWidget *tree)
{
    QStringList fileFormats = {".mp3", ".wav", ".ogg", ".flac", ".m3u", ".pls"};

    for (QString file : getFiles(baseFolder))
    {
        // Check if file is supported
        bool isFile = false;
        for (QString fileType : fileFormats)
        {
            if (file.endsWith(fileType, Qt::CaseInsensitive))
                isFile = true;
        }

        if (isFile)
        {
            QTreeWidgetItem *item = new QTreeWidgetItem(1);
            item->setText(0, file);
            item->setToolTip(0, file);
            item->setIcon(0, style()->standardIcon(QStyle::SP_FileIcon));

            QString path = baseFolder+"/"+file;
            path = path.replace(settingsManager->getSetting(musicPath), "");
            path = path.replace(settingsManager->getSetting(soundPath), "");
            path = path.replace(settingsManager->getSetting(radioPath), "");
            item->setWhatsThis(0, path);

            if (topLevel)
                tree->addTopLevelItem(item);
            else
                baseItem->addChild(item);
        }
    }
}

// When a music item is expanded, load folder contents
void AudioEditor::on_treeWidget_music_itemExpanded(QTreeWidgetItem *item)
{
    if (item->type() == 0 && item->childCount() == 0)
    {
        addSubFoldersToTreeItem(item, item->whatsThis(0));
        addFilesToTreeItem(item, item->whatsThis(0));
    }
}

// When a sound item is expanded, load folder contents
void AudioEditor::on_treeWidget_sound_itemExpanded(QTreeWidgetItem *item)
{
    if (item->type() == 0 && item->childCount() == 0)
    {
        addSubFoldersToTreeItem(item, item->whatsThis(0));
        addFilesToTreeItem(item, item->whatsThis(0));
    }
}

// Create a new project
void AudioEditor::on_pushButton_newProject_clicked()
{
    projectName = ui->lineEdit_project->text();
    ui->lineEdit_project->clear();
    save();
    getProjects();
    loadProject();
}

// Save project
void AudioEditor::save()
{
    if (!projectName.isNull())
    {
        QSettings settings(settingsManager->getSetting(audioPath)+"/"+projectName+".ini", QSettings::IniFormat);
        settings.setValue("ProjectName", projectName);
    }
}

// Add a category
void AudioEditor::on_pushButton_newCategory_clicked()
{
    if (ui->lineEdit_category->text() != NULL && projectName != NULL)
    {
        QSettings settings(settingsManager->getSetting(audioPath)+"/"+projectName+".ini", QSettings::IniFormat);

        int size = settings.beginReadArray("Categories");
        settings.endArray();

        settings.beginWriteArray("Categories");
        settings.setArrayIndex(size);

        settings.setValue("name", ui->lineEdit_category->text());

        settings.endArray();

        // Add to Tree View
        QTreeWidgetItem *catItem = new QTreeWidgetItem(0);
        catItem->setText(0, ui->lineEdit_category->text());
        catItem->setToolTip(0, ui->lineEdit_category->text());
        catItem->setIcon(0, style()->standardIcon(QStyle::SP_FileDialogStart));

        ui->treeWidget_categories->addTopLevelItem(catItem);
    }

    ui->lineEdit_category->clear();
}

// Loads all categories and their sub-elements
void AudioEditor::loadCategories()
{
    qDebug() << "Loading Categories...";

    // Only execute if a project is loaded
    if (isProjectOpen)
    {
        // Making sure the tree widget is empty
        qDebug() << "Clearing tree view";
        ui->treeWidget_categories->clear();

        // Setting up QSettings
        QSettings settings(settingsManager->getSetting(audioPath)+"/"+projectName+".ini", QSettings::IniFormat);
        int catCount = settings.beginReadArray("Categories");

        // Loading categories
        for (int i = 0; i<catCount; i++)
        {
            settings.setArrayIndex(i);

            QString category = settings.value("name").toString();

            qDebug() << "Loading category: " + category +" ...";

            QTreeWidgetItem *catItem = new QTreeWidgetItem(0);
            catItem->setText(0, category);
            catItem->setToolTip(0, category);
            catItem->setIcon(0, style()->standardIcon(QStyle::SP_FileDialogStart));
            catItem->setChildIndicatorPolicy(QTreeWidgetItem::ShowIndicator);

            // add to tree
            ui->treeWidget_categories->addTopLevelItem(catItem);

            // load scenarios
            loadScenarios(catItem);

            // Expand item if it is the current category
            if (category == currentCategory)
                catItem->setExpanded(true);
        }
        settings.endArray();
    }
}

// Add Scenarios of category
void AudioEditor::loadScenarios(QTreeWidgetItem *catItem)
{
    // Setting up QSettings
    QSettings settings(settingsManager->getSetting(audioPath)+"/"+projectName+".ini", QSettings::IniFormat);
    int scenCount = settings.beginReadArray(catItem->text(0) + "_Scenarios");

    // Loading Scenarios
    for (int i = 0; i<scenCount; i++)
    {
        settings.setArrayIndex(i);

        QString scenario = settings.value("name").toString();
        qDebug() << "   Loading scenario: " + scenario +" ...";

        QTreeWidgetItem *scenItem = new QTreeWidgetItem(1);
        scenItem->setText(0, scenario);
        scenItem->setToolTip(0, scenario);
        scenItem->setIcon(0, style()->standardIcon(QStyle::SP_DirIcon));
        scenItem->setChildIndicatorPolicy(QTreeWidgetItem::ShowIndicator);

        catItem->addChild(scenItem);

        qDebug() << "Current Category:" << currentCategory << "Local:" << catItem->text(0);
        qDebug() << "Current Scenario:" << currentScenario << "Local:" << scenario;

        // Expand Item if it is the current Scenario
        if (catItem->text(0) == currentCategory && scenario == currentScenario)
            scenItem->setExpanded(true);

        // Add music lists, sound lists and radios
        loadElements(scenItem, catItem->text(0), 2); // Music
        loadElements(scenItem, catItem->text(0), 3); // Sound
        loadElements(scenItem, catItem->text(0), 4); // Radio
    }

    settings.endArray();
}

// Add all Elements to Scenario
void AudioEditor::loadElements(QTreeWidgetItem *scenItem, QString category, int type)
{
    QString suffix;
    QIcon icon;

    switch (type) {
    case 2: // Music
        suffix = "_MusicLists";
        icon = QIcon(":/resources/mediaIcons/music.png");
        break;
    case 3: // Sound
        suffix = "_SoundLists";
        icon = QIcon(":/resources/mediaIcons/sound.png");
        break;
    case 4: // Radio
        suffix = "_Radios";
        icon = QIcon(":/resources/mediaIcons/radio.png");
        break;
    default:
        break;
    }

    // Setting up QSettings
    QSettings settings(settingsManager->getSetting(audioPath)+"/"+projectName+".ini", QSettings::IniFormat);
    int count = settings.beginReadArray(category + "_" + scenItem->text(0) + suffix);

    // Loading Elements
    for (int i = 0; i<count; i++)
    {
        settings.setArrayIndex(i);

        QString list = settings.value("name").toString();

        QTreeWidgetItem *item = new QTreeWidgetItem(type);
        item->setText(0, list);
        item->setToolTip(0, list);
        item->setIcon(0, icon);

        scenItem->addChild(item);

        if (scenItem->text(0) == currentScenario && scenItem->parent()->text(0) == currentCategory && list == currentElement)
            ui->treeWidget_categories->setCurrentItem(item);
    }

    settings.endArray();
}

// When an item in the project content tree is double clicked
void AudioEditor::on_treeWidget_categories_itemDoubleClicked(QTreeWidgetItem *item, int column)
{
    int itemType = item->type();

    switch (itemType) {
    case 0:
        currentCategory = item->text(column);
        break;
    case 1:
        currentScenario = item->text(column);
        break;
    default:
        break;
    }
}

// Delete Category, Scenario or Element
void AudioEditor::on_pushButton_deleteSelected_clicked()
{
    // Only execute if a project is loaded
    if (isProjectOpen)
    {
        int type = ui->treeWidget_categories->currentItem()->type();

        QString arrayName;

        switch (type) {
        case 0:
            arrayName = "Categories";
            break;
        case 1:
            arrayName = currentCategory + "_Scenarios";
            break;
        case 2:
            arrayName = currentCategory + "_" + currentScenario + "_MusicLists";
            break;
        case 3:
            arrayName = currentCategory + "_" + currentScenario + "_SoundLists";
            break;
        case 4:
            arrayName = currentCategory + "_" + currentScenario + "_Radios";
            break;
        default:
            break;
        }

        // Setting up QSettings
        QSettings settings(settingsManager->getSetting(audioPath)+"/"+projectName+".ini", QSettings::IniFormat);

        int arraySize = settings.beginReadArray(arrayName);

        // Loading categories
        QStringList elements;

        for (int i = 0; i<arraySize; i++)
        {
            settings.setArrayIndex(i);

            QString element = settings.value("name").toString();

            qDebug() << element;

            if (element != ui->treeWidget_categories->currentItem()->text(0))
            {
                elements.push_back(element);
            }
        }
        settings.endArray();

        // Add only not deleted stuff
        settings.beginWriteArray(arrayName);

        for (int i = 0; i < elements.size(); i++)
        {
            settings.setArrayIndex(i);

            settings.setValue("name", elements.at(i));
        }
        settings.endArray();

        // Re-loading categories
        loadCategories();
    }
}

// Add a new scenario
void AudioEditor::on_pushButton_newScenario_clicked()
{
    QString scenario = ui->lineEdit_scenario->text();

    if (isProjectOpen && scenario != NULL && currentCategory != NULL)
    {
        QSettings settings(settingsManager->getSetting(audioPath)+"/"+projectName+".ini", QSettings::IniFormat);

        int size = settings.beginReadArray(currentCategory + "_Scenarios");
        settings.endArray();

        settings.beginWriteArray(currentCategory + "_Scenarios");
        settings.setArrayIndex(size);

        settings.setValue("name", scenario);
        currentScenario = scenario;

        settings.endArray();

        loadCategories();
    }
}

// Add a new Element (Music List, Sound List or Radio)
void AudioEditor::addNewElement(int type)
{
    QString element, arrayName;

    switch (type) {
    case 0:
        element = ui->lineEdit_musicList->text();
        ui->lineEdit_musicList->clear();
        arrayName = currentCategory + "_" + currentScenario + "_MusicLists";
        break;
    case 1:
        element = ui->lineEdit_soundList->text();
        ui->lineEdit_soundList->clear();
        arrayName = currentCategory + "_" + currentScenario + "_SoundLists";
        break;
    case 2:
        element = ui->lineEdit_radio->text();
        ui->lineEdit_radio->clear();
        arrayName = currentCategory + "_" + currentScenario + "_Radios";
        break;
    default:
        break;
    }


    if (isProjectOpen && !currentCategory.isNull() && !currentScenario.isNull() && !element.isNull())
    {
        QSettings settings(settingsManager->getSetting(audioPath)+"/"+projectName+".ini", QSettings::IniFormat);

        int size = settings.beginReadArray(arrayName);
        settings.endArray();

        settings.beginWriteArray(arrayName);
        settings.setArrayIndex(size);

        settings.setValue("name", element);
        currentElement = element;
        settings.endArray();

        loadCategories();
    }
}

// Create a new music list
void AudioEditor::on_pushButton_newMusicList_clicked()
{
    addNewElement(0);
}

// Create a new Sound List
void AudioEditor::on_pushButton_newSoundList_clicked()
{
    addNewElement(1);
}

// Create a new Radio
void AudioEditor::on_pushButton_newRadio_clicked()
{
    addNewElement(2);
}

// When another element is selected
void AudioEditor::on_treeWidget_categories_currentItemChanged(QTreeWidgetItem *current)
{
    if (ui->treeWidget_categories->currentColumn() >= 0)
    {
        int type = current->type();

        // Change stacked widget index based on the item type
        // 0 is the default "No element selected" screen
        ui->stackedWidget_editElement->setCurrentIndex(type + 1);

        // Load stuff
        QSettings settings(settingsManager->getSetting(audioPath)+"/"+projectName+".ini", QSettings::IniFormat);
        int arraySize;

        QString description;

        QTreeWidgetItem *item = current;

        switch (type) {
        // Category
        case 0:
            currentCategory = current->text(0);

            arraySize = settings.beginReadArray("Categories");

            for (int i = 0; i<arraySize; i++)
            {
                settings.setArrayIndex(i);

                if (settings.value("name").toString() == current->text(0))
                {
                    description = settings.value("description", "").toString();

                    ui->textEdit_categoryDescription->setText(description);
                }
            }

            break;

        // Scenario
        case 1:
            // Get current category
            while (item->type() != 0) {
                item = ui->treeWidget_categories->itemAbove(item);
            }
            currentCategory = item->text(0);

            currentScenario = current->text(0);

            arraySize = settings.beginReadArray(currentCategory+"_Scenarios");

            for (int i = 0; i<arraySize; i++)
            {
                settings.setArrayIndex(i);

                if (settings.value("name").toString() == current->text(0))
                {
                    description = settings.value("description", "").toString();

                    ui->textEdit_scenarioDescription->setText(description);
                }
            }

            break;

        // Music List
        case 2:
            // Get scenario
            while (item->type() != 1) {
                item = ui->treeWidget_categories->itemAbove(item);
            }
            currentScenario = item->text(0);

            // Get category
            while (item->type() != 0) {
                item = ui->treeWidget_categories->itemAbove(item);
            }
            currentCategory = item->text(0);

            currentElement = current->text(0);

            arraySize = settings.beginReadArray(currentCategory+"_"+currentScenario+"_MusicLists");

            for (int i = 0; i<arraySize; i++)
            {
                settings.setArrayIndex(i);

                if (settings.value("name").toString() == current->text(0))
                {
                    description = settings.value("description", "").toString();

                    ui->lineEdit_musicListDescription->setText(description);

                    // Get Playback Mode of Music List
                    ui->radioButton_musicListRandomOrder->setChecked(settings.value("randomPlayback", false).toBool());
                    ui->radioButton_musicListShuffleList->setChecked(settings.value("randomPlaylist", true).toBool());
                    ui->radioButton_musicListLoopList->setChecked(settings.value("loop", false).toBool());
                    ui->radioButton_musicListSequential->setChecked(settings.value("sequential", false).toBool());

                    // Songs
                    ui->listWidget_musicList->clear();

                    int songs = settings.beginReadArray("songs");
                    for (int i = 0; i<songs; i++)
                    {
                        settings.setArrayIndex(i);
                        QString songName = settings.value("name", "UNKNOWN NAME").toString();
                        QString songPath = settings.value("path", "UNKNOWN PATH").toString();

                        QListWidgetItem *item = new QListWidgetItem(songName);
                        item->setWhatsThis(songPath);

                        ui->listWidget_musicList->addItem(item);
                    }
                    settings.endArray();
                }
            }
            settings.endArray();

            ui->tabWidget->setCurrentIndex(0);
            break;

        // Sound List
        case 3:
            // Get scenario
            while (item->type() != 1) {
                item = ui->treeWidget_categories->itemAbove(item);
            }
            currentScenario = item->text(0);

            // Get category
            while (item->type() != 0) {
                item = ui->treeWidget_categories->itemAbove(item);
            }
            currentCategory = item->text(0);

            currentElement = current->text(0);

            arraySize = settings.beginReadArray(currentCategory+"_"+currentScenario+"_SoundLists");

            for (int i = 0; i<arraySize; i++)
            {
                settings.setArrayIndex(i);

                if (settings.value("name").toString() == current->text(0))
                {
                    description = settings.value("description", "").toString();

                    ui->lineEdit_soundListDescription->setText(description);

                    ui->radioButton_soundListRandom->setChecked(settings.value("random", true).toBool());
                    ui->radioButton_soundListLoop->setChecked(settings.value("loop", false).toBool());
                    ui->radioButton_soundListSequential->setChecked(settings.value("sequential", false).toBool());

                    // Sounds
                    ui->listWidget_soundList->clear();

                    int sounds = settings.beginReadArray("sounds");
                    for (int i = 0; i<sounds; i++)
                    {
                        settings.setArrayIndex(i);
                        QString soundName = settings.value("name", "UNKNOWN NAME").toString();
                        QString soundPath = settings.value("path", "UNKNOWN PATH").toString();

                        QListWidgetItem *item = new QListWidgetItem(soundName);
                        item->setWhatsThis(soundPath);

                        ui->listWidget_soundList->addItem(item);
                    }
                    settings.endArray();
                }
            }
            settings.endArray();

            ui->tabWidget->setCurrentIndex(1);
            break;

        // Radios
        case 4:
            // Get scenario
            while (item->type() != 1) {
                item = ui->treeWidget_categories->itemAbove(item);
            }
            currentScenario = item->text(0);

            // Get category
            while (item->type() != 0) {
                item = ui->treeWidget_categories->itemAbove(item);
            }
            currentCategory = item->text(0);

            currentElement = current->text(0);

            arraySize = settings.beginReadArray(currentCategory+"_"+currentScenario+"_Radios");

            for (int i = 0; i<arraySize; i++)
            {
                settings.setArrayIndex(i);

                if (settings.value("name").toString() == current->text(0))
                {
                    description = settings.value("description", "").toString();

                    ui->lineEdit_soundListDescription->setText(description);

                    ui->lineEdit_radioURL->setText(settings.value("URL").toString());
                }
            }

            ui->tabWidget->setCurrentIndex(2);
            break;

        default:

            ui->stackedWidget_editElement->setCurrentIndex(0);

            break;
        }
    }
}

// Save the element
void AudioEditor::on_pushButton_saveElement_clicked()
{
    QTreeWidgetItem *item = ui->treeWidget_categories->currentItem();
    int type = item->type();
    QString name = item->text(0);

    int arraySize;
    QString description;

    QString projectPath = settingsManager->getSetting(audioPath)+"/"+projectName+".ini";
    qDebug().noquote() << projectPath;
    qDebug().noquote() << "Current Category:" << currentCategory;
    qDebug().noquote() << "Current Scenario:" << currentScenario;
    qDebug().noquote() << "Current Element:" << currentElement;

    QSettings settings(projectPath, QSettings::IniFormat);

    switch (type) {

    // Category
    case 0:
        qDebug() << "Saving category ...";

        description = ui->textEdit_categoryDescription->toPlainText();

        arraySize = settings.beginReadArray("Categories");
        for (int i = 0; i<arraySize; i++)
        {
            settings.setArrayIndex(i);

            if (settings.value("name").toString() == name)
            {
                settings.setValue("description", description);
            }
        }

        settings.endArray();
        break;

    // Scenario
    case 1:
        qDebug() << "Saving scenario ...";

        description = ui->textEdit_scenarioDescription->toPlainText();

        arraySize = settings.beginReadArray(currentCategory + "_Scenarios");
        for (int i = 0; i<arraySize; i++)
        {
            settings.setArrayIndex(i);

            if (settings.value("name").toString() == name)
            {
                settings.setValue("description", description);
            }
        }

        settings.endArray();
        break;

    // Music List
    case 2:
        qDebug() << "Saving music list ...";

        description = ui->lineEdit_musicListDescription->text();

        arraySize = settings.beginReadArray(currentCategory + "_" + currentScenario + "_MusicLists");
        for (int i = 0; i<arraySize; i++)
        {
            settings.setArrayIndex(i);

            if (settings.value("name").toString() == name)
            {
                settings.setValue("description", description);

                // Save Playback Mode of Music List
                settings.setValue("randomPlayback", ui->radioButton_musicListRandomOrder->isChecked());
                settings.setValue("randomPlaylist", ui->radioButton_musicListShuffleList->isChecked());
                settings.setValue("loop", ui->radioButton_musicListLoopList->isChecked());
                settings.setValue("sequential", ui->radioButton_musicListSequential->isChecked());

                // Save songs
                settings.beginWriteArray("songs");
                for (int i = 0; i<ui->listWidget_musicList->count(); i++)
                {
                    settings.setArrayIndex(i);
                    settings.setValue("name", ui->listWidget_musicList->item(i)->text());
                    settings.setValue("path", ui->listWidget_musicList->item(i)->whatsThis());

                    qDebug().noquote() << " Name:" << ui->listWidget_musicList->item(i)->text();
                    qDebug().noquote() << " Path:" << ui->listWidget_musicList->item(i)->whatsThis();
                }
                settings.endArray();
            }
        }

        settings.endArray();
        break;

    // Sound List
    case 3:
        qDebug() << "Saving sound list ...";

        description = ui->lineEdit_soundListDescription->text();

        arraySize = settings.beginReadArray(currentCategory + "_" + currentScenario + "_SoundLists");
        for (int i = 0; i<arraySize; i++)
        {
            settings.setArrayIndex(i);

            if (settings.value("name").toString() == name)
            {
                settings.setValue("description", description);
                settings.setValue("random", ui->radioButton_soundListRandom->isChecked());
                settings.setValue("loop", ui->radioButton_soundListLoop->isChecked());
                settings.setValue("sequential", ui->radioButton_soundListSequential->isChecked());

                // Save sounds
                settings.beginWriteArray("sounds");
                for (int i = 0; i<ui->listWidget_soundList->count(); i++)
                {
                    settings.setArrayIndex(i);
                    settings.setValue("name", ui->listWidget_soundList->item(i)->text());
                    settings.setValue("path", ui->listWidget_soundList->item(i)->whatsThis());
                }
                settings.endArray();
            }
        }

        settings.endArray();
        break;

    // Radio
    case 4:
        qDebug() << "Saving radio ...";

        description = ui->textEdit_radioDescription->toPlainText();

        arraySize = settings.beginReadArray(currentCategory + "_" + currentScenario + "_Radios");
        for (int i = 0; i<arraySize; i++)
        {
            settings.setArrayIndex(i);

            if (settings.value("name").toString() == name)
            {
                settings.setValue("description", description);
                settings.setValue("URL", ui->lineEdit_radioURL->text());
            }
        }

        settings.endArray();
        break;

    // Everything else, should that actually be possible
    default:
        qDebug() << "ERROR: Something went wrong saving the current element.";
        break;
    }

}

// Remove a song from the music list
void AudioEditor::on_listWidget_musicList_itemDoubleClicked(QListWidgetItem *item)
{
    delete item;
}

// Remove a sound from the sound list
void AudioEditor::on_listWidget_soundList_itemDoubleClicked(QListWidgetItem *item)
{
    delete item;
}

// When an item in the music explorer is double clicked
void AudioEditor::on_treeWidget_music_itemDoubleClicked(QTreeWidgetItem *item, int column)
{
    if (item->type() == 1)
    {
        qDebug() << "Adding song: " + item->text(column);

        QListWidgetItem *listItem = new QListWidgetItem(item->text(column));
        listItem->setWhatsThis(item->whatsThis(column));

        ui->listWidget_musicList->addItem(listItem);

        on_pushButton_saveElement_clicked();
    }
}

// Add all music files from the selected folder
void AudioEditor::on_pushButton_addAllFilesFromMusicFolder_clicked()
{
    QTreeWidgetItem *item = ui->treeWidget_music->currentItem();

    if (item->type() == 0)
    {
        qDebug() << "Adding all music files from folder: " + item->text(0);

        QString path = item->whatsThis(0);

        for (QString file : getFiles(path))
        {
            if (file.contains(".mp3") || file.contains(".wav") || file.contains(".ogg") || file.contains(".flac"))
            {
                QString relativePath = path.replace(settingsManager->getSetting(musicPath), "");
                relativePath += "/"+file;

                QListWidgetItem *listItem = new QListWidgetItem(file);
                listItem->setWhatsThis(relativePath);

                ui->listWidget_musicList->addItem(listItem);
            }
        }

        on_pushButton_saveElement_clicked();
    }
}

// Add a sound to the sound list
void AudioEditor::on_treeWidget_sound_itemDoubleClicked(QTreeWidgetItem *item, int column)
{
    if (item->type() == 1)
    {
        qDebug() << "Adding sound: " + item->text(column);

        QListWidgetItem *listItem = new QListWidgetItem(item->text(column));
        listItem->setWhatsThis(item->whatsThis(column));

        ui->listWidget_soundList->addItem(listItem);

        on_pushButton_saveElement_clicked();
    }
}

void AudioEditor::on_pushButton_addAllFilesFromSoundFolder_clicked()
{
    QTreeWidgetItem *item = ui->treeWidget_sound->currentItem();

    if (item->type() == 0)
    {
        qDebug() << "Adding all sound files from folder: " + item->text(0);

        QString path = item->whatsThis(0);

        for (QString file : getFiles(path))
        {
            if (file.contains(".mp3") || file.contains(".wav") || file.contains(".ogg") || file.contains(".flac"))
            {
                QString relativePath = path.replace(settingsManager->getSetting(soundPath), "");
                relativePath += "/"+file;

                QListWidgetItem *listItem = new QListWidgetItem(file);
                listItem->setWhatsThis(relativePath);

                ui->listWidget_soundList->addItem(listItem);
            }
        }

        on_pushButton_saveElement_clicked();
    }
}

// Set a radio playlist
void AudioEditor::on_treeWidget_radio_itemDoubleClicked(QTreeWidgetItem *item, int column)
{
    ui->lineEdit_radioURL->setText(item->whatsThis(column));
}

// Create a new music list with all the songs from the selected folder
void AudioEditor::on_pushButton_convertFolderToMusicList_clicked()
{
    QTreeWidgetItem *item = ui->treeWidget_music->currentItem();

    if (item->type() == 0)
    {
        QString folderName = item->text(0);
        QString path = item->whatsThis(0);

        qDebug() << "Creating music list from folder: " + folderName;

        // Set the name of the music list
        ui->lineEdit_musicList->setText(folderName);

        // Create new list
        on_pushButton_newMusicList_clicked();

        // Select the new music list
        for( int i = 0; i < ui->treeWidget_categories->topLevelItemCount(); i++)
        {
            // Get the correct category item
            QTreeWidgetItem *catItem = ui->treeWidget_categories->topLevelItem(i);

            if (catItem->text(0) == currentCategory)
             {
                for (int j = 0; j<catItem->childCount(); j++)
                {
                    // Get the scenario item
                    QTreeWidgetItem *scenItem = catItem->child(j);

                    if (scenItem->text(0) == currentScenario)
                    {
                        for (int m = 0; m<scenItem->childCount(); m++)
                        {
                            // Get the music list item
                            QTreeWidgetItem *listItem = scenItem->child(m);

                            if (listItem->text(0) == folderName)
                            {
                                // Expand the scenario item
                                scenItem->setExpanded(true);

                                // Set list Item as curren item
                                ui->treeWidget_categories->setCurrentItem(listItem);
                            }
                        }
                    }
               }
           }
        }

        for (QString file : getFiles(path))
        {
            if (file.contains(".mp3") || file.contains(".wav") || file.contains(".ogg") || file.contains(".flac"))
            {
                QString relativePath = path.replace(settingsManager->getSetting(musicPath), "");
                relativePath += "/"+file;

                QListWidgetItem *listItem = new QListWidgetItem(file);
                listItem->setWhatsThis(relativePath);

                ui->listWidget_musicList->addItem(listItem);
            }
        }

        on_pushButton_saveElement_clicked();
    }
}

// Start preview
void AudioEditor::playPreview(QString path)
{
    qDebug().noquote() << "Previewing file:" << path;

    if (ui->checkBox_autoplay->isChecked() && QFile(path).exists())
    {
        ui->pushButton_playPause->setEnabled(true);
        ui->pushButton_playPause->setChecked(false);
        on_pushButton_playPause_toggled(false);

        previewPlayer->setMedia(QUrl::fromLocalFile(path));
        previewPlayer->setVolume(ui->verticalSlider_volume->value());
        previewPlayer->play();
    }
}

void AudioEditor::on_pushButton_playPause_toggled(bool checked)
{
    if (checked)
    {
        ui->pushButton_playPause->setIcon(QIcon(":/resources/mediaIcons/play.png"));
        ui->pushButton_playPause->setText("Play");
        previewPlayer->pause();

    }
    else
    {
        ui->pushButton_playPause->setIcon(QIcon(":/resources/mediaIcons/pause.png"));
        ui->pushButton_playPause->setText("Pause");
        previewPlayer->play();
    }
}

void AudioEditor::on_treeWidget_music_currentItemChanged(QTreeWidgetItem *current)
{
    if (current->type() == 1)
    {
        playPreview(settingsManager->getSetting(musicPath) + current->whatsThis(0));
    }
}

void AudioEditor::on_treeWidget_sound_currentItemChanged(QTreeWidgetItem *current)
{
    if (current->type() == 1)
    {
        playPreview(settingsManager->getSetting(soundPath) + current->whatsThis(0));
    }
}

void AudioEditor::on_verticalSlider_volume_valueChanged(int value)
{
    previewPlayer->setVolume(value);
}

void AudioEditor::on_horizontalSlider_progress_sliderMoved(int position)
{
    previewPlayer->setPosition(position);
}

void AudioEditor::previewPlayer_positionChanged(qint64 position)
{
    ui->horizontalSlider_progress->setMaximum(previewPlayer->duration());
    ui->horizontalSlider_progress->setValue(position);
}
