project(adsp_base)

#find_package(p8-platform REQUIRED)

set(SOURCES adsp.template/ADSPHelpers.cpp
            adsp.template/client.cpp
            adsp.template/GUIDialogBase.cpp
            adsp.template/Addon/MVC/Interfaces/Controller/IController.hpp
            adsp.template/Addon/MVC/Interfaces/Model/IModel.hpp
            adsp.template/Addon/MVC/Interfaces/Model/IParameter.hpp
            adsp.template/Addon/MVC/Model/TParameter.hpp
            adsp.template/Addon/MessageSystem/Communication/Message.cpp
            adsp.template/Addon/MessageSystem/Communication/ActorProtocol.cpp
            adsp.template/Addon/MessageSystem/Communication/MessageDispatcher.hpp
            adsp.template/Addon/MessageSystem/Communication/MessageDispatcher.cpp
            adsp.template/Addon/MessageSystem/Sockets/TSocketMemcpy.hpp
            adsp.template/Addon/MessageSystem/Sockets/TSocketClassMethodCallback.hpp
            adsp.template/Addon/Utils/EnumIDsToString.hpp
            adsp.template/Addon/Process/AddonProcessManager.cpp
            adsp.template/AudioDSP/FactoryADSPModes/FactoryADSPModes.cpp
            adsp.template/AudioDSP/Streams/ADSPStreamManager.cpp
            adsp.template/AudioDSP/Streams/ADSPStream.cpp
            adsp.template/AudioDSP/Streams/ADSPStreamBuilderAll.cpp
            adsp.template/Settings/SettingsHelpers.cpp
            adsp.template/Settings/SettingsManager.cpp)

set(HEADERS adsp.template/ADSPHelpers.h
            adsp.template/doxygenDocu.h
            adsp.template/Addon/MessageSystem/Interfaces/IProtocol.hpp
            adsp.template/Addon/MessageSystem/Communication/ActorProtocol.h
            adsp.template/Addon/MessageSystem/Communication/Message.hpp
            adsp.template/Addon/MessageSystem/Interfaces/ISocket.hpp
            adsp.template/Addon/Process/AddonProcessManager.hpp
            adsp.template/Addon/Process/IAddonProcess.hpp
            adsp.template/AddonExceptions/IAddonException.h
            adsp.template/AddonExceptions/TAddonException.h
            adsp.template/configuration/templateConfiguration.h
            adsp.template/include/checkTemplateConfig.h
            adsp.template/include/client.h
            adsp.template/include/GUIDialogBase.h
            adsp.template/include/MACROHelper.h
            adsp.template/include/typedefs.h
            adsp.template/Settings/ISettingsElement.h
            adsp.template/Settings/SettingsHelpers.h
            adsp.template/Settings/SettingsManager.h
            adsp.template/Settings/TSettingsElement.h
            
            adsp.template/Addon/MVC/Interfaces/View/IView.hpp
            adsp.template/Addon/MVC/Interfaces/View/ActionIDs.hpp
            adsp.template/Addon/Utils/TCreationStatistics.hpp
            adsp.template/AudioDSP/FactoryADSPModes/FactoryADSPModes.hpp
            adsp.template/AudioDSP/FactoryADSPModes/IADSPMode.hpp
            adsp.template/AudioDSP/Streams/ADSPStreamManager.hpp
            adsp.template/AudioDSP/Streams/ADSPStream.hpp
            adsp.template/AudioDSP/Streams/IADSPStreamBuilder.hpp
            adsp.template/AudioDSP/Streams/ADSPStreamBuilderAll.hpp
            
            adsp.template/utils/stdStringUtils.h)
          
addon_source_group("${SOURCES}")
addon_source_group("${HEADERS}")

include_directories(${kodiplatform_INCLUDE_DIRS}
                    ${p8-platform_INCLUDE_DIRS}
                    ${KODI_INCLUDE_DIR})

add_library(${PROJECT_NAME} STATIC ${SOURCES} ${HEADERS})
add_library(adsp::${PROJECT_NAME} ALIAS ${PROJECT_NAME})
target_include_directories(${PROJECT_NAME}
                           PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}
                           PRIVATE ${CMAKE_CURRENT_SOURCE_DIR}/adsp.template
                           INTERFACE $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/adsp.template>
                                     $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}>)
                           
target_link_libraries(${PROJECT_NAME} kodi::KodiThreads)

# install all specific module files
#asplib_install_module(${PROJECT_NAME} "${HEADERS}" ${CMAKE_INSTALL_PREFIX}/include)
install(TARGETS ${PROJECT_NAME} EXPORT adsp DESTINATION ${CMAKE_INSTALL_PREFIX}/lib)
