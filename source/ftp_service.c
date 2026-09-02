#include "ftp_service.h"

#include <stddef.h>
#include <string.h>
#include <unistd.h>

#include "log.h"
#include "srv.h"

#define LISTENER_READY_WAIT_MS 2000

static void *ftp_thread_main(void *argument) {
    ftp_service *service = (ftp_service *)argument;
    pthread_mutex_lock(&service->mutex);
    const uint16_t port = service->port;
    pthread_mutex_unlock(&service->mutex);

    const int result = ftp_serve(port, 1);

    pthread_mutex_lock(&service->mutex);
    service->running = 0;
    pthread_mutex_unlock(&service->mutex);

    if (result == FTP_SERVE_BIND_FAILED) {
        FTP_LOG_PRINTF("ftpsrv failed to bind TCP %u\n", (unsigned)port);
    } else if (result < 0) {
        FTP_LOG_PRINTF("ftpsrv stopped with error %d\n", result);
    } else {
        FTP_LOG_PRINTF("ftpsrv stopped on TCP %u\n", (unsigned)port);
    }
    return NULL;
}

static int wait_for_listener(ftp_service *service) {
    for (int waited = 0; waited < LISTENER_READY_WAIT_MS; waited += 50) {
        if (ftp_server_is_listening()) return 1;

        pthread_mutex_lock(&service->mutex);
        const int active = service->running;
        pthread_mutex_unlock(&service->mutex);
        if (!active) return 0;
        usleep(50 * 1000);
    }
    return ftp_server_is_listening() != 0;
}

int ftp_service_init(ftp_service *service, uint16_t port) {
    if (!service || port == 0) return 0;
    memset(service, 0, sizeof(*service));
    if (pthread_mutex_init(&service->mutex, NULL) != 0) return 0;
    service->port = port;
    service->initialized = 1;
    return 1;
}

void ftp_service_stop(ftp_service *service) {
    if (!service || !service->initialized) return;

    pthread_t thread = {0};
    int join_thread = 0;
    pthread_mutex_lock(&service->mutex);
    if (service->thread_created) {
        ftp_server_stop();
        thread = service->thread;
        join_thread = 1;
    }
    pthread_mutex_unlock(&service->mutex);

    if (join_thread) pthread_join(thread, NULL);

    pthread_mutex_lock(&service->mutex);
    service->running = 0;
    service->thread_created = 0;
    pthread_mutex_unlock(&service->mutex);
}

void ftp_service_destroy(ftp_service *service) {
    if (!service || !service->initialized) return;
    ftp_service_stop(service);
    pthread_mutex_destroy(&service->mutex);
    memset(service, 0, sizeof(*service));
}

int ftp_service_start(ftp_service *service) {
    if (!service || !service->initialized) return 0;
    ftp_service_stop(service);

    pthread_mutex_lock(&service->mutex);
    service->running = 1;
    ftp_server_prepare();
    const int result = pthread_create(
        &service->thread, NULL, ftp_thread_main, service);
    if (result == 0) {
        service->thread_created = 1;
    } else {
        service->running = 0;
    }
    pthread_mutex_unlock(&service->mutex);

    if (result != 0 || !wait_for_listener(service)) {
        ftp_service_stop(service);
        return 0;
    }
    return 1;
}

int ftp_service_reconfigure(ftp_service *service, uint16_t port) {
    if (!service || !service->initialized || port == 0) return 0;

    pthread_mutex_lock(&service->mutex);
    const uint16_t previous_port = service->port;
    const int was_running = service->running;
    if (!was_running) service->port = port;
    pthread_mutex_unlock(&service->mutex);
    if (!was_running) return 1;

    ftp_service_stop(service);
    pthread_mutex_lock(&service->mutex);
    service->port = port;
    pthread_mutex_unlock(&service->mutex);
    if (ftp_service_start(service)) return 1;

    pthread_mutex_lock(&service->mutex);
    service->port = previous_port;
    pthread_mutex_unlock(&service->mutex);
    (void)ftp_service_start(service);
    return 0;
}

int ftp_service_restart(ftp_service *service) {
    if (!service || !service->initialized) return 0;
    return ftp_service_start(service);
}

int ftp_service_running(ftp_service *service) {
    if (!service || !service->initialized) return 0;
    pthread_mutex_lock(&service->mutex);
    const int running = service->running;
    pthread_mutex_unlock(&service->mutex);
    return running && ftp_server_is_listening();
}

uint16_t ftp_service_port(ftp_service *service) {
    if (!service || !service->initialized) return 0;
    pthread_mutex_lock(&service->mutex);
    const uint16_t port = service->port;
    pthread_mutex_unlock(&service->mutex);
    return port;
}
