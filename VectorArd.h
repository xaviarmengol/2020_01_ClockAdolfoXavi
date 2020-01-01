#ifndef SIZE_MAX_H
#define SIZE_MAX_H

#define SIZE_MAX_VECT 20

#endif // SIZE_MAX_H


#ifndef VECTORARD_H
#define VECTORARD_H


class VectorArd
{
    public:
        VectorArd();
        bool push_back(int value);
        int size();
        int &operator[](int index);

    private:
        int _size;
        int _values[SIZE_MAX_VECT];
};

#endif // VECTORARD_H
