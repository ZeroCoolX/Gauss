#pragma once

#include "GameObject.h"
#include "Tile.h"

template<typename T>
class TileArr
{
private:
	unsigned max;
	unsigned numOfTiles;
	T* *tiles;

	void _initialize(unsigned from);

public:
	TileArr(unsigned max);
	TileArr(const TileArr& obj);
	virtual ~TileArr();

	T& operator[](const unsigned index);
	void operator=(const TileArr& obj);

	inline unsigned Size() const { return this->max; }
	inline unsigned NumberOfTiles() const { return this->numOfTiles; }

	bool IsNull(const unsigned index);
	void Add(const T tile, const unsigned index);
	void Remove(const unsigned index);
};

template<typename T>
TileArr<T>::TileArr(unsigned max)
{
	this->max = max;
	this->numOfTiles = 0;

	this->tiles = new T*[this->max];

	this->_initialize(0);
}

template<typename T>
TileArr<T>::TileArr(const TileArr& obj) {
	this->max = obj.max;
	this->numOfTiles = obj.numOfTiles;

	this->tiles = new Tile*[this->max];

	for (size_t i = 0; i < this->max; i++)
	{
		if (obj.tiles[i] != nullptr) {
			this->tiles[i] = new T(*obj.tiles[i]);
		}
		else {
			this->tiles[i] = nullptr;
		}
	}
}

template<typename T>
TileArr<T>::~TileArr()
{
	for (size_t i = 0; i < this->max; i++)
	{
		delete this->tiles[i];
	}
	delete[] this->tiles;
}

template<typename T>
T& TileArr<T>::operator[](const unsigned index) {
	if (index < 0 || index >= this->max) {
		throw "ERROR: Out of bounds TileArr::operator[]";
	}

	// RETURNS POINTER
	return *this->tiles[index];
}

template<typename T>
void TileArr<T>::operator=(const TileArr& obj) {
	for (size_t i = 0; i < this->max; i++)
	{
		delete this->tiles[i];
	}
	delete[] this->tiles;

	this->max = obj.max;
	this->numOfTiles = obj.numOfTiles;

	this->tiles = new T*[this->max];

	for (size_t i = 0; i < this->max; i++)
	{
		if (obj.tiles[i] != nullptr) {
			this->tiles[i] = new T(*obj.tiles[i]);
		}
		else {
			this->tiles[i] = nullptr;
		}
	}
}

template<typename T>
bool TileArr<T>::IsNull(const unsigned index) {
	if (index < 0 || index >= this->max) {
		throw "ERROR: Out of bounds TileArr::IsNull";
	}
	return this->tiles[index] == nullptr;
}

template<typename T>
void TileArr<T>::Add(const T tile, const unsigned index) {
	if (index < 0 || index >= this->max) {
		throw "ERROR: Out of bounds TileArr::Add";
	}
	this->tiles[index] = new T(tile);
	this->numOfTiles++;
}

template<typename T>
void TileArr<T>::Remove(const unsigned index) {
	if (index < 0 || index >= this->max) {
		throw "ERROR: Out of bounds TileArr::Remove";
	}
	delete this->tiles[index];
	this->tiles[index] = nullptr;
	this->numOfTiles--;
}

template<typename T>
void TileArr<T>::_initialize(unsigned from) {
	for (size_t i = from; i < this->max; i++)
	{
		this->tiles[i] = nullptr;
	}
}

// Resize

// ResizeClear