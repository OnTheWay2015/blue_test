#pragma once

class CPlayerDBAdapterGroup;
class CPlayerDBAdapter :
	public IDistributedObject,
	public CNameObject,
	public CPlayerDBAdapterMsgHandler,
	public CGMCommandMsgHandler
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
	CPlayerDBAdapterGroup *					m_pGroup;
	IDistributedObjectOperator *			m_pOperator;
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

	CESVariableList							m_ESVariableList;
	CESFunctionList							m_ESFunctionList;
	CESBolanStack							m_Script;
	CESThread								m_ESThread;
	CEasyScriptExecutor						m_ScriptExecutor;
	OBJECT_ID								m_RecentGMCmdCaller;
	OBJECT_ID								m_RecentGMCmdCallerParam;

protected:
	DECLARE_DOS_MSG_MAP(CPlayerDBAdapter);
public:
	CPlayerDBAdapter(const INSTANCE_CONFIG& InstanceConfig, CPlayerDBAdapterGroup * pGroup, IDatabase * pDatabase);
	~CPlayerDBAdapter(void);

	virtual BOOL Initialize(IDistributedObjectOperator * pOperator) override;
	virtual void Destory() override;
	virtual UINT AddUseRef() override;
	virtual void Release() override;

protected:
	virtual int Update(int ProcessPacketLimit) override;
	virtual void OnShutDown(BYTE Level, UINT Param) override;
	virtual BOOL OnFindObject(OBJECT_ID CallerID) override;


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
	bool UnpackPlayerInfo(const CSmartStruct& Packet,CPlayerInfo& PlayerInfo,bool IsCreate);
	CSmartStruct PackRoleList(DB_PLAYER_INFO* pPlayerInfo);

	
	//void DoAddOfflinePVPRecord(DB_PLAYER_INFO * pPlayerInfo, const CSmartStruct& BattleRecordPacket);
protected:
	int ESFClearPlayerDataCache(CESThread * pESThread, ES_BOLAN* pResult, ES_BOLAN* pParams, int ParamCount);
protected:
	virtual int DBCreatePlayer(UINT64 AccountID, const CSmartStruct& PlayerCreateInfo, const ACCOUNT_STATUS_INFO& AccountStatusInfo) override;
	virtual int DBDeletePlayer(UINT64 AccountID) override;
	virtual int DBGetPlayerInfo(UINT64 AccountID, BYTE QueryType, const CSmartStruct& QueryParam) override;
	virtual int DBSavePlayerInfo(UINT64 AccountID, const CSmartStruct& PlayerInfo) override;
	virtual int DBClearDataCache(UINT64 AccountID, OBJECT_ID CallerID) override;
	virtual int DBPlayerSearch(const CEasyString& NickName, OBJECT_ID CallerID) override;
	virtual int DBSerachPlayerByTID(const CEasyString& TID, OBJECT_ID CallerID) override;
	virtual int DBGetPlayerCount(OBJECT_ID CallerID) override;
	virtual int DBGetPlayerIDByTID(const CEasyString& TID, const CSmartStruct& Param) override;
	virtual int DBGetRoleList(UINT64 PlayerID) override;
	virtual int DBCreateRole(UINT64 PlayerID, const CSmartStruct& RoleInfo) override;
	virtual int DBRemoveRole(UINT64 PlayerID, UINT64 RoleID) override;
	virtual int DBChangeRoleName(UINT64 PlayerID, UINT64 RoleID, const CEasyString& RoleName) override;
	virtual int DBGetRoleInfo(UINT64 PlayerID, UINT64 RoleID) override;
	virtual int DBSaveRole(UINT64 PlayerID, const CSmartStruct& RoleInfo) override;
protected:
	virtual int ExecCommand(OBJECT_ID PlayerProxyObjectID, const CEasyString& CmdStr) override;
public:	
	void OnDBRetGetPlayerInfo(int Result, OBJECT_ID CallerID, BYTE QueryType, CSmartStruct& QueryParam, UINT64 AccountID, CSmartStruct& PlayerInfo);
	void OnDBRetCreatePlayer(int Result, OBJECT_ID CallerID, UINT64 AccountID, CSmartStruct& PlayerInfo);
	void OnDBRetDeletePlayer(bool IsSucceed, OBJECT_ID CallerID, UINT64 PlayerID);	
	void OnDBRetSavePlayer(bool IsSucceed, OBJECT_ID CallerID, UINT64 AccountID);	
	void OnDBRetPlayerSearch(int Result, OBJECT_ID CallerID, OBJECT_ID Param, CSmartStruct& DataPacket);
	void OnDBRetSearchPlayerByTID(int Result, OBJECT_ID CallerID, OBJECT_ID Param, CSmartStruct& DataPacket);
	void OnDBRetPlayerSearch(int Result, OBJECT_ID CallerID, OBJECT_ID Param, UINT Count);
	void OnDBRetGetIDByTID(int Result, OBJECT_ID CallerID, UINT64 PlayerID, const CSmartStruct& Param);

	void OnDBRetGetRoleIDSeed(UINT64 IDSeed);
	void OnDBRetLoadRoleList(int Result, OBJECT_ID CallerID, UINT64 AccountID, CEasyArray<CSmartStruct>& RoleList);
	void OnDBRetCreateRole(int Result, OBJECT_ID CallerID, UINT64 AccountID, UINT64 RoleID, CSmartStruct& RoleInfo);
	void OnDBRetRemoveRole(bool IsSucceed, OBJECT_ID CallerID, UINT64 AccountID, UINT64 RoleID);
	void OnDBRetSaveRole(bool IsSucceed, OBJECT_ID CallerID, UINT64 RoleID);
	void OnDBRetChangeRoleName(int Result, OBJECT_ID CallerID, UINT64 RoleID, LPCTSTR RoleName);
};

