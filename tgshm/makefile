export TOPDIR := $(PWD)

include tgshm.mk
############################################################
#sub directories
SUBDIRS = appr tgshmr tgshmw
.PHONY : $(SUBDIRS)

#############################################################

all: 
	@for dir in $(SUBDIRS); do $(MAKE) -C $$dir; done
	$(CC) $(CFLAGS) $(LIBS) $(LDFLAGS) tgshmr/*.o --share -o libtgshmpcap.so
	$(CC) $(CFLAGS) $(LIBS) -ltgshmpcap $(LDFLAGS) appr/*.o -o tgshmrd 
	$(CC) $(CFLAGS) $(LIBS) $(LDFLAGS) tgshmw/*.o -o tgshmwd
clean:
	@for dir in $(SUBDIRS); do $(MAKE) -C $$dir clean; done
	$(RM) tgshmr/*.o tgshmw/*.o tgshmrd tgshmwd libtgshmpcap.so

distclean:
	@for dir in $(SUBDIRS); do $(MAKE) -C $$dir distclean; done
	$(RM) tgshmr/*.o tgshmw/*.o tgshmrd tgshmwd libtgshmpcap.so
	
