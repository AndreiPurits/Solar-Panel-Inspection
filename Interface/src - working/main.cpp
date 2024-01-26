#pragma comment(linker, "/SUBSYSTEM:console /ENTRY:mainCRTStartup")
#include "StdAfx.h"
#include "ERIS.h"
//#include "QStyleFactory"
#include "QDebug"
#include <QtWidgets/QApplication>
#include <iostream>
//#include <gtest\gtest.h>




int main(int argc, char* argv[])
{
	QCoreApplication::addLibraryPath("./");
	QApplication a(argc, argv);
	Insp::classERIS mainScreenInstance(nullptr, false, false);
	mainScreenInstance.show();
	return a.exec();
}
