#ifndef VIEWPROFILEEDITION_H
#define VIEWPROFILEEDITION_H

#include <QMainWindow>

namespace Ui {
class ViewProfilEedition;
}

class ViewProfilEedition : public QMainWindow
{
    Q_OBJECT

public:
    explicit ViewProfilEedition(QWidget *parent = 0);
    ~ViewProfilEedition();

private:
    Ui::ViewProfilEedition *ui;
};

#endif // VIEWPROFILEEDITION_H
