﻿if MMO==nil then
	MMO={};
end

--场景类型

MMO.SCENE_TYPE=
{
	SCENE_TYPE_NORMAL = 0,
	SCENE_TYPE_HALL = 1,
	SCENE_TYPE_MAX = 2,

};


--场景配置标志位

MMO.SCENE_CONFIG_FLAG=
{
	SCENE_CONFIG_FLAG_OTHER_PLAYER_INVISIBLE = (1),--玩家间不可见
	SCENE_CONFIG_FLAG_IS_DUPLICATION = (1<<2),--副本场景
	SCENE_CONFIG_FLAG_DUPLICATION_CAN_RESET = (1<<3),--玩家可重置副本

};


--预留场景ID

MMO.RESERVED_SCENE_ID=
{
	RESERVED_SCENE_ID_HALL = 1,--大厅场景
	RESERVED_SCENE_ID_MAX = 1000,--最大系统预留场景ID

};





