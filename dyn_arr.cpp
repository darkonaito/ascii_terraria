template <class T>
const uint8_t DynArr<T>::minAllocate = 4;

template <class T>
DynArr<T>::DynArr(uint16_t startSize):
    allocated(0), length(0)
{
    elements = new T[allocated = ((startSize / minAllocate) + 1) * minAllocate];
}

template <class T>
DynArr<T>::DynArr(const DynArr& dynArr):
    allocated(dynArr.allocated), length(dynArr.length)
{
    elements = new T[allocated];

    copyElements(elements, dynArr.elements);
}

template <class T>
DynArr<T>& DynArr<T>::operator=(const DynArr& dynArr)
{
    delete[] elements;

    allocated = dynArr.allocated;
    length    = dynArr.length;

    elements = new T[allocated];

    copyElements(elements, dynArr.elements);

    return *this;
}

template <class T>
DynArr<T>::~DynArr()
{
    if(length > 0) delete [] elements;
}

template <class T>
T& DynArr<T>::append(const T& t)
{
    if(length + 1 > allocated && allocated + minAllocate <= std::numeric_limits<int16_t>::max())
    {
        transfer(new T[allocated += minAllocate]);
    }

    return elements[length++] = t;
}

template <class T>
T& DynArr<T>::append(const T&& t)
{
    if(length + 1 > allocated && allocated + minAllocate <= std::numeric_limits<int16_t>::max())
    {
        transfer(new T[allocated += minAllocate]);
    }

    return elements[length++] = t;
}

template <class T>
bool DynArr<T>::kickoutLast()
{
    if(length - 1 > 0)
    {
        --length;

        if(allocated - length == minAllocate)
        {
            transfer(new T[allocated -= minAllocate]);
        }

        return true;
    } else if(length - 1 == 0)
    {
        delete [] elements;

        length = allocated = 0;

        return true;
    } else
    {
        return false;
    }
}

template <class T>
T& DynArr<T>::operator[](uint16_t i)
{
    if(i < length)
        return elements[i];
    else return elements[length - 1];
}

template <class T>
const T& DynArr<T>::operator[](uint16_t i) const
{
    if(i < length)
        return elements[i];
    else return elements[length - 1];
}

template <class T>
T& DynArr<T>::insert(const T& t, uint16_t in)
{
    if(length + 1 > allocated && allocated + minAllocate <= std::numeric_limits<int16_t>::max())
    {
        transfer(new T[allocated += minAllocate]);
    }

    for(uint16_t i = length; i > in; --i)
    {
        elements[i] = elements[i - 1];
    }

    ++length;

    return elements[in] = t;
}

template <class T>
bool DynArr<T>::remove(uint16_t rm)
{
    if(length - 1 > 0)
    {
        --length;

        for(uint16_t i = rm; i < length; ++i)
        {
            elements[i] = elements[i + 1];
        }

        if(allocated - length == minAllocate)
        {
            transfer(new T[allocated -= minAllocate]);
        }

        return true;
    } else if(length - 1 == 0)
    {
        delete [] elements;

        length = allocated = 0;

        return true;
    } else
    {
        return false;
    }
}

template <class T>
void DynArr<T>::copyElements(T* e1, T* e2)
{
    for(uint16_t i = 0; i < length; ++i)
    {
        e1[i] = e2[i];
    }
}

template <class T>
void DynArr<T>::transfer(T* newE)
{
    copyElements(newE, elements);

    delete[] elements;
    elements = newE;
}
