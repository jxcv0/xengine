#ifndef ASYNCRESULT_H_
#define ASYNCRESULT_H_

/**
 * @brief object that provides a mechanism to return the result of asynchronous
 *        operations between threads.
 * @tparam T The type of the result.
 */
template <typename T>
class Async {
 public:
  Async(T* t) : mp_result(t) {}
  auto wait() {}
 private:
  // TODO
  // How best to allocate this?
  // std::allocate_shared?
  T* mp_result;
};

#endif // ASYNCRESULT_H_
