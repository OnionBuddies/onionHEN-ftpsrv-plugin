# Contributing

Keep contributions focused on the FTP service, its OnionHEN integration, and
the vendored upstream adapter. SDK ABI or host behavior changes belong in their
respective repositories.

All participation is governed by the [Code of Conduct](CODE_OF_CONDUCT.md).

## Development setup

```sh
export PS5_PAYLOAD_SDK=/path/to/ps5-payload-sdk
cmake --preset ps5 \
  -DONIONHEN_PLUGIN_SDK_SOURCE=/path/to/onionHEN-plugin-sdk
cmake --build --preset ps5
```

Before submitting a change:

1. Build the plugin with the pinned SDK or a clearly identified SDK commit.
2. Confirm the post-build ELF descriptor validation succeeds.
3. Run `git diff --check`.
4. Update both READMEs when setup, behavior, or project structure changes.
5. Record PS5 hardware validation when the change affects runtime behavior.

## Design rules

- Keep process/session lifecycle in `source/main.c`.
- Keep UI construction and action decoding in `source/plugin_ui.c`.
- Keep FTP listener lifecycle in `source/ftp_service.c`.
- Keep persistence in `source/plugin_settings.c`.
- Define plugin identity once in `CMakeLists.txt`.
- Use only the SDK's public C headers across the host boundary.
- Validate input and return specific `onion_status` errors.
- Acquire resources in one direction and release them in reverse order.
- Avoid adding abstractions unless they remove real duplication or isolate a
  dependency.

Do not commit build directories, ELF artifacts, PS5 SDK files, proprietary
libraries, decrypted system files, keys, console identifiers, or logs.

## Pull requests

Use a Conventional Commit subject, keep one logical change per pull request,
and explain the compatibility and hardware-validation status. By contributing,
you agree that your changes are licensed under this repository's GPL-3.0
license.
