
<br />
<p align="center">
  <img width="3840" height="1240" alt="MainKeyArt" src="https://github.com/user-attachments/assets/9425deb8-efff-4c28-ba44-283b32dd4aea" />
  <br />
  <h1 align="center">☂️ Lady Umbrella ☂️</h1>
</p>

Lady Umbrella is a 3d action-adventure shooter in which the player plays the role of special agent Francesca De Angelis
(aka Lady Umbrella) who, while infiltrating the last mafia family in Italy, is betrayed and incriminated by her partner
and must now clear her name by preventing her own agency from arresting her and defeating the mafia using her main weapon:
a shotgun umbrella that when opened functions as a shield and multiple gadgets.

Switch between combat, traversal, and puzzle-solving tools — the umbrella isn’t just a weapon, it’s your lifeline.

<img src="https://github.com/user-attachments/assets/4aee6508-691b-43ef-b264-813fc3abeb55" width="400">
<img src="https://github.com/user-attachments/assets/cec211ad-5ac2-4f94-b322-ba4f90247155" width="400">
<img src="https://github.com/user-attachments/assets/8d223ce6-e1df-46c6-a53f-65ac19cfe8fe" width="400">
<img src="https://github.com/user-attachments/assets/f140a235-c668-46b4-8b40-988defa0bb5a" width="400">

### 🛠️ Implementation details
---

This project was made in Unreal Engine 5.5.3, using C++ exclusively, except for some Blueprints used for material shaders,
animation blueprints and small trigger events. It contains a lot of different gameplay systems, whereas I'd like to highlight
some of the basics:

- ## Combat Mechanics

  The combat system is arguably Lady Umbrella's biggest system, since it handles everything from shooting, melee attacking,
  launching grenades, _you name it_. To not make this section too big, lets focus on just two things, shooting & reloading

  Shooting is a common ability used by both the player and the enemies, so it was places under the responsibility of the
  GenericWeapon, which is the parent of all weapons in the game. This way reusability is favoured, and reduces code
  entanglement.

  ```c++
  bool AGenericWeapon::Fire()
  {
    if (!HasBullets())
    {
      return false;
    }

    AmmoCurrent--;
    MuzzleSmokeNiagaraComponent->ResetSystem();

    FireSoundFModComponent->SetParameter(*FmodEventParameterName, FMath::RandRange(0, 9));
    FireSoundFModComponent->PlayEvent();

    return true;
  }
  ```

  Reloading follows a similar idea, since it is something all weapons and characters in the game will use. It checks if
  the weapon CAN be reloaded first, due to shortage of bullets for example, and if it can, simply calculates how many
  bullets to reload by using the formula:

      AmountToLoad = AmmoCurrentMax - AmmoCurrent
  
  This makes the logic easily transferable to any weapon type in the game, as it is generic enough to be applicable to
  all weapon types in the game.

  ```c++
  bool AGenericWeapon::Reload(const bool ConsumeAmmo)
  {
    if (!CanReload())
    {
      return false;
    }

	const int32 Amount = FMath::Clamp(AmmoCurrentMax - AmmoCurrent, 0, AmmoReserve);

	AmmoCurrent += Amount;
    AmmoReserve -= Amount;
	
	return true;
  }
  ```

- ## Upgrade System

  Throughout the game there are workbenches which allow the player to upgrade both the offensive and defense capabilities
  of the umbrella. This was done by adding a simple Map to the save file, whereby the game could know if the umbrella was
  upgraded or not, and separate them into different types of upgrades.

  ```c++
  UPROPERTY(SaveGame)
  TMap<EUpgradeType, FUpgrade> Upgrades;
  ```

  By using a map it became very fast and efficient to check if an upgrade was bought and if it was triggered or not,
  achieving a time complexity of O(n), leaving the representation to the UI.
  
- ## Steam Integration
  
  Connects to Steam through the Steamworks SDK in order to trigger achievements and track in-game stats. Since this SDK
  provides an API to access Valve's servers, all that had to be done was to use the provided functions and actually call
  the API.

  ```c++
  bool Steam::UnlockAchievement(const ESteamAchievement SteamAchievement)
  {
    const char* AchievementID = ToString(SteamAchievement);
  
    if (!bAPISuccess || !SteamUserStats())
    {
      return false;
    }
	
    if (SteamUserStats()->SetAchievement(AchievementID))
    {
      SteamUserStats()->StoreStats();
      return true;
    }
	
    return false;
  }
  ```

  By storing the Achievement identifiers in an enum (ESteamAchievement) it became trivial to actually trigger the
  achievement using the function displayed above. Since this neither affects gameplay directly, and Steamworks use
  asynchronous functions for networking, it is not required to wait for a response from Valve's servers, making this
  an extremely efficient way to trigger achievements.

### 🛠️ Other systems
---

There are, of course, more systems that haven't been mentioned yet, but are equally important, such as:

- AI – Implemented using behaviour trees to dynamically change the way enemies interact with the player.
- Interactive Movement – Geometry-based motion to increase agility by allowing vaulting, mantling, swinging...
- FMOD – Wide range of sounds in the game called for useage of this external tool that allowed for greater flexibility in sound design.

### 🤝 Acknowledgements
---

All the playtesters who gave feedback on the game, from the earliest state to the end.
Everyone at Zulo Interactive who made this game possible.
