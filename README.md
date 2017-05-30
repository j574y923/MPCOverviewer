# MPCOverviewer
Read LICENSE.txt! I am not responsible for any lost files or damages to your system! I highly recommend backing up your files before using this program! Other than that, enjoy =]

# SETUP
IMPORTANT!!! First of all, find your prefs folder for Mario Paint Composer. This will look something like "C:\Users\J\Documents\MPC\Mario Paint Composer PC\Prefs". 

Then go back to the MPCOverviewer directory and find config.txt. Open that up and change the Directory line: This should be changed to look like "Directory:C:\Users\J\Documents\MPC\Mario Paint Composer PC\Prefs". 

After all that has been done, you can open MPCOverviewer and the program should open the MarioPaintSongList.txt found in MPC's prefs folder (MarioPaintSongList should exist). Now you can load song and open any of the files on the song list. 


# CONTROLS
SHIFT+R: 
Region selector. The region selector button should light up when shift+r is pressed. Left click, drag your mouse, then release left click on the composition to define a region of notes you want to select. NOTE: The rectangular region does not show as you drag. NOTE: this region encapsulates all notes visually confined within its bounds; you can draw this region to encapsulate several measures at a time without going the width of the line of measures. From there you can copy, cut, delete. You can also drag this region to the desired measure; the rectangular region will then disappear.

Volume selector. Begin the region selection by left clicking the volume margins and all selected volumes are then selected INSTEAD of notes.

CTRL+A:
Select all. Select all measures in the song creating a rectangular region that covers the entire song.

CTRL+T:
New tab. You can select tab that gets created on the tab gui in the ribbon menu at the top. Open and edit song same way you do with the original tab. NOTE: region selection rectangle remains when you go to another tab; this means you can copy the same region of another song.

CTRL+W:
Close tab. Close currently selected tab.

CTLR+O:
Open song. Open song from MarioPaintSongList. A window appears with the list of songs. Click on one of these songs to open them. A green rectangle appears to select that song. Then click "Load Song" button. The window does not close until you close it.

CTRL+S:
Save as. Save as a new file name. Some crappy ui window pops up that will take keyboard input for that new file name. File will get saved as .txt extension and into prefs folder. It will not be added to MarioPaintSongList; you will have to do that yourself.

CTRL+SHIFT+S:
Save. Save current file overwriting the original song file's contents. Back up your files!

F:
Filter. Hover over the desired instruments in the small instruments bar in the ribbon menu and press F. This will select the instruments and ignore the other instruments when using clipboard functions like copy, cut, delete.

R:
Replace. You must have a region selected for this to work. A window pops up. You can select the note you want to replace and the note you want to replace with. NOTE: I think the filtered notes screws this function up so that only the filtered notes can be replaced.

CTRL+C:
Copy. You must have a region selected for this to work. Copy all notes in that region. If there are filtered notes, only those notes are copied. Treat this like your system's clipboard. Subsequent copies replace the content on that clipboard; there is no copy history.

CTRL+X:
Cut. You must have a region selected for this to work. Copy and delete all notes in that region. If there are filtered notes, only those notes are copied and deleted.

BACKSPACE/DELETE:
Delete. You must have a region selected for this to work. Delete all notes in that region. If there are filtered notes, only those notes are deleted. 

CTRL+V:
Paste. Hover over your desired measure index. Whatever's copied gets pasted. The notes get pasted ONTO the existing notes if there are any. This is NOT like a text editor where your text gets pasted in between.

I:
Insert. Hover over your desired measure index. Whatever's copied gets pasted. The notes get pasted BETWEEN the existing notes if there are any. This is like a text editor where your text gets pasted in between.

-(MINUS):
Volume down. Hover over your desired measure index. Press '-' and the volume goes down.

+(PLUS):
Volume up. Hover over your desired measure index. Press '+' and the volume goes up. NOTE: '=' key also works.

CTRL+H:
Find and replace. A window pops up. There you can define what the measure looks like that you are looking for. You can define the measure you want to replace it with. You can just place certain notes and this window will find all measures that have that arrangement of notes. Volume is another qualifier and it will find measures that have that defined volume. Find prev and Find next buttons can be clicked which will find the measure and place a purple box around it. Replace button will replace that measure with the purple box. Replace all replaces all... NOTE: I HAVE SEEN THIS FUNCTION BE GLITCHY. DO NOT HEAVILY USE THE FIND AND REPLACE FUNCTION PER SESSION OR IT MAY MESS UP YOUR FILE.

CTRL+Z:
Undo. Undos the last change you made to the file. This change can be copy, cut, delete, volume change, replace, etc.

CTRL+Y:
Redo. Redos the undo. This change can be copy, cut, delete, volume change, replace, etc.

CTRL+B:
Break. Hover over the desired measure index, press CTRL+B and send the rest of the song past this index to another tab.

ALT+F:
Special measure filter. I never finished this and don't remember what it does. PLEASE DO NOT USE THIS, I AM NOT SURE WHAT EFFECT IT WILL HAVE ON YOUR FILES OR THE PROGRAM.

# SYSTEM MINIMUM REQUIREMENTS
OS: Windows 7, Windows 8, or Windows 10 (Vista and XP might work too idk)
Graphics: Any capable of OpenGL 2.1 (Any computer with hardware past 2010 should work)


# UPDATES
There will be no updates to this program. However it is very likely I will work on a newer and more robust version of the same program called MPOverviewer that may be released in the future.
