# @author S. V. Paulauskas
if (PAASS_USE_ROOT)
    add_subdirectory(Scope)
    add_subdirectory(RootScanner)
endif (PAASS_USE_ROOT)

add_subdirectory(EventReader)
add_subdirectory(HeadReader)
add_subdirectory(HexReader)
add_subdirectory(Skeleton)
add_subdirectory(TraceFilterer)
