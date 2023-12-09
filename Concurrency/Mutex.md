
Mutex help you with critical section via `mut.lock()` and `mut.unlock()`.

## Without mutex

We get following weird output
```
Boss: Let's start working.

Herb: Work   Andrei: Work 1 done !!! <------- Bad output!!!!
1 done !!!
    Scott: Work 1 done !!!
      Bjarne: Work 1 done !!!
        Bart: Work 1 done !!!
          Jenne: Work 1 done !!!
Herb: Work 2 done !!!
  Andrei: Work 2 done !!!
    Scott: Work 2 done !!!
      Bjarne: Work 2 done !!!
        Bart: Work 2 done !!!
          Jenne: Work 2 done !!!
Herb: Work 3 done !!!
  Andrei: Work 3 done !!!
    Scott: Work 3 done !!!
        Bart: Work 3 done !!!
          Jenne: Work 3 done !!!
      Bjarne: Work 3 done !!!

Boss: Let's go home.
```

```cpp

// we spawn multiple threads to write to cout without holding a mutex
// we observe that the output is not as expected
#include <chrono>
#include <iostream>
#include <mutex>
#include <thread>

std::mutex coutMutex;

class Worker{
public:
  Worker(std::string n):name(n){};
 
    void operator() (){
      for (int i = 1; i <= 3; ++i){ 
        std::this_thread::sleep_for(std::chrono::milliseconds(200));
        std::cout << name << ": " << "Work " << i << " done !!!" << std::endl;
      }
    }
private:
  std::string name;
};


int main(){

  std::cout << std::endl;
  
  std::cout << "Boss: Let's start working." << "\n\n";
 
  std::thread herb= std::thread(Worker("Herb"));
  std::thread andrei= std::thread(Worker("  Andrei"));
  std::thread scott= std::thread(Worker("    Scott"));
  std::thread bjarne= std::thread(Worker("      Bjarne"));
  std::thread bart= std::thread(Worker("        Bart"));
  std::thread jenne= std::thread(Worker("          Jenne"));
  
  herb.join();
  andrei.join();
  scott.join();
  bjarne.join();
  bart.join();
  jenne.join();
  
  std::cout << "\n" << "Boss: Let's go home." << std::endl;
  std::cout << std::endl;
}
```

## With mutex.lock and mutex.unlock

We get following expected output
```
Boss: Let's start working.

Herb: Work 1 done !!!
          Jenne: Work 1 done !!!
  Andrei: Work 1 done !!!
    Scott: Work 1 done !!!
      Bjarne: Work 1 done !!!
        Bart: Work 1 done !!!
Herb: Work 2 done !!!
          Jenne: Work 2 done !!!
  Andrei: Work 2 done !!!
      Bjarne: Work 2 done !!!
        Bart: Work 2 done !!!
    Scott: Work 2 done !!!
Herb: Work 3 done !!!
      Bjarne: Work 3 done !!!
          Jenne: Work 3 done !!!
  Andrei: Work 3 done !!!
        Bart: Work 3 done !!!
    Scott: Work 3 done !!!

Boss: Let's go home.
```

code:
```cpp

// we spawn multiple threads to write to cout without holding a mutex
// we observe that the output is not as expected
#include <chrono>
#include <iostream>
#include <mutex>
#include <thread>

std::mutex coutMutex;

class Worker{
public:
  Worker(std::string n):name(n){};
 
    void operator() (){
      for (int i = 1; i <= 3; ++i){ 
        std::this_thread::sleep_for(std::chrono::milliseconds(200));

        coutMutex.lock();
        std::cout << name << ": " << "Work " << i << " done !!!" << std::endl;
        coutMutex.unlock();
      }
    }
private:
  std::string name;
};


int main(){
  std::cout << std::endl;
  std::cout << "Boss: Let's start working." << "\n\n";
 
  std::thread herb= std::thread(Worker("Herb"));
  std::thread andrei= std::thread(Worker("  Andrei"));
  std::thread scott= std::thread(Worker("    Scott"));
  std::thread bjarne= std::thread(Worker("      Bjarne"));
  std::thread bart= std::thread(Worker("        Bart"));
  std::thread jenne= std::thread(Worker("          Jenne"));
  
  herb.join();
  andrei.join();
  scott.join();
  bjarne.join();
  bart.join();
  jenne.join();
  
  std::cout << "\n" << "Boss: Let's go home." << std::endl;
  std::cout << std::endl;
}
```
