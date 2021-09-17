cmake_minimum_required(VERSION 3.8.0 FATAL_ERROR)

project(ffmpeg_rtsp VERSION 0.1.0 LANGUAGES CXX C)

message("FFMPEG BIN DIR = ${FFMPEG_BINARY_DIR}")

set(sources 
	FFmpegDecoder.cpp
	FFmpegDecoder.h
	FFmpegH264Encoder.cpp
	FFmpegH264Encoder.h
	FFmpegH264Source.cpp
	FFmpegH264Source.h
	LiveRTSPServer.cpp
	LiveRTSPServer.h
	LiveServerMediaSubsession.cpp
	LiveServerMediaSubsession.h
)

#LIVE555

include_directories(
	${PROJECT_SOURCE_DIR}/live/BasicUsageEnvironment/include
	${PROJECT_SOURCE_DIR}/live/UsageEnvironment/include
	${PROJECT_SOURCE_DIR}/live/groupsock/include
	${PROJECT_SOURCE_DIR}/live/liveMedia/include
)

link_directories(
	${PROJECT_SOURCE_DIR}/live/BasicUsageEnvironment
	${PROJECT_SOURCE_DIR}/live/UsageEnvironment
	${PROJECT_SOURCE_DIR}/live/groupsock
	${PROJECT_SOURCE_DIR}/live/liveMedia
)

set(LIVE_LIBRARIES BasicUsageEnvironment UsageEnvironment groupsock liveMedia)

#PTHREAD

include_directories(${PROJECT_SOURCE_DIR}/pthread/include)

set(PTHREAD_LIBRARIES ${PROJECT_SOURCE_DIR}/pthread/pthreadVC3.lib)

file(
  COPY ${CMAKE_CURRENT_LIST_DIR}/pthread/pthreadVC3.dll 
  DESTINATION ${CMAKE_BINARY_DIR}/Debug/
)

#FFMPEG

set(CMAKE_MODULE_PATH C:/vcpkg/installed/x64-windows/share/ffmpeg)
find_package(FFMPEG COMPONENTS AVCODEC AVFORMAT AVUTIL AVDEVICE REQUIRED)

if(FFMPEG_FOUND)
	message("FFMPEG_INCLUDE_DIRS = ${FFMPEG_INCLUDE_DIRS}")
	message("FFMPEG_LIBRARIES = ${FFMPEG_LIBRARIES}")
	message("FFMPEG_DEFINITIONS = ${FFMPEG_DEFINITIONS}")
	include_directories(${FFMPEG_INCLUDE_DIRS})
endif()

find_library(FFMPEG_LIB FFMPEG)

add_executable(ffmpeg_rtsp main.cpp ${sources})

target_link_libraries(ffmpeg_rtsp 
	PRIVATE
		wsock32 ws2_32
		${FFMPEG_LIBRARIES}
		${PTHREAD_LIBRARIES}
		${LIVE_LIBRARIES}
)