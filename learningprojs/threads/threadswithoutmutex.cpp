
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
        std::cout << "Worker " << name << " started." << std::endl;
      for (int i = 1; i <= 5; ++i){ 
        std::this_thread::sleep_for(std::chrono::milliseconds(150));
        // coutMutex.lock();
        std::cout << name << ": " << "Work " << i << " done !!!" << std::endl;
        // coutMutex.unlock();
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