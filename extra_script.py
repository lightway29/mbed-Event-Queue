# -*- coding: utf-8 -*-
Import("env")

env.AddPostAction(
    "$BUILD_DIR/firmware.elf",
    env.VerboseAction(" ".join([
        "$OBJCOPY", "-O", "ihex", "-R", ".eeprom",
        "$BUILD_DIR/firmware.elf", "$BUILD_DIR/firmware.hex"
    ]), "Building $BUILD_DIR/firmware.hex")
)

env.Replace(
    UPLOADBOOTCMD='lpc21isp -hex $BUILD_DIR/firmware.hex $UPLOAD_PORT $UPLOAD_SPEED 12000'
)

bootloader_path = "$BUILD_DIR/firmware.hex"
uploadboot = env.Alias(
    "uploadboot", bootloader_path,
    [env.VerboseAction(env.AutodetectUploadPort, "Looking for upload port..."),
     env.VerboseAction("$UPLOADBOOTCMD", "Uploading bootloader $SOURCE")])
AlwaysBuild(uploadboot)
