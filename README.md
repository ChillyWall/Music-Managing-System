# 音乐信息管理系统

该项目为程序设计基础实验中的实验。

该项目用于管理音乐信息。

## 依赖

该项目使用sqlite3作为数据库，请确保已安装sqlite3相关依赖。

## 安装依赖

### Linux(以Ubuntu为例)

直接使用包管理器安装sqlite3相关库。

```bash
sudo apt install sqlite3 libsqlite3-dev
```

之后`sqlite3.h`和其他库文件就被成功安装到系统中，cmake的find_package功能即可正常运行。

若使用其他方法安装sqlite3，只需保证相应的文件在cmake的搜索目录中即可，可以通过设置`CMAKE_INCLUDE_PATH`, `CMAKE_LIBRARY_PATH`和 `LD_LIBRARY_PATH` 环境变量添加头文件(.h)，静态库(.a)和动态库文件(.so)的存放位置。

### Windows

Windows下可以直接在[官网](https://www.sqlite.org/download.html)上下载编译好的二进制文件，其中包括`sqlite3.dll`和`sqlite3.def`文件, 可以使用 `dlltool`(MinGW64提供)或`lib`(MSVC提供)命令生成`sqlite3.lib`文件。

```powershell
dlltool -D sqlite3.dll -d sqlite3.def -l sqlite3.lib

lib /DEF:sqlite3.def /OUT:sqlite3.lib
```

## 构建

使用cmake构建该项目。目前在Linux下使用gcc或clang，在Windows下使用MinGW64, clang或MSVC均可正确构建。

```bash
cmake . -B build -G Ninja
cmake --build build
```
