[EntityEditorProps(category: "GameScripted/GameMode", description: "Editable safe zone", visible: false)]
class flabby_safeZonesPrefabClass : SCR_PositionClass {}

class flabby_safeZonesPrefab : SCR_Position
{
	protected RplComponent m_RplComponent;

	[Attribute("0", desc: "Find empty position for spawning within given radius. When none is found, entity position will be used.")]
	protected float m_fSpawnRadius;

	[Attribute("Red", UIWidgets.EditBox, "Determines which faction can spawn on this spawn point."), RplProp(onRplName: "OnSetFactionKey")]
	protected string m_sFaction;

	[Attribute("0")]
	protected bool m_bShowInDeployMapOnly;

	[Attribute("0", desc: "Use custom timer when deploying on this spawn point. Takes the remaining respawn time from SCR_TimedSpawnPointComponent")]
	protected bool m_bTimedSpawnPoint;

	protected SCR_UIInfo m_LinkedInfo;
	protected SCR_FactionAffiliationComponent m_FactionAffiliationComponent;

	[Attribute()]
	protected ref SCR_UIInfo m_Info;

	[Attribute("0", desc: "Allow usage of Spawn Positions in range")]
	protected bool m_bUseNearbySpawnPositions;

	[Attribute("100", desc: "Spawn position detection radius, in metres")]
	protected float m_fSpawnPositionUsageRange;

	[Attribute("0", desc: "Additional respawn time (in seconds) when spawning on this spawn point"), RplProp()]
	protected float m_fRespawnTime;

	// List of all spawn points
	private static ref array<SCR_SpawnPoint> m_aSpawnPoints = new array<SCR_SpawnPoint>();

	static ref ScriptInvoker Event_OnSpawnPointCountChanged = new ScriptInvoker();
	static ref ScriptInvoker Event_SpawnPointFactionAssigned = new ScriptInvoker();
	static ref SpawnPointInvoker Event_SpawnPointAdded = new SpawnPointInvoker();
	static ref SpawnPointInvoker Event_SpawnPointRemoved = new SpawnPointInvoker();
	static ref SCR_SpawnPointFinalizeSpawn_Invoker s_OnSpawnPointFinalizeSpawn;

	[RplProp()]
	protected string m_sSpawnPointName;
	
	static ref SCR_SpawnPointNameChanged_Invoker OnSpawnPointNameChanged = new SCR_SpawnPointNameChanged_Invoker();
	
	// spawn point will work as a spawn point group if it has any SCR_Position as its children
	protected ref array<SCR_Position> m_aChildren = {};

	/*!
		Authority:
			Set of all pending players that have a reservation for this spawn point.
	*/
	protected ref set<int> m_ReservationLocks = new set<int>();
	
	protected ref ScriptInvokerBool  m_OnSetSpawnPointEnabled;
	
	[Attribute("1"), RplProp(onRplName: "OnSetEnabled")]
	protected bool m_bSpawnPointEnabled;
	
	void OnSetFactionKey();
	
	#ifdef WORKBENCH
	//------------------------------------------------------------------------------------------------
	override void SetColorAndText()
	{
		m_sText = m_sFaction;
		m_iColor = Color.Pink.PackToInt();
	}
	#endif
	
	protected bool GetEmptyPositionAndRotationInRange(out vector pos, out vector rot)
	{
		SCR_SpawnPositionComponentManager spawnPosManagerComponent = SCR_SpawnPositionComponentManager.GetInstance();
		if (!spawnPosManagerComponent)
			return false;

		array<SCR_SpawnPositionComponent> positions = {};
		int count = spawnPosManagerComponent.GetSpawnPositionsInRange(GetOrigin(), m_fSpawnPositionUsageRange, positions);
		if (count < 0)
			return false;

		SCR_SpawnPositionComponent position;
		
		while (!positions.IsEmpty())
		{
			position = positions.GetRandomElement();

			if (position.IsFree())
			{
				pos = position.GetOwner().GetOrigin();
				rot = position.GetOwner().GetAngles();
				return true;
			}
			else
			{
				positions.RemoveItem(position);
			}
		}

		return false;
	}
	
	void GetPositionAndRotation(out vector pos, out vector rot)
	{
		if (m_bUseNearbySpawnPositions)
		{
			if (GetEmptyPositionAndRotationInRange(pos, rot))
				return;
		}

		if (m_aChildren.Count() > 1)
		{
			int id = m_aChildren.GetRandomIndex();
			SCR_WorldTools.FindEmptyTerrainPosition(pos, m_aChildren[id].GetOrigin(), GetSpawnRadius());
			rot = m_aChildren[id].GetAngles();
		}
		else
		{
			SCR_WorldTools.FindEmptyTerrainPosition(pos, GetOrigin(), GetSpawnRadius());
			rot = GetAngles();
		}
	}
	float GetSpawnRadius()
	{
		return m_fSpawnRadius;
	}
	void SetSpawnRadius(float radius)
	{
		m_fSpawnRadius = radius;
	}
	protected void OnSetEnabled()
	{
		if (m_OnSetSpawnPointEnabled)
			m_OnSetSpawnPointEnabled.Invoke(m_bSpawnPointEnabled);
	}
	bool PrepareSpawnedEntity_S(SCR_SpawnRequestComponent requestComponent, SCR_SpawnData data, IEntity entity)
	{
		
		// WS target position, pitch yaw roll angles in degrees
		vector position, rotation;
		GetPositionAndRotation(position, rotation);

		// apply transformation
		entity.SetOrigin(position);
		entity.SetAngles(rotation);
		return true;
	}
	
//	override void EOnInit(IEntity owner)
//	{
//		super.EOnInit(owner);
////		// WS target position, pitch yaw roll angles in degrees
////		vector position, rotation;
////		GetPositionAndRotation(position, rotation);
////
////		// apply transformation
////		owner.SetOrigin(position);
////		owner.SetAngles(rotation);
//	}
}