#pragma once

#include <chrono>
#include <thread>
#include <memory>
#include <vector>
#include <iostream>
#include <boost/asio.hpp>


using work_guard_type = boost::asio::executor_work_guard<boost::asio::io_context::executor_type>;

class io_context_group {
public:
  io_context_group(std::size_t size) 
  {
    for (std::size_t i = 0; i < size; ++i)
      contexts.emplace_back(std::make_shared<boost::asio::io_context>());
  }

  void run()
  {
    for (auto& context : contexts)
      threads.emplace_back([&] { context->run(); });

    for (auto& thread : threads)
      thread.join();
  }

  boost::asio::io_context&
  query()
  {
    return *contexts[index++ % contexts.size()];
  }

private:
  template <typename T>
  using vector_ptr = std::vector<std::shared_ptr<T>>;

  vector_ptr<boost::asio::io_context> contexts;
  std::vector<std::thread> threads;
  std::atomic<std::size_t> index{0};
};
