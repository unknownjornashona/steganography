#include <opencv2/opencv.hpp>
#include <iostream>
#include <fstream>
#include <vector>
#include <stdexcept>

using namespace std;
using namespace cv;

// Logging class
class Logger {
public:
    Logger(const string &logFile) : logFilePath(logFile) {}

    void log(const string &message) {
        ofstream logFile(logFilePath, ios::app);
        if (logFile.is_open()) {
            logFile << message << endl;
        } else {
            cerr << "Could not open log file!" << endl;
        }
    }

private:
    string logFilePath;
};

// ImageProcessor class
class ImageProcessor {
public:
    void processXray(const Mat &inputImage, Mat &outputImage) {
        if (inputImage.empty()) {
            throw invalid_argument("Input image is empty.");
        }

        // Convert to grayscale (if the image is not already)
        if (inputImage.channels() == 3) {
            cvtColor(inputImage, outputImage, COLOR_BGR2GRAY);
        } else {
            outputImage = inputImage.clone(); // already grayscale
        }

        // Apply Gaussian Blur to reduce noise
        GaussianBlur(outputImage, outputImage, Size(5, 5), 0);

        // Apply a binary threshold to highlight the bones
        threshold(outputImage, outputImage, 50, 255, THRESH_BINARY);
    }
};

// Batch processor function
void batchProcessXrays(const vector<string> &filePaths, Logger &logger) {
    ImageProcessor processor;
    for (const string &filePath : filePaths) {
        try {
            Mat xrayImage = imread(filePath, IMREAD_UNCHANGED);
            if (xrayImage.empty()) {
                throw runtime_error("Could not load image: " + filePath);
            }

            Mat processedImage;
            processor.processXray(xrayImage, processedImage);

            // Create output file name
            string outputFileName = "processed_" + filePath;
            imwrite(outputFileName, processedImage);
            cout << "Processed: " << filePath << " -> " << outputFileName << endl;

        } catch (const exception &e) {
            logger.log(e.what());
        }
    }
}

int main() {
    Logger logger("xray_processing.log");

    // Example file paths (replace with actual paths)
    vector<string> filePaths = {
        "xray_image1.png",
        "xray_image2.png",
        "xray_image3.png"
        // Add more image paths as needed
    };

    batchProcessXrays(filePaths, logger);

    return 0;
}
