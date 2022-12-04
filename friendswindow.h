#ifndef FRIENDSWINDOW_H
#define FRIENDSWINDOW_H

#include <QMainWindow>
#include <buddy.h>
namespace Ui {
class FriendsWindow;
}

class FriendsWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit FriendsWindow(QWidget *parent = 0);
    ~FriendsWindow();

private slots:
    void on_pushButton_clicked();

    void on_f_initButton_clicked();

    void on_f_arrangeButton_clicked();

    void on_f_recycleButton_clicked();

    void receiveNewSpaceSize(int spaceSize);

    void receiveSpaceAddress(int spaceAddress);

    void f_printList();

    void allocateMemoryToProcess(int processSize);

    void recursiveCheck(Buddy * root);

    void randomSearchAndDelete(Buddy * root,int address);

    void deallocateMemoryOfProcess(int address);

    void computeInternalFragmentation(Buddy * root);

    void printTree(Buddy *n);

    void initMemory(int ramSize);

    void freeList();

    Buddy * splitMemory(Buddy *tree);

private:
    Ui::FriendsWindow *ui;
};

#endif // FRIENDSWINDOW_H
