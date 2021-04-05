#ifndef CLIPBOARDTRACKER_H
#define CLIPBOARDTRACKER_H

#include <QMainWindow>
#include <QClipboard>
#include <QDebug>
#include <QListWidget>
#include <QSystemTrayIcon>
#include <QCloseEvent>
#include <QTextStream>
#include <QFileDialog>
#include <QBuffer>
#include <QDir>
#include <QDesktopServices>
#include <QAbstractItemView>
#include "clipboarddata.h"

QT_BEGIN_NAMESPACE
namespace Ui { class ClipboardTracker; }
QT_END_NAMESPACE

class ClipboardTracker : public QMainWindow
{
    Q_OBJECT

public:
    ClipboardTracker(QWidget *parent = nullptr);
    ~ClipboardTracker();

private slots:
    void Zoom(double zoom);

    void on_clipboardList_currentItemChanged(QListWidgetItem *current, QListWidgetItem *previous);

    void on_clipboard_changed();

    void on_actionDelete_Data_triggered();


    void on_ZoomIn_clicked();

    void on_ZoomOut_clicked();

    void on_bCopy_clicked();

    void on_ScaleToFit_clicked();

    void trayIcon_doubleCLicked(QSystemTrayIcon::ActivationReason reason);

    void closeEvent(QCloseEvent *event);


    void on_actionSave_To_File_triggered();

    void on_actionOpen_File_triggered();

    void on_bResetZoom_clicked();

    void on_actionSave_To_Folder_triggered();

    void on_actionOpen_Folder_triggered();


    void on_openinapp_clicked();

    void on_actionDelete_Duplicates_triggered();

private:
    double currentZoom = 1;

    bool HandleChange = true;
    bool HandleCopy = true;

    Ui::ClipboardTracker *ui;

    QClipboard *clipboard;

    QSystemTrayIcon *trayIcon;
};
#endif // CLIPBOARDTRACKER_H
