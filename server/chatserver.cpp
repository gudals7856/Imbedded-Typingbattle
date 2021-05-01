#include "chatserver.h"
#include <QTcpSocket>
#include <QRegExp>
#include <QTextCodec>
#include <QString>
#include <ctime>

int sum = 0;
int t1, t2;
int sig = 0;

QString st = "Connect\n";
QString win1 = "User1\n";
QString win2 = "User2\n";

QString words[10] =
    {
        "I never think of the future, it comes soon enough\n",
        "Better the last smile than the first laughter\n",
        "Only I can change my life, No one can do it for me\n",
        "Behind the cloud is the sun still shining\n",
        "Do not turn back when you are just at the goal\n",
        "A minute's success pays the failure of years\n",
        "You always pass failure on the way to success\n",
        "How you love yourself is how you teach others to love you\n",
        "Make it happen Shock everyone\n",
        "No one is you and that is your power\n",
    };

chatServer::chatServer(QObject* parent) : QTcpServer(parent)
{
    userAmount = 0;
}

void chatServer::incomingConnection(int socketfd)
{
    QTcpSocket* client=new QTcpSocket(this); //클라리언트 소켓 새성
    client->setSocketDescriptor(socketfd); //소켓드스크립터 설정
    clients.insert(client); //클라이언트 집합에 새로 들어온 클라이언트 추가


    printf("New Client from:%s\n",client->peerAddress().toString().toLocal8Bit().data());

    connect(client,SIGNAL(readyRead()),this,SLOT(readyRead()));

    connect(client,SIGNAL(disconnected()),this,SLOT(disconnected()));
}

void chatServer::readyRead()
{
    QTcpSocket* client=(QTcpSocket*)sender(); //메시지가 들어온 클라이언트를 감지하는 함수 sender
    while(client->canReadLine())
    {
        srand((unsigned int)time(NULL));
        int num = rand() % 10;
        QString line=QString::fromUtf8(client->readLine());

        printf("Read Line:%s\n",line.toLocal8Bit().data());

        QRegExp meRegex("^/me:(.*)$");

        if(meRegex.indexIn(line)!=-1) //새로 들어온 유저면
        {
            QString user=QString("%1").arg(++userAmount);
            users.insert(client, user);
            client->write((user + "\n").toUtf8());

            if(userAmount == 2)
            {
                foreach(QTcpSocket* client,clients)
                {
                    client->write(st.toUtf8()); // connect 완료 알림
                }
            }
        }
        else if(users.contains(client)) //이미 있는 유저가 메시지를 보낸경우
        {
            sum++;
            users_t.insert(client, line); //

            if(sum == 1)
            {
                if(users[client].toInt() == 1) // 먼저 보낸게 user1일때
                    t1 = line.toInt();
                else // 먼저 보낸게 user 2일때
                    t2 = line.toInt();
            }
            else if(sum == 2)
            {
                if(users[client].toInt() == 1)
                    t1 = line.toInt();
                else
                    t2 = line.toInt();

                if(sig == 0) // 양쪽유저에게 문장 전송
                {
                    foreach(QTcpSocket* otherClient,clients)
                        otherClient->write(words[num].toUtf8());
                    sig = 1;
                }
                else // 양쪽유저에게 승패여부 전송
                {
                    if(t1 < t2) // user1이 이겼을때
                    {
                        foreach(QTcpSocket* otherClient,clients)
                            otherClient->write(win1.toUtf8());
                    }
                    else // user2가 이겼을때
                    {
                        foreach(QTcpSocket* otherClient,clients)
                            otherClient->write(win2.toUtf8());
                    }
                    sig = 0;
                }
                sum = 0;
            }
            /*
            QString message = line;
            QString user = users[client];
            printf("User:%s\n",user.toLocal8Bit().data());
            printf("Message:%s\n",message.toLocal8Bit().data());
            */
            //foreach(QTcpSocket* otherClient,clients)
                //otherClient->write(QString("%2:%1").arg(message).arg(users[client]).toUtf8());
        }
        else
        {
            qWarning()<<"Got bad message from client:"<<client->peerAddress().toString()<<line;
        }
    }
}

void chatServer::disconnected()
{
    QTcpSocket* client=(QTcpSocket*)sender();
    printf("Client disconnected:%s\n",client->peerAddress().toString().toLocal8Bit().data());

    clients.remove(client);

    QString user=users[client];
    users.remove(client);

    foreach(QTcpSocket* client,clients)
    {
        client->write(QString("From server : User"+user+" exit.\n").toUtf8());
    }
}
