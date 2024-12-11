class flabby_safeZones_storage
{
	//! Location of persisted file
	protected static string persistedFileLocation = "$profile:/flabby/safe-zones.json";
	
	//! Memory/Session storage for safe zones 
	protected ref array<ref array<string>> safeZones;
	
	//! Get session/memeory safeZones storage variable
	ref array<ref array<string>> getSafeZones()
	{
		return safeZones;
	}
	
	//! Construct session safe zones 
	void flabby_safeZones_storage()
	{
		safeZones = new array<ref array<string>>();
	}
	
	//! Get safe zones from storage 
	static ref array<ref array<string>> getSafeZonesPersistent()
	{
		// Check if file is made
		bool isFile = FileIO.FileExists(persistedFileLocation);
		if (isFile)
		{
			// Open file
			SCR_JsonLoadContext jsonLoader = new SCR_JsonLoadContext();
			jsonLoader.LoadFromFile(persistedFileLocation);
			
			// Read safe_zones array 
			ref array<ref array<string>> safeZonesArray = new array<ref array<string>>();
			jsonLoader.ReadValue("safe-zone-list", safeZonesArray);
			// Return value
			return safeZonesArray;
		}
		else
		{
			return new array<ref array<string>>();
		}
	}
	
	static string generateIdentifier(int length_)
	{
		// Define characters allowed in the session ID
		string characters = "ABCDEFGHIJKLMNOPQRSTUVWXYZ0123456789";
		string sessionId = "";
		
		// Define the length of the session ID
		int length = length_;
		
		// Generate a random session ID of the specified length
		for (int i = 0; i < length; i++)
		{
			RandomGenerator randomGenerator = new RandomGenerator();
			int randomIndex = randomGenerator.RandInt(0, length -1);
			sessionId += characters[randomIndex];
		}
		
		return sessionId;
	}
	
	// ! Store all session safe zones to file.
	static string saveALLSessionSafeZones()
	{
		// Get game mode class
		SCR_BaseGameMode gm = SCR_BaseGameMode.Cast(GetGame().GetGameMode());
		if (!gm)
		{
			return "Error, can not access SCR_BaseGameMode";
		}
		
		//Get session save zones 
		ref array<ref array<string>> sessionSZ = gm.flabby_safeZoneStorage.safeZones;
		// Gwt file save zones 
		ref array<ref array<string>> fileSZ = getSafeZonesPersistent();
		
		// Combine arrays
		foreach (ref array<string> sz : sessionSZ)
		{
			fileSZ.Insert(sz);
		}
		
		// Write safe zones to file 
		SCR_JsonSaveContext fileSaveContext = new SCR_JsonSaveContext();
		fileSaveContext.WriteValue("safe-zone-list", fileSZ);
		fileSaveContext.SaveToFile(persistedFileLocation);
		
		// return
		return string.Format(
			"Success, stored %1 snd now there's a total of %2 safe zones.",
			sessionSZ.Count(),
			fileSZ.Count()
		);
	}
	
	//! Removing a safe zone from files or session
	static string removeSafeZone(string identifier, bool removeFromFile)
	{
		//Get session save zones 
		SCR_BaseGameMode gm = SCR_BaseGameMode.Cast(GetGame().GetGameMode());
		if (!gm)
		{
			return "Error, can not access SCR_BaseGameMode";
		}
		ref array<ref array<string>> sessionSZ = gm.flabby_safeZoneStorage.safeZones;
		// Get safe zones from file
		ref array<ref array<string>> savedSafeZones = getSafeZonesPersistent();
		// Combine arrays
		foreach (ref array<string> sz : sessionSZ)
		{
			savedSafeZones.Insert(sz);
		}
		
		// Remove safezone if exists
		foreach (ref array<string> sz : savedSafeZones)
		{
			if (sz[0] == identifier)
			{
				savedSafeZones.RemoveItem(sz);
			}
		}
		
		// Write safe zones to file 
		SCR_JsonSaveContext fileSaveContext = new SCR_JsonSaveContext();
		fileSaveContext.WriteValue("safe-zone-list", savedSafeZones);
		fileSaveContext.SaveToFile(persistedFileLocation);
		// Write safe zones to session 
		gm.flabby_safeZoneStorage.safeZones.Clear();
		foreach (ref array<string> sz : savedSafeZones)
		{
			gm.flabby_safeZoneStorage.safeZones.Insert(sz);
			Replication.BumpMe();
		}
		
		return "Success, updated and saved ssfe zones";
	}
	
	// ! Store all session safe zones to file.
	static string saveONESessionSafeZones();
	
	//! Storing a new safe zone in session storage
	static string storeNewSafeZone(float x, float y)
	{
		SCR_BaseGameMode gm = SCR_BaseGameMode.Cast(GetGame().GetGameMode());
		if (!gm)
		{
			return "-1";
		}
		
		// Auto incrment safe zone identifier
		string nextSafeZoneIdentifier = generateIdentifier(4);
		
		// Store safe zone in session (memory)
		ref array<string> safeZone = new array<string>();
		safeZone.Insert(nextSafeZoneIdentifier);
		safeZone.Insert(x.ToString());
		safeZone.Insert(y.ToString());
		gm.flabby_safeZoneStorage.safeZones.Insert(safeZone);
		Replication.BumpMe();
		
		// Return new safe zone identifier
		return nextSafeZoneIdentifier;
	}
}

class flabby_safeZone_data
{
	int identifier;
	int x_coordinate;
	int y_coordinate;
	
	void flabby_safeZone_data(int id, int x, int y)
	{
		identifier = id;
		x_coordinate = x;
		y_coordinate = y;
	}
}