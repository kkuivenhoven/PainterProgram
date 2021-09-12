#include "mainwindow.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    // QKeyEvent *keyEvent = new QKeyEvent(this);


    scribblearea = new ScribbleArea;
    setCentralWidget(scribblearea);
    createActions();
    createMenus();
    setWindowTitle(tr("Scribble"));

    subMenu = new SubMenu(*scribblearea);

    resize(500, 500);
}

MainWindow::~MainWindow()
{
    delete ui;
    delete subMenu;
}


void MainWindow::keyPressEvent(QKeyEvent *event) {
    qDebug() << "inside keyPressEvent() \n";
    if(event->key() == Qt::Key_1) {
        qDebug() << "inside key one \n";
        scribblearea->drawRectangle();
    }
}


void MainWindow::closeEvent(QCloseEvent *event) {
    if(maybeSave()) {
        event->accept();
    } else {
        event->ignore();
    }
}

void MainWindow::open() {
    if(maybeSave()) {
        QString fileName = QFileDialog::getOpenFileName(this,
                        tr("Open File"), QDir::currentPath());
        if(!fileName.isEmpty()) {
            scribblearea->openImage(fileName);
        }
    }
}

void MainWindow::save() {
    QAction *action = qobject_cast<QAction *>(sender());
    QByteArray fileFormat = action->data().toByteArray();
    saveFile(fileFormat);
}

void MainWindow::penColor() {
    QColor newColor = QColorDialog::getColor(scribblearea->penColor());
    if(newColor.isValid()) {
        scribblearea->setPenColor(newColor);
    }
}

void MainWindow::fillEasel() {
    scribblearea->setEasel(scribblearea->penColor());
}

void MainWindow::penWidth() {
    bool ok;
    int newWidth = QInputDialog::getInt(this, tr("Scribble"),
                                        tr("Select pen width : "),
                                        scribblearea->penWidth(),
                                        1, 50, 1, &ok);
    if(ok) {
        scribblearea->setPenWidth(newWidth);
    }
}

void MainWindow::about() {
    QMessageBox::about(this, tr("About Scribble"),
           tr("<p>The <b>Scribble</b> example is awesome</p>"));
}

void MainWindow::createActions() {
    openAction = new QAction(tr("&Open"), this);

    openAction->setShortcuts(QKeySequence::Open);

    connect(openAction, SIGNAL(triggered(bool)), this, SLOT(open()));

    foreach(QByteArray format, QImageWriter::supportedImageFormats()) {
        QString text = tr("%1...").arg(QString(format).toUpper());

        QAction *action = new QAction(text, this);

        action->setData(format);

        connect(action, SIGNAL(triggered()), this, SLOT(save()));

        saveAsActs.append(action);
    }
    printAct = new QAction(tr("&Print..."), this);
    connect(printAct, SIGNAL(triggered()), scribblearea, SLOT(print()));

    exitAction = new QAction(tr("&Exit"), this);
    exitAction->setShortcuts(QKeySequence::Quit);
    connect(exitAction, SIGNAL(triggered()), this, SLOT(close()));

    penColorAct = new QAction(tr("&Pen color..."), this);
    connect(penColorAct, SIGNAL(triggered()), this, SLOT(penColor()));

    fillEaselAct = new QAction(tr("&Fill easel..."), this);
    connect(fillEaselAct, SIGNAL(triggered()), this, SLOT(fillEasel()));

    penWidthAct = new QAction(tr("Pen &Width..."), this);
    connect(penColorAct, SIGNAL(triggered()), this, SLOT(penWidth()));

    clearScreenAct = new QAction(tr("&Clear Screen..."), this);
    clearScreenAct->setShortcut(tr("Ctrl+L"));
    connect(clearScreenAct, SIGNAL(triggered()), scribblearea, SLOT(clearImage()));

    aboutAct = new QAction(tr("&About..."), this);
    connect(aboutAct, SIGNAL(triggered()), scribblearea, SLOT(about()));

    aboutQtAct = new QAction(tr("About &Qt..."), this);
    connect(aboutQtAct, SIGNAL(triggered()), qApp, SLOT(aboutQt()));
}

void MainWindow::createMenus() {
    saveAsMenu = new QMenu(tr("&Save As"), this);
    foreach(QAction *action, saveAsActs) {
        saveAsMenu->addAction(action);
    }
    fileMenu = new QMenu(tr("&File"), this);
    fileMenu->addAction(openAct);
    fileMenu->addMenu(saveAsMenu);
    fileMenu->addAction(printAct);
    fileMenu->addSeparator();
    fileMenu->addAction(exitAction);

    optionMenu = new QMenu(tr("&Options"), this);
    optionMenu->addAction(penColorAct);
    optionMenu->addAction(penWidthAct);
    optionMenu->addAction(fillEaselAct);
    optionMenu->addSeparator();
    optionMenu->addAction(clearScreenAct);

    helpMenu = new QMenu(tr("&Help"), this);
    helpMenu->addAction(aboutAct);
    helpMenu->addAction(aboutQtAct);

    menuBar()->addMenu(fileMenu);
    menuBar()->addMenu(optionMenu);
    menuBar()->addMenu(helpMenu);
}

bool MainWindow::eventFilter(QObject *watched, QEvent *event) {
    return true;
}

bool MainWindow::maybeSave() {
    if(scribblearea->isModified()) {
        QMessageBox::StandardButton ret;
        ret = QMessageBox::warning(this, tr("Scribble"),
                                   tr("The image has been modified.\n"
                                      "Do you want to save your changes?"),
                                   QMessageBox::Save | QMessageBox::Discard | QMessageBox::Cancel);
        if(ret == QMessageBox::Save) {
            return saveFile("png");
        } else if(ret == QMessageBox::Cancel) {
            return false;
        }
    }
    return true;
}


bool MainWindow::saveFile(const QByteArray &fileFormat) {
    QString initialPath = QDir::currentPath() + "/untitled." + fileFormat;
    QString fileName = QFileDialog::getSaveFileName(this, tr("Save As"),
                                                    initialPath,
                                                    tr("%1 Files (*.%2);; All Files(*)")
                                                    .arg(QString::fromLatin1(fileFormat.toUpper()))
                                                    .arg(QString::fromLatin1(fileFormat)));
    if(fileName.isEmpty()) {
        return false;
    } else {
        return scribblearea->saveImage(fileName, fileFormat.constData());
    }
}

