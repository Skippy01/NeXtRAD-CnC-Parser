cmake_minimum_required(VERSION 1.0)
project( CnC-Parser )

find_package( Boost )
include_directories( ${Boost_INCLUDE_DIR} )

add_executable( CnC-Parser
                cnc.cpp 	
				tcpsocket/tcpstream.cpp
				tcpsocket/tcpconnector.cpp
				)


target_link_libraries( 	CnC-Parser
                        boost_regex 
						boost_system 
						boost_serialization 
						boost_thread 
						boost_filesystem
						)
