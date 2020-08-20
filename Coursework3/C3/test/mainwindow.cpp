#include "mainwindow.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    sock = new QLocalSocket;

    ui->Debugger->setReadOnly(true);
    ui->Compute->setEnabled(false);
    ui->Break->setEnabled(false);

    ui->Functions->addItem("Sine integral");
    ui->Functions->addItem("Cosine integral");
    ui->Functions->addItem("Bessel function");
    ui->Functions->addItem("Fresnel integral S");
    ui->Functions->addItem("Fresnel integral C");

    connectO = new Connector(this->sock, this->cThread);

    connect(connectO, SIGNAL(DoConnectDone()), this, SLOT(on_connected()));
    // So connectO a seperate object that connects our local socket (sock) to the pipe server.
    // It has to be a seperate object because we have to move it to a thread with moveToThread(Thread &T)
    // When the object function is done, it emits the done signal and runs the ConnectionState() function.

    connect(sock, SIGNAL(readyRead()), this, SLOT(on_readyRead()));
}

MainWindow::~MainWindow()
{
    delete ui;
    delete connectO;
    delete sock;
}

bool MainWindow::inputOK(){

    bool *ok = new bool;

    QString sX0 = ui->X0->text();
    QString sXn = ui->Xn->text();
    QString snPoints = ui->nPoints->text();
    QString sOrder = ui->Order->text();

    if(sX0=="" || sXn=="" || snPoints==""){
        QMessageBox::critical(this, "Alert Empty", "Input cannot be empty!");
        return 0;
    }

    double X0 = sX0.toDouble(ok);
    if(!*ok){
        QMessageBox::critical(this,"Alert - X0", "X0 must be a double");
        return 0;
    }

    double Xn = sXn.toDouble(ok);
    if(*ok){
        if(Xn<=X0){
            QMessageBox::critical(this,"Alert - Xn", "Xn must be more than X0");
            return 0;
        }
    }else{
        QMessageBox::critical(this,"Alert - Xn", "Xn must be a double");
        return 0;
    }

    double nPoints = snPoints.toInt(ok);
    if(*ok){
        if(nPoints<=0){
            QMessageBox::critical(this,"Alert - nPoints", "nPoints must be more than 0");
            return 0;
        }
    }else{
        QMessageBox::critical(this,"Alert - nPoints", "nPoints must be an int");
        return 0;
    }

    if(ui->Functions->currentIndex() == 2){
        // Bessel
        if(sOrder==""){
            QMessageBox::critical(this, "Alert Empty", "Input cannot be empty!");
            return 0;
        }

        sOrder.toInt(ok);
        if(!*ok){
            QMessageBox::critical(this,"Alert - Order", "Order must be an int");
            return 0;
        }
    }

    return 1;
}

double MainWindow::getDouble(QVector<double> y, bool b){

    double num=y[1];
    int range = y.size()-2;

    if(b){
        for(int i=1; i<range; i++){
            if(num<y[i]){
                num = y[i];
            }
        }

    }else{
        for(int i=1; i<range; i++){
            if(num>y[i] && y[i]!=0){
                num = y[i];
            }
        }
    }
    return num;
    //ui->Debugger->appendPlainText(QString::number(num));
}

void MainWindow::stopThread(QThread *Thread){
    if(Thread->isRunning()){
        Thread->exit(); // stop the thread
        Thread->wait(); // wait for it to exit
    }
}

void MainWindow::on_Break_clicked()
{
    stopThread(&cThread);

    QByteArray bytes;

    int n = 14; // Number of bytes in the package
    char* stop = (char*)"Stop\0";   // the command itself

    bytes.append(n);    // an int is 4 bytes, so first 4 is the int
    bytes.append('\0');
    bytes.append('\0');
    bytes.append('\0');

    for(int i=0; i<4; i++){
        bytes.append(stop[i]);  // instead of char the server needs wchar_t (which is twice the size of a char)
        bytes.append('\0');     // It's 16 bits, so since we have an 8 bit char we add a 0
    }

    bytes.append('\0');     // two terminating zeroes at the end.
    bytes.append('\0');

    // so an int isn't 0 0 0 14 (in bytes), but it's the other way around: 14 0 0 0

    if(this->sock->state()==QLocalSocket::ConnectedState){
        sock->write(bytes, n);
        sock->disconnectFromServer();

        stopThread(&cThread);

        ui->Break->setEnabled(false);
        ui->Compute->setEnabled(false);

        ui->Debugger->appendPlainText("Disconnected from pipe server");
    }else{
        ui->Debugger->appendPlainText(sock->errorString());
    }    
}

void MainWindow::on_Connect_clicked()
{
    if(sock->state()!=QLocalSocket::ConnectedState){
        ui->Debugger->appendPlainText("Connecting to pipe server...");

        this->cThread.start();

        ui->Connect->setEnabled(false); // Turns off the connect btn
    }else{
        ui->Debugger->appendPlainText("Already connected to pipe");
    }
}

void MainWindow::on_connected(){

    if(this->sock->state()==QLocalSocket::ConnectedState){
        ui->Compute->setEnabled(true);
        ui->Break->setEnabled(true);
        ui->Debugger->appendPlainText("Connected");

    }else{
        ui->Debugger->appendPlainText(sock->errorString());  // writes the error in debug
        stopThread(&cThread);
    }

    ui->Connect->setEnabled(true);
}

void MainWindow::on_Exit_clicked()
{
    stopThread(&cThread);

    QApplication::exit();
}

void MainWindow::closeEvent(QCloseEvent *Event){

    stopThread(&cThread);

    QWidget::closeEvent(Event);
}

void MainWindow::on_Functions_currentIndexChanged(int index)
{
    if(index==0){
        // Sine
        ui->Order->setDisabled(true);

    }else if(index==1){
        // Cosine
        ui->Order->setDisabled(true);

    }else if(index==2){
        // Bessel
        ui->Order->setDisabled(false);

    }else if(index==3){
        // Fresnel S
        ui->Order->setDisabled(true);

    }else if(index==4){
        // Fresnel C
        ui->Order->setDisabled(true);
    }
}

void MainWindow::on_Compute_clicked()
{
    int func = ui->Functions->currentIndex();

    double X0, Xn;
    char *pD = new char[sizeof(double)]; // This is for moving through double number's bits
    int nPoints, Order;

    if(inputOK()){
        X0 = ui->X0->text().toDouble();
        Xn = ui->Xn->text().toDouble();
        nPoints = ui->nPoints->text().toInt();
        Order = ui->Order->text().toInt();
    }else{
        return;
    }

    int n;
    QByteArray bytes;
    QString sendMes;

    if(func==0){
        // “Sine integral”

        n = 52;
        char* sin = (char*)"Sine integral\0";

        for(int i=0; i<4; i++){
            bytes.append(n<<(8*i));
        }

        for(int i=0; i<13; i++){
            bytes.append(sin[i]);
            bytes.append('\0');
        }

        bytes.append('\0');
        bytes.append('\0');

        pD = (char*)&X0;
        for(int i=0; i<8; i++){
            bytes.append(pD[i]);
        }

        pD = (char*)&Xn;
        for(int i=0; i<8; i++){
            bytes.append(pD[i]);
        }

        for(int i=0; i<4; i++){
            bytes.append(nPoints<<(8*i));
        }

        sendMes.append(ui->Functions->currentText());
        sendMes.append(":  sending X0 = ");
        sendMes.append(QString::number(X0));
        sendMes.append(",  Xn = ");
        sendMes.append(QString::number(Xn));
        sendMes.append(",  nPoints = ");
        sendMes.append(QString::number(nPoints));


    }else if(func==1){
        // Cosine

        n = 56;
        char* sin = (char*)"Cosine integral\0";

        for(int i=0; i<4; i++){
            bytes.append(n<<(8*i));
        }

        for(int i=0; i<15; i++){
            bytes.append(sin[i]);
            bytes.append('\0');
        }

        bytes.append('\0');
        bytes.append('\0');

        pD = (char*)&X0;
        for(int i=0; i<8; i++){
            bytes.append(pD[i]);
        }

        pD = (char*)&Xn;
        for(int i=0; i<8; i++){
            bytes.append(pD[i]);
        }

        for(int i=0; i<4; i++){
            bytes.append(nPoints<<(8*i));
        }


        sendMes.append(ui->Functions->currentText());
        sendMes.append(":  sending X0 = ");
        sendMes.append(QString::number(X0));
        sendMes.append(",  Xn = ");
        sendMes.append(QString::number(Xn));
        sendMes.append(",  nPoints = ");
        sendMes.append(QString::number(nPoints));

    }else if(func==2){
        // Bessel

        n = 60;
        char* sin = (char*)"Bessel function\0";

        for(int i=0; i<4; i++){
            bytes.append(n<<(8*i));
        }

        for(int i=0; i<15; i++){
            bytes.append(sin[i]);
            bytes.append('\0');
        }

        bytes.append('\0');
        bytes.append('\0');

        pD = (char*)&X0;
        for(int i=0; i<8; i++){
            bytes.append(pD[i]);
        }

        pD = (char*)&Xn;
        for(int i=0; i<8; i++){
            bytes.append(pD[i]);
        }

        for(int i=0; i<4; i++){
            bytes.append(nPoints<<(8*i));
        }

        for(int i=0; i<4; i++){
            bytes.append(Order<<(8*i));
        }


        sendMes.append(ui->Functions->currentText());
        sendMes.append(":  sending X0 = ");
        sendMes.append(QString::number(X0));
        sendMes.append(",  Xn = ");
        sendMes.append(QString::number(Xn));
        sendMes.append(",  nPoints = ");
        sendMes.append(QString::number(nPoints));
        sendMes.append(",  Order = ");
        sendMes.append(QString::number(Order));

    }else if(func==3){
        // Fresnel S

        n = 62;
        char* sin = (char*)"Fresnel integral S\0";

        for(int i=0; i<4; i++){
            bytes.append(n<<(8*i));
        }

        for(int i=0; i<18; i++){
            bytes.append(sin[i]);
            bytes.append('\0');
        }

        bytes.append('\0');
        bytes.append('\0');

        pD = (char*)&X0;
        for(int i=0; i<8; i++){
            bytes.append(pD[i]);
        }

        pD = (char*)&Xn;
        for(int i=0; i<8; i++){
            bytes.append(pD[i]);
        }

        for(int i=0; i<4; i++){
            bytes.append(nPoints<<(8*i));
        }


        sendMes.append(ui->Functions->currentText());
        sendMes.append(": sending X0 = ");
        sendMes.append(QString::number(X0));
        sendMes.append(",  Xn = ");
        sendMes.append(QString::number(Xn));
        sendMes.append(",  nPoints = ");
        sendMes.append(QString::number(nPoints));

    }else if(func==4){
        // Fresnel C

        n = 62;
        char* sin = (char*)"Fresnel integral C\0";

        for(int i=0; i<4; i++){
            bytes.append(n<<(8*i));
        }

        for(int i=0; i<18; i++){
            bytes.append(sin[i]);
            bytes.append('\0');
        }

        bytes.append('\0');
        bytes.append('\0');

        pD = (char*)&X0;
        for(int i=0; i<8; i++){
            bytes.append(pD[i]);
        }

        pD = (char*)&Xn;
        for(int i=0; i<8; i++){
            bytes.append(pD[i]);
        }

        for(int i=0; i<4; i++){
            bytes.append(nPoints<<(8*i));
        }


        sendMes.append(ui->Functions->currentText());
        sendMes.append(": sending X0 = ");
        sendMes.append(QString::number(X0));
        sendMes.append(",  Xn = ");
        sendMes.append(QString::number(Xn));
        sendMes.append(",  nPoints = ");
        sendMes.append(QString::number(nPoints));

    }


    ui->Debugger->appendPlainText(sendMes);
    sock->write(bytes);
}

void MainWindow::on_readyRead(){

    int n;
    QVector<double> x, y;

    QByteArray data;
    data = sock->readAll();

    QString curve;
    for(int i=0; i<5; i++){
        curve.append((QChar)data[4+i*2]);
    }

    if(!curve.compare("Curve")){

        ui->Debugger->appendPlainText("Calculation completed");

        memcpy(&n, data, sizeof(int));

        double d;
        char * pD = new char[sizeof(double)];

        for(int i=0; i<n-16; i++){

            for(int y=0; y<8; y++){
                pD[y] = data[16+(i*8)+y];
            }

            memcpy(&d, pD, sizeof(double));

            if(i%2==0){
                // x
                x.append(d);
            }else{
                // y
                y.append(d);
            }
        }

    }else{
        ui->Debugger->appendPlainText("There was an error during the calculation");
        return;
    }


    ///// Here's the main plot

    ui->Plot->addGraph();
    ui->Plot->graph(0)->setData(x, y);

    // give the axes some labels:
    ui->Plot->xAxis->setLabel("x");
    ui->Plot->yAxis->setLabel("y");

    double X0 = ui->X0->text().toDouble();
    double Xn = ui->Xn->text().toDouble();

    // set axes ranges, so we see all data:
    ui->Plot->xAxis->setRange(X0, Xn);

    double maxy = getDouble(y, 1);
    double miny = getDouble(y, 0);
    ui->Plot->yAxis->setRange(miny, maxy);

    ui->Plot->replot();
}
