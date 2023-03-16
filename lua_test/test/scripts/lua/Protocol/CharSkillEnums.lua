﻿if MMO==nil then
	MMO={};
end



MMO.BUFF_CONFIG_FLAG=
{
	BUFF_CONFIG_FLAG_HAVE_PASSIVE_EFFECT = 1,--含被动效果
	BUFF_CONFIG_FLAG_CAN_NOT_BE_DISPERSE = (1<<1),
	BUFF_CONFIG_FLAG_NOT_REMOVE_BY_DEATH = (1<<2),
	BUFF_CONFIG_FLAG_REMOVE_ON_BATTLE_END = (1<<3),
	BUFF_CONFIG_FLAG_IS_VIP_REWARD = (1<<4),
	BUFF_CONFIG_FLAG_RUN_ON_OFFLINE = (1<<5),
	BUFF_CONFIG_FLAG_HIDE = (1<<6),
	BUFF_CONFIG_FLAG_HAVE_MOVE_LIMIT_EFFECT = (1<<7),--含移动限制效果
	BUFF_CONFIG_FLAG_NOT_RESET_TIME_ON_PILE = (1<<8),--叠加层数时不重置Buff时间

};




MMO.BUFF_TYPE=
{
	BUFF_TYPE_PASSIVE = 0,
	BUFF_TYPE_ACTIVE = 1,
	BUFF_TYPE_MAX = 2,

};




MMO.CHAIN_EFFECT_LINK_TYPE=
{
	CHAIN_EFFECT_LINK_TYPE_TO_CASTER = 0,
	CHAIN_EFFECT_LINK_TYPE_TO_PREV_TARGET = 1,
	CHAIN_EFFECT_LINK_TYPE_MAX = 2,

};


--伤害类型

MMO.DAMAGE_TYPE=
{
	DAMAGE_TYPE_PHY = (1 << 0),
	DAMAGE_TYPE_MAG = (1 << 1),
	DAMAGE_TYPE_BIT_START = 0,
	DAMAGE_TYPE_BIT_LEN = 4,
	DAMAGE_TYPE_MASK = 0xF,
	DAMAGE_HIT_HIT = (1 << 4),
	DAMAGE_HIT_CRIT = (1 << 5),
	DAMAGE_HIT_AVOID = (1 << 6),
	DAMAGE_HIT_RESISTANCE = (1 << 7),
	DAMAGE_HIT_IMMUNITY = (1 << 8),
	DAMAGE_HIT_ABSORB = (1 << 9),
	DAMAGE_HIT_BLOCK = (1 << 10),
	DAMAGE_HIT_BIT_START = 4,
	DAMAGE_HIT_BIT_LEN = 12,
	DAMAGE_HIT_MASK = 0xFFF0,
	DAMAGE_ELEMENT_BIT_START = 16,
	DAMAGE_ELEMENT_BIT_LEN = 16,
	DAMAGE_ELEMENT_MASK = 0xFFFF0000,

};


--限制条件类型

MMO.LIMIT_TYPE=
{
	LIMIT_TYPE_PROFRESSION = 0,--职业限制[职业类型]
	LIMIT_TYPE_LEVEL_RANGE = 1,--等级范围[等级ID|最低等级|最高等级]
	LIMIT_TYPE_TARGET_LEVEL_RANGE = 2,--目标等级范围[等级ID|最低等级|最高等级]
	LIMIT_TYPE_BUFF_HAVE = 3,--拥有某个BUFF[Buff类型ID|所需层数]
	LIMIT_TYPE_TARGET_HAVE_BUFF = 4,--目标拥有Buff[Buff类型ID|所需层数]
	LIMIT_TYPE_NOT_HAVE_BUFF = 5,--未拥有BUFF[Buff类型ID|所需层数]
	LIMIT_TYPE_HAVE_PET = 6,--拥有宠物
	LIMIT_TYPE_HAVE_ITEM = 7,--拥有道具[背包类型|道具类型ID|所需数量]
	LIMIT_TYPE_HAVE_EQUIP_ITEM = 8,--装备指定装备[装备位置|装备道具ID(0表示任意装备)]
	LIMIT_TYPE_NOT_HAVE_EQUIP_ITEM = 9,--未装备指定装备[装备位置|装备道具ID(0表示任意装备)]
	LIMIT_TYPE_RES_PERCENT_RANGE = 10,--资源百分比范围[资源属性ID|下限(100=100%)|上限(100=100%)]
	LIMIT_TYPE_TARGET_RES_PERCENT_RANGE = 11,--目标资源百分比范围[资源属性ID|下限(100=100%)|上限(100=100%)]
	LIMIT_TYPE_HAVE_CHAR_STATE = 12,--拥有角色状态[需要拥有的状态<CharStatus>|不能拥有的状态<CharStatus>]
	LIMIT_TYPE_TARGET_HAVE_CHAR_STATE = 13,--目标拥有角色状态[需要拥有的状态<CharStatus>|不能拥有的状态<CharStatus>]
	LIMIT_TYPE_HAVE_SKILL = 14,--拥有技能[技能ID]
	LIMIT_TYPE_TARGET_HAVE_SKILL = 15,--目标拥有技能[技能ID]
	LIMIT_TYPE_NOT_HAVE_SKILL = 16,--未拥有技能[技能ID]
	LIMIT_TYPE_TARGET_NOT_HAVE_SKILL = 17,--目标未拥有技能[技能ID]
	LIMIT_TYPE_HAVE_EMPTY_BAG_POS = 18,--拥有背包空位[背包类型|所需空位]
	LIMIT_TYPE_CASTER_IS_PLAYER = 19,--施放者是玩家
	LIMIT_TYPE_CASTER_IS_PET = 20,--施放者是宠物
	LIMIT_TYPE_CASTER_IS_NPC = 21,--施放者是NPC
	LIMIT_TYPE_TARGET_IS_PLAYER = 22,--目标是玩家
	LIMIT_TYPE_TARGET_IS_PET = 23,--目标是宠物
	LIMIT_TYPE_TARGET_IS_NPC = 24,--目标是NPC
	LIMIT_TYPE_HAVE_EMPTY_SKILL_SLOT = 25,--有空技能槽[槽位数]
	LIMIT_TYPE_HAVE_EMPTY_PET_SLOT = 26,--拥有空余宠物槽[槽位数]
	LIMIT_TYPE_RACE = 27,--自身种族类型[种族类型]
	LIMIT_TYPE_TARGET_RACE = 28,--目标种族类型[种族类型]
	LIMIT_TYPE_LEVEL_GREATER_EQUAL_TARGET = 29,--指定等级大于等于目标[等级ID]
	LIMIT_TYPE_LEVEL_LESS_TARGET = 30,--指定等级小于等于目标[等级ID]
	LIMIT_TYPE_HAVE_QUEST_STATUS = 31,--拥有任务状态
	LIMIT_TYPE_NOT_HAVE_QUEST_STATUS = 32,--未拥有任务状态
	LIMIT_TYPE_TARGET_RES_NOT_FULL = 33,--目标资源未满[资源属性ID|]
	LIMIT_TYPE_IN_SCENE = 34,--指定场景[场景ID]
	LIMIT_TYPE_NOT_IN_SCENE = 35,--非指定场景[场景ID]
	LIMIT_TYPE_MAX = 36,

};




MMO.PASSIVE_EFFECT_TYPE=
{
	PASSIVE_EFFECT_TYPE_VIP_EXP_CHANGE = 0,--VIP经验加成[百分比变化|绝对值变化]
	PASSIVE_EFFECT_TYPE_PRICE_LIST_BUY_COUNT_CHANGE = 1,--价格列表购买次数变化[变化量|价格列表类型]
	PASSIVE_EFFECT_TYPE_CHAR_STATE_BIND = 2,--角色状态绑定[状态位<CharStatus>]
	PASSIVE_EFFECT_TYPE_CHANGE_SPELL_TIME = 3,--施法时间改变[变化百分比|变化绝对值|技能限定]
	PASSIVE_EFFECT_TYPE_CHANGE_CAST_DISTANCE = 4,--施法距离改变[变化百分比|变化绝对值|技能限定]
	PASSIVE_EFFECT_TYPE_CHANGE_COOL_DOWN_TIME = 5,--冷却时间改变[变化百分比|变化绝对值|CooldownID限定]
	PASSIVE_EFFECT_TYPE_CHANGE_ATTR = 6,--角色属性改变[属性类型<CharAttrType>|变化百分比|变化绝对值]
	PASSIVE_EFFECT_TYPE_CHANGE_ATTR_BY_ANOTHER_ATTR = 7,--根据一个角色属性改变另一个属性[参考属性类型<CharAttrType>|改变属性类型<CharAttrType>|变化百分比]
	PASSIVE_EFFECT_TYPE_CHANGE_DAMAGE = 8,--改变伤害值[变化百分比|变化绝对值|伤害类型<DamageType>|技能限定]
	PASSIVE_EFFECT_TYPE_CHANGE_FINAL_DAMAGE = 9,--改变最终伤害值[变化百分比|变化绝对值|伤害类型<DamageType>|技能限定]
	PASSIVE_EFFECT_TYPE_CHANGE_HURT = 10,--受到的伤害改变[变化百分比|变化绝对值|伤害类型<DamageType>|技能限定]
	PASSIVE_EFFECT_TYPE_CHANGE_CRIT_RATE = 11,--爆击率改变[变化绝对值|伤害类型|技能限定]
	PASSIVE_EFFECT_TYPE_CHANGE_BE_CRIT_RATE = 12,--被爆击率改变[变化绝对值|伤害类型|技能限定]
	PASSIVE_EFFECT_TYPE_CHANGE_CRIT_DAMAGE = 13,--爆击伤害率改变[变化绝对值|伤害类型|技能限定]
	PASSIVE_EFFECT_TYPE_CHANGE_HIT_RATE = 14,--命中率改变[变化绝对值|伤害类型|技能限定]
	PASSIVE_EFFECT_TYPE_CHANGE_DODGE_RATE = 15,--回避率改变[变化绝对值|伤害类型|技能限定]
	PASSIVE_EFFECT_TYPE_CHANGE_RES_COST = 16,--资源消耗改变[资源属性ID|变化百分比|变化绝对值|技能限定]
	PASSIVE_EFFECT_TYPE_CHANGE_EXP_RATE = 17,--经验倍率[变化百分比]
	PASSIVE_EFFECT_TYPE_CHANGE_DMG_ELEMENT_TYPE = 18,--改变伤害的元素属性[目标属性<DamageType>]
	PASSIVE_EFFECT_TYPE_AUTO_RESURRECTION = 19,--自动复活[发动率|复活技能ID|技能等级]
	PASSIVE_EFFECT_TYPE_AUTO_INTERCEPT_LONG_RANGE_ATTACKS = 20,--拦截远程攻击[拦截率]
	PASSIVE_EFFECT_TYPE_DMG_REFLEX = 21,--反射伤害[反射率|伤害类型|技能限定]
	PASSIVE_EFFECT_TYPE_CHANGE_RES_RESTORE_RATE = 22,--资源恢复率改变[资源属性ID|恢复千分比]
	PASSIVE_EFFECT_TYPE_MAX = 23,

};




MMO.SKILL_CONFIG_FLAG=
{
	SKILL_CONFIG_FLAG_HAVE_PASSIVE_EFFECT = 1,--含被动效果
	SKILL_CONFIG_FLAG_NORMAL_ATTACK_SKILL = (1<<1),--普通攻击
	SKILL_CONFIG_FLAG_CAN_CAST_ON_DEATH = (1<<2),--死亡时可施放
	SKILL_CONFIG_FLAG_MELLEL_ATTACK = (1<<3),--近战攻击
	SKILL_CONFIG_FLAG_CAN_CAST_IN_SPELL = (1<<4),--可在咏唱/引导时施放

};




MMO.SKILL_COOL_DOWN_INFO_FLAG=
{
	SKILL_COOL_DOWN_INFO_FLAG_USE_WEAPON_SPEED = 0,

};




MMO.SKILL_COST_TYPE=
{
	SKILL_COST_RES = 0,--消耗资源[资源属性ID|消耗量]
	SKILL_COST_BUFF = 1,--消耗BUFF[BuffID|消耗层数]
	SKILL_COST_TARGET_BUFF = 2,--消耗目标BUFF[BuffID|消耗层数]
	SKILL_COST_ITEM = 3,--消耗道具[道具类型ID|消耗数量]
	SKILL_COST_ITEM_DURABLE = 4,--消耗道具耐久[道具类型ID|消耗耐久值]
	SKILL_COST_RES_PERCENT = 5,--消耗资源百分比[资源属性ID|消耗百分比]
	SKILL_COST_PET = 6,
	SKILL_COST_MAX = 7,

};




MMO.SKILL_EFFECT_EXEC_TYPE=
{
	SKILL_EFFECT_EXEC_TYPE_AT_START = 0,
	SKILL_EFFECT_EXEC_TYPE_AT_END = 1,
	SKILL_EFFECT_EXEC_TYPE_REPEAT = 2,
	SKILL_EFFECT_EXEC_TYPE_MAX = 3,

};




MMO.SKILL_EFFECT_FLAG=
{
	SKILL_EFFECT_FLAG_TRIGGER_COST_BUFF = 1,
	SKILL_EFFECT_FLAG_TARGET_IS_CASTER = 1<<1,
	SKILL_EFFECT_FLAG_IGNORE_DEF = 1<<2,
	SKILL_EFFECT_FLAG_IGNORE_DODGE = 1<<3,
	SKILL_EFFECT_FLAG_IGNORE_CRIT = 1<<4,
	SKILL_EFFECT_FLAG_IGNORE_RES = 1<<5,
	SKILL_EFFECT_FLAG_TRIGGER_COST_ALL_BUFF = 1<<6,
	SKILL_EFFECT_FLAG_NOT_MUL_WITH_BUFF_COUNT = 1<<7,

};




MMO.SKILL_EFFECT_TYPE=
{
	SKILL_EFFECT_PASSIVE = 0,--被动效果[被动效果类型|效果参数....]
	SKILL_EFFECT_SCRIPT = 1,--脚本效果[类型|参数1|参数2...]
	SKILL_EFFECT_START_COOL_DOWN = 2,--开始冷却
	SKILL_EFFECT_RESET_COOL_DOWN_TIME = 3,--重置技能冷却[冷却ID]
	SKILL_EFFECT_ADD_BUFF = 4,--增加BUFF[BuffTypeID|层数|成功率]
	SKILL_EFFECT_DEL_BUFF = 5,--移除BUFF[BuffTypeID|层数]
	SKILL_EFFECT_TOGGLE_BUFF = 6,--切换Buff[BuffTypeID]
	SKILL_EFFECT_DISPERSE_BUFF = 7,--驱散Buff[目标Buff立场|驱散数量]
	SKILL_EFFECT_BREAK_SPELL = 8,--打断吟唱[触发冷却ID|冷却时间]
	SKILL_EFFECT_RESURRECTION = 9,--复活[参数1|参数2....]
	SKILL_EFFECT_USE_SKILL = 10,--使用技能[技能ID|技能等级]
	SKILL_EFFECT_TELEPORT = 11,--传送[场景ID|坐标X|坐标Y]
	SKILL_EFFECT_ADD_SHIELD = 12,--添加护盾[优先级|护盾值|护盾值成长|吸收伤害类型<DamageType>|吸收率|吸收率成长]
	SKILL_EFFECT_RES_CHANGE = 13,--伤害/治疗效果[ResID|伤害/治疗类型|附加参数1|附加参数2...]
	SKILL_EFFECT_MAX = 14,

};




MMO.SKILL_EVENT_BREAK_TYPE=
{
	SKILL_EVENT_BREAK_TYPE_BREAK_BY_MOVE = 1,
	SKILL_EVENT_BREAK_TYPE_BREAK_BY_DAMAGE = 1<<1,
	SKILL_EVENT_BREAK_TYPE_DISTURB_BY_DAMAGE = 1<<2,
	SKILL_EVENT_BREAK_TYPE_BREAK_BY_SKILL = 1<<3,

};




MMO.SKILL_EVENT_FLAG=
{
	SKILL_EVENT_FLAG_EVENT_NO_WAIT = 1,
	SKILL_EVENT_FLAG_NOT_SHOW_TIME_PROGRESS = 1<<1,
	SKILL_EVENT_FLAG_BREAK_EFFECT_ON_MISS = 1<<2,
	SKILL_EVENT_FLAG_NOT_BREAK_CAST_ON_ERROR = 1<<3,

};




MMO.SKILL_EVENT_TYPE=
{
	SKILL_EVENT_TYPE_IMMEDIATELY_EVENT = 0,
	SKILL_EVENT_TYPE_SPELL_EVENT = 1,
	SKILL_EVENT_TYPE_CHANNEL_EVENT = 2,
	SKILL_EVENT_TYPE_BULLET_FLY_EVENT = 3,
	SKILL_EVENT_TYPE_FIX_TIME_EVENT = 4,
	SKILL_EVENT_TYPE_SCRIPT = 5,
	SKILL_EVENT_TYPE_BUFF_START_EVENT = 6,
	SKILL_EVENT_TYPE_BUFF_KEEP_EVENT = 7,
	SKILL_EVENT_TYPE_BUFF_END_EVENT = 8,
	SKILL_EVENT_TYPE_MAX = 9,

};


--技能流程类型

MMO.SKILL_FLOW_TYPE=
{
	SKILL_FLOW_TYPE_NEXT_EVENT = 0,--执行下一个事件
	SKILL_FLOW_TYPE_GOTO_EVENT = 1,--执行指定事件[事件ID]
	SKILL_FLOW_TYPE_RANDOM_EVENT = 2,--随机执行事件[事件ID1|事件ID2...]
	SKILL_FLOW_TYPE_CONDITION_EVENT = 3,--随机执行事件[事件ID|LIMIT_CONFIG]

};




MMO.SKILL_PRODUCER_TYPE=
{
	SKILL_PRODUCER_SKILL = 0,
	SKILL_PRODUCER_BUFF = 1,
	SKILL_PRODUCER_MAX = 2,

};




MMO.SKILL_RANGE_TYPE=
{
	SKILL_RANGE_TYPE_NONE = 0,
	SKILL_RANGE_TYPE_SELF = 1,
	SKILL_RANGE_TYPE_ALONE = 2,--单体[施放距离]
	SKILL_RANGE_TYPE_SELF_CYCLE = 3,--自身圆形[半径|最大目标数]
	SKILL_RANGE_TYPE_TARGET_CYCLE = 4,--目标圆形[半径|最大目标数|施放距离]
	SKILL_RANGE_TYPE_POS_CYCLE = 5,--坐标点圆形[半径|最大目标数|施放距离]
	SKILL_RANGE_TYPE_CHAIN = 6,--链条[最大链接距离|最大目标数|施放距离]
	SKILL_RANGE_TYPE_MAX = 7,

};




MMO.SKILL_STAND_TYPE=
{
	SKILL_STAND_TYPE_NEUTRAL = 0,
	SKILL_STAND_TYPE_GAIN = 1,
	SKILL_STAND_TYPE_DEBUFF = 2,
	SKILL_STAND_TYPE_MAX = 3,

};




MMO.SKILL_TARGET_DISTANCE_TYPE=
{
	SKILL_TARGET_DISTANCE_NEAR = 0,
	SKILL_TARGET_DISTANCE_FAR = 1,
	SKILL_TARGET_DISTANCE_MAX = 2,

};




MMO.SKILL_TARGET_TYPE=
{
	SKILL_TARGET_FRIEND = 1,
	SKILL_TARGET_ENEMY = (1<<1),
	SKILL_TARGET_CORPSE = (1<<2),
	SKILL_TARGET_ALIVE = (1<<3),

};


--技能触发器类型

MMO.SKILL_TRIGGER_TYPE=
{
	SKILL_TRIGGER_TYPE_NONE = 0,--无
	SKILL_TRIGGER_TYPE_ON_DAMAGE = 1,--造成伤害/治疗时触发[触发概率|技能限定|伤害类型掩码<DamageType>]
	SKILL_TRIGGER_TYPE_ON_HURT = 2,--受到伤害/治疗时触发[触发概率|技能限定|伤害类型掩码<DamageType>]
	SKILL_TRIGGER_TYPE_ON_USE_SKILL = 3,--使用技能时触发[触发概率|技能限定]
	SKILL_TRIGGER_TYPE_ON_DEATH = 4,--死亡时触发[触发概率]
	SKILL_TRIGGER_TYPE_MAX = 5,

};




MMO.SKILL_TYPE=
{
	SKILL_TYPE_PASSIVE = 0,
	SKILL_TYPE_SKILL = 1,
	SKILL_TYPE_MAGIC = 2,
	SKILL_TYPE_MAX = 3,

};




MMO.SKILL_VISUAL_EFFECT_FLAG=
{
	SKILL_VISUAL_EFFECT_FLAG_EFFECT_IS_LOOP = 1,
	SKILL_VISUAL_EFFECT_FLAG_ADJUST_PLAY_TIME_WITH_EVENT_TIME = 1<<1,
	SKILL_VISUAL_EFFECT_FLAG_GROUND_EFFECT = 1<<2,
	SKILL_VISUAL_EFFECT_FLAG_NOT_BIND_PARENT = 1<<3,

};




MMO.SKILL_VISUAL_EFFECT_PARENT_TYPE=
{
	SKILL_VISUAL_EFFECT_PARENT_TYPE_CASTER = 0,
	SKILL_VISUAL_EFFECT_PARENT_TYPE_TARGET = 1,
	SKILL_VISUAL_EFFECT_PARENT_TYPE_POS = 2,
	SKILL_VISUAL_EFFECT_PARENT_TYPE_MAX = 3,

};




MMO.SKILL_VISUAL_EFFECT_TYPE=
{
	SKILL_VISUAL_EFFECT_TYPE_EFFECT = 0,--特效[特效名|缩放比例|播放时长(秒)|播放速度|绑定骨骼名]
	SKILL_VISUAL_EFFECT_TYPE_ACTION = 1,--动作[动作名|播放时长(秒)|播放速度]
	SKILL_VISUAL_EFFECT_TYPE_SOUND = 2,--音效[音效名|播放时长(秒)|播放速度]
	SKILL_VISUAL_EFFECT_TYPE_ON_RIDE = 3,--乘骑效果[坐骑名]
	SKILL_VISUAL_EFFECT_TYPE_CHAIN_EFFECT = 4,--连接特效[特效名|缩放比例|播放时长(秒)|播放速度|绑定骨骼名|目标骨骼名|连接方式<CHAIN_EFFECT_LINK_TYPE>]
	SKILL_VISUAL_EFFECT_TYPE_BULLET_EFFECT = 5,--子弹特效[特效名|缩放比例|播放速度|绑定骨骼名|目标骨骼名]
	SKILL_VISUAL_EFFECT_TYPE_MAX = 6,

};




MMO.SPELL_BREAK_TYPE=
{
	SPELL_BREAK_TYPE_DAMAGE = 0,
	SPELL_BREAK_TYPE_CRIT = 1,
	SPELL_BREAK_TYPE_MOVE = 2,
	SPELL_BREAK_TYPE_DEATH = 3,
	SPELL_BREAK_TYPE_SKILL = 4,
	SPELL_BREAK_TYPE_CAST_ERROR = 5,

};




MMO.VALUE_CHANGE_TYPE=
{
	VALUE_CHANGE_TYPE_VALUE = 0,
	VALUE_CHANGE_TYPE_MAX_PROPORTION = 1,
	VALUE_CHANGE_TYPE_LEFT_PROPORTION = 2,
	VALUE_CHANGE_TYPE_LOST_PROPORTION = 3,
	VALUE_CHANGE_TYPE_MAX = 4,

};




