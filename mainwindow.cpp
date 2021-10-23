#include "mainwindow.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    m_scribblearea = new ScribbleArea(this);
    setCentralWidget(m_scribblearea);
    // createActions();
    // createMenus();
    setWindowTitle(tr("Scribble"));

    m_subMenu = new SubMenu(*m_scribblearea);

    resize(500, 500);
}


MainWindow::~MainWindow()
{
    delete ui;
    delete m_subMenu;
}


void MainWindow::keyPressEvent(QKeyEvent *event) {
    Q_UNUSED(event);
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
            m_scribblearea->openImage(fileName);
        }
    }
}

void MainWindow::save() {
    QAction *action = qobject_cast<QAction *>(sender());
    QByteArray fileFormat = action->data().toByteArray();
    saveFile(fileFormat);
}

void MainWindow::penColor() {
    QColor newColor = QColorDialog::getColor(m_scribblearea->penColor());
    if(newColor.isValid()) {
        m_scribblearea->setPenColor(newColor);
    }
}

void MainWindow::fillEasel() {
    m_scribblearea->setEasel(m_scribblearea->penColor());
}

void MainWindow::penWidth() {
    bool ok;
    int newWidth = QInputDialog::getInt(this, tr("Scribble"),
                                        tr("Select pen width : "),
                                        m_scribblearea->penWidth(),
                                        1, 50, 1, &ok);
    if(ok) {
        m_scribblearea->setPenWidth(newWidth);
    }
}

void MainWindow::about() {
    QMessageBox::about(this, tr("About Scribble"),
           tr("<p>The <b>Scribble</b> example is awesome</p>"));
}

void MainWindow::createActions() {
    m_openAction = new QAction(tr("&Open"), this);

    m_openAction->setShortcuts(QKeySequence::Open);

    connect(m_openAction, SIGNAL(triggered(bool)), this, SLOT(open()));

    foreach(QByteArray format, QImageWriter::supportedImageFormats()) {
        QString text = tr("%1...").arg(QString(format).toUpper());

        QAction *action = new QAction(text, this);

        action->setData(format);

        connect(action, SIGNAL(triggered()), this, SLOT(save()));

        m_saveAsActs.append(action);
    }
    m_printAct = new QAction(tr("&Print..."), this);
    connect(m_printAct, SIGNAL(triggered()), m_scribblearea, SLOT(print()));

    m_exitAction = new QAction(tr("&Exit"), this);
    m_exitAction->setShortcuts(QKeySequence::Quit);
    connect(m_exitAction, SIGNAL(triggered()), this, SLOT(close()));

    m_penColorAct = new QAction(tr("&Pen color..."), this);
    connect(m_penColorAct, SIGNAL(triggered()), this, SLOT(penColor()));

    m_penWidthAct = new QAction(tr("Pen &Width..."), this);
    connect(m_penColorAct, SIGNAL(triggered()), this, SLOT(penWidth()));

    m_clearScreenAct = new QAction(tr("&Clear Screen..."), this);
    m_clearScreenAct->setShortcut(tr("Ctrl+L"));
    connect(m_clearScreenAct, SIGNAL(triggered()), m_scribblearea, SLOT(clearImage()));

    m_aboutAct = new QAction(tr("&About..."), this);
    connect(m_aboutAct, SIGNAL(triggered()), m_scribblearea, SLOT(about()));

    m_aboutQtAct = new QAction(tr("About &Qt..."), this);
    connect(m_aboutQtAct, SIGNAL(triggered()), qApp, SLOT(aboutQt()));
}

void MainWindow::createMenus() {
    m_saveAsMenu = new QMenu(tr("&Save As"), this);
    foreach(QAction *action, m_saveAsActs) {
        m_saveAsMenu->addAction(action);
    }
    m_fileMenu = new QMenu(tr("&File"), this);
    m_fileMenu->addAction(m_openAct);
    m_fileMenu->addMenu(m_saveAsMenu);
    m_fileMenu->addAction(m_printAct);
    m_fileMenu->addSeparator();
    m_fileMenu->addAction(m_exitAction);

    m_optionMenu = new QMenu(tr("&Options"), this);
    m_optionMenu->addAction(m_penColorAct);
    m_optionMenu->addAction(m_penWidthAct);
    m_optionMenu->addSeparator();
    m_optionMenu->addAction(m_clearScreenAct);

    m_helpMenu = new QMenu(tr("&Help"), this);
    m_helpMenu->addAction(m_aboutAct);
    m_helpMenu->addAction(m_aboutQtAct);

    menuBar()->addMenu(m_fileMenu);
    menuBar()->addMenu(m_optionMenu);
    menuBar()->addMenu(m_helpMenu);
}

bool MainWindow::eventFilter(QObject *watched, QEvent *event) {
    return true;
}

bool MainWindow::maybeSave() {
    if(m_scribblearea->isModified()) {
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
        return m_scribblearea->saveImage(fileName, fileFormat.constData());
    }
}

