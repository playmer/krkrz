# KirikiriZ CMake

## Requirements
- Windows 10 or Higher (for building)
  - Technically Windows 8 and 8.1 likely can also build this project, but that's currently untested.
- [Nasm (>= 2.10.09, <=2.14.03rc2), x64 or x86 should be fine](https://www.nasm.us/pub/nasm/releasebuilds)
- [CMake (>= 3.24.0) x64](https://cmake.org/download/)
- [Git (>= 2.30)](https://git-scm.com/downloads)
- [Visual Studio 2022 Community](https://visualstudio.microsoft.com/vs/community/)
  - Make sure to install the "Desktop Development with C++" workload during installation

CMake and Nasm will need to be in your PATH, such that running the following commands would produce something similar to the following output.

```batch
> nasm -v
NASM version 2.10.09 compiled on Jul 22 2013
> cmake --version
cmake version 3.24.2

CMake suite maintained and supported by Kitware (kitware.com/cmake).
> git --version
git version 2.33.1.windows.1
```

## How to Build

Ensure your cloned repository has it's submodules checked out. We only specifically require the baseclasses submodule checked out. It's likely we'll bring this source in-tree in the future so we don't need to rely on a submodule. This code is from a (likely) frozen Windows 7 code sample, so it's fairly safe to bring in.

### Build Only
If building is all you care about, simply run:
```batch
GenerateProjects.bat
```
From inside the repo's source directory. As long as the above tooling is correctly installed and configured, an `output` directory will be created which contains `tvpwin64.exe` and `tvpwin32.exe`.

# Original Description Follows
# 吉里吉里Z

吉里吉里Zは吉里吉里2フォークプロジェクトです。  

2016/08/18  
リポジトリの分割は一通り完了。  
未追加のプラグインは各 Author が追加予定。
今回 external 内の外部ライブラリをサブモジュール化。  
external の各フォルダが空の場合は、サブモジュールのアップデートを。  
今後、 Android 版開発に伴い、ディレクトリ構成が変更される可能性があります。

2016/08/09  
プラグイン等全て一つのリポジトリに入れていたものを削除し、このリポジトリには本体のソースコードのみ入れるようになりました。  
旧ディレクトリ構成は <https://github.com/krkrz/krkrz/tree/last_hodgepodge_repository> ブランチを参照してください。

従来の構成に近い全てを含んだリポジトリは、<https://github.com/krkrz/krkrz_dev> になりました。  
各プラグイン等をサブモジュールとして参照し、独立した形で管理するようになっています。
