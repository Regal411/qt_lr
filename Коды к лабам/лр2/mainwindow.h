#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QPushButton>
#include <QLineEdit>
#include <QTextEdit>
#include <QLabel>
#include <QDialog>
#include <QComboBox>
#include "data_processor.h"
#include "advanced_data_processor.h"

class MainWindow : public QMainWindow {
    Q_OBJECT
public:
    explicit MainWindow(QWidget *parent = nullptr);

private slots:
    void handleProcessedData(const QString &data);
    void handleVariantProcessed(const QString &data);
    void onProcessButtonClicked();
    void openDialog();
    void handleDialogResult(const QString &result);

private:
    QLineEdit *inputField;
    QTextEdit *outputTextEdit;
    QLabel *variantLabel;
    QPushButton *processButton;
    QPushButton *openDialogButton;
    DataProcessor *processor;
    AdvancedDataProcessor *advancedProcessor;
};

class InputDialog : public QDialog {
    Q_OBJECT
public:
    explicit InputDialog(QWidget *parent = nullptr);

signals:
    void dialogClosed(const QString &result);

private slots:
    void onProcessButtonClicked();

private:
    QLineEdit *inputField;
    QComboBox *modeComboBox;
    QPushButton *processButton;
    QLabel *resultLabel;
    DataProcessor *dialogProcessor;
};

#endif // MAINWINDOW_H
