# 五人团队 Git 协作说明

这份文档是给本项目团队使用的 Git 操作规则。目标很简单：`main` 分支永远保持能编译、能运行；每个人在自己的分支上开发，写完后再合并。

## 1. 核心原则

1. 不直接在 `main` 上写功能代码。
2. 每个功能开一个独立分支。
3. 每次提交只做一件清楚的事情。
4. 推送前至少确认项目能编译。
5. 不提交 `build/`、临时文件、系统文件。

本项目已经在 `.gitignore` 里忽略了 `build/`、`.DS_Store` 等文件。

## 2. 第一次获取项目

每个人第一次参与时，执行：

```bash
git clone https://github.com/ninjaz0/gamecpp.git
cd gamecpp
cmake -S . -B build
cmake --build build
./build/HollowStarter
```

如果项目能启动，说明环境基本没问题。

## 3. 每天开始写代码前

先回到 `main`，拉取最新代码：

```bash
git checkout main
git pull
```

然后开自己的功能分支：

```bash
git checkout -b feature/功能名
```

例如：

```bash
git checkout -b feature/player-dash
git checkout -b feature/physics-platform
git checkout -b feature/player-animation
git checkout -b feature/enemy-fsm
git checkout -b feature/level-spikes
```

## 4. 推荐分支命名

使用这种格式：

```text
feature/简短功能名
fix/简短问题名
docs/简短文档名
```

例子：

```text
feature/player-dash
feature/enemy-patrol
feature/sprite-animation
feature/level-coins
fix/player-stuck-wall
docs/git-guide
```

名字尽量短、清楚、用英文小写和连字符。

## 5. 五个人怎么分文件

为了减少冲突，优先按模块改自己的目录：

| 负责人 | 主要目录 |
| --- | --- |
| 核心框架与游戏循环 | `src/core`、`include/game/core` |
| 物理与碰撞系统 | `src/physics`、`include/game/physics` |
| 动画与渲染引擎 | `src/render`、`include/game/render` |
| 角色逻辑与状态控制 | `src/characters`、`include/game/characters` |
| 关卡与资源管理 | `src/level`、`include/game/level`、`src/assets`、`include/game/assets`、`assets` |

如果你必须改别人的模块，先在群里说明原因。比如角色逻辑的人要改 `PhysicsBody`，就要告诉物理模块负责人。

## 6. 写完代码后提交

先看自己改了什么：

```bash
git status
git diff
```

只添加和本次任务有关的文件。比如你只改了玩家冲刺：

```bash
git add src/characters/Player.cpp include/game/characters/Player.hpp
git commit -m "Add player dash"
```

如果你确定所有改动都属于这次任务，也可以：

```bash
git add .
git commit -m "Add player dash"
```

提交信息建议用英文动词开头：

```text
Add player dash
Fix wall collision
Load level from text file
Document Git workflow
```

## 7. 推送自己的分支

第一次推送这个分支：

```bash
git push -u origin feature/player-dash
```

后面同一个分支继续推送，只需要：

```bash
git push
```

推送后，到 GitHub 上创建 Pull Request，目标分支选择 `main`。

## 8. Pull Request 规则

每个 PR 至少写清楚三件事：

```text
做了什么：
为什么做：
怎么测试：
```

例子：

```text
做了什么：
增加玩家 Dash 状态，按 Left Shift 可以短距离冲刺。

为什么做：
横版动作游戏需要更灵活的移动手感。

怎么测试：
运行 ./build/HollowStarter，确认移动、跳跃、冲刺都正常。
```

合并前建议至少让另一个同学看一眼代码。

## 9. 合并后怎么同步

你的 PR 被合并后，本地回到 `main` 并更新：

```bash
git checkout main
git pull
```

删除已经完成的本地分支：

```bash
git branch -d feature/player-dash
```

如果 GitHub 上远端分支也不需要了，可以在 GitHub 页面删除。

## 10. 开发中同步最新 main

如果你自己的分支开发了很久，别人已经合并了新代码，你可以在自己的分支上同步最新 `main`：

```bash
git fetch origin
git merge origin/main
```

如果没有冲突，Git 会自动完成合并。

如果有冲突，按下一节处理。

## 11. 处理冲突

冲突文件里会出现类似内容：

```text
<<<<<<< HEAD
你的代码
=======
别人的代码
>>>>>>> origin/main
```

你需要手动整理成最终正确版本，删除这些标记。整理完之后：

```bash
git add 冲突文件
git commit
cmake --build build
```

冲突时不要慌，先看冲突文件到底是哪几行。一般小团队只要少改别人的模块，冲突会很少。

## 12. 推送前检查清单

推送前建议执行：

```bash
git status
cmake --build build
```

如果你改了玩法，还要运行：

```bash
./build/HollowStarter
```

确认：

- 当前分支不是 `main`，除非你是维护者并且明确要直接提交文档或小修正。
- `git status` 里没有奇怪的文件。
- 没有把 `build/`、`.DS_Store`、临时截图等文件加入提交。
- 游戏能编译。
- 你提交的信息能看懂。

## 13. 常用命令速查

查看当前分支和改动：

```bash
git status
git branch
```

切到 `main`：

```bash
git checkout main
```

拉取最新代码：

```bash
git pull
```

创建新分支：

```bash
git checkout -b feature/功能名
```

查看具体改动：

```bash
git diff
```

提交：

```bash
git add 文件名
git commit -m "提交说明"
```

推送：

```bash
git push -u origin 当前分支名
```

查看提交历史：

```bash
git log --oneline --decorate -10
```

## 14. 本项目推荐工作方式

你们现在最适合这样协作：

```text
main 保持稳定
每个同学一个 feature 分支
写完后 push
GitHub 上开 Pull Request
至少一个同学检查
确认能编译后合并到 main
```

一句话版本：不要在 `main` 上乱改，一事一分支，合并前能跑。
