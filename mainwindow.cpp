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
    QDir dir("./files");
    if(dir.exists()){
        // 设置需要匹配的文件扩展名
        QStringList filters;
        filters << "*.bmp" << "*.gif" << "*.jpg" << "*.jpeg" << "*.png"
                << "*.tiff" << "*.pbm" << "*.pgm" << "*.ppm" << "*.xbm" << "*.xpm";

        // 获取匹配条件下的文件路径列表
        QStringList fileList = dir.entryList(filters, QDir::Files);

        // 将文件名补充为完整路径
        //QStringList fullFilePaths;
        for (const QString &file : fileList) {
            files << dir.absoluteFilePath(file);
        }
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
    files.sort();
    imgnum = 0;
    imgfile = files[imgnum];
    ui->Num->setText("目前显示第"+QString::number(imgnum+1)+"张图片");
    ShowPic(imgfile, labelbeginrect);
    while(ui->PicList->count())
    {
        ui->PicList->setCurrentRow(0);
        QListWidgetItem* item = ui->PicList->currentItem();
        ui->PicList->takeItem(0);
        delete item;
    }
    ui->PicList->clear();
    for(int i = 0; i <= files.count() - 1; i++)
    {
        QString qstr = files[i].mid(files[i].lastIndexOf('/') + 1);
        qstr.chop(4);
        ui->PicList->addItem(qstr);
    }
    //ui->PicList->addItems(files);
    ui->PicList->setCurrentRow(imgnum);
    }
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::on_open_clicked()
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
    files.sort();
    imgnum = 0;
    imgfile = files[imgnum];
    ui->Num->setText("目前显示第"+QString::number(imgnum+1)+"张图片");
    ShowPic(imgfile, labelbeginrect);
    while(ui->PicList->count())
    {
        ui->PicList->setCurrentRow(0);
        QListWidgetItem* item = ui->PicList->currentItem();
        ui->PicList->takeItem(0);
        delete item;
    }
    ui->PicList->clear();
    for(int i = 0; i <= files.count() - 1; i++)
    {
        QString qstr = files[i].mid(files[i].lastIndexOf('/') + 1);
        qstr.chop(4);
        ui->PicList->addItem(qstr);
    }
    //ui->PicList->addItems(files);
    ui->PicList->setCurrentRow(imgnum);
}

void MainWindow::ShowPic(QString file, QRect rect)
{
    //ldfile = file;
    ui->label->setGeometry(rect);
    //std::cout << "执行显示\n";
    img.load(file);
    //int Iwidth=img.width(),Iheight=img.height();
    img = img.scaled(ui->label->size(), Qt::KeepAspectRatio, Qt::SmoothTransformation);
    ui->label->setPixmap(QPixmap::fromImage(img));
    showing = true;
}

void MainWindow::on_PrePic_clicked(){
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
    ui->Num->setText("目前显示第"+QString::number(imgnum+1)+"张图片");
    getText(imgfile);
}

void MainWindow::getText(QString imgfile)
{
    QString textStr = imgfile.mid(imgfile.lastIndexOf('/') + 1);
    textStr.chop(4);
    textStr = imgfile.mid(0,imgfile.lastIndexOf('/') + 1) + textStr + ".txt";
    qDebug() << textStr;
    ui->text->setPlainText("");
    QFile file(textStr);
    // 尝试打开文件，确保它以只读模式打开
    if (!file.open(QIODevice::ReadOnly | QIODevice::Text)) {
        ui->text->insertPlainText("无法打开文件");
        return; // 退出程序，文件无法打开
    }

    // 使用 QTextStream 来读取文件内容
    QTextStream in(&file);
    while (!in.atEnd()) {
        QString line = in.readLine();  // 逐行读取
        ui->text->insertPlainText(line);
    }

    file.close();  // 关闭文件
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
    ui->Num->setText("目前显示第"+QString::number(imgnum+1)+"张图片");
    getText(imgfile);
}


void MainWindow::on_PicList_itemDoubleClicked(QListWidgetItem *item)
{
    //std::cout << ui->PicList->count() << "\n";
    imgnum = ui->PicList->currentRow();
    imgfile = files[imgnum];
    ShowPic(imgfile, labelbeginrect);
    ui->Num->setText("目前显示第"+QString::number(imgnum+1)+"张图片");
    getText(imgfile);
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
        int labelw = ui->label->width();
        int labelh = ui->label->height();
        if(event->angleDelta().y() < 0)
        {
            px = labelw * 0.1;
        }
        else
        {
            px = -labelw * 0.1;
        }
        int slabelw = labelw + px;
        int slabelh = slabelw*ui->label->height()/ui->label->width();
        QPoint slabelpoint(mousepoint.x()-(double(mousepoint.x()-ui->label->x())/ui->label->width())*slabelw,mousepoint.y()-(double(mousepoint.y()-ui->label->y())/ui->label->height())*slabelh);
        ShowPic(imgfile, QRect(slabelpoint.x(),slabelpoint.y(),slabelw,slabelh));
    }
}


void MainWindow::on_openDir_clicked()
{
    // 打开选择文件夹的对话框
    QString folderPath = QFileDialog::getExistingDirectory(this, "选择文件夹", QString(), QFileDialog::ShowDirsOnly | QFileDialog::DontResolveSymlinks);

    if (!folderPath.isEmpty()) {
        // 创建 QDir 对象
        QDir dir(folderPath);

        // 设置需要匹配的文件扩展名
        QStringList filters;
        filters << "*.bmp" << "*.gif" << "*.jpg" << "*.jpeg" << "*.png"
                << "*.tiff" << "*.pbm" << "*.pgm" << "*.ppm" << "*.xbm" << "*.xpm";

        // 获取匹配条件下的文件路径列表
        QStringList fileList = dir.entryList(filters, QDir::Files);

        // 将文件名补充为完整路径
        //QStringList fullFilePaths;
        for (const QString &file : fileList) {
            files << dir.absoluteFilePath(file);
        }

        // 输出结果
        /*
        qDebug() << "找到的图像文件路径：";
        for (const QString &filePath : files) {
            qDebug() << filePath;
        }
        */
    } else {
        QMessageBox Box;
        Box.setText("没有选中文件夹!");
        Box.exec();
        return;
    }
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
    files.sort();
    imgnum = 0;
    imgfile = files[imgnum];
    ui->Num->setText("目前显示第"+QString::number(imgnum+1)+"张图片");
    ShowPic(imgfile, labelbeginrect);
    while(ui->PicList->count())
    {
        ui->PicList->setCurrentRow(0);
        QListWidgetItem* item = ui->PicList->currentItem();
        ui->PicList->takeItem(0);
        delete item;
    }
    ui->PicList->clear();
    for(int i = 0; i <= files.count() - 1; i++)
    {
        QString qstr = files[i].mid(files[i].lastIndexOf('/') + 1);
        qstr.chop(4);
        ui->PicList->addItem(qstr);
    }
    //ui->PicList->addItems(files);
    ui->PicList->setCurrentRow(imgnum);
}


void MainWindow::on_Jmp_clicked()
{
    ui->PicList->setCurrentRow(imgnum);
}


void MainWindow::on_Search_clicked()
{
    QString itemName;
    int count = ui->PicList->count();
    for (int i = 0; i < count; ++i) {
        QListWidgetItem *item = ui->PicList->item(i);
        itemName = item->text();
        if(itemName.contains(ui->picName->toPlainText(), Qt::CaseInsensitive)){
            ui->PicList->setCurrentRow(i);
            break;
        }
    }

}

