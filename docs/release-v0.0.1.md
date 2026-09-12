# OnionHEN FTP Server Plugin v0.0.1

Managed FTP server plugin for OnionHEN on jailbroken PS5 consoles. It runs the `ftpsrv` listener as a standalone plugin with a native OnionHEN settings page for enable, TCP port, and restart.

**Requires:** [OnionHEN](https://github.com/aydencharles/onionHEN) v0.0.13 or later

**Supported firmware:** 4.03 – 12.70

## Quick start

1. OnionHEN v0.0.13 or later must already be running.
2. Copy `FTPS00001.elf` to:

```
/data/OnionHEN/plugins/FTPS00001.elf
```

3. OnionHEN discovers and starts the plugin. Open **Toolbox → Plugins → FTP Server**.

The FTP listener is enabled by default on TCP `1337`. If another FTP payload already uses that port, stop it or pick another port in the plugin settings.

## What's new

First public release.

* Standalone OnionHEN plugin; FTP is no longer compiled into the utility daemon.
* Single-page settings UI: enable, TCP port (`1`–`65535`), and confirmed restart.
* FTP listener starts by default on TCP `1337`.
* Toolbox UI follows the PS5 system language (`zh-Hans` / `en`) without restarting the plugin.
* Upstream `ftpsrv` commands including `KILL`, `SELF`, `SCHK`, `MTRW`, and `AUTHID` where the firmware supports them.
* Settings are stored in `/data/OnionHEN/plugins/FTPS00001.ini`.

## Changelog

First public release.

## Installation

Place the ELF at:

```
/data/OnionHEN/plugins/FTPS00001.elf
```

To update atomically:

1. Upload to `/data/OnionHEN/plugins/FTPS00001.installing`
2. Rename to `/data/OnionHEN/plugins/FTPS00001.elf`

Plain payloads in `/data/OnionHEN/payloads` are **not** plugins. OnionHEN only starts ELFs that carry a valid `.onion_plugin` descriptor.

From **Toolbox → Plugins** you can start or stop the plugin for the current session, and enable auto-start for the next OnionHEN launch.

## Configuration

Open **Toolbox → Plugins → FTP Server**:

* Enabled (default on)
* TCP port (default `1337`)
* Restart server

Options are also written to:

```
/data/OnionHEN/plugins/FTPS00001.ini
```

Only one service can bind a TCP port. If another FTP payload already uses `1337`, stop it or select another port before enabling this plugin.

## File locations

| Purpose | Location |
| --- | --- |
| Plugin ELF | `/data/OnionHEN/plugins/FTPS00001.elf` |
| Settings | `/data/OnionHEN/plugins/FTPS00001.ini` |
| Log | `/data/OnionHEN/FTPS00001.log` |

## Credits

* [drakmor/ftpsrv](https://github.com/drakmor/ftpsrv) — upstream PS5 FTP server this plugin packages
* [OnionHEN](https://github.com/aydencharles/onionHEN) and [OnionHEN Plugin SDK](https://github.com/OnionBuddies/onionHEN-plugin-sdk)
* [PS5 Payload SDK](https://github.com/ps5-payload-dev/sdk) — John Törnblom and contributors
