# RapidJson Wrapper
 This is a RapidJson wrapper library made to load JSON data in a hurry.

Simply build and include the *"jsonloader.h"* library header file, and use the `json::` namespace to access the `parseJson` and `loadProperty` function features.

See the [example.cpp](https://github.com/jmscreation/json-loader/blob/main/example/example.cpp) file for an example. You can build with MinGW (Windows) using the [build.bat](https://github.com/jmscreation/json-loader/blob/main/example/build.bat) provided with the example.

Build with linux:
```
c++ -c jsonloader.cpp -o jsonloader.o
g++ -o program-name jsonloader.o *your o files here* -static-libstdc++
```
