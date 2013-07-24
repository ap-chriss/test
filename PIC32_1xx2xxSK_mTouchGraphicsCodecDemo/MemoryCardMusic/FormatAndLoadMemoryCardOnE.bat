rem Batch file to reformat and populate memory card on Drive E:
echo on

diskpart /s RepartitionEScript.txt
format E: /Q /FS:FAT /V:MusicWAVs
rem format E: /FS:FAT /V:MusicWAVs

echo off
copy  /v /-y  1stReadMe.txt E:
copy  /V /-Y  File0.wav            E:File0.wav
copy  /V /-Y  PlzDownSelection.wav E:File1.wav
copy  /V /-Y  PlzDownSelection.wav E:File2.wav
copy  /V /-Y  PlzDownSelection.wav E:File3.wav
copy  /V /-Y  PlzDownSelection.wav E:File4.wav
copy  /V /-Y  PlzDownSelection.wav E:File5.wav
copy  /V /-Y  PlzDownSelection.wav E:File6.wav
copy  /V /-Y  PlzDownSelection.wav E:File7.wav
copy  /V /-Y  PlzDownSelection.wav E:File8.wav
copy  /V /-Y  PlzDownSelection.wav E:File9.wav
copy  /V /-Y  PlzDownSelection.wav E:FileA.wav
copy  /V /-Y  PlzDownSelection.wav E:FileB.wav
copy  /V /-Y  PlzDownSelection.wav E:FileC.wav
copy  /V /-Y  PlzDownSelection.wav E:FileD.wav
copy  /V /-Y  PlzDownSelection.wav E:FileE.wav
copy  /V /-Y  PlzDownSelection.wav E:FileF.wav
