/* Copyright (C) 2025 John Törnblom

This program is free software; you can redistribute it and/or modify it
under the terms of the GNU General Public License as published by the
Free Software Foundation; either version 3, or (at your option) any
later version.

This program is distributed in the hope that it will be useful,
but WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
GNU General Public License for more details.

You should have received a copy of the GNU General Public License
along with this program; see the file COPYING. If not, see
<http://www.gnu.org/licenses/>.  */

#include <stdint.h>

#ifdef __cplusplus
extern "C" {
#endif

#pragma once

#define FTP_SERVE_BIND_FAILED (-2)

int ftp_serve(uint16_t port, int notify_user);

/* Reset the stop latch before creating a new serving thread. */
void ftp_server_prepare(void);

/* Interrupt a serving loop blocked in accept().  The caller still owns the
 * serving thread and must join it after requesting the stop. */
void ftp_server_stop(void);

/* True after ftp_serve has completed bind/listen on its control port. */
int ftp_server_is_listening(void);

#ifdef __cplusplus
}
#endif
