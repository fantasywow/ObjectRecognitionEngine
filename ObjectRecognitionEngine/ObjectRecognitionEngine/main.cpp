#include <opencv2/core/core.hpp>
#include <opencv2/highgui/highgui.hpp>

int main(){

	cv::Mat image = cv::imread("test.jpg");
	cv::namedWindow("MY IMAGE");
	cv::imshow("MY IMAGE", image);
	cv::waitKey(5000);
	return 1;
}