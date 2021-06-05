OpcodeEmulator by Meowthra

Intel Haswell Pentium / Celeron Series Or older processor expansion instruction set Emulation

https://www.insanelymac.com/forum/topic/329704-opcode-emulator-opemu-plug-in-project/


requires Lilu plugin

to Compile

git clone https://github.com/acidanthera/Lilu

git clone https://github.com/HelmoHass/OpcodeEmulator

xcodebuild -project ./Lilu/Lilu.xcodeproj -configuration Debug clean build ARCHS=x86_64 ONLY_ACTIVE_ARCH=YES

cp -r $HOME/Lilu/build/Debug/Lilu.kext $HOME/OpcodeEmulator

xcodebuild -project ./OpcodeEmulator/OpcodeEmulator.xcodeproj -configuration Release clean build ARCHS=x86_64 ONLY_ACTIVE_ARCH=YES


