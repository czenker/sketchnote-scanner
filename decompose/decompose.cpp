// #include <opencv2/core.hpp>
// #include <opencv2/highgui.hpp>
#include <opencv2/opencv.hpp>

#include <unistd.h>
#include <iostream>
#include <sstream>
#include <string>

#define HSV(h, s, v)  (Vec3b(h * 255 / 360, s * 255 / 100, v * 255 / 100))

using namespace cv;

bool isCloseTo(Vec3b min, Vec3b max, Vec3b hsv) {
    
    bool hIn;
    if (min[0] <= max[0]) {
      hIn = hsv[0] >= min[0] && hsv[0] <= max[0];
    } else {
      hIn = hsv[0] <= max[0] || hsv[0] >= min[0];
    }
    
    return ( hIn && hsv[1] >= min[1] && hsv[1] <= max[1] && hsv[2] >= min[2] && hsv[2] <= max[2]);
}

void usage(char* binaryName) {
    std::cout << "Usage: " << binaryName << " [filename] [tempdir]" << std::endl;
}

int main(int argc, char** argv) {
  
    if(argc < 3) {
        usage(argv[0]);
        return 1;
    }
    
    if (access(argv[1], R_OK) != 0) {
        std::cerr << "Source file does not exist " << argv[1] << std::endl;
	return 1;
    }
    
    if (access(argv[2], W_OK) != 0) {
        std::cerr << "Can not write to directory " << argv[2] << std::endl;
	return 1;
    }
    
    // We'll start by loading an image from the drive
    Mat image = imread(argv[1], CV_LOAD_IMAGE_COLOR);
    cvtColor(image,image,cv::COLOR_BGR2HSV_FULL);
    
    
    Mat channelSanguine(image.rows, image.cols, CV_8UC3, Scalar(0,0,255));
    Mat channelRed(image.rows, image.cols, CV_8UC3, Scalar(0,0,255));
    Mat channelCobaltGreen(image.rows, image.cols, CV_8UC3, Scalar(0,0,255));
    Mat channelPhthaloBlue(image.rows, image.cols, CV_8UC3, Scalar(0,0,255));
    Mat channelIndanthreneBlue(image.rows, image.cols, CV_8UC3, Scalar(0,0,255));
    Mat channelMayGreen(image.rows, image.cols, CV_8UC3, Scalar(0,0,255));
    Mat channelBlack(image.rows, image.cols, CV_8UC3, Scalar(0,0,255));
    
    
    // We check that our image has been correctly loaded
    if(image.empty()) {
        std::cerr << "Error: the image has been incorrectly loaded." << std::endl;
        return 1;
    }
    
    // values determined by experimentation :)
    Vec3b sanguineMin = HSV(358, 30, 40);
    Vec3b sanguineMax = HSV(16, 65, 100);

    Vec3b redMin = HSV(340, 40, 50);
    Vec3b redMax = HSV(360, 80, 100);
    
    Vec3b cobaltGreenMin = HSV(188, 25, 30);
    Vec3b cobaltGreenMax = HSV(204, 70, 100);
    
    Vec3b phthaloBlueMin = HSV(205, 40, 70);
    Vec3b phthaloBlueMax = HSV(227, 80, 100);
    
    Vec3b indanthreneBlueMin = HSV(209, 20, 30);
    Vec3b indanthreneBlueMax = HSV(245, 60, 90);
    
    Vec3b mayGreenMin = HSV(62, 30, 60);
    Vec3b mayGreenMax = HSV(85, 70, 100);
    
    Vec3b blackMin = HSV(0, 0, 0);
    Vec3b blackMax = HSV(360, 35, 70);

    uint64_t countSanguine = 0;
    uint64_t countRed = 0;
    uint64_t countCobaltGreen = 0;
    uint64_t countPhthaloBlue = 0;
    uint64_t countIndanthreneBlue = 0;
    uint64_t countMayGreen = 0;
    uint64_t countBlack = 0;
    
    for(int y=0;y<image.rows;y++) {
	for(int x=0;x<image.cols;x++) {
	    // get pixel
	    Vec3b hsv = image.at<Vec3b>(Point(x,y));
	    
	    if (isCloseTo(sanguineMin, sanguineMax, hsv)) {
	      // set pixel
	      channelSanguine.at<Vec3b>(Point(x,y)) = hsv;
	      countSanguine++;
	    }
	    
	    if (isCloseTo(redMin, redMax, hsv)) {
	      // set pixel
	      channelRed.at<Vec3b>(Point(x,y)) = hsv;
	      countRed++;
	    }
	    
	    if (isCloseTo(cobaltGreenMin, cobaltGreenMax, hsv)) {
	      // set pixel
	      channelCobaltGreen.at<Vec3b>(Point(x,y)) = hsv;
	      countCobaltGreen++;
	    }
	    
	    if (isCloseTo(phthaloBlueMin, phthaloBlueMax, hsv)) {
	      // set pixel
	      channelPhthaloBlue.at<Vec3b>(Point(x,y)) = hsv;
	      countPhthaloBlue++;
	    }
	    
	    if (isCloseTo(indanthreneBlueMin, indanthreneBlueMax, hsv)) {
	      // set pixel
	      channelIndanthreneBlue.at<Vec3b>(Point(x,y)) = hsv;
	      countIndanthreneBlue++;
	    }
	    
	    if (isCloseTo(mayGreenMin, mayGreenMax, hsv)) {
	      // set pixel
	      channelMayGreen.at<Vec3b>(Point(x,y)) = hsv;
	      countMayGreen++;
	    }
	    
	    if (isCloseTo(blackMin, blackMax, hsv)) {
	      // set pixel
	      channelBlack.at<Vec3b>(Point(x,y)) = hsv;
	      countBlack++;
	    }
	}
    }
    
    if(countBlack > 50) {
        std::cout << countBlack << " Black" << std::endl;
        cvtColor(channelBlack,channelBlack,cv::COLOR_HSV2BGR_FULL);
	std::stringstream ss;
	ss << argv[2] << "/black.ppm";
	imwrite(ss.str(), channelBlack);
    } else {
        std::cerr << "Did only find non-black on the drawing. What happend there?" << std::endl;
	return -1;
    }
    
    if(countSanguine > countBlack / 20) {
        std::cout << countSanguine << " Sanguine" << std::endl;
        cvtColor(channelSanguine,channelSanguine,cv::COLOR_HSV2BGR_FULL);
	std::stringstream ss;
	ss << argv[2] << "/sanguine.ppm";
	imwrite(ss.str(), channelSanguine);
    }
    
    if(countRed > countBlack / 20) {
        std::cout << countRed << " Red" << std::endl;
        cvtColor(channelRed,channelRed,cv::COLOR_HSV2BGR_FULL);
	std::stringstream ss;
	ss << argv[2] << "/red.ppm";
	imwrite(ss.str(), channelRed);
    }
    
    if(countCobaltGreen > countBlack / 20) {
        std::cout << countCobaltGreen << " cobaltGreen" << std::endl;
        cvtColor(channelCobaltGreen,channelCobaltGreen,cv::COLOR_HSV2BGR_FULL);
	std::stringstream ss;
	ss << argv[2] << "/cobaltGreen.ppm";
	imwrite(ss.str(), channelCobaltGreen);
    }
    
    if(countPhthaloBlue > countBlack / 20) {
        std::cout << countPhthaloBlue << " Phthalo Blue" << std::endl;
        cvtColor(channelPhthaloBlue,channelPhthaloBlue,cv::COLOR_HSV2BGR_FULL);
	std::stringstream ss;
	ss << argv[2] << "/phthaloBlue.ppm";
	imwrite(ss.str(), channelPhthaloBlue);
    }
    
    if(countIndanthreneBlue > countBlack / 20) {
        std::cout << countIndanthreneBlue << " Indanthrene Blue" << std::endl;
        cvtColor(channelIndanthreneBlue,channelIndanthreneBlue,cv::COLOR_HSV2BGR_FULL);
	std::stringstream ss;
	ss << argv[2] << "/indanthreneBlue.ppm";
	imwrite(ss.str(), channelIndanthreneBlue);
    }
    
    if(countMayGreen > countBlack / 20) {
        std::cout << countMayGreen << " May Green" << std::endl;
        cvtColor(channelMayGreen,channelMayGreen,cv::COLOR_HSV2BGR_FULL);
	std::stringstream ss;
	ss << argv[2] << "/mayGreen.ppm";
	imwrite(ss.str(), channelMayGreen);
    }
    
    return 0;
}
