FROM mcr.microsoft.com/windows/servercore:ltsc2019

ARG repo="openenclave"
ARG branch="master"
ARG launchConfiguration="SGXFLC1"
ARG dcapClient="Azure"
ARG installPath="C:\oe_prereqs"

ENV repo=${repo}
ENV branch=${branch}
ENV launchConfiguration=${launchConfiguration}
ENV dcapClient=${dcapClient}
ENV installPath=${installPath}

RUN echo Running Windows Test\n\
repo: %repo%\n\
branch: %branch%\n\
launchConfiguration: %launchConfiguration%\n\
dcapClient: %dcapClient%\n\
installPath: %installPath%

#RUN git clone https://github.com/%repo%/openenclave.git --recursive && git checkout -b $branch origin/$branch
copy . .

SHELL ["powershell", "-Command", "$ErrorActionPreference = 'Stop'; $ProgressPreference = 'SilentlyContinue';"]
WORKDIR .
RUN .\scripts\install-windows-prereqs.ps1 -LaunchConfiguration $env:launchConfiguration -DCAPClientType $env:dcapClient -InstallPath $env:installPath

RUN git pull --recurse-submodules
RUN git submodule update --recursive
RUN ls ../
RUN echo PATH $env:Path

SHELL ["cmd", "/S", "/C"]
WORKDIR .
RUN mkdir -p build && cd build && del /S /Q .
RUN vcvars64.bat
RUN cmake.exe -G Ninja -DNUGET_PACKAGE_PATH=%installPath% -DCMAKE_INSTALL_PREFIX=C:/openenclave ..
RUN ninja.exe
RUN ctest.exe
