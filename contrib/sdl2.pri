SOURCES += \
	SDL2/stdlib/SDL_string.c \
	SDL2/stdlib/SDL_stdlib.c \
	SDL2/stdlib/SDL_qsort.c \
	SDL2/stdlib/SDL_malloc.c \
	SDL2/stdlib/SDL_iconv.c \
	SDL2/stdlib/SDL_getenv.c \
	SDL2/SDL.c \
	SDL2/SDL_log.c \
	SDL2/SDL_hints.c \
	SDL2/SDL_error.c \
	SDL2/SDL_assert.c \
	SDL2/events/SDL_windowevents.c \
	SDL2/events/SDL_touch.c \
	SDL2/events/SDL_quit.c \
	SDL2/events/SDL_mouse.c \
	SDL2/events/SDL_keyboard.c \
	SDL2/events/SDL_gesture.c \
	SDL2/events/SDL_events.c \
	SDL2/events/SDL_dropevents.c \
	SDL2/events/SDL_clipboardevents.c \
	SDL2/video/SDL_video.c \
	SDL2/video/SDL_surface.c \
	SDL2/video/SDL_stretch.c \
	SDL2/video/SDL_shape.c \
	SDL2/video/SDL_RLEaccel.c \
	SDL2/video/SDL_rect.c \
	SDL2/video/SDL_pixels.c \
	SDL2/video/SDL_fillrect.c \
	SDL2/video/SDL_egl.c \
	SDL2/video/SDL_clipboard.c \
	SDL2/video/SDL_bmp.c \
	SDL2/video/SDL_blit.c \
	SDL2/video/SDL_blit_slow.c \
	SDL2/video/SDL_blit_N.c \
	SDL2/video/SDL_blit_copy.c \
	SDL2/video/SDL_blit_auto.c \
	SDL2/video/SDL_blit_A.c \
	SDL2/video/SDL_blit_1.c \
	SDL2/video/SDL_blit_0.c \
	SDL2/video/dummy/SDL_nullvideo.c \
	SDL2/video/dummy/SDL_nullframebuffer.c \
	SDL2/video/dummy/SDL_nullevents.c \
	SDL2/timer/SDL_timer.c \
	SDL2/audio/SDL_wave.c \
	SDL2/audio/SDL_audiotypecvt.c \
	SDL2/audio/SDL_audiodev.c \
	SDL2/audio/SDL_audio.c \
	SDL2/audio/dummy/SDL_dummyaudio.c \
	SDL2/audio/dsp/SDL_dspaudio.c \
	SDL2/audio/disk/SDL_diskaudio.c \
	SDL2/audio/arts/SDL_artsaudio.c \
	SDL2/audio/SDL_mixer.c \
	SDL2/audio/SDL_audiocvt.c \
	SDL2/joystick/SDL_joystick.c \
	SDL2/joystick/SDL_gamecontroller.c \
	SDL2/haptic/SDL_haptic.c \
	SDL2/render/software/SDL_rotate.c \
	SDL2/render/software/SDL_render_sw.c \
	SDL2/render/software/SDL_drawpoint.c \
	SDL2/render/software/SDL_drawline.c \
	SDL2/render/software/SDL_blendpoint.c \
	SDL2/render/software/SDL_blendline.c \
	SDL2/render/software/SDL_blendfillrect.c \
	SDL2/render/SDL_yuv_mmx.c \
	SDL2/render/opengles2/SDL_shaders_gles2.c \
	SDL2/render/opengles2/SDL_render_gles2.c \
	SDL2/render/opengles/SDL_render_gles.c \
	SDL2/render/opengl/SDL_shaders_gl.c \
	SDL2/render/opengl/SDL_render_gl.c \
	SDL2/render/SDL_yuv_sw.c \
	SDL2/render/SDL_render.c \
	SDL2/input/evdev/SDL_evdev.c \
	SDL2/thread/SDL_thread.c \
	SDL2/test/SDL_test_random.c \
	SDL2/test/SDL_test_md5.c \
	SDL2/test/SDL_test_log.c \
	SDL2/test/SDL_test_imagePrimitivesBlend.c \
	SDL2/test/SDL_test_imagePrimitives.c \
	SDL2/test/SDL_test_imageFace.c \
	SDL2/test/SDL_test_imageBlitBlend.c \
	SDL2/test/SDL_test_imageBlit.c \
	SDL2/test/SDL_test_harness.c \
	SDL2/test/SDL_test_fuzzer.c \
	SDL2/test/SDL_test_font.c \
	SDL2/test/SDL_test_crc32.c \
	SDL2/test/SDL_test_compare.c \
	SDL2/test/SDL_test_common.c \
	SDL2/test/SDL_test_assert.c \
	SDL2/power/SDL_power.c \
	SDL2/atomic/SDL_spinlock.c \
	SDL2/atomic/SDL_atomic.c \
	SDL2/cpuinfo/SDL_cpuinfo.c \
	SDL2/libm/s_sin.c \
	SDL2/libm/s_scalbn.c \
	SDL2/libm/s_floor.c \
	SDL2/libm/s_fabs.c \
	SDL2/libm/s_cos.c \
	SDL2/libm/s_copysign.c \
	SDL2/libm/s_atan.c \
	SDL2/libm/k_sin.c \
	SDL2/libm/k_rem_pio2.c \
	SDL2/libm/k_cos.c \
	SDL2/libm/e_sqrt.c \
	SDL2/libm/e_rem_pio2.c \
	SDL2/libm/e_pow.c \
	SDL2/libm/e_log.c \
	SDL2/libm/e_atan2.c \
	SDL2/file/SDL_rwops.c \
#	SDL2/core/linux/SDL_udev.c \
#	SDL2/core/android/SDL_android.c \
#	SDL2/video/raspberry/SDL_rpivideo.c \
#	SDL2/video/raspberry/SDL_rpiopengles.c \
#	SDL2/video/raspberry/SDL_rpimouse.c \
#	SDL2/video/raspberry/SDL_rpievents.c \
#	SDL2/video/pandora/SDL_pandora.c \
#	SDL2/video/pandora/SDL_pandora_events.c \
#	SDL2/video/directfb/SDL_DirectFB_WM.c \
#	SDL2/video/directfb/SDL_DirectFB_window.c \
#	SDL2/video/directfb/SDL_DirectFB_video.c \
#	SDL2/video/directfb/SDL_DirectFB_shape.c \
#	SDL2/video/directfb/SDL_DirectFB_render.c \
#	SDL2/video/directfb/SDL_DirectFB_opengl.c \
#	SDL2/video/directfb/SDL_DirectFB_mouse.c \
#	SDL2/video/directfb/SDL_DirectFB_modes.c \
#	SDL2/video/directfb/SDL_DirectFB_events.c \
#	SDL2/video/directfb/SDL_DirectFB_dyn.c \
#	SDL2/video/bwindow/SDL_bwindow.cc \
#	SDL2/video/bwindow/SDL_bvideo.cc \
#	SDL2/video/bwindow/SDL_bopengl.cc \
#	SDL2/video/bwindow/SDL_bmodes.cc \
#	SDL2/video/bwindow/SDL_bkeyboard.cc \
#	SDL2/video/bwindow/SDL_bframebuffer.cc \
#	SDL2/video/bwindow/SDL_bevents.cc \
#	SDL2/video/bwindow/SDL_bclipboard.cc \
#	SDL2/video/android/SDL_androidwindow.c \
#	SDL2/video/android/SDL_androidvideo.c \
#	SDL2/video/android/SDL_androidtouch.c \
#	SDL2/video/android/SDL_androidkeyboard.c \
#	SDL2/video/android/SDL_androidgl.c \
#	SDL2/video/android/SDL_androidevents.c \
#	SDL2/video/android/SDL_androidclipboard.c \
#	SDL2/audio/winmm/SDL_winmm.c \
#	SDL2/audio/sun/SDL_sunaudio.c \
#	SDL2/audio/sndio/SDL_sndioaudio.c \
#	SDL2/audio/qsa/SDL_qsa_audio.c \
#	SDL2/audio/pulseaudio/SDL_pulseaudio.c \
#	SDL2/audio/paudio/SDL_paudio.c \
#	SDL2/audio/nas/SDL_nasaudio.c \
#	SDL2/audio/fusionsound/SDL_fsaudio.c \
#	SDL2/audio/esd/SDL_esdaudio.c \
#	SDL2/audio/bsd/SDL_bsdaudio.c \
#	SDL2/audio/baudio/SDL_beaudio.cc \
#	SDL2/audio/android/SDL_androidaudio.c \
#	SDL2/audio/alsa/SDL_alsa_audio.c \
#	SDL2/joystick/dummy/SDL_sysjoystick.c \
#	SDL2/joystick/bsd/SDL_sysjoystick.c \
#	SDL2/joystick/beos/SDL_bejoystick.cc \
#	SDL2/joystick/android/SDL_sysjoystick.c \
#	SDL2/haptic/dummy/SDL_syshaptic.c \
#	SDL2/filesystem/dummy/SDL_sysfilesystem.c \
#	SDL2/filesystem/beos/SDL_sysfilesystem.cc \
#	SDL2/loadso/dummy/SDL_sysloadso.c \
#	SDL2/loadso/beos/SDL_sysloadso.c \
#	SDL2/main/dummy/SDL_dummy_main.c \
#	SDL2/main/android/SDL_android_main.c \
#	SDL2/main/beos/SDL_BeApp.cc \
#	SDL2/thread/beos/SDL_systls.c \
#	SDL2/thread/beos/SDL_systhread.c \
#	SDL2/thread/beos/SDL_syssem.c \
#	SDL2/thread/generic/SDL_systls.c \
#	SDL2/thread/generic/SDL_systhread.c \
#	SDL2/thread/generic/SDL_syssem.c \
#	SDL2/thread/generic/SDL_sysmutex.c \
#	SDL2/thread/generic/SDL_syscond.c \
#	SDL2/power/beos/SDL_syspower.c \
#	SDL2/power/android/SDL_syspower.c \

HEADERS += \
	SDL2/core/windows/SDL_windows.h \
	SDL2/core/linux/SDL_udev.h \
	SDL2/core/android/SDL_android.h \
	SDL2/SDL_error_c.h \
	SDL2/SDL_assert_c.h \
	SDL2/audio/winmm/SDL_winmm.h \
	SDL2/audio/sun/SDL_sunaudio.h \
	SDL2/audio/sndio/SDL_sndioaudio.h \
	SDL2/audio/SDL_wave.h \
	SDL2/audio/SDL_sysaudio.h \
	SDL2/audio/SDL_audiomem.h \
	SDL2/audio/SDL_audiodev_c.h \
	SDL2/audio/SDL_audio_c.h \
	SDL2/audio/qsa/SDL_qsa_audio.h \
	SDL2/audio/pulseaudio/SDL_pulseaudio.h \
	SDL2/audio/psp/SDL_pspaudio.h \
	SDL2/audio/paudio/SDL_paudio.h \
	SDL2/audio/nas/SDL_nasaudio.h \
	SDL2/audio/fusionsound/SDL_fsaudio.h \
	SDL2/audio/esd/SDL_esdaudio.h \
	SDL2/audio/dummy/SDL_dummyaudio.h \
	SDL2/audio/dsp/SDL_dspaudio.h \
	SDL2/audio/disk/SDL_diskaudio.h \
	SDL2/audio/directsound/SDL_directsound.h \
	SDL2/audio/directsound/directx.h \
	SDL2/audio/bsd/SDL_bsdaudio.h \
	SDL2/audio/baudio/SDL_beaudio.h \
	SDL2/audio/arts/SDL_artsaudio.h \
	SDL2/audio/android/SDL_androidaudio.h \
	SDL2/audio/alsa/SDL_alsa_audio.h \
	SDL2/events/SDL_windowevents_c.h \
	SDL2/events/SDL_touch_c.h \
	SDL2/events/SDL_sysevents.h \
	SDL2/events/SDL_mouse_c.h \
	SDL2/events/SDL_keyboard_c.h \
	SDL2/events/SDL_gesture_c.h \
	SDL2/events/SDL_events_c.h \
	SDL2/events/SDL_dropevents_c.h \
	SDL2/events/SDL_clipboardevents_c.h \
	SDL2/events/scancodes_xfree86.h \
	SDL2/events/scancodes_windows.h \
	SDL2/events/scancodes_linux.h \
	SDL2/events/scancodes_darwin.h \
	SDL2/events/default_cursor.h \
	SDL2/events/blank_cursor.h \
	SDL2/haptic/SDL_syshaptic.h \
	SDL2/haptic/SDL_haptic_c.h \
	SDL2/input/evdev/SDL_evdev.h \
	SDL2/power/uikit/SDL_syspower.h \
	SDL2/main/beos/SDL_BeApp.h \
	SDL2/main/beos/SDL_BApp.h \
	SDL2/libm/math_private.h \
	SDL2/libm/math_libm.h \
	SDL2/joystick/windows/SDL_dxjoystick_c.h \
	SDL2/joystick/SDL_sysjoystick.h \
	SDL2/joystick/SDL_joystick_c.h \
	SDL2/joystick/SDL_gamecontrollerdb.h \
	SDL2/joystick/linux/SDL_sysjoystick_c.h \
	SDL2/joystick/iphoneos/SDLUIAccelerationDelegate.h \
	SDL2/render/software/SDL_rotate.h \
	SDL2/render/software/SDL_render_sw_c.h \
	SDL2/render/software/SDL_drawpoint.h \
	SDL2/render/software/SDL_drawline.h \
	SDL2/render/software/SDL_draw.h \
	SDL2/render/software/SDL_blendpoint.h \
	SDL2/render/software/SDL_blendline.h \
	SDL2/render/software/SDL_blendfillrect.h \
	SDL2/render/SDL_yuv_sw_c.h \
	SDL2/render/SDL_sysrender.h \
	SDL2/render/opengles2/SDL_shaders_gles2.h \
	SDL2/render/opengles2/SDL_gles2funcs.h \
	SDL2/render/opengles/SDL_glesfuncs.h \
	SDL2/render/opengl/SDL_shaders_gl.h \
	SDL2/render/opengl/SDL_glfuncs.h \
	SDL2/render/mmx.h \
	SDL2/thread/windows/SDL_systhread_c.h \
	SDL2/thread/SDL_thread_c.h \
	SDL2/thread/SDL_systhread.h \
	SDL2/thread/pthread/SDL_systhread_c.h \
	SDL2/thread/pthread/SDL_sysmutex_c.h \
	SDL2/thread/psp/SDL_systhread_c.h \
	SDL2/thread/psp/SDL_sysmutex_c.h \
	SDL2/thread/generic/SDL_systhread_c.h \
	SDL2/thread/generic/SDL_sysmutex_c.h \
	SDL2/thread/beos/SDL_systhread_c.h \
	SDL2/timer/SDL_timer_c.h \
	SDL2/video/x11/SDL_x11xinput2.h \
	SDL2/video/x11/SDL_x11window.h \
	SDL2/video/x11/SDL_x11video.h \
	SDL2/video/x11/SDL_x11touch.h \
	SDL2/video/x11/SDL_x11sym.h \
	SDL2/video/x11/SDL_x11shape.h \
	SDL2/video/x11/SDL_x11opengles.h \
	SDL2/video/x11/SDL_x11opengl.h \
	SDL2/video/x11/SDL_x11mouse.h \
	SDL2/video/x11/SDL_x11modes.h \
	SDL2/video/x11/SDL_x11messagebox.h \
	SDL2/video/x11/SDL_x11keyboard.h \
	SDL2/video/x11/SDL_x11framebuffer.h \
	SDL2/video/x11/SDL_x11events.h \
	SDL2/video/x11/SDL_x11dyn.h \
	SDL2/video/x11/SDL_x11clipboard.h \
	SDL2/video/x11/imKStoUCS.h \
	SDL2/video/x11/edid.h \
	SDL2/video/windows/wmmsg.h \
	SDL2/video/windows/SDL_windowswindow.h \
	SDL2/video/windows/SDL_windowsvideo.h \
	SDL2/video/windows/SDL_windowsshape.h \
	SDL2/video/windows/SDL_windowsopengl.h \
	SDL2/video/windows/SDL_windowsmouse.h \
	SDL2/video/windows/SDL_windowsmodes.h \
	SDL2/video/windows/SDL_windowsmessagebox.h \
	SDL2/video/windows/SDL_windowskeyboard.h \
	SDL2/video/windows/SDL_windowsframebuffer.h \
	SDL2/video/windows/SDL_windowsevents.h \
	SDL2/video/windows/SDL_windowsclipboard.h \
	SDL2/video/windows/SDL_vkeys.h \
	SDL2/video/windows/SDL_msctf.h \
	SDL2/video/uikit/SDL_uikitwindow.h \
	SDL2/video/uikit/SDL_uikitviewcontroller.h \
	SDL2/video/uikit/SDL_uikitview.h \
	SDL2/video/uikit/SDL_uikitvideo.h \
	SDL2/video/uikit/SDL_uikitopenglview.h \
	SDL2/video/uikit/SDL_uikitopengles.h \
	SDL2/video/uikit/SDL_uikitmodes.h \
	SDL2/video/uikit/SDL_uikitmessagebox.h \
	SDL2/video/uikit/SDL_uikitevents.h \
	SDL2/video/uikit/SDL_uikitappdelegate.h \
	SDL2/video/uikit/keyinfotable.h \
	SDL2/video/SDL_sysvideo.h \
	SDL2/video/SDL_shape_internals.h \
	SDL2/video/SDL_RLEaccel_c.h \
	SDL2/video/SDL_rect_c.h \
	SDL2/video/SDL_pixels_c.h \
	SDL2/video/SDL_egl.h \
	SDL2/video/SDL_blit.h \
	SDL2/video/SDL_blit_slow.h \
	SDL2/video/SDL_blit_copy.h \
	SDL2/video/SDL_blit_auto.h \
	SDL2/video/raspberry/SDL_rpivideo.h \
	SDL2/video/raspberry/SDL_rpiopengles.h \
	SDL2/video/raspberry/SDL_rpimouse.h \
	SDL2/video/raspberry/SDL_rpievents_c.h \
	SDL2/video/pandora/SDL_pandora.h \
	SDL2/video/pandora/SDL_pandora_events.h \
	SDL2/video/dummy/SDL_nullvideo.h \
	SDL2/video/dummy/SDL_nullframebuffer_c.h \
	SDL2/video/dummy/SDL_nullevents_c.h \
	SDL2/video/directfb/SDL_DirectFB_window.h \
	SDL2/video/directfb/SDL_DirectFB_video.h \
	SDL2/video/directfb/SDL_DirectFB_shape.h \
	SDL2/video/directfb/SDL_DirectFB_render.h \
	SDL2/video/directfb/SDL_DirectFB_opengl.h \
	SDL2/video/directfb/SDL_DirectFB_mouse.h \
	SDL2/video/directfb/SDL_DirectFB_modes.h \
	SDL2/video/directfb/SDL_DirectFB_events.h \
	SDL2/video/directfb/SDL_DirectFB_dyn.h \
	SDL2/video/bwindow/SDL_bwindow.h \
	SDL2/video/bwindow/SDL_BWin.h \
	SDL2/video/bwindow/SDL_bvideo.h \
	SDL2/video/bwindow/SDL_bopengl.h \
	SDL2/video/bwindow/SDL_bmodes.h \
	SDL2/video/bwindow/SDL_bkeyboard.h \
	SDL2/video/bwindow/SDL_bframebuffer.h \
	SDL2/video/bwindow/SDL_bevents.h \
	SDL2/video/bwindow/SDL_bclipboard.h \
	SDL2/video/android/SDL_androidwindow.h \
	SDL2/video/android/SDL_androidvideo.h \
	SDL2/video/android/SDL_androidtouch.h \
	SDL2/video/android/SDL_androidkeyboard.h \
	SDL2/video/android/SDL_androidevents.h \
	SDL2/video/android/SDL_androidclipboard.h \
	SDL2/SDL.h \
	SDL2/SDL_video.h \
	SDL2/SDL_version.h \
	SDL2/SDL_types.h \
	SDL2/SDL_touch.h \
	SDL2/SDL_timer.h \
	SDL2/SDL_thread.h \
	SDL2/SDL_test.h \
	SDL2/SDL_test_random.h \
	SDL2/SDL_test_md5.h \
	SDL2/SDL_test_log.h \
	SDL2/SDL_test_images.h \
	SDL2/SDL_test_harness.h \
	SDL2/SDL_test_fuzzer.h \
	SDL2/SDL_test_font.h \
	SDL2/SDL_test_crc32.h \
	SDL2/SDL_test_compare.h \
	SDL2/SDL_test_common.h \
	SDL2/SDL_test_assert.h \
	SDL2/SDL_syswm.h \
	SDL2/SDL_system.h \
	SDL2/SDL_surface.h \
	SDL2/SDL_stdinc.h \
	SDL2/SDL_shape.h \
	SDL2/SDL_scancode.h \
	SDL2/SDL_rwops.h \
	SDL2/SDL_render.h \
	SDL2/SDL_rect.h \
	SDL2/SDL_quit.h \
	SDL2/SDL_power.h \
	SDL2/SDL_platform.h \
	SDL2/SDL_pixels.h \
	SDL2/SDL_opengles2.h \
	SDL2/SDL_opengles.h \
	SDL2/SDL_opengl.h \
	SDL2/SDL_name.h \
	SDL2/SDL_mutex.h \
	SDL2/SDL_mouse.h \
	SDL2/SDL_messagebox.h \
	SDL2/SDL_main.h \
	SDL2/SDL_log.h \
	SDL2/SDL_loadso.h \
	SDL2/SDL_keycode.h \
	SDL2/SDL_keyboard.h \
	SDL2/SDL_joystick.h \
	SDL2/SDL_hints.h \
	SDL2/SDL_haptic.h \
	SDL2/SDL_gesture.h \
	SDL2/SDL_gamecontroller.h \
	SDL2/SDL_filesystem.h \
	SDL2/SDL_events.h \
	SDL2/SDL_error.h \
	SDL2/SDL_endian.h \
	SDL2/SDL_cpuinfo.h \
	SDL2/SDL_copying.h \
	SDL2/SDL_config.h \
	SDL2/SDL_config_wiz.h \
	SDL2/SDL_config_windows.h \
	SDL2/SDL_config_linux.h \
	SDL2/SDL_config_psp.h \
	SDL2/SDL_config_pandora.h \
	SDL2/SDL_config_minimal.h \
	SDL2/SDL_config_macosx.h \
	SDL2/SDL_config_iphoneos.h \
	SDL2/SDL_config_android.h \
	SDL2/SDL_clipboard.h \
	SDL2/SDL_blendmode.h \
	SDL2/SDL_bits.h \
	SDL2/SDL_audio.h \
	SDL2/SDL_atomic.h \
	SDL2/SDL_assert.h \
	SDL2/close_code.h \
	SDL2/begin_code.h \
	SDL2/SDL_revision.h

unix:!macx:SOURCES += \
	SDL2/loadso/dlopen/SDL_sysloadso.c \
	SDL2/power/linux/SDL_syspower.c \
	SDL2/thread/pthread/SDL_systls.c \
	SDL2/thread/pthread/SDL_syssem.c \
	SDL2/thread/pthread/SDL_sysmutex.c \
	SDL2/thread/pthread/SDL_syscond.c \
	SDL2/thread/pthread/SDL_systhread.c \
	SDL2/timer/unix/SDL_systimer.c \
	SDL2/video/x11/SDL_x11xinput2.c \
	SDL2/video/x11/SDL_x11window.c \
	SDL2/video/x11/SDL_x11video.c \
	SDL2/video/x11/SDL_x11touch.c \
	SDL2/video/x11/SDL_x11shape.c \
	SDL2/video/x11/SDL_x11opengles.c \
	SDL2/video/x11/SDL_x11opengl.c \
	SDL2/video/x11/SDL_x11mouse.c \
	SDL2/video/x11/SDL_x11modes.c \
	SDL2/video/x11/SDL_x11messagebox.c \
	SDL2/video/x11/SDL_x11keyboard.c \
	SDL2/video/x11/SDL_x11framebuffer.c \
	SDL2/video/x11/SDL_x11events.c \
	SDL2/video/x11/SDL_x11dyn.c \
	SDL2/video/x11/SDL_x11clipboard.c \
	SDL2/video/x11/imKStoUCS.c \
	SDL2/video/x11/edid-parse.c \
	SDL2/filesystem/unix/SDL_sysfilesystem.c \
	SDL2/haptic/linux/SDL_syshaptic.c \
	SDL2/joystick/linux/SDL_sysjoystick.c

macx:SOURCES += \
	SDL2/loadso/dlopen/SDL_sysloadso.c \
	SDL2/power/macosx/SDL_syspower.c \
	SDL2/timer/unix/SDL_systimer.c \
	SDL2/thread/pthread/SDL_systls.c \
	SDL2/thread/pthread/SDL_syssem.c \
	SDL2/thread/pthread/SDL_sysmutex.c \
	SDL2/thread/pthread/SDL_syscond.c \
	SDL2/thread/pthread/SDL_systhread.c \
	SDL2/filesystem/unix/SDL_sysfilesystem.c \
	SDL2/haptic/darwin/SDL_syshaptic.c \
	SDL2/joystick/darwin/SDL_sysjoystick.c \
	SDL2/audio/coreaudio/SDL_coreaudio.c \

macx:OBJECTIVE_HEADERS += \
	SDL2/video/cocoa/SDL_cocoawindow.h \
	SDL2/video/cocoa/SDL_cocoavideo.h \
	SDL2/video/cocoa/SDL_cocoashape.h \
	SDL2/video/cocoa/SDL_cocoaopengl.h \
	SDL2/video/cocoa/SDL_cocoamousetap.h \
	SDL2/video/cocoa/SDL_cocoamouse.h \
	SDL2/video/cocoa/SDL_cocoamodes.h \
	SDL2/video/cocoa/SDL_cocoamessagebox.h \
	SDL2/video/cocoa/SDL_cocoakeyboard.h \
	SDL2/video/cocoa/SDL_cocoaevents.h \
	SDL2/video/cocoa/SDL_cocoaclipboard.h \
	SDL2/file/cocoa/SDL_rwopsbundlesupport.h

macx:OBJECTIVE_SOURCES += \
	SDL2/video/uikit/SDL_uikitwindow.m \
	SDL2/video/uikit/SDL_uikitviewcontroller.m \
	SDL2/video/uikit/SDL_uikitview.m \
	SDL2/video/uikit/SDL_uikitvideo.m \
	SDL2/video/uikit/SDL_uikitopenglview.m \
	SDL2/video/uikit/SDL_uikitopengles.m \
	SDL2/video/uikit/SDL_uikitmodes.m \
	SDL2/video/uikit/SDL_uikitmessagebox.m \
	SDL2/video/uikit/SDL_uikitevents.m \
	SDL2/video/uikit/SDL_uikitappdelegate.m \
	#SDL2/joystick/iphoneos/SDLUIAccelerationDelegate.m \
	#SDL2/joystick/iphoneos/SDL_sysjoystick.m \
	#SDL2/filesystem/cocoa/SDL_sysfilesystem.m \
	#SDL2/power/uikit/SDL_syspower.m \
	SDL2/file/cocoa/SDL_rwopsbundlesupport.m \
	SDL2/video/cocoa/SDL_cocoawindow.m \
	SDL2/video/cocoa/SDL_cocoashape.m \
	SDL2/video/cocoa/SDL_cocoamousetap.m \
	SDL2/video/cocoa/SDL_cocoamodes.m \
	SDL2/video/cocoa/SDL_cocoamessagebox.m \
	SDL2/video/cocoa/SDL_cocoakeyboard.m \
	SDL2/video/cocoa/SDL_cocoaevents.m \
	SDL2/video/cocoa/SDL_cocoaclipboard.m \
	SDL2/video/cocoa/SDL_cocoamouse.m \
	SDL2/video/cocoa/SDL_cocoavideo.m \
	SDL2/video/cocoa/SDL_cocoaopengl.m

win32:SOURCES += \
	SDL2/loadso/windows/SDL_sysloadso.c \
	SDL2/power/windows/SDL_syspower.c \
	SDL2/thread/windows/SDL_systls.c \
	SDL2/thread/windows/SDL_systhread.c \
	SDL2/thread/windows/SDL_syssem.c \
	SDL2/thread/windows/SDL_sysmutex.c \
	SDL2/filesystem/windows/SDL_sysfilesystem.c \
	SDL2/main/windows/SDL_windows_main.c \
	SDL2/haptic/windows/SDL_syshaptic.c \
	SDL2/joystick/windows/SDL_mmjoystick.c \
	SDL2/joystick/windows/SDL_dxjoystick.c \
	SDL2/audio/xaudio2/SDL_xaudio2.c \
	SDL2/timer/windows/SDL_systimer.c \
	SDL2/core/windows/SDL_windows.c \
	SDL2/video/windows/SDL_windowswindow.c \
	SDL2/video/windows/SDL_windowsvideo.c \
	SDL2/video/windows/SDL_windowsshape.c \
	SDL2/video/windows/SDL_windowsopengl.c \
	SDL2/video/windows/SDL_windowsmouse.c \
	SDL2/video/windows/SDL_windowsmodes.c \
	SDL2/video/windows/SDL_windowsmessagebox.c \
	SDL2/video/windows/SDL_windowskeyboard.c \
	SDL2/video/windows/SDL_windowsframebuffer.c \
	SDL2/video/windows/SDL_windowsevents.c \
	SDL2/video/windows/SDL_windowsclipboard.c \
	SDL2/thread/generic/SDL_syscond.c \
#	SDL2/audio/directsound/SDL_directsound.c \
#	SDL2/render/direct3d/SDL_render_d3d.c

macx:HEADERS += SDL2/audio/coreaudio/SDL_coreaudio.h
