#!/bin/bash -ex

echo "Checking travis variables"
echo "TRAVIS_PULL_REQUEST: ${TRAVIS_PULL_REQUEST}"
echo "TRAVIS_BRANCH: ${TRAVIS_BRANCH}"

if [ ${TRAVIS_PULL_REQUEST} == "true" ]; then
echo "This is a PR"
else
echo "This is not a PR"
fi

version="latest"
if [ $TRAVIS_BRANCH != "master" ] ; then
  version=$TRAVIS_BRANCH
fi
tag=$TRAVIS_REPO_SLUG:$version

docker login -u="${DOCKER_USERNAME}" -p="${DOCKER_PASSWORD}"
docker tag ${TRAVIS_REPO_SLUG} ${tag}
docker push $tag
