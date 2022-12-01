#ifndef REMOVESPACE_H
#define REMOVESPACE_H

#include <QDialog>

namespace Ui {
class removeSpace;
}

class removeSpace : public QDialog
{
    Q_OBJECT

public:
    explicit removeSpace(QWidget *parent = 0);
    ~removeSpace();

private slots:
    void on_buttonBox_accepted();

private:
    Ui::removeSpace *ui;

signals:
    void sendRemoveSpaceAddress(int spaceAddress);
};


#endif // REMOVESPACE_H
