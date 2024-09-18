# capi v1.0 - An extraordinary API in C

## Build with CMake

Variable         | Type   | Description          | Sets         |
-----------------|--------|----------------------|--------------|
CMAKE_BUILD_TYPE | String | Defaults to "Debug". | CAPI_DEBUG=1 |

```sh
mkdir build
cd build
cmake .. -DCMAKE_BUILD_TYPE={Relase | Debug}
make
```

## Build with Make
Default build mode is DEBUG. You can turn it off with:
```sh
make { all | re } DEBUG=0
```
