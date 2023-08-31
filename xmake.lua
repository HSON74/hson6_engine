add_rules("mode.debug", "mode.release")
add_requires("glfw")
add_requires("spdlog")
add_requires("sokol")

add_packages("glfw", {
    public = true
})
add_packages("spdlog", {
    public = true
})
add_packages("sokol", {
    public = true
})

set_policy("build.warning", true) -- show warnings
set_warnings("all") -- warn about many things

target("helloworld")
    set_kind("binary")
    set_languages("cxx17")

    add_deps("illengine")

    add_files("demo/helloworld.cpp")

target("illengine")
    add_packages("glfw", {
        public = true
    })

    set_kind("static")
    set_languages("cxx17")

    -- Declare our engine's header path.
    -- This allows targets that depend on the engine to #include them.
    add_includedirs("src", {
        public = true
    })
    add_packages("spdlog", {
        public = true
    })
    add_packages("sokol", {
        public = true
    })

-- Add all .cpp files in the `src` directory.
add_files("src/*.cpp")
-- target("src/main")
-- set_kind("binary")
-- set_languages("cxx17")

-- add_deps("illengine")

-- add_files("src/main.cpp")
