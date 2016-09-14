#include "d:\work\bc\include\mcadincl.h"


    LRESULT testFunction(   LPCOMPLEXSCALAR     c,                 // summiruet 2 chisla c=a+b
                                LPCCOMPLEXSCALAR    a,
                                LPCCOMPLEXSCALAR    b   );

    FUNCTIONINFO    test =
    {
    "test",                          // Name by which mathcad will recognize the function
    "a,b",                              // realsum will be called as realsum(a,b)
    "real sum of scalars a and b",      // description of realsum(a,b)
    (LPCFUNCTION)testFunction,       // pointer to the executible code
    COMPLEX_SCALAR,                     // the return type is also a complex scalar
    2,                                  //  the function takes on 2 arguments
    { COMPLEX_SCALAR, COMPLEX_SCALAR}   // both arguments are complex scalars
    };


    LRESULT testFunction(   LPCOMPLEXSCALAR     c,
                                LPCCOMPLEXSCALAR    a,
                                LPCCOMPLEXSCALAR    b   )
    {

        // first check to make sure
        // a has no imaginary component

        if ( a->imag != 0.0 )
            return MAKELRESULT(1,1);

        if ( b->imag != 0.0 )
            return MAKELRESULT(1,2);
        
        // otherwise, all is well so add a and b
        c->real = a->real + b->real;
        

        return 0;               // return 0 to indicate there was no error
            
    }           
    
