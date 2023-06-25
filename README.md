# vscode-qt-app

Features:

- dynamic libraries on Windows/OSX
  - `arm64-osx-dynamic` currently needs [vcpkg: branch fix-install-name-rpath-osx-dynamic](https://github.com/liviaerxin/vcpkg/tree/fix-install-name-rpath-osx-dynamic)
- vcpkg manifest mode
  - dynamic libraries are installed into `build/vcpkg_installed` in project

mac osx:

[Optional]Install

```sh
cd build
vcpkg install --host-triplet=arm64-osx-dynamic --triplet=arm64-osx-dynamic
```

Configure, Build and Install

```sh
cmake -B build -S . --preset=debug -DVCPKG_TARGET_TRIPLET=arm64-osx-dynamic -DVCPKG_HOST_TRIPLET=arm64-osx-dynamic
cmake -B build -S . --preset=debug -DVCPKG_TARGET_TRIPLET=arm64-osx-dynamic -DVCPKG_HOST_TRIPLET=arm64-osx-dynamic -DVCPKG_BINARY_SOURCES=clear

cmake -B build -S . --preset=debug -DVCPKG_TARGET_TRIPLET=arm64-osx-dynamic -DVCPKG_HOST_TRIPLET=arm64-osx-dynamic --fresh

cmake --build build

cmake --install build
```

qt widget application:

```jsonc title="vcpkg.json"
{
  "dependencies": [
    {
      "name": "qtbase",
      "default-features": false,
      "features": [
        "gui",
        "widgets"
      ]
    }
  ]
}
```

qt quick+qml application:

```jsonc title="vcpkg.json"
{
  "dependencies": [
    {
      "name": "qtbase",
      "default-features": false,
      "features": [
        "gui",
        "widgets"
      ]
    },
    "qttools",
    "qtdeclarative"
  ]
}
```

[Getting Started with Manifest mode | Microsoft Learn](https://learn.microsoft.com/en-us/vcpkg/examples/manifest-mode-cmake)

https://doc.magnum.graphics/magnum/platforms-macos.html

https://stackoverflow.com/questions/35612687/cmake-macos-x-bundle-with-bundleutiliies-for-qt-application

https://github.com/ionyshch/cmake-bundle-macos

https://pabloariasal.github.io/2018/02/19/its-time-to-do-cmake-right/

https://stackoverflow.com/questions/17974439/cant-use-fixup-bundle-to-create-a-portable-bundle-with-qt