cd web
npm run build
cd ..
rm -r data
mkdir data
cp -r web/build/* data/
cp config.json data/
pio run -t uploadfs