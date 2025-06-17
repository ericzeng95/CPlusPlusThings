#include <gtest/gtest.h>
#include <vector.h> // Your vector header file

// Test fixture for setting up common test environment
class MyVectorTest : public ::testing::Test {
protected:
    // Helper class to track construction/destruction for memory management tests
    struct TestObject {
        static int constructions;
        static int destructions;
        int value;
        TestObject(int v = 0) : value(v) { ++constructions; }
        ~TestObject() { ++destructions; }
        TestObject(const TestObject&) { ++constructions; }
        TestObject& operator=(const TestObject&) { return *this; }
    };
};

// Initialize static counters
int MyVectorTest::TestObject::constructions = 0;
int MyVectorTest::TestObject::destructions = 0;

// Reset counters before each test
void ResetTestObjectCounters() {
    MyVectorTest::TestObject::constructions = 0;
    MyVectorTest::TestObject::destructions = 0;
}

// 1. Memory Management Tests
TEST_F(MyVectorTest, SeparateAllocationAndConstruction) {
    ResetTestObjectCounters();
    {
        MyVector<TestObject> vec;
        vec.reserve(10); // Allocate memory for 10 elements
        EXPECT_EQ(TestObject::constructions, 0) << "No objects should be constructed during reserve";
        vec.emplace_back(42); // Construct one object
        EXPECT_EQ(TestObject::constructions, 1) << "One object should be constructed";
        EXPECT_EQ(TestObject::destructions, 0) << "No objects should be destroyed yet";
    }
    EXPECT_EQ(TestObject::destructions, 1) << "Object should be destroyed on vector destruction";
}

TEST_F(MyVectorTest, SeparateDeallocationAndDestruction) {
    ResetTestObjectCounters();
    {
        MyVector<TestObject> vec;
        vec.emplace_back(42);
        vec.emplace_back(43);
        EXPECT_EQ(TestObject::constructions, 2);
        vec.clear(); // Destroy objects but may not deallocate
        EXPECT_EQ(TestObject::destructions, 2) << "Objects should be destroyed on clear";
        EXPECT_EQ(TestObject::constructions, 2) << "No new constructions during clear";
    }
    EXPECT_EQ(TestObject::destructions, 2) << "No additional destructions after clear";
}

TEST_F(MyVectorTest, ResizingMechanism) {
    ResetTestObjectCounters();
    {
        MyVector<TestObject> vec;
        vec.resize(5); // Construct 5 default objects
        EXPECT_EQ(TestObject::constructions, 5) << "Should construct 5 objects";
        EXPECT_EQ(TestObject::destructions, 0);
        vec.resize(2); // Destroy 3 objects
        EXPECT_EQ(TestObject::destructions, 3) << "Should destroy 3 objects";
        EXPECT_EQ(TestObject::constructions, 5);
        vec.resize(4); // Construct 2 more objects
        EXPECT_EQ(TestObject::constructions, 7) << "Should construct 2 additional objects";
        EXPECT_EQ(TestObject::destructions, 3);
    }
    EXPECT_EQ(TestObject::destructions, 7) << "All objects destroyed on vector destruction";
}

// 2. Move Constructor and Assignment Operator Tests
TEST_F(MyVectorTest, MoveConstructor) {
    MyVector<int> source;
    source.push_back(1);
    source.push_back(2);
    size_t source_capacity = source.capacity();

    MyVector<int> dest(std::move(source));
    EXPECT_EQ(dest.size(), 2) << "Moved vector should have same size";
    EXPECT_EQ(dest[0], 1);
    EXPECT_EQ(dest[1], 2);
    EXPECT_EQ(source.size(), 0) << "Source should be empty after move";
    EXPECT_EQ(source.capacity(), 0) << "Source capacity should be reset";
    EXPECT_EQ(dest.capacity(), source_capacity) << "Dest should inherit capacity";
}

TEST_F(MyVectorTest, MoveAssignmentOperator) {
    MyVector<int> source;
    source.push_back(1);
    source.push_back(2);
    size_t source_capacity = source.capacity();

    MyVector<int> dest;
    dest.push_back(3);
    dest = std::move(source);
    EXPECT_EQ(dest.size(), 2) << "Moved vector should have same size";
    EXPECT_EQ(dest[0], 1);
    EXPECT_EQ(dest[1], 2);
    EXPECT_EQ(source.size(), 0) << "Source should be empty after move";
    EXPECT_EQ(source.capacity(), 0) << "Source capacity should be reset";
    EXPECT_EQ(dest.capacity(), source_capacity) << "Dest should inherit capacity";
}

// 3. Insert and Erase Tests
TEST_F(MyVectorTest, InsertSingleElement) {
    MyVector<int> vec;
    vec.push_back(1);
    vec.push_back(3);
    auto it = vec.insert(vec.begin() + 1, 2); // Insert 2 between 1 and 3
    EXPECT_EQ(vec.size(), 3);
    EXPECT_EQ(vec[0], 1);
    EXPECT_EQ(vec[1], 2);
    EXPECT_EQ(vec[2], 3);
    EXPECT_EQ(*it, 2) << "Iterator should point to inserted element";
}

TEST_F(MyVectorTest, InsertMultipleElements) {
    MyVector<int> vec;
    vec.push_back(1);
    vec.push_back(4);
    int arr[] = {2, 3};
    auto it = vec.insert(vec.begin() + 1, arr, arr + 2); // Insert 2, 3
    EXPECT_EQ(vec.size(), 4);
    EXPECT_EQ(vec[0], 1);
    EXPECT_EQ(vec[1], 2);
    EXPECT_EQ(vec[2], 3);
    EXPECT_EQ(vec[3], 4);
    EXPECT_EQ(*it, 2) << "Iterator should point to first inserted element";
}

TEST_F(MyVectorTest, EraseSingleElement) {
    MyVector<int> vec;
    vec.push_back(1);
    vec.push_back(2);
    vec.push_back(3);
    auto it = vec.erase(vec.begin() + 1); // Erase 2
    EXPECT_EQ(vec.size(), 2);
    EXPECT_EQ(vec[0], 1);
    EXPECT_EQ(vec[1], 3);
    EXPECT_EQ(*it, 3) << "Iterator should point to element after erased";
}

TEST_F(MyVectorTest, EraseRange) {
    MyVector<int> vec;
    vec.push_back(1);
    vec.push_back(2);
    vec.push_back(3);
    vec.push_back(4);
    auto it = vec.erase(vec.begin() + 1, vec.begin() + 3); // Erase 2, 3
    EXPECT_EQ(vec.size(), 2);
    EXPECT_EQ(vec[0], 1);
    EXPECT_EQ(vec[1], 4);
    EXPECT_EQ(*it, 4) << "Iterator should point to element after erased range";
}

// 4. Iterator Support Tests
TEST_F(MyVectorTest, IteratorTraversal) {
    MyVector<int> vec;
    vec.push_back(1);
    vec.push_back(2);
    vec.push_back(3);
    int expected[] = {1, 2, 3};
    int i = 0;
    for (auto it = vec.begin(); it != vec.end(); ++it) {
        EXPECT_EQ(*it, expected[i++]) << "Iterator should traverse elements in order";
    }
    EXPECT_EQ(i, 3);
}

TEST_F(MyVectorTest, ConstIteratorTraversal) {
    MyVector<int> vec;
    vec.push_back(1);
    vec.push_back(2);
    vec.push_back(3);
    const MyVector<int>& const_vec = vec;
    int expected[] = {1, 2, 3};
    int i = 0;
    for (auto it = const_vec.begin(); it != const_vec.end(); ++it) {
        EXPECT_EQ(*it, expected[i++]) << "Const iterator should traverse elements in order";
    }
    EXPECT_EQ(i, 3);
}

TEST_F(MyVectorTest, ReverseIteratorTraversal) {
    MyVector<int> vec;
    vec.push_back(1);
    vec.push_back(2);
    vec.push_back(3);
    int expected[] = {3, 2, 1};
    int i = 0;
    for (auto it = vec.rbegin(); it != vec.rend(); ++it) {
        EXPECT_EQ(*it, expected[i++]) << "Reverse iterator should traverse elements in reverse";
    }
    EXPECT_EQ(i, 3);
}

TEST_F(MyVectorTest, IteratorModification) {
    MyVector<int> vec;
    vec.push_back(1);
    vec.push_back(2);
    vec.push_back(3);
    for (auto it = vec.begin(); it != vec.end(); ++it) {
        *it += 1; // Modify elements via iterator
    }
    EXPECT_EQ(vec[0], 2);
    EXPECT_EQ(vec[1], 3);
    EXPECT_EQ(vec[2], 4);
}