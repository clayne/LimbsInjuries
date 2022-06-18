#include "SkeletonHandler.h"
#include "Geom.h"
#include "WeaponHandler.h"
#include "Injuries.h"
#include "DebugAPI.h"
#include "SoundPlayer.h"
#include <UselessFenixUtils.h>
#include "DataHandler.h"

using RE::NiPoint3;

enum Skeletons
{
	Human,
	Draugr,
	Dog,
	Dragon,
	Deer,
	Spriggan,
	DragonPriest,
	Wisp,
	Horse,
	Giant,
	Lurker,
	Skeever,
	DraugrS,
	Chaurus,
	Goat,
	Spider,
	Cow,
	Troll,
	SabreCat,
	Mamonth,
	Horker,
	Hagraven,
	AtronachFrost,
	AtronachFlame,
	Falmer,
	DwarvenSpider,
	Sphere,
	Centurion,
	Bear,
	ChaurusFlyer,
	Scrib,
	Vampire,
	HMDaedra,
	Riekling,

	Other,

	Unknown
};

inline const std::vector<const char*> NodeNames[] = {
	{
		// Human
		"NPCEyeBone", "NPC Head [Head]", "NPC Neck [Neck]",                                   // 0 1 2
		"NPC Spine [Spn0]",                                                                   // 3
		"NPC L Thigh [LThg]", "NPC L Calf [LClf]", "NPC L Foot [Lft ]", "NPC L Toe0 [LToe]",  // 4 5 6 7
		"NPC R Thigh [RThg]", "NPC R Calf [RClf]", "NPC R Foot [Rft ]", "NPC R Toe0 [RToe]",  // 8 9 10 11
		"NPC R UpperArm [RUar]", "NPC R Forearm [RLar]", "NPC R Hand [RHnd]",                 // 12 13 14
		"NPC L UpperArm [LUar]", "NPC L Forearm [LLar]", "NPC L Hand [LHnd]"                  // 15 16 17
	},
	{
		// Draugr
		"NPC Head [Beard]", "NPC Head [Head]", "NPC Neck [Neck]",                             // 0 1 2
		"NPC Spine [Spn0]",                                                                   // 3
		"NPC L Thigh [LThg]", "NPC L Calf [LClf]", "NPC L Foot [Lft ]", "NPC L Toe0 [LToe]",  // 4 5 6 7
		"NPC R Thigh [RThg]", "NPC R Calf [RClf]", "NPC R Foot [Rft ]", "NPC R Toe0 [RToe]",  // 8 9 10 11
		"NPC R UpperArm [RUar]", "NPC R Forearm [RLar]", "NPC R Hand [RHnd]",                 // 12 13 14
		"NPC L UpperArm [LUar]", "NPC L Forearm [LLar]", "NPC L Hand [LHnd]"                  // 15 16 17
	},
	{
		// Dog
		"Canine_FrontLip", "Canine_Head", "Canine_Neck1",                                         // 0 1 2
		"Canine_Spine1",                                                                          // 3
		"Canine_LBackLeg1", "Canine_LBackLeg2", "Canine_LBackLegPalm", "Canine_LBackLegToe",      // 4 5 6 7
		"Canine_RBackLeg1", "Canine_RBackLeg2", "Canine_RBackLegPalm", "Canine_RBackLegToe",      // 8 9 10 11
		"Canine_LFrontLeg1", "Canine_LFrontLeg2", "Canine_LFrontLegPalm", "Canine_LFrontLegToe",  // 12 13 14 15
		"Canine_RFrontLeg1", "Canine_RFrontLeg2", "Canine_RFrontLegPalm", "Canine_RFrontLegToe",  // 16 17 18 19
		"Canine_Tail3"                                                                            // 20
	},
	{
		// Dragon
		"NPC LUpLip", "NPC Head", "NPC Neck3", "NPC Neck1",                // 0 1 2 3
		"NPC Spine1",                                                      // 4
		"NPC LLegThigh", "NPC LLegCalf", "NPC LLegFoot", "NPC LLegToe4",   // 5 6 7 8
		"NPC RLegThigh", "NPC RLegCalf", "NPC RLegFoot", "NPC RLegToe4",   // 9 10 11 12
		"NPC RUpArm1", "NPC RForearm1", "NPC RHand", "NPC RFinger22",      // 13 14 15 16
		"NPC LUpArm1", "NPC LForearm1", "NPC LHand", "NPC LFinger22",      // 17 18 19 20
		"NPC Tail2", "NPC Tail4", "NPC Tail6", "NPC Tail8", "NPC TailHub"  // 21 22 23 24 25
	},
	{
		// Deer
		"ElkL_UpperLip", "ElkScull", "ElkNeck2",                         // 0 1 2
		"ElkSpine1",                                                     // 3
		"ElkLFemur", "ElkLTibia", "ElkLLargeCannon", "ElkLRearHoof",     // 4 5 6 7
		"ElkRFemur", "ElkRTibia", "ElkRLargeCannon", "ElkRRearHoof",     // 8 9 10 11
		"ElkRHumerus", "ElkRRadius", "ElkRMetacarpus", "ElkRFrontHoof",  // 12 13 14 15
		"ElkLHumerus", "ElkLRadius", "ElkLMetacarpus", "ElkLFrontHoof",  // 16 17 18 19
		"ElkTail2"                                                       // 20
	},
	{
		// Spriggan
		"PBomb08", "NPC Head [Head]", "NPC Neck [Neck]",                                                                                                  // 0 1 2
		"NPC Spine [Spn0]",                                                                                                                               // 3
		"NPC L Thigh [LThg]", "NPC L Calf [LClf]", "NPC L Foot [Lft ]", "NPC L Toe0 [LToe]",                                                              // 4 5 6 7
		"NPC R Thigh [RThg]", "NPC R Calf [RClf]", "NPC R Foot [Rft ]", "NPC R Toe0 [RToe]",                                                              // 8 9 10 11
		"NPC R UpperArm [RUar]", "NPC R Forearm [RLar]", "NPC R Hand [RHnd]", "NPC R Finger02 [RF02]", "NPC R Finger32 [RF32]", "NPC R Finger12 [RF12]",  // 12 13 14 15 16 17
		"NPC L UpperArm [LUar]", "NPC L Forearm [LLar]", "NPC L Hand [LHnd]", "NPC L Finger02 [RF02]", "NPC L Finger32 [RF32]", "NPC L Finger12 [RF12]"   // 18 19 20 21 22 23
	},
	{
		// DragonPriest
		"DragPriestNPC Head [Brow]", "DragPriestNPC Head [Head]", "DragPriestNPC Neck [Neck]",                                        // 0 1 2
		"DragPriestNPC Spine [Spn0]",                                                                                                 // 3
		"DragPriestNPC R Thigh [RThg]", "DragPriestNPC R Calf [RClf]", "DragPriestNPC R Foot [Rft ]", "DragPriestNPC R Toe0 [RToe]",  // 4 5 6 7
		"DragPriestNPC L Thigh [LThg]", "DragPriestNPC L Calf [LClf]", "DragPriestNPC L Foot [LLft ]", "DragPriestNPC L Toe[LToe]",   // 8 9 10 11
		"DragPriestNPC L UpperArm [LUar]", "DragPriestNPC L Forearm [LLar]", "DragPriestNPC L Finger22 [LF22]",                       // 12 13 14
		"DragPriestNPC R UpperArm [RUar]", "DragPriestNPC R Forearm [RLar]", "DragPriestNPC R Finger22 [LF22]"                        // 15 16 17
	},
	{
		// Wisp
		"Wisp Brow", "Wisp Head", "Wisp Neck",                                           // 0 1 2
		"Wisp Spine1",                                                                   // 3
		"Wisp L Leg Wrap1", "Wisp L Leg Wrap2", "Wisp L Leg Wrap3", "Wisp L Leg Wrap4",  // 4 5 6 7
		"Wisp R Leg Wrap1", "Wisp R Leg Wrap2", "Wisp R Leg Wrap3", "Wisp R Leg Wrap4",  // 8 9 10 11
		"Wisp L Upperarm", "Wisp L Forearm", "Wisp L Hand",                              // 12 13 14
		"Wisp R Upperarm", "Wisp R Forearm", "Wisp R Hand"                               // 15 16 17
	},
	{
		// Horse
		"HorseLowerLip", "HorseScull", "HorseNeck1",                                                             // 0 1 2
		"HorseSpine1",                                                                                           // 3
		"HorseRFemur", "HorseRTibia", "HorseRLargeCannon", "HorseRRearHoof",                                     // 4 5 6 7
		"HorseLFemur", "HorseLTibia", "HorseLLargeCannon", "HorseLRearHoof",                                     // 8 9 10 11
		"HorseLHumerus", "HorseLRadius", "HorseLMetacarpus", "HorseLFrontHoof",                                  // 12 13 14 15
		"HorseFrontRLegHumerus", "HorseFrontRLegRadius", "HorseFrontRLegMetacarpus", "HorseFrontRLegFrontHoof",  // 16 17 18 19
		"HorseTail2", "HorseTail4"                                                                               // 20 21
	},
	{
		// Giant
		"NPC L Mouth", "NPC Head [Head]", "NPC Neck [Neck]1",                                 // 0 1 2
		"NPC Spine [Spn0]",                                                                   // 3
		"NPC L Thigh [LThg]", "NPC L Calf [LClf]", "NPC L Foot [Lft ]", "NPC L Toe0 [LToe]",  // 4 5 6 7
		"NPC R Thigh [RThg]", "NPC R Calf [RClf]", "NPC R Foot [Rft ]", "NPC R Toe0 [RToe]",  // 8 9 10 11
		"NPC R UpperArm [RUar]", "NPC R Forearm [RLar]", "NPC R Hand [RHnd]",                 // 12 13 14
		"NPC L UpperArm [LUar]", "NPC L Forearm [LLar]", "NPC L Hand [LHnd]"                  // 15 16 17
	},
	{
		// Lurker
		"NPC L Mouth", "NPC Head [Head]", "NPC Neck [Neck]1",                                 // 0 1 2
		"NPC Spine [Spn0]",                                                                   // 3
		"NPC L Thigh [RThg]", "NPC L Calf [RClf]", "NPC L Foot [Rft ]", "NPC L Toe0 [RToe]",  // 4 5 6 7
		"NPC R Thigh [RThg]", "NPC R Calf [RClf]", "NPC R Foot [Rft ]", "NPC R Toe0 [RToe]",  // 8 9 10 11
		"NPC R UpperArm [RUar]", "NPC R Forearm [RLar]", "NPC R Hand [RHnd]",                 // 12 13 14
		"NPC L UpperArm [RUar]", "NPC L Forearm [RLar]", "NPC L Hand [RHnd]"                  // 15 16 17
	},
	{
		// Skeever
		"UpperLipFront", "HEAD", "Torso",           // 0 1 2
		"SpineLowerSpine",                          // 3
		"LLeg1", "LLeg2", "LLeg3", "LLegAnkle",     // 4 5 6 7
		"RLeg1", "RLeg2", "RLeg3", "RLegAnkle",     // 8 9 10 11
		"RArm_Upper", "RArm_Forearm", "RArm_Palm",  // 12 13 14
		"LArm_Upper", "LArm_Forearm", "LArm_Palm",  // 15 16 17
		"Tail2", "Tail4"                            // 18 19
	},
	{
		// DraugrS
		"NPC Head [Beard]", "NPC Head [Head]", "NPC Neck [Neck]",                              // 0 1 2
		"NPC Spine [Spn0]",                                                                    // 3
		"NPC L Thigh [LThg]", "NPC L Calf [LClf]", "NPC L Foot [LLft ]", "NPC L Toe0 [LToe]",  // 4 5 6 7
		"NPC R Thigh [RThg]", "NPC R Calf [RClf]", "NPC R Foot [Rft ]", "NPC R Toe0 [RToe]",   // 8 9 10 11
		"NPC R UpperArm [RUar]", "NPC R Forearm [RLar]", "NPC R Hand [RHnd]",                  // 12 13 14
		"NPC L UpperArm [LUar]", "NPC L Forearm [LLar]", "NPC L Hand [LHnd]"                   // 15 16 17
	},
	{
		// Chaurus
		"LagBone", "NPC Head [Head]", "[Neck0]",                                         // 0 1 2
		"Tail[Tal0]",                                                                    // 3
		"R Leg[RThg0]", "R Leg[RThg1]", "R Leg[RClf]", "R Leg[RAnkle]",                  // 4 5 6 7
		"LLeg[RThg0]", "LLeg[LThg1]", "LLeg[LClf]", "LLeg[LAnkle]",                      // 8 9 10 11
		"R UpperArm[RClv]", "R UpperArm[RUar]", "R UpperArm[RLar]", "R UpperArm[RHnd]",  // 12 13 14 15
		"L UpperArm[RClv]", "L UpperArm[LUar]", "L UpperArm[LLar]", "L UpperArm[LHnd]",  // 16 17 18 19
		"Tail[Tal2]", "[Tal3]Bone01"                                                     // 20 21
	},
	{
		// Goat
		"Goat_JawHair11", "Goat_Head", "Goat_Neck1",                                                  // 0 1 2
		"Goat_Spine1",                                                                                // 3
		"Goat_LBackLeg_Thigh", "Goat_LBackLeg_Caf", "Goat_LBackLegPalm", "Goat_LBackLegToe",          // 4 5 6 7
		"Goat_RBackLeg_Thigh", "Goat_RBackLeg_Caf", "Goat_RBackLegPalm", "Goat_RBackLegToe",          // 8 9 10 11
		"Goat_LFrontLeg_Thigh", "Goat_LFrontLeg_Upper", "Goat_LFrontLeg_Lower", "Goat_LFrontLegToe",  // 12 13 14 15
		"Goat_RFrontLeg_Thigh", "Goat_RFrontLeg_Upper", "Goat_RFrontLeg_Lower", "Goat_RFrontLegToe",  // 16 17 18 19
		"Goat_TailHair1"                                                                              // 20
	},
	{
		// Spider
		"[body]",  // 0

		// head
		"FangR[00]", "FangR[02]",  // 1 2
		"FangL[00]", "FangL[02]",  // 3 4

		// legs
		"Leg_R[21]", "Leg_R[22]", "Leg_R[23]", "[RToe2]",  // 5 6 7 8
		"Leg_R[31]", "Leg_R[32]", "Leg_R[33]", "[RToe3]",  // 9 10 11 12
		"Leg_L[31]", "Leg_L[32]", "Leg_L[33]", "[LToe3]",  // 13 14 15 16
		"Leg_L[21]", "Leg_L[22]", "Leg_L[23]", "[LToe2]",  // 17 18 19 20
		"Leg_R[11]", "Leg_R[12]", "Leg_R[13]", "[RToe1]",  // 21 22 23 24
		"Leg_R[01]", "Leg_R[02]", "Leg_R[03]", "[RToe0]",  // 25 26 27 28
		"Leg_L[01]", "Leg_L[02]", "Leg_L[03]", "[LToe0]",  // 29 30 31 32
		"Leg_L[11]", "Leg_L[12]", "Leg_L[13]", "[LToe1]",  // 33 34 35 36

		// long actual arms
		"ArmLCollarbone", "ArmL[01]", "ArmL[02]", "ArmLClaw",  // 37 38 39 40
		"ArmRCollarbone", "ArmR[01]", "ArmR[02]", "ArmRClaw",  // 41 42 43 44

		"Tail3",  // 45
	},
	{
		// Cow
		"Scull_[Rmuzzel]", "Scull", "Neck1",                     // 0 1 2
		"SPINE1",                                                // 3
		"RFemur", "RTibia", "RLargeCannon", "RRearHoof",         // 4 5 6 7
		"LFemur", "LTibia", "LLargeCannon", "LRearHoof",         // 8 9 10 11
		"R_Humerus", "R_Radius", "R_Metacarpus", "R_FrontHoof",  // 12 13 14 15
		"L_Humerus", "L_Radius", "L_Metacarpus", "L_FrontHoof",  // 16 17 18 19
		"Tail1", "Tail4"                                         // 20 21
	},
	{
		// Troll
		"NPC Lip1 [Lip1]", "NPC Head [Head]", "NPC Neck [Neck]",                              // 0 1 2
		"NPC Spine [Spn0]",                                                                   // 3
		"NPC L Thigh [LThg]", "NPC L Calf [LClf]", "NPC L Foot [Lft ]", "NPC L Toe0 [LToe]",  // 4 5 6 7
		"NPC R Thigh [RThg]", "NPC R Calf [RClf]", "NPC R Foot [Rft ]", "NPC R Toe0 [RToe]",  // 8 9 10 11
		"NPC R UpperArm [RUar]", "NPC R Forearm [RLar]", "NPC R Hand [RHnd]",                 // 12 13 14
		"NPC L UpperArm [LUar]", "NPC L Forearm [LLar]", "NPC L Hand [LHnd]"                  // 15 16 17
	},
	{
		// SabreCat
		"Sabrecat_Head [RChk]", "Sabrecat_Head [Head]", "Sabrecat_Neck[Nek0]",                                                     // 0 1 2
		"Sabrecat_Spine[Spn0]",                                                                                                    // 3
		"Sabrecat_RightThigh[RThi]", "Sabrecat_RightCalf[RClf]", "Sabrecat_RightFoot[RFot]", "Sabrecat_RightToe0[RT01]",           // 4 5 6 7
		"Sabrecat_LeftThigh[LThi]", "Sabrecat_LeftCalf[LClf]", "Sabrecat_LeftFoot[LFot]", "Sabrecat_LeftToe0[LT01]",               // 8 9 10 11
		"Sabrecat_RightUpperArm[RUar]", "Sabrecat_RightForearm[RFar]", "Sabrecat_RightHand[RHnd]", "Sabrecat_RightFinger1[RF11]",  // 12 13 14 15
		"Sabrecat_LeftUpperArm[LUar]", "Sabrecat_LeftForearm[LFar]", "Sabrecat_LeftHand[LHnd]", "Sabrecat_LeftFinger1[LF11]",      // 16 17 18 19
		"Sabrecat_Tail0[Tal0]", "Sabrecat_Tail2[Tal2]"                                                                             // 20 21
	},
	{
		// Mamonth
		"Mammoth Trunk 8", "Mammoth Trunk 3", "Mammoth Trunk 1", "Mammoth Head", "Mammoth Spine 4",  // 0 1 2 3 4
		"Mammoth Spine 1",                                                                           // 5
		"Mammoth Back R Upperleg", "Mammoth Back R Lowerleg", "Mammoth Back R Foot",                 // 6 7 8
		"Mammoth Back L Upperleg", "Mammoth Back L Lowerleg", "Mammoth Back L Foot",                 // 9 10 11
		"Mammoth Front R Upperleg", "Mammoth Front R Lowerleg", "Mammoth Front R Foot",              // 12 13 14
		"Mammoth Front L Upperleg", "Mammoth Front L Lowerleg", "Mammoth Front L Foot",              // 15 16 17
		"Mammoth Tail 1", "Mammoth Tail 7"                                                           // 18 19
	},
	{
		// Horker
		"Horker_JawBone", "Horker_Head", "Horker_Neck1",                                                    // 0 1 2
		"Horker_Spine1",                                                                                    // 3
		"Horker_RBackLeg_Thigh", "Horker_RBackLeg_Caf", "Horker_RBackLegPalm", "Horker_RBackLegToe",        // 4 5 6 7
		"Horker_LBackLeg_Thigh", "Horker_LBackLeg_Caf", "Horker_LBackLegPalm", "Horker_LBackLegToe",        // 8 9 10 11
		"Horker_RFrontLeg_Thigh", "Horker_RFrontLeg_Upper", "Horker_RFrontLegPalm", "Horker_RFrontLegToe",  // 12 13 14 15
		"Horker_LFrontLeg_Thigh", "Horker_LFrontLeg_Upper", "Horker_LFrontLegPalm", "Horker_LFrontLegToe"   // 16 17 18 19
	},
	{
		// Hagraven
		"NPC Head [Head]Brow", "NPC Head [Head]", "NPC Spine2 [Spn2]",                        // 0 1 2
		"NPC Spine [Spn0]",                                                                   // 3
		"NPC L Thigh [LThg]", "NPC L Calf [LClf]", "NPC L Foot [Lft ]", "NPC L Toe0 [LToe]",  // 4 5 6 7
		"NPC R Thigh [LThg]", "NPC R Calf [LClf]", "NPC R Foot [Lft ]", "NPC R Toe0 [LToe]",  // 8 9 10 11
		"NPC R UpperArm [RUar]", "NPC R Forearm [RLar]", "NPC R Hand [RHnd]",                 // 12 13 14
		"NPC L UpperArm [RUar]", "NPC L Forearm [RLar]", "NPC L Hand [RHnd]"                  // 15 16 17
	},
	{
		// AtronachFrost
		"NPC Head [Head]", "NPC Neck [Neck]", "NPC Spine2 [Spn2]",       // 0 1 2
		"NPC Spine [Spn0]",                                              // 3
		"NPC L Thigh [LThg]", "NPC L Calf [LClf]", "NPC L Foot [Lft ]",  // 4 5 6
		"NPC R Thigh [LThg]", "NPC R Calf [LClf]", "NPC R Foot [Lft ]",  // 7 8 9
		"NPC R UpperArm [RUar]", "NPC R Forearm [RLar]",                 // 10 11
		"NPC L UpperArm [RUar]", "NPC L Forearm [RLar]"                  // 12 13
	},
	{
		// AtronachFlame
		"P_Lag01", "FireAtronach_Head [Head]", "FireAtronach_Neck [Neck]",                                                        // 0 1 2
		"FireAtronach_Spine [Spn0]",                                                                                              // 3
		"FireAtronach_R Thigh [RThg]", "FireAtronach_R Calf [RClf]", "FireAtronach_R Foot [RLft]", "FireAtronach_R Toe0 [RToe]",  // 4 5 6 7
		"FireAtronach_L Thigh [LThg]", "FireAtronach_L Calf [LClf]", "FireAtronach_L Foot [LLft]", "FireAtronach_L Toe0 [LToe]",  // 8 9 10 11
		"FireAtronach_L UpperArm [LUar]", "FireAtronach_L Forearm [LLar]", "FireAtronach_L Hand [LHnd]",                          // 12 13 14
		"FireAtronach_R UpperArm [RUar]", "FireAtronach_R Forearm [RLar]", "FireAtronach_R Hand [RHnd]"                           // 15 16 17
	},
	{
		// Falmer
		"NPC Brow1", "NPC Head", "NPC Neck",                      // 0 1 2
		"NPC Spine",                                              // 3
		"NPC L Thigh", "NPC L Calf", "NPC L Foot", "NPC L Toe0",  // 4 5 6 7
		"NPC R Thigh", "NPC R Calf", "NPC R Foot", "NPC R Toe0",  // 8 9 10 11
		"NPC R UpperArm", "NPC R Forearm", "NPC R Hand",          // 12 13 14
		"NPC L UpperArm", "NPC L Forearm", "NPC L Hand"           // 15 16 17
	},
	{
		// DwarvenSpider
		"DwarvenSpiderHub", "DwarvenSpiderBody",  // 0 1

		// head
		"DwarvenSpiderRightUpperArm",  // 2
		"DwarvenSpiderLeftUpperArm",   // 3

		// legs
		"DwarvenSpiderRLeg01", "DwarvenSpiderRLeg02", "DwarvenSpiderRLeg03",              // 4 5 6
		"DwarvenSpiderRLeg11", "DwarvenSpiderRLeg12", "DwarvenSpiderRLeg13",              // 7 8 9
		"DwarvenSpiderRLeg21", "DwarvenSpiderRLeg22", "DwarvenSpiderRLeg23",              // 10 11 12
		"DwarvenSpiderLLeg21", "DwarvenSpiderLLeg22", "DwarvenSpiderLLeg23",              // 13 14 15
		"DwarvenSpider_LLeg11", "DwarvenSpider_LLeg12", "DwarvenSpider_LLeg13",           // 16 17 18
		"DwarvenSpider_LegLeft01", "DwarvenSpider_LegLeft02", "DwarvenSpider_LegLeft03",  // 19 20 21

		// long actual arms
		"DwarvenSpiderRightForarm", "DwarvenSpiderRightPincher1",  // 22 23
		"DwarvenSpiderLeftForarm", "DwarvenSpiderLeftPincher1"     // 24 25
	},
	{
		// Sphere
		"NPC L Wing", "NPC Head", "NPC Spine2",                   // 0 1 2
		"NPC Spine1",                                             // 3
		"NPC L Thigh", "NPC L Calf", "NPC L Wheel",               // 4 5 6
		"NPC R Thigh", "NPC R Calf", "NPC R Wheel",               // 7 8 9
		"NPC R Upperarm", "NPC R Forearm", "NPC R FoldingBlade",  // 10 11 12
		"NPC L Upperarm", "NPC L Forearm", "NPC L Hand"           // 13 14 15
	},
	{
		// Centurion
		"LagBone", "NPC R Pauldron",                                                           // 0 1
		"NPC Spine2 [Spn2]", "NPC Spine [Spn1]",                                               // 2 3
		"NPC L Thigh [LThg]", "NPC L Calf [LClf]", "NPC L Foot [Ltft ]", "NPC L Toe0 [LToe]",  // 4 5 6 7
		"NPC R Thigh [RThg]", "NPC R Calf [RClf]", "NPC R Foot [Rtft ]", "NPC R Toe0 [RToe]",  // 8 9 10 11
		"NPC L UpperArm [LUar]", "NPC L Forearm [LLar]",                                       // 12 13
		"NPC R UpperArm [RUar]", "NPC R Forearm [RLar]", "WEAPON"                              // 14 15 16
	},
	{
		// Bear
		"NPC LowerFrontLip", "NPC Head", "NPC Neck1",            // 0 1 2
		"NPC Spine3", "NPC Spine1",                              // 3 4
		"NPC LLeg1", "NPC LLeg2", "NPC LLegAnkle", "NPC L_Toe",  // 5 6 7 8
		"NPC RLeg1", "NPC RLeg2", "NPC RLegAnkle", "NPC R_Toe",  // 9 10 11 12
		"NPC LArm2", "NPC LArmPalm", "NPC LMiddle02",            // 13 14 15
		"NPC RArm2", "NPC RArmPalm", "NPC RMiddle02"             // 16 17 18
	},
	{
		// ChaurusFlyer
		"ChaurusFlyerRightPincherB", "ChaurusFlyerHead", "ChaurusFlyerTorso",                                                  // 0 1 2
		"ChaurusFlyerTail1",                                                                                                   // 3
		"ChaurusFlyerRLegCollarbone", "ChaurusFlyerRLeg1", "ChaurusFlyerRLeg2", "ChaurusFlyerRLeg3", "ChaurusFlyerRLegAnkle",  // 4 5 6 7 8
		"ChaurusFlyerLLegCollarbone", "ChaurusFlyerLLeg1", "ChaurusFlyerLLeg2", "ChaurusFlyerLLeg3", "ChaurusFlyerLLegAnkle",  // 9 10 11 12 13
		"ChaurusFlyerRArm1", "ChaurusFlyerRArm2", "ChaurusFlyerRArm3", "ChaurusFlyerRArmPalm",                                 // 14 15 16 17
		"ChaurusFlyerLArm1", "ChaurusFlyerLArm2", "ChaurusFlyerLArm3", "ChaurusFlyerLArmPalm",                                 // 18 19 20 21
		"ChaurusFlyerTail4", "ChaurusFlyerTail8"                                                                               // 22 23
	},
	{
		// Scrib
		"Jaw[ToothL]", "Head [Mouth]", "Neck [Neck]",  // 0 1 2
		"Spine [Spn0]", "Tail2", "TailStinger",        // 3 4 5

		// legs
		"Leg1[LThg]", "Leg1Calf[LClf]", "Leg1Foot[Lft]", "Leg1(Ltoe1)",  // 6 7 8 9
		"Leg2[LThg]", "Leg2Calf[LClf]", "Leg2Foot[Lft]", "Leg2(Ltoe1)",  // 10 11 12 13
		"Leg3[LThg]", "Leg3Calf[LClf]", "Leg3Foot[Lft]", "Leg3(Ltoe1)",  // 14 15 16 17
		"Leg3[RThg]", "Leg3Calf[RClf]", "Leg3Foot[Rft]", "Leg3(Rtoe1)",  // 18 19 20 21
		"Leg2[RThg]", "Leg2Calf[RClf]", "Leg2Foot[Rft]", "Leg2(Rtoe1)",  // 22 23 24 25
		"Leg1[RThg]", "Leg1Calf[RClf]", "Leg1Foot[Rft]", "Leg1(Rtoe1)"   // 26 27 28 29
	},
	{
		// Vampire
		"NPC Jaw", "NPC Head [Head]", "NPC Neck [Neck]",                                      // 0 1 2
		"NPC Spine [Spn0]",                                                                   // 3
		"NPC L Thigh [LThg]", "NPC L Calf [LClf]", "NPC L Foot [Lft ]", "NPC L Toe0 [LToe]",  // 4 5 6 7
		"NPC R Thigh [RThg]", "NPC R Calf [RClf]", "NPC R Foot [Rft ]", "NPC R Toe0 [RToe]",  // 8 9 10 11
		"NPC R UpperArm [RUar]", "NPC R Forearm [RLar]", "NPC R Hand [RHnd]",                 // 12 13 14
		"NPC L UpperArm [LUar]", "NPC L Forearm [LLar]", "NPC L Hand [LHnd]"                  // 15 16 17
	},
	{
		// HMDaedra
		"NPC R HeadNoodle11", "NPC R HeadNoodle09", "NPC L HeadNoodle09", "NPC L HeadNoodle11",  // 0 1 2 3
		"NPC R Tail05", "NPC R LHand", "NPC L LHand", "NPC L Tail05",                            // 4 5 6 7
		"NPC R ForeArm", "NPC R Hand", "NPC R Finger05",                                         // 8 9 10
		"NPC L ForeArm", "NPC L Hand", "NPC L Finger05",                                         // 11 12 13
		"NPC M Tail07", "NPC R HeadNoodle02", "NPC L HeadNoodle02"                               // 14 15 16
	},
	{
		// Riekling
		"NPC Brow1", "NPC Head", "NPC Neck",        // 0 1 2
		"NPC Spine",                                              // 3
		"NPC L Thigh", "NPC L Calf", "NPC L Foot", "NPC L Toe0",  // 4 5 6 7
		"NPC R Thigh", "NPC R Calf", "NPC R Foot", "NPC R Toe0",  // 8 9 10 11
		"NPC R UpperarmTwist1", "NPC R Forearm", "NPC R Hand",    // 12 13 14
		"NPC L UpperarmTwist1", "NPC L Forearm", "NPC L Hand"     // 15 16 17
	}
};

inline std::vector<std::tuple<int, int, Slot, Limbs>> Bones[] = {
	{ // Human
		// head
		{ 0, 1, Slot::kHead, Limbs::Head },
		{ 1, 2, Slot::kHead, Limbs::Head },

		// body
		{ 2, 3, Slot::kBody, Limbs::Body },

		// left leg
		{ 4, 5, Slot::kFeet, Limbs::Legs },
		{ 5, 6, Slot::kFeet, Limbs::Legs },
		{ 6, 7, Slot::kFeet, Limbs::Legs },

		// right leg
		{ 8, 9, Slot::kFeet, Limbs::Legs },
		{ 9, 10, Slot::kFeet, Limbs::Legs },
		{ 10, 11, Slot::kFeet, Limbs::Legs },

		// left arm
		{ 12, 13, Slot::kHands, Limbs::Arms },
		{ 13, 14, Slot::kHands, Limbs::Arms },

		// right arm
		{ 15, 16, Slot::kHands, Limbs::Arms },
		{ 16, 17, Slot::kHands, Limbs::Arms } },
	{ // Draugr
		// head
		{ 0, 1, Slot::kHead, Limbs::Head },
		{ 1, 2, Slot::kHead, Limbs::Head },

		// body
		{ 2, 3, Slot::kBody, Limbs::Body },

		// left leg
		{ 4, 5, Slot::kFeet, Limbs::Legs },
		{ 5, 6, Slot::kFeet, Limbs::Legs },
		{ 6, 7, Slot::kFeet, Limbs::Legs },

		// right leg
		{ 8, 9, Slot::kFeet, Limbs::Legs },
		{ 9, 10, Slot::kFeet, Limbs::Legs },
		{ 10, 11, Slot::kFeet, Limbs::Legs },

		// left arm
		{ 12, 13, Slot::kHands, Limbs::Arms },
		{ 13, 14, Slot::kHands, Limbs::Arms },

		// right arm
		{ 15, 16, Slot::kHands, Limbs::Arms },
		{ 16, 17, Slot::kHands, Limbs::Arms } },
	{ // Dog
		// head
		{ 0, 1, Slot::kHead, Limbs::Head },
		{ 1, 2, Slot::kHead, Limbs::Head },

		// body
		{ 2, 3, Slot::kBody, Limbs::Body },

		// left leg
		{ 4, 5, Slot::kFeet, Limbs::Legs },
		{ 5, 6, Slot::kFeet, Limbs::Legs },
		{ 6, 7, Slot::kFeet, Limbs::Legs },

		// right leg
		{ 8, 9, Slot::kFeet, Limbs::Legs },
		{ 9, 10, Slot::kFeet, Limbs::Legs },
		{ 10, 11, Slot::kFeet, Limbs::Legs },

		// left arm
		{ 12, 13, Slot::kHands, Limbs::Arms },
		{ 13, 14, Slot::kHands, Limbs::Arms },
		{ 14, 15, Slot::kHands, Limbs::Arms },

		// right arm
		{ 16, 17, Slot::kHands, Limbs::Arms },
		{ 17, 18, Slot::kHands, Limbs::Arms },
		{ 18, 19, Slot::kHands, Limbs::Arms },

		// tail
		{ 3, 20, Slot::kTail, Limbs::Legs } },
	{ // Dragon
		// head
		{ 0, 1, Slot::kHead, Limbs::Head },
		{ 1, 2, Slot::kHead, Limbs::Head },
		{ 2, 3, Slot::kHead, Limbs::Head },

		// body
		{ 3, 4, Slot::kBody, Limbs::Body },

		// left leg
		{ 5, 6, Slot::kFeet, Limbs::Legs },
		{ 6, 7, Slot::kFeet, Limbs::Legs },
		{ 7, 8, Slot::kFeet, Limbs::Legs },

		// right leg
		{ 9, 10, Slot::kFeet, Limbs::Legs },
		{ 10, 11, Slot::kFeet, Limbs::Legs },
		{ 11, 12, Slot::kFeet, Limbs::Legs },

		// TODO: arr wings injuries
		// left arm
		{ 13, 14, Slot::kHands, Limbs::Arms },
		{ 14, 15, Slot::kHands, Limbs::Arms },
		{ 15, 16, Slot::kHands, Limbs::Arms },

		// right arm
		{ 17, 18, Slot::kHands, Limbs::Arms },
		{ 18, 19, Slot::kHands, Limbs::Arms },
		{ 19, 20, Slot::kHands, Limbs::Arms },

		// tail
		{ 4, 21, Slot::kTail, Limbs::Legs },
		{ 21, 22, Slot::kTail, Limbs::Legs },
		{ 22, 23, Slot::kTail, Limbs::Legs },
		{ 23, 24, Slot::kTail, Limbs::Legs },
		{ 24, 25, Slot::kTail, Limbs::Legs } },
	{ // Deer
		// head
		{ 0, 1, Slot::kHead, Limbs::Head },
		{ 1, 2, Slot::kHead, Limbs::Head },

		// body
		{ 2, 3, Slot::kBody, Limbs::Body },

		// left leg
		{ 4, 5, Slot::kFeet, Limbs::Legs },
		{ 5, 6, Slot::kFeet, Limbs::Legs },
		{ 6, 7, Slot::kFeet, Limbs::Legs },

		// right leg
		{ 8, 9, Slot::kFeet, Limbs::Legs },
		{ 9, 10, Slot::kFeet, Limbs::Legs },
		{ 10, 11, Slot::kFeet, Limbs::Legs },

		// left arm
		{ 12, 13, Slot::kHands, Limbs::Arms },
		{ 13, 14, Slot::kHands, Limbs::Arms },
		{ 14, 15, Slot::kHands, Limbs::Arms },

		// right arm
		{ 16, 17, Slot::kHands, Limbs::Arms },
		{ 17, 18, Slot::kHands, Limbs::Arms },
		{ 18, 19, Slot::kHands, Limbs::Arms },

		// tail
		{ 3, 20, Slot::kTail, Limbs::Legs } },
	{ // Spriggan
		// head
		{ 0, 1, Slot::kHead, Limbs::Head },
		{ 1, 2, Slot::kHead, Limbs::Head },

		// body
		{ 2, 3, Slot::kBody, Limbs::Body },

		// left leg
		{ 4, 5, Slot::kFeet, Limbs::Legs },
		{ 5, 6, Slot::kFeet, Limbs::Legs },
		{ 6, 7, Slot::kFeet, Limbs::Legs },

		// right leg
		{ 8, 9, Slot::kFeet, Limbs::Legs },
		{ 9, 10, Slot::kFeet, Limbs::Legs },
		{ 10, 11, Slot::kFeet, Limbs::Legs },

		// left arm
		{ 12, 13, Slot::kHands, Limbs::Arms },
		{ 13, 14, Slot::kHands, Limbs::Arms },
		{ 14, 15, Slot::kHands, Limbs::Arms },
		{ 14, 16, Slot::kHands, Limbs::Arms },
		{ 14, 17, Slot::kHands, Limbs::Arms },

		// right arm
		{ 18, 19, Slot::kHands, Limbs::Arms },
		{ 19, 20, Slot::kHands, Limbs::Arms },
		{ 20, 21, Slot::kHands, Limbs::Arms },
		{ 20, 22, Slot::kHands, Limbs::Arms },
		{ 20, 23, Slot::kHands, Limbs::Arms } },
	{ // DragonPriest
		// head
		{ 0, 1, Slot::kHead, Limbs::Head },
		{ 1, 2, Slot::kHead, Limbs::Head },

		// body
		{ 2, 3, Slot::kBody, Limbs::Body },

		// left leg
		{ 4, 5, Slot::kFeet, Limbs::Legs },
		{ 5, 6, Slot::kFeet, Limbs::Legs },
		{ 6, 7, Slot::kFeet, Limbs::Legs },

		// right leg
		{ 8, 9, Slot::kFeet, Limbs::Legs },
		{ 9, 10, Slot::kFeet, Limbs::Legs },
		{ 10, 11, Slot::kFeet, Limbs::Legs },

		// left arm
		{ 12, 13, Slot::kHands, Limbs::Arms },
		{ 13, 14, Slot::kHands, Limbs::Arms },

		// right arm
		{ 15, 16, Slot::kHands, Limbs::Arms },
		{ 16, 17, Slot::kHands, Limbs::Arms } },
	{ // Wisp
		// head
		{ 0, 1, Slot::kHead, Limbs::Head },
		{ 1, 2, Slot::kHead, Limbs::Head },

		// body
		{ 2, 3, Slot::kBody, Limbs::Body },

		// left leg
		{ 4, 5, Slot::kFeet, Limbs::Legs },
		{ 5, 6, Slot::kFeet, Limbs::Legs },
		{ 6, 7, Slot::kFeet, Limbs::Legs },

		// right leg
		{ 8, 9, Slot::kFeet, Limbs::Legs },
		{ 9, 10, Slot::kFeet, Limbs::Legs },
		{ 10, 11, Slot::kFeet, Limbs::Legs },

		// left arm
		{ 12, 13, Slot::kHands, Limbs::Arms },
		{ 13, 14, Slot::kHands, Limbs::Arms },

		// right arm
		{ 15, 16, Slot::kHands, Limbs::Arms },
		{ 16, 17, Slot::kHands, Limbs::Arms } },
	{ // Horse
		// head
		{ 0, 1, Slot::kHead, Limbs::Head },
		{ 1, 2, Slot::kHead, Limbs::Head },

		// body
		{ 2, 3, Slot::kBody, Limbs::Body },

		// left leg
		{ 4, 5, Slot::kFeet, Limbs::Legs },
		{ 5, 6, Slot::kFeet, Limbs::Legs },
		{ 6, 7, Slot::kFeet, Limbs::Legs },

		// right leg
		{ 8, 9, Slot::kFeet, Limbs::Legs },
		{ 9, 10, Slot::kFeet, Limbs::Legs },
		{ 10, 11, Slot::kFeet, Limbs::Legs },

		// left arm
		{ 12, 13, Slot::kHands, Limbs::Arms },
		{ 13, 14, Slot::kHands, Limbs::Arms },
		{ 14, 15, Slot::kHands, Limbs::Arms },

		// right arm
		{ 16, 17, Slot::kHands, Limbs::Arms },
		{ 17, 18, Slot::kHands, Limbs::Arms },
		{ 18, 19, Slot::kHands, Limbs::Arms },

		// tail
		{ 3, 20, Slot::kTail, Limbs::Legs },
		{ 20, 21, Slot::kTail, Limbs::Legs } },
	{ // Giant
		// head
		{ 0, 1, Slot::kHead, Limbs::Head },
		{ 1, 2, Slot::kHead, Limbs::Head },

		// body
		{ 2, 3, Slot::kBody, Limbs::Body },

		// left leg
		{ 4, 5, Slot::kFeet, Limbs::Legs },
		{ 5, 6, Slot::kFeet, Limbs::Legs },
		{ 6, 7, Slot::kFeet, Limbs::Legs },

		// right leg
		{ 8, 9, Slot::kFeet, Limbs::Legs },
		{ 9, 10, Slot::kFeet, Limbs::Legs },
		{ 10, 11, Slot::kFeet, Limbs::Legs },

		// left arm
		{ 12, 13, Slot::kHands, Limbs::Arms },
		{ 13, 14, Slot::kHands, Limbs::Arms },

		// right arm
		{ 15, 16, Slot::kHands, Limbs::Arms },
		{ 16, 17, Slot::kHands, Limbs::Arms } },
	{ // Lurker
		// head
		{ 0, 1, Slot::kHead, Limbs::Head },
		{ 1, 2, Slot::kHead, Limbs::Head },

		// body
		{ 2, 3, Slot::kBody, Limbs::Body },

		// left leg
		{ 4, 5, Slot::kFeet, Limbs::Legs },
		{ 5, 6, Slot::kFeet, Limbs::Legs },
		{ 6, 7, Slot::kFeet, Limbs::Legs },

		// right leg
		{ 8, 9, Slot::kFeet, Limbs::Legs },
		{ 9, 10, Slot::kFeet, Limbs::Legs },
		{ 10, 11, Slot::kFeet, Limbs::Legs },

		// left arm
		{ 12, 13, Slot::kHands, Limbs::Arms },
		{ 13, 14, Slot::kHands, Limbs::Arms },

		// right arm
		{ 15, 16, Slot::kHands, Limbs::Arms },
		{ 16, 17, Slot::kHands, Limbs::Arms } },
	{ // Skeever
		// head
		{ 0, 1, Slot::kHead, Limbs::Head },
		{ 1, 2, Slot::kHead, Limbs::Head },

		// body
		{ 2, 3, Slot::kBody, Limbs::Body },

		// left leg
		{ 4, 5, Slot::kFeet, Limbs::Legs },
		{ 5, 6, Slot::kFeet, Limbs::Legs },
		{ 6, 7, Slot::kFeet, Limbs::Legs },

		// right leg
		{ 8, 9, Slot::kFeet, Limbs::Legs },
		{ 9, 10, Slot::kFeet, Limbs::Legs },
		{ 10, 11, Slot::kFeet, Limbs::Legs },

		// left arm
		{ 12, 13, Slot::kHands, Limbs::Arms },
		{ 13, 14, Slot::kHands, Limbs::Arms },

		// right arm
		{ 15, 16, Slot::kHands, Limbs::Arms },
		{ 16, 17, Slot::kHands, Limbs::Arms },

		// tail
		{ 3, 18, Slot::kTail, Limbs::Legs },
		{ 18, 19, Slot::kTail, Limbs::Legs } },
	{ // DraugrS
		// head
		{ 0, 1, Slot::kHead, Limbs::Head },
		{ 1, 2, Slot::kHead, Limbs::Head },

		// body
		{ 2, 3, Slot::kBody, Limbs::Body },

		// left leg
		{ 4, 5, Slot::kFeet, Limbs::Legs },
		{ 5, 6, Slot::kFeet, Limbs::Legs },
		{ 6, 7, Slot::kFeet, Limbs::Legs },

		// right leg
		{ 8, 9, Slot::kFeet, Limbs::Legs },
		{ 9, 10, Slot::kFeet, Limbs::Legs },
		{ 10, 11, Slot::kFeet, Limbs::Legs },

		// left arm
		{ 12, 13, Slot::kHands, Limbs::Arms },
		{ 13, 14, Slot::kHands, Limbs::Arms },

		// right arm
		{ 15, 16, Slot::kHands, Limbs::Arms },
		{ 16, 17, Slot::kHands, Limbs::Arms } },
	{ // Chaurus
		// head
		{ 0, 1, Slot::kHead, Limbs::Head },
		{ 1, 2, Slot::kHead, Limbs::Head },

		// body
		{ 2, 3, Slot::kBody, Limbs::Body },

		// left leg
		{ 4, 5, Slot::kFeet, Limbs::Legs },
		{ 5, 6, Slot::kFeet, Limbs::Legs },
		{ 6, 7, Slot::kFeet, Limbs::Legs },

		// right leg
		{ 8, 9, Slot::kFeet, Limbs::Legs },
		{ 9, 10, Slot::kFeet, Limbs::Legs },
		{ 10, 11, Slot::kFeet, Limbs::Legs },

		// left arm
		{ 12, 13, Slot::kHands, Limbs::Arms },
		{ 13, 14, Slot::kHands, Limbs::Arms },
		{ 14, 15, Slot::kHands, Limbs::Arms },

		// right arm
		{ 16, 17, Slot::kHands, Limbs::Arms },
		{ 17, 18, Slot::kHands, Limbs::Arms },
		{ 18, 19, Slot::kHands, Limbs::Arms },

		// tail
		{ 3, 20, Slot::kTail, Limbs::Legs },
		{ 20, 21, Slot::kTail, Limbs::Legs } },
	{ // Goat
		// head
		{ 0, 1, Slot::kHead, Limbs::Head },
		{ 1, 2, Slot::kHead, Limbs::Head },

		// body
		{ 2, 3, Slot::kBody, Limbs::Body },

		// left leg
		{ 4, 5, Slot::kFeet, Limbs::Legs },
		{ 5, 6, Slot::kFeet, Limbs::Legs },
		{ 6, 7, Slot::kFeet, Limbs::Legs },

		// right leg
		{ 8, 9, Slot::kFeet, Limbs::Legs },
		{ 9, 10, Slot::kFeet, Limbs::Legs },
		{ 10, 11, Slot::kFeet, Limbs::Legs },

		// left arm
		{ 12, 13, Slot::kHands, Limbs::Arms },
		{ 13, 14, Slot::kHands, Limbs::Arms },
		{ 14, 15, Slot::kHands, Limbs::Arms },

		// right arm
		{ 16, 17, Slot::kHands, Limbs::Arms },
		{ 17, 18, Slot::kHands, Limbs::Arms },
		{ 18, 19, Slot::kHands, Limbs::Arms },

		// tail
		{ 3, 20, Slot::kTail, Limbs::Legs } },
	{ // Spider
		// head
		{ 0, 1, Slot::kHead, Limbs::Head },
		{ 1, 2, Slot::kHead, Limbs::Head },
		{ 0, 3, Slot::kHead, Limbs::Head },
		{ 3, 4, Slot::kHead, Limbs::Head },

		// body
		{ 0, 5, Slot::kBody, Limbs::Body },
		{ 0, 9, Slot::kBody, Limbs::Body },
		{ 0, 13, Slot::kBody, Limbs::Body },
		{ 0, 17, Slot::kBody, Limbs::Body },
		{ 0, 21, Slot::kBody, Limbs::Body },
		{ 0, 25, Slot::kBody, Limbs::Body },
		{ 0, 29, Slot::kBody, Limbs::Body },
		{ 0, 33, Slot::kBody, Limbs::Body },

		// legs
		{ 5, 6, Slot::kBody, Limbs::Body },
		{ 6, 7, Slot::kBody, Limbs::Body },
		{ 7, 8, Slot::kBody, Limbs::Body },

		{ 9, 10, Slot::kBody, Limbs::Body },
		{ 10, 11, Slot::kBody, Limbs::Body },
		{ 11, 12, Slot::kBody, Limbs::Body },

		{ 13, 14, Slot::kBody, Limbs::Body },
		{ 14, 15, Slot::kBody, Limbs::Body },
		{ 15, 16, Slot::kBody, Limbs::Body },

		{ 17, 18, Slot::kBody, Limbs::Body },
		{ 18, 19, Slot::kBody, Limbs::Body },
		{ 19, 20, Slot::kBody, Limbs::Body },

		{ 21, 22, Slot::kBody, Limbs::Body },
		{ 22, 23, Slot::kBody, Limbs::Body },
		{ 23, 24, Slot::kBody, Limbs::Body },

		{ 25, 26, Slot::kBody, Limbs::Body },
		{ 26, 27, Slot::kBody, Limbs::Body },
		{ 27, 28, Slot::kBody, Limbs::Body },

		{ 29, 30, Slot::kBody, Limbs::Body },
		{ 30, 31, Slot::kBody, Limbs::Body },
		{ 31, 32, Slot::kBody, Limbs::Body },

		{ 33, 34, Slot::kBody, Limbs::Body },
		{ 34, 35, Slot::kBody, Limbs::Body },
		{ 35, 36, Slot::kBody, Limbs::Body },

		// left arm
		{ 37, 38, Slot::kHands, Limbs::Arms },
		{ 38, 39, Slot::kHands, Limbs::Arms },
		{ 39, 40, Slot::kHands, Limbs::Arms },

		// right arm
		{ 41, 42, Slot::kHands, Limbs::Arms },
		{ 42, 43, Slot::kHands, Limbs::Arms },
		{ 43, 44, Slot::kHands, Limbs::Arms },

		// tail
		{ 0, 45, Slot::kTail, Limbs::Legs } },
	{ // Cow
		// head
		{ 0, 1, Slot::kHead, Limbs::Head },
		{ 1, 2, Slot::kHead, Limbs::Head },

		// body
		{ 2, 3, Slot::kBody, Limbs::Body },

		// left leg
		{ 4, 5, Slot::kFeet, Limbs::Legs },
		{ 5, 6, Slot::kFeet, Limbs::Legs },
		{ 6, 7, Slot::kFeet, Limbs::Legs },

		// right leg
		{ 8, 9, Slot::kFeet, Limbs::Legs },
		{ 9, 10, Slot::kFeet, Limbs::Legs },
		{ 10, 11, Slot::kFeet, Limbs::Legs },

		// left arm
		{ 12, 13, Slot::kHands, Limbs::Arms },
		{ 13, 14, Slot::kHands, Limbs::Arms },
		{ 14, 15, Slot::kHands, Limbs::Arms },

		// right arm
		{ 16, 17, Slot::kHands, Limbs::Arms },
		{ 17, 18, Slot::kHands, Limbs::Arms },
		{ 18, 19, Slot::kHands, Limbs::Arms },

		// tail
		{ 3, 20, Slot::kTail, Limbs::Legs },
		{ 20, 21, Slot::kTail, Limbs::Legs } },
	{ // Troll
		// head
		{ 0, 1, Slot::kHead, Limbs::Head },
		{ 1, 2, Slot::kHead, Limbs::Head },

		// body
		{ 2, 3, Slot::kBody, Limbs::Body },

		// left leg
		{ 4, 5, Slot::kFeet, Limbs::Legs },
		{ 5, 6, Slot::kFeet, Limbs::Legs },
		{ 6, 7, Slot::kFeet, Limbs::Legs },

		// right leg
		{ 8, 9, Slot::kFeet, Limbs::Legs },
		{ 9, 10, Slot::kFeet, Limbs::Legs },
		{ 10, 11, Slot::kFeet, Limbs::Legs },

		// left arm
		{ 12, 13, Slot::kHands, Limbs::Arms },
		{ 13, 14, Slot::kHands, Limbs::Arms },

		// right arm
		{ 15, 16, Slot::kHands, Limbs::Arms },
		{ 16, 17, Slot::kHands, Limbs::Arms } },
	{ // SabreCat
		// head
		{ 0, 1, Slot::kHead, Limbs::Head },
		{ 1, 2, Slot::kHead, Limbs::Head },

		// body
		{ 2, 3, Slot::kBody, Limbs::Body },

		// left leg
		{ 4, 5, Slot::kFeet, Limbs::Legs },
		{ 5, 6, Slot::kFeet, Limbs::Legs },
		{ 6, 7, Slot::kFeet, Limbs::Legs },

		// right leg
		{ 8, 9, Slot::kFeet, Limbs::Legs },
		{ 9, 10, Slot::kFeet, Limbs::Legs },
		{ 10, 11, Slot::kFeet, Limbs::Legs },

		// left arm
		{ 12, 13, Slot::kHands, Limbs::Arms },
		{ 13, 14, Slot::kHands, Limbs::Arms },
		{ 14, 15, Slot::kHands, Limbs::Arms },

		// right arm
		{ 16, 17, Slot::kHands, Limbs::Arms },
		{ 17, 18, Slot::kHands, Limbs::Arms },
		{ 18, 19, Slot::kHands, Limbs::Arms },

		// tail
		{ 3, 20, Slot::kTail, Limbs::Legs },
		{ 20, 21, Slot::kTail, Limbs::Legs } },
	{ // Mamonth
		// head
		{ 0, 1, Slot::kHead, Limbs::Head },
		{ 1, 2, Slot::kHead, Limbs::Head },
		{ 2, 3, Slot::kHead, Limbs::Head },
		{ 3, 4, Slot::kHead, Limbs::Head },

		// body
		{ 4, 5, Slot::kBody, Limbs::Body },

		// left leg
		{ 6, 7, Slot::kFeet, Limbs::Legs },
		{ 7, 8, Slot::kFeet, Limbs::Legs },

		// right leg
		{ 9, 10, Slot::kFeet, Limbs::Legs },
		{ 10, 11, Slot::kFeet, Limbs::Legs },

		// left arm
		{ 12, 13, Slot::kHands, Limbs::Arms },
		{ 13, 14, Slot::kHands, Limbs::Arms },

		// right arm
		{ 15, 16, Slot::kHands, Limbs::Arms },
		{ 16, 17, Slot::kHands, Limbs::Arms },

		// tail
		{ 5, 18, Slot::kTail, Limbs::Legs },
		{ 18, 19, Slot::kTail, Limbs::Legs } },
	{ // Horker
		// head
		{ 0, 1, Slot::kHead, Limbs::Head },
		{ 1, 2, Slot::kHead, Limbs::Head },

		// body
		{ 2, 3, Slot::kBody, Limbs::Body },

		// left leg
		{ 4, 5, Slot::kFeet, Limbs::Legs },
		{ 5, 6, Slot::kFeet, Limbs::Legs },
		{ 6, 7, Slot::kFeet, Limbs::Legs },

		// right leg
		{ 8, 9, Slot::kFeet, Limbs::Legs },
		{ 9, 10, Slot::kFeet, Limbs::Legs },
		{ 10, 11, Slot::kFeet, Limbs::Legs },

		// left arm
		{ 12, 13, Slot::kHands, Limbs::Arms },
		{ 13, 14, Slot::kHands, Limbs::Arms },
		{ 14, 15, Slot::kHands, Limbs::Arms },

		// right arm
		{ 16, 17, Slot::kHands, Limbs::Arms },
		{ 17, 18, Slot::kHands, Limbs::Arms },
		{ 18, 19, Slot::kHands, Limbs::Arms } },
	{ // Hagraven
		// head
		{ 0, 1, Slot::kHead, Limbs::Head },
		{ 1, 2, Slot::kHead, Limbs::Head },

		// body
		{ 2, 3, Slot::kBody, Limbs::Body },

		// left leg
		{ 4, 5, Slot::kFeet, Limbs::Legs },
		{ 5, 6, Slot::kFeet, Limbs::Legs },
		{ 6, 7, Slot::kFeet, Limbs::Legs },

		// right leg
		{ 8, 9, Slot::kFeet, Limbs::Legs },
		{ 9, 10, Slot::kFeet, Limbs::Legs },
		{ 10, 11, Slot::kFeet, Limbs::Legs },

		// left arm
		{ 12, 13, Slot::kHands, Limbs::Arms },
		{ 13, 14, Slot::kHands, Limbs::Arms },

		// right arm
		{ 15, 16, Slot::kHands, Limbs::Arms },
		{ 16, 17, Slot::kHands, Limbs::Arms } },
	{ // AtronachFrost
		// head
		{ 0, 1, Slot::kHead, Limbs::Head },
		{ 1, 2, Slot::kHead, Limbs::Head },

		// body
		{ 2, 3, Slot::kBody, Limbs::Body },

		// left leg
		{ 4, 5, Slot::kFeet, Limbs::Legs },
		{ 5, 6, Slot::kFeet, Limbs::Legs },

		// right leg
		{ 7, 8, Slot::kFeet, Limbs::Legs },
		{ 8, 9, Slot::kFeet, Limbs::Legs },

		// left arm
		{ 10, 11, Slot::kHands, Limbs::Arms },

		// right arm
		{ 12, 13, Slot::kHands, Limbs::Arms } },
	{ // AtronachFlame
		// head
		{ 0, 1, Slot::kHead, Limbs::Head },
		{ 1, 2, Slot::kHead, Limbs::Head },

		// body
		{ 2, 3, Slot::kBody, Limbs::Body },

		// left leg
		{ 4, 5, Slot::kFeet, Limbs::Legs },
		{ 5, 6, Slot::kFeet, Limbs::Legs },
		{ 6, 7, Slot::kFeet, Limbs::Legs },

		// right leg
		{ 8, 9, Slot::kFeet, Limbs::Legs },
		{ 9, 10, Slot::kFeet, Limbs::Legs },
		{ 10, 11, Slot::kFeet, Limbs::Legs },

		// left arm
		{ 12, 13, Slot::kHands, Limbs::Arms },
		{ 13, 14, Slot::kHands, Limbs::Arms },

		// right arm
		{ 15, 16, Slot::kHands, Limbs::Arms },
		{ 16, 17, Slot::kHands, Limbs::Arms } },
	{ // Falmer
		// head
		{ 0, 1, Slot::kHead, Limbs::Head },
		{ 1, 2, Slot::kHead, Limbs::Head },

		// body
		{ 2, 3, Slot::kBody, Limbs::Body },

		// left leg
		{ 4, 5, Slot::kFeet, Limbs::Legs },
		{ 5, 6, Slot::kFeet, Limbs::Legs },
		{ 6, 7, Slot::kFeet, Limbs::Legs },

		// right leg
		{ 8, 9, Slot::kFeet, Limbs::Legs },
		{ 9, 10, Slot::kFeet, Limbs::Legs },
		{ 10, 11, Slot::kFeet, Limbs::Legs },

		// left arm
		{ 12, 13, Slot::kHands, Limbs::Arms },
		{ 13, 14, Slot::kHands, Limbs::Arms },

		// right arm
		{ 15, 16, Slot::kHands, Limbs::Arms },
		{ 16, 17, Slot::kHands, Limbs::Arms } },
	{ // DwarvenSpider
		// head
		{ 1, 2, Slot::kHead, Limbs::Head },
		{ 1, 3, Slot::kHead, Limbs::Head },

		// body
		{ 0, 1, Slot::kBody, Limbs::Body },
		{ 0, 4, Slot::kBody, Limbs::Body },
		{ 0, 7, Slot::kBody, Limbs::Body },
		{ 0, 10, Slot::kBody, Limbs::Body },
		{ 0, 13, Slot::kBody, Limbs::Body },
		{ 0, 16, Slot::kBody, Limbs::Body },
		{ 0, 19, Slot::kBody, Limbs::Body },

		// legs
		{ 4, 5, Slot::kBody, Limbs::Body },
		{ 5, 6, Slot::kBody, Limbs::Body },

		{ 7, 8, Slot::kBody, Limbs::Body },
		{ 8, 9, Slot::kBody, Limbs::Body },

		{ 10, 11, Slot::kBody, Limbs::Body },
		{ 11, 12, Slot::kBody, Limbs::Body },

		{ 13, 14, Slot::kBody, Limbs::Body },
		{ 14, 15, Slot::kBody, Limbs::Body },

		{ 16, 17, Slot::kBody, Limbs::Body },
		{ 17, 18, Slot::kBody, Limbs::Body },

		{ 19, 20, Slot::kBody, Limbs::Body },
		{ 20, 21, Slot::kBody, Limbs::Body },

		// left arm
		{ 2, 22, Slot::kHands, Limbs::Arms },
		{ 22, 23, Slot::kHands, Limbs::Arms },

		// right arm
		{ 3, 24, Slot::kHands, Limbs::Arms },
		{ 24, 25, Slot::kHands, Limbs::Arms } },
	{ // Sphere
		// head
		{ 0, 1, Slot::kHead, Limbs::Head },
		{ 1, 2, Slot::kHead, Limbs::Head },

		// body
		{ 2, 3, Slot::kBody, Limbs::Body },

		// left leg
		{ 4, 5, Slot::kFeet, Limbs::Legs },
		{ 5, 6, Slot::kFeet, Limbs::Legs },

		// right leg
		{ 7, 8, Slot::kFeet, Limbs::Legs },
		{ 8, 9, Slot::kFeet, Limbs::Legs },

		// left arm
		{ 10, 11, Slot::kHands, Limbs::Arms },
		{ 11, 12, Slot::kHands, Limbs::Arms },

		// right arm
		{ 13, 14, Slot::kHands, Limbs::Arms },
		{ 14, 15, Slot::kHands, Limbs::Arms } },
	{ // Centurion
		// head
		{ 0, 1, Slot::kHead, Limbs::Head },

		// body
		{ 2, 3, Slot::kBody, Limbs::Body },

		// left leg
		{ 4, 5, Slot::kFeet, Limbs::Legs },
		{ 5, 6, Slot::kFeet, Limbs::Legs },
		{ 6, 7, Slot::kFeet, Limbs::Legs },

		// right leg
		{ 8, 9, Slot::kFeet, Limbs::Legs },
		{ 9, 10, Slot::kFeet, Limbs::Legs },
		{ 10, 11, Slot::kFeet, Limbs::Legs },

		// left arm
		{ 12, 13, Slot::kHands, Limbs::Arms },

		// right arm
		{ 14, 15, Slot::kHands, Limbs::Arms },
		{ 15, 16, Slot::kHands, Limbs::Arms } },
	{ // Bear
		// head
		{ 0, 1, Slot::kHead, Limbs::Head },
		{ 1, 2, Slot::kHead, Limbs::Head },

		// body
		{ 2, 3, Slot::kBody, Limbs::Body },
		{ 3, 4, Slot::kBody, Limbs::Body },

		// left leg
		{ 5, 6, Slot::kFeet, Limbs::Legs },
		{ 6, 7, Slot::kFeet, Limbs::Legs },
		{ 7, 8, Slot::kFeet, Limbs::Legs },

		// right leg
		{ 9, 10, Slot::kFeet, Limbs::Legs },
		{ 10, 11, Slot::kFeet, Limbs::Legs },
		{ 11, 12, Slot::kFeet, Limbs::Legs },

		// left arm
		{ 13, 14, Slot::kHands, Limbs::Arms },
		{ 14, 15, Slot::kHands, Limbs::Arms },

		// right arm
		{ 16, 17, Slot::kHands, Limbs::Arms },
		{ 17, 18, Slot::kHands, Limbs::Arms } },
	{ // ChaurusFlyer
		// head
		{ 0, 1, Slot::kHead, Limbs::Head },
		{ 1, 2, Slot::kHead, Limbs::Head },

		// body
		{ 2, 3, Slot::kBody, Limbs::Body },

		// left leg
		{ 4, 5, Slot::kFeet, Limbs::Legs },
		{ 5, 6, Slot::kFeet, Limbs::Legs },
		{ 6, 7, Slot::kFeet, Limbs::Legs },
		{ 7, 8, Slot::kFeet, Limbs::Legs },

		// right leg
		{ 9, 10, Slot::kFeet, Limbs::Legs },
		{ 10, 11, Slot::kFeet, Limbs::Legs },
		{ 11, 12, Slot::kFeet, Limbs::Legs },
		{ 12, 13, Slot::kFeet, Limbs::Legs },

		// left arm
		{ 14, 15, Slot::kHands, Limbs::Arms },
		{ 15, 16, Slot::kHands, Limbs::Arms },
		{ 16, 17, Slot::kHands, Limbs::Arms },

		// right arm
		{ 18, 19, Slot::kHands, Limbs::Arms },
		{ 19, 20, Slot::kHands, Limbs::Arms },
		{ 20, 21, Slot::kHands, Limbs::Arms },

		// tail
		{ 22, 23, Slot::kHands, Limbs::Arms } },
	{ // Scrib
		// head
		{ 0, 1, Slot::kHead, Limbs::Head },
		{ 1, 2, Slot::kHead, Limbs::Head },

		// body
		{ 2, 3, Slot::kHead, Limbs::Head },
		{ 3, 4, Slot::kHead, Limbs::Head },
		{ 4, 5, Slot::kHead, Limbs::Head },

		{ 3, 6, Slot::kBody, Limbs::Body },
		{ 3, 10, Slot::kBody, Limbs::Body },
		{ 3, 14, Slot::kBody, Limbs::Body },
		{ 3, 18, Slot::kBody, Limbs::Body },
		{ 3, 22, Slot::kBody, Limbs::Body },
		{ 3, 26, Slot::kBody, Limbs::Body },

		// legs
		{ 6, 7, Slot::kBody, Limbs::Body },
		{ 7, 8, Slot::kBody, Limbs::Body },
		{ 8, 9, Slot::kBody, Limbs::Body },

		{ 10, 11, Slot::kBody, Limbs::Body },
		{ 11, 12, Slot::kBody, Limbs::Body },
		{ 12, 13, Slot::kBody, Limbs::Body },

		{ 14, 15, Slot::kBody, Limbs::Body },
		{ 15, 16, Slot::kBody, Limbs::Body },
		{ 16, 17, Slot::kBody, Limbs::Body },

		{ 18, 19, Slot::kBody, Limbs::Body },
		{ 19, 20, Slot::kBody, Limbs::Body },
		{ 20, 21, Slot::kBody, Limbs::Body },

		{ 22, 23, Slot::kBody, Limbs::Body },
		{ 23, 24, Slot::kBody, Limbs::Body },
		{ 24, 25, Slot::kBody, Limbs::Body },

		{ 26, 27, Slot::kBody, Limbs::Body },
		{ 27, 28, Slot::kBody, Limbs::Body },
		{ 28, 29, Slot::kBody, Limbs::Body } },
	{ // Vampire
		// head
		{ 0, 1, Slot::kHead, Limbs::Head },
		{ 1, 2, Slot::kHead, Limbs::Head },

		// body
		{ 2, 3, Slot::kBody, Limbs::Body },

		// left leg
		{ 4, 5, Slot::kFeet, Limbs::Legs },
		{ 5, 6, Slot::kFeet, Limbs::Legs },
		{ 6, 7, Slot::kFeet, Limbs::Legs },

		// right leg
		{ 8, 9, Slot::kFeet, Limbs::Legs },
		{ 9, 10, Slot::kFeet, Limbs::Legs },
		{ 10, 11, Slot::kFeet, Limbs::Legs },

		// left arm
		{ 12, 13, Slot::kHands, Limbs::Arms },
		{ 13, 14, Slot::kHands, Limbs::Arms },

		// right arm
		{ 15, 16, Slot::kHands, Limbs::Arms },
		{ 16, 17, Slot::kHands, Limbs::Arms } },
	{ // Vampire
		// head
		{ 0, 1, Slot::kHead, Limbs::Head },
		{ 1, 2, Slot::kHead, Limbs::Head },
		{ 2, 3, Slot::kHead, Limbs::Head },

		// body
		{ 4, 5, Slot::kBody, Limbs::Body },
		{ 5, 6, Slot::kBody, Limbs::Body },
		{ 6, 7, Slot::kBody, Limbs::Body },
		{ 14, 15, Slot::kBody, Limbs::Body },
		{ 14, 16, Slot::kBody, Limbs::Body },

		// left arm
		{ 8, 9, Slot::kHands, Limbs::Arms },
		{ 9, 10, Slot::kHands, Limbs::Arms },

		// right arm
		{ 11, 12, Slot::kHands, Limbs::Arms },
		{ 12, 13, Slot::kHands, Limbs::Arms } },
	{ // Riekling
		// head
		{ 0, 1, Slot::kHead, Limbs::Head },
		{ 1, 2, Slot::kHead, Limbs::Head },

		// body
		{ 2, 3, Slot::kBody, Limbs::Body },

		// left leg
		{ 4, 5, Slot::kFeet, Limbs::Legs },
		{ 5, 6, Slot::kFeet, Limbs::Legs },
		{ 6, 7, Slot::kFeet, Limbs::Legs },

		// right leg
		{ 8, 9, Slot::kFeet, Limbs::Legs },
		{ 9, 10, Slot::kFeet, Limbs::Legs },
		{ 10, 11, Slot::kFeet, Limbs::Legs },

		// left arm
		{ 12, 13, Slot::kHands, Limbs::Arms },
		{ 13, 14, Slot::kHands, Limbs::Arms },

		// right arm
		{ 15, 16, Slot::kHands, Limbs::Arms },
		{ 16, 17, Slot::kHands, Limbs::Arms } }
};

const static float ArmorPartCoeffs[] = { 1.0f / 0.22f, 1.0f / 0.47f, 1.0f / 0.16f, 1.0f / 0.16f, 1.0f / 0.47f };

namespace DrawDebug
{
	namespace Colors
	{
		static constexpr glm::vec4 RED = glm::vec4(1.0f, 0.0f, 0.0f, 1.0f);
		static constexpr glm::vec4 GRN = glm::vec4(0.0f, 1.0f, 0.0f, 1.0f);
		static constexpr glm::vec4 BLU = glm::vec4(0.0f, 0.0f, 1.0f, 1.0f);
	}

	template <glm::vec4 Color = Colors::RED, int time = 3000>
	void draw_line(const RE::NiPoint3& _from, const RE::NiPoint3& _to, float size = 5.0f)
	{
		glm::vec3 from(_from.x, _from.y, _from.z);
		glm::vec3 to(_to.x, _to.y, _to.z);
		DebugAPI::DrawLineForMS(from, to, time, Color, size);
	}

	template <glm::vec4 Color = Colors::RED, int time = 3000>
	void draw_bone(RE::Actor* a, Skeletons type, int i1, int i2)
	{
		auto node1 = a->GetNodeByName(NodeNames[type][i1])->world.translate;
		auto node2 = a->GetNodeByName(NodeNames[type][i2])->world.translate;

		draw_line<Color, time>(node1, node2, 2.0f);
	}
}
using namespace DrawDebug::Colors;
using DrawDebug::draw_bone;
using DrawDebug::draw_line;

static float get_bone_dist(RE::Actor* a, Skeletons type, int i1, int i2, const NiPoint3& P, const NiPoint3& Q)
{
	auto node1 = a->GetNodeByName(NodeNames[type][i1])->world.translate;
	auto node2 = a->GetNodeByName(NodeNames[type][i2])->world.translate;

	return dist(node1, node2, P, Q);
}

std::vector<float> get_bones_dists(RE::Actor* a, Skeletons type, const NiPoint3& P, const NiPoint3& Q)
{
	std::vector<float> ans;
	
	auto& bones = Bones[type];

	//logger::info("type = {}", type);

	for (int i = 0; i < bones.size(); ++i) {
		ans.push_back(get_bone_dist(a, type, std::get<0>(bones[i]), std::get<1>(bones[i]), P, Q));
	}

	return ans;
}

auto& get_skeleton_path(RE::TESRace* race, RE::SEX male)
{
	return race->skeletonModels[male].model;
}

auto& get_skeleton_path(RE::Actor* a)
{
	RE::TESRace* race = a->race;
	auto male = a->GetActorBase()->GetSex();
	return get_skeleton_path(race, male);
}

bool isHumanoid(RE::Actor* a, Skeletons type)
{
	auto& nodeNames = NodeNames[type];

#ifdef DEBUG
	logger::info("checking {}. skeleton at {}", a->GetName(), get_skeleton_path(a));
#endif  // DEBUG

	bool ans = true;
	for (int i = 0; i < nodeNames.size(); i++) {
		auto node = a->GetNodeByName(nodeNames[i]);
		if (!node) {
			ans = false;
			logger::info("  node {} not found (ind {})", nodeNames[i], i);
			break;
		}

#ifdef DEBUG
		//logger::info("    {} ({}) at {}", nodeNames[i], i, to_string(node->world.translate));
#endif  // DEBUG
	}
	return ans;
}

RE::NiPoint3 TransformVectorByMatrix(const RE::NiPoint3& a_vector, const RE::NiMatrix3& a_matrix)
{
	return RE::NiPoint3(a_matrix.entry[0][0] * a_vector.x + a_matrix.entry[0][1] * a_vector.y + a_matrix.entry[0][2] * a_vector.z,
		a_matrix.entry[1][0] * a_vector.x + a_matrix.entry[1][1] * a_vector.y + a_matrix.entry[1][2] * a_vector.z,
		a_matrix.entry[2][0] * a_vector.x + a_matrix.entry[2][1] * a_vector.y + a_matrix.entry[2][2] * a_vector.z);
}

RE::BSTempEffectParticle* CELL__PlaceParticleEffect(RE::TESObjectCELL* _this, float a_lifetime, const char* a_modelName, const RE::NiMatrix3& a_normal, const NiPoint3& a_pos, float a_scale, std::uint32_t a_flags, RE::NiAVObject* a_target)
{
	using func_t = decltype(&CELL__PlaceParticleEffect);
	REL::Relocation<func_t> func{ REL::ID(29219) };
	return func(_this, a_lifetime, a_modelName, a_normal, a_pos, a_scale, a_flags, a_target);
}

void draw_player_weapon(float)
{
	//NiPoint3 tmp;
	auto a = RE::PlayerCharacter::GetSingleton();
	float reach = FenixUtils::PlayerCharacter__get_reach(a) * 0.1f;
	//Character__get_weapon_vector_625DD0(attacker, false, &tmp);
	//draw_line<GRN>(eye_pos, eye_pos + tmp * reach);
	auto root = netimmerse_cast<RE::BSFadeNode*>(a->Get3D());
	if (!root)
		return;
	auto bone = netimmerse_cast<RE::NiNode*>(root->GetObjectByName("WEAPON"));
	if (!bone)
		return;
	RE::NiPoint3 startPos = bone->world.translate;
	auto weaponDirection = TransformVectorByMatrix({ 0.0f, 1.0f, 0.0f }, bone->world.rotate);
	RE::NiPoint3 endPos = startPos + weaponDirection * reach;
	//draw_line<BLU, 100>(startPos, endPos);
	auto hud = DebugAPI::GetHUD();
	if (!hud || !hud->uiMovie)
		return;
	glm::vec3 from = { startPos.x, startPos.y, startPos.z };
	glm::vec3 to = { endPos.x, endPos.y, endPos.z };
	DebugAPI::DrawLine3D(hud->uiMovie, from, to, BLU, 5.0f);
}

void get_attacker_thing_melee(RE::Actor* attacker, NiPoint3& hit_pos, NiPoint3& eye_pos)
{
	float reach = FenixUtils::PlayerCharacter__get_reach(attacker) * 0.75f;
	FenixUtils::Actor__get_eye_pos(attacker, &hit_pos, 1);
	eye_pos = hit_pos;
	if (attacker->IsPlayerRef()) {
		hit_pos += rotate(reach, attacker->data.angle);
	} else {
		hit_pos += (attacker->data.location - hit_pos) * 0.25f;
		hit_pos += rotateZ(reach, attacker->data.angle);
	}
}

Skeletons get_skeleton_type(const RE::BSFixedString& path)
{
	switch (hash_lowercase(path.data(), path.size())) {
	case "Actors\\Character\\Character Assets\\skeleton.nif"_hl:
	case "actors\\Character\\Character Assets Female\\skeleton_female.nif"_hl:
	case "Actors\\WerewolfBeast\\Character Assets\\skeleton.nif"_hl:
	case "Actors\\Character\\Character Assets\\skeletonBeast.nif"_hl:
	case "Actors\\Character\\Character Assets Female\\skeletonBeast_female.nif"_hl:
		return Human;
	case "Actors\\Draugr\\Character Assets\\Skeleton.nif"_hl:
	case "Actors\\Draugr\\Character Assets\\SkeletonF.nif"_hl:
		return Draugr;
	case "Actors\\Canine\\Character Assets Wolf\\skeleton.nif"_hl:
	case "Actors\\Canine\\Character Assets Dog\\skeleton.nif"_hl:
		return Dog;
	case "Actors\\Dragon\\Character Assets\\Skeleton.nif"_hl:
		return Dragon;
	case "Actors\\Deer\\Character Assets\\Skeleton.nif"_hl:
		return Deer;
	case "Actors\\Spriggan\\Character Assets\\skeleton.nif"_hl:
		return Spriggan;
	case "Actors\\DragonPriest\\Character Assets\\skeleton.nif"_hl:
		return DragonPriest;
	case "Actors\\Wisp\\Character Assets\\skeleton.nif"_hl:
		return Wisp;
	case "Actors\\Horse\\Character Assets\\skeleton.nif"_hl:
		return Horse;
	case "Actors\\Giant\\Character Assets\\skeleton.nif"_hl:
		return Giant;
	case "Actors\\DLC02\\BenthicLurker\\Character Assets\\skeleton.nif"_hl:
		return Lurker;
	case "Actors\\Skeever\\Character Assets\\skeleton.nif"_hl:
		return Skeever;
	case "Actors\\Draugr\\Character Assets\\SkeletonS.nif"_hl:
		return DraugrS;
	case "Actors\\Chaurus\\Character Assets\\skeleton.nif"_hl:
		return Chaurus;
	case "Actors\\Goat\\Character Assets\\skeleton.nif"_hl:
		return Goat;
	case "Actors\\FrostbiteSpider\\Character Assets\\skeleton.nif"_hl:
		return Spider;
	case "Actors\\Cow\\Character Assets\\skeleton.nif"_hl:
		return Cow;
	case "Actors\\Troll\\Character Assets\\skeleton.nif"_hl:
		return Troll;
	case "Actors\\SabreCat\\Character Assets\\Skeleton.nif"_hl:
		return SabreCat;
	case "Actors\\Mammoth\\Character Assets\\skeleton.nif"_hl:
		return Mamonth;
	case "Actors\\Horker\\Character Assets\\skeleton.nif"_hl:
		return Horker;
	case "Actors\\Hagraven\\Character Assets\\skeleton.nif"_hl:
		return Hagraven;
	case "Actors\\AtronachFrost\\Character Assets\\skeleton.nif"_hl:
		return AtronachFrost;
	case "Actors\\AtronachFlame\\Character Assets\\skeleton.nif"_hl:
		return AtronachFlame;
	case "Actors\\Falmer\\Character Assets\\Skeleton.nif"_hl:
		return Falmer;
	case "Actors\\DwarvenSpider\\Character Assets\\skeleton.nif"_hl:
		return DwarvenSpider;
	case "Actors\\DwarvenSphereCenturion\\Character Assets\\skeleton.nif"_hl:
		return Sphere;
	case "Actors\\DwarvenSteamCenturion\\Character Assets\\skeleton.nif"_hl:
		return Centurion;
	case "Actors\\Bear\\Character Assets\\skeleton.nif"_hl:
		return Bear;
	case "Actors\\DLC01\\ChaurusFlyer\\Character Assets\\skeleton.nif"_hl:
		return ChaurusFlyer;
	case "Actors\\DLC02\\Scrib\\Character Assets\\skeleton.nif"_hl:
		return Scrib;
	case "Actors\\VampireLord\\Character Assets\\Skeleton.nif"_hl:
	case "Actors\\DLC01\\VampireBrute\\Character Assets\\skeleton.nif"_hl:
		return Vampire;
	case "Actors\\DLC02\\HMDaedra\\Character Assets\\Skeleton.nif"_hl:
		return HMDaedra;
	case "Actors\\DLC02\\Riekling\\Character Assets\\skeleton.nif"_hl:
		return Riekling;
	case "Actors\\DLC02\\DwarvenBallistaCenturion\\Character Assets\\skeleton.nif"_hl:
	case "Actors\\Mudcrab\\Character Assets\\skeleton.nif"_hl:
	case "Actors\\IceWraith\\Character Assets\\skeleton.nif"_hl:
	case "Actors\\Ambient\\Chicken\\Character Assets\\Skeleton.nif"_hl:
	case "Actors\\Ambient\\Hare\\Character Assets\\skeleton.nif"_hl:
	case "Actors\\Witchlight\\Character Assets\\skeleton.nif"_hl:
	case "Actors\\Slaughterfish\\Character Assets\\skeleton.nif"_hl:
	case "Actors\\AtronachStorm\\Character Assets\\skeleton.nif"_hl:
	case "Actors\\DLC02\\Netch\\CharacterAssets\\skeleton.nif"_hl:
	case "Actors\\DLC02\\BoarRiekling\\Character Assets\\skeleton.nif"_hl:
	case "Actors\\DLC02\\BoarRiekling\\Character Assets\\SkeletonBoar.nif"_hl:
		return Other;
	default:
		return Unknown;
	}
}

Skeletons get_skeleton_type(RE::Actor* a)
{
	return get_skeleton_type(get_skeleton_path(a));
}

std::pair<uint32_t, float> get_LocationalArmor(RE::Actor* victim, Skeletons type, const NiPoint3& hit_pos, const NiPoint3& eye_pos)
{
	auto& bones = Bones[type];

	//logger::info("get_bones_dists start");

	auto dists = get_bones_dists(victim, type, eye_pos, hit_pos);

	//logger::info("get_bones_dists end");

	float min_dist = dists[0];
	int min_dist_ind = 0;
	for (int i = 1; i < dists.size(); i++) {
		if (min_dist > dists[i]) {
			min_dist = dists[i];
			min_dist_ind = i;
		}
	}

	//logger::info("here3");

	float armorPerks = victim->GetActorValue(RE::ActorValue::kArmorPerks);
	float lightArmorRating = ActorValueOwner__get_ArmorRating__1403BE980(victim->As<RE::ActorValueOwner>(), victim->GetActorValue(RE::ActorValue::kLightArmor));
	float heavyArmorRating = ActorValueOwner__get_ArmorRating__1403BE980(victim->As<RE::ActorValueOwner>(), victim->GetActorValue(RE::ActorValue::kHeavyArmor));

	const float TRESHOLD = 1.1f;
	float max_dist = min_dist * TRESHOLD;
	for (int i = 0; i < dists.size(); i++) {
		float cur = dists[i];
		if (cur > max_dist) {
			cur = -1.0f;
		}
		dists[i] = cur;
	}

	//logger::info("here4");

#ifdef DEBUG
	for (int i = 0; i < bones.size(); ++i) {
		if (dists[i] < 0.0f)
			draw_bone(victim, type, std::get<0>(bones[i]), std::get<1>(bones[i]));
		else
			draw_bone<BLU>(victim, type, std::get<0>(bones[i]), std::get<1>(bones[i]));
	}
#endif

	float total_armor = 0.0f;
	int count = 0;
	for (int i = 0; i < dists.size(); i++) {
		if (dists[i] < 0.0f)
			continue;

		total_armor += get_armor(victim, static_cast<uint32_t>(std::get<2>(bones[i])),
			armorPerks, lightArmorRating, heavyArmorRating, static_cast<uint32_t>(std::get<3>(bones[i])));
		++count;
	}
	float avg_armor = total_armor / count;
	return { min_dist_ind, avg_armor };
}

RE::BSTempEffectParticle* play_impact(RE::Actor* attacker, RE::Actor* victim, const char* nodename, const std::string& nifpath)
{
	auto cell = attacker->GetParentCell();
	if (!cell)
		return nullptr;

	auto victim_root = netimmerse_cast<RE::BSFadeNode*>(victim->Get3D());
	if (!victim_root)
		return nullptr;
	auto victim_bone = netimmerse_cast<RE::NiNode*>(victim_root->GetObjectByName(nodename));
	if (!victim_bone)
		return nullptr;

	auto attacker_root = netimmerse_cast<RE::BSFadeNode*>(attacker->Get3D());
	if (!attacker_root)
		return nullptr;

	return CELL__PlaceParticleEffect(cell, 0.0f, nifpath.c_str(), attacker_root->world.rotate, victim_bone->world.translate, 1.0f, 7U, attacker_root);
}

void play_impact_bounce(RE::Actor* attacker, RE::Actor* victim, const char* nodename)
{
	play_impact(attacker, victim, nodename, *Settings::DataImpactBounce);
}

void play_impact_injury(RE::Actor* attacker, RE::Actor* victim, const char* nodename)
{
	play_impact(attacker, victim, nodename, *Settings::DataImpactInjury);
}

void try_injury(Limbs limb, float damage_mult, RE::Actor* victim, RE::Actor* attacker, const char* node_name)
{
#ifdef DEBUG
	logger::info("try injury {}, prop = {}", limb, get_injury_prop(damage_mult));
#endif

	if (FenixUtils::random(get_injury_prop(damage_mult))) {

#ifdef DEBUG
		logger::info("DONE");
#endif

		play_impact_injury(attacker, victim, node_name);
		apply_injury(limb, victim, attacker);
	}
}

bool try_penetrate(float discount, float attack)
{
#ifdef DEBUG
	logger::info("try penetrate, prop = {}", get_penetration_prop(discount, attack));
#endif

	if (!FenixUtils::random(get_penetration_prop(discount, attack))) {

#ifdef DEBUG
		logger::info("LOX");
#endif
		return false;
	}

	return true;
}

bool test_skeleton(RE::Actor* a) {
	auto& path = get_skeleton_path(a->race, RE::SEX::kMale);
	auto type = get_skeleton_type(path);
	if (type == Unknown) {
#ifdef DEBUG
		logger::info("unknown creature {}, path = {}", a->GetActorBase()->formID, path);
#endif  // DEBUG
		return true;
	}
	if (type == Other) {
		return true;
	}

	if (!isHumanoid(a, type)) {
#ifdef DEBUG
		logger::info("{} is NOT valid", a->race->formID);
#endif  // DEBUG
		return false;
	} else {
#ifdef DEBUG
		logger::info("{} is valid", a->race->formID);
#endif  // DEBUG
		return true;
	}
}

void test_skeletons()
{
	auto& array = RE::ProcessLists::GetSingleton()->highActorHandles;
	logger::info("testing {} creatures", array.size());
	for (auto& i : array) {
		auto a = i.get().get();
		if (!a)
			continue;
		
		test_skeleton(a);
	}
}

char Character__get_weapon_vector_625DD0(RE::Actor* a, char left, RE::NiPoint3* P)
{
	using func_t = decltype(&Character__get_weapon_vector_625DD0);
	REL::Relocation<func_t> func{ REL::ID(37620) };
	return func(a, left, P);
}

float LocationalDamage_melee(float ArmorRating, float DamageResist, RE::Actor* victim, RE::Actor* attacker)
{
#ifdef DEBUG
	//test_skeletons();
#endif  // DEBUG

#ifdef DEBUG
	logger::info("{} -> {}", attacker->GetBaseObject()->formID, victim->GetBaseObject()->formID);
#endif  // DEBUG

	auto type = get_skeleton_type(victim);
	if (victim->IsChild() || victim->IsDead(false) || type == Other || type == Unknown || !test_skeleton(victim)) {
		return ArmorRating + DamageResist;
	}
	auto& bones = Bones[type];

	//logger::info("here1");

	NiPoint3 hit_pos, eye_pos;

	get_attacker_thing_melee(attacker, hit_pos, eye_pos);

	//logger::info("here2");

#ifdef DEBUG
	draw_line<RED>(eye_pos, hit_pos);
	draw_player_weapon(3000.0f);
#endif

	//logger::info("here3");

	float attack = get_damage(attacker);

	//logger::info("here4");

	auto locational_data = get_LocationalArmor(victim, type, hit_pos, eye_pos);
	float avg_armor = locational_data.second;
	auto min_dist_ind = locational_data.first;
	float discount = get_discount(attack, avg_armor);
	const char* node_name = NodeNames[type][std::get<1>(bones[min_dist_ind])];

#ifdef DEBUG
	logger::info("attack = {}, avg_armor = {}, discount = {}, ans = {}", attack, avg_armor, discount, DamageResist + discount);
#endif

	Limbs limb = std::get<3>(bones[min_dist_ind]);

	if (!try_penetrate(discount, attack)) {
		//if (true) {
		Sounds::play_sound_nopen_melee(attacker, victim);
		play_impact_bounce(attacker, victim, node_name);

#ifdef DEBUG
		//play_impact_injury(attacker, victim, node_name);
#endif

		return -1.0f;
	}

	try_injury(limb, discount, victim, attacker, node_name);

#ifdef DEBUG
	//apply_injury(Limbs::Arms, victim, attacker);
	//play_impact_injury(victim, NodeNames[type][std::get<1>(bones[min_dist_ind])]);
	//SKSE::GetTaskInterface()->AddTask([attacker]() { play_impact(attacker); });
#endif

	return DamageResist + discount;
}

void get_attacker_thing_proj(RE::Projectile* proj, NiPoint3& hit_pos, NiPoint3& eye_pos)
{
	float reach = 60.0f;
	eye_pos = proj->data.location;
	hit_pos = proj->data.location;
	hit_pos += rotate(reach, proj->data.angle);
	eye_pos += (hit_pos - eye_pos) * 0.2f;
}

float LocationalDamage_proj(float ArmorRating, float DamageResist, RE::Actor* victim, RE::Actor* attacker, RE::ArrowProjectile* proj)
{
#ifdef DEBUG
	//test_skeletons();
#endif  // DEBUG

	auto type = get_skeleton_type(victim);
	if (victim->IsChild() || victim->IsDead(false) || type == Other || type == Unknown || !test_skeleton(victim)) {
		return ArmorRating + DamageResist;
	}
	auto& bones = Bones[type];

	NiPoint3 hit_pos, eye_pos;

	get_attacker_thing_proj(proj, hit_pos, eye_pos);

#ifdef DEBUG
	draw_line<RED, 6000>(eye_pos, hit_pos);
#endif

	float attack = get_damage(attacker);

	auto locational_data = get_LocationalArmor(victim, type, hit_pos, eye_pos);
	float avg_armor = locational_data.second;
	auto min_dist_ind = locational_data.first;

	float discount = get_discount(attack, avg_armor);

#ifdef DEBUG
	logger::info("attack = {}, avg_armor = {}, discount = {}, ans = {}", attack, avg_armor, discount, DamageResist + discount);
#endif

	const char* node_name = NodeNames[type][std::get<1>(bones[min_dist_ind])];
	Limbs limb = std::get<3>(bones[min_dist_ind]);

	if (!try_penetrate(discount, attack)) {
	//if (true) {
		proj->impactResult = RE::ImpactResult::kBounce;
		Sounds::play_sound_nopen_arrow(attacker, victim);
		play_impact_bounce(attacker, victim, node_name);
		return -1.0f;
	}

	try_injury(limb, discount, victim, attacker, node_name);

#ifdef DEBUG
	//apply_injury(Limbs::Legs, victim, attacker);
#endif

	return DamageResist + discount;
}
