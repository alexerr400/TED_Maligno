#pragma once
using namespace std;
#include <iostream>
#include <ostream>
#include <string>
#include "ListaDupla.h"
#include <math.h>
#include <xhash>

template <class TipoKey, class TipoDado>

class HashTable
{
public:
	class NoHashTable 
	{
		friend class HashTable<TipoKey, TipoDado>;
		friend class NoHashTable;
	public:
		NoHashTable(const TipoKey& novaChave = NULL, const TipoDado& novoDado = NULL)
		{
			chave = novaChave;
			dado = novoDado;
		}

		TipoKey getChave() const
		{
			return this->chave;
		}

		TipoDado getDado() const
		{
			return this->dado;
		}

		string toString() const
		{
			return "Key:" + to_string(this->chave) + " Data: " + to_string(this->dado);
		}
	private:
		TipoKey chave;
		TipoDado dado;
	};
	//-------------------------------------------------------------------------------------------------------------------------------------------------------//
	//-------------------------------------------------------------CONSTRUTORES/DESTRUTOR--------------------------------------------------------------------//
	//-------------------------------------------------------------------------------------------------------------------------------------------------------//

	HashTable(const float& _novoLength = 6, const int& _novaTaxaDeCrescimento = 3, const float& _novaTaxaDeOcupacao = 50, const int& _novoTamanhoMaximoLista = 3, const bool& ehPadrao = true) : 
			  size(0), ehPadrao(ehPadrao), length(_novoLength), taxaDeCrescimento(_novaTaxaDeCrescimento),
			  tamanhoMaximoDasListas(_novoTamanhoMaximoLista), quantidadeMaximaDeListas(_novoLength * (_novaTaxaDeOcupacao / 100.0)), taxaMaximaDeOcupacao(_novoLength * (_novaTaxaDeOcupacao / 100.0))
	{
		this->hashTable = (ListaDupla<NoHashTable>*)malloc(this->length * sizeof(ListaDupla<NoHashTable>));
		for (int i = 0; i < length; i++)
			this->hashTable[i] = ListaDupla<NoHashTable>();
	}

	//-------------------------------------------------------------------------------------------------------------------------------------------------------//
	//---------------------------------------------------------------M�TODOS PRINCIPAIS-----------------------------------------------------------------------//
	//-------------------------------------------------------------------------------------------------------------------------------------------------------//

	// retorna se incluiu com sucesso ou n�o
	bool inserir(const TipoKey& key, const TipoDado& dado)
	{
		NoHashTable novoNo = NoHashTable(key, dado);
		int pos = this->calcularPosicao(key);
		(this->hashTable + pos)->inserirNoFim(novoNo);

		this->size++;
		this->balancear();
		return true;
	}

	// retorna o dado removido
	bool deletar(const TipoKey& key)
	{
		int pos = this->calcularPosicao(key);
		if (!this->isEmpty(pos))
		{
			if ((this->hashTable + pos)->getTamanho() == 1)
				(this->hashTable + pos)->removerNoFim();
			else
			{
				for (int i = 0; i < (this->hashTable + pos)->getTamanho(); i++)
				{
					if ((this->hashTable + pos)->operator[](i).chave == key)
						(this->hashTable + pos)->removerPos(i);
				}
			}
		}
		else
			return false;
		 
		this->size--;
		return true;
	}

	// devolve o dado da chave do par�metro, caso n�o exista retorna nullptr
	TipoDado obter(const TipoKey& key) const
	{
		int pos = this->calcularPosicao(key);
		for (int i = 0; i < length; i++)
		{
			if ((this->hashTable + pos)->operator[](i).chave == key)
				return (this->hashTable + pos)->operator[](i).dado;
		}
		return NULL;
	}

	// retorna se o dado existe ou n�o
	bool existe(const TipoKey& key) const
	{
		return this->obter(key) != NULL;
	}

	friend ostream& operator<<(ostream &sai, const HashTable<TipoKey, TipoDado>& outro)
	{
		return (sai << outro.toString());
	}

	string toString() const
	{
		string texto = "{\n";
		for (int i = 0; i < this->length; i++)
		{
			if (!this->isEmpty(i))
				texto += to_string(i) + this->hashTable[i].toString() + ";\n";
		}
		return texto + "}";
	}

	friend class HashTable<TipoKey, TipoDado>;
protected:

	ListaDupla<NoHashTable>* getHashTable()
	{
		return this->hashTable;
	}

	bool balancear()
	{
		bool qtasListasComTamanhoEmExcesso = false;
		int qtasListas = 0;
		int qtosNos = 0;

		for (int i = 0; i < this->length; i++)
		{
			if (!this->isEmpty(i))
			{
				qtosNos++;
				if (this->hashTable[i].getTamanho() > 1)
				{
					qtasListas++;
					if (this->hashTable[i].getTamanho() > this->tamanhoMaximoDasListas)
						qtasListasComTamanhoEmExcesso = true;
				}
			}
		}

		if (qtasListasComTamanhoEmExcesso || qtasListas >= this->quantidadeMaximaDeListas || qtosNos >= this->taxaMaximaDeOcupacao)
			return this->aumentarHashTable();
			
		return false;
	}

	bool aumentarHashTable()
	{
		if (this->ehPadrao)
		{
			int novoLength = this->fatorial(++this->taxaDeCrescimento);
			HashTable<TipoKey, TipoDado> aux = HashTable<TipoKey, TipoDado>(novoLength, this->taxaDeCrescimento);

			for (int i = 0; i < this->length; i++)
			{
				if (!this->isEmpty(i))
				{
					(this->hashTable + i)->iniciarPercursoSequencial();
					while ((this->hashTable + i)->podePercorrer())
						aux.inserir((this->hashTable + i)->getAtual()->getDado()->chave, (this->hashTable + i)->getAtual()->getDado()->dado);
				}
			}
			this->length = novoLength;
			this->quantidadeMaximaDeListas = aux.quantidadeMaximaDeListas;
			this->taxaMaximaDeOcupacao = aux.taxaMaximaDeOcupacao;
			this->hashTable = aux.hashTable;
			return true;
		}
	}

	int calcularPosicao(const TipoKey& key) const
	{
		hash<TipoDado> hashCode;
		return hashCode(key) % this->length;
	}

	int fatorial(const int& numero)
	{
		int fact = 1;
		for (int counter = 1; counter <= numero; counter++)
			fact *= counter;
		return fact;
	}

	bool isEmpty(const int& pos) const
	{
		return (this->hashTable + pos)->isEmpty();
	}

private:

	//-------------------------------------------------------------------------------------------------------------------------------------------------------//
	//-------------------------------------------------------------------ATRIBUTOS---------------------------------------------------------------------------//
	//-------------------------------------------------------------------------------------------------------------------------------------------------------//

	ListaDupla<NoHashTable>* hashTable;
	int size;
	int length;
	int taxaDeCrescimento;
	int tamanhoMaximoDasListas;
	int quantidadeMaximaDeListas;

	float taxaMaximaDeOcupacao;
	bool ehPadrao;

	friend class NoHashTable;
};