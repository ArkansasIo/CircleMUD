/* CircleMUD browser API bridge. Transport only: authoritative state remains in the game engine. */
export class CircleMUDClient {
  constructor(baseUrl = '/api/v1') {
    this.baseUrl = baseUrl.replace(/\/$/, '');
  }

  async request(path, options = {}) {
    const response = await fetch(`${this.baseUrl}${path}`, {
      headers: { Accept: 'application/json', ...(options.headers || {}) },
      ...options
    });
    if (!response.ok) {
      throw new Error(`CircleMUD API ${response.status}: ${response.statusText}`);
    }
    return response.json();
  }

  system() { return this.request('/system'); }
  health() { return this.request('/health'); }
  worldMap() { return this.request('/world/map'); }
  location(id) { return this.request(`/world/locations/${encodeURIComponent(id)}`); }
  playerState() { return this.request('/player/state'); }
  activeQuests() { return this.request('/quests/active'); }
  activeEvents() { return this.request('/events/active'); }

  async loadGameState() {
    const [system, map, player, quests, events] = await Promise.all([
      this.system(), this.worldMap(), this.playerState(), this.activeQuests(), this.activeEvents()
    ]);
    return { system, map, player, quests, events };
  }
}

export function createCircleMUDClient(baseUrl) {
  return new CircleMUDClient(baseUrl);
}
