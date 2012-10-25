# libJSONpp

`libJSONpp` is a C++ library for reading and writing JSON objects.

It's designed to be fast and easy to use.  By default, it accepts common variants of JSON, 
but always outputs correct JSON.

The main difference between this Parser and others, (jsoncpp. | zoolib.) is that it is designed 
to be permissive rather than restrictive. 
It is also designed to work well with the ECMA standard rather than the formal JSON format. 
This increases compatibility with JavaScript. 
Also, compilation does not rely on any stange make-tools. A makefile, and Visual Studio Project file are included.


## Features:

Accepts Single ( //) and Multi-line Comments (/* hi */ )
Allows unquoted names (  { hello : "world" }  )
Internally uses all Unicode
Built-in UTF-8 conversions
No external dependencies (Apart from a modern C++ stdlib) or build requirements


## Performance

The library should perform more then well enough for day-to-day usage. 
A simple speed test using a 52.6Mb complex Json file containing 3,971,363 values 
takes 6 seconds to load, parse, and iterate over, using Windows Mmapping on an dual core Athlon 2Ghz machine. 
Total memory usage was around 108 Mb.

There is some scope for improvements in the code performance.

## Example

```c++
#include <iostream>
#include <string>
#include "json/parser.h"
 
using namespace std;
 
int main(int argc, wchar_t* argv[])
{
char *json_string="{ hello : \"World\", \"number\": 1}";
json::parser myparser;
json::value *myvalue=myparser.parse(json_string,strlen(json_string));
myvalue->addobjectvalue(L"Boolean",new json::value(true));
wstring *out_string=myvalue->getstring(L"",true);
wcout<<L"Output:"<<(out_string->c_str())<<endl;
delete out_string;
delete myvalue;
cin.get();
return 0;
}
```
Should output:

```js
{ "Boolean" : true , "hello" : "World" , "number" : 1 }
```
