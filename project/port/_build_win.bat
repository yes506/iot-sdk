
echo %1
pushd %1
call build.bat
call deploy.bat
popd
