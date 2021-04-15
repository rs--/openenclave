// Copyright (c) Open Enclave SDK contributors.
// Licensed under the MIT License.

OECI_LIB_VERSION = env.OECI_LIB_VERSION ?: "master"
oe = library("OpenEnclaveCommon@${OECI_LIB_VERSION}").jenkins.common.Openenclave.new()

AGENTS_LABEL = params.AGENTS_LABEL
TIMEOUT_MINUTES = params.TIMEOUT_MINUTES ?: 240

INTERNAL_REPO = params.INTERNAL_REPO ?: "https://oejenkinscidockerregistry.azurecr.io"
INTERNAL_REPO_CREDS = params.INTERNAL_REPO_CREDS ?: "oejenkinscidockerregistry"
DOCKERHUB_REPO_CREDS = params.DOCKERHUB_REPO_CREDS ?: "oeciteamdockerhub"

def buildLinuxDockerContainers() {
    node(AGENTS_LABEL) {
        timeout(TIMEOUT_MINUTES) {
            stage("Checkout") {
                cleanWs()
                checkout scm
            }
            String buildArgs = oe.dockerBuildArgs("UID=\$(id -u)", "UNAME=\$(id -un)",
                                                  "GID=\$(id -g)", "GNAME=\$(id -gn)")
            stage("Build Ubuntu 18.04 Full Docker Image") {
                oefull1804 = oe.dockerImage("oetools-full-18.04:${DOCKER_TAG}", ".jenkins/infrastructure/docker/dockerfiles/linux/Dockerfile.full", "${buildArgs} --build-arg ubuntu_version=18.04 --build-arg devkits_uri=${DEVKITS_URI}")
                puboefull1804 = oe.dockerImage("oeciteam/oetools-full-18.04:${DOCKER_TAG}", ".jenkins/infrastructure/docker/dockerfiles/linux/Dockerfile.full", "${buildArgs} --build-arg ubuntu_version=18.04 --build-arg devkits_uri=${DEVKITS_URI}")
            }
            stage("Build Ubuntu 18.04 Minimal Docker image") {
                oeminimal1804 = oe.dockerImage("oetools-minimal-18.04:${DOCKER_TAG}", ".jenkins/infrastructure/docker/dockerfiles/linux/Dockerfile.minimal", "${buildArgs} --build-arg ubuntu_version=18.04")
                puboeminimal1804 = oe.dockerImage("oeciteam/oetools-minimal-18.04:${DOCKER_TAG}", ".jenkins/infrastructure/docker/dockerfiles/linux/Dockerfile.minimal", "${buildArgs} --build-arg ubuntu_version=18.04")
            }
            stage("Build Ubuntu 20.04 Full Docker Image") {
                oefull2004 = oe.dockerImage("oetools-full-20.04:${DOCKER_TAG}", ".jenkins/infrastructure/docker/dockerfiles/linux/Dockerfile.full", "${buildArgs} --build-arg ubuntu_version=20.04 --build-arg devkits_uri=${DEVKITS_URI}")
                puboefull2004 = oe.dockerImage("oeciteam/oetools-full-20.04:${DOCKER_TAG}", ".jenkins/infrastructure/docker/dockerfiles/linux/Dockerfile.full", "${buildArgs} --build-arg ubuntu_version=20.04 --build-arg devkits_uri=${DEVKITS_URI}")
            }
            stage("Build Ubuntu Deploy Docker image") {
                oeDeploy = oe.dockerImage("oetools-deploy:${DOCKER_TAG}", ".jenkins/infrastructure/docker/dockerfiles/linux/Dockerfile.deploy", buildArgs)
                puboeDeploy = oe.dockerImage("oeciteam/oetools-deploy:${DOCKER_TAG}", ".jenkins/infrastructure/docker/dockerfiles/linux/Dockerfile.deploy", buildArgs)
            }
            stage("Push to OE Docker Registry") {
                docker.withRegistry(INTERNAL_REPO, INTERNAL_REPO_CREDS) {
                    oe.exec_with_retry { oefull1804.push() }
                    oe.exec_with_retry { oefull2004.push() }
                    oe.exec_with_retry { oeminimal1804.push() }
                    oe.exec_with_retry { oeDeploy.push() }
                    if(TAG_LATEST == "true") {
                        oe.exec_with_retry { oefull1804.push('latest') }
                        oe.exec_with_retry { oefull2004.push('latest') }
                        oe.exec_with_retry { oeminimal1804.push('latest') }
                        oe.exec_with_retry { oeDeploy.push('latest') }
                    }
                }
            }
            stage("Push to OE Docker Hub Registry") {
                docker.withRegistry('', DOCKERHUB_REPO_CREDS) {
                    if(TAG_LATEST == "true") {
                        oe.exec_with_retry { puboefull1804.push() }
                        oe.exec_with_retry { puboefull2004.push() }
                        oe.exec_with_retry { puboeminimal1804.push() }
                        oe.exec_with_retry { puboeDeploy.push() }
                        oe.exec_with_retry { puboefull1804.push('latest') }
                        oe.exec_with_retry { puboefull2004.push('latest') }
                        oe.exec_with_retry { puboeminimal1804.push('latest') }
                        oe.exec_with_retry { puboeDeploy.push('latest') }
                    }
                }
            }
        }
    }
}

buildLinuxDockerContainers()
