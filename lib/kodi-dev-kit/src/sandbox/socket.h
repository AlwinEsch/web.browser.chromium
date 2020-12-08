#pragma once

#include <unistd.h>
#include <stdlib.h>

#include <sys/socket.h>
#include <sys/un.h>

int create_socket(const char *path)
{
	int sock = socket(AF_UNIX, SOCK_DGRAM, 0);

	struct sockaddr_un addr;
	memset(&addr, 0, sizeof(addr));
	addr.sun_family = AF_UNIX;
	strcpy(addr.sun_path, path);
	unlink(path);
	if (bind(sock, (struct sockaddr *)&addr, sizeof(addr)) < 0)
	{
		exit(-1);
	}

	return sock;
}

int connect_socket(int sock, const char *path)
{
	struct sockaddr_un addr;
	memset(&addr, 0, sizeof(addr));
	addr.sun_family = AF_UNIX;
	strcpy(addr.sun_path, path);
	return connect(sock, (struct sockaddr *)&addr, sizeof(addr));
}

void write_fd(int sock, int fd, void *data, size_t data_len)
{
	struct msghdr msg = {0};
	char buf[CMSG_SPACE(sizeof(fd))];
	memset(buf, '\0', sizeof(buf));

	struct iovec io = {.iov_base = data, .iov_len = data_len};

	msg.msg_iov = &io;
	msg.msg_iovlen = 1;
	msg.msg_control = buf;
	msg.msg_controllen = sizeof(buf);

	struct cmsghdr *cmsg = CMSG_FIRSTHDR(&msg);
	cmsg->cmsg_level = SOL_SOCKET;
	cmsg->cmsg_type = SCM_RIGHTS;
	cmsg->cmsg_len = CMSG_LEN(sizeof(fd));

	memmove(CMSG_DATA(cmsg), &fd, sizeof(fd));

	msg.msg_controllen = CMSG_SPACE(sizeof(fd));

	if (sendmsg(sock, &msg, 0) < 0)
	{
		exit(-1);
	}
}

void read_fd(int sock, int *fd, void *data, size_t data_len)
{
	struct msghdr msg = {0};

	struct iovec io = {.iov_base = data, .iov_len = data_len};
	msg.msg_iov = &io;
	msg.msg_iovlen = 1;

	char c_buffer[256];
	msg.msg_control = c_buffer;
	msg.msg_controllen = sizeof(c_buffer);

	if (recvmsg(sock, &msg, 0) < 0)
	{
		exit(-1);
	}

	struct cmsghdr *cmsg = CMSG_FIRSTHDR(&msg);

	memmove(fd, CMSG_DATA(cmsg), sizeof(fd));
}
