#ifndef _EMPOWER_RESULT_H_
#define _EMPOWER_RESULT_H_
#ifndef _EMPOWER_H_
#error "do not include this file directly, only include empower.h"
#endif /* _EMPOWER_H_ */
#if E_CONFIG_MODULE_RESULT

typedef isize e_result_t;

enum {
	E_OK                            = 0,
	E_ERR_NOT_PERMITTED             = -EPERM,
	E_ERR_NO_SUCH_FILE              = -ENOENT,
	E_ERR_NO_SUCH_PROCESS           = -ESRCH,
	E_ERR_INTERRUPTED               = -EINTR,
	E_ERR_IO_ERROR                  = -EIO,
	E_ERR_NO_SUCH_ADDRESS           = -ENXIO,
	E_ERR_TOO_BIG                   = -E2BIG,
	E_ERR_FORMAT_ERROR              = -ENOEXEC,
	E_ERR_BAD_FILE                  = -EBADF,
	E_ERR_NO_CHILD_PROCESS          = -ECHILD,
	E_ERR_TRY_AGAIN                 = -EAGAIN,
	E_ERR_OUT_OF_MEMORY             = -ENOMEM,
	E_ERR_PERMISSION_DENIED         = -EACCES,
	E_ERR_BAD_ADDRESS               = -EFAULT,
	E_ERR_NOT_A_BLOCK_DEVICE        = -ENOTBLK,
	E_ERR_BUSY                      = -EBUSY,
	E_ERR_EXISTS                    = -EEXIST,
	E_ERR_CROSS_DEVICE_LINK         = -EXDEV,
	E_ERR_NO_SUCH_DEVICE            = -ENODEV,
	E_ERR_NOT_A_DIRECTORY           = -ENOTDIR,
	E_ERR_IS_A_DIRECTORY            = -EISDIR,
	E_ERR_INVALID_ARGUMENT          = -EINVAL,
	E_ERR_TABLE_OVERFLOW            = -ENFILE,
	E_ERR_TOO_MANY_FILES            = -EMFILE,
	E_ERR_NOT_A_TTY                 = -ENOTTY,
	E_ERR_TEXT_FILE_BUSY            = -ETXTBSY,
	E_ERR_FILE_TOO_LARGE            = -EFBIG,
	E_ERR_NO_SPACE_LEFT             = -ENOSPC,
	E_ERR_ILLEGAL_SEEK              = -ESPIPE,
	E_ERR_READ_ONLY                 = -EROFS,
	E_ERR_TOO_MANY_LINKS            = -EMLINK,
	E_ERR_BROKEN_PIPE               = -EPIPE,
	E_ERR_MATH_DOMAIN               = -EDOM,
	E_ERR_MATH_RANGE                = -ERANGE,
	E_ERR_DEADLOCK                  = -EDEADLK,
	E_ERR_FILE_NAME_TOO_LONG        = -ENAMETOOLONG,
	E_ERR_NO_LOCKS_AVAILABLE        = -ENOLCK,
	E_ERR_INVALID_SYSCALL           = -ENOSYS,
	E_ERR_NOT_EMPTY                 = -ENOTEMPTY,
	E_ERR_SYMLINK_LOOP              = -ELOOP,
	E_ERR_WOULD_BLOCK               = -EWOULDBLOCK,
	E_ERR_NO_MESSAGE                = -ENOMSG,
	E_ERR_ID_REMOVED                = -EIDRM,
	E_ERR_OUT_OF_RANGE              = -ECHRNG,
	E_ERR_L2_NOT_SYNCHRONIZED       = -EL2NSYNC,
	E_ERR_L3_HALTED                 = -EL3HLT,
	E_ERR_L3_RESET                  = -EL3RST,
	E_ERR_LINK_OUT_OF_RANGE         = -ELNRNG,
	E_ERR_DRIVER_NOT_ATTACHED       = -EUNATCH,
	E_ERR_NO_CSI_AVAILABLE          = -ENOCSI,
	E_ERR_L2_HALTED                 = -EL2HLT,
	E_ERR_BAD_EXCHANGE              = -EBADE,
	E_ERR_BAD_REQUEST               = -EBADR,
	E_ERR_EXCHANGE_FULL             = -EXFULL,
	E_ERR_NO_ANODE                  = -ENOANO,
	E_ERR_INVALID_REQUEST_CODE      = -EBADRQC,
	E_ERR_INVALID_SLOT              = -EBADSLT,
	E_ERR_DEADLOCK_2                = -EDEADLOCK,
	E_ERR_INVALID_FONT_FILE         = -EBFONT,
	E_ERR_NOT_A_STREAM              = -ENOSTR,
	E_ERR_NOT_AVAILABLE             = -ENODATA,
	E_ERR_EXPIRED                   = -ETIME,
	E_ERR_OUT_OF_STREAM_RESOURCES   = -ENOSR,
	E_ERR_NOT_ON_NETWORK            = -ENONET,
	E_ERR_NOT_INSTALLED             = -ENOPKG,
	E_ERR_IS_REMOTE                 = -EREMOTE,
	E_ERR_SEVERED_LINK              = -ENOLINK,
	E_ERR_ADVERTISE                 = -EADV,
	E_ERR_SRMOUNT                   = -ESRMNT,
	E_ERR_COMMUNICATION             = -ECOMM,
	E_ERR_PROTOCOL                  = -EPROTO,
	E_ERR_MULTIHOP                  = -EMULTIHOP,
	E_ERR_RFS_SPECIFIC              = -EDOTDOT,
	E_ERR_INVALID_MESSAGE           = -EBADMSG,
	E_ERR_OVERFLOW                  = -EOVERFLOW,
	E_ERR_NOT_UNIQUE                = -ENOTUNIQ,
	E_ERR_BAD_FILE_DESCRIPTOR       = -EBADFD,
	E_ERR_REMOTE_CHANGED            = -EREMCHG,
	E_ERR_LIB_ACCESS                = -ELIBACC,
	E_ERR_LIB_CORRUPTED             = -ELIBBAD,
	E_ERR_LIB_SECTION_CORRUPTED     = -ELIBSCN,
	E_ERR_TOO_MANY_LIBS             = -ELIBMAX,
	E_ERR_CANNOT_EXEC_LIB           = -ELIBEXEC,
	E_ERR_ILLEGAL_BYTES             = -EILSEQ,
	E_ERR_RESTART_SYSCALL           = -ERESTART,
	E_ERR_STREAM_PIPE               = -ESTRPIPE,
	E_ERR_TOO_MANY_USERS            = -EUSERS,
	E_ERR_NOT_A_SOCKET              = -ENOTSOCK,
	E_ERR_NO_DESTINATION            = -EDESTADDRREQ,
	E_ERR_MESSAGE_TOO_LONG          = -EMSGSIZE,
	E_ERR_INVALID_PROTOCOL_TYPE     = -EPROTOTYPE,
	E_ERR_PROTOCOL_NOT_AVAILABLE    = -ENOPROTOOPT,
	E_ERR_PROTOCOL_NOT_SUPPORTED    = -EPROTONOSUPPORT,
	E_ERR_SOCKET_TYPE_NOT_SUPPORTED = -ESOCKTNOSUPPORT,
	E_ERR_OPERATION_NOT_SUPPORTED   = -EOPNOTSUPP,
	E_ERR_BAD_PROTOCOL_FAMILY       = -EPFNOSUPPORT,
	E_ERR_BAD_ADDRESS_FAMILY        = -EAFNOSUPPORT,
	E_ERR_ADDRESS_IN_USE            = -EADDRINUSE,
	E_ERR_ADDRESS_NOT_AVAILABLE     = -EADDRNOTAVAIL,
	E_ERR_NETWORK_DOWN              = -ENETDOWN,
	E_ERR_NETWORK_UNREACH           = -ENETUNREACH,
	E_ERR_NETWORK_RESET             = -ENETRESET,
	E_ERR_CONNECTION_ABORTED        = -ECONNABORTED,
	E_ERR_CONNECTION_RESET          = -ECONNRESET,
	E_ERR_NO_BUFFER_SPACE           = -ENOBUFS,
	E_ERR_ALREADY_CONNECTED         = -EISCONN,
	E_ERR_NOT_CONNECTED             = -ENOTCONN,
	E_ERR_IS_SHUTDOWN               = -ESHUTDOWN,
	E_ERR_TOO_MANY_REFERENCES       = -ETOOMANYREFS,
	E_ERR_TIMEOUT                   = -ETIMEDOUT,
	E_ERR_CONNECTION_REFUSED        = -ECONNREFUSED,
	E_ERR_HOST_DOWN                 = -EHOSTDOWN,
	E_ERR_HOST_UNREACHABLE          = -EHOSTUNREACH,
	E_ERR_ALREADY_IN_PROGRESS       = -EALREADY,
	E_ERR_IN_PROGRESS               = -EINPROGRESS,
	E_ERR_STALE_FILE_HANDLE         = -ESTALE,
	E_ERR_NEEDS_CLEANING            = -EUCLEAN,
	E_ERR_NOT_A_NAMED_TYPE_FILE     = -ENOTNAM,
	E_ERR_NO_SEMAPHORES_AVAILABLE   = -ENAVAIL,
	E_ERR_IS_A_NAMED_TYPE_FILE      = -EISNAM,
	E_ERR_REMOTE_IO                 = -EREMOTEIO,
	E_ERR_QUOTA_EXCEEDED            = -EDQUOT,
	E_ERR_NO_MEDIUM                 = -ENOMEDIUM,
	E_ERR_WRONG_MEDIUM_TYPE         = -EMEDIUMTYPE,
	E_ERR_CANCELED                  = -ECANCELED,
	E_ERR_KEY_NOT_AVAILABLE         = -ENOKEY,
	E_ERR_KEY_EXPIRED               = -EKEYEXPIRED,
	E_ERR_KEY_REVOKED               = -EKEYREVOKED,
	E_ERR_KEY_REJECTED              = -EKEYREJECTED,
	E_ERR_OWNER_DIED                = -EOWNERDEAD,
	E_ERR_NOT_RECOVERABLE           = -ENOTRECOVERABLE,
	E_ERR_RFKILL                    = -ERFKILL,
	E_ERR_HARDWARE_ERROR            = -EHWPOISON
};

#endif /* E_CONFIG_MODULE_RESULT */
#endif /* _EMPOWER_RESULT_H_ */
