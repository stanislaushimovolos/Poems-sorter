#include <iostream>
#include <algorithm>

#pragma once

#define MAGIC_CANARY_NUMBER 66
#define REALLOCATION_FACTOR 2
#define START_VECTOR_SZ 5

enum
{
    BAD_INDEX,
    POP_ERROR,
    CONSTRUCT_ERROR,
    REALLOCATION_WARNING
};

const char *const vector_errList[] = {
        "is non-existent elem",
        "Warning, vector is underflow ",
        "Creation error, couldn't initialize the array",
        "Warning, vector was reallocated"
};

template<typename Data_T>
class vector
{
private:

    int _canary1;

    size_t _capacity;
    size_t _size;
    Data_T *_data;

    int *_canaryPtr3;
    int *_canaryPtr4;

    int _canary3;
    int _canary4;
    int _canary2;

    bool vectorCondition;

    int CalcHash();

    void dump(int error);

    bool printElem(int elemNum, std::ostream &out);

public:

    bool Alloc();

    //конструктр по умолчанию
    explicit vector(size_t _size = START_VECTOR_SZ);

    vector(const vector<Data_T> &that);

    vector(size_t _size, const Data_T &extender);

    vector(vector<Data_T> &&that) noexcept;

    vector<Data_T> &operator=(vector &&that) noexcept;

    void swap(vector<Data_T> &that);

    bool vectorOK();

    void dump();

    void push_back(const Data_T &value);

    void pop_back(Data_T *value = NULL);

    Data_T &operator[](int pos);

    vector<Data_T> &operator=(const vector &that);

    size_t getSize();

    ~vector();
};


template<typename Data_T>
vector<Data_T>::vector(vector &&that) noexcept :

        _canary1(that._canary1),
        _canary2(that._canary2),
        _canaryPtr3((int *) (_data - 1)),
        _canaryPtr4((int *) ((char *) (_data - 1) + (that._capacity + 1) * sizeof(Data_T))),
        _canary3(*_canaryPtr3),
        _canary4(*_canaryPtr4),

        _size(that._size),
        _capacity(that._capacity),
        vectorCondition(true),

        _data(that._data)
{
    std::cout << "Hello";
    that._data = nullptr;
}


template<typename T>
vector<T>::vector(size_t _size) :

        _canary1(MAGIC_CANARY_NUMBER),
        _canary2(MAGIC_CANARY_NUMBER),
        _canaryPtr3((int *) _data),
        _canaryPtr4((int *) ((char *) _data + (_size + 1) * sizeof(T))),
        _canary3(*_canaryPtr3),
        _canary4(*_canaryPtr4),

        _size(0),
        _capacity(_size),
        vectorCondition(true),

        _data(new T[_size + 2])
{
    if (!_data)
        dump(CONSTRUCT_ERROR);

    _data++;
}

template<typename Data_T>
vector<Data_T>::vector(const vector<Data_T> &that):

        _canary1(that._canary1),
        _canary2(that._canary2),
        _canaryPtr3((int *) (_data - 1)),
        _canaryPtr4((int *) ((char *) (_data - 1) + (that._capacity + 1) * sizeof(Data_T))),
        _canary3(*_canaryPtr3),
        _canary4(*_canaryPtr4),

        _size(that._size),
        _capacity(that._capacity),
        vectorCondition(true),

        _data(new Data_T[that._capacity + 2])
{
    if (!_data)
        dump(CONSTRUCT_ERROR);

    _data++;

    for (int i = 0; i < _capacity; i++)
        this->_data[i] = that._data[i];
}


template<typename Data_T>
vector<Data_T>::vector(size_t _size, const Data_T &extender) :

        _canary1(MAGIC_CANARY_NUMBER),
        _canary2(MAGIC_CANARY_NUMBER),
        _canaryPtr3((int *) _data),
        _canaryPtr4((int *) ((char *) _data + (_size + 1) * sizeof(Data_T))),
        _canary3(*_canaryPtr3),
        _canary4(*_canaryPtr4),

        _size(0),
        _capacity(_size),
        vectorCondition(true),

        _data(new Data_T[_size + 2])
{
    if (!_data)
        dump(CONSTRUCT_ERROR);

    _data++;
    for (int i = 0; i < _capacity; i++)
        this->_data[i] = extender;

    _size = _capacity;
}


template<typename Data_T>
void vector<Data_T>::swap(vector<Data_T> &that)
{
    std::swap(_size, that._size);
    std::swap(_capacity, that._capacity);
    std::swap(_canary1, that._canary1);
    std::swap(_canary2, that._canary2);
    std::swap(_canary3, that._canary3);
    std::swap(_canary4, that._canary4);
    std::swap(_canaryPtr4, that._canaryPtr4);
    std::swap(_canaryPtr3, that._canaryPtr3);
    std::swap(_data, that._data);
}


template<typename Data_T>
vector<Data_T> &vector<Data_T>::operator=(const vector<Data_T> &that)
{
    vector<Data_T> temp(that);
    temp.swap(*this);
}

template<typename Data_T>
vector<Data_T> &vector<Data_T>::operator=(vector<Data_T> &&that) noexcept
{
    _canary1 = that._canary1;
    _canary2 = that._canary2;
    _canary3 = that._canary3;
    _canary4 = that._canary4;
    _canaryPtr3 = that._canaryPtr3;
    _canaryPtr4 = that._canaryPtr4;
    _size = that._size;
    _capacity = that._capacity;
    vectorCondition = true;

    Data_T *l_Data = that._data;
    that._data = _data;
    _data = l_Data;
}

template<typename Data_T>
Data_T &vector<Data_T>::operator[](int pos)
{
    try {
        if (pos >= _capacity)
            throw 100500;

        if (pos > _size)
            _size = (size_t) pos;

        return _data[pos];
    }
    catch (int errNum) {
        std::cerr << "ExepLst " << errNum << " out of range " << std::endl;
        std::cerr << "[" << pos << "] ";
        dump(BAD_INDEX);
    }
}

template<typename Data_T>
bool vector<Data_T>::printElem(int elemNum, std::ostream &out)
{
    return false;
}

#define printDef(elem_t)                                        \
template<>                                                      \
bool vector<elem_t>::printElem(int elemNum, std::ostream &out)  \
{                                                               \
    if (elemNum < _capacity) {                                   \
        out << _data[elemNum];                                   \
        return true;                                            \
    }                                                           \
    return false;                                               \
}                                                               \


printDef(bool)

printDef(short)

printDef(long long)

printDef(int)

printDef(size_t)

printDef(double)

printDef(char)

printDef(void*)

printDef(long double)

printDef(float)

printDef(unsigned int)

#undef printDef


template<typename T>
bool vector<T>::Alloc()
{
    try {
        auto *Alloc_Data = new T[_capacity * REALLOCATION_FACTOR + 2];
        for (int i = 0; i < _capacity; i++)
            Alloc_Data[i + 1] = _data[i];

        --_data;
        delete[] _data;

        _data = Alloc_Data;

        _capacity = _capacity * REALLOCATION_FACTOR;
        _canaryPtr3 = ((int *) _data);
        _canaryPtr4 = ((int *) ((char *) _data + (_capacity + 1) * sizeof(T)));
        _data++;
    }
    catch (std::bad_alloc &c) {
        return false;
    }
    return true;

}


template<typename T>
void vector<T>::pop_back(T *value)
{
    if (_size > 0) {
        if (value)
            *value = _data[--_size];
        else {
            _data[--_size] = {0};
        }
    } else {
        vectorCondition = false;
        dump(POP_ERROR);
    }
}


template<typename T>
void vector<T>::push_back(const T &value)
{
    if (_size == _capacity) {
        Alloc();
        vectorCondition = false;
        dump(REALLOCATION_WARNING);
    }
    if (vectorCondition) {
        _data[_size++] = value;
    }
}


#define CUR_BYTE (((unsigned char *) _data - sizeof(T))[i])

template<typename T>
int vector<T>::CalcHash()
{
    int l_hash = 0;

    for (int i = 0; i < sizeof(T) * (_size + 1); i++) {
        if (i & 1)
            l_hash += (int) CUR_BYTE;
        else l_hash += 2 * (int) CUR_BYTE;
    }
    l_hash += _size;
    return l_hash;
}

#undef CUR_BYTE


template<typename T>
bool vector<T>::vectorOK()
{
    if (_canary1 != MAGIC_CANARY_NUMBER ||
        _canary2 != MAGIC_CANARY_NUMBER ||
        _canary3 != *_canaryPtr3 ||
        _canary4 != *_canaryPtr4) {

        vectorCondition = false;
        return false;
    }
    return vectorCondition;
}

#undef MAGIC_CANARY_NUMBER


template<typename T>
void vector<T>::dump(int error)
{
    std::cerr << vector_errList[error] << std::endl;
    std::cerr << "Max size of vector is " << _capacity << std::endl;
    std::cerr << "Size of full part is  " << _size << std::endl;

    int counter = 0;
    while (printElem(counter++, std::cerr))
        std::cerr << std::endl;
}

template<typename T>
void vector<T>::dump()
{
    int counter = 0;
    while (printElem(counter++, std::cout))
        std::cout << std::endl;
}


template<typename T>
size_t vector<T>::getSize()
{
    return _capacity;
}

template<typename T>
vector<T>::~vector()
{
    if (_data) {
        --_data;
        delete[] _data;
    }
}
