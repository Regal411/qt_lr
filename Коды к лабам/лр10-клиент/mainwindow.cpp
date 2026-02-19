#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QMessageBox>
#include <QDataStream>
#include <QTime>

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
    , socket(new QTcpSocket(this))
    , nextBlockSize(0)
{
    ui->setupUi(this);


    connect(socket, &QTcpSocket::readyRead, this, &MainWindow::slotReadyRead);
    connect(socket, &QTcpSocket::disconnected, this, [this]() {
        setConnected(false);
        ui->chatBrowser->append("<font color='red'>[" + QTime::currentTime().toString() + "] Отключено от сервера</font>");
    });

    setConnected(false);
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::on_connectButton_clicked()
{
    username = ui->nameLineEdit->text().trimmed();
    QString ip = ui->ipLineEdit->text().trimmed();

    if (username.isEmpty() || ip.isEmpty()) {
        QMessageBox::warning(this, "Ошибка", "Заполните все поля!");
        return;
    }

    socket->connectToHost(ip, 2323);
    if (socket->waitForConnected(3000)) {
        sendToServer("[name]:" + username);
        setConnected(true);
        ui->chatBrowser->append("<font color='green'>[" + QTime::currentTime().toString() + "] Подключено к серверу</font>");
    } else {
        QMessageBox::warning(this, "Ошибка", "Не удалось подключиться к серверу");
    }
}


void MainWindow::on_disconnectButton_clicked()
{
    socket->disconnectFromHost();
}

void MainWindow::on_sendButton_clicked()
{
    QString message = ui->messageLineEdit->text().trimmed();
    if (!message.isEmpty()) {
        QString recipient = ui->userComboBox->currentText();
        if (recipient == "Все") {
            sendToServer(message);
        } else {
            sendToServer("[private]:" + recipient + ":" + message);
        }
        ui->messageLineEdit->clear();
    }
}


void MainWindow::on_messageLineEdit_returnPressed()
{
    on_sendButton_clicked();
}


void MainWindow::slotReadyRead()
{
    QDataStream in(socket);
    in.setVersion(QDataStream::Qt_6_2);

    while (true) {
        if (nextBlockSize == 0) {
            if (socket->bytesAvailable() < sizeof(quint16)) break;
            in >> nextBlockSize;
        }
        if (socket->bytesAvailable() < nextBlockSize) break;

        QString message;
        in >> message;
        nextBlockSize = 0;


        if (message.startsWith("[users]:")) {
            QStringList users = message.mid(8).split(';');
            updateUserList(users);
        }

        else if (message.startsWith("[private]:")) {
            QString formattedMsg = "<font color='purple'>[" + QTime::currentTime().toString() + "] " + message.mid(10) + "</font>";
            ui->chatBrowser->append(formattedMsg);
        }

        else {
            ui->chatBrowser->append("[" + QTime::currentTime().toString() + "] " + message);
        }
    }
}


void MainWindow::updateUserList(const QStringList &users)
{
    ui->userComboBox->clear();
    ui->userComboBox->addItem("Все");
    ui->userComboBox->addItems(users);
}

void MainWindow::setConnected(bool connected)
{
    ui->connectButton->setEnabled(!connected);
    ui->disconnectButton->setEnabled(connected);
    ui->sendButton->setEnabled(connected);
    ui->messageLineEdit->setEnabled(connected);
    ui->userComboBox->setEnabled(connected);
}

void MainWindow::sendToServer(const QString &message)
{
    QByteArray data;
    QDataStream out(&data, QIODevice::WriteOnly);
    out.setVersion(QDataStream::Qt_6_2);
    out << quint16(0) << message;
    out.device()->seek(0);
    out << quint16(data.size() - sizeof(quint16));
    socket->write(data);
}
