#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QProcess>
namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT
    
public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();
    
private:
    Ui::MainWindow *ui;
    QProcess* _pr;
    QString _equation;
private slots:
    void solve();
    void processStarted();
    void processError();
    void processFinished(int f);
};

#endif // MAINWINDOW_H
