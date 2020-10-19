#include "audioproject.h"
#include <QDebug>
#include <QJsonArray>
#include <utility>

AudioProject::AudioProject(const QString& name, int version, QList<AudioCategory *>categories, QObject *parent)
    : QObject(parent), m_name(name), m_version(version), m_categories(std::move(categories))
{
    qDebug() << "Initializing AudioProject:" << name << "...";
    qDebug() << "   Project version:" << version;
    qDebug() << "   Categories:" << m_categories.size();

    if (!m_categories.isEmpty()) setCurrentCategory(m_categories.first());

    for (auto *category : m_categories)
    {
        prepareCategory(category);
    }

    connectSignals();
}

AudioProject::AudioProject(QJsonObject object, QObject *parent)
    : QObject(parent)
{
    m_name    = object["name"].toString();
    m_version = object["version"].toInt();

    for (auto category : object["categories"].toArray())
    {
        auto *object = new AudioCategory(category.toObject(), m_name, this);
        prepareCategory(object);
        m_categories.append(object);
    }

    if (!m_categories.isEmpty()) setCurrentCategory(m_categories.first());
    connectSignals();
}

/**
 * @brief Serialize project to json.
 */
auto AudioProject::toJson() const -> QJsonObject
{
    QJsonObject root;

    root.insert("name",    m_name);
    root.insert("version", m_version);

    // Save Categories
    QJsonArray categoriesJson;

    for (auto *category : m_categories)
    {
        if (category) categoriesJson.append(category->toJson());
    }

    root.insert("categories", categoriesJson);
    return root;
}

/**
    Get the names of all categories in project
    @return QStringList with category names
 */
auto AudioProject::categoryNames() const -> QStringList
{
    qDebug() << "AudioProject: Getting category names of project" << m_name << "...";
    QStringList names;

    for (auto *category : m_categories)
    {
        names.append(category->name());
    }

    return names;
}

/**
 * @brief Remove (and delete) a category from the project
 */
auto AudioProject::deleteCategory(AudioCategory *category) -> bool
{
    if (!category || !m_categories.contains(category)) return false;

    if (!m_categories.removeOne(category)) return false;

    category->deleteLater();
    emit categoriesChanged();
    return true;
}

/**
 * @brief Set the current category
 */
auto AudioProject::setCurrentCategory(AudioCategory *category) -> bool
{
    // Disconnect old scenario changed signal
    if (currentCategory())
    {
        disconnect(currentCategory(), &AudioCategory::currentScenarioChanged,
                   this, &AudioProject::currentScenarioChanged);
    }

    m_currentCategory = category;
    emit currentCategoryChanged();
    emit currentScenarioChanged();

    if (category)
    {
        // Connect new scenario changed signal
        connect(category, &AudioCategory::currentScenarioChanged,
                   this, &AudioProject::currentScenarioChanged);
        return true;
    }

    return false;
}

/**
 * @brief Add a new category to the project
 * @param category The category to be added.
 * @param setAsCurrent Set to true if the category should be set as the current category.
 * @return
 */
auto AudioProject::addCategory(AudioCategory *category, bool setAsCurrent) -> bool
{
    if (!category) return false;

    prepareCategory(category);
    m_categories.append(category);
    emit categoriesChanged();

    if (setAsCurrent)
    {
        return setCurrentCategory(category);
    }

    return true;
}

/**
 * @brief Get the current scenario of the current category
 */
auto AudioProject::currentScenario() const -> AudioScenario *
{
    if (!currentCategory()) return nullptr;

    return currentCategory()->currentScenario();
}

/**
 * @brief Get a list of all elements within this project.
 */
auto AudioProject::elements() const -> QList<AudioElement *>
{
    QList<AudioElement *> list;

    for (auto category : m_categories)
    {
        if (category) list.append(category->elements());
    }

    return list;
}

auto AudioProject::connectSignals() -> void
{
    connect(this, &AudioProject::nameChanged, this, &AudioProject::onWasEdited);
    connect(this, &AudioProject::categoriesChanged, this, &AudioProject::onWasEdited);
}

auto AudioProject::prepareCategory(AudioCategory *category) -> void
{
    if (!category) return;
    category->setParent(this);

    connect(category, &AudioCategory::wasEdited, this, &AudioProject::onWasEdited);
    connect(category, &AudioCategory::nameChanged, this, &AudioProject::onWasEdited);
    connect(category, &AudioCategory::scenariosChanged, this, &AudioProject::onWasEdited);
}

auto AudioProject::onWasEdited() -> void
{
    setIsSaved(false);
}
