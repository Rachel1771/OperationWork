#include "removespace.h"
#include "ui_removespace.h"

removeSpace::removeSpace(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::removeSpace)
{
    ui->setupUi(this);
}

removeSpace::~removeSpace()
{
    delete ui;
}

void removeSpace::on_buttonBox_accepted()
{
    int spaceAddress = ui->spaceAddress->toPlainText().toInt();
    emit sendRemoveSpaceAddress(spaceAddress);
}
