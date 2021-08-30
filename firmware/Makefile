#
# kbuild file for firmware/
#

# Create $(fwabs) from $(CONFIG_EXTRA_FIRMWARE_DIR) -- if it doesn't have a
# leading /, it's relative to $(srctree).
fwdir := $(subst $(quote),,$(CONFIG_EXTRA_FIRMWARE_DIR))
fwabs := $(addprefix $(srctree)/,$(filter-out /%,$(fwdir)))$(filter /%,$(fwdir))

fw-external-y := $(subst $(quote),,$(CONFIG_EXTRA_FIRMWARE))

# There are three cases to care about:
# 1. Building kernel with CONFIG_FIRMWARE_IN_KERNEL=y -- $(fw-shipped-y) should
#    include the firmware files to include, according to .config
# 2. 'make modules_install', which will install firmware for modules, and 
#    _also_ for the in-kernel drivers when CONFIG_FIRMWARE_IN_KERNEL=n
# 3. 'make firmware_install', which installs all firmware, unconditionally.

# For the former two cases we want $(fw-shipped-y) and $(fw-shipped-m) to be
# accurate. In the latter case it doesn't matter -- it'll use $(fw-shipped-all).
# But be aware that the config file might not be included at all.

ifdef CONFIG_ACENIC_OMIT_TIGON_I
acenic-objs := $(DEBLOBBED)
fw-shipped- += $(DEBLOBBED)
else
acenic-objs := $(DEBLOBBED) $(DEBLOBBED)
endif
fw-shipped-$(CONFIG_ACENIC) += $(acenic-objs)
fw-shipped-$(CONFIG_ADAPTEC_STARFIRE) += $(DEBLOBBED) \
					 $(DEBLOBBED)
fw-shipped-$(CONFIG_ATARI_DSP56K) += dsp56k/bootstrap.bin
fw-shipped-$(CONFIG_ATM_AMBASSADOR) += $(DEBLOBBED)
fw-shipped-$(CONFIG_BNX2X) += $(DEBLOBBED) \
			      $(DEBLOBBED) \
			      $(DEBLOBBED)
fw-shipped-$(CONFIG_BNX2) += $(DEBLOBBED) \
			     $(DEBLOBBED) \
			     $(DEBLOBBED) \
			     $(DEBLOBBED) \
			     $(DEBLOBBED)
fw-shipped-$(CONFIG_CASSINI) += $(DEBLOBBED)
fw-shipped-$(CONFIG_CHELSIO_T3) += $(DEBLOBBED) \
				   $(DEBLOBBED) \
				   $(DEBLOBBED) \
				   $(DEBLOBBED) \
				   $(DEBLOBBED)
fw-shipped-$(CONFIG_DRM_MGA) += $(DEBLOBBED) $(DEBLOBBED)
fw-shipped-$(CONFIG_DRM_R128) += $(DEBLOBBED)
fw-shipped-$(CONFIG_DRM_RADEON) += $(DEBLOBBED) $(DEBLOBBED) \
				   $(DEBLOBBED) $(DEBLOBBED) \
				   $(DEBLOBBED) $(DEBLOBBED) \
				   $(DEBLOBBED) \
				   $(DEBLOBBED) $(DEBLOBBED) \
				   $(DEBLOBBED) $(DEBLOBBED) \
				   $(DEBLOBBED) $(DEBLOBBED) \
				   $(DEBLOBBED) $(DEBLOBBED) \
				   $(DEBLOBBED) $(DEBLOBBED) \
				   $(DEBLOBBED) $(DEBLOBBED) \
				   $(DEBLOBBED) $(DEBLOBBED) \
				   $(DEBLOBBED) $(DEBLOBBED) \
				   $(DEBLOBBED) $(DEBLOBBED) \
				   $(DEBLOBBED) $(DEBLOBBED)
fw-shipped-$(CONFIG_DVB_AV7110) += av7110/bootcode.bin
fw-shipped-$(CONFIG_DVB_TTUSB_BUDGET) += $(DEBLOBBED)
fw-shipped-$(CONFIG_E100) += $(DEBLOBBED) $(DEBLOBBED) \
			     $(DEBLOBBED)
fw-shipped-$(CONFIG_MYRI_SBUS) += $(DEBLOBBED)
fw-shipped-$(CONFIG_PCMCIA_PCNET) += $(DEBLOBBED) $(DEBLOBBED) \
				     $(DEBLOBBED) $(DEBLOBBED) \
				     $(DEBLOBBED) $(DEBLOBBED) \
				     $(DEBLOBBED)
fw-shipped-$(CONFIG_PCMCIA_3C589) += $(DEBLOBBED)
fw-shipped-$(CONFIG_PCMCIA_3C574) += $(DEBLOBBED)
fw-shipped-$(CONFIG_SERIAL_8250_CS) += $(DEBLOBBED) $(DEBLOBBED) \
				       $(DEBLOBBED) $(DEBLOBBED) \
				       $(DEBLOBBED) $(DEBLOBBED) \
				       $(DEBLOBBED)
fw-shipped-$(CONFIG_PCMCIA_SMC91C92) += $(DEBLOBBED)
fw-shipped-$(CONFIG_SCSI_ADVANSYS) += $(DEBLOBBED) $(DEBLOBBED) \
				      $(DEBLOBBED) $(DEBLOBBED)
fw-shipped-$(CONFIG_SCSI_QLOGIC_1280) += $(DEBLOBBED) $(DEBLOBBED) \
					 $(DEBLOBBED)
fw-shipped-$(CONFIG_SCSI_QLOGICPTI) += $(DEBLOBBED)
fw-shipped-$(CONFIG_INFINIBAND_QIB) += $(DEBLOBBED)
fw-shipped-$(CONFIG_SND_KORG1212) += $(DEBLOBBED)
fw-shipped-$(CONFIG_SND_MAESTRO3) += $(DEBLOBBED) \
				     $(DEBLOBBED)
fw-shipped-$(CONFIG_SND_SB16_CSP) += $(DEBLOBBED) $(DEBLOBBED) \
				     $(DEBLOBBED) \
				     $(DEBLOBBED) \
				     $(DEBLOBBED)
fw-shipped-$(CONFIG_SND_YMFPCI) += $(DEBLOBBED) $(DEBLOBBED) \
				   $(DEBLOBBED)
fw-shipped-$(CONFIG_SND_WAVEFRONT) += $(DEBLOBBED)
fw-shipped-$(CONFIG_TEHUTI) += $(DEBLOBBED)
fw-shipped-$(CONFIG_TIGON3) += $(DEBLOBBED) $(DEBLOBBED) \
			       $(DEBLOBBED)
fw-shipped-$(CONFIG_TYPHOON) += $(DEBLOBBED)
fw-shipped-$(CONFIG_USB_EMI26) += $(DEBLOBBED) $(DEBLOBBED) \
				  $(DEBLOBBED)
fw-shipped-$(CONFIG_USB_EMI62) += $(DEBLOBBED) $(DEBLOBBED) \
				  $(DEBLOBBED) $(DEBLOBBED)
fw-shipped-$(CONFIG_USB_KAWETH) += $(DEBLOBBED) $(DEBLOBBED) \
				   $(DEBLOBBED) \
				   $(DEBLOBBED)
ifdef CONFIG_FIRMWARE_IN_KERNEL
fw-shipped-$(CONFIG_USB_SERIAL_KEYSPAN_MPR) += $(DEBLOBBED)
fw-shipped-$(CONFIG_USB_SERIAL_KEYSPAN_USA18X) += $(DEBLOBBED)
fw-shipped-$(CONFIG_USB_SERIAL_KEYSPAN_USA19) += $(DEBLOBBED)
fw-shipped-$(CONFIG_USB_SERIAL_KEYSPAN_USA19QI) += $(DEBLOBBED)
fw-shipped-$(CONFIG_USB_SERIAL_KEYSPAN_USA19QW) += $(DEBLOBBED)
fw-shipped-$(CONFIG_USB_SERIAL_KEYSPAN_USA19W) += $(DEBLOBBED)
fw-shipped-$(CONFIG_USB_SERIAL_KEYSPAN_USA28) += $(DEBLOBBED)
fw-shipped-$(CONFIG_USB_SERIAL_KEYSPAN_USA28XA) += $(DEBLOBBED)
fw-shipped-$(CONFIG_USB_SERIAL_KEYSPAN_USA28XB) += $(DEBLOBBED)
fw-shipped-$(CONFIG_USB_SERIAL_KEYSPAN_USA28X) += $(DEBLOBBED)
fw-shipped-$(CONFIG_USB_SERIAL_KEYSPAN_USA49W) += $(DEBLOBBED)
fw-shipped-$(CONFIG_USB_SERIAL_KEYSPAN_USA49WLC) += $(DEBLOBBED)
else
fw-shipped- += $(DEBLOBBED) $(DEBLOBBED) $(DEBLOBBED)	\
	$(DEBLOBBED) $(DEBLOBBED) $(DEBLOBBED)		\
	$(DEBLOBBED) $(DEBLOBBED) $(DEBLOBBED)		\
	$(DEBLOBBED) $(DEBLOBBED) $(DEBLOBBED)
endif
fw-shipped-$(CONFIG_USB_SERIAL_TI) += $(DEBLOBBED) $(DEBLOBBED) \
				      $(DEBLOBBED) $(DEBLOBBED) $(DEBLOBBED)
fw-shipped-$(CONFIG_USB_SERIAL_EDGEPORT) += $(DEBLOBBED) $(DEBLOBBED) \
					    $(DEBLOBBED) $(DEBLOBBED)
fw-shipped-$(CONFIG_USB_SERIAL_EDGEPORT_TI) += $(DEBLOBBED)
fw-shipped-$(CONFIG_USB_SERIAL_WHITEHEAT) += $(DEBLOBBED) $(DEBLOBBED) \
					   # $(DEBLOBBED)
fw-shipped-$(CONFIG_USB_SERIAL_KEYSPAN_PDA) += keyspan_pda/keyspan_pda.fw
fw-shipped-$(CONFIG_USB_SERIAL_XIRCOM) += keyspan_pda/xircom_pgs.fw
fw-shipped-$(CONFIG_USB_VICAM) += $(DEBLOBBED)
fw-shipped-$(CONFIG_VIDEO_CPIA2) += $(DEBLOBBED)
fw-shipped-$(CONFIG_YAM) += $(DEBLOBBED) $(DEBLOBBED)

fw-shipped-all := $(fw-shipped-y) $(fw-shipped-m) $(fw-shipped-)

quiet_cmd_ihex  = IHEX    $@
      cmd_ihex  = $(OBJCOPY) -Iihex -Obinary $< $@

quiet_cmd_ihex2fw  = IHEX2FW $@
      cmd_ihex2fw  = $(objtree)/$(obj)/ihex2fw $< $@

quiet_cmd_h16tofw  = H16TOFW $@
      cmd_h16tofw  = $(objtree)/$(obj)/ihex2fw -w $< $@

quiet_cmd_fwbin = MK_FW   $@
      cmd_fwbin = FWNAME="$(patsubst firmware/%.gen.S,%,$@)";		     \
		  FWSTR="$(subst /,_,$(subst .,_,$(subst -,_,$(patsubst	     \
				firmware/%.gen.S,%,$@))))";		     \
		  ASM_WORD=$(if $(CONFIG_64BIT),.quad,.long);		     \
		  ASM_ALIGN=$(if $(CONFIG_64BIT),3,2);			     \
		  PROGBITS=$(if $(CONFIG_ARM),%,@)progbits;		     \
		  echo "/* Generated by firmware/Makefile */"		> $@;\
		  echo "    .section .rodata"				>>$@;\
		  echo "    .p2align $${ASM_ALIGN}"			>>$@;\
		  echo "_fw_$${FWSTR}_bin:"				>>$@;\
		  echo "    .incbin \"$(2)\""				>>$@;\
		  echo "_fw_end:"					>>$@;\
		  echo "   .section .rodata.str,\"aMS\",$${PROGBITS},1"	>>$@;\
		  echo "    .p2align $${ASM_ALIGN}"			>>$@;\
		  echo "_fw_$${FWSTR}_name:"				>>$@;\
		  echo "    .string \"$$FWNAME\""			>>$@;\
		  echo "    .section .builtin_fw,\"a\",$${PROGBITS}"	>>$@;\
		  echo "    .p2align $${ASM_ALIGN}"			>>$@;\
		  echo "    $${ASM_WORD} _fw_$${FWSTR}_name"		>>$@;\
		  echo "    $${ASM_WORD} _fw_$${FWSTR}_bin"		>>$@;\
		  echo "    $${ASM_WORD} _fw_end - _fw_$${FWSTR}_bin"	>>$@;

# One of these files will change, or come into existence, whenever
# the configuration changes between 32-bit and 64-bit. The .S files
# need to change when that happens.
wordsize_deps := $(wildcard include/config/64bit.h include/config/32bit.h \
		include/config/ppc32.h include/config/ppc64.h \
		include/config/superh32.h include/config/superh64.h \
		include/config/x86_32.h include/config/x86_64.h)

$(patsubst %,$(obj)/%.gen.S, $(fw-shipped-y)): %: $(wordsize_deps)
	$(call cmd,fwbin,$(patsubst %.gen.S,%,$@))
$(patsubst %,$(obj)/%.gen.S, $(fw-external-y)): %: $(wordsize_deps) \
		include/config/extra/firmware/dir.h
	$(call cmd,fwbin,$(fwabs)/$(patsubst $(obj)/%.gen.S,%,$@))

# The .o files depend on the binaries directly; the .S files don't.
$(patsubst %,$(obj)/%.gen.o, $(fw-shipped-y)): %.gen.o: %
$(patsubst %,$(obj)/%.gen.o, $(fw-external-y)): $(obj)/%.gen.o: $(fwdir)/%

# .ihex is used just as a simple way to hold binary files in a source tree
# where binaries are frowned upon. They are directly converted with objcopy.
$(obj)/%: $(obj)/%.ihex
	$(call cmd,ihex)

# Don't depend on ihex2fw if we're installing and it already exists.
# Putting it after | in the dependencies doesn't seem sufficient when
# we're installing after a cross-compile, because ihex2fw has dependencies
# on stuff like /usr/lib/gcc/ppc64-redhat-linux/4.3.0/include/stddef.h and 
# thus wants to be rebuilt. Which it can't be, if the prebuilt kernel tree
# is exported read-only for someone to run 'make install'.
ifeq ($(INSTALL):$(wildcard $(obj)/ihex2fw),install:$(obj)/ihex2fw)
ihex2fw_dep :=
else
ihex2fw_dep := $(obj)/ihex2fw
endif

# .HEX is also Intel HEX, but where the offset and length in each record
# is actually meaningful, because the firmware has to be loaded in a certain
# order rather than as a single binary blob. Thus, we convert them into our
# more compact binary representation of ihex records (<linux/ihex.h>)
$(obj)/%.fw: $(obj)/%.HEX $(ihex2fw_dep)
	$(call cmd,ihex2fw)

# .H16 is our own modified form of Intel HEX, with 16-bit length for records.
$(obj)/%.fw: $(obj)/%.H16 $(ihex2fw_dep)
	$(call cmd,h16tofw)

obj-y				 += $(patsubst %,%.gen.o, $(fw-external-y))
obj-$(CONFIG_FIRMWARE_IN_KERNEL) += $(patsubst %,%.gen.o, $(fw-shipped-y))

ifeq ($(KBUILD_SRC),)
# Makefile.build only creates subdirectories for O= builds, but external
# firmware might live outside the kernel source tree
_dummy := $(foreach d,$(addprefix $(obj)/,$(dir $(fw-external-y))), $(shell [ -d $(d) ] || mkdir -p $(d)))
endif

# Remove .S files and binaries created from ihex
# (during 'make clean' .config isn't included so they're all in $(fw-shipped-))
targets := $(fw-shipped-) $(patsubst $(obj)/%,%, \
				$(shell find $(obj) -name \*.gen.S 2>/dev/null))

# Without this, built-in.o won't be created when it's empty, and the
# final vmlinux link will fail.
obj- := dummy

hostprogs-y := ihex2fw
