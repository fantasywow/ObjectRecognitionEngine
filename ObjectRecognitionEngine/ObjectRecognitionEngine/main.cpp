#include <opencv2/core/core.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <opencv2/imgproc/imgproc.hpp>
#include <opencv2/objdetect/objdetect.hpp>
#include <opencv2/features2d/features2d.hpp>
#include <opencv2/nonfree/features2d.hpp>
#include <iostream>
#include <stdio.h>
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
	SurfDescriptorExtractor surfdesc;
	Mat descriptors1, descriptors2;
	surf.detect(image, points);
	surfdesc.compute(image, points, descriptors1);
	surfdesc.compute(image, points, descriptors2);
	drawKeypoints(image, points, image, Scalar(255, 255, 255), DrawMatchesFlags::DRAW_OVER_OUTIMG);


}

void FLANNMatchTest(Mat &image1, Mat &image2){
	vector<cv::KeyPoint> points1, points2;
	SurfFeatureDetector surf(2500);
	SurfDescriptorExtractor surfdesc;
	Mat descriptors1, descriptors2;
	surf.detect(image1, points1);
	surf.detect(image2, points2);
	surfdesc.compute(image1, points1, descriptors1);
	surfdesc.compute(image2, points2, descriptors2);
	FlannBasedMatcher matcher;
	std::vector< DMatch > matches;
	matcher.match(descriptors1, descriptors2, matches);

	double max_dist = 0; double min_dist = 100;

	//-- Quick calculation of max and min distances between keypoints
	for (int i = 0; i < descriptors1.rows; i++)
	{
		double dist = matches[i].distance;
		if (dist < min_dist) min_dist = dist;
		if (dist > max_dist) max_dist = dist;
	}

	printf("-- Max dist : %f \n", max_dist);
	printf("-- Min dist : %f \n", min_dist);

	//-- Draw only "good" matches (i.e. whose distance is less than 2*min_dist,
	//-- or a small arbitary value ( 0.02 ) in the event that min_dist is very
	//-- small)
	//-- PS.- radiusMatch can also be used here.
	std::vector< DMatch > good_matches;

	for (int i = 0; i < descriptors1.rows; i++)
	{
		if (matches[i].distance <= max(2 * min_dist, 0.02))
		{
			good_matches.push_back(matches[i]);
		}
	}

	//-- Draw only "good" matches
	Mat img_matches;
	drawMatches(image1, points1, image2, points2,
		good_matches, img_matches, Scalar::all(-1), Scalar::all(-1),
		vector<char>(), DrawMatchesFlags::NOT_DRAW_SINGLE_POINTS);

	//-- Show detected matches
	imshow("Good Matches", img_matches);

	for (int i = 0; i < (int)good_matches.size(); i++)
	{
		printf("-- Good Match [%d] Keypoint 1: %d  -- Keypoint 2: %d  \n", i, good_matches[i].queryIdx, good_matches[i].trainIdx);
	}


	waitKey(0);
}





int main(){

	cv::Mat image = cv::imread("airplane.jpg", CV_LOAD_IMAGE_GRAYSCALE);
	double duration;
	duration = static_cast<double>(getTickCount());
	//harissTest(image);
	//FASTTest(image);
	//SURFTest(image);
	FLANNMatchTest(image, image);

	//duration =(static_cast<double>(getTickCount()) - duration)*1000;
	//duration /= getTickFrequency();

	//cout << "time:" << duration << endl;


	//imshow("my image", image);
	waitKey(100000);
	return 1;
}