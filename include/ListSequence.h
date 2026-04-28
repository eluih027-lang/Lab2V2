#ifndef LR2_LIST_SEQUENCE_H
#define LR2_LIST_SEQUENCE_H

#include "LinkedList.h"
#include "Sequence.h"

template <class T>
class ListSequence : public Sequence<T> {
public:
    ListSequence() : list_() {}
    ListSequence(const T* items, int count) : list_(items, count) {}
    explicit ListSequence(const LinkedList<T>& list) : list_(list) {}
    ListSequence(const ListSequence<T>& other) : list_(other.list_) {}
    ~ListSequence() override = default;

    const T& GetFirst() const override { return list_.GetFirst(); }
    const T& GetLast() const override { return list_.GetLast(); }
    const T& Get(int index) const override { return list_.Get(index); }

    Sequence<T>* GetSubsequence(int startIndex, int endIndex) const override {
        LinkedList<T> sub = list_.GetSubList(startIndex, endIndex);
        ListSequence<T>* result = CloneConcrete();
        result->list_ = sub;
        return result;
    }

    int GetLength() const override { return list_.GetLength(); }

    Sequence<T>* Append(const T& item) override {
        ListSequence<T>* target = MutationTarget();
        target->list_.Append(item);
        return target;
    }

    Sequence<T>* Prepend(const T& item) override {
        ListSequence<T>* target = MutationTarget();
        target->list_.Prepend(item);
        return target;
    }

    Sequence<T>* InsertAt(const T& item, int index) override {
        ListSequence<T>* target = MutationTarget();
        target->list_.InsertAt(item, index);
        return target;
    }

    Sequence<T>* Concat(const Sequence<T>& list) const override {
        ListSequence<T>* result = CloneConcrete();
        list.ForEach([&](const T& value) {
            result->list_.Append(value);
        });
        return result;
    }

    Sequence<T>* Clone() const override {
        return CloneConcrete();
    }

    Sequence<T>* RemoveAt(int index) {
        ListSequence<T>* target = MutationTarget();
        target->list_.RemoveAt(index);
        return target;
    }

protected:
    LinkedList<T> list_;

    Sequence<T>* CreateEmptySameKind() const override {
        ListSequence<T>* result = CloneConcrete();
        result->list_ = LinkedList<T>();
        return result;
    }

    void ForEach(const std::function<void(const T&)>& action) const override {
        for (auto it = list_.cbegin(); it != list_.cend(); ++it) {
            action(*it);
        }
    }

    virtual ListSequence<T>* MutationTarget() = 0;
    virtual ListSequence<T>* CloneConcrete() const = 0;
};

#endif
