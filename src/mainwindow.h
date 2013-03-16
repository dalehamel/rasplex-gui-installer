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
 *  Copyright (C) 2009-2013 ImageWriter developers                   *
 *                           https://launchpad.net/~image-writer-devs *
 **********************************************************************/

#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#ifndef WINVER
#define WINVER 0x0500
#endif

#include <QtGui>
#include <QClipboard>
#include <cstdio>
#include <cstdlib>
#ifdef Q_WS_WIN
    #include <windows.h>
    #include <winioctl.h>
#endif
#include "ui_mainwindow.h"
#include "disk.h"

class MainWindow : public QMainWindow, public Ui::MainWindow
{
	Q_OBJECT
	public:
		MainWindow(QWidget *parent=0);
		~MainWindow();
		void closeEvent(QCloseEvent *event);
        enum Status {STATUS_IDLE=0, STATUS_READING, STATUS_WRITING, STATUS_EXIT, STATUS_CANCELED};
        #ifdef Q_WS_WIN
            bool winEvent ( MSG * msg, long * result );
        #endif
	protected slots:
		void on_tbBrowse_clicked();
		void on_bCancel_clicked();
		void on_bWrite_clicked();
		void on_bRead_clicked();
        void on_leFile_textChanged();
		void on_leFile_editingFinished();
		void on_md5CheckBox_stateChanged();
		void on_bMd5Copy_clicked();
	private:
		// find attached devices
		void getLogicalDrives();
		void setReadWriteButtonState();

        #ifdef Q_WS_WIN
            HANDLE hVolume;
            HANDLE hFile;
            HANDLE hRawDisk;
        #endif
		unsigned long long sectorsize;
		int status;
		char *sectorData;
		QTime timer;
		QClipboard *clipboard;
		void generateMd5(char *filename);
		QString myHomeDir;
		void updateMd5CopyButton();
};

#endif // MAINWINDOW_H
