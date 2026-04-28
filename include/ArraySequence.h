#ifndef LR2_ARRAY_SEQUENCE_H
#define LR2_ARRAY_SEQUENCE_H

#include <stdexcept>
#include "DynamicArray.h"
#include "Sequence.h"

template <class T>
class ArraySequence : public Sequence<T> {
public:
    ArraySequence() : items_() {}
    ArraySequence(const T* items, int count) : items_(items, count) {}
    explicit ArraySequence(int size) : items_(size) {}
    ArraySequence(const ArraySequence<T>& other) : items_(other.items_) {}
    ~ArraySequence() override = default;

    const T& GetFirst() const override {
        if (GetLength() == 0) {
            throw std::out_of_range("sequence is empty");
        }
        return items_.Get(0);
    }

    const T& GetLast() const override {
        if (GetLength() == 0) {
            throw std::out_of_range("sequence is empty");
        }
        return items_.Get(GetLength() - 1);
    }

    const T& Get(int index) const override {
        return items_.Get(index);
    }

    Sequence<T>* GetSubsequence(int startIndex, int endIndex) const override {
        if (startIndex < 0 || endIndex < 0 || startIndex > endIndex || endIndex >= GetLength()) {
            throw std::out_of_range("subsequence range out of range");
        }
        ArraySequence<T>* result = CloneConcrete();
        result->items_ = DynamicArray<T>(endIndex - startIndex + 1);
        for (int i = startIndex; i <= endIndex; ++i) {
            result->items_.Set(i - startIndex, items_.Get(i));
        }
        return result;
    }

    int GetLength() const override {
        return items_.GetSize();
    }

    Sequence<T>* Append(const T& item) override {
        ArraySequence<T>* target = MutationTarget();
        target->items_.PushBack(item);
        return target;
    }

    Sequence<T>* Prepend(const T& item) override {
        ArraySequence<T>* target = MutationTarget();
        target->items_.InsertAt(item, 0);
        return target;
    }

    Sequence<T>* InsertAt(const T& item, int index) override {
        if (index < 0 || index > GetLength()) {
            throw std::out_of_range("insert index out of range");
        }
        ArraySequence<T>* target = MutationTarget();
        target->items_.InsertAt(item, index);
        return target;
    }

    Sequence<T>* Concat(const Sequence<T>& list) const override {
        ArraySequence<T>* result = CloneConcrete();
        result->items_ = DynamicArray<T>(GetLength() + list.GetLength());
        for (int i = 0; i < GetLength(); ++i) {
            result->items_.Set(i, Get(i));
        }
        for (int i = 0; i < list.GetLength(); ++i) {
            result->items_.Set(GetLength() + i, list.Get(i));
        }
        return result;
    }

    Sequence<T>* Clone() const override {
        return CloneConcrete();
    }

protected:
    DynamicArray<T> items_;

    Sequence<T>* CreateEmptySameKind() const override {
        ArraySequence<T>* result = CloneConcrete();
        result->items_ = DynamicArray<T>();
        return result;
    }

    void ForEach(const std::function<void(const T&)>& action) const override {
        for (auto it = items_.cbegin(); it != items_.cend(); ++it) {
            action(*it);
        }
    }

    virtual ArraySequence<T>* MutationTarget() = 0;
    virtual ArraySequence<T>* CloneConcrete() const = 0;
};

#endif
