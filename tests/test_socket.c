/*
 * This file is part of the SSH Library
 *
 * Copyright (c) 2009 by Aris Adamantiadis
 *
 * The SSH Library is free software; you can redistribute it and/or modify
 * it under the terms of the GNU Lesser General Public License as published by
 * the Free Software Foundation; either version 2.1 of the License, or (at your
 * option) any later version.
 *
 * The SSH Library is distributed in the hope that it will be useful, but
 * WITHOUT ANY WARRANTY; without even the implied warranty of MERCHANTABILITY
 * or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU Lesser General Public
 * License for more details.
 *
 * You should have received a copy of the GNU Lesser General Public License
 * along with the SSH Library; see the file COPYING.  If not, write to
 * the Free Software Foundation, Inc., 59 Temple Place - Suite 330, Boston,
 * MA 02111-1307, USA.
 */

/* Simple test for the socket callbacks */

#include <unistd.h>
#include <stdlib.h>
#include <stdio.h>
#include <libssh/libssh.h>

#include <libssh/callbacks.h>
#include <libssh/socket.h>
#include <libssh/poll.h>

static int data_rcv(const void *data, size_t len, void *user){
	printf("Received data: '");
	fwrite(data,1,len,stdout);
	printf("'\n");
	return len;
}

static void controlflow(int code,void *user){
	printf("Control flow: %x\n",code);
}

static void exception(int code, int errno_code,void *user){
	printf("Exception: %d (%d)\n",code,errno_code);
}

static void connected(int code, int errno_code,void *user){
	printf("Connected: %d (%d)\n",code, errno_code);
}

struct ssh_socket_callbacks_struct callbacks={
		data_rcv,
		controlflow,
		exception,
		connected,
		NULL
};
int main(int argc, char **argv){
	struct socket *s;
	ssh_session session;
	ssh_poll_ctx ctx;
	if(argc < 3){
		printf("Usage : %s host port\n", argv[0]);
		return EXIT_FAILURE;
	}
	session=ssh_new();
	ssh_init();
	s=ssh_socket_new(session);
	ctx=ssh_poll_ctx_new(2);
	ssh_socket_set_callbacks(s, &callbacks);
	ssh_poll_ctx_add_socket(ctx,s);
	if(ssh_socket_connect(s,argv[1],atoi(argv[2]),NULL)){
		printf("ssh_socket_connect: %s\n",ssh_get_error(session));
		return EXIT_FAILURE;
	}
	ssh_poll_ctx_dopoll(ctx,-1);
	return EXIT_SUCCESS;
}