#ifndef VIEWINSCRIPTION_H
#define VIEWINSCRIPTION_H

#include <QMainWindow>
#include <QFileDialog>
//#include <QCloseEvent>

namespace Ui {
class viewInscription;
}

class ViewInscription : public QMainWindow
{
    Q_OBJECT

public:
    explicit ViewInscription(QWidget *parent = 0);
    ~ViewInscription();

    QString getFirstName() const;

private slots:
    void on_btn_path_clicked();

    void on_btn_inscription_clicked();

private:
    Ui::viewInscription *ui;
    void closeEvent(QCloseEvent *);
};

#endif // VIEWINSCRIPTION_H
