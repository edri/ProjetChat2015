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

    QString getFirstName() const;
    QString getLastName() const;
    QString getUserName() const;
    QString getPassword() const;
    QImage getProfileImage() const;
    bool isTimeShowed() const;

private slots:
    void on_btn_modifier_clicked();

private:
    Ui::ViewProfilEedition *ui;

signals:
    void requestModifiyUser() const;
};

#endif // VIEWPROFILEEDITION_H
