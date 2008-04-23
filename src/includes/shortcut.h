/*
  Hatari - shortcut.h

  This file is distributed under the GNU Public License, version 2 or at
  your option any later version. Read the file gpl.txt for details.
*/

/* If pressed short-cut key modifier or standalone short-cut key,
 * retain keypress until safe to execute (start of VBL). Returns zero
 * if key wasn't a shortcut */
extern int ShortCut_CheckKeys(int modkey, int symkey, BOOL press);
/* Invoke shortcut identified by name */
extern void Shortcut_Invoke(const char *shortcut);
/* Act on the stored keypress (in VBL) */
extern void ShortCut_ActKey(void);
