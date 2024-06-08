#!/bin/bash

ssh -o ServerAliveInterval=60 -fNR 2201:localhost:22 username@servername
