/*
 * @Date: 2020-07-22 17:52:16
 * @Author: Mengsen Wang
 * @LastEditors: Mengsen Wang
 * @LastEditTime: 2022-02-11 00:22:46
 * @FilePath: /mengsen/mengsen/base/Exception.cpp
 */

#include "mengsen/base/Exception.h"

#include "mengsen/base/CurrentThread.h"

using namespace mengsen;

Exception::Exception(std::string what)
    : message_(std::move(what)), stack_(CurrentThread::stackTrace(false)) {}
