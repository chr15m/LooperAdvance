#---------------------------------------------------------------------------------
# Clear the implicit built in rules
#---------------------------------------------------------------------------------
.SUFFIXES:
#---------------------------------------------------------------------------------
ifeq ($(strip $(DEVKITARM)),)
$(error "Please set DEVKITARM in your environment. export DEVKITARM=<path to>devkitARM)
endif
include $(DEVKITARM)/gba_rules

#---------------------------------------------------------------------------------
# TARGET is the name of the output, if this ends with _mb generates a multiboot image
# BUILD is the directory where object files & intermediate files will be placed
# SOURCES is a list of directories containing source code
# INCLUDES is a list of directories containing extra header files
#---------------------------------------------------------------------------------
TARGET		:=	LooperAdvance
BUILD		:=	build
SOURCES		:=	.
INCLUDES	:=	-I $(CURDIR)/../krawall/build/krawall/include -I $(DEVKITPRO)/libgba/include/

#---------------------------------------------------------------------------------
# options for code generation
#---------------------------------------------------------------------------------
ARCH	:=	-mthumb -mthumb-interwork

CFLAGS	:=	-std=gnu99 -Wall\
			-mcpu=arm7tdmi -mtune=arm7tdmi\
 			-fomit-frame-pointer\
			-ffast-math \
			$(ARCH)
CFLAGS	+=	$(INCLUDES)

CPPFLAGS = $(CFLAGS) -fno-exceptions -fno-rtti -std=gnu++11

ASFLAGS	:=	$(ARCH)
LDFLAGS	:=	$(CFLAGS) -Wl,-Map,$(notdir $@).map
# -specs=gba.specs 

#---------------------------------------------------------------------------------
# path to tools - this can be deleted if you set the path in windows
#---------------------------------------------------------------------------------
export PATH		:=	$(DEVKITARM)/bin:$(PATH)

#---------------------------------------------------------------------------------
# any extra libraries we wish to link with the project
#---------------------------------------------------------------------------------
LIBS	:=	-lgba $(CURDIR)/../krawall/build/krawall/lib/libkrawall-32k-60-medium.a $(CURDIR)/../krawall/build/krawall/src/krawall-cross-gba-build/examples/modules/libmodules.a

#---------------------------------------------------------------------------------
# list of directories containing libraries, this must be the top level containing
# include and lib
#---------------------------------------------------------------------------------
LIBDIRS	:=	$(LIBGBA) 

#---------------------------------------------------------------------------------
# no real need to edit anything past this point unless you need to add additional
# rules for different file extensions
#---------------------------------------------------------------------------------
ifneq ($(BUILD),$(notdir $(CURDIR)))
#---------------------------------------------------------------------------------

export OUTPUT	:=	$(CURDIR)/$(TARGET)

export VPATH	:=	$(foreach dir,$(SOURCES),$(CURDIR)/$(dir))
export PATH	:=	$(DEVKITARM)/bin:$(PATH)
export DEPSDIR	:=	$(CURDIR)/$(BUILD)

#---------------------------------------------------------------------------------
# automatically build a list of object files for our project
#---------------------------------------------------------------------------------
CFILES		:=	$(foreach dir,$(SOURCES),$(notdir $(wildcard $(dir)/*.c)))
CPPFILES	:=	$(foreach dir,$(SOURCES),$(notdir $(wildcard $(dir)/*.cpp)))
SFILES		:=	$(foreach dir,$(SOURCES),$(notdir $(wildcard $(dir)/*.s)))
BINFILES	:=	$(foreach dir,$(SOURCES),$(notdir $(wildcard $(dir)/*.bin)))

#---------------------------------------------------------------------------------
# use CXX for linking C++ projects, CC for standard C
#---------------------------------------------------------------------------------
ifeq ($(strip $(CPPFILES)),)
#---------------------------------------------------------------------------------
	export LD	:=	$(CC)
#---------------------------------------------------------------------------------
else
#---------------------------------------------------------------------------------
	export LD	:=	$(CXX)
#---------------------------------------------------------------------------------
endif
#---------------------------------------------------------------------------------

export OFILES	:= $(BINFILES:.bin=.o) $(CPPFILES:.cpp=.o) $(CFILES:.c=.o) $(SFILES:.s=.o)


#---------------------------------------------------------------------------------
# build a list of include paths
#---------------------------------------------------------------------------------
export INCLUDE	:=	$(foreach dir,$(INCLUDES),-I$(CURDIR)/$(dir)) \
					$(foreach dir,$(LIBDIRS),-I$(dir)/include) \
					-I$(CURDIR)/$(BUILD)

#---------------------------------------------------------------------------------
# build a list of library paths
#---------------------------------------------------------------------------------
export LIBPATHS	:=	$(foreach dir,$(LIBDIRS),-L$(dir)/lib)

.PHONY: $(BUILD) clean

#---------------------------------------------------------------------------------
$(BUILD): samples.h
	@[ -d $@ ] || mkdir -p $@
	@make --no-print-directory -C $(BUILD) -f $(CURDIR)/Makefile

#---------------------------------------------------------------------------------
clean: clean-looper
	@rm -rf krawall/build

clean-looper:
	@rm -fr $(BUILD) $(TARGET).elf $(TARGET).gba
	@rm -f samplenames.cpp samplenames.h samples.S  samples.h instruments.s instruments.h modules.h
	@echo clean ...

run: LooperAdvance.gba
	@VisualBoyAdvance LooperAdvance.gba

#---------------------------------------------------------------------------------
samplenames.cpp modules.h samples.h instruments.h samples.s instruments.s: samples.xm ./krawall/build/krawerter/krawerter
	./krawall/build/krawerter/krawerter samples.xm
	./samplenames.py

#---------------------------------------------------------------------------------

unexport CC
unexport CFLAGS
unexport CXX
unexport AR
unexport AS
unexport OBJCOPY
unexport PORTLIBS
./krawall/build/krawerter/krawerter ./krawall/build/krawall/lib/libkrawall-32k-60-medium.a ./krawall/build/krawall/src/krawall-cross-gba-build/examples/modules/libmodules.a: krawall
	@echo Building Krawall library
	mkdir -p $(CURDIR)/krawall/build
	export
	cd $(CURDIR)/krawall/build && cmake -DKRAWALL_BUILD_ALL_VARIANTS:BOOLEAN=ON ..
	make -C krawall/build

else

DEPENDS	:=	$(OFILES:.o=.d)

#---------------------------------------------------------------------------------
# main targets
#---------------------------------------------------------------------------------
$(OUTPUT).gba	:	$(OUTPUT).elf

$(OUTPUT).elf	:	$(OFILES) $(LIBGBA)/lib/libgba.a $(CURDIR)/../krawall/build/krawall/lib/libkrawall-32k-60-medium.a $(CURDIR)/../krawall/build/krawall/src/krawall-cross-gba-build/examples/modules/libmodules.a

%.o	:	%.bin
	@echo	$(notdir $<)
	@$(bin2o)

%.o: %.cpp
	arm-none-eabi-g++ $(CPPFLAGS) -c -o "$@" "$<"

-include $(DEPENDS)

#---------------------------------------------------------------------------------
endif
#---------------------------------------------------------------------------------
