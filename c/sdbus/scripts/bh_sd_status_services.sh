#!/bin/bash
echo
echo '======================================= GEmu Status ========================================'
systemctl status bh_sd_gemu
echo '============================================================================================'
echo
echo '======================================= GHub Status ========================================'
systemctl status bh_sd_ghub
echo '============================================================================================'
echo
echo '=================================== GController Status ====================================='
systemctl status bh_sd_gcontroller
echo '============================================================================================'
echo
echo '======================================= AppA Status ========================================'
systemctl status bh_sd_appA
echo '============================================================================================'

echo
echo '===================================== Process Status ======================================='
ps -ef | grep --color=always -E "bh_sd_gcli|bh_sd_gemu|bh_sd_ghub|bh_sd_gcontroller|bh_sd_appA"
echo '============================================================================================'
