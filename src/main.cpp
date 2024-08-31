#include "FLED.h"
#include <iostream>
#include <filesystem>
#include <vector>

namespace fs = std::filesystem;

std::vector<std::string> getImageFiles(const std::string& folder) {
    std::vector<std::string> imageFiles;
    for (const auto& entry : fs::directory_iterator(folder)) {
        if (entry.path().extension() == ".jpg" || entry.path().extension() == ".png") {
            imageFiles.push_back(entry.path().string());
        }
    }
    return imageFiles;
}

void processImage(const std::string& imagePath, const std::string& outputFolder) {
    Mat imgC, imgG;
    AAMED aamed(1000, 2000);
    aamed.SetParameters(CV_PI / 3, 3.4, 0.77); // config parameters of AAMED

    imgC = cv::imread(imagePath);
    if (imgC.empty()) {
        std::cout << "Failed to read image: " << imagePath << std::endl;
        return;
    }

    // Specify the target size while keeping the aspect ratio
    int targetWidth = 960;  // Example target width
    int targetHeight = 540; // Example target height
    int originalWidth = imgC.cols;
    int originalHeight = imgC.rows;

    double aspectRatio = static_cast<double>(originalWidth) / originalHeight;

    // Calculate new dimensions while preserving aspect ratio
    int newWidth, newHeight;
    if (aspectRatio >= 1.0) {
        // Image is wider than it is tall
        newWidth = targetWidth;
        newHeight = static_cast<int>(targetWidth / aspectRatio);
    } else {
        // Image is taller than it is wide
        newHeight = targetHeight;
        newWidth = static_cast<int>(targetHeight * aspectRatio);
    }

    // Resize the image while preserving aspect ratio
    cv::resize(imgC, imgC, cv::Size(newWidth, newHeight));

    // Convert to grayscale
    cv::cvtColor(imgC, imgG, cv::COLOR_RGB2GRAY);
    aamed.run_FLED(imgG); // run AAMED

    cv::Vec<double, 10> detailTime;
    aamed.showDetailBreakdown(detailTime); // show detail execution time of each step

    // Create output filename
    fs::path inputPath(imagePath);
    std::string outputName = "Meng_" + inputPath.stem().string() + ".jpg";
    std::string outputPath = outputFolder + "/" + outputName;

    aamed.drawFLED(imgG, outputPath);
    aamed.writeDetailData();

    std::cout << "Processed: " << imagePath << " -> " << outputPath << std::endl;
}

int main(int argc, char* argv[]) {
    if (argc != 2) {
        std::cout << "Usage: " << argv[0] << " <input_folder>" << std::endl;
        return 1;
    }

    std::string inputFolder = argv[1];
    std::string outputFolder = inputFolder + "/Meng";

    // Create Meng folder if it doesn't exist
    if (!fs::exists(outputFolder)) {
        fs::create_directory(outputFolder);
    }

    std::vector<std::string> imageFiles = getImageFiles(inputFolder);

    for (const auto& imagePath : imageFiles) {
        processImage(imagePath, outputFolder);
    }

    std::cout << "All images processed. Results saved in: " << outputFolder << std::endl;

    return 0;
}
