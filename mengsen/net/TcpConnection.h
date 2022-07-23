/*
 * @Author: Mengsen.Wang
 * @Date: 2020-09-29 21:46:37
 * @Last Modified by: Mengsen.Wang
 * @Last Modified time: 2020-09-29 21:50:00
 */

#ifndef __MENGSEN_NET_TCPCONNECTION_H__
#define __MENGSEN_NET_TCPCONNECTION_H__

#include <any>
#include <atomic>

#include "mengsen/base/NonCopyable.h"
#include "mengsen/base/Types.h"
#include "mengsen/net/Buffer.h"
#include "mengsen/net/Callbacks.h"
#include "mengsen/net/InetAddress.h"

namespace mengsen {
namespace net {

class Channel;
class EventLoop;
class Socket;

/**
 * @brief: TCP connection, for both client and server usage. This is an
 * interface class, so don't expose too much details.
 */
class TcpConnection : NonCopyable,
                      public std::enable_shared_from_this<TcpConnection> {
 public:
  /**
   * Constructs a TcpConnection with a connected sockfd, user should not
   * create this object directly.
   * @param loop [EventLoop*] TcpConnection's loop
   * @param name [const std::string&] TcpConnect's name
   * @param sockfd [int] accepted sockfd
   * @param localAddr [const InetAddress&] local address
   * @param peerAddr [const InetAddress&] peer address
   */
  TcpConnection(EventLoop* loop, const std::string& name, int sockfd,
                const InetAddress& localAddr, const InetAddress& peerAddr);

  ~TcpConnection();

  /**
   * Get TcpConnection's bind loop
   * @return EventLoop*
   */
  EventLoop* getLoop() const { return loop_; }

  /**
   * Get TcpConnection's name
   * @return const std::string&
   */
  const std::string& name() const { return name_; }

  /**
   * Get TcpConnection's local address
   * @return const InetAddress&
   */
  const InetAddress& localAddress() const { return localAddr_; }

  /**
   * Get TcpConnection's peer address
   * @return const InetAddress&
   */
  const InetAddress& peerAddress() const { return peerAddr_; }

  /**
   * Get TcpConnection connected
   * @return bool
   */
  bool connected() const { return state_ == kConnected; }

  /**
   * Get TcpConnection disconnected
   * @return bool
   */
  bool disconnected() const { return state_ == kDisconnected; }

  /**
   * Get TcpInfo from TcpConnection socket_
   * @param tcp_info* return info to fill this struct
   * @return bool if successful
   */
  bool getTcpInfo(struct tcp_info*) const;

  /**
   * Get TcpInfo string from TcpConnection socket_
   * @return std::string
   */
  std::string getTcpInfoString() const;

  /**
   * Send date to TcpConnection for TcpClient usage
   * @param message [const void*] message pointer
   * @param len [int] message len
   */
  void send(const void* message, int len);

  /**
   * Send date to TcpConnection for TcpClient usage
   * @param message [const StringPiece*] message pointer
   */
  void send(const StringPiece& message);

  /**
   * Send date to TcpConnection for TcpClient usage
   * @param message [std::string&&] message pointer
   */
  void send(std::string&& message);

  /**
   * Send date to TcpConnection, for TcpClient usage, this one will swap data
   * @param message [Buffer&&] message pointer
   */
  void send(Buffer&& message);

  /**
   * Send date to TcpConnection, for TcpClient usage, this one will swap data
   * @param message [Buffer* message] message pointer
   */
  void send(Buffer* message);

  /**
   * Shutdown TcpConnection, not thread safe but in thread loop ,no simultaneous
   * calling. From Connected to kDisconnecting
   */
  void shutdown();

  /**
   * Shutdown and Force close TcpConnection after seconds, will call
   * forceClose(). Not thread safe but in thread loop, no simultaneous calling.
   * From Connected to kDisconnecting
   * @param seconds [double]
   */
  void shutdownAndForceCloseAfter(double seconds);

  /**
   * Force close TcpConnection. From Connected or kDisconnecting to
   * kDisconnecting
   */
  void forceClose();

  /**
   * Force close TcpConnection with delay seconds. Not forceCloseInLoop to avoid
   * race condition
   */
  void forceCloseWithDelay(double seconds);

  /**
   * set tcp no delay
   * @param on [bool] on or off
   */
  void setTcpNoDelay(bool on);

  /**
   * change reading_ to true, if reading already is nothing to do
   */
  void startRead();

  /**
   * change reading_ to false, if disreading already is nothing to do
   */
  void stopRead();

  /**
   * get reading_ state, OT thread safe, may race with startRead() or stopRead()
   * @return bool
   */
  bool isReading() const { return reading_; };

  /**
   * set context
   * @param context [const std::any&]
   */
  void setContext(const std::any& context) { context_ = context; }

  /**
   * get context_
   * @return const std::any&
   */
  const std::any& getContext() const { return context_; }

  /**
   * get mutable context
   * @return std::any*
   */
  std::any* getMutableContext() { return &context_; }

  /**
   * set connection callback it called when connection established
   * @param connectionCallback [const ConnectionCallback&]
   */
  void setConnectionCallback(const ConnectionCallback& cb) {
    connectionCallback_ = cb;
  }

  /**
   * set message callback it called when read event to input buffer
   * @param MessageCallback [const MessageCallback&]
   */
  void setMessageCallback(const MessageCallback& cb) { messageCallback_ = cb; }

  /**
   * set write finish callback it called when write event, if output buffer
   * clear
   * @param WriteCompleteCallback [const WriteCompleteCallback&]
   */
  void setWriteCompleteCallback(const WriteCompleteCallback& cb) {
    writeCompleteCallback_ = cb;
  }

  /**
   * set write finish callback it called when write event, if output buffer size
   * > highWaterMark, willcallback
   * @param HighWaterMarkCallback [const HighWaterMarkCallback&]
   * @param highWaterMark [int]
   */
  void setHighWaterMarkCallback(const HighWaterMarkCallback& cb,
                                size_t highWaterMark) {
    highWaterMarkCallback_ = cb;
    highWaterMark_ = highWaterMark;
  }

  /**
   * get input buffer that used in read callback
   * @return Buffer*
   */
  Buffer* inputBuffer() { return &inputBuffer_; }

  /**
   * get out buffer that used in write callback
   * @return Buffer*
   */
  Buffer* outputBuffer() { return &outputBuffer_; }

  /**
   * set close TcpConnection callback
   * @param CloseCallback [const CloseCallback&]
   */
  void setCloseCallback(const CloseCallback& cb) { closeCallback_ = cb; }

  /**
   * called when TcpServer accepts a new connection, should be called only once,
   * and call connectionCallback_, inter use only
   */
  void connectEstablished();

  /**
   * called when TcpServer has removed me from its map, should be called only
   * once and call connectionCallback_, internal use only
   */
  void connectDestroyed();

 private:
  // connection state
  enum StateE { kDisconnected, kConnecting, kConnected, kDisconnecting };

  /**
   * channel_ setReadCallback
   * @param receiveTime [Timestamp::Time_uint64_t]
   */
  void handleRead(Timestamp::Time_uint64_t receiveTime);

  /**
   * channel_ setWriteCallback
   */
  void handleWrite();

  /**
   * channel_ setCloseCallback
   */
  void handleClose();

  /**
   * channel_ setErrorCallback
   */
  void handleError();

  /**
   * send date inner function, called by loop
   * @param message [std::string&&]
   */
  void sendInLoop(std::string&& message);

  /**
   * send date inner function, called by loop
   * @param message [const StringPiece&]
   */
  void sendInLoop(const StringPiece& message);

  /**
   * send date inner function, called by loop
   * @param message [const void*]
   * @param len [size_t]
   */
  void sendInLoop(const void* message, size_t len);

  /**
   * shutdown inner function, called shutdown()
   */
  void shutdownInLoop();

  /**
   * shutdownAndForceCloseInLoop inner function, called shutdownAndForceClose()
   * @param seconds [double]
   */
  void shutdownAndForceCloseInLoop(double seconds);

  /**
   * forceCloseWithDelayInLoop inner function, called forceCloseWithDelay()
   * @param seconds [double]
   */
  void forceCloseWithDelayInLoop(double seconds);

  /**
   * forceCloseInLoop inner function, called forceClose()
   */
  void forceCloseInLoop();

  /**
   * change state inner function
   * @param state [StateE]
   */
  void setState(StateE s) { state_ = s; }

  /**
   * get state string inner function
   * @return const char*
   */
  const char* stateToString() const;

  /**
   * start read inner function, called by startRead()
   */
  void startReadInLoop();

  /**
   * start read inner function, called by startRead()
   */
  void stopReadInLoop();

  EventLoop* loop_;
  const std::string name_;
  // FIXME: if multiple thread, use atomic
  StateE state_;
  bool reading_;
  // we don't expose those classes to client.
  std::unique_ptr<Socket> socket_;
  std::unique_ptr<Channel> channel_;

  // addr
  const InetAddress localAddr_;
  const InetAddress peerAddr_;

  // callback function
  ConnectionCallback connectionCallback_;
  MessageCallback messageCallback_;
  WriteCompleteCallback writeCompleteCallback_;
  HighWaterMarkCallback highWaterMarkCallback_;
  CloseCallback closeCallback_;

  size_t highWaterMark_;  // water mark for highWaterMarkCallback_
  Buffer inputBuffer_;    // read buffer
  Buffer outputBuffer_;   // write buffer
  // FIXME: use list<Buffer> as output buffer.
  std::any context_;
  // FIXME: creationTime_, lastReceiveTime_
  //        bytesReceived_, bytesSent_
};

typedef std::shared_ptr<TcpConnection> TcpConnectionPtr;

}  // namespace net
}  // namespace mengsen

#endif