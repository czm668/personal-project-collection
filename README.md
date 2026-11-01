# 停车场管理系统（C）

C 语言控制台停车场管理系统：**双角色权限、车辆进出、分时计费规则、月卡管理、停车记录**，数据以文件形式持久化保存。

## 功能

- **双角色权限**：`admin`（管理员）/ `user`（普通用户），通过启动参数 + 密码校验后进入不同菜单
- **车辆管理**：车辆入场 / 出场，车位与在停车辆管理
- **计费规则**：白天 / 夜间 / 节假日费率，费率数据可持久化维护
- **月卡管理**：月卡车辆登记、有效期与余额、续费
- **停车记录**：入场 / 出场留痕与费用记录，支持历史查询
- **数据持久化**：车辆、费率、月卡、记录四类数据文件，程序启动自动加载历史数据

## 编译与运行

### Windows 一键编译（推荐）

**双击 `build.bat`**（或在仓库根目录执行）：

```bat
build.bat
```

> ⚠️ 仓库不包含编译产物（`*.exe` 已被 `.gitignore` 排除），**必须先编译再运行**。
> 脚本以 GBK 编码保存、命令写成单行（不使用 `^` 续行符），兼容中文 Windows（代码页 936）。

### 运行

```bat
run.bat              :: 管理员菜单（等价于 ParkManSys.exe admin 123456）
run.bat user         :: 普通用户菜单
```

### 其他方式（gcc / make）

```bash
# 直接调用 gcc（Windows / Linux 通用）
gcc -Wall -Wextra -std=c99 -o ParkManSys src/main.c src/menu.c src/car.c src/fee.c src/month_card.c src/park_record.c -lm

# 运行（角色 + 密码）
./ParkManSys admin 123456    # 管理员
./ParkManSys user 123456     # 普通用户

# 已安装 make 时（Linux，或装了 MinGW make 的 Windows）
make
```

> 已用 MinGW-W64 gcc 16.1 以 `-Wall -Wextra` 验证：**零警告编译通过**。
>
> 注：`src/common.h` 仅提供公共数据结构与常量声明，不参与编译（Makefile 只编译上列 6 个 `.c` 文件）。

## 目录结构

```
src/
  main.c           程序入口、启动参数与权限校验
  menu.c           管理员 / 用户菜单与交互
  car.c            车辆进场、出场与在停管理
  fee.c            计费规则管理与费用计算
  month_card.c     月卡登记、续费与余额
  park_record.c    停车记录查询与统计
  common.h         公共数据结构与常量（车辆 / 费率 / 月卡 / 记录）
Makefile                 Linux / 已安装 make 时使用
build.bat                Windows 一键编译
run.bat                  Windows 一键运行
```

运行时自动生成数据文件：`car.dat`、`fee_rule.dat`、`month_card.dat`、`park_record.dat`

## 说明

演示密码为源码中的固定值，仅供本地演示使用，请勿用于生产环境。

## 可改进方向

- 数据文件目前为纯文本格式，可加入记录级校验字段与格式版本号
- 可补充计费边界的单元测试（跨天、跨节假日、月卡到期临界点）
- 可增加数据导出（CSV）与按时间段的营收统计报表