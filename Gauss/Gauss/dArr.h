#pragma once

template<typename T>
class dArr {
private:
	unsigned capactity;
	unsigned numOfElements;
	T* *arr;

	void _initialize(unsigned from);
	void _expand();
	
public:
	dArr(unsigned size = 5);
	~dArr();

	T& operator[](unsigned index);

	void Add(const T element);
	void Remove(unsigned index);
	unsigned GetSize() const;
	void Clear();
};

template<typename T>
dArr<T>::dArr(unsigned capactity) {
	this->capactity = capactity;
	this->numOfElements = 0;

	this->arr = new T*[this->capactity];

	this->_initialize(0);
}

template<typename T>
dArr<T>::~dArr() {
	for (size_t i = 0; i < this->numOfElements; i++)
	{
		delete this->arr[i];
	}
	delete this->arr;
}

template<typename T>
void dArr<T>::Add(const T element) {
	if (this->numOfElements >= this->capactity) {
		this->_expand();
	}
	this->arr[this->numOfElements++] = new T(element);
}

template<typename T>
void dArr<T>::Remove(unsigned index) {
	if (index < 0 || index >= this->numOfElements) {
		throw("OutOfBoundsException");
	}
	delete this->arr[index];
	for (size_t i = index; i < this->numOfElements - 1; i++)
	{
		this->arr[i] = this->arr[i + 1];
	}
	this->arr[this->numOfElements--] = nullptr;
}

template<typename T>
unsigned dArr<T>::GetSize() const {
	return this->numOfElements;
}

template<typename T>
void dArr<T>::Clear() {
	for (size_t i = 0; i < this->numOfElements; i++)
	{
		delete this->arr[i];
	}
	
	this->_initialize(0);

	this->numOfElements = 0;
}

template<typename T>
T& dArr<T>::operator[](unsigned index) {
	if (index < 0 || index >= this->numOfElements) {
		throw("OutOfBoundsException");
	}
	return *this->arr[index];
}

template<typename T>
void dArr<T>::_initialize(unsigned from) {
	for (size_t i = from; i < this->capactity; i++)
	{
		this->arr[i] = nullptr;
	}
}

template<typename T>
void dArr<T>::_expand() {
	this->capactity *= 2;
	
	T* *tempArr = new T*[this->capactity];
	for (size_t i = 0; i < this->numOfElements; i++)
	{
		tempArr[i] = this->arr[i];
	}

	delete[] this->arr;
	this->arr = tempArr;

	this->_initialize(this->numOfElements);
}
