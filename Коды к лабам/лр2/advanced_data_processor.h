#ifndef ADVANCED_DATA_PROCESSOR_H
#define ADVANCED_DATA_PROCESSOR_H

#include <QObject>
#include <QString>

class AdvancedDataProcessor : public QObject {
    Q_OBJECT
public:
    explicit AdvancedDataProcessor(QObject *parent = nullptr) : QObject(parent) {}

public slots:
    void removeSpaces(const QString &text);
    void reverse(const QString &text);
    void toLowerCase(const QString &text);

signals:
    void textRemoveSpacesDone(const QString &result);
    void textReverseDone(const QString &result);
    void textLowerCaseDone(const QString &result);
};

#endif // ADVANCED_DATA_PROCESSOR_H
