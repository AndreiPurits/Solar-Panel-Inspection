
#include <stdio.h>
#include "stdafx.h"

#include "hGlobal.h"
#include "hGraphics.h"
#include "ERIS.h"
#include <sstream>
#include <QDir>



#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

oLibHandler Lib;
oGraphicControl MyGraphic;

CEdit* EditLink[PARAM_COUNT];
bool editchangelock = false;
int coordX;
int coordY;




using namespace std;
namespace Insp {

	// Connect signals to slots for UI interactions for 3 screens

	classERIS::classERIS(QWidget* parent, bool useSecondScreenUI, bool useThirdScreenUI) : QMainWindow(parent)
	{
		if (useSecondScreenUI) {
			uiSecond.setupUi(this);

			QObject::connect(uiSecond.transmit, SIGNAL(clicked()), this, SLOT(transmition()));
			
		}
		else if (useThirdScreenUI) {
			uiThird.setupUi(this);

			QObject::connect(uiThird.pushButton_3, SIGNAL(clicked()), this, SLOT(saveReport()));
			QObject::connect(uiThird.pushButton_2, SIGNAL(clicked()), this, SLOT(setPath()));

		}
		else {
			uiERIS.setupUi(this); 

			QObject::connect(uiERIS.actionImport, SIGNAL(triggered()), this, SLOT(importData()));

			QObject::connect(uiERIS.pushButton_plusFrame, SIGNAL(clicked()), this, SLOT(nextFrame()));
			QObject::connect(uiERIS.pushButton_minusFrame, SIGNAL(clicked()), this, SLOT(previousFrame()));
			QObject::connect(uiERIS.pushButton_frame, SIGNAL(clicked()), this, SLOT(setFrameNumber()));

			QObject::connect(uiERIS.pushButton_plusFS, SIGNAL(clicked()), this, SLOT(nextErrorFrame()));
			QObject::connect(uiERIS.pushButton_minusFS, SIGNAL(clicked()), this, SLOT(previousErrorFrame()));
			QObject::connect(uiERIS.pushButton_Error, SIGNAL(clicked()), this, SLOT(setFrameNumberWithError()));

			QObject::connect(uiERIS.pushButton_3, SIGNAL(clicked()), this, SLOT(resetTemp()));
			QObject::connect(uiERIS.pushButton_2, SIGNAL(clicked()), this, SLOT(plusRange()));
			QObject::connect(uiERIS.pushButton, SIGNAL(clicked()), this, SLOT(minusRange()));

			QObject::connect(uiERIS.Auftragsdaten, SIGNAL(clicked()), this, SLOT(findErrors()));

			QObject::connect(uiERIS.pushButton_Speichern, SIGNAL(clicked()), this, SLOT(saveDefects()));

			QObject::connect(uiERIS.pushButton_Auftragsdaten, SIGNAL(clicked()), this, SLOT(openSecondScreen()));
			QObject::connect(uiERIS.pushButton_Bericht, SIGNAL(clicked()), this, SLOT(openThirdScreen()));

			QObject::connect(uiERIS.pushButton_4, SIGNAL(clicked()), this, SLOT(iterationsSet()));
			QObject::connect(uiERIS.pushButton_5, SIGNAL(clicked()), this, SLOT(thresholdSet()));
			QObject::connect(uiERIS.pushButton_7, SIGNAL(clicked()), this, SLOT(maxSizeSet()));
			QObject::connect(uiERIS.pushButton_6, SIGNAL(clicked()), this, SLOT(minSizeSet()));

            // Connect sliders to slots

			QObject::connect(uiERIS.horizontalSlider, SIGNAL(sliderMoved(int)), this, SLOT(sliderIterationsMoved(int)));
			QObject::connect(uiERIS.horizontalSlider_2, SIGNAL(sliderMoved(int)), this, SLOT(sliderThresholdMoved(int)));
			QObject::connect(uiERIS.horizontalSlider_4, SIGNAL(sliderMoved(int)), this, SLOT(sliderMaxSizeMoved(int)));
			QObject::connect(uiERIS.horizontalSlider_3, SIGNAL(sliderMoved(int)), this, SLOT(sliderMinSizeMoved(int)));

			
		}
	}
	
	classERIS::~classERIS() {	}

    // Method to set the maximum size

	void classERIS::maxSizeSet()
	{
		QString text = uiERIS.lineEdit_4->text();
		int newValue = text.toInt();
		maxSize = newValue;
		uiERIS.horizontalSlider_4->setValue(maxSize);
	}

    // Method to handle slider movement for maximum size

	void classERIS::sliderMaxSizeMoved(int value)
	{
		maxSize = value;
		uiERIS.lineEdit_4->setText(QString::number(maxSize));
		changeErrorNumber();
	}

	void classERIS::minSizeSet()
	{
		QString text = uiERIS.lineEdit_3->text();
		int newValue = text.toInt();
		minSize = newValue;
		uiERIS.horizontalSlider_3->setValue(minSize);
	}

	void classERIS::sliderMinSizeMoved(int value)
	{
		minSize = value;
		uiERIS.lineEdit_3->setText(QString::number(minSize));
		changeErrorNumber();
	}

	void classERIS::thresholdSet()
	{
		QString text = uiERIS.lineEdit_2->text();
		int newValue = text.toInt();
		threshold = newValue;
		uiERIS.horizontalSlider_2->setValue(threshold);
	}

	void classERIS::sliderThresholdMoved(int value)
	{
		threshold = value;
		uiERIS.lineEdit_2->setText(QString::number(threshold));
		changeErrorNumber();
	}

	void classERIS::iterationsSet()
	{
		QString text = uiERIS.lineEdit->text();
		int newValue = text.toInt();
		iterations = newValue;
		uiERIS.horizontalSlider->setValue(iterations);
	}

	void classERIS::sliderIterationsMoved(int value)
	{
		iterations = value;
		uiERIS.lineEdit->setText(QString::number(iterations));
		changeErrorNumber();
	}

	void classERIS::saveReport()
	{
		std::string report = "Type of alert: " + uiThird.lineEdit->text().toStdString() + "\n" +
			"Weight of the alert: " + uiThird.lineEdit_2->text().toStdString() + "\n" +
			"Remarks: " + uiThird.plainTextEdit->toPlainText().toStdString() + "\n";
		std::cout << report << std::endl;
	}

	void classERIS::setPath()
	{
		std::string path = "C:\\Andrei\\git\\python\\eris-auswertesoftware-main\\Defects\\";
		QLabel* labelArray[] = { uiThird.label_2, uiThird.label, uiThird.label_3, uiThird.label_4 };

		QDir directory(QString::fromStdString(path));
		QStringList pngFiles = directory.entryList(QStringList("*.png"), QDir::Files);

		for (int i = 0; i < pngFiles.size() && i < 4; ++i)
		{
			std::string imagePath = path + pngFiles[i].toStdString();

			cv::Mat image = cv::imread(imagePath, cv::IMREAD_GRAYSCALE);

			if (!image.empty())
			{
				QImage qtImage(image.data, image.cols, image.rows, image.step, QImage::Format_Grayscale8);

				labelArray[i]->setPixmap(QPixmap::fromImage(qtImage));
				labelArray[i]->setScaledContents(true);
			}
		}
	}



	void classERIS::transmition()
	{	
		userData = "Tester: " + uiSecond.lineEdit->text().toStdString() + "\n" +
			"Location: " + uiSecond.lineEdit_2->text().toStdString() + "\n" +
			"Address: " + uiSecond.lineEdit_3->text().toStdString() + "\n" +
			"Time begin: " + uiSecond.dateTimeEdit->text().toStdString() + "\n" +
			"Time end: " + uiSecond.dateTimeEdit_2->text().toStdString() + "\n";
		std::cout<< userData << std::endl;
	}

	void classERIS::saveDefects()
	{
		std::string path = "C:\\Andrei\\git\\python\\eris-auswertesoftware-main\\Defects\\";
		int currentFrameNumber = uiERIS.lineEdit_Framenummer->text().toInt();
		std::string filename = path + "defect_frame_" + std::to_string(currentFrameNumber) + ".png";
		Lib.SelectFrame(actFN);
		cv::Mat resul = writeTempToRange();
		std::pair<cv::Mat, int> resultAndErrors = fillErrorMat(resul);
		cv::Mat result = resultAndErrors.first;

		const std::type_info& typeInfo = typeid(result);
		std::cout << "Type of data in result: " << typeInfo.name() << std::endl;

		if (cv::imwrite(filename, result))
		{
			std::cout << "Image saved successfully to: " << filename << std::endl;
		}
		else
		{
			std::cout << "Failed to save image to: " << filename << std::endl;
		}
	}

	void classERIS::openSecondScreen()
	{
		secondScreen = new classERIS(nullptr, true, false);
		secondScreen->show(); 
	}

	void classERIS::openThirdScreen()
	{
		thirdScreen = new classERIS(nullptr, false, true);
		thirdScreen->show();
	}
	void classERIS::resetTemp() {
		uiERIS.label_4->setText("15");
		changeFrameNumber();
		changeErrorNumber();
	}
	void classERIS::plusRange() {
		tempRange += 1;
		uiERIS.label_4->setText(QString::number(tempRange));
		changeFrameNumber();
		changeErrorNumber();
	}						
	void classERIS::minusRange() {
		tempRange -= 1;
		uiERIS.label_4->setText(QString::number(tempRange));
		changeFrameNumber();
		changeErrorNumber();
	}
	void classERIS::setFrameNumber() {
		int frameNumber = uiERIS.lineEdit_Framenummer->text().toInt()-1;
		actFN = validFrameNumber(frameNumber);
		uiERIS.lineEdit_Framenummer->setText(QString::number(actFN + 1));
		changeFrameNumber();
	}
	void classERIS::setFrameNumberWithError() {
		int currentErrorIndex = uiERIS.heightInput->text().toInt() - 1;
		int errorFrameNumber = errorFrameNumbers[currentErrorIndex];
		actFN = validErrorNumber(errorFrameNumber);
		uiERIS.heightInput->setText(QString::number(actFN + 1));
		changeErrorNumber();
	}
	void classERIS::nextErrorFrame() {
		int oldFrameNumber = uiERIS.heightInput->text().toInt();
		int errorFrameNumber = errorFrameNumbers[oldFrameNumber + 1];
		actFN = validErrorNumber(errorFrameNumber);
		uiERIS.heightInput->setText(QString::number(actFN));
		changeErrorNumber();
	}
	void classERIS::previousErrorFrame() {
		int oldFrameNumber = uiERIS.heightInput->text().toInt();
		int errorFrameNumber = errorFrameNumbers[oldFrameNumber - 1];
		actFN = validErrorNumber(errorFrameNumber);
		uiERIS.heightInput->setText(QString::number(actFN));
		changeErrorNumber();
	}
	void classERIS::nextFrame() {
		int oldFrameNumber = uiERIS.lineEdit_Framenummer->text().toInt();
		int frameNumber = oldFrameNumber + 1;
		actFN = validFrameNumber(frameNumber);
		uiERIS.lineEdit_Framenummer->setText(QString::number(actFN));
		changeFrameNumber();

	}

	void classERIS::previousFrame() {
		int oldFrameNumber = uiERIS.lineEdit_Framenummer->text().toInt();
		int frameNumber = oldFrameNumber - 1;
		actFN = validFrameNumber(frameNumber);
		uiERIS.lineEdit_Framenummer->setText(QString::number(actFN));
		changeFrameNumber();
	}

	int classERIS::validErrorNumber(int errorFrameNumber) {

		if (errorFrameNumber >= 0 && errorFrameNumber <= int(errorData.size())) {
			return errorFrameNumber;
		}
		else {
			printf("Out of the range!!!!!!!!!! \n");
			return actFN;
		}
	}
	int classERIS::validFrameNumber(int frameNumber) {
		if (frameNumber > 0 && frameNumber <= maxFrameNumber) {
			return frameNumber;
		}
		else {
			printf("Out of the range!!!!!!!!!! \n");
			return actFN;
		}
	}

	void classERIS::importData() {
		{
			CString filename = "";
			char sFrameCount[11];
			QString directory = "C:/Program Files/InfraTec/IRBGRAB_SDK/v4/Testdaten"; // Replace with the path to your directory  
			QString filter = "*.irb"; // Replace with te file extension you want to select
			QString selected_file = QFileDialog::getOpenFileName(nullptr, "Select File", directory, filter);
			filename = selected_file.toStdString().c_str();

#ifdef WIN64
			const char* DLLName = "C:\\Programmieren\\ERIS-Auswertesoftware\\build\\Release\\irbacs_win64.dll";
#else
#ifdef WIN32
			const char* DLLName = "C:\\Program Files\\InfraTec\\IRBACS_SDK\\v2\\irbacs_win64.dll";
#else
#error No platform specifies!;
#endif
#endif
			if (!Lib.IsLibLoaded())
			{
				if (Lib.LoadLib(DLLName))
				{
					std::cout << "Load DLL" << std::endl;
					if (Lib.InitFunctions())
					{
						std::cout << "init functions fails" << std::endl;
					}
					else
					{
						int lenA = lstrlenA((LPCTSTR)filename);
						BSTR unicodestr;
						int lenW = ::MultiByteToWideChar(CP_ACP, 0, (LPCTSTR)filename, lenA, 0, 0);
						if (lenW > 0)
						{
							unicodestr = ::SysAllocStringLen(0, lenW);
							::MultiByteToWideChar(CP_ACP, 0, (LPCTSTR)filename, lenA, unicodestr, lenW);
						}
						char* ansistr;
						lenW = ::SysStringLen(unicodestr);
						lenA = ::WideCharToMultiByte(CP_UTF8, 0, unicodestr, lenW, 0, 0, NULL, NULL);
						if (lenA > 0)
						{
							ansistr = new char[lenA + 1];
							::WideCharToMultiByte(CP_UTF8, 0, unicodestr, lenW, ansistr, lenA, NULL, NULL);
							ansistr[lenA] = 0;
						}

						if (Lib.LoadIRB(ansistr, &MyGraphic))
						{
							std::cout << "Load IRB" << std::endl;
							std::cout <<"Filename: "<< filename <<"\t DLL-Name: \t"<< DLLName << std::endl;
							FormatIntToString(Lib.GetFrameCount(), "", sFrameCount);
							maxFrameNumber = Lib.GetFrameCount();
							std::cout << "Max Frame Number \t" << maxFrameNumber << std::endl;
							std::cout << "Load IRB" << std::endl;
							QString textLabelFrameNummer1 = "from \t";
							QString textLabel1 = textLabelFrameNummer1 + sFrameCount;
							uiERIS.label_maxFrame->setText(textLabel1);
							uiERIS.lineEdit_Framenummer->setText(QString::number(0));
							uiERIS.heightInput->setText(QString::number(0));
							uiERIS.label_4->setText("15");

							uiERIS.lineEdit->setText("7");
							uiERIS.lineEdit_2->setText("194");
							uiERIS.lineEdit_4->setText("3439");
							uiERIS.lineEdit_3->setText("506");

							int iterations = 24;
							int threshold = 195;
							int maxSize = 8000;
							int minSize = 500;

							paintMat(writeTempToRange());
							paintMatError(writeTempToRange());
						}
						else
						{
							std::cout << "File not found" << std::endl;
						}
						delete[] ansistr;
						::SysFreeString(unicodestr);
					}
				}
				else
				{
					std::cout << "Load DLL fails" << std::endl;
				}
			}
			else
			{
				Lib.DeInit();
				MyGraphic.DeInit();
			}
		}
	}

		cv::Mat classERIS::writeTempToRange() {
			cv::Mat tempMat(pixelCameraY, pixelCameraX, CV_64F);
			int pixelX, pixelY;
			double temp;
			tempRange = uiERIS.label_4->text().toInt();
			for (pixelY = 0; pixelY < pixelCameraY; pixelY++) {
				for (pixelX = 0; pixelX < pixelCameraX; pixelX++) {
					temp = Lib.GetTemp(pixelX, pixelY);
					if (temp < 300.0 - tempRange) {
						temp = 300.0 - tempRange; 
					}
					else if (temp > 300.0 + tempRange) { 
						temp = 300.0 + tempRange; 
					}
					tempMat.at<double>(pixelY, pixelX) = temp;
				}
			}
			double minVal, maxVal;
			cv::Point minLoc, maxLoc;
			cv::minMaxLoc(tempMat, &minVal, &maxVal, &minLoc, &maxLoc);
			QString textLabel = QString("Min. temperature: %1, Max. temperature: %2").arg(minVal).arg(maxVal);
			uiERIS.label_19->setText(textLabel);
			return tempMat;
		}



		std::pair<cv::Mat, int> Insp::classERIS::fillErrorMat(cv::Mat matPicture) {
			int frameErrors = 0;
			try {
				cv::normalize(matPicture, matPicture, 0, 255, cv::NORM_MINMAX, CV_8U);
				cv::Mat colorImage;
				cv::cvtColor(matPicture, colorImage, cv::COLOR_GRAY2BGR);
				cv::Mat gray;
				cv::threshold(matPicture, gray, 0, 255, cv::THRESH_BINARY_INV + cv::THRESH_OTSU);
				cv::Mat kernel = cv::getStructuringElement(cv::MORPH_RECT, cv::Size(3, 28));
				cv::Mat opening;
				cv::morphologyEx(gray, opening, cv::MORPH_OPEN, kernel, cv::Point(-1, -1), iterations);
				cv::Mat sure_bg;
				cv::dilate(opening, sure_bg, kernel, cv::Point(-1, -1), 3);
				cv::Mat dist_transform;
				cv::distanceTransform(opening, dist_transform, cv::DIST_L2, 5);
				double max_dist;
				cv::minMaxLoc(dist_transform, nullptr, &max_dist);
				cv::Mat sure_fg;
				cv::threshold(dist_transform, sure_fg, 0.001 * max_dist, 255, 0);
				sure_fg.convertTo(sure_fg, CV_8U);
				cv::Mat unknown = sure_bg - sure_fg;
				cv::Mat markers;
				cv::connectedComponents(sure_fg, markers);
				markers += 1;
				markers.setTo(0, unknown == 255);
				cv::watershed(colorImage, markers);
				cv::Mat mask = (markers != 1);
				cv::Mat result2;
				cv::bitwise_and(matPicture, matPicture, result2, mask);
				cv::Mat img_blur;
				cv::GaussianBlur(result2, img_blur, cv::Size(3, 3), 0);
				cv::Mat edges;
				cv::Canny(img_blur, edges, 21, 21);
				std::vector<std::vector<cv::Point>> contours2;
				cv::findContours(edges, contours2, cv::RETR_CCOMP, cv::CHAIN_APPROX_NONE);
				std::sort(contours2.begin(), contours2.end(), [](const std::vector<cv::Point>& a, const std::vector<cv::Point>& b) {
					return cv::contourArea(a) > cv::contourArea(b);
					});
				for (size_t c = 0; c < contours2.size(); ++c) {
					cv::Rect boundingRect = cv::boundingRect(contours2[c]);
					if (boundingRect.width * boundingRect.height < 50000) {
						cv::rectangle(opening, boundingRect, cv::Scalar(255, 255, 255), -1);
					}
				}
				cv::morphologyEx(opening, opening, cv::MORPH_OPEN, cv::getStructuringElement(cv::MORPH_RECT, cv::Size(5, 5)));
				std::vector<std::vector<cv::Point>> cnts;
				cv::findContours(opening, cnts, cv::RETR_CCOMP, cv::CHAIN_APPROX_NONE);
				std::sort(cnts.begin(), cnts.end(), [](const std::vector<cv::Point>& a, const std::vector<cv::Point>& b) {
					return cv::contourArea(a) > cv::contourArea(b);
					});
				if (!cnts.empty()) {
					cv::RotatedRect rot_rect;
					rot_rect = cv::minAreaRect(cnts[0]);
					cv::Point2f box[4];
					rot_rect.points(box);
					cv::Mat roi = matPicture(cv::Range(int(std::min(box[0].y, box[1].y)), int(std::max(box[2].y, box[3].y))),
						cv::Range(int(std::min(box[0].x, box[1].x)), int(std::max(box[2].x, box[3].x))));
					if (!roi.empty() && roi.rows > 0 && roi.cols > 0) {
						cv::Mat img_gray_roi;
						img_gray_roi = roi.clone();
						cv::Mat binary_mask;
						cv::threshold(img_gray_roi, binary_mask, threshold, 255, cv::THRESH_BINARY);
						std::vector<std::vector<cv::Point>> contours;
						cv::findContours(binary_mask, contours, cv::RETR_EXTERNAL, cv::CHAIN_APPROX_SIMPLE);

						int min_contour_area = minSize;
						int max_contour_area = maxSize;
						std::vector<std::vector<cv::Point>> selected_contours;

						for (const auto& contour : contours) {
							double area = cv::contourArea(contour);
							if (area > min_contour_area && area < max_contour_area) {
								selected_contours.push_back(contour);
								frameErrors++;
							}
						}
						cv::Mat result = roi.clone();
						for (const auto& contour : selected_contours) {
							cv::Rect boundingRect = cv::boundingRect(contour);
							cv::drawContours(result, std::vector<std::vector<cv::Point>>{contour}, -1, cv::Scalar(0, 255, 0), cv::FILLED);
							cv::rectangle(result, boundingRect, cv::Scalar(0, 0, 255), 2);
							cv::putText(result, "Defect", cv::Point(boundingRect.x, boundingRect.y - 10), cv::FONT_HERSHEY_SIMPLEX, 0.5, cv::Scalar(0, 0, 255), 2);
						}

						return std::make_pair(result, frameErrors);
					}
					else {
						std::cout << "original" << std::endl;
						return std::make_pair(matPicture, 0);
					}
				}
				else {
					std::cout << "original" << std::endl;
					return std::make_pair(matPicture, 0);
				}
			}
			catch (const cv::Exception& e) {
				std::cerr << "OpenCV Exception: " << e.what() << std::endl;
				// Handle the exception as needed, e.g., log it or return an error code.
				return std::make_pair(matPicture, -1); // Return an error code.
			}
			catch (const std::exception& e) {
				std::cerr << "Standard Exception: " << e.what() << std::endl;
				// Handle the exception as needed, e.g., log it or return an error code.
				return std::make_pair(matPicture, -1); // Return an error code.
			}
		}
		
	    // Method to find errors similar to outlier_detection.py 

		void classERIS::findErrors()
		{
			int maxFrameNumber = Lib.GetFrameCount();
			for (int frameNumber = 0; frameNumber < maxFrameNumber; frameNumber++)
			{
				int frameErrors = 0;
				Lib.SelectFrame(frameNumber);
				cv::Mat resul = writeTempToRange();
				cv::normalize(resul, resul, 0, 255, cv::NORM_MINMAX, CV_8U);
				std::pair<cv::Mat, int> resultAndErrors = fillErrorMat(resul);
				cv::Mat result = resultAndErrors.first;
				frameErrors = resultAndErrors.second;
				errorData.push_back(std::make_pair(frameNumber, frameErrors));
				errorFrameNumbers.push_back(frameNumber);
				QString heightInputText = uiERIS.heightInput->text();
				if (!heightInputText.isEmpty() && frameErrors != 0)
				{
					QString errorText = QString("Frame %1 has %2 errors.").arg(frameNumber).arg(frameErrors);
					uiERIS.label->setText(uiERIS.label->text() + "\n" + errorText);
				}
			}
			QString textLabelFrameNummer2 = "from \t";
			QString textLabel2 = textLabelFrameNummer2 + QString::number(errorData.size());
			std::cout << errorData.size() << std::endl;
			uiERIS.label_maxImperfection->setText(textLabel2);
		}

		void classERIS::paintMat(cv::Mat matPicture) {
			cv::normalize(matPicture, matPicture, 0, 255, cv::NORM_MINMAX, CV_8U);
			QImage img(matPicture.data, matPicture.cols, matPicture.rows, matPicture.step, QImage::Format_Grayscale8);
			QPixmap pixmap = QPixmap::fromImage(img);
			QPixmap rotated = pixmap.transformed(QTransform().rotate(0));
			uiERIS.label_Bild->setPixmap(rotated.scaled(rotated.width(), rotated.height(), Qt::KeepAspectRatio));
		}


		void classERIS::paintMatError(cv::Mat matPicture) {
			std::pair<cv::Mat, int> resultAndErrors = fillErrorMat(matPicture);
			cv::Mat result = resultAndErrors.first; 
			cv::normalize(result, result, 0, 255, cv::NORM_MINMAX, CV_8U);
			QImage img(result.data, result.cols, result.rows, result.step, QImage::Format_Grayscale8);
			QPixmap pixmap = QPixmap::fromImage(img);
			QPixmap rotated = pixmap.transformed(QTransform().rotate(0));
			uiERIS.label_Fehlstelle->setPixmap(rotated.scaled(rotated.width(), rotated.height(), Qt::KeepAspectRatio));
		}


		void classERIS::changeFrameNumber()
		{
			char Value[64];
			std::string parameterNames[] = {
				"Image width",
				"Image height",
				"Temperature level in K",
				"Temperature span in K",
				"Emissivity",
				"Distance",
				"Path temperature",
				"Environment temperature",
				"Absorption"
			};

			QString outputText;
			for (int i = 0; i < 9; i++) {
				QString line = QString("%1: %2").arg(QString::fromStdString(parameterNames[i])).arg(Lib.GetParameter(i));
				outputText += line + "\n";
			}
			Lib.SelectFrame(actFN);
			uiERIS.label_Uhrzeit->setText(outputText);
			paintMat(writeTempToRange());
		}


		void classERIS::changeErrorNumber()
		{
			Lib.SelectFrame(actFN);
			paintMatError(writeTempToRange());
		}

	}
