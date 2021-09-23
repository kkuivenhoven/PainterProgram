#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QList>
#include <QFileDialog>
#include <QColorDialog>
#include <QInputDialog>
#include <QMessageBox>
#include <QImageWriter>
#include <QMenu>
#include <QMdiSubWindow>
#include <QKeyEvent>
#include <QQuickView>
#include <QtQml>

#include "scribblearea.h"
#include "submenu.h"

class ScribbleArea;

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();

    void keyPressEvent(QKeyEvent *event) override;
    bool eventFilter(QObject *watched, QEvent *event) override;

protected:
    void closeEvent(QCloseEvent *event) override; // used to stop an event from occurring

private slots:
    void open();
    void save();
    void penColor();
    void penWidth();
    void about();
    void fillEasel();

private:
    Ui::MainWindow *ui;
    void createActions();
    void createMenus();

    bool maybeSave();

    bool saveFile(const QByteArray &fileFormat);

    ScribbleArea *scribblearea;
    QMenu *saveAsMenu;
    QMenu *fileMenu;
    QMenu *optionMenu;
    QMenu *helpMenu;

    QAction *openAction;

    QList<QAction *> saveAsActs;
    QAction *penWidthAct;
    QAction *clearScreenAct;
    QAction *aboutAct;
    QAction *aboutQtAct;
    QAction *exitAction;
    QAction *printAct;
    QAction *penColorAct;
    QAction *filLEaselAct;
    QAction *openAct;
    QAction *fillEaselAct;

    SubMenu *subMenu;

};

#endif // MAINWINDOW_H
