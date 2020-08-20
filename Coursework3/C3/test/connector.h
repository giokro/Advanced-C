#include <QObject>
#include <QLocalSocket>
#include <QThread>

class Connector : public QObject{

    Q_OBJECT

private:
    QLocalSocket *conSock;

public:
    Connector(QLocalSocket *sock, QThread &cThread);

public slots:
    void DoConnect();

signals:
    void DoConnectDone();

};
