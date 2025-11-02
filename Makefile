CXX = g++

CXXFLAGS = -D _DEBUG -ggdb3 -std=c++17 -O0 -Wall -Wextra -Weffc++ 				 		   		 \
		   -Waggressive-loop-optimizations -Wc++14-compat -Wmissing-declarations 		   		 \
		   -Wcast-align -Wcast-qual -Wchar-subscripts -Wconditionally-supported  		   		 \
		   -Wconversion -Wctor-dtor-privacy -Wempty-body -Wfloat-equal -Wformat-nonliteral 		 \
		   -Wformat-security -Wformat-signedness -Wformat=2 -Winline -Wlogical-op 		   		 \
		   -Wnon-virtual-dtor -Wopenmp-simd -Woverloaded-virtual -Wpacked -Wpointer-arith  		 \
		   -Winit-self -Wredundant-decls -Wshadow -Wsign-conversion -Wsign-promo		   		 \
		   -Wstrict-null-sentinel -Wstrict-overflow=2 -Wsuggest-attribute=noreturn		   		 \
		   -Wsuggest-final-methods -Wsuggest-final-types -Wsuggest-override -Wswitch-default 	 \
		   -Wswitch-enum -Wsync-nand -Wundef -Wunreachable-code -Wunused -Wuseless-cast 		 \
		   -Wvariadic-macros -Wno-literal-suffix -Wno-missing-field-initializers -Wno-narrowing  \
		   -Wno-old-style-cast -Wno-varargs -Wstack-protector -fcheck-new -fsized-deallocation 	 \
		   -fstack-protector -fstrict-overflow -flto-odr-type-merging -fno-omit-frame-pointer 	 \
		   -Wlarger-than=8192 -Wstack-usage=8192 -pie -fPIE -Werror=vla 					 	 \
		   -fsanitize=address,alignment,bool,bounds,enum,float-cast-overflow,float-divide-by-zero,integer-divide-by-zero,leak,nonnull-attribute,null,object-size,return,returns-nonnull-attribute,shift,signed-integer-overflow,undefined,unreachable,vla-bound,vptr

CXXFLAGS += -I include

AOSFLAGS = -I include/AoS

STDFLAGS = -I include/std

ifdef DEBUG
AOSFLAGS += -D LIST_DEBUG -D AOS_LIST
endif

ifdef DEBUG
STDFLAGS += -D STD_LIST_DEBUG -D STD_LIST
endif

SOURCES = src/main.cpp \
		  src/graphCommon.cpp

AOSSOURCES = src/AoS/listCommands.cpp  \
		  	 src/AoS/listDebug.cpp	   \
		  	 src/AoS/listGraph.cpp

STDSOURCES = src/std/stdListCommands.cpp   \
			 src/std/stdListDebug.cpp	   \
		     src/std/stdListGraph.cpp

EXECUTABLE = run

LOG_DIR = log/*

aos:
	$(CXX) $(CXXFLAGS) $(AOSFLAGS) $(SOURCES) $(AOSSOURCES) -o $(EXECUTABLE)

std:
	$(CXX) $(CXXFLAGS) $(STDFLAGS) $(SOURCES) $(STDSOURCES) -o $(EXECUTABLE)

both:
	$(CXX) $(CXXFLAGS) $(AOSFLAGS) $(STDFLAGS) $(SOURCES) $(AOSSOURCES) $(STDSOURCES) -o $(EXECUTABLE)

clean:
	rm -rf $(EXECUTABLE) $(LOG_DIR)
