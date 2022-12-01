#ifndef INITIALIZEZDDIALOG_H
#define INITIALIZEZDDIALOG_H

#include <QDialog>

namespace Ui {
class InitializezdDialog;
}

class InitializezdDialog : public QDialog
{
    Q_OBJECT

public:
    explicit InitializezdDialog(QWidget *parent = 0);
    ~InitializezdDialog();

private:
    Ui::InitializezdDialog *ui;
};

#endif // INITIALIZEZDDIALOG_H
