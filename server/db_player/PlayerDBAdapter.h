#pragma once

struct DB_PLAYER_INFO
{
	UINT64				AccountID;
	int					Status;
	CEasyTimer			StatusTimer;
	CSmartStruct		PlayerInfo;
	CEasyArray<UINT64>	RoleList;
	bool				IsRoleFetched;
	DB_PLAYER_INFO()
	{
		Clear();
	}
	void Clear()
	{
		AccountID=0;
		Status=0;
		PlayerInfo.Clear();
		RoleList.clear();
		IsRoleFetched = false;
	}
};

struct DB_ROLE_INFO
{
	UINT64				AccountID;
	UINT64				RoleID;
	int					Status;
	CEasyTimer			StatusTimer;
	CSmartStruct		RoleInfo;
	DB_ROLE_INFO()
	{
		Clear();
	}
	void Clear()
	{
		AccountID = 0;
		RoleID = 0;
		Status = 0;
		RoleInfo.Clear();
	}
};


class CPlayerDBAdapter 
{
protected:

	enum STATUS_TYPE
	{
		STATUS_NONE,
		STATUS_WORKING,
		STATUS_SHUTDOWN,
	};

	int										m_Status;

	INSTANCE_CONFIG							m_InstanceConfig;
	IDatabase *								m_pDatabase;
	CDBTransationManager *					m_pDBTransManager;

	CStaticMap<UINT64, DB_PLAYER_INFO>		m_PlayerInfoPool;
	CStaticMap<UINT64, DB_ROLE_INFO>		m_RoleInfoPool;

	CEasyTimer								m_PlayerSaveTimer;
	UINT									m_PlayerSavePtr;

	CEasyTimer								m_RoleSaveTimer;
	UINT									m_RoleSavePtr;
	UINT64									m_RoleIDSeed;
	UINT									m_TotalDataCount;

	CEasyTimer								m_DataClearTimer;
	UINT									m_DataClearPtr;

	CSmartStruct							m_PacketBuffer1;
	CSmartStruct							m_PacketBuffer2;


public:
	CPlayerDBAdapter(const INSTANCE_CONFIG& InstanceConfig,IDatabase * pDatabase);
	~CPlayerDBAdapter(void);

	virtual BOOL Initialize() ;
	virtual void Destory() ;
	virtual UINT AddUseRef() ;
	virtual void Release() ;

protected:
	virtual int Update(int ProcessPacketLimit) ;
	virtual void OnShutDown(BYTE Level, UINT Param) ;
	//virtual BOOL OnFindObject(OBJECT_ID CallerID) ;
	virtual BOOL OnFindObject(UINT64 CallerID) ;


	DB_PLAYER_INFO * FindPlayerInfo(UINT64 AccountID);
	DB_PLAYER_INFO * CreatePlayerInfo(UINT64 AccountID);
	bool DeletePlayerInfo(UINT64 AccountID);
	bool ClearPlayerInfo(UINT64 AccountID);
	void ClearIdlePlayerInfo();
	bool PlayerInfoCanClear(DB_PLAYER_INFO* pPlayerInfo);

	DB_ROLE_INFO* FindRoleInfo(UINT64 RoleID);
	DB_ROLE_INFO* CreateRoleInfo(UINT64 RoleID);
	bool DeleteRoleInfo(UINT64 RoleID);
	

	CSmartStruct PackPlayerInfo(DB_PLAYER_INFO * pPlayerInfo, BYTE QueryType);
	//bool UnpackPlayerInfo(const CSmartStruct& Packet,CPlayerInfo& PlayerInfo,bool IsCreate); //todo  PlayerInfo is message protocol struct
	CSmartStruct PackRoleList(DB_PLAYER_INFO* pPlayerInfo);

	
protected:
	//virtual int DBCreatePlayer(UINT64 AccountID, const CSmartStruct& PlayerCreateInfo, const ACCOUNT_STATUS_INFO& AccountStatusInfo) ; //todo
	virtual int DBDeletePlayer(UINT64 AccountID) ;
	virtual int DBGetPlayerInfo(UINT64 AccountID, BYTE QueryType, const CSmartStruct& QueryParam) ;
	virtual int DBSavePlayerInfo(UINT64 AccountID, const CSmartStruct& PlayerInfo) ;
	virtual int DBClearDataCache(UINT64 AccountID, UINT64 CallerID) ;
	virtual int DBPlayerSearch(const CEasyString& NickName, UINT64 CallerID) ;
	virtual int DBSerachPlayerByTID(const CEasyString& TID, UINT64 CallerID) ;
	virtual int DBGetPlayerCount(UINT64 CallerID) ;
	virtual int DBGetPlayerIDByTID(const CEasyString& TID, const CSmartStruct& Param) ;
	virtual int DBGetRoleList(UINT64 PlayerID) ;
	virtual int DBCreateRole(UINT64 PlayerID, const CSmartStruct& RoleInfo) ;
	virtual int DBRemoveRole(UINT64 PlayerID, UINT64 RoleID) ;
	virtual int DBChangeRoleName(UINT64 PlayerID, UINT64 RoleID, const CEasyString& RoleName) ;
	virtual int DBGetRoleInfo(UINT64 PlayerID, UINT64 RoleID) ;
	virtual int DBSaveRole(UINT64 PlayerID, const CSmartStruct& RoleInfo) ;
protected:
	virtual int ExecCommand(UINT64 PlayerProxyObjectID, const CEasyString& CmdStr) ;
public:	
	void OnDBRetGetPlayerInfo(int Result, UINT64 CallerID, BYTE QueryType, CSmartStruct& QueryParam, UINT64 AccountID, CSmartStruct& PlayerInfo);
	void OnDBRetCreatePlayer(int Result, UINT64 CallerID, UINT64 AccountID, CSmartStruct& PlayerInfo);
	void OnDBRetDeletePlayer(bool IsSucceed, UINT64 CallerID, UINT64 PlayerID);	
	void OnDBRetSavePlayer(bool IsSucceed, UINT64 CallerID, UINT64 AccountID);	
	void OnDBRetPlayerSearch(int Result, UINT64 CallerID, UINT64 Param, CSmartStruct& DataPacket);
	void OnDBRetSearchPlayerByTID(int Result, UINT64 CallerID, UINT64 Param, CSmartStruct& DataPacket);
	void OnDBRetPlayerSearch(int Result, UINT64 CallerID, UINT64 Param, UINT Count);
	void OnDBRetGetIDByTID(int Result, UINT64 CallerID, UINT64 PlayerID, const CSmartStruct& Param);

	void OnDBRetGetRoleIDSeed(UINT64 IDSeed);
	void OnDBRetLoadRoleList(int Result, UINT64 CallerID, UINT64 AccountID, CEasyArray<CSmartStruct>& RoleList);
	void OnDBRetCreateRole(int Result, UINT64 CallerID, UINT64 AccountID, UINT64 RoleID, CSmartStruct& RoleInfo);
	void OnDBRetRemoveRole(bool IsSucceed, UINT64 CallerID, UINT64 AccountID, UINT64 RoleID);
	void OnDBRetSaveRole(bool IsSucceed, UINT64 CallerID, UINT64 RoleID);
	void OnDBRetChangeRoleName(int Result, UINT64 CallerID, UINT64 RoleID, LPCTSTR RoleName);
};

