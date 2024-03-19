
-- this script is in lua format
-- this is a meta script that fills in data that is passed to 'myScript' that in turn calls C++ function
-- some of the data can be set by default, some of the data might be required to be edited from maya
function fillMetaInfoTable(args) -- the script fromat requires existance of this function

-- user modifed data
args['myScript']="SoldierNPC.lua"
args['myScriptPackage']="CharacterControl"

args['skinName'] = "Vampire.mesha"
args['skinPackage'] = "Vampire"

args['gunMeshName'] = "m98.x_m98main_mesh.mesha"
args['gunMeshPackage'] = "CharacterControl"

args['npcType'] = 'Guard'
args['animation'] = 'partial'
end -- required
