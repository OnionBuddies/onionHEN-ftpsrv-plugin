#pragma once

#include <pthread.h>
#include <stdint.h>

typedef struct ftp_service {
    pthread_mutex_t mutex;
    pthread_t thread;
    uint16_t port;
    int initialized;
    int running;
    int thread_created;
} ftp_service;

int ftp_service_init(ftp_service *service, uint16_t port);
void ftp_service_destroy(ftp_service *service);
int ftp_service_start(ftp_service *service);
void ftp_service_stop(ftp_service *service);
int ftp_service_reconfigure(ftp_service *service, uint16_t port);
int ftp_service_restart(ftp_service *service);
int ftp_service_running(ftp_service *service);
uint16_t ftp_service_port(ftp_service *service);
