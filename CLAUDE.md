# CLAUDE.md

This file provides guidance to Claude Code (claude.ai/code) when working with code in this repository.

## 项目概述

基于 SFML 2.6.2 的 C++ 黑白棋（Reversi）桌面游戏。棋盘为 6×6，支持人机对战和双人对战，包含 AI（难度可选）、音效、背景音乐、棋盘颜色/亮度调节等功能。

## 构建与运行

### 编译

使用 Visual Studio Build Tools 18（v143 工具集）+ MSBuild：

```bash
MSBUILD_EXE="C:/Program Files (x86)/Microsoft Visual Studio/18/BuildTools/MSBuild/Current/Bin/MSBuild.exe"
"$MSBUILD_EXE" "f:/项目/c++design/c++design.vcxproj" -p:Configuration=Debug -p:Platform=x64 -t:Build
```

项目使用 SFML 静态链接（`SFML_STATIC` 已定义），但音频库（`sfml-audio-d.lib`）为动态链接。

### 运行前准备

编译后需将以下运行时依赖复制到 `x64\Debug\` 目录：

1. **SFML DLL**：`C:\SFML\SFML-2.6.2\bin\*.dll`（11 个文件）
2. **资源文件**：项目根目录下的 `open.png`、`background.jpg`、`backguandmusic.MP3`、`chess.wav`、`click.wav`、`rules.txt`

程序使用相对路径加载资源，必须从 exe 所在目录启动，或确保资源文件在 exe 同目录下。

### 一键启动

```bash
cmd /c start "" /d "f:\项目\c++design\x64\Debug" "f:\项目\c++design\x64\Debug\c++design.exe"
```

## 代码架构

### 状态机模式（核心设计）

游戏使用经典的**状态模式**管理界面流转：

```
OpenState → MenuState → PlayingState → GameOverState
                              ↑              │
                              └──────────────┘
```

- [State.h](State.h) — 抽象基类，定义 `handleEvent()`、`update()`、`draw()` 三个纯虚函数
- [OpenState.h](OpenState.h)/[OpenState.cpp](OpenState.cpp) — 开屏画面，滚动显示古文，点击后进入菜单
- [MenuState.h](MenuState.h)/[MenuState.cpp](MenuState.cpp) — 主菜单，选择游戏模式/AI难度/棋盘颜色/亮度，查看规则
- [PlayingState.h](PlayingState.h)/[PlayingState.cpp](PlayingState.cpp) — 核心玩法，处理落子、翻转、AI 回合、悔棋
- [GameOverState.h](GameOverState.h)/[GameOverState.cpp](GameOverState.cpp) — 结算界面，显示比分和胜者

`Reversi` 类持有 `State* currentState`，每帧委托给当前状态处理。状态切换通过 `Reversi::setState()` 完成（旧状态在此处 delete）。

### 游戏核心

- [Reversi.h](Reversi.h)/[Reversi.cpp](Reversi.cpp) — 游戏主类，管理棋盘（`vector<vector<Piece>>`）、回合切换、落子合法性检查、翻转棋子、悔棋栈（`stack<MoveRecord>`）、计分、AI 调用
- [AIPlayer.h](AIPlayer.h)/[AIPlayer.cpp](AIPlayer.cpp) — AI 实现，三个难度级别（Easy/Medium/Hard），不同深度和评估策略，使用 `<chrono>` 计时
- [Enums.h](Enums.h) — 枚举定义：`GameState`（Opening/Menu/Playing/GameOver）、`GameMode`（SinglePlayer/TwoPlayer）、`AIDifficulty`（Easy/Medium/Hard）、`Piece`（Empty/Black/White）
- [Constants.h](Constants.h) — 窗口尺寸（1600×900）、棋盘配置、颜色预设、亮度参数

### 辅助组件

- [Picture.h](Picture.h)/[Picture.cpp](Picture.cpp) — Windows 原生文件选择对话框封装（`GetOpenFileNameA`），用于更换背景图片

## 依赖

| 依赖 | 版本/路径 | 说明 |
|------|-----------|------|
| SFML | C:\SFML\SFML-2.6.2 | 图形、音频、窗口 |
| MSVC 工具集 | v143（VS Build Tools 18） | `C:\Program Files (x86)\Microsoft Visual Studio\18\BuildTools\` |
| 字体 | C:\Windows\Fonts\simsun.ttc | 宋体，中文渲染 |

## 注意事项

1. **源文件编码**：原始部分文件为 GBK 编码，已转换为 UTF-8。新增或修改文件时请保持 UTF-8。
2. **资源路径**：所有资源使用相对路径加载，确保 exe 目录下有完整的资源文件。
3. **音频库**：`sfml-audio-d.lib` 是项目中唯一的动态链接 SFML 库，其余均为静态链接。如改为静态链接 `sfml-audio-s-d.lib`，需额外链接 `flac.lib`、`vorbisfile.lib`、`vorbisenc.lib`、`vorbis.lib`、`ogg.lib`、`openal32.lib`。
4. **音频文件名**：背景音乐文件名为 `backguandmusic.MP3`（拼写特殊，非 background），不要重命名。
5. **编译器兼容性**：原项目为 VS2022（v143 工具集），`std::wifstream` 构造函数在 GCC 15 下不兼容 `std::wstring` 参数，需使用 `.c_str()`。
