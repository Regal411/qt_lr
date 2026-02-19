#include "mainwindow.h"
#include "./ui_mainwindow.h"
#include <QDialog>
#include <QVBoxLayout>
#include <QLineEdit>
#include <QTextEdit>
#include <QDateEdit>
#include <QPushButton>
#include <QMessageBox>
#include <QSettings>
#include <QCloseEvent>

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    loadTasks();
    connect(ui->addTaskButton, &QPushButton::clicked, this, &MainWindow::addButton);
    connect(ui->detailsTaskButton, &QPushButton::clicked, this, &MainWindow::detailsButton);
    connect(ui->deleteTaskButton, &QPushButton::clicked, this, &MainWindow::deleteButton);

}

MainWindow::~MainWindow()
{
    delete ui;
}


void MainWindow::closeEvent(QCloseEvent *event)
{
    saveTasks();
    QMainWindow::closeEvent(event);
}




void MainWindow::saveTasks()
{
    QSettings settings("MyCompany", "MyTodoApp");
    settings.beginWriteArray("tasks");

    for (int i = 0; i < ui->listWidget->count(); ++i) {
        settings.setArrayIndex(i);
        QListWidgetItem *item = ui->listWidget->item(i);
        settings.setValue("title", item->text());
        settings.setValue("description", item->data(Qt::UserRole).toString());
        settings.setValue("date", item->data(Qt::UserRole + 1).toDate());
    }

    settings.endArray();
}



void MainWindow::loadTasks()
{
    QSettings settings("MyCompany", "MyTodoApp");
    int size = settings.beginReadArray("tasks");

    for (int i = 0; i < size; ++i) {
        settings.setArrayIndex(i);
        QString title = settings.value("title").toString();
        QString description = settings.value("description").toString();
        QDate date = settings.value("date").toDate();

        QListWidgetItem *item = new QListWidgetItem(title);
        item->setData(Qt::UserRole, description);
        item->setData(Qt::UserRole + 1, date);
        ui->listWidget->addItem(item);
    }

    settings.endArray();
}


void MainWindow::addButton()
{
   QDialog dialog(this);
   dialog.setWindowTitle("Добавить задачу");
   QVBoxLayout layout(&dialog);

   QLineEdit titleEdit;
   titleEdit.setPlaceholderText("Название задачи");
   layout.addWidget(&titleEdit);

   QTextEdit descriptionEdit;
   descriptionEdit.setPlaceholderText("Описание задачи");
   layout.addWidget(&descriptionEdit);

   QDateEdit dateEdit;
   dateEdit.setCalendarPopup(true);
   layout.addWidget(&dateEdit);

   QPushButton addButton("Добавить");
   layout.addWidget(&addButton);

   connect(&addButton, &QPushButton::clicked, [&]() {
       QString title = titleEdit.text().trimmed();
       QString description = descriptionEdit.toPlainText().trimmed();
       QDate date = dateEdit.date();



       if (title.isEmpty()) {
           QMessageBox::warning(&dialog, "Ошибка ввода", "Введите название задачи!");
           return;
       }


       if (description.isEmpty()) {
           QMessageBox::warning(&dialog, "Ошибка ввода", "Введите описание задачи!");
           return;
       }



       QListWidgetItem *newItem = new QListWidgetItem(title);
       newItem->setData(Qt::UserRole, description);
       newItem->setData(Qt::UserRole + 1, date);
       ui->listWidget->addItem(newItem);
       dialog.accept();
   });

   dialog.exec();
}


void MainWindow::detailsButton()
{
   QListWidgetItem *item = ui->listWidget->currentItem();
   if (item) {
       QString taskDetails = item->text();
       QString description = item->data(Qt::UserRole).toString();
       QDate taskDate = item->data(Qt::UserRole + 1).toDate();
       QString details = QString("<b>Задача:</b> %1\n<b>Описание:</b> %2\n<b>Дата выполнения:</b> %3")
                         .arg(taskDetails)
                         .arg(description)
                         .arg(taskDate.toString("dd.MM.yyyy"));
       QMessageBox::information(this, "Детали задачи", details);
   } else {
       QMessageBox::warning(this, "Просмотр задачи", "Выберите задачу для просмотра.");
   }
}


void MainWindow::deleteButton()
{
   QListWidgetItem *item = ui->listWidget->currentItem();
   if (item) {
       QMessageBox msgBox(this);
       msgBox.setWindowTitle("Удаление задачи");
       msgBox.setText("Вы уверены, что хотите удалить задачу?");
       QPushButton* yesButton = msgBox.addButton(QMessageBox::Yes);
       QPushButton* noButton = msgBox.addButton(QMessageBox::No);
       yesButton->setText("Да");
       noButton->setText("Нет");
       msgBox.exec();
       if (msgBox.clickedButton() == yesButton) {
           delete item;
       }
   } else {
       QMessageBox::warning(this, "Удаление задачи", "Выберите задачу для удаления.");
   }
}
