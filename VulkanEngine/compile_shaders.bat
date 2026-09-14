@echo off
REM compile_shaders.bat - compiles GLSL -> SPIR-V and connects shaders to the engine
REM Usage: compile_shaders.bat [ProjectDir] [VulkanSDK]
setlocal

set "PROJECT_DIR=%~1"
if "%PROJECT_DIR%"=="" set "PROJECT_DIR=%~dp0"
if not "%PROJECT_DIR:~-1%"=="\" set "PROJECT_DIR=%PROJECT_DIR%\"

set "VK_SDK=%~2"
if "%VK_SDK%"=="" set "VK_SDK=%VULKAN_SDK%"

set "GLSLC=%VK_SDK%\Bin\glslc.exe"
if not exist "%GLSLC%" (
    echo [compile_shaders] WARNING: glslc not found at "%GLSLC%". Trying glslangValidator...
    set "GLSLC=%VK_SDK%\Bin\glslangValidator.exe"
    set "USE_GLSLANG=1"
) else (
    set "USE_GLSLANG=0"
)

if not exist "%GLSLC%" (
    echo [compile_shaders] ERROR: No shader compiler found. Install Vulkan SDK and set VULKAN_SDK.
    echo [compile_shaders] Tried: "%VK_SDK%\Bin\glslc.exe"
    exit /b 0
)

echo [compile_shaders] Using: %GLSLC%
echo [compile_shaders] ProjectDir: %PROJECT_DIR%

pushd "%PROJECT_DIR%shaders"
if errorlevel 1 (
    echo [compile_shaders] ERROR: shaders folder not found at "%PROJECT_DIR%shaders"
    popd
    exit /b 0
)

if "%USE_GLSLANG%"=="1" (
    echo [compile_shaders] Compiling triangle.vert with glslangValidator -V
    "%GLSLC%" -V triangle.vert -o triangle.vert.spv
    if errorlevel 1 echo [compile_shaders] FAILED triangle.vert
    "%GLSLC%" -V triangle.frag -o triangle.frag.spv
    if errorlevel 1 echo [compile_shaders] FAILED triangle.frag
) else (
    echo [compile_shaders] Compiling triangle.vert
    "%GLSLC%" triangle.vert -o triangle.vert.spv
    if errorlevel 1 echo [compile_shaders] FAILED triangle.vert
    echo [compile_shaders] Compiling triangle.frag
    "%GLSLC%" triangle.frag -o triangle.frag.spv
    if errorlevel 1 echo [compile_shaders] FAILED triangle.frag
)

if exist triangle.vert.spv echo [compile_shaders] OK triangle.vert.spv
if exist triangle.frag.spv echo [compile_shaders] OK triangle.frag.spv

popd
endlocal
