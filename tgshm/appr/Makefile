include $(TOPDIR)/tgshm.mk

COBJS = $(patsubst %.c,%.o,$(wildcard *.c))

all: $(COBJS)

clean:
	$(RM) *.o

%.dep : %.c
	$(CC) -MM $(CFLAGS) $< > $@
include $(COBJS:.o=.dep)

distclean:
	$(RM) *.o
	$(RM) *.dep