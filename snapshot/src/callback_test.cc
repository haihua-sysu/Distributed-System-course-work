#include "lib/runnable.h"
#include "lib/time.h"

#include <unistd.h>
#include <iostream>
using namespace std;

// Task override the Runnable interface which allow the timer execute run() method.
// Task has its parameter and implement the execution logic by overriding the pure virtual method run()
class Task : public Runnable {
public:
    Task(int a, int b): a_(a), b_(b) {}

    void run() override {
        cout << a_ + b_ << endl;
    }
private:
    int a_, b_;
};

int main() {
    Runnable* task = new Task(1, 2);

    // task->run() will be call from another thread after 2500 ms.
    // registerCallback is an unblocked method.
    registerCallback(task, 2500);

    // Sleep 3 seconds to see the result
    sleep(3);
    return 0;
}
