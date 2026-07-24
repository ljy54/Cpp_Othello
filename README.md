# 黑白棋（Reversi）桌面游戏

基于 C++ 和 SFML 2.6.2 图形库开发的 6×6 黑白棋桌面游戏，支持人机对战与双人对战。面向对象程序设计课程设计项目。

## 功能特性

- **两种游戏模式**：人机对战（单人对 AI）、双人本地对战
- **AI 玩家**：支持简单 / 中等 / 困难三种难度
  - 简单：随机落子
  - 中等：70% 概率采用最佳策略，30% 随机
  - 困难：完全基于评估函数决策（位置权重 + 翻转数 + 对手行动力）
- **悔棋功能**：基于栈记录每次落子的翻转列表，支持完整撤销
- **交互功能**：音效反馈、循环背景音乐、棋盘颜色/亮度调节、自定义背景图片
- **状态机架构**：OpenState → MenuState → PlayingState → GameOverState 四状态清晰流转

## 技术栈

| 类别 | 技术 |
|------|------|
| 语言 | C++ |
| 图形/音频 | SFML 2.6.2 |
| 构建工具 | Visual Studio 2022（v143 工具集） |
| 设计模式 | 状态模式（State Pattern） |

## 代码架构

```
main.cpp              # 程序入口，事件循环与音乐控制
Reversi.h/cpp         # 游戏主类：棋盘管理、落子检测、翻转、悔棋、AI 调度
AIPlayer.h/cpp        # AI 玩家：三档难度、位置权重评估、最佳移动决策
State.h               # 状态抽象基类（handleEvent / update / draw）
OpenState.h/cpp       # 开屏画面
MenuState.h/cpp       # 主菜单（模式/难度/颜色/亮度/规则）
PlayingState.h/cpp    # 核心玩法界面（棋盘交互、投降、悔棋按钮）
GameOverState.h/cpp   # 结算界面
Enums.h               # 枚举定义（GameState / GameMode / AIDifficulty / Piece）
Constants.h           # 窗口尺寸、棋盘配置、颜色预设
Picture.h/cpp         # Windows 原生文件对话框封装
```

## 构建与运行

### 环境要求

- Visual Studio Build Tools 18（v143 工具集）
- [SFML 2.6.2](https://www.sfml-dev.org/)（安装到 `C:\SFML\SFML-2.6.2\`）
- Windows 字体：宋体（`C:\Windows\Fonts\simsun.ttc`）

### 编译

```bash
MSBUILD_EXE="C:/Program Files (x86)/Microsoft Visual Studio/18/BuildTools/MSBuild/Current/Bin/MSBuild.exe"
"$MSBUILD_EXE" "c++design.vcxproj" -p:Configuration=Debug -p:Platform=x64 -t:Build
```

### 运行准备

编译后将以下文件复制到 `x64\Debug\` 目录：
- SFML DLL（`C:\SFML\SFML-2.6.2\bin\*.dll`，共 11 个文件）
- 资源文件：`open.png`、`background.jpg`、`backguandmusic.MP3`、`chess.wav`、`click.wav`、`rules.txt`

### 一键启动

```bash
cmd /c start "" /d "x64\Debug" "x64\Debug\c++design.exe"
```

## 操作说明

| 操作 | 方式 |
|------|------|
| 落子 | 鼠标点击棋盘有效位置 |
| 投降 | 点击右下角红色"投降"按钮 |
| 悔棋 | 点击蓝色"悔棋"按钮（人机模式撤销两步，双人模式撤销一步） |
| 音量调节 | 键盘 `+` / `-` 键 |
| 菜单设置 | 游戏模式 / AI 难度 / 棋盘颜色 / 亮度 / 更换背景 |

## 许可证

本项目为课程设计项目，仅供学习参考。
