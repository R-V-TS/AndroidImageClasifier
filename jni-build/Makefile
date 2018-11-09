build:
	ndk-build

install:
	cp -r libs/armeabi-v7a/*.so ../app/src/main/jniLibs/arm64/
	cp -r libs/armeabi-v7a/*.so ../app/src/main/jniLibs/arm/
	cp -r libs/armeabi-v7a/*.so ../app/src/main/jniLibs/armeabi-v7a/
	cp -r libs/armeabi-v7a/*.so ../app/src/main/jniLibs/x86/


clean:
	rm -r obj
	rm -r libs
