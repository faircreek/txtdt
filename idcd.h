
/*
 * Copyright (C) 2002-2015 Olof <jinxolina@gmail.com>
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 2 of the License, or (at
 * your option) any later version.
 *
 * This program is distributed in the hope that it will be useful, but
 * WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
 * General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software
 * Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA  02111-1307  USA
 */


typedef enum  {

  /**
   * Structure_IDCD - 0x00->0x0f
   */

  IDCD_Reserved							= 0x00,

  IDCD_Volume							= 0x01,
  IDCD_Title							= 0x02,
  IDCD_Parental_ID						= 0x03,
  IDCD_Part_Of_Title						= 0x04,
  IDCD_Cell							= 0x05,

  IDCD_Reserved_1						= 0x06,
  IDCD_Reserved_2						= 0x07,
  IDCD_Reserved_3						= 0x08,
  IDCD_Reserved_4						= 0x09,
  IDCD_Reserved_5						= 0x0a,
  IDCD_Reserved_6						= 0x0b,

  IDCD_Provider_Uniq_1						= 0x0c,
  IDCD_Provider_Uniq_2						= 0x0d,
  IDCD_Provider_Uniq_3						= 0x0e,
  IDCD_Provider_Uniq_4						= 0x0f,

  IDCD_Audio_Stream						= 0x10,
  IDCD_Subpicture_Stream					= 0x11,
  IDCD_Angle							= 0x12,

  IDCD_Reserved_7						= 0x13,
  IDCD_Reserved_8						= 0x14,
  IDCD_Reserved_9						= 0x15,
  IDCD_Reserved_A						= 0x16,
  IDCD_Reserved_B						= 0x17,
  IDCD_Reserved_C						= 0x18,
  IDCD_Reserved_D						= 0x19,
  IDCD_Reserved_E						= 0x1a,
  IDCD_Reserved_F						= 0x1b,

  IDCD_Provider_Uniq_5						= 0x1c,
  IDCD_Provider_Uniq_6						= 0x1d,
  IDCD_Provider_Uniq_7						= 0x1e,
  IDCD_Provider_Uniq_8						= 0x1f,

  IDCD_Audio_Channel						= 0x20,

  IDCD_Reserved_11						= 0x21,
  IDCD_Reserved_12						= 0x22,
  IDCD_Reserved_13						= 0x23,
  IDCD_Reserved_14						= 0x24,
  IDCD_Reserved_15						= 0x25,
  IDCD_Reserved_16						= 0x26,
  IDCD_Reserved_17						= 0x27,
  IDCD_Reserved_18						= 0x28,
  IDCD_Reserved_19						= 0x29,
  IDCD_Reserved_1A						= 0x2a,
  IDCD_Reserved_1B						= 0x2b,

  IDCD_Provider_Uniq_9						= 0x2c,
  IDCD_Provider_Uniq_A						= 0x2d,
  IDCD_Provider_Uniq_B						= 0x2e,
  IDCD_Provider_Uniq_C						= 0x2f,



  /**
   * Application_IDCD - 0x30->0xff
   */

  /* General IDCD - 0x30->0x37 */
  IDCD_General_Name						= 0x30,
  IDCD_General_Comments						= 0x31,
  IDCD_General_Unknown_1					= 0x32,
  IDCD_General_Unknown_2					= 0x33,
  IDCD_General_Unknown_3					= 0x34,
  IDCD_General_Unknown_4					= 0x35,
  IDCD_General_Unknown_5					= 0x36,
  IDCD_General_Unknown_6					= 0x37,

  /* Title Name - 0x38->0x3f */
  IDCD_Title_Series						= 0x38,
  IDCD_Title_Movie						= 0x39,
  IDCD_Title_Video						= 0x3a,
  IDCD_Title_Album						= 0x3b,
  IDCD_Title_Song						= 0x3c,
  IDCD_Title_Unknown_1						= 0x3d,
  IDCD_Title_Unknown_2						= 0x3e,
  IDCD_Title_Other						= 0x3f,

  /* Title Name (sub) - 0x40->0x47 NOTE: This is Chapters :)*/
  IDCD_Title_Sub_Series						= 0x40,
  IDCD_Title_Sub_Movie						= 0x41,
  IDCD_Title_Sub_Video						= 0x42,
  IDCD_Title_Sub_Album						= 0x43,
  IDCD_Title_Sub_Song						= 0x44,
  IDCD_Title_Sub_Unknown_1					= 0x45,
  IDCD_Title_Sub_Unknown_2					= 0x46,
  IDCD_Title_Sub_Other						= 0x47,

  /* Title Name (original) - 0x48->0x4f */
  IDCD_Title_Orig_Series					= 0x48,
  IDCD_Title_Orig_Movie						= 0x49,
  IDCD_Title_Orig_Video						= 0x4a,
  IDCD_Title_Orig_Album						= 0x4b,
  IDCD_Title_Orig_Song						= 0x4c,
  IDCD_Title_Orig_Unknown_1					= 0x4d,
  IDCD_Title_Orig_Unknown_2					= 0x4e,
  IDCD_Title_Orig_Other						= 0x4f,

  /* Other Name - 0x50->0x57 */
  IDCD_Other_Scene						= 0x50,
  IDCD_Other_Cut						= 0x51,
  IDCD_Other_Take						= 0x52,
  IDCD_Other_Label						= 0x53,
  IDCD_Other_Unknown_1						= 0x54,
  IDCD_Other_Unknown_2						= 0x55,
  IDCD_Other_Unknown_3						= 0x56,
  IDCD_Other_Unknown_4						= 0x57,

  /* Language - 0x58->0x5b */
  IDCD_Language_Language					= 0x58,
  IDCD_Language_Original_Language				= 0x59,
  IDCD_Language_Unknown_1					= 0x5a,
  IDCD_Language_Unknown_2					= 0x5b,

  /* Work - 0x5c->0x6b */
  IDCD_Work_Picture_Size					= 0x5c,
  IDCD_Work_Picture_Color					= 0x5d,
  IDCD_Work_Picture_Feature					= 0x5e,
  IDCD_Work_Audio_Feature					= 0x5f,
  IDCD_Work_Other_Feature					= 0x60,
  IDCD_Work_Shooting_Location					= 0x61,
  IDCD_Work_Country_Of_Production				= 0x62,
  IDCD_Work_Production_Company					= 0x63,
  IDCD_Work_Production_Studio					= 0x64,
  IDCD_Work_Awarded_Prize					= 0x65,
  IDCD_Work_Original_Writing					= 0x66,
  IDCD_Work_Suggested_Age					= 0x67,
  IDCD_Work_Unknown_1						= 0x68,
  IDCD_Work_Unknown_2						= 0x69,
  IDCD_Work_Unknown_3						= 0x6a,
  IDCD_Work_Unknown_4						= 0x6b,

  /* Character - 0x6c->0x8f */
  IDCD_Character_Leading_Actor					= 0x6c,
  IDCD_Character_Leading_Actress				= 0x6d,
  IDCD_Character_Supporting_Actor				= 0x6e,
  IDCD_Character_Supporting_Actress				= 0x6f,
  IDCD_Character_Other_Actor_Or_Actress				= 0x70,
  IDCD_Character_Producer					= 0x71,
  IDCD_Character_Director					= 0x72,
  IDCD_Character_Scenario_Writer				= 0x73,
  IDCD_Character_Original_Writer				= 0x74,
  IDCD_Character_Camera_Man					= 0x75,
  IDCD_Character_Special_Effect_Director			= 0x76,
  IDCD_Character_Voice_Actor_or_Actress_for_Leading_Actor	= 0x77,
  IDCD_Character_Voice_Actor_or_Actress_for_Leading_Actress	= 0x78,
  IDCD_Character_Voice_Actor_or_Actress_for_Supporting_Actor	= 0x79,
  IDCD_Character_Voice_Actor_or_Actress_for_Supporting_Actress	= 0x7a,
  IDCD_Character_Unknown_1					= 0x7b,
  IDCD_Character_Leading_Musician				= 0x7c,
  IDCD_Character_Musician					= 0x7d,
  IDCD_Character_Vocalist					= 0x7e,
  IDCD_Character_Conductor					= 0x7f,
  IDCD_Character_Orchestra					= 0x80,
  IDCD_Character_Song_Writer					= 0x81,
  IDCD_Character_Music_Composer					= 0x82,
  IDCD_Character_Music_Arranger					= 0x83,
  IDCD_Character_Music_Producer					= 0x84,
  IDCD_Character_Master_Of_Ceremonies				= 0x85,
  IDCD_Character_Vocal_Trainer					= 0x86,
  IDCD_Character_Unknown_2					= 0x87,
  IDCD_Character_Picture_Creator				= 0x88,
  IDCD_Character_Animator					= 0x89,
  IDCD_Character_Character_Designer				= 0x8a,
  IDCD_Character_Mechanism_Designer				= 0x8b,
  IDCD_Character_Leading_Artist					= 0x8c,
  IDCD_Character_Artist						= 0x8d,
  IDCD_Character_Unknown_3					= 0x8e,
  IDCD_Character_Unknown_4					= 0x8f,


  /* Data - 0x90->0x93 */
  IDCD_Data_Production						= 0x90,
  IDCD_Data_Award						= 0x91,
  IDCD_Data_Historical_Background				= 0x92,
  IDCD_Data_Unknown						= 0x93,


  /* Karaoke specification - 0x94-0x9b */
  IDCD_Karaoke_Male_Melody_Original_Vocalist			= 0x94,
  IDCD_Karaoke_Male_Harmony_Original_Vocalist			= 0x95,
  IDCD_Karaoke_Other_Male_Original_Vocalist			= 0x96,
  IDCD_Karaoke_Female_Melody_Original_Vocalist			= 0x97,
  IDCD_Karaoke_Female_Harmony_Original_Vocalist			= 0x98,
  IDCD_Karaoke_Other_Female_Original_Vocalist			= 0x99,
  IDCD_Karaoke_Unknown_1					= 0x9a,
  IDCD_Karaoke_Unknown_2					= 0x9b,

  /* Category - 0x9c->0x9f */
  IDCD_Category_General_Category				= 0x9c,
  IDCD_Category_Video_Category					= 0x9d,
  IDCD_Category_Music_Or_Karaoke_Category			= 0x9e,
  IDCD_Category_Sub_Category					= 0x9f,

  /* Lyrics - 0xa0->0xa3 */
  IDCD_Lyrics_Lyrics						= 0xa0,
  IDCD_Lyrics_Begining_Of_Lyrics				= 0xa1,
  IDCD_Lyrics_Famous_Lyrics					= 0xa2,
  IDCD_Lyrics_Unknown						= 0xa3,

  /* Document - 0xa4->0xa7 */
  IDCD_Document_Liner_Notes					= 0xa4,
  IDCD_Document_Unknown_1					= 0xa5,
  IDCD_Document_Unknown_2					= 0xa6,
  IDCD_Document_Unknown_3					= 0xa7,

  /* Others - 0xa8->0xab */
  IDCD_Others_Others						= 0xa8,
  IDCD_Others_Unknown_1						= 0xa9,
  IDCD_Others_Unknown_2						= 0xaa,
  IDCD_Others_Unknown_3						= 0xab,

  /* Reserved - 0xac->0xaf */
  IDCD_Application_Reserved_1					= 0xac,
  IDCD_Application_Reserved_2					= 0xad,
  IDCD_Application_Reserved_3					= 0xae,
  IDCD_Application_Reserved_4					= 0xaf,

  /* Administration - 0xb0->0xb7 */
  IDCD_Admin_Product_Code					= 0xb0,
  IDCD_Admin_POS_Code						= 0xb1,
  IDCD_Admin_ISRC						= 0xb2,
  IDCD_Admin_Copyright						= 0xb3,
  IDCD_Admin_Supplier						= 0xb4,
  IDCD_Admin_Others						= 0xb5,
  IDCD_Admin_Unknown_1						= 0xb6,
  IDCD_Admin_Unknown_2						= 0xb7,

  /* Additional administration - 0xb8->0xcf */
  IDCD_Add_Admin_Release_Date					= 0xb8,
  IDCD_Add_Admin_Rental_Ban_Information				= 0xb9,
  IDCD_Add_Admin_Resale_Ban_Information				= 0xba,
  IDCD_Add_Admin_Printing_Company				= 0xbb,
  IDCD_Add_Admin_Original_Product_Number			= 0xbc,
  IDCD_Add_Admin_Original_Release_Data				= 0xbd,
  IDCD_Add_Admin_Original_Supplier				= 0xbe,
  IDCD_Add_Admin_Original_Copyright				= 0xbf,
  IDCD_Add_Admin_Unknown_1					= 0xc0,
  IDCD_Add_Admin_Unknown_2					= 0xc1,
  IDCD_Add_Admin_Unknown_3					= 0xc2,
  IDCD_Add_Admin_Unknown_4					= 0xc3,
  IDCD_Add_Admin_Recording_Information				= 0xc4,
  IDCD_Add_Admin_Pre_Mastering_Information			= 0xc5,
  IDCD_Add_Admin_Encoding_Information				= 0xc6,
  IDCD_Add_Admin_Authoring_Information				= 0xc7,
  IDCD_Add_Admin_Master_Tape_infromation			= 0xc8,
  IDCD_Add_Admin_Other_Information				= 0xc9,
  IDCD_Add_Admin_Unknown_5					= 0xca,
  IDCD_Add_Admin_Unknown_6					= 0xcb,
  IDCD_Add_Admin_Unknown_7					= 0xcc,
  IDCD_Add_Admin_Unknown_8					= 0xcd,
  IDCD_Add_Admin_Unknown_9					= 0xce,
  IDCD_Add_Admin_Unknown_A					= 0xcf,


  /* Reserved  - 0xd0->0xdf */
  IDCD_Application_Reserved_5					= 0xd0,
  IDCD_Application_Reserved_6					= 0xd1,
  IDCD_Application_Reserved_7					= 0xd2,
  IDCD_Application_Reserved_8					= 0xd3,
  IDCD_Application_Reserved_9					= 0xd4,
  IDCD_Application_Reserved_A					= 0xd5,
  IDCD_Application_Reserved_B					= 0xd6,
  IDCD_Application_Reserved_C					= 0xd7,
  IDCD_Application_Reserved_D					= 0xd8,
  IDCD_Application_Reserved_E					= 0xd9,
  IDCD_Application_Reserved_F					= 0xda,
  IDCD_Application_Reserved_11					= 0xdb,
  IDCD_Application_Reserved_12					= 0xdc,
  IDCD_Application_Reserved_13					= 0xdd,
  IDCD_Application_Reserved_14					= 0xde,
  IDCD_Application_Reserved_15					= 0xdf,

  /* Vendor/Provider Uniq - 0xe0->0xef */
  IDCD_Application_Vendor_0					= 0xe0,
  IDCD_Application_Vendor_1					= 0xe1,
  IDCD_Application_Vendor_2					= 0xe2,
  IDCD_Application_Vendor_3					= 0xe3,
  IDCD_Application_Vendor_4					= 0xe4,
  IDCD_Application_Vendor_5					= 0xe5,
  IDCD_Application_Vendor_6					= 0xe6,
  IDCD_Application_Vendor_7					= 0xe7,
  IDCD_Application_Vendor_8					= 0xe8,
  IDCD_Application_Vendor_9					= 0xe9,
  IDCD_Application_Vendor_A					= 0xea,
  IDCD_Application_Vendor_B					= 0xeb,
  IDCD_Application_Vendor_C					= 0xec,
  IDCD_Application_Vendor_D					= 0xed,
  IDCD_Application_Vendor_E					= 0xee,
  IDCD_Application_Vendor_F					= 0xef,


  /* Extension - 0xf0->0xf7 */
  IDCD_Extension_Sorting					= 0xf0,
  IDCD_Extension_Unknown_1					= 0xf1,
  IDCD_Extension_Unknown_2					= 0xf2,
  IDCD_Extension_Unknown_3					= 0xf3,
  IDCD_Extension_Unknown_4					= 0xf4,
  IDCD_Extension_Unknown_5					= 0xf5,
  IDCD_Extension_Unknown_6					= 0xf6,
  IDCD_Extension_Unknown_7					= 0xf7,

  /* Reserved - 0xf8->0xff */
  IDCD_Application_Reserved_16					= 0xf8,
  IDCD_Application_Reserved_17					= 0xf9,
  IDCD_Application_Reserved_18					= 0xfa,
  IDCD_Application_Reserved_19					= 0xfb,
  IDCD_Application_Reserved_1A					= 0xfc,
  IDCD_Application_Reserved_1B					= 0xfd,
  IDCD_Application_Reserved_1C					= 0xfe,
  IDCD_Application_Reserved_1D					= 0xff,


} idcd_type_t;



