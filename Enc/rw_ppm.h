#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <string.h>
// Funções que são utilizadas somente em imagens ppm

#ifndef RW_PPM_H
#define RW_PPM_H

// struct para o arquivo ppm

typedef struct{
	unsigned int ppm_rgb_blue;
	unsigned int ppm_rgb_green;
	unsigned int ppm_rgb_red;
} ppm_rgbquad; // contém o mapa de cores da imagem

typedef struct{
	char ppm_type[2]; // identificador do tipo de arquivo, deve conter P6
	unsigned int ppm_width; // largura da imagem
	unsigned int ppm_height; // altura da imagem
    unsigned int ppm_comps_max; // valor máximo das componentes de cor
	ppm_rgbquad* ppm_colors; 
} ppm_file; 

// fim da struct para o arquivo ppm


void ler_ppm(FILE *img_ppm, ppm_file* ppm_dados);

void salvar_ppm(FILE *img_ppm, ppm_file* ppm_dados);

void codificador_ppm(char texto[], ppm_rgbquad* ppm_colors);

void decodificar_ppm(char* texto, char* texto_saida, ppm_rgbquad* ppm_colors, FILE *arq_text_salvar, int salva_noarquivo);


#endif
