LD = g++
target = turing
deps = $(shell find ./ -name "*.h")
src = $(shell find ./ -name "*.cpp")
obj = $(src:%.cpp=%.o) 

$(target): $(obj)
	$(LD) -o $(target) $(obj)
 
%.o: %.cpp $(deps)
	$(LD) -c $< -o $@

test:
	./turing ../Program/case2.tm 1111111111111111111111111

vtest:
	./turing -v ../Program/case2.tm 1111111111111111111111111
 
clean:
	rm -rf $(obj) $(target)