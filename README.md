# Installation

1. Clone repo with submodules

```
git clone --recurse-submodules -j8  git@github.com:EpineCloud/example-epine-sdk-cpp.git
```

2. Follow installation instruction for dependency library at `lib/epine-sdk-cpp/README.md`

3. Compile

```
cmake .
make
```

4. Launch

```
./sdk_test
```