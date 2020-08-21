/*
 * @Author: Mengsen.Wang
 * @Date: 2020-08-21 16:32:45
 * @Last Modified by: Mengsen.Wang
 * @Last Modified time: 2020-08-21 17:14:00
 */

#ifndef __MENGSEN_NET_ENDIAN_H__
#define __MENGSEN_NET_ENDIAN_H__

#include <endian.h>
#include <stdint.h>

namespace mengsen {

namespace net {

namespace sockets {

#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Wconversion"
#pragma GCC diagnostic ignored "-Wold-style-cast"

inline uint64_t hostToNetwork64(uint64_t host64) { return htobe64(host64); }
inline uint32_t hostToNetwork32(uint32_t host32) { return htobe32(host32); }
inline uint16_t hostToNetwork16(uint16_t host16) { return htobe16(host16); }
inline uint64_t networkToHost64(uint64_t net64) { return be64toh(net64); }
inline uint32_t networkToHost32(uint32_t net32) { return be32toh(net32); }
inline uint16_t networkToHost16(uint16_t net16) { return be16toh(net16); }

#pragma GCC diagnostic pop

}  // namespace sockets

}  // namespace net

}  // namespace mengsen

#endif  // __MENGSEN_NET_ENDIAN_H__