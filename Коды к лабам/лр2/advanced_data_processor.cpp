#include "advanced_data_processor.h"

void AdvancedDataProcessor::removeSpaces(const QString &text) {
    QString result = text.simplified().replace(" ", "");
    emit textRemoveSpacesDone(result);
}

void AdvancedDataProcessor::reverse(const QString &text) {
    QString result;
    for (int i = text.length() - 1; i >= 0; --i) {
        result += text[i];
    }
    emit textReverseDone(result);
}

void AdvancedDataProcessor::toLowerCase(const QString &text) {
    QString result = text.toLower();
    emit textLowerCaseDone(result);
}
