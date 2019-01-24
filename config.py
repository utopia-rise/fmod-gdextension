def can_build(env, platform):
    return platform == "x11" or platform == "windows" or platform == "osx"


def configure(env):
    env.Append(LIBPATH=["#modules/fmod/api/lowlevel/lib/",
                        "#modules/fmod/api/studio/lib/"])
    if env["bits"] == "32":
        env.Append(LIBS=["fmod_vc", "fmodstudio_vc"])
    else:
        env.Append(LIBS=["fmod64_vc", "fmodstudio64_vc"])
