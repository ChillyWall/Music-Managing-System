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

若使用其他方法安装sqlite3，只需保证相应的文件在cmake的搜索目录中即可，可以通过设置`CMAKE_INCLUDE_PATH`,
`CMAKE_LIBRARY_PATH`和 `LD_LIBRARY_PATH` 环境变量添加头文件(.h)，静态库(.a)和动态库文件(.so)的存放位置。

### Windows

Windows下可以直接在[官网](https://www.sqlite.org/download.html)上下载编译好的二进制文件，其中包括`sqlite3.dll`和`sqlite3.def`文件,
可以使用 `dlltool`(MinGW64提供)或`lib`(MSVC提供)命令生成`sqlite3.lib`文件。

```powershell
dlltool -D sqlite3.dll -d sqlite3.def -l sqlite3.lib

lib /DEF:sqlite3.def /OUT:sqlite3.lib
```

要构建项目还需要头文件`sqlite3.h`，需要下载SQLite官网上的源文件，其中就包括该文件。

之后配置环境变量`CMAKE_INCLUDE_PATH`和`CMAKE_LIBRARY_PATH`，将`sqlite3.h`文件和`sqlite3.lib`文件的地址加入其中，
再配置环境变量`PATH`，将`sqlite3.dll`文件的目录加入其中。

之后cmake便可以正常找到安装的SQLite3库了。

## 构建

该项目使用cmake构建。目前在Linux下使用gcc或clang，在Windows下使用MinGW64, clang或MSVC均可正确构建。

下面是构建示例，该命令将构建输出目录设置为build文件夹，生成器设置为`Ninja`，安装目录为用户目录下的`MusicManagingSystem`文件夹。

```bash
# 将工作目录切换为该项目的根目录
cd path/to/root
# 进行配置
cmake . -B build -G Ninja -DCMAKE_INSTALL_PREFIX="~/MusicManagingSystem"
# 进行构建
cmake --build build
```

推荐使用`Ninja`作为构建工具，亦可选择其他构建工具。

## 安装

运行cmake的install命令，将程序安装到上一步指定的目录。可使用下面的命令。

```bash
cmake --install build
```

在Windows下，动态库文件会被安装到上面指定文件夹下的`bin`文件夹中，程序会寻找同目录下的`.dll`文件。

在Linux系统中，动态库文件会被安装到下面的`lib`文件夹下，要使程序可以找到`lib`文件夹中的`.so`文件，
可以将`lib`目录添加到环境变量`LD_LIBRARY_PATH`中，或者将程序安装到系统目录中。

修改环境变量可以使用下面语句：

```bash
export LD_LIBRARY_PATH="$LD_LIBRARY_PATH:path/to/MusicManagingSystem"
```

## 使用

运行`MusicManagingSystem(.exe)`即可运行程序，会在当前环境的`HOME`(Unix、mac)或`USERPROFILE`(Windows)目录下创建一个SQLite3数据库文件`data.sqlite3`，
若读取系统中环境变量失败，则会在当前目录下创建。

Windows的cmd和powershell等会使用系统默认语言的编码（中文系统为`GBK`）处理输入输出，而SQLite3数据库使用UTF-8，
这会造成中文乱码的问题。

故Windows环境下，在cmd中使用需要设置`utf-8`编码以处理中文，可通过如下指令切换编码页：

```bat
chcp 65001
```

powershell中需要设置输入输出编码均为`utf-8`。设置编码的命令为：

```ps1
[Console]::OutputEncoding = [System.Text.UTF8Encoding]::new()
[Console]::InputEncoding = [System.Text.UTF8Encoding]::new()
```

Windows默认的Terminal通过以上指令可以做到正确处理中文，但如果使用其他终端模拟器如alacrity，可能仍会出现乱码，
最简单的解决方法是使用Windows Terminal来运行。

Linux环境下一般默认使用`utf-8`编码，无需手动设置，即可正常使用(以Ubuntu20, 22为例)。
