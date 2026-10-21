-- CircleMUD GameEngine scripting API.
-- Scripts are data/gameplay extensions; authoritative state remains in C.

Game = Game or {}

function Game_Update(delta_seconds)
  if Game.on_update then
    Game.on_update(delta_seconds)
  end
end

function Game_Event(event_type, actor_id, target_id, value, name)
  if Game.on_event then
    Game.on_event({
      type = event_type,
      actor_id = actor_id,
      target_id = target_id,
      value = value,
      name = name
    })
  end
end

return Game
