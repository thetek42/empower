#ifndef _EMPOWER_FS_H_
#define _EMPOWER_FS_H_
#include "empower_config.h"
#if E_CONFIG_MODULE_FS

/*! e_fs **********************************************************************
 * 
 * This module provides wrappers for file system APIs.
 *
 * Module dependencies:
 *  - e_result
 *  - e_debug
 *  - e_alloc (optional)
 *  - e_log (transient; optional)
 *
 ******************************************************************************/

#if !E_CONFIG_MODULE_RESULT
# error "module e_fs depends on: e_result"
#endif /* !E_CONFIG_MODULE_RESULT */

#include <stdio.h>
#include "convey.h"
#include "e_types.h"
#include "e_alloc.h"
#include "e_debug.h"
#include "e_result.h"

/**
 * Specifies which mode to use for opening files.
 */
typedef enum {
	E_FS_OPEN_MODE_READ_ONLY, /* r */
	E_FS_OPEN_MODE_WRITE_TRUNC, /* w */
	E_FS_OPEN_MODE_WRITE_APPEND, /* a */
	E_FS_OPEN_MODE_READ_WRITE, /* r+ */
	E_FS_OPEN_MODE_READ_WRITE_TRUNC, /* w+ */
	E_FS_OPEN_MODE_READ_WRITE_APPEND, /* a+ */
} E_Fs_Open_Mode;

/**
 * Specifies the permissions of a file or directory.
 */
typedef enum {
	/* individual options */
	E_FS_PERM_USER_READ = C_PERM_USER_READ,
	E_FS_PERM_USER_WRITE = C_PERM_USER_WRITE,
	E_FS_PERM_USER_EXEC = C_PERM_USER_EXEC,
	E_FS_PERM_GROUP_READ = C_PERM_GROUP_READ,
	E_FS_PERM_GROUP_WRITE = C_PERM_GROUP_WRITE,
	E_FS_PERM_GROUP_EXEC = C_PERM_GROUP_EXEC,
	E_FS_PERM_OTHER_READ = C_PERM_OTHER_READ,
	E_FS_PERM_OTHER_WRITE = C_PERM_OTHER_WRITE,
	E_FS_PERM_OTHER_EXEC = C_PERM_OTHER_EXEC,
	/* combined options */
	E_FS_PERM_USER_RWX = C_PERM_USER_RWX,
	E_FS_PERM_GROUP_RWX = C_PERM_GROUP_RWX,
	E_FS_PERM_OTHER_RWX = C_PERM_OTHER_RWX,
} E_Fs_Permissions;

/**
 * A wrapper for a `FILE` handle.
 */
typedef struct {
	FILE *handle;
} E_File;

E_Result e_fs_read_file (const char *path, char **out, usize *len_out);
E_Result e_fs_file_open (E_File *file_out, const char *path, E_Fs_Open_Mode mode);
void e_fs_file_close (E_File *file);
E_Result e_fs_file_get_size (E_File *file, usize *size_out);
E_Result e_fs_file_get_remaining_size (E_File *file, usize *size_out);
E_Result e_fs_file_get_pos (E_File *file, usize *pos);
E_Result e_fs_file_set_pos (E_File *file, usize pos);
E_Result e_fs_file_set_pos_end (E_File *file, usize pos);
E_Result e_fs_file_rewind (E_File *file);
E_Result e_fs_file_read (E_File *file, char *out, usize max_len, usize *len_out);
E_Result e_fs_file_write (E_File *file, const char *data, usize len);
C_ATTR_FORMAT (printf, 3, 4) E_Result e_fs_file_write_fmt (E_File *file, usize *written, const char *fmt, ...);
bool e_fs_path_exists (const char *path);
bool e_fs_is_file (const char *path);
bool e_fs_is_dir (const char *path);
bool e_fs_is_link (const char *path);
E_Result e_fs_dir_create (const char *path, E_Fs_Permissions perm);

#if E_CONFIG_MODULE_ALLOC
E_Result e_fs_file_read_all (E_File *file, char **out, usize *len_out);
E_Result e_fs_file_read_remaining (E_File *file, char **out, usize *len_out);
#endif /* E_CONFIG_MODULE_ALLOC */

#endif /* E_CONFIG_MODULE_FS */
#endif /* _EMPOWER_FS_H_ */