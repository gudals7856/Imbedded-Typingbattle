#include <QCoreApplication>
#include "chatserver.h"

int main(int argc, char *argv[])
{
    QCoreApplication a(argc, argv);

    chatServer* server=new chatServer();
    bool success=server->listen(QHostAddress::Any,4200);
    if(!success)
    {
        printf("Could not listen on port 4200.\n");
        return 0;
    }

    printf("Ready\n");

    return a.exec();
}
