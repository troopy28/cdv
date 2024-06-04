# cdv

C++ Data structure Visualization

## Introduction

Header-only C++ 17 library providing runtime data structure visualization capabilities, aimed at graphical debugging, visualization of complex hierarchies, and more. General-purpose graph drawing is also available through an intuitive API.

> [!CAUTION]
> This library is currently in heavy development and no API is entirely stable yet. Many features that are part of the library design goals are still missing and are being worked on.

## Design goals

There already are some C++ data structure visualization libraries out there. This one attempts to tick the following boxes:
- **Seamless standard library support**: printing a hierarchy of std containers, such as a `std::vector<std::vector<int*>>`, is a simple function call ([displaying containers of the standard library](#Displaying-containers-of-the-standard-library)). 
- **Flexible**: the non-intrusive adapter-based approach of this library means no modification of existing code is required to visualize data structures. Custom classes can be visualized just by letting the library know about their members ([displaying a custom class](#Displaying-a-custom-class)). This means you can also display classes from external libraries that you cannot modify! ([displaying external library classes](#Adapting-existing-classes-from-other-libraries)) 
- **Simple integration**: the library is header-only, and entirely fits in the `cdv.hpp` header file. No external dependencies. 

## Features and examples

### Displaying containers of the standard library

### Displaying a custom class

Custom classes are displayed by declaring their members to the library. This is done using three macros:  `CDV_DECLARE_MEMBER`, `CDV_DECLARE_PUBLIC_MEMBER` and `CDV_DECLARE_CUSTOM_MEMBER`. The following class is used as an example: 
```c++
class MyClass
{
public:
	const std::string& get_string() const
	{
    	return my_string;
	}

	int get_int() const
	{
    	return my_int;
	}

	double get_double() const
	{
    	return my_double;
	}

	const char* MyPublicMember { "charptr member" };

private:
	std::string my_string{ "some text" };
	int my_int{ 24 };
	double my_double{ 3.14 };
};
```

#### Declaring a public member

To declare a public member (here `MyPublicMember`), simply use:
```c++
CDV_DECLARE_PUBLIC_MEMBER(MyClass, 0, MyPublicMember)
```
The first argument of the macro is always the type owning the member. The second argument is the index of the member. The third argument is the name of the public member. It must be publicly accessible.

> [!WARNING]  
> The member index must be unique per member of the class, or the code will not compile. Furthermore, when a class has several members, they must have consecutive indices. If an index is missing, all the members after the missing index will not be found by the library and therefore not displayed.

#### Declaring a member

Class members are usually private. If a member has a public getter, it is very easy to declare it using the `CDV_DECLARE_MEMBER` macro. For instance, to declare the members `my_string`, `my_int`, and `my_double`, the following lines are used:
```c++
CDV_DECLARE_MEMBER(MyClass, 1, "my_string", get_string())
CDV_DECLARE_MEMBER(MyClass, 2, "my_int", get_int())
CDV_DECLARE_MEMBER(MyClass, 3, "my_double", get_double())
```
The only difference with the previous macro is that the getter method providing access to the member is specified in the last argument:

- The first argument of the macro is, again, the type owning the member.
- The second argument is, again, the index of the member. 
- The third argument is the name *you want to use to display the value obtained through the getter method*. It differs from the previous macro, where it was directly the C++ name of the member.
- The last argument is the name of the getter method. It must be publicly accessible, and marked as `const`.

> [!NOTE]
> These macros allow you to simply declare members in one line and implement the template machinery to expose this member to the library. Refer to the section [Adapting a template class](#Adapting-a-template-class) for an explanation of the underlying system, in case these macros do not fit the needs of your specific classes.

#### Declaring a custom member

It is possible to declare custom members. Custom members can either be:
- Members for which there is **no accessible const getter for**. It can be useful when:
    - Declaring a member for an external class you cannot modify. See the [next section](#Adapting-existing-classes-from-other-libraries) for more details on this.
    - Declaring a member for a class on which the getter is not marked as `const`.
- **Fake members**. These are members that do not exist in the class, but that you want to display as a member in the graph generated by the library. This can be useful to display additional information about a given instance, such as whether class invariants are verified, for example.

The following example will show how to declare a fake member for the example class `MyClass` described above. We want to display a boolean that will be true to indicate that the string and the integer value represent the same number. This can be done as such:

```c++
TODO TODO TODO TODO TODO TODO TODO TODO TODO TODO TODO TODO TODO TODO 
TODO TODO TODO TODO TODO TODO TODO TODO TODO TODO TODO TODO TODO TODO 
TODO TODO TODO TODO TODO TODO TODO TODO TODO TODO TODO TODO TODO TODO 
TODO TODO TODO TODO TODO TODO TODO TODO TODO TODO TODO TODO TODO TODO 
TODO TODO TODO TODO TODO TODO TODO TODO TODO TODO TODO TODO TODO TODO 
TODO TODO TODO TODO TODO TODO TODO TODO TODO TODO TODO TODO TODO TODO 
```

> [!CAUTION]
> When using this member declaration method to work around a getter not being marked as `const`, make sure that the member does not modify the instance's content (that is: it really *should* be marked as `const`, but you cannot change it for reasons). If the getter modifies the displayed instance's internal state, then using this library as a debugging tool is meaningless and absurd, since it will display potentially incorrect data and alter the state of your program. 

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
