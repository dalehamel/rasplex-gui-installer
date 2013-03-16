/********************************************************************************
** Form generated from reading UI file 'mainwindow.ui'
**
** Created: Sat Mar 16 09:02:23 2013
**      by: Qt User Interface Compiler version 4.8.2
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_MAINWINDOW_H
#define UI_MAINWINDOW_H

#include <QtCore/QVariant>
#include <QtGui/QAction>
#include <QtGui/QApplication>
#include <QtGui/QButtonGroup>
#include <QtGui/QCheckBox>
#include <QtGui/QComboBox>
#include <QtGui/QGroupBox>
#include <QtGui/QHBoxLayout>
#include <QtGui/QHeaderView>
#include <QtGui/QLabel>
#include <QtGui/QMainWindow>
#include <QtGui/QProgressBar>
#include <QtGui/QPushButton>
#include <QtGui/QStatusBar>
#include <QtGui/QToolButton>
#include <QtGui/QVBoxLayout>
#include <QtGui/QWidget>
#include "droppablelineedit.h"

QT_BEGIN_NAMESPACE

class Ui_MainWindow
{
public:
    QWidget *centralwidget;
    QVBoxLayout *vboxLayout;
    QHBoxLayout *hboxLayout;
    QGroupBox *groupBox;
    QHBoxLayout *hboxLayout1;
    DroppableLineEdit *leFile;
    QToolButton *tbBrowse;
    QGroupBox *groupBox_2;
    QHBoxLayout *hboxLayout2;
    QComboBox *cboxDevice;
    QHBoxLayout *hboxLayout3;
    QVBoxLayout *verticalLayout;
    QHBoxLayout *horizontalLayout;
    QCheckBox *md5CheckBox;
    QLabel *md5header;
    QLabel *md5label;
    QPushButton *bMd5Copy;
    QGroupBox *groupBox_5;
    QHBoxLayout *hboxLayout4;
    QProgressBar *progressbar;
    QHBoxLayout *hboxLayout5;
    QLabel *VerLabel;
    QPushButton *bCancel;
    QPushButton *bRead;
    QPushButton *bWrite;
    QPushButton *bExit;
    QStatusBar *statusbar;

    void setupUi(QMainWindow *MainWindow)
    {
        if (MainWindow->objectName().isEmpty())
            MainWindow->setObjectName(QString::fromUtf8("MainWindow"));
        MainWindow->resize(405, 175);
        MainWindow->setAcceptDrops(true);
        QIcon icon;
        icon.addFile(QString::fromUtf8(":/images/images/Win32DiskImager.png"), QSize(), QIcon::Normal, QIcon::Off);
        MainWindow->setWindowIcon(icon);
        centralwidget = new QWidget(MainWindow);
        centralwidget->setObjectName(QString::fromUtf8("centralwidget"));
        vboxLayout = new QVBoxLayout(centralwidget);
        vboxLayout->setSpacing(0);
        vboxLayout->setContentsMargins(0, 0, 0, 0);
        vboxLayout->setObjectName(QString::fromUtf8("vboxLayout"));
        hboxLayout = new QHBoxLayout();
        hboxLayout->setObjectName(QString::fromUtf8("hboxLayout"));
        groupBox = new QGroupBox(centralwidget);
        groupBox->setObjectName(QString::fromUtf8("groupBox"));
        hboxLayout1 = new QHBoxLayout(groupBox);
        hboxLayout1->setSpacing(3);
        hboxLayout1->setContentsMargins(0, 0, 0, 0);
        hboxLayout1->setObjectName(QString::fromUtf8("hboxLayout1"));
        leFile = new DroppableLineEdit(groupBox);
        leFile->setObjectName(QString::fromUtf8("leFile"));

        hboxLayout1->addWidget(leFile);

        tbBrowse = new QToolButton(groupBox);
        tbBrowse->setObjectName(QString::fromUtf8("tbBrowse"));
        QIcon icon1;
        icon1.addFile(QString::fromUtf8(":/images/images/browse.png"), QSize(), QIcon::Normal, QIcon::Off);
        tbBrowse->setIcon(icon1);

        hboxLayout1->addWidget(tbBrowse);


        hboxLayout->addWidget(groupBox);

        groupBox_2 = new QGroupBox(centralwidget);
        groupBox_2->setObjectName(QString::fromUtf8("groupBox_2"));
        hboxLayout2 = new QHBoxLayout(groupBox_2);
        hboxLayout2->setSpacing(0);
        hboxLayout2->setContentsMargins(0, 0, 0, 0);
        hboxLayout2->setObjectName(QString::fromUtf8("hboxLayout2"));
        cboxDevice = new QComboBox(groupBox_2);
        cboxDevice->setObjectName(QString::fromUtf8("cboxDevice"));
        QSizePolicy sizePolicy(QSizePolicy::Preferred, QSizePolicy::Fixed);
        sizePolicy.setHorizontalStretch(0);
        sizePolicy.setVerticalStretch(0);
        sizePolicy.setHeightForWidth(cboxDevice->sizePolicy().hasHeightForWidth());
        cboxDevice->setSizePolicy(sizePolicy);

        hboxLayout2->addWidget(cboxDevice);


        hboxLayout->addWidget(groupBox_2);


        vboxLayout->addLayout(hboxLayout);

        hboxLayout3 = new QHBoxLayout();
        hboxLayout3->setSpacing(4);
        hboxLayout3->setContentsMargins(0, 0, 0, 0);
        hboxLayout3->setObjectName(QString::fromUtf8("hboxLayout3"));
        verticalLayout = new QVBoxLayout();
        verticalLayout->setSpacing(0);
        verticalLayout->setObjectName(QString::fromUtf8("verticalLayout"));
        horizontalLayout = new QHBoxLayout();
        horizontalLayout->setObjectName(QString::fromUtf8("horizontalLayout"));
        horizontalLayout->setContentsMargins(5, -1, -1, -1);
        md5CheckBox = new QCheckBox(centralwidget);
        md5CheckBox->setObjectName(QString::fromUtf8("md5CheckBox"));
        QSizePolicy sizePolicy1(QSizePolicy::Fixed, QSizePolicy::Fixed);
        sizePolicy1.setHorizontalStretch(0);
        sizePolicy1.setVerticalStretch(0);
        sizePolicy1.setHeightForWidth(md5CheckBox->sizePolicy().hasHeightForWidth());
        md5CheckBox->setSizePolicy(sizePolicy1);
        md5CheckBox->setMinimumSize(QSize(20, 0));

        horizontalLayout->addWidget(md5CheckBox);

        md5header = new QLabel(centralwidget);
        md5header->setObjectName(QString::fromUtf8("md5header"));
        md5header->setEnabled(false);
        QSizePolicy sizePolicy2(QSizePolicy::Fixed, QSizePolicy::Preferred);
        sizePolicy2.setHorizontalStretch(0);
        sizePolicy2.setVerticalStretch(0);
        sizePolicy2.setHeightForWidth(md5header->sizePolicy().hasHeightForWidth());
        md5header->setSizePolicy(sizePolicy2);

        horizontalLayout->addWidget(md5header);

        md5label = new QLabel(centralwidget);
        md5label->setObjectName(QString::fromUtf8("md5label"));
        md5label->setEnabled(false);
        md5label->setText(QString::fromUtf8(""));
        md5label->setTextInteractionFlags(Qt::LinksAccessibleByMouse|Qt::TextSelectableByMouse);

        horizontalLayout->addWidget(md5label);

        bMd5Copy = new QPushButton(centralwidget);
        bMd5Copy->setObjectName(QString::fromUtf8("bMd5Copy"));
        QSizePolicy sizePolicy3(QSizePolicy::Maximum, QSizePolicy::Fixed);
        sizePolicy3.setHorizontalStretch(0);
        sizePolicy3.setVerticalStretch(0);
        sizePolicy3.setHeightForWidth(bMd5Copy->sizePolicy().hasHeightForWidth());
        bMd5Copy->setSizePolicy(sizePolicy3);
        bMd5Copy->setMaximumSize(QSize(35, 16777215));

        horizontalLayout->addWidget(bMd5Copy);


        verticalLayout->addLayout(horizontalLayout);


        hboxLayout3->addLayout(verticalLayout);


        vboxLayout->addLayout(hboxLayout3);

        groupBox_5 = new QGroupBox(centralwidget);
        groupBox_5->setObjectName(QString::fromUtf8("groupBox_5"));
        hboxLayout4 = new QHBoxLayout(groupBox_5);
        hboxLayout4->setSpacing(0);
        hboxLayout4->setContentsMargins(0, 0, 0, 0);
        hboxLayout4->setObjectName(QString::fromUtf8("hboxLayout4"));
        progressbar = new QProgressBar(groupBox_5);
        progressbar->setObjectName(QString::fromUtf8("progressbar"));
        progressbar->setValue(0);
        progressbar->setTextVisible(true);

        hboxLayout4->addWidget(progressbar);


        vboxLayout->addWidget(groupBox_5);

        hboxLayout5 = new QHBoxLayout();
        hboxLayout5->setObjectName(QString::fromUtf8("hboxLayout5"));
        VerLabel = new QLabel(centralwidget);
        VerLabel->setObjectName(QString::fromUtf8("VerLabel"));
        VerLabel->setBaseSize(QSize(40, 20));
        VerLabel->setText(QString::fromUtf8(""));
        VerLabel->setAlignment(Qt::AlignCenter);

        hboxLayout5->addWidget(VerLabel);

        bCancel = new QPushButton(centralwidget);
        bCancel->setObjectName(QString::fromUtf8("bCancel"));
        bCancel->setEnabled(false);
        sizePolicy1.setHeightForWidth(bCancel->sizePolicy().hasHeightForWidth());
        bCancel->setSizePolicy(sizePolicy1);

        hboxLayout5->addWidget(bCancel);

        bRead = new QPushButton(centralwidget);
        bRead->setObjectName(QString::fromUtf8("bRead"));
        sizePolicy1.setHeightForWidth(bRead->sizePolicy().hasHeightForWidth());
        bRead->setSizePolicy(sizePolicy1);

        hboxLayout5->addWidget(bRead);

        bWrite = new QPushButton(centralwidget);
        bWrite->setObjectName(QString::fromUtf8("bWrite"));
        sizePolicy1.setHeightForWidth(bWrite->sizePolicy().hasHeightForWidth());
        bWrite->setSizePolicy(sizePolicy1);

        hboxLayout5->addWidget(bWrite);

        bExit = new QPushButton(centralwidget);
        bExit->setObjectName(QString::fromUtf8("bExit"));
        sizePolicy1.setHeightForWidth(bExit->sizePolicy().hasHeightForWidth());
        bExit->setSizePolicy(sizePolicy1);

        hboxLayout5->addWidget(bExit);


        vboxLayout->addLayout(hboxLayout5);

        MainWindow->setCentralWidget(centralwidget);
        statusbar = new QStatusBar(MainWindow);
        statusbar->setObjectName(QString::fromUtf8("statusbar"));
        MainWindow->setStatusBar(statusbar);

        retranslateUi(MainWindow);
        QObject::connect(bExit, SIGNAL(clicked()), MainWindow, SLOT(close()));

        QMetaObject::connectSlotsByName(MainWindow);
    } // setupUi

    void retranslateUi(QMainWindow *MainWindow)
    {
        MainWindow->setWindowTitle(QApplication::translate("MainWindow", "Win32 Disk Imager", 0, QApplication::UnicodeUTF8));
        groupBox->setTitle(QApplication::translate("MainWindow", "Image File", 0, QApplication::UnicodeUTF8));
        tbBrowse->setText(QApplication::translate("MainWindow", "...", 0, QApplication::UnicodeUTF8));
        groupBox_2->setTitle(QApplication::translate("MainWindow", "Device", 0, QApplication::UnicodeUTF8));
#ifndef QT_NO_TOOLTIP
        md5CheckBox->setToolTip(QApplication::translate("MainWindow", "If checked, will generate the MD5 hash for the image file", 0, QApplication::UnicodeUTF8));
#endif // QT_NO_TOOLTIP
        md5CheckBox->setText(QString());
        md5header->setText(QApplication::translate("MainWindow", "MD5 Hash: ", 0, QApplication::UnicodeUTF8));
#ifndef QT_NO_TOOLTIP
        bMd5Copy->setToolTip(QApplication::translate("MainWindow", "Copy MD5 to clipboard", 0, QApplication::UnicodeUTF8));
#endif // QT_NO_TOOLTIP
        bMd5Copy->setText(QApplication::translate("MainWindow", "Copy", 0, QApplication::UnicodeUTF8));
        groupBox_5->setTitle(QApplication::translate("MainWindow", "Progress", 0, QApplication::UnicodeUTF8));
        progressbar->setFormat(QApplication::translate("MainWindow", "%p%", 0, QApplication::UnicodeUTF8));
#ifndef QT_NO_STATUSTIP
        bCancel->setStatusTip(QApplication::translate("MainWindow", "Cancel current process.", 0, QApplication::UnicodeUTF8));
#endif // QT_NO_STATUSTIP
        bCancel->setText(QApplication::translate("MainWindow", "Cancel", 0, QApplication::UnicodeUTF8));
#ifndef QT_NO_TOOLTIP
        bRead->setToolTip(QString());
#endif // QT_NO_TOOLTIP
#ifndef QT_NO_STATUSTIP
        bRead->setStatusTip(QApplication::translate("MainWindow", "Read data from 'Device' to 'Image File'", 0, QApplication::UnicodeUTF8));
#endif // QT_NO_STATUSTIP
        bRead->setText(QApplication::translate("MainWindow", "Read", 0, QApplication::UnicodeUTF8));
#ifndef QT_NO_STATUSTIP
        bWrite->setStatusTip(QApplication::translate("MainWindow", "Write data in 'Image File' to 'Device'", 0, QApplication::UnicodeUTF8));
#endif // QT_NO_STATUSTIP
        bWrite->setText(QApplication::translate("MainWindow", "Write", 0, QApplication::UnicodeUTF8));
#ifndef QT_NO_STATUSTIP
        bExit->setStatusTip(QApplication::translate("MainWindow", "Exit Win32 Disk Imager", 0, QApplication::UnicodeUTF8));
#endif // QT_NO_STATUSTIP
        bExit->setText(QApplication::translate("MainWindow", "Exit", 0, QApplication::UnicodeUTF8));
    } // retranslateUi

};

namespace Ui {
    class MainWindow: public Ui_MainWindow {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_MAINWINDOW_H
