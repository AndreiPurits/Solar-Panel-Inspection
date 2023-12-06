
//#if !defined(AFX_ERIS_H__2DDD2ED6_E2C1_44B4_AB6F_E8CE0E33319C__INCLUDED_)
//#define AFX_ERIS_H__2DDD2ED6_E2C1_44B4_AB6F_E8CE0E33319C__INCLUDED_

//#if _MSC_VER > 1000
//#pragma once
//#endif // _MSC_VER > 1000
/*
#ifndef __AFXWIN_H__
#error include 'stdafx.h' before including this file for PCH
#endif
*/


//#ifndef ERIS_H
//#define ERIS_H

#include <QWidget>
#include <QTreeWidgetItem>
#include <QDialog>


#include "StdAfx.h"
#include "../build/ui_ERIS.h"
#include "ClickableLabel.h"
#include <opencv2/opencv.hpp>
#include <opencv2/core/mat.hpp>

//#include "afxwin.h"
//#include "afxcmn.h"


#include <stdio.h>
#include <direct.h>
#include <winbase.h>



#include "afxwin.h"
#include "afxcmn.h"
#include "hIRBACS.h"
#include "hGlobal.h"
//#include "hGraphics.h"

//#include <qwidget.h>
#include <QFileDialog>
#include <QMessageBox>
/*
#include <QDir>
#include <QDirIterator>

#include <QStringList>
#include <QLatin1String>
#include <QCommonStyle>
*/




namespace Insp {
	class classERIS : public QMainWindow {
		Q_OBJECT

	public:
		classERIS(QWidget *parent = 0);
		~classERIS();
		
	private:
		Ui::ERISMainWindow uiERIS;


	public:
		int pixelCameraX = 640;
		int pixelCameraY = 480;
		int maxFrameNumber;
		int actFN =0;
		
		//oGraphicControl graphicC;
		CEdit edtCoordX;
		CEdit edtCoordY;
		CEdit edtTemp;
		CEdit edtFrameIndex;
		CEdit edtFrameCount;
		CEdit edtTempBlackbody;
		void ActualizeFrameParameters(int FrameIndex);
		//void ActualizePixelValues(CEdit* edtTemp, CEdit* edtTempBB);
	private: 
		void ActualizePixelValues(CEdit* edtTemp, CEdit* edtTempBB);
		void writeTempretureArrayToFile();
		cv::Mat writeTempToMat();
		void paintMat(cv::Mat matPicture);
		void changeFrameNumber();
		int validFrameNumber(int frameNumber);
		void paintMatError(cv::Mat matPicture);
		//cv::Mat matTemp(pixelX, pixelY, CV_64F);

	private slots:
		//TODO
		void importData();
		void nextFrame();
		void previousFrame();
		void setFrameNumber();
		//void bigPicture(int pic); //Original und entzerrt
		//void startDataAnalysis();
		//void nextFrame();
		//void previusFrame();
		//void nextImperfection();
		//void previusImperfection();
		//void setThermoPics();
		//void createThermoPics(); //Original, entzerrt
		//void setJobData();
		//void exportData();3


	};

}