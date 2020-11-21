#include <iostream>
#include <stdlib.h>
#include <stdio.h>
#include <time.h>
#include <chrono>
#include <thread>
#include<string>


using namespace std;
using namespace chrono;

char ptleitura[13];

int cpfs [100][10];
int ctrlcpfs = 0;

int cnpjs [100][13];
int ctrlcnpjs = 0;


void calccpf(int xi){
	int v1 = 0;
	int v2 = 0;

	//Professor, o link que o senhor passou esta com o o pseudocodigo errado, resultando em valores inveridos (v1=v2 e v2=1)
	//A logica esta feita de forma correta abaixo
	for(int i = 0; i<9;i++){
		v2 = v2 + cpfs[xi][i] * (9-(i%10));
		v1 = v1 + cpfs[xi][i] * (9-((i+1)%10));	
	}
	v2 = (v2 % 11)%10;
	v1 = v1 + (v2*9);
	v1 = (v1%11)%10;
	
	cpfs[xi][9] = v1;
	cpfs[xi][10] = v2;
	printf("\n v1 %d, v2 %d \n", v1,v2);
}




int main(){	
	FILE* sc = fopen("BASE.txt", "r");			//Ponteiro para Arquivo

	//For para leitura e classificacao das linhas do arquivo
	for(int i =0;i<10;i++){
		
		
		fscanf(sc,"%s", ptleitura);		//Leitura da linha
		
		int soun = 0;		//Contador para classificao
		int iterador=0;		//Iterador para while
		
		//While para ler a string ate o fim
		while(ptleitura[iterador] != '\0'){
			soun++;			//soma contador
			iterador++;		//soma iterador
		}
		
		//printf("\n cheguei no fim %d , %s",soun,ptleitura);
		
		//Se contador > 11 eh CNPJ		
	    if( soun > 11){
	    	for(int j = 0;j<=11;j++){
	    		cnpjs[ctrlcnpjs][j] = ptleitura[j] - '0';		//Eu amo o Pier
			}
			ctrlcnpjs++;			
		}
		//Se nao eh CPF
		else{
	    	for(int j = 0;j<9;j++){
	    		cpfs[ctrlcpfs][j] = ptleitura[j] - '0';			//Eu amo o Pier
			}
			ctrlcpfs++;
			
		}

	}
	
	//Printa vetores
	for(int i = 0; i<ctrlcpfs;i++){
		calccpf(i);
		cout<<"eh cpf ";
		for(int j = 0; j <=10;j++){
		
			cout << cpfs[i][j];
		}
		cout<<endl;

	}
	for(int i = 0; i<ctrlcnpjs;i++){
		cout<<"eh cnpj ";
		for(int j = 0; j <=11;j++){
			cout << cnpjs[i][j];
		}
		cout<<endl;
	}
	fclose(sc);
	
	
	
	
	return 0;
}
