#include "aetherworld.h"

static const struct aetherworld_act acts[AETHERWORLD_ACT_COUNT] = {
  {1, "The First Ember", "Awakening and the lost star", 1, 60},
  {2, "Ashes of the Crown", "The broken kingdoms", 40, 110},
  {3, "Whispers Beneath", "Ancient ruins and buried powers", 90, 160},
  {4, "The Verdant War", "The living world turns hostile", 140, 210},
  {5, "Siege of Aetherfall", "The first great invasion", 190, 270},
  {6, "The Shattered Sky", "Planar storms and sky citadels", 250, 330},
  {7, "Empire of Glass", "A crystalline empire rises", 310, 400},
  {8, "The Drowned Star", "A dead god beneath the sea", 380, 470},
  {9, "War of Twelve Banners", "Factions collide across the world", 450, 540},
  {10, "The Chronal Gate", "Time fractures and forgotten futures", 520, 620},
  {11, "The Eternal Night", "The final enemy crosses the veil", 600, 690},
  {12, "AetherWorld", "The last choice and the new age", 680, 720}
};

static const struct aetherworld_quest quests[AETHERWORLD_QUEST_COUNT] = {
  {1,1,"Ember in the Ruins","Recover the first ember from the ruined watchtower.",1,100,25,0},
  {2,1,"Road to Aetherfall","Escort the wounded scout to Aetherfall.",5,150,40,1},
  {3,1,"Three Broken Seals","Find three seals beneath the old chapel.",10,200,60,2},
  {4,1,"The Night Watch","Defend the eastern gate until dawn.",15,250,80,3},
  {5,2,"Crownless King","Find the missing heir of the northern crown.",40,700,250,4},
  {6,2,"Bandit Accord","Break the warlord's alliance with the road clans.",50,850,300,5},
  {7,2,"Blacksmith's Oath","Recover the royal forge hammer.",60,1000,350,6},
  {8,2,"Ashen Standard","Raise the kingdom's banner above the keep.",70,1200,450,7},
  {9,3,"Below the Stones","Enter the sealed undercity.",90,1600,550,8},
  {10,3,"Echoes of the Deep","Silence the relic that is calling monsters.",100,1800,650,9},
  {11,3,"Archivist's Key","Recover the key to the forbidden archive.",115,2100,750,10},
  {12,3,"The Sleeping Machine","Disable an ancient aether engine.",130,2400,900,11},
  {13,4,"Greenfire","Investigate the burning forest.",140,2700,1000,12},
  {14,4,"Rootbound","Break the corruption beneath the elder tree.",150,2900,1100,13},
  {15,4,"Beast Queen","Defeat the queen of the thornborn.",170,3300,1300,14},
  {16,4,"Heartwood Pact","Negotiate peace with the forest guardians.",200,3800,1500,15},
  {17,5,"Aetherfall Under Siege","Survive the first assault on the city.",190,4200,1700,16},
  {18,5,"The Siege Engine","Destroy the enemy's mobile fortress.",210,4600,1900,17},
  {19,5,"Captain of the Wall","Protect the western battlement.",230,5000,2100,18},
  {20,5,"Dawn at Last","Defeat the invasion commander.",270,6000,2500,19},
  {21,6,"Skybreak","Reach the floating citadel.",250,6500,2700,20},
  {22,6,"Stormglass","Collect stormglass from the upper reaches.",270,7000,3000,21},
  {23,6,"The Fallen Fleet","Rescue the surviving sky sailors.",290,7600,3300,22},
  {24,6,"Citadel of Thunder","Defeat the storm sovereign.",330,9000,4000,23},
  {25,7,"Glass Horizon","Cross the crystal frontier.",310,9500,4200,24},
  {26,7,"Prism Knights","Break the Prism Order's blockade.",330,10000,4500,25},
  {27,7,"The Shard Vault","Steal a shard from the imperial vault.",350,11000,5000,26},
  {28,7,"Emperor of Glass","Challenge the crystal emperor.",400,13000,6000,27},
  {29,8,"Drowned Bell","Find the bell beneath the drowned cathedral.",380,13500,6500,28},
  {30,8,"Salt of Memory","Recover memories stolen by the sea god.",400,14500,7000,29},
  {31,8,"Leviathan Road","Cross the abyssal trench.",430,15500,8000,30},
  {32,8,"The Drowned Star","Defeat the sleeping god's avatar.",470,18000,9500,31},
  {33,9,"Twelve Banners","Gather representatives of all twelve factions.",450,19000,10000,32},
  {34,9,"Broken Alliance","Stop an assassination at the war council.",470,20500,11000,33},
  {35,9,"Banner of Ash","Defeat the Ash Legion champion.",490,22000,12000,34},
  {36,9,"The Last Coalition","Win the battle at Meridian Field.",540,25000,14000,35},
  {37,10,"Chronal Gate","Locate the first temporal anchor.",520,27000,15000,36},
  {38,10,"Yesterday's Enemy","Face an enemy from a lost timeline.",550,29000,16000,37},
  {39,10,"Future Without Dawn","Prevent the doomed future from manifesting.",580,32000,18000,38},
  {40,10,"The Gatekeeper","Defeat the guardian outside time.",620,36000,20000,39},
  {41,11,"Nightfall","Survive the first hour of eternal night.",600,38000,22000,40},
  {42,11,"Black Sun","Destroy the eclipse engine.",620,41000,24000,41},
  {43,11,"Veilbreaker","Open a path through the final veil.",650,44000,26000,42},
  {44,11,"The Twelve Wings","Defeat the twelve-winged herald.",690,50000,30000,43},
  {45,12,"AetherWorld","Reach the heart of the world.",680,52000,32000,44},
  {46,12,"The Paradox Choice","Choose which timeline survives.",690,56000,35000,45},
  {47,12,"Last Light","Defeat the final manifestation.",700,60000,40000,46},
  {48,12,"New Dawn","Restore the world engine.",710,65000,45000,47},
  {49,12,"The Long Road Home","Return to Aetherfall and witness the new age.",715,70000,50000,48},
  {50,12,"Epilogue: The Next World","Complete the post-campaign legacy objective.",720,75000,60000,49}
};

const struct aetherworld_act *aetherworld_get_act(int id) {
  if (id < 1 || id > AETHERWORLD_ACT_COUNT) return 0;
  return &acts[id - 1];
}

const struct aetherworld_quest *aetherworld_get_quest(int id) {
  if (id < 1 || id > AETHERWORLD_QUEST_COUNT) return 0;
  return &quests[id - 1];
}

int aetherworld_quest_count(void) { return AETHERWORLD_QUEST_COUNT; }
int aetherworld_act_count(void) { return AETHERWORLD_ACT_COUNT; }
