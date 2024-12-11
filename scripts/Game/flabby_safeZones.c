#ifdef WORKBENCH
#define DEBUG_FLABBY
#endif

class flabby_safeZones
{
	#ifdef DEBUG_FLABBY
	static const bool DEBUG_LOGS = true;
	#else
	static const bool DEBUG_LOGS = false;
	#endif
	
	//! Add safe zone with damage disabled
	static string createSafeZone(int length, int width)
	{
		return flabby_safeZones_storage.storeNewSafeZone(length, width);
	}
	//! Remove safe zone restoring damage
	static string removeSafeZoneSession(string safeZoneIdentifier)
	{
		return flabby_safeZones_storage.removeSafeZone(safeZoneIdentifier, false);
	}
	//! Presist a ALL session safe zone & Returns how many just persisted and total
	static string persistAllSafeZones()
	{
		return flabby_safeZones_storage.saveALLSessionSafeZones(); 
	}
	//! Return all safe zones in file storage
	static ref array<ref array<string>> getAllSafeZonesInFile()
	{
		return SCR_BaseGameMode.flabby_safeZoneStorage.getSafeZonesPersistent();
	}
	//! Return all safe zones in session storage
	static ref array<ref array<string>> getAllSafeZonesInSession()
	{
		return SCR_BaseGameMode.flabby_safeZoneStorage.getSafeZones();
	}
	static ScrServerCmdResult getHelpMessage()
	{
		string helpString = "Below is a list of all commands,\n";
		helpString += "#sz make [length] [width]\n"; 
		helpString += "#sz persists [Safe-zone-identifier]\n";
		helpString += "#sz removes [Safe-zone-identifier]\n";
		helpString += "#sz alert [Safe-zone-identifier]\n";
		helpString += "#sz resizes [Safe-zone-identifier] [length] [width]\n";
		helpString += "#sz list session\n";;
		helpString += "#sz list persistent\n";
		helpString += "#safezone help";
		return ScrServerCmdResult(helpString, EServerCmdResultType.OK);
	}
	//! Add enter and exit messages to safe zone. Return bool if on or not
	static bool updateMessagesSafeZone(int safeZoneIdentifier);
	//! Change the size/area the safe zone effects
	static void resizeSafeZone(int safeZoneIdentifier, int length, int width);
	//! Presist an already existing safe zone
	static void persistSafeZone(int safeZoneIdentifier);
}
