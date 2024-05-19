import org.jetbrains.kotlin.incremental.createDirectory

plugins {
    id("com.android.library")
    id("org.jetbrains.kotlin.android")
}

android {
    namespace = "com.utopiarise.godot.fmod.android.plugin"
    compileSdk = 33

    defaultConfig {
        minSdk = 24
        targetSdk = 33
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
        sourceCompatibility = JavaVersion.VERSION_1_8
        targetCompatibility = JavaVersion.VERSION_1_8
    }
    kotlinOptions {
        jvmTarget = "1.8"
    }
}

val copyAarFiles = tasks.create("copyAarFiles") {
    val aarOutputDir = project
        .buildDir
        .resolve("outputs/aar/")

    aarOutputDir.createDirectory()
    
    val aarFiles = aarOutputDir
        .listFiles()
        .filter { it.extension == "aar" }
    for (aarFile in aarFiles) {
        aarFile
            .copyTo(
                project.projectDir.resolve("../../demo/android/plugins/${aarFile.name}"),
                overwrite = true
            )
    }
}

tasks.build {
    finalizedBy(copyAarFiles)
}

dependencies {
    implementation("androidx.core:core-ktx:1.10.1")
    implementation(files("libraries/fmod.jar"))
    compileOnly(files("libraries/godot-lib.4.2.2.stable.template_release.aar"))
}