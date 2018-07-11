# source-mapping
Source mapping utility for C style code.

# Examples

Some examples can be found in examples folder. The compiler used to test the library
has been g++.

# Purpose

This library provides some classes and functions to extract information of used namespaces
and called functions in each defined function in the provided source codes. Also, facilite
create a dependence graph in [dot](https://en.wikipedia.org/wiki/DOT_(graph_description_language)) format that can be ploted, for example, with [graphviz](https://www.graphviz.org/)
library.

![alt text](https://github.com/vigial/source-mapping/blob/master/examples/dependencies.png)

# Limitations

At the moment, the library has the following known limitations,

- Can distinguish between overload function declaration (functions with same name but diferent aguments),
  but can't distinguish which overload has been called.
- If the function is in a namespace or is a class member, the namespace/class will be part of defined
  function name (namespace::fname()). Because that, when check called functions, only the functions 
  called specifying the namespace will be identified as a namespace::fname call. This provoque that
  if "using namespace" statment is used on namespaces with defined functions, the call to that functions
  will not be identified correctly.
- Only C/C++ code style is supported at this moment.
