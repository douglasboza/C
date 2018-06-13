#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "rw_all.h"
#include "rw_bmp.h"

// Funções que são utilizadas somente em imagens bmp

void ler_bmp(FILE* img_bmp, bmp_file_header* bmp_fh, bmp_info* bmp_if){
	unsigned int ch;
	unsigned int binario;
	int qtd_pixels=0;
	int pos_rgb = 0;
	unsigned int pos_pixel = 0;

    fread(&bmp_fh->bmp_type, 2, 1, img_bmp);//Lendo tipo da imagem
	
	if(bmp_fh->bmp_type != 19778){
		printf("Erro, a imagem não tem o formato bmp.\n");
		exit(0);
	}
	//Lendo cabeçalho da imagem
    fread(&bmp_fh->bmp_size, 4, 1, img_bmp);
    fread(&bmp_fh->bmp_res1, 2, 1, img_bmp);
    fread(&bmp_fh->bmp_res2, 2, 1, img_bmp);
	fread(&bmp_fh->bmp_bytesoff, 4, 1, img_bmp);

	fread(&bmp_if->bmp_header.bmp_size_infh, 4, 1, img_bmp);
	fread(&bmp_if->bmp_header.bmp_width, 4, 1, img_bmp);
	fread(&bmp_if->bmp_header.bmp_height, 4, 1, img_bmp);
	fread(&bmp_if->bmp_header.bmp_planes, 2, 1, img_bmp);
	fread(&bmp_if->bmp_header.bmp_bits_count, 2, 1, img_bmp);
	fread(&bmp_if->bmp_header.bmp_compression, 4, 1, img_bmp);
	fread(&bmp_if->bmp_header.bmp_size_image, 4, 1, img_bmp);
	fread(&bmp_if->bmp_header.bmp_pix_mx, 4, 1, img_bmp);
	fread(&bmp_if->bmp_header.bmp_pix_my, 4, 1, img_bmp);
	fread(&bmp_if->bmp_header.bmp_cl_used, 4, 1, img_bmp);
	fread(&bmp_if->bmp_header.bmp_cl_important, 4, 1, img_bmp);
	
	qtd_pixels = (bmp_if->bmp_header.bmp_width) * (bmp_if->bmp_header.bmp_height); // quantiade de pixels
	bmp_if->bmp_colors = (bmp_rgbquad*)malloc(qtd_pixels*sizeof(bmp_rgbquad)); // alocando memória para os pixels
	
	if (bmp_if->bmp_colors == NULL) { // verifica se a alocação foi possível
		printf ("Erro: Memoria Insuficiente.");
		exit(0);
	}
	
	while((ch = fgetc(img_bmp)) != EOF){

		// lendo cada caractere
			binario = int_to_bin(ch); // transforma o char em binário

			if(pos_rgb==0){ // preenche cada cor do array de pixels
 				bmp_if->bmp_colors[pos_pixel].bmp_rgb_blue = binario;	
			}
			if(pos_rgb==1){
				bmp_if->bmp_colors[pos_pixel].bmp_rgb_green = binario;	
			}
			if(pos_rgb==2){
				bmp_if->bmp_colors[pos_pixel].bmp_rgb_red = binario;	
				bmp_if->bmp_colors[pos_pixel].bmp_rgb_reserved = 0;
				pos_rgb = -1;
				pos_pixel = pos_pixel+1;
			}
			pos_rgb = pos_rgb+1;
	}

	quantidade_pixels_bmp = qtd_pixels; // seta a variável global quantidade_pixels_bmp
	
}

void salvar_bmp(FILE* new_img_bmp, bmp_file_header* bmp_fh, bmp_info* bmp_if){ // salva a struct na imagem
	int i;
	//escrevendo o cabeçalho da imagem
	fwrite(&bmp_fh->bmp_type, 2, 1, new_img_bmp);
    fwrite(&bmp_fh->bmp_size, 4, 1, new_img_bmp);
    fwrite(&bmp_fh->bmp_res1, 2, 1, new_img_bmp);
    fwrite(&bmp_fh->bmp_res2, 2, 1, new_img_bmp);
	fwrite(&bmp_fh->bmp_bytesoff, 4, 1, new_img_bmp);

	fwrite(&bmp_if->bmp_header.bmp_size_infh, 4, 1, new_img_bmp);
	fwrite(&bmp_if->bmp_header.bmp_width, 4, 1, new_img_bmp);
	fwrite(&bmp_if->bmp_header.bmp_height, 4, 1, new_img_bmp);
	fwrite(&bmp_if->bmp_header.bmp_planes, 2, 1, new_img_bmp);
	fwrite(&bmp_if->bmp_header.bmp_bits_count, 2, 1, new_img_bmp);
	fwrite(&bmp_if->bmp_header.bmp_compression, 4, 1, new_img_bmp);
	fwrite(&bmp_if->bmp_header.bmp_size_image, 4, 1, new_img_bmp);
	fwrite(&bmp_if->bmp_header.bmp_pix_mx, 4, 1, new_img_bmp);
	fwrite(&bmp_if->bmp_header.bmp_pix_my, 4, 1, new_img_bmp);
	fwrite(&bmp_if->bmp_header.bmp_cl_used, 4, 1, new_img_bmp);
	fwrite(&bmp_if->bmp_header.bmp_cl_important, 4, 1, new_img_bmp);

	for (i = 0; i < quantidade_pixels_bmp; ++i){
		fprintf(new_img_bmp, "%c", binary_to_int(bmp_if->bmp_colors[i].bmp_rgb_blue)); // transforma o binário em decimal e escreve no arquivo
		fprintf(new_img_bmp, "%c", binary_to_int(bmp_if->bmp_colors[i].bmp_rgb_green));
		fprintf(new_img_bmp, "%c", binary_to_int(bmp_if->bmp_colors[i].bmp_rgb_red));	
	}
}

void codificador_bmp(char texto[], bmp_rgbquad* bmp_colors){ // insere o texto na struct da imagem
	// função que percorre os bits de cada char do texto de entrada, pega cada bit e seta em cada byte de cada cor.
	int total_bits_texto = (strlen(texto)+3)*8; // total de bits do texto, o +3 é devido ao padrão adicionado 'EOF'
	int i,j,h;
	int binario_int=0; // guarda cada caractere em formato binário
	char binario[8]; // array com os 8 bits de cada caractere a ser salvo
	int pos_pixel=0;
	int pos_rgb=0; // define em qua cor será setado o bit

	if((total_bits_texto/3) > quantidade_pixels_bmp ){ // verfica a quantidade necessária de pixels
		printf("Não há pixels suficientes na imagem.\n");
		exit(0);
	}
	
	pos_rgb=0;
	for (i = 0; i < strlen(texto); ++i){ //percorre cada caractere, que tem 1 byte, do arquivo
		binario_int = int_to_bin(texto[i]); // transforma o char em binário;
		for (h = 7; h >= 0; --h){
			if(binario_int%10 == 0){
				binario[h] = '0';
			}else{
				binario[h] = '1';
			}
			binario_int=binario_int/10;
		}

		for (j = 0; j < 8; j++){ // percorre os 8 bits, colocando cada um em um elemento do pixel
			if(pos_rgb == 0){ // chama a função set_bits, que recebe o bit e byte do rgb, insere o bit no byte 
				bmp_colors[pos_pixel].bmp_rgb_blue = set_bits(binario[j], bmp_colors[pos_pixel].bmp_rgb_blue);
				pos_rgb=1;
			}else{	
				if(pos_rgb == 1){
					bmp_colors[pos_pixel].bmp_rgb_green = set_bits(binario[j], bmp_colors[pos_pixel].bmp_rgb_green);
					pos_rgb=2;
				}else{
					if(pos_rgb == 2){
						bmp_colors[pos_pixel].bmp_rgb_red = set_bits(binario[j], bmp_colors[pos_pixel].bmp_rgb_red);
						pos_rgb=0;
						pos_pixel = pos_pixel+1;
					}
				}
			}
		}
	}

}


void decodificar_bmp(char* binario_entrada, char* texto_saida, bmp_rgbquad* bmp_colors, FILE *arq_text_salvar, int salva_noarquivo){
	//função que pega o bit menos significativo de cada cor, adiciona em um array e depois chama outra função para converter o binário em decimal
	int i,h=0;
	int total_chars_text = 0;
	int primeiro_bit=0, segundo_bit=0, terceiro_bit=0;
	
	for (i = 0; i < quantidade_pixels_bmp; ++i){
		primeiro_bit = bmp_colors[i].bmp_rgb_blue; // seta cada variável com cada cor
		segundo_bit = bmp_colors[i].bmp_rgb_green;
		terceiro_bit = bmp_colors[i].bmp_rgb_red;
		
		primeiro_bit = primeiro_bit%10; // pega o bit menos significativo de cada cor
		segundo_bit = segundo_bit%10;
		terceiro_bit = terceiro_bit%10;	
		
		if(primeiro_bit == 1){ binario_entrada[h] = '1'; }else{ binario_entrada[h] = '0'; } // salva  bit menos significativo no array
		h = h+1;
		if(segundo_bit == 1){ binario_entrada[h] = '1'; }else{ binario_entrada[h] = '0'; }
		h = h+1;
		if(terceiro_bit == 1){ binario_entrada[h] = '1'; }else{ binario_entrada[h] = '0'; }
		h = h+1;
		
	}

	total_chars_text = binary_to_text(texto_saida, binario_entrada); // chama a função que recebe o array com os bits e retorna ele em char

	if(salva_noarquivo == 1){
		for (i = 0; i < total_chars_text; ++i){ // escreve o texto encontrado no arquivo
			fprintf(arq_text_salvar, "%c", texto_saida[i]);	
		}
	}else{
		printf("Mensagem encontrada:\n");
		for (i = 0; i < total_chars_text; ++i){ // print do texto encontrado no arquivo
			printf("%c", texto_saida[i]);
		}
		printf("\n");
	}

}
