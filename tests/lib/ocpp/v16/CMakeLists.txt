target_sources(libocpp_unit_tests PRIVATE
        # test_database_migration_files.cpp
        test_smart_charging_handler.cpp
        test_composite_schedule.cpp
        )

file(COPY ${CMAKE_CURRENT_SOURCE_DIR}/json DESTINATION ${CMAKE_CURRENT_BINARY_DIR}/json)
