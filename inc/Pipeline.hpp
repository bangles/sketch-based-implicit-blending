//
//  Pipeline.h
//  ebib_demo
//
//  Created by Ishmeet Singh Kohli on 06/09/17.
//
//

#ifndef Pipeline_h
#define Pipeline_h

#include "Processor.hpp"

namespace ebib {
    
    class Pipeline {
    private:
        Template* _template = NULL;
        
    public:
        Pipeline(Template &inTemplate);
        ~Pipeline();

        void start();
        void generateOperator();
        VectorXf findVs(int res);

    };
}


#endif /* Pipeline_h */
