EXE            = render
PATHSRC        = src
PATHBUILD      = build
PATHBIN        = bin
SRCEXT         = cpp
SOURCES        = $(wildcard $(PATHSRC)/*.$(SRCEXT))
PROGRAM        = $(PATHBIN)/$(EXE)
OBJFILES       = $(SOURCES:%.$(SRCEXT)=$(PATHBUILD)/%.o)


### Common options
DEBUG          = 1

### Compiler options
CXX            = clang++
INCLUDEPATHS   = -I/usr/include/freetype2
WARNINGS       = -Weverything -Wno-c++98-compat
CXXFLAGS       = -std=c++14 -O2 -c $(WARNINGS) $(INCLUDEPATHS)
#CXX_FLAGS   += -Weverything -Wno-missing-prototypes -Wno-c++98-compat -Wno-c++98-compat-pedantic -Wno-old-style-cast -Wdocumentation-unknown-command



### Linker options
LIBPATHS       = 
LIBS           = -lGLEW -lglfw -lOpenGL -lfreetype
LDFLAGS        = -std=c++14


### Options
CMD_PREFIX     = @
ifneq ($(VERBOSE),)
	CMD_PREFIX  :=
endif

ifneq ($(DEBUG),)
	SANITIZER    = -fsanitize=address -fno-omit-frame-pointer
	CXXFLAGS    += $(SANITIZER)
	LDFLAGS     += $(SANITIZER)
endif





all: mkdirs $(OBJFILES) $(PROGRAM)

$(PROGRAM): $(OBJFILES)
	@echo "Linking program $@"
	$(CMD_PREFIX)$(CXX) $(LDFLAGS) -o $@ $^ $(LIBPATHS) $(LIBS)


$(PATHBUILD)/$(PATHSRC)/%.o: $(PATHSRC)/%.$(SRCEXT)
	@echo "Compiling $@"
	$(CMD_PREFIX)$(CXX) $(CXXFLAGS) -c $< -o $@

mkdirs: $(PATHBUILD)/$(PATHSRC) $(PATHBIN)

$(PATHBUILD)/$(PATHSRC):
	$(CMD_PREFIX)mkdir -p $@

$(PATHBIN):
	$(CMD_PREFIX)mkdir -p $(PATHBIN)

clean:
	rm -rf $(OBJFILES) $(PATHBIN) $(PATHBUILD)

.PHONY=clean all mkdirs
