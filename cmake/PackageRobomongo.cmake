set(CPACK_GENERATOR TGZ)
set(CPACK_MONOLITHIC_INSTALL ON)
set(CPACK_PACKAGE_DIRECTORY ${CMAKE_BINARY_DIR}/package)
set(CPACK_STRIP_FILES ON)

set(CPACK_PACKAGE_NAME robomongo)
set(CPACK_PACKAGE_VERSION ${PROJECT_VERSION})
string(TOLOWER ${CMAKE_SYSTEM_NAME} CPACK_SYSTEM_NAME)
include(CPack)