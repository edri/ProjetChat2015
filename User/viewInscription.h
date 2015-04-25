#ifndef VIEWINSCRIPTION_H
#define VIEWINSCRIPTION_H

#include <QMainWindow>
#include <QFileDialog>

namespace Ui {
class viewInscription;
}

class ViewInscription : public QMainWindow
{
    Q_OBJECT

public:
    explicit ViewInscription(QWidget *parent = 0);
    ~ViewInscription();

private slots:
    void on_btn_path_clicked();

private:
    Ui::viewInscription *ui;
};

#endif // VIEWINSCRIPTION_H
