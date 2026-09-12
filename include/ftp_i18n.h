#pragma once

#include <stdbool.h>

#ifdef __cplusplus
extern "C" {
#endif

typedef enum ftp_lang {
    FTP_LANG_EN = 0,
    FTP_LANG_ZH_HANS = 1,
} ftp_lang_t;

/**
 * Initialize / refresh the current system language.
 * Returns true if language changed since last check.
 */
bool ftp_i18n_refresh(void);

/**
 * Get current active language.
 */
ftp_lang_t ftp_i18n_get_lang(void);

/**
 * Translate a key to current language string.
 */
const char *ftp_tr(const char *key);

#ifdef __cplusplus
}
#endif
