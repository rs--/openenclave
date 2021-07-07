// Copyright (c) Open Enclave SDK contributors.
// Licensed under the MIT License.

OECI_LIB_VERSION = env.OECI_LIB_VERSION ?: "master"
oe = library("OpenEnclaveCommon@${OECI_LIB_VERSION}").jenkins.common.Openenclave.new()

AGENTS_LABEL = params.AGENTS_LABEL
TIMEOUT_MINUTES = params.TIMEOUT_MINUTES ?: 240

DOCKERHUB_REPO_CREDS = params.DOCKERHUB_REPO_CREDS ?: "oeciteamdockerhub"
LINUX_DOCKERFILE = ".jenkins/infrastructure/docker/dockerfiles/linux/Dockerfile.mystikos"

DOCKER_TAG = params.DOCKER_TAG ?: "mystikos"
MYSTIKOS_OS = params.MYSTIKOS_OS ?: "18.04"
MYSTIKOS_BRANCH = params.MYSTIKOS_BRANCH ?: "mystikos.v4"
MYSTIKOS_OE_PATH = params.MYSTIKOS_OE_PATH ?: "/work/install"

def buildMystikosDockerContainers() {
    node(AGENTS_LABEL) {
        timeout(TIMEOUT_MINUTES) {
            stage("Checkout") {
                cleanWs()
                checkout scm
            }
            String buildArgs = oe.dockerBuildArgs("ubuntu_version=" + MYSTIKOS_OS,
                                                  "mystikos_branch=" + MYSTIKOS_BRANCH,
                                                  "install_dir=" + MYSTIKOS_OE_PATH)
            stage("Build Ubuntu 18.04 Docker Image") {
                docker.withRegistry('https://registry.hub.docker.com', DOCKERHUB_REPO_CREDS) {
                    oe1804 = oe.dockerImage("oeciteam/oetools-18.04:${DOCKER_TAG}", LINUX_DOCKERFILE, "${buildArgs} --progress=plain")
                }
            }
            stage("Push to OE Docker Hub Registry") {
                docker.withRegistry('', DOCKERHUB_REPO_CREDS) {
                    oe.exec_with_retry { oe1804.push() }
                }
            }
        }
    }
}

buildMystikosDockerContainers()
