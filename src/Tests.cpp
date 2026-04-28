#include "Tests.h"

#include <functional>
#include <iostream>
#include <stdexcept>
#include <string>

#include "BitSequence.h"
#include "DynamicArray.h"
#include "ImmutableArraySequence.h"
#include "ImmutableListSequence.h"
#include "LinkedList.h"
#include "MutableArraySequence.h"
#include "MutableListSequence.h"
#include "SequenceMapSupport.h"

using std::cout;
using std::endl;

namespace {

void PrintResult(const char* name, bool ok) {
    cout << name << ": " << (ok ? "OK" : "FAIL") << endl;
}

bool ExpectThrow(const std::function<void()>& action) {
    try {
        action();
    } catch (...) {
        return true;
    }
    return false;
}

void TestDynamicArray() {
    int arr[] = {1, 2, 3};
    DynamicArray<int> da(arr, 3);

    PrintResult("Get", da.Get(0) == 1 && da.Get(2) == 3);
    PrintResult("GetSize", da.GetSize() == 3);
    PrintResult("Initial capacity", da.GetCapacity() >= da.GetSize());

    da.Set(1, 10);
    PrintResult("Set", da.Get(1) == 10);

    da.Resize(5);
    PrintResult("Resize grow", da.GetSize() == 5 && da.Get(0) == 1 && da.Get(1) == 10 && da.Get(2) == 3);

    da.Resize(2);
    PrintResult("Resize shrink", da.GetSize() == 2 && da.Get(0) == 1 && da.Get(1) == 10);

    da.PushBack(99);
    da.InsertAt(77, 1);
    PrintResult("PushBack/InsertAt", da.GetSize() == 4 && da.Get(1) == 77 && da.Get(3) == 99);

    PrintResult("Get index error", ExpectThrow([&]() { (void)da.Get(10); }));
    PrintResult("Invalid size error", ExpectThrow([]() { DynamicArray<int> invalid(-1); }));
    PrintResult("Null source error", ExpectThrow([]() { DynamicArray<int> invalid(nullptr, 1); }));
}

void TestLinkedList() {
    int arr[] = {1, 2, 3};
    LinkedList<int> list(arr, 3);

    list.Append(4);
    PrintResult("Append", list.GetLast() == 4 && list.GetLength() == 4);

    list.Prepend(0);
    PrintResult("Prepend", list.GetFirst() == 0 && list.GetLength() == 5);

    list.InsertAt(99, 2);
    PrintResult("InsertAt", list.Get(2) == 99 && list.GetLength() == 6);

    LinkedList<int> sub = list.GetSubList(1, 3);
    bool subOk = sub.GetLength() == 3 && sub.Get(0) == 1 && sub.Get(1) == 99 && sub.Get(2) == 2;
    PrintResult("GetSubList", subOk);

    LinkedList<int> rhs(arr, 3);
    LinkedList<int> concat = list.Concat(rhs);
    bool concatOk = rhs.GetLength() == 3 &&
                    concat.GetLength() == list.GetLength() + rhs.GetLength() &&
                    concat.Get(0) == 0 && concat.Get(5) == 4 &&
                    concat.Get(6) == 1 && concat.Get(8) == 3;
    PrintResult("Concat", concatOk);

    list.RemoveAt(2);
    PrintResult("RemoveAt", list.Get(2) == 2 && list.GetLength() == 5);

    PrintResult("InsertAt index error", ExpectThrow([&]() { list.InsertAt(1, 100); }));
    PrintResult("GetSubList range error", ExpectThrow([&]() { (void)list.GetSubList(-1, 2); }));
}

void TestMutableImmutable() {
    int arr[] = {1, 2, 3};

    MutableArraySequence<int> mutableArray(arr, 3);
    Sequence<int>* sameArray = mutableArray.Append(4);
    PrintResult("MutableArraySequence mutates self", sameArray == &mutableArray && mutableArray.GetLength() == 4 && mutableArray.Get(3) == 4);

    ImmutableArraySequence<int> immutableArray(arr, 3);
    Sequence<int>* newArray = immutableArray.Append(4);
    bool immutableArrayOk = newArray != &immutableArray && immutableArray.GetLength() == 3 && newArray->GetLength() == 4 && newArray->Get(3) == 4;
    PrintResult("ImmutableArraySequence returns new object", immutableArrayOk);
    delete newArray;

    MutableListSequence<int> mutableList(arr, 3);
    Sequence<int>* sameList = mutableList.Prepend(0);
    PrintResult("MutableListSequence mutates self", sameList == &mutableList && mutableList.GetFirst() == 0 && mutableList.GetLength() == 4);

    ImmutableListSequence<int> immutableList(arr, 3);
    Sequence<int>* newList = immutableList.InsertAt(10, 1);
    bool immutableListOk = newList != &immutableList && immutableList.GetLength() == 3 && newList->GetLength() == 4 && newList->Get(1) == 10;
    PrintResult("ImmutableListSequence returns new object", immutableListOk);
    delete newList;
}

void TestMapWhereReduce() {
    int arr[] = {1, 2, 3, 4};
    MutableArraySequence<int> seq(arr, 4);

    Sequence<int>* mapped = seq.Map<int>([](const int& x) { return x + 1; });
    bool mapOk = mapped->GetLength() == 4 && mapped->Get(0) == 2 && mapped->Get(1) == 3 && mapped->Get(2) == 4 && mapped->Get(3) == 5;
    PrintResult("Map", mapOk);

    Sequence<int>* filtered = seq.Where([](const int& x) { return x % 2 == 0; });
    bool whereOk = filtered->GetLength() == 2 && filtered->Get(0) == 2 && filtered->Get(1) == 4;
    PrintResult("Where", whereOk);

    int sum = seq.Reduce<int>([](int acc, const int& x) { return acc + x; }, 0);
    PrintResult("Reduce", sum == 10);

    Option<int> found = seq.TryGetFirst([](const int& x) { return x > 3; });
    PrintResult("TryGetFirst found", found.HasValue() && found.Value() == 4);

    Option<int> missing = seq.TryGetFirst([](const int& x) { return x > 100; });
    PrintResult("TryGetFirst none", !missing.HasValue() && missing.ValueOr(-1) == -1);

    MutableListSequence<int> listSeq(arr, 4);
    Sequence<int>* listFiltered = listSeq.Where([](const int& x) { return x >= 3; });
    bool listWhereOk = listFiltered->GetLength() == 2 && listFiltered->Get(0) == 3 && listFiltered->Get(1) == 4;
    PrintResult("Where on ListSequence", listWhereOk);

    delete mapped;
    delete filtered;
    delete listFiltered;
}

void TestBitSequence() {
    Bit a[] = {1, 0, 1, 0};
    Bit b[] = {1, 1, 0, 0};
    BitSequence first(a, 4);
    BitSequence second(b, 4);

    BitSequence anded = first.BitwiseAnd(second);
    BitSequence ored = first.BitwiseOr(second);
    BitSequence xored = first.BitwiseXor(second);
    BitSequence notted = first.BitwiseNot();

    bool andOk = anded.Get(0) == Bit(1) && anded.Get(1) == Bit(0) && anded.Get(2) == Bit(0) && anded.Get(3) == Bit(0);
    bool orOk = ored.Get(0) == Bit(1) && ored.Get(1) == Bit(1) && ored.Get(2) == Bit(1) && ored.Get(3) == Bit(0);
    bool xorOk = xored.Get(0) == Bit(0) && xored.Get(1) == Bit(1) && xored.Get(2) == Bit(1) && xored.Get(3) == Bit(0);
    bool notOk = notted.Get(0) == Bit(0) && notted.Get(1) == Bit(1) && notted.Get(2) == Bit(0) && notted.Get(3) == Bit(1);

    PrintResult("BitwiseAnd", andOk);
    PrintResult("BitwiseOr", orOk);
    PrintResult("BitwiseXor", xorOk);
    PrintResult("BitwiseNot", notOk);

    Bit shortBits[] = {1, 0};
    BitSequence shortSeq(shortBits, 2);
    PrintResult("Bitwise length error", ExpectThrow([&]() {
        (void)first.BitwiseAnd(shortSeq);
    }));
}

}  // namespace

void RunDynamicArrayTests() {
    TestDynamicArray();
}

void RunLinkedListTests() {
    TestLinkedList();
}

void RunMutableImmutableTests() {
    TestMutableImmutable();
}

void RunMapReduceTests() {
    TestMapWhereReduce();
}

void RunBitSequenceTests() {
    TestBitSequence();
}

void RunAllTests() {
    cout << "RUNNING TESTS\n";

    TestDynamicArray();
    TestLinkedList();
    TestMutableImmutable();
    TestMapWhereReduce();
    TestBitSequence();

    cout << "ALL TESTS FINISHED\n";
}
