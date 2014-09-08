#include "opencvhead.h"


#include <iostream>
#include <stdio.h>
#include "json/json.h"

#include "jsonTest.h"
#include "engine.h"
using namespace std;

#include "leveldb/db.h"
#pragma comment(lib, "Shlwapi")

void getSURFDescriptor(Mat &image, Mat &descriptor){
	vector<cv::KeyPoint> points;
	SurfFeatureDetector surf(2500);
	SurfDescriptorExtractor surfdesc;
	Mat descriptors;
	surf.detect(image, points);
	surfdesc.compute(image, points, descriptor);
}

double getFLANNMatchRatio(Mat &descriptor1, Mat &descriptor2){
	FlannBasedMatcher matcher;
	std::vector< DMatch > matches;
	matcher.match(descriptor1, descriptor2, matches);

	//get the good match
	double max_dist = 0; double min_dist = 100;
	for (int i = 0; i < descriptor1.rows; i++)
	{
		double dist = matches[i].distance;
		if (dist < min_dist) min_dist = dist;
		if (dist > max_dist) max_dist = dist;
	}
	printf("-- Max dist : %f \n", max_dist);
	printf("-- Min dist : %f \n", min_dist);
	std::vector< DMatch > good_matches;
	for (int i = 0; i < descriptor1.rows; i++)
	{
		if (matches[i].distance <= max(2 * min_dist, 0.02))
		{
			good_matches.push_back(matches[i]);
		}
	}
	return double(good_matches.size()) / descriptor1.rows;
}




int main(){

	writeJson();
	readJson();


	cv::Mat image = cv::imread("airplane.jpg", CV_LOAD_IMAGE_GRAYSCALE);
	double duration;
	duration = static_cast<double>(getTickCount());

	vector<cv::KeyPoint> points1, points2;
	Mat descriptors1, descriptors2;
	getSURFDescriptor(image, descriptors1);
	getSURFDescriptor(image, descriptors2);
	double result = getFLANNMatchRatio(descriptors1, descriptors2);
	result;
	leveldb::DB* db;
	leveldb::Options options;
	options.create_if_missing = true;
	std::string dbpath = "testdb";
	leveldb::Status status = leveldb::DB::Open(options, dbpath, &db);
	assert(status.ok());

	leveldb::Status s;
	//s = db->Put(leveldb::WriteOptions(), key1, key2);/*key1和key2作为一对key-value对插入*/
	//s = db->Get(leveldb::ReadOptions(), key1, &value);/*根据key返回对应的value值*/

	//==== storing data ====
	FileStorage fsin(".xml", FileStorage::WRITE + FileStorage::MEMORY);
	fsin << "mat" << descriptors1;
	string buf = fsin.releaseAndGetString();
	string value = "first";
	string outTest;
	db->Put(leveldb::WriteOptions(), buf, value);
	db->Get(leveldb::ReadOptions(), buf, &outTest);
	//==== reading it back ====
	FileStorage fsout(buf, FileStorage::READ + FileStorage::MEMORY);
	fsout["mat"] >> descriptors2;

	FlannBasedMatcher matcher;
	std::vector< DMatch > matches;
	matcher.match(descriptors1, descriptors2, matches);

	delete db;/*关闭数据库*/

	//FLANNMatchTest(image, image);
	//leveldbtest(); 
	//duration =(static_cast<double>(getTickCount()) - duration)*1000;
	//duration /= getTickFrequency();

	//cout << "time:" << duration << endl;


	//imshow("my image", image);
	//waitKey(100000);
	return 1;
}