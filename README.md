# README

本例程采用了"stm32cubemx + vscode + ozone"的软件工具流, 工具链采用了CMakeLists生成ELF文件(CMake + Ninja) + ELF转HEX(arm-none-eabi-objcopy) + jlink下载(JFlash)的方法

## 基本软件的安装

我将会从一台电脑装好windows开始——

1. 下载steam++，搜索watt，到官方网站上下载（注意一定要下载windows版本），下载完成后打开steam++，不用登陆，找到网络加速界面，勾选github加速选项，点击开始加速。
2. 下载主要软件
   1. 搜索vscode(https://code.visualstudio.com/Download) 并下载windows版本。
   2. 搜索st中文官网，进入后搜索stm32cubemx下方设计资源中找到按类型筛选中找到固件和软件项下PC端软件，可以找到stm32cubemx下载链接 (https://www.stmcu.com.cn/Designresource/detail/software/711298) ，点击进入后点击下载会要求你登录st账号，中文官网允许使用微信登陆或者手机号登陆，不需要输入密码，之后肯定会有人忘密码的，st账号会经常用到，密码最好记住，不要设置太复杂。
   3. 搜索jlink，找到SEGGER官网，进入下载jflash的64bit，windows版本(https://www.segger.com/downloads/jlink/) ，建议下载7.96版本以上的，jflash下载需要你同意一份协议，这个不用管，无脑同意就行。
   4. 搜索ozone，最好搜索segger ozone，还是在SEGGER官网里下载ozone的64位windows版本(https://www.segger.com/products/development-tools/ozone-j-link-debugger/) ，这个直接下载。
   5. 搜索git，还是git官网下载git安装程序(https://git-scm.com/downloads) 。
   6. 搜索msys2(https://www.msys2.org/) ，进入下载。
   7. 搜索cmake，在他们官网里或者直接上github上找到cmake的64位windows安装程序(https://github.com/Kitware/CMake/releases/download/v4.0.0/cmake-4.0.0-windows-x86_64.msi) ，注意这里需要你开着加速器，不然很难下载的了。
   8. 搜索ninja-build，在官网最下有一行 download the Ninja binary ，点击进入ninja的github网页，找到ninja-win的zip文件(https://github.com/ninja-build/ninja/releases) ，点击下载。
3. 安装主要程序，基本上就是无脑一直点就可以，无非是换换盘之类的，除了git可能有些复杂，就不再这里赘述了。
4. 安装完软件后，首先是打开msys2.exe，输入命令，之后就是enter，enter，y，enter了。

```bash
    pacman -S mingw-w64-x86_64-toolchain mingw-w64-x86_64-arm-none-eabi-toolchain mingw-w64-x86_64-ccache  mingw-w64-x86_64-openocd
```

## 环境变量和vscode配置

1. 环境变量的添加：
    1. jflash.exe所在的目录
    2. mingw64的bin目录
    3. cmake的bin目录
    4. ninja-build.exe所在的目录(不加也可以)
2. vscode的setting.json文件

```json
    "terminal.integrated.defaultProfile.windows": "mysys2-mingw64", // 若希望为默认终端可以加入这一句
        "terminal.integrated.profiles.windows": {
        "mysys2-mingw64": {
            "path": "cmd.exe", // 意思是使用cmd作为父进程启动终端
            "args": ["/c","D:\\msys64\\msys2_shell.cmd -defterm -mingw64 -no-start -here"]
            // msys2_shell.cmd在msys的安装目录下，这里要改为你自己的目录
        }
        },
        "makefile.makePath": "mysys2-mingw64",
```

## git的配置

1. 配置git，如果你已经有了github账号，那么打开git bash，输入：

```bash
    git config --global user.name "君の名は"
    git config --global user.email 君のemail
    git config --global http.sslVerify false //如果发现推送时或者拉取时有问题可以试试这行代码
```

## 准备工作的收尾

1. 在vscode上登陆github这个没难度。
2. 到这里准备工作就基本完成了。(为什么这个markdown报错这么多！！！)
3. 接下来就是一个小测验了

## 测试工作-创建工程

1. 打开stm32cubemx并点击右上角st徽标登陆st账号，登陆后在打开时主页面先找到check for updata，点击检查版本更新情况(可选)，之后点击下面的install package & xxx(这个我忘了叫什么了)，找到stm32f1，stm32f4，stm32h7系列的开发支持包并下载。
    · 这一步可能会有登陆失败，链接不上服务器的情况，如果你使用的是从正点原子或者csdn上找到的古老的安装包，这种情况极有可能是因为你电脑的java环境不对，正确的java环境应该是java8u351(建议)，这里推一篇csdn的文章(https://blog.csdn.net/qq_63922192/article/details/127892120) ，但是我还是建议你直接去官网上下载最新的stm32cubemx来解决这个问题，因为旧版本的cubemx没有把java环境集成到安装包中，但是最新的版本是集成的，如果你想之后再学习一下java，那么我其实不建议你为了cubemx再配置一个电脑的java环境，现在网络流行的java教学我记得是java18u的版本
2. 点击左边第一个选项创建一个工程，等待他自动弹出问答框，选择本工程需要的mpu，本例程采用的是嵌入式的白月光stm32f103c8t6，选择后进入配置环节，注意配置sys里的debug和rcc，配置好时钟树和文件选项-文件名-工程所在位置-使用CMake开发-然后反正就是各种配置，注意选择CMake开发，就是在之前选择MDK地方换成CMake就可以了，最后点击创建工程
3. 在vscode中打开你刚创建的文件夹，注意打开后一级目录可以看到CMakeLists.txt不然你就是打开了工程文件夹的父级目录，这里建议你先在资源管理器里打开文件夹到可以看见CMakeLists.txt之后直接空白处右键在终端打开然后输入我最会的视觉代码。

```powershell
code .
```

## 测试工作-编译工程

1. 打开下方终端右边有一个选择终端的选项打开powershell然后开始操作

```powershell
mkdir build
cd build
```

```powershell
cmake .. -G 'Ninja'
ninja -v
```

最后如果终端没报错并且打开build文件夹可以找到 “工程名”.elf 的文件那么就是成功了。

## 编译过程-使用MinGW

打开终端powershell，和Ninja流一样先创建build，之后在build文件夹里用powershell执行以下命令

```powershell
cmake .. -G 'MinGW Makefiles'
```

之后打开bash终端运行下面代码

```bash
mingw32-make -j24
```

过程无报错且最后在build文件夹中生成目标elf文件即可
