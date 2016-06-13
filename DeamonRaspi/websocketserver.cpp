#include "websocketserver.h"
#include "QtWebSockets/QWebSocketServer"
#include "QtWebSockets/QWebSocket"
#include <QtCore/QDebug>
#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include <sys/time.h>

#define SIMULATE_DATA 0

#if SIMULATE_DATA
#else
#include "src/devices.h"
#endif

QT_USE_NAMESPACE

WebSocketServer::WebSocketServer(QObject *parent) : QObject(parent),
    m_pWebSocketServer(nullptr),
    m_clients()
{
    m_pWebSocketServer = new QWebSocketServer(QStringLiteral("Data Server"),
                                              QWebSocketServer::NonSecureMode,
                                              this);
    if (m_pWebSocketServer->listen(QHostAddress::Any, 5439))
    {
        qDebug() << "Chat Server listening on port" << 5439;
        connect(m_pWebSocketServer, &QWebSocketServer::newConnection,
                this, &WebSocketServer::onNewConnection);
    }
    data_count = 1;

}

WebSocketServer::~WebSocketServer()
{
    m_pWebSocketServer->close();
    qDeleteAll(m_clients.begin(), m_clients.end());
}


void WebSocketServer::onNewConnection()
{
    QWebSocket *pSocket = m_pWebSocketServer->nextPendingConnection();

    connect(pSocket, &QWebSocket::textMessageReceived, this, &WebSocketServer::processMessage);
    connect(pSocket, &QWebSocket::disconnected, this, &WebSocketServer::socketDisconnected);

    m_clients << pSocket;
}

//#if SIMULATE_DATA
//#else
//extern "C" struct Device dev_host[DEV_HOST_NUMBER];
//#endif

typedef struct {
    QString sensor_id;
    QString sensor_type;
} SensorInfos;

static QList<SensorInfos> listOfCurrentSensor;

void WebSocketServer::processMessage(QString message)
{
    QWebSocket *pSender = qobject_cast<QWebSocket *>(sender());
    qDebug() << Q_FUNC_INFO << ": msg=" << message;
    if (message == "getdata")
    {
        char msg[128];
        char curr_time_str[32];
        const char * msg_format = "%d,'%s',%0.3f,%0.3f,%0.3f,%0.3f|";

        time_t t = time(NULL);
        struct tm tm;

#if SIMULATE_DATA
        float f1, f2, f3, f4;
#endif

        memset(curr_time_str, 0, sizeof(curr_time_str));

        tm = *localtime(&t);

        sprintf(curr_time_str, "%d-%d-%d %d:%d:%d",
                tm.tm_year + 1900, tm.tm_mon + 1, tm.tm_mday, tm.tm_hour, tm.tm_min, tm.tm_sec);

        memset(msg, 0, sizeof(msg));
#if SIMULATE_DATA
        srand(time(NULL));
#include "math.h"
        f1 = 100 * sin(data_count / 1.0);
        f2 = 110 * sin(data_count / 10.0);
        f3 = 120 * sin(data_count / 100.0);
        f4 = 140 * sin(data_count / 1000.0);
        sprintf(msg, msg_format, data_count++,
                curr_time_str,
                f1, f2, f3, f4);
#else
        union float_s dev1_data, dev2_data, dev3_data, dev4_data;

        dev1_data.b[0] = dev_host[0].dev_data.data[3];
        dev1_data.b[1] = dev_host[0].dev_data.data[2];
        dev1_data.b[2] = dev_host[0].dev_data.data[1];
        dev1_data.b[3] = dev_host[0].dev_data.data[0];

        dev2_data.b[0] = dev_host[1].dev_data.data[3];
        dev2_data.b[1] = dev_host[1].dev_data.data[2];
        dev2_data.b[2] = dev_host[1].dev_data.data[1];
        dev2_data.b[3] = dev_host[1].dev_data.data[0];

        dev3_data.b[0] = dev_host[2].dev_data.data[3];
        dev3_data.b[1] = dev_host[2].dev_data.data[2];
        dev3_data.b[2] = dev_host[2].dev_data.data[1];
        dev3_data.b[3] = dev_host[2].dev_data.data[0];

        dev4_data.b[0] = dev_host[3].dev_data.data[3];
        dev4_data.b[1] = dev_host[3].dev_data.data[2];
        dev4_data.b[2] = dev_host[3].dev_data.data[1];
        dev4_data.b[3] = dev_host[3].dev_data.data[0];

        sprintf(msg, msg_format, data_count++,
                curr_time_str,
                dev1_data.f, dev2_data.f, dev3_data.f, dev4_data.f);
#endif

        pSender->sendTextMessage(QString(msg));
    }
    else if (message == "sensor"){
        /**
         * Data format:
         * sensor id, type | sensor id, type | ...
         */
#if SIMULATE_DATA
        static int test = 0;
        if (test < 4){
            /** Add sensor */
            SensorInfos sensor;
            sensor.sensor_id = QString::number(test);
            sensor.sensor_type = "type:" + QString::number(test);
            listOfCurrentSensor.append(sensor);
        }
        else {
            listOfCurrentSensor.removeFirst();
        }
        test = (test + 1) % 8;
#else
        int i;
        union int_s unique_number;

        listOfCurrentSensor.clear();
        for (i = 0; i < DEV_HOST_NUMBER; i++)
        {
            if (dev_host[i].last_try <= 3)
            {
                SensorInfos sensor;

                unique_number.b[0] = dev_host[i].dev_data.unique_number[0];
                unique_number.b[1] = dev_host[i].dev_data.unique_number[1];
                unique_number.b[2] = dev_host[i].dev_data.unique_number[2];
                unique_number.b[3] = dev_host[i].dev_data.unique_number[3];

                sensor.sensor_id = QString::number(unique_number.n);
                sensor.sensor_type = "type:" + QString::number(dev_host[i].dev_data.id);
                listOfCurrentSensor.append(sensor);
            }
        }
#endif


        /** Prepare message reply */
        QString reply = "";
        for (int i = 0; i < listOfCurrentSensor.length(); i++){
            SensorInfos s = listOfCurrentSensor.at(i);
            reply += s.sensor_id + "," + s.sensor_type + "|";
        }

        pSender->sendTextMessage(reply);
    }
    else {
        qDebug() << Q_FUNC_INFO << ": Unknown message";
    }
}

void WebSocketServer::socketDisconnected()
{
    QWebSocket *pClient = qobject_cast<QWebSocket *>(sender());
    if (pClient)
    {
        m_clients.removeAll(pClient);
        pClient->deleteLater();
    }
}

