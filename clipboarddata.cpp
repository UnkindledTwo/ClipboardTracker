#include "clipboarddata.h"
#include "ui_clipboarddata.h"

ClipboardData::ClipboardData(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::ClipboardData)
{
    ui->setupUi(this);
}

ClipboardData::~ClipboardData()
{
    delete ui;
}


void ClipboardData::setText(QString text){
    if(text.length() > 100){
        ui->text->setText(text.left(100));
    }
    else{
    ui->text->setText(text);
    }
    text1 = text;
}

QString ClipboardData::getText(){
    return text1;
}

void ClipboardData::setPixmap(QImage image){
    ui->text->setPixmap(QPixmap::fromImage(image));

}

QPixmap ClipboardData::getPixmap(){
    return pixmap;

    //return *ui->text->pixmap();
}


void ClipboardData::on_bCopy_clicked()
{

}
