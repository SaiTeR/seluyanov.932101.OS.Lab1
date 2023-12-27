#include <iostream>
#include <thread>
#include <mutex>
#include <condition_variable>

using namespace std;


mutex mtx;
condition_variable cv;
bool eventSent = false;


void producer() {
    unique_lock<mutex> lock(mtx);

    while (eventSent) {
        cv.wait(lock);
    }

    cout << "[ПОСТАВЩИК]: \tсобытие отправлено" << endl;
    eventSent = true;
    
    cv.notify_one();
    lock.unlock();
}

void consumer() {
    unique_lock<mutex> lock(mtx);

    while (!eventSent) {
        cv.wait(lock);
    }

    cout << "[ПОТРЕБИТЕЛЬ]: \tсобытие получено" << endl;
    eventSent = false;

    cv.notify_one();
    lock.unlock();
}

int main() {
    setlocale(LC_ALL, "RUS");

    thread producerThread(producer);
    thread consumerThread(consumer);

    consumerThread.join();

    return 0;
}
