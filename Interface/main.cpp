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
	//a.setStyle(QStyleFactory::create("Fusion"));
	Insp::classERIS w;
	w.show();
	return a.exec();
}
