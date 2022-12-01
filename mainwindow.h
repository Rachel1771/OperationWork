#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();

private slots:
    void on_initButton_clicked();

    void on_resetButton_clicked();

    void on_arrangeButton_clicked();

    void receiveNewSpaceSize(int spaceSize);

    void receiveSpaceAddress(int spaceAddress);

    void on_recycleButton_clicked();

    void printList(struct memLinkList *pmemlist);

private:
    Ui::MainWindow *ui;
};



#endif // MAINWINDOW_H
