///  @file Singleton.hpp
///  @author Tyler Mayoff
///  @brief Simple singleton class.
///  https://www.modernescpp.com/index.php/thread-safe-initialization-of-a-singleton
///  @version 0.1
///  @date 2021-06-25
///
///  @copyright Copyright (c) 2021
///

#ifndef COMMON_SINGLETON_HPP_
#define COMMON_SINGLETON_HPP_

template <typename T> class Singleton {
public:
  static T &GetInstance() {
    static T instance;
    return instance;
  }

protected:
  static T instance;

  Singleton() noexcept = default;
  Singleton(const Singleton &) = delete;
  Singleton &operator=(const Singleton &) = delete;
  virtual ~Singleton() = default;
};

#endif // COMMON_SINGLETON_HPP_
