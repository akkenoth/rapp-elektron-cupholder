#include <chrono>
#include <thread>

#include <rapp-robots-api/info/info.hpp>
#include <rapp-robots-api/communication/communication.hpp>
#include <rapp-robots-api/vision/vision.hpp>
#include <opencv2/opencv.hpp>

using camera_resolution = rapp::robot::vision::camera_resolution;

int main(int argc, char * argv[]) {
	// create info module
	rapp::robot::info info(argc, argv);
	// create communication module
	rapp::robot::communication comm(argc, argv);
	// create vision module
	rapp::robot::vision vis(argc, argv);

	std::this_thread::sleep_for(std::chrono::seconds(1));
	rapp::object::picture::Ptr picture;
	picture = vis.capture_image(4, camera_resolution::vga, "jpg");

	// convert captured image to OpenCV format
	cv::Mat img;
	img = cv::imdecode(picture->bytearray(), CV_LOAD_IMAGE_ANYDEPTH|CV_LOAD_IMAGE_ANYCOLOR);

	cv::imwrite("test.jpg", img);
	cv::namedWindow("testImg", CV_WINDOW_AUTOSIZE);
	cv::imshow("testImg", img);

	cv::waitKey(0);

/*
	// check image size and cut middle part
	cv::Size s = img.size();
	cv::Mat img2 = img(cv::Rect(s.width * 0.3, s.height * 0.3, s.width * 0.7, s.height * 0.7));

	// create ranges for histogram; note, that hue values are divided by 2 in OpenCV
	// set number of histogram bins
	int histSize[] = {7};
	float hranges[] = {0, 10, 29, 35, 85, 126, 160, 181};
	const float * ranges[] = {hranges};
	int channels[] = {0}; // use only H channel

	// calculate histogram for hue channel
	cv::MatND hist;
	cv::calcHist(&img, 1, channels, cv::Mat(), hist, 1, histSize, ranges, false, false);

	// add first and last bin, as both are for red color
	hist.at<float>(0) += hist.at<float>(6);

	// find maximum value
	float max = -1;
	int maxid = -1;
	for (int i = 0; i < 5; ++i) {
		if (hist.at<float>(i) > max) {
			max = hist.at<float>(i);
			maxid = i;
		}
	}

	// tell the name of the dominant color
	std::vector<std::string> colors = {"red", "orange", "yelow", "green", "blue", "violet", "red"};
	comm.text_to_speech(std::string("I can see the ") + colors[maxid] + " color!");
*/

	return 0;
}
