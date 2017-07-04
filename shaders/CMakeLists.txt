set(SHADER_DIR "${CMAKE_CURRENT_SOURCE_DIR}")
set(PROJECT_SHADER_LIST
        "${PROJECT_SHADER_LIST}"
        "${SHADER_DIR}/vs.glsl"
        "${SHADER_DIR}/fs.glsl"
        PARENT_SCOPE)
