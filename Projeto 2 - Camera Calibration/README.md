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

# Requisito 1
```console
$ ./Requisito1
```

O programa não recebe argumento. Para medir uma distância na imagem simplesmente clique em dois pixels, o resultado da medida irá aparecer sobre a imagem.



# Requisito 2
```console
$ ./Requisito2
```

O programa não recebe argumento.

Para realizar a calibração o programa primeiro captura 25 vezes imagens diferentes do padrão de calibração com intervalo de pelo menos 1 segundo entre as capturas, as capturas e o intervalo são automaticos sendo necessário apenas que o padrão de calibração estaja visivel pela webcam.

Então o programa calcula dos parâmetros de calibração e os salva nos arquivos **intrinsics.xml** e **distortion.xml**, esses arquivos serão utilizados pelos requisitos 3 e 4.

E então com os parâmentos de calibração calculados, são abertas duas novas saídas de vídeo "raw" e "undistorted". A saída "raw" contem o vídeo original da webcam e a "undistorted" é o vídeo processado para minimizar as distorções. Nessas janelas também é possivel medir a distância entre os pixels como no Requisito 1.

Alguns resultados obtidos nessa etapa estão diponíveis no diretório Requisito2_resultados/



# Requisito 3
```console
$ ./Requisito3 arquivo_de_saída.xml
```

O programa recebe exatamente um argumento, que corresponde ao nome do arquivo onde serão salvos os parâmetos extrínsecos. O programa também utiliza os arquivos **intrinsics.xml** e **distortion.xml** gerados pelo Requisito 2 que devem estar disponíveis no mesmo diretório do executável.

Para calcular os extrínsecos apenas posicione o padrão de calibração na imagem e aperte qualquer tecla do teclado para capturar a imagem, se o padrão de calibração for identificado na imagem então os extrínsecos são calculados e salvos no arquivo indicado pelo primeiro arquimento do programa.

Alguns resultados obtidos nessa etapa estão diponíveis no diretório Requisito3_resultados/



# Requisito 4
```console
$ ./Requisito4
```

O programa recebe exatamente um argumento, que corresponde ao nome do arquivo onde com os parâmetos extrínsecos que serão utilizados, gerado pelo Requisito 3. O programa também utiliza os arquivos **intrinsics.xml** e **distortion.xml** gerados pelo Requisito 2 que devem estar disponíveis no mesmo diretório do executável.

São mostradas duas saídas de vídeo "raw" e "undistorted", em ambas é possivel realizar medições simplesmente clicando em pontos da imagem. 