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

## 使用

运行Main.exe即可开始使用，会在当前目录的父目录创建一个数据库文件data.db。

Windows环境下，在cmd中使用需要设置utf-8编码以处理中文，可通过如下指令切换编码页：

```bat
chcp 65001
```

powershell中需要设置输入输出编码均为utf-8。设置编码指令：

```ps1
[Console]::OutputEncoding = [System.Text.UTF8Encoding]::new()
[Console]::InputEncoding = [System.Text.UTF8Encoding]::new()
```

Windows默认的Terminal通过以上指令可以正常使用，但在一些终端模拟器可能会仍会出现其它的问题，如alacritty在Windows下运行该程序会出现输入的中文无法正确被识别的问题。

Linux环境下无需设置即可正常使用(Ubuntu20, 22)。
