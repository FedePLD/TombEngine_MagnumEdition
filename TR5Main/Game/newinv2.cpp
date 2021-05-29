#include "framework.h"
#include "newinv2.h"

#include "draw.h"
#include "control.h"
#include "lara_fire.h"
#include "gameflow.h"
#include "sound.h"
#include "savegame.h"
#include "Lara.h"
#include "camera.h"
#include "spotcam.h"
#include "input.h"
#include "configuration.h"
#include "lara_one_gun.h"
#include "lara_two_guns.h"
#include "level.h"
#include "input.h"
#include <Game/health.h>

using namespace T5M::Renderer;
bool goUp, goDown, goRight, goLeft, goSelect, goDeselect;
bool dbUp, dbDown, dbRight, dbLeft, dbSelect, dbDeselect;
long rptRight, rptLeft;
__int64 title_selected_option = 1;
int title_menu_to_display = 0;
int settings_flag;
int pause_menu_to_display = 0;
__int64 pause_selected_option = 1;
int pause_flag;
int GLOBAL_invMode;
extern GameFlow* g_GameFlow;
titleSettings CurrentSettings;
/*inventory*/
RINGME pcring1;//items ring
RINGME pcring2;//other ring
RINGME* rings[2];
int current_selected_option;
int menu_active;
char ammo_selector_flag;
char num_ammo_slots;
char *current_ammo_type;
AMMOLIST ammo_object_list[3];
short ammo_selector_fade_val;
short ammo_selector_fade_dir;
short combine_ring_fade_val;
short combine_ring_fade_dir;
short normal_ring_fade_val;
short normal_ring_fade_dir;
unsigned char ammo_active;
#define font_height 25
int OBJLIST_SPACING;
static unsigned short AmountShotGunAmmo1;
static unsigned short AmountShotGunAmmo2;
static unsigned short AmountHKAmmo1;
static unsigned short AmountCrossBowAmmo1;
static unsigned short AmountCrossBowAmmo2;
static unsigned short AmountCrossBowAmmo3;
static unsigned short AmountGrenadeAmmo1;
static unsigned short AmountGrenadeAmmo2;
static unsigned short AmountGrenadeAmmo3;
static unsigned short AmountRocketsAmmo;
static unsigned short AmountHarpoonAmmo;
static unsigned short AmountUziAmmo;
static unsigned short AmountRevolverAmmo;
static unsigned short AmountPistolsAmmo;
static char CurrentPistolsAmmoType;
static char CurrentUziAmmoType;
static char CurrentRevolverAmmoType;
static char CurrentShotGunAmmoType;
static char CurrentHKAmmoType;
static char CurrentGrenadeGunAmmoType;
static char CurrentCrossBowAmmoType;
static char CurrentHarpoonAmmoType;
static char CurrentRocketAmmoType;
static char StashedCurrentPistolsAmmoType;
static char StashedCurrentUziAmmoType;
static char StashedCurrentRevolverAmmoType;
static char StashedCurrentShotGunAmmoType;
static char StashedCurrentGrenadeGunAmmoType;
static char StashedCurrentCrossBowAmmoType;
static char Stashedcurrent_selected_option;
static char StashedCurrentHKAmmoType;
static char StashedCurrentHarpoonAmmoType;
static char StashedCurrentRocketAmmoType;
int GLOBAL_inventoryitemchosen = NO_ITEM;
int GLOBAL_enterinventory = NO_ITEM;
int GLOBAL_lastinvitem = NO_ITEM;
char useItem;
char loading_or_saving;
char seperate_type_flag;
char combine_type_flag;
short combine_obj1;
short combine_obj2;
short examine_mode = 0;
bool stats_mode = 0;
bool stop_killing_me_you_dumb_input_system;
bool stop_killing_me_you_dumb_input_system2;
int compassNeedleAngle;

uhmG current_options[3];

short optmessages[] =
{
	STRING_USE,
	STRING_CHOOSE_AMMO,
	STRING_COMBINE,
	STRING_SEPARE,
	1,1,//remove later
//	STRING_EQUIP,
//	STRING_COMBINE_WITH,
	STRING_LOAD_GAME,
	STRING_SAVE_GAME,
	1,1,1//remove later
//	STRING_EXAMINE,
//	STRING_STATISTICS,
//	STRING_CHOOSE_WEAPON,
};

#define phd_winxmax g_Configuration.Width
#define phd_winymax g_Configuration.Height
#define phd_centerx 400
#define phd_centery phd_winymax / 2
#define max_combines	24//update this if you add anything to the combine table otherwise it wont work since the relative functions use it!

COMBINELIST combine_table[max_combines] =
{
	{combine_revolver_lasersight, INV_OBJECT_REVOLVER, INV_OBJECT_LASERSIGHT, INV_OBJECT_REVOLVER_LASER},
	{combine_crossbow_lasersight, INV_OBJECT_CROSSBOW, INV_OBJECT_LASERSIGHT, INV_OBJECT_CROSSBOW_LASER},
	{combine_HK_SILENCER, INV_OBJECT_HK, INV_OBJECT_SILENCER, INV_OBJECT_HK_SILENCER},
	{combine_PuzzleItem1, INV_OBJECT_PUZZLE1_COMBO1, INV_OBJECT_PUZZLE1_COMBO2, INV_OBJECT_PUZZLE1},
	{combine_PuzzleItem2, INV_OBJECT_PUZZLE2_COMBO1, INV_OBJECT_PUZZLE2_COMBO2, INV_OBJECT_PUZZLE2},
	{combine_PuzzleItem3, INV_OBJECT_PUZZLE3_COMBO1, INV_OBJECT_PUZZLE3_COMBO2, INV_OBJECT_PUZZLE3},
	{combine_PuzzleItem4, INV_OBJECT_PUZZLE4_COMBO1, INV_OBJECT_PUZZLE4_COMBO2, INV_OBJECT_PUZZLE4},
	{combine_PuzzleItem5, INV_OBJECT_PUZZLE5_COMBO1, INV_OBJECT_PUZZLE5_COMBO2, INV_OBJECT_PUZZLE5},
	{combine_PuzzleItem6, INV_OBJECT_PUZZLE6_COMBO1, INV_OBJECT_PUZZLE6_COMBO2, INV_OBJECT_PUZZLE6},
	{combine_PuzzleItem7, INV_OBJECT_PUZZLE7_COMBO1, INV_OBJECT_PUZZLE7_COMBO2, INV_OBJECT_PUZZLE7},
	{combine_PuzzleItem8, INV_OBJECT_PUZZLE8_COMBO1, INV_OBJECT_PUZZLE8_COMBO2, INV_OBJECT_PUZZLE8},
	{combine_KeyItem1, INV_OBJECT_KEY1_COMBO1, INV_OBJECT_KEY1_COMBO2, INV_OBJECT_KEY1},
	{combine_KeyItem2, INV_OBJECT_KEY2_COMBO1, INV_OBJECT_KEY2_COMBO2, INV_OBJECT_KEY2},
	{combine_KeyItem3, INV_OBJECT_KEY3_COMBO1, INV_OBJECT_KEY3_COMBO2, INV_OBJECT_KEY3},
	{combine_KeyItem4, INV_OBJECT_KEY4_COMBO1, INV_OBJECT_KEY4_COMBO2, INV_OBJECT_KEY4},
	{combine_KeyItem5, INV_OBJECT_KEY5_COMBO1, INV_OBJECT_KEY5_COMBO2, INV_OBJECT_KEY5},
	{combine_KeyItem6, INV_OBJECT_KEY6_COMBO1, INV_OBJECT_KEY6_COMBO2, INV_OBJECT_KEY6},
	{combine_KeyItem7, INV_OBJECT_KEY7_COMBO1, INV_OBJECT_KEY7_COMBO2, INV_OBJECT_KEY7},
	{combine_KeyItem8, INV_OBJECT_KEY8_COMBO1, INV_OBJECT_KEY8_COMBO2, INV_OBJECT_KEY8},
	{combine_PickupItem1, INV_OBJECT_PICKUP1_COMBO1, INV_OBJECT_PICKUP1_COMBO2, INV_OBJECT_PICKUP1},
	{combine_PickupItem2, INV_OBJECT_PICKUP2_COMBO1, INV_OBJECT_PICKUP2_COMBO2, INV_OBJECT_PICKUP2},
	{combine_PickupItem3, INV_OBJECT_PICKUP3_COMBO1, INV_OBJECT_PICKUP3_COMBO2, INV_OBJECT_PICKUP3},
	{combine_PickupItem4, INV_OBJECT_PICKUP4_COMBO1, INV_OBJECT_PICKUP4_COMBO2, INV_OBJECT_PICKUP4},
	{combine_ClockWorkBeetle, INV_OBJECT_BEETLE_PART1, INV_OBJECT_BEETLE_PART2, INV_OBJECT_BEETLE}
};

/*
if you wanna add an object to the inventory, edit the inv_objects array then edit THIS inventry_objects_list array with the object IN THE RIGHT PLACE
the #s MUST MATCH.
i.e if uzi item is #2 in inv_objects (starting count from 0), IT HAS TO BE THE THIRD ELEMENT IN inventry_objects_list. thank you.

note: don't forget to add your object to the proper list construction function, and EDIT THE OPTIONS TABLE ALSO!!!!
and if it's a weapon, add its ammo handling shit. (look at vars at the beginning of the file) 
*/

INVOBJ	inventry_objects_list[INVENTORY_TABLE_SIZE] =
{
	//weps
{ID_PISTOLS_ITEM, 6, 1000, 16384, 44364, 48448, 2, STRING_PISTOLS, -1},
{ID_PISTOLS_AMMO_ITEM, 4, 700, 0, 16384, 0, 2, STRING_PISTOLS_AMMO, -1},
{ID_UZI_ITEM, -4, 1000, 16384, 24576, 16384, 2, STRING_UZI , -1},
{ID_UZI_AMMO_ITEM, 5, 700, 0, 5384, 0, 2, STRING_UZI_AMMO, -1},
{ID_SHOTGUN_ITEM, -6, 640, 32768, 49152, 8192, 2, STRING_SHOTGUN, 1},
{ID_SHOTGUN_AMMO1_ITEM, 0, 500, 16384, 0, 0, 2, STRING_SHOTGUN_AMMO1, -1},
{ID_SHOTGUN_AMMO2_ITEM, 0, 500, 16384, 0, 0, 2, STRING_SHOTGUN_AMMO2, -1},
{ID_REVOLVER_ITEM, 0, 800, 16384, 10922, 15298,	2, STRING_REVOLVER , 1},
{ID_REVOLVER_AMMO_ITEM, 0, 700, 16384, -3000, 0, 2, STRING_REVOLVER_AMMO, -1},
{ID_REVOLVER_ITEM, 0, 800, 16384, 10922, 15298, 2, STRING_REVOLVER_LASER, 7},
{ID_CROSSBOW_ITEM, 0, 900, 8192, 6144, 0, 2, STRING_CROSSBOW, 1},
{ID_CROSSBOW_ITEM, 0, 900, 8192, 6144, 0, 2, STRING_CROSSBOW_LASER, 1/*idk*/},
{ID_CROSSBOW_AMMO1_ITEM, 0, 1100, 16384, -4096, 0, 2, STRING_CROSSBOW_AMMO1, -1},
{ID_CROSSBOW_AMMO2_ITEM, 0, 1100, 16384, -4096, 0, 2, STRING_CROSSBOW_AMMO1, -1},
{ID_CROSSBOW_AMMO3_ITEM, 0, 1100, 16384, -4096, 0, 2, STRING_CROSSBOW_AMMO1, -1},
{ID_HK_ITEM, 0, 800, 0, 0xc000, 0, 2, STRING_HK, 1},
{ID_HK_ITEM, 0, 800, 0, 0xc000, 0, 2, STRING_HK_SILENCED, -1},
{ID_HK_AMMO_ITEM, 3, 800, 16384, 0, 0, 2, STRING_HK_AMMO, 2},
{ID_GRENADE_GUN_ITEM, 0, 800, 16384, 0, 11980, 2, STRING_GRENADE_LAUNCHER, -1},
{ID_GRENADE_AMMO1_ITEM, 3, 800, 16384, 0, 0, 2, STRING_GRENADE_AMMO1, -1},
{ID_GRENADE_AMMO2_ITEM, 3, 800, 16384, 0, 0, 2, STRING_GRENADE_AMMO2, -1},
{ID_GRENADE_AMMO3_ITEM, 3, 800, 16384, 0, 0, 2, STRING_GRENADE_AMMO3, -1},
{ID_HARPOON_ITEM, 0, 800, 16384, 0, 0, 2, STRING_HARPOON_GUN, -1},
{ID_HARPOON_AMMO_ITEM, 3, 800, 16384, 0, 0, 2, STRING_HARPOON_AMMO, -1},
{ID_ROCKET_LAUNCHER_ITEM, 0, 800, 16384, 0, 0, 2, STRING_ROCKET_LAUNCHER, -1},
{ID_ROCKET_LAUNCHER_AMMO_ITEM, 3, 800, 16384, 0, 0, 2, STRING_ROCKET_AMMO, -1},

	//misc

{ID_LASERSIGHT_ITEM, 2, 700, 16384, 2000, 0, 2, STRING_LASERSIGHT, -1},
{ID_SILENCER_ITEM, 1, 900, 0, 2000, 0, 2, STRING_SILENCER, -1},
{ID_BIGMEDI_ITEM, 2, 800, 0, 0, 0, 2, STRING_LARGE_MEDIPACK, -1},
{ID_SMALLMEDI_ITEM, 0, 512, 0, 20480, 0, 2, STRING_SMALL_MEDIPACK, -1},
{ID_BINOCULARS_ITEM, -1, 700, 4096, 2000, 0, 2, STRING_BINOCULARS, -1},
{ID_FLARE_INV_ITEM, 2, 1100, 16384, 0, 0, 2, STRING_FLARES, -1},
{ID_TIMEX_ITEM, 2, 1100, 32768, 0, 0, 2, STRING_TIMEX, -1},
{ID_PC_LOAD_INV_ITEM, 52, 2200, 32768, 0, 0, 2, STRING_LOAD_GAME, -1},
{ID_PC_LOAD_SAVE_ITEM, 52, 2200, 32768, 0, 0, 2, STRING_SAVE_GAME, -1},
{ID_BURNING_TORCH_ITEM, 14, 1200, 0, 16384, 0, 2, STRING_LOAD_GAME, -1},
{ID_CROWBAR_ITEM, 4, 1900, 0, 16384, 0, 2, STRING_CROWBAR, -1},
{ID_DIARY_ITEM, 0, 0, 0, 0, 0, 2, STRING_DIARY, -1},
{ID_COMPASS_ITEM, 0x0FFF2, 0x258, 0, 0x36B0, 0, 0, STRING_LOAD_GAME, -1},
{ID_CLOCKWORK_BEETLE, 14, 1200, 0x4000, 0, 0, 2, STRING_LOAD_GAME, -1},
{ID_CLOCKWORK_BEETLE_COMBO1, 18, 700, 0x4000, 0, 0, 2, STRING_LOAD_GAME, -1},
{ID_CLOCKWORK_BEETLE_COMBO2, 14, 700, 0x4000, 0, 0, 2, STRING_LOAD_GAME, -1},

	//puzzles

{ID_PUZZLE_ITEM1, 14, 1200, 0, 0, 0, 2, STRING_LOAD_GAME, -1},
{ID_PUZZLE_ITEM2, 14, 600, 0, 49152, 0, 2, STRING_LOAD_GAME, -1},
{ID_PUZZLE_ITEM3, 8, 1200, 0, 0, 0, 2, STRING_LOAD_GAME, -1},
{ID_PUZZLE_ITEM4, 8, 1200, 0, 0, 0, 2, STRING_LOAD_GAME, -1},
{ID_PUZZLE_ITEM5, 8, 1200, 0, 0, 0, 2, STRING_LOAD_GAME, -1},
{ID_PUZZLE_ITEM6, 8, 1200, 0, 0, 0, 2, STRING_LOAD_GAME, -1},
{ID_PUZZLE_ITEM7, 8, 1200, 0, 0, 0, 2, STRING_LOAD_GAME, -1},
{ID_PUZZLE_ITEM8, 8, 1200, 0, 0, 0, 2, STRING_LOAD_GAME, -1},

	//puzzle combos

{ID_PUZZLE_ITEM1_COMBO1, 18, 1200, 0, 0, 0, 2, STRING_LOAD_GAME, -1},
{ID_PUZZLE_ITEM1_COMBO2, 18, 1200, 0, 0, 0, 2, STRING_LOAD_GAME, -1},
{ID_PUZZLE_ITEM2_COMBO1, 8, 1200, 0, 0, 0, 2, STRING_LOAD_GAME, -1},
{ID_PUZZLE_ITEM2_COMBO2, 8, 1200, 0, 0, 0, 2, STRING_LOAD_GAME, -1},
{ID_PUZZLE_ITEM3_COMBO1, 8, 1200, 0, 0, 0, 2, STRING_LOAD_GAME, -1},
{ID_PUZZLE_ITEM3_COMBO2, 8, 1200, 0, 0, 0, 2, STRING_LOAD_GAME, -1},
{ID_PUZZLE_ITEM4_COMBO1, 8, 1200, 0, 0, 0, 2, STRING_LOAD_GAME, -1},
{ID_PUZZLE_ITEM4_COMBO2, 8, 1200, 0, 0, 0, 2, STRING_LOAD_GAME, -1},
{ID_PUZZLE_ITEM5_COMBO1, 8, 1200, 0, 0, 0, 2, STRING_LOAD_GAME, -1},
{ID_PUZZLE_ITEM5_COMBO2, 8, 1200, 0, 0, 0, 2, STRING_LOAD_GAME, -1},
{ID_PUZZLE_ITEM6_COMBO1, 8, 1200, 0, 0, 0, 2, STRING_LOAD_GAME, -1},
{ID_PUZZLE_ITEM6_COMBO2, 8, 1200, 0, 0, 0, 2, STRING_LOAD_GAME, -1},
{ID_PUZZLE_ITEM7_COMBO1, 8, 1200, 0, 0, 0, 2, STRING_LOAD_GAME, -1},
{ID_PUZZLE_ITEM7_COMBO2, 8, 1200, 0, 0, 0, 2, STRING_LOAD_GAME, -1},
{ID_PUZZLE_ITEM8_COMBO1, 8, 1200, 0, 0, 0, 2, STRING_LOAD_GAME, -1},
{ID_PUZZLE_ITEM8_COMBO2, 8, 1200, 0, 0, 0, 2, STRING_LOAD_GAME, -1},

	//keys

{ID_KEY_ITEM1, 14, 1200, 0, 0, 0, 2, STRING_LOAD_GAME, -1},
{ID_KEY_ITEM2, 8, 1200, 0, 0, 0, 2, STRING_LOAD_GAME, -1},
{ID_KEY_ITEM3, 8, 1200, 0, 0, 0, 2, STRING_LOAD_GAME, -1},
{ID_KEY_ITEM4, 8, 1200, 0, 0, 0, 2, STRING_LOAD_GAME, -1},
{ID_KEY_ITEM5, 8, 1200, 0, 0, 0, 2, STRING_LOAD_GAME, -1},
{ID_KEY_ITEM6, 8, 1200, 0, 0, 0, 2, STRING_LOAD_GAME, -1},
{ID_KEY_ITEM7, 8, 1200, 0, 0, 0, 2, STRING_LOAD_GAME, -1},
{ID_KEY_ITEM8, 8, 1200, 0, 0, 0, 2, STRING_LOAD_GAME, -1},

	//key combos

{ID_KEY_ITEM1_COMBO1, 18, 1200, 0, 0, 0, 2, STRING_LOAD_GAME, -1},
{ID_KEY_ITEM1_COMBO2, 18, 1200, 0, 0, 0, 2, STRING_LOAD_GAME, -1},
{ID_KEY_ITEM2_COMBO1, 8, 1200, 0, 0, 0, 2, STRING_LOAD_GAME, -1},
{ID_KEY_ITEM2_COMBO2, 8, 1200, 0, 0, 0, 2, STRING_LOAD_GAME, -1},
{ID_KEY_ITEM3_COMBO1, 8, 1200, 0, 0, 0, 2, STRING_LOAD_GAME, -1},
{ID_KEY_ITEM3_COMBO2, 8, 1200, 0, 0, 0, 2, STRING_LOAD_GAME, -1},
{ID_KEY_ITEM4_COMBO1, 8, 1200, 0, 0, 0, 2, STRING_LOAD_GAME, -1},
{ID_KEY_ITEM4_COMBO2, 8, 1200, 0, 0, 0, 2, STRING_LOAD_GAME, -1},
{ID_KEY_ITEM5_COMBO1, 8, 1200, 0, 0, 0, 2, STRING_LOAD_GAME, -1},
{ID_KEY_ITEM5_COMBO2, 8, 1200, 0, 0, 0, 2, STRING_LOAD_GAME, -1},
{ID_KEY_ITEM6_COMBO1, 8, 1200, 0, 0, 0, 2, STRING_LOAD_GAME, -1},
{ID_KEY_ITEM6_COMBO2, 8, 1200, 0, 0, 0, 2, STRING_LOAD_GAME, -1},
{ID_KEY_ITEM7_COMBO1, 8, 1200, 0, 0, 0, 2, STRING_LOAD_GAME, -1},
{ID_KEY_ITEM7_COMBO2, 8, 1200, 0, 0, 0, 2, STRING_LOAD_GAME, -1},
{ID_KEY_ITEM8_COMBO1, 8, 1200, 0, 0, 0, 2, STRING_LOAD_GAME, -1},
{ID_KEY_ITEM8_COMBO2, 8, 1200, 0, 0, 0, 2, STRING_LOAD_GAME, -1},

	//pickups

{ID_PICKUP_ITEM1, 14, 1200, 0, 0, 0, 2, STRING_LOAD_GAME, -1},
{ID_PICKUP_ITEM2, 8, 1200, 0, 0, 0, 2, STRING_LOAD_GAME, -1},
{ID_PICKUP_ITEM3, 8, 1200, 0, 0, 0, 2, STRING_LOAD_GAME, -1},
{ID_PICKUP_ITEM4, 2, 800, 0, 0, 0, 2, STRING_LOAD_GAME, -1},

	//pickup combos

{ID_PICKUP_ITEM1_COMBO1, 14, 1200, 0, 0, 0, 2, STRING_LOAD_GAME, -1},
{ID_PICKUP_ITEM1_COMBO2, 14, 1200, 0, 0, 0, 2, STRING_LOAD_GAME, -1},
{ID_PICKUP_ITEM2_COMBO1, 14, 1200, 0, 0, 0, 2, STRING_LOAD_GAME, -1},
{ID_PICKUP_ITEM2_COMBO2, 14, 1200, 0, 0, 0, 2, STRING_LOAD_GAME, -1},
{ID_PICKUP_ITEM3_COMBO1, 14, 1200, 0, 0, 0, 2, STRING_LOAD_GAME, -1},
{ID_PICKUP_ITEM3_COMBO2, 14, 1200, 0, 0, 0, 2, STRING_LOAD_GAME, -1},
{ID_PICKUP_ITEM4_COMBO1, 14, 1200, 0, 0, 0, 2, STRING_LOAD_GAME, -1},
{ID_PICKUP_ITEM4_COMBO2, 14, 1200, 0, 0, 0, 2, STRING_LOAD_GAME, -1},

	//examines

{ID_EXAMINE1, 4, 1300, 0, 16384, 0, 2, STRING_LOAD_GAME, -1},
{ID_EXAMINE2, 14, 1200, 0, 16384, 0, 2, STRING_LOAD_GAME, -1},
{ID_EXAMINE3, 14, 1200, 0, 16384, 0, 2, STRING_LOAD_GAME, -1},

	//examines combos

{ID_EXAMINE1_COMBO1, 14, 1200, 0, 0, 0, 2, STRING_LOAD_GAME, -1},
{ID_EXAMINE1_COMBO2, 14, 1200, 0, 0, 0, 2, STRING_LOAD_GAME, -1},
{ID_EXAMINE2_COMBO1, 14, 1200, 0, 0, 0, 2, STRING_LOAD_GAME, -1},
{ID_EXAMINE2_COMBO2, 14, 1200, 0, 0, 0, 2, STRING_LOAD_GAME, -1},
{ID_EXAMINE3_COMBO1, 14, 1200, 0, 0, 0, 2, STRING_LOAD_GAME, -1},
{ID_EXAMINE3_COMBO2, 14, 1200, 0, 0, 0, 2, STRING_LOAD_GAME, -1},
};

//available options for each object in the list^^
unsigned short options_table[] =
{
	//Weps
	OPT_EQUIP | OPT_COMBINABLE | OPT_CHOOSEAMMO_PISTOLS, //pistol item
	OPT_USE, //pistol ammo
	OPT_EQUIP | OPT_COMBINABLE | OPT_CHOOSEAMMO_UZI, //uzi item
	OPT_USE, //uzi ammo
	OPT_EQUIP | OPT_COMBINABLE | OPT_CHOOSEAMMO_SHOTGUN, //shotgun item
	OPT_USE, //shotgun ammo1
	OPT_USE, //shotgun ammo2
	OPT_EQUIP | OPT_COMBINABLE | OPT_CHOOSEAMMO_REVOLVER, //revolver
	OPT_USE, //revolver ammo
	OPT_EQUIP | OPT_SEPERATABLE | OPT_CHOOSEAMMO_REVOLVER, //revolver lasersight
	OPT_EQUIP | OPT_COMBINABLE | OPT_CHOOSEAMMO_CROSSBOW, //crossbow
	OPT_EQUIP | OPT_SEPERATABLE | OPT_CHOOSEAMMO_CROSSBOW, //crossbow laser
	OPT_USE, //crossbow ammo1
	OPT_USE, //crossbow ammo2
	OPT_USE, //crossbow ammo3
	OPT_EQUIP | OPT_COMBINABLE | OPT_CHOOSEAMMO_HK, //HK
	OPT_EQUIP | OPT_SEPERATABLE | OPT_CHOOSEAMMO_HK, //HK silencer
	OPT_USE, //HK ammo
	OPT_EQUIP | OPT_COMBINABLE | OPT_CHOOSEAMMO_GRENADEGUN, //Grenade laucher
	OPT_USE, //grenade launcher ammo1
	OPT_USE, //grenade launcher ammo2
	OPT_USE, //grenade launcher ammo3
	OPT_EQUIP | OPT_COMBINABLE | OPT_CHOOSEAMMO_HARPOON, //Harpoon gun
	OPT_USE, //harpoon ammo
	OPT_EQUIP | OPT_COMBINABLE | OPT_CHOOSEAMMO_ROCKET, //rocket launcher
	OPT_USE, //rocket ammo

	//misc
	OPT_USE | OPT_COMBINABLE, //lasersight
	OPT_USE | OPT_COMBINABLE, //silencer
	OPT_USE, //large med
	OPT_USE, //small med
	OPT_USE, //binocs
	OPT_USE, //flares
	OPT_STATS, //timex
	OPT_LOAD, //load floppy
	OPT_SAVE, //save floppy
	OPT_USE, //torch?
	OPT_USE, //crowbar
	OPT_USE, //diary
	0, //compass
	OPT_USE,//clockwork beetle
	OPT_COMBINABLE,//clockwork beetle combo 1
	OPT_COMBINABLE,//clockwork beetle combo 2

	//puzzles
	OPT_USE,
	OPT_USE,
	OPT_USE,
	OPT_USE,
	OPT_USE,//8 puzzle items
	OPT_USE,
	OPT_USE,
	OPT_USE,

	//puzzle combines
	OPT_USE | OPT_COMBINABLE,
	OPT_USE | OPT_COMBINABLE,
	OPT_USE | OPT_COMBINABLE,
	OPT_USE | OPT_COMBINABLE,
	OPT_USE | OPT_COMBINABLE,
	OPT_USE | OPT_COMBINABLE,
	OPT_USE | OPT_COMBINABLE,
	OPT_USE | OPT_COMBINABLE,//8 puzzle items, each one has 2 parts, so 16
	OPT_USE | OPT_COMBINABLE,
	OPT_USE | OPT_COMBINABLE,
	OPT_USE | OPT_COMBINABLE,
	OPT_USE | OPT_COMBINABLE,
	OPT_USE | OPT_COMBINABLE,
	OPT_USE | OPT_COMBINABLE,
	OPT_USE | OPT_COMBINABLE,
	OPT_USE | OPT_COMBINABLE,

	//keys
	OPT_USE,
	OPT_USE,
	OPT_USE,
	OPT_USE,
	OPT_USE,//8 key items
	OPT_USE,
	OPT_USE,
	OPT_USE,

	//key combines
	OPT_USE | OPT_COMBINABLE,
	OPT_USE | OPT_COMBINABLE,
	OPT_USE | OPT_COMBINABLE,
	OPT_USE | OPT_COMBINABLE,
	OPT_USE | OPT_COMBINABLE,
	OPT_USE | OPT_COMBINABLE,
	OPT_USE | OPT_COMBINABLE,
	OPT_USE | OPT_COMBINABLE,//8 key items, each one has 2 parts, so 16
	OPT_USE | OPT_COMBINABLE,
	OPT_USE | OPT_COMBINABLE,
	OPT_USE | OPT_COMBINABLE,
	OPT_USE | OPT_COMBINABLE,
	OPT_USE | OPT_COMBINABLE,
	OPT_USE | OPT_COMBINABLE,
	OPT_USE | OPT_COMBINABLE,
	OPT_USE | OPT_COMBINABLE,

	//pickups
	OPT_USE,
	OPT_USE,
	OPT_USE,//4 of them
	OPT_USE,

	//pickup combines
	OPT_USE | OPT_COMBINABLE,
	OPT_USE | OPT_COMBINABLE,
	OPT_USE | OPT_COMBINABLE,
	OPT_USE | OPT_COMBINABLE,//4 pickup items, each one has 2 parts, so 8
	OPT_USE | OPT_COMBINABLE,
	OPT_USE | OPT_COMBINABLE,
	OPT_USE | OPT_COMBINABLE,
	OPT_USE | OPT_COMBINABLE,

	//examines
	OPT_EXAMINABLE,
	OPT_EXAMINABLE,
	OPT_EXAMINABLE,

	OPT_USE | OPT_COMBINABLE,
	OPT_USE | OPT_COMBINABLE,
	OPT_USE | OPT_COMBINABLE,
	OPT_USE | OPT_COMBINABLE,//3 examine items, each one has 2 parts, so 6
	OPT_USE | OPT_COMBINABLE,
	OPT_USE | OPT_COMBINABLE,
};

void DrawInv()
{
	g_Renderer.renderInventory();
}

void clear_input_vars(bool flag)
{
	goUp = goDown = goRight = goLeft = goSelect = goDeselect = 0;
	if (flag)
		return;
	else //don't wanna make another function for the rest
	{
		dbUp = dbDown = dbRight = dbLeft = dbSelect = dbDeselect = 0;
		rptRight = rptLeft = 0;
	}
}

void do_debounced_input()
{
	clear_input_vars(1);

	if (TrInput & IN_LEFT)
	{
		if (rptLeft >= 8)
			goLeft = 1;
		else
			rptLeft++;

		if (!dbLeft)
			goLeft = 1;

		dbLeft = 1;
	}
	else
	{
		dbLeft = 0;
		rptLeft = 0;
	}

	if (TrInput & IN_RIGHT)
	{
		if (rptRight >= 8)
			goRight = 1;
		else
			rptRight++;

		if (!dbRight)
			goRight = 1;

		dbRight = 1;
	}
	else
	{
		dbRight = 0;
		rptRight = 0;
	}

	if (TrInput & IN_FORWARD)
	{
		if (!dbUp)
			goUp = 1;

		dbUp = 1;
	}
	else
		dbUp = 0;

	if (TrInput & IN_BACK)
	{
		if (!dbDown)
			goDown = 1;

		dbDown = 1;
	}
	else
		dbDown = 0;

	if (TrInput & IN_ACTION || TrInput & IN_SELECT)
		dbSelect = 1;
	else
	{
		if (dbSelect == 1 && !stop_killing_me_you_dumb_input_system)
			goSelect = 1;

		dbSelect = 0;
		stop_killing_me_you_dumb_input_system = 0;
	}

	if ((TrInput & IN_DESELECT))
		dbDeselect = 1;
	else
	{
		if (dbDeselect == 1 && !stop_killing_me_you_dumb_input_system2)
			goDeselect = 1;

		dbDeselect = 0;
		stop_killing_me_you_dumb_input_system2 = 0;
	}
}

int TitleOptions()
{
	int ret, ret2, i, n, n2, load, flag;

	static int always0 = 0;
	static __int64 selected_option_bak = 0;

	ret = 0;

	/*stuff for credits go here!*/

	switch (title_menu_to_display)
	{
	case title_main_menu:
		flag = 8;
		break;

	case title_select_level:
		ret = 0;
		flag = 1 << (g_GameFlow->GetNumLevels() - 2);
		break;

	case title_load_game:
		if (title_selected_option == 1)//shitty but works
			title_selected_option = 2;

		flag = 1 << (MAX_SAVEGAMES);
		break;
	case title_options_menu:
		flag = 1 << 2;
		break;

	case title_display_menu:
		settings_flag = 1 << 6;
		handle_display_setting_input();
		break;

	case title_controls_menu:
		settings_flag = 1 << 19;
		handle_control_settings_input();
		break;

	case title_sounds_menu:
		settings_flag = 1 << 5;
		handle_sound_settings_input();
		break;
	}

	do_debounced_input();

	if (title_menu_to_display <= title_options_menu)
	{
		if (goUp)
		{
			if (title_selected_option > 1)
				title_selected_option >>= 1;

			SoundEffect(SFX_TR4_MENU_CHOOSE, 0, SFX_ALWAYS);
		}

		if (goDown)
		{
			if (title_selected_option < flag)
				title_selected_option <<= 1;

			SoundEffect(SFX_TR4_MENU_CHOOSE, 0, SFX_ALWAYS);
		}

		if (goDeselect && title_menu_to_display != title_main_menu)
		{
			title_menu_to_display = title_main_menu;
			title_selected_option = selected_option_bak;
			SoundEffect(SFX_TR4_MENU_SELECT, 0, SFX_ALWAYS);
		}
	}

	if (goSelect)
	{
		if (title_menu_to_display <= 2)
		{
			SoundEffect(SFX_TR4_MENU_SELECT, 0, SFX_ALWAYS);

			if (title_menu_to_display != 0)
			{
				if (title_menu_to_display == title_select_level)
				{
					LevelComplete = 0;

					n = 0;
					n2 = (int)title_selected_option;

					if (n2)
					{
						do
						{
							n2 >>= 1;
							n++;

						} while (n2);

						g_GameFlow->SelectedLevelForNewGame = n;
						title_menu_to_display = title_main_menu;
						title_selected_option = 1;
					}

					ret = INV_RESULT_NEW_GAME_SELECTED_LEVEL;
				}
			}
			else if (title_selected_option > 0 && title_selected_option <= 8)
			{
				switch (title_selected_option)
				{
				case 1:
					if (g_GameFlow->PlayAnyLevel)
					{
						selected_option_bak = title_selected_option;
						title_menu_to_display = title_select_level;
					}
					else
					{
					//	LevelComplete = 1;
						ret = INV_RESULT_NEW_GAME;
					}

					break;

				case 2:
					selected_option_bak = title_selected_option;
					title_menu_to_display = title_load_game;
					break;

				case 3:
				case 5:
				case 6:
				case 7:

					break;

				case 4:
					selected_option_bak = title_selected_option;
					title_selected_option = 1;
					title_menu_to_display = title_options_menu;
					break;

				case 8:
					ret = INV_RESULT_EXIT_GAME;
					break;
				}
			}
		}
		else if (title_menu_to_display == 3)
		{
			switch (title_selected_option)
			{
			case 1:
				FillDisplayOptions();
				title_menu_to_display = title_display_menu;
				break;

			case 2:
				title_menu_to_display = title_controls_menu;
				title_selected_option = 1;
				break;

			case 4:
				fillSound();
				title_menu_to_display = title_sounds_menu;
				title_selected_option = 1;
				break;
			}
		}
	}

	if (ret == INV_RESULT_EXIT_GAME)
		return INV_RESULT_EXIT_GAME;

	if (ret)
	{
		LevelComplete = 0;
		ret = INV_RESULT_NEW_GAME;
	}

	return ret;
}

__int64 getTitleSelection()
{
	return title_selected_option;
}

int getTitleMenu()
{
	return title_menu_to_display;
}

void FillDisplayOptions()
{
	// Copy configuration to a temporary object
	memcpy(&CurrentSettings.conf, &g_Configuration, sizeof(GameConfiguration));

	// Get current display mode
	vector<RendererVideoAdapter>* adapters = g_Renderer.getAdapters();
	RendererVideoAdapter* adapter = &(*adapters)[CurrentSettings.conf.Adapter];
	CurrentSettings.videoMode = 0;
	for (int i = 0; i < adapter->DisplayModes.size(); i++)
	{
		RendererDisplayMode* mode = &adapter->DisplayModes[i];
		if (mode->Width == CurrentSettings.conf.Width && mode->Height == CurrentSettings.conf.Height &&
			mode->RefreshRate == CurrentSettings.conf.RefreshRate)
		{
			CurrentSettings.videoMode = i;
			break;
		}
	}
}

void handle_display_setting_input()
{
	vector<RendererVideoAdapter>* adapters = g_Renderer.getAdapters();
	RendererVideoAdapter* adapter = &(*adapters)[CurrentSettings.conf.Adapter];

	SetDebounce = true;
	S_UpdateInput();
	SetDebounce = false;

	do_debounced_input();

	if (goDeselect)
	{
		SoundEffect(SFX_TR4_MENU_SELECT, NULL, 0);

		title_menu_to_display = title_options_menu;
		title_selected_option = 1;
		return;
	}

	if (goLeft)
	{
		switch (title_selected_option)
		{
		case 1:
			SoundEffect(SFX_TR4_MENU_CHOOSE, NULL, 0);
			if (CurrentSettings.videoMode > 0)
				CurrentSettings.videoMode--;
			break;

		case 2:
			SoundEffect(SFX_TR4_MENU_CHOOSE, NULL, 0);
			CurrentSettings.conf.Windowed = !CurrentSettings.conf.Windowed;
			break;

		case 4:
			SoundEffect(SFX_TR4_MENU_CHOOSE, NULL, 0);
			CurrentSettings.conf.EnableShadows = !CurrentSettings.conf.EnableShadows;
			break;

		case 8:
			SoundEffect(SFX_TR4_MENU_CHOOSE, NULL, 0);
			CurrentSettings.conf.EnableCaustics = !CurrentSettings.conf.EnableCaustics;
			break;

		case 16:
			SoundEffect(SFX_TR4_MENU_CHOOSE, NULL, 0);
			CurrentSettings.conf.EnableVolumetricFog = !CurrentSettings.conf.EnableVolumetricFog;
			break;
		}
	}

	if (goRight)
	{
		switch (title_selected_option)
		{
		case 1:
			SoundEffect(SFX_TR4_MENU_CHOOSE, NULL, 0);
			if (CurrentSettings.videoMode < adapter->DisplayModes.size() - 1)
				CurrentSettings.videoMode++;
			break;

		case 2:
			SoundEffect(SFX_TR4_MENU_CHOOSE, NULL, 0);
			CurrentSettings.conf.Windowed = !CurrentSettings.conf.Windowed;
			break;

		case 4:
			SoundEffect(SFX_TR4_MENU_CHOOSE, NULL, 0);
			CurrentSettings.conf.EnableShadows = !CurrentSettings.conf.EnableShadows;
			break;

		case 8:
			SoundEffect(SFX_TR4_MENU_CHOOSE, NULL, 0);
			CurrentSettings.conf.EnableCaustics = !CurrentSettings.conf.EnableCaustics;
			break;

		case 16:
			SoundEffect(SFX_TR4_MENU_CHOOSE, NULL, 0);
			CurrentSettings.conf.EnableVolumetricFog = !CurrentSettings.conf.EnableVolumetricFog;
			break;
		}
	}

	if (goUp)
	{
		if (title_selected_option > 1)
			title_selected_option >>= 1;

		SoundEffect(SFX_TR4_MENU_CHOOSE, 0, SFX_ALWAYS);
	}

	if (goDown)
	{
		if (title_selected_option < settings_flag)
			title_selected_option <<= 1;

		SoundEffect(SFX_TR4_MENU_CHOOSE, 0, SFX_ALWAYS);
	}

	if (goSelect)
	{
		SoundEffect(SFX_TR4_MENU_SELECT, NULL, 0);

		if (title_selected_option & (1 << 5))
		{
			// Save the configuration
			RendererDisplayMode* mode = &adapter->DisplayModes[CurrentSettings.videoMode];
			CurrentSettings.conf.Width = mode->Width;
			CurrentSettings.conf.Height = mode->Height;
			CurrentSettings.conf.RefreshRate = mode->RefreshRate;

			memcpy(&g_Configuration, &CurrentSettings.conf, sizeof(GameConfiguration));
			SaveConfiguration();

			// Reset screen and go back
			g_Renderer.changeScreenResolution(CurrentSettings.conf.Width, CurrentSettings.conf.Height,
				CurrentSettings.conf.RefreshRate, CurrentSettings.conf.Windowed);
			return;
		}

		if (title_selected_option & (1 << 6))
		{
			title_menu_to_display = title_options_menu;
			title_selected_option = 1;
			return;
		}
	}
}

void handle_control_settings_input()
{
	CurrentSettings.waitingForkey = 0;

	memcpy(&CurrentSettings.conf.KeyboardLayout, &KeyboardLayout[1], NUM_CONTROLS);

	SetDebounce = true;
	S_UpdateInput();
	SetDebounce = false;

	do_debounced_input();

	if (goDeselect)
	{
		if (!CurrentSettings.waitingForkey)
		{
			title_menu_to_display = title_options_menu;
			title_selected_option = 2;
		}
		else
			CurrentSettings.waitingForkey = 0;

		return;
	}

	if (!CurrentSettings.waitingForkey)
	{
		if (goUp)
		{
			if (title_selected_option > 1)
				title_selected_option >>= 1;

			SoundEffect(SFX_TR4_MENU_CHOOSE, 0, SFX_ALWAYS);
		}

		if (goDown)
		{
			if (title_selected_option < settings_flag)
				title_selected_option <<= 1;

			SoundEffect(SFX_TR4_MENU_CHOOSE, 0, SFX_ALWAYS);
		}

		if (goSelect)
		{
			if (title_selected_option & (1 << 18))//apply
			{
				SoundEffect(SFX_TR4_MENU_CHOOSE, NULL, 0);
				memcpy(KeyboardLayout[1], CurrentSettings.conf.KeyboardLayout, NUM_CONTROLS);
				SaveConfiguration();
				title_menu_to_display = title_options_menu;
				title_selected_option = 2;
				return;
			}

			if (title_selected_option & (1 << 19))//cancel
			{
				SoundEffect(SFX_TR4_MENU_CHOOSE, NULL, 0);
				title_menu_to_display = title_options_menu;
				title_selected_option = 2;
				return;
			}
		}
	}

	if (KeyMap[DIK_RETURN])
	{
		SoundEffect(SFX_TR4_MENU_SELECT, NULL, 0);
		CurrentSettings.waitingForkey = 1;
	}

	if (CurrentSettings.waitingForkey)
	{
		TrInput = 0;
		DbInput = 0;
		ZeroMemory(KeyMap, 256);

		while (true)
		{
			if (DbInput & IN_DESELECT)
			{
				CurrentSettings.waitingForkey = false;
				break;
			}

			int selectedKey = 0;
			for (selectedKey = 0; selectedKey < 256; selectedKey++)
			{
				if (KeyMap[selectedKey] & 0x80)
					break;
			}

			if (selectedKey == 256)
				selectedKey = 0;

			if (selectedKey && g_KeyNames[selectedKey])
			{
				if (!(selectedKey == DIK_RETURN || selectedKey == DIK_LEFT || selectedKey == DIK_RIGHT ||
					selectedKey == DIK_UP || selectedKey == DIK_DOWN))
				{
					if (selectedKey != DIK_ESCAPE)
					{
						int index;
						int selection = (int)title_selected_option;

						index = 0;

						if (selection)
						{
							do
							{
								selection >>= 1;
								index++;

							} while (selection);
						}

						KeyboardLayout[1][index - 1] = selectedKey;
						DefaultConflict();
						DbInput = 0;
						CurrentSettings.waitingForkey = false;
						return;
					}
				}
			}

			g_Renderer.renderTitle();
			Camera.numberFrames = g_Renderer.SyncRenderer();
			int nframes = Camera.numberFrames;
			ControlPhase(nframes, 0);

			SetDebounce = true;
			S_UpdateInput();
			SetDebounce = false;
		}
	}
}

void fillSound()
{
	memcpy(&CurrentSettings.conf, &g_Configuration, sizeof(GameConfiguration));
}

void handle_sound_settings_input()
{
	int oldVolume = CurrentSettings.conf.MusicVolume;
	int oldSfxVolume = CurrentSettings.conf.SfxVolume;
	bool wasSoundEnabled = CurrentSettings.conf.EnableSound;

	SetDebounce = true;
	S_UpdateInput();
	SetDebounce = false;

	do_debounced_input();

	if (goDeselect)
	{
		GlobalMusicVolume = oldVolume;
		GlobalFXVolume = oldSfxVolume;
		title_menu_to_display = title_options_menu;
		title_selected_option = 4;
		return;
	}

	if (goLeft)
	{
		switch (title_selected_option)
		{
		case 1:
			SoundEffect(SFX_TR4_MENU_CHOOSE, NULL, 0);
			CurrentSettings.conf.EnableSound = !CurrentSettings.conf.EnableSound;

			break;

		case 2:
			SoundEffect(SFX_TR4_MENU_CHOOSE, NULL, 0);
			CurrentSettings.conf.EnableAudioSpecialEffects = !CurrentSettings.conf.EnableAudioSpecialEffects;
			break;

		case 4:
			if (CurrentSettings.conf.MusicVolume > 0)
			{
				static int db = 0;
				CurrentSettings.conf.MusicVolume--;
				GlobalMusicVolume = CurrentSettings.conf.MusicVolume;
				if (!db)
				{
					SoundEffect(SFX_TR4_MENU_CHOOSE, NULL, 0);
					db = 10;
				}
				else
					db -= 2;
			}

			break;

		case 8:
			if (CurrentSettings.conf.SfxVolume > 0)
			{
				static int db = 0;
				CurrentSettings.conf.SfxVolume--;
				GlobalFXVolume = CurrentSettings.conf.SfxVolume;
				if (!db)
				{
					SoundEffect(SFX_TR4_MENU_CHOOSE, NULL, 0);
					db = 10;
				}
				else
					db -= 2;
			}

			break;
		}
	}

	if (goRight)
	{
		switch (title_selected_option)
		{
		case 1:
			SoundEffect(SFX_TR4_MENU_CHOOSE, NULL, 0);
			CurrentSettings.conf.EnableSound = !CurrentSettings.conf.EnableSound;
			break;

		case 2:
			SoundEffect(SFX_TR4_MENU_CHOOSE, NULL, 0);
			CurrentSettings.conf.EnableAudioSpecialEffects = !CurrentSettings.conf.EnableAudioSpecialEffects;
			break;

		case 4:
			if (CurrentSettings.conf.MusicVolume < 100)
			{
				static int db = 0;
				CurrentSettings.conf.MusicVolume++;
				GlobalMusicVolume = CurrentSettings.conf.MusicVolume;
				if (!db)
				{
					SoundEffect(SFX_TR4_MENU_CHOOSE, NULL, 0);
					db = 10;
				}
				else
					db -= 2;
			}

			break;

		case 8:
			if (CurrentSettings.conf.SfxVolume < 100)
			{
				static int db = 0;
				CurrentSettings.conf.SfxVolume++;
				GlobalFXVolume = CurrentSettings.conf.SfxVolume;
				if (!db)
				{
					SoundEffect(SFX_TR4_MENU_CHOOSE, NULL, 0);
					db = 10;
				}
				else
					db -= 2;
			}

			break;
		}
	}

	if (goUp)
	{
		if (title_selected_option > 1)
			title_selected_option >>= 1;

		SoundEffect(SFX_TR4_MENU_CHOOSE, 0, SFX_ALWAYS);
	}

	if (goDown)
	{
		if (title_selected_option < settings_flag)
			title_selected_option <<= 1;

		SoundEffect(SFX_TR4_MENU_CHOOSE, 0, SFX_ALWAYS);
	}

	if (goSelect)
	{
		SoundEffect(SFX_TR4_MENU_SELECT, NULL, 0);

		if (title_selected_option & (1 << 4))
		{
			// Save the configuration
			GlobalMusicVolume = CurrentSettings.conf.MusicVolume;
			GlobalFXVolume = CurrentSettings.conf.SfxVolume;
			memcpy(&g_Configuration, &CurrentSettings.conf, sizeof(GameConfiguration));
			SaveAudioConfig();

			// Init or deinit the sound system
			if (wasSoundEnabled && !g_Configuration.EnableSound)
				Sound_DeInit();
			else if (!wasSoundEnabled && g_Configuration.EnableSound)
				Sound_Init();

			return;
		}

		if (title_selected_option & (1 << 5))
		{
			SoundEffect(SFX_TR4_MENU_SELECT, NULL, 0);
			GlobalMusicVolume = oldVolume;
			GlobalFXVolume = oldSfxVolume;
			title_menu_to_display = title_options_menu;
			title_selected_option = 4;
			return;
		}
	}
}

int DoPauseMenu()
{
	//basically mini title

	switch (pause_menu_to_display)
	{
	case pause_main_menu:
		pause_flag = 4;
		break;

	case pause_statistics:
		pause_flag = 0;
		break;

	case pause_options_menu:
		pause_flag = 4;
		break;

	case pause_display_menu:
		pause_flag = 1 << 6;
		handle_display_setting_input_pause();
		break;

	case pause_controls_menu:
		pause_flag = 1 << 17;
		handle_control_settings_input_pause();
		break;

	case pause_sounds_menu:
		pause_flag = 1 << 5;
		handle_sound_settings_input_pause();
		break;
	}

	clear_input_vars(1);
	SetDebounce = 1;
	S_UpdateInput();
	SetDebounce = 0;
	do_debounced_input();

	if (pause_menu_to_display <= pause_options_menu)
	{
		if (goUp)
		{
			if (pause_selected_option > 1)
				pause_selected_option >>= 1;

			SoundEffect(SFX_TR4_MENU_CHOOSE, 0, SFX_ALWAYS);
		}

		if (goDown)
		{
			if (pause_selected_option < pause_flag)
				pause_selected_option <<= 1;

			SoundEffect(SFX_TR4_MENU_CHOOSE, 0, SFX_ALWAYS);
		}
	}

	if (goDeselect)
	{
		if (pause_menu_to_display == pause_main_menu)
		{
			GLOBAL_invMode = IM_NONE;
			SoundEffect(SFX_TR4_MENU_SELECT, 0, SFX_ALWAYS);
			return INV_RESULT_NONE;
		}

		if (pause_menu_to_display == pause_statistics || pause_menu_to_display == pause_options_menu)
		{
			pause_selected_option = pause_menu_to_display == pause_statistics ? 1 : 2;
			pause_menu_to_display = pause_main_menu;
			SoundEffect(SFX_TR4_MENU_SELECT, 0, SFX_ALWAYS);
		}
	}

	if (goSelect)
	{
		if (pause_menu_to_display == pause_main_menu)
		{
			switch (pause_selected_option)
			{
			case 1:
				pause_selected_option = 1;
				pause_menu_to_display = pause_statistics;
				break;

			case 2:
				pause_selected_option = 1;
				pause_menu_to_display = pause_options_menu;
				break;

			case 4:
				GLOBAL_invMode = IM_NONE;
				return INV_RESULT_EXIT_TO_TILE;
			}
		}
		else if (pause_menu_to_display == pause_options_menu)
		{
			switch (pause_selected_option)
			{
			case 1:
				FillDisplayOptions();
				pause_menu_to_display = pause_display_menu;
				break;

			case 2:
				pause_selected_option = 1;
				pause_menu_to_display = pause_controls_menu;
				break;

			case 4:
				fillSound();
				pause_selected_option = 1;
				pause_menu_to_display = pause_sounds_menu;
				break;
			}
		}
	}

	return INV_RESULT_NONE;
}

int GetPauseMenu()
{
	return pause_menu_to_display;
}

__int64 GetPauseSelection()
{
	return pause_selected_option;
}

void handle_display_setting_input_pause()
{
	vector<RendererVideoAdapter>* adapters = g_Renderer.getAdapters();
	RendererVideoAdapter* adapter = &(*adapters)[CurrentSettings.conf.Adapter];

	if (goDeselect)
	{
		SoundEffect(SFX_TR4_MENU_SELECT, NULL, 0);

		pause_menu_to_display = pause_options_menu;
		pause_selected_option = 1;
		return;
	}

	if (goLeft)
	{
		switch (pause_selected_option)
		{
		case 1:
			SoundEffect(SFX_TR4_MENU_CHOOSE, NULL, 0);
			if (CurrentSettings.videoMode > 0)
				CurrentSettings.videoMode--;
			break;

		case 2:
			SoundEffect(SFX_TR4_MENU_CHOOSE, NULL, 0);
			CurrentSettings.conf.Windowed = !CurrentSettings.conf.Windowed;
			break;

		case 4:
			SoundEffect(SFX_TR4_MENU_CHOOSE, NULL, 0);
			CurrentSettings.conf.EnableShadows = !CurrentSettings.conf.EnableShadows;
			break;

		case 8:
			SoundEffect(SFX_TR4_MENU_CHOOSE, NULL, 0);
			CurrentSettings.conf.EnableCaustics = !CurrentSettings.conf.EnableCaustics;
			break;

		case 16:
			SoundEffect(SFX_TR4_MENU_CHOOSE, NULL, 0);
			CurrentSettings.conf.EnableVolumetricFog = !CurrentSettings.conf.EnableVolumetricFog;
			break;
		}
	}

	if (goRight)
	{
		switch (pause_selected_option)
		{
		case 1:
			SoundEffect(SFX_TR4_MENU_CHOOSE, NULL, 0);
			if (CurrentSettings.videoMode < adapter->DisplayModes.size() - 1)
				CurrentSettings.videoMode++;
			break;

		case 2:
			SoundEffect(SFX_TR4_MENU_CHOOSE, NULL, 0);
			CurrentSettings.conf.Windowed = !CurrentSettings.conf.Windowed;
			break;

		case 4:
			SoundEffect(SFX_TR4_MENU_CHOOSE, NULL, 0);
			CurrentSettings.conf.EnableShadows = !CurrentSettings.conf.EnableShadows;
			break;

		case 8:
			SoundEffect(SFX_TR4_MENU_CHOOSE, NULL, 0);
			CurrentSettings.conf.EnableCaustics = !CurrentSettings.conf.EnableCaustics;
			break;

		case 16:
			SoundEffect(SFX_TR4_MENU_CHOOSE, NULL, 0);
			CurrentSettings.conf.EnableVolumetricFog = !CurrentSettings.conf.EnableVolumetricFog;
			break;
		}
	}

	if (goUp)
	{
		if (pause_selected_option > 1)
			pause_selected_option >>= 1;

		SoundEffect(SFX_TR4_MENU_CHOOSE, 0, SFX_ALWAYS);
	}

	if (goDown)
	{
		if (pause_selected_option < pause_flag)
			pause_selected_option <<= 1;

		SoundEffect(SFX_TR4_MENU_CHOOSE, 0, SFX_ALWAYS);
	}

	if (goSelect)
	{
		SoundEffect(SFX_TR4_MENU_SELECT, NULL, 0);

		if (pause_selected_option & (1 << 5))
		{
			// Save the configuration
			RendererDisplayMode* mode = &adapter->DisplayModes[CurrentSettings.videoMode];
			CurrentSettings.conf.Width = mode->Width;
			CurrentSettings.conf.Height = mode->Height;
			CurrentSettings.conf.RefreshRate = mode->RefreshRate;

			memcpy(&g_Configuration, &CurrentSettings.conf, sizeof(GameConfiguration));
			SaveConfiguration();

			// Reset screen and go back
			g_Renderer.changeScreenResolution(CurrentSettings.conf.Width, CurrentSettings.conf.Height,
				CurrentSettings.conf.RefreshRate, CurrentSettings.conf.Windowed);
			return;
		}

		if (pause_selected_option & (1 << 6))
		{
			pause_menu_to_display = pause_options_menu;
			pause_selected_option = 1;
			return;
		}
	}
}

void handle_control_settings_input_pause()
{
	CurrentSettings.waitingForkey = 0;

	memcpy(&CurrentSettings.conf.KeyboardLayout, &KeyboardLayout[1], NUM_CONTROLS);

	if (goDeselect)
	{
		if (!CurrentSettings.waitingForkey)
		{
			pause_menu_to_display = pause_options_menu;
			pause_selected_option = 2;
		}
		else
			CurrentSettings.waitingForkey = 0;

		return;
	}

	if (!CurrentSettings.waitingForkey)
	{
		if (goUp)
		{
			if (pause_selected_option > 1)
				pause_selected_option >>= 1;

			SoundEffect(SFX_TR4_MENU_CHOOSE, 0, SFX_ALWAYS);
		}

		if (goDown)
		{
			if (pause_selected_option < pause_flag)
				pause_selected_option <<= 1;

			SoundEffect(SFX_TR4_MENU_CHOOSE, 0, SFX_ALWAYS);
		}

		if (goSelect)
		{
			if (pause_selected_option & (1 << 18))//apply
			{
				SoundEffect(SFX_TR4_MENU_CHOOSE, NULL, 0);
				memcpy(KeyboardLayout[1], CurrentSettings.conf.KeyboardLayout, NUM_CONTROLS);
				SaveConfiguration();
				pause_menu_to_display = pause_options_menu;
				pause_selected_option = 2;
				return;
			}

			if (pause_selected_option & (1 << 19))//cancel
			{
				SoundEffect(SFX_TR4_MENU_CHOOSE, NULL, 0);
				pause_menu_to_display = pause_options_menu;
				pause_selected_option = 2;
				return;
			}
		}
	}

	if (KeyMap[DIK_RETURN])
	{
		SoundEffect(SFX_TR4_MENU_SELECT, NULL, 0);
		CurrentSettings.waitingForkey = 1;
	}

	if (CurrentSettings.waitingForkey)
	{
		TrInput = 0;
		DbInput = 0;
		ZeroMemory(KeyMap, 256);

		while (true)
		{
			if (DbInput & IN_DESELECT)
			{
				CurrentSettings.waitingForkey = false;
				break;
			}

			int selectedKey = 0;
			for (selectedKey = 0; selectedKey < 256; selectedKey++)
			{
				if (KeyMap[selectedKey] & 0x80)
					break;
			}

			if (selectedKey == 256)
				selectedKey = 0;

			if (selectedKey && g_KeyNames[selectedKey])
			{
				if (!(selectedKey == DIK_RETURN || selectedKey == DIK_LEFT || selectedKey == DIK_RIGHT ||
					selectedKey == DIK_UP || selectedKey == DIK_DOWN))
				{
					if (selectedKey != DIK_ESCAPE)
					{
						int index;
						int selection = (int)pause_selected_option;

						index = 0;

						if (selection)
						{
							do
							{
								selection >>= 1;
								index++;

							} while (selection);
						}

						KeyboardLayout[1][index - 1] = selectedKey;
						DefaultConflict();
						DbInput = 0;
						CurrentSettings.waitingForkey = false;
						return;
					}
				}
			}

			g_Renderer.renderInventory();
			Camera.numberFrames = g_Renderer.SyncRenderer();

			SetDebounce = true;
			S_UpdateInput();
			SetDebounce = false;
		}
	}
}

void handle_sound_settings_input_pause()
{
	int oldVolume = CurrentSettings.conf.MusicVolume;
	int oldSfxVolume = CurrentSettings.conf.SfxVolume;
	bool wasSoundEnabled = CurrentSettings.conf.EnableSound;

	if (goDeselect)
	{
		GlobalMusicVolume = oldVolume;
		GlobalFXVolume = oldSfxVolume;
		pause_menu_to_display = pause_options_menu;
		pause_selected_option = 4;
		return;
	}

	if (goLeft)
	{
		switch (pause_selected_option)
		{
		case 1:
			SoundEffect(SFX_TR4_MENU_CHOOSE, NULL, 0);
			CurrentSettings.conf.EnableSound = !CurrentSettings.conf.EnableSound;

			break;

		case 2:
			SoundEffect(SFX_TR4_MENU_CHOOSE, NULL, 0);
			CurrentSettings.conf.EnableAudioSpecialEffects = !CurrentSettings.conf.EnableAudioSpecialEffects;
			break;

		case 4:
			if (CurrentSettings.conf.MusicVolume > 0)
			{
				static int db = 0;
				CurrentSettings.conf.MusicVolume--;
				GlobalMusicVolume = CurrentSettings.conf.MusicVolume;
				if (!db)
				{
					SoundEffect(SFX_TR4_MENU_CHOOSE, NULL, 0);
					db = 10;
				}
				else
					db -= 2;
			}

			break;

		case 8:
			if (CurrentSettings.conf.SfxVolume > 0)
			{
				static int db = 0;
				CurrentSettings.conf.SfxVolume--;
				GlobalFXVolume = CurrentSettings.conf.SfxVolume;
				if (!db)
				{
					SoundEffect(SFX_TR4_MENU_CHOOSE, NULL, 0);
					db = 10;
				}
				else
					db -= 2;
			}

			break;
		}
	}

	if (goRight)
	{
		switch (pause_selected_option)
		{
		case 1:
			SoundEffect(SFX_TR4_MENU_CHOOSE, NULL, 0);
			CurrentSettings.conf.EnableSound = !CurrentSettings.conf.EnableSound;
			break;

		case 2:
			SoundEffect(SFX_TR4_MENU_CHOOSE, NULL, 0);
			CurrentSettings.conf.EnableAudioSpecialEffects = !CurrentSettings.conf.EnableAudioSpecialEffects;
			break;

		case 4:
			if (CurrentSettings.conf.MusicVolume < 100)
			{
				static int db = 0;
				CurrentSettings.conf.MusicVolume++;
				GlobalMusicVolume = CurrentSettings.conf.MusicVolume;
				if (!db)
				{
					SoundEffect(SFX_TR4_MENU_CHOOSE, NULL, 0);
					db = 10;
				}
				else
					db -= 2;
			}

			break;

		case 8:
			if (CurrentSettings.conf.SfxVolume < 100)
			{
				static int db = 0;
				CurrentSettings.conf.SfxVolume++;
				GlobalFXVolume = CurrentSettings.conf.SfxVolume;
				if (!db)
				{
					SoundEffect(SFX_TR4_MENU_CHOOSE, NULL, 0);
					db = 10;
				}
				else
					db -= 2;
			}

			break;
		}
	}

	if (goUp)
	{
		if (pause_selected_option > 1)
			pause_selected_option >>= 1;

		SoundEffect(SFX_TR4_MENU_CHOOSE, 0, SFX_ALWAYS);
	}

	if (goDown)
	{
		if (pause_selected_option < pause_flag)
			pause_selected_option <<= 1;

		SoundEffect(SFX_TR4_MENU_CHOOSE, 0, SFX_ALWAYS);
	}

	if (goSelect)
	{
		SoundEffect(SFX_TR4_MENU_SELECT, NULL, 0);

		if (pause_selected_option & (1 << 4))
		{
			// Save the configuration
			GlobalMusicVolume = CurrentSettings.conf.MusicVolume;
			GlobalFXVolume = CurrentSettings.conf.SfxVolume;
			memcpy(&g_Configuration, &CurrentSettings.conf, sizeof(GameConfiguration));
			SaveAudioConfig();

			// Init or deinit the sound system
			if (wasSoundEnabled && !g_Configuration.EnableSound)
				Sound_DeInit();
			else if (!wasSoundEnabled && g_Configuration.EnableSound)
				Sound_Init();

			return;
		}

		if (pause_selected_option & (1 << 5))
		{
			SoundEffect(SFX_TR4_MENU_SELECT, NULL, 0);
			GlobalMusicVolume = oldVolume;
			GlobalFXVolume = oldSfxVolume;
			pause_menu_to_display = pause_options_menu;
			pause_selected_option = 4;
			return;
		}
	}
}

/*inventory*/

int do_these_objects_combine(int obj1, int obj2)
{
	for (int n = 0; n < max_combines; n++)
	{
		if (combine_table[n].item1 == obj1 &&
			combine_table[n].item2 == obj2)
			return 1;

		if (combine_table[n].item1 == obj2 &&
			combine_table[n].item2 == obj1)
			return 1;
	}

	return 0;
}

int is_item_currently_combinable(short obj)
{
	for (int n = 0; n < max_combines; n++)
	{
		if (combine_table[n].item1 == obj)
			if (have_i_got_item(combine_table[n].item2))
				return 1;

		if (combine_table[n].item2 == obj)
			if (have_i_got_item(combine_table[n].item1))
				return 1;
	}

	return 0;
}

int have_i_got_item(short obj)
{
	for (int i = 0; i < INVENTORY_TABLE_SIZE; i++)
		if (rings[RING_INVENTORY]->current_object_list[i].invitem == obj)
			return 1;

	return 0;
}

void combine_these_two_objects(short obj1, short obj2)
{
	int n;

	for (n = 0; n < max_combines; n++)
	{
		if (combine_table[n].item1 == obj1 &&
			combine_table[n].item2 == obj2)
			break;

		if (combine_table[n].item1 == obj2 &&
			combine_table[n].item2 == obj1)
			break;
	}

	combine_table[n].combine_routine(0);
	construct_object_list();
	setup_objectlist_startposition(combine_table[n].combined_item);
	handle_object_changeover(RING_INVENTORY);
}

void seperate_object(short obj)
{
	int n;

	for (n = 0; n < max_combines; n++)
		if (combine_table[n].combined_item == obj)
			break;

	combine_table[n].combine_routine(1);
	construct_object_list();
	setup_objectlist_startposition(combine_table[n].item1);
}

void setup_objectlist_startposition(short newobj)
{
	for (int i = 0; i < INVENTORY_TABLE_SIZE; i++)
		if (rings[RING_INVENTORY]->current_object_list[i].invitem == newobj)
			rings[RING_INVENTORY]->curobjinlist = i;
}

void handle_object_changeover(int ringnum)
{
	current_selected_option = 0;
	menu_active = 1;
	setup_ammo_selector();
}

void setup_ammo_selector()
{
	int num;
	int opts;

	num = 0;
	opts = options_table[rings[RING_INVENTORY]->current_object_list[rings[RING_INVENTORY]->curobjinlist].invitem];
	ammo_selector_flag = 0;
	num_ammo_slots = 0;

	if (rings[RING_AMMO]->ringactive)
		return;
	
	ammo_object_list[2].yrot = 0;
	ammo_object_list[1].yrot = 0;
	ammo_object_list[0].yrot = 0;

	if (opts & 
		(OPT_CHOOSEAMMO_UZI | OPT_CHOOSEAMMO_PISTOLS | OPT_CHOOSEAMMO_REVOLVER | OPT_CHOOSEAMMO_CROSSBOW |
		OPT_CHOOSEAMMO_HK | OPT_CHOOSEAMMO_SHOTGUN | OPT_CHOOSEAMMO_GRENADEGUN | OPT_CHOOSEAMMO_HARPOON | OPT_CHOOSEAMMO_ROCKET))
	{
		ammo_selector_flag = 1;
		ammo_selector_fade_dir = 1;

		if (opts & OPT_CHOOSEAMMO_UZI)
		{
			ammo_object_list[0].invitem = INV_OBJECT_UZI_AMMO;
			ammo_object_list[0].amount = AmountUziAmmo;
			num++;
			num_ammo_slots = num;
			current_ammo_type = &CurrentUziAmmoType;
		}

		if (opts & OPT_CHOOSEAMMO_PISTOLS)
		{
			num++;
			ammo_object_list[0].invitem = INV_OBJECT_PISTOLS_AMMO;
			ammo_object_list[0].amount = -1;
			num_ammo_slots = num;
			current_ammo_type = &CurrentPistolsAmmoType;
		}

		if (opts & OPT_CHOOSEAMMO_REVOLVER)
		{
			num++;
			ammo_object_list[0].invitem = INV_OBJECT_REVOLVER_AMMO;
			ammo_object_list[0].amount = AmountRevolverAmmo;
			num_ammo_slots = num;
			current_ammo_type = &CurrentRevolverAmmoType;
		}

		if (opts & OPT_CHOOSEAMMO_CROSSBOW)
		{
			current_ammo_type = &CurrentCrossBowAmmoType;
			ammo_object_list[num].invitem = INV_OBJECT_CROSSBOW_AMMO1;
			ammo_object_list[num].amount = AmountCrossBowAmmo1;
			num++;
			ammo_object_list[num].invitem = INV_OBJECT_CROSSBOW_AMMO2;
			ammo_object_list[num].amount = AmountCrossBowAmmo2;
			num++;
			ammo_object_list[num].invitem = INV_OBJECT_CROSSBOW_AMMO3;
			ammo_object_list[num].amount = AmountCrossBowAmmo3;
			num++;
			num_ammo_slots = num;
		}

		if (opts & OPT_CHOOSEAMMO_HK)
		{
			current_ammo_type = &CurrentHKAmmoType;
			ammo_object_list[num].invitem = INV_OBJECT_HK_AMMO;
			ammo_object_list[num].amount = AmountHKAmmo1;
			num++;
			num_ammo_slots = num;
		}

		if (opts & OPT_CHOOSEAMMO_SHOTGUN)
		{
			current_ammo_type = &CurrentShotGunAmmoType;
			ammo_object_list[num].invitem = INV_OBJECT_SHOTGUN_AMMO1;
			ammo_object_list[num].amount = AmountShotGunAmmo1;
			num++;
			ammo_object_list[num].invitem = INV_OBJECT_SHOTGUN_AMMO2;
			ammo_object_list[num].amount = AmountShotGunAmmo2;
			num++;
			num_ammo_slots = num;
		}

		if (opts & OPT_CHOOSEAMMO_GRENADEGUN)
		{
			current_ammo_type = &CurrentGrenadeGunAmmoType;
			ammo_object_list[num].invitem = INV_OBJECT_GRENADE_AMMO1;
			ammo_object_list[num].amount = AmountGrenadeAmmo1;
			num++;
			ammo_object_list[num].invitem = INV_OBJECT_GRENADE_AMMO2;
			ammo_object_list[num].amount = AmountGrenadeAmmo2;
			num++;
			ammo_object_list[num].invitem = INV_OBJECT_GRENADE_AMMO3;
			ammo_object_list[num].amount = AmountGrenadeAmmo3;
			num++;
			num_ammo_slots = num;
		}

		if (opts & OPT_CHOOSEAMMO_HARPOON)
		{
			current_ammo_type = &CurrentHarpoonAmmoType;
			ammo_object_list[num].invitem = INV_OBJECT_HARPOON_AMMO;
			ammo_object_list[num].amount = AmountHarpoonAmmo;
			num++;
			num_ammo_slots = num;
		}

		if (opts & OPT_CHOOSEAMMO_ROCKET)
		{
			current_ammo_type = &CurrentRocketAmmoType;
			ammo_object_list[num].invitem = INV_OBJECT_ROCKET_AMMO;
			ammo_object_list[num].amount = AmountRocketsAmmo;
			num++;
			num_ammo_slots = num;
		}
	}
}

void insert_object_into_list(int num)
{
	rings[RING_INVENTORY]->current_object_list[rings[RING_INVENTORY]->numobjectsinlist].invitem = num;
	rings[RING_INVENTORY]->current_object_list[rings[RING_INVENTORY]->numobjectsinlist].yrot = 0;
	rings[RING_INVENTORY]->current_object_list[rings[RING_INVENTORY]->numobjectsinlist].bright = 32;
	rings[RING_INVENTORY]->numobjectsinlist++;
}

void insert_object_into_list_v2(int num)
{
	if (options_table[num] & (OPT_COMBINABLE | OPT_ALWAYSCOMBINE))
	{
		if (rings[RING_INVENTORY]->current_object_list[rings[RING_INVENTORY]->curobjinlist].invitem != num)
		{
			rings[RING_AMMO]->current_object_list[rings[RING_AMMO]->numobjectsinlist].invitem = num;
			rings[RING_AMMO]->current_object_list[rings[RING_AMMO]->numobjectsinlist].yrot = 0;
			rings[RING_AMMO]->current_object_list[rings[RING_AMMO]->numobjectsinlist++].bright = 32;
		}
	}
}

void construct_object_list()
{
	rings[RING_INVENTORY]->numobjectsinlist = 0;

	for (int i = 0; i < INVENTORY_TABLE_SIZE; i++)
		rings[RING_INVENTORY]->current_object_list[i].invitem = NO_ITEM;

	CurrentPistolsAmmoType = 0;
	CurrentUziAmmoType = 0;
	CurrentRevolverAmmoType = 0;
	CurrentShotGunAmmoType = 0;
	CurrentGrenadeGunAmmoType = 0;
	CurrentCrossBowAmmoType = 0;

	if (!(g_GameFlow->GetLevel(CurrentLevel)->LaraType == LARA_YOUNG))
	{
		if (Lara.Weapons[WEAPON_PISTOLS].Present)
			insert_object_into_list(INV_OBJECT_PISTOLS);
		else if (AmountPistolsAmmo)
			insert_object_into_list(INV_OBJECT_PISTOLS_AMMO);

		if (Lara.Weapons[WEAPON_UZI].Present)
			insert_object_into_list(INV_OBJECT_UZIS);
		else if (AmountUziAmmo)
			insert_object_into_list(INV_OBJECT_UZI_AMMO);

		if (Lara.Weapons[WEAPON_REVOLVER].Present)
		{
			if (Lara.Weapons[WEAPON_REVOLVER].HasLasersight)
				insert_object_into_list(INV_OBJECT_REVOLVER_LASER);
			else
				insert_object_into_list(INV_OBJECT_REVOLVER);
		}
		else if (AmountRevolverAmmo)
			insert_object_into_list(INV_OBJECT_REVOLVER_AMMO);

		if (Lara.Weapons[WEAPON_SHOTGUN].Present)
		{
			insert_object_into_list(INV_OBJECT_SHOTGUN);

			if (Lara.Weapons[WEAPON_SHOTGUN].SelectedAmmo == WEAPON_AMMO2)
				CurrentShotGunAmmoType = 1;
		}
		else
		{
			if (AmountShotGunAmmo1)
				insert_object_into_list(INV_OBJECT_SHOTGUN_AMMO1);

			if (AmountShotGunAmmo2)
				insert_object_into_list(INV_OBJECT_SHOTGUN_AMMO2);
		}

		if (Lara.Weapons[WEAPON_HK].Present)
		{
			if (Lara.Weapons[WEAPON_HK].HasSilencer)
				insert_object_into_list(INV_OBJECT_HK_SILENCER);
			else
				insert_object_into_list(INV_OBJECT_HK);
		}
		else if (AmountHKAmmo1)
			insert_object_into_list(INV_OBJECT_HK_AMMO);

		if (Lara.Weapons[WEAPON_CROSSBOW].Present)
		{
				if (Lara.Weapons[WEAPON_CROSSBOW].HasLasersight)
					insert_object_into_list(INV_OBJECT_CROSSBOW_LASER);
				else
					insert_object_into_list(INV_OBJECT_CROSSBOW);

				if (Lara.Weapons[WEAPON_CROSSBOW].SelectedAmmo == WEAPON_AMMO2)
					CurrentCrossBowAmmoType = 1;

				if (Lara.Weapons[WEAPON_CROSSBOW].SelectedAmmo == WEAPON_AMMO3)
					CurrentCrossBowAmmoType = 2;
		}
		else
		{
			if (AmountCrossBowAmmo1)
				insert_object_into_list(INV_OBJECT_CROSSBOW_AMMO1);

			if (AmountCrossBowAmmo2)
				insert_object_into_list(INV_OBJECT_CROSSBOW_AMMO2);

			if (AmountCrossBowAmmo3)
				insert_object_into_list(INV_OBJECT_CROSSBOW_AMMO3);
		}

		if (Lara.Weapons[WEAPON_GRENADE_LAUNCHER].Present)
		{
			insert_object_into_list(INV_OBJECT_GRENADE_LAUNCHER);

			if (Lara.Weapons[WEAPON_CROSSBOW].SelectedAmmo == WEAPON_AMMO2)
				CurrentGrenadeGunAmmoType = 1;

			if (Lara.Weapons[WEAPON_CROSSBOW].SelectedAmmo == WEAPON_AMMO3)
				CurrentGrenadeGunAmmoType = 2;
		}
		else
		{
			if (AmountGrenadeAmmo1)
				insert_object_into_list(INV_OBJECT_GRENADE_AMMO1);

			if (AmountGrenadeAmmo2)
				insert_object_into_list(INV_OBJECT_GRENADE_AMMO2);

			if (AmountGrenadeAmmo3)
				insert_object_into_list(INV_OBJECT_GRENADE_AMMO3);
		}

		if (Lara.Weapons[WEAPON_ROCKET_LAUNCHER].Present)
			insert_object_into_list(INV_OBJECT_ROCKET_LAUNCHER);
		else if (AmountRocketsAmmo)
			insert_object_into_list(INV_OBJECT_ROCKET_AMMO);

		if (Lara.Weapons[WEAPON_HARPOON_GUN].Present)
			insert_object_into_list(INV_OBJECT_HARPOON_GUN);
		else if (AmountHarpoonAmmo)
			insert_object_into_list(INV_OBJECT_HARPOON_AMMO);

		if (Lara.Lasersight)
			insert_object_into_list(INV_OBJECT_LASERSIGHT);

		if (Lara.Silencer)
			insert_object_into_list(INV_OBJECT_SILENCER);

		if (Lara.Binoculars)
			insert_object_into_list(INV_OBJECT_BINOCULARS);

		if (Lara.NumFlares)
			insert_object_into_list(INV_OBJECT_FLARES);
	}

	insert_object_into_list(INV_OBJECT_TIMEX);//every level has the timex? what's a good way to check?!

	if (Lara.NumSmallMedipacks)
		insert_object_into_list(INV_OBJECT_SMALL_MEDIPACK);

	if (Lara.NumLargeMedipacks)
		insert_object_into_list(INV_OBJECT_LARGE_MEDIPACK);

	if (Lara.Crowbar)
		insert_object_into_list(INV_OBJECT_CROWBAR);

	if (Lara.hasBeetleThings)
	{
		if (Lara.hasBeetleThings & 1)
			insert_object_into_list(INV_OBJECT_BEETLE);

		if (Lara.hasBeetleThings & 2)
			insert_object_into_list(INV_OBJECT_BEETLE_PART1);

		if (Lara.hasBeetleThings & 4)
			insert_object_into_list(INV_OBJECT_BEETLE_PART2);
	}

	for (int i = 0; i < 8; i++)
		if (Lara.Puzzles[i])
			insert_object_into_list(INV_OBJECT_PUZZLE1 + i);

	for (int i = 0; i < 16; i++)
		if (Lara.PuzzlesCombo[i])
			insert_object_into_list(INV_OBJECT_PUZZLE1_COMBO1 + i);

	for (int i = 0; i < 8; i++)
		if (Lara.Keys[i])
			insert_object_into_list(INV_OBJECT_KEY1 + i);

	for (int i = 0; i < 16; i++)
		if (Lara.KeysCombo[i])
			insert_object_into_list(INV_OBJECT_KEY1_COMBO1 + i);

	for (int i = 0; i < 4; i++)
		if (Lara.Pickups[i])
			insert_object_into_list(INV_OBJECT_PICKUP1 + i);

	for (int i = 0; i < 8; i++)
		if (Lara.PickupsCombo[i])
			insert_object_into_list(INV_OBJECT_PICKUP1_COMBO1 + i);

	for (int i = 0; i < 3; i++)
		if (Lara.Examines[i])
			insert_object_into_list(INV_OBJECT_EXAMINE1 + i);

	for (int i = 0; i < 6; i++)
		if (Lara.ExaminesCombo[i])
			insert_object_into_list(INV_OBJECT_EXAMINE1_COMBO1 + i);

	if (Lara.Diary.Present)
		insert_object_into_list(INV_OBJECT_DIARY);

	if (g_GameFlow->EnableLoadSave)
	{
		insert_object_into_list(INV_OBJECT_LOAD_FLOPPY);
		insert_object_into_list(INV_OBJECT_SAVE_FLOPPY);
	}

	rings[RING_INVENTORY]->objlistmovement = 0;
	rings[RING_INVENTORY]->curobjinlist = 0;
	rings[RING_INVENTORY]->ringactive = 1;
	rings[RING_AMMO]->objlistmovement = 0;
	rings[RING_AMMO]->curobjinlist = 0;
	rings[RING_AMMO]->ringactive = 0;
	handle_object_changeover(RING_INVENTORY);
	ammo_active = 0;
}

void construct_combine_object_list()
{
	rings[RING_AMMO]->numobjectsinlist = 0;

	for (int i = 0; i < INVENTORY_TABLE_SIZE; i++)
		rings[RING_AMMO]->current_object_list[i].invitem = NO_ITEM;

	if (!(g_GameFlow->GetLevel(CurrentLevel)->LaraType == LARA_YOUNG))
	{
		if (Lara.Weapons[WEAPON_REVOLVER].Present)
		{
			if (Lara.Weapons[WEAPON_REVOLVER].HasLasersight)
				insert_object_into_list_v2(INV_OBJECT_REVOLVER_LASER);
			else
				insert_object_into_list_v2(INV_OBJECT_REVOLVER);
		}

		if (Lara.Weapons[WEAPON_HK].Present)
			insert_object_into_list_v2(INV_OBJECT_HK);

		if (Lara.Weapons[WEAPON_CROSSBOW].Present)
		{
			if (Lara.Weapons[WEAPON_CROSSBOW].HasLasersight)
				insert_object_into_list_v2(INV_OBJECT_CROSSBOW_LASER);
			else
				insert_object_into_list_v2(INV_OBJECT_CROSSBOW);
		}

		if (Lara.Lasersight)
			insert_object_into_list_v2(INV_OBJECT_LASERSIGHT);

		if (Lara.Silencer)
			insert_object_into_list_v2(INV_OBJECT_SILENCER);
	}

	if (Lara.hasBeetleThings)
	{
		if (Lara.hasBeetleThings & 2)
			insert_object_into_list_v2(INV_OBJECT_BEETLE_PART1);

		if (Lara.hasBeetleThings & 4)
			insert_object_into_list_v2(INV_OBJECT_BEETLE_PART2);
	}

	for (int i = 0; i < 16; i++)
		if (Lara.PuzzlesCombo[i])
			insert_object_into_list_v2(INV_OBJECT_PUZZLE1_COMBO1 + i);

	for (int i = 0; i < 16; i++)
		if (Lara.KeysCombo[i])
			insert_object_into_list_v2(INV_OBJECT_KEY1_COMBO1 + i);

	for (int i = 0; i < 8; i++)
		if (Lara.PickupsCombo[i])
			insert_object_into_list_v2(INV_OBJECT_PICKUP1_COMBO1 + i);

	rings[RING_AMMO]->objlistmovement = 0;
	rings[RING_AMMO]->curobjinlist = 0;
	rings[RING_AMMO]->ringactive = 0;
}

void init_inventry()
{
	compassNeedleAngle = 4096;
	examine_mode = 0;
	stats_mode = 0;
	AlterFOV(14560);
	Lara.busy = 0;
	GLOBAL_inventoryitemchosen = NO_ITEM;
	clear_input_vars(0);
	loading_or_saving = 0;
	useItem = 0;

	if (Lara.Weapons[WEAPON_SHOTGUN].Ammo[0].getCount() == -1)
		AmountShotGunAmmo1 = -1;
	else
		AmountShotGunAmmo1 = Lara.Weapons[WEAPON_SHOTGUN].Ammo[0].getCount() / 6;

	if (Lara.Weapons[WEAPON_SHOTGUN].Ammo[1].getCount() == -1)
		AmountShotGunAmmo2 = -1;
	else
		AmountShotGunAmmo2 = Lara.Weapons[WEAPON_SHOTGUN].Ammo[1].getCount() / 6;

	AmountHKAmmo1 = Lara.Weapons[WEAPON_HK].Ammo[WEAPON_AMMO1].hasInfinite() ? -1 : Lara.Weapons[WEAPON_HK].Ammo[WEAPON_AMMO1].getCount();
	AmountCrossBowAmmo1 = Lara.Weapons[WEAPON_CROSSBOW].Ammo[WEAPON_AMMO1].hasInfinite() ? -1 : Lara.Weapons[WEAPON_CROSSBOW].Ammo[WEAPON_AMMO1].getCount();
	AmountCrossBowAmmo2 = Lara.Weapons[WEAPON_CROSSBOW].Ammo[WEAPON_AMMO2].hasInfinite() ? -1 : Lara.Weapons[WEAPON_CROSSBOW].Ammo[WEAPON_AMMO2].getCount();
	AmountCrossBowAmmo3 = Lara.Weapons[WEAPON_CROSSBOW].Ammo[WEAPON_AMMO3].hasInfinite() ? -1 : Lara.Weapons[WEAPON_CROSSBOW].Ammo[WEAPON_AMMO3].getCount();
	AmountUziAmmo = Lara.Weapons[WEAPON_UZI].Ammo[WEAPON_AMMO1].hasInfinite() ? -1 : Lara.Weapons[WEAPON_UZI].Ammo[WEAPON_AMMO1].getCount();
	AmountRevolverAmmo = Lara.Weapons[WEAPON_REVOLVER].Ammo[WEAPON_AMMO1].hasInfinite() ? -1 : Lara.Weapons[WEAPON_REVOLVER].Ammo[WEAPON_AMMO1].getCount();
	AmountPistolsAmmo = Lara.Weapons[WEAPON_PISTOLS].Ammo[WEAPON_AMMO1].hasInfinite() ? -1 : Lara.Weapons[WEAPON_PISTOLS].Ammo[WEAPON_AMMO1].getCount();
	AmountRocketsAmmo = Lara.Weapons[WEAPON_ROCKET_LAUNCHER].Ammo[WEAPON_AMMO1].hasInfinite() ? -1 : Lara.Weapons[WEAPON_ROCKET_LAUNCHER].Ammo[WEAPON_AMMO1].getCount();
	AmountHarpoonAmmo = Lara.Weapons[WEAPON_HARPOON_GUN].Ammo[WEAPON_AMMO1].hasInfinite()? -1 : Lara.Weapons[WEAPON_HARPOON_GUN].Ammo[WEAPON_AMMO1].getCount();
	AmountGrenadeAmmo1 = Lara.Weapons[WEAPON_GRENADE_LAUNCHER].Ammo[WEAPON_AMMO1].hasInfinite()? -1 : Lara.Weapons[WEAPON_GRENADE_LAUNCHER].Ammo[WEAPON_AMMO1].getCount();
	AmountGrenadeAmmo2 = Lara.Weapons[WEAPON_GRENADE_LAUNCHER].Ammo[WEAPON_AMMO2].hasInfinite() ? -1 : Lara.Weapons[WEAPON_GRENADE_LAUNCHER].Ammo[WEAPON_AMMO2].getCount();
	AmountGrenadeAmmo3 = Lara.Weapons[WEAPON_GRENADE_LAUNCHER].Ammo[WEAPON_AMMO3].hasInfinite() ? -1 : Lara.Weapons[WEAPON_GRENADE_LAUNCHER].Ammo[WEAPON_AMMO3].getCount();
	construct_object_list();

	if (GLOBAL_enterinventory == NO_ITEM)
	{
		if (GLOBAL_lastinvitem != NO_ITEM)
		{
			if (have_i_got_item(GLOBAL_lastinvitem))
				setup_objectlist_startposition(GLOBAL_lastinvitem);

			GLOBAL_lastinvitem = NO_ITEM;
		}
	}
	else
	{
		if (have_i_got_object(GLOBAL_enterinventory))
			setup_objectlist_startposition2(GLOBAL_enterinventory);

		GLOBAL_enterinventory = NO_ITEM;
	}

	ammo_selector_fade_val = 0;
	ammo_selector_fade_dir = 0;
	combine_ring_fade_val = 0;
	combine_ring_fade_dir = 0;
	combine_type_flag = 0;
	seperate_type_flag = 0;
	combine_obj1 = 0;
	combine_obj2 = 0;
	normal_ring_fade_val = 128;
	normal_ring_fade_dir = 0;
	handle_object_changeover(RING_INVENTORY);
}

int have_i_got_object(short object_number)
{
	if (object_number >= ID_PUZZLE_ITEM1_COMBO1 && object_number <= ID_PUZZLE_ITEM8_COMBO2)
		return Lara.PuzzlesCombo[object_number - ID_PUZZLE_ITEM1_COMBO1];

	if (object_number >= ID_PUZZLE_ITEM1 && object_number <= ID_PUZZLE_ITEM8)
		return Lara.Puzzles[object_number - ID_PUZZLE_ITEM1];

	if (object_number >= ID_KEY_ITEM1_COMBO1 && object_number <= ID_KEY_ITEM8_COMBO2)
		return Lara.KeysCombo[object_number - ID_KEY_ITEM1_COMBO1];

	if (object_number >= ID_KEY_ITEM1 && object_number <= ID_KEY_ITEM8)
		return Lara.Keys[object_number - ID_KEY_ITEM1];

	if (object_number >= ID_PICKUP_ITEM1_COMBO1 && object_number <= ID_PICKUP_ITEM4_COMBO2)
		return Lara.PickupsCombo[object_number - ID_PICKUP_ITEM1_COMBO1];

	if (object_number >= ID_PICKUP_ITEM1 && object_number <= ID_PICKUP_ITEM4)
		return Lara.Pickups[object_number - ID_PICKUP_ITEM1];

	if (object_number == ID_CROWBAR_ITEM)
		return Lara.Crowbar;

	return 0;
}

void setup_objectlist_startposition2(short newobj)
{
	for (int i = 0; i < INVENTORY_TABLE_SIZE; i++)
		if (inventry_objects_list[rings[RING_INVENTORY]->current_object_list[i].invitem].object_number == newobj)
			rings[RING_INVENTORY]->curobjinlist = i;
}

int convert_obj_to_invobj(short obj)
{
	for (int i = 0; i < INVENTORY_TABLE_SIZE; i++)
	{
		if (inventry_objects_list[i].object_number == obj)
			return i;
	}

	return 27;
}

int convert_invobj_to_obj(int obj)
{
	return inventry_objects_list[obj].object_number;
}

void fade_ammo_selector()
{
	if (rings[RING_INVENTORY]->ringactive && (rptLeft >= 8 || rptRight >= 8))
		ammo_selector_fade_val = 0;
	else if (ammo_selector_fade_dir == 1)
	{
		if (ammo_selector_fade_val < 128)
			ammo_selector_fade_val += 32;

		if (ammo_selector_fade_val > 128)
		{
			ammo_selector_fade_val = 128;
			ammo_selector_fade_dir = 0;
		}
	}
	else if (ammo_selector_fade_dir == 2)
	{
		if (ammo_selector_fade_val > 0)
			ammo_selector_fade_val -= 32;

		if (ammo_selector_fade_val < 0)
		{
			ammo_selector_fade_val = 0;
			ammo_selector_fade_dir = 0;
		}
	}
}

void use_current_item()
{
	short invobject, gmeobject;
	long OldBinocular;

	OldBinocular = BinocularRange;
	OldLaraBusy = 0;
	BinocularRange = 0;
	LaraItem->meshBits = -1;
	invobject = rings[RING_INVENTORY]->current_object_list[rings[RING_INVENTORY]->curobjinlist].invitem;
	gmeobject = inventry_objects_list[invobject].object_number;

	if (Lara.waterStatus == LW_ABOVE_WATER || Lara.waterStatus == LW_WADE)
	{
		if (gmeobject == ID_PISTOLS_ITEM)
		{
			Lara.requestGunType = WEAPON_PISTOLS;

			if (Lara.gunStatus != LG_NO_ARMS)
				return;

			if (Lara.gunType == WEAPON_PISTOLS)
				Lara.gunStatus = LG_DRAW_GUNS;

			return;
		}

		if (gmeobject == ID_UZI_ITEM)
		{
			Lara.requestGunType = WEAPON_UZI;

			if (Lara.gunStatus != LG_NO_ARMS)
				return;

			if (Lara.gunType == WEAPON_UZI)
				Lara.gunStatus = LG_DRAW_GUNS;

			return;
		}
	}

	if (gmeobject != ID_SHOTGUN_ITEM && gmeobject != ID_REVOLVER_ITEM && gmeobject != ID_HK_ITEM && gmeobject != ID_CROSSBOW_ITEM &&
		gmeobject != ID_GRENADE_GUN_ITEM && gmeobject != ID_ROCKET_LAUNCHER_ITEM && gmeobject != ID_HARPOON_ITEM)
	{
		if (gmeobject == ID_FLARE_INV_ITEM)
		{
			if (Lara.gunStatus == LG_NO_ARMS)
			{
				if (LaraItem->currentAnimState != LS_CRAWL_IDLE &&
					LaraItem->currentAnimState != LS_CRAWL_FORWARD &&
					LaraItem->currentAnimState != LS_CRAWL_TURN_LEFT &&
					LaraItem->currentAnimState != LS_CRAWL_TURN_RIGHT &&
					LaraItem->currentAnimState != LS_CRAWL_BACK &&
					LaraItem->currentAnimState != LS_CRAWL_TO_HANG)
				{
					if (Lara.gunType != WEAPON_FLARE)
					{
						TrInput = IN_FLARE;
						LaraGun();
						TrInput = 0;
					}

					return;
				}
			}

			SayNo();
			return;
		}

		switch (invobject)
		{
		case INV_OBJECT_BINOCULARS:

			if ((LaraItem->currentAnimState == LS_STOP && LaraItem->animNumber == LA_STAND_IDLE
				|| Lara.isDucked && !(TrInput & IN_DUCK))
			//	&& !SniperCamActive
				&& !UseSpotCam
				&& !TrackCameraInit)
			{
				OldLaraBusy = 1;
				BinocularRange = 128;

				if (Lara.gunStatus != LG_NO_ARMS)
					Lara.gunStatus = LG_UNDRAW_GUNS;
			}

			if (OldBinocular)
				BinocularRange = OldBinocular;
			else
				BinocularOldCamera = Camera.oldType;

			return;

		case INV_OBJECT_SMALL_MEDIPACK:

			if ((LaraItem->hitPoints <= 0 || LaraItem->hitPoints >= 1000) && !Lara.poisoned)
			{
				SayNo();
				return;
			}

			if (Lara.NumSmallMedipacks != 0)
			{
				if (Lara.NumSmallMedipacks != -1)
					Lara.NumSmallMedipacks--;

				Lara.dpoisoned = 0;
				LaraItem->hitPoints += 500;

				if (LaraItem->hitPoints > 1000)
					LaraItem->hitPoints = 1000;

				SoundEffect(SFX_TR4_MENU_MEDI, 0, SFX_ALWAYS);
				Savegame.Game.HealthUsed++;
			}
			else
				SayNo();

			return;

		case INV_OBJECT_LARGE_MEDIPACK:

			if ((LaraItem->hitPoints <= 0 || LaraItem->hitPoints >= 1000) && !Lara.poisoned)
			{
				SayNo();
				return;
			}

			if (Lara.NumLargeMedipacks != 0)
			{
				if (Lara.NumLargeMedipacks != -1)
					Lara.NumLargeMedipacks--;

				Lara.dpoisoned = 0;
				LaraItem->hitPoints = 1000;

				SoundEffect(SFX_TR4_MENU_MEDI, 0, SFX_ALWAYS);
				Savegame.Game.HealthUsed++;
			}
			else
				SayNo();

			return;

		default:
			GLOBAL_inventoryitemchosen = gmeobject;
			return;
		}

		return;
	}

	if (Lara.gunStatus == LG_HANDS_BUSY)
	{
		SayNo();
		return;
	}

	if (LaraItem->currentAnimState == LS_CRAWL_IDLE ||
		LaraItem->currentAnimState == LS_CRAWL_FORWARD ||
		LaraItem->currentAnimState == LS_CRAWL_TURN_LEFT ||
		LaraItem->currentAnimState == LS_CRAWL_TURN_RIGHT ||
		LaraItem->currentAnimState == LS_CRAWL_BACK ||
		LaraItem->currentAnimState == LS_CRAWL_TO_HANG ||
		LaraItem->currentAnimState == LS_CROUCH_IDLE ||
		LaraItem->currentAnimState == LS_CROUCH_TURN_LEFT ||
		LaraItem->currentAnimState == LS_CROUCH_TURN_RIGHT)
	{
		SayNo();
		return;
	}

	if (gmeobject == ID_SHOTGUN_ITEM)
	{
		Lara.requestGunType = WEAPON_SHOTGUN;

		if (Lara.gunStatus != LG_NO_ARMS)
			return;

		if (Lara.gunType == WEAPON_SHOTGUN)
			Lara.gunStatus = LG_DRAW_GUNS;

		return;
	}

	if (gmeobject == ID_REVOLVER_ITEM)
	{
		Lara.requestGunType = WEAPON_REVOLVER;

		if (Lara.gunStatus != LG_NO_ARMS)
			return;

		if (Lara.gunType == WEAPON_REVOLVER)
			Lara.gunStatus = LG_DRAW_GUNS;

		return;
	}
	else if (gmeobject == ID_HK_ITEM)
	{
		Lara.requestGunType = WEAPON_HK;

		if (Lara.gunStatus != LG_NO_ARMS)
			return;

		if (Lara.gunType == WEAPON_HK)
			Lara.gunStatus = LG_DRAW_GUNS;

		return;
	}
	else if (gmeobject == ID_CROSSBOW_ITEM)
	{
		Lara.requestGunType = WEAPON_CROSSBOW;

		if (Lara.gunStatus != LG_NO_ARMS)
			return;

		if (Lara.gunType == WEAPON_CROSSBOW)
			Lara.gunStatus = LG_DRAW_GUNS;

		return;
	}
	else if (gmeobject == ID_GRENADE_GUN_ITEM)
	{
		Lara.requestGunType = WEAPON_GRENADE_LAUNCHER;

		if (Lara.gunStatus != LG_NO_ARMS)
			return;

		if (Lara.gunType == WEAPON_GRENADE_LAUNCHER)
			Lara.gunStatus = LG_DRAW_GUNS;

		return;
	}
	else if (gmeobject == ID_HARPOON_ITEM)
	{
		Lara.requestGunType = WEAPON_HARPOON_GUN;

		if (Lara.gunStatus != LG_NO_ARMS)
			return;

		if (Lara.gunType == WEAPON_HARPOON_GUN)
			Lara.gunStatus = LG_DRAW_GUNS;

		return;
	}
	else if (gmeobject == ID_ROCKET_LAUNCHER_ITEM)
	{
		Lara.requestGunType = WEAPON_ROCKET_LAUNCHER;

		if (Lara.gunStatus != LG_NO_ARMS)
			return;

		if (Lara.gunType == WEAPON_ROCKET_LAUNCHER)
			Lara.gunStatus = LG_DRAW_GUNS;

		return;
	}
}

void handle_inventry_menu()
{
	int n;
	int opts;
	int i;
	int ypos;
	int num;

	if (rings[RING_AMMO]->ringactive)
	{
		g_Renderer.drawString(phd_centerx, phd_centery, g_GameFlow->GetString(optmessages[5]), PRINTSTRING_COLOR_WHITE, PRINTSTRING_BLINK | PRINTSTRING_CENTER);
	//	PrintString(phd_centerx, phd_centery, 1, &gfStringWad[gfStringOffset[optmessages[5]]], FF_CENTER);

		if (rings[RING_INVENTORY]->objlistmovement)
			return;

		if (rings[RING_AMMO]->objlistmovement)
			return;

		if (goSelect)
		{
			if (do_these_objects_combine(rings[RING_INVENTORY]->current_object_list[rings[RING_INVENTORY]->curobjinlist].invitem, rings[RING_AMMO]->current_object_list[rings[RING_AMMO]->curobjinlist].invitem))
			{
				combine_ring_fade_dir = 2;
				combine_type_flag = 1;
				combine_obj1 = rings[RING_INVENTORY]->current_object_list[rings[RING_INVENTORY]->curobjinlist].invitem;
				combine_obj2 = rings[RING_AMMO]->current_object_list[rings[RING_AMMO]->curobjinlist].invitem;
				SoundEffect(SFX_TR4_MENU_COMBINE, 0, SFX_ALWAYS);
			}
			else
			{
				SayNo();
				combine_ring_fade_dir = 2;
			}
		}

		if (goDeselect)
		{
			SoundEffect(SFX_TR4_MENU_SELECT, 0, SFX_ALWAYS);
			combine_ring_fade_dir = 2;
			goDeselect = 0;
		}

		return;
	}
	else
	{
		num = rings[RING_INVENTORY]->current_object_list[rings[RING_INVENTORY]->curobjinlist].invitem;

		for (n = 0; n < 3; n++)
		{
			current_options[n].type = 0;
			current_options[n].text = 0;
		}

		n = 0;

		if (!ammo_active)
		{
			opts = options_table[rings[RING_INVENTORY]->current_object_list[rings[RING_INVENTORY]->curobjinlist].invitem];

			if ((opts & OPT_LOAD))
			{
				current_options[0].type = 9;
				current_options[0].text = g_GameFlow->GetString(optmessages[6]);
				n = 1;
			}

			if ((opts & OPT_SAVE))
			{
				current_options[n].type = 10;
				current_options[n].text = g_GameFlow->GetString(optmessages[7]);
				n++;
			}

			if ((opts & OPT_EXAMINABLE))
			{
				current_options[n].type = 11;
				current_options[n].text = g_GameFlow->GetString(optmessages[8]);
				n++;
			}

			if ((opts & OPT_STATS))
			{
				current_options[n].type = 12;
				current_options[n].text = g_GameFlow->GetString(optmessages[9]);
				n++;
			}

			if ((opts & OPT_USE))
			{
				current_options[n].type = 1;
				current_options[n].text = g_GameFlow->GetString(optmessages[0]);
				n++;
			}

			if ((opts & OPT_EQUIP))
			{
				current_options[n].type = 5;
				current_options[n].text = g_GameFlow->GetString(optmessages[4]);
				n++;
			}

			if ((opts & (OPT_CHOOSEAMMO_SHOTGUN | OPT_CHOOSEAMMO_CROSSBOW | OPT_CHOOSEAMMO_GRENADEGUN)))
			{
				current_options[n].type = 2;
				current_options[n].text = g_GameFlow->GetString(optmessages[1]);
				n++;
			}

			if ((opts & OPT_COMBINABLE))
			{
				if (is_item_currently_combinable(num))
				{
					current_options[n].type = 3;
					current_options[n].text = g_GameFlow->GetString(optmessages[2]);
					n++;
				}
			}

			if ((opts & OPT_ALWAYSCOMBINE))
			{
				current_options[n].type = 3;
				current_options[n].text = g_GameFlow->GetString(optmessages[2]);
				n++;
			}

			if ((opts & OPT_SEPERATABLE))
			{
				current_options[n].type = 4;
				current_options[n].text = g_GameFlow->GetString(optmessages[3]);
				n++;
			}
		}
		else
		{
			current_options[0].type = 6;
			current_options[0].text = g_GameFlow->GetString(inventry_objects_list[ammo_object_list[0].invitem].objname);
			current_options[1].type = 7;
			current_options[1].text = g_GameFlow->GetString(inventry_objects_list[ammo_object_list[1].invitem].objname);
			n = 2;

			if ((options_table[rings[RING_INVENTORY]->current_object_list[rings[RING_INVENTORY]->curobjinlist].invitem] & (OPT_CHOOSEAMMO_CROSSBOW | OPT_CHOOSEAMMO_GRENADEGUN)))
			{
				n = 3;
				current_options[2].type = 8;
				current_options[2].text = g_GameFlow->GetString(inventry_objects_list[ammo_object_list[2].invitem].objname);
			}

			current_selected_option = current_ammo_type[0];
		}

		ypos = 310 - font_height;

		if (n == 1)
			ypos += font_height;
		else if (n == 2)
			ypos += font_height >> 1;

		if (n > 0)
		{
			for (i = 0; i < n; i++)
			{
				if (i == current_selected_option)
				{
					g_Renderer.drawString(phd_centerx, ypos, current_options[i].text, PRINTSTRING_COLOR_WHITE, PRINTSTRING_BLINK | PRINTSTRING_CENTER);
				//	PrintString(phd_centerx, ypos, 1, current_options[i].text, FF_CENTER);
					ypos += font_height;
				}
				else
				{
					g_Renderer.drawString(phd_centerx, ypos, current_options[i].text, PRINTSTRING_COLOR_WHITE, PRINTSTRING_CENTER);
				//	PrintString(phd_centerx, ypos, 5, current_options[i].text, FF_CENTER);
					ypos += font_height;
				}
			}
		}

		if (menu_active && !rings[RING_INVENTORY]->objlistmovement && !rings[RING_AMMO]->objlistmovement)
		{
			if (goUp && current_selected_option > 0)
			{
				current_selected_option--;
				SoundEffect(SFX_TR4_MENU_SELECT, 0, SFX_ALWAYS);
			}
			else if (goDown && current_selected_option < n - 1)
			{
				current_selected_option++;
				SoundEffect(SFX_TR4_MENU_SELECT, 0, SFX_ALWAYS);
			}

			if (ammo_active)
			{
				if (goLeft && current_selected_option > 0)
				{
					current_selected_option--;
					SoundEffect(SFX_TR4_MENU_SELECT, 0, SFX_ALWAYS);
				}

				if (goRight && current_selected_option < n - 1)
				{
					current_selected_option++;
					SoundEffect(SFX_TR4_MENU_SELECT, 0, SFX_ALWAYS);
				}

				current_ammo_type[0] = current_selected_option;
			}

			if (goSelect)
			{
				if (current_options[current_selected_option].type != 5 && current_options[current_selected_option].type != 1)
					SoundEffect(SFX_TR4_MENU_CHOOSE, 0, SFX_ALWAYS);

				switch (current_options[current_selected_option].type)
				{
				case 2:
					rings[RING_INVENTORY]->ringactive = 0;
					ammo_active = 1;
					Stashedcurrent_selected_option = current_selected_option;
					StashedCurrentPistolsAmmoType = CurrentPistolsAmmoType;
					StashedCurrentUziAmmoType = CurrentUziAmmoType;
					StashedCurrentRevolverAmmoType = CurrentRevolverAmmoType;
					StashedCurrentShotGunAmmoType = CurrentShotGunAmmoType;
					StashedCurrentGrenadeGunAmmoType = CurrentGrenadeGunAmmoType;
					StashedCurrentCrossBowAmmoType = CurrentCrossBowAmmoType;
					StashedCurrentHKAmmoType = CurrentHKAmmoType;
					StashedCurrentHarpoonAmmoType = CurrentHarpoonAmmoType;
					StashedCurrentRocketAmmoType = CurrentRocketAmmoType;
					break;

				case 9:
					loading_or_saving = 1;
					break;

				case 10:
					loading_or_saving = 2;
					break;

				case 11:
					examine_mode = 1;
					break;

				case 12:
					stats_mode = 1;
					break;

				case 6:
				case 7:
				case 8:
					ammo_active = 0;
					rings[RING_INVENTORY]->ringactive = 1;
					current_selected_option = 0;
					break;

				case 3:
					construct_combine_object_list();
					rings[RING_INVENTORY]->ringactive = 0;
					rings[RING_AMMO]->ringactive = 1;
					ammo_selector_flag = 0;
					menu_active = 0;
					combine_ring_fade_dir = 1;
					break;

				case 4:
					seperate_type_flag = 1;
					normal_ring_fade_dir = 2;
					break;

				case 5:
				case 1:
					menu_active = 0;
					useItem = 1;
					break;
				}
			}

			if (goDeselect && ammo_active)
			{
				SoundEffect(SFX_TR4_MENU_SELECT, 0, SFX_ALWAYS);
				goDeselect = 0;
				ammo_active = 0;
				rings[RING_INVENTORY]->ringactive = 1;
				CurrentPistolsAmmoType = StashedCurrentPistolsAmmoType;
				CurrentUziAmmoType = StashedCurrentUziAmmoType;
				CurrentRevolverAmmoType = StashedCurrentRevolverAmmoType;
				CurrentShotGunAmmoType = StashedCurrentShotGunAmmoType;
				CurrentGrenadeGunAmmoType = StashedCurrentGrenadeGunAmmoType;
				CurrentCrossBowAmmoType = StashedCurrentCrossBowAmmoType;
				CurrentHKAmmoType = StashedCurrentHKAmmoType;
				CurrentHarpoonAmmoType = StashedCurrentHarpoonAmmoType;
				CurrentRocketAmmoType = StashedCurrentRocketAmmoType;
				current_selected_option = Stashedcurrent_selected_option;
			}
		}
	}
}

//this function is to UPDATE THE SELECTED AMMO OF WEPS THAT REQUIRE DOING SO, and only these..
void update_laras_weapons_status()
{
	if (Lara.Weapons[WEAPON_SHOTGUN].Present)
	{
		if (CurrentShotGunAmmoType)
			Lara.Weapons[WEAPON_SHOTGUN].SelectedAmmo = WEAPON_AMMO2;
		else
			Lara.Weapons[WEAPON_SHOTGUN].SelectedAmmo = WEAPON_AMMO1;
	}

	if (Lara.Weapons[WEAPON_CROSSBOW].Present)
	{
		Lara.Weapons[WEAPON_CROSSBOW].SelectedAmmo = WEAPON_AMMO1;

		if (CurrentCrossBowAmmoType == 1)
			Lara.Weapons[WEAPON_CROSSBOW].SelectedAmmo = WEAPON_AMMO2;
		else if (CurrentCrossBowAmmoType == 2)
			Lara.Weapons[WEAPON_CROSSBOW].SelectedAmmo = WEAPON_AMMO3;
	}

	if (Lara.Weapons[WEAPON_GRENADE_LAUNCHER].Present)
	{
		Lara.Weapons[WEAPON_GRENADE_LAUNCHER].SelectedAmmo = WEAPON_AMMO1;

		if (CurrentGrenadeGunAmmoType == 1)
			Lara.Weapons[WEAPON_GRENADE_LAUNCHER].SelectedAmmo = WEAPON_AMMO2;
		else if (CurrentGrenadeGunAmmoType == 2)
			Lara.Weapons[WEAPON_GRENADE_LAUNCHER].SelectedAmmo = WEAPON_AMMO3;
	}
}

void spinback(unsigned short* angle)
{
	unsigned short val;
	unsigned short val2;

	val = *angle;

	if (val)
	{
		if (val <= 32768)
		{
			val2 = val;

			if (val2 < 1022)
				val = 1022;
			else if (val2 > 16384)
				val2 = 16384;

			val -= (val2 >> 3);

			if (val > 32768)
				val = 0;
		}
		else
		{
			val2 = -val;

			if (val2 < 1022)
				val = 1022;
			else if (val2 > 16384)
				val2 = 16384;

			val += (val2 >> 3);

			if (val < 32768)
				val = 0;
		}

		*angle = val;
	}
}

void draw_ammo_selector()
{
	int n;
	int xpos;
	short yrot;
	INVOBJ* objme;
	char invTextBuffer[256];
	int x, y;

	if (!ammo_selector_flag)
		return;
	
	xpos = (2 * phd_centerx - OBJLIST_SPACING) >> 1;

	if (num_ammo_slots == 2)
		xpos -= OBJLIST_SPACING / 2;
	else if (num_ammo_slots == 3)
		xpos -= OBJLIST_SPACING;

	if (num_ammo_slots > 0)
	{
		for (n = 0; n < num_ammo_slots; n++)
		{
			objme = &inventry_objects_list[ammo_object_list[n].invitem];

			if (n == current_ammo_type[0])
			{
				if ((objme->flags & 2))
					ammo_object_list[n].yrot += 1022;
			}
			else
				spinback(&ammo_object_list[n].yrot);

			yrot = ammo_object_list[n].yrot;
			x = phd_centerx - 300 + xpos;
			y = 430;
			short obj = convert_invobj_to_obj(ammo_object_list[n].invitem);

			if (n == current_ammo_type[0])
			{
				if (ammo_object_list[n].amount == -1)
					sprintf(&invTextBuffer[0], "Unlimited %s", g_GameFlow->GetString(inventry_objects_list[ammo_object_list[n].invitem].objname));
				else
					sprintf(&invTextBuffer[0], "%d x %s", ammo_object_list[n].amount, g_GameFlow->GetString(inventry_objects_list[ammo_object_list[n].invitem].objname));

				if (ammo_selector_fade_val)
					g_Renderer.drawString(phd_centerx, 380, &invTextBuffer[0], PRINTSTRING_COLOR_YELLOW, PRINTSTRING_CENTER);
				//		PrintString(phd_centerx, font_height + phd_centery + 2 * font_height - 9, 8, &invTextBuffer[0], FF_CENTER);

				
				if (n == current_ammo_type[0])
					//g_Renderer.drawObjectOn2DPosition(x, y, ammo_object_list[n].invitem, ammo_selector_fade_val, 0, yrot, 0, 0, 0);
					g_Renderer.drawObjectOn2DPosition(x, y, obj, 0, yrot, 0);
				else
					//DrawThreeDeeObject2D(x, y, ammo_object_list[n].invitem, ammo_selector_fade_val, 0, yrot, 0, 1, 0);
					g_Renderer.drawObjectOn2DPosition(x, y, obj, 0, yrot, 0);


				//drawObjectOn2DPosition
				//DrawThreeDeeObject2D(int x, int y, int num, int shade, int xrot, int yrot, int zrot, int bright, int overlay)
			}
			else
				g_Renderer.drawObjectOn2DPosition(x, y, obj, 0, yrot, 0);
		//	else
		//		DrawThreeDeeObject2D(x, y, ammo_object_list[n].invitem, ammo_selector_fade_val, 0, yrot, 0, 1, 0);

			xpos += OBJLIST_SPACING;
		}
	}
}

void draw_current_object_list(int ringnum)
{
	int n;
	int maxobj;
	int xoff;
	int i;
	int shade;
	int minobj;
	char textbufme[128];
	int objmeup;
	int nummeup;
	short ymeup;
	short yrot;
	//	INVOBJ* objme;
	int activenum;
	int count;

	if (rings[ringnum]->current_object_list <= 0)
		return;

	if (ringnum == RING_AMMO)
	{
		ammo_selector_fade_val = 0;
		ammo_selector_fade_dir = 0;

		if (combine_ring_fade_dir == 1)
		{
			if (combine_ring_fade_val < 128)
				combine_ring_fade_val += 32;

			if (combine_ring_fade_val > 128)
			{
				combine_ring_fade_val = 128;
				combine_ring_fade_dir = 0;
			}
		}
		else if (combine_ring_fade_dir == 2)
		{
			combine_ring_fade_val -= 32;

			if (combine_ring_fade_val <= 0)
			{
				combine_ring_fade_val = 0;
				combine_ring_fade_dir = 0;

				if (combine_type_flag)
					normal_ring_fade_dir = 2;
				else
				{
					rings[RING_INVENTORY]->ringactive = 1;
					menu_active = 1;
					rings[RING_AMMO]->ringactive = 0;
					handle_object_changeover(RING_INVENTORY);
				}

				rings[RING_AMMO]->ringactive = 0;
			}
		}
	}
	else if (normal_ring_fade_dir == 1)
	{
		if (normal_ring_fade_val < 128)
			normal_ring_fade_val += 32;

		if (normal_ring_fade_val > 128)
		{
			normal_ring_fade_val = 128;
			normal_ring_fade_dir = 0;
			rings[RING_INVENTORY]->ringactive = 1;
			menu_active = 1;
		}

	}
	else if (normal_ring_fade_dir == 2)
	{
		normal_ring_fade_val -= 32;

		if (normal_ring_fade_val <= 0)
		{
			normal_ring_fade_val = 0;
			normal_ring_fade_dir = 1;

			if (combine_type_flag == 1)
			{
				combine_type_flag = 0;
				combine_these_two_objects(combine_obj1, combine_obj2);
			}
			else if (seperate_type_flag)
				seperate_object(rings[RING_INVENTORY]->current_object_list[rings[RING_INVENTORY]->curobjinlist].invitem);

			handle_object_changeover(RING_INVENTORY);
		}
	}

	minobj = 0;
	maxobj = 0;
	xoff = 0;
	n = 0;

	if (rings[ringnum]->numobjectsinlist != 1)
		xoff = (OBJLIST_SPACING * rings[ringnum]->objlistmovement) >> 16;

	if (rings[ringnum]->numobjectsinlist == 2)
	{
		minobj = -1;
		maxobj = 0;
		n = rings[ringnum]->curobjinlist - 1;
	}

	if (rings[ringnum]->numobjectsinlist == 3 || rings[ringnum]->numobjectsinlist == 4)
	{
		minobj = -2;
		maxobj = 1;
		n = rings[ringnum]->curobjinlist - 2;
	}

	if (rings[ringnum]->numobjectsinlist >= 5)
	{
		minobj = -3;
		maxobj = 2;
		n = rings[ringnum]->curobjinlist - 3;
	}

	if (n < 0)
		n += rings[ringnum]->numobjectsinlist;

	if (rings[ringnum]->objlistmovement < 0)
		maxobj++;

	if (minobj <= maxobj)
	{
		for (i = minobj; i <= maxobj; i++)
		{
			if (minobj == i)
			{
				if (rings[ringnum]->objlistmovement < 0)
					shade = 0;
				else
					shade = rings[ringnum]->objlistmovement >> 9;
			}
			else if (i != minobj + 1 || maxobj == minobj + 1)
			{
				if (i != maxobj)
					shade = 128;
				else
				{
					if (rings[ringnum]->objlistmovement < 0)
						shade = (-128 * rings[ringnum]->objlistmovement) >> 16;
					else
						shade = 128 - (short)(rings[ringnum]->objlistmovement >> 9);
				}
			}
			else
			{
				if (rings[ringnum]->objlistmovement < 0)
					shade = 128 - ((-128 * rings[ringnum]->objlistmovement) >> 16);
				else
					shade = 128;
			}

			if (!minobj && !maxobj)
				shade = 128;

			if (ringnum == RING_AMMO && combine_ring_fade_val < 128 && shade)
				shade = combine_ring_fade_val;
			else if (ringnum == RING_INVENTORY && normal_ring_fade_val < 128 && shade)
				shade = normal_ring_fade_val;

			if (!i)
			{
				nummeup = 0;

				switch (inventry_objects_list[rings[ringnum]->current_object_list[n].invitem].object_number)
				{
				case ID_BIGMEDI_ITEM:
					nummeup = Lara.NumLargeMedipacks;
					break;

				case ID_SMALLMEDI_ITEM:
					nummeup = Lara.NumSmallMedipacks;
					break;

				case ID_FLARE_INV_ITEM:
					nummeup = Lara.NumFlares;
					break;

				default:
					if (inventry_objects_list[rings[ringnum]->current_object_list[n].invitem].object_number < ID_PUZZLE_ITEM1 ||
						inventry_objects_list[rings[ringnum]->current_object_list[n].invitem].object_number > ID_PUZZLE_ITEM8)
					{
						switch (inventry_objects_list[rings[ringnum]->current_object_list[n].invitem].object_number)
						{
						case ID_SHOTGUN_AMMO1_ITEM:
							count = Lara.Weapons[WEAPON_SHOTGUN].Ammo[WEAPON_AMMO1].getCount();
							nummeup = count == -1 ? count : count / 6;
							break;

						case ID_SHOTGUN_AMMO2_ITEM:
							count = Lara.Weapons[WEAPON_SHOTGUN].Ammo[WEAPON_AMMO2].getCount();
							nummeup = count == -1 ? count : count / 6;
							break;

						case ID_HK_AMMO_ITEM:
							nummeup = Lara.Weapons[WEAPON_HK].Ammo[WEAPON_AMMO1].getCount();
							break;

						case ID_CROSSBOW_AMMO1_ITEM:
							count = Lara.Weapons[WEAPON_CROSSBOW].Ammo[WEAPON_AMMO1].getCount();
							nummeup = count;
							break;

						case ID_CROSSBOW_AMMO2_ITEM:
							count = Lara.Weapons[WEAPON_CROSSBOW].Ammo[WEAPON_AMMO2].getCount();
							nummeup = count;
							break;

						case ID_CROSSBOW_AMMO3_ITEM:
							count = Lara.Weapons[WEAPON_CROSSBOW].Ammo[WEAPON_AMMO3].getCount();
							nummeup = count;
							break;

						case ID_GRENADE_AMMO1_ITEM:
							count = Lara.Weapons[WEAPON_GRENADE_LAUNCHER].Ammo[WEAPON_AMMO1].getCount();
							nummeup = count;
							break;

						case ID_GRENADE_AMMO2_ITEM:
							count = Lara.Weapons[WEAPON_GRENADE_LAUNCHER].Ammo[WEAPON_AMMO2].getCount();
							nummeup = count;
							break;

						case ID_GRENADE_AMMO3_ITEM:
							count = Lara.Weapons[WEAPON_GRENADE_LAUNCHER].Ammo[WEAPON_AMMO3].getCount();
							nummeup = count;
							break;

						case ID_ROCKET_LAUNCHER_ITEM:
							nummeup = Lara.Weapons[WEAPON_ROCKET_LAUNCHER].Ammo[WEAPON_AMMO1].getCount();
							break;

						case ID_HARPOON_ITEM:
							nummeup = Lara.Weapons[WEAPON_HARPOON_GUN].Ammo[WEAPON_AMMO1].getCount();
							break;

						case ID_REVOLVER_AMMO_ITEM:
							nummeup = Lara.Weapons[WEAPON_REVOLVER].Ammo[WEAPON_AMMO1].getCount();
							break;

						case ID_UZI_AMMO_ITEM:
							nummeup = Lara.Weapons[WEAPON_UZI].Ammo[WEAPON_AMMO1].getCount();
							break;
						}
					}
					else
					{
						nummeup = Lara.Puzzles[inventry_objects_list[rings[ringnum]->current_object_list[n].invitem].object_number - ID_PUZZLE_ITEM1];

						if (nummeup <= 1)
							sprintf(textbufme, g_GameFlow->GetString(inventry_objects_list[rings[ringnum]->current_object_list[n].invitem].objname));
						else
							sprintf(textbufme, "%d x %s", nummeup, g_GameFlow->GetString(inventry_objects_list[rings[ringnum]->current_object_list[n].invitem].objname));
					}

					break;
				}

				if (inventry_objects_list[rings[ringnum]->current_object_list[n].invitem].object_number < ID_PUZZLE_ITEM1 ||
					inventry_objects_list[rings[ringnum]->current_object_list[n].invitem].object_number > ID_PUZZLE_ITEM8)
				{
					if (nummeup)
					{
						if (nummeup == -1)
							sprintf(textbufme, "Unlimited %s", g_GameFlow->GetString(inventry_objects_list[rings[ringnum]->current_object_list[n].invitem].objname));
						else
							sprintf(textbufme, "%d x %s", nummeup, g_GameFlow->GetString(inventry_objects_list[rings[ringnum]->current_object_list[n].invitem].objname));
					}
					else
						sprintf(textbufme, g_GameFlow->GetString(inventry_objects_list[rings[ringnum]->current_object_list[n].invitem].objname));
				}

				if (ringnum == RING_INVENTORY)
					objmeup = (int)(phd_centery - (phd_winymax + 1) * 0.0625 * 3.0);
				else
					objmeup = (int)((phd_winymax + 1) * 0.0625 * 3.0 + phd_centery);

				g_Renderer.drawString(phd_centerx, ringnum == RING_INVENTORY ? 230 : 300, textbufme, PRINTSTRING_COLOR_YELLOW, PRINTSTRING_CENTER);
			//	PrintString(phd_centerx, objmeup, 8, textbufme, 0x8000);
			}

			if (!i && !rings[ringnum]->objlistmovement)
			{
				if ((inventry_objects_list[rings[ringnum]->current_object_list[n].invitem].flags & 2))
					rings[ringnum]->current_object_list[n].yrot += 1022;
			}
			else
				spinback(&rings[ringnum]->current_object_list[n].yrot);

			yrot = rings[ringnum]->current_object_list[n].yrot;

			if (rings[ringnum]->objlistmovement)
			{
				if (rings[ringnum]->objlistmovement > 0)
					activenum = -1;
				else
					activenum = 1;
			}
			else
				activenum = 0;

			if (i == activenum)
			{
				if (rings[ringnum]->current_object_list[n].bright < 160)
					rings[ringnum]->current_object_list[n].bright += 16;

				if (rings[ringnum]->current_object_list[n].bright > 160)
					rings[ringnum]->current_object_list[n].bright = 160;
			}
			else
			{
				if (rings[ringnum]->current_object_list[n].bright > 32)
					rings[ringnum]->current_object_list[n].bright -= 16;

				if (rings[ringnum]->current_object_list[n].bright < 32)
					rings[ringnum]->current_object_list[n].bright = 32;
			}

			int x, y, y2;
			x = 400 + xoff + i * OBJLIST_SPACING;
			y = 150;
			y2 = 430;//combine 
			short obj = convert_invobj_to_obj(rings[ringnum]->current_object_list[n].invitem);
			short scaler = inventry_objects_list[rings[ringnum]->current_object_list[n].invitem].scale1;
			g_Renderer.drawObjectOn2DPosition(x, ringnum == RING_INVENTORY ? y : y2, obj, 0, yrot, 0);

		/*	DrawThreeDeeObject2D((int)((phd_centerx * 0.00390625 * 256.0 + inventry_xpos) + xoff + i * OBJLIST_SPACING),
				(int)(phd_centery * 0.0083333338 * ymeup + inventry_ypos),
				rings[ringnum]->current_object_list[n].invitem,
				shade, 0, yrot, 0, rings[ringnum]->current_object_list[n].bright, 0);*/

			if (++n >= rings[ringnum]->numobjectsinlist)
				n = 0;
		}

		if (rings[ringnum]->ringactive)
		{
			if (rings[ringnum]->numobjectsinlist != 1 && (ringnum != 1 || combine_ring_fade_val == 128))
			{
				if (rings[ringnum]->objlistmovement > 0)
					rings[ringnum]->objlistmovement += 8192;

				if (rings[ringnum]->objlistmovement < 0)
					rings[ringnum]->objlistmovement -= 8192;

				if (goLeft)
				{
					if (!rings[ringnum]->objlistmovement)
					{
						SoundEffect(SFX_TR4_MENU_ROTATE, 0, SFX_ALWAYS);
						rings[ringnum]->objlistmovement += 8192;

						if (ammo_selector_flag)
							ammo_selector_fade_dir = 2;
					}
				}

				if (goRight)
				{
					if (!rings[ringnum]->objlistmovement)
					{
						SoundEffect(SFX_TR4_MENU_ROTATE, 0, SFX_ALWAYS);
						rings[ringnum]->objlistmovement -= 8192;

						if (ammo_selector_flag)
							ammo_selector_fade_dir = 2;
					}

				}

				if (rings[ringnum]->objlistmovement < 65536)
				{
					if (rings[ringnum]->objlistmovement < -65535)
					{
						rings[ringnum]->curobjinlist++;

						if (rings[ringnum]->curobjinlist >= rings[ringnum]->numobjectsinlist)
							rings[ringnum]->curobjinlist = 0;

						rings[ringnum]->objlistmovement = 0;

						if (ringnum == RING_INVENTORY)
							handle_object_changeover(0);
					}
				}
				else
				{
					rings[ringnum]->curobjinlist--;

					if (rings[ringnum]->curobjinlist < 0)
						rings[ringnum]->curobjinlist = rings[ringnum]->numobjectsinlist - 1;

					rings[ringnum]->objlistmovement = 0;

					if (ringnum == RING_INVENTORY)
						handle_object_changeover(0);
				}
			}
		}
	}
}

int S_CallInventory2()
{
	int return_value;

	OldLaraBusy = Lara.busy;

	if (TrInput & IN_SELECT)
		stop_killing_me_you_dumb_input_system = 1;

	rings[RING_INVENTORY] = &pcring1;
	rings[RING_AMMO] = &pcring2;
	g_Renderer.DumpGameScene();
	GLOBAL_invMode = IM_INGAME;
	init_inventry();
	Camera.numberFrames = 2;

	while (true)
	{
		int val = 0;

		OBJLIST_SPACING = phd_centerx >> 1;

		if (compassNeedleAngle != 1024)
			compassNeedleAngle -= 32;

		SetDebounce = 1;
		S_UpdateInput();
		TrInput = InputBusy;
	//	UpdatePulseColour();
		GameTimer++;

		if (DbInput & IN_OPTION)
		{
			SoundEffect(SFX_TR4_MENU_SELECT, 0, SFX_ALWAYS);
			val = 1;
		}

		return_value = ThreadEnded;

		if (return_value)
			return return_value;

		do_debounced_input();

	/*	if (examine_mode)
			do_examine_mode();
		else*/
		if (stats_mode)
			do_stats_mode();
	//	else
		{
			DrawInv();
		/*	draw_current_object_list(RING_INVENTORY);
			handle_inventry_menu();

			if (rings[RING_AMMO]->ringactive)
				draw_current_object_list(RING_AMMO);

			draw_ammo_selector();
			fade_ammo_selector();*/
		}

		if (useItem & !TrInput)
			val = 1;

	//	S_OutputPolyList();
		Camera.numberFrames = g_Renderer.SyncRenderer();

		if (loading_or_saving)
		{
			loading_or_saving = 0;//fix meeeeeeeeeeeee
			stop_killing_me_you_dumb_input_system2 = 1;
			stop_killing_me_you_dumb_input_system = 1;
		/*	do
			{
				S_InitialisePolyList();
				SetDebounce = 1;
				S_UpdateInput();
				input = inputBusy;
				UpdatePulseColour();

				if (loading_or_saving == 1)
					val = go_and_load_game();
				else if (go_and_save_game())
					val = 1;

			} while (!val);

			if (val == 1 && loading_or_saving == val)
			{
				return_value = 1;
				val = 1;
			}

			stop_killing_me_you_dumb_input_system2 = 1;
			stop_killing_me_you_dumb_input_system = 1;
			deselect_debounce = 0;
			go_deselect = 0;
			loading_or_saving = 0;*/
		}

		if (val)
			break;
	}

	InitialisePickupDisplay();
	GLOBAL_lastinvitem = rings[RING_INVENTORY]->current_object_list[rings[RING_INVENTORY]->curobjinlist].invitem;
	update_laras_weapons_status();

	if (useItem)
		use_current_item();

	Lara.busy = OldLaraBusy;
	GLOBAL_invMode = IM_NONE;

/*	if (GLOBAL_invkeypadmode)
	{
		short room_number;
		ITEM_INFO* item;
		int val;

		val = 0;
		GLOBAL_invkeypadmode = 0;

		if (keypadnuminputs == 4)
			val = keypadinputs[3] + 10 * (keypadinputs[2] + 10 * (keypadinputs[1] + 10 * keypadinputs[0]));

		if (GLOBAL_invkeypadcombination == val)
		{
			item = lara_item;
			room_number = lara_item->room_number;
			GetHeight(GetFloor(lara_item->pos.x_pos, lara_item->pos.y_pos, lara_item->pos.z_pos, &room_number),
				item->pos.x_pos, item->pos.y_pos, item->pos.z_pos);
			TestTriggers(trigger_index, 1, 0);
		}
	}*/

	return return_value;
}

void do_stats_mode()
{
	GLOBAL_invMode = IM_STATS;

	if (goDeselect)
	{
		GLOBAL_invMode = IM_NONE;
		stats_mode = 0;
	}
}

void draw_compass()
{
	g_Renderer.drawObjectOn2DPosition(130, 480, ID_COMPASS_ITEM, ANGLE(90), 0, ANGLE(180));
	short compass_speed = phd_sin(compassNeedleAngle - LaraItem->pos.yRot);
	short compass_angle = (LaraItem->pos.yRot + compass_speed) - 32768;
	Matrix::CreateRotationY(compass_angle);
}



void combine_revolver_lasersight(int flag)
{
	if (flag)
	{
		Lara.Lasersight = 1;
		Lara.Weapons[WEAPON_REVOLVER].HasLasersight = 0;
	}
	else
	{
		Lara.Lasersight = 0;
		Lara.Weapons[WEAPON_REVOLVER].HasLasersight = 1;
	}

	if (Lara.gunStatus && Lara.gunType == WEAPON_REVOLVER)
	{
		undraw_pistol_mesh_right(WEAPON_REVOLVER);
		draw_pistol_meshes(WEAPON_REVOLVER);
	}
}

void combine_crossbow_lasersight(int flag)
{
	if (flag)
	{
		Lara.Lasersight = 1;
		Lara.Weapons[WEAPON_CROSSBOW].HasLasersight = 0;
	}
	else
	{
		Lara.Lasersight = 0;
		Lara.Weapons[WEAPON_CROSSBOW].HasLasersight = 1;
	}

	if (Lara.gunStatus && Lara.gunType == WEAPON_CROSSBOW)
	{
		undraw_shotgun_meshes(WEAPON_CROSSBOW);
		draw_shotgun_meshes(WEAPON_CROSSBOW);
	}
}

void combine_HK_SILENCER(int flag)
{
	if (flag)
	{
		Lara.Silencer = 1;
		Lara.Weapons[WEAPON_HK].HasSilencer = 0;

	}
	else
	{
		Lara.Silencer = 0;
		Lara.Weapons[WEAPON_HK].HasSilencer = 1;
	}
}

void combine_PuzzleItem1(int flag)
{
	Lara.PuzzlesCombo[0] = 0;
	Lara.PuzzlesCombo[1] = 0;
	Lara.Puzzles[0] = 1;
}

void combine_PuzzleItem2(int flag)
{
	Lara.PuzzlesCombo[2] = 0;
	Lara.PuzzlesCombo[3] = 0;
	Lara.Puzzles[1] = 1;
}

void combine_PuzzleItem3(int flag)
{
	Lara.PuzzlesCombo[4] = 0;
	Lara.PuzzlesCombo[5] = 0;
	Lara.Puzzles[2] = 1;
}

void combine_PuzzleItem4(int flag)
{
	Lara.PuzzlesCombo[6] = 0;
	Lara.PuzzlesCombo[7] = 0;
	Lara.Puzzles[3] = 1;
}

void combine_PuzzleItem5(int flag)
{
	Lara.PuzzlesCombo[8] = 0;
	Lara.PuzzlesCombo[9] = 0;
	Lara.Puzzles[4] = 1;
}

void combine_PuzzleItem6(int flag)
{
	Lara.PuzzlesCombo[10] = 0;
	Lara.PuzzlesCombo[11] = 0;
	Lara.Puzzles[5] = 1;
}

void combine_PuzzleItem7(int flag)
{
	Lara.PuzzlesCombo[12] = 0;
	Lara.PuzzlesCombo[13] = 0;
	Lara.Puzzles[6] = 1;
}

void combine_PuzzleItem8(int flag)
{
	Lara.PuzzlesCombo[14] = 0;
	Lara.PuzzlesCombo[15] = 0;
	Lara.Puzzles[7] = 1;
}

void combine_KeyItem1(int flag)
{
	Lara.Keys[0] = 1;
	Lara.KeysCombo[0] = 0;
	Lara.KeysCombo[1] = 0;
}

void combine_KeyItem2(int flag)
{
	Lara.Keys[1] = 1;
	Lara.KeysCombo[2] = 0;
	Lara.KeysCombo[3] = 0;
}

void combine_KeyItem3(int flag)
{
	Lara.Keys[2] = 1;
	Lara.KeysCombo[4] = 0;
	Lara.KeysCombo[5] = 0;
}

void combine_KeyItem4(int flag)
{
	Lara.Keys[3] = 1;
	Lara.KeysCombo[6] = 0;
	Lara.KeysCombo[7] = 0;
}

void combine_KeyItem5(int flag)
{
	Lara.Keys[4] = 1;
	Lara.KeysCombo[8] = 0;
	Lara.KeysCombo[9] = 0;
}

void combine_KeyItem6(int flag)
{
	Lara.Keys[5] = 1;
	Lara.KeysCombo[10] = 0;
	Lara.KeysCombo[11] = 0;
}

void combine_KeyItem7(int flag)
{
	Lara.Keys[6] = 1;
	Lara.KeysCombo[12] = 0;
	Lara.KeysCombo[13] = 0;
}

void combine_KeyItem8(int flag)
{
	Lara.Keys[7] = 1;
	Lara.KeysCombo[14] = 0;
	Lara.KeysCombo[15] = 0;
}

void combine_PickupItem1(int flag)
{
	Lara.Pickups[0] = 1;
	Lara.PickupsCombo[0] = 0;
	Lara.PickupsCombo[1] = 0;
}

void combine_PickupItem2(int flag)
{
	Lara.Pickups[1] = 1;
	Lara.PickupsCombo[2] = 0;
	Lara.PickupsCombo[3] = 0;
}

void combine_PickupItem3(int flag)
{
	Lara.Pickups[2] = 1;
	Lara.PickupsCombo[4] = 0;
	Lara.PickupsCombo[5] = 0;
}

void combine_PickupItem4(int flag)
{
	Lara.Pickups[3] = 1;
	Lara.PickupsCombo[6] = 0;
	Lara.PickupsCombo[7] = 0;
}

void combine_ClockWorkBeetle(int flag)
{
	Lara.hasBeetleThings &= 2;//remove combo1
	Lara.hasBeetleThings &= 4;//remove combo2
	Lara.hasBeetleThings |= 1;//get beetle
}