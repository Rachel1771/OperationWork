#ifndef SETSPACE_H
#define SETSPACE_H

#include <QDialog>

namespace Ui {
class setSpace;
}

class setSpace : public QDialog
{
    Q_OBJECT

public:
    explicit setSpace(QWidget *parent = 0);
    ~setSpace();

private slots:
    void on_buttonBox_accepted();

signals:
    void sendSpaceSize(int spaceSize);

private:
    Ui::setSpace *ui;

};

#endif // SETSPACE_H
