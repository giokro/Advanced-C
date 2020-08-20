#include "connector.h"

Connector::Connector(QLocalSocket *sock, QThread &cThread)
{
    this->conSock = sock;

    connect(&cThread, SIGNAL(started()), this, SLOT(DoConnect()));
    // here when the thread starts, it automatically runs the DoConnect() function in our object

    this->moveToThread(&cThread); // move the object to thread
}

void Connector::DoConnect(){
    this->conSock->connectToServer("\\\\.\\pipe\\ICS0025");

    emit DoConnectDone();
}

