#include <unity.h>

template <class T>
class SmartPtr {
    T *ptr;  // Actual pointer
   public:
    // Constructor
    explicit SmartPtr(T *p = NULL) { ptr = p; }

    // Destructor
    ~SmartPtr() { delete (ptr); }

    // Overloading dereferncing operator
    T &operator*() { return *ptr; }

    // Overloding arrow operator so that members of T can be accessed
    // like a pointer (useful if T represents a class or struct or
    // union type)
    T *operator->() { return ptr; }
};

void setUp(void) {
    // set stuff up here
}

void tearDown(void) {
    // // clean stuff up here
}

void smartTest(void) {
    SmartPtr<int> ptr(new int());
    *ptr = 20;
}

int main(int argc, char **argv) {
    UNITY_BEGIN();
    RUN_TEST(smartTest);
    UNITY_END();

    return 0;
}
