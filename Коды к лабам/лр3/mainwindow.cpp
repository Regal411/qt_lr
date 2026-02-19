#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QInputDialog>
#include <QMessageBox>
#include <QFileDialog>
#include <QTextStream>
#include <QFileDialog>
#include <QTextStream>
#include <QDialog>
#include <QJsonDocument>
#include <QJsonObject>
#include <QFile>
#include <QJsonArray>

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    setupModel();
    setupSearch();

    connect(ui->btnAdd, &QPushButton::clicked, this, &MainWindow::addBook);
    connect(ui->btnEdit, &QPushButton::clicked, this, &MainWindow::editBook);
    connect(ui->btnRemove, &QPushButton::clicked, this, &MainWindow::removeBook);
    connect(ui->searchButton, &QPushButton::clicked, this, &MainWindow::searchBooks);
    connect(ui->actionImportCSV, &QAction::triggered, this, &MainWindow::importCSV);
    connect(ui->actionExportCSV, &QAction::triggered, this, &MainWindow::exportCSV);
    connect(ui->actionExit, &QAction::triggered, this, &QApplication::quit);
    connect(ui->actionImportJSON, &QAction::triggered, this, &MainWindow::importJSON);
    connect(ui->actionExportJSON, &QAction::triggered, this, &MainWindow::exportJSON);
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::exportCSV()
{
    QString fileName = QFileDialog::getSaveFileName(this, "Экспорт в CSV", "", "CSV Files (*.csv)");
    if (fileName.isEmpty()) {
        return;
    }

    if (!fileName.endsWith(".csv", Qt::CaseInsensitive)) {
        fileName += ".csv";
    }

    QFile file(fileName);
    if (!file.open(QIODevice::WriteOnly | QIODevice::Text)) {
        QMessageBox::warning(this, "Ошибка", "Не удалось сохранить файл");
        return;
    }

    QTextStream out(&file);

    for (int row = 0; row < model->rowCount(); ++row) {
        QStringList fields;
        for (int col = 0; col < model->columnCount(); ++col) {
            QString text = model->item(row, col)->text();

            // text.replace("\"", "\"\"");
            text.replace("\"", "");
            text.replace(";", ",");
            // fields.append('"' + text + '"');
            fields.append(text);
        }
        out << fields.join(";") << "\n";
    }

    file.close();
    QMessageBox::information(this, "Экспорт", "Файл сохранён успешно!");
}

void MainWindow::importCSV()
{
    QString fileName = QFileDialog::getOpenFileName(this, "Импорт CSV", "", "CSV Files (*.csv)");
    if (fileName.isEmpty()) {
        return;
    }

    QFile file(fileName);
    if (!file.open(QIODevice::ReadOnly | QIODevice::Text)) {
        QMessageBox::warning(this, "Ошибка", "Не удалось открыть файл");
        return;
    }

    QTextStream in(&file);
    model->clear();
    setupModel();

    int rowCount = 0;
    while (!in.atEnd()) {
        QString line = in.readLine();
        QStringList fields = line.split(";");

        if (fields.size() != 7) {
            continue;
        }

        QList<QStandardItem *> rowItems;
        for (const QString &field : fields) {
            rowItems.append(new QStandardItem(field.trimmed()));
        }

        model->appendRow(rowItems);
        rowCount++;
    }

    file.close();

    if (rowCount == 0) {
        QMessageBox::information(this, "Импорт CSV", "Файл загружен, но он пуст.");
    } else {
        QMessageBox::information(this, "Импорт CSV", "Файл успешно загружен: " + fileName);
    }
}

void MainWindow::exportJSON()
{
    QString fileName = QFileDialog::getSaveFileName(this, "Экспорт в JSON", "", "JSON Files (*.json)");
    if (fileName.isEmpty()) {
        return;
    }

    if (!fileName.endsWith(".json", Qt::CaseInsensitive)) {
        fileName += ".json";
    }

    QFile file(fileName);
    if (!file.open(QIODevice::WriteOnly | QIODevice::Text)) {
        QMessageBox::warning(this, "Ошибка", "Не удалось сохранить файл");
        return;
    }

    QJsonArray jsonArray;
    for (int row = 0; row < model->rowCount(); ++row) {
        QJsonObject jsonObject;
        jsonObject["author"] = model->item(row, 0)->text();
        jsonObject["title"] = model->item(row, 1)->text();
        jsonObject["year"] = model->item(row, 2)->text();
        jsonObject["genre"] = model->item(row, 3)->text();
        jsonObject["publisher"] = model->item(row, 4)->text();
        jsonObject["isbn"] = model->item(row, 5)->text();
        jsonObject["pageCount"] = model->item(row, 6)->text();
        jsonArray.append(jsonObject);
    }

    QJsonDocument jsonDoc(jsonArray);
    file.write(jsonDoc.toJson());
    file.close();

    QMessageBox::information(this, "Экспорт", "Файл сохранён успешно!");
}

void MainWindow::importJSON()
{
    QString fileName = QFileDialog::getOpenFileName(this, "Импорт JSON", "", "JSON Files (*.json)");
    if (fileName.isEmpty()) {
        return;
    }

    QFile file(fileName);
    if (!file.open(QIODevice::ReadOnly | QIODevice::Text)) {
        QMessageBox::warning(this, "Ошибка", "Не удалось открыть файл");
        return;
    }

    QByteArray jsonData = file.readAll();
    QJsonDocument jsonDoc = QJsonDocument::fromJson(jsonData);
    if (!jsonDoc.isArray()) {
        QMessageBox::warning(this, "Ошибка", "Неверный формат JSON");
        return;
    }

    model->clear();
    setupModel();

    QJsonArray jsonArray = jsonDoc.array();
    for (const QJsonValue &value : jsonArray) {
        if (value.isObject()) {
            QJsonObject jsonObject = value.toObject();
            QList<QStandardItem *> rowItems;
            rowItems.append(new QStandardItem(jsonObject["author"].toString()));
            rowItems.append(new QStandardItem(jsonObject["title"].toString()));
            rowItems.append(new QStandardItem(jsonObject["year"].toString()));
            rowItems.append(new QStandardItem(jsonObject["genre"].toString()));
            rowItems.append(new QStandardItem(jsonObject["publisher"].toString()));
            rowItems.append(new QStandardItem(jsonObject["isbn"].toString()));
            rowItems.append(new QStandardItem(jsonObject["pageCount"].toString()));
            model->appendRow(rowItems);
        }
    }

    file.close();
    QMessageBox::information(this, "Импорт JSON", "Файл успешно загружен: " + fileName);
}

void MainWindow::setupModel()
{
    model = new QStandardItemModel(this);
    model->setColumnCount(7);
    model->setHeaderData(0, Qt::Horizontal, "Автор");
    model->setHeaderData(1, Qt::Horizontal, "Название");
    model->setHeaderData(2, Qt::Horizontal, "Год");
    model->setHeaderData(3, Qt::Horizontal, "Жанр");
    model->setHeaderData(4, Qt::Horizontal, "Издательство");
    model->setHeaderData(5, Qt::Horizontal, "ISBN");
    model->setHeaderData(6, Qt::Horizontal, "Количество страниц");

    proxyModel = new QSortFilterProxyModel(this);
    proxyModel->setSourceModel(model);
    proxyModel->setFilterCaseSensitivity(Qt::CaseInsensitive);
    proxyModel->setFilterKeyColumn(-1);

    ui->tableView->setModel(proxyModel);
    ui->tableView->setSortingEnabled(true);
    ui->tableView->horizontalHeader()->setSortIndicatorShown(true);
}

void MainWindow::addBook()
{
    QString author = QInputDialog::getText(this, "Добавить книгу", "Автор:");
    if (author.isEmpty()) return;

    QString title = QInputDialog::getText(this, "Добавить книгу", "Название:");
    if (title.isEmpty()) return;

    int year = QInputDialog::getInt(this, "Добавить книгу", "Год издания:", 1000, 0, 2100);

    QString genre = QInputDialog::getText(this, "Добавить книгу", "Жанр:");
    if (genre.isEmpty()) return;

    QString publisher = QInputDialog::getText(this, "Добавить книгу", "Издательство:");
    if (publisher.isEmpty()) return;

    QString isbn = QInputDialog::getText(this, "Добавить книгу", "ISBN:");
    if (isbn.isEmpty()) return;

    int pageCount = QInputDialog::getInt(this, "Добавить книгу", "Количество страниц:", 100, 1);

    QList<QStandardItem *> rowItems;
    rowItems << new QStandardItem(author)
             << new QStandardItem(title)
             << new QStandardItem(QString::number(year))
             << new QStandardItem(genre)
             << new QStandardItem(publisher)
             << new QStandardItem(isbn)
             << new QStandardItem(QString::number(pageCount));

    model->appendRow(rowItems);
}

void MainWindow::editBook()
{
    QModelIndex index = ui->tableView->currentIndex();
    if (!index.isValid()) {
        QMessageBox::warning(this, "Ошибка", "Выберите книгу для редактирования");
        return;
    }

    QString author = model->item(index.row(), 0)->text();
    QString title = model->item(index.row(), 1)->text();
    int year = model->item(index.row(), 2)->text().toInt();
    QString genre = model->item(index.row(), 3)->text();
    QString publisher = model->item(index.row(), 4)->text();
    QString isbn = model->item(index.row(), 5)->text();
    int pageCount = model->item(index.row(), 6)->text().toInt();

    EditBookDialog dialog(this);
    dialog.setBookData(author, title, year, genre, publisher, isbn, pageCount);

    if (dialog.exec() == QDialog::Accepted) {
        model->setItem(index.row(), 0, new QStandardItem(dialog.getAuthor()));
        model->setItem(index.row(), 1, new QStandardItem(dialog.getTitle()));
        model->setItem(index.row(), 2, new QStandardItem(QString::number(dialog.getYear())));
        model->setItem(index.row(), 3, new QStandardItem(dialog.getGenre()));
        model->setItem(index.row(), 4, new QStandardItem(dialog.getPublisher()));
        model->setItem(index.row(), 5, new QStandardItem(dialog.getISBN()));
        model->setItem(index.row(), 6, new QStandardItem(QString::number(dialog.getPageCount())));
    }
}

void MainWindow::removeBook()
{
    QModelIndex index = ui->tableView->currentIndex();
    if (!index.isValid()) {
        QMessageBox::warning(this, "Ошибка", "Выберите книгу для удаления");
        return;
    }

    model->removeRow(index.row());
}

void MainWindow::setupSearch()
{
    filterComboBox = new QComboBox(this);
    filterComboBox->addItem("Все поля");
    filterComboBox->addItem("Автор");
    filterComboBox->addItem("Название");
    filterComboBox->addItem("Год");
    filterComboBox->addItem("Жанр");
    filterComboBox->addItem("Издательство");
    filterComboBox->addItem("ISBN");
    filterComboBox->addItem("Количество страниц");

    ui->searchBoxLayout->addWidget(filterComboBox);

    connect(ui->searchLineEdit, &QLineEdit::textChanged, this, &MainWindow::searchBooks);
    connect(filterComboBox, &QComboBox::currentIndexChanged, this, &MainWindow::searchBooks);
}

void MainWindow::searchBooks()
{
    QString searchText = ui->searchLineEdit->text();
    int filterColumn = -1;

    switch (filterComboBox->currentIndex()) {
        case 1: // Автор
            filterColumn = 0;
            break;
        case 2: // Название
            filterColumn = 1;
            break;
        case 3: // Год
            filterColumn = 2;
            break;
        case 4: // Жанр
            filterColumn = 3;
            break;
        case 5: // Издательство
            filterColumn = 4;
            break;
        case 6: // ISBN
            filterColumn = 5;
            break;
        case 7: // Количество страниц
            filterColumn = 6;
            break;
        default: // Все поля
            filterColumn = -1;
            break;
    }

    proxyModel->setFilterKeyColumn(filterColumn);
    proxyModel->setFilterFixedString(searchText);
}
