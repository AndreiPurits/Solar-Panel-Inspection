
#include <QWidget>
#include <QTreeWidgetItem>
#include <QDialog>


#include "StdAfx.h"
#include "../build_last_hope/ui_ERIS.h"
#include "../build_last_hope/ui_second_screen.h"
#include "../build_last_hope/ui_third_screen.h"
#include "ClickableLabel.h"
#include "opencv2/opencv.hpp"
#include "opencv2/core/mat.hpp"
#include <opencv2/highgui/highgui.hpp>

#include <stdio.h>
#include <direct.h>
#include <winbase.h>



#include "afxwin.h"
#include "afxcmn.h"
#include "hIRBACS.h"
#include "hGlobal.h"

#include <QFileDialog>
#include <QMessageBox>



namespace Insp {
	class classERIS : public QMainWindow {
		Q_OBJECT

	public:
		classERIS(QWidget* parent = 0, bool useSecondScreenUI = false, bool useThirdScreenUI = false);
		~classERIS();
		
	private:
		Ui::ERISMainWindow uiERIS;
		Ui::SecondWindow uiSecond;
		Ui::MainWindow uiThird;
	public:
		int pixelCameraX = 640;
		int pixelCameraY = 480;
		int maxFrameNumber;
		int actFN = 0;
		int tempRange;
		int currentErrorIndex;
		int totalErrors;
		int frameErrors;

		// Four parameters for image preprocessing 

		int iterations;
		int threshold;
		int maxSize;
		int minSize;

		// Frame number and quantity of errors are input in errorData

		std::vector<std::pair<int, int>> errorData;
		std::vector<int> errorFrameNumbers;

		std::string userData;
		std::string report;

		CEdit edtCoordX;
		CEdit edtCoordY;
		CEdit edtTemp;
		CEdit edtFrameIndex;
		CEdit edtFrameCount;
		CEdit edtTempBlackbody;
	private: 

		std::pair<cv::Mat, int> fillErrorMat(cv::Mat matPicture);
		cv::Mat writeTempToRange();

		void paintMat(cv::Mat matPicture);
		void paintMatError(cv::Mat matPicture);
		void saveReport();

		Insp::classERIS* secondScreen = nullptr;
		Insp::classERIS* thirdScreen = nullptr;

		void changeFrameNumber();
		void changeErrorNumber();

		int validFrameNumber(int frameNumber);
		int validErrorNumber(int frameNumber);


	private slots:
		void iterationsSet();
		void thresholdSet();
		void maxSizeSet();
		void minSizeSet();


		void sliderIterationsMoved(int iterations);
		void sliderThresholdMoved(int threshold);
		void sliderMaxSizeMoved(int maxSize);
		void sliderMinSizeMoved(int minSize);

		void transmition();
		void setPath();
		void openSecondScreen();
		void openThirdScreen();
		void saveDefects();
		void resetTemp();
		void plusRange();
		void minusRange();
		void findErrors();
		void importData();
		void nextFrame();
		void previousFrame();
		void setFrameNumber();
		void setFrameNumberWithError();
		void nextErrorFrame();
		void previousErrorFrame();
	};

}