
plugins {
    id("com.android.library")
    id("org.jetbrains.kotlin.android")
}

val pluginName = "fmod"
val pluginPackageName = "com.utopiarise.godot.fmod.android.plugin"

android {
    namespace = pluginPackageName
    compileSdk = 33

    defaultConfig {
        minSdk = 24
        targetSdk = 33

        manifestPlaceholders["godotPluginName"] = pluginName
        manifestPlaceholders["godotPluginPackageName"] = pluginPackageName
        buildConfigField("String", "GODOT_PLUGIN_NAME", "\"${pluginName}\"")
        setProperty("archivesBaseName", pluginName)
    }

    buildFeatures {
        buildConfig = true
    }

    buildTypes {
        release {
            isMinifyEnabled = false
            buildConfigField("boolean", "DEBUG", "false")
        }

        debug {
            buildConfigField("boolean", "DEBUG", "true")
        }
    }
    compileOptions {
        sourceCompatibility = JavaVersion.VERSION_17
        targetCompatibility = JavaVersion.VERSION_17
    }
    kotlinOptions {
        jvmTarget = "17"
    }
}

// The extension is an Android AAR plugin (android_aar_plugin in fmod.gdextension): Godot exports neither its
// .gdextension nor its libraries for Android, so each AAR carries the .gdextension and the native libraries of its
// variant, built by SCons into nativeLibrariesDir/<arch>/.
val nativeLibrariesDir = providers.gradleProperty("nativeLibrariesDir")
    .map(::file)
    .orElse(rootProject.file("../demo/addons/$pluginName/libs/android"))
val abiByArch = mapOf("arm64" to "arm64-v8a", "x86_64" to "x86_64")

fun registerJniLibs(variant: String, target: String, fmodSuffix: String) =
    tasks.register<Sync>("prepare${variant.replaceFirstChar(Char::uppercase)}JniLibs") {
        val libraries = listOf("libGodotFmod.android.$target.%s.so", "libfmod$fmodSuffix.so", "libfmodstudio$fmodSuffix.so")
        val requiredFiles = abiByArch.keys.flatMap { arch ->
            libraries.map { library -> nativeLibrariesDir.map { it.resolve("$arch/${library.format(arch)}") } }
        }
        inputs.files(requiredFiles)
        abiByArch.forEach { (arch, abi) ->
            from(nativeLibrariesDir.map { it.resolve(arch) }) {
                include(libraries.map { it.format(arch) })
                into(abi)
            }
        }
        into(layout.buildDirectory.dir("jniLibs/$variant"))
        doFirst {
            requiredFiles.forEach { file ->
                check(file.get().isFile) { "Missing Android native library: ${file.get()}" }
            }
        }
    }

val prepareDebugJniLibs = registerJniLibs("debug", "template_debug", "L")
val prepareReleaseJniLibs = registerJniLibs("release", "template_release", "")

val prepareGdextensionAsset by tasks.registering(Sync::class) {
    from(rootProject.file("../demo/addons/$pluginName/$pluginName.gdextension"))
    into(layout.buildDirectory.dir("gdextensionAssets/addons/$pluginName"))
}

android.sourceSets {
    getByName("main").assets.srcDir(layout.buildDirectory.dir("gdextensionAssets"))
    getByName("debug").jniLibs.srcDir(layout.buildDirectory.dir("jniLibs/debug"))
    getByName("release").jniLibs.srcDir(layout.buildDirectory.dir("jniLibs/release"))
}

tasks.configureEach {
    when {
        name.endsWith("Assets") -> dependsOn(prepareGdextensionAsset)
        name == "mergeDebugJniLibFolders" -> dependsOn(prepareDebugJniLibs)
        name == "mergeReleaseJniLibFolders" -> dependsOn(prepareReleaseJniLibs)
    }
}

val copyDebugAARToDemoAddons by tasks.registering(Copy::class) {
    description = "Copies the generated debug AAR binary to the plugin's addons directory"
    from("build/outputs/aar")
    include("$pluginName-debug.aar")
    into("../../demo/addons/$pluginName/libs/android")
}

val copyReleaseAARToDemoAddons by tasks.registering(Copy::class) {
    description = "Copies the generated release AAR binary to the plugin's addons directory"
    from("build/outputs/aar")
    include("$pluginName-release.aar")
    into("../../demo/addons/$pluginName/libs/android")
}


tasks.named("assemble").configure {
    finalizedBy(copyDebugAARToDemoAddons)
    finalizedBy(copyReleaseAARToDemoAddons)
}

dependencies {
    implementation(files("libraries/fmod.jar"))
    implementation("org.godotengine:godot:4.6.3.stable")
}