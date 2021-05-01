#include "widget.h"
#include "ui_widget.h"
#include <QRegExp>
#include <QTcpSocket>
#include <QTime>
#include <QElapsedTimer>
#include <QMessageBox>

QString tmp;        // 문장 일치하는지 확인
QTime t;            // 시간 측정
QString userCmp;    // 누가 이겼는지 확인
QString userReal;   // 이름 저장
QString gameStart = "For game start [Press Enter]";
QString win = "You win! For next sentence [Press Enter]";
QString lose = "You lose! For next sentence [Press Enter]";
QString gameWin = "Game Set. You Win!  Retry [Press Enter]";
QString gameLose = "Game Se t. You Lose..  Retry [Press Enter]";

int sig = 0;
int userPoint = 0;
int othersPoint = 0;

Widget::Widget(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::Widget)
{
    ui->setupUi(this);
    setWindowTitle("Client");
}

Widget::~Widget()
{
    delete ui;
    delete socket;
}

void Widget::readyRead()
{
    while(socket->canReadLine())
    {
        QString line = QString::fromUtf8(socket->readLine());   // 서버에서 문자열 받아옴
        if(line.toInt() == 1) // User1 이름 저장
        {
            ui->pointEdit->setText("0"); // 자신의 포인트 초기값
            ui->pointEdit_2->setText("0"); // 상대 포인트 초기값
            ui->timeEdit->setText("0"); // 타수 초기값
            ui->nameEdit->setText("User1");
            userReal = "User1";
        }
        else if(line.toInt() == 2)          // User2 이름 저장
        {
            ui->pointEdit->setText("0"); // 자신의 포인트 초기값
            ui->pointEdit_2->setText("0"); // 상대 포인트 초기값
            ui->timeEdit->setText("0"); // 타수 초기값
            ui->nameEdit->setText("User2");
            userReal = "User2";
        }

        // User1 포인트 획득 출력
        else if(line.left(5) == "User1")
        {
            if(userReal == "User1") // 내가 승리일때
            {
                userPoint++;

                if(userPoint == 2) // 내가 승리하여 게임이 끝났을때
                {
                    ui->listWidget->setTextColor(QColor(255, 0, 0));
                    ui->listWidget->setText(gameWin);
                    ui->listWidget->setTextColor(QColor(0, 0, 0));
                    userPoint = 0;
                    othersPoint = 0;
                    ui->pointEdit->setText("0");
                    ui->pointEdit_2->setText("0");
                    QMessageBox endBox;
                    endBox.setWindowTitle("Notice");
                    endBox.setText("GameSet! Retry?");
                    endBox.setStandardButtons(QMessageBox::Retry | QMessageBox::Cancel);
                    endBox.setDefaultButton(QMessageBox::Retry);
                    if(endBox.exec() == QMessageBox::Cancel)
                    {
                        this->close();
                    }
                }
                else    // 게임 진행중
                {
                    ui->listWidget->setTextColor(QColor(0, 0, 255));
                    ui->listWidget->setText(win);
                    ui->listWidget->setTextColor(QColor(0, 0, 0));
                    ui->pointEdit->setText(QString("%1").arg(userPoint));
                    ui->pointEdit_2->setText(QString("%1").arg(othersPoint));
                }
            }
            else // 상대가 승리일때
            {
                othersPoint++;

                if(othersPoint == 2)  // 상대가 승리하여 게임이 끝났을때
                {
                    ui->listWidget->setTextColor(QColor(255, 0, 0));
                    ui->listWidget->setText(gameLose);
                    ui->listWidget->setTextColor(QColor(0, 0, 0));
                    userPoint = 0;
                    othersPoint = 0;
                    ui->pointEdit->setText("0");
                    ui->pointEdit_2->setText("0");
                    QMessageBox endBox;
                    endBox.setWindowTitle("Notice");
                    endBox.setText("GameSet! Retry?");
                    endBox.setStandardButtons(QMessageBox::Retry | QMessageBox::Cancel);
                    endBox.setDefaultButton(QMessageBox::Retry);
                    if(endBox.exec() == QMessageBox::Cancel)
                    {
                        this->close();
                    }
                }
                else    // 게임 진행중
                {
                    ui->listWidget->setTextColor(QColor(0, 0, 255));
                    ui->listWidget->setText(lose);
                    ui->listWidget->setTextColor(QColor(0, 0, 0));
                    ui->pointEdit->setText(QString("%1").arg(userPoint));
                    ui->pointEdit_2->setText(QString("%1").arg(othersPoint));
                }
            }
        }

        // User2 포인트 획득 출력
        else if(line.left(5) == "User2")
        {
            if(userReal == "User2") // 내가 승리일때
            {
                userPoint++;

                if(userPoint == 2) // 내가 승리하여 게임이 끝났을때
                {
                    ui->listWidget->setTextColor(QColor(255, 0, 0));
                    ui->listWidget->setText(gameWin);
                    ui->listWidget->setTextColor(QColor(0, 0, 0));
                    userPoint = 0;
                    othersPoint = 0;
                    ui->pointEdit->setText("0");
                    ui->pointEdit_2->setText("0");
                    QMessageBox endBox;
                    endBox.setWindowTitle("Notice");
                    endBox.setText("GameSet! Retry?");
                    endBox.setStandardButtons(QMessageBox::Retry | QMessageBox::Cancel);
                    endBox.setDefaultButton(QMessageBox::Retry);
                    if(endBox.exec() == QMessageBox::Cancel)
                    {
                        this->close();
                    }
                }
                else    // 게임 진행중
                {
                    ui->listWidget->setTextColor(QColor(0, 0, 255));
                    ui->listWidget->setText(win);
                    ui->listWidget->setTextColor(QColor(0, 0, 0));
                    ui->pointEdit->setText(QString("%1").arg(userPoint));
                    ui->pointEdit_2->setText(QString("%1").arg(othersPoint));
                }
            }
            else // 상대가 승리일때
            {
                othersPoint++;

                if(othersPoint == 2) // 상대방이 승리하여 게임이 끝났을때
                {
                    ui->listWidget->setTextColor(QColor(255, 0, 0));
                    ui->listWidget->setText(gameLose);
                    ui->listWidget->setTextColor(QColor(0, 0, 0));
                    userPoint = 0;
                    othersPoint = 0;
                    ui->pointEdit->setText("0");
                    ui->pointEdit_2->setText("0");
                    QMessageBox endBox;
                    endBox.setWindowTitle("Notice");
                    endBox.setText("GameSet! Retry?");
                    endBox.setStandardButtons(QMessageBox::Retry | QMessageBox::Cancel);
                    endBox.setDefaultButton(QMessageBox::Retry);
                    if(endBox.exec() == QMessageBox::Cancel)
                    {
                        this->close();
                    }
                }
                else    // 게임 진행중
                {
                    ui->listWidget->setTextColor(QColor(0, 0, 255));
                    ui->listWidget->setText(lose);
                    ui->listWidget->setTextColor(QColor(0, 0, 0));
                    ui->pointEdit->setText(QString("%1").arg(userPoint));
                    ui->pointEdit_2->setText(QString("%1").arg(othersPoint));
                }
            }
        }

        else if(line.left(7) == "Connect")    // 연결 여부 확인
        {
            line=line.left(line.length()-1);
            ui->isConnect->setText(QString("%1").arg(line));
            QMessageBox connectBox;
            connectBox.setWindowTitle("Notice");
            connectBox.setText("Connect Success!");
            connectBox.exec();
            ui->listWidget->setText(gameStart);
            t.start();
            tmp = line;
        }

        else    // 랜덤 문자열 출력
        {
            line=line.left(line.length()-1);
            ui->listWidget->setText(QString("%1").arg(line));
            ui->timeEdit->setText("0");
            ui->isCorrect->setText("0");
            t.start();
            tmp = line;
        }
    }
}

void Widget::connected()
{
    socket->write(QString("/me:"+ui->messageLineEdit->text()+"\n").toUtf8());
}

void Widget::on_connectButton_clicked()
{
    socket=new QTcpSocket(this);

    connect(socket,SIGNAL(readyRead()),this,SLOT(readyRead()));
    connect(socket,SIGNAL(connected()),this,SLOT(connected()));
    socket->connectToHost(ui->ipLineEdit->text(),ui->portLineEdit->text().toInt());
}

void Widget::on_sendButton_clicked()
{
    QString message=ui->messageLineEdit->text().trimmed();

    if(sig == 0) // enter 가능
    {
        if(!message.isEmpty())
        {
            if(tmp == message)
            {
                ui->isCorrect->setText("Correct!");
                int nElapsedTime = t.elapsed();
                socket->write((QString("%1\n").arg(nElapsedTime)).toUtf8());
                nElapsedTime = (tmp.length() * 59400) / nElapsedTime;
                ui->timeEdit->setText(QString("%1").arg(nElapsedTime));
            }
            else
            {
                ui->isCorrect->setText("Incorrect!");
            }
        }
        else if(message.isEmpty())
            socket->write(QString("next\n").toUtf8());
        sig = 1;
    }
    else // enter 불가능
    {
        if(!message.isEmpty())
        {
            if(tmp == message)
            {
                ui->isCorrect->setText("Correct");
                int nElapsedTime = t.elapsed();
                socket->write((QString("%1\n").arg(nElapsedTime)).toUtf8());
                nElapsedTime = (tmp.length() * 59400) / nElapsedTime;
                ui->timeEdit->setText(QString("%1").arg(nElapsedTime));
            }
            else
            {
                ui->isCorrect->setText("Incorrect");
            }
            sig = 0;
        }
    }

    ui->messageLineEdit->clear();
    ui->messageLineEdit->setFocus();
}

void Widget::on_messageLineEdit_returnPressed()
{
    on_sendButton_clicked();
}
