set(LAOWANGSBITWISETOOLBOX_SOURCE
    memprintf.cpp
    ppstr_parser.cpp
        )
set(LAOWANGSBITWISETOOLBOX_HEADER
    cvt.h
    memprintf.h
    ppstr_parser.h
    )

ADD_LIBRARY(LaowangsBitwiseToolBox 
           ${LAOWANGSBITWISETOOLBOX_SOURCE}
           ${LAOWANGSBITWISETOOLBOX_HEADER})

ADD_EXECUTABLE(lbtb main.cpp)
TARGET_LINK_LIBRARIES(lbtb LaowangsBitwiseToolBox)
