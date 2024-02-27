
-- this script is in lua format
-- this is a meta script that fills in data that is passed to 'myScript' that in turn calls C++ function
-- some of the data can be set by default, some of the data might be required to be edited from maya
function fillMetaInfoTable(args) -- the script fromat requires existance of this function

-- user modifed data
args['myScript']="Target.lua"
args['myScriptPackage']="ExportTutorial"

--needed for static mesh
args['meshName'] = "imrod.x_main_mesh.mesha"
args['meshPackage'] = "ExportTutorial"
args['npcType'] = 'Enemy'
args['health'] = 100
args['weapon'] = 'sword'
args['lethality'] = 50
args['range'] = 500 -- 10 meters
args['behavior'] = 'random_patrol'

end -- required
