#ifndef LR2_DYNAMIC_ARRAY_H
#define LR2_DYNAMIC_ARRAY_H

#include <algorithm>
#include <stdexcept>

template <class T>
class DynamicArray {
public:
    class Iterator {
    public:
        explicit Iterator(T* ptr) : ptr_(ptr) {}

        T& operator*() const { return *ptr_; }
        Iterator& operator++() {
            ++ptr_;
            return *this;
        }
        bool operator!=(const Iterator& other) const { return ptr_ != other.ptr_; }
        bool operator==(const Iterator& other) const { return ptr_ == other.ptr_; }

    private:
        T* ptr_;
    };

    class ConstIterator {
    public:
        explicit ConstIterator(const T* ptr) : ptr_(ptr) {}

        const T& operator*() const { return *ptr_; }
        ConstIterator& operator++() {
            ++ptr_;
            return *this;
        }
        bool operator!=(const ConstIterator& other) const { return ptr_ != other.ptr_; }
        bool operator==(const ConstIterator& other) const { return ptr_ == other.ptr_; }

    private:
        const T* ptr_;
    };

    DynamicArray() : size_(0), capacity_(0), data_(nullptr) {}

    DynamicArray(const T* items, int count) : size_(count), capacity_(count), data_(nullptr) {
        if (count < 0) {
            throw std::invalid_argument("count must be non-negative");
        }
        if (count > 0 && items == nullptr) {
            throw std::invalid_argument("items must not be null when count is positive");
        }
        if (size_ > 0) {
            data_ = new T[size_];
            for (int i = 0; i < size_; ++i) {
                data_[i] = items[i];
            }
        }
    }

    explicit DynamicArray(int size) : size_(size), capacity_(size), data_(nullptr) {
        if (size < 0) {
            throw std::invalid_argument("size must be non-negative");
        }
        if (size_ > 0) {
            data_ = new T[size_]{};
        }
    }

    DynamicArray(const DynamicArray<T>& other) : size_(other.size_), capacity_(other.size_), data_(nullptr) {
        if (size_ > 0) {
            data_ = new T[capacity_];
            for (int i = 0; i < size_; ++i) {
                data_[i] = other.data_[i];
            }
        }
    }

    DynamicArray<T>& operator=(const DynamicArray<T>& other) {
        if (this == &other) {
            return *this;
        }
        DynamicArray<T> copy(other);
        Swap(copy);
        return *this;
    }

    ~DynamicArray() {
        delete[] data_;
    }

    const T& Get(int index) const {
        CheckIndex(index);
        return data_[index];
    }

    T& Get(int index) {
        CheckIndex(index);
        return data_[index];
    }

    int GetSize() const {
        return size_;
    }

    int GetCapacity() const {
        return capacity_;
    }

    void Set(int index, const T& value) {
        CheckIndex(index);
        data_[index] = value;
    }

    void Resize(int newSize) {
        if (newSize < 0) {
            throw std::invalid_argument("new size must be non-negative");
        }
        if (newSize == size_) {
            return;
        }
        if (newSize <= capacity_) {
            if (newSize > size_) {
                for (int i = size_; i < newSize; ++i) {
                    data_[i] = T();
                }
            }
            size_ = newSize;
            return;
        }
        Reserve(newSize);
        size_ = newSize;
    }

    void Reserve(int newCapacity) {
        if (newCapacity <= capacity_) {
            return;
        }
        T* newData = new T[newCapacity]{};
        for (int i = 0; i < size_; ++i) {
            newData[i] = data_[i];
        }
        delete[] data_;
        data_ = newData;
        capacity_ = newCapacity;
    }

    void PushBack(const T& value) {
        EnsureCapacityForOneMore();
        data_[size_] = value;
        ++size_;
    }

    void InsertAt(const T& value, int index) {
        if (index < 0 || index > size_) {
            throw std::out_of_range("dynamic array index out of range");
        }
        EnsureCapacityForOneMore();
        for (int i = size_; i > index; --i) {
            data_[i] = data_[i - 1];
        }
        data_[index] = value;
        ++size_;
    }

    Iterator begin() { return Iterator(data_); }
    Iterator end() { return Iterator(data_ + size_); }
    ConstIterator begin() const { return ConstIterator(data_); }
    ConstIterator end() const { return ConstIterator(data_ + size_); }
    ConstIterator cbegin() const { return ConstIterator(data_); }
    ConstIterator cend() const { return ConstIterator(data_ + size_); }

private:
    int size_;
    int capacity_;
    T* data_;

    void CheckIndex(int index) const {
        if (index < 0 || index >= size_) {
            throw std::out_of_range("dynamic array index out of range");
        }
    }

    void Swap(DynamicArray<T>& other) noexcept {
        std::swap(size_, other.size_);
        std::swap(capacity_, other.capacity_);
        std::swap(data_, other.data_);
    }

    void EnsureCapacityForOneMore() {
        if (size_ < capacity_) {
            return;
        }
        const int newCapacity = capacity_ == 0 ? 1 : capacity_ * 2;
        Reserve(newCapacity);
    }
};

#endif
