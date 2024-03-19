-- this is game object script for a oldierNPC object of CharacterControl demo
-- this script will be executed whenever a SoldierNPC is added to the game

-- expected arguments:
-- skin name: which asset to use
-- skin package
-- gunMeshName,Package mesh for the gun
-- base
-- uuid of the game object
-- patrolWayPoint (optional)
-- npcType

outputDebugString("Executing Vampire.lua")
function runScript(args)
    local patrolWayPoint = nil
    if args['patrolWayPoint'] ~= nil then
        patrolWayPoint = args['patrolWayPoint']
    end
    local enemy = nil
    if args['enemy'] ~= nil then
        enemy = args['enemy']
    end
    local pos = args['base']['pos']
    local u = args['base']['u']
    local v = args['base']['v']
    local n = args['base']['n']
    evt = root.CharacterControl.Events.Event_CreateSoldierNPC.Construct(l_getGameContext(),
        args['skinName'], args['skinPackage'], 
        args['gunMeshName'], args['gunMeshPackage'],
        args['npcType'],
        pos[1], pos[2], pos[3],
        u[1], u[2], u[3],
        v[1], v[2], v[3],
        n[1], n[2], n[3],
        args['peuuid'],
        patrolWayPoint, -- could be nil
        enemy
    )
    handler = getGameObjectManagerHandle(l_getGameContext())
    root.PE.Components.Component.SendEventToHandle(handler, evt)
end
