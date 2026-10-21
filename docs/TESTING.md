# CircleMUD Testing Strategy

## Current baseline

The primary regression check is the Linux configure-and-build workflow in `.github/workflows/build.yml`. It verifies that the repository can be configured and that the engine and utilities compile on the GitHub-hosted Ubuntu runner.

## Test layers

### 1. Build test

Run:

```sh
./configure
cd src
make
```

A build failure is a release-blocking regression until investigated.

### 2. Static checks

Future CI jobs should run compiler diagnostics and, where practical, a static analyzer. Warnings should initially be recorded rather than converted wholesale into errors because this is a legacy C codebase.

### 3. Unit tests

Prioritize deterministic helpers that can be separated from the server process:

- string and parsing helpers
- random-number boundaries
- damage calculations
- experience and level calculations
- movement/coordinate helpers
- object and character utility functions

Tests must not depend on production player files or live network sockets.

### 4. Integration smoke test

Create a disposable test world and verify:

1. server starts;
2. world files load;
3. a local client can connect;
4. login reaches the expected game state;
5. representative commands execute;
6. server shuts down cleanly.

The test environment must use temporary data directories.

### 5. Sanitizer build

Use AddressSanitizer and UndefinedBehaviorSanitizer as a separate CI job. Keep sanitizer failures visible without making the first migration of the legacy codebase unnecessarily disruptive.

## Regression workflow

When fixing a bug:

1. Reproduce it.
2. Add the smallest deterministic test that captures the failure.
3. Fix the implementation.
4. Run the normal build.
5. Run the relevant test layer.
6. Document compatibility implications.

## Test data policy

Never commit real player records, passwords, private keys, production configuration, or live server logs.
