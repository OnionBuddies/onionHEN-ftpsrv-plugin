#include "ftp_i18n.h"

#include <stdatomic.h>
#include <string.h>

extern int sceSystemServiceParamGetInt(int param_id, int *value);

typedef struct ftp_tr_entry {
    const char *key;
    const char *en;
    const char *zh_hans;
} ftp_tr_entry;

static const ftp_tr_entry kTranslations[] = {
    {"title", "FTP Server", "FTP 服务器"},
    {"about_title", "About", "关于此插件"},
    {"about_desc",
     "Managed PS5 FTP server. Enable the listener, choose a TCP port, or restart the service.",
     "由 OnionHEN 管理的 PS5 FTP 服务。可启用监听、设置 TCP 端口，或重启服务。"},
    {"enabled_title", "Enabled", "启用 FTP 服务"},
    {"enabled_desc",
     "Start the FTP listener on the configured TCP port.",
     "开启后在配置的 TCP 端口上监听 FTP 连接。"},
    {"port_title", "TCP Port", "TCP 端口"},
    {"port_desc",
     "Listener port (1-65535). Changing the port restarts the server.",
     "FTP 监听端口 (1-65535)。修改端口会重启服务。"},
    {"restart_title", "Restart Server", "重启 FTP 服务"},
    {"restart_desc",
     "Stop and start the FTP listener on the current port.",
     "在当前端口停止并重新启动 FTP 监听器。"},
};

#define TRANSLATION_COUNT (sizeof(kTranslations) / sizeof(kTranslations[0]))

static atomic_int g_current_lang = ATOMIC_VAR_INIT(FTP_LANG_EN);
static atomic_int g_initialized = ATOMIC_VAR_INIT(0);

bool ftp_i18n_refresh(void) {
    int sys_lang = 1;
    if (sceSystemServiceParamGetInt(1, &sys_lang) < 0) {
        if (!atomic_load_explicit(&g_initialized, memory_order_relaxed)) {
            atomic_store_explicit(&g_current_lang, FTP_LANG_EN, memory_order_relaxed);
            atomic_store_explicit(&g_initialized, 1, memory_order_relaxed);
            return true;
        }
        return false;
    }

    ftp_lang_t resolved = FTP_LANG_EN;
    /* 11 = Simplified Chinese, 10 = Traditional Chinese */
    if (sys_lang == 11 || sys_lang == 10) {
        resolved = FTP_LANG_ZH_HANS;
    } else {
        resolved = FTP_LANG_EN;
    }

    ftp_lang_t old_lang =
        (ftp_lang_t)atomic_load_explicit(&g_current_lang, memory_order_relaxed);
    int was_inited = atomic_load_explicit(&g_initialized, memory_order_relaxed);

    atomic_store_explicit(&g_current_lang, resolved, memory_order_relaxed);
    atomic_store_explicit(&g_initialized, 1, memory_order_relaxed);

    return (!was_inited) || (old_lang != resolved);
}

ftp_lang_t ftp_i18n_get_lang(void) {
    if (!atomic_load_explicit(&g_initialized, memory_order_relaxed)) {
        ftp_i18n_refresh();
    }
    return (ftp_lang_t)atomic_load_explicit(&g_current_lang, memory_order_relaxed);
}

const char *ftp_tr(const char *key) {
    if (!key) return "";
    ftp_lang_t lang = ftp_i18n_get_lang();
    for (size_t i = 0; i < TRANSLATION_COUNT; ++i) {
        if (strcmp(kTranslations[i].key, key) == 0) {
            return (lang == FTP_LANG_ZH_HANS) ? kTranslations[i].zh_hans
                                              : kTranslations[i].en;
        }
    }
    return key;
}
