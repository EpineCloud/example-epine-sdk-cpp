#include "./lib/epine-sdk-cpp/src/epine.h"

#include <functional>
#include <iostream>
#include <thread>
#include <mutex>
#include <condition_variable>
#include <string>
#ifdef WIN32
#define HIGHLIGHT(__O__) std::cout << __O__ << std::endl
#define EM(__O__) std::cout << __O__ << std::endl

#include <stdio.h>
#include <tchar.h>
#define MAIN_FUNC int _tmain(int argc, _TCHAR *argv[])
#else
#define HIGHLIGHT(__O__) std::cout << "\e[1;31m" << __O__ << "\e[0m" << std::endl
#define EM(__O__) std::cout << "\e[1;30;1m" << __O__ << "\e[0m" << std::endl

#define MAIN_FUNC int main(int argc, const char *args[])
#endif

using namespace std;
using namespace sio;

std::mutex _lock;
std::condition_variable_any _cond;
bool isReady = false;

void wait() {
  isReady = false;
  // Lock thread till done
  _lock.lock();
  if (!isReady)
  {
    _cond.wait(_lock);
  }
  _lock.unlock();
}

void stopWaiting() {
  _lock.lock();
  _cond.notify_all();
  isReady = true;
  _lock.unlock();
}

MAIN_FUNC {
  // Define Epine Client
  Epine::Client epineClient;

  // Set init callback
  auto on_init_callback = [](){
    HIGHLIGHT("Client is ready");

    stopWaiting();
  };
  epineClient.set_on_init_callback(on_init_callback);

  // Call init
  epineClient.init();

  wait();

  // Get wallet balance
  std::string balanceJSON = epineClient.tokens->getAddressBalance(
    "vitalik.eth",
    Epine::Constants::Chains::Type::EVM,
    Epine::Constants::Chains::ID::EVM_ETHEREUM
  );
  HIGHLIGHT("Wallet balance JSON: " + balanceJSON);

  // Set wallet connection callback
  epineClient.auth->wallet->on(Epine::Auth::Wallet::Event::CONNECTED, [&](std::string addresses[]){
    HIGHLIGHT("CONNECTED CALLBACK");

    HIGHLIGHT("Connected address #0: " + addresses[0]);

    stopWaiting();
  });

  // Request wallet connection
  std::string uri = epineClient.auth->wallet->connect(Epine::Constants::Chains::Type::EVM);
  HIGHLIGHT("Received URI: " + uri);

  wait();

  EM("Finished");
  return 0;
}
