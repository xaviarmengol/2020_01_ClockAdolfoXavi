
#include<cstdlib>
#include "VectorArd.h"

VectorArd::VectorArd(){
    for (int i=0;i<SIZE_MAX_VECT;i++) _values[i]=0;
    _size=0;
}

bool VectorArd::push_back(int value){
    bool full = (_size == SIZE_MAX_VECT);
    if (full) return false;
    else {
        _values[_size]=value;
        _size++;
        return true;
    }
}

int VectorArd::size(){
    return _size;
}


int &VectorArd::operator[](int index) {
    if (index >= _size)
    {
        exit(0);
    }
    return _values[index];
}

