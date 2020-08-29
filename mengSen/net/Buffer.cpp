/*
 * @Author: Mengsen.Wang
 * @Date: 2020-08-29 18:21:54
 * @Last Modified by: Mengsen.Wang
 * @Last Modified time: 2020-08-29 18:35:58
 */

#include "Buffer.h"

#include <error.h>
#include <sys/uio.h>

#include "SocketsOps.h"

namespace mengsen {

namespace net {

const char Buffer::kCRLF[] = "\r\n";

}

}  // namespace mengsen