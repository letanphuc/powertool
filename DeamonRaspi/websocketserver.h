#ifndef WEBSOCKETSERVER_H
#define WEBSOCKETSERVER_H

#include <QObject>
#include <QList>
#include <QByteArray>

QT_FORWARD_DECLARE_CLASS(QWebSocketServer)
QT_FORWARD_DECLARE_CLASS(QWebSocket)

class WebSocketServer : public QObject
{
    Q_OBJECT
public:
    explicit WebSocketServer(QObject *parent = 0);
    virtual ~WebSocketServer();

signals:

public slots:
    void onNewConnection();
    void processMessage(QString message);
    void socketDisconnected();

private:
    QWebSocketServer * m_pWebSocketServer;
    QList<QWebSocket * > m_clients;
    int data_count;
};

#endif // WEBSOCKETSERVER_H
