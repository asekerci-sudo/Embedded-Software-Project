@REM usage: ./compile CMD [e.g. ./compile bazel build test:all
@REM @echo off

@set localdir=%CD%
@set tmpdir=%CD%\tmpfolder

@echo "mounting local dir: %localdir%"
@echo "tmp dir: %tmpdir%"
@echo "%*"

@echo off
setlocal EnableDelayedExpansion
set "substring=test"
set "string=%*"
if "!string:%substring%=!"=="!string!" (
    set suffix=--platforms @AvrToolchain//platforms:ArduinoUno
) else ( set suffix=)

docker run -v %localdir%:/app -v %tmpdir%:/root/.cache/bazel uniduees/bazel-coverage:latest %* %suffix%

endlocal
