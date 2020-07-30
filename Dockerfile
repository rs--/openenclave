from mcr.microsoft.com/windows/servercore:ltsc2019

copy . .

run dir

shell ["powershell", "-Command", "$ErrorActionPreference = 'Stop'; $ProgressPreference = 'SilentlyContinue';"]
workdir .
run .\scripts\install-windows-prereqs.ps1 -LaunchConfiguration SGX1FLC-NoDriver -DCAPClientType None -InstallPath C:\oe_prereqs

shell ["cmd", "/S", "/C"]
workdir .
run mkdir -p build && cd build && del /S /Q .
run vcvars64.bat && cmake -G Ninja -DNUGET_PACKAGE_PATH=C:/oe_prereqs -DCMAKE_INSTALL_PREFIX=C:/openenclave .. && ninja && ctest
