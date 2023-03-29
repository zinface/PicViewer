#include "mainwindow.h"
#include "ui_mainwindow.h"

QRect labelbeginrect;
QString imgfile;
//QString ldfile;
int imgnum = 0;
QStringList files = {};
QPoint mousepoint;
bool ifpress = false;
bool showing = false;
QImage img;

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    ui->label->installEventFilter(this);
    labelbeginrect = QRect(ui->label->x(),ui->label->y(),ui->label->width(),ui->label->height());
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::on_pushButton_clicked()
{
    files.append(QFileDialog::getOpenFileNames(this,"选择图片","./","Images(*.bmp *.gif *.jpg *.jpeg *.png *.tiff *.pbm *.pgm *.ppm *.xbm *.xpm)"));
    if(files.isEmpty())
    {
        QMessageBox Box;
        Box.setText("没有选中文件!");
        Box.exec();
        return;
    }
    for(int i = 0; i <= files.count() - 1; i++)
    {
        for(int s = i + 1; s <= files.count() - 1; s++)
        {
            if(files[i] == files[s])
            {
                files.removeAt(s);
            }
        }
    }
    imgnum = 0;
    imgfile = files[imgnum];
    ShowPic(imgfile, labelbeginrect);
    while(ui->PicList->count())
    {
        ui->PicList->setCurrentRow(0);
        QListWidgetItem* item = ui->PicList->currentItem();
        ui->PicList->takeItem(0);
        delete item;
    }
    for(int i = 0; i <= files.count() - 1; i++)
    {
        ui->PicList->addItem(files[i].mid(files[i].lastIndexOf('/') + 1));
    }
    //ui->PicList->addItems(files);
    ui->PicList->setCurrentRow(imgnum);
}

void MainWindow::ShowPic(QString file, QRect rect)
{
    //ldfile = file;
    ui->label->setGeometry(rect);
    std::cout << "执行显示\n";
    img.load(file);
    //int Iwidth=img.width(),Iheight=img.height();
    img = img.scaled(ui->label->size(), Qt::KeepAspectRatio, Qt::SmoothTransformation);
    ui->label->setPixmap(QPixmap::fromImage(img));
    showing = true;
}

void MainWindow::on_PrePic_clicked()
{
    QMessageBox Box;
    if(files.isEmpty())
    {
        Box.setText("列表中没有文件!");
        Box.exec();
        return;
    }
    //int sumnum = files.count();
    if(imgnum == 0)
    {
        Box.setText("已经是第一张了!");
        Box.exec();
        return;
    }
    imgnum--;
    ui->PicList->setCurrentRow(imgnum);
    imgfile = files[imgnum];
    ShowPic(imgfile, labelbeginrect);
}

void MainWindow::on_NextPic_clicked()
{
    QMessageBox Box;
    if(files.isEmpty())
    {
        Box.setText("列表中没有文件!");
        Box.exec();
        return;
    }
    int sumnum = files.count();
    if(imgnum == sumnum - 1)
    {
        Box.setText("已经是最后一张了!");
        Box.exec();
        return;
    }
    imgnum++;
    ui->PicList->setCurrentRow(imgnum);
    imgfile = files[imgnum];
    ShowPic(imgfile, labelbeginrect);
}


void MainWindow::on_PicList_itemDoubleClicked(QListWidgetItem *item)
{
    //std::cout << ui->PicList->count() << "\n";
    imgnum = ui->PicList->currentRow();
    imgfile = files[imgnum];
    ShowPic(imgfile, labelbeginrect);
}


void MainWindow::mousePressEvent(QMouseEvent *event)
{
    if(event->button() == Qt::LeftButton)
    {
        mousepoint = QPoint(event->x(),event->y());
        if(!(mousepoint.x() < ui->label->x() || mousepoint.x() > ui->label->x() + ui->label->width() || mousepoint.y() < ui->label->y() || mousepoint.y() > ui->label->y() + ui->label->height()))
        {
            ifpress = true;
        }
    }
}

void MainWindow::mouseMoveEvent(QMouseEvent *event)
{
    if(ifpress && showing)
    {
        //qDebug() << "hello\n";
        QPoint presspoint(event->x(),event->y());
        int cx,cy;
        cx = presspoint.x() - mousepoint.x();
        cy = presspoint.y() - mousepoint.y();
        ui->label->move(ui->label->x() + cx, ui->label->y() + cy);
        mousepoint = presspoint;
    }
}

void MainWindow::mouseReleaseEvent(QMouseEvent *event)
{
    ifpress = false;
    //qDebug() << "done\n";
}

void MainWindow::wheelEvent(QWheelEvent *event)
{
    if(!ifpress || !showing)
    {
        return;
    }
    if(!(mousepoint.x() < ui->label->x() || mousepoint.x() > ui->label->x() + ui->label->width() || mousepoint.y() < ui->label->y() || mousepoint.y() > ui->label->y() + ui->label->height()))
    {
        int px;
        if(event->angleDelta().y() < 0)
        {
            px = 150;
        }
        else
        {
            px = -150;
        }
        int labelw = ui->label->width();
        int labelh = ui->label->height();
        int slabelw = labelw + px;
        int slabelh = slabelw*ui->label->height()/ui->label->width();
        QPoint slabelpoint(mousepoint.x()-(double(mousepoint.x()-ui->label->x())/ui->label->width())*slabelw,mousepoint.y()-(double(mousepoint.y()-ui->label->y())/ui->label->height())*slabelh);
        ShowPic(imgfile, QRect(slabelpoint.x(),slabelpoint.y(),slabelw,slabelh));
    }
}

