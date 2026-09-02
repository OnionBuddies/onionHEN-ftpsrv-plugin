# Third-Party Notices

## ftpsrv

- Upstream: <https://github.com/drakmor/ftpsrv>
- Branch: `nexgen`
- Imported revision: `b6c4784f33d6fc8d26f719f7dc9e1cc9bb8d58cd`
- License: GNU General Public License v3.0 or later
- Local source: `third_party/ftpsrv/`

The imported PS5 sources retain their upstream copyright and license notices.
OnionHEN integration adds a stoppable listener lifecycle used by
`source/ftp_service.c`; the standalone upstream entry points and platform files
that are not needed by the plugin build are omitted. The upstream README and
license are preserved as `third_party/ftpsrv/README.upstream.md` and
`third_party/ftpsrv/LICENSE`.
