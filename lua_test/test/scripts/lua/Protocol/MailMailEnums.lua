﻿if MMO==nil then
	MMO={};
end



MMO.MAIL_FLAG=
{
	MAIL_FLAG_READED = 1,
	MAIL_FLAG_HAVE_ATTACHMENT = (1<<1),
	MAIL_FLAG_IS_SYSTEM_MAIL = (1<<2),
	MAIL_FLAG_TEXT_NEED_TRANSLATE = (1<<3),
	MAIL_FLAG_IS_DETAIL_FETCHED = (1<<4),

};




MMO.MAIL_ID_SUB_TYPE=
{
	MAIL_ID_SUB_TYPE_BROADCAST = 1,
	MAIL_ID_SUB_TYPE_NORMAL = 20,

};




MMO.MAIL_SYSTEM_RESULT=
{
	MSG_MSR_SUCCEED = 0,
	MSG_MSR_ADDRESEE_NOT_EXIST = 1,
	MSG_MSR_ADDRESEE_MAIL_BOX_FULL = 2,
	MSG_MSR_ATTACHMENT_NOT_EXIST = 3,
	MSG_MSR_NOT_LOGIN = 4,
	MSG_MSR_PICK_BY_PAY_MUST_PICK_ALL = 5,
	MSG_MSR_MAIL_CAN_NOT_BE_DECLINE = 6,
	MSG_MSR_BAG_FULL = 7,
	MSG_MSR_NOT_ENOUGH_ITEM = 8,
	MSG_MSR_SYSTEM_BUSY = 9,

};





