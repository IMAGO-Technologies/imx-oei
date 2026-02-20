cpu     = m33
SOC     = MIMX95
SOCFULL = $(SOC)
SOC_DEVICE_DIR = devices/$(SOC)

ifneq ($(REV),A0)
FLAGS += -DCONFIG_DUMMY_TRAIN_DATA_SUPPORT -DCONFIG_ELE
endif

ifeq ($(OEI),ddr)
FLAGS += -DOEI_DDR
endif

BOARDS_DIR = $(ROOT_DIR)/boards
BOARD_DIR = $(BOARDS_DIR)/$(SOM)

INCLUDE +=  \
	-I$(BOARDS_DIR)  \
	-I$(BOARD_DIR)

OBJS += \
	$(OUT)/board.o

OBJS += \
	$(OUT)/pin_mux.o

VPATH +=  \
	$(BOARDS_DIR)  \
	$(BOARD_DIR)
