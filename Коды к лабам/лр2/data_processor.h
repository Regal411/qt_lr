#ifndef DATA_PROCESSOR_H
#define DATA_PROCESSOR_H

#include <QObject>
#include <QVariant>

class DataProcessor : public QObject {
    Q_OBJECT
public:
    explicit DataProcessor(QObject *parent = nullptr) : QObject(parent) {}

public slots:
    void processData(const QString &input, int mode);
    void processVariant(const QVariant &input);
    QString processVariantSync(const QVariant &input);

signals:
    void dataProcessed(const QString &result);
    void variantProcessed(const QString &result);

private:
    bool isInteger(const QString &str);
    bool isDouble(const QString &str);
    bool isBool(const QString &str);
};

#endif // DATA_PROCESSOR_H
