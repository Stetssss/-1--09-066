#include <iostream>
#include <vector>
#include <list>
#include <chrono>
#include <random>
#include <iomanip>

using namespace std;
using namespace chrono;

const int N = 100000;
const int INSERT_COUNT = 1000;

vector<int> generateRandom(int count) {
    vector<int> data(count);
    mt19937 gen(random_device{}());
    uniform_int_distribution<> dist(0, count);
    for (int& x : data) x = dist(gen);
    return data;
}

template <typename Func>
double measureTime(Func f) {
    auto start = high_resolution_clock::now();
    f();
    auto end = high_resolution_clock::now();
    auto duration = duration_cast<microseconds>(end - start).count();
    return duration / 1000.0; // milliseconds with fractional part
}

int main() {
    vector<int> data = generateRandom(N);
    vector<int> inserts = generateRandom(INSERT_COUNT);

    vector<int> vec;
    list<int> lst;

    cout << fixed << setprecision(2);

    cout << "1. Filling lists:\n";
    double timeVecFill = measureTime([&]() {
        vec = data;
        });
    double timeListFill = measureTime([&]() {
        lst.assign(data.begin(), data.end());
        });
    cout << "Vector: " << timeVecFill << " ms\n";
    cout << "List  : " << timeListFill << " ms\n\n";

    cout << "2. Random Access (every 1000th element):\n";
    volatile int temp;
    double timeVecRandom = measureTime([&]() {
        for (int i = 0; i < N; i += 1000)
            temp = vec[i];
        });
    double timeListRandom = measureTime([&]() {
        auto it = lst.begin();
        for (int i = 0; i < N; i += 1000) {
            advance(it, (i == 0) ? 0 : 1000);
            temp = *it;
        }
        });
    cout << "Vector: " << timeVecRandom << " ms\n";
    cout << "List  : " << timeListRandom << " ms\n\n";

    cout << "3. Sequential Access (full iteration):\n";
    double timeVecSeq = measureTime([&]() {
        for (auto x : vec) temp = x;
        });
    double timeListSeq = measureTime([&]() {
        for (auto x : lst) temp = x;
        });
    cout << "Vector: " << timeVecSeq << " ms\n";
    cout << "List  : " << timeListSeq << " ms\n\n";

    cout << "4. Insert at front:\n";
    double timeVecInsertFront = measureTime([&]() {
        for (int x : inserts)
            vec.insert(vec.begin(), x);
        });
    double timeListInsertFront = measureTime([&]() {
        for (int x : inserts)
            lst.push_front(x);
        });
    cout << "Vector: " << timeVecInsertFront << " ms\n";
    cout << "List  : " << timeListInsertFront << " ms\n\n";

    cout << "5. Insert at back:\n";
    double timeVecInsertBack = measureTime([&]() {
        for (int x : inserts)
            vec.push_back(x);
        });
    double timeListInsertBack = measureTime([&]() {
        for (int x : inserts)
            lst.push_back(x);
        });
    cout << "Vector: " << timeVecInsertBack << " ms\n";
    cout << "List  : " << timeListInsertBack << " ms\n\n";

    cout << "6. Insert in middle:\n";
    double timeVecInsertMiddle = measureTime([&]() {
        for (int x : inserts) {
            auto midVec = vec.begin() + vec.size() / 2;
            vec.insert(midVec, x);
        }
        });

    auto itMid = lst.begin();
    advance(itMid, lst.size() / 2);
    double timeListInsertMiddle = measureTime([&]() {
        for (int x : inserts)
            lst.insert(itMid, x);
        });

    cout << "Vector: " << timeVecInsertMiddle << " ms\n";
    cout << "List  : " << timeListInsertMiddle << " ms\n";

    return 0;
}
