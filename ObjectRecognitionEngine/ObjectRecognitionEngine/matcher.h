#include "opencvhead.h"


class Matcher {
public:
	virtual void init();
	virtual double getMatchRatio(Mat &image, Mat &descriptor) = 0;

};

class FlannMatcher : public Matcher{
public:
	void init(){};
	double getMatchRatio(Mat &descriptor1, Mat &descriptor2){
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
};