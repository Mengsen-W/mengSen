/*
 * @Author: Mengsen.Wang
 * @Date: 2020-08-31 20:19:15
 * @Last Modified by: Mengsen.Wang
 * @Last Modified time: 2020-08-31 20:27:32
 */

#ifndef __MENGSEN_NET_CALLBACK_H__
#define __MENGSEN_NET_CALLBACK_H__

#include <functional>
#include <memory>

namespace mengsen {

// should really belong to base/Types.h, but <memory> is not included there

template <typename T>
inline T* get_pointer(const std::shared_ptr<T>& ptr) {
  return ptr.get();
}

template <typename T>
inline T* get_pointer(const std::unique_ptr<T>& ptr) {
  return ptr.get();
}

template <typename To, typename From>
inline ::std::shared_ptr<To> down_pointer_cast(
    const ::std::shared_ptr<From>& f) {
  if (false) {
    implicit_cast<From*, To*>(0);
  }
#ifndef NDEBUG
  assert(f == NULL || dynamic_cast<To*>(get_pointer(f)) != NULL);
#endif return ::std::static_pointer_cast < To>(f);
}

namespace net {

class Buffer;
class TcpConnection;

typedef std::shared_ptr<TcpConnection> TcpConnectionPtr;
typedef std::function<void()> TimerCallback;
typedef std::function<void(const TcpConnectionPtr&)> ConnectionCallback;
typedef std::function<void(const TcpConnectionPtr&)> CloseCallback;
typedef std::function<void(const TcpConnectionPtr&)> WriteCompleteCallback;
typedef std::function<void(const TcpConnectionPtr&, size_t)>
    HighWaterMarkCallback;
// the data has been read to (buf, len)
typedef std::function<void(const TcpConnectionPtr&, Buffer*, uint64_t)>
    MessageCallback;
void defaultConnectionCallback(const TcpConnectionPtr& conn);
void defaultMessageCallback(const TcpConnectionPtr& conn, Buffer* buffer,
                            uint64_t receiveTime);

}  // namespace net

}  // namespace mengsen

#endif  // __MENGSEN_NET_CALLBACK_H__