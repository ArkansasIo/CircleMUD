# CircleMUD Modernization Roadmap

## Mission

Maintain the legacy CircleMUD codebase as a buildable, documented text-MUD foundation. Modernize incrementally without silently changing classic gameplay or removing upstream attribution.

## Phase 1 — Reproducible baseline

- [x] Add a Linux GitHub Actions build workflow.
- [x] Document the current source layout and basic build entry point.
- [ ] Confirm the workflow against the real hosted runner and fix compiler or dependency failures.
- [ ] Record compiler warnings and distinguish known legacy warnings from regressions.
- [ ] Add ignore rules for generated binaries, object files, logs, and local runtime data after checking existing repository conventions.

## Phase 2 — Regression checks

- Add unit tests for isolated helpers where they can be tested without launching a full server.
- Add smoke tests for configuration parsing and world-data loading.
- Add a disposable test world and launch/stop integration test.
- Add sanitizers (AddressSanitizer and UndefinedBehaviorSanitizer) as a separate, non-blocking job once the baseline build is stable.
- Ensure tests never read or modify production player files.

## Phase 3 — Security and operations

- Audit input parsing, string formatting, file paths, authentication, and privilege boundaries.
- Verify password storage and any legacy cryptographic dependency before public deployment.
- Add documented server bind-address, firewall, backup, rotation, and least-privilege guidance.
- Establish a supported compiler/platform matrix and a repeatable release process.
- Do not expose administrative commands or data directories to untrusted users.

## Phase 4 — Developer experience

- Document module boundaries and lifecycle: communications, interpreter, database/world loading, movement, combat, spells, objects, persistence, and utilities.
- Add architecture diagrams and command-registration guidance.
- Document data formats and safe world-building workflows.
- Track compatibility decisions in architecture decision records.

## Phase 5 — Optional RPG/MMORPG extensions

Implement only after the baseline is tested and maintain classic behavior by default:

- Data-driven items, enemies, quests, zones, and encounter definitions.
- Character progression, equipment, crafting, guilds, parties, and instance/raid concepts.
- Structured logging, metrics, moderation/audit events, and administrative tooling.
- A versioned external API or web status dashboard only if a clear operational need exists.

## Compatibility and licensing

This project is an unofficial mirror of CircleMUD. Retain original credits and license notices. Review the included license document and any third-party notices before distributing modifications, binaries, or hosted services.

## Definition of done

A phase is complete only when its tests pass on the configured CI runner, the relevant behavior is documented, and any known limitations are recorded.
