#include <QCoreApplication>

#include "src/server.h"
#include "websocketserver.h"
#include <pthread.h>

extern "C" void * main_f (void * params);

int main(int argc, char *argv[])
{

	QCoreApplication a(argc, argv);
    WebSocketServer * server = new WebSocketServer();

    pthread_t tID;
    pthread_create(&tID, NULL, &main_f, NULL);

    return a.exec();
}
