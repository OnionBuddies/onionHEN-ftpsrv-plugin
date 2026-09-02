<p align="center">
  <img src="assets/logo.png" alt="OnionHEN" height="128" width="128"/>
</p>

<p align="center">
  <b>OnionHEN FTP Server Plugin</b><br/>
  A managed PS5 FTP server with native OnionHEN dynamic settings UI
</p>

<p align="center">
  <b>English</b> · <a href="README_ZH.md">简体中文</a>
</p>

This repository packages the PS5 `ftpsrv` implementation as a standalone
OnionHEN plugin. FTP is no longer compiled into OnionHEN's utility daemon: the
daemon discovers this ELF, validates its embedded `.onion_plugin` descriptor,
owns its process lifecycle, and removes its UI contribution when it stops.

## Features

- Dynamic OnionHEN settings page with enable, TCP port, and confirmed restart
  controls
- Persistent `enabled` and `port` settings
- Graceful start, stop, reload, deletion, and rest-mode process recovery through
  the OnionHEN plugin manager
- Upstream `ftpsrv` commands including `KILL`, `SELF`, `SCHK`, `MTRW`, and
  `AUTHID` where the firmware supports them
- No package container or extraction step; metadata is embedded in the ELF

The plugin process starts automatically so it can publish its settings page.
The FTP listener itself is disabled by default and listens on TCP `1337` when
enabled.

## Requirements

- An OnionHEN build with plugin discovery and dynamic UI support
- [PS5 Payload SDK](https://github.com/ps5-payload-dev/sdk)
- CMake 3.20 or newer and Ninja
- Git and Python 3.9 or newer

## Build

```sh
export PS5_PAYLOAD_SDK=/path/to/ps5-payload-sdk
cmake --preset ps5
cmake --build --preset ps5
```

The output is `build-ps5/bin/ftpsrv.elf`. The build validates that the ELF
contains plugin ID `FTPS00001`, version `1.00`, and a valid SDK descriptor.

To build against a local SDK checkout:

```sh
cmake --preset ps5 \
  -DONIONHEN_PLUGIN_SDK_SOURCE=/path/to/onionHEN-plugin-sdk
cmake --build --preset ps5
```

Delete `build-ps5/` before switching between a downloaded SDK and a local SDK
checkout.

## Install

Upload the completed ELF atomically:

```text
/data/OnionHEN/plugins/FTPS00001.installing
    rename after upload
/data/OnionHEN/plugins/FTPS00001.elf
```

OnionHEN detects the final `.elf`, validates it, and starts the plugin. Open
**★ OnionHEN Plugins**, select **FTP Server**, then use its contributed settings
page to enable the listener or change its port. Replacing the ELF restarts the
managed process; deleting it stops the process and removes its UI.

Runtime files:

| Path | Purpose |
| --- | --- |
| `/data/OnionHEN/plugins/FTPS00001.elf` | Installed plugin |
| `/data/OnionHEN/plugins/FTPS00001.ini` | Persistent `enabled` and `port` values |
| `/data/OnionHEN/FTPS00001.log` | Plugin lifecycle and error log |

Only one service can bind a TCP port. If another FTP payload already uses
`1337`, stop it or select another port before enabling this plugin.

## Architecture

```text
OnionHEN plugin manager
  -> trusted SDK session
  -> source/main.c                  lifecycle and event loop
     -> source/plugin_ui.c          UI document and action validation
     -> source/plugin_settings.c    atomic local persistence
     -> source/ftp_service.c        synchronized listener lifecycle
        -> third_party/ftpsrv       FTP protocol implementation
```

The descriptor declares IPC, UI, process, and kernel capabilities together
with `AUTO_START`, `LONG_RUNNING`, and `STOP_SUPPORTED`. Plugin code uses only
the SDK's public C ABI. `main.c` owns resources and releases them in reverse
order; the FTP adapter isolates the vendored server from UI and transport code.

## Contributing and security

Read [CONTRIBUTING.md](CONTRIBUTING.md) before opening a pull request.
Participation is governed by [CODE_OF_CONDUCT.md](CODE_OF_CONDUCT.md). Report
security-sensitive issues privately according to [SECURITY.md](SECURITY.md).

## Credits and license

This plugin is based on [`drakmor/ftpsrv`](https://github.com/drakmor/ftpsrv)
and retains its GPL notices. See [THIRD_PARTY_NOTICES.md](THIRD_PARTY_NOTICES.md)
for the pinned source revision and local integration notes.

The repository is licensed under the [GNU General Public License v3.0](LICENSE).
OnionHEN is an unofficial homebrew project and is not affiliated with Sony
Interactive Entertainment. Use it only on hardware you own and at your own
risk.
