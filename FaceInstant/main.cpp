#include <iostream>
#include <opencv2/opencv.hpp>

using namespace std;
using namespace cv;

char opcao;
String nome_arquivo = "assets\\imagens_exemplo\\microsoft.jpg";
String diretorio_base_download = "assets\\imagens_resultado\\";

cv::Mat imagem_original;

void transforma_imagem_cinza(cv::Mat imagem_para_conversao) {
	// Cria matriz para recepcao da imagem convertida em tons de cinza
	cv::Mat imagem_cinza;
	// Realiza a conversao das cores dos pixeis pra cinza
	cv::cvtColor(imagem_para_conversao, imagem_cinza, COLOR_BGR2GRAY);
	// Apresenta a imagem convertida
	cv::imshow("Tons cinza", imagem_cinza);
	// Salva em disco a imagem resultante da conversao
	cv::imwrite(diretorio_base_download + "cinza.jpg", imagem_cinza);
}

void transforma_imagem_negativa(String caminho_arquivo) {
	// Prepara matriz para receber a imagem
	cv::Mat imagem_negativa;

	// Carrega a imagem, ja em tons de cinza (necessario), na matriz criada
	imagem_negativa = imread(caminho_arquivo, IMREAD_GRAYSCALE);

	// Itera sobre as linhas da matriz
	for (int i = 0; i < imagem_negativa.rows; i++)
	{
		// Itera sobre as colunas da matriz
		for (int j = 0; j < imagem_negativa.cols; j++)
		{
			// Identifica a cor de cada pixel em cada ponto e subtrai toda a cor dele
			imagem_negativa.at<uchar>(i, j) = 255 - imagem_negativa.at<uchar>(i, j);
		}
	}

	// Mostra o resultado
	cv::imshow("Negativa", imagem_negativa);
	// Salva no disco o resultado
	cv::imwrite(diretorio_base_download + "negativa.jpg", imagem_negativa);
}

int main() {
	std::cout << "Iniciando projeto com OpenCV" << std::endl;
	// Carrega a imagem a ser usada na aplicacao
	imagem_original = cv::imread(nome_arquivo);

	if (!imagem_original.data) { // Verifica o carregamento correto da imagem
		std::cout << "Imagem nao carregada: " << nome_arquivo << endl;
		cv::waitKey(0);
		exit(1);
	}

	//while (opcao != 's')
	//{
	//	std::cin >> opcao;
	//}

	std::cout << "Selecione o que deseja realizar com a imagem" << endl;
	std::cout << "Digite \'c\' e pressione \'Enter\' para transformar a imagem em tons de cinza" << endl;
	std::cout << "Digite \'n\' e pressione \'Enter\' para transformar a imagem em sua negativa" << endl;
	std::cout << "Digite \'s\' e pressione \'Enter\' para sair do App" << endl;
	std::cout << "Pressione qualquer outra tecla e \'Enter\' para apenas mostrar a imagem aberta" << endl;

	std::cin >> opcao;

	switch (opcao)
	{
	case 'c':
		transforma_imagem_cinza(imagem_original);
		break;
	case 'n':
		transforma_imagem_negativa(nome_arquivo);
		break;
	case 's':
		break;
	default:
		cv::imshow("Original", imagem_original);
		break;
	}
	
	cv::waitKey(0);
}