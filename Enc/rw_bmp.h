#include <stdio.h>
#include <stdlib.h>
#include <string.h>
// Header - Funções que são utilizadas somente em imagens bmp

#ifndef RW_BMP_H
#define RW_BMP_H

//------------------ struct para o arquivo bmp

typedef struct{
	unsigned short bmp_type; //Contém o decimal 19778 (4D42 hexa).
	unsigned int bmp_size; //Comprimento total do ficheiro em bytes.
	unsigned int bmp_res1;
	unsigned int bmp_res2;
	unsigned int bmp_bytesoff; //Número de bytes que, no ficheiro, precedem o primeiro byte correspondente ao conteúdo da imagem
} bmp_file_header; // cabeçalho do arquivo bmp, descreve o ficheiro bmp

typedef struct{
	unsigned int bmp_size_infh; //Espaço, em bytes, ocupado por esta estrutura de dados, ou seja, 40 bytes
	unsigned int bmp_width; //Largura da imagem, em píxeis.
	unsigned int bmp_height; // Altura da imagem, em píxeis.
	unsigned short bmp_planes; // deve ter o valor 1, obrigatoriamente.
	unsigned short bmp_bits_count; // Número de bits por píxel que, obrigatoriamente, deve ser 1, 4, 8 ou 24
	unsigned int bmp_compression; // Tipo  de  compressão  0, 1 ou 2
	unsigned int bmp_size_image; // Tamanho, em bytes, ocupado pelos dados da imagem. 
	unsigned int bmp_pix_mx; // Resolução horizontal e resolução vertical, em píxeis por metro, da imagem no dispositivo gráfico de saída
	unsigned int bmp_pix_my; 
	unsigned int bmp_cl_used; //Número de cores do mapa de cores referenciadas pelos dados da imagem
	unsigned int bmp_cl_important; //Número de cores do mapa de cores que são consideradas importantes
} bmp_info_header; // descreve a imagem contida no ficheiro

typedef struct{
	unsigned int bmp_rgb_blue;
	unsigned int bmp_rgb_green;
	unsigned int bmp_rgb_red;
	unsigned int bmp_rgb_reserved;
} bmp_rgbquad;// contém o mapa de cores da imagem se esta não empregar 24 bits por píxel.

typedef struct{
	bmp_info_header bmp_header; // sub bloco 1
	bmp_rgbquad* bmp_colors;   // sub bloco 2
} bmp_info; // descreve o bitmap da imagem

//------------------ fim da struct para o arquivo bmp


void ler_bmp(FILE* img_bmp, bmp_file_header* bmp_fh, bmp_info* bmp_if);

void salvar_bmp(FILE* new_img_bmp, bmp_file_header* bmp_fh, bmp_info* bmp_if);

void codificador_bmp(char texto[], bmp_rgbquad* bmp_colors);

void decodificar_bmp(char* texto, char* texto_saida_bmp, bmp_rgbquad* bmp_colors, FILE *arq_text_salvar, int salva_noarquivo);


#endif

