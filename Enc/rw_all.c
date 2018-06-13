#include <string.h>
#include <stdio.h>
#include <math.h>
#include <stdlib.h>
#include "rw_all.h"
#include "rw_bmp.h"
#include "rw_ppm.h"

// Funções genéricas
// As funções com operações com bytes, foram feitas utilizando array de bits

unsigned int int_to_bin(int n) { // função para transformar decimal para binário
    if (n == 0){
    	return 0;
    } 
    if (n == 1){
    	return 1;
    }
    return(n % 2) + 10 * int_to_bin(n/2);
}

int binary_to_int(int binario){ // função para transformar binário para decimal
    int decimal = 0, base = 1, bit;
    while (binario > 0){
        bit = binario % 10; // pega o primeiro bit
        decimal = decimal + bit * base; //faz a operação com o primeiro bit e soma com o decimal
        binario = binario / 10 ; // elimina o primeiro bit
        base = base * 2;
    }
    return decimal;
}

unsigned int set_bits(char bit_in, unsigned int bits_out){ // recebe um bit e um byte(int) e coloca esse bit no último bit do byte(int)
	char new_bits[8]={'0','0','0','0','0','0','0','0'}; // novo byte que será gerado
	int  i;
	int return_bits;
	
	for (i = 7; i >= 0; --i){  // pega cada bit do byte(em formato int) e coloca no array
		if((bits_out % 10) == 0){
			new_bits[i]= '0';
		}else{ 
			new_bits[i]= '1'; 
		}
		bits_out = bits_out/10; // elimina o bit já pego
	}
	
	if(bit_in == '1' ){
	 	new_bits[7] = '1'; //seta o bit menos significativo
	}
	if(bit_in == '0' ){
	 	new_bits[7] = '0'; //seta o bit menos significativo
	}
	sscanf(new_bits, "%d", &return_bits); // Converte o array de bits em um int(não em decimal). Ex {'1','1','0'} vira 110

	return return_bits;
}


int binary_to_text(char *texto_saida, char *binario) { // Transforma o array de bits em char
	int i,j=7;
	int num=0;
	int aux1=0;
	int aux2=0;
	int total_char=-1;	
	int total_bits = strlen(binario); // Total de bits

    for (i = 0; i < total_bits; ++i){
    	if(binario[i]=='1'){
    		num = num + 1*pow(2, j);
    	}	
    
    	if(j==0){ // Calcula o decimal referente a cada grupo de 8 bits
    		total_char = total_char+1;
    		texto_saida[total_char]= (char)num; // Salva o decimal
    		num=0; // Zera o decimal e calcula novamente para os próximos 8 bits
    		j=8;
    	}
    	j = j-1;
    	
    	if((aux1 == 1) && (aux2 == 1) && (texto_saida[total_char] == 'F')){ // Calcula até chegar na string EOF, que é o padrão para indicar o fim do texto.
    		i = total_bits+1;
    	}
    	if((aux1==1) && (texto_saida[total_char] == 'O')){
    		aux2=1;
    	}
    	if((texto_saida[total_char] == 'E')){
    		aux1=1;	
    	} 

    }
  
    return total_char-3;
}
