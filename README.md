# -meme-：可爱的猫Meme桌面宠物

一个使用 Qt 构建的桌面宠物应用程序，猫Meme为您的桌面增添乐趣。

## 🧩 项目简介

**-meme-** 是一个轻量级的桌面宠物应用，几十种猫Meme等待你探索，旨在为用户带来愉快的桌面体验。

## 📦 功能特性

- 几十种猫Meme桌面宠物
- 鼠标点击触发声音/动作
- 声控唤醒功能（检测麦克风音量）
- 自定义右键菜单（切换表情、退出程序）
- 模拟跳跃、弹幕吐槽、自动动作等效果
- 跑酷小游戏（点击跳跃，躲避障碍）

## 🛠️ 安装与运行

### 前置条件

- Qt 5 或更高版本（推荐 Qt 6）
- C++ 编译器（支持 C++17 以上）
- CMake（可选，若使用 CMake 构建）

### 构建步骤（使用 Qt Creator）

1. 克隆仓库：
   ```
   bash
   git clone https://github.com/yuanzhengze/-meme-.git
   cd -meme-
   ```
2.	使用 Qt Creator 打开 .pro 或 CMakeLists.txt 文件
3.	构建并运行项目
   
### 构建步骤（使用命令行）
```
mkdir build && cd build
cmake ..
make
./DesktopPet
```

### 📁项目结构
```
- images/        # 存放 Meme 动图与表情资源
- sounds/        # 存放点击/唤醒等音效资源
- main.cpp       # 程序入口
- mainwindow.*   # 主窗口逻辑（桌宠控制）
- gamewindow.*   # 跑酷游戏窗口
- resources.qrc  # Qt 资源文件
- .gitignore     # 忽略编译输出与中间文件
```
### 🎮 特色功能示例
	•	左键点击猫猫 → 播放喵叫 + 弹跳 + 弹幕吐槽
	•	声控唤醒 → 嘶吼触发猫猫跳起来回应
	•	右键菜单 → 切换 Meme 表情 / 退出程序
	•	桌面跑酷小游戏 → 躲避狗头障碍，实时加分
	•	多只桌宠共存 → 每只都有独立动画与行为

### 🤝 贡献指南
欢迎你为项目贡献创意和代码！你可以：
	•	提交 Issue 报告 Bug 或建议功能
	•	Fork 并提交 Pull Request
	•	添加新表情包、新功能或新玩法
 ___
 如果你喜欢这个项目，欢迎点个 ⭐ Star 鼓励一下作者！
 ___
