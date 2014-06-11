#include <opencv2/core/core.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <opencv2/imgproc/imgproc.hpp>
#include <opencv2/objdetect/objdetect.hpp>
#include <opencv2/features2d/features2d.hpp>
#include <opencv2/nonfree/features2d.hpp>
#include <iostream>
using namespace cv;
using namespace std;

void harissTest(Mat &image){
	Mat cornerStrength;
	
	Mat harrisCorners;
	Mat cornerMap, localMax;

	Mat dilated;
	double minStrength, maxStrength;
	double threshold = 0.0001;
	cornerHarris(image, cornerStrength, 3, 3, 0.01);
	//minMaxLoc(cornerStrength, &minStrength, &maxStrength);
	dilate(cornerStrength, dilated, Mat());
	compare(cornerStrength, dilated, localMax, CMP_EQ);
	cv::threshold(cornerStrength, harrisCorners, threshold, 255, THRESH_BINARY);
	harrisCorners.convertTo(cornerMap, CV_8U);
	bitwise_and(cornerMap, localMax, cornerMap);
	vector<Point> points;
	for (int y = 0; y < cornerMap.rows; y++)
	{
		const uchar* rowPtr = cornerMap.ptr<uchar>(y);
		for (int x = 0; x < cornerMap.cols; x++)
		{
			if (rowPtr[x])
			{
				points.push_back(Point(x, y));
			}
		}
	}

	for (int i = 0; i < points.size(); i++)
	{
		circle(image, points[i], 3, Scalar(255, 255, 255), 1, CV_AA);
	}

	

}

void FASTTest(Mat &image){
	vector<cv::KeyPoint> points;
	FastFeatureDetector fast(40);
	fast.detect(image, points);
	drawKeypoints(image, points, image, Scalar(255, 255, 255), DrawMatchesFlags::DRAW_OVER_OUTIMG);
}

void SURFTest(Mat &image){
	vector<cv::KeyPoint> points;
	SurfFeatureDetector surf(2500);
	surf.detect(image, points);
	drawKeypoints(image, points, image, Scalar(255, 255, 255), DrawMatchesFlags::DRAW_OVER_OUTIMG);

}



int main(){

	cv::Mat image = cv::imread("airplane.jpg", CV_LOAD_IMAGE_GRAYSCALE);
	double duration;
	duration = static_cast<double>(getTickCount());
	//harissTest(image);
	FASTTest(image);
	//SURFTest(image);

	duration =(static_cast<double>(getTickCount()) - duration)*1000;
	duration /= getTickFrequency();

	cout << "time:" << duration << endl;


	imshow("my image", image);
	waitKey(100000);
	return 1;
}