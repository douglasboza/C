#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <getopt.h>
#include "rw_all.h" // funções para ambos os tipos
#include "rw_bmp.h" // funções exclusivas para o tipo bmp
#include "rw_ppm.h" // funções exclusivas para o tipo ppm

int main(int argc, char* const argv[]){
	int c;
	int encoder = 0;
	int decoder = 0;
	int salva_noarquivo = 1;
	char* input_text_file = "\0";
	char* formato_img = "\0";
	char* img_input = "\0";
	char* output_text_file = "\0";
	opterr = 0;
	bmp_file_header bmp_fh;	
	bmp_info bmp_if; 
	ppm_file ppm_dados;
	FILE *img_bmp;
	FILE *img_ppm;
	FILE *texto_enc;
	FILE *arq_text_salvar;
	char* texto_encriptar;
	int cont_pos_text=0;
	int total_pos_text=0;

	 while ((c = getopt (argc, argv, "esdo:i:f:")) != -1)
	    switch (c){
	    case 's':
	      	salva_noarquivo = 0;
	        break;
	      case 'e':
	      	encoder = 1;
	        break;
	      case 'd':
	        decoder = 1;
	        break;
	      case 'i':
	      	input_text_file = optarg;
	      	if((strcmp(input_text_file, "\0")) == 0){
				printf("O arquivo com o texto para encritar não foi encontrado.\n");
				return 0;
			}
	        break;
	      case 'o':
	      	output_text_file = optarg;
	      	if((strcmp(output_text_file, "\0")) == 0){
				printf("Não foi encontrado o arquivo em que será escrita a mensagem.\n");
				return 0;
			}
	        break;
	      case 'f':
	      	formato_img = optarg;
	   		if(argc >= optind+1 ){
	   			img_input = argv[optind++];
	      	}
	      	if((strcmp(img_input, "\0")) == 0){
				printf("O endereço da imagem não foi encontrado.\n");
				return 0;
			}
	        break;
	      case '?':
	         fprintf(stderr, "Opção -%c requer um argumento.\n", optopt);
	        return 1;
	      default:
	        abort ();
	 }
	
	
	if( (strcmp(formato_img, "ppm")) == 0 ){
			// leitura da imagem ppm
			img_ppm = fopen(img_input, "a+b");
			if(img_ppm == NULL){ // leitura do arquivo
				printf("Erro na abertura da imagem ppm.\n" ); 
				return 0;
			}
			ler_ppm(img_ppm,&ppm_dados); // lê os dados da imagem e seta na variável ppm_dados
	}else{	
			// lendo arquivo bmp
			if( (strcmp(formato_img, "bmp")) == 0 ){	
				img_bmp = fopen(img_input, "a+b"); 
				if(img_bmp == NULL){
					printf("Erro na abertura da imagem bmp.\n" ); 
					return 0;
				}
				ler_bmp(img_bmp, &bmp_fh, &bmp_if); // preenchendo os dados 
			}else{
				printf("O parâmetro com o formato da imagem não foi encontrado.\n");
				return 0;
			}
	}

	
	if(encoder == 1){
		texto_enc = fopen(input_text_file, "r");
		if(texto_enc == NULL){
			printf("Erro na abertura do arquivo com o texto para encriptar.\n" ); 
			return 0;
		}

		while(fgetc(texto_enc) != EOF){ 
			total_pos_text = total_pos_text+1; // quantidade de caracteres que possui o arquivo	
		}
		texto_encriptar = (char*)malloc(total_pos_text*sizeof(char*)); // aloca memória para o texto de entrada
		
		rewind(texto_enc); //posiciona o ponteiro no início

		if (texto_encriptar == NULL) { // verifica se a alocação foi possível
			printf ("Erro: Memoria Insuficiente.");
			return 0;
		}

		while((texto_encriptar[cont_pos_text] = fgetc(texto_enc)) != EOF){
			cont_pos_text = cont_pos_text+1;
		}
		texto_encriptar[cont_pos_text+1]='E'; //adiciona ao final da mensagem a string EOF, padrão usado para identificar que é o fim da mensagem
		texto_encriptar[cont_pos_text+2]='O';
		texto_encriptar[cont_pos_text+3]='F';

		if( (strcmp(formato_img, "bmp")) == 0 ){
				codificador_bmp(texto_encriptar, bmp_if.bmp_colors);
				fclose(fopen(img_input, "w"));
				salvar_bmp(img_bmp, &bmp_fh, &bmp_if);
		}else{
			if( (strcmp(formato_img, "ppm")) == 0 ){
				codificador_ppm(texto_encriptar, ppm_dados.ppm_colors); // codifica o ppm com o texto da palavra. Por padrão, coloca EOF ao final
				fclose(fopen(img_input, "w"));	
				salvar_ppm(img_ppm, &ppm_dados);
			}else{	
				printf("O parâmetro com o formato da imagem não foi encontrado ou está incorreto.\n");
				return 0;
			}
		}
	}	

	if(decoder == 1){
		if(salva_noarquivo == 1){ // verifica se a mensagem será salva no arquivo, caso sim, ele abre o arquivo passado como parâmetro
			arq_text_salvar = fopen(output_text_file, "w");
			if(arq_text_salvar == NULL){ 
				printf("Erro ao tentar abrir o arquivo em que será salvo o texto.\n" ); 
				return 0;
			}
		}

		if( (strcmp(formato_img, "bmp")) == 0 ){
			char* binario_entrada_bmp = (char*)malloc(quantidade_pixels_bmp*3*sizeof(char*)); // array que receberá os bits menos significativos
			char* texto_saida_bmp = (char*)malloc(((quantidade_pixels_bmp*3)/8)*sizeof(char*)); // array que receberá o texto descriptografado
			decodificar_bmp(binario_entrada_bmp, texto_saida_bmp, bmp_if.bmp_colors, arq_text_salvar, salva_noarquivo);
		}else{
			if( (strcmp(formato_img, "ppm")) == 0 ){
				char* binario_entrada = (char*)malloc(quantidade_pixels_ppm*3*sizeof(char*)); // array que receberá os bits menos significativos
				char* texto_saida = (char*)malloc(((quantidade_pixels_ppm*3)/8)*sizeof(char*)); // array que receberá o texto descriptografado
				decodificar_ppm(binario_entrada, texto_saida, ppm_dados.ppm_colors, arq_text_salvar, salva_noarquivo);
			}else{	
				printf("O parâmetro com o formato da imagem não foi encontrado.\n");
				return 0;
			}
		}
	}	


	if( (strcmp(formato_img, "ppm")) == 0 ){
		fclose(img_ppm);//fechando os arquivos
		free(ppm_dados.ppm_colors); // limpando memória
	}
	if( (strcmp(formato_img, "bmp")) == 0 ){
		fclose(img_bmp);//fechando os arquivos
		free(bmp_if.bmp_colors); // limpando memória
	}
	if((decoder == 1) && (salva_noarquivo == 1)){
		fclose(arq_text_salvar);
	}

	return 0;
}