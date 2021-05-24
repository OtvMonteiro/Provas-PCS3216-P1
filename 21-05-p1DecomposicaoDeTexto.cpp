#include <iostream>
#include <fstream>
#include <string>
#include <list>
#include <deque>
#include <stdio.h>

using namespace std;

int caracterEspecial(int l);

int main(){
   ifstream arquivo;
   arquivo.open("liberdade.txt",ios::in);  
   std::list<std::string> palavras;
   cout << "Le arquivos em ANSI, pois c++ nao tem suporte nativo ao UTF-8"<<endl;

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
	std::deque<std::string> p_unicas;
	std::deque<int> ocorrencias;



	for (it_palavras = palavras.begin(); it_palavras != palavras.end(); ++it_palavras) {
		
		bool jaExiste = false;
		int pos_encontrada = 0;
		int pos_contador = 0;

		std::deque<std::string>::iterator itpu; //iterador de palavras unicas
		for (itpu=p_unicas.begin(); itpu != p_unicas.end(); ++itpu)
		{
			if (*itpu == *it_palavras) {//Testa se as palavras são iguais. Precisa ser método próprio de comparação?
				jaExiste = true;
				pos_encontrada = pos_contador;
			}
			
			pos_contador++;
		}

		if (jaExiste) {
			//incrementa um no contador de ocorrencias
			ocorrencias[pos_encontrada] += 1;
		}
		else {//Se ainda nao apareceu, cria uma posicao e poe 1 no contador de ocorrencias
			p_unicas.push_back(*it_palavras);
			ocorrencias.push_back(1);
		}

	}

	//Saida da lista de contagem de palavras

	std::deque<std::string>::iterator itpu; //iterador de palavras unicas
	int i = 0;
	for (itpu = p_unicas.begin(); itpu != p_unicas.end(); ++itpu)
	{
		cout << "Palavra: " << *itpu << " ; Ocorrencias: " << ocorrencias[i] << endl;
		i++;
	}

	//ORDENAR (lista duplamente linkada?)
	


	//OUTPUT





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
