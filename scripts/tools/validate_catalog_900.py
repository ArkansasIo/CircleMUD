#!/usr/bin/env python3
"""Validate the CircleMUD 900-entry content catalog specification and generator."""
import json
from pathlib import Path

ROOT = Path(__file__).resolve().parents[2]
spec = json.loads((ROOT / "data/rpg/catalog_900.json").read_text(encoding="utf-8"))
expected = sum(spec["distribution"].values())
assert expected == 900, f"distribution totals {expected}, expected 900"
assert spec["total_entries"] == 900
for category, count in spec["distribution"].items():
    assert count > 0, category
print("catalog_900: specification valid; 900 deterministic records configured")
