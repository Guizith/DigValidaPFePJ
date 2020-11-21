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

string cpfs [100][10];
int ctrlcpfs = 0;

string cnpjs [100][13];
int ctrlcnpjs = 0;


int main(){	
	FILE* sc = fopen("BASE.txt", "r");			//Ponteiro para Arquivo

	//For para leitura e classificacao das linhas do arquivo
	for(int i =0;i<100;i++){
		
		
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
	    	*cnpjs[ctrlcnpjs] = ptleitura;
			ctrlcnpjs++;			
		}
		//Se nao eh CPF
		else{
	    	*cpfs[ctrlcpfs] = ptleitura;
			ctrlcpfs++;
			
		}

	}
	
	
	
	
	
	
	
	//Printa vetores
	for(int i = 0; i<ctrlcpfs;i++){
		cout << "eh cpf " << *cpfs[i]<<endl;
	}
	for(int i = 0; i<ctrlcnpjs;i++){
		cout << "eh cnpj " << *cnpjs[i]<<endl;
		
	}
	
	
	
	fclose(sc);
	return 0;
}
