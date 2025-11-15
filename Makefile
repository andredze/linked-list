CXX = g++

CXXFLAGS =  -Wshadow -Winit-self -Wredundant-decls -Wcast-align -Wundef 							\
			-Wfloat-equal -Winline -Wunreachable-code -Wmissing-declarations 						\
			-Wmissing-include-dirs -Wswitch-default -Weffc++ 										\
			-Wmain -Wextra -Wall -g -pipe -fexceptions -Wcast-qual -Wconversion 					\
			-Wctor-dtor-privacy -Wempty-body -Wformat-security -Wformat=2 -Wignored-qualifiers 		\
			-Wlogical-op -Wno-missing-field-initializers -Wnon-virtual-dtor -Woverloaded-virtual	\
			-Wpointer-arith -Wsign-promo -Wstack-usage=8192 -Wstrict-aliasing 						\
			-Wstrict-null-sentinel -Wtype-limits -Wwrite-strings -Werror=vla -D_DEBUG 				\
			-D_EJUDGE_CLIENT_SIDE \

CXXFLAGS += -Iinclude

AOSFLAGS = -Iinclude/AoS -DAOS_LIST

STDFLAGS = -Iinclude/std -DSTD_LIST

ifdef DEBUG
AOSFLAGS += -DLIST_DEBUG
endif

ifdef DEBUG
STDFLAGS += -DSTD_LIST_DEBUG
endif

SOURCES = src/main.cpp \
		  src/graphCommon.cpp

AOSSOURCES = src/AoS/listCommands.cpp  \
		  	 src/AoS/listDebug.cpp	   \
		  	 src/AoS/listGraph.cpp

STDSOURCES = src/std/stdListCommands.cpp   \
			 src/std/stdListDebug.cpp 	   \
		     src/std/stdListGraph.cpp

EXECUTABLE = run

LOG_DIR = log/* std_log/*

aos:
	$(CXX) $(CXXFLAGS) $(AOSFLAGS) $(SOURCES) $(AOSSOURCES) -o $(EXECUTABLE)

std:
	$(CXX) $(CXXFLAGS) $(STDFLAGS) $(SOURCES) $(STDSOURCES) -o $(EXECUTABLE)

both:
	$(CXX) -Iinclude $(AOSFLAGS) $(STDFLAGS) $(SOURCES) $(AOSSOURCES) $(STDSOURCES) -o $(EXECUTABLE)

clean:
	rm -rf $(EXECUTABLE) $(LOG_DIR)
