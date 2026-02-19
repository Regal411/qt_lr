#include "mainwindow.h"
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QWidget>

MainWindow::MainWindow(QWidget *parent) : QMainWindow(parent) {
    QWidget *centralWidget = new QWidget(this);
    setCentralWidget(centralWidget);

    inputField = new QLineEdit(this);
    processButton = new QPushButton("Обработать", this);
    openDialogButton = new QPushButton("Открыть диалог", this);
    outputTextEdit = new QTextEdit(this);
    outputTextEdit->setReadOnly(true);
    variantLabel = new QLabel("Результат обработки QVariant: ", this);

    QVBoxLayout *layout = new QVBoxLayout(centralWidget);
    layout->addWidget(inputField);
    layout->addWidget(processButton);
    layout->addWidget(outputTextEdit);
    layout->addWidget(variantLabel);
    layout->addWidget(openDialogButton);

    processor = new DataProcessor(this);
    advancedProcessor = new AdvancedDataProcessor(this);


    connect(processor, &DataProcessor::variantProcessed, this, &MainWindow::handleVariantProcessed);
    connect(processButton, &QPushButton::clicked, this, &MainWindow::onProcessButtonClicked);
    connect(openDialogButton, &QPushButton::clicked, this, &MainWindow::openDialog);


    connect(advancedProcessor, &AdvancedDataProcessor::textRemoveSpacesDone,
            advancedProcessor, &AdvancedDataProcessor::reverse);
    connect(advancedProcessor, &AdvancedDataProcessor::textReverseDone,
            advancedProcessor, &AdvancedDataProcessor::toLowerCase);


    connect(advancedProcessor, &AdvancedDataProcessor::textRemoveSpacesDone,
            this, [this](const QString &res) {
                outputTextEdit->append("После удаления пробелов: " + res);
            });
    connect(advancedProcessor, &AdvancedDataProcessor::textReverseDone,
            this, [this](const QString &res) {
                outputTextEdit->append("После реверса: " + res);
            });
    connect(advancedProcessor, &AdvancedDataProcessor::textLowerCaseDone,
            this, [this](const QString &res) {
                outputTextEdit->append("После нижнего регистра (финал): " + res);
            });
}

void MainWindow::handleProcessedData(const QString &data) {
}

void MainWindow::handleVariantProcessed(const QString &data) {
    variantLabel->setText("Результат обработки: " + data);
}

void MainWindow::onProcessButtonClicked() {
    QString inputData = inputField->text();


    outputTextEdit->clear();


    QVariant variantData(inputData);
    processor->processVariant(variantData);


    advancedProcessor->removeSpaces(inputData);
}

void MainWindow::openDialog() {
    InputDialog *dialog = new InputDialog(this);
    connect(dialog, &InputDialog::dialogClosed, this, &MainWindow::handleDialogResult);
    dialog->exec();
}

void MainWindow::handleDialogResult(const QString &result) {

    outputTextEdit->clear();
    outputTextEdit->append("Результат из диалога: " + result);
}


InputDialog::InputDialog(QWidget *parent) : QDialog(parent) {
    QVBoxLayout *layout = new QVBoxLayout(this);
    inputField = new QLineEdit(this);
    modeComboBox = new QComboBox(this);
    modeComboBox->addItem("В нижний регистр");
    modeComboBox->addItem("Обратный порядок");
    modeComboBox->addItem("Удалить пробелы");
    processButton = new QPushButton("Обработать", this);
    resultLabel = new QLabel("", this);
    dialogProcessor = new DataProcessor(this);

    layout->addWidget(inputField);
    layout->addWidget(modeComboBox);
    layout->addWidget(processButton);
    layout->addWidget(resultLabel);

    connect(processButton, &QPushButton::clicked, this, &InputDialog::onProcessButtonClicked);
}

void InputDialog::onProcessButtonClicked() {
    QString inputData = inputField->text();
    int mode = modeComboBox->currentIndex();
    QString processedData;


    QVariant variantData(inputData);
    QString typeResult = dialogProcessor->processVariantSync(variantData);


    switch (mode) {
        case 0:
            processedData = inputData.toLower();
            break;
        case 1: {
            for (int i = inputData.length() - 1; i >= 0; --i) {
                processedData += inputData[i];
            }
            break;
        }
        case 2:
            processedData = inputData.simplified().replace(" ", "");
            break;
        default:
            processedData = inputData;
            break;
    }


    resultLabel->setText(typeResult + "\nОбработанный текст: " + processedData);

    emit dialogClosed(processedData);
    close();
}
