/***************************************************************************\
* Module Name: Layout01.C
*
* keyboard layout
*
* Copyright (c) 1985-2001, Microsoft Corporation
*
* History:
* KBDTOOL v3.40 - Created  Sat Apr 19 00:35:17 2025
\***************************************************************************/

#include <windows.h>
#include "kbd.h"
#include "Layout01.h"

#if defined(_M_IA64)
#pragma section(".data")
#define ALLOC_SECTION_LDATA __declspec(allocate(".data"))
#else
#pragma data_seg(".data")
#define ALLOC_SECTION_LDATA
#endif

/***************************************************************************\
* ausVK[] - Virtual Scan Code to Virtual Key conversion table
\***************************************************************************/

static ALLOC_SECTION_LDATA USHORT ausVK[] = {
    T00, T01, T02, T03, T04, T05, T06, T07,
    T08, T09, T0A, T0B, T0C, T0D, T0E, T0F,
    T10, T11, T12, T13, T14, T15, T16, T17,
    T18, T19, T1A, T1B, T1C, T1D, T1E, T1F,
    T20, T21, T22, T23, T24, T25, T26, T27,
    T28, T29, T2A, T2B, T2C, T2D, T2E, T2F,
    T30, T31, T32, T33, T34, T35,

    /*
     * Right-hand Shift key must have KBDEXT bit set.
     */
    T36 | KBDEXT,

    T37 | KBDMULTIVK,               // numpad_* + Shift/Alt -> SnapShot

    T38, T39, T3A, T3B, T3C, T3D, T3E,
    T3F, T40, T41, T42, T43, T44,

    /*
     * NumLock Key:
     *     KBDEXT     - VK_NUMLOCK is an Extended key
     *     KBDMULTIVK - VK_NUMLOCK or VK_PAUSE (without or with CTRL)
     */
    T45 | KBDEXT | KBDMULTIVK,

    T46 | KBDMULTIVK,

    /*
     * Number Pad keys:
     *     KBDNUMPAD  - digits 0-9 and decimal point.
     *     KBDSPECIAL - require special processing by Windows
     */
    T47 | KBDNUMPAD | KBDSPECIAL,   // Numpad 7 (Home)
    T48 | KBDNUMPAD | KBDSPECIAL,   // Numpad 8 (Up),
    T49 | KBDNUMPAD | KBDSPECIAL,   // Numpad 9 (PgUp),
    T4A,
    T4B | KBDNUMPAD | KBDSPECIAL,   // Numpad 4 (Left),
    T4C | KBDNUMPAD | KBDSPECIAL,   // Numpad 5 (Clear),
    T4D | KBDNUMPAD | KBDSPECIAL,   // Numpad 6 (Right),
    T4E,
    T4F | KBDNUMPAD | KBDSPECIAL,   // Numpad 1 (End),
    T50 | KBDNUMPAD | KBDSPECIAL,   // Numpad 2 (Down),
    T51 | KBDNUMPAD | KBDSPECIAL,   // Numpad 3 (PgDn),
    T52 | KBDNUMPAD | KBDSPECIAL,   // Numpad 0 (Ins),
    T53 | KBDNUMPAD | KBDSPECIAL,   // Numpad . (Del),

    T54, T55, T56, T57, T58, T59, T5A, T5B,
    T5C, T5D, T5E, T5F, T60, T61, T62, T63,
    T64, T65, T66, T67, T68, T69, T6A, T6B,
    T6C, T6D, T6E, T6F, T70, T71, T72, T73,
    T74, T75, T76, T77, T78, T79, T7A, T7B,
    T7C, T7D, T7E

};

static ALLOC_SECTION_LDATA VSC_VK aE0VscToVk[] = {
        { 0x10, X10 | KBDEXT              },  // Speedracer: Previous Track
        { 0x19, X19 | KBDEXT              },  // Speedracer: Next Track
        { 0x1D, X1D | KBDEXT              },  // RControl
        { 0x20, X20 | KBDEXT              },  // Speedracer: Volume Mute
        { 0x21, X21 | KBDEXT              },  // Speedracer: Launch App 2
        { 0x22, X22 | KBDEXT              },  // Speedracer: Media Play/Pause
        { 0x24, X24 | KBDEXT              },  // Speedracer: Media Stop
        { 0x2E, X2E | KBDEXT              },  // Speedracer: Volume Down
        { 0x30, X30 | KBDEXT              },  // Speedracer: Volume Up
        { 0x32, X32 | KBDEXT              },  // Speedracer: Browser Home
        { 0x35, X35 | KBDEXT              },  // Numpad Divide
        { 0x37, X37 | KBDEXT              },  // Snapshot
        { 0x38, X38 | KBDEXT              },  // RMenu
        { 0x47, X47 | KBDEXT              },  // Home
        { 0x48, X48 | KBDEXT              },  // Up
        { 0x49, X49 | KBDEXT              },  // Prior
        { 0x4B, X4B | KBDEXT              },  // Left
        { 0x4D, X4D | KBDEXT              },  // Right
        { 0x4F, X4F | KBDEXT              },  // End
        { 0x50, X50 | KBDEXT              },  // Down
        { 0x51, X51 | KBDEXT              },  // Next
        { 0x52, X52 | KBDEXT              },  // Insert
        { 0x53, X53 | KBDEXT              },  // Delete
        { 0x5B, X5B | KBDEXT              },  // Left Win
        { 0x5C, X5C | KBDEXT              },  // Right Win
        { 0x5D, X5D | KBDEXT              },  // Application
        { 0x5F, X5F | KBDEXT              },  // Speedracer: Sleep
        { 0x65, X65 | KBDEXT              },  // Speedracer: Browser Search
        { 0x66, X66 | KBDEXT              },  // Speedracer: Browser Favorites
        { 0x67, X67 | KBDEXT              },  // Speedracer: Browser Refresh
        { 0x68, X68 | KBDEXT              },  // Speedracer: Browser Stop
        { 0x69, X69 | KBDEXT              },  // Speedracer: Browser Forward
        { 0x6A, X6A | KBDEXT              },  // Speedracer: Browser Back
        { 0x6B, X6B | KBDEXT              },  // Speedracer: Launch App 1
        { 0x6C, X6C | KBDEXT              },  // Speedracer: Launch Mail
        { 0x6D, X6D | KBDEXT              },  // Speedracer: Launch Media Selector
        { 0x1C, X1C | KBDEXT              },  // Numpad Enter
        { 0x46, X46 | KBDEXT              },  // Break (Ctrl + Pause)
        { 0,      0                       }
};

static ALLOC_SECTION_LDATA VSC_VK aE1VscToVk[] = {
        { 0x1D, Y1D                       },  // Pause
        { 0   ,   0                       }
};

/***************************************************************************\
* aVkToBits[]  - map Virtual Keys to Modifier Bits
*
* See kbd.h for a full description.
*
* The keyboard has only three shifter keys:
*     SHIFT (L & R) affects alphabnumeric keys,
*     CTRL  (L & R) is used to generate control characters
*     ALT   (L & R) used for generating characters by number with numpad
\***************************************************************************/
static ALLOC_SECTION_LDATA VK_TO_BIT aVkToBits[] = {
    { VK_SHIFT    ,   KBDSHIFT     },
    { VK_CONTROL  ,   KBDCTRL      },
    { VK_MENU     ,   KBDALT       },
    { 0           ,   0           }
};

/***************************************************************************\
* aModification[]  - map character modifier bits to modification number
*
* See kbd.h for a full description.
*
\***************************************************************************/

static ALLOC_SECTION_LDATA MODIFIERS CharModifiers = {
    &aVkToBits[0],
    2,
    {
    //  Modification# //  Keys Pressed
    //  ============= // =============
        0,            // 
        1,            // Shift 
        2             // Control 
     }
};

/***************************************************************************\
*
* aVkToWch2[]  - Virtual Key to WCHAR translation for 2 shift states
* aVkToWch3[]  - Virtual Key to WCHAR translation for 3 shift states
* aVkToWch4[]  - Virtual Key to WCHAR translation for 4 shift states
*
* Table attributes: Unordered Scan, null-terminated
*
* Search this table for an entry with a matching Virtual Key to find the
* corresponding unshifted and shifted WCHAR characters.
*
* Special values for VirtualKey (column 1)
*     0xff          - dead chars for the previous entry
*     0             - terminate the list
*
* Special values for Attributes (column 2)
*     CAPLOK bit    - CAPS-LOCK affect this key like SHIFT
*
* Special values for wch[*] (column 3 & 4)
*     WCH_NONE      - No character
*     WCH_DEAD      - Dead Key (diaresis) or invalid (US keyboard has none)
*     WCH_LGTR      - Ligature (generates multiple characters)
*
\***************************************************************************/

static ALLOC_SECTION_LDATA VK_TO_WCHARS2 aVkToWch2[] = {
//                      |         |  Shift  |
//                      |=========|=========|
  {VK_TAB       ,0      ,'\t'     ,'\t'     },
  {VK_ADD       ,0      ,'+'      ,'+'      },
  {VK_DIVIDE    ,0      ,'/'      ,'/'      },
  {VK_MULTIPLY  ,0      ,'*'      ,'*'      },
  {VK_SUBTRACT  ,0      ,'-'      ,'-'      },
  {0            ,0      ,0        ,0        }
};

static ALLOC_SECTION_LDATA VK_TO_WCHARS3 aVkToWch3[] = {
    //                      |         |  Shift     |  Ctrl      |
    //                      |=========|============|============|
    {'1'          ,PH_F_00 ,PH_N_00 ,PH_S_00 ,PH_C_00 },
    {'2'          ,PH_F_01 ,PH_N_01 ,PH_S_01 ,PH_C_01 },
    {'3'          ,PH_F_02 ,PH_N_02 ,PH_S_02 ,PH_C_02 },
    {'4'          ,PH_F_03 ,PH_N_03 ,PH_S_03 ,PH_C_03 },
    {'5'          ,PH_F_04 ,PH_N_04 ,PH_S_04 ,PH_C_04 },
    {'6'          ,PH_F_05 ,PH_N_05 ,PH_S_05 ,PH_C_05 },
    {'7'          ,PH_F_06 ,PH_N_06 ,PH_S_06 ,PH_C_06 },
    {'8'          ,PH_F_07 ,PH_N_07 ,PH_S_07 ,PH_C_07 },
    {'9'          ,PH_F_08 ,PH_N_08 ,PH_S_08 ,PH_C_08 },
    {'0'          ,PH_F_09 ,PH_N_09 ,PH_S_09 ,PH_C_09 },
    {VK_OEM_MINUS ,PH_F_10 ,PH_N_10 ,PH_S_10 ,PH_C_10 },
    {VK_OEM_PLUS  ,PH_F_11 ,PH_N_11 ,PH_S_11 ,PH_C_11 },
    {'Q'          ,PH_F_12 ,PH_N_12 ,PH_S_12 ,PH_C_12 },
    {'W'          ,PH_F_13 ,PH_N_13 ,PH_S_13 ,PH_C_13 },
    {'E'          ,PH_F_14 ,PH_N_14 ,PH_S_14 ,PH_C_14 },
    {'R'          ,PH_F_15 ,PH_N_15 ,PH_S_15 ,PH_C_15 },
    {'T'          ,PH_F_16 ,PH_N_16 ,PH_S_16 ,PH_C_16 },
    {'Y'          ,PH_F_17 ,PH_N_17 ,PH_S_17 ,PH_C_17 },
    {'U'          ,PH_F_18 ,PH_N_18 ,PH_S_18 ,PH_C_18 },
    {'I'          ,PH_F_19 ,PH_N_19 ,PH_S_19 ,PH_C_19 },
    {'O'          ,PH_F_20 ,PH_N_20 ,PH_S_20 ,PH_C_20 },
    {'P'          ,PH_F_21 ,PH_N_21 ,PH_S_21 ,PH_C_21 },
    {VK_OEM_4     ,PH_F_22 ,PH_N_22 ,PH_S_22 ,PH_C_22 },
    {VK_OEM_6     ,PH_F_23 ,PH_N_23 ,PH_S_23 ,PH_C_23 },
    {'A'          ,PH_F_24 ,PH_N_24 ,PH_S_24 ,PH_C_24 },
    {'S'          ,PH_F_25 ,PH_N_25 ,PH_S_25 ,PH_C_25 },
    {'D'          ,PH_F_26 ,PH_N_26 ,PH_S_26 ,PH_C_26 },
    {'F'          ,PH_F_27 ,PH_N_27 ,PH_S_27 ,PH_C_27 },
    {'G'          ,PH_F_28 ,PH_N_28 ,PH_S_28 ,PH_C_28 },
    {'H'          ,PH_F_29 ,PH_N_29 ,PH_S_29 ,PH_C_29 },
    {'J'          ,PH_F_30 ,PH_N_30 ,PH_S_30 ,PH_C_30 },
    {'K'          ,PH_F_31 ,PH_N_31 ,PH_S_31 ,PH_C_31 },
    {'L'          ,PH_F_32 ,PH_N_32 ,PH_S_32 ,PH_C_32 },
    {VK_OEM_1     ,PH_F_33 ,PH_N_33 ,PH_S_33 ,PH_C_33 },
    {VK_OEM_7     ,PH_F_34 ,PH_N_34 ,PH_S_34 ,PH_C_34 },
    {VK_OEM_3     ,PH_F_35 ,PH_N_35 ,PH_S_35 ,PH_C_35 },
    {VK_OEM_5     ,PH_F_36 ,PH_N_36 ,PH_S_36 ,PH_C_36 },
    {'Z'          ,PH_F_37 ,PH_N_37 ,PH_S_37 ,PH_C_37 },
    {'X'          ,PH_F_38 ,PH_N_38 ,PH_S_38 ,PH_C_38 },
    {'C'          ,PH_F_39 ,PH_N_39 ,PH_S_39 ,PH_C_39 },
    {'V'          ,PH_F_40 ,PH_N_40 ,PH_S_40 ,PH_C_40 },
    {'B'          ,PH_F_41 ,PH_N_41 ,PH_S_41 ,PH_C_41 },
    {'N'          ,PH_F_42 ,PH_N_42 ,PH_S_42 ,PH_C_42 },
    {'M'          ,PH_F_43 ,PH_N_43 ,PH_S_43 ,PH_C_43 },
    {VK_OEM_COMMA ,PH_F_44 ,PH_N_44 ,PH_S_44 ,PH_C_44 },
    {VK_OEM_PERIOD,PH_F_45 ,PH_N_45 ,PH_S_45 ,PH_C_45 },
    {VK_OEM_2     ,PH_F_46 ,PH_N_46 ,PH_S_46 ,PH_C_46 },
    {VK_SPACE     ,PH_F_47 ,PH_N_47 ,PH_S_47 ,PH_C_47 },
    {VK_OEM_102   ,PH_F_48 ,PH_N_48 ,PH_S_48 ,PH_C_48 },
    {VK_DECIMAL   ,PH_F_49 ,PH_N_49 ,PH_S_49 ,PH_C_49 },
    {VK_BACK      ,PH_F_50 ,PH_N_50 ,PH_S_50 ,PH_C_50 },
    {VK_ESCAPE    ,PH_F_51 ,PH_N_51 ,PH_S_51 ,PH_C_51 },
    {VK_RETURN    ,PH_F_52 ,PH_N_52 ,PH_S_52 ,PH_C_52 },
    {VK_CANCEL    ,PH_F_53 ,PH_N_53 ,PH_S_53 ,PH_C_53 },
    {0            ,0        ,0        ,0        ,0      }
  };
  

// Put this last so that VkKeyScan interprets number characters
// as coming from the main section of the kbd (aVkToWch2 and
// aVkToWch5) before considering the numpad (aVkToWch1).

static ALLOC_SECTION_LDATA VK_TO_WCHARS1 aVkToWch1[] = {
    { VK_NUMPAD0   , 0      ,  '0'   },
    { VK_NUMPAD1   , 0      ,  '1'   },
    { VK_NUMPAD2   , 0      ,  '2'   },
    { VK_NUMPAD3   , 0      ,  '3'   },
    { VK_NUMPAD4   , 0      ,  '4'   },
    { VK_NUMPAD5   , 0      ,  '5'   },
    { VK_NUMPAD6   , 0      ,  '6'   },
    { VK_NUMPAD7   , 0      ,  '7'   },
    { VK_NUMPAD8   , 0      ,  '8'   },
    { VK_NUMPAD9   , 0      ,  '9'   },
    { 0            , 0      ,  '\0'  }
};

static ALLOC_SECTION_LDATA VK_TO_WCHAR_TABLE aVkToWcharTable[] = {
    {  (PVK_TO_WCHARS1)aVkToWch3, 3, sizeof(aVkToWch3[0]) },
    {  (PVK_TO_WCHARS1)aVkToWch2, 2, sizeof(aVkToWch2[0]) },
    {  (PVK_TO_WCHARS1)aVkToWch1, 1, sizeof(aVkToWch1[0]) },
    {                       NULL, 0, 0                    },
};

/***************************************************************************\
* aKeyNames[], aKeyNamesExt[]  - Virtual Scancode to Key Name tables
*
* Table attributes: Ordered Scan (by scancode), null-terminated
*
* Only the names of Extended, NumPad, Dead and Non-Printable keys are here.
* (Keys producing printable characters are named by that character)
\***************************************************************************/

static ALLOC_SECTION_LDATA VSC_LPWSTR aKeyNames[] = {
    0x01,    L"Esc",
    0x0e,    L"Backspace",
    0x0f,    L"Tab",
    0x1c,    L"Enter",
    0x1d,    L"Ctrl",
    0x2a,    L"Shift",
    0x36,    L"Right Shift",
    0x37,    L"Num *",
    0x38,    L"Alt",
    0x39,    L"Space",
    0x3a,    L"Caps Lock",
    0x3b,    L"F1",
    0x3c,    L"F2",
    0x3d,    L"F3",
    0x3e,    L"F4",
    0x3f,    L"F5",
    0x40,    L"F6",
    0x41,    L"F7",
    0x42,    L"F8",
    0x43,    L"F9",
    0x44,    L"F10",
    0x45,    L"Pause",
    0x46,    L"Scroll Lock",
    0x47,    L"Num 7",
    0x48,    L"Num 8",
    0x49,    L"Num 9",
    0x4a,    L"Num -",
    0x4b,    L"Num 4",
    0x4c,    L"Num 5",
    0x4d,    L"Num 6",
    0x4e,    L"Num +",
    0x4f,    L"Num 1",
    0x50,    L"Num 2",
    0x51,    L"Num 3",
    0x52,    L"Num 0",
    0x53,    L"Num Del",
    0x54,    L"Sys Req",
    0x57,    L"F11",
    0x58,    L"F12",
    0x7c,    L"F13",
    0x7d,    L"F14",
    0x7e,    L"F15",
    0x7f,    L"F16",
    0x80,    L"F17",
    0x81,    L"F18",
    0x82,    L"F19",
    0x83,    L"F20",
    0x84,    L"F21",
    0x85,    L"F22",
    0x86,    L"F23",
    0x87,    L"F24",
    0   ,    NULL
};

static ALLOC_SECTION_LDATA VSC_LPWSTR aKeyNamesExt[] = {
    0x1c,    L"Num Enter",
    0x1d,    L"Right Ctrl",
    0x35,    L"Num /",
    0x37,    L"Prnt Scrn",
    0x38,    L"Right Alt",
    0x45,    L"Num Lock",
    0x46,    L"Break",
    0x47,    L"Home",
    0x48,    L"Up",
    0x49,    L"Page Up",
    0x4b,    L"Left",
    0x4d,    L"Right",
    0x4f,    L"End",
    0x50,    L"Down",
    0x51,    L"Page Down",
    0x52,    L"Insert",
    0x53,    L"Delete",
    0x54,    L"<00>",
    0x56,    L"Help",
    0x5b,    L"Left Windows",
    0x5c,    L"Right Windows",
    0x5d,    L"Application",
    0   ,    NULL
};

static ALLOC_SECTION_LDATA KBDTABLES KbdTables = {
    /*
     * Modifier keys
     */
    &CharModifiers,

    /*
     * Characters tables
     */
    aVkToWcharTable,

    /*
     * Diacritics
     */
    NULL,

    /*
     * Names of Keys
     */
    aKeyNames,
    aKeyNamesExt,
    NULL,

    /*
     * Scan codes to Virtual Keys
     */
    ausVK,
    sizeof(ausVK) / sizeof(ausVK[0]),
    aE0VscToVk,
    aE1VscToVk,

    /*
     * Locale-specific special processing
     */
    MAKELONG(0, KBD_VERSION),

    /*
     * Ligatures
     */
    0,
    0,
    NULL
};

PKBDTABLES KbdLayerDescriptor(VOID)
{
    return &KbdTables;
}
