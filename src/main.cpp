#include <chrono>
#include <iostream>
#include <limits>
#include <memory>
#include <string>
#include "DynamicArray.h"
#include "LinkedList.h"
#include "MutableArraySequence.h"
#include "ImmutableArraySequence.h"
#include "MutableListSequence.h"
#include "ImmutableListSequence.h"
#include "BitSequence.h"
#include "SequenceMapSupport.h"
#include "Tests.h"

using std::cin;
using std::cout;
using std::endl;

int ReadInt(const std::string& prompt) {
    cout << prompt;
    int value;
    cin >> value;
    if (!cin) {
        throw std::runtime_error("invalid integer input");
    }
    return value;
}

void PrintIntSequence(const Sequence<int>& seq) {
    cout << "[";
    for (int i = 0; i < seq.GetLength(); ++i) {
        if (i > 0) cout << ", ";
        cout << seq.Get(i);
    }
    cout << "]\n";
}

void PrintBitSequence(const BitSequence& seq) {
    cout << "[";
    for (int i = 0; i < seq.GetLength(); ++i) {
        if (i > 0) cout << " ";
        cout << seq.Get(i);
    }
    cout << "]\n";
}

void DemoDynamicArray() {
    DynamicArray<int> array;
    bool running = true;
    while (running) {
        cout << "\nDynamicArray<int>\n";
        cout << "1. Resize\n2. Set\n3. Get\n4. Print\n0. Back\n> ";
        int cmd = ReadInt("");
        switch (cmd) {
            case 1:
                array.Resize(ReadInt("New size: "));
                break;
            case 2: {
                int index = ReadInt("Index: ");
                int value = ReadInt("Value: ");
                array.Set(index, value);
                break;
            }
            case 3:
                cout << array.Get(ReadInt("Index: ")) << endl;
                break;
            case 4:
                cout << "[";
                for (int i = 0; i < array.GetSize(); ++i) {
                    if (i > 0) cout << ", ";
                    cout << array.Get(i);
                }
                cout << "]\n";
                break;
            case 0:
                running = false;
                break;
            default:
                cout << "Unknown command\n";
        }
    }
}

void DemoLinkedList() {
    LinkedList<int> list;
    bool running = true;
    while (running) {
        cout << "\nLinkedList<int>\n";
        cout << "1. Append\n2. Prepend\n3. InsertAt\n4. RemoveAt\n5. Get\n6. Print\n0. Back\n> ";
        int cmd = ReadInt("");
        switch (cmd) {
            case 1:
                list.Append(ReadInt("Value: "));
                break;
            case 2:
                list.Prepend(ReadInt("Value: "));
                break;
            case 3: {
                int value = ReadInt("Value: ");
                int index = ReadInt("Index: ");
                list.InsertAt(value, index);
                break;
            }
            case 4:
                list.RemoveAt(ReadInt("Index: "));
                break;
            case 5:
                cout << list.Get(ReadInt("Index: ")) << endl;
                break;
            case 6:
                cout << "[";
                for (int i = 0; i < list.GetLength(); ++i) {
                    if (i > 0) cout << ", ";
                    cout << list.Get(i);
                }
                cout << "]\n";
                break;
            case 0:
                running = false;
                break;
            default:
                cout << "Unknown command\n";
        }
    }
}

template <class TSequence>
void SequenceMenu(const std::string& title) {
    TSequence seq;
    auto applyUpdate = [&](Sequence<int>* updated) {
        if (updated == &seq) {
            return;
        }
        TSequence* typedUpdated = dynamic_cast<TSequence*>(updated);
        if (typedUpdated == nullptr) {
            delete updated;
            throw std::runtime_error("unexpected sequence type returned from update");
        }
        seq = *typedUpdated;
        delete typedUpdated;
    };

    bool running = true;
    while (running) {
        cout << "\n" << title << "\n";
        cout << "1. Append\n2. Prepend\n3. InsertAt\n4. Get\n5. Print\n6. Map x2\n7. Where even\n8. Reduce sum\n9. TryGetFirst > x\n0. Back\n> ";
        int cmd = ReadInt("");
        switch (cmd) {
            case 1: {
                applyUpdate(seq.Append(ReadInt("Value: ")));
                break;
            }
            case 2: {
                applyUpdate(seq.Prepend(ReadInt("Value: ")));
                break;
            }
            case 3: {
                int value = ReadInt("Value: ");
                int index = ReadInt("Index: ");
                applyUpdate(seq.InsertAt(value, index));
                break;
            }
            case 4:
                cout << seq.Get(ReadInt("Index: ")) << endl;
                break;
            case 5:
                PrintIntSequence(seq);
                break;
            case 6: {
                std::unique_ptr<Sequence<int>> mapped(seq.template Map<int>([](int x) { return x * 2; }));
                PrintIntSequence(*mapped);
                break;
            }
            case 7: {
                std::unique_ptr<Sequence<int>> filtered(seq.Where([](int x) { return x % 2 == 0; }));
                PrintIntSequence(*filtered);
                break;
            }
            case 8:
                cout << seq.template Reduce<int>([](int acc, int x) { return acc + x; }, 0) << endl;
                break;
            case 9: {
                int limit = ReadInt("x = ");
                Option<int> found = seq.TryGetFirst([limit](int x) { return x > limit; });
                if (found.HasValue()) {
                    cout << "Found: " << found.Value() << endl;
                } else {
                    cout << "No element found\n";
                }
                break;
            }
            case 0:
                running = false;
                break;
            default:
                cout << "Unknown command\n";
        }
    }
}

void DemoBitSequence() {
    BitSequence first;
    BitSequence second;
    int n = ReadInt("Length: ");
    cout << "Enter first sequence bits: ";
    for (int i = 0; i < n; ++i) {
        first.Append(Bit(ReadInt("")));
    }
    cout << "Enter second sequence bits: ";
    for (int i = 0; i < n; ++i) {
        second.Append(Bit(ReadInt("")));
    }

    BitSequence anded = first.BitwiseAnd(second);
    BitSequence ored = first.BitwiseOr(second);
    BitSequence xored = first.BitwiseXor(second);
    BitSequence notted = first.BitwiseNot();

    cout << "A      = "; PrintBitSequence(first);
    cout << "B      = "; PrintBitSequence(second);
    cout << "A AND B= "; PrintBitSequence(anded);
    cout << "A OR B = "; PrintBitSequence(ored);
    cout << "A XOR B= "; PrintBitSequence(xored);
    cout << "NOT A  = "; PrintBitSequence(notted);
}

void RunPerformanceDemo() {
    const int n = 20000;
    MutableArraySequence<int> arraySeq;
    MutableListSequence<int> listSeq;

    auto t1 = std::chrono::high_resolution_clock::now();
    for (int i = 0; i < n; ++i) {
        arraySeq.Append(i);
    }
    auto t2 = std::chrono::high_resolution_clock::now();
    for (int i = 0; i < n; ++i) {
        listSeq.Append(i);
    }
    auto t3 = std::chrono::high_resolution_clock::now();

    auto arrayMs = std::chrono::duration_cast<std::chrono::milliseconds>(t2 - t1).count();
    auto listMs = std::chrono::duration_cast<std::chrono::milliseconds>(t3 - t2).count();

    cout << "Append benchmark for " << n << " elements:\n";
    cout << "MutableArraySequence: " << arrayMs << " ms\n";
    cout << "MutableListSequence : " << listMs << " ms\n";
}

void PrintProjectHelp() {
    cout << "\nThis program contains:\n";
    cout << "- DynamicArray: dynamic array ADT\n";
    cout << "- LinkedList: doubly linked list ADT\n";
    cout << "- Sequence hierarchy: common polymorphic interface\n";
    cout << "- Mutable/Immutable ArraySequence and ListSequence\n";
    cout << "- BitSequence with AND/OR/XOR/NOT\n";
    cout << "- Tests, console UI and simple performance demo\n";
    cout << "See README.md for a file-by-file explanation.\n";
}

int main() {
    bool running = true;
    while (running) {
        try {
            cout << "\nLab work #2 menu\n";
            cout << "1. Run tests\n";
            cout << "2. DynamicArray demo\n";
            cout << "3. LinkedList demo\n";
            cout << "4. MutableArraySequence demo\n";
            cout << "5. ImmutableArraySequence demo\n";
            cout << "6. MutableListSequence demo\n";
            cout << "7. ImmutableListSequence demo\n";
            cout << "8. BitSequence demo\n";
            cout << "9. Performance demo\n";
            cout << "10. Project help\n";
            cout << "0. Exit\n> ";
            int cmd = ReadInt("");
            switch (cmd) {
                case 1: RunAllTests(); break;
                case 2: DemoDynamicArray(); break;
                case 3: DemoLinkedList(); break;
                case 4: SequenceMenu<MutableArraySequence<int>>("MutableArraySequence<int>"); break;
                case 5: SequenceMenu<ImmutableArraySequence<int>>("ImmutableArraySequence<int>"); break;
                case 6: SequenceMenu<MutableListSequence<int>>("MutableListSequence<int>"); break;
                case 7: SequenceMenu<ImmutableListSequence<int>>("ImmutableListSequence<int>"); break;
                case 8: DemoBitSequence(); break;
                case 9: RunPerformanceDemo(); break;
                case 10: PrintProjectHelp(); break;
                case 0: running = false; break;
                default: cout << "Unknown command\n"; break;
            }
        } catch (const std::exception& ex) {
            cout << "Error: " << ex.what() << endl;
            cin.clear();
            cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
        }
    }
    return 0;
}
