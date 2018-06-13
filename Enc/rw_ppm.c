#include "rw_ppm.h"
#include "rw_all.h"

// Funções que são utilizadas somente em imagens ppm

void ler_ppm(FILE *img_ppm, ppm_file* ppm_dados){ // função que faz a leitura da imagem ppm e salva os dados na struct
	char tipo_arq[2];
	unsigned int ch;
	unsigned int pos_pixel=0;
	unsigned int pos_rgb=0;
	unsigned int binario;
	unsigned long int total_pixels;
	
	fgets(tipo_arq, 3, img_ppm);
	
	if((strcmp(tipo_arq, "P6")) == 0 ){		// verificando se a imagem realmente é do tipo ppm
		ppm_dados->ppm_type[0] = 'P';
		ppm_dados->ppm_type[1] = '6';
	}else{
		printf("Erro, a imagem não tem o formato ppm.\n");
		exit(0);
	}
	
	fscanf(img_ppm, "%u %u", &ppm_dados->ppm_width, &ppm_dados->ppm_height); // lendo altura e largura
	fscanf(img_ppm, "%u", &ppm_dados->ppm_comps_max); //máximo de cores por pixel

 	total_pixels = (ppm_dados->ppm_width * ppm_dados->ppm_height); // calculando total necessário para guardar os pixels
 	quantidade_pixels_ppm = total_pixels;

	ppm_dados->ppm_colors = (ppm_rgbquad*)malloc(total_pixels*sizeof(ppm_rgbquad)); // alocando memória para os pixels
	if (ppm_dados->ppm_colors == NULL) { // verifica se a alocação foi possível
		printf ("Erro: Memoria Insuficiente.");
		exit(0);
	}
	
	ch = fgetc(img_ppm);
	while((ch = fgetc(img_ppm)) != EOF){ // enquanto houver caracteres
			binario = int_to_bin(ch); // transforma o char em binário
			if(pos_rgb==0){
 				ppm_dados->ppm_colors[pos_pixel].ppm_rgb_blue = binario;	
			}
			if(pos_rgb==1){
				ppm_dados->ppm_colors[pos_pixel].ppm_rgb_green = binario;	
			}
			if(pos_rgb==2){
				ppm_dados->ppm_colors[pos_pixel].ppm_rgb_red = binario;	
				pos_rgb = -1;
				pos_pixel = pos_pixel+1;
			}
			pos_rgb = pos_rgb+1;
	}
}


void salvar_ppm(FILE *new_img_ppm, ppm_file* ppm_dados){
	// salva os dados da struct no arquivo passado por parâmetro
	int i;

	fprintf(new_img_ppm, "%c%c\n", ppm_dados->ppm_type[0], ppm_dados->ppm_type[1]);
	fprintf(new_img_ppm, "%i %i\n", ppm_dados->ppm_width, ppm_dados->ppm_height);
	fprintf(new_img_ppm, "%i\n", ppm_dados->ppm_comps_max);

	for (i = 0; i < quantidade_pixels_ppm; ++i){
		fprintf(new_img_ppm, "%c", binary_to_int(ppm_dados->ppm_colors[i].ppm_rgb_blue)); // transforma o binário em decimal
		fprintf(new_img_ppm, "%c", binary_to_int(ppm_dados->ppm_colors[i].ppm_rgb_green));
		fprintf(new_img_ppm, "%c", binary_to_int(ppm_dados->ppm_colors[i].ppm_rgb_red));	
	}

}

void codificador_ppm(char texto[], ppm_rgbquad* ppm_colors){ // insere o texto na imagem ppm
	// função que percorre os bits de cada char do texto de entrada, pega cada bit e seta em cada byte de cada cor.
	int total_bits_texto = (strlen(texto)+3)*8; // total de bits do texto, o +3 é devido ao padrão adicionado 'EOF'
	int i,j,h;
	int binario_int=0;
	char binario[8];
	int pos_pixel=0;
	int pos_rgb=0;

	if((total_bits_texto/3) > quantidade_pixels_ppm ){ // verfica a quantidade necessária de pixels
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

			if(pos_rgb == 0){ // chama a função set_bits, que recebe o bit e byte do rgb, insere o bit no primeiro bit do byte 
				ppm_colors[pos_pixel].ppm_rgb_blue = set_bits(binario[j], ppm_colors[pos_pixel].ppm_rgb_blue);
				pos_rgb=1;
			}else{	
				if(pos_rgb == 1){
					ppm_colors[pos_pixel].ppm_rgb_green = set_bits(binario[j], ppm_colors[pos_pixel].ppm_rgb_green);
					pos_rgb=2;
				}else{
					if(pos_rgb == 2){
						ppm_colors[pos_pixel].ppm_rgb_red = set_bits(binario[j], ppm_colors[pos_pixel].ppm_rgb_red);
						pos_rgb=0;
						pos_pixel = pos_pixel+1;
					}
				}
			}
		}
	}
}

void decodificar_ppm(char* binario_entrada, char* texto_saida, ppm_rgbquad* ppm_colors, FILE *arq_text_salvar, int salva_noarquivo){ // decodifica a imagem
	//função que pega o bit menos significativo de cada cor, adiciona em um array e depois chama outra função para converter o binário em decimal
	int i,h=0;
	int total_chars_text=0;
	int primeiro_bit=0, segundo_bit=0, terceiro_bit=0;

	
	for (i = 0; i < quantidade_pixels_ppm; ++i){ 
		primeiro_bit = ppm_colors[i].ppm_rgb_blue; // seta cada variável com cada cor
		segundo_bit = ppm_colors[i].ppm_rgb_green;
		terceiro_bit = ppm_colors[i].ppm_rgb_red;
			
		primeiro_bit = primeiro_bit%10;// pega o bit menos significativo de cada cor
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
