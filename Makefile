.PHONY: all
all: codepad.exe

CXX      := g++
CPPFLAGS := -imacros src/config.h
CXXFLAGS := -std=c++11 -g -pedantic-errors -Werror -mthreads -mwindows
LDFLAGS  := -lcomctl32 -lgdi32

SRCS := AppWindow CodeEditor global main MainWindow StatusBar Window
OBJS := $(SRCS:%=src/%.o)
DEPS := $(SRCS:%=src/%.d)

codepad.exe: $(OBJS) res/resource.res
	$(CXX) -o $@ $^ $(LDFLAGS) $(LIBS)

# NOTE: -MF is redundant but required for distcc
src/%.o: src/%.cpp src/config.h
	$(CXX) -c -MMD -MF src/$*.d -MP -MT $@ $(CPPFLAGS) $(CXXFLAGS) -o $@ $<

-include $(DEPS)

res/resource.res: res/resource.rc res/codepad.exe.manifest res/icon.ico src/config.h
	windres -Ocoff $< $@

.PHONY: clean
clean:
	rm -f codepad.exe res/resource.res $(OBJS) $(DEPS)
