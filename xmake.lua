add_rules("mode.debug", "mode.release")

-- target("header")
--     set_kind("static")
--     add_includedirs("src/include", {public = true})
--     add_files("src/header.c")

-- target("helloworld")
--     set_kind("binary")
--     add_files("src/*.cpp")
--     add_deps("header")