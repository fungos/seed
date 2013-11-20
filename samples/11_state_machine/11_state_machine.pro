include("../sample.pri")

SOURCES += main.cpp \
	data/agent_data.cpp \
	state_machine_sample.cpp \
	states/state_working.cpp \
	states/state_sleeping.cpp

HEADERS += \
	data/agent_data.h \
	state_machine_sample.h \
	states/state_working.h \
	states/state_sleeping.h

OTHER_FILES += \
	../bin/state_machine_app.config \
	../bin/state_machine_sample.config \
	../bin/state_machine_sample.scene \
	../bin/frame03.png
