#pragma once
using namespace std;
#include <iostream>
#include <ostream>
#include <string>
#include "ListaDupla.h"
#include <math.h>
#include <xhash>

#define DEFAULT_LENGTH 24
#define DEFAULT_TAXA_CRESCIMENTO 4
#define DEFAULT_TAXA_OCUPACAO 50
#define DEFAULT_TAMANHO_MAXIMO_LISTA 3
#define DEFAULT_OPERACAO '*'
#define DEFAULT_DIFERENCA_TAM 3
#define DEFAULT_DIFERENCA_POS 4

template <class TipoKey, class TipoDado>

class HashTable
{
public:
	//-------------------------------------------------------------------------------------------------------------------------------------------------------//
	//------------------------------------------------------------------CONSTRUTORES-------------------------------------------------------------------------//
	//-------------------------------------------------------------------------------------------------------------------------------------------------------//

	// Construtor padr�o sem argumentos
	HashTable() : qtosDados(0), ehPadrao(true), length(DEFAULT_LENGTH), taxaDeCrescimento(DEFAULT_TAXA_CRESCIMENTO), tamMaxDasListas(DEFAULT_TAMANHO_MAXIMO_LISTA), 
		qtdMaxDeDados(DEFAULT_LENGTH * (DEFAULT_TAXA_OCUPACAO / 100.0)), operadorDeCrescimento(DEFAULT_OPERACAO), ehRadical(false)
	{
		this->hashTable = (ListaDupla<NoHashTable>*)malloc(this->length * qtosDadosof(ListaDupla<NoHashTable>));
	}

	// Construtor padr�o e radical sem argumentos
	HashTable(const bool& _ehRadical) : qtosDados(0), ehPadrao(true), length(DEFAULT_LENGTH), taxaDeCrescimento(DEFAULT_TAXA_CRESCIMENTO), tamMaxDasListas(DEFAULT_TAMANHO_MAXIMO_LISTA),
		qtdMaxDeDados(DEFAULT_LENGTH * (DEFAULT_TAXA_OCUPACAO / 100.0)), operadorDeCrescimento(DEFAULT_OPERACAO), diferencaDeTam(DEFAULT_DIFERENCA_TAM), diferencaDePos(DEFAULT_DIFERENCA_POS),
		ehRadical(_ehRadical)
	{
		this->hashTable = (ListaDupla<NoHashTable>*)malloc(this->length * qtosDadosof(ListaDupla<NoHashTable>));
	}

	// Construtor personalizado, que tem par�metros de tamanho, taxa de crescimento, taxa de ocupa��o, tamanho m�ximo das listas e a opera��o de crescimento da hashTable
	HashTable(const unsigned int& _novoLength, const unsigned int& _novaTaxaDeCrescimento, const unsigned int& _novaTaxaDeOcupacao, const unsigned int& _novoTamanhoMaximoLista, const char& _novaOperacao) : 
			  qtosDados(0), ehPadrao(false), length(_novoLength), taxaDeCrescimento(_novaTaxaDeCrescimento), tamMaxDasListas(_novoTamanhoMaximoLista), 
			  qtdMaxDeDados(_novoLength * (_novaTaxaDeOcupacao / 100.0)), operadorDeCrescimento(_novaOperacao), ehRadical(false)
	{
		if (_novoLength <= 0 || _novaTaxaDeCrescimento <= 0 || _novaTaxaDeOcupacao <= 0 || _novoTamanhoMaximoLista <= 0 || (_novaOperacao != '*' && _novaOperacao != '+') || _novaTaxaDeOcupacao >= 100)
		{
			cout << "Valores inv�lidos para a HashTable, criando vers�o padr�o." << endl;
			*this = HashTable<TipoKey, TipoDado>();
		}
		else
			this->hashTable = (ListaDupla<NoHashTable>*)malloc(this->length * qtosDadosof(ListaDupla<NoHashTable>));
	}

	// Construtor personalizado e radical, que tem os mesmos par�metros do construtor anterior, mas com dois outros par�metros de diferen�a de posi��o e de tamanho
	HashTable(const bool& _novoehRadical, const unsigned int& _novoLength, const unsigned int& _novaTaxaDeCrescimento, const unsigned int& _novaTaxaDeOcupacao, const unsigned int& _novoTamanhoMaximoLista, 
			  const char& _novaOperacao, const unsigned int _novaDiferencaDeTam, const unsigned int _novaDiferencaDePos) :
			  qtosDados(0), ehPadrao(false), length(_novoLength), taxaDeCrescimento(_novaTaxaDeCrescimento), tamMaxDasListas(_novoTamanhoMaximoLista), qtdMaxDeDados(_novoLength * (_novaTaxaDeOcupacao / 100.0)),
			  operadorDeCrescimento(_novaOperacao), diferencaDePos(_novaDiferencaDePos), diferencaDeTam(_novaDiferencaDeTam), ehRadical(_novoehRadical)
	{
		if (_novoLength <= 0 || _novaTaxaDeCrescimento <= 0 || _novaTaxaDeOcupacao <= 0 || _novoTamanhoMaximoLista <= 0 || (_novaOperacao != '*' && _novaOperacao != '+') || _novaTaxaDeOcupacao >= 100 ||
			_novaDiferencaDePos <= 0 || _novaDiferencaDeTam <= 0)
		{
			cout << "Valores inv�lidos para a HashTable, criando vers�o padr�o." << endl;
			*this = HashTable<TipoKey, TipoDado>();
		}
		else
			this->hashTable = (ListaDupla<NoHashTable>*)malloc(this->length * qtosDadosof(ListaDupla<NoHashTable>));
	}

	// Construtor de c�pia
	HashTable(const HashTable<TipoKey, TipoDado>& outro) : qtosDados(outro.qtosDados), ehPadrao(outro.ehPadrao), length(outro.length), taxaDeCrescimento(outro.taxaDeCrescimento), tamMaxDasListas(outro.tamMaxDasListas), 
		qtdMaxDeDados(outro.qtdMaxDeDados), operadorDeCrescimento(outro.operadorDeCrescimento), ehRadical(outro.ehRadical), diferencaDeTam(outro.diferencaDeTam), diferencaDePos(outro.diferencaDePos)
	{
		this->hashTable = (ListaDupla<NoHashTable>*)malloc(this->length * qtosDadosof(ListaDupla<NoHashTable>));
		for (int i = 0; i < length; i++)
			*(this->hashTable + i) = *(outro.hashTable + i);
	}

	//--------------------------------------------------------------------------------------------------------------------------------------------------------//
	//---------------------------------------------------------------------GETTERS----------------------------------------------------------------------------//
	//--------------------------------------------------------------------------------------------------------------------------------------------------------//

	// Retorna o tamanho m�ximo que as listas devem ter
	unsigned int getTamMaxDasListas() const
	{
		return this->tamMaxDasListas;
	}

	// Retorna a quantidade m�xima de dados que a hashtable deve ter
	unsigned int getQtdMaxDeDados() const
	{
		return this->qtdMaxDeDados;
	}

	// Retorna a diferen�a m�xima de tamanho das listas dentro da hashTable
	unsigned int getDiferencaDeTam() const
	{
		return this->diferencaDeTam;
	}

	// Retorna a diferen�a m�xima de posi��o dos n�s entre s�
	unsigned int getDiferencaDePos() const
	{
		return this->diferencaDePos;
	}

	// Retorna se a hashtable � padr�o
	bool isPadrao() const
	{
		return this->ehPadrao;
	}

	// Retorna se a hashtable � radical
	bool isRadical() const
	{
		return this->ehRadical;
	}

	//--------------------------------------------------------------------------------------------------------------------------------------------------------//
	//---------------------------------------------------------------M�TODOS PRINCIPAIS-----------------------------------------------------------------------//
	//--------------------------------------------------------------------------------------------------------------------------------------------------------//

	// Retorna se incluiu com sucesso ou n�o, chamando o m�todo de incluir protegido
	bool inserir(const TipoKey& key, const TipoDado& dado)
	{
		return inserir(key, dado, true);
	}

	// Retorna se o dado foi removido com sucesso
	bool deletar(const TipoKey& key)
	{
		if (!existe(key))
			return false;
		int pos = this->calcularPosicao(key);
		if (!this->isEmpty(pos))
		{
			for (int i = 0; i < (this->hashTable + pos)->getTamanho(); i++)
			{
				if ((this->hashTable + pos)->operator[](i).getChave() == key)
				{
					(this->hashTable + pos)->removerPos(i);
					if (this->isEmpty(pos))
						(this->hashTable + pos)->~ListaDupla();
				}
			}
		}
		else
			return false;
		 
		this->qtosDados--;
		return true;
	}

	// Retorna o dado da chave do par�metro, caso n�o exista retorna nullptr
	TipoDado* obter(const TipoKey& key) const
	{
		int pos = this->calcularPosicao(key);
		if (this->isEmpty(pos))
			return nullptr;
		for (int i = 0; i < (this->hashTable + pos)->getTamanho(); i++)
		{
			if ((this->hashTable + pos)->operator[](i).getChave() == key)
				return (this->hashTable + pos)->operator[](i).getPontDado();
		}
		return nullptr;
	}

	// Retorna se o dado existe ou n�o
	bool existe(const TipoKey& key) const
	{
		return this->obter(key) != nullptr;
	}

	// Retorna uma string com os dados da classe
	string toString() const
	{
		string texto = "{";
		for (int i = 0; i < this->length; i++)
		{
			if (!this->isEmpty(i))
				texto += to_string(i) + this->hashTable[i].toString() + ";";
		}
		return texto + "}";
	}

	//-------------------------------------------------------------------------------------------------------------------------------------------------------//
	//--------------------------------------------------------------------OPERATOR---------------------------------------------------------------------------//
	//-------------------------------------------------------------------------------------------------------------------------------------------------------//

	// Printa o conte�do da classe
	friend ostream& operator<<(ostream &sai, const HashTable<TipoKey, TipoDado>& outro)
	{
		return (sai << outro.toString());
	}

	friend class HashTable<TipoKey, TipoDado>;
protected:

	//-------------------------------------------------------------------------------------------------------------------------------------------------------//
	//----------------------------------------------------------------STRUCTS AUXLILIAR----------------------------------------------------------------------//
	//-------------------------------------------------------------------------------------------------------------------------------------------------------//

	class NoHashTable
	{
	public:
		NoHashTable(const TipoKey& novaChave = NULL, const TipoDado& novoDado = NULL)
		{
			chave = new TipoKey(novaChave);
			dado = new TipoDado(novoDado);
		}

		TipoKey getChave() const
		{
			return *this->chave;
		}

		TipoDado getDado() const
		{
			return *this->dado;
		}

		TipoKey* getPontChave() const
		{
			return this->chave;
		}

		TipoDado* getPontDado() const
		{
			return this->dado;
		}

		string toString() const
		{
			return "Key:" + to_string(*this->chave) + " Data: " + to_string(*this->dado);
		}
	private:
		TipoKey* chave;
		TipoDado* dado;
	};

	//--------------------------------------------------------------------------------------------------------------------------------------------------------//
	//---------------------------------------------------------------M�TODOS AUXILIARES-----------------------------------------------------------------------//
	//--------------------------------------------------------------------------------------------------------------------------------------------------------//

	// M�todo protegido que insere o n� da hashtable e verifica se � necess�rio aumentar o tamanho da hashtable, caso a inser��o venha do m�todo aumentarHashTable, n�o ser� preciso verificar
	bool inserir(const TipoKey& key, const TipoDado& dado, const bool& vaiAumenta)
	{
		if (existe(key))
			return false;

		NoHashTable novoNo = NoHashTable(key, dado);
		int pos = this->calcularPosicao(key);

		if (this->isEmpty(pos))
			*(this->hashTable + pos) = ListaDupla<NoHashTable>();
		(this->hashTable + pos)->inserirNoFim(novoNo);

		this->qtosDados++;
		if (vaiAumenta)
			this->precisaAumentar();
		return true;
	}

	// Verifica se precisa aumentar a HashTable
	bool precisaAumentar()
	{
		if (this->qtosDados >= this->qtdMaxDeDados)
			return this->aumentarHashTable();
		else
		{
			for (int i = 0; i < this->length; i++)
			{
				if (!this->isEmpty(i) && (this->hashTable + i)->getTamanho() >= this->tamMaxDasListas)
					return this->aumentarHashTable();
			}
		}

		if (ehRadical)
		{
			int tamMin = this->tamMaxDasListas+1;
			int tamMax = 0;
			
			int atual = -1;
			int menorValor = this->length+1;
			int qtosPercorreu = 0;

			for (int i = 0; i < this->length; i++)
			{
				if (!this->isEmpty(i) && (this->hashTable + i)->getTamanho() < tamMin)
					tamMin = (this->hashTable + i)->getTamanho();

				if (!this->isEmpty(i) && (this->hashTable + i)->getTamanho() > tamMax)
					tamMax = (this->hashTable + i)->getTamanho();
			
				if (!this->isEmpty(i + 1) && ((i + 1) - atual) < menorValor && qtosPercorreu != this->qtosDados)
				{
					if (atual != -1)
						menorValor = (i + 1) - atual;
					qtosPercorreu++;
					atual = (i + 1);
				}
			}

			if (tamMax - tamMin >= this->diferencaDeTam || (this->qtosDados > 1 && menorValor < this->diferencaDePos))
				return this->aumentarHashTable();
		}
		return false;
	}

	// Aumentar a HashTable ou na forma padr�o ou na forma personalizada, dependendo da vari�vel ehPadrao e ehRadical
	bool aumentarHashTable()
	{
		cout << "BOT>Desbalaceou a HASHTABLE" << endl;
		int novoLength = 0;
		HashTable<TipoKey, TipoDado>* aux = nullptr;
		if (this->ehPadrao)
		{
			if (this->ehRadical)
				aux = new HashTable<TipoKey, TipoDado>(true, this->fatorial(++this->taxaDeCrescimento), this->taxaDeCrescimento, (this->qtdMaxDeDados * 100) / this->length, this->tamMaxDasListas, this->operadorDeCrescimento, this->diferencaDeTam, this->diferencaDePos);
			else
				aux = new HashTable<TipoKey, TipoDado>(this->fatorial(++this->taxaDeCrescimento), this->taxaDeCrescimento, (this->qtdMaxDeDados * 100) / this->length, this->tamMaxDasListas, this->operadorDeCrescimento);
			aux->ehPadrao = true;
		}
		else
		{
			if (this->operadorDeCrescimento == '+')
				novoLength = this->length + this->taxaDeCrescimento;
			else
				novoLength = this->length * this->taxaDeCrescimento;

			if (ehRadical)
				aux = new HashTable<TipoKey, TipoDado>(true, novoLength, this->taxaDeCrescimento, (this->qtdMaxDeDados * 100) / this->length, this->tamMaxDasListas, this->operadorDeCrescimento, this->diferencaDeTam, this->diferencaDePos);
			else
				aux = new HashTable<TipoKey, TipoDado>(novoLength, this->taxaDeCrescimento, (this->qtdMaxDeDados * 100) / this->length, this->tamMaxDasListas, this->operadorDeCrescimento);
		}

		for (int i = 0; i < this->length; i++)
		{
			if (!this->isEmpty(i))
				this->transfereDados(*(this->hashTable + i), *aux);
		}

		*this = HashTable<TipoKey, TipoDado>(*aux);
		cout << "BOT>HASHTABLE alterada --> Tamanho: " << this->length << "; Quantidade maxima de dados: " << this->qtdMaxDeDados << "; Tamanho maximo das listas: " << this->tamMaxDasListas << "; " << endl;
		if (this->ehRadical)
			cout << "Diferenca de tamanho: " << this->diferencaDeTam << "; Diferenca de Posicao: " << this->diferencaDePos << ";" << endl;
		cout << endl;
		return true;
	}

	// Transfere dados de uma lista para uma HashTable
	void transfereDados(const ListaDupla<NoHashTable>& lista, HashTable<TipoKey, TipoDado>& hash)
	{
		for (int i = 0; i < lista.getTamanho(); i++)
			hash.inserir(lista.operator[](i).getChave(), lista.operator[](i).getDado(), false);
	}

	// M�todo que calcula a posi��o do dado com a sua respectiva chave
	int calcularPosicao(const TipoKey& key) const
	{
		hash<TipoKey> hashCode;
		return hashCode(key) % this->length;
	}

	// Retorna o fatorial de um n�mero
	int fatorial(const int& numero)
	{
		int fact = 1;
		for (int counter = 1; counter <= numero; counter++)
			fact *= counter;
		return fact;
	}

	// Verifica se a lista de uma determinada posi��o est� vaiza
	bool isEmpty(const int& pos) const
	{
		return (this->hashTable + pos) == nullptr || (this->hashTable + pos)->isEmpty();
	}
private:
	//-------------------------------------------------------------------------------------------------------------------------------------------------------//
	//-------------------------------------------------------------------ATRIBUTOS---------------------------------------------------------------------------//
	//-------------------------------------------------------------------------------------------------------------------------------------------------------//

	ListaDupla<NoHashTable>* hashTable;
	unsigned int qtosNos;
	unsigned int length;
	unsigned int taxaDeCrescimento;
	unsigned int tamMaxDasListas;

	float qtdMaxDeDados;
	char operadorDeCrescimento;
	bool ehPadrao;

	bool ehRadical;
	unsigned int diferencaDeTam;
	unsigned int diferencaDePos;
};