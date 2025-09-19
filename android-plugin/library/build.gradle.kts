import org.jetbrains.kotlin.incremental.createDirectory

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
    implementation("org.godotengine:godot:4.4.1.stable")
}