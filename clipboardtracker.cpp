#include "clipboardtracker.h"
#include "ui_clipboardtracker.h"

ClipboardTracker::ClipboardTracker(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::ClipboardTracker)
{
    ui->setupUi(this);
    //clipboard = new QClipboard(this);
    clipboard = QGuiApplication::clipboard();
    trayIcon = new QSystemTrayIcon(this);
    trayIcon->setIcon(QIcon(":/clipboard.png"));
    trayIcon->setVisible(true);
    connect(trayIcon, &QSystemTrayIcon::activated, this, &ClipboardTracker::trayIcon_doubleCLicked);

    connect(clipboard, SIGNAL(dataChanged()), this, SLOT(on_clipboard_changed()));
}

ClipboardTracker::~ClipboardTracker()
{
    delete ui;
}

void ClipboardTracker::closeEvent(QCloseEvent *event){
    event->setAccepted(false);
    this->setVisible(false);

}

void ClipboardTracker::on_clipboardList_currentItemChanged(QListWidgetItem *current, QListWidgetItem *previous)
{
    qDebug() << "Current Item Changed";
    if(ui->clipboardList->currentRow() > -1){
        //ui->prevBox->setPixmap(QPixmap());
        //ui->prevBox->setText(QString());
        qDebug() << "Processing new item";
        ClipboardData *data =  dynamic_cast<ClipboardData *>(ui->clipboardList->itemWidget(current));
        if(!data->getText().isNull()){
            qDebug() << "No pixmap detected";
            ui->prevBox->setText(data->getText());

        }
        else if(!data->getPixmap().isNull()){
            currentZoom = 1.00;
            qDebug() << "Pixmap detected";
            ui->prevBox->setPixmap(data->pixmap.scaled(ui->prevBox->size(), Qt::KeepAspectRatio));
        }
    }
}

void ClipboardTracker::on_clipboard_changed(){
    if(HandleCopy){

        qDebug() << "Clipboard data changed. New data: " + clipboard->text();

        ClipboardData *data = new ClipboardData();

        if(!clipboard->image().isNull()){
            data->pixmap = QPixmap::fromImage(clipboard->image());
            data->setPixmap(clipboard->image());

            QListWidgetItem *i = new QListWidgetItem("");
            data->setPixmap(data->pixmap.toImage().scaled(300, 300,
                                                          Qt::KeepAspectRatio));
            data->resize(data->pixmap.toImage().scaled(300, 300,
                                                       Qt::KeepAspectRatio).size());
            ;
            i->setSizeHint(data->size());


            ui->clipboardList->addItem(i);
            ui->clipboardList->setItemWidget(i, data);
        }
        else{

            data->setText(clipboard->text());
            QListWidgetItem *i = new QListWidgetItem("");
            i->setSizeHint(data->sizeHint());
            ui->clipboardList->addItem(i);
            ui->clipboardList->setItemWidget(i, data);
        }

    }
    HandleCopy = true;
}

void ClipboardTracker::Zoom(double zoom){
    ClipboardData *data =  dynamic_cast<ClipboardData *>(ui->clipboardList->itemWidget(ui->clipboardList->currentItem()));
    if(!data->getPixmap().isNull())
    {
        ui->prevBox->setPixmap(data->pixmap.scaled(data->pixmap.width() * zoom, data->pixmap.height() *zoom,
                                                        Qt::IgnoreAspectRatio));
    }
}

void ClipboardTracker::on_actionDelete_Data_triggered()
{

    qDebug() << "Delete Triggered";
    ui->clipboardList->takeItem(ui->clipboardList->currentRow());
    if(ui->clipboardList->count() == 0){
    ui->prevBox->setPixmap(QPixmap());
    ui->prevBox->setText(QString());

    }}

void ClipboardTracker::on_ZoomIn_clicked()
{
    if(ui->clipboardList->currentRow() > -1){
        currentZoom += 0.10;
        Zoom(currentZoom);
    }
}

void ClipboardTracker::on_ZoomOut_clicked()
{
    if(currentZoom > 0.20 && ui->clipboardList->currentRow() > -1){
        currentZoom -=  0.10;
        Zoom(currentZoom);
    }
}

void ClipboardTracker::on_bCopy_clicked()
{
    if(ui->clipboardList->currentRow() > -1){

        HandleCopy = false;
        ClipboardData *data =  dynamic_cast<ClipboardData *>(ui->clipboardList->itemWidget(ui->clipboardList->currentItem()));
        if(!data->pixmap.isNull()) {
            clipboard->setImage(data->pixmap.toImage());
        }
        else if(!data->getText().isNull()){
            clipboard->setText(data->getText());
        }

    }
}

void ClipboardTracker::on_ScaleToFit_clicked()
{
    if(ui->clipboardList->currentRow() > -1){
        ClipboardData *data =  dynamic_cast<ClipboardData *>(ui->clipboardList->itemWidget(ui->clipboardList->currentItem()));
        if(!data->pixmap.isNull()){
            ui->prevBox->setPixmap(data->pixmap.scaled(ui->prevBox->size(), Qt::KeepAspectRatio));
        }
    }
}

void ClipboardTracker::trayIcon_doubleCLicked(QSystemTrayIcon::ActivationReason reason){
    if(reason == QSystemTrayIcon::DoubleClick){
        this->setVisible(!this->isVisible());
        if(this->isVisible()){
            this->raise();
            this->activateWindow();
            this->show();
        }
    }

}

void ClipboardTracker::on_actionSave_To_File_triggered()
{
    QFileDialog *fileDialog = new QFileDialog(this);
    QString path = QFileDialog::getSaveFileName(this, "Save To File");
    if(!path.isNull()){
        QFile file(path);
        ClipboardData *data =  dynamic_cast<ClipboardData *>(ui->clipboardList->itemWidget(ui->clipboardList->currentItem()));
        if(!data->pixmap.isNull())
        {
            QFileInfo fi(path);
            data->pixmap.save(fi.path()+ "/"+fi.completeBaseName()+".png", "PNG");
            file.remove();
        }
        else if(!data->getText().isNull()){
            QFileInfo fi(path);
            QFile file2 = fi.path() + "/" + fi.completeBaseName()+ ".txt";
            file.open(QIODevice::WriteOnly);
            file2.open(QIODevice::WriteOnly);
            QTextStream stream(&file2);
            stream << data->getText();
            file.remove();
            file2.close();
            file.close();
            stream.flush();
        }
        }

}

void ClipboardTracker::on_actionOpen_File_triggered()
{
    QString path = QFileDialog::getOpenFileName(this, "Open File", "");
    if(!path.isNull()){
        QFile file(path);
        QFileInfo fi(path);
        file.open(QIODevice::ReadOnly);
        QTextStream stream(&file);
        ClipboardData *data = new ClipboardData(this);
        if(fi.suffix().toLower() == "txt"){
            data->setText(stream.readAll());
            QListWidgetItem *i = new QListWidgetItem("");
            i->setSizeHint(data->sizeHint());
            ui->clipboardList->addItem(i);
            ui->clipboardList->setItemWidget(i, data);

        }

        else if(fi.suffix().toLower() == "png"){
            QPixmap pixmap;
            pixmap.load(path);
            data->pixmap = pixmap;
            data->setPixmap(pixmap.toImage());

            QListWidgetItem *i = new QListWidgetItem("");
            data->setPixmap(data->pixmap.toImage().scaled(300, 300,
                                                          Qt::KeepAspectRatio));
            data->resize(data->pixmap.toImage().scaled(300, 300,
                                                       Qt::KeepAspectRatio).size());
            ;
            i->setSizeHint(data->size());


            ui->clipboardList->addItem(i);
            ui->clipboardList->setItemWidget(i, data);
        }

    }
}

void ClipboardTracker::on_bResetZoom_clicked()
{
    if(ui->clipboardList->currentRow() > -1){
        ClipboardData *data =  dynamic_cast<ClipboardData *>(ui->clipboardList->itemWidget(ui->clipboardList->currentItem()));
        if(!data->pixmap.isNull()){

    Zoom(1.00);
        }
    }
}

void ClipboardTracker::on_actionSave_To_Folder_triggered()
{
    QString path = QFileDialog::getExistingDirectory(this, "Save To Folder");
    if(!path.isNull()){
        QDir dir(path);
        for(int i = 0; i < ui->clipboardList->count(); i++){
            ClipboardData *data =  dynamic_cast<ClipboardData *>(ui->clipboardList->itemWidget(ui->clipboardList->item(i)));
            if(!data->pixmap.isNull())
            {
                data->pixmap.save(dir.path()+ "/"+QString::number(i)+".png", "PNG");
            }
            else if(!data->getText().isNull()){
                QFile file2 = dir.path() + "/" + QString::number(i)+ ".txt";
                file2.open(QIODevice::WriteOnly);
                QTextStream stream(&file2);
                stream << data->getText();
                file2.close();
                stream.flush();
            }
        }
    }



}

void ClipboardTracker::on_actionOpen_Folder_triggered()
{

    QString path = QFileDialog::getExistingDirectory(this, "Open Folder", "");
    if(!path.isNull()){
        QDir dir(path);
        for(int i = 0; i < dir.count(); i++){
            QFile file(dir.path() + "/" + dir[i]);
            file.open(QIODevice::ReadOnly);
            QFileInfo fi(dir.path() + "/" + dir[i]);
            ClipboardData *data = new ClipboardData(this);
            qDebug() << dir[i];
            if(fi.suffix().toLower() == "txt"){
                QTextStream stream(&file);
                data->setText(stream.readAll());
                QListWidgetItem *i = new QListWidgetItem("");
                i->setSizeHint(data->sizeHint());
                ui->clipboardList->addItem(i);
                ui->clipboardList->setItemWidget(i, data);

            }

            else if(fi.suffix().toLower() == "png"){
                QPixmap pixmap;
                pixmap.load(dir.path() + "/" + dir[i]);
                data->pixmap = pixmap;
                data->setPixmap(pixmap.toImage());

                QListWidgetItem *i = new QListWidgetItem("");
                data->setPixmap(data->pixmap.toImage().scaled(200, 200,
                                                              Qt::KeepAspectRatio));
                data->resize(data->pixmap.toImage().scaled(200, 200,
                                                           Qt::KeepAspectRatio).size());


                i->setSizeHint(data->size());


                ui->clipboardList->addItem(i);
                ui->clipboardList->setItemWidget(i, data);
            }
        }


    }

}


void ClipboardTracker::on_openinapp_clicked()
{
    ClipboardData *data =  dynamic_cast<ClipboardData *>(ui->clipboardList->itemWidget(ui->clipboardList->currentItem()));
    if(!QDir(QDir::tempPath() + "/" + "ClipboardTrackerTemp").exists()){
        QDir().mkdir(QDir::tempPath() + "/" + "ClipboardTrackerTemp");
    }
    QString path = QDir::tempPath() + "/" + "ClipboardTrackerTemp" + "/" +"TempFile"+ QString::number(rand());
    if(!data->pixmap.isNull()){
        path += ".png";
        QPixmap pixmap = data->pixmap;
        pixmap.save(path);
        QDesktopServices::openUrl(QUrl::fromLocalFile(path));
    }
    else if(!data->getText().isNull()){
        path += ".txt";
        QFile file(path);
        file.open(QIODevice::ReadWrite);
        QTextStream stream(&file);
        stream << data->getText();
        QDesktopServices::openUrl(QUrl::fromLocalFile(path));
    }
}

void ClipboardTracker::on_actionDelete_Duplicates_triggered()
{
    for(int a = 0; a < 3; a++){

    for(int i = 0; i < ui->clipboardList->count(); i++){
        ClipboardData *data =  dynamic_cast<ClipboardData *>(ui->clipboardList->itemWidget(ui->clipboardList->item(i)));
        for(int j = 0; j < ui->clipboardList->count(); j++){
            if(i != j){
                ClipboardData *data1 =  dynamic_cast<ClipboardData *>(ui->clipboardList->itemWidget(ui->clipboardList->item(j)));
                if(!data->getText().isNull()) {
                    if(data->getText() == data1->getText()) {ui->clipboardList->takeItem(j);}
                }
                else if(!data->pixmap.isNull()){
                    if(data->pixmap.toImage() == data1->pixmap.toImage()) {ui->clipboardList->takeItem(j);}
                }
            }
        }
    }

    }}
