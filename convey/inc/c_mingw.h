#ifndef _CONVEY_POSIX_H_
#define _CONVEY_POSIX_H_

#define _XOPEN_SOURCE 600
#define _POSIX_C_SOURCE 200809L

#include <sys/types.h>

typedef ssize_t __c_ssize_t;

#define ENOTBLK		200
#define ECHRNG		201
#define EL2NSYNC	202
#define EL3HLT		203
#define EL3RST		204
#define ELNRNG		205
#define EUNATCH		206
#define ENOCSI		207
#define EL2HLT		208
#define EBADE		209
#define EBADR		210
#define EXFULL		211
#define ENOANO		212
#define EBADRQC		213
#define EBADSLT		214
#define EBFONT		215
#define ENONET		216
#define ENOPKG		217
#define EREMOTE		218
#define EADV		219
#define ESRMNT		220
#define ECOMM		221
#define EMULTIHOP	222
#define EDOTDOT		223
#define ENOTUNIQ	224
#define EBADFD		225
#define EREMCHG		226
#define ELIBACC		227
#define ELIBBAD		228
#define ELIBSCN		229
#define ELIBMAX		230
#define ELIBEXEC	231
#define ERESTART	232
#define ESTRPIPE	233
#define EUSERS		234
#define ESOCKTNOSUPPORT	235
#define EPFNOSUPPORT	236
#define ESHUTDOWN	237
#define ETOOMANYREFS	238
#define EHOSTDOWN	239
#define ESTALE		240
#define EUCLEAN		241
#define ENOTNAM		242
#define ENAVAIL		243
#define EISNAM		244
#define EREMOTEIO	245
#define EDQUOT		246
#define ENOMEDIUM	247
#define EMEDIUMTYPE	248
#define ENOKEY		249
#define EKEYEXPIRED	250
#define EKEYREVOKED	251
#define EKEYREJECTED	252
#define ERFKILL		253
#define EHWPOISON	254

#endif /* _CONVEY_POSIX_H_ */
