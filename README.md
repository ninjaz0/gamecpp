# HollowStarter 横版动作游戏框架

这是一个给 C++ 初学团队使用的 2D 横版游戏骨架，目标是先跑通“窗口、输入、delta time、重力、跳跃、AABB 碰撞、FSM、文本关卡、资源管理”这些基础，再逐步替换为真实美术、敌人 AI、地图编辑器导出的关卡和音效。

项目使用 CMake + raylib。raylib 会在第一次配置 CMake 时自动下载。

## 构建与运行

```bash
cmake -S . -B build
cmake --build build
./build/HollowStarter
```

## 控制

- `A / D` 或方向键：移动
- `Space / W`：跳跃
- `J / K`：攻击
- `R`：重置位置
- `F1`：显示/隐藏调试碰撞框
- 手柄左摇杆、A 键、X 键也可以用

## 五人分工对应目录

1. 核心框架与游戏循环：`src/core`、`include/game/core`
   - `Game` 创建窗口、维护主循环、计算 delta time、更新和绘制所有系统。
   - `Input` 把键盘和手柄输入整理成统一的 `InputState`。

2. 物理与碰撞系统：`src/physics`、`include/game/physics`
   - `PhysicsBody` 保存位置、速度、大小和是否落地。
   - `PhysicsSystem` 负责重力、分轴移动和 AABB 碰撞修正。

3. 动画与渲染引擎：`src/render`、`include/game/render`
   - `AnimationPlayer` 已经支持 Sprite Sheet 的帧切换、循环和左右翻转。
   - 当前 demo 先用色块渲染；后续可把 `Player::draw` / `Enemy::draw` 改为贴图动画。

4. 角色逻辑与状态控制：`src/characters`、`include/game/characters`
   - `Player` 使用有限状态机：Idle、Run、Jump、Fall、Attack、Hurt。
   - `Enemy` 是一个最小巡逻敌人示例，碰墙或前方没地面会转向。

5. 关卡与资源管理：`src/level`、`src/assets`
   - `Level` 从 `assets/levels/demo.txt` 读取文本地图。
   - `AssetManager` 统一加载和释放贴图，避免同一资源重复加载。

更完整的分模块学习教程见：`docs/team-module-tutorial.md`。

多人协作 Git 规则见：`docs/git-collaboration-guide.md`。

## 文本关卡格式

`assets/levels/demo.txt` 使用字符表示地图：

- `#`：实心碰撞块
- `P`：玩家出生点
- `E`：敌人出生点
- `.` 或空格：空气

后续可以把 Tiled 等地图编辑器导出的 JSON 转换到 `Level` 模块里。

## 推荐开发顺序

1. 先让每个人读懂自己负责的目录和头文件接口。
2. 给 `assets/sprites` 放入角色精灵图，并在 `Player::draw` 接入 `AnimationPlayer`。
3. 扩展 `Level`，增加尖刺、平台、门、保存点等 tile 类型。
4. 给 `Player` 增加冲刺、受伤无敌帧、攻击碰撞盒。
5. 给 `Enemy` 增加 FSM，例如 Patrol、Chase、Attack、Stunned。
