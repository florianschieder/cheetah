#include <gtest/gtest.h>

#include <Lock.h>
#include <memory>
#include <thread>

using namespace std;
using namespace cheetah;

int result[100] = { 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
                    0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
                    0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
                    0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
                    0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
                    0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
                    0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
                    0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
                    0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
                    0, 0, 0, 0, 0, 0, 0, 0, 0, 0};

int expected_variant_1[100] = { 7, 7, 7, 7, 7, 7, 7, 7, 7, 7,
                                7, 7, 7, 7, 7, 7, 7, 7, 7, 7,
                                7, 7, 7, 7, 7, 7, 7, 7, 7, 7,
                                7, 7, 7, 7, 7, 7, 7, 7, 7, 7,
                                7, 7, 7, 7, 7, 7, 7, 7, 7, 7,
                                5, 5, 5, 5, 5, 5, 5, 5, 5, 5,
                                5, 5, 5, 5, 5, 5, 5, 5, 5, 5,
                                5, 5, 5, 5, 5, 5, 5, 5, 5, 5,
                                5, 5, 5, 5, 5, 5, 5, 5, 5, 5,
                                5, 5, 5, 5, 5, 5, 5, 5, 5, 5 };

int expected_variant_2[100] = { 5, 5, 5, 5, 5, 5, 5, 5, 5, 5,
                                5, 5, 5, 5, 5, 5, 5, 5, 5, 5,
                                5, 5, 5, 5, 5, 5, 5, 5, 5, 5,
                                5, 5, 5, 5, 5, 5, 5, 5, 5, 5,
                                5, 5, 5, 5, 5, 5, 5, 5, 5, 5,
                                7, 7, 7, 7, 7, 7, 7, 7, 7, 7,
                                7, 7, 7, 7, 7, 7, 7, 7, 7, 7,
                                7, 7, 7, 7, 7, 7, 7, 7, 7, 7,
                                7, 7, 7, 7, 7, 7, 7, 7, 7, 7,
                                7, 7, 7, 7, 7, 7, 7, 7, 7, 7 };

#include <Windows.h>

int i = 0;

void print_block(shared_ptr<Lock> lock, int count, int value)
{
    if (lock != nullptr) {
        lock->acquire();
    }

    for (int c = 0; c < count; ++c) {
        result[i] = value;
        Sleep(2);
        i++;
    }

    if (lock != nullptr) {
        lock->free();
    }
}

bool compare_arr(int arr1[50], int arr2[50])
{
    for (int c = 0; c < 50; ++c)
    {
        if (arr1[c] != arr2[c]) {
            return false;
        }
    }

    return true;
}

TEST(LockTest, lockImplementationAvoidsRaceConditions) {
    // (1) Without locks: Race conditions should occur.
    i = 0; // reset global counter

    thread th1_withoutlock(&print_block, nullptr, 50, 5);
    thread th2_withoutlock(&print_block, nullptr, 50, 7);
    th1_withoutlock.join();
    th2_withoutlock.join();

    ASSERT_FALSE(compare_arr(result, expected_variant_1));
    ASSERT_FALSE(compare_arr(result, expected_variant_2));

    // (2) Using locks should prevent race conditions
    i = 0;  // reset global counter

    auto lock = std::make_shared<Lock>(L"test_lock");
    thread th1(&print_block, lock, 50, 5);
    thread th2(&print_block, lock, 50, 7);
    th1.join();
    th2.join();

    ASSERT_TRUE(
        compare_arr(result, expected_variant_1)
        ^ compare_arr(result, expected_variant_2));
}
