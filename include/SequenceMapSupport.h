#ifndef LR2_SEQUENCE_MAP_SUPPORT_H
#define LR2_SEQUENCE_MAP_SUPPORT_H

#include "MutableArraySequence.h"

template <class T>
template <class TResult>
Sequence<TResult>* Sequence<T>::CreateMappedEmpty() const {
    return new MutableArraySequence<TResult>();
}

#endif
