#ifndef VIEWABOUT_H
#define VIEWABOUT_H

#include <QDialog>

namespace Ui {
class viewAbout;
}

class ViewAbout : public QDialog
{
    Q_OBJECT

public:
    explicit ViewAbout(QWidget *parent = 0);
    ~ViewAbout();

private:
    Ui::viewAbout *_ui;
    QWidget* _parent;

    void closeEvent(QCloseEvent*);
};

#endif // VIEWABOUT_H