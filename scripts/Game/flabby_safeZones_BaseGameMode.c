modded class SCR_BaseGameMode
{
	[RplProp(onRplName: "onReplicatedSafeZonesUpdate")]
	ref array<string> flabby_safeZonesGM = new array<string>();
	
	protected void onReplicatedSafeZonesUpdate()
	{
		if (flabby_safeZones.DEBUG_LOGS)
			Print(string.Format("DEBUG_LOG: SCR_BaseGameMode.onReplicatedSafeZonesUpdate(), flabby_safeZonesGM.ToString() = %1", flabby_safeZonesGM.ToString()));
		
		SCR_PlayerController pc = SCR_PlayerController.Cast(GetGame().GetPlayerController());
		if (!pc)
		{
			return;
		}
		
		SCR_ChatComponent cc = SCR_ChatComponent.Cast(pc.FindComponent(SCR_ChatComponent));
		if (!cc)
		{
			return;
		}

		cc.ShowMessage(string.Format("RplProp variable 'flabby_safeZonesGM' has been updated, %1", flabby_safeZonesGM.ToString()));
	}
	
	void flabby_updateReplicatedSafeZones()
	{
		if (flabby_safeZones.DEBUG_LOGS)
			Print("DEBUG_LOG: SCR_BaseGameMode.flabby_updateReplicatedSafeZones()");
		
		Rpc(RpcAskForSafeZones);
	}
	
	[RplRpc(RplChannel.Reliable, RplRcver.Server)]
	protected void RpcAskForSafeZones()
	{
		if (flabby_safeZones.DEBUG_LOGS)
			Print("DEBUG_LOG: SCR_BaseGameMode.RpcAskForSafeZones()");
		
		ref array<ref array<string>> replicatedSafeZones = flabby_safeZones.getAllSafeZonesInSession();
		foreach(ref array<string> safeZone : replicatedSafeZones)
		{
			string returnString = string.Empty;
			for (int i; i < safeZone.Count(); i++)
			{
				returnString += string.Format("%1;", safeZone.Get(i));
			}
			
			if (!flabby_safeZonesGM.Contains(returnString))
			{
				flabby_safeZonesGM.Insert(returnString);
			}
		}
		Replication.BumpMe();
	}
	
	static ref flabby_safeZones_storage flabby_safeZoneStorage;
	override void EOnInit(IEntity owner)
	{
		super.EOnInit(owner);
		
		if (flabby_safeZones.DEBUG_LOGS)
			Print(string.Format("DEBUG_LOG: SCR_BaseGameMode.EOnInit(), flabby_safeZoneStorage = %1", flabby_safeZoneStorage));
		
		if (!flabby_safeZoneStorage)
		{
			flabby_safeZoneStorage = new flabby_safeZones_storage();
		}
		
		// Set up persistent safe zones 
	}
}