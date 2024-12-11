class flabby_safe_zone_command : ScrServerCommand
{
	// Specify keyword of command
	//-----------------------------------------------------------------------------
	override string GetKeyword()
	{
		return "sz";
	}
	
	// Run command server-side
	//-----------------------------------------------------------------------------
	override bool IsServerSide()
	{
		return true;
	}
	
	// Set requirement to admin permission via RCON
	//-----------------------------------------------------------------------------
	override int RequiredRCONPermission()
	{
		return ERCONPermissions.PERMISSIONS_ADMIN;
	}
	
	// Set requirement to be logged in administrator for chat command
	//-----------------------------------------------------------------------------
	override int RequiredChatPermission()
	{
		return EPlayerRole.ADMINISTRATOR;
	}
	
	// Handle Chat command on server
	//-----------------------------------------------------------------------------
	override ref ScrServerCmdResult OnChatServerExecution(array<string> argv, int playerId)
	{
		if (flabby_safeZones.DEBUG_LOGS)
			Print(string.Format("DEBUG_LOG: flabby_safe_zone_command.OnChatServerExecution(%1, %2)", argv.ToString(), playerId));
		
		if (argv.Count() > 1)
		{
			// List functions
			if (argv[1] == "list" && argv[2] == "session")
			{
				ref array<ref array<string>> safeZones = flabby_safeZones.getAllSafeZonesInSession();
				string returnString = string.Empty;
				foreach (array<string> safeZone : safeZones)
				{
					returnString += string.Format("SafeZone ID: %1, X: %2, Y: %2\n", safeZone.Get(0), safeZone.Get(1), safeZone.Get(2));
				}
				
				return ScrServerCmdResult(returnString, EServerCmdResultType.OK);
			}
			if (argv[1] == "list" && argv[2] == "persistent")
			{
				array<ref array<string>> safeZones = flabby_safeZones.getAllSafeZonesInFile();
				string returnString = string.Empty;
				foreach (array<string> safeZone : safeZones)
				{
					returnString += string.Format("SafeZone ID: %1, X: %2, Y: %2\n", safeZone.Get(0), safeZone.Get(1), safeZone.Get(2));
				}
				return ScrServerCmdResult(returnString, EServerCmdResultType.OK);
			}
			if (argv[1] == "make" && argv[2] && argv[3])
			{
				return ScrServerCmdResult(string.Format(
					"Made new safe zone with the non-persistent ID = %1",
					flabby_safeZones.createSafeZone(String(argv[2]).ToInt(), String(argv[3]).ToInt())
				), EServerCmdResultType.OK);
			}
			// #sz persists SAFE-ZONE-ID or #sz persists all
			if (argv[1] == "persists" && argv[2])
			{
				string arg2 = argv[2];
				arg2.ToUpper();
				if (arg2 == "ALL")
				{
				
				}
				return ScrServerCmdResult(string.Format(
					"Made new safe zone with the non-persistent ID = %1",
					flabby_safeZones.createSafeZone(String(argv[2]).ToInt(), String(argv[3]).ToInt())
				), EServerCmdResultType.OK);
			}
			
			if (argv[1] == "test")
			{
				return ScrServerCmdResult(flabby_safeZones_storage.saveALLSessionSafeZones(), EServerCmdResultType.OK);
			}
			
			// helps
			if (argv[1] == "help")
			{
				return flabby_safeZones.getHelpMessage();
			}
		}
		
		return ScrServerCmdResult(string.Empty, EServerCmdResultType.PARAMETERS);
	}
	
	// Handle Chat command on client
	//-----------------------------------------------------------------------------
	override ref ScrServerCmdResult OnChatClientExecution(array<string> argv, int playerId)
	{
		return ScrServerCmdResult(string.Empty, EServerCmdResultType.OK);
	}
	
	// Handle RCON command on server
	//-----------------------------------------------------------------------------
	override ref ScrServerCmdResult OnRCONExecution(array<string> argv)
	{
		return ScrServerCmdResult(string.Empty, EServerCmdResultType.OK);
	}
	
	// Handle Pending command
	//-----------------------------------------------------------------------------
	override ref ScrServerCmdResult OnUpdate()
	{
		return ScrServerCmdResult(string.Empty, EServerCmdResultType.OK);
	}

}