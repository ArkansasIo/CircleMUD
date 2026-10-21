# AetherWorld GitHub Pages

This directory contains the static web dashboard for the AetherWorld / CircleMUD RPG project.

## Local preview

From the repository root:

```sh
python3 -m http.server 8080 --directory docs/site
```

Then open `http://localhost:8080`.

## GitHub Pages

`.github/workflows/pages.yml` publishes this directory automatically when changes are pushed to `master` or `main`.

The site is a frontend/dashboard only. It does not expose CircleMUD server credentials, player save files, or private runtime configuration.

## UI areas

- Dashboard
- Character / Play
- World map and zones
- RPG systems
- API console
- Developer documentation

The API console is intentionally a static demonstration until a live HTTP/WebSocket API endpoint is configured.
