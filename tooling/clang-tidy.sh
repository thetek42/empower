#!/usr/bin/env sh

pushd empower
clang-tidy --use-color *.h
popd

echo "================================================================================"

pushd tests
clang-tidy --use-color *.c
popd
