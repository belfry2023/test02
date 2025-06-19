# STM32开发环境搭建指南（CMake + VSCode + Ozone）

## 目录
1. [开发环境概述](#一开发环境概述)
2. [软件安装清单](#二软件安装清单)
3. [Windows环境配置](#三windows环境配置)
4. [Linux环境配置](#四linux环境配置)
5. [工程创建与编译（通用）](#五工程创建与编译通用)
6. [程序烧录与调试（通用）](#六程序烧录与调试通用)
7. [版本控制与协作（通用）](#七版本控制与协作通用)
8. [常见问题解决](#八常见问题解决)
9. [最佳实践建议](#九最佳实践建议)

---

## 一、开发环境概述 <a id="一开发环境概述"></a>
采用现代化工具链组合：
- **核心工具**：STM32CubeMX（配置）、VSCode（编码）、CMake+Ninja（构建）、J-Link+Ozone（烧录调试）
- **支持功能**：
  ✅ 多芯片支持（F1/F4/H7等系列）
  ✅ 双平台兼容（Windows/Linux）
  ✅ 版本控制集成（Git）
  ✅ 高级调试功能（实时监控/性能分析）
- **工作流程**：
  `CubeMX初始化 → VSCode编码 → CMake编译 → J-Link烧录 → Ozone调试`

---

## 二、软件安装清单 <a id="二软件安装清单"></a>
| 软件名称       | Windows安装源                     | Linux安装命令                      |
|----------------|----------------------------------|-----------------------------------|
| VSCode         | [官网下载](https://code.visualstudio.com) | `sudo apt install code`           |
| STM32CubeMX    | [ST官网](https://www.st.com/stm32cubemx) | [手动安装](#四linux环境配置)      |
| J-Link & Ozone | [SEGGER官网](https://www.segger.com)   | `sudo dpkg -i JLink_*.deb`        |
| Git            | [官网](https://git-scm.com)         | `sudo apt install git`            |
| MSYS2          | [官网](https://www.msys2.org)         | -                                 |
| CMake          | [GitHub](https://github.com/Kitware/CMake) | `sudo apt install cmake`         |
| Ninja          | [GitHub](https://github.com/ninja-build/ninja) | `sudo apt install ninja-build` |

> **网络加速**：使用 Steam++（Windows）或 Watt（Linux）加速GitHub访问

---

## 三、Windows环境配置 <a id="三windows环境配置"></a>
### 1. 安装MSYS2工具链
```bash
# 更新系统后安装工具链
pacman -Syu
pacman -S mingw-w64-x86_64-toolchain \
          mingw-w64-x86_64-arm-none-eabi-toolchain \
          mingw-w64-x86_64-ccache \
          mingw-w64-x86_64-openocd
```

### 2. 环境变量配置（PATH添加）
```plaintext
# 示例路径（根据实际安装位置修改）
C:\Program Files (x86)\SEGGER\JLink   # J-Link
D:\msys64\mingw64\bin                 # MSYS2
C:\Program Files\CMake\bin            # CMake
D:\Tools\ninja                        # Ninja
```

### 3. VSCode配置（`settings.json`）
```json
{
  "terminal.integrated.profiles.windows": {
    "msys2-mingw64": {
      "path": "cmd.exe",
      "args": ["/c", "D:\\msys64\\msys2_shell.cmd -defterm -mingw64 -no-start -here"]
    }
  },
  "cmake.configureOnOpen": true,
  "cmake.buildDirectory": "${workspaceFolder}/build",
  "cmake.generator": "Ninja",
  "cmake.cmakePath": "C:\\Program Files\\CMake\\bin\\cmake.exe",
  "cmake.ninjaPath": "D:\\Tools\\ninja.exe",
  "C_Cpp.default.configurationProvider": "ms-vscode.cmake-tools"
}
```

### 4. 安装VSCode扩展
```bash
code --install-extension ms-vscode.cpptools
code --install-extension twxs.cmake
code --install-extension ms-vscode.cmake-tools
```

---

## 四、Linux环境配置 <a id="四linux环境配置"></a>
### 1. 基础工具安装
```bash
# 安装编译工具链和依赖库
sudo apt update
sudo apt install -y build-essential cmake ninja-build git \
                   gcc-arm-none-eabi binutils-arm-none-eabi \
                   libusb-1.0-0-dev unzip default-jre \
                   libxcb-xinerama0 libxkbcommon-x11-0
```

### 2. STM32CubeMX安装
```bash
# 1. 下载安装包（从ST官网获取最新链接）
wget https://www.st.com/content/ccc/resource/technical/software/sw_development_suite/group0/.../en.stm32cubemx-lin.zip

# 2. 解压并安装
unzip en.stm32cubemx-lin.zip
chmod +x SetupSTM32CubeMX-*.linux
sudo ./SetupSTM32CubeMX-*.linux

# 3. 创建快捷方式
sudo cp ~/STM32CubeMX/STM32CubeMX.desktop /usr/share/applications/

# 4. 添加环境变量
echo 'export PATH=$PATH:~/STM32CubeMX' >> ~/.bashrc
source ~/.bashrc

# 5. 验证安装
STM32CubeMX
```

### 3. 安装J-Link
```bash
# 下载最新版（替换V780c为实际版本）
wget https://www.segger.com/downloads/jlink/JLink_Linux_V780c_x86_64.deb
sudo dpkg -i JLink_Linux_*.deb
```

### 4. USB设备权限配置
```bash
# 添加用户到设备组
sudo usermod -a -G plugdev $USER

# 创建J-Link设备规则
echo 'SUBSYSTEM=="usb", ATTR{idVendor}=="1366", MODE="0666"' | sudo tee /etc/udev/rules.d/99-jlink.rules

# 应用规则
sudo udevadm control --reload-rules
sudo udevadm trigger

# 验证设备识别
lsusb | grep "SEGGER"
```

### 5. VSCode配置（`settings.json`）
```json
{
  "cmake.cmakePath": "/usr/bin/cmake",
  "cmake.ninjaPath": "/usr/bin/ninja",
  "cmake.configureOnOpen": true,
  "cmake.buildDirectory": "${workspaceFolder}/build",
  "cmake.generator": "Ninja",
  "C_Cpp.default.configurationProvider": "ms-vscode.cmake-tools"
}
```

---

## 五、工程创建与编译（通用） <a id="五工程创建与编译通用"></a>
### 1. STM32CubeMX配置流程
1. 启动CubeMX并登录ST账号
2. 安装所需芯片支持包（F1/F4/H7等）
3. 创建新工程 → 选择MCU型号
4. 关键配置：
   - SYS → Debug: Serial Wire
   - RCC → 启用外部晶振
   - Clock Configuration → 配置时钟树
5. Project Manager → Toolchain: **CMake** → Generate Code

### 2. 编译工程
```bash
# 在工程根目录执行
mkdir build && cd build
cmake .. -G Ninja  # 生成构建系统
ninja               # 编译项目
```
> 成功标志：生成`build/project_name.elf`和`project_name.hex`文件

### 3. HEX文件生成（修改CMakeLists.txt）
```cmake
# 在add_executable()后添加
set(HEX_FILE ${PROJECT_NAME}.hex)
add_custom_command(
  TARGET ${PROJECT_NAME} POST_BUILD
  COMMAND ${CMAKE_OBJCOPY} -O ihex $<TARGET_FILE:${PROJECT_NAME}> ${HEX_FILE}
  COMMENT "Generating HEX file: ${HEX_FILE}"
)
```

---

## 六、程序烧录与调试（通用） <a id="六程序烧录与调试通用"></a>
### 1. J-Link烧录方法
```bash
# 手动烧录
JFlash -openprj stm32.jflash -open project.hex -auto -startapp -exit

# CMake集成烧录（添加至CMakeLists.txt）
set(JFLASH "JFlash.exe" WIN32 ELSE "JFlashExe")
add_custom_target(flash
  COMMAND ${JFLASH} -openprj ${CMAKE_SOURCE_DIR}/stm32.jflash 
          -open ${HEX_FILE},0x08000000 -auto -startapp -exit
  DEPENDS ${PROJECT_NAME}
  COMMENT "Programming device with J-Link..."
)
```

### 2. Ozone调试技巧
1. **基础操作**：
   - 创建新工程 → 选择设备型号 → 指定ELF文件
   - 连接J-Link → 加载程序 → 启动调试
2. **核心功能**：
   - 断点管理（源码/汇编级）
   - 实时变量监控与修改
   - 外设寄存器查看（支持位操作）
   - 数据跟踪（Timeline）
3. **高级调试**：
   ```c
   // 代码中插入调试断点
   #define DEBUG_BREAK() asm volatile ("bkpt #0")
   void critical_section() {
     DEBUG_BREAK();  // 程序将在此暂停
     // ...关键代码...
   }
   ```

---

## 七、版本控制与协作（通用） <a id="七版本控制与协作通用"></a>
### 1. Git工作流
```bash
# 初始化仓库
git init
git add .
git commit -m "初始提交：STM32基础工程"

# 推送到远程仓库
git remote add origin https://github.com/yourname/project.git
git push -u origin main
```

### 2. VSCode集成操作
- **提交更改**：`Ctrl+Shift+G` → 暂存修改 → 填写提交信息
- **分支管理**：左下角分支图标 → 创建/切换分支
- **冲突解决**：差异对比视图 → 手动合并变更

### 3. `.gitignore`模板
```gitignore
# 编译输出
/build/
*.elf
*.hex
*.bin
*.map

# CubeMX生成文件
/MX/
*.loc

# IDE配置文件
.vscode/
.idea/

# J-Link工程文件
*.jflash
```

---

## 八、常见问题解决 <a id="八常见问题解决"></a>
### 1. 编译问题
**症状**：`ld returned 1 exit status`  
**解决方案**：
```diff
/* 修改链接脚本 STM32FXXX_FLASH.ld */
.init_array : 
-   READONLY  /* 删除该关键字（GCC 11+不兼容） */
```

### 2. 环境问题
**症状**：Linux下CubeMX启动失败  
**修复**：
```bash
# 安装缺失库
sudo apt install -y libxcb-xinerama0 libxkbcommon-x11-0

# 修复权限
sudo chmod 755 /usr/local/STMicroelectronics/STM32Cube/STM32CubeMX
```

### 3. 烧录问题
**症状**：`J-Link: No device found`  
**排查步骤**：
1. 检查USB连接和驱动：`lsusb | grep SEGGER`
2. 验证用户组：`groups | grep plugdev`
3. 重载udev规则：
   ```bash
   sudo udevadm control --reload-rules
   sudo udevadm trigger
   ```

---

## 九、最佳实践建议 <a id="九最佳实践建议"></a>
### 1. 标准化目录结构
```markdown
project/
├── CMakeLists.txt        # 主构建脚本
├── Core/
│   ├── Src/              # 用户源码
│   └── Inc/              # 头文件
├── Drivers/              # HAL库
├── STM32CubeMX/          # CubeMX配置
├── build/                # 构建目录
├── scripts/              # 自动化脚本
└── stm32.jflash          # J-Link工程模板
```

### 2. 自动化构建脚本
`scripts/build_flash.sh`:
```bash
#!/bin/bash

# 清理并重新构建
rm -rf build
mkdir build && cd build
cmake .. -G Ninja

# 编译并烧录
if ninja; then
  echo "Build successful! Programming device..."
  JFlash -openprj ../stm32.jflash -open app.hex -auto -startapp -exit
else
  echo "Build failed!"
  exit 1
fi
```

### 3. 参考资源
- **示例工程**：[STM32 CMake模板](https://github.com/belfry2023/Electromagnetic_Field-Waves.git)
- **官方文档**：
  - [CubeMX问题排查](https://www.st.com)
  - [J-Link文档中心](https://www.segger.com/support/)
  - [CMake官方教程](https://cmake.org/documentation/)

> 本指南全面覆盖Windows/Linux双平台开发环境搭建，集成最佳实践和问题解决方案，大幅提升STM32开发效率。