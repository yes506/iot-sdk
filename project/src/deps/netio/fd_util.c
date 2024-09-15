/** 
 *  Copyright (c) 2018 KT Corp. All rights reserved.
 *
 *  This is a proprietary software of KT corp, and you may not use this file except in
 *  compliance with license agreement with KT corp. Any redistribution or use of this
 *  software, with or without modification shall be strictly prohibited without prior written
 *  approval of KT corp, and the copyright notice above does not evidence any actual or
 *  intended publication of such software.
 *
 */

#include <errno.h>
#include "kt_iot_log.h"

#include "net_context.h"


#if defined(WIN32) || defined(FREERTOS)
/* ===============================
 * > 0 : read event
 * = 0 : timeout
 * < 0 : error; peer may closed
 * =============================== */
int net_fd_available_for_read_timeout_msec(int fd, int msec)
{
  struct timeval tv;
  fd_set fds_read;
  fd_set fds_err;
  int state;

  if (fd < 0) {
		ERR("invalid fd [%d] ", fd);
    return -1;
  }

	FD_ZERO(&fds_read);
	FD_ZERO(&fds_err);
	FD_SET(fd, &fds_read);
	FD_SET(fd, &fds_err);

  tv.tv_sec = (int)(msec / 1000);
  tv.tv_usec = ((int)(msec % 1000)) * 1000;

  state = select(fd+1, &fds_read, (fd_set *)0, &fds_err, &tv);

  if ( state < 0 ) {
		ERR("fd=[%d], select err=[%d], [%s]", fd, state, strerror( errno ));
		return -1;
	} else 	if ( state == 0 ){
		//DBG("select timeout");
		return 0;
	};

	if ( FD_ISSET(fd, &fds_err)){
		ERR("exception on fd [%d]", fd);
		return -1;
	}

  char buffer[8];
#if 0  // WIN32 does not have MSG_DONTWAIT
  if (recv(fd, buffer, sizeof(buffer), MSG_PEEK | MSG_DONTWAIT) == 0) {
#else
  if (recv(fd, buffer, sizeof(buffer), MSG_PEEK ) == 0) {
#endif
    // if recv returns zero, that means the connection has been closed:
    WRN("peer may closed; fd=[%d]", fd);
    return -1;
  }

  return 1;
}
#else

#include <poll.h>
int net_fd_available_for_read_timeout_msec(int fd, int msec)
{
  struct pollfd fdp;
  int ret;

  fdp.fd = fd;
  fdp.events = POLLIN;
  ret = poll(&fdp, 1, msec); // 1 second for timeout
  switch (ret) {
      case -1:
          // Error
        ERR("fd=[%d], select err=[%d], [%s]", fd, ret, strerror( errno ));
        return -1;
          break;
      case 0:
          // Timeout 
          break;
      default:
        {
          char buffer[8];
          if (recv(fd, buffer, sizeof(buffer), MSG_PEEK | MSG_DONTWAIT) == 0) {
            // if recv returns zero, that means the connection has been closed:
            WRN("peer may closed; fd=[%d]", fd);
            return -1;
          }
        }
        break;
  }

  return ret;
}
#endif
