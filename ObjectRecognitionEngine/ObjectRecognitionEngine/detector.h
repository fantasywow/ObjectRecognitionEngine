#include "opencvhead.h"


class Detector {
public:
	virtual void init();
	virtual void getDescriptor(Mat &image, Mat &descriptor) = 0;

};

class SURFdetector :public Detector{
public:
	void init(){};
	void getDescriptor(Mat &image, Mat &descriptor){
		vector<cv::KeyPoint> points;
		SurfFeatureDetector surf(2500);
		SurfDescriptorExtractor surfdesc;
		Mat descriptors;
		surf.detect(image, points);
		surfdesc.compute(image, points, descriptor);
	}


};