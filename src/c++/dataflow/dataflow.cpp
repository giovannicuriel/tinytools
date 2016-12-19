#include <iostream>
#include <tuple>
#include <unistd.h>

struct AbstractTask {
    virtual int32_t execute() = 0;
};

template<typename InputParameters, typename OutputParameters> 
struct Task : public AbstractTask {
    InputParameters inputParameters;
    OutputParameters * outputParameters;
    
    template<typename X>
    void setOutputParameters(Task<OutputParameters, X> & out) {
        outputParameters = &out.inputParameters;
    };
    
    
};

struct Sum : public Task<std::tuple<int, int>, int> {
    int32_t execute() { 
        int a = std::get<0>(inputParameters);
        int b = std::get<1>(inputParameters);
        *outputParameters = a + b;
        return 0;
    }
};

struct Printer : public Task<int, int> {
    int32_t execute() {
        std::cout << inputParameters << std::endl;
        return 0;
    };
};

struct Source : public Task<int, std::tuple<int, int>> {
    int32_t execute() {
        std::get<0>(*outputParameters) = 10;
        std::get<1>(*outputParameters) = 20;
        return 0;
    };
};

int main(void) {
    Source source1;
    Sum s1;
    Printer p1;
    
    source1.setOutputParameters(s1);
    s1.setOutputParameters(p1);
    
    source1.execute();
    s1.execute();
    p1.execute();
}
