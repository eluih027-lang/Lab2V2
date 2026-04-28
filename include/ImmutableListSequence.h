#ifndef LR2_IMMUTABLE_LIST_SEQUENCE_H
#define LR2_IMMUTABLE_LIST_SEQUENCE_H

#include "ListSequence.h"

template <class T>
class ImmutableListSequence : public ListSequence<T>, private SequenceFactoryMixin<T, ImmutableListSequence> {
public:
    using ListSequence<T>::ListSequence;

protected:
    ListSequence<T>* MutationTarget() override { return new ImmutableListSequence<T>(*this); }
    Sequence<T>* CreateEmptySameKind() const override { return this->CreateNew<T>(); }
    ImmutableListSequence<T>* CloneConcrete() const override { return this->CloneSelf(*this); }
};

#endif
