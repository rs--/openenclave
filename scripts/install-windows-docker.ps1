# Copyright (c) Open Enclave SDK contributors.
# Licensed under the MIT License.

# Must be ran on an elevated PowerShell session
# Requires a reboot after installation

Install-Module -Name DockerMsftProvider -Repository PSGallery -Force
Install-Package -Name docker -ProviderName DockerMsftProvider -Force
