<p align="center">
  <img alt="Termux VCC" src="https://img.shields.io/badge/Termux-VCC-1f6feb?style=for-the-badge" width="220">
</p>

<p align="center">第三方命令和一些工具集合</p>

<p align="center">
  <a href="#"><img src="https://img.shields.io/badge/language-C%2B%2B-blue.svg"></a>
  <a href="#"><img src="https://img.shields.io/badge/version-1.4.4-green.svg"></a>
  <a href="#"><img src="https://img.shields.io/badge/license-MIT-orange.svg"></a>
  <a href="#"><img src="https://img.shields.io/badge/platform-Windows-lightgrey.svg"></a>
</p>

---

## 简介

本集合意在为了方便操作 Windows。如果你对命令行脚本、Windows API 感兴趣，本集合很可能会帮到你。

> 想在 Windows 上学习 Linux 上的命令？想深入了解句柄、内存、命令行的本质？想学习 shell 脚本，各个脚本之间的调用？自己写命令？

这是一个通过 shell 脚本调用运行的集合，也就是说，是以 `cmd` 为基础运行的 —— 你不仅可以使用 cmd 自带的命令，还可以使用本集合提供的命令。

## 目录结构

```
.
├─ Termux.bat                       主入口：启动类 Termux 控制台
├─ my_path.cmd                      把 Compile-bin 加入 PATH 的参考脚本
├─ index.html                       项目落地页（GitHub Pages）
├─ .nojekyll                        跳过 Jekyll 构建，让 Pages 直接发布静态文件
└─ Compile/
   ├─ Com.bat                       批量编译脚本
   └─ Compile-bin/
      ├─ *.exe                      编译好的命令（自制，均有对应源码）
      ├─ *.bat / *.cmd              命令脚本
      └─ Sourse Code/               全部源码（C / C++ / Go / Python）
```

## 功能

- `Compile-bin` 目录下是所有的外部命令
- 宏命令在 `Termux.bat` 内定义
- 源码统一放在 `Compile-bin/Sourse Code/`，含 C、C++、Go、Python 四种语言

命令大致分这几类：

| 类别 | 说明 | 代表命令 |
|---|---|---|
| 终端增强 | 彩色输出、光标定位、表格、进度 | `printf` `gotoxy` `colors` `table` `cmatrix` |
| 桌面 / 窗口 | 窗口枚举、置顶、定位、托盘、隐藏 | `LsHwnd` `TopMost` `location` `tray` `hidedesk` |
| 输入控制 | 鼠标、按键 | `GSmouse` `mouses` `movePos` |
| 文件处理 | 复制、切割、统计、查找 | `ls` `files` `copyw` `cut` `fow` `length` |
| 网络 | HTTP / TCP / UDP 客户端 | `http` `tcp` `udp` `kping` |
| 加解密 | AES / RSA 工具 | `aes` `BatchEncryption` |
| 图像 | 位图与图片处理 | `bmp` `mdpi` `screen` |

## 编译

源码在 `Compile-bin/Sourse Code/`，进入 `Compile/` 后运行：

```bat
Com.bat
```

批量编译脚本会依次调用各语言工具链（TCC / g++ / Go / Python），生成的 exe 输出到 `Compile-bin/`。

## 下载

- [下载最新发行版](https://gitee.com/cctv3058084277/main/releases/download/TERMUX-VCC/TERMUX-VCC-1.4.4.exe)

> 提示：杀毒软件可能报毒属于正常现象 —— 外部命令需要调用系统 API 才能做更多事情。安装过程中包含注册表操作，因此需要管理员权限。

## 使用方法

本集合专为小白设计，默认情况下你不需要修改任何参数，直接运行发行版 exe 安装即可，集合会自动完成添加。

集合不依赖任何运行库，只需安装一次 **TERMUX-VCC** 至目标电脑即可运行。如果版本有更新，初始页面会提示升级，运行 `install -d` 即可。

安装完成后：

- 右键桌面或资源管理器 → 点击 **Termux**，即可在该路径下打开控制台
- 控制台内可直接使用 `cat` `cp` `mv` `ls` `kill` `clear` 等 Linux 风格命令

## 许可

MIT
