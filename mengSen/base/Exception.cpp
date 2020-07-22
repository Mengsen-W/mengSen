/*
 * @Author: Mengsen.Wang
 * @Date: 2020-07-22 17:52:16
 * @Last Modified by: Mengsen.Wang
 * @Last Modified time: 2020-07-22 17:54:43
 */

#include "Exception.h"

#include "../thread/CurrentThread.h"

using namespace mengsen;

Exception::Exception(std::string what)
    : message_(std::move(what)), stack_(CurrentThread::stackTrace(false)) {}
