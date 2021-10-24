sample-1.app: test-template-2.o
	$(CXX) $^ -o $@
clean:
	rm -fr *.app *.o
%.o: %.cpp
	${CXX} -O0 -g3 -Wall -c $< -o $@
