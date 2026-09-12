<p align="center">
  <img src="assets/logo.png" alt="OnionHEN" height="128" width="128"/>
</p>

<p align="center">
  <b>OnionHEN FTP Server Plugin</b><br/>
  带 OnionHEN 原生动态设置界面的 PS5 FTP 服务插件
</p>

<p align="center">
  <b>简体中文</b> · <a href="README.md">English</a>
</p>

本仓库把 PS5 `ftpsrv` 实现封装为独立的 OnionHEN 插件。FTP 不再编译进
OnionHEN 的 util daemon；daemon 会发现这个 ELF，校验其中的 `.onion_plugin`
descriptor，管理进程生命周期，并在插件停止后自动清理它注册的 UI。

## 功能

- 单页 OnionHEN 设置界面，提供启用、TCP 端口和带确认的重启操作，无需进入子页面
- 跟随 PS5 系统语言（`zh-Hans` / `en`）实时切换中英双语，无需重启插件
- 持久保存 `enabled` 和 `port` 配置
- 通过 OnionHEN 插件管理器支持启动、停止、重载、删除和休息模式恢复
- 保留上游 `ftpsrv` 的 `KILL`、`SELF`、`SCHK`、`MTRW`、`AUTHID` 等命令
  （具体取决于固件支持）
- 不使用压缩包或自定义容器，插件元数据直接嵌入 ELF

插件进程会自动启动，以便发布动态设置页；FTP 监听器默认开启，监听 TCP `1337`。

## 环境要求

- 支持插件发现和动态 UI 的 OnionHEN 版本
- [PS5 Payload SDK](https://github.com/ps5-payload-dev/sdk)
- CMake 3.20 或更高版本、Ninja
- Git 与 Python 3.9 或更高版本

## 编译

```sh
export PS5_PAYLOAD_SDK=/path/to/ps5-payload-sdk
cmake --preset ps5
cmake --build --preset ps5
```

产物为 `build-ps5/bin/FTPS00001.elf`。构建流程会验证 ELF 中的插件 ID
`FTPS00001`、版本 `1.00` 和 SDK descriptor。

开发 SDK 时可直接使用本地源码：

```sh
cmake --preset ps5 \
  -DONIONHEN_PLUGIN_SDK_SOURCE=/path/to/onionHEN-plugin-sdk
cmake --build --preset ps5
```

在下载的 SDK 与本地 SDK 之间切换前，请删除 `build-ps5/` 后重新配置。

## 安装

使用临时后缀完成原子上传：

```text
/data/OnionHEN/plugins/FTPS00001.installing
    上传完成后重命名
/data/OnionHEN/plugins/FTPS00001.elf
```

OnionHEN 会发现并校验最终的 `.elf`，然后启动插件。进入 **★ OnionHEN 插件**，
选择 **FTP Server**，即可在插件贡献的设置页中启用监听器或修改端口。替换 ELF
会重启受管进程；删除 ELF 会停止进程并移除 UI。

运行时文件：

| 路径 | 用途 |
| --- | --- |
| `/data/OnionHEN/plugins/FTPS00001.elf` | 已安装插件 |
| `/data/OnionHEN/plugins/FTPS00001.ini` | 持久化的 `enabled` 与 `port` 配置 |
| `/data/OnionHEN/FTPS00001.log` | 插件生命周期和错误日志 |

同一个 TCP 端口只能由一个服务绑定。如果已有 FTP payload 占用 `1337`，请先停止
它，或为本插件选择其他端口。

## 架构

```text
OnionHEN plugin manager
  -> 可信 SDK session
  -> source/main.c                  生命周期、事件循环与语言轮询
     -> source/ftp_i18n.c           双语文案与系统语言跟踪
     -> source/plugin_ui.c          单页 UI document 与动作校验
     -> source/plugin_settings.c    原子本地持久化
     -> source/ftp_service.c        同步的监听器生命周期
        -> third_party/ftpsrv       FTP 协议实现
```

descriptor 声明 IPC、UI、process、kernel capability，以及 `AUTO_START`、
`LONG_RUNNING`、`STOP_SUPPORTED` flag。插件代码只依赖 SDK 的公开 C ABI。
`main.c` 统一持有资源并按逆序释放；FTP adapter 把 vendored server 与 UI、transport
代码隔离开。

## 贡献与安全

提交 Pull Request 前请阅读 [CONTRIBUTING.md](CONTRIBUTING.md)。参与项目时请遵守
[CODE_OF_CONDUCT.md](CODE_OF_CONDUCT.md)。安全问题请按照
[SECURITY.md](SECURITY.md) 私下报告。

## 致谢与许可证

本插件基于 [`drakmor/ftpsrv`](https://github.com/drakmor/ftpsrv)，并保留其 GPL
声明。固定源码版本与本地适配说明见
[THIRD_PARTY_NOTICES.md](THIRD_PARTY_NOTICES.md)。

本仓库采用 [GNU General Public License v3.0](LICENSE)。OnionHEN 是非官方自制
软件项目，与 Sony Interactive Entertainment 无关。请仅在自己拥有的硬件上使用，
风险自负。
