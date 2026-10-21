export class CircleMUDMapController {
  constructor(root, data) {
    this.root = root;
    this.data = data;
    this.zoomLevel = 1;
    this.offset = { x: 0, y: 0 };
    this.selected = null;
    this.layers = new Map();
  }

  setLayer(id, enabled) {
    this.layers.set(id, Boolean(enabled));
    this.root.dispatchEvent(new CustomEvent('map-layer-change', {
      detail: { id, enabled: Boolean(enabled) }
    }));
  }

  selectLocation(locationId) {
    const location = (this.data.locations || []).find(x => x.id === locationId);
    if (!location) return null;
    this.selected = location;
    this.root.dispatchEvent(new CustomEvent('map-location-select', {
      detail: location
    }));
    return location;
  }

  setPlayerPosition(x, y, zone) {
    this.data.player = { x, y, zone: zone || '' };
    this.root.dispatchEvent(new CustomEvent('map-player-move', {
      detail: this.data.player
    }));
  }

  zoom(delta) {
    this.zoomLevel = Math.max(0.25, Math.min(3, this.zoomLevel + delta));
    this.root.dispatchEvent(new CustomEvent('map-zoom', {
      detail: { zoom: this.zoomLevel }
    }));
  }

  reset() {
    this.zoomLevel = 1;
    this.offset = { x: 0, y: 0 };
    this.root.dispatchEvent(new Event('map-reset'));
  }
}
