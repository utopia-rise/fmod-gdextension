class_name PerformancesDisplay extends Node


const CORE_CPU_DSP_CATEGORY = "FMOD [Core]/Cpu DSP"
const CORE_CPU_GEOMETRY_CATEGORY = "FMOD [Core]/Cpu Geometry"
const CORE_CPU_STREAM_CATEGORY = "FMOD [Core]/Cpu Stream"
const CORE_CPU_UPDATE_CATEGORY = "FMOD/[Core] Cpu Update"
const CORE_CPU_CONVOLUTION_THREAD1_CATEGORY = "FMOD/[Core] Cpu convolution Thread 1"
const CORE_CPU_CONVOLUTION_THREAD2_CATEGORY = "FMOD/[Core] Cpu convolution Thread 2"

const STUDIO_CPU_UPDATE_CATEGORY = "FMOD/[Studio] Cpu Update"

const MEMORY_CURRENTLY_ALLOCATED_CATEGORY = "FMOD/[Memory] Currently allocated"
const MEMORY_MAX_ALLOCATED_CATEGORY = "FMOD/[Memory] Max allocated"

const FILE_SAMPLE_CATEGORY = "FMOD/[File] Sample bytes read"
const FILE_STREAM_CATEGORY = "FMOD/[File] Stream bytes read"
const FILE_OTHER_CATEGORY = "FMOD/[File] Other bytes read"

func _enter_tree():
	var performance_data: FmodPerformanceData = FmodServer.get_performance_data()
	add_monitor(CORE_CPU_DSP_CATEGORY, func(): return performance_data.dsp)
	add_monitor(CORE_CPU_GEOMETRY_CATEGORY, func(): return performance_data.geometry)
	add_monitor(CORE_CPU_STREAM_CATEGORY, func(): return performance_data.stream)
	add_monitor(CORE_CPU_UPDATE_CATEGORY, func(): return performance_data.update)
	add_monitor(CORE_CPU_CONVOLUTION_THREAD1_CATEGORY, func(): return performance_data.convolution1)
	add_monitor(CORE_CPU_CONVOLUTION_THREAD2_CATEGORY, func(): return performance_data.convolution2)
	
	add_monitor(STUDIO_CPU_UPDATE_CATEGORY, func(): return performance_data.studio)
	
	add_monitor(MEMORY_CURRENTLY_ALLOCATED_CATEGORY, func(): return performance_data.currently_allocated)
	add_monitor(MEMORY_MAX_ALLOCATED_CATEGORY, func(): return performance_data.max_allocated)
	
	add_monitor(FILE_SAMPLE_CATEGORY, func(): return performance_data.sample_bytes_read)
	add_monitor(FILE_STREAM_CATEGORY, func(): return performance_data.stream_bytes_read)
	add_monitor(FILE_OTHER_CATEGORY, func(): return performance_data.other_bytes_read)

func _exit_tree() -> void:
	remove_monitor(CORE_CPU_DSP_CATEGORY)
	remove_monitor(CORE_CPU_GEOMETRY_CATEGORY)
	remove_monitor(CORE_CPU_STREAM_CATEGORY)
	remove_monitor(CORE_CPU_UPDATE_CATEGORY)
	remove_monitor(CORE_CPU_CONVOLUTION_THREAD1_CATEGORY)
	remove_monitor(CORE_CPU_CONVOLUTION_THREAD2_CATEGORY)
	
	remove_monitor(STUDIO_CPU_UPDATE_CATEGORY)
	
	remove_monitor(MEMORY_CURRENTLY_ALLOCATED_CATEGORY)
	remove_monitor(MEMORY_MAX_ALLOCATED_CATEGORY)
	
	remove_monitor(FILE_SAMPLE_CATEGORY)
	remove_monitor(FILE_STREAM_CATEGORY)
	remove_monitor(FILE_OTHER_CATEGORY)

func add_monitor(title: String, callable: Callable) -> void:
	if not Performance.has_custom_monitor(title):
		Performance.add_custom_monitor(title, callable)

func remove_monitor(title: String) -> void:
	if Performance.has_custom_monitor(title):
		Performance.remove_custom_monitor(title)
