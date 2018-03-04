CC = i486-openwrt-linux-gcc
#XX = g++

LIBS = -lpthread
CFLAGS = -I$(TOPDIR)/include -L.
LDFLAGS = -g -O2 -Wl 
RM = rm -rf
CP = cp -rf
OBJS = obj
MV = mv
