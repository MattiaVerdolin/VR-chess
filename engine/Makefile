
CXX = g++
AR = ar
LD = g++
WINDRES = windres

INC = -I../dependencies/glm/include
CFLAGS = -Wall -std=c++20 -fPIC -m64 -fexceptions
RCFLAGS = 
RESINC = 
LIBDIR =
LIB =
LDFLAGS = -m64 -lGL -lGLU -lglut -lfreeimage

INC_DEBUG = $(INC)
CFLAGS_DEBUG = $(CFLAGS) -g -D_DEBUG
RESINC_DEBUG = $(RESINC)
RCFLAGS_DEBUG = $(RCFLAGS)
LIBDIR_DEBUG = $(LIBDIR)
LIB_DEBUG = $(LIB)
LDFLAGS_DEBUG = $(LDFLAGS)
OBJDIR_DEBUG = obj/Debug
DEP_DEBUG = 
OUT_DEBUG = bin/Debug/libengine.so

INC_RELEASE = $(INC)
CFLAGS_RELEASE = $(CFLAGS) -O2
RESINC_RELEASE = $(RESINC)
RCFLAGS_RELEASE = $(RCFLAGS)
LIBDIR_RELEASE = $(LIBDIR)
LIB_RELEASE = $(LIB)
LDFLAGS_RELEASE = $(LDFLAGS) -s
OBJDIR_RELEASE = obj/Release
DEP_RELEASE = 
OUT_RELEASE = bin/Release/libengine.so

SRC_DIR = $(wildcard *.cpp)
OBJ_DEBUG = $(SRC_DIR:%.cpp=$(OBJDIR_DEBUG)/%.o)
OBJ_RELEASE = $(SRC_DIR:%.cpp=$(OBJDIR_RELEASE)/%.o)

all: debug release test

clean: clean_debug clean_release clean_test

before_test:
	test -d test/Debug || mkdir -p test/Debug
	test -d test/Release || mkdir -p test/Release

after_test: out_test
	cd test/Debug && LD_LIBRARY_PATH=../../bin/Debug ./engineTest
	cd test/Release && LD_LIBRARY_PATH=../../bin/Release ./engineTest

test: before_test out_test after_test

out_test: before_test
	cd ../engineTest && make all
	cp -r ../engineTest/bin/Debug/* test/Debug
	cp -r ../engineTest/bin/Release/* test/Release

clean_test:
	rm -rf test
	cd ../engineTest && make clean

before_debug: 
	test -d bin/Debug || mkdir -p bin/Debug
	test -d $(OBJDIR_DEBUG) || mkdir -p $(OBJDIR_DEBUG)

after_debug: 

debug: before_debug out_debug after_debug

out_debug: before_debug $(OBJ_DEBUG) $(DEP_DEBUG)
	$(LD) -shared $(LIBDIR_DEBUG) $(OBJ_DEBUG)  -o $(OUT_DEBUG) $(LDFLAGS_DEBUG) $(LIB_DEBUG)

$(OBJDIR_DEBUG)/%.o: %.cpp
	$(CXX) $(CFLAGS_DEBUG) $(INC_DEBUG) -c $< -o $@

clean_debug: 
	rm -f $(OBJ_DEBUG) $(OUT_DEBUG)
	rm -rf bin/Debug
	rm -rf $(OBJDIR_DEBUG)

before_release: 
	test -d bin/Release || mkdir -p bin/Release
	test -d $(OBJDIR_RELEASE) || mkdir -p $(OBJDIR_RELEASE)

after_release: 

release: before_release out_release after_release

out_release: before_release $(OBJ_RELEASE) $(DEP_RELEASE)
	$(LD) -shared $(LIBDIR_RELEASE) $(OBJ_RELEASE)  -o $(OUT_RELEASE) $(LDFLAGS_RELEASE) $(LIB_RELEASE)

$(OBJDIR_RELEASE)/%.o: %.cpp
	$(CXX) $(CFLAGS_RELEASE) $(INC_RELEASE) -c $< -o $@

clean_release: 
	rm -f $(OBJ_RELEASE) $(OUT_RELEASE)
	rm -rf bin/Release
	rm -rf $(OBJDIR_RELEASE)

.PHONY: before_debug after_debug clean_debug before_release after_release clean_release before_test after_test clean_test
