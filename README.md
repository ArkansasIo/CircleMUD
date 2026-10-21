# CircleMUD — ArkansasIo mirror

This repository is an unofficial mirror of the legacy CircleMUD 3.1 engine. Preserve upstream credits and the license materials included with the source. Review `license.doc` before redistributing or operating modified builds.

## Project status

The codebase is a classic C / Make-based MUD server. The current goal is to make the existing source easier to build and maintain before introducing gameplay changes.

## Build on Linux

A typical build uses a POSIX-compatible environment and a C toolchain:

```sh
cd src
../configure
make
```

Read `doc/README.UNIX` and the original `README` for platform-specific details. This is an older codebase; modern compilers may emit compatibility warnings or expose build issues. Review the output rather than assuming a successful build.

## CI

GitHub Actions runs a Linux build smoke test. The workflow does not deploy a public game server or publish a binary release.

## Documentation

- [Modernization roadmap](docs/ROADMAP.md)
- Original documentation: `doc/`
- Original project notes: `README`, `ChangeLog`, and `FAQ`

## Development principles

1. Preserve CircleMUD attribution and the original license notices.
2. Keep gameplay changes incremental and testable.
3. Avoid committing player data, passwords, generated binaries, or local configuration.
4. Treat an internet-facing server as untrusted input: validate protocol data and apply operational security controls before public deployment.
