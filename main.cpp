#include <opencv2/opencv.hpp>
#include <iostream>
#include <vector>
#include <fstream>
#include <bitset>

using namespace cv;
using namespace std;

void encodeLSB(string imgPath,string scriptPath,string outputPath){
    Mat img = imread(imgPath);
    if(img.empty())return;


    //citire scriptBash
    ifstream scriptFile(scriptPath,ios::binary);

    vector<char> scriptData((istreambuf_iterator<char>(scriptFile)), istreambuf_iterator<char>());
    scriptData.push_back('\0');


    int bitIndex = 0;
    int charIndex = 0;

    for(int i=0;i<img.rows;i++){
        for(int j=0;j<img.cols;j++){
            if(charIndex < scriptData.size()){
                int bit = (scriptData[charIndex] >> bitIndex)&1; //bitcurent din caracterul curent
                img.at<Vec3b>(i,j)[0] &= ~1; //resetare ultimul bit la 0
                img.at<Vec3b>(i,j)[0] |=bit; //setare bit din script

                bitIndex++;
                if(bitIndex == 8){
                    bitIndex = 0;
                    charIndex++;
                }
            }
        }
    }
    imwrite(outputPath,img);
    cout<<"Script Ascus cu succes in "<< outputPath<<endl;

}
void encodeMSB(string imgPath, string scriptPath, string outputPath) {
    Mat img = imread(imgPath);
    if (img.empty()) return;

    ifstream scriptFile(scriptPath, ios::binary);
    vector<char> scriptData((istreambuf_iterator<char>(scriptFile)), istreambuf_iterator<char>());
    scriptData.push_back('\0');

    int bitIdx = 0;
    int charIdx = 0;

    for (int r = 0; r < img.rows; r++) {
        for (int c = 0; c < img.cols; c++) {
            if (charIdx < scriptData.size()) {
                int bit = (scriptData[charIdx] >> bitIdx) & 1;
                img.at<Vec3b>(r, c)[0] &= ~(1 << 7); 
                if(bit) {
                    img.at<Vec3b>(r, c)[0] |= (1 << 7); 
                }

                bitIdx++;
                if (bitIdx == 8) {
                    bitIdx = 0;
                    charIdx++;
                }
            }
        }
    }
    imwrite(outputPath, img);
    cout << "MSB Demo: Imaginea a fost distrusa vizual si salvata in " << outputPath << endl;
}
void showRichHistogram(string windowName, Mat& image) {
    vector<Mat> bgr_planes;
    split(image, bgr_planes);
    int histSize = 256;
    float range[] = { 0, 256 };
    const float* histRange = { range };
    
    Mat b_hist, g_hist, r_hist;
    calcHist(&bgr_planes[0], 1, 0, Mat(), b_hist, 1, &histSize, &histRange);
    calcHist(&bgr_planes[1], 1, 0, Mat(), g_hist, 1, &histSize, &histRange);
    calcHist(&bgr_planes[2], 1, 0, Mat(), r_hist, 1, &histSize, &histRange);

    int hist_w = 512, hist_h = 400;
    Mat histImage(hist_h, hist_w, CV_8UC3, Scalar(20, 20, 20)); 

    normalize(b_hist, b_hist, 0, histImage.rows, NORM_MINMAX);
    normalize(g_hist, g_hist, 0, histImage.rows, NORM_MINMAX);
    normalize(r_hist, r_hist, 0, histImage.rows, NORM_MINMAX);

    for(int i = 1; i < histSize; i++) {
        line(histImage, Point(2*(i-1), hist_h - cvRound(b_hist.at<float>(i-1))),
             Point(2*(i), hist_h - cvRound(b_hist.at<float>(i))), Scalar(255, 0, 0), 2);

        line(histImage, Point(2*(i-1), hist_h - cvRound(g_hist.at<float>(i-1))),
             Point(2*(i), hist_h - cvRound(g_hist.at<float>(i))), Scalar(0, 255, 0), 2);

        line(histImage, Point(2*(i-1), hist_h - cvRound(r_hist.at<float>(i-1))),
             Point(2*(i), hist_h - cvRound(r_hist.at<float>(i))), Scalar(0, 0, 255), 2);
    }
    imshow(windowName, histImage);
}
void showLSBPlane(string windowName, Mat& img, int channel = 0) {
    Mat bitPlane = Mat::zeros(img.size(), CV_8UC1);
    for(int i = 0; i < img.rows; i++) {
        for(int j = 0; j < img.cols; j++) {
            // Extragem ultimul bit și îl înmulțim cu 255 pentru a fi vizibil
            int lsb = img.at<Vec3b>(i,j)[channel] & 1;
            bitPlane.at<uchar>(i,j) = lsb * 255;
        }
    }
    imshow(windowName, bitPlane);
}
void extractAndRun(string stegoPath) {
    Mat img = imread(stegoPath);
    if(img.empty()) {
        cout << "Eroare: Nu pot citi imaginea stego pentru extractie!" << endl;
        return;
    }

    string extractedScript = "";
    unsigned char currentChar = 0; 
    int bitIdx = 0;

    for (int r = 0; r < img.rows; r++) {
        for (int c = 0; c < img.cols; c++) {
            int bit = img.at<Vec3b>(r, c)[0] & 1;
            
            if (bit) {
                currentChar |= (1 << bitIdx);
            } else {
                currentChar &= ~(1 << bitIdx);
            }
            
            bitIdx++;

            if (bitIdx == 8) {
                if (currentChar == '\0') goto done; 
                extractedScript += (char)currentChar;
                currentChar = 0;
                bitIdx = 0;
            }
        }
    }
done:
    if(extractedScript.empty()) {
        cout << "Eroare: Scriptul extras este gol!" << endl;
        return;
    }

    ofstream out("extracted.sh");
    out << extractedScript;
    out.close();
    
    cout << "Script extras cu succes. Rulez..." << endl;
    system("chmod +x extracted.sh && ./extracted.sh");
}

int main() {
   string originalImg = "input.png";
   string secretScript = "payload.sh";
   string stegoImg = "stego_outputLSB.png";
   string stegoImg2 = "stego_outputMSB.png";
   encodeLSB(originalImg,secretScript,stegoImg);
   encodeMSB(originalImg,secretScript,stegoImg2);
   Mat imgOrig = imread(originalImg);
   Mat imgStego = imread(stegoImg);
   Mat imgStego2 = imread(stegoImg2);
    if(!imgOrig.empty() && !imgStego.empty() && !imgStego2.empty()) {
        imshow("Imagine Originala", imgOrig);
        imshow("Imagine Stego LSB (Pare neschimbata)", imgStego);
        imshow("Imagine Stego MSB (Distrusa vizual)", imgStego2);

        showRichHistogram("Histograma Originala", imgOrig);
        showRichHistogram("Histograma Stego (LSB)", imgStego);
        showRichHistogram("Histograma Stego (MSB)", imgStego2);

        showLSBPlane("Plan LSB - Original (Zgomot natural)", imgOrig, 0);
        showLSBPlane("Plan LSB - Steganografiat (Aici e scriptul!)", imgStego, 0);
    }

    cout << "Apasă o tastă pentru a extrage și rula scriptul..." << endl;
    waitKey(0);
    extractAndRun(stegoImg);

    return 0;
}