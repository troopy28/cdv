# cdv

C++ Data structure Visualization

## Introduction

Header-only C++ 17 library providing runtime data structure visualization capabilities, aimed at graphical debugging, visualization of complex hierarchies, and more. General-purpose graph drawing is also available through an intuitive API.



## Design goals

There already are some C++ data structure visualization libraries out there. This one attempts to tick the following boxes:
- **Seamless standard library support**: printing a hierarchy of std containers, such as a `std::vector<std::vector<int*>>`, is a simple function call ([displaying containers of the standard library](#Displaying-containers-of-the-standard-library)). 
- **Flexible**: the non-intrusive adapter-based approach of this library means no modification of existing code is required to visualize data structures. Custom classes can be visualized just by letting the library know about their members ([displaying a custom class](#Displaying-a-custom-class)). This means you can also display classes from external libraries that you cannot modify! ([displaying external library classes](#Adapting-existing-classes-from-other-libraries)) 
- **Simple integration**: the library is header-only, and entirely fits in the `cdv.hpp` header file. No external dependencies. 

## Features and examples

### Displaying containers of the standard library

### Displaying a custom class

### Adapting existing classes from other libraries

### Adapting a template class

Template classes require a little more work to adapt, because no macros are provided by the library to do the dirty work: you must define the adapting structures manually.

This example shows how to adapt the **template class** `std::pair<T1, T2>`, meaning the adapter will work with any `T1` and `T2`. Note that this is directly how the library adapts `std::pair<T1, T2>`, the code can be found in the library's source code.



### Fully custom graphs

## Supported compilers

## CMake integration

## License

## Contact

## Notes
