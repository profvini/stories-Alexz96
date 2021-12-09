#include <iostream>
#include <opencv2/opencv.hpp>
#include <cstdlib>

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

void clusteriza_imagem(String arquivo) {
	// Define a quantidade de locais de mesclagem de dados
	int numero_clusters = 8;
	// Cria a matriz de rotulos
	cv::Mat rotulos;
	// Define quantas iteracoes o algoritmo K-Means realizara
	int numero_rodadas = 5;
	// Cria a matriz com os centros
	cv::Mat centros;

	cv::Mat imagem = imread(arquivo, IMREAD_COLOR);

	// Cria a matriz com as amostras
	cv:Mat amostras(
		imagem.rows * imagem.cols,
		3, 
		CV_32F
	);

	// Itera sobre as linhas da matriz da imagem recebida por parametro
	for (int i = 0; i < imagem.rows; i++)
	{
		// Itera as sobre colunas da matriz da imagem recebida por parametro
		for (int j = 0; j < imagem.cols; j++)
		{
			// Itera em profundidade a matriz
			for (int z = 0; z < 3; z++)
			{
				// Atualiza a matriz de amostras com as informacoes obtidas da imagem tridimensional
				amostras.at<float>(i + j * imagem.rows, z) = imagem.at<Vec3b>(i, j)[z];
			}
		}
	}

	std::cout << "Executando K MEANS, aguarde..." << endl;

	// Executa o algoritmo de K-Means com os parametros definidos
	kmeans(amostras,
		numero_clusters,
		rotulos,
		TermCriteria(TermCriteria::MAX_ITER| TermCriteria::EPS, 10000, 0.0001),
		numero_rodadas,
		KMEANS_PP_CENTERS,
		centros);

	// Cria a matriz da imagem clusterizada, com mesmo tipo e tamanho da imagem de entrada
	Mat rotulada(imagem.size(), imagem.type());
	// Itera sobre a matriz da imagem de entrada
	for (int y = 0; y < imagem.rows; y++) {
		for (int x = 0; x < imagem.cols; x++) {
			int indice = rotulos.at<int>(y + x * imagem.rows, 0);
			rotulada.at<Vec3b>(y, x)[0] = (uchar)centros.at<float>(indice, 0);
			rotulada.at<Vec3b>(y, x)[1] = (uchar)centros.at<float>(indice, 1);
			rotulada.at<Vec3b>(y, x)[2] = (uchar)centros.at<float>(indice, 2);
		}
	}

	// Apresenta a imagem resultante
	imshow("Clusterizada", rotulada);
	// Salva no disco o resultado
	cv::imwrite(diretorio_base_download + "clusterizada.jpg", rotulada);

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
	std::cout << "Digite \'k\' e pressione \'Enter\' para transformar a imagem em \'salpicada\'" << endl;
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
	case 'k':
		clusteriza_imagem(nome_arquivo);
		break;
	case 's':
		break;
	default:
		cv::imshow("Original", imagem_original);
		break;
	}
	
	cv::waitKey(0);
}