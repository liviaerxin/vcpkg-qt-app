# vscode-qt-app

mac osx:

```sh
make -B build -S . --preset=debug -DVCPKG_TARGET_TRIPLET=arm64-osx-dynamic -DVCPKG_HOST_TRIPLET=arm64-osx-dynamic
cmake --build build
cmake --install build
```

https://doc.magnum.graphics/magnum/platforms-macos.html

https://stackoverflow.com/questions/35612687/cmake-macos-x-bundle-with-bundleutiliies-for-qt-application

https://github.com/ionyshch/cmake-bundle-macos

https://pabloariasal.github.io/2018/02/19/its-time-to-do-cmake-right/

https://stackoverflow.com/questions/17974439/cant-use-fixup-bundle-to-create-a-portable-bundle-with-qt