#include <opencv2/opencv.hpp>
#include <iostream>
#include <thread>
#include <mutex>
#include <cstdio>
#include <string>
#include <process.h>

using namespace cv;
using namespace std;

string getFEN(const vector<string>& labels, const int rows, const int cols) {
    string FEN = "";
    int empty_count = 0;
    for (int y = 0; y < rows; y++) {
        for (int x = 0; x < cols; x++) {
            int index = x + y * cols;
            if (labels[index] != "") {
                if (empty_count > 0) {
                    FEN += to_string(empty_count);
                    empty_count = 0;
                }
                FEN += labels[index];
            }
            else {
                empty_count++;
            }
        }
        if (empty_count > 0) {
            FEN += to_string(empty_count);
            empty_count = 0;
        }
        if (y < rows - 1) {
            FEN += "/";
        }
    }
    return FEN;
}

void detectPiece(const Mat& image_gray, const Mat& piece, const int i, const string label, vector<string>& board, vector<double>& maxVals, mutex& mtx) {
    Mat result;
    matchTemplate(image_gray, piece, result, TM_CCOEFF_NORMED);
    double minVal, maxVal;
    Point minLoc, maxLoc;
    minMaxLoc(result, &minVal, &maxVal, &minLoc, &maxLoc);

    // Check if the match is above a certain threshold
    while (maxVal > 0.97) {
        // Get the chess piece location on the board
        int x = maxLoc.x / piece.cols;
        int y = maxLoc.y / piece.rows;

        int index = x + y * 8;

        lock_guard<mutex> lock(mtx);
        if (board[index] == "") {
            board[index] = label;
            maxVals[index] = maxVal;
        }

        // set a mask on the detected area to avoid detecting the same piece multiple times
        Mat mask = Mat::zeros(result.size(), CV_8UC1);
        rectangle(mask, maxLoc, Point(maxLoc.x + piece.cols, maxLoc.y + piece.rows), Scalar(255), -1);
        result.setTo(Scalar(0), mask);
        minMaxLoc(result, &minVal, &maxVal, &minLoc, &maxLoc);
    }
}


void drawPieceRectangles(Mat& image, const vector<string>& labels, const vector<double>& maxVals, const int rows, const int cols, const Mat& piece) {
    int cellWidth = image.cols / cols;
    int cellHeight = image.rows / rows;

    for (int y = 0; y < rows; y++) {
        for (int x = 0; x < cols; x++) {
            int index = x + y * cols;
            if (labels[index] != "") {
                Point topLeft(x * cellWidth, y * cellHeight);
                Point bottomRight((x + 1) * cellWidth, (y + 1) * cellHeight);

                Scalar color;
                if (isupper(labels[index][0])) { // White pieces have uppercase labels
                    color = Scalar(0, 255, 0); // Green color
                }
                else {
                    color = Scalar(0, 0, 255); // Red color
                }

                int thickness = 2;
                rectangle(image, topLeft, bottomRight, color, thickness);

                // Display maxVal with 2 decimal places and template name
                stringstream ss;
                ss << fixed << setprecision(3) << maxVals[index] << " " << labels[index];
                string text = ss.str();
                int fontFace = FONT_HERSHEY_SIMPLEX;
                double fontScale = 0.5;
                int baseline;
                Size textSize = getTextSize(text, fontFace, fontScale, thickness, &baseline);
                Point textOrg(topLeft.x + (cellWidth - textSize.width) / 2, topLeft.y + (cellHeight + textSize.height) / 2);
                putText(image, text, textOrg, fontFace, fontScale, color, thickness);
            }
        }
    }
}


int main() {
    cout << "Choose your color: (w)hite or (b)lack? (w/b): ";
    char color;
    cin >> color;
    while (color != 'w' && color != 'b') {
        cout << "Invalid input. Please enter 'w' for white or 'b' for black: ";
        cin >> color;
    }

    // Use OpenCV to read the makrukthai board image
    Mat image, image_gray;
    image = imread("testimages/makrukthai1.png", IMREAD_COLOR);
    cvtColor(image, image_gray, COLOR_BGR2GRAY);

    // Define the makrukthai pieces templates
    Mat bs = imread("extractimages/bs.png", IMREAD_GRAYSCALE);
    Mat bk = imread("extractimages/bk.png", IMREAD_GRAYSCALE);
    Mat bn = imread("extractimages/bn.png", IMREAD_GRAYSCALE);
    Mat bp = imread("extractimages/bp.png", IMREAD_GRAYSCALE);
    Mat bm = imread("extractimages/bm.png", IMREAD_GRAYSCALE);
    Mat bm2 = imread("extractimages/bm2.png", IMREAD_GRAYSCALE);
    Mat br = imread("extractimages/br.png", IMREAD_GRAYSCALE);
    Mat ws = imread("extractimages/ws.png", IMREAD_GRAYSCALE);
    Mat wk = imread("extractimages/wk.png", IMREAD_GRAYSCALE);
    Mat wn = imread("extractimages/wn.png", IMREAD_GRAYSCALE);
    Mat wp = imread("extractimages/wp.png", IMREAD_GRAYSCALE);
    Mat wm = imread("extractimages/wm.png", IMREAD_GRAYSCALE);
    Mat wm2 = imread("extractimages/wm2.png", IMREAD_GRAYSCALE);
    Mat wr = imread("extractimages/wr.png", IMREAD_GRAYSCALE);

    vector<Mat> pieces = { bs, bk, bn, bp, bm, bm2, br, ws, wk, wn, wp, wm, wm2, wr };

    // Store the labels for each piece in a vector
    vector<string> labels = { "s", "k", "n", "p", "m", "m", "r", "S", "K", "N", "P", "M", "M", "R" };

    vector<string> board(8 * 8, "");
    vector<double> maxVals(8 * 8, 0.0);

    mutex mtx;
    vector<thread> threads;

    for (int i = 0; i < pieces.size(); i++) {
        threads.push_back(thread(detectPiece, ref(image_gray), ref(pieces[i]), i, labels[i], ref(board), ref(maxVals), ref(mtx)));
    }

    for (int i = 0; i < threads.size(); i++) {
        threads[i].join();
    }

    string FEN = getFEN(board, 8, 8);
    FEN += " ";
    FEN += color;
    cout << "FEN: " << FEN << endl;

    // Draw rectangles around detected pieces and display maxVal and template name
    drawPieceRectangles(image, board, maxVals, 8, 8, pieces[0]);

    // Show the final image with the rectangles drawn around the detected pieces
    imshow("Detected Pieces", image);
    waitKey(0);

    return 0;
}
