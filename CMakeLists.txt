# Generated on 2019-4-14 14:05:41 by version 0.7.1 of mulle-xcode-to-cmake
# Command line:
#    mulle-xcode-to-cmake -f -l c++ export Hoa.xcodeproj

project(Hoa)
cmake_minimum_required (VERSION 3.4)

set (CMAKE_CXX_STANDARD 11)

set (CMAKE_OSX_ARCHITECTURES "x86_64" "i386")

#
# Platform specific definitions
#

if( APPLE)
   # # CMAKE_OSX_SYSROOT must be set for CMAKE_OSX_DEPLOYMENT_TARGET (cmake bug)
   # if( NOT CMAKE_OSX_SYSROOT)
   #    set( CMAKE_OSX_SYSROOT "/" CACHE STRING "SDK for OSX" FORCE)   # means current OS X
   # endif()
   #
   # # baseline set to 10.6 for rpath
   # if( NOT CMAKE_OSX_DEPLOYMENT_TARGET)
   #   set(CMAKE_OSX_DEPLOYMENT_TARGET "10.6" CACHE STRING "Deployment target for OSX" FORCE)
   # endif()

   set( CMAKE_POSITION_INDEPENDENT_CODE FALSE)

   set( BEGIN_ALL_LOAD "-all_load")
   set( END_ALL_LOAD)
else()
   set( CMAKE_POSITION_INDEPENDENT_CODE TRUE)

   if( WIN32)
   # windows
   else()
   # linux / gcc
      set( BEGIN_ALL_LOAD "-Wl,--whole-archive")
      set( END_ALL_LOAD "-Wl,--no-whole-archive")
   endif()
endif()


##
## Hoa Files
##

# uncomment this for mulle-objc to search libraries first
# set( CMAKE_FIND_FRAMEWORK "LAST")

FILE(GLOB HOA_SOURCES_C ThirdParty/CicmWrapper/Sources/*.c)
FILE(GLOB HOA_WRAPPER_HEADERS ThirdParty/CicmWrapper/Sources/*.h)

#set ( HOA_SOURCES_C
#    ThirdParty/CicmWrapper/Sources/eattr.c
#    ThirdParty/CicmWrapper/Sources/ebox.c
#    ThirdParty/CicmWrapper/Sources/eclass.c
#    ThirdParty/CicmWrapper/Sources/ecommon.c
##    ThirdParty/CicmWrapper/Sources/egraphics.c
#    ThirdParty/CicmWrapper/Sources/eobj.c
#    ThirdParty/CicmWrapper/Sources/epopup.c

#    # replacement:
##    ThirdParty/Compatibility/pd-extended/ebox.c
#    )

# SET_SOURCE_FILES_PROPERTIES( ${HOA_SOURCES_C} PROPERTIES LANGUAGE CPP )

set( HOA_SOURCES
    ${HOA_SOURCES_C}
Sources/hoa.decoder_tilde.cpp
Sources/hoa.encoder_tilde.cpp
Sources/hoa.exchanger_tilde.cpp
Sources/hoa.io.cpp
Sources/hoa.map_gui.cpp
Sources/hoa.map_tilde.cpp
Sources/hoa.meter_gui_tilde.cpp
Sources/hoa.optim_tilde.cpp
Sources/hoa.process_tilde.cpp
Sources/hoa.projector_tilde.cpp
Sources/hoa.recomposer_tilde.cpp
Sources/hoa.rotate_tilde.cpp
Sources/hoa.scope_gui_tilde.cpp
Sources/hoa.space_gui.cpp
Sources/hoa.tools.cpp
Sources/hoa.wider_tilde.cpp
hoa.library.cpp
)

#set( HOA_SOURCES_EXTENDED
#Sources/hoa.decoder_tilde.cpp
#Sources/hoa.encoder_tilde.cpp
#Sources/hoa.exchanger_tilde.cpp
#Sources/hoa.io.cpp
#Sources/hoa.map_gui.cpp
#Sources/hoa.map_tilde.cpp
#Sources/hoa.meter_gui_tilde.cpp
#Sources/hoa.optim_tilde.cpp
#Sources/hoa.process_tilde.cpp
#Sources/hoa.projector_tilde.cpp
#Sources/hoa.recomposer_tilde.cpp
#Sources/hoa.rotate_tilde.cpp
#Sources/hoa.scope_gui_tilde.cpp
#Sources/hoa.space_gui.cpp
#Sources/hoa.tools.cpp
#Sources/hoa.wider_tilde.cpp
## replacement:
#ThirdParty/Compatibility/pd-extended/ebox.c
##ThirdParty/CicmWrapper/Sources/ebox.c
#ThirdParty/CicmWrapper/Sources/eclass.c
#ThirdParty/CicmWrapper/Sources/ecommon.c
#ThirdParty/CicmWrapper/Sources/egraphics.c
#ThirdParty/CicmWrapper/Sources/eobj.c
#ThirdParty/CicmWrapper/Sources/epopup.c
#hoa.library.cpp
#)

find_library( ACCELERATE_LIBRARY Accelerate)
message( STATUS "ACCELERATE_LIBRARY is ${ACCELERATE_LIBRARY}")

set( HOA_DEPENDENCIES
${ACCELERATE_LIBRARY}
)

set( HOA_INCLUDE_DIRS
.
ThirdParty/CicmWrapper/Sources
ThirdParty/HoaLibrary/Sources

ThirdParty/Compatibility/pd-extended
)

#
set( PD_INCLUDE_DIRS_VANILLA
    ThirdParty/PdHeaders/pd-vanilla
    )
set( PD_INCLUDE_DIRS_EXTENDED
    ThirdParty/PdHeaders/pd-extended
    )
set( PD_INCLUDE_DIRS_L2ORK
    ThirdParty/PdHeaders/pd-l2ork
    )
set( PD_INCLUDE_DIRS_CEAMMC
    ThirdParty/PdHeaders/pd-ceammc
    )

#
set( PD_VANILLA_HEADERS
    ThirdParty/PdHeaders/pd-vanilla/g_canvas.h
    ThirdParty/PdHeaders/pd-vanilla/m_imp.h
    ThirdParty/PdHeaders/pd-vanilla/m_pd.h
    ThirdParty/PdHeaders/pd-vanilla/s_stuff.h
)
set( PD_EXTENDED_HEADERS
    ThirdParty/PdHeaders/pd-extended/g_canvas.h
    ThirdParty/PdHeaders/pd-extended/m_imp.h
    ThirdParty/PdHeaders/pd-extended/m_pd.h
    ThirdParty/PdHeaders/pd-extended/s_stuff.h
)
set( PD_L2ORK_HEADERS
    # replacement from 'extended':
    ThirdParty/PdHeaders/pd-extended/g_canvas.h
    #    ThirdParty/PdHeaders/pd-l2ork/g_canvas.h
    ThirdParty/PdHeaders/pd-l2ork/m_imp.h
    ThirdParty/PdHeaders/pd-l2ork/m_pd.h
    ThirdParty/PdHeaders/pd-l2ork/s_stuff.h
)
set( PD_CEAMMC_HEADERS
    ThirdParty/PdHeaders/pd-ceammc/g_canvas.h
    ThirdParty/PdHeaders/pd-ceammc/m_imp.h
    ThirdParty/PdHeaders/pd-ceammc/m_pd.h
    ThirdParty/PdHeaders/pd-ceammc/s_stuff.h
)

#
set( HOA_PROJECT_HEADERS
    ${HOA_WRAPPER_HEADERS}
#ThirdParty/CicmWrapper/Sources/cicm_wrapper.h
#ThirdParty/CicmWrapper/Sources/ebox.h
#ThirdParty/CicmWrapper/Sources/eclass.h
#ThirdParty/CicmWrapper/Sources/ecommon.h
#ThirdParty/CicmWrapper/Sources/edefine.h
#ThirdParty/CicmWrapper/Sources/egraphics.h
#ThirdParty/CicmWrapper/Sources/eobj.h
#ThirdParty/CicmWrapper/Sources/epdmax.h
#ThirdParty/CicmWrapper/Sources/epopup.h
ThirdParty/HoaLibrary/Sources/Decoder.hpp
ThirdParty/HoaLibrary/Sources/Defs.hpp
ThirdParty/HoaLibrary/Sources/Encoder.hpp
ThirdParty/HoaLibrary/Sources/Exchanger.hpp
ThirdParty/HoaLibrary/Sources/Harmonics.hpp
ThirdParty/HoaLibrary/Sources/Hoa.hpp
ThirdParty/HoaLibrary/Sources/Hrir.hpp
ThirdParty/HoaLibrary/Sources/HrirIrc1002C2D.hpp
ThirdParty/HoaLibrary/Sources/HrirIrc1002C3D.hpp
ThirdParty/HoaLibrary/Sources/Math.hpp
ThirdParty/HoaLibrary/Sources/Meter.hpp
ThirdParty/HoaLibrary/Sources/Optim.hpp
ThirdParty/HoaLibrary/Sources/Planewaves.hpp
ThirdParty/HoaLibrary/Sources/Processor.hpp
ThirdParty/HoaLibrary/Sources/Projector.hpp
ThirdParty/HoaLibrary/Sources/Recomposer.hpp
ThirdParty/HoaLibrary/Sources/Rotate.hpp
ThirdParty/HoaLibrary/Sources/Scope.hpp
ThirdParty/HoaLibrary/Sources/Signal.hpp
ThirdParty/HoaLibrary/Sources/Source.hpp
ThirdParty/HoaLibrary/Sources/Tools.hpp
ThirdParty/HoaLibrary/Sources/Vector.hpp
ThirdParty/HoaLibrary/Sources/Voronoi.hpp
ThirdParty/HoaLibrary/Sources/Wider.hpp
hoa.library.hpp
)

##
## Hoa
##

add_library( Hoa SHARED
${HOA_SOURCES}
${HOA_PROJECT_HEADERS}
${PD_VANILLA_HEADERS}
)

target_include_directories( Hoa
   PUBLIC
${HOA_INCLUDE_DIRS}
${PD_INCLUDE_DIRS_VANILLA}
)

target_link_libraries( Hoa
${BEGIN_ALL_LOAD}
${HOA_STATIC_DEPENDENCIES}
${END_ALL_LOAD}
${HOA_DEPENDENCIES} "-undefined dynamic_lookup"
)

# TODO
set_target_properties(Hoa PROPERTIES PREFIX "")
if (WIN32)
      set_target_properties(Hoa PROPERTIES SUFFIX ".pd")
else()
      set_target_properties(Hoa PROPERTIES SUFFIX ".pd_darwin")
      set_target_properties(Hoa PROPERTIES BUNDLE_EXTENSION "pd_darwin")
endif()

install( TARGETS Hoa DESTINATION "lib")
install( FILES ${HOA_PUBLIC_HEADERS} DESTINATION "include/Hoa")


##
## Hoa_extended
##

add_library( Hoa_extended SHARED
${HOA_SOURCES}
${HOA_PROJECT_HEADERS}
${PD_EXTENDED_HEADERS}
)

target_include_directories( Hoa_extended
   PUBLIC
${HOA_INCLUDE_DIRS}
${PD_INCLUDE_DIRS_EXTENDED}
)

target_link_libraries( Hoa_extended
${BEGIN_ALL_LOAD}
${HOA_STATIC_DEPENDENCIES}
${END_ALL_LOAD}
${HOA_DEPENDENCIES} "-undefined dynamic_lookup"
)

# TODO
set_target_properties(Hoa_extended PROPERTIES PREFIX "")
if (WIN32)
      set_target_properties(Hoa_extended PROPERTIES SUFFIX ".pd")
else()
      set_target_properties(Hoa_extended PROPERTIES BUNDLE TRUE)
      set_target_properties(Hoa_extended PROPERTIES SUFFIX ".pd_darwin")
      set_target_properties(Hoa_extended PROPERTIES BUNDLE_EXTENSION "pd_darwin")
endif()

install( TARGETS Hoa_extended DESTINATION "lib")
install( FILES ${HOA_EXTENDED_PUBLIC_HEADERS} DESTINATION "include/Hoa_extended")

##
## Hoa_ceammc
##

add_library( Hoa_ceammc SHARED
${HOA_SOURCES}
${HOA_PROJECT_HEADERS}
${PD_CEAMMC_HEADERS}
)

target_include_directories( Hoa_ceammc
   PUBLIC
${HOA_INCLUDE_DIRS}
${PD_INCLUDE_DIRS_CEAMMC}
)

target_link_libraries( Hoa_ceammc
${BEGIN_ALL_LOAD}
${HOA_STATIC_DEPENDENCIES}
${END_ALL_LOAD}
${HOA_DEPENDENCIES} "-undefined dynamic_lookup"
)

# TODO
set_target_properties(Hoa_ceammc PROPERTIES PREFIX "")
if (WIN32)
      set_target_properties(Hoa_ceammc PROPERTIES SUFFIX ".pd")
else()
      set_target_properties(Hoa_ceammc PROPERTIES BUNDLE TRUE)
      set_target_properties(Hoa_ceammc PROPERTIES SUFFIX ".pd_darwin")
      set_target_properties(Hoa_ceammc PROPERTIES BUNDLE_EXTENSION "pd_darwin")
endif()

install( TARGETS Hoa_ceammc DESTINATION "lib")
install( FILES ${HOA_EXTENDED_PUBLIC_HEADERS} DESTINATION "include/Hoa_ceammc")

##
## Hoa_l2ork
##

add_library( Hoa_l2ork SHARED
${HOA_SOURCES}
${HOA_PROJECT_HEADERS}
${PD_L2ORK_HEADERS}
)

target_include_directories( Hoa_l2ork
   PUBLIC
${HOA_INCLUDE_DIRS}
${PD_INCLUDE_DIRS_L2ORK}
)

target_link_libraries( Hoa_l2ork
${BEGIN_ALL_LOAD}
${HOA_STATIC_DEPENDENCIES}
${END_ALL_LOAD}
${HOA_DEPENDENCIES} "-undefined dynamic_lookup"
)

# TODO
set_target_properties(Hoa_l2ork PROPERTIES PREFIX "")
if (WIN32)
      set_target_properties(Hoa_l2ork PROPERTIES SUFFIX ".pd")
else()
      set_target_properties(Hoa_l2ork PROPERTIES BUNDLE TRUE)
      set_target_properties(Hoa_l2ork PROPERTIES SUFFIX ".pd_darwin")
      set_target_properties(Hoa_l2ork PROPERTIES BUNDLE_EXTENSION "pd_darwin")
endif()

install( TARGETS Hoa_l2ork DESTINATION "lib")
install( FILES ${HOA_EXTENDED_PUBLIC_HEADERS} DESTINATION "include/Hoa_l2ork")
