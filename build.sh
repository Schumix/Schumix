#!/bin/sh
# Copyright (C) 2010 Megax <http://www.megaxx.info/>
# Auto Builder.

echo "\nSchumix Auto Builder by Megax.";
echo "Program segitsegevel konyeden fordithato a kod.\n";
chmod +x ./extract_svn_revision.sh && ./extract_svn_revision.sh
if(ls | grep bin); then
	echo "bin mappa mar letezik\n";
else
	mkdir bin
	echo "bin mappa letrehozva.\n";
fi
g++ -finput-charset=UTF-8 -std=c++0x -o schumix -pipe -g -Wfatal-errors -w src/*.cpp src/shared/Auth/*.cpp src/shared/Config/*.cpp src/shared/Database/*.cpp src/shared/Network/*.cpp -lcrypto -lcurl -lboost_regex -lmysqlclient -lboost_thread -lpthread
mv ./schumix ./bin
echo "Forditas befejezodot. Az allomany a bin mappaban talalhato!";
