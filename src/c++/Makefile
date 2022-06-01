all: \
	patricia-tree-demo.app \
	test-1.app \
	test-2.app \
	test-3.app \
	test-4.app \
	minesweep.app
%.app: %.o
	$(CXX) $^ -o $@
clean:
	rm -fr *.app *.o
%.o: %.cpp
	${CXX} -std=c++20 -O0 -g3 -Wall -c $< -o $@
