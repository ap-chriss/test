rem Batch file to reformat and populate memory card on Drive D:
echo on

diskpart /s RepartitionDScript.txt
format D: /FS:FAT /V:MusicWAVs

echo off
copy  /v /-y  1stReadMe.txt D:
copy  /V /-Y  File0.wav            D:File0.wav
copy  /V /-Y  PlzDownSelection.wav D:File1.wav
copy  /V /-Y  PlzDownSelection.wav D:File2.wav
copy  /V /-Y  PlzDownSelection.wav D:File3.wav
copy  /V /-Y  PlzDownSelection.wav D:File4.wav
copy  /V /-Y  PlzDownSelection.wav D:File5.wav
copy  /V /-Y  PlzDownSelection.wav D:File6.wav
copy  /V /-Y  PlzDownSelection.wav D:File7.wav
copy  /V /-Y  PlzDownSelection.wav D:File8.wav
copy  /V /-Y  PlzDownSelection.wav D:File9.wav
copy  /V /-Y  PlzDownSelection.wav D:FileA.wav
copy  /V /-Y  PlzDownSelection.wav D:FileB.wav
copy  /V /-Y  PlzDownSelection.wav D:FileC.wav
copy  /V /-Y  PlzDownSelection.wav D:FileD.wav
copy  /V /-Y  PlzDownSelection.wav D:FileE.wav
copy  /V /-Y  PlzDownSelection.wav D:FileF.wav

