#ifndef CLIPBOARDDATA_H
#define CLIPBOARDDATA_H

#include <QMainWindow>
#include <QPixmap>

namespace Ui {
class ClipboardData;
}

class ClipboardData : public QMainWindow
{
    Q_OBJECT

public:
    explicit ClipboardData(QWidget *parent = nullptr);
    ~ClipboardData();

    void setText(QString text);
    QString getText();
    void setPixmap(QImage image);
    QPixmap getPixmap();
    QPixmap pixmap;
    QString text1;

private slots:

    void on_bCopy_clicked();

private:
    Ui::ClipboardData *ui;
};

#endif // CLIPBOARDDATA_H
