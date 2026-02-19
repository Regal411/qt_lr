#include "data_processor.h"
#include <QDebug>
#include <QRegularExpression>

void DataProcessor::processData(const QString &input, int mode) {
    QString processedData;
    switch (mode) {
        case 0:
            processedData = input.toLower();
            break;
        case 1: {
            for (int i = input.length() - 1; i >= 0; --i) {
                processedData += input[i];
            }
            break;
        }
        case 2:
            processedData = input.simplified().replace(" ", "");
            break;
        default:
            processedData = input;
            break;
    }
    emit dataProcessed(processedData);
}

void DataProcessor::processVariant(const QVariant &input) {
    QString result = processVariantSync(input);
    emit variantProcessed(result);
}

QString DataProcessor::processVariantSync(const QVariant &input) {
    QString result;

    if (input.typeId() == QMetaType::QString) {
        QString str = input.toString().trimmed();

        if (str.isEmpty()) {
            result = "Пустая строка";
        } else if (isInteger(str)) {
            int value = str.toInt();
            result = QString("Целое число: %1").arg(value);
        } else if (isDouble(str)) {
            double value = str.toDouble();
            result = QString("Вещественное число: %1").arg(value, 0, 'f', 2);
        } else if (isBool(str)) {
            bool value = (str.toLower() == "true" || str == "1");
            result = QString("Логическое значение: %1").arg(value ? "true" : "false");
        } else {
            result = "Строка: " + str;
        }
    } else {
        switch (input.typeId()) {
            case QMetaType::Int:
                result = QString("Целое число: %1").arg(input.toInt());
                break;
            case QMetaType::Double:
                result = QString("Вещественное число: %1").arg(input.toDouble(), 0, 'f', 2);
                break;
            case QMetaType::Bool:
                result = QString("Логическое значение: %1").arg(input.toBool() ? "true" : "false");
                break;
            default:
                result = "Неизвестный тип данных";
                break;
        }
    }

    return result;
}

bool DataProcessor::isInteger(const QString &str) {
    QRegularExpression regex("^-?\\d+$");
    return regex.match(str).hasMatch();
}

bool DataProcessor::isDouble(const QString &str) {
    QRegularExpression regex("^-?\\d+(\\.\\d+)?$");
    return regex.match(str).hasMatch();
}

bool DataProcessor::isBool(const QString &str) {
    QString lowerStr = str.toLower();
    return (lowerStr == "true" || lowerStr == "false" || lowerStr == "1" || lowerStr == "0");
}
