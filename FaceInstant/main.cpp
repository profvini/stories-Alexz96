#include <iostream>
#include <cstdlib>
#include <opencv2/opencv.hpp>
#include <opencv2/highgui.hpp>
#include <opencv2/imgproc.hpp>
#include "opencv2/imgcodecs.hpp"

// Desabilita erros de compilacao em funcao do 'sprintf'
#pragma warning(disable : 4996)

//using namespace std;
using std::cin;
using std::cout;
using std::endl;
using namespace cv;

#define DIRETORIO_FONTE = "assets\\fonte\\"

char opcao;
String nome_arquivo = "assets\\imagens_exemplo\\microsoft.jpg";
String diretorio_base_download = "assets\\imagens_resultado\\";
String diretorio_figurinhas = "assets\\figurinhas\\";

// Variaveis para captura de video
cv::VideoCapture cap;
int altura, largura;
int key;
float gauss[] = { 0.0625, 0.125,  0.0625, 0.125, 0.25,
				   0.125,  0.0625, 0.125,  0.0625 };

struct Parametros {
	cv::Mat* src;
	cv::Mat* dest;
};

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
	cv::kmeans(amostras,
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

void ajusta_claridade_imagem(int pos, void* dados) {
	Parametros* params = (Parametros*)dados;
	for (int i = 0; i < params->src->rows; i++) {
		for (int j = 0; j < params->src->cols; j++) {
			Vec3b pixelColor;
			pixelColor = params->src->at<Vec3b>(Point(j, i));
			for (int k = 0; k < 3; k++) {
				if (pixelColor[k] + pos > 255)
					pixelColor[k] = 255;
				else
					pixelColor[k] += pos;
				params->dest->at<Vec3b>(Point(j, i)) = pixelColor;
			}
		}
	}
	imshow("Imagem Clara", *(params->dest));
}

void insere_marca_tempo(cv::Mat imagem_colocar_texto) {
	// Define o texto para mostrar na imagem
	String texto_para_incluir = "";

	std::cout << "Digite o texto que deseja inserir na imagem: " << std::endl;
	std::cin >> texto_para_incluir;
	std::getline(std::cin, texto_para_incluir);

	// Apresenta o tamanho da imagem
	std:cout << "Tamanho da imagem: " << imagem_colocar_texto.size << std::endl;

	// Calcula a posicao do texto na imagem recebida - fica na parte superior
	int um_nono_altura_imagem = imagem_colocar_texto.cols / 10;
	int um_nono_largura_imagem = imagem_colocar_texto.rows / 10;
	
	// Inclui o texto na imagem com as configuracoes predeterminadas
	cv::putText(
		imagem_colocar_texto,
		texto_para_incluir,
		cv::Point(um_nono_largura_imagem, um_nono_altura_imagem),
		FONT_HERSHEY_SIMPLEX,
		1.0,
		Scalar(250, 250, 250)
	);

	// Apresenta a imagem com o texto inserido
	cv::imshow("Imagem com texto", imagem_colocar_texto);
	// Salva a imagem com o texto inserido em disco
	cv::imwrite(diretorio_base_download + "imagem_com_texto.jpg", imagem_colocar_texto);
}

void insere_figurinha_imagem(cv::Mat imagem_incluir_figurinha) {
	String nome_figurinha = "pizza.png"; // Opcoes - pizza, jake ou come - todas .png
	int posicao_x = 5;
	int posicao_y = 5;

	cv::Mat figurinha = cv::imread(diretorio_figurinhas + nome_figurinha);

	figurinha.copyTo(imagem_incluir_figurinha(cv::Rect(posicao_x,posicao_y, figurinha.cols, figurinha.rows)));

	cv::imshow("Imagem com figurinha", imagem_incluir_figurinha);

	cv::imwrite(diretorio_base_download + "figurinha.jpg", imagem_incluir_figurinha);
}

int abre_webcam() {
	cv::Mat frames;

	cv::Mat frame, framegray, frame32f, frameFiltered;
	cv::Mat mask(3, 3, CV_32F);
	cv::Mat result;

	// habilita o negativo da imagem
	bool negative = false;

	cap.open(0);

	// Valida se foi localizada uma câmera
	if (!cap.isOpened()) {
		std::cout << "cameras indisponiveis";
		exit(-1);
	}

	// Define a largura e altura da tela, respectivamente
	cap.set(cv::CAP_PROP_FRAME_WIDTH, 640);
	cap.set(cv::CAP_PROP_FRAME_HEIGHT, 480);

	// 
	cv::namedWindow("Video", cv::WINDOW_NORMAL);
	
	// Enquanto roda a funcao...
	while (1) {
		// fica salvando cada frame para a matriz de frame
		cap >> frames;

		// Apresenta video capturado
		cv::imshow("Video", frames);
		
		// Monitora digitacao
		key = cv::waitKey(30);
		if (key == 27) break;  // esc pressed!
		switch (key) {
			case 'n':
				negative = true;
				bitwise_not(frames, frameFiltered);
				cv::imshow("Video Negativo", frameFiltered);
				cv::imwrite(diretorio_base_download + "v_negativo.jpg", frameFiltered);
				break;
			default:
				break;
		}
	}

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

	// Cria a matriz que recebera a imagem ajustada
	cv::Mat imagem_ajustada;

	// Valor inicial para a claridade
	int valor_claridade = 50;
	// Valor maximo do pixel com o aumento da claridade
	int claridade_maxima = 255;

	// Inicializa a struct
	Parametros p;
	p.src = &imagem_original;
	p.dest = &imagem_ajustada;

	std::cout << "Selecione o que deseja realizar com a imagem" << endl;
	std::cout << "Digite \'c\' e pressione \'Enter\' para transformar a imagem em tons de cinza" << endl;
	std::cout << "Digite \'n\' e pressione \'Enter\' para transformar a imagem em sua negativa" << endl;
	std::cout << "Digite \'k\' e pressione \'Enter\' para transformar a imagem com base nas cores similares identificadas" << endl;
	std::cout << "Digite \'b\' e pressione \'Enter\' para ajustar a claridade da imagem" << endl;
	std::cout << "Digite \'t\' e pressione \'Enter\' para incluir texto na imagem" << endl;
	std::cout << "Digite \'f\' e pressione \'Enter\' para inserir uma figurinha na imagem" << endl;
	std::cout << "Digite \'v\' e pressione \'Enter\' para capturar um video" << endl;
	std::cout << "Digite \'s\' e pressione \'Enter\' para sair do App" << endl;
	std::cout << "Pressione qualquer outra tecla e \'Enter\' para apenas mostrar a imagem aberta" << endl;

	std::cin >> opcao;
	
	switch (tolower(opcao))
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
	case 'b':
		// Copia imagem base para a imagem a ser ajustada
		imagem_original.copyTo(imagem_ajustada);
		// Cria uma janela para inserir a TrackBar
		cv::namedWindow("Clarificada");

		// Cria a Trackbar
		cv::createTrackbar("Claridade", "Clarificada", &valor_claridade, claridade_maxima, ajusta_claridade_imagem, &p);
		ajusta_claridade_imagem(valor_claridade, &p);

		// Salva em disco a imagem ajustada, no diretorio configurado
		cv::imwrite(diretorio_base_download + "clarificada.jpg", imagem_ajustada);
		break;
	case 't':
		insere_marca_tempo(imagem_original);
		break;
	case 'f':
		insere_figurinha_imagem(imagem_original);
		break;
	case 'v':
		abre_webcam();
		break;
	case 's':
		break;
	default:
		cv::imshow("Original", imagem_original);
		break;
	}
	
	cv::waitKey(0);
}