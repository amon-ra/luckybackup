/* ---------------------------------- winDialog.cpp --------------------------------------------------------------------------
Class to display a dialog and declare rsync and ssh paths

===============================================================================================================================
===============================================================================================================================
    This file is part of "luckyBackup" project
    Copyright 2008-2012, Loukas Avgeriou
    luckyBackup is distributed under the terms of the GNU General Public License
    luckyBackup is free software: you can redistribute it and/or modify
    it under the terms of the GNU General Public License as published by
    the Free Software Foundation, either version 3 of the License, or
    (at your option) any later version.

    luckyBackup is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU General Public License for more details.

    You should have received a copy of the GNU General Public License
    along with luckyBackup.  If not, see <http://www.gnu.org/licenses/>.


project version    : Please see "main.cpp" for project version

developer          : luckyb 
last modified      : 17 Jan 2012

===============================================================================================================================
===============================================================================================================================
*/

#include "winDialog.h"

// class winDialog Constructor=================================================================================================
// Displays a simple dialog with text info, ok-cancel buttons and lineEdits for user input
winDialog::winDialog (QWidget *parent) : QDialog (parent)
{
    uiD.setupUi(this);
    
    uiD.lineEdit_rsyncPath -> setText(rsyncCommandPath);
    uiD.lineEdit_sshPath -> setText(sshCommandPath);
    
    
    //connect okay & cancel pushButton SLOTs ----------------
    connect ( uiD.pushButton_ok, SIGNAL( clicked() ), this, SLOT( okay() ) );
    connect ( uiD.pushButton_cancel, SIGNAL( clicked() ), this, SLOT( cancel() ) );

    //Connect all toolButtons with the browse SLOT
    //Map all the toolButton signals and transmit a different integer depending on the toolbutton
    QSignalMapper *signalMapper = new QSignalMapper(this);
    signalMapper ->    setMapping ( uiD.toolButton_rsync, 0 );
    connect (uiD.toolButton_rsync, SIGNAL (clicked() ), signalMapper , SLOT (map()));
    signalMapper ->    setMapping ( uiD.toolButton_ssh, 1 );
    connect (uiD.toolButton_ssh, SIGNAL (clicked() ), signalMapper , SLOT (map()));
    connect(signalMapper, SIGNAL(mapped( const int)), this, SLOT( browse( const int) )  );
    
    //Connect all toolButtons with the useDefault SLOT
    //Map all the toolButton signals and transmit a different integer depending on the toolbutton
    QSignalMapper *signalMapper2 = new QSignalMapper(this);
    signalMapper2 ->    setMapping ( uiD.toolButton_defaultRsync, 0 );
    connect (uiD.toolButton_defaultRsync, SIGNAL (clicked() ), signalMapper2 , SLOT (map()));
    signalMapper2 ->    setMapping ( uiD.toolButton_defaultSsh, 1 );
    connect (uiD.toolButton_defaultSsh, SIGNAL (clicked() ), signalMapper2 , SLOT (map()));
    connect(signalMapper2, SIGNAL(mapped( const int)), this, SLOT( useDefault( const int) )  );
    
    //initialize a completer for the lineEdits
    QCompleter *completer = new QCompleter(this);
    completer                       -> setModel(new QDirModel(completer));
    uiD.lineEdit_rsyncPath          -> setCompleter(completer);
    uiD.lineEdit_sshPath            -> setCompleter(completer);
}


// --------------------------------okay pressed------------------------------------------------
void winDialog::okay()
{
    rsyncCommandPath    = uiD.lineEdit_rsyncPath -> text();
    sshCommandPath      = uiD.lineEdit_sshPath -> text();
    
    close();
}

// --------------------------------cancel pressed------------------------------------------------
void winDialog::cancel()
{
    close();
}

void winDialog::closeEvent(QCloseEvent *event)
{
    event->accept();
}

// browse button pressed=====================================================================================================
//opens a QFileDialog to browse for files/folders locally
void winDialog::browse(const int type)
{
    QString newLineEdit;
    switch (type)
    {
        //rsync browse button
        case 0:    newLineEdit = QFileDialog::getOpenFileName (this, tr("Select rsync command",
            "file selection dialog title"),uiD.lineEdit_rsyncPath -> text(),tr("Executable Files (*.exe)"));
            if (newLineEdit != "")    //if something is selected indeed
                uiD.lineEdit_rsyncPath  -> setText(modifySlashes(newLineEdit));
            break;
        //ssh browse button
        case 1: newLineEdit = QFileDialog::getOpenFileName (this, tr("Select ssh command",
            "file selection dialog title"),uiD.lineEdit_sshPath -> text(),tr("Executable Files (*.exe)"));
            if (newLineEdit != "")    //if something is selected indeed
                uiD.lineEdit_sshPath -> setText(modifySlashes(newLineEdit));
            break;
    }

}

// default button pressed=====================================================================================================
//opens a QFileDialog to browse for files/folders locally
//This dialog is possible only for non-*nix systems
void winDialog::useDefault(const int type)
{
    QString newLineEdit;
    switch (type)
    {
        //rsync default button
        case 0:    if (WINrunning)
                        uiD.lineEdit_rsyncPath  -> setText(rsyncDefaultWinCommand);     // Default rsync path for windows (declared in global.h)
                   if (OS2running)
                        uiD.lineEdit_rsyncPath  -> setText(rsyncDefaultCommand);        // Default rsync path for OS2 (declared in global.h)
            break;
        //ssh default button
        case 1: if (WINrunning)
                        uiD.lineEdit_sshPath  -> setText(sshDefaultWinCommand);     // Default ssh path for windows (declared in global.h)
                if (OS2running)
                        uiD.lineEdit_sshPath  -> setText(sshDefaultCommand);        // Default ssh path for OS2 (declared in global.h)
            break;
    }
}

// modifySlashes ===========================================================================================================
// function to change / to \ for windows (normaly when file dialog is used)
QString winDialog::modifySlashes(QString pathToModify)
{    
    pathToModify.replace("/",SLASH);
    return pathToModify;
}

// end of winDialog.cpp ---------------------------------------------------------------------------

