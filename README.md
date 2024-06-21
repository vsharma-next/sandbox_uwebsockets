
### Get prerequisites

#### uWebSockets

```bash

1.
git clone --recurse-submodules https://github.com/uNetworking/uWebSockets.git

2. Modify build.c ( add examples/execs/ for execs )
 
    for (int i = 0; i < sizeof(EXAMPLE_FILES) / sizeof(char *); i++) {
            if (run("%s%s examples/%s.cpp %s -o examples/execs/%s%s", CXX, CXXFLAGS, EXAMPLE_FILES[i], LDFLAGS, EXAMPLE_FILES[i], EXEC_SUFFIX)) {
                return -1;
            }
        }

3. mkdir examples/execs

4. make
```

#### simdjson

```bash
wget https://raw.githubusercontent.com/simdjson/simdjson/master/singleheader/simdjson.h https://raw.githubusercontent.com/simdjson/simdjson/master/singleheader/simdjson.cpp https://raw.githubusercontent.com/simdjson/simdjson/master/jsonexamples/twitter.json
```

#### compilation of counter
```bash
g++ -std=c++20 -I./uWebSockets/src -I./uWebSockets/uSockets/src ./simdjson/simdjson.cpp ./counter.cpp ./uWebSockets/uSockets/*.o -lz -o ./counter.exe
```
