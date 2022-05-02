sources := $(shell ls)
includes := $(filter %.hpp, $(sources))
cpps := $(filter %.cpp, $(sources))

os := $(subst .cpp,.o, $(cpps))

FLAG = -Wc++11-extensions -Wno-unknown-escape-sequence -std=c++11

tocken: $(os)
	$(CXX) $? -o $@

%.o: %.cpp
	$(CXX) $(FLAG) -c $? -o $@
