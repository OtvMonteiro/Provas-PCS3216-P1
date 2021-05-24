#include <iostream>
#include <fstream>
#include <string>
#include <list>
#include <deque>
#include <stdio.h>
#include <malloc.h>

using namespace std;
int caracterEspecial(int l);

struct Palavras
{
	std::string word;
	int occurency;
	int group;
};

int main(){
    // Apresentação
	cout << "Le arquivos em ANSI, pois c++ nao tem suporte nativo ao UTF-8" << endl;
	
	//Entradas de usuario
	cout << "Digite o nome do arquivo que deseja abrir: ";
	string nome_arquivo;
	cin >> nome_arquivo;
	cout << "Digite o nome do arquivo de saida: ";
	string nome_arquivo_out;
	cin >> nome_arquivo_out;
	cout << "Digite 0 para limpar arquivo de saida, ou 1 para adicionar ao fim (append): ";
	int modo_arquivo_saida = 0;
	cin >> modo_arquivo_saida;
   
	//Preparando arquivo
	ifstream arquivo;
	arquivo.open(nome_arquivo.c_str(),ios::in);
	
	//Listas de palavras e palavras-chave
	std::list<std::string> palavras;
	std::list<std::string> kw1;
	std::list<std::string> kw2;

   
	//palavras-chave precisam estar no padrao usado
	int n_kw = 0; //Nº de grupos
	//kw1.push_front("CIENTIFICO"); kw1.push_front("NATURAL"); kw1.push_front("FILOSOFICO"); kw1.push_front("CIENCIA");
	//kw2.push_front("LEI"); kw2.push_front("LIBERDADE"); kw2.push_front("TRABALHO"); kw2.push_front("SOCIAL");

    if (arquivo.is_open()){   //se o arquivo estiver aberto executa o codigo
		//cout << "ABRIU ARQUIVO" << endl;
		//No inicio pega uma linha, passa todos os caracteres para o tipo desejado (maiusculo sem acentos){não distingue palavras com e sem acentos, e.g. "nos" e "nós"}
		string linhaAtual;
	  
		while(getline(arquivo, linhaAtual)){  //le uma linha do arquivo e poe na string tp
			std::list<char> letras;
			//cout << "Linha: " <<linhaAtual<< endl;
			
			for (unsigned int i = 0; i < linhaAtual.length(); i++)
			{
				unsigned char l = linhaAtual[i];
				//maiusculas
				if (l >= 65 && l <= 90) { letras.push_back(l); }
				//minusculas para maiusculas
				else if (l >= 97 && l <= 122) { letras.push_back(l+65-97); }
				//caracteres especiais e acentos
				else { letras.push_back(caracterEspecial(l)); }
			}
			
			//Passa de letra em letra construindo uma string
			//quando achar separador (caracter especial, doferente de letras comuns ou numeros)
			//coloca a palavra na lista de palavras
			string palavraAtual;
			std::list<char>::iterator it_letras;

			for (it_letras = letras.begin(); it_letras != letras.end(); ++it_letras) {
				//avalia fim da palavra
				bool fimDaPalavra;
				int l = *it_letras; //Cast de char para int
				
				switch (l)
				{
				case 48 ... 57: //numeros
					fimDaPalavra = false;
					break;
				case 65 ... 90: //letras
					fimDaPalavra = false;
					break;
				default:
					fimDaPalavra = true;
					break;
				}
				

				if (fimDaPalavra) {
					//coloca a palavra completa na lista de palavras
					palavras.push_back(palavraAtual);
					//reinicia a palavra atual
					palavraAtual = "";
					//palavra atual como caracter especial
					palavraAtual.push_back(*it_letras); 
					palavras.push_back(palavraAtual);
					palavraAtual = "";
					//cout << "COMPLETOU UMA PALAVRA" << endl;
				}
				else {
					palavraAtual.push_back(*it_letras); //poe caracter na palavra
				}

			}
			//Ultima palavra antes do fim da linha
			palavras.push_back(palavraAtual);
	  
		}

		
      arquivo.close();   //fecha arquivo
    }

	//Ordenar a lista antes da contagem
	palavras.sort();
	
	std::list<std::string>::iterator it_palavras;
	std::deque<Palavras> p_unicas;
	std::deque<int> ocorrencias;
	
	

	for (it_palavras = palavras.begin(); it_palavras != palavras.end(); ++it_palavras) {
		
		bool jaExiste = false;

		std::deque<Palavras>::iterator itpu; //iterador de palavras unicas
		for (itpu=p_unicas.begin(); itpu != p_unicas.end(); ++itpu)
		{
			if ((*itpu).word == *it_palavras) {//Testa se as palavras são iguais. Precisa ser método próprio de comparação?
				jaExiste = true;
				(*itpu).occurency++;
			}
		}
		
		if (!jaExiste){//Se ainda nao apareceu, cria uma posicao e poe 1 no contador de ocorrencias
			//struct Palavras* p = (struct Palavras*) malloc(sizeof(struct Palavras));
			Palavras p;
			p.word = *it_palavras;
			p.occurency = 1;
			p.group = 0;
			p_unicas.push_back(p);
		}
		
	}
	

	//Seleciona grupos
	std::deque<Palavras>::iterator itpu; //iterador de palavras unicas
	for (itpu = p_unicas.begin(); itpu != p_unicas.end(); ++itpu)
	{
		//Palavras chave 1
		for (std::list<std::string>::iterator itkw1 = kw1.begin(); itkw1 != kw1.end(); ++itkw1)
		{
			if ((*itpu).word == *itkw1) {(*itpu).group = 1;}
		}
		//Palavras chave 2
		for (std::list<std::string>::iterator itkw2 = kw2.begin(); itkw2 != kw2.end(); ++itkw2)
		{
			if ((*itpu).word == *itkw2) { (*itpu).group = 2; }
		}
	}

	

	//ORDENAR (lista duplamente linkada?)
	


	//OUTPUT
	ofstream out_f;
	if (modo_arquivo_saida == 1) { out_f.open(nome_arquivo_out.c_str(), ios::app); }
	else						 { out_f.open(nome_arquivo_out.c_str(), ios::out); }
	
	
	for (int i = 0; i <= n_kw; i++)
	{
		//out_f << endl << "Grupo " << i << endl;
		//out_f << "Palavras ; Occorrencias:" << endl << endl;
		std::deque<Palavras>::iterator itpu; //iterador de palavras unicas
		for (itpu = p_unicas.begin(); itpu != p_unicas.end(); ++itpu)
		{
			if (i == (*itpu).group) {
				out_f << (*itpu).word <<" "<< (*itpu).occurency << endl;
				cout << (*itpu).word << " " << (*itpu).occurency << endl;
			}
		}
		
	}

	out_f.close();
	
	
	//Fechamento
	cout << endl  <<"Fim da execucao"<< endl;
	int close;
	cin >> close;

}

int caracterEspecial(int l) {
	int out;
	//cout << "l: " <<l<< endl;
	switch (l)
	{
		//A
		case 192 ... 197:
			out = 65;
			break;
		case 225 ... 229:
			out = 65;
			break;
		//E
		case 200 ... 203:
			out = 69;
			break;
		case 232 ... 235:
			out = 69;
			break;
		//I
		case 204 ... 207:
			out = 73;
			break;
		case 236 ... 239:
			out = 73;
			break;
		//O
		case 210 ... 214:
			out = 79;
			break;
		case 242 ... 246:
			out = 79;
			break;
		//U
		case 217 ... 220:
			out = 85;
			break;
		case 249 ... 252:
			out = 85;
			break;
		//C
		case 199:
			out = 67;
			break;
		case 231:
			out = 67;
			break;
		//OUTROS
		default:
			out = l;

	}
	//cout << "out: " << out << endl;

	return out;


}
