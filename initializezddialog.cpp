#include "initializezddialog.h"
#include "ui_initializezddialog.h"

InitializezdDialog::InitializezdDialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::InitializezdDialog)
{
    ui->setupUi(this);
}

InitializezdDialog::~InitializezdDialog()
{
    delete ui;
}
