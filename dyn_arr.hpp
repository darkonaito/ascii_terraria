#ifndef DYN_ARR_H_INCLUDED
#define DYN_ARR_H_INCLUDED

template <class T>

class DynArr
{
    public:
        DynArr(uint16_t startSize = 0);
        DynArr(const DynArr&);
        ~DynArr();

        DynArr& operator=(const DynArr&);

        T& operator[](uint16_t);
        const T& operator[](uint16_t) const;

        T& getLast() {return elements[length - 1];}
        const T& getLast() const {return elements[length - 1];}

        T& append(const T&);
        T& append(const T&&);
        T& insert(const T&, uint16_t);

        bool kickoutLast();
        bool remove(uint16_t);

        uint16_t getLength() const {return length;}
        uint16_t getAllocated() const {return allocated;}

    private:
        static const uint8_t minAllocate;

        uint16_t allocated;
        uint16_t length;

        T* elements;

        void copyElements(T*, T*);
        void transfer(T*);
};


#include "dyn_arr.cpp"

#endif // DYN_ARR_H_INCLUDED
