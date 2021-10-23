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

    ScribbleArea *m_scribblearea;
    QMenu *m_saveAsMenu;
    QMenu *m_fileMenu;
    QMenu *m_optionMenu;
    QMenu *m_helpMenu;

    QAction *m_openAction;

    QList<QAction *> m_saveAsActs;
    QAction *m_penWidthAct;
    QAction *m_clearScreenAct;
    QAction *m_aboutAct;
    QAction *m_aboutQtAct;
    QAction *m_exitAction;
    QAction *m_printAct;
    QAction *m_penColorAct;
    QAction *m_filLEaselAct;
    QAction *m_openAct;
    QAction *m_fillEaselAct;

    SubMenu *m_subMenu;

};

#endif // MAINWINDOW_H
