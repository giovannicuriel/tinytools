all: test-1.app test-2.app test-3.app
test-%.app: test-%.o
	$(CXX) $^ -o $@
clean:
	rm -fr *.app *.o
%.o: %.cpp
	${CXX} -O0 -g3 -Wall -c $< -o $@
