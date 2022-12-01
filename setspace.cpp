#include "setspace.h"
#include "ui_setspace.h"


setSpace::setSpace(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::setSpace)
{
    ui->setupUi(this);
}

setSpace::~setSpace()
{
    delete ui;
}

void setSpace::on_buttonBox_accepted()
{
    int spaceSize = ui->spaceSize->toPlainText().toInt();
    emit sendSpaceSize(spaceSize);
}
