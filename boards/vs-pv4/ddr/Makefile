DDR_CONFIG ?= lpddr4x_timing-revB0

OBJS  += $(OUT)/$(DDR_CONFIG)-8GB.o
ifeq ($(UPPER_R),B0)
	OBJS  += $(OUT)/$(DDR_CONFIG)-2GB.o
	OBJS  += $(OUT)/$(DDR_CONFIG)-4GB.o
	OBJS  += $(OUT)/$(DDR_CONFIG)-4GB-sr.o
endif

VPATH += boards/vs-pv4/ddr
