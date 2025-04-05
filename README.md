# README

本例程采用了"stm32cubemx + vscode + ozone"的软件工具流, 工具链采用了CMakeLists生成ELF文件(CMake + Ninja) + ELF转HEX(arm-none-eabi-objcopy) + jlink下载(JFlash)的方法

## 基本软件的安装

我将会从一台电脑装好windows开始——

1. 下载steam++，搜索watt，到官方网站上下载（注意一定要下载windows版本），下载完成后打开steam++，不用登陆，找到网络加速界面，勾选github加速选项，点击开始加速。
2. 下载主要软件
   1. 搜索vscode(https://code.visualstudio.com/Download)并下载windows版本。
   2. 搜索st中文官网，进入后搜索stm32cubemx下方设计资源中找到按类型筛选中找到固件和软件项下PC端软件，可以找到stm32cubemx下载链接 (https://www.stmcu.com.cn/Designresource/detail/software/711298)，点击进入后点击下载会要求你登录st账号，中文官网允许使用微信登陆或者手机号登陆，不需要输入密码，之后肯定会有人忘密码的，st账号会经常用到，密码最好记住，不要设置太复杂。
   3. 搜索jlink，找到SEGGER官网，进入下载jflash的64bit，windows版本(https://www.segger.com/downloads/jlink/)，建议下载7.96版本以上的，jflash下载需要你同意一份协议，这个不用管，无脑同意就行。
   4. 搜索ozone，最好搜索segger ozone，还是在SEGGER官网里下载ozone的64位windows版本(https://www.segger.com/products/development-tools/ozone-j-link-debugger/)，这个直接下载。
   5. 搜索git，还是git官网下载git安装程序(https://git-scm.com/downloads)。
   6. 搜索msys2(https://www.msys2.org/)，进入下载。
   7. 搜索cmake，在他们官网里或者直接上github上找到cmake的64位windows安装程序(https://github.com/Kitware/CMake/releases/download/v4.0.0/cmake-4.0.0-windows-x86_64.msi)，注意这里需要你开着加速器，不然很难下载的了。
   8.  搜索ninja-build，在官网最下有一行 download the Ninja binary ，点击进入ninja的github网页，找到ninja-win的zip文件(https://github.com/ninja-build/ninja/releases)，点击下载。
3. 安装主要程序，基本上就是无脑一直点就可以，无非是换换盘之类的，除了git可能有些复杂，就不再这里赘述了。
4. 安装完软件后，首先是打开msys2.exe，输入命令，之后就是enter，enter，y，enter了。
```
    pacman -S mingw-w64-x86_64-toolchain mingw-w64-x86_64-arm-none-eabi-toolchain mingw-w64-x86_64-ccache  mingw-w64-x86_64-openocd
```
5. 环境变量的添加：
    1. jflash.exe所在的目录
    2. mingw64的bin目录
    3. cmake的bin目录
    4. ninja-build.exe所在的目录(不加也可以)
6. vscode的setting.json文件
```
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
7. 配置git，如果你已经有了github账号，那么打开git bash，输入
```
    git config --global user.name "君の名は"
    git config --global user.email 君のemail
    git config --global http.sslVerify false //如果发现推送时或者拉取时有问题可以试试这行代码
```
8. 在vscode上登陆github这个没难度。
9. 到这里准备工作就基本完成了。(为什么这个markdown报错这么多！！！)
