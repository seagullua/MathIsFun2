/********************************************************************************
** Form generated from reading UI file 'MainWindow.ui'
**
** Created by: Qt User Interface Compiler version 5.1.0
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_MAINWINDOW_H
#define UI_MAINWINDOW_H

#include <QtCore/QVariant>
#include <QtWidgets/QAction>
#include <QtWidgets/QApplication>
#include <QtWidgets/QButtonGroup>
#include <QtWidgets/QHBoxLayout>
#include <QtWidgets/QHeaderView>
#include <QtWidgets/QLabel>
#include <QtWidgets/QLineEdit>
#include <QtWidgets/QMainWindow>
#include <QtWidgets/QPushButton>
#include <QtWidgets/QTextEdit>
#include <QtWidgets/QVBoxLayout>
#include <QtWidgets/QWidget>

QT_BEGIN_NAMESPACE

class Ui_MainWindow
{
public:
    QWidget *centralWidget;
    QHBoxLayout *horizontalLayout_3;
    QVBoxLayout *verticalLayout;
    QHBoxLayout *horizontalLayout;
    QLineEdit *equation;
    QPushButton *solve;
    QLabel *label;
    QHBoxLayout *horizontalLayout_2;
    QTextEdit *solutions;
    QTextEdit *markup;

    void setupUi(QMainWindow *MainWindow)
    {
        if (MainWindow->objectName().isEmpty())
            MainWindow->setObjectName(QStringLiteral("MainWindow"));
        MainWindow->resize(589, 367);
        centralWidget = new QWidget(MainWindow);
        centralWidget->setObjectName(QStringLiteral("centralWidget"));
        horizontalLayout_3 = new QHBoxLayout(centralWidget);
        horizontalLayout_3->setSpacing(6);
        horizontalLayout_3->setContentsMargins(11, 11, 11, 11);
        horizontalLayout_3->setObjectName(QStringLiteral("horizontalLayout_3"));
        verticalLayout = new QVBoxLayout();
        verticalLayout->setSpacing(6);
        verticalLayout->setObjectName(QStringLiteral("verticalLayout"));
        horizontalLayout = new QHBoxLayout();
        horizontalLayout->setSpacing(6);
        horizontalLayout->setObjectName(QStringLiteral("horizontalLayout"));
        equation = new QLineEdit(centralWidget);
        equation->setObjectName(QStringLiteral("equation"));
        QFont font;
        font.setFamily(QStringLiteral("Calibri"));
        font.setPointSize(14);
        equation->setFont(font);

        horizontalLayout->addWidget(equation);

        solve = new QPushButton(centralWidget);
        solve->setObjectName(QStringLiteral("solve"));

        horizontalLayout->addWidget(solve);


        verticalLayout->addLayout(horizontalLayout);

        label = new QLabel(centralWidget);
        label->setObjectName(QStringLiteral("label"));

        verticalLayout->addWidget(label);

        horizontalLayout_2 = new QHBoxLayout();
        horizontalLayout_2->setSpacing(6);
        horizontalLayout_2->setObjectName(QStringLiteral("horizontalLayout_2"));
        solutions = new QTextEdit(centralWidget);
        solutions->setObjectName(QStringLiteral("solutions"));

        horizontalLayout_2->addWidget(solutions);

        markup = new QTextEdit(centralWidget);
        markup->setObjectName(QStringLiteral("markup"));

        horizontalLayout_2->addWidget(markup);


        verticalLayout->addLayout(horizontalLayout_2);


        horizontalLayout_3->addLayout(verticalLayout);

        MainWindow->setCentralWidget(centralWidget);

        retranslateUi(MainWindow);

        QMetaObject::connectSlotsByName(MainWindow);
    } // setupUi

    void retranslateUi(QMainWindow *MainWindow)
    {
        MainWindow->setWindowTitle(QApplication::translate("MainWindow", "MiF Solver", 0));
        solve->setText(QApplication::translate("MainWindow", "Solve", 0));
        label->setText(QApplication::translate("MainWindow", "<html><head/><body><p><span style=\" font-weight:600; color:#00aa00;\">_</span><span style=\" color:#00aa00;\"> - any digit</span></p><p><span style=\" font-weight:600; color:#ff5500;\">#</span><span style=\" color:#ff5500;\"> - any operation</span></p><p><span style=\" font-weight:600; color:#aa007f;\">$</span><span style=\" color:#aa007f;\"> - any parenthesis</span></p><p><span style=\" font-weight:600;\">@</span> - <span style=\" color:#00aa00;\">digit </span><span style=\" color:#ff5500;\">or operation</span></p><p><span style=\" font-weight:600;\">&amp;</span> - <span style=\" color:#00aa00;\">digit </span><span style=\" color:#aa007f;\">or parenthesis</span></p><p><span style=\" font-weight:600;\">%</span> - <span style=\" color:#ff5500;\">operation </span><span style=\" color:#aa007f;\">or parenthesis</span></p><p><span style=\" font-weight:600;\">A</span> - <span style=\" color:#00aa00;\">digit </span><span style=\" color:#ff5500;\">or operation </span><span style=\" color:#aa007f;\">or parenthesis</sp"
                        "an></p></body></html>", 0));
    } // retranslateUi

};

namespace Ui {
    class MainWindow: public Ui_MainWindow {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_MAINWINDOW_H
