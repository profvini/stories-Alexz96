# Projeto Final de Computação Gráfica e Processamento de Imagens

# FaceInstant

  Este é um App desenvolvido com base no Instagram do Meta, porém não tão robusto quanto. <br>
O App em questão utiliza principalmente a biblioteca OpenCV (na versão 4.5.3) em conjunto com a linguagem C++ e suas bibliotecas.

## Como executar

  A solução foi desenvolvida utilizando o Visual Studio Community 2019, portanto recomenda-se o uso do mesmo para compilação e execução. 
  Dito isso, seguem os passos:

- Clonar o repositório;
- Abrir o Visual Studio;
- Clicar em "Abrir um projeto ou uma solução";
- Navegar até "FaceInstant" na pasta baixada do repositório;
- Clicar duas vezes sobre o arquivo com a extensão .sln;
- Selecionar na barra superior as opções "Debug" e "x64";
- Pressionar `F5`;

Como alternativa, pode-se clicar em "Clonar um repositório" logo após abrir o Visual Studio.
Ainda, como outra alternativa, pode-se clicar à direita (em Releases) sobre o link v0.2.0 e baixar um arquivo compactado com a solução desenvolvida.

## Executando

Durante a execução do aplicativo, será aberto uma janela do console, onde conterão as opções para se utilizar e que são descritas abaixo:
- Pressione a tecla `c` e `Enter` que então uma nova janela será aberta, com a imagem importada em Tons de Cinza;
- Pressione a tecla `n` e `Enter` para converter e apresentar a imagem importada em sua negativa;
- Pressione a tecla `k` e `Enter` para realizar um processamento maior na imagem importada e então apresentar ela com efeito com base nas cores identificadas agrupadas;
- Pressione a tecla `b` e `Enter` para poder ajustar a claridade da imagem importada, em  tempo real, apresentando-a em tela;
- Pressione a tecla `t` e `Enter` para incluir um texto dinâmico na imagem, em um local determinado;
- Pressione a tecla `v` e `Enter` para iniciar uma captura de vídeo com base no primeiro dispositivo de vídeo disponível;
  - Durante a captura de vídeo, é possível capturar frames com as cores negativas, basta apenas teclar `n`;
- Pressionando `s` e `Enter` a execução do aplicativo será encerrada;

Observação: Para que a captura de frames em Negativo ocorram, o `n` deve ser pressionado estando em minúsculas;
Observação 2: Ao fim das execuções de qualquer que sejam as opções, sempre são salvas imagens no diretório de Download configurado;

## Código

Saliento que as explicações referentes ao código desenvolvido, estarão primariamente no próprio fonte "main.cpp" e na maioria dos casos, o comentário refere-se à linha logo abaixo do mesmo. Além disso, saliento que na maioria das situações, utilizou-se funções próprias do OpenCV que convertem a imagem a partir do objeto Mat e em algumas situações, foi necessário iterar sobre a matriz da imagem para que se pudesse realizar o devido processamento.

## Bibliotecas utilizadas

- OpenCV (v4.5.3);
