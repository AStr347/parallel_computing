#! func for create static lib
# \flag:LOG         - [optional]    if set func print other data 
# \param:LNAME      - [must]        static lib name
# \group:SRCS       - [must]        source of lib
# \group:INCLUDE    - [optional]    path to headers
# \group:PRIV_REQ   - [optional]    private libs req
# \group:PUB_REQ    - [optional]    public libs req
#
function(SLIB_INIT)
    cmake_parse_arguments(
        SLIB_INIT_PREFIX
        "LOG"
        "LNAME"
        "SRCS;INCLUDE;PRIV_REQ;PUB_REQ"
        ${ARGN}
    )
    # check LNAME and SRCS exist
    if(NOT DEFINED SLIB_INIT_PREFIX_LNAME)
        message(FATAL_ERROR "lib name must be defined")
    endif()
    if(NOT DEFINED SLIB_INIT_PREFIX_SRCS)
        message(FATAL_ERROR "lib sources must be defined")
    endif()

    if(SLIB_INIT_PREFIX_LOG)
        message("\n")
        message("LNAME:\t" ${SLIB_INIT_PREFIX_LNAME})
        message("SRCS:\t" ${SLIB_INIT_PREFIX_SRCS})
        message("INCLUDE:\t" ${SLIB_INIT_PREFIX_INCLUDE})
        message("PRIV_REQ:\t" ${SLIB_INIT_PREFIX_PRIV_REQ})
        message("PUB_REQ:\t" ${SLIB_INIT_PREFIX_PUB_REQ})
        message("\n")
    endif()

    add_library(${SLIB_INIT_PREFIX_LNAME} STATIC ${SLIB_INIT_PREFIX_SRCS})# Create static lib
    if(DEFINED SLIB_INIT_PREFIX_INCLUDE)
        # add public include dir
        target_include_directories(${SLIB_INIT_PREFIX_LNAME} 
            PUBLIC ${SLIB_INIT_PREFIX_INCLUDE}
        )
    endif()
    # link private and public libs
    target_link_libraries(${SLIB_INIT_PREFIX_LNAME} 
        PRIVATE ${SLIB_INIT_PREFIX_PRIV_REQ}
        PUBLIC ${SLIB_INIT_PREFIX_PUB_REQ}
    )
endfunction(SLIB_INIT)