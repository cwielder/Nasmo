# Alternative GNU Make project makefile autogenerated by Premake

ifndef config
  config=debug
endif

ifndef verbose
  SILENT = @
endif

.PHONY: clean prebuild

SHELLTYPE := posix
ifeq ($(shell echo "test"), "test")
	SHELLTYPE := msdos
endif

# Configurations
# #############################################

RESCOMP = windres
DEFINES += -DMSDFGEN_USE_CPP11
INCLUDES += -I../../msdf-atlas-gen/msdfgen/include -I../../msdf-atlas-gen/msdfgen/freetype/include
FORCE_INCLUDE +=
ALL_CPPFLAGS += $(CPPFLAGS) -MD -MP $(DEFINES) $(INCLUDES)
ALL_RESFLAGS += $(RESFLAGS) $(DEFINES) $(INCLUDES)
LINKCMD = $(AR) -rcs "$@" $(OBJECTS)
define PREBUILDCMDS
endef
define PRELINKCMDS
endef
define POSTBUILDCMDS
endef

ifeq ($(config),debug)
TARGETDIR = bin/msdfgen-Debug/out
TARGET = $(TARGETDIR)/libmsdfgen.a
OBJDIR = bin/msdfgen-Debug/int
ALL_CFLAGS += $(CFLAGS) $(ALL_CPPFLAGS) -m64 -g
ALL_CXXFLAGS += $(CXXFLAGS) $(ALL_CPPFLAGS) -m64 -g -std=c++17
LIBS += bin/freetype-Debug/out/libfreetype.a
LDDEPS += bin/freetype-Debug/out/libfreetype.a
ALL_LDFLAGS += $(LDFLAGS) -L/usr/lib64 -m64

else ifeq ($(config),release)
TARGETDIR = bin/msdfgen-Release/out
TARGET = $(TARGETDIR)/libmsdfgen.a
OBJDIR = bin/msdfgen-Release/int
ALL_CFLAGS += $(CFLAGS) $(ALL_CPPFLAGS) -m64 -O2
ALL_CXXFLAGS += $(CXXFLAGS) $(ALL_CPPFLAGS) -m64 -O2 -std=c++17
LIBS += bin/freetype-Release/out/libfreetype.a
LDDEPS += bin/freetype-Release/out/libfreetype.a
ALL_LDFLAGS += $(LDFLAGS) -L/usr/lib64 -m64 -s

else ifeq ($(config),dist)
TARGETDIR = bin/msdfgen-Dist/out
TARGET = $(TARGETDIR)/libmsdfgen.a
OBJDIR = bin/msdfgen-Dist/int
ALL_CFLAGS += $(CFLAGS) $(ALL_CPPFLAGS) -m64 -O2
ALL_CXXFLAGS += $(CXXFLAGS) $(ALL_CPPFLAGS) -m64 -O2 -std=c++17
LIBS += bin/freetype-Dist/out/libfreetype.a
LDDEPS += bin/freetype-Dist/out/libfreetype.a
ALL_LDFLAGS += $(LDFLAGS) -L/usr/lib64 -m64 -s

endif

# Per File Configurations
# #############################################


# File sets
# #############################################

GENERATED :=
OBJECTS :=

GENERATED += $(OBJDIR)/Contour.o
GENERATED += $(OBJDIR)/EdgeHolder.o
GENERATED += $(OBJDIR)/MSDFErrorCorrection.o
GENERATED += $(OBJDIR)/Projection.o
GENERATED += $(OBJDIR)/Scanline.o
GENERATED += $(OBJDIR)/Shape.o
GENERATED += $(OBJDIR)/SignedDistance.o
GENERATED += $(OBJDIR)/Vector2.o
GENERATED += $(OBJDIR)/contour-combiners.o
GENERATED += $(OBJDIR)/edge-coloring.o
GENERATED += $(OBJDIR)/edge-segments.o
GENERATED += $(OBJDIR)/edge-selectors.o
GENERATED += $(OBJDIR)/equation-solver.o
GENERATED += $(OBJDIR)/import-font.o
GENERATED += $(OBJDIR)/import-svg.o
GENERATED += $(OBJDIR)/lodepng.o
GENERATED += $(OBJDIR)/msdf-error-correction.o
GENERATED += $(OBJDIR)/msdfgen.o
GENERATED += $(OBJDIR)/rasterization.o
GENERATED += $(OBJDIR)/render-sdf.o
GENERATED += $(OBJDIR)/resolve-shape-geometry.o
GENERATED += $(OBJDIR)/save-bmp.o
GENERATED += $(OBJDIR)/save-png.o
GENERATED += $(OBJDIR)/save-tiff.o
GENERATED += $(OBJDIR)/sdf-error-estimation.o
GENERATED += $(OBJDIR)/shape-description.o
GENERATED += $(OBJDIR)/tinyxml2.o
OBJECTS += $(OBJDIR)/Contour.o
OBJECTS += $(OBJDIR)/EdgeHolder.o
OBJECTS += $(OBJDIR)/MSDFErrorCorrection.o
OBJECTS += $(OBJDIR)/Projection.o
OBJECTS += $(OBJDIR)/Scanline.o
OBJECTS += $(OBJDIR)/Shape.o
OBJECTS += $(OBJDIR)/SignedDistance.o
OBJECTS += $(OBJDIR)/Vector2.o
OBJECTS += $(OBJDIR)/contour-combiners.o
OBJECTS += $(OBJDIR)/edge-coloring.o
OBJECTS += $(OBJDIR)/edge-segments.o
OBJECTS += $(OBJDIR)/edge-selectors.o
OBJECTS += $(OBJDIR)/equation-solver.o
OBJECTS += $(OBJDIR)/import-font.o
OBJECTS += $(OBJDIR)/import-svg.o
OBJECTS += $(OBJDIR)/lodepng.o
OBJECTS += $(OBJDIR)/msdf-error-correction.o
OBJECTS += $(OBJDIR)/msdfgen.o
OBJECTS += $(OBJDIR)/rasterization.o
OBJECTS += $(OBJDIR)/render-sdf.o
OBJECTS += $(OBJDIR)/resolve-shape-geometry.o
OBJECTS += $(OBJDIR)/save-bmp.o
OBJECTS += $(OBJDIR)/save-png.o
OBJECTS += $(OBJDIR)/save-tiff.o
OBJECTS += $(OBJDIR)/sdf-error-estimation.o
OBJECTS += $(OBJDIR)/shape-description.o
OBJECTS += $(OBJDIR)/tinyxml2.o

# Rules
# #############################################

all: $(TARGET)
	@:

$(TARGET): $(GENERATED) $(OBJECTS) $(LDDEPS) | $(TARGETDIR)
	$(PRELINKCMDS)
	@echo Linking msdfgen
	$(SILENT) $(LINKCMD)
	$(POSTBUILDCMDS)

$(TARGETDIR):
	@echo Creating $(TARGETDIR)
ifeq (posix,$(SHELLTYPE))
	$(SILENT) mkdir -p $(TARGETDIR)
else
	$(SILENT) mkdir $(subst /,\\,$(TARGETDIR))
endif

$(OBJDIR):
	@echo Creating $(OBJDIR)
ifeq (posix,$(SHELLTYPE))
	$(SILENT) mkdir -p $(OBJDIR)
else
	$(SILENT) mkdir $(subst /,\\,$(OBJDIR))
endif

clean:
	@echo Cleaning msdfgen
ifeq (posix,$(SHELLTYPE))
	$(SILENT) rm -f  $(TARGET)
	$(SILENT) rm -rf $(GENERATED)
	$(SILENT) rm -rf $(OBJDIR)
else
	$(SILENT) if exist $(subst /,\\,$(TARGET)) del $(subst /,\\,$(TARGET))
	$(SILENT) if exist $(subst /,\\,$(GENERATED)) del /s /q $(subst /,\\,$(GENERATED))
	$(SILENT) if exist $(subst /,\\,$(OBJDIR)) rmdir /s /q $(subst /,\\,$(OBJDIR))
endif

prebuild: | $(OBJDIR)
	$(PREBUILDCMDS)

ifneq (,$(PCH))
$(OBJECTS): $(GCH) | $(PCH_PLACEHOLDER)
$(GCH): $(PCH) | prebuild
	@echo $(notdir $<)
	$(SILENT) $(CXX) -x c++-header $(ALL_CXXFLAGS) -o "$@" -MF "$(@:%.gch=%.d)" -c "$<"
$(PCH_PLACEHOLDER): $(GCH) | $(OBJDIR)
ifeq (posix,$(SHELLTYPE))
	$(SILENT) touch "$@"
else
	$(SILENT) echo $null >> "$@"
endif
else
$(OBJECTS): | prebuild
endif


# File Rules
# #############################################

$(OBJDIR)/Contour.o: ../../msdf-atlas-gen/msdfgen/core/Contour.cpp
	@echo "$(notdir $<)"
	$(SILENT) $(CXX) $(ALL_CXXFLAGS) $(FORCE_INCLUDE) -o "$@" -MF "$(@:%.o=%.d)" -c "$<"
$(OBJDIR)/EdgeHolder.o: ../../msdf-atlas-gen/msdfgen/core/EdgeHolder.cpp
	@echo "$(notdir $<)"
	$(SILENT) $(CXX) $(ALL_CXXFLAGS) $(FORCE_INCLUDE) -o "$@" -MF "$(@:%.o=%.d)" -c "$<"
$(OBJDIR)/MSDFErrorCorrection.o: ../../msdf-atlas-gen/msdfgen/core/MSDFErrorCorrection.cpp
	@echo "$(notdir $<)"
	$(SILENT) $(CXX) $(ALL_CXXFLAGS) $(FORCE_INCLUDE) -o "$@" -MF "$(@:%.o=%.d)" -c "$<"
$(OBJDIR)/Projection.o: ../../msdf-atlas-gen/msdfgen/core/Projection.cpp
	@echo "$(notdir $<)"
	$(SILENT) $(CXX) $(ALL_CXXFLAGS) $(FORCE_INCLUDE) -o "$@" -MF "$(@:%.o=%.d)" -c "$<"
$(OBJDIR)/Scanline.o: ../../msdf-atlas-gen/msdfgen/core/Scanline.cpp
	@echo "$(notdir $<)"
	$(SILENT) $(CXX) $(ALL_CXXFLAGS) $(FORCE_INCLUDE) -o "$@" -MF "$(@:%.o=%.d)" -c "$<"
$(OBJDIR)/Shape.o: ../../msdf-atlas-gen/msdfgen/core/Shape.cpp
	@echo "$(notdir $<)"
	$(SILENT) $(CXX) $(ALL_CXXFLAGS) $(FORCE_INCLUDE) -o "$@" -MF "$(@:%.o=%.d)" -c "$<"
$(OBJDIR)/SignedDistance.o: ../../msdf-atlas-gen/msdfgen/core/SignedDistance.cpp
	@echo "$(notdir $<)"
	$(SILENT) $(CXX) $(ALL_CXXFLAGS) $(FORCE_INCLUDE) -o "$@" -MF "$(@:%.o=%.d)" -c "$<"
$(OBJDIR)/Vector2.o: ../../msdf-atlas-gen/msdfgen/core/Vector2.cpp
	@echo "$(notdir $<)"
	$(SILENT) $(CXX) $(ALL_CXXFLAGS) $(FORCE_INCLUDE) -o "$@" -MF "$(@:%.o=%.d)" -c "$<"
$(OBJDIR)/contour-combiners.o: ../../msdf-atlas-gen/msdfgen/core/contour-combiners.cpp
	@echo "$(notdir $<)"
	$(SILENT) $(CXX) $(ALL_CXXFLAGS) $(FORCE_INCLUDE) -o "$@" -MF "$(@:%.o=%.d)" -c "$<"
$(OBJDIR)/edge-coloring.o: ../../msdf-atlas-gen/msdfgen/core/edge-coloring.cpp
	@echo "$(notdir $<)"
	$(SILENT) $(CXX) $(ALL_CXXFLAGS) $(FORCE_INCLUDE) -o "$@" -MF "$(@:%.o=%.d)" -c "$<"
$(OBJDIR)/edge-segments.o: ../../msdf-atlas-gen/msdfgen/core/edge-segments.cpp
	@echo "$(notdir $<)"
	$(SILENT) $(CXX) $(ALL_CXXFLAGS) $(FORCE_INCLUDE) -o "$@" -MF "$(@:%.o=%.d)" -c "$<"
$(OBJDIR)/edge-selectors.o: ../../msdf-atlas-gen/msdfgen/core/edge-selectors.cpp
	@echo "$(notdir $<)"
	$(SILENT) $(CXX) $(ALL_CXXFLAGS) $(FORCE_INCLUDE) -o "$@" -MF "$(@:%.o=%.d)" -c "$<"
$(OBJDIR)/equation-solver.o: ../../msdf-atlas-gen/msdfgen/core/equation-solver.cpp
	@echo "$(notdir $<)"
	$(SILENT) $(CXX) $(ALL_CXXFLAGS) $(FORCE_INCLUDE) -o "$@" -MF "$(@:%.o=%.d)" -c "$<"
$(OBJDIR)/msdf-error-correction.o: ../../msdf-atlas-gen/msdfgen/core/msdf-error-correction.cpp
	@echo "$(notdir $<)"
	$(SILENT) $(CXX) $(ALL_CXXFLAGS) $(FORCE_INCLUDE) -o "$@" -MF "$(@:%.o=%.d)" -c "$<"
$(OBJDIR)/msdfgen.o: ../../msdf-atlas-gen/msdfgen/core/msdfgen.cpp
	@echo "$(notdir $<)"
	$(SILENT) $(CXX) $(ALL_CXXFLAGS) $(FORCE_INCLUDE) -o "$@" -MF "$(@:%.o=%.d)" -c "$<"
$(OBJDIR)/rasterization.o: ../../msdf-atlas-gen/msdfgen/core/rasterization.cpp
	@echo "$(notdir $<)"
	$(SILENT) $(CXX) $(ALL_CXXFLAGS) $(FORCE_INCLUDE) -o "$@" -MF "$(@:%.o=%.d)" -c "$<"
$(OBJDIR)/render-sdf.o: ../../msdf-atlas-gen/msdfgen/core/render-sdf.cpp
	@echo "$(notdir $<)"
	$(SILENT) $(CXX) $(ALL_CXXFLAGS) $(FORCE_INCLUDE) -o "$@" -MF "$(@:%.o=%.d)" -c "$<"
$(OBJDIR)/save-bmp.o: ../../msdf-atlas-gen/msdfgen/core/save-bmp.cpp
	@echo "$(notdir $<)"
	$(SILENT) $(CXX) $(ALL_CXXFLAGS) $(FORCE_INCLUDE) -o "$@" -MF "$(@:%.o=%.d)" -c "$<"
$(OBJDIR)/save-tiff.o: ../../msdf-atlas-gen/msdfgen/core/save-tiff.cpp
	@echo "$(notdir $<)"
	$(SILENT) $(CXX) $(ALL_CXXFLAGS) $(FORCE_INCLUDE) -o "$@" -MF "$(@:%.o=%.d)" -c "$<"
$(OBJDIR)/sdf-error-estimation.o: ../../msdf-atlas-gen/msdfgen/core/sdf-error-estimation.cpp
	@echo "$(notdir $<)"
	$(SILENT) $(CXX) $(ALL_CXXFLAGS) $(FORCE_INCLUDE) -o "$@" -MF "$(@:%.o=%.d)" -c "$<"
$(OBJDIR)/shape-description.o: ../../msdf-atlas-gen/msdfgen/core/shape-description.cpp
	@echo "$(notdir $<)"
	$(SILENT) $(CXX) $(ALL_CXXFLAGS) $(FORCE_INCLUDE) -o "$@" -MF "$(@:%.o=%.d)" -c "$<"
$(OBJDIR)/import-font.o: ../../msdf-atlas-gen/msdfgen/ext/import-font.cpp
	@echo "$(notdir $<)"
	$(SILENT) $(CXX) $(ALL_CXXFLAGS) $(FORCE_INCLUDE) -o "$@" -MF "$(@:%.o=%.d)" -c "$<"
$(OBJDIR)/import-svg.o: ../../msdf-atlas-gen/msdfgen/ext/import-svg.cpp
	@echo "$(notdir $<)"
	$(SILENT) $(CXX) $(ALL_CXXFLAGS) $(FORCE_INCLUDE) -o "$@" -MF "$(@:%.o=%.d)" -c "$<"
$(OBJDIR)/resolve-shape-geometry.o: ../../msdf-atlas-gen/msdfgen/ext/resolve-shape-geometry.cpp
	@echo "$(notdir $<)"
	$(SILENT) $(CXX) $(ALL_CXXFLAGS) $(FORCE_INCLUDE) -o "$@" -MF "$(@:%.o=%.d)" -c "$<"
$(OBJDIR)/save-png.o: ../../msdf-atlas-gen/msdfgen/ext/save-png.cpp
	@echo "$(notdir $<)"
	$(SILENT) $(CXX) $(ALL_CXXFLAGS) $(FORCE_INCLUDE) -o "$@" -MF "$(@:%.o=%.d)" -c "$<"
$(OBJDIR)/lodepng.o: ../../msdf-atlas-gen/msdfgen/lib/lodepng.cpp
	@echo "$(notdir $<)"
	$(SILENT) $(CXX) $(ALL_CXXFLAGS) $(FORCE_INCLUDE) -o "$@" -MF "$(@:%.o=%.d)" -c "$<"
$(OBJDIR)/tinyxml2.o: ../../msdf-atlas-gen/msdfgen/lib/tinyxml2.cpp
	@echo "$(notdir $<)"
	$(SILENT) $(CXX) $(ALL_CXXFLAGS) $(FORCE_INCLUDE) -o "$@" -MF "$(@:%.o=%.d)" -c "$<"

-include $(OBJECTS:%.o=%.d)
ifneq (,$(PCH))
  -include $(PCH_PLACEHOLDER).d
endif