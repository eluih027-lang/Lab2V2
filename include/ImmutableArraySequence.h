#ifndef LR2_IMMUTABLE_ARRAY_SEQUENCE_H
#define LR2_IMMUTABLE_ARRAY_SEQUENCE_H

#include "ArraySequence.h"

template <class T>
class ImmutableArraySequence : public ArraySequence<T>, private SequenceFactoryMixin<T, ImmutableArraySequence> {
public:
    using ArraySequence<T>::ArraySequence;

protected:
    ArraySequence<T>* MutationTarget() override { return new ImmutableArraySequence<T>(*this); }
    Sequence<T>* CreateEmptySameKind() const override { return this->CreateNew<T>(); }
    ImmutableArraySequence<T>* CloneConcrete() const override { return this->CloneSelf(*this); }
};

#endif
