LD = g++
target = turing
deps = $(shell find ./ -name "*.h")
src = $(shell find ./ -name "*.cpp")
obj = $(src:%.cpp=%.o) 

$(target): $(obj)
	$(LD) -o $(target) $(obj)
 
%.o: %.cpp $(deps)
	$(LD) -c $< -o $@
 
clean:
	rm -rf $(obj) $(target)