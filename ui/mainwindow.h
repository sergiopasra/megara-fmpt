// Copyright (c) 2012-2017 Isaac Morales Durán. All rights reserved.
// Institute of Astrophysics of Andalusia, IAA-CSIC
//
// This file is part of FMPT (Fiber MOS Positioning Tools)
//
// FMPT is free software: you can redistribute it and/or modify
// it under the terms of the GNU General Public License as published by
// the Free Software Foundation, either version 3 of the License, or
// (at your option) any later version.
//
// This program is distributed in the hope that it will be useful,
// but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
// GNU General Public License for more details.
//
// You should have received a copy of the GNU General Public License
// along with this program.  If not, see <http://www.gnu.org/licenses/>.

//---------------------------------------------------------------------------
//File: mainwindow.h
//Content: gráfical classes and grphical functions
//Autor: José Enrique Ruíz
//---------------------------------------------------------------------------

#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QLabel>
#include <QDir>
#include <QList>
#include <QTimer>

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0);
    void setFramesDir(QDir framesDir);
    QDir getFramesDir();
    QTimer *timer;

    void readFrames();
    ~MainWindow();

    void generatePPFrames();
    void generateDPFrames();

private slots:
    void open();
    void close();
    void about();
    void loopFrames();
    void displayFrame();    

    void on_horizontalSlider_sliderMoved(int position);

private:

    Ui::MainWindow *ui;

    QDir framesDir;
    QFileInfoList ImageList;
    int index;

    QAction *actionOpen;
    QAction *actionClose;
    QAction *actionAboutFMPT;
    QAction *actionAboutQt;

};

#endif // MAINWINDOW_H
