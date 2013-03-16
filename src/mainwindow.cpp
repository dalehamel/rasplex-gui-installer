/**********************************************************************
 *  This program is free software; you can redistribute it and/or     *
 *  modify it under the terms of the GNU General Public License       *
 *  as published by the Free Software Foundation; either version 2    *
 *  of the License, or (at your option) any later version.            *
 *                                                                    *
 *  This program is distributed in the hope that it will be useful,   *
 *  but WITHOUT ANY WARRANTY; without even the implied warranty of    *
 *  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the     *
 *  GNU General Public License for more details.                      *
 *                                                                    *
 *  You should have received a copy of the GNU General Public License *
 *  along with this program; if not, write to the Free Software       *
 *  Foundation, Inc., 51 Franklin Street, Fifth Floor,                *
 *  Boston, MA  02110-1301, USA.                                      *
 *                                                                    *
 *  ---                                                               *
 *  Copyright (C) 2009, Justin Davis <tuxdavis@gmail.com>             *
 *  Copyright (C) 2009, 2012 ImageWriter developers                   *
 *                           https://launchpad.net/~image-writer-devs *
 **********************************************************************/

#ifndef WINVER
#define WINVER 0x0500
#endif

#include <QtGui>
#include <QCoreApplication>
#include <QFileInfo>
#include <QDirIterator>
#include <cstdio>
#include <cstdlib>

#ifdef Q_WS_WIN
    #include <windows.h>
    #include <winioctl.h>
    #include <dbt.h>
    #include <shlobj.h>

#else
    #define INVALID_HANDLE_VALUE 1
#endif

#include "disk.h"
#include "mainwindow.h"
#include "md5.h"

MainWindow::MainWindow(QWidget *parent) : QMainWindow(parent)
{
    setupUi(this);
    getLogicalDrives();
    status = STATUS_IDLE;
    progressbar->reset();
    clipboard = QApplication::clipboard();
    bMd5Copy->setVisible(false);
    statusbar->showMessage(tr("Waiting for a task."));

#ifdef Q_WS_WIN
    hVolume = INVALID_HANDLE_VALUE;
    hFile = INVALID_HANDLE_VALUE;
    hRawDisk = INVALID_HANDLE_VALUE;
#endif
    if (QCoreApplication::arguments().count() > 1)
    {
        QString filelocation = QApplication::arguments().at(1);
        QFileInfo fileInfo(filelocation);
        leFile->setText(fileInfo.absoluteFilePath());
    }

    setReadWriteButtonState();
    QString myver = tr("Version: %1").arg(VER);
    VerLabel->setText(myver);
    sectorData = NULL;
    sectorsize = 0ul;

    myHomeDir = QDir::homePath();
    if (myHomeDir == NULL){
        myHomeDir = qgetenv("USERPROFILE");
    }
    QRegExp dir(tr("/Downloads$"));
    dir.setPatternSyntax(QRegExp::RegExp);
    QDirIterator it(myHomeDir, QDir::AllDirs|QDir::NoDotAndDotDot, QDirIterator::Subdirectories);
    while (it.hasNext()) {
        it.next();
        if (it.fileInfo().filePath().contains(dir)){
            myHomeDir = it.filePath();
            break;

        }
    }
}

MainWindow::~MainWindow()
{
#ifdef Q_WS_WIN
    if (hRawDisk != INVALID_HANDLE_VALUE)
    {
        CloseHandle(hRawDisk);
        hRawDisk = INVALID_HANDLE_VALUE;
    }
    if (hFile != INVALID_HANDLE_VALUE)
    {
        CloseHandle(hFile);
        hFile = INVALID_HANDLE_VALUE;
    }
    if (hVolume != INVALID_HANDLE_VALUE)
    {
        CloseHandle(hVolume);
        hVolume = INVALID_HANDLE_VALUE;
    }
    if (sectorData != NULL)
    {
        delete sectorData;
        sectorData = NULL;
    }
#endif
}

void MainWindow::setReadWriteButtonState()
{
    bool fileSelected = !(leFile->text().isEmpty());
    bool deviceSelected = (cboxDevice->count() > 0);

    // set read and write buttons according to status of file/device
    bRead->setEnabled(deviceSelected && fileSelected);
    bWrite->setEnabled(deviceSelected && fileSelected);
}

void MainWindow::closeEvent(QCloseEvent *event)
{
    if (status == STATUS_READING)
    {
        if (QMessageBox::warning(NULL, tr("Exit?"), tr("Exiting now will result in a corrupt image file.\n"
                                                       "Are you sure you want to exit?"),
                                 QMessageBox::Yes|QMessageBox::No, QMessageBox::No) == QMessageBox::Yes)
        {
            status = STATUS_EXIT;
        }
        event->ignore();
    }
    else if (status == STATUS_WRITING)
    {
        if (QMessageBox::warning(NULL, tr("Exit?"), tr("Exiting now will result in a corrupt disk.\n"
                                                       "Are you sure you want to exit?"),
                                 QMessageBox::Yes|QMessageBox::No, QMessageBox::No) == QMessageBox::Yes)
        {
            status = STATUS_EXIT;
        }
        event->ignore();
    }
}

void MainWindow::on_tbBrowse_clicked()
{
    QString filelocation = QFileDialog::getOpenFileName(NULL, tr("Select a disk image"), myHomeDir, "*.img;*.IMG;;*.*",
                                                        0, QFileDialog::DontConfirmOverwrite);
    if (!filelocation.isNull())
    {
        leFile->setText(filelocation);
        md5label->clear();

        // if the md5 checkbox is checked, verify that it's a good file
        // and then generate the md5 hash
        if(md5CheckBox->isChecked())
        {
            QFileInfo fileInfo(filelocation);

            if (fileInfo.exists() && fileInfo.isFile() &&
                    fileInfo.isReadable() && (fileInfo.size() > 0) )
            {
                generateMd5(filelocation.toLatin1().data());
            }
        }
    }
    updateMd5CopyButton();
}

void MainWindow::on_bMd5Copy_clicked()
{
    QString md5sum(md5label->text());
    if ( !(md5sum.isEmpty()) )
    {
        clipboard->setText(md5sum);
    }
}

// generates the md5 hash
void MainWindow::generateMd5(char *filename)
{
    md5label->setText(tr("Generating..."));
    QApplication::processEvents();

    MD5 md5;

    // may take a few secs - display a wait cursor
    QApplication::setOverrideCursor(QCursor(Qt::WaitCursor));

    // "digestFile" computes the md5 - display it in the textbox
    md5label->setText(md5.digestFile(filename));

    // redisplay the normal cursor
    QApplication::restoreOverrideCursor();
}

void MainWindow::on_leFile_textChanged()
{
    setReadWriteButtonState();

    // since the filename was edited, the md5sum no longer
    //    applies - clear it...
    if( !(md5label->text().isEmpty()) )
    {
        md5label->clear();
    }
    updateMd5CopyButton();
}

// on an "editingFinished" signal (IE: return press), if the lineedit
// contains a valid file, and generate the md5
void MainWindow::on_leFile_editingFinished()
{
    if(md5CheckBox->isChecked())
    {
        QFileInfo fileinfo(leFile->text());
        if (fileinfo.exists() && fileinfo.isFile() &&
                fileinfo.isReadable() && (fileinfo.size() > 0) )
        {
            generateMd5(leFile->text().toLatin1().data());
        }
    }
    updateMd5CopyButton();
}

void MainWindow::on_bCancel_clicked()
{
    if ( (status == STATUS_READING) || (status == STATUS_WRITING) )

    {
        if (QMessageBox::warning(NULL, tr("Cancel?"), tr("Canceling now will result in a corrupt destination.\n"
                                                         "Are you sure you want to cancel?"),
                                 QMessageBox::Yes|QMessageBox::No, QMessageBox::No) == QMessageBox::Yes)
        {
            status = STATUS_CANCELED;
        }
    }
}

// if the md5 checkbox becomes "checked", verify the file and generate md5
// when it's "unchecked", clear the md5 label
void MainWindow::on_md5CheckBox_stateChanged()
{
    bool state = md5CheckBox->isChecked();

    md5header->setEnabled(state);
    md5label->setEnabled(state);

    if(state)
    {
        // changed from unchecked to checked
        if( !(leFile->text().isEmpty()) )
        {
            QFileInfo fileinfo(leFile->text());
            if (fileinfo.exists() && fileinfo.isFile() &&
                    fileinfo.isReadable() && (fileinfo.size() > 0) )
            {
                generateMd5(leFile->text().toLatin1().data());
            }
        }

    }
    else
    {
        // changed from checked to unchecked
        md5label->clear();
    }

    updateMd5CopyButton();
}

void MainWindow::on_bWrite_clicked()
{
#ifdef Q_WS_WIN
    bool passfail = true;
    if (!leFile->text().isEmpty())
    {
        QFileInfo fileinfo(leFile->text());
        if (fileinfo.exists() && fileinfo.isFile() &&
                fileinfo.isReadable() && (fileinfo.size() > 0) )
        {
            if (leFile->text().at(0) == cboxDevice->currentText().at(1))
            {
                QMessageBox::critical(NULL, tr("Write Error"), tr("Image file cannot be located on the target device."));
                return;
            }

            // build the drive letter as a const char *
            //   (without the surrounding brackets)
            QString qs = cboxDevice->currentText();
            qs.replace(QRegExp("[\\[\\]]"), "");
            QByteArray qba = qs.toLocal8Bit();
            const char *ltr = qba.data();
            if (QMessageBox::warning(NULL, tr("Confirm overwrite"), tr("Writing to a physical device can corrupt the device.\n"
                                                                       "(Target Device: %1 \"%2\")\n"
                                                                       "Are you sure you want to continue?").arg(cboxDevice->currentText()).arg(getDriveLabel(ltr)),
                                     QMessageBox::Yes | QMessageBox::No, QMessageBox::No) == QMessageBox::No)
            {
                return;
            }
            status = STATUS_WRITING;
            bCancel->setEnabled(true);
            bWrite->setEnabled(false);
            bRead->setEnabled(false);
            double mbpersec;
            unsigned long long i, lasti, availablesectors, numsectors;
            int volumeID = cboxDevice->currentText().at(1).toAscii() - 'A';
            int deviceID = cboxDevice->itemData(cboxDevice->currentIndex()).toInt();
            hVolume = getHandleOnVolume(volumeID, GENERIC_WRITE);
            if (hVolume == INVALID_HANDLE_VALUE)
            {
                status = STATUS_IDLE;
                bCancel->setEnabled(false);
                setReadWriteButtonState();
                return;
            }
            if (!getLockOnVolume(hVolume))
            {
                CloseHandle(hVolume);
                status = STATUS_IDLE;
                hVolume = INVALID_HANDLE_VALUE;
                bCancel->setEnabled(false);
                setReadWriteButtonState();
                return;
            }
            if (!unmountVolume(hVolume))
            {
                removeLockOnVolume(hVolume);
                CloseHandle(hVolume);
                status = STATUS_IDLE;
                hVolume = INVALID_HANDLE_VALUE;
                bCancel->setEnabled(false);
                setReadWriteButtonState();
                return;
            }
            hFile = getHandleOnFile(leFile->text().toAscii().data(), GENERIC_READ);
            if (hFile == INVALID_HANDLE_VALUE)
            {
                removeLockOnVolume(hVolume);
                CloseHandle(hVolume);
                status = STATUS_IDLE;
                hVolume = INVALID_HANDLE_VALUE;
                bCancel->setEnabled(false);
                setReadWriteButtonState();
                return;
            }
            hRawDisk = getHandleOnDevice(deviceID, GENERIC_WRITE);
            if (hRawDisk == INVALID_HANDLE_VALUE)
            {
                removeLockOnVolume(hVolume);
                CloseHandle(hFile);
                CloseHandle(hVolume);
                status = STATUS_IDLE;
                hVolume = INVALID_HANDLE_VALUE;
                hFile = INVALID_HANDLE_VALUE;
                bCancel->setEnabled(false);
                setReadWriteButtonState();
                return;
            }
            availablesectors = getNumberOfSectors(hRawDisk, &sectorsize);
            numsectors = getFileSizeInSectors(hFile, sectorsize);
            if (numsectors > availablesectors)
            {
                QMessageBox::critical(NULL, tr("Write Error"),
                                      tr("Not enough space on disk: Size: %1 sectors  Available: %2 sectors  Sector size: %3").arg(numsectors).arg(availablesectors).arg(sectorsize));
                removeLockOnVolume(hVolume);
                CloseHandle(hRawDisk);
                CloseHandle(hFile);
                CloseHandle(hVolume);
                status = STATUS_IDLE;
                hVolume = INVALID_HANDLE_VALUE;
                hFile = INVALID_HANDLE_VALUE;
                hRawDisk = INVALID_HANDLE_VALUE;
                bCancel->setEnabled(false);
                setReadWriteButtonState();
                return;
            }

            progressbar->setRange(0, (numsectors == 0ul) ? 100 : (int)numsectors);
            lasti = 0ul;
            timer.start();
            for (i = 0ul; i < numsectors && status == STATUS_WRITING; i += 1024ul)
            {
                sectorData = readSectorDataFromHandle(hFile, i, (numsectors - i >= 1024ul) ? 1024ul:(numsectors - i), sectorsize);
                if (sectorData == NULL)
                {
                    delete sectorData;
                    removeLockOnVolume(hVolume);
                    CloseHandle(hRawDisk);
                    CloseHandle(hFile);
                    CloseHandle(hVolume);
                    status = STATUS_IDLE;
                    sectorData = NULL;
                    hRawDisk = INVALID_HANDLE_VALUE;
                    hFile = INVALID_HANDLE_VALUE;
                    hVolume = INVALID_HANDLE_VALUE;
                    bCancel->setEnabled(false);
                    setReadWriteButtonState();
                    return;
                }
                if (!writeSectorDataToHandle(hRawDisk, sectorData, i, (numsectors - i >= 1024ul) ? 1024ul:(numsectors - i), sectorsize))
                {
                    delete sectorData;
                    removeLockOnVolume(hVolume);
                    CloseHandle(hRawDisk);
                    CloseHandle(hFile);
                    CloseHandle(hVolume);
                    status = STATUS_IDLE;
                    sectorData = NULL;
                    hRawDisk = INVALID_HANDLE_VALUE;
                    hFile = INVALID_HANDLE_VALUE;
                    hVolume = INVALID_HANDLE_VALUE;
                    bCancel->setEnabled(false);
                    setReadWriteButtonState();
                    return;
                }
                delete sectorData;
                sectorData = NULL;
                QCoreApplication::processEvents();
                if (timer.elapsed() >= 1000)
                {
                    mbpersec = (((double)sectorsize * (i - lasti)) * (1000.0 / timer.elapsed())) / 1024.0 / 1024.0;
                    statusbar->showMessage(QString("%1MB/s").arg(mbpersec));
                    timer.start();
                    lasti = i;
                }
                progressbar->setValue(i);
                QCoreApplication::processEvents();
            }
            removeLockOnVolume(hVolume);
            CloseHandle(hRawDisk);
            CloseHandle(hFile);
            CloseHandle(hVolume);
            sectorData = NULL;
            hRawDisk = INVALID_HANDLE_VALUE;
            hFile = INVALID_HANDLE_VALUE;
            hVolume = INVALID_HANDLE_VALUE;
            if (status == STATUS_CANCELED){
                passfail = false;
            }
        }
        else if (!fileinfo.exists() || !fileinfo.isFile())
        {
            QMessageBox::critical(NULL, tr("File Error"), tr("The selected file does not exist."));
            passfail = false;
        }
        else if (!fileinfo.isReadable())
        {
            QMessageBox::critical(NULL, tr("File Error"), tr("You do not have permision to read the selected file."));
            passfail = false;
        }
        else if (fileinfo.size() == 0)
        {
            QMessageBox::critical(NULL, tr("File Error"), tr("The specified file contains no data."));
            passfail = false;
        }
        progressbar->reset();
        statusbar->showMessage(tr("Done."));
        bCancel->setEnabled(false);
        setReadWriteButtonState();
        if (passfail){
            QMessageBox::information(NULL, tr("Complete"), tr("Write Successful."));
        }

    }
    else
    {
        QMessageBox::critical(NULL, tr("File Error"), tr("Please specify an image file to use."));
    }
    if (status == STATUS_EXIT)
    {
        close();
    }
    status = STATUS_IDLE;
#endif

}

void MainWindow::on_bRead_clicked()
{
#ifdef Q_WS_WIN
    QString myFile;
    if (!leFile->text().isEmpty())
    {
        myFile = leFile->text();
        QFileInfo fileinfo(myFile);
        if (fileinfo.path()=="."){
            myFile=(myHomeDir + "/" + leFile->text());
            QFileInfo fileinfo(myFile);
        }
        if (myFile.at(0) == cboxDevice->currentText().at(1))
        {
            QMessageBox::critical(NULL, tr("Write Error"), tr("Image file cannot be located on the target device."));
            return;
        }
        if (fileinfo.exists())
        {
            if (QMessageBox::warning(NULL, tr("Confirm Overwrite"), tr("Are you sure you want to overwrite the specified file?"),
                                     QMessageBox::Yes|QMessageBox::No, QMessageBox::No) == QMessageBox::No)
            {
                return;
            }
        }
        bCancel->setEnabled(true);
        bWrite->setEnabled(false);
        bRead->setEnabled(false);
        status = STATUS_READING;
        double mbpersec;
        unsigned long long i, lasti, numsectors, filesize, spaceneeded = 0ull;
        int volumeID = cboxDevice->currentText().at(1).toAscii() - 'A';
        int deviceID = cboxDevice->itemData(cboxDevice->currentIndex()).toInt();
        hVolume = getHandleOnVolume(volumeID, GENERIC_READ);
        if (hVolume == INVALID_HANDLE_VALUE)
        {
            status = STATUS_IDLE;
            bCancel->setEnabled(false);
            setReadWriteButtonState();
            return;
        }
        if (!getLockOnVolume(hVolume))
        {
            CloseHandle(hVolume);
            status = STATUS_IDLE;
            hVolume = INVALID_HANDLE_VALUE;
            bCancel->setEnabled(false);
            setReadWriteButtonState();
            return;
        }
        if (!unmountVolume(hVolume))
        {
            removeLockOnVolume(hVolume);
            CloseHandle(hVolume);
            status = STATUS_IDLE;
            hVolume = INVALID_HANDLE_VALUE;
            bCancel->setEnabled(false);
            setReadWriteButtonState();
            return;
        }
        hFile = getHandleOnFile(myFile.toAscii().data(), GENERIC_WRITE);
        if (hFile == INVALID_HANDLE_VALUE)
        {
            removeLockOnVolume(hVolume);
            CloseHandle(hVolume);
            status = STATUS_IDLE;
            hVolume = INVALID_HANDLE_VALUE;
            bCancel->setEnabled(false);
            setReadWriteButtonState();
            return;
        }
        hRawDisk = getHandleOnDevice(deviceID, GENERIC_READ);
        if (hRawDisk == INVALID_HANDLE_VALUE)
        {
            removeLockOnVolume(hVolume);
            CloseHandle(hFile);
            CloseHandle(hVolume);
            status = STATUS_IDLE;
            hVolume = INVALID_HANDLE_VALUE;
            hFile = INVALID_HANDLE_VALUE;
            bCancel->setEnabled(false);
            setReadWriteButtonState();
            return;
        }
        numsectors = getNumberOfSectors(hRawDisk, &sectorsize);
        filesize = getFileSizeInSectors(hFile, sectorsize);
        if (filesize >= numsectors)
        {
            spaceneeded = 0ull;
        }
        else
        {
            spaceneeded = (unsigned long long)(numsectors - filesize) * (unsigned long long)(sectorsize);
        }
        if (!spaceAvailable(myFile.left(3).replace(QChar('/'), QChar('\\')).toAscii().data(), spaceneeded))
        {
            QMessageBox::critical(NULL, tr("Write Error"), tr("Disk is not large enough for the specified image."));
            removeLockOnVolume(hVolume);
            CloseHandle(hRawDisk);
            CloseHandle(hFile);
            CloseHandle(hVolume);
            status = STATUS_IDLE;
            sectorData = NULL;
            hRawDisk = INVALID_HANDLE_VALUE;
            hFile = INVALID_HANDLE_VALUE;
            hVolume = INVALID_HANDLE_VALUE;
            bCancel->setEnabled(false);
            setReadWriteButtonState();
            return;
        }
        if (numsectors == 0ul)
        {
            progressbar->setRange(0, 100);
        }
        else
        {
            progressbar->setRange(0, (int)numsectors);
        }
        lasti = 0ul;
        timer.start();
        for (i = 0ul; i < numsectors && status == STATUS_READING; i += 1024ul)
        {
            sectorData = readSectorDataFromHandle(hRawDisk, i, (numsectors - i >= 1024ul) ? 1024ul:(numsectors - i), sectorsize);
            if (sectorData == NULL)
            {
                delete sectorData;
                removeLockOnVolume(hVolume);
                CloseHandle(hRawDisk);
                CloseHandle(hFile);
                CloseHandle(hVolume);
                status = STATUS_IDLE;
                sectorData = NULL;
                hRawDisk = INVALID_HANDLE_VALUE;
                hFile = INVALID_HANDLE_VALUE;
                hVolume = INVALID_HANDLE_VALUE;
                bCancel->setEnabled(false);
                setReadWriteButtonState();
                return;
            }
            if (!writeSectorDataToHandle(hFile, sectorData, i, (numsectors - i >= 1024ul) ? 1024ul:(numsectors - i), sectorsize))
            {
                delete sectorData;
                removeLockOnVolume(hVolume);
                CloseHandle(hRawDisk);
                CloseHandle(hFile);
                CloseHandle(hVolume);
                status = STATUS_IDLE;
                sectorData = NULL;
                hRawDisk = INVALID_HANDLE_VALUE;
                hFile = INVALID_HANDLE_VALUE;
                hVolume = INVALID_HANDLE_VALUE;
                bCancel->setEnabled(false);
                setReadWriteButtonState();
                return;
            }
            delete sectorData;
            sectorData = NULL;
            if (timer.elapsed() >= 1000)
            {
                mbpersec = (((double)sectorsize * (i - lasti)) * (1000.0 / timer.elapsed())) / 1024.0 / 1024.0;
                statusbar->showMessage(QString("%1MB/s").arg(mbpersec));
                timer.start();
                lasti = i;
            }
            progressbar->setValue(i);
            QCoreApplication::processEvents();
        }
        removeLockOnVolume(hVolume);
        CloseHandle(hRawDisk);
        CloseHandle(hFile);
        CloseHandle(hVolume);
        sectorData = NULL;
        hRawDisk = INVALID_HANDLE_VALUE;
        hFile = INVALID_HANDLE_VALUE;
        hVolume = INVALID_HANDLE_VALUE;
        progressbar->reset();
        statusbar->showMessage(tr("Done."));
        bCancel->setEnabled(false);
        setReadWriteButtonState();
        if (status == STATUS_CANCELED){
            QMessageBox::information(NULL, tr("Complete"), tr("Read Canceled."));
        } else {
            QMessageBox::information(NULL, tr("Complete"), tr("Read Successful."));

        }
        if(md5CheckBox->isChecked())
        {
            QFileInfo fileinfo(myFile);
            if (fileinfo.exists() && fileinfo.isFile() &&
                    fileinfo.isReadable() && (fileinfo.size() > 0) )
            {
                generateMd5(myFile.toLatin1().data());
            }
        }
	updateMd5CopyButton();
    }
    else
    {
        QMessageBox::critical(NULL, tr("File Info"), tr("Please specify a file to save data to."));
    }
    if (status == STATUS_EXIT)
    {
        close();
    }
    status = STATUS_IDLE;

#endif
}

// getLogicalDrives sets cBoxDevice with any logical drives found, as long
// as they indicate that they're either removable, or fixed and on USB bus
void MainWindow::getLogicalDrives()
{
#ifdef Q_WS_WIN
    // GetLogicalDrives returns 0 on failure, or a bitmask representing
    // the drives available on the system (bit 0 = A:, bit 1 = B:, etc)
    unsigned long driveMask = GetLogicalDrives();
    int i = 0;
    ULONG pID;

    cboxDevice->clear();

    while (driveMask != 0)
    {
        if (driveMask & 1)
        {
            // the "A" in drivename will get incremented by the # of bits
            // we've shifted
            char drivename[] = "\\\\.\\A:\\";
            drivename[4] += i;
            if (checkDriveType(drivename, &pID))
            {
                cboxDevice->addItem(QString("[%1:\\]").arg(drivename[4]), (qulonglong)pID);
            }
        }
        driveMask >>= 1;
        cboxDevice->setCurrentIndex(0);
        ++i;
    }

#endif
}

// support routine for winEvent - returns the drive letter for a given mask
//   taken from http://support.microsoft.com/kb/163503

#ifdef Q_WS_WIN
char FirstDriveFromMask (ULONG unitmask)
{
    char i;

    for (i = 0; i < 26; ++i)
    {
        if (unitmask & 0x1)
        {
            break;
        }
        unitmask = unitmask >> 1;
    }

    return (i + 'A');
}
// register to receive notifications when USB devices are inserted or removed
// adapted from http://www.known-issues.net/qt/qt-detect-event-windows.html
bool MainWindow::winEvent ( MSG * msg, long * result )
{
    if(msg->message == WM_DEVICECHANGE)
    {
        PDEV_BROADCAST_HDR lpdb = (PDEV_BROADCAST_HDR)msg->lParam;
        switch(msg->wParam)
        {
        case DBT_DEVICEARRIVAL:
            if (lpdb -> dbch_devicetype == DBT_DEVTYP_VOLUME)
            {
                PDEV_BROADCAST_VOLUME lpdbv = (PDEV_BROADCAST_VOLUME)lpdb;
                if(DBTF_NET)
                {
                    char ALET = FirstDriveFromMask(lpdbv->dbcv_unitmask);
                    // add device to combo box (after sanity check that
                    // it's not already there, which it shouldn't be)
                    QString qs = QString("[%1:\\]").arg(ALET);
                    if (cboxDevice->findText(qs) == -1)
                    {
                        ULONG pID;
                        char longname[] = "\\\\.\\A:\\";
                        longname[4] = ALET;
                        // checkDriveType gets the physicalID
                        if (checkDriveType(longname, &pID))
                        {
                            cboxDevice->addItem(qs, (qulonglong)pID);
                            setReadWriteButtonState();
                        }
                    }
                }
            }
            break;
        case DBT_DEVICEREMOVECOMPLETE:
            if (lpdb -> dbch_devicetype == DBT_DEVTYP_VOLUME)
            {
                PDEV_BROADCAST_VOLUME lpdbv = (PDEV_BROADCAST_VOLUME)lpdb;
                if(DBTF_NET)
                {
                    char ALET = FirstDriveFromMask(lpdbv->dbcv_unitmask);
                    //  find the device that was removed in the combo box,
                    //  and remove it from there....
                    //  "removeItem" ignores the request if the index is
                    //  out of range, and findText returns -1 if the item isn't found.
                    cboxDevice->removeItem(cboxDevice->findText(QString("[%1:\\]").arg(ALET)));
                    setReadWriteButtonState();
                }
            }
            break;
        } // skip the rest
    } // end of if msg->message
    *result = 0; //get rid of obnoxious compiler warning
    return false; // let qt handle the rest
}

#endif

void MainWindow::updateMd5CopyButton()
{
    // if the md5 checkbox is checked, and there's a value is the md5 label,
    //   make the copy button visible
    if ( md5CheckBox->isChecked() &&  !(md5label->text().isEmpty()) )
    {
    	bMd5Copy->setVisible(true);
    }
    else
    {
    	bMd5Copy->setVisible(false);
    }
}
