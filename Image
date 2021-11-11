#include <iostream>
#include <vector>

using namespace std;

class Image {
private:
	vector <vector <vector <int> > > img;

public:
	Image();
	Image(int* raw, int width, int height, int depth) {
		img.resize(depth);
		for (int i = 0; i < img.size(); ++i) {
			img[i].resize(width);
			for (int j = 0; j < img[0].size(); ++j)
				img[i][j].resize(height);
		}
		for (int i = 0; i < height; ++i)
			for (int j = 0; j < width; ++j)
				for (int k = 0; k < depth; ++k)
					img[k][j][i] = raw[k + j * depth + i * depth * width];
	}

	Image operator=(const Image img2) {
		img.resize(img2.img.size());
		for (int i = 0; i < img.size(); ++i) {
			img[i].resize(img2.img[0].size());
			for (int j = 0; j < img[0].size(); ++j)
				img[i][j].resize(img2.img[0][0].size());
		}
		for (int i = 0; i < img2.img[0][0].size(); ++i)
			for (int j = 0; j < img2.img[0].size(); ++j)
				for (int k = 0; k < img2.img.size(); ++k)
					img[k][j][i] = img2.img[k][j][i];
	}

	Image(int width, int height, int depth) {
		img.resize(depth);
		for (int i = 0; i < img.size(); ++i) {
			img[i].resize(width);
			for (int j = 0; j < img[0].size(); ++j)
				img[i][j].resize(height);
		}
		for (int i = 0; i < height; ++i)
			for (int j = 0; j < width; ++j)
				for (int k = 0; k < depth; ++k)
					img[k][j][i] = 0;
	}

	Image(Image const& other) {
		img = other.img;
	}

	Image crop(int xi, int gi, int xt, int gt) {
		Image img2(img.size(), xt - xi, gt - gi);
		for (int i = 0; i < img.size(); ++i)
			for (int j = xi; j < xt; ++j)
				for (int k = gi; k < gt; ++k)
					img2.img[i][j - xi][k - gi] = img[i][j][k];
		return img2;
	}

	Image to_gs() {
		Image img2(img[0].size(), img[0][0].size(), 1);
		for (int i = 0; i < img[0].size(); ++i)
			for (int j = 0; j < img[0][0].size(); ++j) {
				int sum = 0;
				for (int k = 0; k < img.size(); ++k)
					sum += img[k][i][j];
				img2.img[0][i][j] = sum / img.size();
			}
		return img2;
	}

	void clear() {
		img.resize(0);
	}

	const int width() {
		return img[0].size();
	}

	const int height() {
		return img[0][0].size();
	}

	const int depth() {
		return img.size();
	}

	const void print() {
		for (int i = 0; i < img[0][0].size(); ++i) {
			for (int j = 0; j < img[0].size(); ++j) {
				cout << '(';
				for (int k = 0; k < img.size() - 1; ++k)
					cout << img[k][j][i] << ", ";
				cout << img[img.size() - 1][j][i] << ") ";
			}
			cout << '\n';
		}
	}

	const int at(int x, int y, int d) {
		return img.at(x).at(y).at(d);
	}
};

int main() {
	int r[24] = { 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15, 16, 17, 18, 19, 20, 21, 22, 23, 24 };
	Image img(r, 2, 4, 3);
	img.print();
	Image img2(img.to_gs());
	img2.print();
	return 0;
}
