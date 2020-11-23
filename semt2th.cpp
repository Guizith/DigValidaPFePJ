#include <iostream>
#include <stdlib.h>
#include <stdio.h>
#include <time.h>
#include <chrono>
#include <thread>
#include<string>

#define NUMLIN 20 			//Define o numero de linhas que serao lidas da base de dados


using namespace std;
using namespace chrono;

int cnpjs [NUMLIN][14];		//Vetor Matriz dos CNPJs separados por digitos. cnpjs[cnpjN][digN] 
int ctrlcnpjs = 0;			//Variave de controle do vetor de CNPJs
int cpfs [NUMLIN][11];		//Vetor Matriz dos CPFs separados por digitos. cpfs[cpfN][digN] 
int ctrlcpfs = 0;			//Variave de controle do vetor de CPFs

char ptleitura[14];			//Variavel auxiliar para armazenamento temporario da leitura do arquivo
int solog = true;			//Variavel auxiliar utilizada para escolha se quer print no terminou ou so log

milliseconds tmple;			//Variavel de armazenamento do tempo de execucao da funcao de leitura dos dados
milliseconds tmpclc;		//Variavel de armazenamento do tempo de execucao da funcao de calculo dos digitos verificadores
milliseconds tmpprint;		//Variavel de armazenamento do tempo de execucao da funcao de print e salvamento de arquivo de log



//Funcao de leitura do arquivo e armazenamento 
void learq(){
	
	FILE* sc = fopen("BASE.txt", "r");			//Ponteiro para o arquivo BASE.txt em modo "Read"

	//For para leitura e classificacao das linhas do arquivo
	for(int i =0;i<NUMLIN;i++){

		fscanf(sc,"%s", ptleitura);		//Leitura da linha
		
		int soun = 0;		//Contador para classificao
		int iterador=0;		//Iterador para while
		
		//While para ler a string ate o fim
		while(ptleitura[iterador] != '\0'){
			soun++;			//soma contador
			iterador++;		//soma iterador
		}
		
		//Se contador > 11 eh CNPJ
	    if(soun > 11){
	    	for(int j = 0;j<=12;j++){
	    		cnpjs[ctrlcnpjs][j] = ptleitura[j] - '0';		//Eu amo o Pier
			}
			ctrlcnpjs++;			//Soma variavel de controle de cnpj
		}
		//Se nao eh CPF
		else{
	    	for(int j = 0;j<9;j++){
	    		cpfs[ctrlcpfs][j] = ptleitura[j] - '0';			//Eu amo o Pier
	    		
			}
			
			ctrlcpfs++;				//Soma variavel de controle de cpf
			
		}

	}
	fclose(sc);		//Fecha o arquivo
}

//Funcao de calculo de digitos verifcadores de CPFs
//Parametros: (inicio, fim), representam o intervalo em que a função calcula-rá os digitos verificadores
//Necessario especificar o inicio e fim (intervalo) pois a utilização de mais de uma thread necessita tal organizacao
void calccpf(int xi, int xend){

	for(int j =xi; j<=xend;j++){
		
		//Variaveis auxiliares para o calculo dos digitos verificadores
		int v1 = 0;				
		int v2 = 0;
		
		//calculo dos digitos verificadores
		v1 = (cpfs[j][0]*10)+(cpfs[j][1]*9)+(cpfs[j][2]*8)+(cpfs[j][3]*7)+(cpfs[j][4]*6)+(cpfs[j][5]*5)+(cpfs[j][6]*4)+(cpfs[j][7]*3)+(cpfs[j][8]*2);
		v1 = v1%11;
		v1 = 11 - v1;
		if(v1 >=10){
			v1 = 0;
		}
		v2 = (cpfs[j][0]*11)+(cpfs[j][1]*10)+(cpfs[j][2]*9)+(cpfs[j][3]*8)+(cpfs[j][4]*7)+(cpfs[j][5]*6)+(cpfs[j][6]*5)+(cpfs[j][7]*4)+(cpfs[j][8]*3)+(v1*2);
		v2 = v2%11;
		v2 = 11 - v2;
		if(v2 >=10){
			v2=0;
		}

		//cout<< v1 <<".." <<v2 <<endl;
		//Atribuição dos digitos no respectivo cpf
		cpfs[j][9] = v1;
		cpfs[j][10] = v2;
	}
	

}

//Funcao de calculo de digitos verifcadores de CNPJs
//Parametros: (inicio, fim), representam o intervalo em que a função calcula-rá os digitos verificadores
//Necessario especificar o inicio e fim (intervalo) pois a utilização de mais de uma thread necessita tal organizacao
void calccnpj(int xi, int xend){
	//Variaveis auxiliares para o calculo dos digitos verificadores
	int v1 = 0;
	int v2 = 0;

	//calculo dos digitos verificadores
	for(int j = xi ; j<=xend;j++){
		v1 = (6*cnpjs[j][0])+(7*cnpjs[j][1])+(8*cnpjs[j][2])+(9*cnpjs[j][3]);
		v1 = v1 + ((2*cnpjs[j][4])+(3*cnpjs[j][5])+(4*cnpjs[j][6])+(5*cnpjs[j][7]));
		v1 = v1 + ((6*cnpjs[j][8])+(7*cnpjs[j][9])+(8*cnpjs[j][10])+(9*cnpjs[j][11]));
		v1 = v1%11;
		
		if(v1 >= 10){
			v1 = 0;
		}
		
		v2 = (5*cnpjs[j][0])+(6*cnpjs[j][1])+(7*cnpjs[j][2])+(8*cnpjs[j][3]);
		v2 = v2 + ((9*cnpjs[j][4])+(2*cnpjs[j][5])+(3*cnpjs[j][6])+(4*cnpjs[j][7]));
		v2 = v2 + ((5*cnpjs[j][8])+(6*cnpjs[j][9])+(7*cnpjs[j][10])+(8*cnpjs[j][11] + (9*v1)));
		v2 = v2%11;
		
		if(v2 >= 10){
			v2 = 0;
		}
			
		//cout<< v1 <<".." <<v2 <<endl;
		//Atribuição dos digitos no respectivo cnpj
		cnpjs[j][12] = v1;
		cnpjs[j][13] = v2;
	}

}

//Funcao para fazer os calculos dos digitos verificadores dos CPFs e CNPJs em suas respectivas threads
void fazcalcTh(){
	
	//Threads de calculo de CPFs
	std::thread halfcpf1(calccpf,0,(ctrlcpfs/2));				//Thread que calcula a primeira metade do vetor de cpf
	std::thread halfcpf2(calccpf,((ctrlcpfs/2)+1),ctrlcpfs);	//Thread que calcula a segunda metade do vetor de cpf

	//Threads de calculo de CNPJs
	std::thread halfcnpj1(calccnpj,0,(ctrlcnpjs/2));			//Thread que calcula a primeira metade do vetor de cnpj
	std::thread halfcnpj2(calccnpj,((ctrlcnpjs/2)+1),ctrlcnpjs);//Thread que calcula a segunda metade do vetor de cnpj
						
	//Joins das threads de calculo de CPFs
	halfcpf1.join();
	halfcpf2.join();
	
	//Joins das threads de calculo de CNPJs
	halfcnpj1.join();
	halfcnpj2.join();
	
}

//Funcao para fazer os calculos dos digitos verifcadores dos CPFs e CNPJs sem a utilizacao de threads
void fazcalc(){
	
	//calccpf(0,ctrlcpfs);		//Calcula todos os digitos de CPFs
	//calccnpj(0,ctrlcnpjs);	//Calcula todos os digitos de CNPJs
}

//Funcao para printar na tela e salvar o arquivo log.txt com ponto
void prints(){
	
	//Print de menu de escolha (salvar e mostrar na tela ou apenas salvar)
	cout << "[-]Voce quer ver os CPFs e CNPJs no terminal ou apenas exibir o tempo das execucoes?[-]" << endl;
	cout << "OBS: Mesmo que escolha nao exibir os dados no terminal, sera gerado um arquivo \"Log.txt\" com todos CPFs e CNPJs calculados"<<endl;
	cout << "0 - Nao"<<endl;
	cout << "1 - Sim"<<endl;
	scanf("%d",&solog);			//Leitura da escolha
	
	
	FILE* sc = fopen("log.txt", "w");	//Ponteiro para o arquivo log.txt em modo Write
	
	//Se escolha for 1 entao mostra dados no terminal
	if(solog == 1){
		
		//Print e save CPFs
		//For para percorrer todos os CPFs
		for(int i = 0; i<ctrlcpfs;i++){
			
			int point = 0;					//Variavel Flag para indicar aonde printar o ponto (a cada 3 numeros xxx.xxx.xxx-xx)
			cout<<"eh cpf: ";
			fprintf(sc,"eh cpf: ");
			
			//For para percorrer todos os numeros do respetivo CPF
			for(int j = 0; j <=10;j++){
				//Caso nao tenha chegado no digito 8
				if( j !=8){
					//Se estiver no terceiro digito, printa digito e ponto
					if(point == 2){
						cout << cpfs[i][j] << ".";
						fprintf(sc,"%d.",cpfs[i][j]);
						point = 0;
					}
					//Senao printa apenas o numero
					else{
						cout << cpfs[i][j];
						fprintf(sc,"%d",cpfs[i][j]);
						point++;
					}
				}
				//Caso tenha chegado no digito 8, printar numero e colocar o traco
				else{
					cout << cpfs[i][j]<<"-";
					fprintf(sc,"%d-",cpfs[i][j]);
					point = 0;
				}
			}
			//Pula linha para proximos prints/iteracoes
			cout<<endl;
			fprintf(sc,"\n");	
		}
		
		//Print e save CNPJs
		//For para percorrer todos os CNPJs
		for(int i = 0; i<ctrlcnpjs;i++){
			
			int point1 = 0;			//Variavel Flag para indicar aonde printar o  primeiro ponto (XX.XXX.XXX/XXXX-XX)
			int point = 0;			//Variavel Flag para indicar aonde printar o  segundo ponto (XX.XXX.XXX/XXXX-XX)
			int barra = 0;			//Variavel Flag para indicar aonde printar a barra (XX.XXX.XXX/XXXX-XX)
			cout<<"eh cnpj: ";
			fprintf(sc,"eh cnpj: ");
			
			//For para percorrer todos os digitos do respectivo CNPJ
			for(int j = 0; j <=13;j++){
				//Caso nao tenha chegado no digito 11 (aonde coloca o traco)
				if(j != 11){
					//Se nao esta no segundo digito, printa apenas o numero 
					if(point1 < 1){		
						cout << cnpjs[i][j];
						fprintf(sc,"%d",cnpjs[i][j]);
						point1++;
						point++;
						barra++;
					}
					//Se esta no segundo digito, printa o numero e o primeiro ponto
					else if(point1 == 1){
						cout << cnpjs[i][j]<<".";
						fprintf(sc,"%d.",cnpjs[i][j]);
						point1=3;
						point++;
						barra++;
					}
					//Se nao esta no quarto digito, printa apenas o numero
					else if(point < 4){
						cout << cnpjs[i][j];
						fprintf(sc,"%d",cnpjs[i][j]);
						point++;
						barra++;	
					}
					//Se esta no quarto digito, printa o digito e o segundo ponto
					else if(point == 4){
						cout << cnpjs[i][j]<<".";
						fprintf(sc,"%d.",cnpjs[i][j]);
						point=5;
						barra++;
					}
					//Se nao esta no setimo digito, printa apenas o numero
					else if(barra < 7){
						//cout << "cheguei";
						cout << cnpjs[i][j];
						fprintf(sc,"%d",cnpjs[i][j]);
						barra++;
					}
					//se esta no setimo digito, printa o digito e a barra
					else if(barra == 7){
						cout << cnpjs[i][j]<<"/";
						fprintf(sc,"%d/",cnpjs[i][j]);
						barra = 300;
					}
					//Se nao printa apenas numero
					else{
						cout << cnpjs[i][j];
						fprintf(sc,"%d",cnpjs[i][j]);
					}
				}
				//Caso esteja no decimo primeiro digito, printa o numero e o traco
				else{
					cout << cnpjs[i][j] <<"-";
					fprintf(sc,"%d-",cnpjs[i][j]);
					//Eleva todas as flags para nao serem mais validadas nos ifs
					point = 300;
					point1 = 300;
					barra = 300;
				}
				
			}
			//Pula linha para proximos prints/iteracoes
			cout<<endl;
			fprintf(sc,"\n");
		}	
	}
	//Se escolha nao foi 1, entao
	else{
		//Printa CPFs
		//For para percorrer todos CPFs
		for(int i = 0; i<ctrlcpfs;i++){
			
			int point = 0;//Variavel Flag para indicar aonde printar o ponto (a cada 3 numeros xxx.xxx.xxx-xx)
			fprintf(sc,"eh cpf: ");
			
			//For para percorrer todos os numeros do respetivo CPF
			for(int j = 0; j <=10;j++){	
				//Caso nao tenha chegado no digito 8
				if( j !=8){
					//Se estiver no terceiro digito, printa digito e ponto
					if(point == 2){
						fprintf(sc,"%d.",cpfs[i][j]);
						point = 0;
					}
					//Senao printa apenas o numero
					else{
						fprintf(sc,"%d",cpfs[i][j]);
						point++;
					}
				}
				//Caso tenha chegado no digito 8, printar numero e colocar o traco
				else{
					fprintf(sc,"%d-",cpfs[i][j]);
					point = 0;
				}
			}
			//Pula linha para proximos prints/iteracoes
			fprintf(sc,"\n");	
		}
		
		//Printa CNPJs
		//For para percorrer todos os CNPJs
		for(int i = 0; i<ctrlcnpjs;i++){
			
			int point1 = 0;			//Variavel Flag para indicar aonde printar o  primeiro ponto (XX.XXX.XXX/XXXX-XX)
			int point = 0;			//Variavel Flag para indicar aonde printar o  segundo ponto (XX.XXX.XXX/XXXX-XX)
			int barra = 0;			//Variavel Flag para indicar aonde printar a barra (XX.XXX.XXX/XXXX-XX)
			fprintf(sc,"eh cnpj: ");
			
			//For para percorrer todos os digitos do respectivo CNPJ
			for(int j = 0; j <=13;j++){
				//Caso nao tenha chegado no digito 11 (aonde coloca o traco)
				if(j != 11){
					//Se nao esta no segundo digito, printa apenas o numero 
					if(point1 < 1){		
						fprintf(sc,"%d",cnpjs[i][j]);
						point1++;
						point++;
						barra++;
					}
					//Se esta no segundo digito, printa o numero e o primeiro ponto
					else if(point1 == 1){
						fprintf(sc,"%d.",cnpjs[i][j]);
						point1=3;
						point++;
						barra++;
					}
					//Se nao esta no quarto digito, printa apenas o numero
					else if(point < 4){
						fprintf(sc,"%d",cnpjs[i][j]);
						point++;
						barra++;	
					}
					//Se esta no quarto digito, printa o digito e o segundo ponto
					else if(point == 4){
						fprintf(sc,"%d.",cnpjs[i][j]);
						point=5;
						barra++;
					}
					//Se nao esta no setimo digito, printa apenas o numero
					else if(barra < 7){
						fprintf(sc,"%d",cnpjs[i][j]);
						barra++;
					}
					//se esta no setimo digito, printa o digito e a barra
					else if(barra == 7){
						fprintf(sc,"%d/",cnpjs[i][j]);
						barra = 300;
					}
					//Se nao printa apenas numero
					else{
						fprintf(sc,"%d",cnpjs[i][j]);
					}
				}
				//Caso esteja no decimo primeiro digito, printa o numero e o traco
				else{
					fprintf(sc,"%d-",cnpjs[i][j]);
					//Eleva todas as flags para nao serem mais validadas nos ifs
					point = 300;
					point1 = 300;
					barra = 300;
				}
				
			}
			
			//Pula linha para proximos prints/iteracoes
			fprintf(sc,"\n");
		}
		
	}
	
	//Printa e salva o tempo de execucao de leitura de dados e calculo de digitos
	cout << "O tempo de excucao da leitura da base de dados foi de :"<< tmple.count() << " m/s"<<endl;
	cout << "O tempo de excucao do calculo dos digitos foi de :"<< tmpclc.count() << " m/s"<<endl;
	fprintf(sc,"O tempo de excucao da leitura da base de dados foi de : %d m/s",tmple.count());
	fprintf(sc,"O tempo de excucao do calculo dos digitos foi de : %d m/s",tmpclc.count());
	
	//Fecha arquivo
	fclose(sc);	
}

//Funcao para printar na tela e salvar o arquivo log.txt sem ponto
void printsSemPonto(){
	
	//Print de menu de escolha (salvar e mostrar na tela ou apenas salvar)
	cout << "[-]Voce quer ver os CPFs e CNPJs no terminal ou apenas exibir o tempo das execucoes?[-]" << endl;
	cout << "OBS: Mesmo que escolha nao exibir os dados no terminal, sera gerado um arquivo \"Log.txt\" com todos CPFs e CNPJs calculados"<<endl;
	cout << "0 - Nao"<<endl;
	cout << "1 - Sim"<<endl;
	scanf("%d",&solog);					//Leitura da escolha
	
	FILE* sc = fopen("log.txt", "w");	//Ponteiro para o arquivo log.txt em modo Write
	
	//Se escolha for 1 entao mostra dados no terminal
	if(solog == 1){
		//Printa CPFs
		//For para percorrer todos os CPFs
		for(int i = 0; i<ctrlcpfs;i++){
			cout<<"eh cpf: ";
			fprintf(sc,"eh cpf: ");
			
			//For para percorrer todos os numeros do respetivo CPF
			for(int j = 0; j <=10;j++){	
				cout << cpfs[i][j];
				fprintf(sc,"%d",cpfs[i][j]);
			}
			cout<<endl;
			fprintf(sc,"\n");	
		}
		
		//Printa CNPJs
		//For para percorrer todos os CNPJs
		for(int i = 0; i<ctrlcnpjs;i++){
			cout<<"eh cnpj: ";
			fprintf(sc,"eh cnpj: ");
			
			//For para percorrer todos os digitos do respectivo CNPJ
			for(int j = 0; j <=13;j++){
				cout<<cnpjs[i][j];
				fprintf(sc,"%d",cnpjs[i][j]);	
			}
			cout<<endl;
			fprintf(sc,"\n");
		}	
	}
	//Se escolha nao foi 1, entao
	else{
		//Printa CPFs
		//For para percorrer todos os CPFs
		for(int i = 0; i<ctrlcpfs;i++){
			fprintf(sc,"eh cpf: ");
			
			//For para percorrer todos os numeros do respetivo CPF
			for(int j = 0; j <=10;j++){	
					fprintf(sc,"%d",cpfs[i][j]);
				}
			}
			fprintf(sc,"\n");	
		}
		
		//Printa CNPJs
		//For para percorrer todos os CNPJs
		for(int i = 0; i<ctrlcnpjs;i++){
			fprintf(sc,"eh cnpj: ");
			
			//For para percorrer todos os digitos do respectivo CNPJ
			for(int j = 0; j <=13;j++){
				fprintf(sc,"%d",cnpjs[i][j]);
			}
			fprintf(sc,"\n");
		}
		
		
	//Printa e salva o tempo de execucao de leitura de dados e calculo de digitos
	cout << "O tempo de excucao da leitura da base de dados foi de :"<< tmple.count() << " m/s"<<endl;
	cout << "O tempo de excucao do calculo dos digitos foi de :"<< tmpclc.count() << " m/s"<<endl;
	fprintf(sc,"O tempo de excucao da leitura da base de dados foi de : %d m/s",tmple.count());
	fprintf(sc,"O tempo de excucao do calculo dos digitos foi de : %d m/s",tmpclc.count());
	
	//Fecha arquivo
	fclose(sc);		
}

//Funcao orquestradora que organiza as chamadas de funcoes e faz a medicao do tempo de cada funcao
void orqs(){
	
	//Chamada da leitura da base de dados
	auto start = std::chrono::steady_clock::now(); 					//Salva o tempo atual do clock
	learq();														//Chamada da funcao de leitura de dados
	auto end = steady_clock::now();									//Salva o tempo atual do clock
	tmple = duration_cast<milliseconds>(end - start);	//Calculo da duração da chamada da funcao learq() em millisegundos
	
	start = std::chrono::steady_clock::now();						//Salva o tempo atual do clock
	fazcalcTh();													//Chamada da funcao que faz o calculo dos digitos COM thread
	fazcalc();														//Chamada da funcao que faz o calculo dos digitos SEM thread
	end = steady_clock::now();										//Salva o tempo atual do clock
	tmpclc = duration_cast<milliseconds>(end - start);	//Calculo da duração da chamada da funcao learq() em millisegundos
	
	start = std::chrono::steady_clock::now();						//Salva o tempo atual do clock
	prints();														//Chamada da funcao de print e save do arquivo de log COM ponto
	//printsSemPonto();												//Chamada da funcao de print e save do arquivo de log SEM ponto
	end = steady_clock::now();										//Salva o tempo atual do clock
	tmpprint = duration_cast<milliseconds>(end - start);//Calculo da duração da chamada da funcao learq() em millisegundos
	
	
	//Print do tempo de execucao da chamada de print que salva os dados
	//Este dado nao eh impresso no arquivo de log, pois o mesmo mostra o tempo que demorou para salvar os dados no arquivo
	cout << "O tempo de excucao da funcao de print foi de :"<< tmpprint.count() << " m/s"<<endl;
}

int main(){	
	orqs();			//Chamada da funcao orquestrador
	return 0;	
}
