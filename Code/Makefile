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
	./turing ../Program/test.tm 000111

vtest:
	./turing -v ../Program/test.tm 1001
 
clean:
	rm -rf $(obj) $(target)