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


#ifndef WIN32

#include <stdio.h>
#include <fcntl.h>  /* File Control Definitions          */
#include <termios.h>/* POSIX Terminal Control Definitions*/
#include <unistd.h> /* UNIX Standard Definitions         */

#include "net_context.h"
#include "kt_iot_log.h"



/**
110, 300, 120, 2400, 4800, 9600, 14400, 19200, 28800, 38400, 57600, 115200
*/

static int set_interface_attribs(int fd, int speed)
{
    struct termios tty;

    if (tcgetattr(fd, &tty) < 0) {
        ERR("Error from tcgetattr: %s", strerror(errno));
        return -1;
    }

    cfsetospeed(&tty, (speed_t)speed);
    cfsetispeed(&tty, (speed_t)speed);

    tty.c_cflag |= (CLOCAL | CREAD);    /* ignore modem controls */
    tty.c_cflag &= ~CSIZE;
    tty.c_cflag |= CS8;         /* 8-bit characters */
    tty.c_cflag &= ~PARENB;     /* no parity bit */
    tty.c_cflag &= ~CSTOPB;     /* only need 1 stop bit */
    tty.c_cflag &= ~CRTSCTS;    /* no hardware flowcontrol */

    /* setup for non-canonical mode */
    tty.c_iflag &= ~(IGNBRK | BRKINT | PARMRK | ISTRIP | INLCR | IGNCR | ICRNL | IXON);
    tty.c_lflag &= ~(ECHO | ECHONL | ICANON | ISIG | IEXTEN);
    tty.c_oflag &= ~OPOST;

    /* fetch bytes as they become available */
    tty.c_cc[VMIN] = 1;
    tty.c_cc[VTIME] = 1;

    if (tcsetattr(fd, TCSANOW, &tty) != 0) {
        ERR("Error from tcsetattr: %s", strerror(errno));
        return -1;
    }
    return 0;
}

static void set_mincount(int fd, int mcount)
{
    struct termios tty;

    if (tcgetattr(fd, &tty) < 0) {
        ERR("Error tcgetattr: %s", strerror(errno));
        return;
    }

    tty.c_cc[VMIN] = mcount ? 1 : 0;
    tty.c_cc[VTIME] = 5;        /* half second timer */

    if (tcsetattr(fd, TCSANOW, &tty) < 0)
        ERR("Error tcsetattr: %s", strerror(errno));
}

///////////////////////////////////////////////

int net_term_open(connection_tPtr ctx, char *portname)
{
  termio_channel_tPtr term_ctx = (termio_channel_tPtr)&(ctx->termio_channel);

	term_ctx->fd = socket(PF_INET, SOCK_DGRAM, 0);
  if ( term_ctx->fd < 0 ) {
		ERR("fail socket()");
		return -1;
	}

  term_ctx->fd = open(portname, O_RDWR | O_NOCTTY | O_SYNC);
  if (term_ctx->fd < 0) {
    ERR("Error opening %s: %s\n", portname, strerror(errno));
    return -1;
  }

  ctx->netChannelType = TERMIO;

  return 0;
}

void net_term_close(connection_tPtr ctx)
{
  termio_channel_tPtr term_ctx = (termio_channel_tPtr)&(ctx->termio_channel);
  
  if ( term_ctx->fd > 0 )  {
    close (term_ctx->fd);
    term_ctx->fd = -1;
  }
}

void net_term_set_speed_115200(connection_tPtr ctx)
{
  termio_channel_tPtr term_ctx = (termio_channel_tPtr)&(ctx->termio_channel);
  set_interface_attribs(term_ctx->fd, B115200);
}

void net_term_set_speed_9600(connection_tPtr ctx)
{
  termio_channel_tPtr term_ctx = (termio_channel_tPtr)&(ctx->termio_channel);
  set_interface_attribs(term_ctx->fd, B9600);
}



int net_term_send_buffer(connection_tPtr ctx)
{
  termio_channel_tPtr term_ctx = (termio_channel_tPtr)&(ctx->termio_channel);
  buffer_tPtr buff = (buffer_tPtr)&(ctx->send_buff);

  if (term_ctx->fd < 0) {
		ERR("invalid fd [%d] ", term_ctx->fd);
    return -1;
  }

  size_t bytes_written = write(term_ctx->fd, buff->data, buff->length);
  if ( bytes_written != buff->length) {
    ERR("write() %ld but %ld, errno=%d\n", buff->length, bytes_written, errno);
    return -1;
  }

  tcdrain(term_ctx->fd);    /* delay for output */

  return bytes_written;
}


int net_term_recv_buffer(connection_tPtr ctx)
{
  termio_channel_tPtr term_ctx = (termio_channel_tPtr)&(ctx->termio_channel);
  buffer_tPtr buff = (buffer_tPtr)&(ctx->recv_buff);

  if (term_ctx->fd < 0) {
		ERR("invalid fd [%d] ", term_ctx->fd);
    return -1;
  }

  // receive message.
  size_t want_read = IOBUFF_MAX_SIZE;
  size_t total_length = 0;
  buff->length = 0;


  /* simple noncanonical input */
  total_length = read(term_ctx->fd, buff->data, want_read);
  if (total_length == 0) {
    WRN("peer closed, %s\n", strerror(errno));
    return -1;
  }

  if (total_length < 0) {
    ERR("Error from read: %ld: %s\n", total_length, strerror(errno));
    return -1;
  }

  buff->length = total_length;

  return buff->length;
}



/* ===============================
 * > 0 : read event
 * = 0 : timeout
 * < 0 : error
 * =============================== */
int net_term_available_timeout(connection_tPtr ctx, int seconds)
{
  termio_channel_tPtr term_ctx = (termio_channel_tPtr)&(ctx->termio_channel);

  if (term_ctx->fd < 0) {
		ERR("invalid fd [%d] ", term_ctx->fd);
    return -1;
  }

  return net_fd_available_for_read_timeout_msec(term_ctx->fd, seconds*1000);
}

int net_term_available(connection_tPtr ctx)
{
  termio_channel_tPtr term_ctx = (termio_channel_tPtr)&(ctx->termio_channel);

  if (term_ctx->fd < 0) {
		ERR("invalid fd [%d] ", term_ctx->fd);
    return -1;
  }

  return net_fd_available_for_read_timeout_msec(term_ctx->fd, 1);
}


#endif //#ifndef WIN32
