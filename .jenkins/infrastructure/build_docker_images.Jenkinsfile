// Copyright (c) Open Enclave SDK contributors.
// Licensed under the MIT License.

GLOBAL_TIMEOUT_MINUTES = 240

OETOOLS_REPO = "https://oejenkinscidockerregistry.azurecr.io"
OETOOLS_REPO_CREDENTIAL_ID = "oejenkinscidockerregistry"
OETOOLS_DOCKERHUB_REPO_CREDENTIAL_ID = "oeciteamdockerhub"

parallel "Windows Stage": {
        stage("Build Windows Docker Containers") {
          build job: '/CI-CD_Infrastructure/Windows Docker Container Build',
          parameters: [string(name: 'REPOSITORY_NAME', value: env.REPOSITORY_NAME),
                       string(name: 'BRANCH_NAME', value: env.BRANCH_NAME),
                       string(name: 'INTERNAL_REPO', value: OETOOLS_REPO),
                       string(name: 'INTERNAL_REPO_CREDS', value: OETOOLS_REPO_CREDENTIAL_ID),
                       string(name: 'DOCKERHUB_REPO_CREDS', value: OETOOLS_DOCKERHUB_REPO_CREDENTIAL_ID),
                       string(name: 'DOCKER_TAG', value: env.DOCKER_TAG),
                       string(name: 'AGENTS_LABEL', value: env.WINDOWS_AGENTS_LABEL),
                       string(name: 'OECI_LIB_VERSION', value: OECI_LIB_VERSION),
                       booleanParam(name: 'TAG_LATEST',value: false)]
        }
    }, "Linux Stage": {
        stage("Build Linux Docker Containers") {
          build job: '/CI-CD_Infrastructure/Linux Docker Container Build',
          parameters: [string(name: 'REPOSITORY_NAME', value: env.REPOSITORY_NAME),
                       string(name: 'BRANCH_NAME', value: env.BRANCH_NAME),
                       string(name: 'INTERNAL_REPO', value: OETOOLS_REPO),
                       string(name: 'INTERNAL_REPO_CREDS', value: OETOOLS_REPO_CREDENTIAL_ID),
                       string(name: 'DOCKERHUB_REPO_CREDS', value: OETOOLS_DOCKERHUB_REPO_CREDENTIAL_ID),
                       string(name: 'DOCKER_TAG', value: env.DOCKER_TAG),
                       string(name: 'AGENTS_LABEL', value: env.AGENTS_LABEL),
                       string(name: 'OECI_LIB_VERSION', value: OECI_LIB_VERSION),
                       booleanParam(name: 'TAG_LATEST',value: false)]
        }
    }
