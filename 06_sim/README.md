# 06_sim · 地面站联调与仿真

**时间**：W9 - W10

## 每日任务

**W9 通信与地面站联调**

- W9D1 MAVLink 消息封装：`ATTITUDE`、`RC_CHANNELS`、`HEARTBEAT`
- W9D2 QGroundControl 连接测试（USB / UDP）
- W9D3 遥控器 SBUS / CRSF 信号解析
- W9D4 电机 PWM 输出（了解 DShot 协议）
- W9D5 数据记录 + 上位机实时曲线显示
- W9D6 联调：遥控 → 飞控 → MAVLink 遥测 → QGC 显示
- W9D7 Boss：波形 / 截图 / 日志归档

## 产出物

- `qgc_params/`：QGroundControl 参数备份
- `logs/`：飞行 / 仿真日志（大文件已在 .gitignore 中排除）
- 联调截图与录屏 → 记入 `/波形证据.md` 与 `/简历弹药库.md`

## 通过标准

能完整复现一条链路：遥控输入 → 飞控解算 → MAVLink 输出 → 地面站显示。

## 环境搭建备忘

```bash
# PX4 + Gazebo 联合仿真
make px4_sitl gz_x500
```

> 注意：Windows 上建议用 WSL2 + Ubuntu 22.04 搭建 PX4 环境，原生 Windows 编译容易踩坑。
