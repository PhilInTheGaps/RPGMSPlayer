#ifndef DICETOOL_H
#define DICETOOL_H

#include <QObject>
#include <QSettings>
#include <QList>

class DiceTool : public QObject
{
    Q_OBJECT
    Q_PROPERTY(int sides READ sides NOTIFY sidesChanged)
    Q_PROPERTY(int roll READ roll NOTIFY rollChanged)
    Q_PROPERTY(QString calculationString READ calculationString NOTIFY calculationStringChanged)

public:
    explicit DiceTool(QObject *parent = nullptr);

    int sides() const { return m_sides; }
    int roll();
    QString calculationString() const { return m_calculation_string; }

    Q_INVOKABLE void setSides(int sides);
    Q_INVOKABLE void setBonusDice(int count) { m_bonus_dice = count; }
    Q_INVOKABLE void setAmount(int amount) { m_amount = amount; }
    Q_INVOKABLE void setModifier(int modifier) { m_modifier = modifier; }

    Q_INVOKABLE void setDiceSettings(bool enableCriticals, int success, int failure, bool minMax, bool successMax);
    Q_INVOKABLE bool getCriticalEnabled() const { return settings->value("enableCriticals", true).toBool(); }
    Q_INVOKABLE int getSuccess() const { return settings->value("success", 20).toInt(); }
    Q_INVOKABLE int getFailure() const { return settings->value("failure", 1).toInt(); }
    Q_INVOKABLE bool getMinMax() const { return settings->value("useMinMax", true).toBool(); }
    Q_INVOKABLE bool getSuccessMax() const { return settings->value("successMax", true).toBool(); }

signals:
    void sidesChanged();
    void rollChanged();
    void calculationStringChanged();

    void mixedCriticalResult();
    void successfulCriticalResult();
    void failedCriticalResult();
    void normalResult();

private:
    int m_sides = 20;
    int m_bonus_dice = 0;
    int m_modifier = 0;
    int m_amount = 1;

    QSettings *settings;

    QString m_calculation_string;
};

#endif // DICETOOL_H
