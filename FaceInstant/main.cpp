#include <iostream>
#include <opencv2/opencv.hpp>

using namespace std;
using namespace cv;


String nome_arquivo = "assets\\imagens_exemplo\\microsoft.jpg";

cv::Mat imagem_original;

int main() {
	std::cout << "Iniciando projeto com OpenCV" << std::endl;

	imagem_original = cv::imread(nome_arquivo);

	cv::imshow("Original", imagem_original);
	cv::waitKey(0);

}