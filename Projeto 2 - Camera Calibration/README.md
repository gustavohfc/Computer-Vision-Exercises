# Requisitos
Os requisitos para compilar e executar o projeto são:
1. CMake vesão 3.1 ou maior.
2. OpenCV versão 3.2.0 ou maior

# Compilar
Para compilar os programas é necessário executar os seguintes comandos:
```console
$ cmake .
$ make
``` 

# Executar
## Requisito 1
O programa não recebe argumento. Para medir uma distância na imagem simplesmente clique em dois pixels, o resultado da medida também irá aparecer sobre a imagem.

```console
$ ./Requisito1
``` 

## Requisito 2
O programa não recebe argumento.

O programa primeiro captura 25 vezes os pontos do padrão de calibração com intervalo de pelo menos 1 segundo entre as capturas. Nessa etapa é necessário movimentar o padrã na frete da webcam.

Então o programa calcula dos parâmetros de calibração e salva nos arquivos intrinsics.xml e distortion.xml.

Agora com os parâmentos de calibração são abertas duas novas saídas de vídeo "raw" e "undistorted", na "raw" contem o vídeo vindo da webcam e a "undistorted" é o vídeo processado para minimizar as distorções. Nessas janelas é possivel medir a distância entre os pixels assim como no Requisito 1.

```console
$ ./Requisito2
``` 

## Requisito 3
O programa recebe exatamente um argumento, que corresponde ao nome do arquivo onde serão salvos os parâmetos extrínsecos. O programa também utiliza os arquivos intrinsics.xml e distortion.xml gerados pelo Requisito 2.

Para calcular os extrínsecos apenas posicione o padrão de calibração na imagem e aperte qualquer tecla do teclado. O resultado estará no arquivo que você passou como arqumento para o programa.

```console
$ ./Requisito3 arquivo_de_saída
``` 

## Requisito 4

```console
$ ./Requisito4
``` 