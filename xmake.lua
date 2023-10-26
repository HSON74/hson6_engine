add_rules("mode.debug", "mode.release")
add_requires("glfw")
add_requires("spdlog")
add_requires("sokol")
add_requires("wgpu-native", "glfw3webgpu")
add_requires("soloud")
add_requires("glm")
add_requires("stb")
add_requires("lua", "sol2")
add_requires("imgui", {configs = {glfw = true, wgpu = true}}) 


add_packages("glfw", {
    public = true
})
add_packages("spdlog", {
    public = true
})
add_packages("sokol", {
    public = true
})
add_packages("wgpu-native", "glfw3webgpu",{public = true})
add_packages("soloud", {public = true})
add_packages("glm", {public = true})
add_packages("stb", {public = true})
add_packages("imgui", {public = true})

set_policy("build.warning", true) -- show warnings
set_warnings("all") -- warn about many things

target("helloworld")
    set_kind("binary")
    set_languages("cxx20")
    set_rundir("$(projectdir)")
     -- Copy assets
    after_build(function (target)
        cprint("Copying assets")
        os.cp("$(projectdir)/assets", path.directory(target:targetfile()))
    end)
    add_deps("hson6_engine")

    add_files("demo/helloworld.cpp")

target("hson6_engine")
    add_packages("stb", {public = true})
    add_packages("glfw", {
        public = true
    })
    add_packages("spdlog", {
        public = true
    })
    add_packages("sokol", {
        public = true
    })
    add_packages("glm", {public = true})
    add_packages("wgpu-native", "glfw3webgpu",{public = true})
    add_packages("soloud", {public = true})
    add_packages("lua")
    add_packages("sol2", {public = true})
    add_packages("imgui", {public = true})
    set_kind("static")
    set_languages("cxx20")

    -- Declare our engine's header path.
    -- This allows targets that depend on the engine to #include them.
    add_includedirs("src", {
        public = true
    })
    

    -- Add all .cpp files in the `src` directory.
    add_files("src/*.cpp")
-- target("src/main")
-- set_kind("binary")
-- set_languages("cxx20")
--set_rundir("$(projectdir)")
-- Copy assets
    --after_build(function (target)
       -- cprint("Copying assets")
       -- os.cp("$(projectdir)/assets", path.directory(target:targetfile()))
    --end)

-- add_deps("illengine")

-- add_files("src/main.cpp")
