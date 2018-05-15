#include <boost/lexical_cast.hpp>
#include <boost/lockfree/spsc_queue.hpp>
#include <functional>
#include <iostream>
#include <string>
#include <thread>

using namespace std::placeholders;

struct Data {
  Data(int i) : i_(i) {}
  Data(const Data& other) {
    std::cout << "copy constract" << std::endl;
    i_ = other.i_;
  }

  Data(Data&& other) = default;

  friend std::ostream& operator<<(std::ostream& os, const Data& data) {
    return os << data.i_ << std::endl;
  }

  int i_;
};

using LockFreeQueue =
    boost::lockfree::spsc_queue<Data,
                                boost::lockfree::capacity<1024>,
                                boost::lockfree::fixed_sized<false> >;
typedef typename LockFreeQueue::value_type QueueItemType;

void Processer(const QueueItemType& msg) {
  std::cout << msg << std::endl;
}

void Consumer(LockFreeQueue& queue, bool& stoped) {
  std::this_thread::sleep_for(std::chrono::seconds(10));
  while (!stoped) {
    queue.consume_all(std::bind(&Processer, _1));
  }
}

int main(int argc, char** argv) {
  LockFreeQueue queue;
  bool stoped = false;

  auto thrd =
      std::thread(std::bind(&Consumer, std::ref(queue), std::ref(stoped)));

  std::string input;
  while (std::cin >> input) {
    if (input == "exit") {
      stoped = true;
      break;
    }
    // queue.push(input);
    std::cout << "perpare push" << std::endl;
    queue.push(Data{boost::lexical_cast<int>(input)});
    std::cout << "after push" << std::endl;
  }

  thrd.join();
  return 0;
}
