#!/bin/bash
git config --global http.proxy socks5://localhost:8080
git config --global https.proxy socks5://localhost:8080
git config -l
