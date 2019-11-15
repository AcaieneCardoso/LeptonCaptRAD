# Makefile for lepton-sdk-fork
# Jacob Killelea

CC = gcc -c
CFLAGS = -Wall -march=native

# Optimization
CFLAGS += -O3

# debug
# CFLAGS += -g

OUTDIR = Debug
OUTFILE = $(OUTDIR)/libLEPTON_SDK.a

# SOURCES = crc16fast.c crc16.h LEPTON_AGC.c LEPTON_AGC.h \
# LEPTON_ErrorCodes.h LEPTON_I2C_Protocol.c LEPTON_I2C_Protocol.h \
# LEPTON_I2C_Reg.h LEPTON_I2C_Service.c LEPTON_I2C_Service.h \
# LEPTON_Macros.h LEPTON_OEM.c LEPTON_OEM.h LEPTON_RAD.c \
# LEPTON_RAD.h LEPTON_SDK.c LEPTON_SDKConfig.h LEPTON_SDK.h \
# LEPTON_SYS.c LEPTON_SYS.h LEPTON_Types.h LEPTON_VID.c \
# LEPTON_VID.h raspi_I2C.c raspi_I2C.h

OBJS = $(OUTDIR)/crc16fast.o $(OUTDIR)/LEPTON_AGC.o $(OUTDIR)/LEPTON_I2C_Protocol.o \
$(OUTDIR)/LEPTON_I2C_Service.o $(OUTDIR)/LEPTON_OEM.o $(OUTDIR)/LEPTON_RAD.o \
$(OUTDIR)/LEPTON_SDK.o $(OUTDIR)/LEPTON_SYS.o $(OUTDIR)/LEPTON_VID.o \
$(OUTDIR)/raspi_I2C.o $(OUTDIR)/LEPTON_ErrorCodes.o

.PHONY: all
all: $(OUTFILE)  $(OBJS)

$(OUTFILE): $(OUTDIR) $(OBJS)
	ar -rs $@ $(OBJS)

$(OUTDIR)/%.o: %.c
	$(CC) $(CFLAGS) -o $@ $^

$(OUTDIR):
	mkdir -p $@

.PHONY: clean
clean:
	rm -rf $(OUTDIR)
