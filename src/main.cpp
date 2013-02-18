/*-------------------------main.cpp------------------------------------------
    Main project is "luckyBackup"
    a backup and sync tool
    project version : 0.4.7

    ===============================================================================================================================
    ===============================================================================================================================
    This file is part of "luckyBackup" project
    Copyright, Loukas Avgeriou
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

    developer       : luckyb 
    last modified   : 16 Jan 2012
    ===============================================================================================================================
    ===============================================================================================================================
*/

// Perform various initial checks
// include the qt libraries needed & the main window header file
// show the main window or execute in command line mode

#include <QApplication>
#include <QTranslator>

#include "luckybackupwindow.h"
#include "commandline.cpp"

int main(int argc, char *argv[])
{
    //arguments test
    if (!argumentsTest(argc,argv))
        return 1;		// error code1: something wrong with the command arguments - app fail to start (or just --help or --version given as args)
    
    //profile validity check for console mode or gui-no profile specified (will use default)
    if ( (runImmediately) && (!loadCurrentProfile()) )
        return 2;		// profile is not valid or does not exist
    
    // task list validity check
    if ( (runImmediately) && (!check_list()) )
        return 3;		// task list is not ok

    // declared directories check
    if ( (runImmediately) && (!check_dirs()) )
        return 4;		// declared dirs are not ok
        
    // declare the rsync & ssh commands for use either at console or gui mode
    declareRsyncCommand();
        
    if (!console)	//execute gui
    {
        QApplication app(argc, argv);
        
        //translation
        QString locale = QLocale::system().name();
        transDir.setPath(relativeTransDir);
        if (transDir.exists())
            appTranslator.load(QString("luckybackup_") + locale, relativeTransDir);
        else 
            appTranslator.load(QString("luckybackup_") + locale, systemTransDir);
        app.installTranslator(&appTranslator);

        luckyBackupWindow luckybackup;
        if (!silentMode)
            luckybackup.show();
        return app.exec();
    }
    else
    //execute luckybackup in console mode
    {
        commandline commandLine;
        return commandLine.result();		//then execute anything necessary
    }

    return 1;
} 

// end of main.cpp---------------------------------------------------------
