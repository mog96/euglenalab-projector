# To get Node.js running
```
npm install
./run.sh
```

# To get openFrameworks running
```
sudo ./openFrameworks/scripts/linux/debian/install_dependencies.sh
sudo ./openFrameworks/scripts/linux/debian/install_codecs.sh
./openFrameworks/scripts/linux/download_libs.sh
cd openFrameworks/apps/devApps/euglenalab-projector
make -j2 -s
make run
```

(Using a Raspberry Pi 2 B, I followed method 1 [here](https://forum.openframeworks.cc/t/compiling-of-in-raspbian-stretch/27562/15).)
