rem Batch file to reformat and populate memory card on Drive F:
echo on

diskpart /s RepartitionFScript.txt
format F: /Q /FS:FAT /V:MusicWAVs

echo off
copy  /v /-y  1stReadMe.txt F:
copy  /V /-Y  File0.wav            F:File0.wav
copy  /V /-Y  PlzDownSelection.wav F:File1.wav
copy  /V /-Y  PlzDownSelection.wav F:File2.wav
copy  /V /-Y  PlzDownSelection.wav F:File3.wav
copy  /V /-Y  PlzDownSelection.wav F:File4.wav
copy  /V /-Y  PlzDownSelection.wav F:File5.wav
copy  /V /-Y  PlzDownSelection.wav F:File6.wav
copy  /V /-Y  PlzDownSelection.wav F:File7.wav
copy  /V /-Y  PlzDownSelection.wav F:File8.wav
copy  /V /-Y  PlzDownSelection.wav F:File9.wav
copy  /V /-Y  PlzDownSelection.wav F:FileA.wav
copy  /V /-Y  PlzDownSelection.wav F:FileB.wav
copy  /V /-Y  PlzDownSelection.wav F:FileC.wav
copy  /V /-Y  PlzDownSelection.wav F:FileD.wav
copy  /V /-Y  PlzDownSelection.wav F:FileE.wav
copy  /V /-Y  PlzDownSelection.wav F:FileF.wav

