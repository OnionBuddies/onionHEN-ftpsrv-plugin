# Security Policy

## Supported versions

Security fixes are applied to the latest code on the default branch. Before a
stable release exists, older snapshots are not maintained.

## Report a vulnerability

Do not open a public issue for vulnerabilities that could expose console data,
allow unintended privileged operations, bypass plugin capabilities, corrupt
memory through malformed IPC, or execute untrusted code.

Use the repository's **Security** tab and select **Report a vulnerability**.
Include the affected commit, reproduction steps, expected impact, required
privileges, and a suggested mitigation when known. Do not attach proprietary
SDK files, decrypted system binaries, keys, credentials, or console identifiers.

Maintainers will review complete reports, coordinate fixes privately when
practical, and credit reporters unless anonymity is requested. Do not disclose
the issue publicly before a fix or coordinated disclosure decision.

Ordinary build failures, crashes without a security impact, and documentation
errors should use the public bug report form.

## Scope

This repository is an unofficial PS5 homebrew FTP plugin. Reports about
Sony services, PlayStation Network, unrelated payloads, or vulnerabilities in
the PS5 platform itself are outside this project's scope.
