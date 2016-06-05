#include <QCoreApplication>

#include "server.h"
#include "websocketserver.h"

int main(int argc, char *argv[])
{
    QCoreApplication a(argc, argv);

    CreateServer(8);

    WebSocketServer * server = new WebSocketServer();

    return a.exec();
}
