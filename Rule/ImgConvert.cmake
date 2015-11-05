####################
# ImgConvert #
####################

file(GLOB proj_src ${CMAKE_SOURCE_DIR}/src/[A-Za-z0-9]*.h ${CMAKE_SOURCE_DIR}/src/[A-Za-z0-9]*.cpp)
file(GLOB Qt_ui ${CMAKE_SOURCE_DIR}/src/UIFile/[A-Za-z0-9]*.ui)
file(GLOB Qt_moc ${CMAKE_SOURCE_DIR}/src/Moc/[A-Za-z0-9]*.h)

if (QT_VERSION EQUAL "4")
	QT4_WRAP_UI(QT_UI ${Qt_ui})
	QT4_WRAP_CPP(QT_MOC ${Qt_moc})
else (QT_VERSION EQUAL "4")
	if (QT_VERSION EQUAL "5")
		QT5_WRAP_UI(QT_UI ${Qt_ui})
		QT5_WRAP_CPP(QT_MOC ${Qt_moc})
	endif (QT_VERSION EQUAL "5")
endif (QT_VERSION EQUAL "4")

source_group(Others REGULAR_EXPRESSION ".*")
source_group(Header REGULAR_EXPRESSION ".*\\.h")
source_group(Source REGULAR_EXPRESSION ".*\\.cpp")
source_group(Main REGULAR_EXPRESSION ".*main\\.cpp")
source_group(Ui\\file FILES ${Qt_ui})
source_group(Ui\\header REGULAR_EXPRESSION "ui_.*\\.h")

IF (CMAKE_SYSTEM_NAME MATCHES "Windows")
	add_executable(${PROJECT_NAME} WIN32 ${proj_src} ${QT_UI} ${QT_MOC} ${Qt_moc} ${CMAKE_SOURCE_DIR}/Rule/ImgConvert.cmake)
ELSE (CMAKE_SYSTEM_NAME MATCHES "Windows")
	IF (CMAKE_SYSTEM_NAME MATCHES "Darwin")
		add_executable(${PROJECT_NAME} MACOSX_BUNDLE ${proj_src} ${QT_UI} ${QT_MOC} ${Qt_moc} ${Qt_tr} ${CMAKE_SOURCE_DIR}/Rule/ImgConvert.cmake)
	ELSE (CMAKE_SYSTEM_NAME MATCHES "Darwin")
		add_executable(${PROJECT_NAME} ${proj_src} ${QT_UI} ${QT_MOC} ${Qt_moc} ${CMAKE_SOURCE_DIR}/Rule/ImgConvert.cmake)
	ENDIF (CMAKE_SYSTEM_NAME MATCHES "Darwin")
ENDIF (CMAKE_SYSTEM_NAME MATCHES "Windows")

set_target_properties(${PROJECT_NAME} PROPERTIES DEBUG_OUTPUT_NAME "${PROJECT_NAME}_D")
set_target_properties(${PROJECT_NAME} PROPERTIES RELEASE_OUTPUT_NAME "${PROJECT_NAME}")
set_target_properties(${PROJECT_NAME} PROPERTIES RELWITHDEBINFO_OUTPUT_NAME "${PROJECT_NAME}_RWDI")
set_target_properties(${PROJECT_NAME} PROPERTIES MINSIZEREL_OUTPUT_NAME "${PROJECT_NAME}_MR")

if (QT_VERSION EQUAL "4")
	target_link_libraries(${PROJECT_NAME} ${QT_LIBRARIES})
else (QT_VERSION EQUAL "4")
	if (QT_VERSION EQUAL "5")
			target_link_libraries(${PROJECT_NAME} Qt5::Widgets)
	endif (QT_VERSION EQUAL "5")
endif (QT_VERSION EQUAL "4")