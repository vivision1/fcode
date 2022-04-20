
CFLAGS = -w -Os -std=c++17 `pkg-config gtk+-3.0 --cflags` -Wfatal-errors 
# CFLAGS +=-fpermissive
INC = -I. -I../Mpm/lib -Ilibscintilla -DGTK
OUT_DIR=.
LDIR=-L../Mpm/lib/.lib -Llibscintilla  -LC:\msys64\mingw64\lib\gcc\x86_64-w64-mingw32\10.1.0
LDFLAGS+= -s  -Wl,--as-needed 
# LDFLAGS= -s    -Wl,--as-needed `pkg-config gtk+-3.0 gthread-2.0 gmodule-no-export-2.0 --libs`

OUT_FILE_NAME = code

DEPENDENCIES=makefile 

ifeq ($(OS),Windows_NT)
    uname_S := Windows
else
    uname_S := $(shell uname -s)
endif

ifeq ($(uname_S), Windows)
	LDFLAGS+= -mwindows 
	OBJ_DIR=.objw64
	LDFLAGS+= -lscintillagtk3w 
	# LDFLAGS+= -ldlibw64 
	# DFLAGS= -DWIN32 -DSCI_LEXER -DSCI_NAMESPACE  -D_CRT_SECURE_NO_DEPRECATE -D_SCL_SECURE_NO_WARNINGS -D_WINDOWS -D_USRDLL -fpermissive
	# LDFLAGS+= -lflscintillaw64 -lglew32 -l:libglew32.a -lcurl -lBZ2 
	LDFLAGS+= -l:libboost_system-mt.a -l:libboost_chrono-mt.a -l:libboost_serialization-mt.a -l:libboost_iostreams-mt.a -l:libboost_thread-mt.a    -l:libboost_filesystem-mt.a -l:libboost_regex-mt.a
	LDFLAGS+= -l:libbz2.a -l:libwinpthread.a
	LDFLAGS+=  -l:libstdc++.a
	# LDFLAGS+= -l:\\msys64\\mingw64\\lib\\gcc\\x86_64-w64-mingw32\\10.1.0\\libstdc++.a
	# LDFLAGS+= -lfltk -lfltk_gl -liconv -lopengl32 -lgdi32 -lglu32 -lm   -l:libstdc++.a
	EXE=.exe
endif
ifeq ($(uname_S), Linux)
	OBJ_DIR=.objl64
	LDFLAGS= -lscintillagtk3l 
	# LDFLAGS+=-ldlibl64  
	LDFLAGS+= -l:libboost_system.a -l:libboost_chrono.a -l:libboost_serialization.a -l:libboost_iostreams.a -l:libboost_thread.a    -l:libboost_filesystem.a -l:libboost_regex.a 
	LDFLAGS+= -lm -lrt -l:libstdc++.a
	# LDFLAGS+= -lrt -l:libeasylzma_s.a  -lcurl  -l:libssh2.a  -l:libssl.a  -l:libgcrypt.a  -l:libcrypto.a  -l:libgpg-error.a  -lIlmImf  -l:libboost_system.a -l:libboost_chrono.a -l:libboost_serialization.a -l:libboost_iostreams.a -l:libboost_thread.a    -l:libboost_filesystem.a -l:libboost_regex.a  -lIlmImfUtil  -l:libbz2.a  -l:libz.a -lpthread -ldl  -l:libfltk_gl.a  -l:libfltk_images.a  -ljpeg  -lpng  -l:libfltk.a -ldl -lm -lopencv_calib3d -lopencv_core -lopencv_features2d -lopencv_flann -lopencv_highgui -lopencv_imgproc  -lopencv_ml  -lopencv_objdetect -lopencv_photo -lopencv_stitching -lopencv_superres -lopencv_ts -lopencv_video -lopencv_videostab  -lGLU -l:libGL.so -l:libGLEW.so  -lfreetype  -lX11 -lXi -lXinerama -lXft  -lsupc++ -lfontconfig -lXrender -lXcursor  -lxcb-sync -lxcb -lXau -lXext -lXdmcp -lXfixes -l:libstdc++.a 

endif

LDFLAGS+=  $(shell pkg-config  gtk+-3.0 gthread-2.0 gmodule-no-export-2.0 --libs) 

LDFLAGS+= -l:libbz2.a -l:libwinpthread.a
	
OBJSCPP= $(patsubst %.cpp,$(OBJ_DIR)/%.cpp.o,$(wildcard *.cpp))
OBJSC= $(patsubst %.c,$(OBJ_DIR)/%.c.o,$(wildcard *.c))

all: dirmake $(OUT_FILE_NAME)$(EXE)

# Enumerating of every *.cpp as *.o and using that as dependency
$(OUT_FILE_NAME)$(EXE):  $(OBJSCPP) $(OBJSC) $(DEPENDENCIES)
	echo $(OUT_FILE_NAME)
	$(CC)  $(OBJSCPP) $(OBJSC) -o $(OUT_DIR)/$@ $(LDIR) $(LDFLAGS) 
	./code


#Compiling every *.cpp to *.o
$(OBJ_DIR)/%.cpp.o: %.cpp 
	$(CC) -c $(INC) $(CFLAGS) $(DFLAGS) -o $@  $<
$(OBJ_DIR)/%.c.o: %.c 
	$(CC) -c $(INC) $(CFLAGS) $(DFLAGS) -o $@  $<
	
dirmake:
	@mkdir -p $(OUT_DIR)
	@mkdir -p $(OBJ_DIR)
	
clean:
	rm -f $(OBJ_DIR)/*.o $(OUT_DIR)/$(OUT_FILE_NAME)

rebuild: clean all

.PHONY: all remake clean cleaner