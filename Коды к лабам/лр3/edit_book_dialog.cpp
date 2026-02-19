#include "edit_book_dialog.h"
#include "ui_editbookdialog.h"
#include <QMessageBox>

EditBookDialog::EditBookDialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::EditBookDialog)
{
    ui->setupUi(this);
}

EditBookDialog::~EditBookDialog()
{
    delete ui;
}

void EditBookDialog::setBookData(const QString &author, const QString &title, int year, const QString &genre, const QString &publisher, const QString &isbn, int pageCount)
{
    ui->authorLineEdit->setText(author);
    ui->titleLineEdit->setText(title);
    ui->yearSpinBox->setValue(year);
    ui->genreLineEdit->setText(genre);
    ui->publisherLineEdit->setText(publisher);
    ui->isbnLineEdit->setText(isbn);
    ui->pageCountSpinBox->setValue(pageCount);
}

QString EditBookDialog::getPublisher() const
{
    return ui->publisherLineEdit->text();
}

QString EditBookDialog::getISBN() const
{
    return ui->isbnLineEdit->text();
}

int EditBookDialog::getPageCount() const
{
    return ui->pageCountSpinBox->value();
}

QString EditBookDialog::getAuthor() const
{
    return ui->authorLineEdit->text();
}

QString EditBookDialog::getTitle() const
{
    return ui->titleLineEdit->text();
}

int EditBookDialog::getYear() const
{
    return ui->yearSpinBox->value();
}

QString EditBookDialog::getGenre() const
{
    return ui->genreLineEdit->text();
}

void EditBookDialog::on_buttonBox_accepted()
{
    if (getAuthor().isEmpty() || getTitle().isEmpty() || getGenre().isEmpty() || getPublisher().isEmpty() || getISBN().isEmpty()) {
        QMessageBox::warning(this, "Ошибка", "Все поля должны быть заполнены.");
        return;
    }

    int year = getYear();
    if ((year < 0) || (year > 2100)) {
        QMessageBox::warning(this, "Ошибка", "Введите корректный год издания.");
        return;
    }

    int pageCount = getPageCount();
    if (pageCount <= 0) {
        QMessageBox::warning(this, "Ошибка", "Количество страниц должно быть положительным.");
        return;
    }

    accept();
}
