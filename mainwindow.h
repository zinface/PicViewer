#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QFileDialog>
#include <iostream>
#include <QMessageBox>
#include <QListWidget>
#include <QMouseEvent>
#include <QWheelEvent>
#include <QPoint>
#include <QRect>
//#include <QSet>

QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

private slots:
    void ShowPic(QString file, QRect rect);

    void on_pushButton_clicked();

    void on_PrePic_clicked();

    void on_NextPic_clicked();

    void on_PicList_itemDoubleClicked(QListWidgetItem *item);

    void mousePressEvent(QMouseEvent *event);

    void mouseMoveEvent(QMouseEvent *event);

    void mouseReleaseEvent(QMouseEvent *event);

    void wheelEvent(QWheelEvent *event);

private:
    Ui::MainWindow *ui;

};
#endif // MAINWINDOW_H
