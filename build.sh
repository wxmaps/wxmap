cd web
npm run build
cd ..
rm -r data
mkdir data
cp -r web/build/* data/
cp config.json data/
rm data/precache-manifest*.json
pio run -t uploadfs