#!/bin/bash
echo "Incoming param:" + $1 + $2
echo "Rm bak folder"
rm -rf /home/admin1/AirCorebak
echo "backup core to bak folder"
mv /home/admin1/AirCore /home/admin1/AirCorebak
echo "Upgrade Aircore"
rm -rf /home/admin1/AirCore
cp -r $1/AirCore /home/admin1/
bash /home/admin1/AirCore/app/install/share/app_communication/params/fake_ota.sh
