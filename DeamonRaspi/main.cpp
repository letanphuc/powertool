#include <QCoreApplication>

#include "src/server.h"
#include "websocketserver.h"

extern "C" void * main_f (void * params);

int main(int argc, char *argv[])
{
//    CreateServer(8);

    WebSocketServer * server = new WebSocketServer();

    main_f(NULL);
}
