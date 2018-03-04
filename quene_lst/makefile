export TOPDIR := $(PWD)

include tgaudit.mk
LIBS_A =  $(LIBS) -ltgshmpcap
############################################################
#sub directories
#SUBDIRS = appr
SUBDIRS = appr netlink_sev 
.PHONY : $(SUBDIRS)

#############################################################

all:  
	@for dir in $(SUBDIRS); do $(MAKE) -C $$dir; done
	$(CC)  $(CFLAGS) $(LIBS) $(LDFLAGS) netlink_sev/*.o --share -o libtgshmpcap.so
	$(CC)  $(CFLAGS) -L$(TOPDIR) $(LIBS_A) $(LDFLAGS) appr/*.o -o nl_sev 
	
clean:
	@for dir in $(SUBDIRS); do $(MAKE) -C $$dir clean; done
	$(RM) netlink_sev/*.o appr/*.o nl_sev libtgshmpcap.so 
	#$(RM) netlink_sev/*.o appr/*.o nl_sev 

distclean:
	@for dir in $(SUBDIRS); do $(MAKE) -C $$dir distclean; done
	$(RM) netlink_sev/*.o netlink_sev/*.dep appr/*.o appr/*.dep nl_sev libtgshmpcap.so 
	#$(RM) netlink_sev/*.o netlink_sev/*.dep appr/*.o appr/*.dep nl_sev
	
