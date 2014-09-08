#include "detector.h"
#include "matcher.h"
#include "leveldb/db.h"
#include "opencvhead.h"
#include "json/json.h"


class OREngine {
private:
	Detector *detector;
	Matcher *matcher;
	leveldb::DB* db;

public:
	bool init(Detector *detector, Matcher *matcher,string dbpath){
		this->detector = detector;
		this->matcher = matcher;

		leveldb::Options options;
		options.create_if_missing = true;
		leveldb::Status status = leveldb::DB::Open(options, dbpath, &db);
		if (status.ok())return false;

		return true;
	}

	bool addPicture(string picPath, vector<string> tags){
		cv::Mat image = cv::imread(picPath, CV_LOAD_IMAGE_GRAYSCALE);
		Mat descriptors;
		detector->getDescriptor(image, descriptors);

		FileStorage fsin(".xml", FileStorage::WRITE + FileStorage::MEMORY);
		//mat指示格式
		fsin << "mat" << descriptors;
		string key = fsin.releaseAndGetString();

		Json::Value root, arrayObj;
		for (size_t i = 0; i < tags.size(); i++)
		{
			arrayObj.append(tags[i]);
		}
		root["tags"] = arrayObj;
		Json::FastWriter writer;
		std::string value = writer.write(root);

		db->Put(leveldb::WriteOptions(), key, value);
	}

	vector<string> recognition(string picPath){

	}


	~OREngine()
	{
		delete db;
	}


};

