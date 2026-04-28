#ifndef LR2_MUTABLE_ARRAY_SEQUENCE_H
#define LR2_MUTABLE_ARRAY_SEQUENCE_H

#include "ArraySequence.h"

template <class T>
class MutableArraySequence : public ArraySequence<T>, private SequenceFactoryMixin<T, MutableArraySequence> {
public:
    using ArraySequence<T>::ArraySequence;

protected:
    ArraySequence<T>* MutationTarget() override { return this; }
    Sequence<T>* CreateEmptySameKind() const override { return this->CreateNew<T>(); }
    MutableArraySequence<T>* CloneConcrete() const override { return this->CloneSelf(*this); }
};

#endif
