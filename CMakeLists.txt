project( battleship )
cmake_minimum_required( VERSION 3.1 )
set( CMAKE_CXX_STANDARD 17 )
set( CMAKE_INCLUDE_CURRENT_DIR ON )
set( CMAKE_AUTOMOC ON )
#set( CMAKE_BUILD_TYPE Release )
set( CMAKE_BUILD_TYPE Debug )
add_definitions ( -Wall )

set ( battleship_HDRS
      FleetValidator.h
      PRNG.h
	)

set ( battleship_SRCS
	  BattleShip.cpp
      FleetValidator.cpp	
	)

add_executable ( battleship ${battleship_SRCS} ${UIS} ${RSCS} ${TRS} )
