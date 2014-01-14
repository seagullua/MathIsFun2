#include "MainWindow.h"
#include "ui_MainWindow.h"
#include <QProcess>
#include <QFile>
#include <QDebug>
#include <QDir>
MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow), _pr(0)
{
    ui->setupUi(this);
    connect(ui->solve, SIGNAL(clicked()), SLOT(solve()));
}
void MainWindow::solve()
{
    if(_pr)
        delete _pr;

    ui->solve->setEnabled(false);
    ui->markup->clear();
    ui->solutions->clear();

    _pr=new QProcess(this);
    QString cmd = QString("EquationSolverConsole.exe");

    if(!QFile::exists(cmd))
        ui->solutions->append(tr("Enable to run utility: %1").arg(cmd));
    //QString cmd = QString("cmd.exe");
    QStringList args;
    args << ui->equation->text();
    _equation = ui->equation->text();
    connect(_pr, SIGNAL(started()), SLOT(processStarted()));
    connect(_pr, SIGNAL(error(QProcess::ProcessError)), SLOT(processError()));
    connect(_pr, SIGNAL(finished(int)), SLOT(processFinished(int)));
    _pr->setWorkingDirectory(QDir::currentPath());
    _pr->start(cmd,args);
}
void MainWindow::processFinished(int f)
{
    qDebug() << "Finished";
    QString arr = _pr->readAllStandardOutput();
    QStringList list = arr.split(QRegExp("[\r\n]"),QString::SkipEmptyParts);
    QString markup("LEVEL\n");
    markup += QString("\tEQUATION(%1)\n").arg(_equation);
    for(int i=0; i<list.size(); ++i)
    {
        QString cur = _equation;
        QString solution = list[i];
        int s_i = 0;
        for(int i=0; i<cur.size() && s_i < solution.size(); ++i)
        {
            QChar ch = cur[i];
            if(ch == '_' || ch == '#' || ch =='A' || ch=='$' || ch =='@' || ch=='&' || ch=='%')
            {
                cur[i] = solution[s_i++];
            }
        }
        ui->solutions->append(cur);

        markup += QString("\tADD_SOLUTION(%1)\n").arg(solution);

    }
    markup += "END_LEVEL\n";
    ui->markup->setText(markup);
    ui->solve->setEnabled(true);
}
void MainWindow::processStarted()
{
    qDebug() << "Started";
}

void MainWindow::processError()
{
    qDebug() << "Error" << _pr->errorString();
    ui->solutions->append("Error");
    ui->solutions->append(_pr->errorString());
    ui->solve->setEnabled(true);
}

MainWindow::~MainWindow()
{
    delete ui;
}
