#include "empower.h"

#if E_CONFIG_MODULE_RESULT

/**
 * Convert an `E_Result` \res into a human-readable string. If \res is not a
 * valid result, `nullptr` is returned.
 */
const char *
e_result_to_str (E_Result res)
{
	switch (res) {
	case E_OK:                            return "ok";
	case E_ERR_NOT_PERMITTED:             return "not permitted";
	case E_ERR_FILE_NOT_FOUND:            return "file not found";
	case E_ERR_PROCESS_NOT_FOUND:         return "process not found";
	case E_ERR_INTERRUPTED:               return "interrupted";
	case E_ERR_IO_ERROR:                  return "io error";
	case E_ERR_ADDRESS_NOT_FOUND:         return "address not found";
	case E_ERR_TOO_BIG:                   return "too big";
	case E_ERR_FORMAT_ERROR:              return "format error";
	case E_ERR_BAD_FILE:                  return "bad file";
	case E_ERR_NO_CHILD_PROCESS:          return "no child process";
	case E_ERR_TRY_AGAIN:                 return "try again";
	case E_ERR_OUT_OF_MEMORY:             return "out of memory";
	case E_ERR_PERMISSION_DENIED:         return "permission denied";
	case E_ERR_BAD_ADDRESS:               return "bad address";
	case E_ERR_NOT_A_BLOCK_DEVICE:        return "not a block device";
	case E_ERR_BUSY:                      return "device or resource busy";
	case E_ERR_EXISTS:                    return "file exists";
	case E_ERR_CROSS_DEVICE_LINK:         return "cross device link";
	case E_ERR_DEVICE_NOT_FOUND:          return "device not found";
	case E_ERR_NOT_A_DIRECTORY:           return "not a directory";
	case E_ERR_IS_A_DIRECTORY:            return "is a directory";
	case E_ERR_INVALID_ARGUMENT:          return "invalid argument";
	case E_ERR_TABLE_OVERFLOW:            return "table overflow";
	case E_ERR_TOO_MANY_FILES:            return "too many files";
	case E_ERR_NOT_A_TTY:                 return "not a tty";
	case E_ERR_TEXT_FILE_BUSY:            return "text file busy";
	case E_ERR_FILE_TOO_LARGE:            return "file too large";
	case E_ERR_NO_SPACE_LEFT:             return "no spacel eft";
	case E_ERR_ILLEGAL_SEEK:              return "illegal seek";
	case E_ERR_READ_ONLY:                 return "read only";
	case E_ERR_TOO_MANY_LINKS:            return "too many links";
	case E_ERR_BROKEN_PIPE:               return "broken pipe";
	case E_ERR_MATH_DOMAIN:               return "math domain error";
	case E_ERR_MATH_RANGE:                return "math range error";
	case E_ERR_DEADLOCK:                  return "deadlock";
	case E_ERR_FILE_NAME_TOO_LONG:        return "file name too long";
	case E_ERR_NO_LOCKS_AVAILABLE:        return "no locks available";
	case E_ERR_INVALID_SYSCALL:           return "invalid syscall";
	case E_ERR_NOT_EMPTY:                 return "not empty";
	case E_ERR_SYMLINK_LOOP:              return "symlink loop";
	case E_ERR_NO_MESSAGE:                return "no message";
	case E_ERR_ID_REMOVED:                return "id removed";
	case E_ERR_OUT_OF_RANGE:              return "out of range";
	case E_ERR_L2_NOT_SYNCHRONIZED:       return "L2 not synchronized";
	case E_ERR_L3_HALTED:                 return "L3 halted";
	case E_ERR_L3_RESET:                  return "L3 reset";
	case E_ERR_LINK_OUT_OF_RANGE:         return "link out of range";
	case E_ERR_DRIVER_NOT_ATTACHED:       return "driver not attached";
	case E_ERR_NO_CSI_AVAILABLE:          return "no csi available";
	case E_ERR_L2_HALTED:                 return "L2 halted";
	case E_ERR_BAD_EXCHANGE:              return "bad exchange";
	case E_ERR_BAD_REQUEST:               return "bad request";
	case E_ERR_EXCHANGE_FULL:             return "exchange full";
	case E_ERR_NO_ANODE:                  return "no anode";
	case E_ERR_INVALID_REQUEST_CODE:      return "invalid request code";
	case E_ERR_INVALID_SLOT:              return "invalid slot";
	case E_ERR_INVALID_FONT_FILE:         return "invalid font file";
	case E_ERR_NOT_A_STREAM:              return "not a stream";
	case E_ERR_NOT_AVAILABLE:             return "not available";
	case E_ERR_EXPIRED:                   return "expired";
	case E_ERR_OUT_OF_STREAM_RESOURCES:   return "out of stream resources";
	case E_ERR_NOT_ON_NETWORK:            return "not on network";
	case E_ERR_NOT_INSTALLED:             return "not installed";
	case E_ERR_IS_REMOTE:                 return "is remote";
	case E_ERR_SEVERED_LINK:              return "severed link";
	case E_ERR_ADVERTISE:                 return "advertise error";
	case E_ERR_SRMOUNT:                   return "srmount error";
	case E_ERR_COMMUNICATION:             return "communication error";
	case E_ERR_PROTOCOL:                  return "protocol error";
	case E_ERR_MULTIHOP:                  return "multihop";
	case E_ERR_RFS_SPECIFIC:              return "rfs specific error";
	case E_ERR_INVALID_MESSAGE:           return "invalid message";
	case E_ERR_OVERFLOW:                  return "overflow";
	case E_ERR_NOT_UNIQUE:                return "not unique";
	case E_ERR_BAD_FILE_DESCRIPTOR:       return "bad file descriptor";
	case E_ERR_REMOTE_CHANGED:            return "remote changed";
	case E_ERR_LIB_ACCESS:                return "library access error";
	case E_ERR_LIB_CORRUPTED:             return "library corrupted";
	case E_ERR_LIB_SECTION_CORRUPTED:     return "library section corrupted";
	case E_ERR_TOO_MANY_LIBS:             return "too many libraries";
	case E_ERR_CANNOT_EXEC_LIB:           return "cannot execute library";
	case E_ERR_ILLEGAL_BYTES:             return "illegal bytes";
	case E_ERR_RESTART_SYSCALL:           return "restart syscall";
	case E_ERR_STREAM_PIPE:               return "stream pipe error";
	case E_ERR_TOO_MANY_USERS:            return "too many users";
	case E_ERR_NOT_A_SOCKET:              return "not a socket";
	case E_ERR_NO_DESTINATION:            return "no destination";
	case E_ERR_MESSAGE_TOO_LONG:          return "message too long";
	case E_ERR_INVALID_PROTOCOL_TYPE:     return "invalid protocol type";
	case E_ERR_PROTOCOL_NOT_AVAILABLE:    return "protocol not available";
	case E_ERR_PROTOCOL_NOT_SUPPORTED:    return "protocol not supported";
	case E_ERR_SOCKET_TYPE_NOT_SUPPORTED: return "socket type not supported";
	case E_ERR_OPERATION_NOT_SUPPORTED:   return "operation not supported";
	case E_ERR_BAD_PROTOCOL_FAMILY:       return "bad protocol family";
	case E_ERR_BAD_ADDRESS_FAMILY:        return "bad address family";
	case E_ERR_ADDRESS_IN_USE:            return "address in use";
	case E_ERR_ADDRESS_NOT_AVAILABLE:     return "address not available";
	case E_ERR_NETWORK_DOWN:              return "network down";
	case E_ERR_NETWORK_UNREACH:           return "network unreachable";
	case E_ERR_NETWORK_RESET:             return "network reset";
	case E_ERR_CONNECTION_ABORTED:        return "connection aborted";
	case E_ERR_CONNECTION_RESET:          return "connection reset";
	case E_ERR_NO_BUFFER_SPACE:           return "no buffer space";
	case E_ERR_ALREADY_CONNECTED:         return "already connected";
	case E_ERR_NOT_CONNECTED:             return "not connected";
	case E_ERR_IS_SHUTDOWN:               return "is shutdown";
	case E_ERR_TOO_MANY_REFERENCES:       return "too many references";
	case E_ERR_TIMEOUT:                   return "timeout";
	case E_ERR_CONNECTION_REFUSED:        return "connection refused";
	case E_ERR_HOST_DOWN:                 return "host down";
	case E_ERR_HOST_UNREACHABLE:          return "host unreachable";
	case E_ERR_ALREADY_IN_PROGRESS:       return "already in progress";
	case E_ERR_IN_PROGRESS:               return "in progress";
	case E_ERR_STALE_FILE_HANDLE:         return "stale file handle";
	case E_ERR_NEEDS_CLEANING:            return "needs cleaning";
	case E_ERR_NOT_A_NAMED_TYPE_FILE:     return "not a named type file";
	case E_ERR_NO_SEMAPHORES_AVAILABLE:   return "no semaphores available";
	case E_ERR_IS_A_NAMED_TYPE_FILE:      return "is a named type file";
	case E_ERR_REMOTE_IO:                 return "remote io error";
	case E_ERR_QUOTA_EXCEEDED:            return "quota exceeded";
	case E_ERR_NO_MEDIUM:                 return "no medium";
	case E_ERR_WRONG_MEDIUM_TYPE:         return "wrong medium type";
	case E_ERR_CANCELED:                  return "canceled";
	case E_ERR_KEY_NOT_AVAILABLE:         return "key not available";
	case E_ERR_KEY_EXPIRED:               return "key expired";
	case E_ERR_KEY_REVOKED:               return "key revoked";
	case E_ERR_KEY_REJECTED:              return "key rejected";
	case E_ERR_OWNER_DIED:                return "owner died";
	case E_ERR_NOT_RECOVERABLE:           return "not recoverable";
	case E_ERR_RFKILL:                    return "rfkill error";
	case E_ERR_HARDWARE_ERROR:            return "hardware error";
	case E_ERR_FAIL:                      return "failure";
	default:                              return nullptr;
	}
}

#endif /* E_CONFIG_MODULE_RESULT */
