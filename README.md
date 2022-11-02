Original OpcodeEmulator Project by Meowthra

Intel Haswell Pentium / Celeron Series Or older processor expansion instruction set Emulation

https://www.insanelymac.com/forum/topic/329704-opcode-emulator-opemu-plug-in-project/

requires Lilu plugin

#
# Updated source to be compiled using new acidanthera Lilu requirements.
#
To Compile
- Lilu 
- OpcodeEmulator
 
Copy/Paste the whole command into your Terminal window.
#
`git clone https://github.com/acidanthera/Lilu`

`cd Lilu && git clone https://github.com/acidanthera/MacKernelSDK`

`xcodebuild -project ./Lilu.xcodeproj -configuration Debug clean build ARCHS=x86_64 ONLY_ACTIVE_ARCH=YES`

`cd ..`

`git clone https://github.com/LAbyOne/OpcodeEmulator`

`cp -r $HOME/Lilu/build/Debug/Lilu.kext $HOME/OpcodeEmulator`

`cp -Rf $HOME/Lilu/MacKernelSDK` $HOME/OpcodeEmulator

`xcodebuild -project ./OpcodeEmulator/OpcodeEmulator.xcodeproj -configuration Release clean build ARCHS=x86_64 ONLY_ACTIVE_ARCH=YES`
#
