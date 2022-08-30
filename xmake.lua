add_rules("mode.debug", "mode.release")

target("fjson")
    set_kind("static")
    add_includedirs("src/magnum/fjson", {public = true})
    add_files("src/magnum/fjson/*.cpp")

target("test")
    set_kind("binary")
    add_files("src/test.cpp")
    add_deps("fjson")
