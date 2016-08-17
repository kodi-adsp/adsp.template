project(KodiThreads)

set(SOURCES threads/Atomics.cpp
            threads/Event.cpp
            threads/Thread.cpp
            threads/Timer.cpp
            threads/SystemClock.cpp
            commons/Exception.cpp
            system.cpp)

set(HEADERS threads/platform/Implementation.cpp

            threads/Atomics.h
            threads/Condition.h
            threads/CriticalSection.h
            threads/Event.h
            threads/Helpers.h
            threads/Lockables.h
            threads/MipsAtomics.h
            threads/SharedSection.h
            threads/SingleLock.h
            threads/SystemClock.h
            threads/Thread.h
            threads/ThreadImpl.h
            threads/ThreadLocal.h
            threads/Timer.h
            threads/platform/Condition.h
            threads/platform/CriticalSection.h
            threads/platform/ThreadImpl.h
            threads/platform/ThreadLocal.h

            system.h
            commons/Exception.h)

include_directories(${p8-platform_INCLUDE_DIRS})
            
addon_source_group("${SOURCES}")
addon_source_group("${HEADERS}")

if(MSVC)
  list(APPEND SOURCES threads/platform/win/Win32Exception.cpp)
  list(APPEND HEADERS threads/platform/win/Win32Exception.h)
endif()

add_library(${PROJECT_NAME} STATIC ${SOURCES} ${HEADERS})
add_library(kodi::${PROJECT_NAME} ALIAS ${PROJECT_NAME})
target_include_directories(${PROJECT_NAME}
                           PRIVATE ${CMAKE_CURRENT_SOURCE_DIR}
                           INTERFACE $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}>
                           INTERFACE $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/..>)
if(MSVC)
  target_link_libraries(${PROJECT_NAME} PUBLIC winmm.lib)
endif()

# install all specific module files
#asplib_install_module(${PROJECT_NAME} "${HEADERS}" ${CMAKE_INSTALL_PREFIX}/include)
install(TARGETS ${PROJECT_NAME} EXPORT kodi DESTINATION ${CMAKE_INSTALL_PREFIX}/lib)
