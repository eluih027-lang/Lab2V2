#ifndef LR2_MUTABLE_LIST_SEQUENCE_H
#define LR2_MUTABLE_LIST_SEQUENCE_H

#include "ListSequence.h"

template <class T>
class MutableListSequence : public ListSequence<T>, private SequenceFactoryMixin<T, MutableListSequence> {
public:
    using ListSequence<T>::ListSequence;

protected:
    ListSequence<T>* MutationTarget() override { return this; }
    Sequence<T>* CreateEmptySameKind() const override { return this->CreateNew<T>(); }
    MutableListSequence<T>* CloneConcrete() const override { return this->CloneSelf(*this); }
};

#endif
