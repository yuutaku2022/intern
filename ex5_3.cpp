#include <opencv2/opencv.hpp>
#include <iostream>
#include <vector>
#include <algorithm>
#include <random>

using namespace cv;
using namespace std;

// 画像を32x32のブロックに分割してランダムに入れ替える関数
void ex5_3(const string& inputImagePath, const string& outputImagePath) {
	// 画像を読み込む
	Mat img = imread(inputImagePath);
	if (img.empty()) {
    	cerr << "Error: Cannot load image!" << endl;
    	return;
	}

	// 画像をリサイズ（必要に応じて）
	resize(img, img, Size(336, 252));

	// 画像のサイズを取得
	int width = img.size().width;
	int height = img.size().height;
	int blockSize = 32;

	// ブロックの数を計算
	int blocksX = (width + blockSize - 1) / blockSize;
	int blocksY = (height + blockSize - 1) / blockSize;

	// 画像をブロックに分割し、ブロックの位置を保存
	vector<Mat> blocks;
	for (int by = 0; by < blocksY; ++by) {
    	for (int bx = 0; bx < blocksX; ++bx) {
        	int startX = bx * blockSize;
        	int startY = by * blockSize;
        	Rect blockRect(startX, startY, blockSize, blockSize);
        	Mat block;

        	// 画像の境界を考慮してブロックを切り取る
        	if (startX + blockSize <= width && startY + blockSize <= height) {
            	block = img(blockRect);
        	} else {
            	// 境界を越える場合は黒い部分でパディング
            	block = Mat::zeros(blockSize, blockSize, img.type());
            	for (int y = 0; y < blockSize && startY + y < height; ++y) {
                	for (int x = 0; x < blockSize && startX + x < width; ++x) {
                    	block.at<Vec3b>(y, x) = img.at<Vec3b>(startY + y, startX + x);
                	}
            	}
        	}
        	blocks.push_back(block);
    	}
	}

	// ブロックのインデックスをシャッフル
	vector<int> indices(blocks.size());
	iota(indices.begin(), indices.end(), 0);
	random_device rd;
	mt19937 g(rd());
	shuffle(indices.begin(), indices.end(), g);

	// ランダムに並べ替えたブロックを新しい画像に配置
	Mat shuffled_img(height, width, img.type());
	for (int by = 0; by < blocksY; ++by) {
    	for (int bx = 0; bx < blocksX; ++bx) {
        	int idx = indices[by * blocksX + bx];
        	Mat block = blocks[idx];
        	int startX = bx * blockSize;
        	int startY = by * blockSize;
        	for (int y = 0; y < blockSize && startY + y < height; ++y) {
            	for (int x = 0; x < blockSize && startX + x < width; ++x) {
                	shuffled_img.at<Vec3b>(startY + y, startX + x) = block.at<Vec3b>(y, x);
            	}
        	}
    	}
	}

	// 結果を保存
	imwrite(outputImagePath, shuffled_img);

	// 結果を表示
	imshow("ex5_3.png", shuffled_img);
	waitKey(0);  // キーが押されるまで待つ
}

int main() {
	string inputImagePath = "myphoto.jpg"; // 入力画像のパス
	string outputImagePath = "ex5-3.jpg";  // 出力画像のパス
	ex5_3(inputImagePath, outputImagePath);
	cout << "Image processing complete. Result saved as " << outputImagePath << endl;
	return 0;
}





