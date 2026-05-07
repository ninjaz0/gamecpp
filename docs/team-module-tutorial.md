# HollowStarter 五人分工开发教程

这份教程是给 C++ 初学团队用的。你们不需要一开始就理解所有代码，每个人先吃透自己负责的模块，再通过接口和其他人协作。

建议所有人先完成“共同基础”，然后阅读自己负责模块的教程。

## 0. 共同基础

### 先把项目跑起来

在项目根目录执行：

```bash
cmake -S . -B build
cmake --build build
./build/HollowStarter
```

如果第一次很慢，是因为 CMake 会自动下载 raylib。

### 项目结构

```text
include/game/       头文件：类和函数的声明，告诉别人怎么调用你的模块
src/                源文件：具体实现
assets/             关卡、图片、音频
docs/               团队教程和设计文档
CMakeLists.txt      构建配置
```

### 你们需要先理解的 C++ 概念

- `class`：把数据和函数放在一起，例如 `Player`。
- `struct`：通常用来保存简单数据，例如 `InputState`、`PhysicsBody`。
- `#include`：使用别的文件里声明的类型或函数。
- `namespace game`：避免名字冲突，本项目代码都放在 `game` 命名空间。
- `const`：表示函数不会修改对象，或者参数不会被修改。
- `std::vector`：动态数组，用来保存碰撞块、敌人列表等。
- `std::unique_ptr`：独占指针，用来管理玩家对象生命周期。

### 团队协作规则

1. 改自己的模块时，尽量只改自己负责目录。
2. 如果必须改别人的接口，先和对方说清楚。
3. 每次改完都执行：

```bash
cmake --build build
./build/HollowStarter
```

4. 不要把 `build/` 提交到仓库，它已经在 `.gitignore` 里。
5. 新资源放进 `assets/`，不要写死绝对路径。

---

## 1. 核心框架与游戏循环

负责人主要看这些文件：

- `include/game/core/Game.hpp`
- `src/core/Game.cpp`
- `include/game/core/Input.hpp`
- `src/core/Input.cpp`
- `src/main.cpp`

### 这个模块负责什么

你负责让游戏“活起来”：

- 创建窗口。
- 每一帧读取输入。
- 每一帧更新游戏逻辑。
- 每一帧绘制画面。
- 管理 `delta time`，保证不同电脑上移动速度一致。
- 控制摄像机跟随玩家。

### 主循环怎么读

入口在 `src/main.cpp`：

```cpp
game::Game game;
game.run();
```

真正循环在 `Game::run()`：

```cpp
while (!WindowShouldClose()) {
    const float dt = std::min(GetFrameTime(), 1.0f / 20.0f);
    input_.update();
    update(dt);
    draw();
}
```

这里的 `dt` 是上一帧到这一帧经过的秒数。比如 60 FPS 时，`dt` 大约是 `0.016`。

角色移动不要写成：

```cpp
position.x += 5;
```

应该写成：

```cpp
position.x += speed * dt;
```

这样电脑快慢不同，角色每秒移动距离仍然一致。

### 你可以先做的练习

1. 在窗口标题里显示 FPS。
2. 给 `InputState` 增加 `pausePressed`，按 `Esc` 暂停游戏。
3. 给摄像机增加边界限制，不让镜头看到地图外面。
4. 增加一个简单菜单状态：`Menu`、`Playing`、`Paused`。

### 常见坑

- 不要在 `draw()` 里改游戏数据，绘制函数只负责画。
- 不要在每一帧重复加载资源，加载资源应该交给资源管理模块。
- 不要忘记给新输入加“按下瞬间”和“持续按住”的区别。raylib 里 `IsKeyPressed` 是按下瞬间，`IsKeyDown` 是持续按住。

---

## 2. 物理与碰撞系统

负责人主要看这些文件：

- `include/game/physics/AABB.hpp`
- `include/game/physics/PhysicsBody.hpp`
- `include/game/physics/PhysicsSystem.hpp`
- `src/physics/PhysicsSystem.cpp`
- `src/level/Level.cpp`

### 这个模块负责什么

你负责让角色有“身体感”：

- 重力。
- 跳跃。
- 下落。
- 落地。
- 撞墙。
- 不穿过地板和平台。

### 当前实现思路

本项目现在用最基础的 AABB 碰撞。

AABB 就是矩形碰撞盒：

```cpp
Rectangle{x, y, width, height}
```

`PhysicsSystem::step()` 每帧做三件事：

1. 给物体加重力。
2. 先移动 X 轴，检查左右碰撞。
3. 再移动 Y 轴，检查上下碰撞。

分开处理 X 和 Y 的好处是简单稳定。比如角色斜着撞墙时，可以只修正 X，不影响 Y 的下落。

### 你可以先做的练习

1. 增加“单向平台”：角色从下面可以跳上去，从上面会站住。
2. 增加摩擦力：松开方向键时，角色不是立刻停下，而是慢慢减速。
3. 增加终端速度：当前已有 `maxFallSpeed`，试着调不同数值感受手感。
4. 增加调试信息：显示玩家速度 `velocity.x`、`velocity.y`。
5. 增加攻击碰撞盒检测，让玩家攻击可以打到敌人。

### 常见坑

- 不要直接把玩家位置改到墙里面。
- 碰撞修正后要把对应方向速度清零，例如落地后 `velocity.y = 0`。
- 初期不要做太复杂的物理引擎，横版动作游戏更常用“手感优先”的运动学逻辑。

---

## 3. 动画与渲染引擎

负责人主要看这些文件：

- `include/game/render/Animation.hpp`
- `src/render/Animation.cpp`
- `src/characters/Player.cpp`
- `src/characters/Enemy.cpp`
- `src/level/Level.cpp`
- `src/assets/AssetManager.cpp`

### 这个模块负责什么

你负责把游戏画出来：

- 绘制角色、敌人、地图。
- 加载图片。
- 从 Sprite Sheet 中切出每一帧。
- 根据状态播放不同动画。
- 后续可以研究 shader，做发光、残影、受击闪白。

### Sprite Sheet 是什么

Sprite Sheet 是把很多动画帧放在一张图片里。例如：

```text
idle_0 idle_1 idle_2 idle_3
run_0  run_1  run_2  run_3
jump_0 fall_0 atk_0 atk_1
```

`AnimationPlayer` 里已经有这些能力：

- `setSpriteSheet()`：设置整张图和单帧大小。
- `addClip()`：定义一个动画片段。
- `play()`：切换动画。
- `update()`：根据时间推进帧。
- `draw()`：绘制当前帧。

### 接入玩家动画的大致步骤

1. 把图片放进 `assets/sprites/player.png`。
2. 在 `Game` 初始化时通过 `AssetManager` 加载贴图。
3. 给 `Player` 添加一个 `AnimationPlayer` 成员。
4. 根据 `PlayerState` 调用 `animation_.play("idle")`、`animation_.play("run")` 等。
5. 在 `Player::draw()` 里用 `animation_.draw()` 替换当前色块绘制。

### 你可以先做的练习

1. 给玩家接入一张简单 Sprite Sheet。
2. 给敌人接入巡逻动画。
3. 给攻击状态加一个黄色挥砍特效。
4. 给受伤状态加闪烁效果。
5. 做一个背景远景层，让摄像机移动时背景移动更慢，形成视差。

### 常见坑

- 图片不要每帧加载，必须只加载一次。
- Sprite Sheet 的单帧宽高要和代码里一致。
- 翻转角色时，注意不要让绘制位置突然偏移。
- 初学阶段可以先用色块和线框验证逻辑，再换正式美术。

---

## 4. 角色逻辑与状态控制

负责人主要看这些文件：

- `include/game/characters/Player.hpp`
- `src/characters/Player.cpp`
- `include/game/characters/Enemy.hpp`
- `src/characters/Enemy.cpp`
- `include/game/core/Input.hpp`

### 这个模块负责什么

你负责角色“什么时候做什么”：

- 玩家站立、奔跑、跳跃、下落、攻击、受伤。
- 敌人巡逻、追击、攻击、死亡。
- 状态之间如何切换。
- 输入如何转化为动作。

### 为什么要用 FSM

FSM 是有限状态机。意思是角色在任意时刻只处于一种主要状态，比如：

```text
Idle -> Run -> Jump -> Fall -> Idle
Idle -> Attack -> Idle
Run  -> Attack -> Idle
Any  -> Hurt -> Idle
```

这样比写一堆 `if-else` 更清楚。

当前玩家状态在 `PlayerState`：

```cpp
enum class PlayerState {
    Idle,
    Run,
    Jump,
    Fall,
    Attack,
    Hurt
};
```

切换逻辑在 `Player::chooseState()`。

### 你可以先做的练习

1. 增加 `Dash` 状态，按 `Left Shift` 冲刺。
2. 增加攻击冷却，防止连续无限攻击。
3. 增加受伤无敌时间，刚受伤后短时间不再受伤。
4. 给敌人也做 FSM：`Patrol`、`Chase`、`Attack`。
5. 增加生命值，玩家和敌人都可以死亡。

### 常见坑

- 不要让两个状态同时控制速度。例如 `Attack` 和 `Run` 同时抢 `velocity.x` 会很乱。
- 状态切换要有优先级。通常 `Hurt` 优先级最高，其次 `Attack`，再是空中状态，最后是地面移动。
- 动画状态和逻辑状态可以相关，但不要完全混在一起。逻辑先对，动画再跟随逻辑。

---

## 5. 关卡与资源管理

负责人主要看这些文件：

- `include/game/level/Level.hpp`
- `src/level/Level.cpp`
- `include/game/assets/AssetManager.hpp`
- `src/assets/AssetManager.cpp`
- `assets/levels/demo.txt`
- `assets/README.md`

### 这个模块负责什么

你负责“游戏世界从哪里来”：

- 加载地图。
- 解析出生点。
- 生成碰撞块。
- 统一加载图片和音频。
- 避免重复加载资源。
- 保证资源在退出时释放。

### 当前关卡格式

`assets/levels/demo.txt` 是文本地图：

```text
#  表示实心墙体
P  表示玩家出生点
E  表示敌人出生点
.  表示空气
```

`Level::parseLines()` 会把 `#` 转换成 `Rectangle` 碰撞块，把 `P`、`E` 转换成出生点。

### AssetManager 的意义

如果很多地方都写：

```cpp
LoadTexture("assets/sprites/player.png");
```

就可能重复加载同一张图，浪费内存，也容易忘记释放。

所以统一使用：

```cpp
assets.loadTexture("player", "assets/sprites/player.png");
assets.getTexture("player");
```

资源管理模块负责确保同一个 id 只加载一次，退出时统一释放。

### 你可以先做的练习

1. 给文本地图增加新字符：`^` 表示尖刺。
2. 给文本地图增加 `C` 表示金币。
3. 把多个相邻的 `#` 合并成更大的矩形，减少碰撞检测次数。
4. 支持加载第二关，例如 `assets/levels/level2.txt`。
5. 给 `AssetManager` 增加音频加载：`Sound` 和 `Music`。

### 常见坑

- 不要在关卡解析里写玩家逻辑，关卡只负责提供数据。
- 不要用绝对路径加载资源。
- C++ 里资源释放很重要。贴图要 `UnloadTexture`，音效要 `UnloadSound`。
- 如果之后接入 JSON 地图，优先使用成熟 JSON 库，不要自己手写复杂解析器。

---

## 6. 模块之间怎么协作

### Core 调用所有模块

`Game` 是调度中心：

```text
Input -> Player/Enemy -> Physics -> Level -> Render
```

Core 负责人不要把所有逻辑都写进 `Game.cpp`，它应该负责调度，而不是吞掉别人的工作。

### Physics 不关心谁在移动

物理系统只处理 `PhysicsBody` 和 `Rectangle`，它不应该知道这是玩家还是敌人。

### Character 不直接解析地图

玩家和敌人可以读取 `Level` 提供的碰撞块，但不应该自己打开地图文件。

### Render 跟随状态

渲染模块可以读取玩家状态来决定播什么动画，但不要让动画代码决定玩家能不能跳。

### AssetManager 是公共服务

贴图、音频都从这里拿。不要在每个角色里重复加载资源。

---

## 7. 一周开发计划

### 第 1 天：全员熟悉项目

- 每个人跑通构建。
- 每个人读自己模块的 `.hpp` 和 `.cpp`。
- 每个人改一个小参数并重新编译，比如移动速度、重力、颜色、地图。

### 第 2 天：玩家基础手感

- Core：加暂停。
- Physics：调重力和跳跃曲线。
- Render：准备玩家 Sprite Sheet。
- Character：增加 Dash 状态。
- Level/Asset：增加尖刺 tile。

### 第 3 天：敌人和攻击

- Physics：实现攻击碰撞盒检测所需工具函数。
- Character：敌人生命值、受击、死亡。
- Render：攻击特效。
- Core：调试显示生命值和状态。
- Level/Asset：地图里支持多个敌人。

### 第 4 天：关卡体验

- Level/Asset：做第二关。
- Physics：处理平台和边缘情况。
- Character：增加玩家死亡和重生。
- Render：背景和简单 UI。
- Core：关卡切换。

### 第 5 天：打磨和展示

- 修 bug。
- 加开始界面。
- 加音效。
- 准备演示关卡。
- 每个人写一段自己模块的说明，方便答辩。

---

## 8. 初学者调试方法

### 编译错误

先看第一条 error，不要被后面几十条吓到。C++ 经常一个分号错误引发很多后续报错。

### 游戏逻辑不对

优先打开调试碰撞框：

```text
F1
```

然后观察：

- 碰撞框是否对齐角色。
- 玩家是否真的落在地面上。
- 速度方向是否正确。
- 状态显示是否符合预期。

### 打印调试

可以临时使用 raylib 的日志：

```cpp
TraceLog(LOG_INFO, "velocity y = %f", body_.velocity.y);
```

调完记得删掉或减少输出。

---

## 9. 推荐命名习惯

- 类名：`Player`、`PhysicsSystem`，使用大驼峰。
- 函数名：`update()`、`draw()`、`loadFromFile()`，使用小驼峰。
- 成员变量：`body_`、`state_`，末尾加 `_`。
- 常量：`maxFallSpeed`、`tileSize`，名字表达意义，不要写神秘数字。

---

## 10. 下一步最值得做的功能

如果你们想尽快做出“像横版动作游戏”的感觉，推荐顺序是：

1. 玩家 Dash。
2. 玩家攻击打中敌人。
3. 玩家和敌人生命值。
4. 尖刺和重生点。
5. Sprite Sheet 动画。
6. 第二关和关卡切换。
7. 背景音乐、攻击音效、受伤音效。

先把玩法闭环做出来，再追求精美画面。这样项目不会卡在美术资源还没准备好的阶段。
