﻿if MMO==nil then
	MMO={};
end



MMO.LOG_TYPE=
{
	LOG_TYPE_NONE = 0,
	LOG_TYPE_LOGIN = 1,
	LOG_TYPE_LOGOUT = 2,
	LOG_TYPE_CREATE_ACCOUNT = 3,
	LOG_TYPE_CREATE_PLAYER = 4,
	LOG_TYPE_RECHARGE_INFO = 5,
	LOG_TYPE_ONLINE = 6,
	LOG_TYPE_ITEM = 7,
	LOG_TYPE_SHOP_BUY = 8,
	LOG_TYPE_EXP_CHANGE = 9,
	LOG_TYPE_FINISH_QUEST = 10,
	LOG_TYPE_ADD_HERO = 11,
	LOG_TYPE_REMOVE_HERO = 12,
	LOG_TYPE_GET_SHIP = 13,
	LOG_TYPE_REMOVE_SHIP = 14,
	LOG_TYPE_CREATE_SHIP_DRAWING = 15,
	LOG_TYPE_NEW_FLEET = 16,
	LOG_TYPE_UPDATE_FLEET = 17,
	LOG_TYPE_REMOVE_FLEET = 18,
	LOG_TYPE_FLEET_WRAP_START = 19,
	LOG_TYPE_FLEET_WRAP_END = 20,
	LOG_TYPE_FLEET_WRAP_CHANGE = 21,
	LOG_TYPE_STELLAR_OWNER_CHANGE = 22,
	LOG_TYPE_MAX = 23,

};




MMO.LOG_ITEM_SUB_TYPE=
{
	ADD = 1,
	DEL = 2,
	MOD = 3,

};





