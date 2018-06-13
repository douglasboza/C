Foram feitas as funcionalidades de codificação e decodifição de textos em imagens bmp e ppm. 


Podia ser removido o trecho de código repetido nas funções de codificação e decodificação das imagens bmp e ppm, pois o que muda nelas é apenas o formato da struct. Por isso, poderia ter sido feita uma função genérica para codificação e decodificação.

Na parte da decodificação, primeiro é lido o bit menos significativo de cada byte. Ao final, esses bits são convertidos para decimal, até chegar a string EOF, que foi um padrão que adotei para identificar onde acaba a mensagem.
Porém, seria melhor eu ter convertido cada grupo de 8 bits a medida que eu fosse lendo eles, até chegar em EOF. Dessa forma, só seriam lidos os bits que fazem parte da mensagem, tornando a execução mais rápida.

gcc -Wall -ansi -pedantic -std=c99 -o steg main.c rw_all.c rw_bmp.c rw_ppm.c -lm

