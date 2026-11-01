CC = gcc
CFLAGS = -Wall -Wextra -std=c99 -g

SRCDIR = src
SRCS = $(SRCDIR)/main.c \
       $(SRCDIR)/common.c \
       $(SRCDIR)/persistence.c \
       $(SRCDIR)/session.c \
       $(SRCDIR)/user_manager.c \
       $(SRCDIR)/device_manager.c \
       $(SRCDIR)/maintenance.c

all: admin engineer

# 管理员端
admin: $(SRCS)
	$(CC) $(CFLAGS) -DAPP_ADMIN -o admin $(SRCS)

# 工程师端
engineer: $(SRCS)
	$(CC) $(CFLAGS) -DAPP_ENGINEER -o engineer $(SRCS)

clean:
	rm -f admin engineer

.PHONY: all admin engineer clean
